
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
//#include "../Include/Internal.h"
#include "../Include/Exception.h"
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <libxml/xmlreader.h>
#include <errno.h>
#include <string.h>
#define MY_ENCODING "ISO-8859-1"
int LastIndexParsed=0;

void setIndexAttributes(xmlTextReaderPtr reader, CIndex* objIndex, bool& hasPDO)
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
				//printf("\tName:%s, objIndex->getIndexValue:%s\n", name, Buffer);
				
				/*$S Commented out the TPDO and RPDO grouping $S*/
				//TODO: Check point 1 
				char* data = (char*) value;
				if(strncmp(data,"14",2)==0 ||strncmp(data,"16",2) ==0)
					{
						objIndex->setPDOType(PDO_RPDO);				
						hasPDO  = true;		
					}
				else if(strncmp(data,"18",2 )==0 ||strncmp(data,"1A",2) ==0)
					{
						objIndex->setPDOType(PDO_TPDO);
						hasPDO		= true;
					}

			}

		else if( strcmp(ConvertToUpper((char*)name), "NAME") == 0 )
		{	
			objIndex->setName((char*)value);
			//objIndex->setName("CFM_VerifyConfiguration_REC12ksdhksdhk");
			// Test print
			//printf("\tName:%s, objIndex->getName:%s\n", name, objIndex->getName());
		}

		else if(strcmp(ConvertToUpper((char*)name), "OBJECTTYPE")==0)					
		objIndex->setObjectType((char*)value);

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
		{
			if(CheckIfDataTypeExists((char*)value, objIndex->getNodeID()))
			objIndex->setDataType((char*)value);		
			else
			{
				ocfmException ex;
				ex.ocfm_Excpetion(OCFM_ERR_DATATYPE_NOT_FOUND);
				throw ex;
			}					
		}

		else if(strcmp(ConvertToUpper((char*)name), "UNIQUEIDREF")==0)					
		objIndex->setUniqueIDRef((char*)value);							
	
																	
	}
void setSubIndexAttributes(xmlTextReaderPtr reader, CSubIndex* objSubIndex)
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
		objSubIndex->setObjectType((char*)value);

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

		else if(strcmp(ConvertToUpper((char*)name), "UNIQUEIDREF")==0)					
		objSubIndex->setUniqueIDRef((char*)value);							
		
		
														
	}
	
void setDataTypeAttributes(xmlTextReaderPtr reader ,DataType* objDataType)
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
			
			if(ret!=1)
			{
			
						ocfmException objException;						
						objException.ocfm_Excpetion(OCFM_ERR_XML_FILE_CORRUPTED);
						throw objException;
				
			}
			ret = xmlTextReaderRead(reader);
			if(ret!=1)
			{
			
						ocfmException objException;						
						objException.ocfm_Excpetion(OCFM_ERR_XML_FILE_CORRUPTED);
						throw objException;
				
			}
			value = xmlTextReaderConstValue(reader);
			name =xmlTextReaderConstName(reader);
					
			/*objDataType->setName((char*)name);*/
			objDataType->Name = new char[strlen((char*)name) + 1];
			//printf("\n Size:%d",sizeof(objDataType->Name));
			//printf("\n Size of Name:%d", strlen((char*)name));
			strcpy(objDataType->Name, (char*)name);
			//printf("\n outsideName:%s", objDataType->Name);
			//printf("objDataType->DataTypeValue:%s, objDataType->Name:%s\n", objDataType->DataTypeValue, objDataType->Name);
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
				
				if(strcmp(ConvertToUpper(objDataType->Name),"UNSIGNED64")==0)
				strcpy(objDataType->DataSize,"0008");
			
				}
				//printf("\noutside DataType");
	}

void setParameterAttributes(xmlTextReaderPtr reader, Parameter* stParameter)
	{
		const xmlChar* name,*value;
		//Retrieve the name and Value of an attribute
		value = xmlTextReaderConstValue(reader);
		name =xmlTextReaderConstName(reader);					

		if(strcmp(ConvertToUpper((char*)name), "UNIQUEID")==0)
		{						
			stParameter->name_id_dt_attr.setUniqueID((char*)value);
		}

		else if(strcmp(ConvertToUpper((char*)name), "NAME")==0)					
		{									
			stParameter->name_id_dt_attr.setName((char*)value);		
		}
		else if(strcmp(ConvertToUpper((char*)name), "ACCESS")==0)					
		{						
			stParameter->access  = new char[strlen((const char*)value) + 1];			
			strcpy(stParameter->access, (const char*)value);
		}

		/* TO DO: DATATYPE..There is no tag for it..need to check after how many reads datatype is define </label>
            <USINT/>*/					
	
	}
	void getParaDT(xmlTextReaderPtr reader, Parameter* stParameter)
	{
		const xmlChar* name,*value;
		int ret;
  
  ret = xmlTextReaderRead(reader);
  
  if(ret!=1)
  {
				ocfmException objException;						
				objException.ocfm_Excpetion(OCFM_ERR_XML_FILE_CORRUPTED);
				throw objException;
  }
  
  name = xmlTextReaderConstName(reader);
  value = xmlTextReaderConstValue(reader);
		while(!(CheckEndElement(xmlTextReaderNodeType(reader),(char*)name, "parameter")))
		{
			ret = xmlTextReaderRead(reader);
			if(ret!=1)
			{
				ocfmException objException;						
				objException.ocfm_Excpetion(OCFM_ERR_XML_FILE_CORRUPTED);
				throw objException;
			}
			value = xmlTextReaderConstValue(reader);
			name =xmlTextReaderConstName(reader);			
			char size[3];
			if(CheckifSimpleDT((char*)name, size))
			{
					stParameter->name_id_dt_attr.setDataType((char*)name);
			}		
			if(CheckStartElement(xmlTextReaderNodeType(reader),(char*)name, "dataTypeIDRef"))
			{
				if (xmlTextReaderHasAttributes(reader)==1)
				{
				
					xmlTextReaderMoveToNextAttribute(reader);
					value = xmlTextReaderConstValue(reader);
					name =xmlTextReaderConstName(reader);							

					if(strcmp(ConvertToUpper((char*)name), "UNIQUEIDREF")==0)
					{
						stParameter->name_id_dt_attr.setDtUniqueRefId((char*)value);
					}
				}		
			}
		}
	}
	static void setCDTAttributes(xmlTextReaderPtr reader, CComplexDataType* objCDT)
	{
		const xmlChar* name,*value;
		//Retrieve the name and Value of an attribute
		value = xmlTextReaderConstValue(reader);
		name =xmlTextReaderConstName(reader);							

		if(strcmp(ConvertToUpper((char*)name), "UNIQUEID")==0)
		{						
			objCDT->name_id_attr->setUniqueID((char*)value);
		}

		else if(strcmp(ConvertToUpper((char*)name), "NAME")==0)					
		{						
			objCDT->name_id_attr->setName((char*)value);			
		}
			
	}
