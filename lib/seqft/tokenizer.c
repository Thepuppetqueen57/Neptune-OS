#include "tokenizer.h"
#include "common.h"
#include "stack.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*  Rules:
 *
 *  These operators are allowed and treated as single tokens: + - / % * ^
 *
 *  There can also exist:
 *    - Decimal Numbers:
 *      - No letter or leading zeros.
 *      - Can have a decimal point.
 *      - Can be prefixed with ~ in order to negate the number.
 *
 *    - Other Base Numbers:
 *      - The first character of the character stack must be 0.
 *      - The second character must be one of: x (hex), b (bin), o (oct).
 *      - The rest of the characters must be valid for the base
 *        - Hex: 0-9 a-f
 *        - Bin: 0-1
 *        - Oct: 0-7
 *      - The base character is copied into a char that is normally '\0',
 *         to indicate that the base has been found, so any letters after the
 *         base are treated as part of the number if allowed for that base.
 *         In this case, only hex numbers can have letters. Digits following
 *         the base must also be valid for the base.
 *
 *                 Logic
 *         -------------------------------------------------------------
 *
 *     - If letters are encountered, and the chars stack is empty, they are
 *       accumulated into the chars stack with no restrictions.
 *
 *     - If letters are encountered, and the chars stack is not empty, then
 *       the following checks are made:
 *
 *       - Is the length of the chars stack 1, and the first character a 0?
 *         - If yes: Is the second character a valid base character?
 *         - If yes: Accumulate the letter, and trip the boooolean flag.
 *
 *       - Has the BOOLean BOOLean flag been tripped? If so, then is the letter
 *         permissible for the base?
 *         - If yes: Accumulate the letter.
 *         - If no:  Clear the chars stack, and add an error to the error stack.
 *
 *       - If not, then does the char stack lead with a digit? If so, then
 *       clear the chars stack, and add an error to the error stack.
 *
 *     - If digits are encountered, and the chars stack is empty, then they are
 *       accumulated into the chars stack.
 *
 *     - If digits are encountered, and the chars stack is not empty, then the
 *       following checks are made:
 *
 *       - Has a base been determined? If so, then is the digit valid for the
 *        base?
 *         - If yes: Accumulate the digit.
 *         - If no:  Clear the chars stack, and add an error to the error stack.
 *
 *     - If non-digit, non-letter characters are encountered:
 *       - If it's an open parenthesis, and the char steck is not empty, and
 *         the base has not been determined, then add an open parenthesis token
 *         but treat the charstack as a function name, and copy it to the
 *         fn (function name) field.
 *
 *       - If it's anything else, then:
 *         - If the charstack is not empty, then parse the char stack as either
 *           a regular decimal number, or a number with a base if the base was
 *           determined. If it's not a number, then clear the char stack and
 *           add an error to the error stack.
 *
 *         - Always: add the token for the operator, if it's a valid operator.
 * */

char* Token_toString(Token* token) {
    memset(token->str, 0, sizeof(token->str));

    if(token->type == TT_OPA && token->func) {
        memcpy(token->str, token->func, strlen(token->func));
        token->str[strlen(token->func)] = '(';
    }

    else if(token->type == TT_NUM) {
        snprintf(token->str, sizeof(token->str), "%.2f", token->f64);
    }

    else if(token->type == TT_ADD) {
        token->str[0] = '+';
    }

    else if(token->type == TT_COM) {
        token->str[0] = ',';
    }

    else if(token->type == TT_SUB) {
        token->str[0] = '-';
    }

    else if(token->type == TT_DIV) {
        token->str[0] = '/';
    }

    else if(token->type == TT_MOD) {
        token->str[0] = '%';
    }

    else if(token->type == TT_MUL) {
        token->str[0] = '*';
    }

    else if(token->type == TT_POW) {
        token->str[0] = '^';
    }

    else if(token->type == TT_NEG) {
        token->str[0] = '~';
    }

    else if(token->type == TT_OPA) {
        token->str[0] = '(';
    }

    else if(token->type == TT_CPA) {
        token->str[0] = ')';
    }

    else {
        token->str[0] = '?';
    }

    return token->str;
}

