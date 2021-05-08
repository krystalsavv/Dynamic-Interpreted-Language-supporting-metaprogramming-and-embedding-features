# Dynamic Interpreted Language supporting metaprogramming and embedding features

## **Requirements:**

* cmake
* bison version 3.0.4
* flex version 2.6.3


## **Setup Instructions for Windows:**

1. Unzip win_flex_bison.rar or download it from https://sourceforge.net/projects/winflexbison/ 

2. Create a folder in C: disk and extract files in there.

3. Go to Environment variables, in User Variables edit the environment variable ***Path*** and add the path of the created folder.

## **Setup Instructions for Ubuntu:**

1. Install cmake, bison and flex via apt-get tool.


## **Language:**

* ### Untyped object-based language with dynamically typed variables (like JavaScript)

* ### Statement based, i.e program code is a series of statements 

* ### Closures are supported 

* ### Simple garbage collection is implemented


    ### **Implementation Overview**

    Given programmer's code an AST (Abstract Syntax Tree) is created.
    While the AST is being traversed 
    execution environment chain is created and maintained,
    functions and variables are introduced to the environment respecting scoping rules and finally program statements are executed. 

    AST and environment chain (and all structures) are implemented using the same dictionary-based class used for the Object language type. 

    ### **Supported Types:**

    * Bool 
    * String 
    * Number 
    * Object
    * Undefined 
    * NIL 
    * ValueReference (embedded system)

    ### **Metaprogramming**

    Using special tags, programmer's code can generate programs during the execution (AST). Also some tags are supported for this generated code execution.

    ### **Embedding**

    With our embedding technique, untyped language code can be executed from C++ code. A programmer can reference to C++ variables from untyped language via simple wrappers (ValueReference). 

    Supported C++ native types:
    
    * Double 
    * String 
    * Bool



#### PS Git accounts were used from all members so commits' signatures are not representative.

#### *This project is implemented by Crystal and Dimi with :heart: for CS540*
