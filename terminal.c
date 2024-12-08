#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Stuff for the calculator 
// Thanks to PsychedelicShayna for making seqft-c

#include "lib/seqft/common.h"
#include "lib/seqft/evaluator.h"
#include "lib/seqft/stack.h"
#include "lib/seqft/tokenizer.h"

void highlight_error(const char* expr,
                     size_t      expr_len,
                     IterErr     error,
                     size_t      indent) {

    printf("\n");
char stripped[expr_len];
    filter_whitespace(expr, expr_len, stripped);

    const size_t bufsize = indent + expr_len + strlen(error.message) + 1;
    char         padding[bufsize];
    memset(padding, 0, bufsize);

    memset(padding, ' ', indent);
    printf(">%s%s\n", padding, stripped);
    memset(padding, 0, bufsize);

    memset(padding, '~', indent + error.index);
    printf("%s ^\n%s\n", padding, error.message);
    memset(padding, 0, bufsize);

    memset(padding, '~', strlen(error.message));
    printf("%s\n", padding);
    memset(padding, 0, bufsize);

    printf("\n\n");
}

void calculator(const char* expr) {
    Tokenizer* t   = Tokenizer_new();
    Sft*       sft = Sft_new();

    size_t expr_len = strlen(expr);

    if(!expr_len) {
        Tokenizer_free(t);
        return;
    }

    TokenArray* token_array = Tokenizer_parse(t, expr, expr_len);

    // char buffer[256];
    //
    // for(int i = 0; i < token_array->count; ++i) {
    //     Token  t   = token_array->tokens[i];
    //     size_t len = Token_toString(&t, buffer);
    //     printf("Token '%s'\n", buffer);
    // }
    //
    // return;

#ifdef DEBUG
    if(token_array) {

        for(int i = 0; i < token_array->count; ++i) {
            Token* t = &token_array->tokens[i];
            Token_print(t);
            if(t->func) {
                printf("Has func: %s\n", t->func);
            }
        }

        // TokenArray_freeMembers(token_array);
        // free(token_array);
    }
#endif

    if(t->error) {
        highlight_error(expr, expr_len, *t->error, 2);
        TokenArray_free(token_array);
        Tokenizer_free(t);
        return;
    }

    if(token_array) {
        double result = 0;

        SftError* error =Sft_evalTokens(sft, token_array, &result);

        if(error) {
          printf("%s", error->message);
        } else {
          printf("Result: %f\n", result);
        }

    }

    TokenArray_free(token_array);
    Tokenizer_free(t);
}

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
            printf("5: credits (List of people who helped with Neptune OS)\n");
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
                    printf("Enter expression: ");
                    char expr[100];
                    scanf("%99s", expr);
            
                    for (int i = 0; expr[i]; i++) {
                        expr[i] = tolower(expr[i]);
                    }           

                    calculator(expr);
                }
            } else if (runchoice == 'c') {
                printf("Custom programs will hopefully be added eventually!\n");
            } else {
                printf("Invalid choice. Please enter 'b' for built in or 'c' for custom.\n");
            }
        } else if (strcmp(cmd, "clear") == 0) {
            system("clear");
        } else if (strcmp(cmd, "credits") == 0) {
            printf("Heres a list of people who helped with Neptune OS!\n");
            printf("All of these usernames are github usernames.\n");

            printf("\n");

            printf("Thepuppetqueen57: Made Neptune OS\n");
            printf("No one has helped yet this command is just for if someone does\n");
        }
        
        else {
            printf("Unknown command: %s\n", cmd);
        }
    }
}