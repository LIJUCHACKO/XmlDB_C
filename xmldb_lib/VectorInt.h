#ifndef VECTORINT
#define VECTORINT
#include<malloc.h>
#include<memory.h>

struct VectorInt{
    int length;
    int size;
    int* items;
};

void init_VectorInt(struct VectorInt* v,int size);
void VectorInt_Resize(struct VectorInt *vect,int new_size);
void free_VectorInt(struct VectorInt *vect);
void clear_VectorInt(struct VectorInt *vect);
void appendto_VectorInt(struct VectorInt *src_dest, int value);
void concatenate_VectorInt(struct VectorInt *dest, struct VectorInt* src);
void inserto_VectorInt(struct VectorInt *src_dest, int index, int value);
void removefrom_VectorInt(struct VectorInt *src_dest, int index);
void Slice_VectorInt(struct VectorInt *Result, struct VectorInt *input, int from, int to);
void free_VectorIntReturn(struct VectorInt *vect);
#endif
