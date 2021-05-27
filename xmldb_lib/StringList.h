 
#ifndef STRINGLIST
#define STRINGLIST
#include<malloc.h>
#include<memory.h>
#include "string.h"
#include "String.h"
struct StringList{
    size_t length;
    size_t size;
    struct String* items;
};

void init_StringList(struct StringList*v,size_t size);
void StringList_Resize(struct StringList *vect,size_t new_size);
void free_StringList(struct StringList *vect);
void appendto_StringList(struct StringList *src_dest, struct String* string);
void concatenate_StringList(struct StringList *dest, struct StringList* src);
void insertInTo_StringList(struct StringList *src_dest, size_t index, struct String* string);
void removeFrom_StringList(struct StringList *src_dest, size_t index);
void String_Split(struct StringList *result ,struct String *string, char* Separator);
void clear_StringList(struct StringList *vect);
void free_StringListReturn(struct StringList *vect);
#endif
