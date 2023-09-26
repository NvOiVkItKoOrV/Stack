#include "../include/stack.h"

int main()
{
    struct stack_t stk;
    stack_ctor(&stk);
    for (int i = 0; i != 11; ++i)
        stack_push(&stk, 5);

    for (int i = 0; i != 2; ++i)
        stack_pop(&stk, nullptr);
    stk.size = 2134;
    stack_dtor(&stk);
}
