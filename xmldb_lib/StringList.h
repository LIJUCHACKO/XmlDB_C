/*
** This file is the part of XmlDBlib project, an easy to use xmlparser written from scratch.
**
** The author disclaims copyright to this source code.  In place of
** a legal notice, here is a blessing:
**    May you share freely, never taking more than you give.
**
*/
#ifndef STRINGLIST
#define STRINGLIST
#include<malloc.h>
#include<memory.h>
#include "string.h"
#include "String.h"
struct StringList{
    size_t length;  //length of vector
    size_t size;    //total capacity
    struct String* string;
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
int StringListIndexOf(struct StringList *vect,struct String *String);
#endif
