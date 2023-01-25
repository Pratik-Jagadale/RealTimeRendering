#pragma once

#include<math.h>

FILE* gpFileSphere = NULL;

// Variables for Sphere
int iNoOfVertices;
int iNoOfElements;
GLfloat fRadius = 1.0f;
int iStacks = 90;
int iSlices = 90;

int iRotate=0;
float fAngleX=0.0f;
float fAngleZ=0.0f;
float fAngleY=0.0f;

bool bHalfCircle=false;

GLfloat *sphereVertices_new;
GLfloat *sphereNormals_new;
GLushort *sphereElements_new;

// Sphere 
GLuint Vao_sphere;
GLuint Vbo_sphere_position;
GLuint Vbo_sphere_normal;
GLuint Vbo_sphere_element;



//#----Prototypes of new Sphere Functions------#

void newSphere( GLfloat radius, GLint slices, GLint stacks,GLfloat **vertices, GLfloat **normals, int* nVert,const GLboolean halfCircle,GLshort **sphereElement_New);

void newGenerateSphere(GLfloat radius, GLint slices, GLint stacks, GLfloat **vertices, GLfloat **normals, int* nVert,const GLboolean halfCircle);
void newCircleTable(GLfloat **sint, GLfloat **cost, const int n, const GLboolean halfCircle);

