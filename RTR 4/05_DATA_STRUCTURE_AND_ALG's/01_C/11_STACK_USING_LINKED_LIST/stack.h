#include "list.h"

/* Symbolic constants */
#define STACK_EMPTY LIST_EMPTY

/* typedefs */
typedef list_t stack_t;

stack_t *create_stack();
status_t push(stack_t *p_stack, data_t new_data);
status_t top(stack_t *p_stack, data_t *p_data);
status_t pop(stack_t *p_stack, data_t *p_data);
status_t is_stack_empty(stack_t *p_stack);
status_t destroy_stack(stack_t **pp_stack);
