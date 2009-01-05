///////////////////////////////////////////////////////////////////////////////////////////////
//
//  $Header: $
//
// NAME:  ClassNAME
//
// BASE  CLASSES: none
//  
// PURPOSE:  purpose description
//
// AUTHOR:  
//
//  COPYRIGHT NOTICE:
//
//	****************************************************************************

// (c) Kalycito Infotech Private Limited
//
//  Project:      openCONFIGURATOR 
//
//  Description:  
//
//
//  License:
//
//    Redistribution and use in source and binary forms, with or without
//    modification, are permitted provided that the following conditions
//    are met:
//
//    1. Redistributions of source code must retain the above copyright
//       notice, this list of conditions and the following disclaimer.
//
//    2. Redistributions in binary form must reproduce the above copyright
//       notice, this list of conditions and the following disclaimer in the
//       documentation and/or other materials provided with the distribution.
//
//    3. Neither the name of Kalycito Infotech Private Limited nor the names of 
//       its contributors may be used to endorse or promote products derived
//       from this software without prior written permission. For written
//       permission, please contact info@kalycito.com.
//
//    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
//    "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
//    LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
//    FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
//    COPYRIGHT HOLDERS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
//    INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
//    BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
//    LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
//    CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
//    LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
//    ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
//    POSSIBILITY OF SUCH DAMAGE.
//
//    Severability Clause:
//
//        If a provision of this License is or becomes illegal, invalid or
//        unenforceable in any jurisdiction, that shall not affect:
//        1. the validity or enforceability in that jurisdiction of any other
//           provision of this License; or
//        2. the validity or enforceability in other jurisdictions of that or
//           any other provision of this License.
//
//****************************************************************************/
//
//  REVISION HISTORY:
// $Log:      $
///////////////////////////////////////////////////////////////////////////////////////////////

#include "../Include/openCONFIGURATOR.h"
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <libxml\xmlreader.h>
#include <errno.h>

char* strerr;
int LastIndexParsed=0;

void LoadObjectDictionary()
	{
	
		//Parse the ObjectDictionary xml and creates the objectDictionary object
		
	}
static void setIndexAttributes(xmlTextReaderPtr reader, CIndex* objIndex)
	{
		const xmlChar* name,*value;
		//Retrieve the name and Value of an attribute
		value = xmlTextReaderConstValue(reader);
		name =xmlTextReaderConstName(reader);							

		if(strcmp(ConvertToUpper((char*)name), "INDEX")==0)						
		objIndex->setIndex((char*)value);

		if(strcmp(ConvertToUpper((char*)name), "NAME")==0)					
		objIndex->setName((char*)value);

		if(strcmp(ConvertToUpper((char*)name), "OBJECTTYPE")==0)					
		objIndex->setObjectType((int)value);

		if(strcmp(ConvertToUpper((char*)name), "LOWLIMIT")==0)					
		objIndex->setLowLimit((char*)value);

		if(strcmp(ConvertToUpper((char*)name), "HIGHLIMIT")==0)					
		objIndex->setHighLimit((char*)value);

		if(strcmp(ConvertToUpper((char*)name), "ACCESSTYPE")==0)					
		objIndex->setAccessType((char*)value);

		if(strcmp(ConvertToUpper((char*)name), "PDOMAPPING")==0)					
		objIndex->setPDOMapping((char*)value);

		if(strcmp(ConvertToUpper((char*)name), "DEFAULTVALUE")==0)					
		objIndex->setDefaultValue((char*)value);

		if(strcmp(ConvertToUpper((char*)name), "ACTUALVALUE")==0)					
		objIndex->setActualValue((char*)value);

		if(strcmp(ConvertToUpper((char*)name), "DATATYPE")==0)					
		objIndex->setDataType((char*)value);							
																	
	}
