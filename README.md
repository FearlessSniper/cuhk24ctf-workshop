# CUHK CTF 2024 Workshop: `pwn` section
## Contents
Under the `pwn` folder you can find 2 things:
1. `pwn-intro.md`: Markdown used for the presentation
2. `chall`: Files for the challenge given in the workshop
    - `deploy/bin/chall` / `src/bin/chall`: The challenge binary
    - `src/chall.c`: Source file for the challenge binary.

## Usage
### Presentation
Install [Marp](https://marp.app/) and render it as a presentation format you like.

### Challenge
Deployment:
1. `cd` into `pwn/chall/deploy`
2. Add a flag of your own to `flag.txt`
3. Run `docker compose up`.

For a live one with flag, try it here:
```
nc workshop.f21endsh1p.me 9999
```
*Limited availability. The challenge may be down without notice.*

## Solution
Read the writeup for the challenge [here](pwn/chall/writeup/writeup.md)
