/*
** This file is the part of XmlDBlib project, an easy to use xmlparser written from scratch.
**
** The author disclaims copyright to this source code.  In place of
** a legal notice, here is a blessing:
**    May you share freely, never taking more than you give.
**
*/
#include "XmlDB.h"

static void readLines(struct String *Lines, char *path) {
    FILE *fp = fopen(path, "r");
    if (fp == NULL){
        fprintf(stderr,"\nCannot open file %s",path);
        exit(1);
        //return ;
    }
#if defined(__linux__)
    char * line = NULL;
    size_t len = 0;
    ssize_t read;
    read = getline(&line, &len, fp);
    while (read != -1) {

        size_t from=0;
        size_t linelengt=strlen(line);
        size_t last=linelengt-1;
        //scan forward from begining
        while((line[from]=='\r'||line[from]=='\n') && (from<last)){
            from++;
        }

        //scan backward from end
        while((line[last]=='\r'||line[last]=='\n')  && (from<=last)){
            last--;
        }
        line[last+1]='\0';
        StringNCharConcat(Lines,line+from,last-from+1);

        read = getline(&line, &len, fp);
    }
    free(line);
#else
    char line[3000];
    while(fgets(line,3000,fp)){
        size_t from=0;
        size_t linelengt=strlen(line);
        size_t last=linelengt-1;
        //scan forward from begining
        while((line[from]=='\r'||line[from]=='\n') && (from<last)){
            from++;
        }

        //scan backward from end
        while((line[last]=='\r'||line[last]=='\n')  && (from<=last)){
            last--;
        }
        line[last+1]='\0';
        StringNCharConcat(Lines,line+from,last-from+1);
    }
 #endif
    fclose(fp);
    return ;
}
struct VectorInt* Get_common(struct VectorInt* set1 ,struct VectorInt*  set2 )  {
    struct VectorInt* result = malloc(sizeof (struct VectorInt));
    if(result==NULL){
        fprintf(stderr,"\nError-Memory allocation failed");
        exit(1);
    }
    init_VectorInt(result,0);
    for(size_t i=0;i<set1->length;i++){
        int element1=set1->items[i];
        for(size_t j=0;j<set2->length;j++){
            int element2=set2->items[j];
            if (element1 == element2) {
                appendto_VectorInt(result, element1);
            }
        }
    }
    return result;
}


static  void ReplaceHTMLSpecialEntities(struct String* Result,struct String* input )  {
    StringStringCpy(Result,input);
    ReplcSubstring(Result, "&lt;" , "<");
    ReplcSubstring(Result, "&gt;" , ">");
    ReplcSubstring(Result, "&#xA;", "\n");
    ReplcSubstring(Result, "&quot;","\"" );
    ReplcSubstring(Result, "&lsquo;" , "‘");
    ReplcSubstring(Result, "&rsquo;", "’");
    ReplcSubstring(Result, "&tilde;", "~");
    ReplcSubstring(Result, "&ndash;" , "–");
    ReplcSubstring(Result, "&mdash;", "—" );
    ReplcSubstring(Result, "&apos;", "'");
    ReplcSubstring(Result, "&lpar;", "(");
    ReplcSubstring(Result, "&rpar;", ")");
    ReplcSubstring(Result, "&ast;", "*");
    ReplcSubstring(Result, "&plus;", "+");
    ReplcSubstring(Result, "&comma;", ",");
    ReplcSubstring(Result, "&period;", ".");
    ReplcSubstring(Result, "&sol;", "/");
    ReplcSubstring(Result, "&bsol;", "\\");
    ReplcSubstring(Result, "&colon;", ":");
    ReplcSubstring(Result, "&quest;", "?");
    ReplcSubstring(Result, "&commat;", "@");
    ReplcSubstring(Result, "&lsqb;", "[");
    ReplcSubstring(Result, "&rsqb;", "]");
    ReplcSubstring(Result, "&Hat;", "^");
    ReplcSubstring(Result, "&grave;", "`");
    ReplcSubstring(Result, "&verbar;", "|");
    ReplcSubstring(Result, "&lcub;", "{");
    ReplcSubstring(Result, "&rcub;", "}");
    ReplcSubstring(Result,"&amp;", "&");
    ReplcSubstring(Result, "&semi;", ";");//& and ; create problem

}
//libreoffice replace only &amp;  &lt; &gt;
void ReplacewithHTMLSpecialEntities(struct Database *DB,struct String* Result,struct String* input )
{
    StringStringCpy(Result,input);
    if(!DB->libreofficemod){
       ReplcSubstring(Result, ";", "$semi$");//& and ; create problem
    }
    ReplcSubstring(Result,"&", "&amp;" );
    ReplcSubstring(Result, "<", "&lt;" );
    ReplcSubstring(Result, ">", "&gt;" );

    if(!DB->libreofficemod){
    ReplcSubstring(Result, "\n", "&#xA;");
    ReplcSubstring(Result,"\"", "&quot;" );
    ReplcSubstring(Result, "‘", "&lsquo;" );
    ReplcSubstring(Result, "’", "&rsquo;");
    ReplcSubstring(Result, "~", "&tilde;");
    ReplcSubstring(Result, "–", "&ndash;" );
    ReplcSubstring(Result, "—", "&mdash;" );
    ReplcSubstring(Result, "'", "&apos;");
    ReplcSubstring(Result, "(", "&lpar;");
    ReplcSubstring(Result, ")", "&rpar;");
    ReplcSubstring(Result, "*", "&ast;");
    ReplcSubstring(Result, "+", "&plus;");
    ReplcSubstring(Result, ",", "&comma;");
    ReplcSubstring(Result, ".", "&period;");
    ReplcSubstring(Result, "/", "&sol;");
    ReplcSubstring(Result, "\\", "&bsol;");
    ReplcSubstring(Result, ":", "&colon;");
    ReplcSubstring(Result, "?", "&quest;");
    ReplcSubstring(Result, "@", "&commat;");
    ReplcSubstring(Result, "[", "&lsqb;");
    ReplcSubstring(Result, "]", "&rsqb;");
    ReplcSubstring(Result, "^", "&Hat;");
    ReplcSubstring(Result, "`", "&grave;");
    ReplcSubstring(Result, "|", "&verbar;");
    ReplcSubstring(Result, "{", "&lcub;");
    ReplcSubstring(Result, "}", "&rcub;");
    ReplcSubstring(Result, "$semi$", "&semi;");
   }
}

static bool writeLines(struct Database *DB ,char* path )  {
    FILE *fp = fopen(path, "w");
    if (fp == NULL){
        fprintf(stderr,"\nCannot open file %s",path);
        return false;
    }
    struct String buffer;
    init_String(&buffer,10);
    size_t seg=0;
    struct StringList NewLines; init_StringList(&NewLines,0);
    while(seg<= DB->global_dbLines.lastSegment){
        for(size_t i=0;i<DB->global_dbLines.Segments[seg].length;i++){
            struct String *line=&DB->global_dbLines.Segments[seg].string[i];
            clear_String(&buffer);
            StringStringCpy(&buffer,line);
            ReplcSubstring(&buffer,"<nil:node>","");
            ReplcSubstring(&buffer,"</nil:node>","");
            appendto_StringList(&NewLines,&buffer);
            //StringCharConcat(&buffer,"\n");
            //fwrite(buffer.charbuf , sizeof(char) , buffer.length , fp );
        }
        seg++;
    }
    struct StringList NewLines2; init_StringList(&NewLines2,0);
    formatxml(&NewLines2,&NewLines);
    for(size_t i=0;i<NewLines2.length;i++){
        StringCharConcat(&NewLines2.string[i],"\n");
        fwrite(NewLines2.string[i].charbuf, sizeof(char) , NewLines2.string[i].length , fp );
    }
    free_StringList(&NewLines2);
    free_StringList(&NewLines);
    free_String(&buffer);
    fclose(fp);
    return true;
}
struct String *Dump_DB(struct Database *DB){
    struct String *content=malloc(sizeof (struct String));
    if(content==NULL){
        fprintf(stderr,"\nError-Memory allocation failed");
        exit(1);
    }
    init_String(content,0);
    struct String buffer;
    init_String(&buffer,10);
    size_t seg=0;
    while(seg<= DB->global_dbLines.lastSegment){
        for(size_t i=0;i<DB->global_dbLines.Segments[seg].length;i++){
            struct String *line=&DB->global_dbLines.Segments[seg].string[i];
            clear_String(&buffer);
            StringStringCpy(&buffer,line);
            ReplcSubstring(&buffer,"<nil:node>","");
            ReplcSubstring(&buffer,"</nil:node>","");
            StringCharConcat(&buffer,"\n");
            StringStringConcat(content,&buffer);
        }
        seg++;
    }
    free_String(&buffer);
    return content;
}

void formatxml(struct StringList* newlines,struct StringList* lines){
    int level = 0;
    for(size_t i=0;i<lines->length;i++){
        struct String linestr;init_String(&linestr,10);
        StringStringCpy(&linestr,&lines->string[i]);
        TrimSpaceString(&linestr);
        char* line=linestr.charbuf;
        if ((line[0]=='<') && (line[1]=='/')) {

        } else if((line[0]=='<') && (strstr(line, "</")!=NULL)) {
            level++;
        } else if (line[0]=='<'){
            level++;
        }
        struct String newline;init_String(&newline,0);
        int i = 1;
        while(i<level) {
            StringCharConcat(&newline, "  ");
            i++;
        }
        StringStringConcat(&newline, &linestr);
        appendto_StringList(newlines,&newline);
        if(strlen(line)>2){
		if ((line[0]=='<') && (line[1]=='/'))  {
		    level--;
		} else if ((line[0]=='<') && (strstr(line, "</")!=NULL)){
		    level--;
		} else if ((line[strlen(line)-2]=='/') && (line[strlen(line)-1]=='>'))  {
		    level--;
		} else if ((line[0]=='<') && (line[1]=='!')){
		    level--;
		}
        }
        free_String(&newline);
        free_String(&linestr);
    }
}
void free_DB(struct Database* DB){
    free_String(&DB->removeattribute);
    free_String(&DB->path);

    free_hashtable(&DB->pathKeylookup);

    free_VectorInt(&DB->global_ids);
    free_VectorInt(&DB->deleted_ids);
    free_VectorInt(&DB->nodeNoToLineno);
    free_VectorInt(&DB->Nodeendlookup);

    free_SegmentedStringList(&DB->global_paths);
    free_SegmentedStringList(&DB->global_dbLines);
    free_SegmentedStringList(&DB->global_values);
    free_SegmentedStringList(&DB->global_attributes);

    free(DB);
}
void free_compare_path_Result(struct compare_path_Result * v){
    free_StringList(&v->label);
    free_StringList(&v->value);
    free(v);
}
static struct compare_path_Result * compare_path(struct String* current_path ,struct String* reference_path )  {
    //multiple /../ is not supported
    struct compare_path_Result* Result=malloc(sizeof(struct compare_path_Result));
    if(Result==NULL){
        fprintf(stderr,"\nError-Memory allocation failed");
        exit(1);
    }
    struct StringList ref_pathParts;init_StringList(&ref_pathParts,0);
    struct StringList cur_pathParts;init_StringList(&cur_pathParts,0);
    String_Split(&ref_pathParts,reference_path, "/");
    String_Split(&cur_pathParts,current_path, "/");
    int len_cur_pathParts = cur_pathParts.length;
    int len_ref_pathParts = ref_pathParts.length;
    int cur_pathPartindex = 0;
    int ref_pathPartindex = 0;
    bool skipoccured = false;
    init_StringList(&Result->label,1) ;
    init_StringList(&Result->value,1) ;
    while(1) {
        if (cur_pathPartindex >= len_cur_pathParts) {
            break;
        }
        if (ref_pathPartindex >= len_ref_pathParts) {
            if (skipoccured) {
                Result->status=true;
                return Result;
            }
            break;
        }
        char* refpathpart=ref_pathParts.string[ref_pathPartindex].charbuf;
        if((strstr(refpathpart, "<") != NULL) &&  (strstr(refpathpart, ">") != NULL)){
            if (skipoccured) {
                //no of remaining parts are matching then
                if ((cur_pathParts.length - cur_pathPartindex) > (ref_pathParts.length - ref_pathPartindex)) {
                    cur_pathPartindex++;
                    continue;
                } else {

                }
            }
            appendto_StringList(&Result->label,&ref_pathParts.string[ref_pathPartindex] );
            appendto_StringList(&Result->value,&cur_pathParts.string[cur_pathPartindex] );
            cur_pathPartindex++;
            ref_pathPartindex++;
            skipoccured = false;
            continue;
        } else if (strstr(refpathpart, "..") != NULL){
            skipoccured = true;
            ref_pathPartindex++;
            continue;
        }
        if(strcmp(cur_pathParts.string[cur_pathPartindex].charbuf,refpathpart)==0){

            if(skipoccured){
                //and also if remaining no of parts are same
                //to handle ../<x>/node
                // /../df/dfd/df/     /df/ df/dfdf/ d/
                //if node names are repeating down the line
                if( (len_cur_pathParts-cur_pathPartindex) == (len_ref_pathParts-ref_pathPartindex)){
                    skipoccured = false;
                    ref_pathPartindex++;
                }
            }else{
                ref_pathPartindex++;
            }
             cur_pathPartindex++;
        } else {
            if( skipoccured ){
                cur_pathPartindex++;
                continue;
            } else {
                break;
            }
        }

    }
    free_StringList(&ref_pathParts);
    free_StringList(&cur_pathParts);
    if ((cur_pathPartindex == len_cur_pathParts) && (ref_pathPartindex == len_ref_pathParts)) {
        //return label, value, true
        Result->status=true;
        return Result;
    } else {
        //return label, value, false
        Result->status=false;
        return Result;
    }
}
static bool isParentPath(struct String* parentp,struct String* nodep )  {
    if (nodep->length >= parentp->length) {
        struct String nodep0_lenparentp;init_String(&nodep0_lenparentp,0);
        Sub_String(&nodep0_lenparentp,nodep,0,parentp->length);
        if (strcmp(nodep0_lenparentp.charbuf , parentp->charbuf)==0) {
            if (strcmp(nodep->charbuf, parentp->charbuf) == 0 ){
                free_String(&nodep0_lenparentp);
                return true;
            } else if( nodep->length > parentp->length) {
                struct String nodep0_lenparentp1;init_String(&nodep0_lenparentp1,0);
                struct String parentp1;init_String(&parentp1,0);
                Sub_String(&nodep0_lenparentp1,nodep,0,parentp->length+1);
                StringStringCpy(&parentp1,parentp);
                StringCharConcat(&parentp1,"/");
                if (strcmp(nodep0_lenparentp1.charbuf, parentp1.charbuf)==0) {
                    free_String(&nodep0_lenparentp);
                    free_String(&parentp1);
                    free_String(&nodep0_lenparentp1);
                    return true;
                }
                free_String(&parentp1);
                free_String(&nodep0_lenparentp1);
            }

        }
        free_String(&nodep0_lenparentp);
    }
    return false;
}
static int stringtono(struct Database *DB, struct String* line )  {
    int hash = 0;
    char* charbuf=line->charbuf;
    for (size_t i=0;i< line->length;i++) {
        hash = hash + (hash<<5) + ((int)charbuf[i])*i;
        hash = hash % DB->maxHashValue;
    }
    return hash;
}

