%module openconfigurator
%{
#include "Include/openCONFIGURATOR.h"
#include "Include/Application.h"
#include "Include/BaseIndex.h"
#include "Include/DataTypeCollection.h"
#include "Include/Declarations.h"
#include "Include/globals.h"
#include "Include/IndexCollection.h"
#include "Include/Index.h"
#include "Include/NodeCollection.h"
#include "Include/Node.h"
#include "Include/ObjectDictionary.h"
#include "Include/Socket.h"
#include "Include/StackOperations.h"
#include "Include/SubIndex.h"
#include "Include/TCollection.h"
#include "Include/template.h"
%}

/* Let's just grab the original header file here */

%include "Include/openCONFIGURATOR.h"
%include "Include/Application.h"
%include "Include/BaseIndex.h"
%include "Include/DataTypeCollection.h"
%include "Include/Declarations.h"
%include "Include/globals.h"
%include "Include/IndexCollection.h"
%include "Include/Index.h"
%include "Include/NodeCollection.h"
%include "Include/Node.h"
%include "Include/ObjectDictionary.h"
%include "Include/Socket.h"
%include "Include/StackOperations.h"
%include "Include/SubIndex.h"
%include "Include/TCollection.h"
%include "Include/template.h"

%include cpointer.i
%pointer_functions(CIndex, CIndexp)

%include typemaps.i

%inline %{
extern int Tcl_ImportXML(char*, char*, int, ENodeType);
extern void Tcl_CreateNode(int, ENodeType);
extern void Tcl_TransferCDC(char*);
%}

