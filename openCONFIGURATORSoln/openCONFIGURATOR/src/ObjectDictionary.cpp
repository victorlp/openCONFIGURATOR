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
#include "../Include/ObjectDictionary.h"
#include "../Include/Internal.h"
#include "../Include/Exception.h"
#include <iostream>
using namespace std;
int LastObjDictIndexParsed=0;
CObjectDictionary::CObjectDictionary(void)
	{	
			objDictNode = new CNode();		
			objDictNode->setNodeId(-100);
			objDictNode->CreateIndexCollection();
			objDictNode->CreateDataTypeCollection();
			objDictNode->CreateApplicationProcess();
			//printf("\n Object collections Created");
			m_s_attrIdx_SIdx = collectionObj.Count();
		}

CObjectDictionary::~CObjectDictionary(void)
	{
		
	}
bool CObjectDictionary::instanceFlag=false;
CObjectDictionary* CObjectDictionary::objectDictionary = NULL;
CNode* CObjectDictionary::objDictNode = NULL;
CObjectDictionary* CObjectDictionary::getObjDictPtr()
	{
		if(!instanceFlag)
			{
				objectDictionary = new CObjectDictionary();
				//printf("\n Object Created");
				instanceFlag=true;
			}
		return objectDictionary;
		
	}
	/**************************************************************************************************
	* Function Name: ProcessObjectDictionary
    * Description: Process the Node value,Name and its attributes
/****************************************************************************************************/
void CObjectDictionary::ProcessObjectDictionary(xmlTextReaderPtr reader)
{
		const xmlChar *name, *value;	
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
						

					objDataTypeCollection = objDictNode->getDataTypeCollection();
					objDataTypeCollection->addDataType(objDataType);
					//printf("\n Datatypes added");

				}
			
			else if(strcmp(((char*)name),"Object")==0)
				{
					CIndexCollection* objIndexCollection;
					CIndex objIndex;		
											
							//Set the NodeID
						objIndex.setNodeID(-100);
			
					if (xmlTextReaderHasAttributes(reader)==1)
						{	
											
							while(xmlTextReaderMoveToNextAttribute(reader))
								{
									value = xmlTextReaderConstValue(reader);
									name = xmlTextReaderConstName(reader);	
									bool hasPDO;
									if(strcmp(ConvertToUpper((char*)name), "DATATYPE")==0)		
									{			
										//objIndex.setDataTypeValue((char*)value);
										DataType *dt;
										dt = objDictNode->getDataTypeCollection()->getDataType((char*)value);
										objIndex.setDataTypeST(*dt);
									}
									else if(strcmp(ConvertToUpper((char*)name), "RANGE")==0)								
										createSameattrObject((char*)value, INDEX,(char*) objIndex.getIndexValue());
										
									
									else setIndexAttributes(reader, &objIndex, hasPDO);	
									//$Svalue = xmlTextReaderConstValue(reader);
									//$Sprintf("\tValue_2:%s\n", value);
									//$Sname =xmlTextReaderConstName(reader);																								
								}
						}
						
						objIndexCollection = objDictNode->getIndexCollection();
						
						//Add Index object to the IndexCollection
						objIndexCollection->addIndex(objIndex);				
						LastObjDictIndexParsed = objIndexCollection->getNumberofIndexes()-1;
						//printf("\n index %s Indexes added",objIndex.getIndexValue());
				}
		else	if(strcmp(((char*)name),"SubObject")==0 )
				{
					CIndexCollection* objIndexCollection;
					CSubIndex objSubIndex;
					bool same = false;
					CIndex* objIndexPtr;								
					objIndexCollection =objDictNode->getIndexCollection();
					objIndexPtr =objIndexCollection->getIndex(LastObjDictIndexParsed);
						//Set the NodeID
						objSubIndex.setNodeID(-100);
			
					if (xmlTextReaderHasAttributes(reader) ==1)
						{
							
							while(xmlTextReaderMoveToNextAttribute(reader))
								{
									value = xmlTextReaderConstValue(reader);
									name = xmlTextReaderConstName(reader);	
									if(strcmp(ConvertToUpper((char*)name), "DATATYPE")==0)		
									{			
										/*objSubIndex.setDataTypeValue((char*)value);*/
										DataType *dt;
										dt = objDictNode->getDataTypeCollection()->getDataType((char*)value);
										objSubIndex.setDataTypeST(*dt);
									}
									else if(strcmp(ConvertToUpper((char*)name), "RANGE")==0)								
									{
										same =true;
										createSameattrObject((char*)value, SUBINDEX, (char*)objIndexPtr->getIndexValue());										
									}
										
								else setSubIndexAttributes(reader,&objSubIndex);
									//value = xmlTextReaderConstValue(reader);
									//printf("\t\tValue_3:%s\n", value);
									//name =xmlTextReaderConstName(reader);																									
								}
						}
				
					
							objIndexPtr->addSubIndex(objSubIndex);
								//printf("\n  subIndexes added");
						//if (same)
						//	{
						//		//printf("\n Same ********* Attr Index Name: %s",objIndexPtr->getName());
						//	}
				
						
				}	
			}
		
		}
		catch(ocfmException* ex)
		{
			 throw ex;
		}	
}
/**************************************************************************************************
	* Function Name: addSubIndex
    * Description: add the SubIndex in the Index Object
/****************************************************************************************************/

void CObjectDictionary::addSameAttributesObjects(s_attrIdx_SIdx object)
	{
		int i = collectionObj.Add();
		collectionObj[i] = object ;
		m_s_attrIdx_SIdx = collectionObj.Count();
		
	}
