#include "evaluator.h"


double sft_round(double nums[], size_t len) {
    if(len < 1)
        return 0;

    double num = nums[0];
    return round(num);
}

double sft_ceil(double nums[], size_t len) {
    if(len < 1)
        return 0;

    double num = nums[0];
    return ceil(num);
}

Function FN_LOOKUP[] = {
    (Function) {.ptr = sft_round, .name = "round"},
    (Function) {.ptr = sft_ceil,  .name = "ceil" }
};

#ifdef DEBUG
    #define debug_step(drawer, ...)   \
        Sft_draw(drawer);             \
        fprintf(stderr, __VA_ARGS__); \
        getchar();
#else
    #define debug_step(...)
#endif

void Sft_draw(SftDrawer* drawer) {
#ifdef DEBUG
    system("clear");

    if(!drawer || !drawer->tarray || !drawer->sft)
        return;
    // Draw expression
    // -----------------------------------------------------------------------

    for(int i = 0; i < drawer->tarray->count; ++i) {
        Token token = drawer->tarray->tokens[i];

        char* as_string = Token_toString(&token);

        fprintf(stderr, "%s ", as_string);
    }

    fprintf(stderr, "\n");

    for(int i = 0; i < drawer->tarray->count; ++i) {
        Token token = drawer->tarray->tokens[i];

        if(i == drawer->tarray_idx) {
            fprintf(stderr, "^");
            break;
        } else {
            char* as_string = Token_toString(&token);

            size_t astr_len = strlen(as_string);

            for(int i = 0; i < astr_len; ++i) {
                fprintf(stderr, " ");
            }

            fprintf(stderr, " ");
        }
    }

    fprintf(stderr, "\n");

    // Matrix
    // =======================================================================

    // Initialize
    // -------------------------------
    char* matrix[12][2];
    char* empty = " ";

    for(int i = 0; i < 12; ++i) {
        matrix[i][0] = empty;
        matrix[i][1] = empty;
    }

    // Populate
    // -------------------------------
    Stack* ostack = drawer->sft->operator_stack;
    Stack* nstack = drawer->sft->number_stack;

    for(int i = 0; i < Stack_getCount(ostack); ++i) {
        if(i >= 12) {
            fprintf(stderr,
                    "Broke while populating matrix, operator stack index %d "
                    "exceeded matrix size!\n",
                    i);
            sleep(2);
            break;
        }

        Token* token = Stack_itemAt(ostack, i);

        char* as_string = Token_toString(token);

        if(strlen(as_string) > 1) {
            as_string[1] = '\0';
        }

        matrix[i][0] = as_string;
    }

    for(int i = 0; i < Stack_getCount(nstack); ++i) {
        if(i >= 12) {
            fprintf(stderr,
                    "Broke while populating matrix, number stack index %d "
                    "exceeded matrix size!\n",
                    i);
            sleep(2);
            break;
        }

        double* number = Stack_itemAt(nstack, i);

        char* as_string = (char*)malloc(256);
        memset(as_string, 0, 256);

        snprintf(as_string, 256, "%.2f", *number);

        matrix[i][1] = as_string;
    }

    // Draw
    // -------------------------------
    for(int i = 11; i >= 0; i--) {
        char* str_operator = matrix[i][0];
        char* str_number   = matrix[i][1];
        fprintf(stderr, "[%s] | [%s]\n", str_operator, str_number);

        if(str_operator != empty) {
            matrix[i][0] = empty;
        }

        if(str_number != empty) {
            matrix[i][1] = empty;
        }
    }

    fprintf(stderr, "\n");
#endif
}



Sft* Sft_new() {
    Sft* sft = malloc(sizeof(Sft));

    sft->operator_stack = Stack_withCapacity(sizeof(Token), 100);
    sft->number_stack   = Stack_withCapacity(sizeof(double), 100);

    Stack_setDeallocator(sft->operator_stack,
                         (void (*)(void*))Token_freeMembers);

    return sft;
}






double eval_binary_op(TokenType operator_type, double num1, double num2) {
    if(operator_type == TT_ADD)
        return num1 + num2;

    else if(operator_type == TT_SUB)
        return num1 - num2;

    else if(operator_type == TT_DIV)
        return num1 / num2;

    else if(operator_type == TT_MUL)
        return num1 * num2;

    else if(operator_type == TT_MOD)
        return (uint64_t)num1 % (uint64_t)num2;

    else if(operator_type == TT_POW)
        return pow(num1, num2);
    else
        return 0xDEADC0DE;
}



