 
#ifndef HASHTABLE
#define HASHTABLE
#include<memory.h>
#include "VectorInt.h"
#include "stdbool.h"
struct Hashtable{
    int size;
    struct VectorInt* lists;
};
void free_hashtable(struct Hashtable* hashtable);
void init_hashtable(struct Hashtable* hashtable ,size_t size);
void insertid_intohashtable(struct Hashtable* HashTB,int hashno ,int  nodeId,int reference_linenotoinsert,struct VectorInt *nodeNoToLineno) ;

void removeid_fromhashtable(struct Hashtable* HashTB,int hashno ,int  nodeId,struct VectorInt *nodeNoToLineno);
int find_indexhashtable(struct Hashtable* HashTB,int hashno ,int  node_lineno, bool roof,struct VectorInt *nodeNoToLineno) ;
#endif