void Token_print(Token* t) {
    char* b = TokenType_toString(t->type);

    printf("Token: {\n    type: %s,\n    f64: %f,\n    func: %s\n}\n",
           b,
           t->f64,
           t->func ? t->func : "null");

    free(b);
}

void Token_freeMembers(Token* t) {
    if(t && t->func) {
        free(t->func);
        t->func = 0;
    }
}

void TokenArray_freeMembers(TokenArray* t) {
    if(t && t->tokens) {
        for(int i = 0; i < t->count; ++i) {
            Token_freeMembers(&t->tokens[i]);
        }

        free(t->tokens);
    }
}

void TokenArray_free(TokenArray* t) {
    if(t) {
        TokenArray_freeMembers(t);
        free(t);
    }
}

BOOL valid_for_base(char c, AccFlag base) {
    switch(base) {
        case ACC_HEX:
            return isxdigit(c);
        case ACC_DEC:
            return isdigit(c);
        case ACC_OCT:
            return c >= '0' && c <= '7';
        case ACC_BIN:
            return c == '0' || c == '1';
        default:
            return isdigit(c);
    }
}

char* TokenType_toString(TokenType ttype) {
    char* buffer = csrxmalloc(100);
    memset(buffer, 0, 100);

    switch(ttype) {
        case TT_NUM:
            sprintf(buffer, "Number");
            break;
        case TT_ADD:
            sprintf(buffer, "Operator [ + ]");
            break;
        case TT_SUB:
            sprintf(buffer, "Operator [ - ]");
            break;
        case TT_DIV:
            sprintf(buffer, "Operator [ / ]");
            break;
        case TT_MOD:
            sprintf(buffer, "Operator [ %% ]");
            break;
        case TT_MUL:
            sprintf(buffer, "Operator [ * ]");
            break;
        case TT_POW:
            sprintf(buffer, "Operator [ ^ ]");
            break;
        case TT_NEG:
            sprintf(buffer, "Operator [ ~ ]");
            break;
        case TT_OPA:
            sprintf(buffer, "Operator [ ( ]");
            break;
        case TT_CPA:
            sprintf(buffer, "Operator [ ) ]");
            break;
        default:
            sprintf(buffer, "Unknown Token Type: %b", ttype);
            break;
    }

    return buffer;
}

Tokenizer* Tokenizer_new() {
    Tokenizer* t = xmalloc(sizeof(Tokenizer));
    memset(t, 0, sizeof(Tokenizer));

    t->tokens = Stack_withCapacity(sizeof(Token), 100);
    t->stacc  = Stack_withCapacity(sizeof(char), 100);
    Stack_setDeallocator(t->tokens, (void (*)(void*)) & Token_freeMembers);
    Stack_setDefaultAlloc(t->tokens, 100);
    Stack_setDefaultAlloc(t->stacc, 100);

    t->accfl = ACC_NIL;
    t->error = 0;

    // Define char -> TokenType resolution.
    memset(t->tt_map, 0, sizeof(TokenType) * 256);
    t->tt_map['+'] = TT_ADD;
    t->tt_map['-'] = TT_SUB;
    t->tt_map['/'] = TT_DIV;
    t->tt_map['%'] = TT_MOD;
    t->tt_map['*'] = TT_MUL;
    t->tt_map['^'] = TT_POW;
    t->tt_map['~'] = TT_NEG;
    t->tt_map['('] = TT_OPA;
    t->tt_map[','] = TT_OPA;
    t->tt_map[')'] = TT_CPA;

    return t;
}

void Tokenizer_free(Tokenizer* t) {
    if(t) {
        Stack_free(t->tokens);
        Stack_free(t->stacc);
        if(t->error) {
            free(t->error);
            t->error = 0;
        }
        free(t);
    }
}

void Tokenizer_addToken(Tokenizer* t, Token* token) {
    Stack_pushFrom(t->tokens, token);
    Stack_clear(t->stacc);

#ifdef DEBUG
    {
        char* token_str = TokenType_toString(token->type);

        if(token_str) {
            printf("Added token to stack %s\n", token_str);
            free(token_str);
        }
    }
#endif

    t->accfl = ACC_NIL;
}

