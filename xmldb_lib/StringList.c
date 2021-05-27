#include "StringList.h"

void init_StringList(struct StringList *v,size_t size){
    if(size<1){
        size=1;
    }
    v->items = malloc( size * sizeof(struct String));
    v->size=size;
    v->length=0;
}

inline void StringList_Resize(struct StringList *vect,size_t new_size){
        vect->size=new_size;
        vect->items= realloc(vect->items, new_size * sizeof(struct String));

}
void free_StringList(struct StringList *vect){
    for(size_t i=0;i<vect->length;i++){
        free_String(&vect->items[i]);
    }
    free(vect->items);
}
void free_StringListReturn(struct StringList *vect){
    for(size_t i=0;i<vect->length;i++){
        free_String(&vect->items[i]);
    }
    free(vect->items);
    free(vect);
}
void clear_StringList(struct StringList *vect){
    for(size_t i=0;i<vect->length;i++){
        free_String(&vect->items[i]);
    }
    vect->length=0;
}
void appendto_StringList(struct StringList *src_dest,struct String *string){

    init_String(&src_dest->items[src_dest->length],string->length);
    StringStringCpy(&src_dest->items[src_dest->length],string);
    src_dest->length++;

    if(src_dest->length>= src_dest->size){
            StringList_Resize( src_dest ,src_dest->size*2);
    }
}
void concatenate_StringList(struct StringList *dest,struct StringList* src){
    for(size_t i=0;i<src->length;i++){
        appendto_StringList(dest,&src->items[i]);
    }
}

void insertInTo_StringList(struct StringList *src_dest,size_t index,struct String* string){

    init_String(&src_dest->items[src_dest->length],0);
    struct String* remaining=  malloc((src_dest->length-index) * sizeof(struct String));
    memcpy(remaining, src_dest->items+index, (src_dest->length-index)*sizeof (struct String));
    src_dest->items[index].charbuf = src_dest->items[src_dest->length].charbuf;
    src_dest->items[index].size=src_dest->items[src_dest->length].size;
    StringStringCpy(&src_dest->items[index],string);
    memcpy( src_dest->items+index+1,remaining, (src_dest->length-index)*sizeof (struct String));
    free(remaining);
    src_dest->length++;
    if(src_dest->length>= src_dest->size){
        if(src_dest->size<1000){
            StringList_Resize( src_dest ,src_dest->size*2);
        }else{
            StringList_Resize( src_dest ,src_dest->size+1000);
        }
    }
}
void removeFrom_StringList(struct StringList *src_dest,size_t index){

    struct String* remaining=  malloc( sizeof(struct String));
    memcpy(remaining, src_dest->items+index, 1*sizeof (struct String));

    memmove( src_dest->items+index,src_dest->items+index+1, (src_dest->length-index-1)*sizeof (struct String));
    memcpy(src_dest->items+src_dest->length, remaining, 1*sizeof (struct String));
    free_String(&src_dest->items[src_dest->length]);
    src_dest->length--;
    free(remaining);
}

void String_Split(struct StringList *result ,struct String *string, char* Separator){
    clear_StringList(result);
    struct String buffer;
    init_String(&buffer,0);
    char *copy = (char *)malloc(string->length + 1);
    memcpy(copy, string->charbuf,string->length+1);

    char * token;
    token= strtok(copy, Separator);
    // loop through the string to extract all other tokens
    while( token != NULL ) {
        //printf( " %s\n", token ); //printing each token
        StringCharCpy(&buffer,token);
        appendto_StringList(result,&buffer);

        token = strtok(NULL, Separator);
    }
    free(copy);
    free_String(&buffer);
}
