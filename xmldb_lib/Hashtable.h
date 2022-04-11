/*
** This file is the part of XmlDBlib project, an easy to use xmlparser written from scratch.
**
** The author disclaims copyright to this source code.  In place of
** a legal notice, here is a blessing:
**    May you share freely, never taking more than you give.
**
*/
#ifndef HASHTABLE
#define HASHTABLE
#include<memory.h>
#include "VectorInt.h"
#include "stdbool.h"
#include <stdio.h>
struct Hashtable{
    int size;       //No of hashs
    struct VectorInt* lists;
};
void free_hashtable(struct Hashtable* hashtable);
void init_hashtable(struct Hashtable* hashtable ,size_t size);
void insertid_intohashtable(struct Hashtable* HashTB,int hashno ,int  nodeId,int reference_linenotoinsert,struct VectorInt *nodeNoToLineno) ;

void removeid_fromhashtable(struct Hashtable* HashTB,int hashno ,int  nodeId,struct VectorInt *nodeNoToLineno);
int find_indexhashtable(struct Hashtable* HashTB,int hashno ,int  node_lineno, bool roof,struct VectorInt *nodeNoToLineno) ;
#endif