void free_suspectedLinenos_Result(struct suspectedLinenos_Result * v){
    free_VectorInt(&v->suspectedLineStarts);
    free_VectorInt(&v->suspectedLineEnds);
    free(v);
}
static struct suspectedLinenos_Result *suspectedLinenos(struct Database *DB, struct String *path, int lowerbound , int upperbound) {
    struct suspectedLinenos_Result* Result=malloc(sizeof(struct suspectedLinenos_Result));
    if(Result==NULL){
        fprintf(stderr,"\nError-Memory allocation failed");
        exit(1);
    }
    struct StringList pathParts  ; init_StringList(&pathParts,0);
    String_Split(&pathParts,path, "/");
    struct VectorInt NodeNos ; init_VectorInt(&NodeNos,0);
    init_VectorInt(&Result->suspectedLineStarts,0);
    init_VectorInt(&Result->suspectedLineEnds,0);
    int SearchtillEnd = 0;
    int index = pathParts.length - 1;
    while(index >= 0) {
        //search using last available path /x/y/.. then use y
        //printf("\nindex -%d ",index);
        struct String *part = &pathParts.string[index];
        //printf("\n%s keypart %s", path, part);
        if ((strstr(part->charbuf, "<")!=NULL) || (strstr(part->charbuf, "..")!=NULL)){
            SearchtillEnd = 1;
        } else {
            int hashno = stringtono(DB, part);
            int from = find_indexhashtable(&DB->pathKeylookup, hashno, lowerbound, false,&DB->nodeNoToLineno);
            int to = find_indexhashtable(&DB->pathKeylookup, hashno, upperbound, true,&DB->nodeNoToLineno);
            if (from >= 0 && to >= 0) {
                struct VectorInt hashlineids = DB->pathKeylookup.lists[hashno];
                struct VectorInt Slice  ; init_VectorInt(&Slice,0);
                Slice_VectorInt(&Slice,&hashlineids,from,to);
                concatenate_VectorInt(&NodeNos,&Slice);
                free_VectorInt(&Slice);
             }
            break;
        }
        index--;
    }
    //printf("\nNodeNos ")
    //fmt.Println(NodeNos)
    if (NodeNos.length == 0) {
        appendto_VectorInt(&Result->suspectedLineStarts,0);
        appendto_VectorInt(&Result->suspectedLineEnds,DB->global_dbLines.length);
        free_VectorInt(&NodeNos);
        free_StringList(&pathParts);
        return Result;//suspectedLineStarts, suspectedLineEnds //1
    } else {
        for(size_t i=0;i<NodeNos.length;i++)
        {
            int node=NodeNos.items[i];
            appendto_VectorInt(&Result->suspectedLineStarts,DB->nodeNoToLineno.items[node]);
            //printf("\n DB->nodeNoToLineno[node] %d ", DB->nodeNoToLineno[node])
            if (SearchtillEnd == 1) {
                appendto_VectorInt(&Result->suspectedLineEnds,DB->nodeNoToLineno.items[DB->Nodeendlookup.items[node]]+1);
                //printf("\n DB->suspectedLineEnds[i] %d ", DB->suspectedLineEnds[i])
            } else {
                appendto_VectorInt(&Result->suspectedLineEnds,DB->nodeNoToLineno.items[node]);
            }
        }
    }
    free_VectorInt(&NodeNos);
    free_StringList(&pathParts);
    return Result;//suspectedLineStarts, suspectedLineEnds //1
}
static void updateNodenoLineMap(struct Database *DB ,int fromLine ) {
    int lineno = fromLine;
    if (lineno < 0) {
            lineno = 0;
    }
    while ((size_t)lineno < DB->global_dbLines.length) {
        int id = DB->global_ids.items[lineno];
        if (id >= 0 ){
            DB->nodeNoToLineno.items[id] = lineno;
        }

        lineno++;
    }
    DB->WriteLock = false;
}
int NodeLine(struct Database *DB, int nodeId )  {
    while( DB->WriteLock ){
        fprintf(stderr,"Waiting for WriteLock-NodeLine\n");
    }
    int lineno = DB->nodeNoToLineno.items[nodeId];
    if (lineno < 0 ){
        fprintf(stderr,"NodeLine-:Warning :node  doesnot exist-%d\n",nodeId);
        lineno = -1;
    }
    return lineno;
}
int NodeEnd(struct Database *DB, int nodeId )  {
    while(DB->WriteLock) {
        fprintf(stderr,"Waiting for WriteLock-NodeEnd\n");
    }
    int lineno = DB->nodeNoToLineno.items[nodeId];
    if (lineno < 0) {
        fprintf(stderr,"NodeEnd-:Warning :node  doesnot exist-%d\n",nodeId);
        return -1;
    }

    if( DB->Nodeendlookup.items[nodeId] >= 0 ){

        lineno = DB->nodeNoToLineno.items[DB->Nodeendlookup.items[nodeId]] + 1;

    } else {
        fprintf(stderr,"NodeEnd-:Warning :node  doesnot exist-%d\n",nodeId);
        lineno = -1;
    }
    return lineno;

}
static void update_path(struct Database *DB ,struct String* NodeName ,int mode  ){

    //1.Add 2.Add and Remove 3.Remove
    if (DB->path.length > 3) {
        if (( DB->path.charbuf[DB->path.length-2]=='/') &&  (DB->path.charbuf[ DB->path.length-1]=='~')){
            TrimRightString(&DB->path,2);
        }
    }
    if (DB->removeattribute.length > 0) {
        struct String path_end;init_String(&path_end,0);
        Sub_String(&path_end,&DB->path,DB->path.length-DB->removeattribute.length,DB->path.length);
        fflush(stdout);
        if (strcmp(path_end.charbuf, DB->removeattribute.charbuf)==0) {
            TrimRightString(&DB->path,DB->removeattribute.length);
            clear_String(&DB->removeattribute);
        }
        free_String(&path_end);
    }
    if( mode >= 2) {
        StringCharCpy(&DB->removeattribute,"/");
        StringStringConcat(&DB->removeattribute,NodeName);
    }
    if (mode <= 2 ){
        StringCharConcat(&DB->path,"/");
        StringStringConcat(&DB->path,NodeName);
    }

    if (mode == 3) {
        struct String path_end;init_String(&path_end,0);
        Sub_String(&path_end,&DB->path,DB->path.length-NodeName->length,DB->path.length);
        if(strcmp(path_end.charbuf,NodeName->charbuf)!=0){
            fprintf(stderr,"xml is corrupt\n");
            exit(1);
        }

        StringCharConcat(&DB->path,"/~");
    }
    //printf(" path- %s   \n", DB->path.charbuf);
}

static int fill_DBdata(struct Database *DB, struct String* dbline,struct String* value ,struct String* attribute ,struct String* NodeName ,int mode ) {
    //printf("dbline- %s,value- %s,attribute- %s,NodeName- %s, mode- %d\n",dbline->charbuf,value->charbuf,attribute->charbuf,NodeName->charbuf,mode);
    update_path(DB, NodeName, mode);
    if ((NodeName->charbuf[0] == '!') && (DB->global_lineLastUniqueid == 0)) {
        DB->global_lineLastUniqueid = -1;
    }
    int unique_id = DB->global_lineLastUniqueid;
    if (DB->startindex < 0 ){
        //printf("\n%s %s %d %s %s", DB->path, dbline, unique_id, value, attribute)
        appendto_SegmentedStringList(&DB->global_dbLines,dbline);
        appendto_SegmentedStringList(&DB->global_values,value);
        appendto_SegmentedStringList(&DB->global_attributes,attribute);
        appendto_SegmentedStringList(&DB->global_paths,&DB->path);

        appendto_VectorInt(&DB->global_ids,unique_id);


        DB->global_lineLastUniqueid++;
        if (DB->global_lineLastUniqueid >= DB->maxInt ){
            fprintf(stderr,"load_db: Total no. of Uniqueid>= DB->MaxNooflines, Please increase DB->MaxNooflines before loading db");
            exit(1);
        }
    } else {

        if (DB->retainid >= 0 ){
            unique_id = DB->retainid;
            DB->retainid = -1;
        } else {
            if (DB->global_lineLastUniqueid >= DB->maxInt) {
                if (DB->deleted_ids.length > 0) {
                    unique_id = DB->deleted_ids.items[0];
                    removefrom_VectorInt(&DB->deleted_ids,0);
                } else {
                    fprintf(stderr,"InsertAtLine: Total no. of Uniqueid>= DB->maxInt, Please increase maxNoofLines in init_Database(int maxNoofLines);");
                    exit(1);
                }
            }
        }
        insertInTo_SegmentedStringList(&DB->global_dbLines,DB->startindex,dbline);
        insertInTo_SegmentedStringList(&DB->global_values,DB->startindex,value);
        insertInTo_SegmentedStringList(&DB->global_attributes,DB->startindex,attribute);
        insertInTo_SegmentedStringList(&DB->global_paths,DB->startindex,&DB->path);
        inserto_VectorInt(&DB->global_ids,DB->startindex,unique_id);

        if (DB->Debug_enabled ){
            printf("insertatline :Inserting New Node %d\n", unique_id);
        }
        DB->startindex++;
        if ((DB->global_lineLastUniqueid < DB->maxInt) && (unique_id == DB->global_lineLastUniqueid)) {
            DB->global_lineLastUniqueid++;
            if (DB->global_lineLastUniqueid >= DB->maxInt ){
                fprintf(stderr,"load_db: Total no. of Uniqueid>= DB->MaxNooflines, Please increase  maxNoofLines in init_Database(int maxNoofLines);");
                exit(1);
            }
        }
    }
    if (unique_id >= 0 ){
        if (mode < 3 ){
            int Node_hash = stringtono(DB, NodeName);
            DB->nodeNoToLineno.items[unique_id] = DB->reference_linenotoinsert;//will be updated by updateNodenoLineMap()
            insertid_intohashtable(&DB->pathKeylookup, Node_hash, unique_id,DB->reference_linenotoinsert,&DB->nodeNoToLineno);
        }
        if (mode == 1 ){
            DB->pathIdStack[DB->pathIdStack_index] = unique_id;
            DB->pathIdStack_index++;
        } else if (mode == 2) {
            DB->Nodeendlookup.items[unique_id] = unique_id;
        } else if (mode == 3) {
            DB->pathIdStack_index--;
            DB->Nodeendlookup.items[DB->pathIdStack[DB->pathIdStack_index]] = unique_id;

        }
    }
    clear_String(dbline);
    clear_String(value);
    clear_String(attribute );
    clear_String( NodeName);
    //printf("\n%s %d %d %d", DB->path, DB->pathIdStack_index, unique_id, mode)
    return unique_id;
}