double eval_unary_op(TokenType operator_type, double num) {
    if(operator_type & TT_NEG) {
        return -(num);
    }

    return 0xDEADC0DE;
}


SftError* eval_x_is_operator(Sft* sft, Token token) {
    Stack*     operator_cellar = sft->operator_stack;
    Stack*     number_cellar   = sft->number_stack;
    SftDrawer* drawer          = sft->drawer;

    while(1) {
        if(Stack_empty(operator_cellar))
            break;

        Token* top = Stack_getHead(operator_cellar);

        if(top->type & TT_OPA)
            break;

        if(top->type < token.type)
            break;

        Token* operator_token = Stack_pop(operator_cellar);
        DEBUGBLOCK({ Sft_draw(drawer); });

        double result_to_push = 0;

        // Eval binary operators.
        if(operator_token->type & TT_BOP) {
            double* num2 = Stack_pop(number_cellar);
            DEBUGBLOCK({ Sft_draw(drawer); });

            double* num1 = Stack_pop(number_cellar);
            DEBUGBLOCK({ Sft_draw(drawer); });

            if(!num2 || !num1) {
                sprintf(sft->error.message,
                        "Invalid expression, missing '%s' for binary operator "
                        "'%s'\n\n",
                        num1 ? "num1" : "num2",
                        Token_toString(operator_token));

                return &sft->error;
            }

            result_to_push = eval_binary_op(operator_token->type, *num1, *num2);
        }

        // Eval unary operators.
        else if(operator_token->type & TT_UOP) {
            double* num = Stack_pop(number_cellar);

            if(!num) {
                sprintf(sft->error.message,
                        "Invalid expression, missing '%s' for unary operator "
                        "'%s'\n\n",
                        "num",
                        Token_toString(operator_token));

                return &sft->error;
            }

            DEBUGBLOCK({ Sft_draw(drawer); });

            result_to_push = eval_unary_op(operator_token->type, *num);
        }

        // Add calculation result to number cellar.
        Stack_pushFrom(number_cellar, &result_to_push);
        DEBUGBLOCK({ Sft_draw(drawer); });
    }

    return 0;
}

SftError* eval_x_is_close_paren(Sft* sft, Token token) {

    Stack*     operator_cellar = sft->operator_stack;
    Stack*     number_cellar   = sft->number_stack;
    SftDrawer* drawer          = sft->drawer;

    while(1) {
        double result_to_push = 0;

        if(Stack_empty(operator_cellar))
            break;

        Token* top = Stack_getHead(operator_cellar);

        if(top->type == TT_OPA) {
            if(top->func) {

                int found_func = 0;

                for(int i = 0; i < 2; ++i) {
                    Function* f = &FN_LOOKUP[i];

                    if(!strcmp(f->name, top->func)) {
                        found_func = 1;

                        double* num = Stack_pop(number_cellar);

                        DEBUGBLOCK({ Sft_draw(drawer); });

                        if(!num) {
                            sprintf(sft->error.message,
                                    "Invalid expression, missing argument for"
                                    "function '%s'\n\n",
                                    Token_toString(top));

                            return &sft->error;
                        }

                        double nums[1] = {*num};
                        double result  = f->ptr(nums, 1);
                        Stack_pushFrom(number_cellar, &result);
                        DEBUGBLOCK({ Sft_draw(drawer); });
                        break;
                    }
                }

                if(!found_func) {
                    printf("\nNo such function '%s'\n", top->func);
                }
            }

            free(Stack_pop(operator_cellar));
            DEBUGBLOCK({ Sft_draw(drawer); });
            break;
        }

        Token* operator_token = Stack_pop(operator_cellar);
        DEBUGBLOCK({ Sft_draw(drawer); });

        // Eval binary operators.
        if(operator_token->type & TT_BOP) {
            double* num2 = Stack_pop(number_cellar);
            DEBUGBLOCK({ Sft_draw(drawer); });

            double* num1 = Stack_pop(number_cellar);
            DEBUGBLOCK({ Sft_draw(drawer); });

            if(!num2 || !num1) {
                sprintf(sft->error.message,
                        "Invalid expression, missing '%s' for binary operator "
                        "'%s'\n\n",
                        num1 ? "num1" : "num2",
                        Token_toString(operator_token));

                return &sft->error;
            }

            result_to_push = eval_binary_op(operator_token->type, *num1, *num2);
        }

        // Eval unary operators.
        else if(operator_token->type & TT_UOP) {
            double* num = Stack_pop(number_cellar);

            if(!num) {
                sprintf(sft->error.message,
                        "Invalid expression, missing '%s' for unary operator "
                        "'%s'\n\n",
                        "num",
                        Token_toString(operator_token));

                return &sft->error;
            }

            DEBUGBLOCK({ Sft_draw(drawer); });
            result_to_push = eval_unary_op(operator_token->type, *num);
        }

        // Add calculation result to number cellar.
        Stack_pushFrom(number_cellar, &result_to_push);
        DEBUGBLOCK({ Sft_draw(drawer); });
    }

    return 0;
}



