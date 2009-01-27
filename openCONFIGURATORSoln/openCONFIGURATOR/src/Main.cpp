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
#include "../Include/Internal.h"
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <libxml/xmlreader.h>
#include <errno.h>
#include <string.h>

char* strerr;
int LastIndexParsed=0;

void LoadObjectDictionary()
	{
	
		//Parse the ObjectDictionary xml and creates the objectDictionary object
		
	}
	
static void setIndexAttributes(xmlTextReaderPtr reader, CIndex* objIndex)
	{
		const xmlChar* name, *value;
		//Retrieve the name and Value of an attribute
		value = xmlTextReaderConstValue(reader);
		name = xmlTextReaderConstName(reader);		

		if(strcmp(ConvertToUpper((char*)name), "INDEX")==0)
			{	
				// For holding temp data to print
				char* Buffer;
						
				// Setting the Index Value
				objIndex->setIndexValue((char*)value);
				
				// For Testing. 
				Buffer = (char *) malloc(sizeof(objIndex->getIndexValue()));
				strcpy(Buffer, objIndex->getIndexValue());
				printf("\tName:%s, objIndex->getIndexValue:%s\n", name, Buffer);
				
				/*$S Commented out the TPDO and RPDO grouping $S*/
				//TODO: Check point 1 
				char* data = (char*) value;
				if(strncmp(data,"14",2)==0 ||strncmp(data,"16",2) ==0)
					{
						objIndex->setPDOType(PDO_RPDO);
					}
				else if(strncmp(data,"18",2 )==0 ||strncmp(data,"1A",2) ==0)
					{
						objIndex->setPDOType(PDO_TPDO);
					}

			}

		else if( strcmp(ConvertToUpper((char*)name), "NAME") == 0 )
		{	
			objIndex->setName((char*)value);
			//objIndex->setName("CFM_VerifyConfiguration_REC12ksdhksdhk");
			// Test print
			printf("\tName:%s, objIndex->getName:%s\n", name, objIndex->getName());
		}

		else if(strcmp(ConvertToUpper((char*)name), "OBJECTTYPE")==0)					
		objIndex->setObjectType(atoi((const char*)value));

		else if(strcmp(ConvertToUpper((char*)name), "LOWLIMIT")==0)
		{
			objIndex->setLowLimit((char*)value);
			//objIndex->setLowLimit("123456789101112131415");
			//printf("\tName:%s, objIndex->getLowLimit:%s\n", name, objIndex->getLowLimit());
		}

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

		objDataType->Name = NULL;
		objDataType->DataTypeValue = NULL;
		objDataType->DataSize = NULL;
		
		//printf("\nInside DataType");
		
		//Retrieve the name and Value of an attribute
		value = xmlTextReaderConstValue(reader);
		name =xmlTextReaderConstName(reader);
		//printf("Value_1:%s\n", value);							

		if(strcmp(ConvertToUpper((char*)name), "DATATYPE") == 0)						
		{
			objDataType->DataTypeValue =new char[strlen((char*)value) + 1];
			strcpy(objDataType->DataTypeValue, (char*)value);
			//printf("objDataType->DataTypeValue:%s\n",objDataType->DataTypeValue);
			//Read the Equivalent name of a datatype
			ret = xmlTextReaderRead(reader);
			ret = xmlTextReaderRead(reader);
			value = xmlTextReaderConstValue(reader);
			name =xmlTextReaderConstName(reader);
					
			/*objDataType->setName((char*)name);*/
			objDataType->Name = new char[strlen((char*)name) + 1];
			//printf("\n Size:%d",sizeof(objDataType->Name));
			//printf("\n Size of Name:%d", strlen((char*)name));
			strcpy(objDataType->Name, (char*)name);
			//printf("\n outsideName:%s", objDataType->Name);
			printf("objDataType->DataTypeValue:%s, objDataType->Name:%s\n", objDataType->DataTypeValue, objDataType->Name);
			objDataType->DataSize = new char[4];
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
				printf("\noutside DataType");
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

		printf("Inside Importxml");
		parseFile(fileName, NodeID, NodeType);
		//Cleanup function for the XML library.
   
		printf("Parsing Done");
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
	
		CNodeCollection *objNodeCollection;
		CNode objNode;
	
    name = xmlTextReaderConstName(reader);
    if (name == NULL)
		printf("\n\n\n\nGot NULL for Name\n\n\n\n");
	//	name = BAD_CAST "--";		
    value = xmlTextReaderConstValue(reader);
    
	//If the NodeTYPE is ELEMENT
	if( xmlTextReaderNodeType(reader)==1)
		{
			if(strcmp(((char*)name),"defType")==0)
				{
					objNodeCollection= CNodeCollection::getNodeColObjectPointer();
					CDataTypeCollection* objDataTypeCollection;			
					//printf("createed Nodecol\n");
					DataType objDataType;
					if (xmlTextReaderHasAttributes(reader)==1)
						{						
							while(xmlTextReaderMoveToNextAttribute(reader))
								{
									//printf("Calling setDataTypeAttributes..\n ");
									setDataTypeAttributes(reader, &objDataType);
									//$Svalue = xmlTextReaderConstValue(reader);
									//$Sprintf("Value_1:%s\n", value);
									//$Sname =xmlTextReaderConstName(reader);																									
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
									//printf("Calling setIndexAttributes..\n ");
									setIndexAttributes(reader, &objIndex);	
									//$Svalue = xmlTextReaderConstValue(reader);
									//$Sprintf("\tValue_2:%s\n", value);
									//$Sname =xmlTextReaderConstName(reader);																								
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
									//value = xmlTextReaderConstValue(reader);
									//printf("\t\tValue_3:%s\n", value);
									//name =xmlTextReaderConstName(reader);																									
								}
						}
						CIndex* objIndexPtr;
								
				
						objIndexCollection =objNode.getIndexCollection();
						objIndexPtr =objIndexCollection->getIndex(LastIndexParsed);
										
						if(strcmp(objIndexPtr->getIndexValue(),"2103")==0)
							{
								if(strcmp(objSubIndex.getIndexValue(),"17")==0)
									{	
									printf("%d",objIndexPtr->getNumberofSubIndexes());
									}
							}
						printf("LastIndexParsed: %s\n",objIndexPtr->getIndexValue());
						printf("SubIndex value: %s \n",objSubIndex.getIndexValue());
						objIndexPtr->addSubIndex(objSubIndex);

						////free memory
						//objIndexPtr->~CIndex();
						//delete objIndexPtr;
						/*printf("subindex deleted:%s",objIndexPtr->getIndexValue());*/
						//printf("Index added\n");
						
						
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

		
    reader = xmlReaderForFile(filename, NULL, 0);
    if (reader != NULL)
				{
        ret = xmlTextReaderRead(reader);
        while (ret == 1)
								{		
										processNode(reader,NodeType,NodeIndex );
										ret = xmlTextReaderRead(reader);
								}
		
				printf("\n\n\nCalling xmlFreeTextReader\n\n\n\n");

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
       fprintf(stderr, "Unable to open %s\n", filename);
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
void CreateNode(int NodeID, ENodeType NodeType)
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
void GetIndexData(CIndex* objIndex, char* Buffer)
	{
			int len;
						
			if(objIndex->getNumberofSubIndexes() !=0)
				{}
			//Get the Index Value		
			
			//Buffer = (char *) malloc(sizeof(IndexValue));
			strcpy(Buffer,objIndex->getIndexValue());

			//Place a tab
			//Buffer = (char *)realloc(Buffer,2* sizeof(char));
			len = strlen(Buffer);
			strcat(Buffer,"\t");
			printf("%s",Buffer);

			//If subindexes are none, add "00"
			//Buffer = (char *)realloc(Buffer,2* sizeof(char));
			if(objIndex->getNumberofSubIndexes() ==0)
				strcat(Buffer,"00");

			strcat(Buffer,"\t");

			//Add datatype
			//Buffer = (char *)realloc(Buffer,4* sizeof(char));
			DataType dt;			
			dt = objIndex->getDataType();
			if(dt.Name != NULL)
				{
					strcat(Buffer ,dt.DataSize);					
				}
			else strcat(Buffer,"0000");

			//CHANGE			
			if (objIndex->getActualValue()!=NULL)
			{
				//Buffer = (char *)realloc(Buffer,sizeof(objIndex->getActualValue()));
				strcat(Buffer,objIndex->getActualValue());
			//Add a newline
				//Buffer = (char *)realloc(Buffer,2*sizeof(char));
			strcat(Buffer,"\n");
			}
			//free(Buffer1);
		
			
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
				/*strcpy(Buffer2, comment);*/
				int NumberOfIndexes = objIndexCollection->getNumberofIndexes();
				for(int i=0; i<NumberOfIndexes; i++)
					{
						CIndex* objIndex;
						objIndex = objIndexCollection->getIndex(i);
						const char* IndexValue = objIndex->getIndexValue();
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
		CNode objNode;	
		CIndexCollection* objIndexCollection;
		ofstream file;
		char *Buffer1,*Buffer2;
		int len;
				
		char* CNData;

		printf("Inside GenerateCDC");
				
		Buffer1 = NULL;		

		//get the MN Node object from the NodeCollection
		CNodeCollection* objNodeCollection;
		objNodeCollection = CNodeCollection::getNodeColObjectPointer();		
		
	/******************************* Write MN's Indexes ******************************************************************/
		objNode = objNodeCollection->getMNNode();

		if(objNode.isNull())
		{
			printf("No MN Node exists");
			exit(1);
		}
		printf("Node id %d",objNode.getNodeId());
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

		objIndexCollection = objNode.getIndexCollection();

		//Get all the MN's Default Data in Buffer1
		for(int i=0;i < objIndexCollection->getNumberofIndexes ();i++)
			{
				CIndex* objIndex;
				objIndex = objIndexCollection->getIndex(i);
				if(CheckIfNotPDO((char*)objIndex->getIndexValue()))
					{						
						printf("Index%s",objIndex->getIndexValue());						
						Buffer1 = (char*)malloc(50);
						GetIndexData(objIndex,Buffer1);
						len = strlen(Buffer1);
						if((fwrite(Buffer1, sizeof(char),len,fileptr))!=NULL)
						{
							printf("Buffer1 written");
						
						}
						delete[] Buffer1;
					}
						
			}
			
			
		/*************************Write CN's Data in Buffer2***************************************************/
		char* Buffer3;
		Buffer3 = new char;
		strcpy(Buffer3,"");
		char* c;
		c= new char;
		int count;
		for(int count=0; count<objNodeCollection->getNumberOfNodes(); count++)
			{
				objNode = objNodeCollection->getNodebyCollectionIndex(count);
				if (objNode.getNodeType() == CN)
					{
						
							objIndexCollection = objNode.getIndexCollection();
							char* comment= new char;
							itoa(count+1,c,10);
							strcpy(comment,"\\\\Configuration Data for CN-");
							comment = strcat(comment,c);
							comment = strcat(comment,"\n");
							len = strlen(comment);
							if((fwrite(comment, sizeof(char),len,fileptr))!=NULL)
							{
								printf("\nComments for CN-%d",count);
								
							}
							char* NodeId;
							//comment = strcat(comment,(char*)count);
							/*strcpy(Buffer2, comment);*/
							int NumberOfIndexes;
							NumberOfIndexes = objIndexCollection->getNumberofIndexes();
							Buffer2 = (char*)malloc(50);
							for(int i=0; i<NumberOfIndexes; i++)
								{
									CIndex* objIndex;
									objIndex = objIndexCollection->getIndex(i);
									const char* IndexValue = objIndex->getIndexValue();
									if(strcmp(IndexValue,"1006")==0 ||strcmp(IndexValue,"2000")==0)
										{
											
											//strcpy(Buffer2,GetIndexData(objIndex));
											GetIndexData(objIndex, Buffer2);											
										}
										delete[] Buffer2;
								}	

						
							//Convert CN NodeID to Hex
							itoa(count+1,c,10);	
							char CNDataLength[18];
							
							strcpy(CNDataLength, "1FF2\t");

							if (strlen(c)< 2)
								{
									char clen[2];
									strcpy(clen,c);
									strcpy(c,"0");
									strcat(c,clen);
								}
							//write CN-n NodeID  in the next to 1F22
							strcat(CNDataLength, c);
							strcat(CNDataLength, "\t");
							
							//write the size of CN-n Buffer
							int len = strlen(Buffer2);

							//Convert length to Hex
							itoa(len,c,16);
							if (strlen(c)< 4)
								{
									char clen[2];
									strcpy(clen,c);
									if(strlen(c)==1){strcpy(c,"000");}
									else if(strlen(c)==2){strcpy(c,"00");}
									else if(strlen(c)==3){strcpy(c,"0");}
									strcat(c,clen);
								}
							strcat(CNDataLength, c);				
							
							// First write the IF22 data in a Buffer and then CN-ns Object Dictionary
							strcat(Buffer3, CNDataLength);
							strcat(Buffer3, "\n");
							strcat(Buffer3, Buffer2);

							
							/*free(Buffer2);	*/

									
						
							//strcpy(Buffer2,GenerateCNOBD(objNodeCollection));
						
							//write all CNs data in the file
							len = strlen(Buffer3);
							if((fwrite(Buffer3, sizeof(char),len,fileptr))!=NULL)
								{
									printf("success");
									delete[] Buffer3;
								}
					}
			}

		fclose(fileptr);		

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

/**************************************************************************************************
	* Function Name: ProcessPDONodes
    * Description: Processes the Node
/****************************************************************************************************/
void ProcessPDONodes()
	{
		CNodeCollection* objNodeCol;
		objNodeCol = CNodeCollection::getNodeColObjectPointer();
		for(int i =0;i<objNodeCol->getNumberOfNodes(); i++)
			{
				CNode objNode;
				objNode = objNodeCol->getNode(CN,i);
				CIndexCollection* objPDOCollection;
				CIndexCollection* objIndexCollection;
				objPDOCollection = objNode.getPDOIndexCollection(PDO_RPDO);
				objIndexCollection = objNode.getIndexCollection();
				for(int count = 0; count<objPDOCollection->getNumberofIndexes(); count++)
					{
						CIndex* objIndex;
						objIndex = objPDOCollection->getIndex(count);
						const char* value = objIndex->getActualValue();
						int len = strlen(value);
					

					}

			}
	}

void ConvertToBinary(char* fileName)
{
	FILE *fin, *fout_cdc, *fout_Cfile;
	int count=0;
	int num=0;
	char ch=0;
	int iCtr;
	int iLength;
	unsigned char ca_cn1obd[10000];
	unsigned char tempCn1Obd[10000];

	fin = fopen(fileName, "r");
	fout_cdc = fopen("config_data.cdc", "wb");
	fout_Cfile = fopen("../../openPOWERLINK_v1.1.0/ObdConf.c", "wb");
	//if(argc<=1 || argc>2)
	//{
	//	printf("\n Invalid argument \n");
	//	goto Exit;
	//}
	//else
	//{
		while(ch != EOF)
		{
			ch = fgetc(fin);
			if(ch == '/') 
		    {
				while(fgetc(fin) != '\n');
		    }
			else if( ch != '\t' || ch != '\n' || ch != ' ')
			{
				// Convert to Upper case
				ch = toupper(ch);
				if(ch >= 65 && ch <= 70)
				{
					ca_cn1obd[num] = ch - 55;
					num++;
				}
				else if (( ch >=48 && ch <= 57))
				{
					ca_cn1obd[num] = ch - 48;
					num++;
				}
			}
		//}
		ca_cn1obd[num] = '\0';		
		// For testing purpose.
		for ( count = 0; count<num ; count++)
		{
			//printf("0X%x, ",ca_cn1obd[count]);
		}
		//
		//
		iLength = num;
		//printf( "\niLength = %d\n",iLength);
		fprintf(fout_Cfile,"static unsigned char tempBaMnObd[%d] = {",((iLength/2)-1));
		//For Byte Packing
		for (iCtr = 0 , count = 0; iCtr < iLength; iCtr++, count++ )
		{
			tempCn1Obd[count] = (unsigned char)( ( ca_cn1obd[ iCtr ] << 4 ) | ca_cn1obd[ ++iCtr ] );
			//printf("0x%x\t",tempCn1Obd[count]);
			fprintf(fout_Cfile,"0X%x, ",tempCn1Obd[count]);
		}
		//printf("Size : %d\n", count);
		fprintf(fout_Cfile,"};\n");		
		count = count -1;
		fprintf(fout_Cfile,"static unsigned int DataCount = %d;\n",count);
		// Write to Binary file
		fwrite(&tempCn1Obd,1,count,fout_cdc);
	}
	Exit :
	fclose(fin);
	fclose(fout_cdc);
	fclose(fout_Cfile);
}
