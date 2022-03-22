#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "graph.h"

/* graph interface routines */

graph_t *create_graph(void)
{
    graph_t *g = NULL;
    g->nr_vertices = 0;
    g->nr_edges = 0;
    g->pv_head_node = v_create_list();

    return (g);
}

status_t add_vertex(graph_t *g, vertex_t new_vertex)
{
    vnode_t *p = NULL;
    status_t status;

    p = v_search_node(g->pv_head_node, new_vertex);
    if (p != NULL)
        return (G_VERTEX_EXITS);
    status = v_insert_end(g->pv_head_node, new_vertex);
    assert(status == SUCCESS);

    return (SUCCESS);
}

status_t remove_vertex(graph_t *g, vertex_t r_vertex)
{
    
}

status_t add_edge(graph_t *g, vertex_t v_start, vertex_t v_end);
status_t remove_edge(graph_t *g, vertex_t v_start, vertex_t v_end);

void show_graph(graph_t *g, const char *msg);
status_t destroy_graph(graph_t **pp_g);

/* Graph helper routines -> Vertical List Management -> vertical list Interface routines */

vlist_t *v_create_list(void)
{
    vnode_t *pv_head_list = NULL;
    pv_head_list = (vlist_t *)xmalloc(sizeof(vlist_t));
    pv_head_list->v = -1;
    pv_head_list->ph_head_node = NULL;
    pv_head_list->v_next = pv_head_list;
    pv_head_list->v_prev = pv_head_list;

    return (pv_head_list);
}

status_t v_insert_end(vlist_t *pv_list, vertex_t new_vertex)
{
    v_generic_insert(pv_list->v_prev, v_get_list_node(new_vertex), pv_list);
    return (SUCCESS);
}

/* Graph helper routines -> Vertical List Management -> vertical list helper routines */
void v_generic_insert(vnode_t *pv_beg, vnode_t *pv_mid, vnode_t *pv_end)
{
    pv_mid->v_next = pv_end;
    pv_mid->v_prev = pv_beg;
    pv_beg->v_next = pv_mid;
    pv_end->v_prev = pv_mid;
}

void v_generic_delete(vnode_t *p_delete_node)
{
    p_delete_node->v_prev->v_next = p_delete_node->v_next;
    p_delete_node->v_next->v_prev = p_delete_node->v_prev;
    free(p_delete_node);
}

vnode_t *v_search_node(vlist_t *pv_list, vertex_t search_vertex)
{
    vnode_t *pv_run = NULL;
    pv_run = pv_list->v_next;

    while (pv_run != pv_list)
    {
        if (pv_run->v == search_vertex)
            return (pv_run);
        pv_run = pv_run->v_next;
    }
    return (NULL);
}

vnode_t *v_get_list_node(vertex_t new_vertex)
{
    vnode_t *p_vnode = NULL;
    p_vnode = (vnode_t *)xmalloc(sizeof(vnode_t));
    p_vnode->v = new_vertex;
    p_vnode->ph_head_node = h_create_list();
    p_vnode->v_next = NULL;
    p_vnode->v_prev = NULL;

    return (p_vnode);
}

/* Graph helper routines -> Horizontal List Management -> Horizontal Interfcae routines */
hlist_t *h_create_list(void)
{
    hlist_t *ph_list = NULL;
    ph_list = (hlist_t *)xmalloc(sizeof(hlist_t));

    ph_list->v = -1;
    ph_list->h_next = ph_list;
    ph_list->h_prev = ph_list;
    return (ph_list);
}

status_t *h_insert_end(hlist_t *ph_list, vertex_t new_vertex)
{
    h_generic_insert(ph_list->h_prev, h_get_list_node(new_vertex), ph_list);
    return (SUCCESS);
}

/* Graph helper routines -> Horizontal List Management -> Vertical Interfcae routines */
void h_generic_insert(hnode_t *ph_beg, hnode_t *ph_mid, hnode_t *ph_end)
{
    ph_mid->h_next = ph_end;
    ph_mid->h_prev = ph_beg;
    ph_beg->h_prev = ph_mid;
    ph_end->h_next = ph_mid;
}

void h_generic_delete(hnode_t *ph_delete_node)
{
    ph_delete_node->h_prev->h_next = ph_delete_node->h_next;
    ph_delete_node->h_next->h_prev = ph_delete_node->h_prev;
    free(ph_delete_node);
}

hnode_t *h_search_node(hlist_t *ph_list, vertex_t search_vertex)
{
    hnode_t *p_run = NULL;
    p_run = ph_list->h_next;
    while (p_run != ph_list)
    {
        if (p_run->v == search_vertex)
            return (p_run);
        p_run = p_run->h_next;
    }
    return (NULL);
}

hnode_t *h_get_list_node(vertex_t new_vertex)
{
    hnode_t *p_hnode;
    p_hnode = (hnode_t *)xmalloc(sizeof(hnode_t));
    p_hnode->v = new_vertex;
    p_hnode->h_next = NULL;
    p_hnode->h_prev = NULL;
    return (p_hnode);
}

/* graph helper routine -> general prpose */
void *xmalloc(size_t nr_bytes)
{
    void *p = NULL;
    p = malloc(nr_bytes);
    assert(p != NULL);

    return (p);
}
