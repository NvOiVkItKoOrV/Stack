#include "../include/stack.h"

int main()
{
    struct stack_t stk;
    stack_ctor(&stk);
    //for (int i = 0; i != 15; ++i)
        stack_push(&stk, 5);

    //for (int i = 0; i != 7; ++i)
      //  stack_pop(&stk, nullptr);

    stack_dtor(&stk);
}