bool CheckifSimpleDT(char* Name, char* size)
{

	int count = 0;
	char* g_simple_element;
	
	while(count < g_simple_arr_size )
	{
			g_simple_element = (char*)g_Simple[count][0];
		 if(strcmp(g_simple_element, Name)==0)
		 {
				strcpy(size, (char*)g_Simple[count][1]);
				return true;
			}
		 count++;
		 	 
	}
	return false;
	
}

 void setVarDecAttributes(xmlTextReaderPtr reader, varDeclaration& vdecl)
	{
		const xmlChar* name,*value;
		//Retrieve the name and Value of an attribute
		value = xmlTextReaderConstValue(reader);
		name =xmlTextReaderConstName(reader);		
		bool vardecCompleted = false;
							
	
		if(strcmp(ConvertToUpper((char*)name), "UNIQUEID")==0)
		{						
			vdecl.nam_id_dt_attr->setUniqueID((char*)value);
		}

		else if(strcmp(ConvertToUpper((char*)name), "NAME")==0)					
		{						
			vdecl.nam_id_dt_attr->setName((char*)value);			
		}
		 if(strcmp(ConvertToUpper((char*)name), "SIZE")==0)					
		{						
			strcpy((char*)vdecl.size,(const char*)value);
		}
		/*	vdecl->nam_id_dt_attr = app;*/
	}
	bool CheckEndElement(int NodeType, char* element, char* comparewith)
	{
		if((NodeType == XML_READER_TYPE_END_ELEMENT) && (strcmp(element, comparewith)==0))
		return true;
		else
		return false;
	} 
	bool CheckStartElement(int NodeType, char* element, char* comparewith)
	{
		if((NodeType == XML_READER_TYPE_ELEMENT) && (strcmp(element, comparewith)==0))
		return true;
		else
		return false;
	} 
static void getVarDeclaration(xmlTextReaderPtr reader, CComplexDataType* objCDT)
{
		const xmlChar* name,*value;
		int ret;
  varDeclaration stvardecl;
		stvardecl.Initialize();
  ret = xmlTextReaderRead(reader);
  
  if(ret!=1)
  {
  	ocfmException objException;						
			objException.ocfm_Excpetion(OCFM_ERR_XML_FILE_CORRUPTED);
			throw objException;
		}
  
  name = xmlTextReaderConstName(reader);
  value = xmlTextReaderConstValue(reader);
   
		while(!(CheckEndElement(xmlTextReaderNodeType(reader),(char*)name, "struct")))
		{
			varDeclaration temp;
			temp.Initialize();
			
			try
			{
					ret = xmlTextReaderRead(reader);
					if (ret != 1) 
					{
						ocfmException* objException = new ocfmException;
						objException->ocfm_Excpetion(OCFM_ERR_XML_FILE_CORRUPTED);
					}
				  
			}
			catch(ocfmException *ex)
			{
				throw ex;
			}
			name = xmlTextReaderConstName(reader);
			value = xmlTextReaderConstValue(reader);
			//printf("\nName:%s",name);
			//printf("\n NodeType: %d",xmlTextReaderNodeType(reader));
			
			if(CheckEndElement(xmlTextReaderNodeType(reader),(char*)name, "varDeclaration"))
			{
				stvardecl.StructUniqueId = (char*)malloc(strlen(objCDT->name_id_attr->getUniqueID() +1));
				strcpy(stvardecl.StructUniqueId , objCDT->name_id_attr->getUniqueID());
				objCDT->addVarDeclaration(stvardecl);
				stvardecl = temp;
			}
			if(CheckStartElement(xmlTextReaderNodeType(reader),(char*)name, "varDeclaration"))
			{
						printf("\nELEMENT----- Name = %s ",name);
						if (value==NULL)
						{
							//printf("Value = NULL \n");  
						}
						else
						{
							printf("Value= %s",value);
						}
					if (xmlTextReaderHasAttributes(reader)==1)
						{						
							while(xmlTextReaderMoveToNextAttribute(reader))
								{

										setVarDecAttributes(reader,stvardecl);		
										printf("\n***Attribute  *********\n");
										value = xmlTextReaderConstValue(reader);
										name =xmlTextReaderConstName(reader);
										printf("\ndepth : %d NodeType= %d Name= %s ,value= %s\n", 
										xmlTextReaderDepth(reader),
										xmlTextReaderNodeType(reader),
										name,value);																												
										
								}
							}
						}
						/*	}*/
		/*	ret = xmlTextReaderRead(reader);
		name = xmlTextReaderConstName(reader);
		value = xmlTextReaderConstValue(reader);*/
			char size[3];
			if(CheckifSimpleDT((char*)name, size))
			{
					stvardecl.nam_id_dt_attr->setDataType((char*)name);
					strcpy(stvardecl.size, size);
			}		
			if(CheckStartElement(xmlTextReaderNodeType(reader),(char*)name, "dataTypeIDRef"))
		{
				if (xmlTextReaderHasAttributes(reader)==1)
				{
				
					xmlTextReaderMoveToNextAttribute(reader);
					value = xmlTextReaderConstValue(reader);
					name =xmlTextReaderConstName(reader);							

					if(strcmp(ConvertToUpper((char*)name), "UNIQUEIDREF")==0)
					{
						stvardecl.nam_id_dt_attr->setDtUniqueRefId((char*)value);
					}
				}		
			}
												
		
		}
			
	}

	
/**************************************************************************************************
	* Function Name: ImportXML
    * Description: Imports the XML file
/****************************************************************************************************/

ocfmRetCode ImportXML(char* fileName, int NodeID, ENodeType NodeType)
	{

		
		/*ocfmRetCode ocfmError;
		ocfmError.code = OCFM_ERR_SUCCESS;

		ocfmRetCode ErrStruct;*/			
		ocfmRetCode ErrStruct;
		try
		{
			//printf("Inside Importxml");
			/*ErrStruct = */
			ErrStruct = parseFile(fileName, NodeID, NodeType);
			if(ErrStruct.code!= 0)
				return ErrStruct;

					/* Process PDO Objects for CN*/
				if (NodeType != MN )
				{
					CNode objNode;
					CNodeCollection  *objNodeCol;
					objNodeCol = CNodeCollection ::getNodeColObjectPointer();
					objNode = objNodeCol->getNode(NodeType, NodeID);
					/* Check if any PDO's present */
					if(objNode.HasPdoObjects())	
					{
						ProcessPDONodes(NodeID);
					}
				}
				//printf("Parsing Done");
			 xmlCleanupParser();
			/*
			* this is to debug memory for regression tests
			*/
			xmlMemoryDump();
			ErrStruct.code = OCFM_ERR_SUCCESS;
			return ErrStruct;
		}
		catch(ocfmException& ex)
		{
			return ex._ocfmRetCode;
		}	
	}
