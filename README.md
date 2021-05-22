# About
This is a c lib to treat xml as a native database. You can access, modify or replace a node. This is c version of go project- https://github.com/LIJUCHACKO/XmlDB


## Usage


2. Include Header.

     `include "xmldb_lib/XmlDB.h" `

2. Declare Database.

     `struct Database *DB=init_Database(0); `

3. Load Database.

    `Load_db(DB,"sample.html");`

<u> Content of example.html</u>

```
<!DOCTYPE html>
<html>
   <head>
      <title>This is document title</title>
   </head>	
   <body style="123">
      <h1>This is a heading</h1>
      <p>Hello World!</p>
   </body>	
</html>
```

4. Identifying Nodes using Query.

    `struct ResultStruct * result= GetNode(DB, 0,  "head*/title[This is document title]");`

   -  '*identifiedNodes*' contains list of nodes ids(`<head>`) identified under parent node with id '*0*' (`<html> ..</html>`).
   - '\*'  marks the node we require.
   -  [] encloses attribute or value of the node as additional search criteria.

5. Getting Content of identified nodes.
```
    for(int i=0;i<result->nodeids.length;i++){
        printf("\n%s", GetNodeContents(DB, result->nodeids.items[i])->charbuf);
    }
```
<u>Output</u>
```
    <head>
          <title>This is document title</title>
    </head>
```
6. Getting Value of identified nodes.

```
   result= GetNode(DB, 0, "head/title*");
    for(int i=0;i<result->nodeids.length;i++){
        printf("\n%s", GetNodeValue(DB, result->nodeids.items[i])->charbuf);
    }
```
<u>Output</u>
```
This is document title
```
7. Searching using attribute value.

    `result = GetNode(DB, 0, "body*[style=\"123\"]/h1");`

8. Updating node

```
	printf("\n### Updating node value ##\n");
    result =  GetNode(DB, 0, "head/title");
    for(int i=0;i<result->nodeids.length;i++){
        UpdateNodevalue(DB, result->nodeids.items[i], "test_new");
        printf("After updating value\n");
        printf("\n%s", GetNodeContents(DB, result->nodeids.items[i])->charbuf);
    }
	printf("\n### Updating node ##\n");
    result = GetNode(DB, 0, "head/title");
    for(int i=0;i<result->nodeids.length;i++){
        struct ResultStruct *newnodes = ReplaceNode(DB, result->nodeids.items[i], "<title>test</title>");
        printf("After updation\n");
        printf("old node value- %s", GetNodeValue(DB, result->nodeids.items[i])->charbuf); //no output, existing id is removed and new id added
        printf("\nnew node value- %s", GetNodeValue(DB, newnodes->nodeids.items[0])->charbuf);
    }
```
<u>Output</u>
```
### Updating node value##
After updation
Warning :node  doesnot exist
old node value- 
new node value- test
### Updating node attribute##

123
after updating Attribute-
<body style="test2">
  <h1>This is a heading with style</h1>
  <p>Hello World!</p>
</body>

after adding Attribute-
<body style="test2" label="value">
  <h1>This is a heading with style</h1>
  <p>Hello World!</p>
</body>
```

9. Recursive search.

     `result = GetNode(DB, 0, "../h1");`



## Working/Software Design

#### INPUT(sample)

```
<!DOCTYPE html>
<html>
   <head><title>This is document title</title></head>	
   <body style="123">
      <h1>This is a heading</h1>
      <p>Hello World!</p>
   </body>	
</html>
```
#### After parsing- global variables are filled as shown

| global_dbLines                         |   global_ids |  global_paths |  global_values         |  global_attributes      |
|----------------------------------------|--------------|---------------|------------------------|-------------------------|
|` <!DOCTYPE html> `                     |  -1          |               |                        |                         |
|` <head> `                              |   0          | /head         |                        |                         |
|` <title>This is document title</title>`|   1          | /head/title   | This is document title |                         |
| ` <\head> `                            |   2          | /head/~       |                        |                         |
| `<body style="123" font="arial">`      |   3          | /body         |                        | style="123"\|\| font="arial"|
| `<h1>This is a heading</h1>  `         |   4          | /body/h1      |                        |                         |
| `<p>Hello World!</p> `                 |   5          | /body/p       | This is a heading      |                         |
| `</body>`                              |   6          | /body/~       | Hello World!           |                         |
|                                        |              |               |                        |                         |

Note:- If a new node is inserted in between, global_id '7' will be assigned to it. global_id will be unique and will be retained till the node is deleted.


#### 'nodeNoToLineno' contains  line no for every global id.

|   index/global_id|  nodeNoToLineno[index]|
|------------------|-----------------------|
|     0            |    1                  |
|     1            |    2                  |
|     2            |    3                  |
|     3            |    4                  |





#### 'Nodeendlookup' contains global_id of the node end.

|   index/global_id|  Nodeendlookup[index]|
|------------------|----------------------|
|     0            |    2                 |
|     1            |    1                 |
|     2            |    2                 |
|     3            |    6                 |
|     4            |    4                 |
|     5            |    5                 |


##### 'pathKeylookup'  is for quick lookup for line nos corresponding to innermost node.

-Say the path is '/head/title' then hash is calculated for 'title'.

-Corresponding to the hash value a list of global_ids ,arranged in the increasing order of lineno, is stored.


| Hash no |  global id list corresponding to the hash |
|---------|-------------------------------------------|
|    0    |                                           |
|    1    |                                           |
|    2    |   [id1,id2,id3,id4]                       |


-Binary search is utilised  to  find ids, under a parent node, from this list.
