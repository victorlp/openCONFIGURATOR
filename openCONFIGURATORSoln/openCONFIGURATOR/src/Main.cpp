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
//#pragma comment(lib, "..//\\debug\\libxml2.lib")
#include "../Include/openCONFIGURATOR.h"
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <libxml\xmlreader.h>
#include <errno.h>

const char* RPDO_COMM_PARAM="1400";
const char* RPDO_MAPP_PARAM="1600";
const char* TPDO_COMM_PARAM="1800";
const char* TPDO_MAPP_PARAM="1A00";

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
			{
				objIndex->setIndexValue((char*)value);
				char* data=(char*)value;
				if(strcmp(data,RPDO_COMM_PARAM )==0 ||strcmp(data,RPDO_MAPP_PARAM) ==0)
					{
						objIndex->setPDOType(RPDO);
					}
				else if(strcmp(data,TPDO_COMM_PARAM )==0 ||strcmp(data,TPDO_MAPP_PARAM) ==0)
					{
						objIndex->setPDOType(TPDO);
					}

			}

		else if(strcmp(ConvertToUpper((char*)name), "NAME")==0)					
		objIndex->setName((char*)value);

		else if(strcmp(ConvertToUpper((char*)name), "OBJECTTYPE")==0)					
		objIndex->setObjectType(atoi((const char*)value));

		else if(strcmp(ConvertToUpper((char*)name), "LOWLIMIT")==0)					
		objIndex->setLowLimit((char*)value);

		else if(strcmp(ConvertToUpper((char*)name), "HIGHLIMIT")==0)					
		objIndex->setHighLimit((char*)value);

		else if(strcmp(ConvertToUpper((char*)name), "ACCESSTYPE")==0)					
		objIndex->setAccessType((char*)value);

		else if(strcmp(ConvertToUpper((char*)name), "PDOMAPPING")==0)					
		objIndex->setPDOMapping((char*)value);

		else if(strcmp(ConvertToUpper((char*)name), "DEFAULTVALUE")==0)					
		objIndex->setDefaultValue((char*)value);

		else if(strcmp(ConvertToUpper((char*)name), "ACTUALVALUE")==0)					
		objIndex->setActualValue((char*)value);

		else if(strcmp(ConvertToUpper((char*)name), "DATATYPE")==0)					
		objIndex->setDataType((char*)value);							
																	
	}
static void setSubIndexAttributes(xmlTextReaderPtr reader, CSubIndex* objSubIndex)
	{
		const xmlChar* name,*value;
		//Retrieve the name and Value of an attribute
		value = xmlTextReaderConstValue(reader);
		name =xmlTextReaderConstName(reader);							

		if(strcmp(ConvertToUpper((char*)name), "SUBINDEX")==0)						
		objSubIndex->setIndexValue((char*)value);

		else if(strcmp(ConvertToUpper((char*)name), "NAME")==0)					
		objSubIndex->setName((char*)value);

		else if(strcmp(ConvertToUpper((char*)name), "OBJECTTYPE")==0)					
		objSubIndex->setObjectType((int)value);

		else if(strcmp(ConvertToUpper((char*)name), "LOWLIMIT")==0)					
		objSubIndex->setLowLimit((char*)value);

		else if(strcmp(ConvertToUpper((char*)name), "HIGHLIMIT")==0)					
		objSubIndex->setHighLimit((char*)value);

		else if(strcmp(ConvertToUpper((char*)name), "ACCESSTYPE")==0)					
		objSubIndex->setAccessType((char*)value);

		else if(strcmp(ConvertToUpper((char*)name), "PDOMAPPING")==0)					
		objSubIndex->setPDOMapping((char*)value);

		else if(strcmp(ConvertToUpper((char*)name), "DEFAULTVALUE")==0)					
		objSubIndex->setDefaultValue((char*)value);

		else if(strcmp(ConvertToUpper((char*)name), "ACTUALVALUE")==0)					
		objSubIndex->setActualValue((char*)value);

		else if(strcmp(ConvertToUpper((char*)name), "DATATYPE")==0)					
		objSubIndex->setDataType((char*)value);							
																	
	}
