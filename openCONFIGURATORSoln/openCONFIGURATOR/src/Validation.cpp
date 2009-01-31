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
#include <iostream>
#include <stdio.h>
#include <errno.h>
#include <string.h>

/**************************************************************************************************
	* Function Name: IfNodeExists
    * Description:	Checks for existance of a Node
	Returns -1 and fills the error string if the node doesnot exist. Returns the node position 
	[signed integer] on existance.
	
	Return value Legend:
	On success	- OCFM_ERR_SUCCESS
	On error	- Return appropriate error code
	Node Doesn't Exist 		- -1
	Index Doesn't Exist 	- -2
	SubIndex Doesn't Exist 	- -3
	Invalid NodeType		- -4
/****************************************************************************************************/
ocfmRetValError IfNodeExists(int NodeID, ENodeType NodeType)
{	
	cout << "NodeType:" << NodeType << endl;
	int count;
	CNode objNode;		
	CNodeCollection *objNodeCollection = NULL;
	ocfmRetValError ErrStruct;
	
	// TODO: Check for Invalid NodeType. Yet to Implement
		//if(NodeType == 1)
		//temp_ret 
	//else if(NodeType == 0)
		//temp_ret = IfNodeExists(NodeID, NodeType, ErrStr);
	//else
	//{
	//	printf("Invalid Node Type");
	//	return -4;
	//}
	
	objNodeCollection = CNodeCollection::getNodeColObjectPointer();	
	if(objNodeCollection == NULL)
	{
		cout<< "objNodeCollection is NULL!!!" << endl;
		exit;
	}
	cout<< "getNumberOfNodes: \n" <<objNodeCollection->getNumberOfNodes()<<endl;
	if( objNodeCollection->getNumberOfNodes() > 0)
	{
		for(int count = 0; count < objNodeCollection->getNumberOfNodes(); count++)
		{
				objNode = objNodeCollection->getNodebyCollectionIndex(count);
				if (objNode.getNodeType() == NodeType)
				{
					if(objNode.getNodeId() == NodeID)
					{
						printf("Node Pos:%d\n\n", count);
						ErrStruct.returnValue = count;
						ErrStruct.errCode.code = OCFM_ERR_SUCCESS;
						return ErrStruct;
					}
					else
					{
						//Node Doesn't match
					}
				}
					
		}
	}
	else
	{
			ErrStruct.errCode.code = OCFM_ERR_NO_NODES_FOUND;
			return ErrStruct;
	}
	ErrStruct.errCode.code = OCFM_ERR_INVALID_NODEID;
	return ErrStruct;
}