static void parseAndLoadXml(struct VectorInt *nodes,struct Database *DB ,struct String* contentStr )  {

    DB->WriteLock = true;
    struct String nodeStart;init_String(&nodeStart,0);
    struct String attributebuffer;init_String(&attributebuffer,0);
    struct String valuebuffer ;init_String(&valuebuffer,0);
    struct String NodeName;init_String(&NodeName,0);
    bool nodeEnded = false;
    bool CommentStarted = false;
    bool Comment2Started = false;
    bool xmldeclarationStarted = false;
    bool CDATAStarted = false;

    size_t lastindex = 0;
    size_t index = 0;

    struct String buffer1;init_String(&buffer1,0);
    struct String buffer2;init_String(&buffer2,0);
    struct String buffer3;init_String(&buffer3,0);
    struct String buffer4;init_String(&buffer4,0);
    char* content=contentStr->charbuf;

    while(index < contentStr->length) {
        //printf("\n%ld/%ld ",index,strlen(content));
        if (content[index] == '<') {
            if (!CommentStarted && !CDATAStarted && !xmldeclarationStarted) {
                if (content[index+1] != '/') {
                    if (nodeStart.length > 0) {
                        //////
                        ReplcSubstring(&attributebuffer,"\"\"", "\"");
                        int node = fill_DBdata(DB, &nodeStart, &valuebuffer, &attributebuffer, &NodeName, 1);
                        if (DB->startindex >= 0) {
                            appendto_VectorInt(nodes,node);
                        }
                        /////
                    }


                    StringStringCpypart(&buffer2,contentStr,lastindex,index);
                    StringStringCpy(&buffer4,&buffer2);
                    TrimSpaceString(&buffer2);
                    if (buffer2.length > 0) {
                        StringCharCpy(&nodeStart,"<nil:node>");
                        StringStringConcatpart(&nodeStart,contentStr,lastindex,index);
                        StringCharConcat(&nodeStart,"</nil:node>");
                        StringCharCpy(&attributebuffer,"");
                        StringCharCpy(&NodeName, "nil:node");
                        ////
                        //ReplcSubstring(&attributebuffer,"\"\"", "\"");
                        int node = fill_DBdata(DB, &nodeStart, &buffer4, &attributebuffer, &NodeName, 2);
                        if (DB->startindex >= 0) {
                            appendto_VectorInt(nodes,node);
                        }
                        ////
                    }

                    lastindex = index;
                    nodeEnded = false;
                }
                if (content[index+1] == '!') {
                    //comparestringForward(line, "<![CDATA[")
                    nodeEnded = false;
                    if( content[index+2] == '[') {
                        lastindex = index;
                        clear_String(&nodeStart);
                        CDATAStarted = true;
                        lastindex = index;
                    } else if( content[index+2] == '-') {
                        CommentStarted = true;
                        lastindex = index;
                    } else {
                        Comment2Started = true;
                        lastindex = index;
                    }
                } else if (content[index+1] == '?' ){
                    //comparestringForward(line, "<?")
                    nodeEnded = false;
                    xmldeclarationStarted = true;
                    lastindex = index;
                } else if (content[index+1] == '/') {
                    //comparestringForward(line, "</")
                    nodeEnded = true;

                    StringStringCpypart(&buffer2,contentStr,lastindex,index);
                    StringStringCpy(&buffer4,&buffer2);
                    TrimSpaceString(&buffer2);
                    if ( nodeStart.length > 0) {
                        StringStringConcatpart(&nodeStart,contentStr,lastindex,index);
                        StringStringConcat(&valuebuffer,&buffer4);

                        lastindex = index;
                    } else {

                        if (buffer2.length > 0) {
                            StringCharConcat( &nodeStart,"<nil:node>");
                            StringStringConcatpart(  &nodeStart,contentStr,lastindex,index);
                            StringCharConcat(  &nodeStart,"</nil:node>");
                            StringCharCpy(&attributebuffer,"");
                            StringCharCpy(&NodeName, "nil:node");
                            ////
                            //ReplcSubstring(&attributebuffer,"\"\"", "\"");
                            int node = fill_DBdata(DB, &nodeStart, &buffer4, &attributebuffer, &NodeName, 2);
                            if (DB->startindex >= 0 ){
                                appendto_VectorInt(nodes,node);
                            }
                            ////

                        }
                        lastindex = index;
                    }
                } else {

                }
            }
        }
        if (content[index] == '>') {
            if (CommentStarted) {
                if ((content[index-1] == '-' ) && (content[index-2] == '-' )) {
                    StringStringCpypart(&buffer1,contentStr,lastindex,index+1);
                    StringStringCpy(&buffer3,&buffer1);
                    TrimSpaceString(&buffer3);
                    //comparestringBackward(line, "-->")
                    CommentStarted = false;
                    lastindex = index + 1;
                    if (buffer3.length > 0) {
                        StringCharCpy(&NodeName,"!COMMENT!");
                        ////
                        ReplcSubstring(&attributebuffer,"\"\"", "\"");
                        int node = fill_DBdata(DB, &buffer1, &valuebuffer, &attributebuffer, &NodeName, 2);
                        if (DB->startindex >= 0) {
                            appendto_VectorInt(nodes,node);
                        }
                        ////
                    }

                }
            } else if (CDATAStarted) {
                //comparestringBackward(line, "]]>")
                if ((content[index-1] == ']') && (content[index-2] == ']')) {
                    StringStringCpypart(&buffer1,contentStr,lastindex,index+1);
                    StringStringCpy(&buffer3,&buffer1);
                    TrimSpaceString(&buffer3);
                    lastindex = index + 1;
                    if (buffer3.length > 0 ){
                        StringCharCpy(&NodeName,"!CDATA!");
                        ////
                        ReplcSubstring(&attributebuffer,"\"\"", "\"");
                        int node = fill_DBdata(DB, &buffer1, &valuebuffer, &attributebuffer, &NodeName, 2);
                        if ( DB->startindex >= 0 ){
                            appendto_VectorInt(nodes,node);
                        }
                        ////
                    }
                    CDATAStarted = false;
                }
            } else if (xmldeclarationStarted) {
                //comparestringBackward(line, "?>"
                if( content[index-1] == '?' ){
                    StringStringCpypart(&buffer1,contentStr,lastindex,index+1);
                    StringStringCpy(&buffer3,&buffer1);
                    TrimSpaceString(&buffer3);
                    xmldeclarationStarted = false;
                    lastindex = index + 1;
                    if (buffer3.length > 0) {
                        ////
                        /// \brief node
                        StringCharCpy(&NodeName,"!XMLDECL!");
                        ReplcSubstring(&attributebuffer,"\"\"", "\"");
                        int node = fill_DBdata(DB, &buffer1, &valuebuffer, &attributebuffer, &NodeName, 2);
                        if ( DB->startindex >= 0 ){
                            appendto_VectorInt(nodes,node);
                        }
                        ////
                    }
                }

            } else if( Comment2Started) {
                StringStringCpypart(&buffer1,contentStr,lastindex,index+1);
                StringStringCpy(&buffer3,&buffer1);
                TrimSpaceString(&buffer3);
                Comment2Started = false;
                lastindex = index + 1;
                if (buffer3.length > 0 ){
                    StringCharCpy(&NodeName,"!COMMENT2!");
                    ///
                    ReplcSubstring(&attributebuffer,"\"\"", "\"");
                    int node = fill_DBdata(DB,&buffer1, &valuebuffer, &attributebuffer, &NodeName, 2);
                    if ( DB->startindex >= 0 ){
                        appendto_VectorInt(nodes,node);
                    }
                    ////
                }
            } else {

                //extract attribute
                if (content[index-1] == '/') {
                    //if comparestringBackward(line, "/>", index) {
                    StringStringConcatpart(&nodeStart,contentStr,lastindex,index+1);
                    StringStringCpypart(&buffer2,contentStr,lastindex+1,index-1);
                    TrimSpaceString(&buffer2);
                    struct StringList parts; init_StringList(&parts,0);
                    String_Split(&parts,&buffer2,"\" ");
                    clear_String(&attributebuffer);
                    for (size_t partind=0;partind<parts.length;partind++) {
                        struct String *part=&parts.string[partind];
                        TrimSpaceString(part);
                        if (partind > 0){
                            if (part->length > 0) {
                                if (attributebuffer.length > 1) {
                                    StringCharConcat(&attributebuffer,"||");
                                }
                                StringStringConcat(&attributebuffer,part);
                                StringCharConcat(&attributebuffer,"\"");
                            }
                        } else {
                            struct StringList subparts; init_StringList(&subparts,0);
                            String_Split(&subparts,part," ");
                            if (subparts.length > 1 ){
                                StringStringCpypart(&attributebuffer,part,subparts.string[0].length,part->length);
                                TrimSpaceString(&attributebuffer);
                                //StringStringConcat(&attributebuffer,&subparts.string[1]);
                                StringCharConcat(&attributebuffer,"\"");
                            }
                            StringStringCpy(&NodeName,&subparts.string[0]);
                            free_StringList(&subparts);
                        }
                    }
                    lastindex = index + 1;
                    ///
                    ReplcSubstring(&attributebuffer,"\"\"", "\"");
                    int node = fill_DBdata(DB, &nodeStart, &valuebuffer, &attributebuffer, &NodeName, 2);
                    if ( DB->startindex >= 0 ){
                        appendto_VectorInt(nodes,node);
                    }
                    ///
                    nodeEnded = false;
                    free_StringList(&parts);
                } else {
                    //comparestringBackward(line, ">", index) {
                    if (nodeEnded ){

                        StringStringCpypart(&buffer2,contentStr,lastindex+2,index);
                        TrimSpaceString(&buffer2);
                        ///
                        if (nodeStart.length > 0 ){
                            StringStringConcatpart(&nodeStart,contentStr,lastindex,index+1);
                            ReplcSubstring(&attributebuffer,"\"\"", "\"");
                            int node = fill_DBdata(DB, &nodeStart , &valuebuffer , &attributebuffer , &buffer2, 2);
                            if ( DB->startindex >= 0 ){
                                appendto_VectorInt(nodes,node);
                            }
                        } else {
                            StringStringCpypart(&buffer3,contentStr,lastindex,index+1);
                            ReplcSubstring(&attributebuffer,"\"\"", "\"");
                            int node = fill_DBdata(DB, &buffer3, &valuebuffer, &attributebuffer , &buffer2, 3);
                            if (DB->startindex >= 0) {
                                appendto_VectorInt(nodes,node);
                            }
                        }

                        ////
                        clear_String(&nodeStart);
                        nodeEnded = false;
                    } else {

                        StringStringCpypart(&nodeStart,contentStr,lastindex,index+1);
                        StringStringCpypart(&buffer2,contentStr,lastindex+1,index);
                        TrimSpaceString(&buffer2);
                        clear_String(&attributebuffer);
                        struct StringList parts; init_StringList(&parts,0);
                        String_Split(&parts,&buffer2,"\" ");
                        for (size_t partind=0;partind<parts.length;partind++) {
                            struct String *part=&parts.string[partind];
                            TrimSpaceString(part);
                            if (partind > 0){
                                if (part->length > 0) {
                                    if (attributebuffer.length > 1) {
                                        StringCharConcat(&attributebuffer,"||");
                                    }
                                    StringStringConcat(&attributebuffer,part);
                                    StringCharConcat(&attributebuffer,"\"");
                                }
                            } else {
                                struct StringList subparts; init_StringList(&subparts,0);
                                String_Split(&subparts,part," ");
                                if (subparts.length > 1 ){
                                    StringStringCpypart(&attributebuffer,part,subparts.string[0].length,part->length);
                                    TrimSpaceString(&attributebuffer);
                                    //StringStringConcat(&attributebuffer,&subparts.string[1]);
                                    StringCharConcat(&attributebuffer,"\"");
                                }
                                StringStringCpy(&NodeName,&subparts.string[0]);
                                free_StringList(&subparts);
                            }
                        }
                        free_StringList(&parts);
                    }
                    lastindex = index + 1;
                }
            }
        }
        index++;
    }
    struct String contentlast; init_String(&contentlast,0);
    Sub_String(&contentlast,contentStr,lastindex,index);
    TrimSpaceString(&contentlast);
    if( contentlast.length > 0 ){
        fprintf(stderr,"xml is corrupt\n");
        exit(1);
    }
    free_String(&contentlast);
    if (CommentStarted || xmldeclarationStarted || Comment2Started || CDATAStarted) {
        fprintf(stderr,"xml is corrupt\n");
        exit(1);
    }
    TrimSpaceString(&nodeStart);
    if( nodeStart.length > 0 ){
        fprintf(stderr,"xml is corrupt\n");
        exit(1);
    }
    if(DB->Nodeendlookup.items[0]<0){
        fprintf(stderr,"xml is corrupt\n");
        exit(1);
    }
    free_String(&buffer1);
    free_String(&buffer2);
    free_String(&buffer3);
    free_String(&buffer4);
    free_String(&nodeStart);
    free_String(&attributebuffer);
    free_String(&valuebuffer);
    free_String(&NodeName);
}

/*memory allocation is also done for you*/
struct Database* init_Database(int maxNoofLines){
    struct Database* DB=malloc(sizeof(struct Database));
    if(DB==NULL){
        fprintf(stderr,"\nError-Memory allocation failed");
        exit(1);
    }
    if (maxNoofLines < 99999) {
        maxNoofLines = 99999;
    }
    DB->Debug_enabled=false;
    DB->libreofficemod=false;
    DB->maxInt = maxNoofLines;
    init_VectorInt(&DB->deleted_ids,10);
    init_VectorInt(&DB->nodeNoToLineno, DB->maxInt);/*fixed size*/
    DB->maxHashValue=97343;
    init_hashtable(&DB->pathKeylookup, DB->maxHashValue);/*fixed size*/
    init_VectorInt(&DB->Nodeendlookup, DB->maxInt);/*fixed size*/
    DB->startindex = -1;
    DB->retainid = -1;
    DB->pathIdStack_index = 0;
    init_VectorInt(&DB->global_ids,DB->maxInt);
    init_SegmentedStringList(&DB->global_paths );
    init_SegmentedStringList(&DB->global_attributes );
    init_SegmentedStringList(&DB->global_values );
    init_SegmentedStringList(&DB->global_dbLines );
    DB->global_lineLastUniqueid = 0;
    init_String(&DB->path,0);
    init_String(&DB->removeattribute,0);
    return DB;
}

static void load_xmlstring(struct Database *DB ,struct String* content ) {

    DB->reference_linenotoinsert = 0;
    struct VectorInt nodes; init_VectorInt(&nodes,0);
    parseAndLoadXml(&nodes,DB, content);
    //printf("\nparseAndLoadXml");
    //fflush(stdout);
    if (DB->Debug_enabled) {
        printf("load_db : over\n");
    }

    updateNodenoLineMap(DB, 0);
    //fmt.Println(DB->global_ids)
    if (DB->Debug_enabled ){
        printf("load_db :xml db loaded\n No of nodes-%d\n", DB->global_lineLastUniqueid);

        for(size_t i=0;i<DB->global_dbLines.length;i++ ) {
            char *line=Valueat(&DB->global_dbLines,i)->charbuf;
            int nodeend = 0;
            int nodebeg = 0;
            if (DB->global_ids.items[i] >= 0) {
                nodebeg = DB->nodeNoToLineno.items[DB->global_ids.items[i]];
                nodeend = DB->nodeNoToLineno.items[DB->Nodeendlookup.items[DB->global_ids.items[i]]] + 1;
            }

            printf("\n path- %s  line- %s  nodeid-%d nodebeg-%d nodeend-%d", Valueat(&DB->global_paths,i)->charbuf, line, DB->global_ids.items[i], nodebeg, nodeend);
            printf("\n value- %s attribute-%s", Valueat(&DB->global_values,i)->charbuf, Valueat(&DB->global_attributes,i)->charbuf);
            //printf("\n value- %ld attribute-%ld", DB->global_values->items[i]->length, DB->global_attributes->items[i]->length);
        }
    }
    free_VectorInt(&nodes);

}
bool SaveAs_DB(struct Database *DB, char *filename ) {
    while( DB->WriteLock) {
        fprintf(stderr,"Waiting for WriteLock-Save_DB\n");
    }

    bool status= writeLines(DB, filename);
    if (!status) {
        fprintf(stderr,"Cannot save db  : %s\n", filename);

    }
    return status;
}
bool Save_DB(struct Database *DB) {
    while( DB->WriteLock) {
        fprintf(stderr,"Waiting for WriteLock-Save_DB\n");
    }
    if (strlen(DB->filename) == 0) {
        fprintf(stderr,"Filename not specified\n");
        return false;
    }
    bool status= writeLines(DB, DB->filename);
    if (!status) {
        fprintf(stderr,"Cannot save db  : %s\n", DB->filename);

    }
    return status;
}
void Load_dbcontent(struct Database *DB,struct StringList *xmllines) {
    /* xmllines should not contain /n /r*/
    struct String lines;init_String(&lines,2000);
    for(size_t i=0;i<xmllines->length;i++) {
        StringStringConcat(&lines,&xmllines->string[i]);

    }
    if (lines.length < 2000) {
        if (!validatexml(&lines)) {
            fprintf(stderr,"\n%s\n",lines.charbuf);
            fprintf(stderr,"Load_dbcontent-XML not valid ,DB not loaded");
            free_String(&lines);
            return;
        }
    }

    load_xmlstring(DB, &lines);
    free_String(&lines);
}
bool Load_db(struct Database *DB , char* filename) {
    strcpy(DB->filename ,filename);
    struct String lines;init_String(&lines,2000);
    readLines(&lines,filename);
    //printf("\nRead lines");
    //fflush(stdout);
    if (lines.charbuf== NULL) {
        return false;
    }
    load_xmlstring(DB, &lines);

    free_String(&lines);


    return true;
}
struct String* GetNodeAttribute(struct Database *DB ,int nodeId ,char* labelchar )  {
    struct String label;init_String(&label,0);
    StringCharCpy(&label,labelchar);
    struct String *content=malloc(sizeof (struct String));
    if(content==NULL){
        fprintf(stderr,"\nError-Memory allocation failed");
        exit(1);
    }
    init_String(content,0);
    while( DB->WriteLock) {
        fprintf(stderr,"Waiting for WriteLock-GetNodeAttribute\n");
    }
    int LineNo = DB->nodeNoToLineno.items[nodeId];
    if (LineNo < 0 ){
        fprintf(stderr,"Warning :node  doesnot exist\n");
        free_String(&label);
        return NULL;
    }
    TrimSpaceString(&label);
    StringCharConcat(&label,"=");
    struct StringList attributes ;init_StringList(&attributes,0);
    String_Split(&attributes,Valueat(&DB->global_attributes,LineNo), "||");
    
