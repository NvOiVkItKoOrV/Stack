#ifndef STACK_H
#define STACK_H

typedef int elem_t;
typedef unsigned long long canary_t;
typedef unsigned long hash_t;


#define CANARY_PROTECTION 1
#define HASH_PROTECTION 1

const canary_t LEFT_CANARY_VALUE = 0xBADDED;
const canary_t RIGHT_CANARY_VALUE = 0xBE3BAB;

struct stack_t
{
    #if CANARY_PROTECTION == 1
        canary_t l_canary = LEFT_CANARY_VALUE;
    #endif

    char* data; // elem_t* -> char*
    long unsigned int size; // int -> some unsigned type
    long unsigned int capacity; // int -> some usigned type

    #if CANARY_PROTECTION == 1
        canary_t r_canary = RIGHT_CANARY_VALUE;
    #endif

    #if HASH_PROTECTION
        hash_t hash;
    #endif
};

void stack_ctor(struct stack_t* stk);
void stack_dtor(struct stack_t* stk);

void stack_push(struct stack_t* stk, elem_t parameter);
void stack_pop (struct stack_t* stk, elem_t* parameter);

#endif /* STACK_H */
