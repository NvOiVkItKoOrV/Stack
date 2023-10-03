#ifndef STACK_H
#define STACK_H

typedef enum
{
    NO_ERR = 0,

    SIZE_BIGGER_CAPACITY = 1,

    CAPACITY_LESS_DEFAULT_SZ = 2,

    DATA_IS_LOST = 4,
    STACK_IS_LOST = 8,

    CANARY_IS_CHANGED_ERR = 16,

    HASH_IS_CHANGED_ERR = 32

} err_types;


typedef enum
{
    RESIZE_UP,
    RESIZE_DOWN

} code_of_resize;

typedef enum
{
    HASH_IS_EQUAL = 0,
    HASH_IS_CHANGED = 1

} hash_comp;

typedef double elem_t;
typedef unsigned long long canary_t;
typedef unsigned long hash_t;


#define CANARY_PROTECTION
#define HASH_PROTECTION

const canary_t LEFT_CANARY_VALUE  = 0xBADDED;
const canary_t RIGHT_CANARY_VALUE = 0xBE3BAB;

struct stack_t
{
    #ifdef CANARY_PROTECTION
        canary_t l_canary = LEFT_CANARY_VALUE;
    #endif

    char* data; // elem_t* -> char*
    long unsigned int size; // int -> some unsigned type
    long unsigned int capacity; // int -> some usigned type

    #ifdef CANARY_PROTECTION
        canary_t r_canary = RIGHT_CANARY_VALUE;
    #endif

    #ifdef HASH_PROTECTION
        hash_t hash;
    #endif
};

void stack_ctor(struct stack_t* stk);
void stack_dtor(struct stack_t* stk);

void stack_push(struct stack_t* stk, elem_t parameter);
void stack_pop (struct stack_t* stk, elem_t* parameter);

#endif /* STACK_H */