void Tokenizer_clear(Tokenizer* t) {
    Stack_reClear(t->tokens);
    Stack_reClear(t->stacc);

    t->accfl = ACC_NIL;

    if(t->error) {
        free(t->error);
        t->error = 0;
    }
}

void Tokenizer_error(Tokenizer* t, const char* message, size_t expr_index) {
    IterErr error = {.message = message, .index = expr_index};

    if(t->error) {
        free(t->error);
        t->error = 0;
    }

    t->error    = xmalloc(sizeof(IterErr));
    *(t->error) = error;
}

BOOL Tokenizer_parseAccNum(Tokenizer* t) {
    Token token = {.type = TT_NUM, .f64 = 0, .func = 0};

    char*  base_ptr = Stack_getBase(t->stacc);
    size_t count    = Stack_getCount(t->stacc);

    if(!count) {
        Tokenizer_error(t, "Not enough digits to construct a number.", count);
        return TRUE;
    }

    char buffer[count + 1]; // Nullbyte
    memset(buffer, 0, count + 1);

    char*  begin = base_ptr;
    size_t len   = count;

    // Futureproof against me adding any control bits in the future by
    // filtering out anything that isn't ACC_HEX, ACC_BIN, or ACC_OCT.
    int custom_base =
        (t->accfl & (~(t->accfl & ~(ACC_HEX | ACC_OCT | ACC_BIN))));

    BOOL is_float = t->accfl & ACC_FPN;

    int strtoll_base = custom_base ? custom_base : 10;

    if(custom_base || is_float) {
        if(count < 3) {
            Tokenizer_error(t, "Incomplete number.", count);

            return TRUE;
        }

        // Exclude base specifier (0x, 0b, 0o, etc)
        if(custom_base) {
            begin += 2;
            len -= 2;
        }
    }

    memcpy(buffer, begin, len);

    if(is_float) {
        token.f64 = atof(buffer);
    } else {
        token.f64 = strtoll(buffer, 0, strtoll_base);
    }

    Tokenizer_addToken(t, &token);
    return 0;
}

