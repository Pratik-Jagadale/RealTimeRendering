#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "stack.h"

int main(void)
{
    stack_t *p_stack = NULL;
    status_t status;
    data_t data;

    p_stack = create_stack();

    assert(is_stack_empty(p_stack) == TRUE);
    assert(top(p_stack, &data) == STACK_EMPTY);
    assert(pop(p_stack, &data) == STACK_EMPTY);

    for (data = 0; data != 10; ++data)
        assert(push(p_stack, data) == SUCCESS);

    data = -1;
    status = top(p_stack, &data);
    assert(status);
    printf("Top data:%d\n", data);

    while (is_stack_empty(p_stack) != TRUE)
    {
        status = pop(p_stack, &data);
        assert(status);
        printf("Poped data:%d\n", data);
    }

    status = destroy_stack(&p_stack);
    assert(status && p_stack == NULL);

    return (EXIT_SUCCESS);
}
