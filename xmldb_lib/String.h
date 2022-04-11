/*
** This file is the part of XmlDBlib project, an easy to use xmlparser written from scratch.
**
** The author disclaims copyright to this source code.  In place of
** a legal notice, here is a blessing:
**    May you share freely, never taking more than you give.
**
*/
#ifndef STRING
#define STRING
#include<malloc.h>
#include<memory.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
struct String{
    size_t length;//length of vector
    size_t size;  //total capacity
    char* charbuf;
};
void init_String(struct String*v,size_t size);
struct String* create_String(char* value);
void String_Resize(struct String *vect,size_t new_size);
void free_String(struct String *vect);
void clear_String(struct String *vect);
void StringCharCpy(struct String *v,char* value);
void StringStringCpy(struct String *dest, struct String* src);
void StringCharConcat(struct String *dest, char* src);
void StringStringConcat(struct String *dest, struct String* src);
void removeSubString(struct String *src_dest, size_t from, size_t to);
void Sub_String(struct String *dest,struct String *src, size_t from, size_t to);
void ReplcSubstring(struct String *src_dest, char *SubString, char* NewString);
void TrimSpaceString(struct String *src_dest);
//struct String *CopyString(struct String *v);
void StringStringConcatpart(struct String *dest, struct String* src, size_t from, size_t to);
void StringStringCpypart(struct String *dest, struct String* src, size_t from, size_t to);
void TrimRightString(struct String *src_dest,  size_t size);
void free_StringReturn(struct String *vect);
void StringNCharConcat(struct String *dest,char* src, size_t valu_len);
int Index(struct String *string,char* substring);
#endif