//#-----Definitions of new Sphere Functions-------#

    //#-- Inner 2---#

     void newCircleTable(GLfloat **sint, GLfloat **cost, const int n, const GLboolean halfCircle)
     {
        // prototype:
        // local:

        // code:
        int i;

        /* Table size, the sign of n flips the circle direction */
        const int size = abs(n);

        /* Determine the angle between samples */
        const GLfloat angle = (halfCircle ? 1 : 2) * (GLfloat)M_PI / (GLfloat)((n == 0) ? 1 : n);

        /* Allocate memory for n samples, plus duplicate of first entry at the end */
        *sint = (GLfloat *)calloc(1, sizeof(GLfloat) * (size + 1));
        *cost = (GLfloat *)calloc(1, sizeof(GLfloat) * (size + 1));

        /* Bail out if memory allocation fails, fgError never returns */
        if (!(*sint) || !(*cost))
        {
            free(*sint);
            free(*cost);
            *sint = NULL;
            *cost = NULL;
            fprintf(gpFileSphere,"\n # Failed to allocate memory in fghCircleTable");
            return;
        }

        /* Compute cos and sin around the circle */
        (*sint)[0] = 0.0;
        (*cost)[0] = 1.0;

        for (i = 1; i < size; i++)
        {
            (*sint)[i] = (GLfloat)sin(angle * i);
            (*cost)[i] = (GLfloat)cos(angle * i);
        }

        if (halfCircle)
        {
            (*sint)[size] = 0.0f;  /* sin PI */
            (*cost)[size] = -1.0f; /* cos PI */
        }
        else
        {
            /* Last sample is duplicate of the first (sin or cos of 2 PI) */
            (*sint)[size] = (*sint)[0];
            (*cost)[size] = (*cost)[0];
        }
    }


   //#-- Inner 1---#

     void newGenerateSphere(GLfloat radius, GLint slices, GLint stacks, GLfloat **vertices, GLfloat **normals, int* nVert,const GLboolean halfCircle)
     {
        //prototype:
	    void newCircleTable(GLfloat **sint, GLfloat **cost, const int n, const GLboolean halfCircle);
        //local:
        int i, j;
        int idx = 0; /* idx into vertex/normal buffer */
        GLfloat x, y, z;

        /* Pre-computed circle */
        GLfloat *sint1, *cost1;
        GLfloat *sint2, *cost2;

        //code:

        /* number of unique vertices */
        if (slices == 0 || stacks < 2)
        {
            /* nothing to generate */
            *nVert = 0;
            return;
        }
        *nVert = slices * (stacks - 1) + 2;
        if ((*nVert) > 65535)
            /*
             * limit of glushort, thats 256*256 subdivisions, should be enough in practice. See note above
             */
           fprintf(gpFileSphere,"\n # fghGenerateSphere: too many slices or stacks requested, indices will wrap");

        /* precompute values on unit circle */
        newCircleTable(&sint1, &cost1, -slices, GL_FALSE);
        newCircleTable(&sint2, &cost2, stacks, GL_TRUE);

        /* Allocate vertex and normal buffers, bail out if memory allocation fails */
        *vertices = (GLfloat *)malloc((*nVert) * 3 * sizeof(GLfloat));
        *normals = (GLfloat *)malloc((*nVert) * 3 * sizeof(GLfloat));
        if (!(*vertices) || !(*normals))
        {
            free(*vertices);
            free(*normals);
            *vertices = NULL;
            *normals = NULL;
            fprintf(gpFileSphere,"\n # Failed to allocate memory in fghGenerateSphere");
            return;
        }

        /* top */
        (*vertices)[0] = 0.f;
        (*vertices)[1] = 0.f;
        (*vertices)[2] = radius;
        (*normals)[0] = 0.f;
        (*normals)[1] = 0.f;
        (*normals)[2] = 1.f;
        idx = 3;

        /* each stack */
        for (i = 1; i < stacks; i++)
        {
            for (j = 0; j < slices; j++, idx += 3)
            {
                x = cost1[j] * sint2[i];
                y = sint1[j] * sint2[i];
                z = cost2[i];

                (*vertices)[idx] = x * radius;
                (*vertices)[idx + 1] = y * radius;
                (*vertices)[idx + 2] = z * radius;
                (*normals)[idx] = x;
                (*normals)[idx + 1] = y;
                (*normals)[idx + 2] = z;
            }
        }

        /* bottom */
        (*vertices)[idx] = 0.f;
        (*vertices)[idx + 1] = 0.f;
        (*vertices)[idx + 2] = -radius;
        (*normals)[idx] = 0.f;
        (*normals)[idx + 1] = 0.f;
        (*normals)[idx + 2] = -1.f;

        /* Done creating vertices, release sin and cos tables */
        free(sint1);
        free(cost1);
        free(sint2);
        free(cost2);
     }


    //#-- Outer--#

    void newSphere( GLfloat radius, GLint slices, GLint stacks,GLfloat **vertices, GLfloat **normals, int* nVert,const GLboolean halfCircle,GLushort **sphereElement_New)
    {
        //prototypes:
       
	   
        //local:
        int i,j,idx ;   //, nVert;

         /* Calling Inner 1 To  Generate vertices and normals */
        newGenerateSphere(radius,slices,stacks,vertices,normals,nVert,halfCircle);

        if (nVert == 0 || !normals || !vertices)
        {
            if (normals)
                free(normals);
            if (vertices)
                free(vertices);
            /* nothing to draw */
            return;
        }
   
   
        GLushort offset;
        /* Allocate buffers for indices, bail out if memory allocation fails */
        (*sphereElement_New) = (GLushort *)malloc((slices + 1) * 2 * (stacks) * sizeof(GLushort));
        if (!(*sphereElement_New))
        {
            free(*sphereElement_New);
            fprintf(gpFileSphere, "Failed to allocate memory in fghSphere");
        }

        /* top stack */
        for (j = 0, idx = 0; j < slices; j++, idx += 2)
        {
            (*sphereElement_New)[idx] = (GLushort)(j + 1); /* 0 is top vertex, 1 is first for first stack */
            (*sphereElement_New)[idx + 1] = 0;
        }
        (*sphereElement_New)[idx] = 1; /* repeat first slice's idx for closing off shape */
        (*sphereElement_New)[idx + 1] = 0;
        idx += 2;

        /* middle stacks: */
        /* Strip indices are relative to first index belonging to strip, NOT relative to first vertex/normal pair in array */
        for (i = 0; i < stacks - 2; i++, idx += 2)
        {
            offset = (GLushort)(1 + i * slices); /* triangle_strip indices start at 1 (0 is top vertex), and we advance one stack down as we go along */
            for (j = 0; j < slices; j++, idx += 2)
            {
                 
                (*sphereElement_New)[idx] = (GLushort)(offset + j + slices);
                (*sphereElement_New)[idx + 1] = (GLushort)(offset + j);
            }
            (*sphereElement_New)[idx] = (GLushort)(offset + slices); /* repeat first slice's idx for closing off shape */
            (*sphereElement_New)[idx + 1] = offset;
        }

        /* bottom stack */
        offset = (GLushort)(1 + (stacks - 2) * slices); /* triangle_strip indices start at 1 (0 is top vertex), and we advance one stack down as we go along */
        for (j = 0; j < slices; j++, idx += 2)
        {
            // (*sphereElement_New)[idx] = (GLushort)(nVert - 1); /* zero based index, last element in array (bottom vertex)... */
            (*sphereElement_New)[idx] = (GLushort)(*nVert - 1);
            
            (*sphereElement_New)[idx + 1] = (GLushort)(offset + j);
        }
       (*sphereElement_New)[idx] = (*nVert - 1); /* repeat first slice's idx for closing off shape */
        (*sphereElement_New)[idx + 1] = offset;

        // #-----USE stripIdx array to copy it into our ELEMENTS Array----#
        /// # ----STEP  2 ----#

        iNoOfElements = idx;
        /* cleanup allocated memory */
    }

 


void unInitializeNewSphere(void)
{
    //code:
	if (Vbo_sphere_element)
	{
		glDeleteBuffers(1, &Vbo_sphere_element);
		Vbo_sphere_element = 0;
	}

	if (Vbo_sphere_normal)
	{
		glDeleteBuffers(1, &Vbo_sphere_normal);
		Vbo_sphere_normal = 0;
	}

	if (Vbo_sphere_position)
	{
		glDeleteBuffers(1, &Vbo_sphere_position);
		Vbo_sphere_position = 0;
	}

	// delete gVAO_Sphere :
	if (Vao_sphere)
	{
		glDeleteVertexArrays(1, &Vao_sphere);
		Vao_sphere = 0;
	}


}





