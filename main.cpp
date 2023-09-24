#include <stdio.h>
#include <stdlib.h>
#include "config.h"
#include "stack.h"

int main()
{
    struct stack_t stk;
    stack_ctor(&stk);
    stack_push(&stk, 5);
    //printf("%d\n", stk.size);
    stack_push(&stk, 5);
    stack_push(&stk, 5);
    stack_push(&stk, 5);
    stack_push(&stk, 5);
    stack_push(&stk, 5);

    stack_push(&stk, 5);
    stack_push(&stk, 5);
    stack_push(&stk, 5);
    stack_push(&stk, 5);
    stack_push(&stk, 5);

    stk.size = 24120379;
    stack_dtor(&stk);
}
