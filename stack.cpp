#include <stdio.h>
#include <stdlib.h>
#include "stack.h"


typedef enum
{
    NO_ERR = 0,

    SIZE_BIGGER_CAPACITY = 1,

    CAPACITY_LESS_DEFAULT_SZ = 2,

    DATA_IS_LOST = 4,
    STACK_IS_LOST = 8,

    CANARY_IS_CHANGED_ERR = 16

} err_types;


typedef enum
{
    RESIZE_UP,
    RESIZE_DOWN

} code_of_resize;


static int error_code = NO_ERR;

const int DEFAULT_STACK_CAPACITY = 10;

const canary_t LEFT_CANARY_VALUE_DATA = 0xC0FFEE;
const canary_t RIGHT_CANARY_VALUE_DATA = 0xBABADED;

const int CANARY_IS_GOOD = 0;
const int CANARY_IS_CHANGED = 1;


#define CHECK_CANARY(stk)                                     \
if(check_canary(stk))                                         \
{                                                             \
    stack_dump(stk, __FILE__, __LINE__, __func__, error_code);\
}


#define STACK_CHECK(stk)                                      \
if((error_code = stack_verify(stk)) != NO_ERR)                \
{                                                             \
    stack_dump(stk, __FILE__, __LINE__, __func__, error_code);\
}


#define MY_ASSERT(condition)                              \
if(!condition)                                            \
{                                                         \
    printf("ERROR CONDITION IN file: %s func: %s line %d",\
                            __FILE__, __func__, __LINE__);\
}


static void stack_resize(struct stack_t* stk, code_of_resize rsz_code);

static int check_canary(const struct stack_t* stk);

static int stack_verify(const struct stack_t* stk);
static void stack_dump(const struct stack_t* stk, const char* file_name, int n_line,
                                                  const char* func_name, int code_of_err);


void stack_ctor(struct stack_t* stk)
{
    MY_ASSERT(stk);

        stk->capacity = DEFAULT_STACK_CAPACITY;
        stk->size     = 0;
    #if CANARY_PROTECTION == 0
        stk->data = (char*)calloc(DEFAULT_STACK_CAPACITY * sizeof(elem_t), sizeof(char));
    #elif CANARY_PROTECTION == 1
        stk->data = (char*)calloc(stk->capacity*sizeof(elem_t) + 2*sizeof(canary_t), sizeof(char));
        *(canary_t*)stk->data = LEFT_CANARY_VALUE_DATA;
        stk->data += sizeof(canary_t);
        *(canary_t*)(stk->data + stk->capacity * sizeof(elem_t)) = RIGHT_CANARY_VALUE_DATA;
    #endif

    #if CANARY_PROTECTION == 1
        CHECK_CANARY(stk);
    #endif

    STACK_CHECK(stk);

}


void stack_dtor(struct stack_t* stk)
{
    STACK_CHECK(stk);

    #if CANARY_PROTECTION == 1
        CHECK_CANARY(stk);
    #endif

    stk->capacity = 0;
    stk->size = 0;

    #if CANARY_PROTECTION == 0
        free(stk->data);
    #elif CANARY_PROTECTION == 1
        stk->data -= sizeof(canary_t);
        free(stk->data);
    #endif

    stk->data = nullptr;
}


void stack_push(struct stack_t* stk, elem_t parameter)
{
    STACK_CHECK(stk);

    #if CANARY_PROTECTION == 1
        CHECK_CANARY(stk);
    #endif

    printf("%lu %lu\n", stk->capacity, stk->size);


    if(stk->capacity == stk->size)
    {
        printf("zhopa\n");
        printf("zhopa\n");
        printf("zhopa\n");

        stack_resize(stk, RESIZE_UP);
    }

    *((elem_t*)stk->data + stk->size) = parameter;
    stk->size++;

    #if CANARY_PROTECTION == 1
        CHECK_CANARY(stk);
    #endif

    STACK_CHECK(stk);

}

//stack_t + rename parameter
void stack_pop(struct stack_t* stk, elem_t* parameter)
{
    STACK_CHECK(stk);

    #if CANARY_PROTECTION == 1
        CHECK_CANARY(stk);
    #endif

    if(stk->size > 0)
    {
        printf("%lu %lu\n", stk->capacity, stk->size);

        if(stk->capacity > DEFAULT_STACK_CAPACITY && stk->size <= stk->capacity / 4)
            stack_resize(stk, RESIZE_DOWN);

        STACK_CHECK(stk);

        --stk->size;
        if (parameter)
        {
            *parameter = *((elem_t*)stk->data + stk->size);
            *((elem_t*)stk->data + stk->size) = 0;
        }
    }

    #if CANARY_PROTECTION == 1
        CHECK_CANARY(stk);
    #endif

    STACK_CHECK(stk);
}


