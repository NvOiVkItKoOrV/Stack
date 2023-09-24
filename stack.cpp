#include <stdio.h>
#include <stdlib.h>
#include "config.h"
#include "stack.h"

//memcpy + size of data + void*

void stack_ctor(struct stack_t* stk)
{
    MY_ASSERT(stk);

    stk->capacity = DEFAULT_STACK_CAPACITY;
    stk->size     = ZERO_VAL;
    stk->data = (elem_t*)calloc(stk->capacity, sizeof(elem_t));

    STACK_CHECK(stk);
}


void stack_dtor(struct stack_t* stk)
{
    STACK_CHECK(stk);

    stk->capacity = ERR_VAL;
    stk->size = ERR_VAL;
    free(stk->data);
    stk->data = nullptr;

}


void stack_push(struct stack_t* stk, elem_t parameter)
{
    STACK_CHECK(stk);

    printf("%d %d\n", stk->capacity, stk->size);


    if(stk->capacity == stk->size)
    {
        printf("zhopa\n");
        printf("zhopa\n");
        printf("zhopa\n");

        stack_resize(stk, RESIZE_UP);
    }
    stk->data[stk->size++] = parameter;

    STACK_CHECK(stk);
}

//stack_t + rename parameter
elem_t stack_pop(struct stack_t* stk, elem_t* parameter)
{
    STACK_CHECK(stk);

    if(stk->capacity > DEFAULT_STACK_CAPACITY && stk->size <= stk->capacity / 4)
        stack_resize(stk, RESIZE_DOWN);

    STACK_CHECK(stk);

    *parameter = stk->data[--stk->size];
    stk->data[stk->size] = ZERO_VAL;

    STACK_CHECK(stk);

    return *parameter;
}


void stack_resize(struct stack_t* stk, code_of_resize rsz_code)
{
    STACK_CHECK(stk);
    printf("zhirnaya");
    if(rsz_code == RESIZE_UP)
    {
        int old_capacity = stk->capacity;

        stk->capacity *= 2;
        stk->data = (elem_t*)realloc(stk->data, (size_t)stk->capacity);

        int counter = old_capacity;

        //while(counter < stk->capacity)
        //{
            stk->data[11] = 0;
            stk->data[12] = 0;
            stk->data[13] = 0;
            stk->data[14] = 0;
            stk->data[15] = 0;
        //}

        if(old_capacity > stk->capacity)
            error_code |= RESIZE_UP_ERR;
    }
    else if(rsz_code == RESIZE_DOWN)
    {
        int old_capacity = stk->capacity;

        stk->capacity /= 2;
        stk->data = (elem_t*)realloc(stk->data, (size_t)stk->capacity);

        if(old_capacity < stk->capacity)
            error_code |= RESIZE_DOWN_ERR;
    }
    else
        printf("ERROR! CODE OF RESIZE IS SHIT!!!!!");

    STACK_CHECK(stk);
}


int stack_verify(struct stack_t* stk)
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

    if(stk->size < 0)
    {
        check_err |= SIZE_LESS_ZERO;
    }

    if(stk->size > stk->capacity)
    {
        check_err |= SIZE_BIGGER_CAPACITY;
    }

    if(stk->capacity < DEFAULT_STACK_CAPACITY)
    {
        check_err |= CAPACITY_LESS_DEFAULT_SZ;
    }

    if(stk->capacity < ZERO_VAL)
    {
        check_err |= CAPACITY_LESS_ZERO;
    }

    return check_err;
}


void stack_dump(struct stack_t* stk, const char* file_name, int n_line, const char* func_name, int code_of_err)
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
        exit(0);
    }

    if((code_of_err & DATA_IS_LOST) == DATA_IS_LOST)
    {
        printf("ERROR! Data adress is lost!");
        exit(0);
    }

    if((code_of_err & SIZE_LESS_ZERO) == SIZE_LESS_ZERO)
    {
        printf("ERROR! In stack size less than zero!\n");
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

    if((code_of_err & CAPACITY_LESS_ZERO) == CAPACITY_LESS_ZERO)
    {
        printf("ERROR! Capacity less zero!\n");
        exit(0);
    }

    printf("----------------------------------------\n");

    printf("ALL INFORMATION ABOUT STACK:\n");

    printf("size - %d\n", stk->size);
    printf("capacity - %d\n", stk->capacity);

    for(int counter = 0; counter < stk->capacity; counter++)
    {
        if(counter < stk->size)
            printf("[%d] =  %d\n", counter, stk->data[counter]);
        else
            printf("[%d] =  %d (POISON)\n", counter, stk->data[counter]);
    }
}
