#ifndef _H_EVALUATOR_
#define _H_EVALUATOR_

#include "common.h"
#include "stack.h"
#include "tokenizer.h"

#include <math.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

typedef struct {
    char* name;
    double (*ptr)(double nums[], size_t len);
} Function;

extern double sft_round(double nums[], size_t len);

extern double sft_ceil(double nums[], size_t len);

extern Function FN_LOOKUP[];

typedef struct SftDrawer SftDrawer;

typedef struct SftError {
    char message[256];
} SftError;

typedef struct {
    Stack*     operator_stack;
    Stack*     number_stack;
    SftDrawer* drawer;
    SftError   error;
} Sft;

typedef struct SftDrawer {
    Sft*        sft;
    TokenArray* tarray;
    size_t      tarray_idx;
    size_t      padding;
} SftDrawer;

extern void Sft_draw(SftDrawer* drawer);

extern Sft* Sft_new();

extern double eval_binary_op(TokenType operator_type, double num1, double num2);

extern double eval_unary_op(TokenType operator_type, double num);

extern SftError* eval_x_is_operator(Sft* sft, Token token);

extern SftError* eval_x_is_close_paren(Sft* sft, Token token);

// Returns pointer to SftError stored internally in Sft instance on error.
// Does not allocate any new memory when returning an error. The Sft's
// SftError field is used as the "last error" buffer.
extern SftError* Sft_evalTokens(Sft* sft, TokenArray* tokens, double* out_result);

#endif // _H_EVALUATOR_