static void setDataTypeAttributes(xmlTextReaderPtr reader ,DataType* objDataType)
	{
		const xmlChar* name,*value;
		int ret;

		objDataType->DataTypeValue = new char;
		objDataType->Name = new char;
		objDataType->DataSize = new char;

		
		//Retrieve the name and Value of an attribute
		value = xmlTextReaderConstValue(reader);
		name =xmlTextReaderConstName(reader);							

		if(strcmp(ConvertToUpper((char*)name), "DATATYPE")==0)						
		{
			strcpy(objDataType->DataTypeValue, (char*)value);
		}



		//Read the Equivalent name of a datatype
		ret = xmlTextReaderRead(reader);

		value = xmlTextReaderConstValue(reader);
		name =xmlTextReaderConstName(reader);
		
		strcpy(objDataType->Name, (char*)name);
		
		if((strcmp(ConvertToUpper(objDataType->Name),"UNSIGNED8")==0) ||
			(strcmp(ConvertToUpper(objDataType->Name),"BOOLEAN")==0 ) ||
			(strcmp(ConvertToUpper(objDataType->Name),"INTEGER8")==0 ))
				strcpy(objDataType->DataSize,"0001");

		if((strcmp(ConvertToUpper(objDataType->Name),"UNSIGNED16")==0) ||
			(strcmp(ConvertToUpper(objDataType->Name),"INTEGER16")==0 ))
			strcpy(objDataType->DataSize,"0002");

		if((strcmp(ConvertToUpper(objDataType->Name),"UNSIGNED32")==0) ||
			(strcmp(ConvertToUpper(objDataType->Name),"INTEGER32")==0 ) ||
			(strcmp(ConvertToUpper(objDataType->Name),"REAL32")==0 ))
			strcpy(objDataType->DataSize,"0004");
		
	}
/**************************************************************************************************
	* Function Name: ImportXML
    * Description: Imports the XML file
/****************************************************************************************************/
int ImportXML(char* fileName, char* errorString, int NodeID, ENodeType NodeType)
	{
		/*char* fileLocation;
		fileLocation= fileName + filePath;*/
		//fileLocation ="E:\\Kalycito\\checkout\\Source\\SharedLibrary\\openCONFIGURATORSoln\\XMLParser\\12345678_MyName.xml";
		//parseFile("E:\\Kalycito\\checkout\\Source\\SharedLibrary\\openCONFIGURATORSoln\\XMLParser\\12345678_MyName_BitCoding.xml",(ENodeType)NodeType,NodeID);

		parseFile(fileName,NodeID,NodeType);
		//Cleanup function for the XML library.
   
		 xmlCleanupParser();
    /*
     * this is to debug memory for regression tests
     */
    xmlMemoryDump();
    
		return(1);


	}
/**************************************************************************************************
	* Function Name: processNode
    * Description: Process the Node value,Name and its attributes
/****************************************************************************************************/
void processNode(xmlTextReaderPtr reader,ENodeType NodeType,int NodeIndex)
{
    const xmlChar *name, *value;
	
	char* AttributeName;
	CNodeCollection *objNodeCollection;
	CNode objNode;
	
	
	
    name = xmlTextReaderConstName(reader);
    if (name == NULL)
	name = BAD_CAST "--";
	
    value = xmlTextReaderConstValue(reader);
	//If the NodeTYPE is ELEMENT
	if( xmlTextReaderNodeType(reader)==1)
		{
			if(strcmp(((char*)name),"defType")==0)
				{
					objNodeCollection= CNodeCollection::getNodeColObjectPointer();
					CDataTypeCollection* objDataTypeCollection;					
					DataType objDataType;
					if (xmlTextReaderHasAttributes(reader)==1)
						{						
							while(xmlTextReaderMoveToNextAttribute(reader))
								{
									setDataTypeAttributes(reader, &objDataType);																									
								}
						}

					objNode = objNodeCollection->getNode(NodeType,NodeIndex);							
					objDataTypeCollection = objNode.getDataTypeCollection();
					objDataTypeCollection->addDataType(objDataType);
						

				}

			if(strcmp(((char*)name),"Object")==0)
				{
					objNodeCollection= CNodeCollection::getNodeColObjectPointer();
					CIndexCollection* objIndexCollection;
					CIndex objIndex;

					
					objNode = objNodeCollection->getNode(NodeType, NodeIndex);	
						//Set the NodeID
						objIndex.setNodeID(objNode.getNodeId());

					if (xmlTextReaderHasAttributes(reader)==1)
						{						
							while(xmlTextReaderMoveToNextAttribute(reader))
								{
									setIndexAttributes(reader, &objIndex);																									
								}
						}
						
						objIndexCollection = objNode.getIndexCollection();
						
						//Add Index object to the IndexCollection
						objIndexCollection->addIndex(objIndex);

						LastIndexParsed = objIndexCollection->getNumberofIndexes()-1;
						
				}
			if(strcmp(((char*)name),"SubObject")==0 )
				{
					objNodeCollection = CNodeCollection::getNodeColObjectPointer();
					CIndexCollection* objIndexCollection;
					CSubIndex objSubIndex;
					objNode =objNodeCollection->getNode(NodeType,NodeIndex);
						//Set the NodeID
						objSubIndex.setNodeID(objNode.getNodeId());		

					if (xmlTextReaderHasAttributes(reader) ==1)
						{
							
							while(xmlTextReaderMoveToNextAttribute(reader))
								{
									setSubIndexAttributes(reader,&objSubIndex);																									
								}
						}
						CIndex* objIndexPtr;
								
				
						objIndexCollection =objNode.getIndexCollection();
						objIndexPtr =objIndexCollection->getIndex(LastIndexParsed);
										
						if(strcmp(objIndexPtr->getIndexValue(),"1A00")==0)
							{
								if(strcmp(objSubIndex.getIndexValue(),"33")==0)
									{	
									printf("%d",objIndexPtr->getNumberofSubIndexes());
									}
							}
						printf("LastIndexParsed: %s\n",objIndexPtr->getIndexValue());
						printf("SubIndex value: %s \n",objSubIndex.getIndexValue());
						objIndexPtr->addSubIndex(objSubIndex);
						
						
				}	
			}

			
}
/**************************************************************************************************
	* Function Name: parseFile
    * Description: Parses the XML file
/****************************************************************************************************/
void parseFile(char* filename, int NodeIndex, ENodeType  NodeType) 
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
			
		processNode(reader,NodeType,NodeIndex );
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
		
		objNode.CreateIndexCollection();
		objNode.CreateDataTypeCollection();

		objNodeCollection = CNodeCollection::getNodeColObjectPointer();
		objNodeCollection->addNode(objNode);

		

		
	}