/**************************************************************************************************
* Function Name: IfIndexExists
* Description: Checks for the existance of a Index.
Returns -2 if the Index doesnot exist. Returns Zero and fills the error string on Index existance or 
if Node doesn't exist or if NodeType is invalid. 
	Return value Legend:
	Node Doesn't Exist 		- -1
	Index Doesn't Exist 	- -2
	SubIndex Doesn't Exist 	- -3
	Invalid NodeType		- -4
/****************************************************************************************************/
ocfmRetValError IfIndexExists(int NodeID, ENodeType NodeType, char* IndexID)
{
	int count;
	CNode objNode;		
	CNodeCollection *objNodeCollection;
	CIndexCollection *objIndexCollection;
	CIndex objIndex;
	ocfmRetValError ErrStruct;
	int retPos;

	ErrStruct = IfNodeExists(NodeID, NodeType);
	if (ErrStruct.errCode.code == 0)
	{		
		retPos = ErrStruct.returnValue;
	}
	else
	{	
		cout << "\n\nErrStruct.errCode.code:" << ErrStruct.errCode.code << "\n\n!!!" << endl;
		// Node Doesn't Exist
		ErrStruct.errCode.code = OCFM_ERR_INVALID_NODEID;
		return ErrStruct;
	}
	//else
	//{	
		objNodeCollection = CNodeCollection::getNodeColObjectPointer();
		objNode = objNodeCollection->getNode(NodeType, NodeID);
		objIndexCollection = objNode.getIndexCollection();
		
		//cout <<"Node Exists: objIndexCollection->getNumberofIndexes()" << objIndexCollection->getNumberofIndexes() <<endl;

		if(objIndexCollection->getNumberofIndexes() == 0)
		{
			//AddIndexAttributes(IndexID, &objIndex);
			//objIndexCollection->addIndex(objIndex);	
			printf("Index Doesn't exist\n\n");
			// Index Doesn't Exist
			//return -2;
			ErrStruct.errCode.code = OCFM_ERR_INVALID_INDEXID;
			return ErrStruct;
		}
		
		else if(objIndexCollection->getNumberofIndexes() > 0)
		{
			//Check for existance of the Index
			for(int tmpIndexcount = 0; tmpIndexcount < objIndexCollection->getNumberofIndexes(); tmpIndexcount++)
			{
				CIndex* objIndexPtr;
				objIndexPtr =objIndexCollection->getIndex(tmpIndexcount);						
				printf("IndexValue:%s-%s\n", objIndexPtr->getIndexValue(), IndexID);
				if((strcmp(objIndexPtr->getIndexValue(), IndexID) == 0))
				{
					printf("Index Already Exists tmpIndexcount:%d!\n",tmpIndexcount);
					CIndex* objIndexPtr;
					objIndexPtr = objIndexCollection->getIndex(tmpIndexcount);
					cout<< "objIndex.getName():" << objIndexPtr->getName() << endl;
					cout<< "objIndex.getActualValue():" << objIndexPtr->getActualValue() << endl;
					
					ErrStruct.returnValue = tmpIndexcount;
					ErrStruct.errCode.code = OCFM_ERR_SUCCESS;
					return ErrStruct;
					//return tmpIndexcount;
				}
				else if(tmpIndexcount == (objIndexCollection->getNumberofIndexes() - 1))
				{
					printf("Index Doesn't exist \n\n");
					// Index Doesn't Exist
					ErrStruct.errCode.code = OCFM_ERR_INVALID_INDEXID;
					return ErrStruct;
					//return -2;
				}
			}
		}
	//}
	// Index Doesn't Exist
	printf("\n\nIndex Doesn't Exist\n\n\n");
	ErrStruct.errCode.code = OCFM_ERR_INVALID_INDEXID;
	return ErrStruct;
	//return -2;
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
/****************************************************************************************************/
ocfmRetValError IfSubIndexExists(int NodeID, ENodeType NodeType, char* IndexID, char* SubIndexID)
{
		int count;
		CNode objNode;		
		CNodeCollection *objNodeCollection;
		CIndexCollection *objIndexCollection;
		CIndex objIndex;
		CIndex* objSubIndex;
		ocfmRetValError ErrStruct;
		
		int IndexPos = -1;
		ErrStruct = IfIndexExists(NodeID, NodeType, IndexID);
		if (ErrStruct.errCode.code == 0)
		{				
			IndexPos = ErrStruct.returnValue;
		}
		else
		{	
			cout << "\n\nErrStruct.errCode.code:" << ErrStruct.errCode.code << "\n\n!!!" << endl;
			// Node Doesn't Exist
			ErrStruct.errCode.code = OCFM_ERR_INVALID_INDEXID;
			return ErrStruct;
		}
			cout <<"Index Exists"<<endl;
			objIndex.setNodeID(objNode.getNodeId());
			objNodeCollection= CNodeCollection::getNodeColObjectPointer();
			objNode = objNodeCollection->getNode(NodeType, NodeID);

			objIndexCollection = objNode.getIndexCollection();
			objSubIndex =objIndexCollection->getIndex(IndexPos);
			cout << "NumberofSubIndexes:" << objSubIndex->getNumberofSubIndexes()<< endl;
			if(objSubIndex->getNumberofSubIndexes() == 0)
			{
				cout << "SubIndex Doesn't Exist" << endl;
				ErrStruct.errCode.code = OCFM_ERR_INVALID_SUBINDEXID;
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
					printf("SubIndexValue:%s-%s\n", objSubIndexPtr->getIndexValue(), SubIndexID);
					if((strcmp(objSubIndexPtr->getIndexValue(), SubIndexID) == 0))
					{
						printf("SubIndex Already Exists tmpIndexcount:%d!\n",tmpSubIndexcount);				
						ErrStruct.errCode.code = OCFM_ERR_SUCCESS;
						ErrStruct.returnValue = tmpSubIndexcount;
						return ErrStruct;						
						//return tmpSubIndexcount;
					}
					else if(tmpSubIndexcount == (objSubIndex->getNumberofSubIndexes() - 1))
					{
						printf("SubIndex Doesn't exist \n\n");
						// SubIndex Doesn't Exist
						ErrStruct.errCode.code = OCFM_ERR_INVALID_SUBINDEXID;
						return ErrStruct;
						//return -3;
					}
				}
			}
		
	ErrStruct.errCode.code = OCFM_ERR_INVALID_SUBINDEXID;
	return ErrStruct;
	//return -3;
}