void CObjectDictionary::createSameattrObject(char* value, ObjectType objType, char*Idx )
{
		
		s_attrIdx_SIdx object;											
		char* s_idx, *e_idx = new char[3];
									
		e_idx = strchr(value, '-');			
		s_idx = subString(value, 0, strlen(e_idx)-1);
		e_idx = subString(e_idx, 1, strlen(e_idx));
		
		object.start_Index = new char[5];
		object.end_Index		 = new char[5];
		object.objectType = objType;
		object.Idx								= new char[5];
		
		if(objType == INDEX) 
		{
			strcpy(object.end_Index, subString(Idx, 0, 4 - strlen(s_idx)));
			strcat(object.end_Index, e_idx);
			strcpy(object.start_Index, Idx);	
		}
		else 
		{
			strcpy(object.start_Index, s_idx);
			strcpy(object.end_Index, e_idx);
		}				
			
		strcpy(object.Idx, Idx);
		addSameAttributesObjects(object); 
}
CIndex* CObjectDictionary::getObjectDictIndex(char* Idx)
{
	CIndex* objIndex = NULL;
	CIndexCollection* objIndexCol;
	objIndexCol = objDictNode->getIndexCollection();
	objIndex = objIndexCol->getIndexbyIndexValue(Idx);
	if(objIndex != NULL)
	{
		return objIndex;
	}
	else
	{
		for(int i=0; i<collectionObj.Count(); i++)
		{
			s_attrIdx_SIdx obj;
			obj = collectionObj[i];
			if(obj.objectType == INDEX)
			{
				if(checkInTheRange(Idx, obj.start_Index, obj.end_Index))
				{
					objIndex = objIndexCol->getIndexbyIndexValue(obj.start_Index);
				/*	name = strchr(objIndex->getName(), "X");
					*/
					return objIndex;
				}
			}
		}
		return NULL;
	}
}
CSubIndex* CObjectDictionary::getObjectDictSubIndex(char* Idx, char* SIdx)
{
 CSubIndex* objSIdx = NULL;
 CIndex* objIndex = NULL;
 CIndexCollection* objIndexCol;
	objIndexCol = objDictNode->getIndexCollection();
	objIndex = objIndexCol->getIndexbyIndexValue(Idx);
	
	if(objIndex == NULL)
	{
		objIndex = getObjectDictIndex(Idx);
		if(objIndex!= NULL)
		{
			Idx =  (char*)objIndex->getIndexValue();
		}
		else
		return objSIdx;
	}

	
	objSIdx = objIndex->getSubIndexbyIndexValue(SIdx);
	if(objSIdx!=NULL)
	return objSIdx;
 
	else
	{
		for(int i=0; i<collectionObj.Count(); i++)
		{
			s_attrIdx_SIdx obj;
			obj = collectionObj[i];
			if((obj.objectType == SUBINDEX) && (strcmp(obj.Idx, Idx)==0) )
			{
				if(checkInTheRange(SIdx, obj.start_Index, obj.end_Index))
				{
					objSIdx = objIndex->getSubIndexbyIndexValue(obj.start_Index);
					if(objSIdx !=NULL)
					return objSIdx;
				}
			}
		}
		return NULL;
	}
}
bool CObjectDictionary::checkInTheRange(char* Idx, char* StartIdx, char* EndIdx)
{
	if(hex2int(Idx) >= hex2int(StartIdx)&& (hex2int(Idx) <= hex2int(EndIdx)))
	return true;
	else return false;
}
void CObjectDictionary::printall()
{
	//printf("\n Index		StartIndex		EndIndex		ObjectType\n");

	for(int i=0; i<collectionObj.Count(); i++)
	{
				s_attrIdx_SIdx obj;
				obj = collectionObj[i];
		//printf("\n %s  		%s  		%s	  	%d",obj.Idx, obj.start_Index, obj.end_Index, obj.objectType);
}
}

int CObjectDictionary::ifObjectDictIndexExists(char* Idx)
{
	CIndex* objIndex = NULL;
	CIndexCollection* objIndexCol;
	objIndexCol = objDictNode->getIndexCollection();
	objIndex = objIndexCol->getIndexbyIndexValue(Idx);
	if(objIndex == NULL)
	{
		return 1;		
	}
	else
		return 0;
}

int CObjectDictionary::ifObjectDictSubIndexExists(char* Idx, char* SIdx)
{
	CSubIndex* objSIdx = NULL;
	CIndex* objIndex = NULL;
	CIndexCollection* objIndexCol;
	objIndexCol = objDictNode->getIndexCollection();
	objIndex = objIndexCol->getIndexbyIndexValue(Idx);

	if(objIndex == NULL)
	return 1;

	objSIdx = objIndex->getSubIndexbyIndexValue(SIdx);
	if(objSIdx == NULL)
		return 1;
	else
		return 0;
}
//char* setIndexName(char* ObjectIndex, char* ObjectName)
//{
//	char* Name = new char[100];
//	if(strlen(ObjectIndex) == 2)
//	Name = strchr(ObjectName, 'X');
//	int pos = strlen(Name);
//	int i = 0;
//	while(i < strlen(ObjectIndex))	
//	{
//		if(Name[0] == 'X')
//		{
//			Name = ObjectIndex;
//		}
//		Name++;
//		ObjectIndex;		
//	}
//	ObjectName = subString(ObjectName, 0, strlen(ObjectName)-pos);
//	strcat(ObjectName, Name);	
//}
