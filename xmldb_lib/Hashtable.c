/*
** This file is the part of XmlDBlib project, an easy to use xmlparser written from scratch.
**
** The author disclaims copyright to this source code.  In place of
** a legal notice, here is a blessing:
**    May you share freely, never taking more than you give.
**
*/
#include "Hashtable.h"

void init_hashtable(struct Hashtable* hashtable ,size_t size){
    hashtable->lists= (struct VectorInt *)malloc(size*sizeof(struct VectorInt));
    if(hashtable->lists==NULL){
        fprintf(stderr,"\nError-Memory allocation failed");
        exit(1);
    }
    hashtable->size=size;
    for(size_t i=0;i<size;i++){
        init_VectorInt(&hashtable->lists[i],0);
    }


}

void free_hashtable(struct Hashtable* hashtable){
    for(int i=0;i<hashtable->size;i++){
        free_VectorInt(&hashtable->lists[i]);
    }
    free(hashtable->lists);
}
void insertid_intohashtable(struct Hashtable* HashTB,int hashno ,int  nodeId,int reference_linenotoinsert,struct VectorInt *nodeNoToLineno) {

    //fmt.Printf("\ninsertid_intohashtable-entry")
    struct VectorInt *Vect= &HashTB->lists[hashno];
    if (Vect->length==0 ){
        appendto_VectorInt(Vect,nodeId);
        return;
    }
    int index = Vect->length - 1;
    unsigned long lastno=Vect->length;
    while(nodeNoToLineno->items[HashTB->lists[hashno].items[index]] > reference_linenotoinsert ){
        lastno=index;
        index--;
        if(index<0){
            break;
        }
    }
    if(lastno > Vect->length - 1){
        appendto_VectorInt(Vect,nodeId);
        return;
    }
    inserto_VectorInt(Vect,lastno,nodeId);
    return;
}

void removeid_fromhashtable(struct Hashtable* HashTB,int hashno ,int  nodeId,struct VectorInt *nodeNoToLineno){
    struct VectorInt *Vect= &HashTB->lists[hashno];
   int lineno = nodeNoToLineno->items[nodeId];
   int LowLM = 0;
    int UpLM = Vect->length - 1;
    if (UpLM < 0) {
         return;
    }
    int MidLM = 0;
    int index = -1;
    while(1) {
        MidLM = (int)((LowLM + UpLM) / 2);
        if ((lineno >= nodeNoToLineno->items[Vect->items[LowLM]]) && (lineno <= nodeNoToLineno->items[Vect->items[MidLM]])) {
            UpLM = MidLM;
        } else if ((lineno > nodeNoToLineno->items[Vect->items[MidLM]]) && (lineno <= nodeNoToLineno->items[Vect->items[UpLM]])) {
            LowLM = MidLM;
        } else {
            break;
        }
        //fmt.Printf("\n%d  %d  %d", UpLM, MidLM, LowLM)
        if ((UpLM == LowLM) || (UpLM == (LowLM+1))) {
            if (Vect->items[UpLM] == nodeId) {
                index = UpLM;
                break;
            }
            if (Vect->items[LowLM] == nodeId) {
                index = LowLM;
                break;
            }
            break;
        }

    }
    if (index < 0) {
        return;
    }
    removefrom_VectorInt(Vect,index);

    return;
}

int find_indexhashtable(struct Hashtable* HashTB,int hashno ,int  node_lineno, bool roof,struct VectorInt *nodeNoToLineno) {

    struct VectorInt *Vect= &HashTB->lists[hashno];

   int LowLM = 0;
    int UpLM = Vect->length - 1;
    if (UpLM < 0) {
         return -1;
    }
    int MidLM = 0;
    int index = -1;


    while(1) {
        MidLM = (int)((LowLM + UpLM) / 2);
        if (roof) {
            if ((node_lineno >= nodeNoToLineno->items[Vect->items[LowLM]]) && (node_lineno < nodeNoToLineno->items[Vect->items[MidLM]])) {
                if (nodeNoToLineno->items[Vect->items[LowLM]] == nodeNoToLineno->items[Vect->items[MidLM]]) {
                    LowLM = MidLM;
                } else {
                    UpLM = MidLM;
                }

            } else if ((node_lineno >= nodeNoToLineno->items[Vect->items[MidLM]]) && (node_lineno <= nodeNoToLineno->items[Vect->items[UpLM]])) {
                if (nodeNoToLineno->items[Vect->items[UpLM]] == nodeNoToLineno->items[Vect->items[MidLM]]) {
                    UpLM = MidLM;
                } else {
                    LowLM = MidLM;
                }

            } else {
                break;
            }
        } else {
            if ((node_lineno >= nodeNoToLineno->items[Vect->items[LowLM]]) && (node_lineno <= nodeNoToLineno->items[Vect->items[MidLM]])) {
                if( nodeNoToLineno->items[Vect->items[LowLM]] == nodeNoToLineno->items[Vect->items[MidLM]]) {
                    LowLM = MidLM;
                } else {
                    UpLM = MidLM;
                }

            } else if ((node_lineno > nodeNoToLineno->items[Vect->items[MidLM]]) && (node_lineno <= nodeNoToLineno->items[Vect->items[UpLM]])){
                if (nodeNoToLineno->items[Vect->items[UpLM]] == nodeNoToLineno->items[Vect->items[MidLM]]) {
                    UpLM = MidLM;
                } else {
                    LowLM = MidLM;
                }

            } else {
                break;
            }
        }

        //fmt.Printf("\n%d  %d  %d", UpLM, MidLM, LowLM)
        if ((UpLM == LowLM) || (UpLM == (LowLM+1))) {
            index = UpLM;
            break;
        }

    }
    if (index < 0) {
        if (node_lineno < nodeNoToLineno->items[Vect->items[0]]) {
            index = 0;
        } else {
            index = Vect->length;
        }

    }
    return index;
}


