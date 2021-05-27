 
#ifndef XMLDB
#define XMLDB
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include "VectorInt.h"
#include "StringList.h"
#include "Hashtable.h"
#include "String.h"
#include <ctype.h>
#include<regex.h>
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
//struct  GetNode_Result *locateNodeLine(struct Database *DB,int parent_nodeLine,char* QUERY ,char* RegExp ,bool onlypath ,bool isRegExp ) ;
int ParentNode(struct Database *DB,int nodeId) ;
struct VectorInt *ChildNodes(struct Database *DB,int nodeId) ;
struct  ResultStruct * GetNode(struct Database *DB,int parent_nodeId ,char*  QUERY_inp) ;
void NodeDebug(struct Database *DB, int nodeId ) ;
void free_DB(struct Database* DB);
int NextNode(struct Database *DB,int nodeId);
#endif