    for(size_t i=0;i<attributes.length;i++){
        struct String *attri=&attributes.string[i];
        TrimSpaceString(attri);

        if(attri->length>label.length+1){
            if (strncmp(attri->charbuf ,label.charbuf,label.length)==0) {
                Sub_String(content,attri,label.length+1,attri->length-1);
                free_StringList(&attributes);
                free_String(&label);
                return content;
            }
        }
    }
    free_String(&label);
    free_StringList(&attributes);
    return content;
}
struct String *GetNodeValue(struct Database *DB ,int nodeId)  {
    struct String* content=malloc(sizeof (struct String));
    if(content==NULL){
        fprintf(stderr,"\nError-Memory allocation failed");
        exit(1);
    }
    init_String(content,0);
    while( DB->WriteLock) {
        fprintf(stderr,"Waiting for WriteLock-GetNodeValue\n");
    }
    int lineno = DB->nodeNoToLineno.items[nodeId];
    if (lineno < 0) {
        fprintf(stderr,"Warning :node  doesnot exist\n");
        return content;
    }
    struct String content_tmp;init_String(&content_tmp,0);

    StringStringCpy(&content_tmp,Valueat(&DB->global_values,lineno));

    ReplaceHTMLSpecialEntities(content,&content_tmp);
    free_String(&content_tmp);

    return content;
}
struct String * GetNodeName(struct Database *DB, int nodeId )  {
    struct String* content=malloc(sizeof (struct String));
    if(content==NULL){
        fprintf(stderr,"\nError-Memory allocation failed");
        exit(1);
    }
    init_String(content,0);
    while( DB->WriteLock) {
        fprintf(stderr,"Waiting for WriteLock-GetNodeName\n");
    }
    int lineno = DB->nodeNoToLineno.items[nodeId];
    if (lineno < 0) {
        fprintf(stderr,"Warning :node  doesnot exist\n");
        return content;
    }
    struct StringList pathparts ;init_StringList(&pathparts,0);
    String_Split(&pathparts,Valueat(&DB->global_paths,lineno), "/");
    StringStringCpy(content,&pathparts.string[pathparts.length-1]);
    free_StringList(&pathparts);
    return content;
}
struct String * GetNodeContentsRaw(struct Database *DB, int nodeId )  {
    while( DB->WriteLock) {
        fprintf(stderr,"Waiting for WriteLock-GetNodeContents\n");
    }
    struct String* Output=malloc(sizeof (struct String));
    if(Output==NULL){
        fprintf(stderr,"\nError-Memory allocation failed");
        exit(1);
    }
    init_String(Output,0);
    int beginning = NodeLine(DB, nodeId);
    if (beginning < 0) {
        return Output;
    }
    int end = NodeEnd(DB, nodeId);
    if (DB->Debug_enabled ){
        fprintf(stderr,"getNodeContents :Fetching Contents from line %d to %d \n", beginning, end);
    }
    struct StringList lines ;init_StringList(&lines,0);
    for(int i=beginning;i<end;i++) {
        appendto_StringList(&lines,Valueat(&DB->global_dbLines,i));

    }
    struct StringList lines2 ;init_StringList(&lines2,0);
    formatxml(&lines2,&lines);
    free_StringList(&lines);
    for(size_t i=0;i<lines2.length;i++) {
        StringStringConcat(Output,&lines2.string[i]);
        StringCharConcat(Output,"\n");

    }
    free_StringList(&lines2);
    return Output;
}
struct String * GetNodeContents(struct Database *DB, int nodeId )  {
    while( DB->WriteLock) {
        fprintf(stderr,"Waiting for WriteLock-GetNodeContents\n");
    }
    struct String* Output=malloc(sizeof (struct String));
    if(Output==NULL){
        fprintf(stderr,"\nError-Memory allocation failed");
        exit(1);
    }
    init_String(Output,0);
    int beginning = NodeLine(DB, nodeId);
    if (beginning < 0) {
        return Output;
    }
    int end = NodeEnd(DB, nodeId);
    if (DB->Debug_enabled ){
        printf("getNodeContents :Fetching Contents from line %d to %d \n", beginning, end);
    }
    struct StringList lines ;init_StringList(&lines,0);
    int end_tmp=end;
    if ((end - beginning) > 200 ){
        end_tmp=beginning+200;
        printf("\n No of lines more than 200 \n ");
    }
    for(int i=beginning;i<end_tmp;i++) {
        appendto_StringList(&lines,Valueat(&DB->global_dbLines,i));

    }
    struct StringList lines2 ;init_StringList(&lines2,0);
    formatxml(&lines2,&lines);
    free_StringList(&lines);
    for(size_t i=0;i<lines2.length;i++) {
        ReplcSubstring(&lines2.string[i],"<nil:node>", "");
        ReplcSubstring(&lines2.string[i],"<nil:node/>", "");
        StringStringConcat(Output,&lines2.string[i]);
        StringCharConcat(Output,"\n");

    }
    free_StringList(&lines2);
    if ((end - beginning) > 200) {
        StringCharConcat(Output,"\n .....Remaining lines are not printed......\n ");
    }
    return Output;
}

void NodeDebug(struct Database *DB, int nodeId )  {
    while( DB->WriteLock) {
        fprintf(stderr,"Waiting for WriteLock-GetNodeContents\n");
    }

    int beginning = NodeLine(DB, nodeId);
    if (beginning < 0) {
        return ;
    }
    int end = NodeEnd(DB, nodeId);
    if (DB->Debug_enabled ){
        printf("getNodeContents :Fetching Contents from line %d to %d \n", beginning, end);
    }

    for(int i=beginning;i<end;i++) {
        int nodeend = 0;
        int nodebeg = 0;
        if (DB->global_ids.items[i] >= 0) {
            nodebeg = DB->nodeNoToLineno.items[DB->global_ids.items[i]];
            nodeend = DB->nodeNoToLineno.items[DB->Nodeendlookup.items[DB->global_ids.items[i]]] + 1;
        }
        printf("\n path- %s  line- %s  nodeid-%d nodebeg-%d nodeend-%d", Valueat(&DB->global_paths,i)->charbuf, Valueat(&DB->global_dbLines,i)->charbuf, DB->global_ids.items[i], nodebeg, nodeend);
        printf("\n value- %s attribute-%s", Valueat(&DB->global_values,i)->charbuf, Valueat(&DB->global_attributes,i)->charbuf);

    }

    return ;
}
bool validatexml(struct String *contentStr ) {
    struct StringList nodesnames ; init_StringList(&nodesnames,0);
    bool nodeEnded = false;
    bool CommentStarted = false;
    bool Comment2Started = false;
    bool xmldeclarationStarted = false;
    bool CDATAStarted = false;
    size_t lastindex = 0;
    size_t index = 0;
    char *content=contentStr->charbuf;
    while(index<contentStr->length) {
        if( content[index] == '<') {
            if (!CommentStarted && !CDATAStarted && !xmldeclarationStarted) {
                if (content[index+1] == '!') {
                    nodeEnded = false;
                    if (content[index+2] == '[') {
                        lastindex = index + 3;
                        CDATAStarted = true;
                        lastindex = index;
                    } else if (content[index+2] == '-') {
                        CommentStarted = true;
                        lastindex = index + 3;
                    } else {
                        Comment2Started = true;
                        lastindex = index + 2;
                    }
                } else if (content[index+1] == '?') {
                    nodeEnded = false;
                    xmldeclarationStarted = true;
                    lastindex = index + 2;
                } else if( content[index+1] == '/') {
                    nodeEnded = true;
                    lastindex = index + 2;
                } else {
                    lastindex = index + 1;
                }
            }
        }
        if (content[index] == '>') {
            if (CommentStarted) {
                if ((content[index-1] == '-') && (content[index-2] == '-')) {
                    CommentStarted = false;
                    lastindex = index + 1;
                }
            } else if (CDATAStarted) {
                if ((content[index-1] == ']') && (content[index-2] == ']')) {
                    lastindex = index + 1;
                    CDATAStarted = false;
                }
            } else if (xmldeclarationStarted) {
                if (content[index-1] == '?') {
                    xmldeclarationStarted = false;
                    lastindex = index + 1;
                }
            } else if (Comment2Started) {
                Comment2Started = false;
                lastindex = index + 1;
            } else {
                if( content[index-1] == '/' ){
                    lastindex = index + 1;
                    nodeEnded = false;
                } else {
                    struct String contentlast;init_String(&contentlast,0);
                    Sub_String(&contentlast,contentStr,lastindex,index);
                    TrimSpaceString(&contentlast);
                    if (nodeEnded) {
                        if( nodesnames.length == 0 ){
                            free_StringList(&nodesnames);
                            free_String(&contentlast);
                            return false;
                        }
                        TrimSpaceString(&nodesnames.string[nodesnames.length-1]);

                        if(strcmp(nodesnames.string[nodesnames.length-1].charbuf,contentlast.charbuf)!=0){
                            free_StringList(&nodesnames);
                            free_String(&contentlast);
                            return false;
                        }
                        removeFrom_StringList(&nodesnames,nodesnames.length-1);
                        nodeEnded = false;

                    } else {
                        struct StringList parts ;init_StringList(&parts,0);
                        String_Split(&parts,&contentlast, " ");
                        appendto_StringList(&nodesnames,&parts.string[0]);
                        free_StringList(&parts);
                    }
                    free_String(&contentlast);
                    lastindex = index + 1;
                }
            }
        }
        index++;

    }
    struct String contentlast;init_String(&contentlast,0);
    Sub_String(&contentlast,contentStr,lastindex,index);
    TrimSpaceString(&contentlast);
    if( contentlast.length > 0 ){
        fprintf(stderr,"\n%s\n",contentlast.charbuf);
        free_StringList(&nodesnames);
        free_String(&contentlast);
        return false;
    }
    free_String(&contentlast);

    if (CommentStarted || xmldeclarationStarted || Comment2Started || CDATAStarted) {
        fprintf(stderr,"\nComment/xmldeclaration/CDATA session not closed\n");
        free_StringList(&nodesnames);
        return false;
    }
    if (nodesnames.length > 0) {
        fprintf(stderr,"\nNodes not closed\n");
        for(size_t i=0;i<nodesnames.length;i++) {
            fprintf(stderr,"\n%s\n",nodesnames.string[i].charbuf);
        }
        free_StringList(&nodesnames);
        return false;
    }
    free_StringList(&nodesnames);
    return true;
}
static void remove_Node(struct VectorInt *removedids, struct Database *DB, int nodeId)  {

    if (DB->Debug_enabled ){
        printf("removeNode :Removing node %d\n", nodeId);
    }

    int startindex = NodeLine(DB, nodeId);
    int end = NodeEnd(DB, nodeId);

    DB->startindex = startindex;
    DB->WriteLock = true;
    for (int i = startindex; i < end; i++ ){
        struct String *path = Valueat(&DB->global_paths,startindex);//do not free
        struct StringList path_parts;init_StringList(&path_parts,0);
        String_Split(&path_parts,path, "/");
        if(strcmp(path_parts.string[path_parts.length-1].charbuf,"~")!=0){
            int hashno = stringtono(DB, &path_parts.string[path_parts.length-1]);
            removeid_fromhashtable(&DB->pathKeylookup, hashno, DB->global_ids.items[startindex],&DB->nodeNoToLineno);
        }
        removeFrom_SegmentedStringList(&DB->global_dbLines,startindex);
        appendto_VectorInt(&DB->deleted_ids, DB->global_ids.items[startindex]);
        appendto_VectorInt(removedids, DB->global_ids.items[startindex]);

        DB->nodeNoToLineno.items[DB->global_ids.items[startindex]] = -1;
        removefrom_VectorInt(&DB->global_ids, startindex);
        removeFrom_SegmentedStringList(&DB->global_paths,startindex);
        removeFrom_SegmentedStringList(&DB->global_values,startindex);
        removeFrom_SegmentedStringList(&DB->global_attributes,startindex);
        free_StringList(&path_parts);
    }

}


static struct ResultStruct * insertAtLine(struct Database *DB, int lineno,struct String * sub_xml ,int retainid ) {
    struct ResultStruct* ResultSend= malloc(sizeof(struct ResultStruct));
    if(ResultSend==NULL){
        fprintf(stderr,"\nError-Memory allocation failed");
        exit(1);
    }
    init_VectorInt(&ResultSend->nodeids,0);
    ResultSend->Error=NULL;
    init_StringList(&ResultSend->labelvalues,0);
    DB->retainid = retainid;
    StringCharCpy(&DB->removeattribute ,"");
    DB->pathIdStack_index = 0;
    DB->startindex = lineno;
    int startindex_tmp = lineno;
    if (lineno >0 ){
        DB->reference_linenotoinsert = lineno - 1;
        struct String  *path_tmp = Valueat(&DB->global_paths,lineno-1);//donot free
        struct String  path;init_String(&path,0);
        StringStringCpy(&path,path_tmp);
        if (( path.charbuf[path.length-2]=='/') &&  (path.charbuf[path.length-1]=='~')){
            TrimRightString(&path,2);
        }
        char *pre_line= Valueat(&DB->global_dbLines,lineno-1)->charbuf;//don't free
        if (strstr(pre_line, "</")!=NULL || strstr(pre_line, "/>")!=NULL || strstr(pre_line, "<!")!=NULL) {
            struct StringList path_parts ;init_StringList(&path_parts,0);
            String_Split(&path_parts,&path, "/");
            TrimRightString(&path,path_parts.string[path_parts.length-1].length+1);
            free_StringList(&path_parts);
        }

        StringStringCpy(&DB->path, &path);
        free_String(&path);
    }else{
        DB->reference_linenotoinsert=0;
        clear_String(&DB->path);
    }


    ReplcSubstring(sub_xml,"\n","");

    if (!validatexml(sub_xml)) {
        fprintf(stderr,"\n xml content is not proper- aborting insertion");
        char* error= (char*) malloc((ERRORLENGTH) * sizeof(char));
        if(error==NULL){
            fprintf(stderr,"\nError-Memory allocation failed");
            exit(1);
        }
        strcpy(error,"xml content is not proper- aborting insertion");
        ResultSend->Error=error;
        return ResultSend;
    }
    //printf("path-%s,sub_xml-%s",DB->path,sub_xml);
    parseAndLoadXml(&ResultSend->nodeids,DB, sub_xml);
    ResultSend->Error=NULL;
    updateNodenoLineMap(DB, startindex_tmp-1);
    DB->startindex = -1;