TokenArray* Tokenizer_parse(Tokenizer* t, const char* cexpr, size_t expr_len) {
    Tokenizer_clear(t);

    char expr[expr_len + 1];
    expr_len = filter_whitespace(cexpr, expr_len, expr);

    if(!expr_len) {
        perror("Could not filter whitespace from expression!\n");
        abort();
    }

    printdbg("Stripped Expression: '%s'(%zu). Originally '%s'(%zu)\n",
             expr,
             expr_len,
             cexpr,
             strlen(cexpr));

    for(int i = 0; i < expr_len; ++i) {
        char c = expr[i];

        TokenType op = t->tt_map[c];

        // It's an operator. Parse the accumulator, and add the operator token.
        // ------------------------------------------------------------------------
        if(op & TT_OPA && t->accfl & ACC_FUN) {
            Token token = {.type = TT_OPA, .f64 = 0, .func = 0};
            token.func  = (char*)xmalloc(Stack_getCount(t->stacc) + 1);
            memset(token.func, 0, Stack_getCount(t->stacc) + 1);
            memcpy(
                token.func, Stack_getBase(t->stacc), Stack_getCount(t->stacc));

            Tokenizer_addToken(t, &token);
        } else if(op & (TT_OPS | TT_PAS) && t->accfl & ACC_NUM) {
            // Returns non-zero on error.
            if(Tokenizer_parseAccNum(t)) {
                return 0;
            }

            Tokenizer_addToken(t, &(Token) {.type = op, .f64 = 0, .func = 0});
        } else if(op & (TT_OPS | TT_PAS)) {
            Token token = {.type = op, .f64 = 0, .func = 0};
            Tokenizer_addToken(t, &token);
        }

        // If it's a letter and accflg is NIL, start accumulating a function.
        // --------------------------------------------------------------------
        else if(isalpha(c) && t->accfl == ACC_NIL) {
            Stack_pushFrom(t->stacc, &c);
            t->accfl = ACC_FUN;
        }

        // If it's a digit and accflg is NIL, start accumulating a number.
        // --------------------------------------------------------------------
        else if(isdigit(c) && t->accfl == ACC_NIL) {
            // If the first digit of an accumulation is 0, then there are some
            // rules that must be followed. A single zero is okay, but if a
            // next character exists and also a digit, then this 0 would be
            // a leading 0, which is not allowed in decimal numbers, but
            // if the next character is a valid base character, or a decimal
            // point, then the accfl is changed, and any future 0's are ok.
            t->accfl = ACC_DEC | (c == '0' ? ACC_DTZ : 0);
            Stack_pushFrom(t->stacc, &c);
        }

        // If we're accumulating a function name, simply push the character.
        // --------------------------------------------------------------------
        else if(t->accfl & ACC_FUN) {
            Stack_pushFrom(t->stacc, &c);
        }

        // If we're accumulating a number, push the character, but..
        // --------------------------------------------------------------------
        else if(t->accfl & ACC_NUM) {
            // If this is the second character, check for a format specifier.
            if(Stack_getCount(t->stacc) == 1) {
                switch(c) {
                    case 'x':
                        t->accfl = ACC_HEX;
                        break;
                    case 'b':
                        t->accfl = ACC_BIN;
                        break;
                    case 'o':
                        t->accfl = ACC_OCT;
                        break;
                    case '.':
                        t->accfl = ACC_FPN;
                        break;
                    default:
                        break;
                }
            } else if((t->accfl & ACC_DEC) && c == '.') {
                t->accfl = ACC_FPN;
            }

            // Otherwise, check if the character is valid for the number type.
            else if(!valid_for_base(c, t->accfl)) {
                Tokenizer_error(t, "Invalid character in number.", i);
                return 0;
            }

            // If the trailing zero flag has been set, and not removed by the
            // conversion to another number type (hex, bin, oct, float), then
            // this must be decimal, in which case a leading zero is illegal.
            if(t->accfl & ACC_DTZ) {
                Tokenizer_error(
                    t, "Leading zero in number is illegal in this context.", 0);
                return 0;
            }

            Stack_pushFrom(t->stacc, &c);
        }

        // If it's not an operator, letter, digit, the expression is invalid.
        // --------------------------------------------------------------------
        else {
            Tokenizer_error(t, "Invalid expression.", i);
            return 0;
        }
    }

    // If there's any remaining elements in the accumulator, and
    // a function is being accumulated, then that's an error, since an open
    // paren is needed to complete it.
    //
    // A number is fine though.

    if(!Stack_empty(t->stacc)) {
        if(t->accfl & ACC_FUN) {
            Tokenizer_error(
                t, "Function with no opening parenthesis.", expr_len);
            return 0;
        } else if(t->accfl & ACC_NUM) {
            Tokenizer_parseAccNum(t);
        }
    }

    if(t->accfl & ACC_FUN) {
        Tokenizer_error(t, "Function with no opening parenthesis.", expr_len);
        return 0;
    }

    TokenArray* tkr = csrxmalloc(sizeof(TokenArray));

    size_t item_size  = Stack_getItemSize(t->tokens);
    size_t item_count = Stack_getCount(t->tokens);
    void*  base_ptr   = Stack_getBase(t->tokens);

    size_t mem_to_copy = item_size * item_count;

    tkr->tokens = csrxmalloc(mem_to_copy);
    memcpy(tkr->tokens, base_ptr, mem_to_copy);

    for(int i = 0; i < item_count; ++i) {
        Token* t = &tkr->tokens[i];
        if(t->func) {
            size_t func_len = strlen(t->func);
            char*  new_func = csrxmalloc(func_len + 1);
            memset(new_func, 0, func_len + 1);
            memcpy(new_func, t->func, func_len);
            t->func = new_func;
        }
    }

    tkr->count = item_count;

    Stack_reClear(t->tokens);
    Stack_reClear(t->stacc);

    return tkr;
}
