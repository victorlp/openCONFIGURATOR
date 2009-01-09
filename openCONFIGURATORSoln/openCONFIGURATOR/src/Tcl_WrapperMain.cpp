
#include "../Include/Declarations.h"

#include <iostream>
#include <fstream>
#include <stdio.h>
#include <errno.h>
#include <string.h>
int Tcl_ImportXML(char*, char*, int, ENodeType);
void Tcl_CreateNode(int, int);
int Tcl_ImportXML(char* fileName, char* errorString, int NodeID, ENodeType NodeType) 
{
	printf("Inside Wrapper cpp\n");
	ImportXML(fileName, errorString, NodeID, NodeType);
	return 1;
}

void Tcl_CreateNode(int NodeID, ENodeType NodeType)
{
	CreateNode(NodeID, NodeType);
}
void Tcl_TransferCDC(char* fileName)
	{
		printf("Tcl_TransferCDC");
		GenerateCDC(fileName);
	}
