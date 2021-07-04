
#ifndef SGMSTRINGLIST
#define SGMSTRINGLIST
#include "StringList.h"
#define SEGMENTLENGTH 2000
struct SegmentedStringList{
    size_t length;
    size_t lastSegment;
    size_t NoofSegments;
    struct StringList* Segments;
};
void init_SegmentedStringList(struct SegmentedStringList*v);
void free_SegmentedStringList(struct SegmentedStringList *vect);
void SegmentedStringList_Resize(struct SegmentedStringList *vect,size_t new_size);
void appendto_SegmentedStringList(struct SegmentedStringList *vect,struct String *string);
void removeFrom_SegmentedStringList(struct SegmentedStringList *vect,size_t index);

struct String * Valueat(struct SegmentedStringList *vect,size_t index);
void insertInTo_SegmentedStringList(struct SegmentedStringList *vect,size_t index,struct String *string);
void ModifyValueat(struct SegmentedStringList *vect,size_t index,struct String *newvalue);
#endif
