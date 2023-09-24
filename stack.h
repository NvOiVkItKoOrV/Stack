#ifndef STACK_H
#define STACK_H

#include "config.h"


static int error_code = NO_ERR;

const int DEFAULT_STACK_CAPACITY = 10;
const int ZERO_VAL = 0;
const int ERR_VAL = -1;


struct stack_t
{
    elem_t* data;
    int size;
    int capacity;
};

void stack_ctor(struct stack_t* stk);
void stack_dtor(struct stack_t* stk);

void stack_push(struct stack_t* stk, elem_t parameter);
elem_t stack_pop(struct stack_t* stk, elem_t* parameter);

//void stack_resize_up(struct stack_t* stk);
//void stack_resize_down(struct stack_t* stk);
void stack_resize(struct stack_t* stk, code_of_resize rsz_code);

int stack_verify(struct stack_t* stk);
void stack_dump(struct stack_t* stk, const char* file_name, int       n_line,
                                   const char* func_name,int code_of_err);

#endif /* STACK_H */