    return ResultSend;
}
static struct ResultStruct * replaceNodeRetainid(struct Database *DB, int nodeId,struct String * sub_xml ){
    if (DB->Debug_enabled ){
        printf("replaceNodeRetainid :Replacing node %d\n", nodeId);
    }
    if (!validatexml(sub_xml)) {
        fprintf(stderr,"\n xml content is not proper- aborting replacing");
        struct ResultStruct* ResultSend= malloc(sizeof(struct ResultStruct));
        if(ResultSend==NULL){
            fprintf(stderr,"\nError-Memory allocation failed");
            exit(1);
        }
        char* error= (char*) malloc((ERRORLENGTH) * sizeof(char));
        if(error==NULL){
            fprintf(stderr,"\nError-Memory allocation failed");
            exit(1);
        }
        strcpy(error,"xml content is not proper- aborting replacing");
        ResultSend->Error=error;
        init_VectorInt(&ResultSend->nodeids,0);
        init_StringList(&ResultSend->labelvalues,0);
        return ResultSend;
    }
    int startindex = NodeLine(DB, nodeId);
    if(startindex<0){
        fprintf(stderr,"\n Node doesnot exist");
        char* error= (char*) malloc((ERRORLENGTH) * sizeof(char));
        if(error==NULL){
            fprintf(stderr,"\nError-Memory allocation failed");
            exit(1);
        }
        struct ResultStruct* ResultSend= malloc(sizeof(struct ResultStruct));
        if(ResultSend==NULL){
            fprintf(stderr,"\nError-Memory allocation failed");
            exit(1);
        }
        strcpy(error,"Node doesnot exist");
        ResultSend->Error=error;
        init_VectorInt(&ResultSend->nodeids,0);
        init_StringList(&ResultSend->labelvalues,0);
        return ResultSend;
    }
    struct VectorInt removed ;init_VectorInt(&removed,0);
    remove_Node(&removed,DB, nodeId);
    removefrom_VectorInt(&DB->deleted_ids, DB->deleted_ids.length-removed.length);
    struct ResultStruct* ResultSend=  insertAtLine(DB, startindex, sub_xml, removed.items[0]);
    free_VectorInt(&removed);
    return ResultSend;
}
static struct ResultStruct * update_nodevalue(struct Database *DB, int nodeId,struct String * new_value ) {
    int Nooflines=NodeEnd(DB, nodeId) - NodeLine(DB, nodeId);
    if (Nooflines > 2) {
        struct ResultStruct* ResultSend= malloc(sizeof(struct ResultStruct));
        if(ResultSend==NULL){
            fprintf(stderr,"\nError-Memory allocation failed");
            exit(1);
        }
        fprintf(stderr,"\nError :Cannot update value- Node contains subnodes\n");
        char* error= (char*) malloc((ERRORLENGTH) * sizeof(char));
        if(error==NULL){
            fprintf(stderr,"\nError-Memory allocation failed");
            exit(1);
        }
        strcpy(error,"node  doesnot exist");
        ResultSend->Error=error;
        init_VectorInt(&ResultSend->nodeids,0);
        init_StringList(&ResultSend->labelvalues,0);
        return ResultSend;
    }
    struct String *content = GetNodeContentsRaw(DB, nodeId);
    ReplcSubstring(content,"\n","-");//for strtok
    ReplcSubstring(content,"><",">-<");//for strtok
    if (content->length == 0) {
        struct ResultStruct* ResultSend= malloc(sizeof(struct ResultStruct));
        if(ResultSend==NULL){
            fprintf(stderr,"\nError-Memory allocation failed");
            exit(1);
        }
        fprintf(stderr,"Warning :node  doesnot exist\n");
        char* error= (char*) malloc((ERRORLENGTH) * sizeof(char));
        if(error==NULL){
            fprintf(stderr,"\nError-Memory allocation failed");
            exit(1);
        }
        strcpy(error,"node  doesnot exist");
        ResultSend->Error=error;
        init_VectorInt(&ResultSend->nodeids,0);
        init_StringList(&ResultSend->labelvalues,0);
        free_StringReturn(content);
        return ResultSend;
    }
    struct String *value = GetNodeValue(DB, nodeId);
    struct String result ; init_String(&result,new_value->length*2);
    //char *result= malloc(strlen(new_value)*2 * sizeof(char));
    if (value->length == 0 && Nooflines == 1 && (strstr(content->charbuf, "/>"))!=NULL) {
            struct StringList parts ;init_StringList(&parts,0);
            String_Split(&parts,content, "/>");
            StringStringCpy(&result,&parts.string[0]);
            StringCharConcat(&result,">" );
            StringStringConcat(&result,new_value);
            StringCharConcat(&result,"</");
            struct String *nodename=GetNodeName(DB, nodeId);
            StringStringConcat(&result,nodename);
            StringCharConcat(&result,">" );
            free_StringReturn(nodename);
            free_StringList(&parts);
    } else {
        struct StringList parts ;init_StringList(&parts,0);
        String_Split(&parts,content, ">");
        if (parts.length > 1) {
            struct StringList part1parts ;init_StringList(&part1parts,0);
            String_Split(&part1parts,&parts.string[1], "<");
            StringStringCpy(&result,&parts.string[0]);
            StringCharConcat(&result,">" );
            StringStringConcat(&result,new_value);
            StringCharConcat(&result,"<");
            StringStringConcat(&result,&part1parts.string[1]);
            StringCharConcat(&result,">" );
            free_StringList(&part1parts);
        }
        free_StringList(&parts);
    }
    free_StringReturn(value);

    struct ResultStruct* ResultSend= replaceNodeRetainid(DB, nodeId, &result);
    if (DB->Debug_enabled ){
        printf("UpdateNodevalue :Updating node %d\n", nodeId);
        struct String * prints=GetNodeContentsRaw(DB, nodeId);
        printf("%s\n",prints->charbuf );
        free_String(prints);
    }
    free_String(&result);
    free_StringReturn(content);
    return ResultSend;
}
struct ResultStruct *UpdateNodevalue(struct Database *DB, int nodeId,char* new_valuechar )  {
    struct String new_value;init_String(&new_value,0);
    StringCharCpy(&new_value,new_valuechar);
    while( DB->WriteLock) {
        fprintf(stderr,"Waiting for WriteLock-UpdateNodevalue\n");
    }

    struct String Result;init_String(&Result,0);
    ReplacewithHTMLSpecialEntities(DB,&Result,&new_value);
    struct ResultStruct* ResultSend=  update_nodevalue(DB, nodeId, &Result);
    free_String(&Result);
    free_String(&new_value);
    return  ResultSend;
}
struct ResultStruct * UpdateAttributevalue(struct Database *DB, int nodeId,char* labelchar ,char* valuechar ) {
    struct String label;init_String(&label,0);
    StringCharCpy(&label,labelchar);
    struct String value;init_String(&value,0);
    StringCharCpy(&value,valuechar);
    TrimSpaceString(&value);
    while( DB->WriteLock) {
        fprintf(stderr,"Waiting for WriteLock-UpdateAttributevalue\n");
    }
    int beginning = NodeLine(DB, nodeId);
    struct String* content = Valueat(&DB->global_dbLines,beginning);//donot free
    bool NodeWithoutValue=false;
    if (strstr(content->charbuf, "/>")!=NULL) {
        NodeWithoutValue=true;
    }
   // struct String* content = GetNodeContents(DB, nodeId);
    if ( beginning < 0) {
        fprintf(stderr,"Warning :node  doesnot exist\n");
        char* error= (char*) malloc((ERRORLENGTH) * sizeof(char));
        if(error==NULL){
            fprintf(stderr,"\nError-Memory allocation failed");
            exit(1);
        }
        struct ResultStruct* ResultSend= malloc(sizeof(struct ResultStruct));
        if(ResultSend==NULL){
            fprintf(stderr,"\nError-Memory allocation failed");
            exit(1);
        }
        strcpy(error,"node  doesnot exist");
        ResultSend->Error=error;
        init_VectorInt(&ResultSend->nodeids,0);
        init_StringList(&ResultSend->labelvalues,0);
        free_String(&label);
        free_String(&value);
        return ResultSend;
    }
    struct StringList contentparts ;init_StringList(&contentparts,0);


    struct String newlabelvalue;init_String(&newlabelvalue,0);
    struct String  oldlabelvalue;init_String(&oldlabelvalue,0);
    struct String labelcpy;init_String(&labelcpy,0);
    StringStringCpy(&labelcpy,&label);
    StringCharConcat(&labelcpy,"=");

    if(value.length>0){
        StringStringCpy(&newlabelvalue,&label);
        StringCharConcat(&newlabelvalue,"=\"");
        StringStringConcat(&newlabelvalue,&value);
        StringCharConcat(&newlabelvalue,"\"");
    }
    if(NodeWithoutValue){
       String_Split(&contentparts,content, "/>");
    }else{
       String_Split(&contentparts,content, ">");
    }


    struct String *contentparts0= &contentparts.string[0];//don't free

    if (strstr(contentparts0->charbuf, labelcpy.charbuf)!=NULL) {
        struct String* oldvalue = GetNodeAttribute(DB, nodeId, labelchar);

        //printf("oldvalue -%s --", oldvalue->charbuf);
        StringStringCpy(&oldlabelvalue,&label);
        StringCharConcat(&oldlabelvalue,"=\"");
        StringStringConcat(&oldlabelvalue,oldvalue);
        StringCharConcat(&oldlabelvalue,"\"");
        if (DB->Debug_enabled) {
            printf("replacing -%s -by- %s", oldlabelvalue.charbuf, newlabelvalue.charbuf);
        }
        ReplcSubstring(contentparts0,oldlabelvalue.charbuf,newlabelvalue.charbuf);
        free_StringReturn(oldvalue);
    } else {
        if(value.length>0){
          StringCharConcat(contentparts0," ");
          StringStringConcat(contentparts0,&newlabelvalue);
        }
    }
    struct String contentnew;init_String(&contentnew,0);
    if (NodeWithoutValue) {
        StringStringConcat(&contentnew,contentparts0);
       StringCharConcat(&contentnew,"/>");
    }else {
        StringStringConcat(&contentnew,contentparts0);
       StringCharConcat(&contentnew,">");
    }

    for(size_t i=0;i<contentparts.length;i++){
        struct String *part=&contentparts.string[i];
        TrimSpaceString(part);
        if( i > 0 && part->length > 0 ){
            StringStringConcat(&contentnew,part);
            StringCharConcat(&contentnew,">");
        }
    }
    ModifyValueat(&DB->global_dbLines,beginning,&contentnew);
    struct String attributebuffer;init_String(&attributebuffer,0);
    struct StringList parts; init_StringList(&parts,0);
    TrimSpaceString(contentparts0);
    String_Split(&parts,contentparts0,"\" ");
    for (size_t partind=0;partind<parts.length;partind++) {
        struct String *part=&parts.string[partind];
        TrimSpaceString(part);
        if (partind > 0){
            if (part->length > 0) {
                if (attributebuffer.length > 1) {
                    StringCharConcat(&attributebuffer,"||");
                }
                StringStringConcat(&attributebuffer,part);
                StringCharConcat(&attributebuffer,"\"");
            }
        } else {
            struct StringList subparts; init_StringList(&subparts,0);
            String_Split(&subparts,part," ");
            if (subparts.length > 1 ){
                StringStringCpypart(&attributebuffer,part,subparts.string[0].length,part->length);
                TrimSpaceString(&attributebuffer);
                //StringStringConcat(&attributebuffer,&subparts.string[1]);
                StringCharConcat(&attributebuffer,"\"");
            }
            free_StringList(&subparts);
        }
    }
    ReplcSubstring(&attributebuffer,"\"\"", "\"");
    ModifyValueat(&DB->global_attributes,beginning,&attributebuffer);
    //printf("newcontents-%s",contentparts0.charbuf);
   // struct ResultStruct* ResultSend= replaceNodeRetainid(DB, nodeId, contentparts0);
    if (DB->Debug_enabled) {
        printf("UpdateNodevalue :Updating node %d\n", nodeId);
        struct String* NodeContents=GetNodeContentsRaw(DB, nodeId);
        printf("%s\n", NodeContents->charbuf);
        free_StringReturn(NodeContents);
    }
    free_String(&label);
    free_String(&value);
    free_StringList(&parts);
    free_String(&attributebuffer);
    free_StringList(&contentparts);
    free_String(&newlabelvalue);
    free_String(&oldlabelvalue);
    free_String(&labelcpy);
    free_String(&contentnew);
    //printf("\npPASSED\n");
    struct ResultStruct* ResultSend= malloc(sizeof(struct ResultStruct));
    if(ResultSend==NULL){
        fprintf(stderr,"\nError-Memory allocation failed");
        exit(1);
    }
    ResultSend->Error=NULL;
    init_VectorInt(&ResultSend->nodeids,0);
    appendto_VectorInt(&ResultSend->nodeids,nodeId);
    init_StringList(&ResultSend->labelvalues,0);
    return ResultSend;
}
struct VectorInt* RemoveNode(struct Database *DB, int nodeId) {
    while(DB->WriteLock) {
        fprintf(stderr,"Waiting for WriteLock-RemoveNode\n");
    }

    struct VectorInt *nodes = malloc(sizeof(struct VectorInt));
    if(nodes==NULL){
        fprintf(stderr,"\nError-Memory allocation failed");
        exit(1);
    }
    init_VectorInt(nodes,0);
    remove_Node(nodes,DB, nodeId);

    updateNodenoLineMap(DB, DB->startindex);
    return nodes;
}
struct ResultStruct *ReplaceNode(struct Database *DB, int nodeId,char* sub_xmlchar ) {
    struct String sub_xml;init_String(&sub_xml,0);
    StringCharCpy(&sub_xml,sub_xmlchar);
    while(DB->WriteLock) {
        fprintf(stderr,"Waiting for WriteLock-ReplaceNode\n");
    }