/**************************************************************************************************
	* Function Name: processNode
    * Description: Process the Node value,Name and its attributes
/****************************************************************************************************/
void processNode(xmlTextReaderPtr reader,ENodeType NodeType,int NodeIndex)
{
		const xmlChar *name, *value;
	
		CNodeCollection *objNodeCollection;
		CNode *objNode;
	
    name = xmlTextReaderConstName(reader);
    if (name == NULL)
		//printf("\n\n\n\nGot NULL for Name\n\n\n\n");
	//	name = BAD_CAST "--";		
    value = xmlTextReaderConstValue(reader);
 try
 { 
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

					objNode = objNodeCollection->getNodePtr(NodeType,NodeIndex);							
					objDataTypeCollection = objNode->getDataTypeCollection();
					objDataTypeCollection->addDataType(objDataType);
			

				}
			else if(strcmp(((char*)name),"parameter")==0)
				{
					objNodeCollection= CNodeCollection::getNodeColObjectPointer();	
					CApplicationProcess* objApplicationProcess;				
					Parameter stParameter;

					
					objNode = objNodeCollection->getNodePtr(NodeType, NodeIndex);	
					
					if (xmlTextReaderHasAttributes(reader)==1)
						{						
							while(xmlTextReaderMoveToNextAttribute(reader))
								{
										setParameterAttributes(reader,&stParameter);																															
								}
						}
						
						getParaDT(reader, &stParameter);
						// Add parameter to the parameter collection of a node
						objApplicationProcess = objNode->getApplicationProcess();
						objApplicationProcess->addParameter(stParameter);
					
				}
			else	if(strcmp(((char*)name),"struct")==0)
				{
					objNodeCollection= CNodeCollection::getNodeColObjectPointer();	
					CApplicationProcess* objApplicationProcess;		
					CComplexDataType objCDT;				
			
					objNode = objNodeCollection->getNodePtr(NodeType, NodeIndex);	
					
					if (xmlTextReaderHasAttributes(reader)==1)
						{						
							while(xmlTextReaderMoveToNextAttribute(reader))
								{
										setCDTAttributes(reader,&objCDT);																															
								}
						}
						
						 getVarDeclaration(reader,&objCDT);
						// Add parameter to the parameter collection of a node
						objApplicationProcess = objNode->getApplicationProcess();
						objApplicationProcess->addComplexDataType(objCDT);
					
				}
			else if(strcmp(((char*)name),"Object")==0)
				{
					objNodeCollection= CNodeCollection::getNodeColObjectPointer();
					CIndexCollection* objIndexCollection;
					CIndex objIndex;

					
					objNode = objNodeCollection->getNodePtr(NodeType, NodeIndex);	
						//Set the NodeID
						objIndex.setNodeID(objNode->getNodeId());
						bool hasPDO = false;

					if (xmlTextReaderHasAttributes(reader)==1)
						{						
							while(xmlTextReaderMoveToNextAttribute(reader))
								{
									//printf("Calling setIndexAttributes..\n ");
									setIndexAttributes(reader, &objIndex, hasPDO);	
									//$Svalue = xmlTextReaderConstValue(reader);
									//$Sprintf("\tValue_2:%s\n", value);
									//$Sname =xmlTextReaderConstName(reader);																								
								}
						}
						if(hasPDO)
						objNode->setFlagForPdoObjects(hasPDO);
						objIndexCollection = objNode->getIndexCollection();
						
						//Add Index object to the IndexCollection
						objIndexCollection->addIndex(objIndex);				
						LastIndexParsed = objIndexCollection->getNumberofIndexes()-1;
						
				}
		else	if(strcmp(((char*)name),"SubObject")==0 )
				{
					objNodeCollection = CNodeCollection::getNodeColObjectPointer();
					CIndexCollection* objIndexCollection;
					CSubIndex objSubIndex;
					objNode =objNodeCollection->getNodePtr(NodeType,NodeIndex);
						//Set the NodeID
						objSubIndex.setNodeID(objNode->getNodeId());		

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
								
				
						objIndexCollection =objNode->getIndexCollection();
						objIndexPtr =objIndexCollection->getIndex(LastIndexParsed);
						//				
						//if(strcmp(objIndexPtr->getIndexValue(),"2103")==0)
						//	{
						//		if(strcmp(objSubIndex.getIndexValue(),"17")==0)
						//			{	
						//			printf("%d",objIndexPtr->getNumberofSubIndexes());
						//			}
						//	}
						//printf("LastIndexParsed: %d-%s\n",LastIndexParsed, objIndexPtr->getIndexValue());
						//printf("SubIndex value: %s \n",objSubIndex.getIndexValue());
						objIndexPtr->addSubIndex(objSubIndex);

						////free memory
						//objIndexPtr->~CIndex();
						//delete objIndexPtr;
						/*printf("subindex deleted:%s",objIndexPtr->getIndexValue());*/
						//printf("Index added\n");
						
						
				}	
			}
		
		}
		catch(ocfmException* ex)
		{
			
			 throw ex;
		}	
}
/**************************************************************************************************
	* Function Name: parseFile
    * Description: Parses the XML file
/****************************************************************************************************/
ocfmRetCode parseFile(char* filename, int NodeIndex, ENodeType  NodeType) 
{
    xmlTextReaderPtr reader;
    int ret;

		
    reader = xmlReaderForFile(filename, NULL, 0);
    try
    {
					if (reader != NULL)
					{
					 ret = xmlTextReaderRead(reader);
      while (ret == 1)
						{		
								processNode(reader,NodeType,NodeIndex );
								ret = xmlTextReaderRead(reader);
						}
						if(ret!=0)
						{
							ocfmException objException;
							/*objException->ocfm_Excpetion(o, true);*/
							objException.ocfm_Excpetion(OCFM_ERR_PARSE_XML);
							throw objException;
						}
					}
					else 
					{
						ocfmException objException;
						objException.ocfm_Excpetion(OCFM_ERR_CANNOT_OPEN_FILE);
						throw objException;
					}
			}								
		
				catch(ocfmException& ex)
				{
					DeleteNodeObjDict(NodeIndex, NodeType);
					return ex._ocfmRetCode;
				}
		ocfmRetCode ErrStruct;		 
		ErrStruct.code = OCFM_ERR_SUCCESS;
		return ErrStruct;
}
 
