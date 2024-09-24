#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 8

void win() {
    system("cat /flag");
}

const char *help = "Commands:\ninput: Change to input mode to enter contents.\nwin: Display the address of the win function.\nmain: Display the address of the main function.\ndump <n>: Dump <n> bytes of the memory after the buffer.\nquit: Leave the program.\nhelp: Print this help message.";

void dump(void *mem, int size) {
    int rem_bytes = size;
    char *c_mem = mem;
    while (rem_bytes > 0) {
        int line_length = ((rem_bytes > MAX_LINE_LENGTH)? MAX_LINE_LENGTH : rem_bytes);
        // Print hex dump
        printf("%02hhx", c_mem[0]);
        for (int i = 1; i < line_length; i++) {
            printf(" %02hhx", c_mem[i]);
        }
        printf("\t");
        // Print as chars
        for (int i = 0; i < line_length; i++) {
            char c = c_mem[i];
            switch (c) {
                case '\t':
                case '\n':
                putchar('.');
                break;
                default:
                if (isprint(c)) {
                    putchar(c);
                } else {
                    putchar(' ');
                }
                break;
            }
        }
        printf("\n");
        c_mem += line_length;
        rem_bytes -= line_length;
    }
}

int main(int argc, char const *argv[]);

void getwin() {
    printf("win: %p", win);
}

void getmain() {
    printf("main: %p", main);
}

void play() {
    puts("Hello there!");
    puts("What is your order for today? (Type 'help' for available commands)");
    printf("> ");
    char command[32];
    fgets(command, 32, stdin);
    command[strcspn(command, "\n")] = '\0';
    char content[16];
    while (1) {
        if (strcmp(command, "input") == 0) {
            printf("Enter your stuff: ");
            // scanf("%s", content);
            gets(content);
        } else if (strcmp(command, "win") == 0) {
            getwin();
        } else if (strcmp(command, "main") == 0) {
            getmain();
        } else if (strncmp(command, "dump", 4) == 0) {
            int dump_size;
            int args_read = sscanf(command, "dump %d", &dump_size);
            if (args_read > 0 && dump_size > 0) {
                dump(content, dump_size);
            } else {
                puts("Enter a valid number!");
            }
        } else if (strcmp(command, "quit") == 0) {
            break;
        } else if (strcmp(command, "help") == 0) {
            puts(help);
        } else {
            puts("Enter a valid command!");
        }
        printf("\n> ");
        fgets(command, 32, stdin);
        command[strcspn(command, "\n")] = '\0';
    }
}

int main(int argc, char const *argv[])
{
    setvbuf(stdin, NULL, _IONBF, 0);
    setvbuf(stdout, NULL, _IONBF, 0);
    play();
    return 0;
}
