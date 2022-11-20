#include "mesh.h"
#include <stdlib.h>
#include <stdio.h>   // For fopen_s(), fPrintLog(), fclose()
#include <windows.h> // For BOOL, TEXT(), MessageBox(), etc.cd

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
    if (p_mesh_data->gp_vertex == NULL)
    {
        return NULL;
        PrintLog("Error5 \n");
    }

    p_mesh_data->gp_texture = create_vec_float();
    if (p_mesh_data->gp_texture == NULL)
    {
        return NULL;
        PrintLog("Error6 \n");
    }

    p_mesh_data->gp_normal = create_vec_float();
    if (p_mesh_data->gp_normal == NULL)
    {
        return NULL;
        PrintLog("Error7 \n");
    }

    p_mesh_data->gp_vertex_indices = create_vec_int();
    if (p_mesh_data->gp_vertex_indices == NULL)
    {
        return NULL;
        PrintLog("Error8 \n");
    }

    p_mesh_data->gp_texture_indices = create_vec_int();
    if (p_mesh_data->gp_texture_indices == NULL)
    {
        return NULL;
        PrintLog("Error9 \n");
    }

    p_mesh_data->gp_normal_indices = create_vec_int();
    if (p_mesh_data->gp_normal_indices == NULL)
    {
        PrintLog("Error10 \n");
        return NULL;
    }

    while (fgets(buffer, BUFFER_SIZE, gp_mesh_file) != NULL)
    {
        first_token = strtok(buffer, space);
        // PrintLog("Error 2\n");
        if (strcmp(first_token, "v") == 0) // IF VERTICES
        {
            nr_pos_cords++;
            while ((token = strtok(NULL, space)) != NULL)
                push_back_vec_float(p_mesh_data->gp_vertex, atof(token));
        }
        else if (strcmp(first_token, "vt") == 0) // IF TEXCOORDS
        {
            nr_tex_cords++;
            while ((token = strtok(NULL, space)) != NULL)
                push_back_vec_float(p_mesh_data->gp_texture, atof(token));
        }
        else if (strcmp(first_token, "vn") == 0) // IF NORMALS
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

    // FETCH DATA INDECIES WISE
    p_mesh_data->gp_vertex_filter = create_vec_float();
    if (p_mesh_data->gp_vertex_filter == NULL)
    {
        PrintLog("Error8 \n");
        return NULL;
    }

    p_mesh_data->gp_texture_filter = create_vec_float();
    if (p_mesh_data->gp_texture_filter == NULL)
    {
        PrintLog("Error9 \n");
        return NULL;
    }

    p_mesh_data->gp_normal_filter = create_vec_float();
    if (p_mesh_data->gp_normal_filter == NULL)
    {
        PrintLog("Error10 \n");
        return NULL;
    }

    // FILTER VERTEX DATA
    for (int i = 0; i < p_mesh_data->gp_vertex_indices->size; i++)
    {
        // push_back_vec_float(p_mesh_data->gp_vertex_filter, p_mesh_data->gp_vertex->p_arr[p_mesh_data->gp_vertex_indices->p_arr[i]]);
        // push_back_vec_float(p_mesh_data->gp_vertex_filter, p_mesh_data->gp_vertex->p_arr[p_mesh_data->gp_vertex_indices->p_arr[i + 1]]);
        // push_back_vec_float(p_mesh_data->gp_vertex_filter, p_mesh_data->gp_vertex->p_arr[p_mesh_data->gp_vertex_indices->p_arr[i + 2]]);

        // p_mesh_data->gp_vertex_filter = p_mesh_data->gp_vertex->p_arr[p_mesh_data->gp_vertex_indices->p_arr[i]];
    }
    // PrintLog("szie  === %d\n", p_mesh_data->gp_vertex_filter->size);

    // FILTER NOMRAL DATA
    for (int i = 0; i < p_mesh_data->gp_normal_indices->size; i++)
    {
        // push_back_vec_float(p_mesh_data->gp_normal_filter, p_mesh_data->gp_normal->p_arr[p_mesh_data->gp_normal_indices->p_arr[i]]);
        // push_back_vec_float(p_mesh_data->gp_normal_filter, p_mesh_data->gp_normal->p_arr[p_mesh_data->gp_normal_indices->p_arr[i] + 1]);
        // push_back_vec_float(p_mesh_data->gp_normal_filter, p_mesh_data->gp_normal->p_arr[p_mesh_data->gp_normal_indices->p_arr[i] + 2]);

        // p_mesh_data->gp_normal_filter->p_arr[i] = p_mesh_data->gp_normal->p_arr[p_mesh_data->gp_normal_indices->p_arr[i]];
    }

    // FILTER TEXCOOORD
    for (int i = 0; i < p_mesh_data->gp_texture_indices->size; i++)
    {
        // push_back_vec_float(p_mesh_data->gp_texture_filter, p_mesh_data->gp_texture->p_arr[p_mesh_data->gp_texture_indices->p_arr[i]]);
        // p_mesh_data->gp_texture_filter->p_arr[i] = p_mesh_data->gp_texture->p_arr[p_mesh_data->gp_texture_indices->p_arr[i]];
    }

    return (p_mesh_data);
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

    destroy_vec_float(p_mesh_data->gp_vertex_filter);
    destroy_vec_float(p_mesh_data->gp_texture_filter);
    destroy_vec_float(p_mesh_data->gp_normal_filter);

    free(pp_mesh_data);
    pp_mesh_data = NULL;

    PrintLog("OBJ Data Destroyed Successfulyy..");
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
/*
int main(void)
{
    mesh_data_t *new_mash_data;

    new_mash_data = load_mesh("castle.obj");
    if (new_mash_data == NULL)
        PrintLog("Error \n");
    else
        PrintLog("DATA READED SUCCEESFULLY\n");

    int i = 0;

    PrintLog("gp_vertex->size : %d\n", new_mash_data->gp_vertex->size);
    PrintLog("gp_vertex->size : %d\n", new_mash_data->gp_vertex->size);
    PrintLog("gp_vertex->size : %d\n", new_mash_data->gp_vertex->size);

    while (i < new_mash_data->gp_vertex->size)
    {
        int j = 0;
        j = i;

        for (j = 0; j < 3; j++)
        {
            PrintLog("%f,", new_mash_data->gp_vertex->p_arr[i]);
        }
        PrintLog("\n");
        i = i + 3;
    }

    free_mesh_data(&new_mash_data);

    return 0;
}
*/