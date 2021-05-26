#include "VectorInt.h"

void init_VectorInt(struct VectorInt* v,int size){
    if(size<1){
        size=1;
    }
    v->items = (int*) malloc(size * sizeof(int));
    v->size=size;
    v->length=0;
    v->items[0]=-1;
}

inline void VectorInt_Resize(struct VectorInt *vect,int new_size){
        vect->size=new_size;
        vect->items= realloc(vect->items, new_size * sizeof(int));
}
void free_VectorInt(struct VectorInt *vect){
     free(vect->items);
}
void free_VectorIntReturn(struct VectorInt *vect){
    free(vect->items);
    free(vect);
}
void clear_VectorInt(struct VectorInt *vect){
    vect->length=0;
}
void appendto_VectorInt(struct VectorInt *src_dest,int value){
    src_dest->items[src_dest->length]=value;
    src_dest->length++;
    if(src_dest->length>= src_dest->size){
        VectorInt_Resize( src_dest ,src_dest->size*2);
    }
}

void concatenate_VectorInt(struct VectorInt *dest,struct VectorInt* src){
    if(dest->length+src->length>= dest->size){
        VectorInt_Resize( dest ,dest->size+(src->length)*2);
    }
    memcpy(dest->items+dest->length, src->items, src->length*sizeof (int));
    dest->length=dest->length+src->length;
}
void Slice_VectorInt(struct VectorInt *Result,struct VectorInt *input,int from,int to){
    if(from<input->length && to<=input->length){
        VectorInt_Resize(Result,to-from+1);
        memcpy(Result->items,input->items+from,(to-from)*sizeof (int));
        Result->length=to-from;
    }
}
void inserto_VectorInt(struct VectorInt *src_dest,int index,int value){
    if(src_dest->length+1>= src_dest->size){
        VectorInt_Resize( src_dest ,(src_dest->size+1)*2);
    }
    int* remaining=  malloc((src_dest->length-index) * sizeof(int));
    memcpy(remaining, src_dest->items+index, (src_dest->length-index)*sizeof (int));
    src_dest->items[index]=value;
    memcpy( src_dest->items+index+1,remaining, (src_dest->length-index)*sizeof (int));
    src_dest->length++;
    free(remaining);
}
void removefrom_VectorInt(struct VectorInt *src_dest,int index){

    int* remaining= malloc((src_dest->length-index-1) * sizeof(int));
    memcpy(remaining, src_dest->items+index+1, (src_dest->length-index-1)*sizeof (int));
    memcpy( src_dest->items+index,remaining, (src_dest->length-index-1)*sizeof (int));
    src_dest->length--;
    free(remaining);
}
