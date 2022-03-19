#include <stdio.h>
#include "List.h"

int main()
{
    list_t* p_list = NULL;

    p_list = create_list();

    
    show(p_list,"After list creation");

    insert_start(p_list,130);

    show(p_list,"After instert start");

    data_t data = 140;
    insert_end(p_list,data);

    show(p_list,"after insert end");


    insert_after(p_list,130,135);

    show(p_list, "Insert after pos = 130");

    insert_before(p_list,140,137);

    show(p_list, "insert before pos = 140");

    get_start(p_list,&data);
    printf("get_start() ->%d\n",data);

    get_end(p_list,&data);
    printf("get_end() ->%d\n",data);

    pop_start(p_list,&data);
    printf("pop_start() ->%d\n",data);
    
    show(p_list, "after pop start");

    pop_end(p_list,&data);
    printf("pop_end() ->%d\n",data);
    show(p_list,"after pop end");

    destroy(&p_list);
    if(p_list != NULL)
        p_list = NULL;
    return (0);
}