static void setSubIndexAttributes(xmlTextReaderPtr reader, CSubIndex* objSubIndex)
	{
		const xmlChar* name,*value;
		//Retrieve the name and Value of an attribute
		value = xmlTextReaderConstValue(reader);
		name =xmlTextReaderConstName(reader);							

		if(strcmp(ConvertToUpper((char*)name), "SUBINDEX")==0)						
		objSubIndex->setIndex((char*)value);

		if(strcmp(ConvertToUpper((char*)name), "NAME")==0)					
		objSubIndex->setName((char*)value);

		if(strcmp(ConvertToUpper((char*)name), "OBJECTTYPE")==0)					
		objSubIndex->setObjectType((int)value);

		if(strcmp(ConvertToUpper((char*)name), "LOWLIMIT")==0)					
		objSubIndex->setLowLimit((char*)value);

		if(strcmp(ConvertToUpper((char*)name), "HIGHLIMIT")==0)					
		objSubIndex->setHighLimit((char*)value);

		if(strcmp(ConvertToUpper((char*)name), "ACCESSTYPE")==0)					
		objSubIndex->setAccessType((char*)value);

		if(strcmp(ConvertToUpper((char*)name), "PDOMAPPING")==0)					
		objSubIndex->setPDOMapping((char*)value);

		if(strcmp(ConvertToUpper((char*)name), "DEFAULTVALUE")==0)					
		objSubIndex->setDefaultValue((char*)value);

		if(strcmp(ConvertToUpper((char*)name), "ACTUALVALUE")==0)					
		objSubIndex->setActualValue((char*)value);

		if(strcmp(ConvertToUpper((char*)name), "DATATYPE")==0)					
		objSubIndex->setDataType((char*)value);							
																	
	}
/**************************************************************************************************
	* Function Name: ImportXML
    * Description: Imports the XML file
/****************************************************************************************************/
static void ImportXML(char* fileName,char* filePath,char* errorString,int NodeID,int NodeType)
	{
		/*char* fileLocation;
		fileLocation= fileName + filePath;*/
		//fileLocation ="E:\\Kalycito\\checkout\\Source\\SharedLibrary\\openCONFIGURATORSoln\\XMLParser\\12345678_MyName.xml";
		parseFile("E:\\Kalycito\\checkout\\Source\\SharedLibrary\\openCONFIGURATORSoln\\XMLParser\\12345678_MyName.xml",(ENodeType)NodeType,NodeID);


		//Cleanup function for the XML library.
   
		 xmlCleanupParser();
    /*
     * this is to debug memory for regression tests
     */
    xmlMemoryDump();
    /*return(0);*/


	}
/**************************************************************************************************
	* Function Name: processNode
    * Description: Process the Node value,Name and its attributes
/****************************************************************************************************/
void processNode(xmlTextReaderPtr reader,ENodeType NodeType,int NodeID)
{
    const xmlChar *name, *value;
	CIndex objIndex;
	char* AttributeName;
	CNodeCollection *objNodeCollection;
	CNode objNode;
	CSubIndex objSubIndex;
	
	
    name = xmlTextReaderConstName(reader);
    if (name == NULL)
	name = BAD_CAST "--";
	
    value = xmlTextReaderConstValue(reader);
	//If the NodeTYPE is ELEMENT
	if( xmlTextReaderNodeType(reader)==1)
		{
			if(strcmp((char*)name,"Object")==0)
				{
					objNodeCollection= CNodeCollection::getNodeColObjectPointer();
					CIndexCollection* objIndexCollection;
					if (xmlTextReaderHasAttributes(reader)==1)
						{						
							while(xmlTextReaderMoveToNextAttribute(reader))
								{
									setIndexAttributes(reader, &objIndex);																									
								}
						}
						objNode = objNodeCollection->getNode(NodeType,NodeID);							
						objIndexCollection = objNode.getIndexCollection();

						//Add Index object to the IndexCollection
						objIndexCollection->addIndex(objIndex);

						LastIndexParsed = objIndexCollection->getNumberofIndexes()-1;
						
				}
			if(strcmp((char*)name,"SubObject")==0 )
				{
					objNodeCollection = CNodeCollection::getNodeColObjectPointer();
					CIndexCollection* objIndexCollection;
					if (xmlTextReaderHasAttributes(reader) ==1)
						{
							
							while(xmlTextReaderMoveToNextAttribute(reader))
								{
									setSubIndexAttributes(reader,&objSubIndex);																									
								}
						}
						CIndex* objIndexPtr;
						objNode =objNodeCollection->getNode(NodeType,NodeID);
							
						objIndexCollection =objNode.getIndexCollection();
						objIndexPtr =objIndexCollection->getIndex(LastIndexParsed);
						objIndexPtr->addSubIndex(objSubIndex);
						
				}	
			}

			
}
/**************************************************************************************************
	* Function Name: parseFile
    * Description: Parses the XML file
/****************************************************************************************************/
void parseFile(char* filename,ENodeType  NodeType,int NodeID) 
{
    xmlTextReaderPtr reader;
    int ret;

	strerr ="";
    reader = xmlReaderForFile(filename, NULL, 0);
    if (reader != NULL)
	{
        ret = xmlTextReaderRead(reader);
        while (ret == 1)
		{
			
		processNode(reader,NodeType,NodeID );
            ret = xmlTextReaderRead(reader);
        }
        xmlFreeTextReader(reader);
        if (ret != 0)
		{
            fprintf(stderr, "%s : failed to parse\n", filename);
			/*strerr= strerror(errno);
			strerr =" Error:" +strerr;*/
        }
    }
	else 
		{
			/*strerr=strerror(errno);
			strerr =" Error:" +strerr;*/
       /* fprintf(stderr, "Unable to open %s\n", filename);*/
		}
 }
