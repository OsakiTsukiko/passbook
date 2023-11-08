#include <stdlib.h>
#include <string.h>

#ifndef VECTOR_H
#define VECTOR_H

/*
VERY PRIMITIVE VECTOR..
*/

typedef struct _vector
{
    int len;
    void** array;
} vector;

vector* new_vector();

int vector_size(vector* vec);

void vector_push_front(vector* vec, void* element);
void vector_push_back(vector* vec, void* element);
void* vector_pop_front(vector* vec);
void* vector_pop_back(vector* vec);

void* vector_remove(vector* vec, unsigned int index);

#endif