/**************************************************************************************************
	* Function Name: GetIndexData
   * Description: 
/****************************************************************************************************/
char* GetIndexData(CIndex* objIndex,char* Buffer1)
	{
			char* Buffer;
			int len;

			//Get the Index Value
			Buffer = (char *) malloc(sizeof(objIndex->getIndexValue()));
			strcat(Buffer1,objIndex->getIndexValue());

			//Place a tab
			strcat(Buffer1,"\t");

			//If subindexes are none, add "00"
			if(objIndex->getNumberofSubIndexes() ==0)
				strcat(Buffer1,"00");

			strcat(Buffer1,"\t");

			//Add datatype
			DataType dt;			
			dt = objIndex->getDataType();
			if(dt.Name != NULL)
				{
					strcat(Buffer1 ,dt.DataSize);					
				}
			else strcat(Buffer1,"0000");

			//Add a newline
			strcat(Buffer1,"\n");
			//free(Buffer1);
			return "c";
	}
/**************************************************************************************************
	* Function Name: GenerateCNOBD
   * Description: 
/****************************************************************************************************/
char* GenerateCNOBD(CNodeCollection* objNodeCol)
	{
		CNode objNode;
		CIndexCollection* objIndexCollection;
		char* Buffer2;

		Buffer2 =  new char;
		for(int count=0; count<objNodeCol->getNumberOfNodes(); count++)
			{
				objNode = objNodeCol->getNode(CN,count+1);
				objIndexCollection = objNode.getIndexCollection();
				char* comment= new char;
				char* c;
				c= new char;
				itoa(count+1,c,10);
				strcpy(comment,"\\\\Configuration Data for CN-");
				comment = strcat(comment,c);
				comment = strcat(comment,"\n");
				char* NodeId;
				//comment = strcat(comment,(char*)count);
				strcpy(Buffer2, comment);
				int NumberOfIndexes = objIndexCollection->getNumberofIndexes();
				for(int i=0; i<NumberOfIndexes; i++)
					{
						CIndex* objIndex;
						objIndex = objIndexCollection->getIndex(i);
						char* IndexValue = objIndex->getIndexValue();
						if(strcmp(IndexValue,"1006")==0 ||strcmp(IndexValue,"2000")==0)
							{
								//strcpy(Buffer2,GetIndexData(objIndex));
								GetIndexData(objIndex, Buffer2);
								
							}
					}
						
			}
		return Buffer2;
	}
