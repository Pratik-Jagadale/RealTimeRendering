#include "mesh.h"
#include <stdlib.h>

// FUNCTION DEFINITION - MESH RELATED
mesh_data_t *load_mesh(const char *path)
{
    // VARIABLE DECLARATIONS
    mesh_data_t *p_mesh_data = NULL;

    char *space = " ", *slash = "/", *first_token = NULL, *token;
    char *f_entries[3] = {NULL, NULL, NULL};
    int nr_pos_cords = 0, nr_tex_cords = 0, nr_normal_cords = 0, nr_faces = 0;
    int i, vi;

    // CODE
    gp_mesh_file = fopen(path, "r");
    if (gp_mesh_file == NULL)
    {
        PrintLog("load_mesh() FAILED... ERROR IN OPENING FILE...\n");
        return NULL;
    }

    p_mesh_data = create_mesh_data();
    if (p_mesh_data == NULL)
        return NULL;

    p_mesh_data->gp_vertex = create_vec_float();
    p_mesh_data->gp_texture = create_vec_float();
    p_mesh_data->gp_normal = create_vec_float();

    p_mesh_data->gp_vertex_indices = create_vec_int();
    p_mesh_data->gp_texture_indices = create_vec_int();
    p_mesh_data->gp_normal_indices = create_vec_int();

    while (fgets(buffer, BUFFER_SIZE, gp_mesh_file) != NULL)
    {
        first_token = strtok(buffer, space);

        if (strcmp(first_token, "V") == 0) // IF VERTICES
        {
            nr_pos_cords++;
            while ((token = strtok(NULL, space)) != NULL)
                push_back_vec_float(p_mesh_data->gp_vertex, atof(token));
        }
        else if (strcmp(first_token, "Vt") == 0) // IF TEXCOORDS
        {
            nr_tex_cords++;
            while ((token = strtok(NULL, space)) != NULL)
                push_back_vec_float(p_mesh_data->gp_texture, atof(token));
        }
        else if (strcmp(first_token, "Vn") == 0) // IF NORMALS
        {
            nr_normal_cords++;
            while ((token = strtok(NULL, space)) != NULL)
                push_back_vec_float(p_mesh_data->gp_normal, atof(token));
        }
        else if (strcmp(first_token, "f") == 0) // IF FACES
        {
            nr_faces++;
            for (i = 0; i < 3; i++)
                f_entries[i] = strtok(NULL, space); // Store token of "436/150/166" , "436/150/166" ,"436/150/166"

            for (i = 0; i < 3; i++)
            {
                token = strtok(f_entries[i], slash);
                push_back_vec_int(p_mesh_data->gp_vertex_indices, atoi(token) - 1);
                token = strtok(NULL, slash);
                push_back_vec_int(p_mesh_data->gp_texture_indices, atoi(token) - 1);
                token = strtok(NULL, slash);
                push_back_vec_int(p_mesh_data->gp_normal_indices, atoi(token) - 1);
            }
        }
    }
}

void free_mesh_data(mesh_data_t **pp_mesh_data)
{
    // VARIABLE DECLARATIONS
    mesh_data_t *p_mesh_data = NULL;

    // CODE
    p_mesh_data = *pp_mesh_data;

    destroy_vec_int(p_mesh_data->gp_vertex_indices);
    destroy_vec_int(p_mesh_data->gp_texture_indices);
    destroy_vec_int(p_mesh_data->gp_normal_indices);

    destroy_vec_float(p_mesh_data->gp_vertex);
    destroy_vec_float(p_mesh_data->gp_texture);
    destroy_vec_float(p_mesh_data->gp_normal);

    free(pp_mesh_data);
    pp_mesh_data = NULL;
}

// FUNCTION DEFINITION - VECTOR RELATED
mesh_data_t *create_mesh_data(void)
{
    // VARIABLE DECLARATIONS
    mesh_data_t *p_mesh_data = NULL;

    // CODE
    p_mesh_data = (mesh_data_t *)malloc(1 * sizeof(mesh_data_t));
    if (p_mesh_data == NULL)
    {
        PrintLog("create_mesh_data FAILED... malloc():OUT OF MEMORY.\n");
        return (NULL);
    }

    p_mesh_data->gp_vertex = NULL;
    p_mesh_data->gp_texture = NULL;
    p_mesh_data->gp_normal = NULL;

    p_mesh_data->gp_vertex_indices = NULL;
    p_mesh_data->gp_texture_indices = NULL;
    p_mesh_data->gp_normal_indices = NULL;

    return (p_mesh_data);
}

vec_int_t *create_vec_int(void)
{
    // VARIABLE DECLARATIONS
    vec_int_t *p = NULL;

    // CODE
    // ALLOCATE MEMORY FOR STRUCT INTIGER VECTOR
    p = (vec_int_t *)malloc(1 * sizeof(vec_int_t));

    if (p == NULL)
    {
        PrintLog("create_vec_int FAILED... malloc():OUT OF MEMORY.\n");
        return (NULL);
    }

    // INITIALLIZATION
    p->p_arr = NULL;
    p->size = 0;

    return (p);
}

vec_float *create_vec_float(void)
{
    // VARIABLE DECLARATIONS
    vec_float_t *p = NULL;

    // CODE
    // ALLOCATE MEMORY FOR STRUCT FLOAT VECTOR
    p = (vec_float_t *)malloc(1 * sizeof(vec_float_t));

    if (p == NULL)
    {
        PrintLog("create_vec_float FAILED... - malloc() OUT OF MEMORY.\n");
        return (NULL);
    }

    // INITIALLIZATION
    p->p_arr = NULL;
    p->size = 0;

    return (p);
}

void push_back_vec_int(vec_int_t *p_vec_int, GLint new_element)
{
    // CODE
    p_vec_int->size += 1;
    p_vec_int->p_arr = (GLint *)realloc(p_vec_int->p_arr, p_vec_int->size * sizeof(GLint));
    p_vec_int->p_arr[p_vec_int->size - 1] = new_element;
}

void push_back_vec_float(vec_float_t *p_vec_float, GLfloat new_element)
{
    // CODE
    p_vec_float->size += 1;
    p_vec_float->p_arr = (GLfloat *)realloc(p_vec_float->p_arr, p_vec_float->size * sizeof(GLfloat));
    p_vec_float->p_arr[p_vec_float->size - 1] = new_element;
}

void show_vec_int(vec_int_t *)
{
}

void show_vec_float(vec_float *)
{
}

void destroy_vec_int(vec_int_t *p_vec_int)
{
    // CODE
    free(p_vec_int->p_arr);
    free(p_vec_int);
}

void destroy_vec_float(vec_float_t *p_vec_float)
{
    // CODE
    free(p_vec_float->p_arr);
    free(p_vec_float);
}

// CLIENT SIDE - TEST
/*

*/
