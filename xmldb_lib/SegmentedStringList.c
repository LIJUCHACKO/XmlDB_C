#include "SegmentedStringList.h"
void init_SegmentedStringList(struct SegmentedStringList *v){

    v->Segments = malloc( 10 * sizeof(struct StringList));
    v->NoofSegments=10;
    v->lastSegment=0;
    v->length=0;
    init_StringList(&v->Segments[0],SEGMENTLENGTH);
}

void free_SegmentedStringListReturn(struct SegmentedStringList *vect){
    for(size_t i=0;i<=vect->lastSegment;i++){
        free_StringList(&vect->Segments[i]);
    }

    free(vect->Segments);
}

inline void SegmentedStringList_Resize(struct SegmentedStringList *vect,size_t new_size){
        vect->NoofSegments=new_size;
        vect->Segments= realloc(vect->Segments, new_size * sizeof(struct StringList));
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
        if((size+vect->Segments[i].length) >index){
            vect->length++;
            insertInTo_StringList(&vect->Segments[i], index-size,  string);
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
            return &vect->Segments[i].items[index-size];
        }
        size=size+vect->Segments[i].length;
        i++;
    }
    return  NULL;
}
