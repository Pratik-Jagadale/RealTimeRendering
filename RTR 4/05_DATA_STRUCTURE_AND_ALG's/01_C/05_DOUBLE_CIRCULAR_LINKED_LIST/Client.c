
#include "dcll.h"

int main(void)
{
    //varibale decartions
    list_t *p_list = NULL;
    data_t p_data;
    status_t result;
    int i, j;

    printf("\n\n");
    printf("This is Client for Test dcll Server...\n\n");

    p_list = create_list();
    for (i = 0; i < 10; i++)
    {
        insert_beg(p_list, i * 4);
    }

    for (i = 0; i < 10; i++)
    {
        insert_end(p_list, i);
    }

    show_list(p_list, "After Insert First And Insert at End\n");

    printf("\ndg\n");
    get_beg(p_list, &p_data);

    printf("\ndgwefw\n");
    printf("Get Beg = %d\n", p_data);
    printf("\n--------------\n");
    
    get_end(p_list, &p_data);
    printf("Get End = %d\n", p_data);
    printf("\n--------\n");
    show_list(p_list, "Before PopBeg & PopEnd\n");
    pop_beg(p_list, &p_data);
    printf("Pop Beg = %d\n", p_data);
    pop_end(p_list, &p_data);
    printf("Pop End = %d\n", p_data);

    show_list(p_list, "After PopBeg & PopEnd\n");

    show_list(p_list, "Before Remove Beg and Remove End\n");
    remove_beg(p_list);
    remove_end(p_list);
    show_list(p_list, "After remove Beg And Remove End\n");

    for (i = 0; i < 8; i++)
    {
        remove_data(p_list, i);
    }
    show_list(p_list, "After remove Data fron 0 to 7\n");

    len_t len = get_length(p_list);

    printf("Length Of The List is  = %d\n\n", len);

    destroy_list(&p_list);

    return (0);
}