/**************************************************************************************************
	* Function Name: GenerateCDC
    * Description: Generates the CDC file
/****************************************************************************************************/
void GenerateCDC(char* fileName)
	{
		CNode objMNNode;	
		CIndexCollection* objIndexCollection;
		ofstream file;
		char *Buffer1,*Buffer2;

			

		Buffer1 =  new char;
		Buffer2 = new char;
		//get the MN Node object from the NodeCollection
		CNodeCollection* objNodeCollection;
		objNodeCollection = CNodeCollection::getNodeColObjectPointer();
		objMNNode = objNodeCollection->getNode(CN,1);

		/*if(file.is_open())
			{printf("x");}*/
		FILE* fileptr = new FILE();
		if (( fileptr = fopen(fileName,"w+")) == NULL)
			{
				printf ( "Cannot open file you have named...!\n" );
			
			}
		else
			{
			}

		//opens the file 
	/*	file.open(fileName);*/

		objIndexCollection = objMNNode.getIndexCollection();
		strcpy(Buffer1,"");

		//Get all the MN's Default Data in Buffer1
		for(int i=0;i < objIndexCollection->getNumberofIndexes ();i++)
			{
				CIndex* objIndex;
				objIndex = objIndexCollection->getIndex(i);
				//strcpy(Buffer1,GetIndexData(objIndex));
				GetIndexData(objIndex, Buffer1);
				
			}

				
		//Get CN's Data in Buffer2
		strcpy(Buffer2,GenerateCNOBD(objNodeCollection));
		char* Buffer3;
		Buffer3 = new char;
		strcpy(Buffer3, Buffer1);
		strcat(Buffer3, Buffer2);

		int count;
		count = strlen(Buffer3);
		if((fwrite(Buffer3, sizeof(char),count,fileptr))!=NULL)
			{
				printf("success");
			}

		// Write Buffer1 to the filestream
	/*	file << Buffer3 <<endl;*/
		
		strcpy(Buffer1,"");
		fclose(fileptr);
		//file.close();
		/*free(Buffer1);
		free(Buffer2);
		free(Buffer3);*/
		

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
int main(int argc, char **argv) 
{
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
	//objNode.setNodeId(1);
	//objNode.setNodeType(CN);
	CreateNode(1,CN);
	CreateNode(2,MN);
	parseFile("E:\\Kalycito\\checkout\\Source\\SharedLibrary\\openCONFIGURATORSoln\\XMLParser\\cn1.xDC",2,MN);

	//objNodeCollection->addNode(objNode);
	//objNodecol.addNode(objNode);
	printf("Total Nodes %d\n",objNodecol.getNumberOfNodes());
	printf("Total Nodes %d\n",objNodeCollection->getNumberOfNodes());

	parseFile("E:\\Kalycito\\checkout\\Source\\SharedLibrary\\openCONFIGURATORSoln\\XMLParser\\12345678_MyName.xml",1,CN);

	/* Show the Indexes*/
	int i;
	CIndexCollection* obj;
	objNode= objNodeCollection->getNode(CN,1);
	obj =objNode.getPDOIndexCollection(TPDO);
	obj=objNode.getIndexCollection();
	for (i=0;i<obj->getNumberofIndexes();i++)
		{
			CIndex* objIndex;
			objIndex = obj->getIndex(i);
			char* index;
			index=objIndex->getIndexValue();
			printf("Index: %s Name: %s\n",index,objIndex->getName());
		}
	
	
			CIndex *objIndex;
			objIndex = obj->getIndex(1);
			/*CSubIndex* objsub;
			objsub= objIndex->getSubIndex(1);*/


			CDataTypeCollection* dtcol;
			dtcol= objNode.getDataTypeCollection();
			DataType* dt;
			dt=dtcol->getDataType(0);
			printf("DataType Name: %s", dt->Name);
			DataType  t;
			t= objIndex->getDataType();
			//index=objIndex.getNumberofSubIndexes();
			printf("Number of subindexes: %d",objIndex->getNumberofSubIndexes());
		/*	printf("SubIndex Name: %s",objsub->getName());*/
			//printf("Index: %s Name: %s\n",index,objIndex.getName());
			char* fileName=new char ;
			strcpy(fileName,"C:\\Try.txt");
			GenerateCDC(fileName);
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

/**************************************************************************************************
	* Function Name: ProcessPDONodes
    * Description: Processes the Node
/****************************************************************************************************/
void CreateProcessImage(xmlTextReaderPtr reader)
	{
		CNode objMNNode;		
		char* fullFileName;

		//get the MN Node object from the NodeCollection
		/*fullFileName = fileName + filePath;*/
		ofstream file;
		//file.open(fullFileName);


		//char *WriteBuffer;		

	}