#include "StringList.h"

void init_StringList(struct StringList *v,size_t size){
    if(size<1){
        size=1;
    }
    v->string = malloc( size * sizeof(struct String));
    v->size=size;
    v->length=0;
}

inline void StringList_Resize(struct StringList *vect,size_t new_size){
        vect->size=new_size;
        vect->string= realloc(vect->string, new_size * sizeof(struct String));

}
void free_StringList(struct StringList *vect){
    for(size_t i=0;i<vect->length;i++){
        free_String(&vect->string[i]);
    }
    free(vect->string);
}
void free_StringListReturn(struct StringList *vect){
    for(size_t i=0;i<vect->length;i++){
        free_String(&vect->string[i]);
    }
    free(vect->string);
    free(vect);
}
void clear_StringList(struct StringList *vect){
    for(size_t i=0;i<vect->length;i++){
        free_String(&vect->string[i]);
    }
    vect->length=0;
}
void appendto_StringList(struct StringList *src_dest,struct String *string){

    init_String(&src_dest->string[src_dest->length],string->length+1);
    StringStringCpy(&src_dest->string[src_dest->length],string);
    src_dest->length++;

    if(src_dest->length>= src_dest->size){
            StringList_Resize( src_dest ,src_dest->size*2);
    }
}
void concatenate_StringList(struct StringList *dest,struct StringList* src){
    for(size_t i=0;i<src->length;i++){
        appendto_StringList(dest,&src->string[i]);
    }
}

void insertInTo_StringList(struct StringList *src_dest,size_t index,struct String* string){
      memmove(src_dest->string+index+1,src_dest->string+index,(src_dest->length-index)*sizeof (struct String));
      init_String(&src_dest->string[index],string->length+1);
       StringStringCpy(&src_dest->string[index],string);
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
    free_String(&src_dest->string[index]);
    memmove( src_dest->string+index,src_dest->string+index+1, (src_dest->length-index-1)*sizeof (struct String));
    src_dest->length--;
}

void String_Split(struct StringList *result ,struct String *string, char* Separator){
    clear_StringList(result);
    struct String buffer;
    init_String(&buffer,0);
    struct String string_cpy;
    init_String(&string_cpy,0);
    StringStringCpy(&string_cpy,string);

    //strtok doesnot support substring as delimiter
    ReplcSubstring(&string_cpy,Separator,"^"); //hope nobody use ^
    char strtokSeperator[2];
    strcpy(strtokSeperator,"^");
    char *copy=string_cpy.charbuf;

    char * token;
    token= strtok(copy, strtokSeperator);
    size_t pos=token-copy;
    // loop through the string to extract all other tokens
    while( pos<  string->length ) {
        //printf( "- %s\n", token ); //printing each token
        StringCharCpy(&buffer,token);
        appendto_StringList(result,&buffer);

        token = strtok(NULL, strtokSeperator);
        pos=token-copy;
    }
    free(copy);
    free_String(&buffer);
}

int indexof(struct StringList *vect,struct String *String){
    for(int i=0;i<vect->length;i++){
        if(strcmp(vect->string->charbuf,String->charbuf)==0){
            return i;
        }
    }
    return -1;
}
