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

#include "../Include/Validation.h"
#include "../Include/Exception.h"
#include <iostream>
#include <stdio.h>
#include <errno.h>
#include <string.h>

/**************************************************************************************************
* Function Name: IfNodeExists
* Description:	Checks for existance of a Node
* Return value: ocfmRetCode
****************************************************************************************************/
ocfmRetCode IfNodeExists(int NodeID, ENodeType NodeType, int* NodePos, bool& ExistfFlag)
{	
	//cout << "NodeType:" << NodeType << endl;
	CNode objNode;		
	CNodeCollection *objNodeCollection = NULL;
	ocfmRetCode ErrStruct;
	try
	{
		//if(NodeType != CN || NodeType != MN)
		//{
		//	ocfmException* objException = new ocfmException;
		//	objException->ocfm_Excpetion(OCFM_ERR_INVALID_NODETYPE);
		//	throw objException;	
		//}
				
		objNodeCollection = CNodeCollection::getNodeColObjectPointer();	
		if(objNodeCollection == NULL)
		{
			//cout<< "objNodeCollection is NULL!!!" << endl;
		}
		//cout<< "getNumberOfNodes: \n" <<objNodeCollection->getNumberOfNodes()<<endl;
		if( objNodeCollection->getNumberOfNodes() > 0)
		{
			for(int count = 0; count < objNodeCollection->getNumberOfNodes(); count++)
			{
				objNode = objNodeCollection->getNodebyCollectionIndex(count);
				if (objNode.getNodeType() == NodeType)
				{
					if(objNode.getNodeId() == NodeID)
					{
						//printf("Node Pos:%d\n\n", count);
						//ErrStruct.returnValue = count;
						*NodePos = count;
						ErrStruct.code = OCFM_ERR_SUCCESS;
						ExistfFlag = true;
						return ErrStruct;
					}					
				}						
			}
			/*ErrStruct.code = OCFM_ERR_NODEID_NOT_FOUND;
			return ErrStruct;*/
			ocfmException objException;// = new ocfmException;
			objException.ocfm_Excpetion(OCFM_ERR_NODEID_NOT_FOUND);
			throw &objException;	
			
		}
		else
		{
			ocfmException objException;// = new ocfmException;
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
Returns -2 if the Index doesnot exist. Returns Zero and fills the error string on Index existance or 
if Node doesn't exist or if NodeType is invalid. 

****************************************************************************************************/
ocfmRetCode IfIndexExists(int NodeID, ENodeType NodeType, char* IndexID, int *IndexPos)
{
	CNode objNode;		
	CNodeCollection *objNodeCollection;
	CIndexCollection *objIndexCollection;
	CIndex objIndex;
	ocfmRetCode ErrStruct;
	int NodePos;
	bool flag = false;
	try
	{
		ErrStruct = IfNodeExists(NodeID, NodeType, &NodePos, flag );
		if (ErrStruct.code == 0 && flag ==true) 
		{		
			//retPos = ErrStruct.returnValue;
		}
		else
		{	
			//cout << "\n\nErrStruct.errCode.code:" << ErrStruct.code << "\n\n!!!" << endl;
			// Node Doesn't Exist
			//ErrStruct.code = OCFM_ERR_INVALID_NODEID;
			//return ErrStruct;
			ocfmException objException;				
			objException.ocfm_Excpetion(OCFM_ERR_INVALID_NODEID);
			throw objException;
		}
		objNodeCollection = CNodeCollection::getNodeColObjectPointer();
		objNode = objNodeCollection->getNode(NodeType, NodeID);
		objIndexCollection = objNode.getIndexCollection();
		
		//cout <<"Node Exists: objIndexCollection->getNumberofIndexes()" << objIndexCollection->getNumberofIndexes() <<endl;

		if(objIndexCollection->getNumberofIndexes() == 0)
		{
			//AddIndexAttributes(IndexID, &objIndex);
			//objIndexCollection->addIndex(objIndex);	
			//printf("Index Doesn't exist!!\n\n");
			// Index Doesn't Exist
			//return -2;
			//ErrStruct.code = OCFM_ERR_NO_INDEX_FOUND;
			//ErrStruct.errorString = NULL;
			*IndexPos = 0;
			//return ErrStruct;
			//ocfmException objException;			
			ErrStruct.code = OCFM_ERR_NO_INDEX_FOUND;	
				return ErrStruct;
			//objException.ocfm_Excpetion(OCFM_ERR_NO_INDEX_FOUND);
			//throw objException;
		}
		
		else if(objIndexCollection->getNumberofIndexes() > 0)
		{
			//Check for existance of the Index
			for(int tmpIndexcount = 0; tmpIndexcount < objIndexCollection->getNumberofIndexes(); tmpIndexcount++)
			{
				CIndex* objIndexPtr;
				objIndexPtr =objIndexCollection->getIndex(tmpIndexcount);						
				//printf("IndexValue:%s-%s\n", objIndexPtr->getIndexValue(), IndexID);
				char *str = new char[50];
					strcpy(str, (char*)objIndexPtr->getIndexValue());
								
		
				if((strcmp(ConvertToUpper(str), ConvertToUpper(IndexID)) == 0))
				{
					//printf("Index Already Exists tmpIndexcount:%d!\n",tmpIndexcount);
					CIndex* objIndexPtr;
					objIndexPtr = objIndexCollection->getIndex(tmpIndexcount);
					//cout<< "objIndex.getName():" << objIndexPtr->getName() << endl;
					if(objIndexPtr->getActualValue() == NULL)
					{
						//cout<< "objIndex.getActualValue():NULL" << endl;
					}
					else
					{
						//cout<< "objIndex.getActualValue():" << objIndexPtr->getActualValue() << endl;
					}
					
					*IndexPos = tmpIndexcount;
					ErrStruct.code = OCFM_ERR_SUCCESS;
					return ErrStruct;
				}
				else if(tmpIndexcount == (objIndexCollection->getNumberofIndexes() - 1))
				{
					//printf("Index Doesn't exist.. \n\n");
					// Index Doesn't Exist
					ErrStruct.code = OCFM_ERR_INDEXID_NOT_FOUND;
					return ErrStruct;
					//ocfmException objException;				
					//objException.ocfm_Excpetion(OCFM_ERR_INDEXID_NOT_FOUND);
					//throw objException;
				}
			}
		}
		// Index Doesn't Exist
		//printf("\n\n$S Index Doesn't Exist\n\n\n");
		//ErrStruct.code = OCFM_ERR_UNKNOWN;
		//return ErrStruct;
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
Returns -3 if the SubIndex doesnot exist. Returns Zero and fills the error string on SubIndex existance or 
if Node doesn't exist or if NodeType is invalid. 
	Return value Legend:
	Node Doesn't Exist 		- -1
	Index Doesn't Exist 	- -2
	SubIndex Doesn't Exist 	- -3
	Invalid NodeType		- -4
****************************************************************************************************/
ocfmRetCode IfSubIndexExists(int NodeID, ENodeType NodeType, char* IndexID, char* SubIndexID, int* SubIndexPos, int* IndexPos)
{
		CNode objNode;		
		CNodeCollection *objNodeCollection;
		CIndexCollection *objIndexCollection;
		CIndex objIndex;
		CIndex* objSubIndex;
		ocfmRetCode ErrStruct;
		try
		{
			ErrStruct = IfIndexExists(NodeID, NodeType, IndexID, IndexPos);
			if (ErrStruct.code == 0)
			{				
				//IndexPos = ErrStruct.returnValue;
			}
			else
			{	
				//cout << "\n\nErrStruct.errCode.code:" << ErrStruct.code << "\n\n!!!" << endl;
				// Node Doesn't Exist
				ErrStruct.code = OCFM_ERR_INDEXID_NOT_FOUND;
				return ErrStruct;
				//ocfmException objException;				
				//objException.ocfm_Excpetion(OCFM_ERR_INDEXID_NOT_FOUND);
				//throw objException;
			}
				//cout <<"Index Exists"<<endl;
				objIndex.setNodeID(objNode.getNodeId());
				objNodeCollection= CNodeCollection::getNodeColObjectPointer();
				objNode = objNodeCollection->getNode(NodeType, NodeID);

				objIndexCollection = objNode.getIndexCollection();
				objSubIndex =objIndexCollection->getIndex(*IndexPos);
				//cout << "NumberofSubIndexes:" << objSubIndex->getNumberofSubIndexes()<< endl;
				if(objSubIndex->getNumberofSubIndexes() == 0)
				{
					//cout << "SubIndex Doesn't Exist" << endl;
					//ocfmException objException;				
					//objException.ocfm_Excpetion(OCFM_ERR_NO_SUBINDEXS_FOUND);
					//throw objException;
					ErrStruct.code = OCFM_ERR_NO_SUBINDEXS_FOUND;
					return ErrStruct;
					//return -3;
				}
				else if(objSubIndex->getNumberofSubIndexes() > 0)
				{
					//Check for existance of the SubIndex
					for(int tmpSubIndexcount = 0; tmpSubIndexcount < objSubIndex->getNumberofSubIndexes(); tmpSubIndexcount++)
					{
						CSubIndex* objSubIndexPtr;
						objSubIndexPtr = objSubIndex->getSubIndex(tmpSubIndexcount);						
						//printf("SubIndexValue:%s-%s\n", objSubIndexPtr->getIndexValue(), SubIndexID);
						if((strcmp(ConvertToUpper((char*) objSubIndexPtr->getIndexValue()), ConvertToUpper(SubIndexID)) == 0))
						{
							//printf("SubIndex Already Exists tmpIndexcount:%d!\n",tmpSubIndexcount);				
							ErrStruct.code = OCFM_ERR_SUCCESS;
							*SubIndexPos = tmpSubIndexcount;
							return ErrStruct;
							//return tmpSubIndexcount;
						}
						else if(tmpSubIndexcount == (objSubIndex->getNumberofSubIndexes() - 1))
						{
							//printf("SubIndex Doesn't exist \n\n");
							// SubIndex Doesn't Exist
							ErrStruct.code = OCFM_ERR_SUBINDEXID_NOT_FOUND;
							return ErrStruct;
							//ocfmException objException;				
							//objException.ocfm_Excpetion(OCFM_ERR_SUBINDEXID_NOT_FOUND);
							//throw objException;
							//return -3;
						}
					}
				}
			
		//ErrStruct.code = OCFM_ERR_UNKNOWN;
		//return ErrStruct;
		ocfmException objException;				
		objException.ocfm_Excpetion(OCFM_ERR_UNKNOWN);
		throw objException;
		
	}
	catch(ocfmException* ex)
	{
		return ex->_ocfmRetCode;
	}
}
bool CheckIfDataTypeExists(char* dtValue, int NodeID)
{
	CNodeCollection* objNodeCol;
	objNodeCol= CNodeCollection::getNodeColObjectPointer();
	CNode objNode = objNodeCol->getNode(NodeID);
	CDataTypeCollection* dtcol;
	dtcol=objNode.getDataTypeCollection();
	DataType* dt;
	dt = dtcol->getDataType(dtValue);
	if (dt== NULL)
	return false;
	else return true;
}

bool CheckIfSubIndexExists(int NodeID, ENodeType NodeType, char* IndexID, char* SubIndexID)
{
		CNode objNode;		
		CNodeCollection *objNodeCollection;
		CIndexCollection *objIndexCollection;
		CIndex objIndex;
		CIndex* objIdx;
		objIndex.setNodeID(objNode.getNodeId());
		objNodeCollection= CNodeCollection::getNodeColObjectPointer();
		objNode = objNodeCollection->getNode(NodeType, NodeID);

		objIndexCollection = objNode.getIndexCollection();
		objIdx =objIndexCollection->getIndexbyIndexValue(IndexID);
		//cout << "NumberofSubIndexes:" << objSubIndex->getNumberofSubIndexes()<< endl;
				if(objIdx->getNumberofSubIndexes() == 0)
				{
					return false;
				}
				else if(objIdx->getNumberofSubIndexes() > 0)
				{
					//Check for existance of the SubIndex
					for(int tmpSubIndexcount = 0; tmpSubIndexcount < objIdx->getNumberofSubIndexes(); tmpSubIndexcount++)
					{
						CSubIndex* objSubIndexPtr;
						objSubIndexPtr = objIdx->getSubIndex(tmpSubIndexcount);						
						//printf("SubIndexValue:%s-%s\n", objSubIndexPtr->getIndexValue(), SubIndexID);
						if((strcmp(ConvertToUpper((char*) objSubIndexPtr->getIndexValue()), ConvertToUpper(SubIndexID)) == 0))
						{
							return true;
						}
						else if(tmpSubIndexcount == (objIdx->getNumberofSubIndexes() - 1))
						{
							return false;
						}
					}
				}	
			return false;
}

bool IfVersionNumberMatches(xmlTextReaderPtr reader)
{
	const xmlChar* name,*value;
	//Retrieve the name and Value of an attribute
	value = xmlTextReaderConstValue(reader);
	name = xmlTextReaderConstName(reader);
	if(value == NULL || name == NULL)
		return false;
	#if defined DEBUG
		cout << "\nName:" << name << endl;	
		cout << "\nValue:" << value << endl;
	#endif
	// Check for Version Tool-Project Version
	if(strcmp(ConvertToUpper((char*)name), "VERSION") == 0)
	{
		if(strcmp(ConvertToUpper((char*)value), TOOL_VERSION) == 0)
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