/**************************************************************************************************
	* Function Name: ReImport
    * Description: Parses the XML file
	Return value Legend:
	Cannot ReImport 		- -1
	ReImport Success 		- 1
/****************************************************************************************************/
ocfmRetCode ReImportXML(char* fileName, int NodeID, ENodeType NodeType)
{
	int NodePos;
	//if((IfNodeExists(NodeID, NodeType, errorString)) >= 0)
	//ocfmRetValError retPos;
	ocfmRetCode ErrStruct;
	try
	{
			bool flag = false;
		ErrStruct = IfNodeExists(NodeID, NodeType, &NodePos, flag);
		if(ErrStruct.code == 0 && flag ==true)
		{
			CNode objNode;
			CNodeCollection *objNodeCollection;
			CIndexCollection *objIndexCollection;
			CDataTypeCollection *objDataTypeCollection;
			CIndex objIndex;

			objIndex.setNodeID(objNode.getNodeId());
			objNodeCollection= CNodeCollection::getNodeColObjectPointer();
			objNode = objNodeCollection->getNode(NodeType, NodeID);
			
			objDataTypeCollection = objNode.getDataTypeCollection();

			objIndexCollection = objNode.getIndexCollection();
			//cout<< "Number of DataType:" << objDataTypeCollection->getNumberOfDataTypes() << endl;
			// Delete IndexCollection
			objIndexCollection->DeleteIndexCollection();
			// Delete DataTypeCollection
			objDataTypeCollection->DeleteDataTypeCollection();
			//cout<< "Number of DataType:" << objDataTypeCollection->getNumberOfDataTypes() << endl;
			parseFile(fileName, NodeID, NodeType);
			ErrStruct.code = OCFM_ERR_SUCCESS;
			return ErrStruct;
		}
		else
		{
			//printf("\nCannot ReImport!!\n");
			//ErrStruct.code = OCFM_ERR_INVALID_NODEID;
			//return ErrStruct;
			ocfmException objException;				
			objException.ocfm_Excpetion(OCFM_ERR_INVALID_NODEID);
			throw objException;
		}
	}
	catch(ocfmException* ex)
	{
		return ex->_ocfmRetCode;
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
	

void ConvertCdcToBinary(char* fileName)
{
	FILE *fin, *fout;
	char* filePath;
	int count=0;
	int num=0;
	char ch=0;
	int iCtr;
	int iLength;
	unsigned char ca_cn1obd[10000];
	unsigned char tempCn1Obd[10000];

	fin = fopen(fileName, "r");
	filePath = (char*)malloc(80);
	filePath = strchr(fileName,'\\');
	filePath = subString(fileName, 0, strlen(fileName) - strlen(filePath) +1);
	strcat(filePath,"config_data.cdc");
	//fout = fopen("config_data.cdc", "wb");
	fout = fopen(filePath, "wb");
	
	
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
				if((ch >= 65 && ch <= 70) || (ch >= 97 && ch <=102))
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
		//ca_cn1obd[num] = '\0';
		// For testing purpose.
		/*for ( count = 0; count<num ; count++)
		{
			printf("%x",ca_cn1obd[count]);

		}*/
		iLength = num;
		//printf( "\niLength = %d\n",iLength);
		//For Byte Packing
		for (iCtr = 0 , count = 0; iCtr < iLength; iCtr++, count++ )
		{
			tempCn1Obd[count] = (unsigned char)( ( ca_cn1obd[ iCtr ] << 4 ) | ca_cn1obd[ ++iCtr ] );
			//printf("0x%2x\t",tempCn1Obd[count]);
		}
		//printf("Size : %d\n", count);
		count = count -1;
		// Write to Binary file
		fwrite(&tempCn1Obd,1,count,fout);
	}
	Exit :
	fclose(fin);
	fclose(fout);	
}
void ProcessUniqueIDRefs(CNode* objNode)
 {
	 
	 
	
 }
 

/**************************************************************************************************
	* Function Name: ImportObjDictXML
    * Description: Imports the Object Dictionary XML file. Objects from this Object dictionary file 
	will be used when Adding Index and/or Adding SubIndex
	* Return value: ocfmRetCode
/****************************************************************************************************/
//ocfmRetCode ImportObjDictXML(char* fileName)
//{
//	xmlTextReaderPtr reader;
//	int ret;
//	ocfmRetCode ErrStruct;
//	CIndex objIndex;
//
//	cout << "ImportObjDictXML:" << fileName << endl;
//	
//	CObjectDictionary* ObjDictObject;
//	ObjDictObject = CObjectDictionary::getObjDictObjectPointer();
//	//char* IndexID = "1006";
//	
//	//objIndex = ObjDictObject->CreateIndexCollection();
//	
//	//objNodeCollection= CNodeCollection::getNodeColObjectPointer();
//	//objNode = objNodeCollection->getNode(NodeType, NodeID);
//	//objIndexCollection = objNode.getIndexCollection();
//			
//	//AddIndexAttributes(IndexID, &objIndex);
//	//ObjDictObject->addIndex(objIndex);
//
//	//reader = xmlReaderForFile(fileName, NULL, 0);
//	//if (reader != NULL)
//	//{
//	//	ret = xmlTextReaderRead(reader);
//	//	while (ret == 1)
//	//	{		
//	//		processObjDict(reader);
//	//		ret = xmlTextReaderRead(reader);
//	//	}
//	//}
//	//else
//	//{
//	//	fprintf(stderr, "Unable to open %s\n", filename);
//	//}
//
//	//printf("\n\n\nCalling xmlFreeTextReader\n\n\n\n");	
//	
//}
/**************************************************************************************************
	* Function Name: processNode
    * Description: Process the Node value,Name and its attributes
/****************************************************************************************************/
void processObjDict(xmlTextReaderPtr reader)
{
	//const xmlChar *name, *value;
	
    //name = xmlTextReaderConstName(reader);
    //if (name == NULL)
	//	printf("\n\n\n\nGot NULL for Name\n\n\n\n");
	////	name = BAD_CAST "--";		
    //value = xmlTextReaderConstValue(reader);
    
	////If the NodeTYPE is ELEMENT
	//if( xmlTextReaderNodeType(reader)==1)
	//	{
	//		if(strcmp(((char*)name),"defType")==0)
	//			{
	//				//objNodeCollection= CNodeCollection::getNodeColObjectPointer();
	//				//CDataTypeCollection* objDataTypeCollection;
	//				
	//				DataType objDataType;
	//				if (xmlTextReaderHasAttributes(reader)==1)
	//					{						
	//						while(xmlTextReaderMoveToNextAttribute(reader))
	//							{
	//								//printf("Calling setDataTypeAttributes..\n ");
	//								setDataTypeAttributes(reader, &objDataType);
	//								//$Svalue = xmlTextReaderConstValue(reader);
	//								//$Sprintf("Value_1:%s\n", value);
	//								//$Sname =xmlTextReaderConstName(reader);																									
	//							}
	//					}

	//				//objNode = objNodeCollection->getNode(NodeType,NodeIndex);							
	//				//objDataTypeCollection = objNode.getDataTypeCollection();
	//				//objDataTypeCollection->addDataType(objDataType);
	//		

	//			}
	//		if(strcmp(((char*)name),"Parameter")==0)
	//			{				
	//				Parameter stParameter;

	//				
	//				//objNode = objNodeCollection->getNode(NodeType, NodeIndex);	
	//				
	//				if (xmlTextReaderHasAttributes(reader)==1)
	//					{						
	//						while(xmlTextReaderMoveToNextAttribute(reader))
	//							{
	//									setParameterAttributes(reader,&stParameter);																															
	//							}
	//					}
	//					
	//					// Add parameter to the parameter collection of a node
	//					objNode.addParameter(stParameter);				
	//				
	//			}
	//		if(strcmp(((char*)name),"Object")==0)
	//			{
	//				//objNodeCollection= CNodeCollection::getNodeColObjectPointer();
	//				//CIndexCollection* objIndexCollection;
	//				//CIndex objIndex;
	//				//$S
	//				CObjectDictionary* ObjDictObject;
	//				ObjDictObject = CObjectDictionary::getObjDictObjectPointer();

	//				
	//				objNode = objNodeCollection->getNode(NodeType, NodeIndex);	
	//					//Set the NodeID
	//					objIndex.setNodeID(objNode.getNodeId());

	//				if (xmlTextReaderHasAttributes(reader)==1)
	//					{						
	//						while(xmlTextReaderMoveToNextAttribute(reader))
	//							{
	//								//printf("Calling setIndexAttributes..\n ");
	//								setIndexAttributes(reader, &objIndex);	
	//								//$Svalue = xmlTextReaderConstValue(reader);
	//								//$Sprintf("\tValue_2:%s\n", value);
	//								//$Sname =xmlTextReaderConstName(reader);																								
	//							}
	//					}
	//					
	//					objIndexCollection = objNode.getIndexCollection();
	//					
	//					//Add Index object to the IndexCollection
	//					objIndexCollection->addIndex(objIndex);				
	//					LastIndexParsed = objIndexCollection->getNumberofIndexes()-1;
	//					
	//			}
	//		if(strcmp(((char*)name),"SubObject")==0 )
	//			{
	//				objNodeCollection = CNodeCollection::getNodeColObjectPointer();
	//				CIndexCollection* objIndexCollection;
	//				CSubIndex objSubIndex;
	//				objNode =objNodeCollection->getNode(NodeType,NodeIndex);
	//					//Set the NodeID
	//					objSubIndex.setNodeID(objNode.getNodeId());		

	//				if (xmlTextReaderHasAttributes(reader) ==1)
	//					{
	//						
	//						while(xmlTextReaderMoveToNextAttribute(reader))
	//							{
	//								setSubIndexAttributes(reader,&objSubIndex);
	//								//value = xmlTextReaderConstValue(reader);
	//								//printf("\t\tValue_3:%s\n", value);
	//								//name =xmlTextReaderConstName(reader);																									
	//							}
	//					}
	//					CIndex* objIndexPtr;
	//							
	//			
	//					objIndexCollection =objNode.getIndexCollection();
	//					objIndexPtr =objIndexCollection->getIndex(LastIndexParsed);
	//									
	//					if(strcmp(objIndexPtr->getIndexValue(),"2103")==0)
	//						{
	//							if(strcmp(objSubIndex.getIndexValue(),"17")==0)
	//								{	
	//								printf("%d",objIndexPtr->getNumberofSubIndexes());
	//								}
	//						}
	//					printf("LastIndexParsed: %d-%s\n",LastIndexParsed, objIndexPtr->getIndexValue());
	//					printf("SubIndex value: %s \n",objSubIndex.getIndexValue());
	//					objIndexPtr->addSubIndex(objSubIndex);						
	//					
	//			}	
	//		}
	//		
}

/**************************************************************************************************
	* Function Name: SaveNode
    * Description: Saves the content of a Node into a XML in XDC format
/****************************************************************************************************/

ocfmRetCode SaveNode(const char* fileName, int NodeID, ENodeType NodeType)
{	
	int rc;
    xmlTextWriterPtr writer;
    xmlChar *tmp;
    xmlDocPtr doc;
	ocfmRetCode ErrStruct;

	try 
	{

		/* Create a new XmlWriter for DOM, with no compression. */
		writer = xmlNewTextWriterDoc(&doc, 0);
		if (writer == NULL) 
		{
			printf("testXmlwriterDoc: Error creating the xml writer\n");
			//return;
			ocfmException* objException = new ocfmException;
			objException->ocfm_Excpetion(OCFM_ERR_CREATE_XML_WRITER_FAILED);
			throw objException;
		}
		/* Start the document with the xml default for the version,
		* encoding ISO 8859-1 and the default for the standalone
		* declaration. */
		rc = xmlTextWriterStartDocument(writer, NULL, MY_ENCODING, NULL);
		if (rc < 0) 
		{
			printf("testXmlwriterDoc: Error at xmlTextWriterStartDocument\n");
			ocfmException* objException = new ocfmException;
			objException->ocfm_Excpetion(OCFM_ERR_XML_START_DOC_FAILED);
			throw objException;
		}	
		rc = xmlTextWriterWriteComment(writer,BAD_CAST "This file was autogenerated by openCONFIGURATOR");
		
		//int NodeID = 1;
		//ENodeType NodeType = CN;
		int IndexPos = 0;
		//cout << "*\n" <<endl;

		CNode objNode;		
		CNodeCollection *objNodeCollection;
		CIndexCollection *objIndexCollection;
		CIndex objIndex;
		CIndex* objIndexPtr;
		Parameter* para;
		CApplicationProcess* objAppProc;
		
		objIndex.setNodeID(objNode.getNodeId());
		objNodeCollection= CNodeCollection::getNodeColObjectPointer();
		objNode = objNodeCollection->getNode(NodeType, NodeID);

		objIndexCollection = objNode.getIndexCollection();

		// Start ISO15745ProfileContainer Tag		
		rc = xmlTextWriterStartElement(writer, BAD_CAST "ISO15745ProfileContainer");
		if (rc < 0) 
		{
			printf("testXmlwriterMemory: Error at xmlTextWriterStartElement\n");
			ocfmException* objException = new ocfmException;
			objException->ocfm_Excpetion(OCFM_ERR_XML_WRITER_START_ELT_FAILED);
			throw objException;
		}
		
		// Start ProfileBody Tag
		rc = xmlTextWriterStartElement(writer, BAD_CAST "ProfileBody");
		if (rc < 0) 
		{
			printf("testXmlwriterMemory: Error at xmlTextWriterStartElement\n");
			ocfmException* objException = new ocfmException;
			objException->ocfm_Excpetion(OCFM_ERR_XML_WRITER_START_ELT_FAILED);
			throw objException;
		}
		
		// Start ApplicationProcess Tag	
		rc = xmlTextWriterStartElement(writer, BAD_CAST "ApplicationProcess");
		if (rc < 0) 
		{
			printf("testXmlwriterMemory: Error at xmlTextWriterStartElement\n");
			ocfmException* objException = new ocfmException;
			objException->ocfm_Excpetion(OCFM_ERR_XML_WRITER_START_ELT_FAILED);
			throw objException;
		}
			xmlTextWriterSetIndent(writer, 1);
						
			// Start dataTypeList Tag
			rc = xmlTextWriterStartElement(writer, BAD_CAST "dataTypeList");
			if (rc < 0) 
			{
				printf("testXmlwriterMemory: Error at xmlTextWriterStartElement\n");
				ocfmException* objException = new ocfmException;
				objException->ocfm_Excpetion(OCFM_ERR_XML_WRITER_START_ELT_FAILED);
				throw objException;
			}
			
				
				
				CComplexDataType* objCDT;
				objAppProc = objNode.getApplicationProcess();							
				
				cout << "\nobjAppProc->getCDTCount():" << objAppProc->getCDTCount() << endl;
				for(int count = 0; count < objAppProc->getCDTCount(); count++)
				{
					// Start struct Tag
					rc = xmlTextWriterStartElement(writer, BAD_CAST "struct");
					if (rc < 0) 
					{
						printf("testXmlwriterMemory: Error at xmlTextWriterStartElement\n");
						ocfmException* objException = new ocfmException;
						objException->ocfm_Excpetion(OCFM_ERR_XML_WRITER_START_ELT_FAILED);
						throw objException;
					}
					objCDT = objAppProc->getCDTbyCount(count);
					//cout << "\nobjCDT.name_id_attr.getUniqueID():" << objCDT->name_id_attr->UniqueId << endl;
					rc = xmlTextWriterWriteAttribute(writer, BAD_CAST "name", BAD_CAST objCDT->name_id_attr->getName());
					rc = xmlTextWriterWriteAttribute(writer, BAD_CAST "uniqueID", BAD_CAST objCDT->name_id_attr->UniqueId);
					
					cout << "objCDT->getVarDeclarationCount():" << objCDT->varCollection.Count() << endl;
					for(int tempVarCount = 0; tempVarCount < objCDT->varCollection.Count(); tempVarCount++)
					{
						varDeclaration vd;
						vd.Initialize();
						vd = objCDT->varCollection[tempVarCount];
						// Start varDeclaration Tag
						rc = xmlTextWriterStartElement(writer, BAD_CAST "varDeclaration");
						if (rc < 0) 
						{
							printf("testXmlwriterMemory: Error at xmlTextWriterStartElement\n");
							ocfmException* objException = new ocfmException;
							objException->ocfm_Excpetion(OCFM_ERR_XML_WRITER_START_ELT_FAILED);
							throw objException;
						}
						rc = xmlTextWriterWriteAttribute(writer, BAD_CAST "name", BAD_CAST vd.nam_id_dt_attr->getName());
						rc = xmlTextWriterWriteAttribute(writer, BAD_CAST "uniqueID", BAD_CAST vd.nam_id_dt_attr->UniqueId);
						
						//$STODO:
						//rc = xmlTextWriterWriteAttribute(writer, BAD_CAST "size", BAD_CAST vd.size);
							// Start DataType Tag							
							if(vd.nam_id_dt_attr->getDataType() != NULL)
							{
								//cout << "\nvd.nam_id_dt_attr->dataTypeUniqueIDRef:" << vd.nam_id_dt_attr->dataTypeUniqueIDRef << endl;
								//cout << "\nvd.nam_id_dt_attr->dataType:" << vd.nam_id_dt_attr->getDataType() << endl;
								rc = xmlTextWriterStartElement(writer, BAD_CAST vd.nam_id_dt_attr->getDataType());
							
								if (rc < 0) 
								{
									printf("testXmlwriterMemory: Error at xmlTextWriterStartElement\n");
									ocfmException* objException = new ocfmException;
									objException->ocfm_Excpetion(OCFM_ERR_XML_WRITER_START_ELT_FAILED);
									throw objException;
								}
								// End varDeclaration Tag
								rc = xmlTextWriterEndElement(writer);
								if (rc < 0)
								{
									printf("testXmlwriterDoc: Error at xmlTextWriterEndElement\n");
									ocfmException* objException = new ocfmException;
									objException->ocfm_Excpetion(OCFM_ERR_XML_WRITER_END_ELT_FAILED);
									throw objException;
								}
							}
							else if(vd.nam_id_dt_attr->getDtUniqueRefId() != NULL)
							{
								cout << "\nvd.nam_id_dt_attr->getDtUniqueRefId():" << vd.nam_id_dt_attr->getDtUniqueRefId() << endl;
								// Start dataTypeIDRef Tag
								rc = xmlTextWriterStartElement(writer, BAD_CAST "dataTypeIDRef");							
								if (rc < 0) 
								{
									printf("testXmlwriterMemory: Error at xmlTextWriterStartElement\n");
									ocfmException* objException = new ocfmException;
									objException->ocfm_Excpetion(OCFM_ERR_XML_WRITER_START_ELT_FAILED);
									throw objException;
								}
								
								rc = xmlTextWriterWriteAttribute(writer, BAD_CAST "uniqueIDRef", BAD_CAST vd.nam_id_dt_attr->getDtUniqueRefId());
								
								// End dataTypeIDRef Tag
								rc = xmlTextWriterEndElement(writer);
								if (rc < 0)
								{
									printf("testXmlwriterDoc: Error at xmlTextWriterEndElement\n");
									ocfmException* objException = new ocfmException;
									objException->ocfm_Excpetion(OCFM_ERR_XML_WRITER_END_ELT_FAILED);
									throw objException;
								}
							}
						
						// End varDeclaration Tag
						rc = xmlTextWriterEndElement(writer);
						if (rc < 0)
						{
							printf("testXmlwriterDoc: Error at xmlTextWriterEndElement\n");
							ocfmException* objException = new ocfmException;
							objException->ocfm_Excpetion(OCFM_ERR_XML_WRITER_END_ELT_FAILED);
							throw objException;
						}						
					}
					
					// End struct Tag
					rc = xmlTextWriterEndElement(writer);
					if (rc < 0)
					{
						printf("testXmlwriterDoc: Error at xmlTextWriterEndElement\n");
						ocfmException* objException = new ocfmException;
						objException->ocfm_Excpetion(OCFM_ERR_XML_WRITER_END_ELT_FAILED);
						throw objException;
					}
				}
										
			// End dataTypeList Tag
			rc = xmlTextWriterEndElement(writer);
			if (rc < 0)
			{
				printf("testXmlwriterDoc: Error at xmlTextWriterEndElement\n");
				ocfmException* objException = new ocfmException;
				objException->ocfm_Excpetion(OCFM_ERR_XML_WRITER_END_ELT_FAILED);
				throw objException;
			}
			
			// Start parameterList Tag
			rc = xmlTextWriterStartElement(writer, BAD_CAST "parameterList");
			if (rc < 0) 
			{
				printf("testXmlwriterMemory: Error at xmlTextWriterStartElement\n");
				ocfmException* objException = new ocfmException;
				objException->ocfm_Excpetion(OCFM_ERR_XML_WRITER_START_ELT_FAILED);
				throw objException;
			}			
			
			if(objNode.getApplicationProcess()!=NULL)
			{			
				objAppProc = objNode.getApplicationProcess();				
				cout << "\nobjAppProc->ParameterCollection.Count():" << objAppProc->ParameterCollection.Count() << endl;
				for(int count = 0; count < objAppProc->ParameterCollection.Count(); count++)
				{
					Parameter Param;
					Param = objAppProc->get_UniqueIDRef_by_ParameterIndex(count);		
					// Start parameter Tag
					rc = xmlTextWriterStartElement(writer, BAD_CAST "parameter");
					if (rc < 0) 
					{
						printf("testXmlwriterMemory: Error at xmlTextWriterStartElement\n");
						ocfmException* objException = new ocfmException;
						objException->ocfm_Excpetion(OCFM_ERR_XML_WRITER_START_ELT_FAILED);
						throw objException;
					}
					//cout << "\nParam.name_id_dt_attr.getUniqueID():" << Param.name_id_dt_attr.getUniqueID() << endl;
					rc = xmlTextWriterWriteAttribute(writer, BAD_CAST "uniqueID", BAD_CAST Param.name_id_dt_attr.getUniqueID());
					//cout << "\nParam.access:" << Param.access << endl;
					rc = xmlTextWriterWriteAttribute(writer, BAD_CAST "access", BAD_CAST Param.access);
					
					
						// Start dataTypeIDRef Tag
						rc = xmlTextWriterStartElement(writer, BAD_CAST "dataTypeIDRef");
						if (rc < 0) 
						{
							printf("testXmlwriterMemory: Error at xmlTextWriterStartElement\n");
							ocfmException* objException = new ocfmException;
							objException->ocfm_Excpetion(OCFM_ERR_XML_WRITER_START_ELT_FAILED);
							throw objException;
						}
						//cout << "\nParam.name_id_dt_attr.getDtUniqueRefId():" << Param.name_id_dt_attr.getDtUniqueRefId() << endl;
						rc = xmlTextWriterWriteAttribute(writer, BAD_CAST "uniqueIDRef", BAD_CAST Param.name_id_dt_attr.getDtUniqueRefId());
						// End dataTypeIDRef Tag
						rc = xmlTextWriterEndElement(writer);
						if (rc < 0)
						{
							printf("testXmlwriterDoc: Error at xmlTextWriterEndElement\n");
							ocfmException* objException = new ocfmException;
							objException->ocfm_Excpetion(OCFM_ERR_XML_WRITER_END_ELT_FAILED);
							throw objException;
						}
					
					// End parameter Tag
					rc = xmlTextWriterEndElement(writer);
					if (rc < 0)
					{
						printf("testXmlwriterDoc: Error at xmlTextWriterEndElement\n");
						ocfmException* objException = new ocfmException;
						objException->ocfm_Excpetion(OCFM_ERR_XML_WRITER_END_ELT_FAILED);
						throw objException;
					}
				}
			}
			// End parameterList Tag
			rc = xmlTextWriterEndElement(writer);
			if (rc < 0)
			{
				printf("testXmlwriterDoc: Error at xmlTextWriterEndElement\n");
				ocfmException* objException = new ocfmException;
				objException->ocfm_Excpetion(OCFM_ERR_XML_WRITER_END_ELT_FAILED);
				throw objException;
			}
		
		// End ApplicationProcess Tag
		rc = xmlTextWriterEndElement(writer);
		if (rc < 0)
		{
			printf("testXmlwriterDoc: Error at xmlTextWriterEndElement\n");
			ocfmException* objException = new ocfmException;
			objException->ocfm_Excpetion(OCFM_ERR_XML_WRITER_END_ELT_FAILED);
			throw objException;
		}

		// End ProfileBody Tag
		rc = xmlTextWriterEndElement(writer);
		if (rc < 0)
		{
			printf("testXmlwriterDoc: Error at xmlTextWriterEndElement\n");
			ocfmException* objException = new ocfmException;
			objException->ocfm_Excpetion(OCFM_ERR_XML_WRITER_END_ELT_FAILED);
			throw objException;
		}
		
		// Start ProfileBody Tag
		rc = xmlTextWriterStartElement(writer, BAD_CAST "ProfileBody");
		if (rc < 0) 
		{
			printf("testXmlwriterMemory: Error at xmlTextWriterStartElement\n");
			ocfmException* objException = new ocfmException;
			objException->ocfm_Excpetion(OCFM_ERR_XML_WRITER_START_ELT_FAILED);
			throw objException;
		}
		
		// Start ApplicationLayers Tag
		rc = xmlTextWriterStartElement(writer, BAD_CAST "ApplicationLayers");		
		if (rc < 0) 
		{
			printf("testXmlwriterMemory: Error at xmlTextWriterStartElement\n");
			ocfmException* objException = new ocfmException;
			objException->ocfm_Excpetion(OCFM_ERR_XML_WRITER_START_ELT_FAILED);
			throw objException;
		}
		
		// Start DataTypeList Tag
		rc = xmlTextWriterStartElement(writer, BAD_CAST "DataTypeList");
		if (rc < 0) 
		{
			printf("testXmlwriterMemory: Error at xmlTextWriterStartElement\n");
			ocfmException* objException = new ocfmException;
			objException->ocfm_Excpetion(OCFM_ERR_XML_WRITER_START_ELT_FAILED);
			throw objException;
		}
			
		//objIndexPtr = objIndexCollection->getNumberOfDataTypes;
		
		CDataTypeCollection *objDataTypeCollection;
		
		objDataTypeCollection = objNode.getDataTypeCollection();
		
		for(int i = 0; i < objDataTypeCollection->getNumberOfDataTypes(); i++)
		{		
			DataType* dt = NULL;
			dt = objDataTypeCollection->getDataTypeElement(i);
			//char temp_DTValue[50] = dt->DataTypeValue;
			//char temp_DTName[50] = dt->getName();			
			
			// Start defType Tag
			rc = xmlTextWriterStartElement(writer, BAD_CAST "defType");	
			if (rc < 0) 
			{
				printf("testXmlwriterMemory: Error at xmlTextWriterStartElement\n");
				ocfmException* objException = new ocfmException;
				objException->ocfm_Excpetion(OCFM_ERR_XML_WRITER_START_ELT_FAILED);
				throw objException;
			}
			
			rc = xmlTextWriterWriteAttribute(writer, BAD_CAST "dataType", BAD_CAST dt->DataTypeValue);			
			// Start DataType Tag
			rc = xmlTextWriterStartElement(writer, BAD_CAST dt->getName());			
			if (rc < 0) 
			{
				printf("testXmlwriterMemory: Error at xmlTextWriterStartElement\n");
				ocfmException* objException = new ocfmException;
				objException->ocfm_Excpetion(OCFM_ERR_XML_WRITER_START_ELT_FAILED);
				throw objException;
			}
			xmlTextWriterSetIndent(writer, 1);
			// End DataType Tag
			rc = xmlTextWriterEndElement(writer);
			if (rc < 0)
			{
				printf("testXmlwriterDoc: Error at xmlTextWriterEndElement\n");
				ocfmException* objException = new ocfmException;
				objException->ocfm_Excpetion(OCFM_ERR_XML_WRITER_END_ELT_FAILED);
				throw objException;				
			}
			xmlTextWriterSetIndent(writer, 1);
			// End defType Tag
			rc = xmlTextWriterEndElement(writer);
			if (rc < 0)
			{
				printf("testXmlwriterDoc: Error at xmlTextWriterEndElement\n");
				ocfmException* objException = new ocfmException;
				objException->ocfm_Excpetion(OCFM_ERR_XML_WRITER_END_ELT_FAILED);
				throw objException;				
			}
			
		}	
		xmlTextWriterSetIndent(writer, 1);
		// End DataTypeList Tag
		rc = xmlTextWriterEndElement(writer);
		if (rc < 0)
		{
			printf("testXmlwriterDoc: Error at xmlTextWriterEndElement\n");
			ocfmException* objException = new ocfmException;
			objException->ocfm_Excpetion(OCFM_ERR_XML_WRITER_END_ELT_FAILED);
			throw objException;
		}
		
		
		if(objIndexCollection->getNumberofIndexes() == 0)
		{
			ocfmException objException;				
			objException.ocfm_Excpetion(OCFM_ERR_NO_INDEX_FOUND);
			throw objException;
		}
		
		// Start ObjectList Tag
		rc = xmlTextWriterStartElement(writer, BAD_CAST "ObjectList");
		if (rc < 0) 
		{
			printf("testXmlwriterMemory: Error at xmlTextWriterStartElement\n");
			ocfmException* objException = new ocfmException;
			objException->ocfm_Excpetion(OCFM_ERR_XML_WRITER_START_ELT_FAILED);
			throw objException;
		}
		
		for(IndexPos = 0; IndexPos < objIndexCollection->getNumberofIndexes(); IndexPos++)		
		{		
			// Start Object Tag
			rc = xmlTextWriterStartElement(writer, BAD_CAST "Object");
			if (rc < 0) 
			{
				printf("testXmlwriterMemory: Error at xmlTextWriterStartElement\n");
				ocfmException* objException = new ocfmException;
				objException->ocfm_Excpetion(OCFM_ERR_XML_WRITER_START_ELT_FAILED);
				throw objException;
			}
			
			objIndexPtr = objIndexCollection->getIndex(IndexPos);
			
			if(objIndexPtr->getIndexValue() != NULL)
			{
				rc = xmlTextWriterWriteAttribute(writer, BAD_CAST "index", BAD_CAST objIndexPtr->getIndexValue());
				//cout << "\nobjIndexPtr->getIndexValue():" << objIndexPtr->getIndexValue() << endl;				
			}
			if(objIndexPtr->getName() != NULL)
				rc = xmlTextWriterWriteAttribute(writer, BAD_CAST "name", BAD_CAST objIndexPtr->getName());
			if(objIndexPtr->getObjectType() != NULL)
				rc = xmlTextWriterWriteAttribute(writer, BAD_CAST "objectType", BAD_CAST objIndexPtr->getObjectType());
			DataType tmpDataType;
			tmpDataType = objIndexPtr->getDataType();		
			if(tmpDataType.DataTypeValue != NULL)// TODO:$S
			if(strlen(tmpDataType.DataTypeValue) != 0)
			{				
				//cout << "\nstrlen(tmpDataType.DataTypeValue):" << strlen(tmpDataType.DataTypeValue) << endl;
				rc = xmlTextWriterWriteAttribute(writer, BAD_CAST "dataType", BAD_CAST tmpDataType.DataTypeValue);
				//cout << "\ntmpDataType.DataTypeValue:" << tmpDataType.DataTypeValue << endl;
			}
			if(objIndexPtr->getAccessType() != NULL)
				rc = xmlTextWriterWriteAttribute(writer, BAD_CAST "accessType", BAD_CAST objIndexPtr->getAccessType());
			if(objIndexPtr->getDefaultValue() != NULL)
				rc = xmlTextWriterWriteAttribute(writer, BAD_CAST "defaultValue", BAD_CAST objIndexPtr->getDefaultValue());
			if(objIndexPtr->getActualValue() != NULL)
				rc = xmlTextWriterWriteAttribute(writer, BAD_CAST "actualValue", BAD_CAST objIndexPtr->getActualValue());
			if(objIndexPtr->getPDOMapping() != NULL)
				rc = xmlTextWriterWriteAttribute(writer, BAD_CAST "PDOmapping", BAD_CAST objIndexPtr->getPDOMapping());
			if(objIndexPtr->getUniqueIDRef() != NULL)
				rc = xmlTextWriterWriteAttribute(writer, BAD_CAST "uniqueIDRef", BAD_CAST objIndexPtr->getUniqueIDRef());
				
			xmlTextWriterSetIndent(writer, 1);
								
			int SubIndexPos = 0;
			
			if(!objIndexPtr->getNumberofSubIndexes() <= 0)
			{					
				for(SubIndexPos = 0; SubIndexPos < objIndexPtr->getNumberofSubIndexes(); SubIndexPos++)
				{
					
					// Start SubObject Tag
					rc = xmlTextWriterStartElement(writer, BAD_CAST "SubObject");
					if (rc < 0) 
					{
						printf("testXmlwriterMemory: Error at xmlTextWriterStartElement\n");
						ocfmException* objException = new ocfmException;
						objException->ocfm_Excpetion(OCFM_ERR_XML_WRITER_START_ELT_FAILED);
						throw objException;
					}
					
					CSubIndex* objSubIndexPtr;
					objSubIndexPtr = objIndexPtr->getSubIndex(SubIndexPos);	
					//SubObject subIndex="01" name="NodeID_U8" objectType="7" dataType="0005" accessType="rw" defaultValue="0"
					if(objSubIndexPtr->getIndexValue() != NULL)
						rc = xmlTextWriterWriteAttribute(writer, BAD_CAST "subIndex", BAD_CAST objSubIndexPtr->getIndexValue());
					if(objSubIndexPtr->getName() != NULL)
						rc = xmlTextWriterWriteAttribute(writer, BAD_CAST "name", BAD_CAST objSubIndexPtr->getName());
					if(objSubIndexPtr->getObjectType() != NULL)
						rc = xmlTextWriterWriteAttribute(writer, BAD_CAST "objectType", BAD_CAST objSubIndexPtr->getObjectType());
					DataType tmpDataType;
					tmpDataType = objSubIndexPtr->getDataType();		
					if(tmpDataType.DataTypeValue != NULL)			
						rc = xmlTextWriterWriteAttribute(writer, BAD_CAST "dataType", BAD_CAST tmpDataType.DataTypeValue);
					if(objSubIndexPtr->getAccessType() != NULL)
						rc = xmlTextWriterWriteAttribute(writer, BAD_CAST "accessType", BAD_CAST objSubIndexPtr->getAccessType());
					if(objSubIndexPtr->getDefaultValue() != NULL)
						rc = xmlTextWriterWriteAttribute(writer, BAD_CAST "defaultValue", BAD_CAST objSubIndexPtr->getDefaultValue());
					if(objSubIndexPtr->getActualValue() != NULL)
						rc = xmlTextWriterWriteAttribute(writer, BAD_CAST "actualValue", BAD_CAST objSubIndexPtr->getActualValue());
					if(objSubIndexPtr->getPDOMapping() != NULL)
						rc = xmlTextWriterWriteAttribute(writer, BAD_CAST "PDOmapping", BAD_CAST objSubIndexPtr->getPDOMapping());
					if(objSubIndexPtr->getUniqueIDRef() != NULL)
						rc = xmlTextWriterWriteAttribute(writer, BAD_CAST "uniqueIDRef", BAD_CAST objSubIndexPtr->getUniqueIDRef());
					// End SubObject Tag
					rc = xmlTextWriterEndElement(writer);
					if (rc < 0)
					{
						printf("testXmlwriterDoc: Error at xmlTextWriterEndElement\n");
						ocfmException* objException = new ocfmException;
						objException->ocfm_Excpetion(OCFM_ERR_XML_WRITER_END_ELT_FAILED);
						throw objException;				
					}
					
				}
			}
			
			// End Object Tag
			rc = xmlTextWriterEndElement(writer);
			if (rc < 0)
			{
				printf("testXmlwriterDoc: Error at xmlTextWriterEndElement\n");
				ocfmException* objException = new ocfmException;
				objException->ocfm_Excpetion(OCFM_ERR_XML_WRITER_END_ELT_FAILED);
				throw objException;				
			}
			
		}							
		
		xmlTextWriterSetIndent(writer, 1);
		// End ObjectList Tag
		rc = xmlTextWriterEndElement(writer);
		if (rc < 0)
		{
			printf("testXmlwriterDoc: Error at xmlTextWriterEndElement\n");
			ocfmException* objException = new ocfmException;
			objException->ocfm_Excpetion(OCFM_ERR_XML_WRITER_END_ELT_FAILED);
			throw objException;
		}
		
		xmlTextWriterSetIndent(writer, 1);
		// End ApplicationLayers Tag
		rc = xmlTextWriterEndElement(writer);
		if (rc < 0)
		{
			printf("testXmlwriterDoc: Error at xmlTextWriterEndElement\n");
			ocfmException* objException = new ocfmException;
			objException->ocfm_Excpetion(OCFM_ERR_XML_WRITER_END_ELT_FAILED);
			throw objException;
		}
		
		xmlTextWriterSetIndent(writer, 1);
		// End ProfileBody Tag
		rc = xmlTextWriterEndElement(writer);
		if (rc < 0)
		{
			printf("testXmlwriterDoc: Error at xmlTextWriterEndElement\n");
			ocfmException* objException = new ocfmException;
			objException->ocfm_Excpetion(OCFM_ERR_XML_WRITER_END_ELT_FAILED);
			throw objException;
		}
		
		xmlTextWriterSetIndent(writer, 1);
		// End ISO15745ProfileContainer Tag
		rc = xmlTextWriterEndElement(writer);
		if (rc < 0)
		{
			printf("testXmlwriterDoc: Error at xmlTextWriterEndElement\n");
			ocfmException* objException = new ocfmException;
			objException->ocfm_Excpetion(OCFM_ERR_XML_WRITER_END_ELT_FAILED);
			throw objException;
		}
				
		cout << "\nEnd Write Doc\n" <<endl;
		rc = xmlTextWriterEndDocument(writer);
		if (rc < 0) 
		{
			printf("testXmlwriterDoc: Error at xmlTextWriterEndDocument\n");
			ocfmException* objException = new ocfmException;
			objException->ocfm_Excpetion(OCFM_ERR_XML_END_DOC_FAILED);
			throw objException;
		}

		xmlFreeTextWriter(writer);

		xmlSaveFileEnc(fileName, doc, MY_ENCODING);

		xmlFreeDoc(doc);
	}
	catch(ocfmException *ex)
	{
		throw ex;
	}

	ErrStruct.code = OCFM_ERR_SUCCESS;
	return ErrStruct;	
}
