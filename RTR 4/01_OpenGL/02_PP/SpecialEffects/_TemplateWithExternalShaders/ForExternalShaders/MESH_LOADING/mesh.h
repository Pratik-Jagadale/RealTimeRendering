#define pragma once

#include <stdio.h>
#include <stdlib.h>

#include "../GLHeadersAndMacros.h"
#include "../GLLog.h"

// MACROS
#define BUFFER_SIZE 1024

// TYPEDEFS
typedef GLint size_t;
typedef struct vec_int vec_int_t;
typedef struct vec_float vec_float_t;
typedef struct mesh_data mesh_data_t;

// GLOBAL VARIABLES
FILE *gp_mesh_file;
char buffer[BUFFER_SIZE];

struct vec_int
{
    GLint *p_arr;
    size_t size;
};

struct vec_float
{
    GLfloat *p_arr;
    size_t size;
};

struct mesh_data
{
    vec_float_t *gp_vertex;
    vec_float_t *gp_texture;
    vec_float_t *gp_normal;

    vec_int_t *gp_vertex_indices;
    vec_int_t *gp_texture_indices;
    vec_int_t *gp_normal_indices;
};

// FUNCTION PROTOTYPE - MESH RELATED
mesh_data_t *load_mesh(const char *);
void free_mesh_data(mesh_data_t *);

// FUNCTION PROTOTYPE - VECTOR RELATED
vec_int_t *create_vec_int(void);
vec_float *create_vec_float(void);

void push_back_vec_int(vec_int_t *, GLint);
void push_back_vec_int(vec_float_t *, GLfloat);

void show_vec_int(vec_int_t *);
void show_vec_float(vec_float *);

void destroy_vec_int(vec_int_t *);
void destroy_vec_float(vec_float_t *);
