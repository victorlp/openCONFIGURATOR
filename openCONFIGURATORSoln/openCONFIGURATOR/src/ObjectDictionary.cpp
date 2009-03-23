///////////////////////////////////////////////////////////////////////////////////////////////
//
//  $Header: $
//
// NAME:  ObjectDictionary.cpp
//
// BASE  CLASSES: none
//  
// PURPOSE:  purpose description
//
// AUTHOR:  Kalycito Powerlink Team
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

/****************************************************************************************************
* Includes
****************************************************************************************************/
#include <iostream>
#include "../Include/ObjectDictionary.h"
#include "../Include/Internal.h"
#include "../Include/Exception.h"

using namespace std;

/****************************************************************************************************
* Global Variables
****************************************************************************************************/
INT32 iLastObjDictIndexParsed			 = 0;
bool CObjectDictionary::instanceFlag	 = false;
CObjectDictionary* CObjectDictionary::objectDictionary = NULL;
CNode* CObjectDictionary::objDictNode = NULL;

/****************************************************************************************************
* FUNCTION DEFINITIONS
****************************************************************************************************/

/****************************************************************************************************
* Constructor
****************************************************************************************************/
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

/****************************************************************************************************
* Destructor
****************************************************************************************************/
CObjectDictionary::~CObjectDictionary(void)
{
		//Add destructor code here
}
	
/****************************************************************************************************
* Function Name: CObjectDictionary::getObjDictPtr
* Description:
* Return value: CObjectDictionary*
****************************************************************************************************/
CObjectDictionary* CObjectDictionary::getObjDictPtr()
{
		if(!instanceFlag)
		{
				objectDictionary = new CObjectDictionary();
				//printf("\n Object Created");
				instanceFlag = true;
		}
		return objectDictionary;
		
}
	
