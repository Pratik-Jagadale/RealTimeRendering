#include <stdio.h>
#include "List.h"

int main()
{
    list_t* p_list = NULL;

    p_list = create_list();

    show(p_list);

    return (0);
}