static void stack_resize(struct stack_t* stk, code_of_resize rsz_code)
{
    STACK_CHECK(stk);

    #if CANARY_PROTECTION == 1
        CHECK_CANARY(stk);
    #endif

    printf("zhirnaya");
    if(rsz_code == RESIZE_UP)
    {
        #if CANARY_PROTECTION == 1
            unsigned long int old_capacity = stk->capacity;
        #endif

        stk->capacity *= 2;

        #if CANARY_PROTECTION == 0
            stk->data = (char*)realloc(stk->data, (size_t)stk->capacity * sizeof(elem_t));
        #elif CANARY_PROTECTION == 1
            stk->data -= sizeof(canary_t);
            stk->data = (char*)realloc(stk->data, stk->capacity * sizeof(elem_t) + 2*sizeof(canary_t));
            stk->data += sizeof(canary_t);
            *(canary_t*)((elem_t*)stk->data + old_capacity) = 0;
            *(canary_t*)((elem_t*)stk->data + stk->capacity) = RIGHT_CANARY_VALUE_DATA;
        #endif
    }
    else if(rsz_code == RESIZE_DOWN)
    {
        stk->capacity /= 2;

        #if CANARY_PROTECTION == 0
            stk->data = (char*)realloc(stk->data, (size_t)stk->capacity * sizeof(elem_t));
        #elif CANARY_PROTECTION == 1
            stk->data -= sizeof(canary_t);
            stk->data = (char*)realloc(stk->data, stk->capacity * sizeof(elem_t) + 2*sizeof(canary_t));
            stk->data += sizeof(canary_t);
            *(canary_t*)((elem_t*)stk->data + stk->capacity) = RIGHT_CANARY_VALUE_DATA;
        #endif

    }
    else
        printf("ERROR! CODE OF RESIZE IS SHIT!!!!!");

    #if CANARY_PROTECTION == 1
        CHECK_CANARY(stk);
    #endif

    STACK_CHECK(stk);
}


static int stack_verify(const struct stack_t* stk)
{

    int check_err = NO_ERR;

    if(!stk)
    {
        check_err |= STACK_IS_LOST;
    }

    if(!stk->data)
    {
        check_err |= DATA_IS_LOST;
    }

    if(stk->size > stk->capacity)
    {
        check_err |= SIZE_BIGGER_CAPACITY;
    }

    if(stk->capacity < DEFAULT_STACK_CAPACITY)
    {
        check_err |= CAPACITY_LESS_DEFAULT_SZ;
    }

    return check_err;
}


static void stack_dump(const struct stack_t* stk, const char* file_name, int n_line, const char* func_name, int code_of_err)
{
    printf("Problem is found in:\n");
    printf("File - %s \n", file_name);
    printf("Function - %s \n", func_name);
    printf("Number of line - %d \n", n_line);
    printf("Code of error - %d \n", code_of_err);

    printf("----------------------------------------\n");
    if((code_of_err & STACK_IS_LOST) == STACK_IS_LOST)
    {
        printf("ERROR! Adress of stack is lost!");
        exit(EXIT_FAILURE);
    }

    if((code_of_err & DATA_IS_LOST) == DATA_IS_LOST)
    {
        printf("ERROR! Data adress is lost!");
        exit(0);
    }

    if((code_of_err & SIZE_BIGGER_CAPACITY) == SIZE_BIGGER_CAPACITY)
    {
        printf("ERROR! In stack size bigger than capacity!\n");
    }

    if((code_of_err & CAPACITY_LESS_DEFAULT_SZ) == CAPACITY_LESS_DEFAULT_SZ)
    {
        printf("ERROR! Capacity is less, than default value!\n");
    }

    #if CANARY_PROTECTION == 1
    if ((code_of_err & CANARY_IS_CHANGED_ERR) == CANARY_IS_CHANGED_ERR)
    {
        printf("ERROR! Canary in stack are changed!!!");
    }
    #endif

    printf("----------------------------------------\n");

    printf("ALL INFORMATION ABOUT STACK:\n");

    printf("size - %lu\n", stk->size);
    printf("capacity - %lu\n", stk->capacity);
    printf("adress of data - %p\n", stk->data);

    #if CANARY_PROTECTION == 1
    printf("left canary - %llX\n", stk->l_canary);
    printf("right canary - %llX\n", stk->r_canary);

    printf("DEFAULT left canary value - %llX\n", LEFT_CANARY_VALUE);
    printf("DEFAULT right canary value - %llX\n", RIGHT_CANARY_VALUE);

    printf("data left canary - %llX\n", *(canary_t*)(stk->data -sizeof(canary_t)));
    printf("data right canary - %llX\n", *(canary_t*)(stk->data + stk->capacity * sizeof(elem_t)));

    printf("DEFAULT data left canary - %llX\n", LEFT_CANARY_VALUE_DATA);
    printf("DEFAULT data right canary - %llX\n", RIGHT_CANARY_VALUE_DATA);
    #endif

    for(unsigned long int counter = 0; counter < stk->capacity; counter++)
    {
        if(counter < stk->size)
            printf("[%lu] =  %d\n", counter, *((elem_t*)stk->data + counter));
        else
            printf("[%lu] =  %d (POISON)\n", counter, *((elem_t*)stk->data + counter));
    }
}
#if CANARY_PROTECTION == 1
static int check_canary(const struct stack_t* stk)
{
    canary_t l_canary_check = stk->l_canary;
    canary_t r_canary_check = stk->r_canary;

    canary_t* l_data_canary_check = (canary_t*)(stk->data - sizeof(canary_t));
    canary_t* r_data_canary_check = (canary_t*)(stk->data + stk->capacity * sizeof(elem_t));


    if(l_canary_check != LEFT_CANARY_VALUE)
    {
        error_code |= CANARY_IS_CHANGED_ERR;
        return CANARY_IS_CHANGED;
    }

    else if(r_canary_check != RIGHT_CANARY_VALUE)
    {
        error_code |= CANARY_IS_CHANGED_ERR;
        return CANARY_IS_CHANGED;
    }

    else if(*l_data_canary_check != LEFT_CANARY_VALUE_DATA)
    {
        error_code |= CANARY_IS_CHANGED_ERR;
        return CANARY_IS_CHANGED;
    }

    else if(*r_data_canary_check != RIGHT_CANARY_VALUE_DATA)
    {
        error_code |= CANARY_IS_CHANGED_ERR;
        return CANARY_IS_CHANGED;
    }

    else
        return CANARY_IS_GOOD;
}
#endif
