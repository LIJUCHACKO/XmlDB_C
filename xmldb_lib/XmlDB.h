/*
** This file is the part of XmlDBlib project, an easy to use xmlparser written from scratch.
**
** The author disclaims copyright to this source code.  In place of
** a legal notice, here is a blessing:
**    May you share freely, never taking more than you give.
**
*/
#ifndef XMLDB
#define XMLDB
#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include "VectorInt.h"
#include "StringList.h"
#include "Hashtable.h"
#include "String.h"
#include <ctype.h>
#if defined(__linux__)
#include<regex.h>
#endif
#include "SegmentedStringList.h"

#define ERRORLENGTH 2000
struct ResultStruct {
    struct VectorInt nodeids;
    struct StringList labelvalues;
    char* Error;
};

struct suspectedLinenos_Result{
    struct VectorInt suspectedLineStarts;
    struct VectorInt suspectedLineEnds;
};
struct compare_path_Result{
    struct StringList label;
    struct StringList value;
    bool status;
};
struct ResultAttributes {
    struct StringList values;
    struct StringList labels;
};

struct Database  {
    char  filename[2000];
    struct String  removeattribute ;
    struct VectorInt global_ids ;
    struct VectorInt deleted_ids;

    struct SegmentedStringList global_paths;
    struct  SegmentedStringList global_dbLines;
    struct SegmentedStringList global_values;
    struct SegmentedStringList global_attributes;

    int global_lineLastUniqueid;
    bool Debug_enabled;
    bool libreofficemod;
    struct VectorInt nodeNoToLineno;
    struct Hashtable pathKeylookup;
    struct VectorInt  Nodeendlookup;
    int pathIdStack[2000];
    int pathIdStack_index ;
    int reference_linenotoinsert;
    int retainid;
    int startindex;
    struct String path;
    int maxInt;
    int maxHashValue;
    bool WriteLock;
};
struct VectorInt* Get_common(struct VectorInt* set1 ,struct VectorInt*  set2 );
void formatxml(struct StringList* newlines,struct StringList* lines);
void free_compare_path_Result(struct compare_path_Result * v);
void free_suspectedLinenos_Result(struct suspectedLinenos_Result * v);
int NodeLine(struct Database *DB, int nodeId );
int NodeEnd(struct Database *DB, int nodeId );
struct Database* init_Database(int maxNoofLines);
bool SaveAs_DB(struct Database *DB, char *filename );
bool Save_DB(struct Database *DB);
bool Load_db(struct Database *DB , char* filename);
struct String *GetNodeAttribute(struct Database *DB , int nodeId , char *labelchar );
struct String *GetNodeValue(struct Database *DB ,int nodeId);
struct String *GetNodeName(struct Database *DB, int nodeId ) ;
struct String *GetNodeContentsRaw(struct Database *DB, int nodeId ) ;
struct String *GetNodeContents(struct Database *DB, int nodeId );
bool validatexml(struct String *content ) ;
struct ResultStruct *UpdateNodevalue(struct Database *DB, int nodeId, char* new_valuechar ) ;
struct ResultStruct * UpdateAttributevalue(struct Database *DB, int nodeId,char* labelchar ,char* valuechar ) ;
struct VectorInt* RemoveNode(struct Database *DB, int nodeId) ;
struct ResultStruct *ReplaceNode(struct Database *DB, int nodeId, char *sub_xmlchar );
struct ResultStruct * InserSubNode(struct Database *DB, int nodeId, char* sub_xmlchar );
struct ResultStruct * AppendAfterNode(struct Database *DB, int nodeId, char *sub_xmlchar ) ;
struct ResultStruct * AppendBeforeNode(struct Database *DB, int nodeId, char *sub_xmlchar )  ;
int LocateRequireParentdNode(struct Database *DB ,int parent_nodeLine ,struct String* RequiredPath ,int LineNo_inp );
void free_ResultStruct(struct ResultStruct *v);
bool Regular_expmatch(char* String,char*Regexpr);
int ParentNode(struct Database *DB,int nodeId) ;
struct VectorInt *ChildNodes(struct Database *DB,int nodeId) ;
struct  ResultStruct * GetNode(struct Database *DB,int parent_nodeId ,char*  QUERY_inp) ;
void NodeDebug(struct Database *DB, int nodeId ) ;
void free_DB(struct Database* DB);
int NextNode(struct Database *DB,int nodeId);
struct String *Dump_DB(struct Database *DB);
void Load_dbcontent(struct Database *DB,struct StringList *xmllines) ;
bool IslowestNode(struct Database *DB, int nodeId);
//struct String*  CutPasteAsSubNode(struct Database *DB ,int UnderId,int nodeId);
void ReplacewithHTMLSpecialEntities(struct Database *DB,struct String* Result,struct String* input );
#endif
