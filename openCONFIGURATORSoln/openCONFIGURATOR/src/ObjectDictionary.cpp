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
    
	value = xmlTextReaderConstValue(reader);    
	try
	{
		if((name == NULL) || (value == NULL))
		{
			cout << "Got NULL for name or value\n" << endl;
		}		
		
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
						setDataTypeAttributes(reader, &objDataType);																									
					}
				}
				objDataTypeCollection = objDictNode->getDataTypeCollection();
				objDataTypeCollection->addDataType(objDataType);
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
						{
							createSameattrObject((char*)value, INDEX,(char*) objIndex.getIndexValue());
						}							
						else 
						{
							setIndexAttributes(reader, &objIndex, hasPDO);								
						}
					}
				}					
				objIndexCollection = objDictNode->getIndexCollection();
				
				//Add Index object to the IndexCollection
				objIndexCollection->addIndex(objIndex);				
				iLastObjDictIndexParsed = objIndexCollection->getNumberofIndexes()-1;
			}
			else if(strcmp(((char*)name),"SubObject")==0 )
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
							
						else 
						{
							setSubIndexAttributes(reader,&objSubIndex);
						}							
					}
				}
				objIndexPtr->addSubIndex(objSubIndex);					
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
	CIndex objIndex;
	
	pobjIndexCol = objDictNode->getIndexCollection();
	pobjIndex 	 = pobjIndexCol->getIndexbyIndexValue(pbIdx);
	
	if(pobjIndex != NULL)
	{		
		//pobjIndex->setName(getIndexName(subString(pbIdx,2,4), (char*)pobjIndex->getName()));
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
					//pobjIndex->setName(getIndexName(subString(pbIdx,2,4), (char*)pobjIndex->getName()));
					//name = strchr(pobjIndex->getName(), "X");					
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
	for(INT32 iLoopCount=0; iLoopCount < collectionObj.Count(); iLoopCount++)
	{
		s_attrIdx_SIdx stAttrIdx;
		stAttrIdx = collectionObj[iLoopCount];
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

/**************************************************************************************************
* Function Name: CObjectDictionary::getIndexName
* Description:
* Return value: char*
****************************************************************************************************/
char* CObjectDictionary::getIndexName(char* pbObjectIndex, char* pbObjectName)
{
	//char* pbName = new char[strlen(pbObjectName) +  ALLOC_BUFFER];
	char* pbName = NULL;
	char* pbModifiedName = new char[strlen(pbObjectName) +  ALLOC_BUFFER];		
	
	int iLen;
	if(pbObjectName == NULL)
	return NULL;
	
	pbName = strchr(pbObjectName, 'X')	;
	if(pbName != NULL)
	{
		iLen = 1;
		if(strcmp(subString(pbName,1,1), "X") ==0)
		iLen++;
	
		int pos = strlen(pbName);
		int iCount = strlen(pbObjectIndex) - iLen;
		//int iLoopCount = 0;
		strcpy(pbModifiedName, subString(pbObjectName, 0, strlen(pbObjectName)-pos));	
		strcat(pbModifiedName, subString(pbObjectIndex, iCount, iLen ));
		//while(iLoopCount < iLen)
		//{
		//	//if(Name[0] == 'X' && len==2)
		//	if(pbName[0] == 'X')
		//	{
		//		
		//		//pbName[0] = *(pbObjectIndex + iCount);
		//	}
		//	//pbName++;
		//	iLoopCount++;	
		//	iCount++;
		//}
		strcat(pbModifiedName, subString(pbName, iLen, strlen(pbName) - iLen));	
		//printf("\n ModifiedName %s",pbModifiedName);
		return pbModifiedName;
	}
	else
	{ 
		return pbObjectName;
	}
}