/**************************************************************************************************
	* Function Name: CreateTree
    * Description:
/****************************************************************************************************/
void CreateTree()
	{
		//objNodeCollection= CNodeCollection::getNodeColObject();
		
	}
/**************************************************************************************************
	* Function Name: CreateNode
    * Description:
/****************************************************************************************************/
static void CreateNode(int NodeID, ENodeType NodeType)
	{
		CNode objNode;
		CNodeCollection *objNodeCollection;

		objNode.setNodeId(NodeID);
		objNode.setNodeType(NodeType);
		
		objNodeCollection = CNodeCollection::getNodeColObjectPointer();
		objNodeCollection->addNode(objNode);

		
	}
/**************************************************************************************************
	* Function Name: GenerateCDC
    * Description: Generates the CDC file
/****************************************************************************************************/
void GenerateCDC(char* fileName, char* filePath)
	{
		CNode objMNNode;		
		char* fullFileName;

		//get the MN Node object from the NodeCollection
		/*fullFileName = fileName + filePath;*/
		ofstream file;
		//file.open(fullFileName);


		//char *WriteBuffer;		

	}
/**************************************************************************************************
	* Function Name: GenerateMNOBD
    * Description: Generates the MN Object Dictionary
/****************************************************************************************************/
//void GenerateMNOBD()
//	{
//		CNodeCollection *objNodeCollection;
//		objNodeCollection= CNodeCollection::getNodeColObject();
//
//		//get the MN Node
//		CNode objMNNode;
//		
//		objMNNode=objNodeCollection->getNode(MN,249);
//
//		//get the Index collection of MN Object
//		CIndexCollection *objIndexCollection;
//		objIndexCollection = objMNNode->getIndexCollection();
//
//		//loop through the Indexes of IndexCollection
//		int count=0;
//		for (count=0;count < objIndexCollection->getNumberofIndexes(); count++)
//			{
//				CIndex objIndex;
//				objIndex = objIndexCollection->getIndex(count);
//				objIndex.getIndex();
//				
//			}
//
//
//
//
//	}
int main(int argc, char **argv) {
   /* if (argc != 2)
        return(1);*/

    /*
     * this initialize the library and check potential ABI mismatches
     * between the version it was compiled for and the actual shared
     * library used.
     */
   
    //parseFile("E:\\Kalycito\\checkout\\Source\\SharedLibrary\\openCONFIGURATORSoln\\XMLParser\\12345678_MyName.xml",NodeType,NodeID);

	CNodeCollection *objNodeCollection;
	CNodeCollection objNodecol;
	objNodecol = CNodeCollection::getNodeColObject();
	objNodeCollection= CNodeCollection::getNodeColObjectPointer();
	CNode objNode;
	objNode.setNodeId(1);
	objNode.setNodeType(CN);

	objNode.CreateIndexCollection();
	objNodeCollection->addNode(objNode);
	objNodecol.addNode(objNode);
	printf("Total Nodes %d\n",objNodecol.getNumberOfNodes());
	printf("Total Nodes %d\n",objNodeCollection->getNumberOfNodes());

	parseFile("E:\\Kalycito\\checkout\\Source\\SharedLibrary\\openCONFIGURATORSoln\\XMLParser\\12345678_MyName.xml",CN,1);

	/* Show the Indexes*/
	int i;
	CIndexCollection* obj;
	obj =objNode.getIndexCollection();
	for (i=0;i<obj->getNumberofIndexes();i++)
		{
			CIndex* objIndex;
			objIndex = obj->getIndex(i);
			char* index;
			index=objIndex->getIndex();
			printf("Index: %s Name: %s\n",index,objIndex->getName());
		}
	
	
			CIndex *objIndex;
			objIndex = obj->getIndex(8);
			CSubIndex objsub;
			//index=objIndex.getNumberofSubIndexes();
			printf("Number of subindexes: %d",objIndex->getNumberofSubIndexes());
			//printf("Index: %s Name: %s\n",index,objIndex.getName());
	
    /*
     * Cleanup function for the XML library.
     */
    xmlCleanupParser();
    /*
     * this is to debug memory for regression tests
     */
    xmlMemoryDump();
    return(0);
}