/**************************************************************************************************
* Function Name: CObjectDictionary::ProcessObjectDictionary
* Description: Process the Node value,Name and its attributes
* Return value: void
****************************************************************************************************/
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
						iLastObjDictIndexParsed = objIndexCollection->getNumberofIndexes()-1;
						//printf("\n index %s Indexes added",objIndex.getIndexValue());
				}
		else	if(strcmp(((char*)name),"SubObject")==0 )
				{
					CIndexCollection* objIndexCollection;
					CSubIndex objSubIndex;
					bool same = false;
					CIndex* objIndexPtr;								
					objIndexCollection =objDictNode->getIndexCollection();
					objIndexPtr =objIndexCollection->getIndex(iLastObjDictIndexParsed);
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
* Function Name: CObjectDictionary::addSameAttributesObjects
* Description: add the SubIndex in the Index Object
* Return value: void
****************************************************************************************************/

void CObjectDictionary::addSameAttributesObjects(s_attrIdx_SIdx stAttrIdx)
{
		INT32 iItemPosition = collectionObj.Add();
		
		collectionObj[iItemPosition] = stAttrIdx ;
		m_s_attrIdx_SIdx = collectionObj.Count();
		
}
	
/**************************************************************************************************
* Function Name: CObjectDictionary::createSameattrObject
* Description: 
* Return value: void
****************************************************************************************************/
void CObjectDictionary::createSameattrObject(char* pbValue, ObjectType enumObjType, char* pbIdx )
{
		
		s_attrIdx_SIdx stAttrIdx;											
		char* pbSubIdx = new char[RANGE_INDEX];
		char *pbEIdx   = NULL;
									
		pbEIdx = strchr(pbValue, '-');			
		if(pbEIdx != NULL)
		{
		pbSubIdx = subString(pbValue, 0, strlen(pbEIdx)-1);
		pbEIdx = subString(pbValue, strlen(pbSubIdx) + 1, strlen(pbValue));
		
		
		stAttrIdx.objectType  = enumObjType;
		stAttrIdx.Idx		  = new char[INDEX_LEN];
		
		if(enumObjType == INDEX) 
		{
			stAttrIdx.start_Index = new char[INDEX_LEN];
			stAttrIdx.end_Index	  = new char[INDEX_LEN];
			strcpy(stAttrIdx.end_Index, subString(pbIdx, 0, 4 - strlen(pbSubIdx)));
			strcat(stAttrIdx.end_Index, pbEIdx);
			strcpy(stAttrIdx.start_Index, pbIdx);	
		}
		else 
		{
			stAttrIdx.start_Index = new char[SUBINDEX_LEN];
			stAttrIdx.end_Index		 = new char[SUBINDEX_LEN];
			strcpy(stAttrIdx.start_Index, pbSubIdx);
			strcpy(stAttrIdx.end_Index, pbEIdx);
		}				
			
		strcpy(stAttrIdx.Idx, pbIdx);
		addSameAttributesObjects(stAttrIdx); 
}
		//delete [] e_idx;
		delete [] pbSubIdx;
}
/**************************************************************************************************
* Function Name: CObjectDictionary::getObjectDictIndex
* Description:
* Return value: CIndex*
****************************************************************************************************/
CIndex* CObjectDictionary::getObjectDictIndex(char* pbIdx)
{
	CIndex* pobjIndex = NULL;
	CIndexCollection* pobjIndexCol;
	
	pobjIndexCol = objDictNode->getIndexCollection();
	pobjIndex 	 = pobjIndexCol->getIndexbyIndexValue(pbIdx);
	
	if(pobjIndex != NULL)
	{
		return pobjIndex;
	}
	else
	{
		for(INT32 iLoopCount=0; iLoopCount < collectionObj.Count(); iLoopCount++)
		{
			s_attrIdx_SIdx stAttrIdx;
			stAttrIdx = collectionObj[iLoopCount];
			
			if(stAttrIdx.objectType == INDEX)
			{
				if(checkInTheRange(pbIdx, stAttrIdx.start_Index, stAttrIdx.end_Index))
				{
					pobjIndex = pobjIndexCol->getIndexbyIndexValue(stAttrIdx.start_Index);
				/*	name = strchr(pobjIndex->getName(), "X");
					*/
					return pobjIndex;
				}
			}
		}
		return NULL;
	}
}

/**************************************************************************************************
* Function Name: CObjectDictionary::getObjectDictSubIndex
* Description:
* Return value: CSubIndex*
****************************************************************************************************/
CSubIndex* CObjectDictionary::getObjectDictSubIndex(char* pbIdx, char* pbSIdx)
{
	CSubIndex* pobjSIdx = NULL;
	CIndex* pobjIndex   = NULL;
	CIndexCollection* pobjIndexCol;
	
	pobjIndexCol = objDictNode->getIndexCollection();
	pobjIndex = pobjIndexCol->getIndexbyIndexValue(pbIdx);
	
	if(pobjIndex == NULL)
	{
		pobjIndex = getObjectDictIndex(pbIdx);
		
		if(pobjIndex != NULL)
		{
			pbIdx =  (char*)pobjIndex->getIndexValue();
		}
		else
		{
			return pobjSIdx;
		}
	}
	
	pobjSIdx = pobjIndex->getSubIndexbyIndexValue(pbSIdx);
	
	if(pobjSIdx!=NULL)
	{
		return pobjSIdx;
	}
	else
	{
		for(INT32 iLoopCount = 0; iLoopCount < collectionObj.Count(); iLoopCount++)
		{
			s_attrIdx_SIdx stAttrIdx;
			stAttrIdx = collectionObj[iLoopCount];
			
			if((stAttrIdx.objectType == SUBINDEX) && (strcmp(stAttrIdx.Idx, pbIdx)==0) )
			{
				if(checkInTheRange(pbSIdx, stAttrIdx.start_Index, stAttrIdx.end_Index))
				{
					pobjSIdx = pobjIndex->getSubIndexbyIndexValue(stAttrIdx.start_Index);
					
					if(pobjSIdx !=NULL)
					{
						return pobjSIdx;
					}
				}
			}
		}
		return NULL;
	}
}

/**************************************************************************************************
* Function Name: CObjectDictionary::checkInTheRange
* Description: 
* Return value: bool
****************************************************************************************************/
bool CObjectDictionary::checkInTheRange(char* pbIdx, char* pbStartIdx, char* pbEndIdx)
{
	if(hex2int(pbIdx) >= hex2int(pbStartIdx)&& (hex2int(pbIdx) <= hex2int(pbEndIdx)))
	{
		return true;
	}
	else
	{
		return false;
	}
}

/**************************************************************************************************
* Function Name: CObjectDictionary::printall
* Description:
* Return value: void
****************************************************************************************************/
void CObjectDictionary::printall()
{
	//printf("\n Index		StartIndex		EndIndex		ObjectType\n");

	for(INT32 iLoopCount=0; iLoopCount < collectionObj.Count(); iLoopCount++)
	{
				s_attrIdx_SIdx stAttrIdx;
				stAttrIdx = collectionObj[iLoopCount];
				//printf("\n %s  		%s  		%s	  	%d",stAttrIdx.Idx, stAttrIdx.start_Index, stAttrIdx.end_Index, stAttrIdx.objectType);
	}
}

/**************************************************************************************************
* Function Name: CObjectDictionary::ifObjectDictIndexExists
* Description: 
* Return value: int
****************************************************************************************************/
INT32 CObjectDictionary::ifObjectDictIndexExists(char* pbIdx)
{
	CIndex* pobjIndex = NULL;
	CIndexCollection* pobjIndexCol;
	
	pobjIndexCol = objDictNode->getIndexCollection();
	pobjIndex 	 = pobjIndexCol->getIndexbyIndexValue(pbIdx);
	
	if(pobjIndex == NULL)
	{
		return TRUE;		
	}
	else
	{
		return FALSE;
	}
}

/**************************************************************************************************
* Function Name: CObjectDictionary::ifObjectDictSubIndexExists
* Description:
* Return value: int
****************************************************************************************************/
INT32 CObjectDictionary::ifObjectDictSubIndexExists(char* pbIdx, char* pbSIdx)
{
	CSubIndex* pobjSIdx = NULL;
	CIndex* pobjIndex = NULL;
	CIndexCollection* pobjIndexCol;
	
	pobjIndexCol = objDictNode->getIndexCollection();
	pobjIndex 	 = pobjIndexCol->getIndexbyIndexValue(pbIdx);

	if(pobjIndex == NULL)
	{
		return TRUE;
	}

	pobjSIdx = pobjIndex->getSubIndexbyIndexValue(pbSIdx);
	
	if(pobjSIdx == NULL)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
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