    if (DB->Debug_enabled) {
        printf("replaceNode :Replacing node %d\n", nodeId);
    }
    if (!validatexml(&sub_xml)) {
        printf("%s",sub_xml.charbuf);
        fprintf(stderr,"\n xml content is not proper- aborting replacing");
        char* error= (char*) malloc((ERRORLENGTH) * sizeof(char));
        if(error==NULL){
            fprintf(stderr,"\nError-Memory allocation failed");
            exit(1);
        }
        struct ResultStruct* ResultSend= malloc(sizeof(struct ResultStruct));
        if(ResultSend==NULL){
            fprintf(stderr,"\nError-Memory allocation failed");
            exit(1);
        }
        strcpy(error,"xml content is not proper- aborting replacing");
        ResultSend->Error=error;
        init_VectorInt(&ResultSend->nodeids,0);
        init_StringList(&ResultSend->labelvalues,0);
        free_String(&sub_xml);
        return ResultSend;
    }
    int startindex = NodeLine(DB, nodeId);
    if(startindex<0){
        fprintf(stderr,"\n Node doesnot exist");
        char* error= (char*) malloc((ERRORLENGTH) * sizeof(char));
        if(error==NULL){
            fprintf(stderr,"\nError-Memory allocation failed");
            exit(1);
        }
        struct ResultStruct* ResultSend= malloc(sizeof(struct ResultStruct));
        if(ResultSend==NULL){
            fprintf(stderr,"\nError-Memory allocation failed");
            exit(1);
        }
        strcpy(error,"Node doesnot exist");
        ResultSend->Error=error;
        init_VectorInt(&ResultSend->nodeids,0);
        init_StringList(&ResultSend->labelvalues,0);
        free_String(&sub_xml);
        return ResultSend;
    }
    struct VectorInt rmids ;init_VectorInt(&rmids,0);
    remove_Node(&rmids,DB, nodeId);
    if (rmids.length > 0) {
        struct ResultStruct* ResultSend= insertAtLine(DB, startindex, &sub_xml, -1);
        free_VectorInt(&rmids);
        free_String(&sub_xml);
        return ResultSend;
    }
    free_VectorInt(&rmids);
    struct ResultStruct* ResultSend= malloc(sizeof(struct ResultStruct));
    if(ResultSend==NULL){
        fprintf(stderr,"\nError-Memory allocation failed");
        exit(1);
    }
    char* error= (char*) malloc((ERRORLENGTH) * sizeof(char));
    if(error==NULL){
        fprintf(stderr,"\nError-Memory allocation failed");
        exit(1);
    }
    strcpy(error,"Node not found");
    ResultSend->Error=error;
    init_VectorInt(&ResultSend->nodeids,0);
    init_StringList(&ResultSend->labelvalues,0);
    free_String(&sub_xml);
    return ResultSend;
}
bool IslowestNode(struct Database *DB, int nodeId){
    int end = NodeEnd(DB, nodeId);
    if ((end - NodeLine(DB, nodeId)) == 1 ){
        return true;
    }
    return false;
}

struct ResultStruct * InserSubNode(struct Database *DB, int nodeId,char* sub_xmlchar ) {
    struct String sub_xml;init_String(&sub_xml,0);
    StringCharCpy(&sub_xml,sub_xmlchar);
    while(DB->WriteLock) {
        printf("Waiting for WriteLock-InserSubNode\n");
    }

    if (!validatexml(&sub_xml) ){
        fprintf(stderr,"\n xml content is not proper- aborting InserSubNode");
        char* error= (char*) malloc((ERRORLENGTH) * sizeof(char));
        if(error==NULL){
            fprintf(stderr,"\nError-Memory allocation failed");
            exit(1);
        }
        struct ResultStruct* ResultSend= malloc(sizeof(struct ResultStruct));
        if(ResultSend==NULL){
            fprintf(stderr,"\nError-Memory allocation failed");
            exit(1);
        }
        strcpy(error,"xml content is not proper- aborting InserSubNode");
        ResultSend->Error=error;
        init_VectorInt(&ResultSend->nodeids,0);
        init_StringList(&ResultSend->labelvalues,0);
        free_String(&sub_xml);
        return ResultSend;
    }

    int end = NodeEnd(DB, nodeId);
    if ((end - NodeLine(DB, nodeId)) == 1 ){
        struct ResultStruct *result=update_nodevalue(DB, nodeId, &sub_xml);
        removefrom_VectorInt(&result->nodeids,0);
        free_String(&sub_xml);
        return result;
    }
    if (end < 0 ){
        fprintf(stderr,"Error :node  doesnot exist\n");
        char* error= (char*) malloc((ERRORLENGTH) * sizeof(char));
        if(error==NULL){
            fprintf(stderr,"\nError-Memory allocation failed");
            exit(1);
        }
        struct ResultStruct* ResultSend= malloc(sizeof(struct ResultStruct));
        if(ResultSend==NULL){
            fprintf(stderr,"\nError-Memory allocation failed");
            exit(1);
        }
        strcpy(error,"node  doesnot exist");
        ResultSend->Error=error;
        init_VectorInt(&ResultSend->nodeids,0);
        init_StringList(&ResultSend->labelvalues,0);
        free_String(&sub_xml);
        return ResultSend;
    }
    struct ResultStruct* ResultSend= insertAtLine(DB, end-1, &sub_xml, -1);
    free_String(&sub_xml);
    return ResultSend;

}
struct ResultStruct * AppendAfterNode(struct Database *DB, int nodeId,char* sub_xmlchar ) {
    struct String sub_xml;init_String(&sub_xml,0);
    StringCharCpy(&sub_xml,sub_xmlchar);
    while(DB->WriteLock) {
        fprintf(stderr,"Waiting for WriteLock-AppendAfterNode\n");
    }

    if (!validatexml(&sub_xml) ){
        fprintf(stderr,"\nError : xml content is not proper- aborting AppendAfterNode");
        char* error= (char*) malloc((ERRORLENGTH) * sizeof(char));
        if(error==NULL){
            fprintf(stderr,"\nError-Memory allocation failed");
            exit(1);
        }
        struct ResultStruct* ResultSend= malloc(sizeof(struct ResultStruct));
        if(ResultSend==NULL){
            fprintf(stderr,"\nError-Memory allocation failed");
            exit(1);
        }
        strcpy(error,"xml content is not proper- aborting ");
        ResultSend->Error=error;
        init_VectorInt(&ResultSend->nodeids,0);
        init_StringList(&ResultSend->labelvalues,0);
        free_String(&sub_xml);
        return ResultSend;

    }
    int end = NodeEnd(DB, nodeId);
    if (end < 0) {
        fprintf(stderr,"Error :node  doesnot exist\n");
        char* error= (char*) malloc((ERRORLENGTH) * sizeof(char));
        if(error==NULL){
            fprintf(stderr,"\nError-Memory allocation failed");
            exit(1);
        }
        struct ResultStruct* ResultSend= malloc(sizeof(struct ResultStruct));
        if(ResultSend==NULL){
            fprintf(stderr,"\nError-Memory allocation failed");
            exit(1);
        }
        strcpy(error,"node  doesnot exist ");
        ResultSend->Error=error;
        init_VectorInt(&ResultSend->nodeids,0);
        init_StringList(&ResultSend->labelvalues,0);
        free_String(&sub_xml);
        return ResultSend;
    }
     struct ResultStruct* ResultSend=  insertAtLine(DB, end, &sub_xml, -1);
     free_String(&sub_xml);
     return ResultSend;
}
struct ResultStruct * AppendBeforeNode(struct Database *DB, int nodeId,char* sub_xmlchar )  {
    struct String sub_xml;init_String(&sub_xml,0);
    StringCharCpy(&sub_xml,sub_xmlchar);
    while(DB->WriteLock) {
        fprintf(stderr,"Waiting for WriteLock-AppendBeforeNode\n");
    }

    if (!validatexml(&sub_xml) ){
        fprintf(stderr,"\n xml content is not proper- aborting AppendBeforeNode");
        char* error= (char*) malloc((ERRORLENGTH) * sizeof(char));
        if(error==NULL){
            fprintf(stderr,"\nError-Memory allocation failed");
            exit(1);
        }
        struct ResultStruct* ResultSend= malloc(sizeof(struct ResultStruct));
        if(ResultSend==NULL){
            fprintf(stderr,"\nError-Memory allocation failed");
            exit(1);
        }
        strcpy(error,"xml content is not proper- aborting ");
        ResultSend->Error=error;
        init_VectorInt(&ResultSend->nodeids,0);
        init_StringList(&ResultSend->labelvalues,0);
        free_String(&sub_xml);
        return ResultSend;

    }
    int start = NodeLine(DB, nodeId);
    if (start < 0) {
        fprintf(stderr,"Error :node  doesnot exist\n");
        char* error= (char*) malloc((ERRORLENGTH) * sizeof(char));
        if(error==NULL){
            fprintf(stderr,"\nError-Memory allocation failed");
            exit(1);
        }
        struct ResultStruct* ResultSend= malloc(sizeof(struct ResultStruct));
        if(ResultSend==NULL){
            fprintf(stderr,"\nError-Memory allocation failed");
            exit(1);
        }
        strcpy(error,"node  doesnot exist ");
        ResultSend->Error=error;
        init_VectorInt(&ResultSend->nodeids,0);
        init_StringList(&ResultSend->labelvalues,0);
        free_String(&sub_xml);
        return ResultSend;
    }
    struct ResultStruct* ResultSend= insertAtLine(DB, start, &sub_xml, -1);
    free_String(&sub_xml);
    return ResultSend;

}
int LocateRequireParentdNode(struct Database *DB ,int parent_nodeLine ,struct String* RequiredPath ,int LineNo_inp )  {
    //Search 'required node' backward
    if (LineNo_inp < 0 || parent_nodeLine < 0 ){
        return -1;
    }
    struct String *ParentPath = Valueat(&DB->global_paths,parent_nodeLine);

    struct suspectedLinenos_Result *suspectedLines= suspectedLinenos(DB, RequiredPath, parent_nodeLine, LineNo_inp+1);
    if (DB->Debug_enabled) {
        printf("#####LocateRequireParentdNode###\n ");
        printf("ParentPath- %s\n", ParentPath->charbuf);
        printf("LineNo %d\n", LineNo_inp);
        printf("RequiredPath %s\n", RequiredPath->charbuf);
        printf("parent_nodeLine %d\n", parent_nodeLine);
    }

    //locate line just above LineNo_inp
    int requiredline = 0;
    //for _, start := range Starts {
    for(size_t i=0;i<suspectedLines->suspectedLineStarts.length;i++){
        int start=suspectedLines->suspectedLineStarts.items[i];
        if (start >= parent_nodeLine && start <= LineNo_inp) {
            if (start > requiredline) {
                struct compare_path_Result *compareresult= compare_path(Valueat(&DB->global_paths,start), RequiredPath);
                if (compareresult->status ){
                  requiredline = start;
		}
		free_compare_path_Result(compareresult);
            }
        }
    }
    if( Valueat(&DB->global_paths,requiredline)->length >= ParentPath->length) {

        struct compare_path_Result *compareresult= compare_path(Valueat(&DB->global_paths,requiredline), RequiredPath);

        if (compareresult->status ){
            if (DB->Debug_enabled) {
                printf("Located parent %d\n", DB->global_ids.items[requiredline]);
                printf("====LocateRequireParentdNode===\n ");
            }
            free_compare_path_Result(compareresult);
            free_suspectedLinenos_Result(suspectedLines);
            return DB->global_ids.items[requiredline];
        } else {
            free_compare_path_Result(compareresult);
            free_suspectedLinenos_Result(suspectedLines);
            return -1;

        }

    } else {
        free_suspectedLinenos_Result(suspectedLines);
        return -1;

    }
    free_suspectedLinenos_Result(suspectedLines);
    return -1;
}

void free_ResultStruct(struct ResultStruct *v){
    free_VectorInt(&v->nodeids);
    free_StringList(&v->labelvalues);
    if(v->Error!=NULL){
        free(v->Error);
    }
    free(v);
}
bool Regular_expmatch(char* String,char*Regexpr){
   //regular expression is not in windows
#if defined(__linux__)

    regex_t regex;

    int reti = regcomp(&regex,Regexpr , REG_EXTENDED);
    if (reti) {
        fprintf(stderr, "Could not compile regex-%s\n",Regexpr );
        return false;
    }

    /* Execute regular expression */
    reti = regexec(&regex, String, 0, NULL, 0);
    regfree(&regex);
    if (!reti) {
        return true;
    }
#endif
    return false;


}
static struct  ResultStruct *locateNodeLine(struct Database *DB,int parent_nodeLine,struct String* QUERY ,struct String* RegExp ,bool onlypath ,bool isRegExp )  {
    struct ResultStruct* ResultSend= malloc(sizeof(struct ResultStruct));
    if(ResultSend==NULL){
        fprintf(stderr,"\nError-Memory allocation failed");
        exit(1);
    }
    int parent_endline = DB->nodeNoToLineno.items[DB->Nodeendlookup.items[DB->global_ids.items[parent_nodeLine]]] + 1;
    init_VectorInt(&ResultSend->nodeids,0);
    init_StringList(&ResultSend->labelvalues,0);
    ResultSend->Error=NULL;
    if (parent_nodeLine < 0) {
        return ResultSend;
    }
    bool InsideParent = true;
    struct String QueryPath;init_String(&QueryPath,0);
    struct String *ParentPath=Valueat(&DB->global_paths,parent_nodeLine);
    StringStringCpy(&QueryPath ,ParentPath );
    if ( QUERY->length > 0) {
        StringCharConcat(&QueryPath,"/");
        StringStringConcat(&QueryPath,QUERY);
    }

    if (DB->Debug_enabled) {
        printf("####Locate Node#####\n");
        printf("QUERY- %s\n", QUERY->charbuf);
    }
    ReplcSubstring(&QueryPath,"*","");
    if (DB->Debug_enabled) {
        printf("ParentPath- %s\n", ParentPath->charbuf);
        printf("QueryPATH- %s\n", QueryPath.charbuf);
        printf("Search Value %s\n", RegExp->charbuf);

    }

