/*
** This file is the part of XmlDBlib project, an easy to use xmlparser written from scratch.
**
** The author disclaims copyright to this source code.  In place of
** a legal notice, here is a blessing:
**    May you share freely, never taking more than you give.
**
*/
#ifndef VECTORINT
#define VECTORINT
#include<malloc.h>
#include<memory.h>
#include <stdio.h>
struct VectorInt{
    size_t length;
    size_t size;
    int* items;
};

void init_VectorInt(struct VectorInt* v,size_t size);
void VectorInt_Resize(struct VectorInt *vect,size_t new_size);
void free_VectorInt(struct VectorInt *vect);
void clear_VectorInt(struct VectorInt *vect);
void appendto_VectorInt(struct VectorInt *src_dest, int value);
void concatenate_VectorInt(struct VectorInt *dest, struct VectorInt* src);
void inserto_VectorInt(struct VectorInt *src_dest, size_t index, int value);
void removefrom_VectorInt(struct VectorInt *src_dest, size_t index);
void Slice_VectorInt(struct VectorInt *Result, struct VectorInt *input, size_t from, size_t to);
void free_VectorIntReturn(struct VectorInt *vect);
#endif
