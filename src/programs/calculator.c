#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "../../lib/seqft/evaluator.h"
#include "../../lib/seqft/tokenizer.h"
#include "../terminal.h"

void calculate(const char* expr) {
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

void calculator() {
    char expr[100];
    int loop = 1;
    while (loop == 1) {
        if (loop == 0) {
            break;
        }

        printf("Enter expression: ");
        scanf("%99s", expr);

        for (int i = 0; expr[i]; i++) {
            expr[i] = tolower(expr[i]);
        }

        if (strcmp(expr, "exit") == 0) {
            loop = 0;
        } else {
            calculate(expr);
        }
    }
}