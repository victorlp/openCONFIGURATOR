///////////////////////////////////////////////////////////////////////////////////////////////
//
//  $Header: $
//
// NAME:  Validation.cpp
//
// BASE  CLASSES: none
//  
// PURPOSE:  purpose description
//
// AUTHOR:   Kalycito Powerlink Team
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
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include "../Include/Validation.h"
#include "../Include/Exception.h"

/****************************************************************************************************
* FUNCTION DEFINITIONS
****************************************************************************************************/
/**************************************************************************************************
* Function Name: IfNodeExists
* Description:	Checks for existance of a Node
* Return value: ocfmRetCode
****************************************************************************************************/
ocfmRetCode IfNodeExists(INT32 iNodeID, ENodeType iNodeType, INT32 *piNodePos, bool& ExistfFlag)
{	
	CNode objNode;		
	CNodeCollection *pobjNodeCollection = NULL;
	ocfmRetCode stErrStruct;
	
	try
	{				
		pobjNodeCollection = CNodeCollection::getNodeColObjectPointer();	
		if(pobjNodeCollection == NULL)
		{
			cout<< "objNodeCollection is NULL!" << endl;
		}
		
		if( pobjNodeCollection->getNumberOfNodes() > 0)
		{
			for(INT32 iLoopCount = 0; iLoopCount < pobjNodeCollection->getNumberOfNodes(); iLoopCount++)
			{
				objNode = pobjNodeCollection->getNodebyCollectionIndex(iLoopCount);
				
				if (objNode.getNodeType() == iNodeType)
				{
					if(objNode.getNodeId() == iNodeID)
					{
						//printf("Node Pos:%d\n\n", iLoopCount);
						//stErrStruct.returnValue = iLoopCount;
						*piNodePos 		 = iLoopCount;
						stErrStruct.code = OCFM_ERR_SUCCESS;
						ExistfFlag		 = true;
						
						return stErrStruct;
					}					
				}						
			}
			/*stErrStruct.code = OCFM_ERR_NODEID_NOT_FOUND;
			return stErrStruct;*/
			ocfmException objException;
			objException.ocfm_Excpetion(OCFM_ERR_NODEID_NOT_FOUND);
			throw &objException;			
		}
		else
		{
			ocfmException objException;
			objException.ocfm_Excpetion(OCFM_ERR_NO_NODES_FOUND);
			throw &objException;	
		}
	}
	catch(ocfmException* ex)
	{
		return ex->_ocfmRetCode;
	}
}

/**************************************************************************************************
* Function Name: IfIndexExists
* Description: Checks for the existance of a Index.
* Return value: -2 if the Index doesnot exist. Returns Zero and fills the error string on Index existance or 
  if Node doesn't exist or if NodeType is invalid. 
****************************************************************************************************/
ocfmRetCode IfIndexExists(INT32 iNodeID, ENodeType enumNodeType, char* pbIndexID, INT32 *piIndexPos)
{
	CNode objNode;
	CIndex objIndex;
	ocfmRetCode stErrStruct;
	INT32 iNodePos;
	bool bFlag = false;
	CNodeCollection *pobjNodeCollection		= NULL;
	CIndexCollection *pobjIndexCollection	= NULL;
	
	try
	{
		stErrStruct = IfNodeExists(iNodeID, enumNodeType, &iNodePos, bFlag );
		
		if (stErrStruct.code == OCFM_ERR_SUCCESS && bFlag == true) 
		{		
			//retPos = stErrStruct.returnValue;
		}
		else
		{	
			ocfmException objException;				
			objException.ocfm_Excpetion(OCFM_ERR_INVALID_NODEID);
			throw objException;
		}
		pobjNodeCollection = CNodeCollection::getNodeColObjectPointer();
		objNode = pobjNodeCollection->getNode(enumNodeType, iNodeID);
		pobjIndexCollection = objNode.getIndexCollection();		

		if(pobjIndexCollection->getNumberofIndexes() == 0)
		{
			*piIndexPos = 0;
			stErrStruct.code = OCFM_ERR_NO_INDEX_FOUND;	
			return stErrStruct;		
		}		
		else if(pobjIndexCollection->getNumberofIndexes() > 0)
		{
			//Check for existance of the Index
			for(INT32 iIndexCount = 0; iIndexCount < pobjIndexCollection->getNumberofIndexes(); iIndexCount++)
			{
				CIndex* objIndexPtr = NULL;
				char *pbIndexValue  = NULL;
				
				objIndexPtr = pobjIndexCollection->getIndex(iIndexCount);										
				pbIndexValue = new char[strlen((char*)objIndexPtr->getIndexValue()) + ALLOC_BUFFER];				
				strcpy(pbIndexValue, (char*)objIndexPtr->getIndexValue());
										
				if((strcmp(ConvertToUpper(pbIndexValue), ConvertToUpper(pbIndexID)) == 0))
				{
					CIndex* objIndexPtr = NULL;
					
					objIndexPtr = pobjIndexCollection->getIndex(iIndexCount);				
					if(objIndexPtr->getActualValue() == NULL)
					{
						//cout<< "objIndex.getActualValue():NULL" << endl;
					}
					else
					{
						//cout<< "objIndex.getActualValue():" << objIndexPtr->getActualValue() << endl;
					}
					//delete[] pbIndexValue;
					*piIndexPos 	 = iIndexCount;
					stErrStruct.code = OCFM_ERR_SUCCESS;
					return stErrStruct;
				}
				else if(iIndexCount == (pobjIndexCollection->getNumberofIndexes() - 1))
				{
					//delete[] pbIndexValue;
					// Index Doesn't Exist
					stErrStruct.code = OCFM_ERR_INDEXID_NOT_FOUND;
					return stErrStruct;
				}
				else
				{
					//delete[] pbIndexValue;
				}
			}
		}
		// Index Doesn't Exist
		ocfmException objException;				
		objException.ocfm_Excpetion(OCFM_ERR_UNKNOWN);
		throw objException;
	}
	catch(ocfmException* ex)
	{
		return ex->_ocfmRetCode;
	}
}

