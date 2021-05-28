#include "String.h"


void init_String(struct String*v,size_t size){

    if(size<1){
        size=100;
    }
    v->charbuf = malloc(size * sizeof(char));
    v->charbuf[0]='\0';
    v->size=size;
    v->length=0;
}

struct String* create_String(char* value){
    size_t valu_len=strlen(value);
    struct String *v=malloc(sizeof (struct String));
    init_String(v,valu_len+2);
    if(v->length+valu_len+2> v->size){
        String_Resize( v ,(v->length+valu_len*2));
    }
    memcpy(v->charbuf,value,(valu_len+1)*sizeof (char));
    v->length=v->length+valu_len;
    return v;
}

inline void String_Resize(struct String *vect, size_t new_size){
     vect->size=new_size;
     vect->charbuf= realloc(vect->charbuf, new_size * sizeof(char));


}
void free_String(struct String *vect){
    free(vect->charbuf);

}
void free_StringReturn(struct String *vect){
   free(vect->charbuf);
    free(vect);
}
void clear_String(struct String *vect){
    vect->length=0;
    vect->charbuf[0]='\0';
}
void StringCharCpy(struct String *v,char* value){
    size_t valu_len=strlen(value);
    if(valu_len+1> v->size){
        String_Resize( v ,(valu_len*2)+1);
    }
    memcpy(v->charbuf,value,(valu_len+1)*sizeof (char));
    v->length=valu_len;


}

void StringStringCpy(struct String *dest,struct String* src){
    if((src->length+1)> dest->size){
        String_Resize( dest ,(src->length*2));
    }
    memcpy(dest->charbuf, src->charbuf, (src->length+1)*sizeof (char));
    dest->length=src->length;
}
void StringStringCpypart(struct String *dest,struct String* src,size_t from,size_t to){
    if((to-from+1)> dest->size){
        String_Resize( dest ,(to-from)*2);
    }
    memcpy(dest->charbuf, src->charbuf+from, (to-from)*sizeof (char));

    dest->length=to-from;
    dest->charbuf[dest->length]='\0';
}


void StringCharConcat(struct String *dest,char* src){
    size_t valu_len=strlen(src);
    if(dest->length+valu_len+1> dest->size){
        String_Resize( dest ,(dest->length+valu_len)*2);
    }
    memcpy(dest->charbuf+dest->length,src,(valu_len+1)*sizeof (char));
    dest->length=dest->length+valu_len;


}
inline void StringNCharConcat(struct String *dest,char* src, size_t valu_len){
    if(dest->length+valu_len+1> dest->size){
        String_Resize( dest ,(dest->length+valu_len)*2);
    }
    memcpy(dest->charbuf+dest->length,src,(valu_len)*sizeof (char));
    dest->length=dest->length+valu_len;
}
void StringStringConcat(struct String *dest,struct String* src){
    if((dest->length+src->length+1)> dest->size){
        String_Resize( dest ,(dest->length+src->length)*2);
    }
    memcpy(dest->charbuf+dest->length, src->charbuf, (src->length+1)*sizeof (char));
    dest->length=dest->length+src->length;
}
void StringStringConcatpart(struct String *dest,struct String* src,size_t from,size_t to){
    if((dest->length+to-from+1)> dest->size){
        String_Resize( dest ,(dest->length+1+(to-from)*2));
    }
    memcpy(dest->charbuf+dest->length, src->charbuf+from, (to-from)*sizeof (char));

    dest->length=dest->length+to-from;
    dest->charbuf[dest->length]='\0';
}


void Sub_String(struct String *dest,struct String *src, size_t from, size_t to){
    if((to-from+1)> dest->size){
        String_Resize( dest ,((to-from)*2));
    }

    if(from>src->length){
        return;
    }
    if(to>src->length){
        to=src->length;
    }
    memcpy(dest->charbuf,src->charbuf+from,(to-from)*sizeof (char));
    dest->length=to-from;
    dest->charbuf[dest->length]='\0';
}

void ReplcSubstring(struct String *src_dest,char *SubString,char* NewString){
    //*String should have enough space

    size_t newstrlen=strlen(NewString);
    size_t substrlen=strlen(SubString);
    String_Resize( src_dest ,(src_dest->length+1+newstrlen*10)*2);
    char* buffer=malloc((src_dest->length+1)*sizeof (char));
    //cpy to a buffer
    memcpy(buffer,src_dest->charbuf,src_dest->length+1);
    char* bufferptrmov=buffer;
    char *bufferptrprev=bufferptrmov;
    char* inputptr_tmp= src_dest->charbuf ;
    while((bufferptrmov=strstr(bufferptrmov, SubString))!=NULL){
        //copy till ret
        memcpy(inputptr_tmp,bufferptrprev,bufferptrmov-bufferptrprev);
        inputptr_tmp=inputptr_tmp+(bufferptrmov-bufferptrprev);
        //cpy new word
        memcpy(inputptr_tmp,NewString,newstrlen);
        inputptr_tmp=inputptr_tmp+newstrlen;
        //incre ret beyond old word
        bufferptrmov=bufferptrmov+substrlen;
        bufferptrprev=bufferptrmov;
    }
    //cpy remaining
    memcpy(inputptr_tmp,bufferptrprev,src_dest->length-(bufferptrprev- buffer));
    inputptr_tmp=inputptr_tmp+(src_dest->length-(bufferptrprev- buffer));

    src_dest->length=inputptr_tmp-src_dest->charbuf;
    src_dest->charbuf[src_dest->length]='\0';
    free(buffer);
}

void TrimSpaceString(struct String *src_dest){
    int from=0;
    int last=src_dest->length-1;

    //scan forward from begining
    while(isspace(src_dest->charbuf[from]) && (from<last)){
        from++;
    }

    //scan backward from end
    while(isspace(src_dest->charbuf[last]) && (from<=last)){
        last--;
    }
    memmove(src_dest->charbuf,src_dest->charbuf+from,last-from+1);

    src_dest->length=last-from+1;
    src_dest->charbuf[src_dest->length]='\0';
}


void removeSubString(struct String *src_dest, size_t from, size_t to){
    memmove(src_dest->charbuf+from,src_dest->charbuf+to,src_dest->length-to+1);
    src_dest->length=src_dest->length-to+from;
    src_dest->charbuf[src_dest->length]='\0';
}
void TrimRightString(struct String *src_dest,  size_t size){
    src_dest->length=src_dest->length-size;
    src_dest->charbuf[src_dest->length]='\0';
}
