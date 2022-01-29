#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

#include "xmldb_lib/XmlDB.h"
#include <ctype.h>
#include<regex.h>

int main()
{

   struct Database *DB=init_Database(0);

    DB->Debug_enabled=false;
    Load_db(DB,"sample.html");

    printf("\n###Identifying Nodes using Query##");
    struct ResultStruct * result= GetNode(DB, 0,  "head*/title[This is document title]");
    //for _, node := range identifiedNodes {
    for(size_t i=0;i<result->nodeids.length;i++){
        printf("\n%s", GetNodeContents(DB, result->nodeids.items[i])->charbuf);
    }
    free_ResultStruct(result);/*Do not forget to free memory if you don't want memory leakage  */
    printf("\n###Getting Value of identified nodes.##");
    result= GetNode(DB, 0, "head/title*");
    for(size_t i=0;i<result->nodeids.length;i++){
        printf("\n%s", GetNodeValue(DB, result->nodeids.items[i])->charbuf);
    }
    printf("\n###Searching using attribute value.##");
    result = GetNode(DB, 0, "body*[style=\"123\"]/h1");
    for(size_t i=0;i<result->nodeids.length;i++){
        printf("\n%s", GetNodeContents(DB, result->nodeids.items[i])->charbuf);
    }
    printf("\n###[2]Searching using attribute value.##");
    result =  GetNode(DB, 0, "<x>*[style=\"123\"]/h1");
    for(size_t i=0;i<result->nodeids.length;i++){
        printf("\n%s", GetNodeContents(DB, result->nodeids.items[i])->charbuf);
    }

    printf("\n### Updating node value ##\n");
    result =  GetNode(DB, 0, "head/title");
    for(size_t i=0;i<result->nodeids.length;i++){
        UpdateNodevalue(DB, result->nodeids.items[i], "test_new");
        printf("After updating value\n");
        printf("\n%s", GetNodeContents(DB, result->nodeids.items[i])->charbuf);
    }
    struct ResultStruct * result2 =  GetNode(DB, 0, "head*");
    struct VectorInt *ids = ChildNodes(DB, result2->nodeids.items[0]);
    printf("\nchildren");

    for(size_t i=0;i<ids->length;i++){
        printf("\n%s", GetNodeContents(DB, ids->items[i])->charbuf);
    }
    printf("\nDebug head\n");
    NodeDebug(DB,result2->nodeids.items[0]);
    printf("\n### Updating node ##\n");
    struct ResultStruct *result3 = GetNode(DB, 0, "head/title");
    for(size_t i=0;i<result3->nodeids.length;i++){
        struct ResultStruct *newnodes = ReplaceNode(DB, result3->nodeids.items[i], "<title>test</title>");
        printf("After updation\n");
        printf("old node value- %s", GetNodeValue(DB, result3->nodeids.items[i])->charbuf); //no output, existing id is removed and new id added
        printf("\nnew node value- %s", GetNodeValue(DB, newnodes->nodeids.items[0])->charbuf);
    }
    printf("\nAfter updation- research\n");
    result =  GetNode(DB, 0, "head/title");
    for(size_t i=0;i<result->nodeids.length;i++){
        printf("\n%s", GetNodeContents(DB, result->nodeids.items[i])->charbuf);
    }
    printf("\n### Updating node attribute##\n");
    result = GetNode(DB, 0, "<x>*[style=\"123\"]/h1");
    for(size_t i=0;i<result->nodeids.length;i++){
        printf("\n%s", GetNodeAttribute(DB, result->nodeids.items[i], "style")->charbuf);
        UpdateAttributevalue(DB, result->nodeids.items[i], "style", "test2");
        printf("\nafter updating Attribute-\n%s", GetNodeContents(DB, result->nodeids.items[i])->charbuf);
        UpdateAttributevalue(DB, result->nodeids.items[i], "label", "value");
        printf("\nafter adding Attribute-\n%s", GetNodeContents(DB, result->nodeids.items[i])->charbuf);
        UpdateAttributevalue(DB, result->nodeids.items[i], "font", "");
        printf("\nafter removing Attribute-\n%s", GetNodeContents(DB, result->nodeids.items[i])->charbuf);

    }
    printf("\n### Recursive Search##\n");
    result = GetNode(DB, 0, "../h1");
    for(size_t i=0;i<result->nodeids.length;i++){
        printf("\n%s", GetNodeContents(DB, result->nodeids.items[i])->charbuf);
    }

    NodeDebug(DB,0);
     printf("\n---------\n");
    CutPasteAsSubNode(DB,8,1);
    NodeDebug(DB,0);
    SaveAs_DB(DB, "sample_mod.html" ) ;
    free(DB);
    return 0;
}
