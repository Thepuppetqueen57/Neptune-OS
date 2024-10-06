#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int osmain(int *processes, int maxprocesses, int maxthreadsperprocess) {
    char cmd[100];
    printf("Welcome to Neptune OS! Type 'help' for a list of commands.\n");
    while (1) {
        printf("> ");
        scanf("%99s", cmd);

        for (int i = 0; cmd[i]; i++) {
            cmd[i] = tolower(cmd[i]);
        }

        if (strcmp(cmd, "shutdown") == 0 || strcmp(cmd, "exit") == 0) {
            printf("Shutting down...\n");
            exit(0);
        } else if (strcmp(cmd, "processes") == 0) {
            printf("Processes: %d\n", processes[0]);
        } else if (strcmp(cmd, "help") == 0 || strcmp(cmd, "commands") == 0) {
            printf("List of commands:\n");
            printf("1: shutdown (Shuts down Neptune OS)\n");
            printf("2: processes (Shows the number of processes)\n");
            printf("3: run (Runs a program)\n");
            printf("4: clear (Clears the console)\n");
        } else if (strcmp(cmd, "run") == 0) {
            printf("Would you like to run a built in program or a custom program? [B/C]: ");
            char runchoice;
            scanf(" %c", &runchoice);
            
            runchoice = tolower(runchoice);

            if (runchoice == 'b') {
                printf("Built in programs:\n");
                printf("1: Calculator\n");
                
                printf("What program would you like to run: ");
                int programchoice;
                scanf("%d", &programchoice);

                if (programchoice == 1) {
                    printf("Enter a math type [A/S/M/D]: ");
                    char mathchoice;
                    scanf(" %c", &mathchoice);
                    mathchoice = tolower(mathchoice);
                    if (mathchoice == 'a' || mathchoice == '+') {
                        printf("Enter the first number: ");
                        int a;
                        scanf("%d", &a);
                        printf("Enter the second number: ");
                        int b;
                        scanf("%d", &b);
                        printf("The answer is: %d\n", a + b);
                    } else if (mathchoice == 's' || mathchoice == '-') {
                        printf("Enter the first number: ");
                        int a;
                        scanf("%d", &a);
                        printf("Enter the second number: ");
                        int b;
                        scanf("%d", &b);
                        printf("The answer is: %d\n", a - b);
                    } else if (mathchoice == 'm' || mathchoice == '*') {
                        printf("Enter the first number: ");
                        int a;
                        scanf("%d", &a);
                        printf("Enter the second number: ");
                        int b;
                        scanf("%d", &b);
                        printf("The answer is: %d\n", a * b);
                    } else if (mathchoice == 'd' || mathchoice == '/') {
                        printf("Enter the first number: ");
                        int a;
                        scanf("%d", &a);
                        printf("Enter the second number: ");
                        int b;
                        scanf("%d", &b);
                        printf("The answer is: %d\n", a / b);
                    } else {
                        printf("Invalid choice. Please enter 'a' for addition, 's' for subtraction, 'm' for multiplication, or 'd' for division.\n");
                    }
                }
            } else if (runchoice == 'c') {
                printf("Custom programs will hopefully be added eventually!\n");
            } else {
                printf("Invalid choice. Please enter 'b' for built in or 'c' for custom.\n");
            }
        } else if (strcmp(cmd, "clear") == 0) {
            system("clear");
        }
        
        else {
            printf("Unknown command: %s\n", cmd);
        }
    }
}