/**************************************************************************************************
* Function Name: IfSubIndexExists
* Description: Checks for the existance of a SubIndex.
* Return value: ocfmRetCode
****************************************************************************************************/
ocfmRetCode IfSubIndexExists(INT32 iNodeID, ENodeType enumNodeType, char* pbIndexID, char* pbSubIndexID, INT32* piSubIndexPos, INT32* piIndexPos)
{
	CNode objNode;	
	CIndex objIndex;
	CNodeCollection *pobjNodeCollection		= NULL;
	CIndexCollection *pobjIndexCollection	= NULL;
	CIndex* pobjSubIndex					= NULL;
	ocfmRetCode stErrStruct;
	
	try
	{
		stErrStruct = IfIndexExists(iNodeID, enumNodeType, pbIndexID, piIndexPos);
		
		if (stErrStruct.code == 0)
		{				
			//piIndexPos = stErrStruct.returnValue;
		}
		else
		{	
			// Node Doesn't Exist
			stErrStruct.code = OCFM_ERR_INDEXID_NOT_FOUND;
			return stErrStruct;			
		}
		objIndex.setNodeID(objNode.getNodeId());
		pobjNodeCollection 	= CNodeCollection::getNodeColObjectPointer();
		objNode 			= pobjNodeCollection->getNode(enumNodeType, iNodeID);

		pobjIndexCollection = objNode.getIndexCollection();
		pobjSubIndex 		= pobjIndexCollection->getIndex(*piIndexPos);
		if(pobjSubIndex->getNumberofSubIndexes() == 0)
		{
			stErrStruct.code = OCFM_ERR_NO_SUBINDEXS_FOUND;
			return stErrStruct;
		}
		else if(pobjSubIndex->getNumberofSubIndexes() > 0)
		{
			//Check for existance of the SubIndex
			for(INT32 iSubIndexcount = 0; iSubIndexcount < pobjSubIndex->getNumberofSubIndexes(); iSubIndexcount++)
			{
				CSubIndex* objSubIndexPtr = NULL;
				
				objSubIndexPtr = pobjSubIndex->getSubIndex(iSubIndexcount);						
				if((strcmp(ConvertToUpper((char*) objSubIndexPtr->getIndexValue()), ConvertToUpper(pbSubIndexID)) == 0))
				{			
					stErrStruct.code 	= OCFM_ERR_SUCCESS;
					*piSubIndexPos 		= iSubIndexcount;
					return stErrStruct;
				}
				else if(iSubIndexcount == (pobjSubIndex->getNumberofSubIndexes() - 1))
				{
					// SubIndex Doesn't Exist
					stErrStruct.code = OCFM_ERR_SUBINDEXID_NOT_FOUND;
					return stErrStruct;
				}
			}
		}
		ocfmException objException;				
		objException.ocfm_Excpetion(OCFM_ERR_UNKNOWN);
		throw objException;		
	}
	catch(ocfmException* ex)
	{
		return ex->_ocfmRetCode;
	}
}

/**************************************************************************************************
* Function Name: CheckIfDataTypeExists
* Description:	
* Return value: bool
****************************************************************************************************/
bool CheckIfDataTypeExists(char* pbDataValue, INT32 iNodeID)
{
	CNodeCollection* pobjNodeCol 		= NULL;
	CDataTypeCollection* pobjDataCol 	= NULL;
	DataType* pobjDataType 			 	= NULL;
	
	pobjNodeCol		= CNodeCollection::getNodeColObjectPointer();
	CNode objNode 	= pobjNodeCol->getNode(iNodeID);
	pobjDataCol		= objNode.getDataTypeCollection();
	
	pobjDataType 	= pobjDataCol->getDataType(pbDataValue);
	
	if (pobjDataType == NULL)
	{
		return false;
	}
	else
	{
		return true;
	}
}

