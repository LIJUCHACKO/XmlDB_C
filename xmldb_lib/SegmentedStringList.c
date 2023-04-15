/*
** This file is the part of XmlDBlib project, an easy to use xmlparser written from scratch.
**
** The author disclaims copyright to this source code.  In place of
** a legal notice, here is a blessing:
**    May you share freely, never taking more than you give.
**
*/
#include "SegmentedStringList.h"
void init_SegmentedStringList(struct SegmentedStringList *v){

    v->Segments = malloc( 10 * sizeof(struct StringList));
    if(v->Segments==NULL){
        fprintf(stderr,"\nError-Memory allocation failed");
        exit(1);
    }
    v->NoofSegments=10;
    v->lastSegment=0;
    v->length=0;
    init_StringList(&v->Segments[0],SEGMENTLENGTH);
}

void free_SegmentedStringList(struct SegmentedStringList *vect){
    //lastsegment is size -1
    for(size_t i=0;i<=vect->lastSegment;i++){
        free_StringList(&vect->Segments[i]);
    }

    free(vect->Segments);
}

inline void SegmentedStringList_Resize(struct SegmentedStringList *vect,size_t new_size){
        vect->NoofSegments=new_size;
        vect->Segments= realloc(vect->Segments, new_size * sizeof(struct StringList));
        if(vect->Segments==NULL){
            fprintf(stderr,"\nError-Memory allocation failed");
            exit(1);
        }
}

static void increase_Segments(struct SegmentedStringList *vect){
    vect->lastSegment++;

    if(vect->lastSegment>= vect->NoofSegments){
          SegmentedStringList_Resize( vect ,vect->NoofSegments*2);
    }
    init_StringList(&vect->Segments[vect->lastSegment],SEGMENTLENGTH);
}

void appendto_SegmentedStringList(struct SegmentedStringList *vect,struct String *string){

    if (vect->Segments[vect->lastSegment].length>=SEGMENTLENGTH){
        increase_Segments(vect);
    }
    appendto_StringList(&vect->Segments[vect->lastSegment], string);
    vect->length++;
}
void insertInTo_SegmentedStringList(struct SegmentedStringList *vect,size_t index,struct String *string){

    size_t size=0;
    size_t i=0;
    while(i<= vect->lastSegment){
        if((size+vect->Segments[i].length) >=index){
            vect->length++;
            insertInTo_StringList(&vect->Segments[i], index-size,  string);
            if (vect->Segments[i].length>=2*SEGMENTLENGTH){
                /*dividing into 2*/
                vect->lastSegment++;
                if(vect->lastSegment>= vect->NoofSegments){
                      SegmentedStringList_Resize( vect ,vect->NoofSegments*2);
                }
                memmove(vect->Segments+index+2, vect->Segments+i+1,(vect->lastSegment-i-1)*sizeof (struct StringList));
                init_StringList(&vect->Segments[i+1],SEGMENTLENGTH);
                /*copying last half contents to next i+1 from i*/
                memmove(vect->Segments[i+1].string,vect->Segments[i].string+SEGMENTLENGTH,SEGMENTLENGTH*sizeof (struct String));
                vect->Segments[i].length=SEGMENTLENGTH;
                vect->Segments[i+1].length=SEGMENTLENGTH;

            }
            return;
        }
        size=size+vect->Segments[i].length;
        i++;
    }
}
void removeFrom_SegmentedStringList(struct SegmentedStringList *vect,size_t index){

    size_t size=0;
    size_t i=0;
    while(i<= vect->lastSegment ){
        if((size+vect->Segments[i].length) >index){
            vect->length--;
            removeFrom_StringList(&vect->Segments[i], index-size);
            return;
        }
        size=size+vect->Segments[i].length;
        i++;
    }
}
struct String * Valueat(struct SegmentedStringList *vect,size_t index){
    size_t size=0;
    size_t i=0;
    while(i<= vect->lastSegment){
        if((size+vect->Segments[i].length) >index){
           // printf("\nSegments[%ld][%ld]",i,index-size);
            return &vect->Segments[i].string[index-size];
        }
        size=size+vect->Segments[i].length;
        i++;
    }
    return  NULL;
}

void ModifyValueat(struct SegmentedStringList *vect,size_t index,struct String *newvalue){
    size_t size=0;
    size_t i=0;
    while(i<= vect->lastSegment){
        if((size+vect->Segments[i].length) >index){
           // printf("\nSegments[%ld][%ld]",i,index-size);
            StringStringCpy(&vect->Segments[i].string[index-size],newvalue);
            return;
        }
        size=size+vect->Segments[i].length;
        i++;
    }
}