SftError* Sft_evalTokens(Sft* sft, TokenArray* tokens, double* out_result) {
    SftDrawer* drawer = (SftDrawer*)malloc(sizeof(SftDrawer));
    drawer->sft       = sft;
    drawer->tarray    = tokens;
    drawer->padding   = 0;

    // Iterate from left to right.
    for(int i = 0; i < tokens->count; ++i) {
        drawer->tarray_idx = i;
        DEBUGBLOCK({ Sft_draw(drawer); });

        Token token = tokens->tokens[i];

        // If X is a number, place X in the number cellar.
        if(token.type & TT_NUM) {
            debug_step(drawer, "\n> Push Number\n");
            Stack_pushFrom(sft->number_stack, &token.f64);
        }

        // If token is an operator, evaluate operators until either
        // - Operator cellar is empty.
        //
        // - The top of the operator cellar is an open paren.
        //
        // - The precedence of the operator at the top of the operator
        //   cellar is LOWER than the precedence of t.
        else if(token.type & (TT_OPS | TT_COM)) {
            // The first time we encounter an operator, simply add it, no
            // evaluation.

            debug_step(drawer, "\n> Evaluate Stack\n");

            SftError* error = eval_x_is_operator(sft, token);

            if(error) {
                return error;
            }

            // Then place X in the cellar.
            debug_step(drawer, "\n> Push Operator\n");
            Stack_pushFrom(sft->operator_stack, &token);
        }

        // If X is an open parenthesis, push X onto the operator cellar.
        else if(token.type & TT_OPA) {
            debug_step(drawer, "\n> Push Operator\n");
            Stack_pushFrom(sft->operator_stack, &token);
        }

        // If X is a close parenthesis
        // - Evaluate operators until an open parenthesis is at the
        //   top of the operator cellar
        // - Remove the open parenthesis from the operator cellar.
        else if(token.type & TT_CPA) {
            debug_step(drawer, "\n> Evaluate Stack\n");
            SftError* error = eval_x_is_close_paren(sft, token);

            if(error) {
                return error;
            }
        }
    }

    debug_step(drawer, "\n> Evaluate Stack\n");

    // If there are no more tokens to read, evaluate the remaining operators.
    while(1) {
        if(Stack_empty(sft->operator_stack)) {
            break;
        }

        Token* operator_token = Stack_pop(sft->operator_stack);

        double result_to_push = 0;

        // Eval binary operators.
        if(operator_token->type & TT_BOP) {
            double* num2 = Stack_pop(sft->number_stack);
            double* num1 = Stack_pop(sft->number_stack);

            if(!num2 || !num1) {
                sprintf(sft->error.message,
                        "Invalid expression, missing '%s' for binary operator "
                        "'%s'\n\n",
                        num1 ? "num2" : "num1",
                        Token_toString(operator_token));

                return &sft->error;
            }

            result_to_push = eval_binary_op(operator_token->type, *num1, *num2);
        }

        // Eval unary operators.
        else if(operator_token->type & TT_UOP) {
            double* num = Stack_pop(sft->number_stack);

            if(!num) {
                sprintf(sft->error.message,
                        "Invalid expression, missing '%s' for unary operator "
                        "'%s'\n\n",
                        "num",
                        Token_toString(operator_token));

                return &sft->error;
            }

            result_to_push = eval_unary_op(operator_token->type, *num);
        }

        // Add calculation result to number cellar.
        Stack_pushFrom(sft->number_stack, &result_to_push);
    }

    debug_step(drawer, "\n> Pop Result\n");
    double* result = Stack_pop(sft->number_stack);
    DEBUGBLOCK({ Sft_draw(drawer); });

    if(result) {
        *out_result = *result;
        free(result);
    }

    return NULL;
}