    struct suspectedLinenos_Result *suspectedLines= suspectedLinenos(DB, &QueryPath, parent_nodeLine, parent_endline);
    if (DB->Debug_enabled) {
        printf("suspectedLines->suspectedLineStarts.length-%d\n",suspectedLines->suspectedLineStarts.length);
    }
    for(size_t index=0;index<suspectedLines->suspectedLineStarts.length;index++){
        int start=suspectedLines->suspectedLineStarts.items[index];
        if (DB->Debug_enabled) {
          printf("\tcase-%d\tstart- %d  parent_nodeLine-%d  parent_endline-%d\n",index, start,parent_nodeLine,parent_endline);
        }
        if (start >= parent_nodeLine && start <= parent_endline) {
            int LineNo = start;
            while (InsideParent && (size_t)LineNo < DB->global_dbLines.length && LineNo <= suspectedLines->suspectedLineEnds.items[index]) {
                //printf("\nDB.global_paths[LineNo] %s ParentPath %s\n", DB.global_paths[LineNo], ParentPath)
                if (isParentPath(ParentPath, Valueat(&DB->global_paths,LineNo))) {
                    struct compare_path_Result *compare_result= compare_path(Valueat(&DB->global_paths,LineNo), &QueryPath);
                    struct String labelValueStr;init_String(&labelValueStr,0);
                    for(size_t index2=0;index2<compare_result->label.length;index2++){
                        StringStringConcat(&labelValueStr,&compare_result->label.string[index2]);
                        StringCharConcat(&labelValueStr,"=");
                        StringStringConcat(&labelValueStr,&compare_result->value.string[index2]);
                        StringCharConcat(&labelValueStr,";");
                    }
                    if (compare_result->status) {
                        //printf("\npath matching %s", QueryPath);
                        if (onlypath) {
                            appendto_VectorInt(&ResultSend->nodeids, LineNo);
                            if (DB->Debug_enabled) {
                                printf(" Query matching -lineno %d\n", LineNo);
                            }
                            appendto_StringList(&ResultSend->labelvalues, &labelValueStr);
                        } else {
                            //iterate through all
                            struct StringList values_attributes ;init_StringList(&values_attributes,0);
                            String_Split(&values_attributes,RegExp, ";");
                            bool all_satisfied = true;
                            for(size_t j=0;j<values_attributes.length;j++){
                                struct String *valueorAttribute=&values_attributes.string[j];
                                TrimSpaceString(valueorAttribute);
                                if (valueorAttribute->length > 0 ){

                                    if (strstr(RegExp->charbuf, "=\"")!=NULL) {
                                        struct String *global_Attribute=Valueat(&DB->global_attributes,LineNo);/*don't free*/
                                        //printf("global_Attribute_trimmed-%ld",strlen(global_Attribute_trimmed->charbuf));
                                        // fflush(stdout);
                                        TrimSpaceString(global_Attribute);
                                        if (global_Attribute->length == 0 ) {
                                            all_satisfied = false;
                                        } else {
                                            struct StringList attributes ;init_StringList(&attributes,0);
                                            String_Split(&attributes,global_Attribute, "||");
                                            bool attrib_matching = false;
                                            for(size_t k=0;k<attributes.length;k++){
                                                struct String *attrib =&attributes.string[k];
                                                TrimSpaceString(attrib);
                                                if (attrib->length > 0) {
                                                    bool match = false;
                                                    if (isRegExp) {
                                                        match= Regular_expmatch(attrib->charbuf,valueorAttribute->charbuf);
                                                    } else {
                                                        match = (strcmp(valueorAttribute->charbuf , attrib->charbuf)==0);
                                                    }
                                                    if (match) {
                                                        attrib_matching = true;
                                                    }

                                                } else {

                                                }
                                            }
                                            if (!attrib_matching) {

                                                all_satisfied = false;
                                            }
                                            free_StringList(&attributes);
                                        }

                                    } else {

                                        bool match = false;
                                        if (isRegExp) {
                                            match= Regular_expmatch(Valueat(&DB->global_values,LineNo)->charbuf,valueorAttribute->charbuf);
                                        } else {
                                            struct String valueorAttributeRe ;init_String(&valueorAttributeRe,0);
                                            ReplacewithHTMLSpecialEntities(DB,&valueorAttributeRe,valueorAttribute);
                                            struct String tmp ;init_String(&tmp,0);
                                            StringStringCpy(&tmp,Valueat(&DB->global_values,LineNo));
                                            TrimSpaceString(&tmp);
                                            match = (strcmp(valueorAttributeRe.charbuf , tmp.charbuf)==0);

                                            free_String(&tmp);
                                            free_String(&valueorAttributeRe);
                                        }
                                        if (!match) {
                                            all_satisfied = false;
                                        }


                                    }
                                }


                            }
                            free_StringList(&values_attributes);
                            if (all_satisfied) {
                                appendto_VectorInt(&ResultSend->nodeids, LineNo);
                                appendto_StringList(&ResultSend->labelvalues, &labelValueStr);

                                if (DB->Debug_enabled) {
                                    printf("Query  matching -lineno %d\n", LineNo);
                                }
                            }

                        }

                    }
                    free_compare_path_Result(compare_result);
                    free_String(&labelValueStr);
                } else {
                    InsideParent = false;
                }
                LineNo++;
            }
        }

    }
    free_suspectedLinenos_Result(suspectedLines);
    free_String(&QueryPath);
    if (DB->Debug_enabled) {
        printf("===LocateNode===\n");
    }
    return ResultSend;
}
int ParentNode(struct Database *DB,int nodeId)  {
    while( DB->WriteLock ){
        fprintf(stderr,"Waiting for WriteLock-ParentNode\n");
    }
    int LineNo = DB->nodeNoToLineno.items[nodeId];
    int ResultId = -1;
    if (nodeId < 0) {
            return ResultId;
    }
    if (LineNo < 0) {
        return ResultId;
    }
    struct String *NodePath = Valueat(&DB->global_paths,LineNo);//donot free
    struct StringList parts;init_StringList(&parts,0);
    String_Split(&parts,NodePath, "/");

    struct String RequiredPath ; init_String(&RequiredPath,0);
    Sub_String(&RequiredPath, NodePath,0 , NodePath->length-parts.string[parts.length-1].length-1);

    ResultId= LocateRequireParentdNode(DB,NodeLine(DB,0), &RequiredPath, LineNo);
    free_String(&RequiredPath);
    free_StringList(&parts);
    return ResultId;;
}

struct VectorInt *ChildNodes(struct Database *DB,int nodeId) {
    struct VectorInt * ResultIds=malloc(sizeof (struct VectorInt)) ;
    if(ResultIds==NULL){
        fprintf(stderr,"\nError-Memory allocation failed");
        exit(1);
    }
    init_VectorInt(ResultIds,0);
    while( DB->WriteLock ){
        fprintf(stderr,"Waiting for WriteLock-ChildNodes\n");
    }
    int LineNo = DB->nodeNoToLineno.items[nodeId];
    if (nodeId < 0) {
            return ResultIds;
    }
    if (LineNo < 0) {
        return ResultIds;
    }
    struct String *NodePath = Valueat(&DB->global_paths,LineNo);//donot free
    struct StringList Nodepathparts;init_StringList(&Nodepathparts,0);
    String_Split(&Nodepathparts,NodePath, "/");
    size_t nodeDepth = Nodepathparts.length;
    free_StringList(&Nodepathparts);
    int  Node_end = DB->nodeNoToLineno.items[DB->Nodeendlookup.items[nodeId]] + 1;
    bool InsideParent = true;
    LineNo++;
    while( InsideParent && (LineNo < Node_end)) {
        struct String *path=Valueat(&DB->global_paths,LineNo);
        if (isParentPath(NodePath, path)) {

            if ((path->charbuf[ path->length-2] == '/') && (path->charbuf[ path->length-1] =='~')) {

            } else {
                struct StringList pathparts;init_StringList(&pathparts,0);
                String_Split(&pathparts,path, "/");
                if (pathparts.length== nodeDepth+1 ){
                    appendto_VectorInt(ResultIds,DB->global_ids.items[LineNo]);
                }
                free_StringList(&pathparts);
            }
        } else {
            InsideParent = false;

        }
        LineNo++;
    }
    return ResultIds;
}
int NextNode(struct Database *DB,int nodeId) {
    int LineNo = DB->nodeNoToLineno.items[nodeId];
    if (LineNo < 0 ){
        return -1;
    }
    struct StringList Nodepathparts;init_StringList(&Nodepathparts,0);

    struct String *NodePath = Valueat(&DB->global_paths,LineNo);//donot free
    String_Split(&Nodepathparts,NodePath, "/");
    int nodeDepth = Nodepathparts.length;

    int  Node_end = DB->nodeNoToLineno.items[DB->Nodeendlookup.items[nodeId]] + 1;
    struct String *nextNodePath = Valueat(&DB->global_paths,Node_end);

    String_Split(&Nodepathparts,nextNodePath, "/");
    int nextnodeDepth = Nodepathparts.length;

   
    if(strcmp(Nodepathparts.string[Nodepathparts.length-1].charbuf,"~")==0){
        free_StringList(&Nodepathparts);
        return -1;
    }
    free_StringList(&Nodepathparts);
    int nextnodeid=DB->global_ids.items[Node_end];

    if (nodeDepth == nextnodeDepth) {
        return nextnodeid;
    } else {
        return -1;
    }

}
static void separateValue(struct String *pathpart ,struct String *path,struct String * value) {

    int i = Index(pathpart, "[");
    StringStringCpy(path,pathpart);
    clear_String(value);
    if( i > -1 ){
        Sub_String(path,pathpart, 0, i);
        Sub_String(value,pathpart, i+1, pathpart->length-1 );
    }
}
/**
 * @brief preparePathparts -splits query into parts to search in stages
 * when attribute or value is given in the query in the inbetween path
 * eg:-
 * @param path_in -A/B[def]/C/D[de]
 * @param result - A/B[def]; C/D[de]
 */
static void preparePathparts(struct String *path_in,struct StringList *result){
    struct String tmp;init_String(&tmp,100);
    struct String path;init_String(&path,100);
    StringStringCpy(&path,path_in);
   clear_StringList(result);
    int i = Index(&path, "]/");
    while( i > -1) {
        Sub_String(&tmp,&path, 0, i+1);
        appendto_StringList(result,&tmp);
        removeSubString(&path, 0, i+2);
        i = Index(&path, "]/");
    }
    appendto_StringList(result,&path);
    free_String(&tmp);
    free_String(&path);
}
static void pathwithoutvalue(struct String * path ,struct String *output)  {
    struct StringList items;init_StringList(&items,10);
    struct String value;init_String(&value,100);
    struct String pathonly;init_String(&pathonly,100);
    clear_String(output);
     preparePathparts(path,&items);
     for(size_t i=0;i<items.length;i++){
        separateValue(&items.string[i],&pathonly,&value);
        if (output->length == 0) {
            StringStringCpy(output,&pathonly);
        } else {
            StringCharConcat(output,"/");
            StringStringConcat(output,&pathonly);
        }

    }
     free_StringList(&items);
     free_String(&value);
     free_String(&pathonly);
}
/**
 * @brief GetNode - Process search for xpath(QUERY_inpch) under node (parent_nodeId)
 * @param DB
 * @param parent_nodeId
 * @param QUERY_inpch - query is similar to xpath
 * CASES:
 * (1) A/B/C/D
 * (2) A/<x>/<y>/D               - here <x> & <y> is unknown
 * (3) A/../D                  - recursive search
 * (4) A/B/C[value]            - value of the node is also a search parameter
 * (4) A/B/C[attri="value"]    - an attribute of the node is also a search parameter
 * (5) A/B* /C[attri="value"]   - returns node B which satisfies the query.
 * @return
 */

struct  ResultStruct * GetNode(struct Database *DB,int parent_nodeId , char*  QUERY_inpch) {
    struct String  QUERY_inp;init_String(&QUERY_inp,0);
    StringCharCpy(&QUERY_inp, QUERY_inpch);
    struct ResultStruct* ResultSend= malloc(sizeof(struct ResultStruct));
    if(ResultSend==NULL){
        fprintf(stderr,"\nError-Memory allocation failed");
        exit(1);
    }
    init_VectorInt(&ResultSend->nodeids,0);
    init_StringList(&ResultSend->labelvalues,0);
    ResultSend->Error=NULL;
    // ldld/dkdicmk*/<xe>/kjk[]/lkl
    while( DB->WriteLock ){
        fprintf(stderr,"Waiting for WriteLock-GetNode\n");
    }
    if (DB->Debug_enabled ){

        printf("\n==Process Query===\n");
        printf("ProcessQuery :QUERY_inp- %s\n", QUERY_inp.charbuf);
    }
    struct String tmp;init_String(&tmp,0);
    struct String RequiredPath;init_String(&RequiredPath,0);
    pathwithoutvalue(&QUERY_inp,&RequiredPath);

    if (strstr(RequiredPath.charbuf, "*")!=NULL) {
        struct StringList QUERY_Parts ;init_StringList(&QUERY_Parts,0);
        String_Split(&QUERY_Parts,&RequiredPath, "*");
        StringStringCpy(&RequiredPath,&QUERY_Parts.string[0]);
        free_StringList(&QUERY_Parts);
    }

    if (DB->Debug_enabled) {
        // ldld/dkdicmk*/<xe>/kjk[]/lkl
        printf("ProcessQuery :QUERY- %s\n", QUERY_inp.charbuf); //gives- ldld/dkdicmk*/<xe>/kjk[]/lkl
        printf("ProcessQuery :RequiredPath- %s\n", RequiredPath.charbuf); //gives- ldld/dkdicmk*

    }
    struct StringList  parts ;init_StringList(&parts,0);
    preparePathparts(&QUERY_inp,&parts);
    //eg-: ldld/dkdicmk[a]/<xe>/kjk[b]/lkl --> ldld/dkdicmk[a] ; <xe>/kjk[b] ; lkl
    struct VectorInt  final_nodesLineNo;init_VectorInt(&final_nodesLineNo,0);
    int parent_nodeLine = NodeLine(DB, parent_nodeId);
    if (parent_nodeLine < 0) {
        return ResultSend;
    }
    appendto_VectorInt(&final_nodesLineNo, parent_nodeLine);
    struct String nilstr;init_String(&nilstr,0);
    StringCharCpy(&nilstr,"");
    appendto_StringList(&ResultSend->labelvalues,&nilstr) ;
    free_String(&nilstr);
    struct VectorInt   nextnodesLineNo ;init_VectorInt(&nextnodesLineNo,0);
    struct StringList  nextlabels ;init_StringList(&nextlabels,0);

