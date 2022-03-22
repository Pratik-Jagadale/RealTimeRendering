#define pragama once

#define TRUE 1
#define FLASE 0
#define SUCCESS 1

#define G_VERTEX_EXITS 2
#define G_INVALID_VERTEX 3
#define G_EDGE_EXITS 4
#define G_INVALID 5

typedef int vertex_t;
typedef int status_t;
typedef struct hnode hnode_t;
typedef hnode_t hlist_t;
typedef struct vnode vnode_t;
typedef vnode_t vlist_t;
typedef struct graph graph_t;

struct hnode
{
    vertex_t v;
    hnode_t *h_prev;
    hnode_t *h_next;
};

struct vnode
{
    vertex_t v;
    hnode_t *ph_head_node;
    vnode_t *v_prev;
    vnode_t *v_next;
};

struct graph
{
    vnode_t *pv_head_node;
    unsigned long nr_vertices;
    unsigned long nr_edges;
};

/* graph interface routines */

graph_t *create_graph(void);

status_t add_vertex(graph_t *g, vertex_t new_vertex);
status_t remove_vertex(graph_t *g, vertex_t r_vertex);
status_t add_edge(graph_t *g, vertex_t v_start, vertex_t v_end);
status_t remove_edge(graph_t *g, vertex_t v_start, vertex_t v_end);

void show_graph(graph_t *g, const char *msg);
status_t destroy_graph(graph_t **pp_g);

/* graph helper routines */
vlist_t *v_create_list(void);
status_t v_insert_end(vlist_t *pv_list, vertex_t new_vertex);

/* Graph helper routines -> Vertical List Management -> vertical list Interface routines */
vlist_t *v_create_list(void);
status_t v_insert_end(vlist_t *pv_list, vertex_t new_vertex);

/* Graph helper routines -> Vertical List Management -> vertical list helper routines */
void v_generic_insert(vnode_t *pv_beg, vnode_t *pv_mid, vnode_t *pv_end);
void v_generic_delete(vnode_t *p_delete_node);
vnode_t *v_search_node(vlist_t *pv_list, vertex_t search_vertex);
vnode_t *v_get_list_node(vertex_t new_vertex);

/* Graph helper routines -> Horizontal List Management -> Horizontal Interfcae routines */
hlist_t *h_create_list(void);
status_t *h_insert_end(hlist_t *ph_list, vertex_t new_vertex);

/* Graph helper routines -> Horizontal List Management -> Vertical Interfcae routines */
void h_generic_insert(hnode_t *ph_beg, hnode_t *ph_mid, hnode_t *ph_end);
void h_generic_delete(hnode_t *ph_delete_node);
hnode_t *h_search_node(hlist_t *ph_list, vertex_t search_vertex);
hnode_t *h_get_list_node(vertex_t new_vertex);

/* graph helper routine -> general prpose */
void *xmalloc(size_t nr_bytes);
