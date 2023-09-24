#ifndef CONFIG_H
#define CONFIG_H

typedef enum
{
    NO_ERR = 0,

    SIZE_LESS_ZERO = 1,
    SIZE_BIGGER_CAPACITY = 2,

    CAPACITY_LESS_DEFAULT_SZ = 4,
    CAPACITY_LESS_ZERO = 8,

    RESIZE_UP_ERR = 16,
    RESIZE_DOWN_ERR = 32,

    DATA_IS_LOST = 64,
    STACK_IS_LOST = 128

} err_types;


typedef enum
{
    RESIZE_UP,
    RESIZE_DOWN

} code_of_resize;


typedef int elem_t;


#define MY_ASSERT(condition)                              \
if(!condition)                                            \
{                                                         \
    printf("ERROR CONDITION IN file: %s func: %s line %d",\
                            __FILE__, __func__, __LINE__);\
}                                                         \


#define STACK_CHECK(stk)                                      \
if((error_code = stack_verify(stk)) != NO_ERR)                \
{                                                             \
    stack_dump(stk, __FILE__, __LINE__, __func__, error_code);\
}                                                             \


#endif /* CONFIG_H */