    for(size_t i=0;i<parts.length;i++){
        //Does search for each part
        //Result of search of first part becomes root node for second part and so on.
        //In the subsequent parts search in done for all possible node found in the previous part
        // and the label value in each search is recorded to derive final required path.
        struct String *part=&parts.string[i];
        struct String QUERYSTR;init_String(&QUERYSTR,0);
        struct String RegExp;init_String(&RegExp,0);
        separateValue(part,&QUERYSTR,&RegExp);
         ReplcSubstring(&QUERYSTR,"*","");
        if (part->length > 0 ){
            clear_StringList(&nextlabels);
            clear_VectorInt(&nextnodesLineNo);
            for(size_t ind=0;ind<final_nodesLineNo.length;ind++){
                int node=final_nodesLineNo.items[ind];
                bool onlypath = true;
                if (RegExp.length > 0) {
                    onlypath = false;
                }
                if (QUERYSTR.charbuf[0] == '/') {
                    removeSubString(&QUERYSTR,0,1);
                    //QUERYSTR = QUERYSTR[1:]
                }

                bool isRegExp = false;
                //RegExp starts with > eg :- [>[a-b]{1,2}]
                if (RegExp.length > 0) {
                    if (RegExp.charbuf[0] == '>' ){
                        removeSubString(&RegExp,0,1);
                        //RegExp = RegExp[1:];
                        isRegExp = true;
                    }
                }
                struct  ResultStruct *NodeLocated= locateNodeLine(DB, node, &QUERYSTR, &RegExp, onlypath, isRegExp);

                //for i, label := range labels {
                for(size_t i=0;i<NodeLocated->labelvalues.length;i++){
                    struct String *label=&NodeLocated->labelvalues.string[i];//<label>=value;
                    StringStringCpy(&tmp,label);
                    //value used for the previously found label also have to be kept eg for <x>/dd/<y>/err/:- value of <x> used while finding <y>
                    StringStringConcat(&tmp,&ResultSend->labelvalues.string[ind]);
                    appendto_StringList(&nextlabels,&tmp);
                    appendto_VectorInt(&nextnodesLineNo, NodeLocated->nodeids.items[i]);
                    if (DB->Debug_enabled) {
                        printf("ProcessQuery :label %s\n", label->charbuf);
                        printf("ProcessQuery :identifiedNode %d\n", NodeLocated->nodeids.items[i]);
                    }

                }
                free_ResultStruct(NodeLocated);

            }
            clear_StringList(&ResultSend->labelvalues);
            clear_VectorInt(&final_nodesLineNo);

            concatenate_StringList(&ResultSend->labelvalues ,& nextlabels);//["<label1>=value;<label2>=value;",".."]
            concatenate_VectorInt( &final_nodesLineNo , &nextnodesLineNo);//[result_final nodeid1(with above labels & values),result_final nodeid2    ]
        }
        free_String(&QUERYSTR);
        free_String(&RegExp);
    }
    free_VectorInt(&nextnodesLineNo);
    free_StringList(&nextlabels);
    free_StringList(&parts);
    //struct VectorInt *ResultIds = init_VectorInt(0);
    for(size_t index=0;index<ResultSend->labelvalues.length;index++){
        struct String *label_res=&ResultSend->labelvalues.string[index];
        int nodeLine = final_nodesLineNo.items[index];
        if (nodeLine >= 0) {
            struct String  RequiredPathN;init_String(&RequiredPathN,0);
            StringStringCpy(&RequiredPathN , &RequiredPath);
            //printf("ProcessQuery :label_res %s\n", label_res)//<label1>=value1;<label2>=value2;
            struct StringList  entries ;init_StringList(&entries,0);
            String_Split(&entries,label_res, ";");
            //replacing all <x>
            for(size_t k=0;k<entries.length;k++){
                struct String *entry=&entries.string[k];
                struct StringList parts ;init_StringList(&parts,0);
                String_Split(&parts,entry, "=");
                if (parts.string[0].length > 0 ){
                    ReplcSubstring(&RequiredPathN, parts.string[0].charbuf, parts.string[1].charbuf);
                }
                free_StringList(&parts);
            }
            free_StringList(&entries);
            //printf("ProcessQuery :parent_nodeLine %s\n", DB.global_paths[parent_nodeLine]+"/"+RequiredPathN)
            StringStringCpy(&tmp,Valueat(&DB->global_paths,parent_nodeLine));
            StringCharConcat(&tmp,"/");
            StringStringConcat(&tmp,&RequiredPathN);
            int ResultId = LocateRequireParentdNode(DB, parent_nodeLine, &tmp, nodeLine);//parent path just above the child
            if( ResultId > 0 ){
                if (DB->Debug_enabled ){
                    printf("ProcessQuery :ResultId %d\n", ResultId);
                }
                appendto_VectorInt(&ResultSend->nodeids, ResultId);
            }
            free_String(&RequiredPathN);
        }

    }
    free_VectorInt(&final_nodesLineNo);
    free_String(&tmp);
    free_String(&RequiredPath);
    free_String(&QUERY_inp);
    return ResultSend;

}
/* ////////////to be corrected/////////////
struct String*  CutPasteAsSubNode(struct Database *DB ,int UnderId,int nodeId)  {
    struct String* Error=malloc(sizeof (struct String)); init_String(Error,0);
    if(Error==NULL){
        fprintf(stderr,"\nError-Memory allocation failed");
        exit(1);
    }
    while(DB->WriteLock) {
        fprintf(stderr,"Waiting for WriteLock-CutPasteAsSubNode\n");
    }
    int previousparentid = ParentNode(DB, nodeId);
    if (previousparentid == -1 ){
        fprintf(stderr,"\nNode doesnot exists");
        StringCharCpy(Error,"Node doesnot exists");
        return Error;
    }
    //  pSegNo, pindex := getSegmenNoIndex(DB, NodeLine(DB, previousparentid))
    struct String* previousparentpath = Valueat(&DB->global_paths,NodeLine(DB, previousparentid));

    //remove from old location
    int Line = NodeLine(DB, nodeId);
    int end = NodeEnd(DB, nodeId);
    int startindex = Line;

    struct VectorInt DB_global_ids; init_VectorInt(&DB_global_ids,0);
    struct StringList DB_global_dbLines; init_StringList(&DB_global_dbLines,0);
    struct StringList DB_global_paths; init_StringList(&DB_global_paths,0);
    struct StringList DB_global_values; init_StringList(&DB_global_values,0);
    struct StringList DB_global_attributes; init_StringList(&DB_global_attributes,0);
    DB->WriteLock = true;
    while( Line < end ){
        appendto_VectorInt(&DB_global_ids, DB->global_ids.items[startindex]);
        removefrom_VectorInt(&DB->global_ids, startindex);

        appendto_StringList(&DB_global_dbLines, Valueat(&DB->global_dbLines,startindex));
        removeFrom_SegmentedStringList(&DB->global_dbLines,startindex);
        appendto_StringList(&DB_global_paths, Valueat(&DB->global_paths,startindex));
        removeFrom_SegmentedStringList(&DB->global_paths,startindex);
        appendto_StringList(&DB_global_values, Valueat(&DB->global_values,startindex));
        removeFrom_SegmentedStringList(&DB->global_values,startindex);
        appendto_StringList(&DB_global_attributes, Valueat(&DB->global_attributes,startindex));
        removeFrom_SegmentedStringList(&DB->global_attributes,startindex);
        Line++;

    }
    updateNodenoLineMap(DB, 0);

    int ToNodeend = NodeEnd(DB, UnderId);
    if (ToNodeend == -1) {
         printf("\nNew Parent Node doesnot exists");
         StringCharCpy(Error,"New Parent Node doesnot exists");
         return Error;
    }
    struct String *NewParentNodename = GetNodeName(DB, UnderId);
    bool NewParentNodeisEmpty = false;
    int insertLine = ToNodeend - 1;
    if ((ToNodeend - NodeLine(DB, UnderId)) == 1) {
        struct String * dbLine=Valueat(&DB->global_dbLines,insertLine);//don't free
        if (strstr(dbLine->charbuf, "/>")!=NULL  ) {
            NewParentNodeisEmpty = true;
            ReplcSubstring(dbLine, "/>", ">");
        } else {
            fprintf(stderr,"\nNode is a lowest node , not a nil node");
            StringCharCpy(Error," Node is a lowest node , not a nil node");
            return Error;
        }

    }
    ;
    struct String newparentpath;
    init_String(&newparentpath,0);
    //prepare initial path
    if (NewParentNodeisEmpty) {
        StringStringCpy(&newparentpath,Valueat(&DB->global_paths,insertLine));
    } else {
        StringStringCpy(&newparentpath,Valueat(&DB->global_paths,insertLine-1));
    }
    if (( newparentpath.charbuf[newparentpath.length-2]=='/') &&  (newparentpath.charbuf[ newparentpath.length-1]=='~')){
        TrimRightString(&newparentpath,2);
    }
     char *pre_line= Valueat(&DB->global_dbLines,insertLine)->charbuf;//don't free
    if (strstr(pre_line, "</")!=NULL || strstr(pre_line, "/>")!=NULL || strstr(pre_line, "<!")!=NULL) {
        struct StringList path_parts ;init_StringList(&path_parts,0);
        String_Split(&path_parts,&newparentpath, "/");
        TrimRightString(&newparentpath,path_parts.string[path_parts.length-1].length+1);
        free_StringList(&path_parts);
    }
    //Paste to new location
    Line = 0;
    if (NewParentNodeisEmpty) {
        insertLine++;
    }
    DB->WriteLock = true;
    while( Line < (int) DB_global_dbLines.length) {

        //DB->path = newparentpath + strings.ReplaceAll(DB_global_paths[Line], previousparentpath, "")
        //strings.ReplaceAll(DB_global_paths[Line], previousparentpath, "")
        struct String tmpstr;
        init_String(&tmpstr,0);
        StringStringCpy(&tmpstr,Valueat(&DB->global_dbLines,Line));
        ReplcSubstring(&tmpstr,previousparentpath->charbuf, "");


        StringStringCpy(&DB->path,&newparentpath);
        StringStringConcat(&DB->path,&tmpstr);

       // ReplcSubstring(&newparentpath,previousparentpath->charbuf, "");


        insertInTo_SegmentedStringList(&DB->global_dbLines , insertLine, &DB_global_dbLines.string[Line]);
        insertInTo_SegmentedStringList(&DB->global_values , insertLine, &DB_global_values.string[Line]);
        insertInTo_SegmentedStringList(&DB->global_attributes  ,insertLine, &DB_global_attributes.string[Line]);
        insertInTo_SegmentedStringList(&DB->global_paths , insertLine, &DB->path);
        inserto_VectorInt(&DB->global_ids, insertLine, DB_global_ids.items[Line]);
        insertLine++;

        Line++;
    }
    free_VectorInt(&DB_global_ids);
    free_StringList(&DB_global_dbLines);
    free_StringList(&DB_global_values);
    free_StringList(&DB_global_attributes);
    free_StringList(&DB_global_paths);

    if (NewParentNodeisEmpty) {
        struct String tmp ;init_String(&tmp,0);
        insertInTo_SegmentedStringList(&DB->global_values , insertLine, &tmp);
        insertInTo_SegmentedStringList(&DB->global_attributes , insertLine, &tmp);

        StringStringCpy(&tmp,&newparentpath);
        StringCharConcat(&tmp,"/~");
        insertInTo_SegmentedStringList(&DB->global_paths , insertLine, &tmp);

        StringCharCpy(&tmp,"</");
        StringStringConcat(&tmp,NewParentNodename);
        StringCharConcat(&tmp,">");
        insertInTo_SegmentedStringList(&DB->global_dbLines , insertLine, &tmp);

        free_String(&tmp);


        inserto_VectorInt(&DB->global_ids, insertLine, DB->global_lineLastUniqueid);
        DB->Nodeendlookup.items[UnderId] = DB->global_lineLastUniqueid;
        DB->global_lineLastUniqueid++;
        if (DB->global_lineLastUniqueid >= DB->maxInt ){
            fprintf(stderr,"load_db: Total no. of Uniqueid>= DB.MaxNooflines, Please increase DB.MaxNooflines before loading db");
            exit(1);
        }
    }
    free_String(&newparentpath);
    free_StringReturn(NewParentNodename);
    updateNodenoLineMap(DB, 0);

    DB->startindex = -1;
    free(Error->charbuf);
    Error->charbuf=NULL;
    return Error;
}
*/
void free_Attributes(struct ResultAttributes *v){
    free_StringList(&v->labels);
    free_StringList(&v->values);
    free(v);
}
struct ResultAttributes* GetAllNodeAttributes(struct Database *DB,int nodeId)  {
    while( DB->WriteLock) {
        fprintf(stderr,"Waiting for WriteLock-GetNodeAttribute\n");
    }
    struct ResultAttributes* Attributes= malloc(sizeof(struct ResultStruct));
    if(Attributes==NULL){
        fprintf(stderr,"\nError-Memory allocation failed");
        exit(1);
    }
    init_StringList(&Attributes->labels,0);
    init_StringList(&Attributes->values,0);
    int LineNo = DB->nodeNoToLineno.items[nodeId];
    if (LineNo < 0 ){
        fprintf(stderr,"Warning :node  doesnot exist\n");
        return Attributes;
    }


    struct StringList attributes_tmp ;init_StringList(&attributes_tmp,0);
    String_Split(&attributes_tmp,Valueat(&DB->global_attributes,LineNo), "||");

    for(size_t i=0;i<attributes_tmp.length;i++){
        struct String *attri=&attributes_tmp.string[i];
        TrimSpaceString(attri);

        struct StringList parts ;init_StringList(&parts,0);
        String_Split(&parts,attri, "=\"");
        if(parts.length>=2){
            appendto_StringList(&Attributes->labels, &parts.string[0]);
            TrimRightString(&parts.string[1],1);
            appendto_StringList(&Attributes->values, &parts.string[1]);
        }
        free_StringList(&parts);
    }
    free_StringList(&attributes_tmp);
    return Attributes;
}

struct String* MergeNodes(struct Database *DB,int fromNodeId,int toNodeId) {
    //merge attributes only copy if not present
    struct String* Error=malloc(sizeof (struct String)); init_String(Error,0);
    if(Error==NULL){
        fprintf(stderr,"\nError-Memory allocation failed");
        exit(1);
    }
    while(DB->WriteLock) {
        fprintf(stderr,"Waiting for WriteLock-MergeNodes\n");
    }
    struct ResultAttributes* labelsvalues1 = GetAllNodeAttributes(DB, fromNodeId);
    struct ResultAttributes* labelsvalues2 = GetAllNodeAttributes(DB, fromNodeId);

     for(size_t i=0;i<labelsvalues1->labels.length;i++){
          bool label_present = false;
          for(size_t j=0;j<labelsvalues2->labels.length;j++){
              if (strcmp(labelsvalues1->labels.string[i].charbuf,labelsvalues2->labels.string[j].charbuf)==0){
                  label_present = true;
              }
          }
          if (!label_present) {
              UpdateAttributevalue(DB, toNodeId, labelsvalues1->labels.string[i].charbuf, labelsvalues1->values.string[i].charbuf);
          }
     }
     free_Attributes(labelsvalues1);
     free_Attributes(labelsvalues2);

    struct VectorInt *childnodes_From = ChildNodes(DB, fromNodeId);
    struct VectorInt *childnodes_To = ChildNodes(DB, toNodeId);

    for(size_t i=0;i<childnodes_From->length;i++){
        struct String *node_name1 = GetNodeName(DB, childnodes_From->items[i]) ;
        bool child_considered = false ;
        for(size_t j=0;j<childnodes_To->length;j++){
            struct String *node_name2 = GetNodeName(DB, childnodes_To->items[j]);
            if (strcmp(node_name1->charbuf, node_name2->charbuf)==0) {
                struct String* err = MergeNodes(DB, childnodes_From->items[i], childnodes_To->items[j]);
                if (err != NULL) {
                    free_StringReturn(Error);
                    return err;
                }
                child_considered = true;
            }
        }
        if (!child_considered) {
            //copy child
            struct String* content= GetNodeContentsRaw(DB, childnodes_From->items[i]);
            struct ResultStruct *result = InserSubNode(DB, toNodeId, content->charbuf);
            free_StringReturn(content);
            if (result->Error != NULL) {
                StringCharCpy(Error,result->Error);
                free_ResultStruct(result);
                return Error;
            }
            free_ResultStruct(result);
        }
    }


    if (childnodes_From->length == 0 ){
        //<a/> or <a></a>
        //assign value if not present
        struct String *old_value = GetNodeValue(DB, toNodeId);
        if (old_value->length == 0) {
            struct String *new_value = GetNodeValue(DB, fromNodeId);
            struct ResultStruct *result = update_nodevalue(DB, toNodeId, new_value);
            if (result->Error != NULL) {
                StringCharCpy(Error,result->Error);
                free_ResultStruct(result);
                free_StringReturn(new_value);
                free_StringReturn(old_value);
                return Error;
            }
        }
    }
    free(Error->charbuf);
    Error->charbuf=NULL;
    return Error;
}