/**************************************************************************************************
* Function Name: CheckIfDataTypeByNameExists
* Description:	
* Return value: bool
****************************************************************************************************/
bool CheckIfDataTypeByNameExists(char* dtName, int NodeID)
{
	CNodeCollection* objNodeCol;
	objNodeCol= CNodeCollection::getNodeColObjectPointer();
	CNode objNode = objNodeCol->getNode(NodeID);
	CDataTypeCollection* dtcol;
	dtcol=objNode.getDataTypeCollection();
	DataType* dt;
	dt = dtcol->getDataTypeByName(dtName); 
	if (dt == NULL)
		return false;
	else 
		return true;
}

/**************************************************************************************************
* Function Name: CheckIfSubIndexExists
* Description:	
* Return value: bool
****************************************************************************************************/
bool CheckIfSubIndexExists(INT32 iNodeID, ENodeType enumNodeType, char* pbIndexID, char* pbSubIndexID)
{
	CNode objNode;		
	CIndex objIndex;
	CNodeCollection *pobjNodeCollection 	= NULL;
	CIndexCollection *pobjIndexCollection	= NULL;
	CIndex			 *pobjIdx				= NULL;				
	
	cout << "\n inside CheckIfSubIndexExists\n\n";
	pobjNodeCollection	= CNodeCollection::getNodeColObjectPointer();
	cout << "\n\n\n Nodeid\n\n" << iNodeID;
	objNode				= pobjNodeCollection->getNode(enumNodeType, iNodeID);
	cout << "\n\n\n 1\n\n" << endl;
	pobjIndexCollection = objNode.getIndexCollection();
	cout << "\n\n\n 2\n\n" << endl;
	pobjIdx 			= pobjIndexCollection->getIndexbyIndexValue(pbIndexID);
	/*if(pobjIdx == NULL)
	{
		cout << "\n\nGOT NULL!!!!!!!!\n\n" << endl;
		return false;	
	}	*/	
	cout << "\n\n\n 3\n\n" << endl;
	//cout << "\n\n\nNumberofSubIndexes:" << pobjIdx->getNumberofSubIndexes()<< endl;
	cout << "\n\n\n 4\n\n" << endl;
	if(pobjIdx->getNumberofSubIndexes() == 0)
	{
		return false;
	}
	else if(pobjIdx->getNumberofSubIndexes() > 0)
	{
		//Check for existance of the SubIndex
		for(INT32 iSubIndexCount = 0; iSubIndexCount < pobjIdx->getNumberofSubIndexes(); iSubIndexCount++)
		{
			CSubIndex* objSubIndexPtr;
			objSubIndexPtr = pobjIdx->getSubIndex(iSubIndexCount);						
			printf("SubIndexValue:%s-%s\n", objSubIndexPtr->getIndexValue(), pbSubIndexID);
			if((strcmp(StringToUpper((char*) objSubIndexPtr->getIndexValue()), StringToUpper(pbSubIndexID)) == 0))
			{
				return true;
			}
			else if(iSubIndexCount == (pobjIdx->getNumberofSubIndexes() - 1))
			{
				return false;
			}
		}
	}	
	return false;
}

/**************************************************************************************************
* Function Name: IfVersionNumberMatches
* Description:	
* Return value: bool
****************************************************************************************************/
bool IfVersionNumberMatches(xmlTextReaderPtr pxReader)
{
	const xmlChar* pxcName	=	NULL;
	const xmlChar* pxcValue	=	NULL;
	//Retrieve the name and Value of an attribute
	pxcValue 	= xmlTextReaderConstValue(pxReader);
	pxcName 	= xmlTextReaderConstName(pxReader);
	
	if(pxcValue == NULL || pxcName == NULL)
	{
		return false;
	}
	#if defined DEBUG
		cout << "\nName:" << pxcName << endl;	
		cout << "\nValue:" << pxcValue << endl;
	#endif
	// Check for Version Tool-Project Version
	if(strcmp(ConvertToUpper((char*)pxcName), "VERSION") == FALSE)
	{
		if( (strcmp(ConvertToUpper((char*)pxcValue), TOOL_VERSION) == FALSE) ||
			(strcmp(ConvertToUpper((char*)pxcValue), PREV_TOOL_VERSION) == FALSE) )
		{
			#if defined DEBUG
				cout << "\nVersion number matched" << endl;
			#endif
			return true;
		}
		else
		{
			#if defined DEBUG
				cout << "\nVersion number MisMatch" << endl;
			#endif
			return false;
			
		}
	}
	else
	{
		cout << "\nError! IfVersionNumberMatches function can't find VERSION" << endl;
		return false;	
	}
}
