cmake_minimum_required(VERSION 3.5)

project(example LANGUAGES C)
set(sources "example.c" "xmldb_lib/XmlDB.c" "xmldb_lib/XmlDB.h" "xmldb_lib/VectorInt.h" "xmldb_lib/VectorInt.c" "xmldb_lib/StringList.c" "xmldb_lib/StringList.h" "xmldb_lib/Hashtable.c" "xmldb_lib/Hashtable.h" "xmldb_lib/String.c" "xmldb_lib/String.h")
add_executable(example ${sources} )
