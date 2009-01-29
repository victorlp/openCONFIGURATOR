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
	Node Doesn't Exist 		- -1
	Index Doesn't Exist 	- -2
	SubIndex Doesn't Exist 	- -3
	Invalid NodeType		- -4
/****************************************************************************************************/
int IfNodeExists(int NodeID, ENodeType NodeType, char* ErrStr)
{	
	cout << "NodeType:" << NodeType << endl;
	int count;
	CNode objNode;		
	CNodeCollection *objNodeCollection = NULL;	
	// TODO: Check for Invalid NodeType. Yet to Implement
	objNodeCollection = CNodeCollection::getNodeColObjectPointer();	
	if(objNodeCollection == NULL)
	{
		cout<< "objNodeCollection is NULL!!!" << endl;
		exit;
	}
	cout<< "getNumberOfNodes: \n" <<objNodeCollection->getNumberOfNodes()<<endl;
	//cout << "." <<endl;
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
						return count;
					}
	//				//else if(count == (objNodeCollection->getNumberOfNodes() -1 ))
					else
					{
						//strcpy(ErrStr, "Node Doesn't exist");
						//return -1;
					}
				}
					
		}
	}
	else
	{
			// TODO: Beware!!!. The following line is a room for BUGS!
			//strcpy(ErrStr, "No Nodes found!");
			printf("No Nodes found!\n\n");
			return -1;
	}
	// TODO: Beware!!!. The following line is a room for BUGS!
	//strcpy(ErrStr, "Node Doesn't exist");
	printf("Node Doesn't exist\n\n");
	return -1;

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
int IfIndexExists(int NodeID, ENodeType NodeType, char* IndexID, char* ErrStr)
{
	int count;
	CNode objNode;		
	CNodeCollection *objNodeCollection;
	CIndexCollection *objIndexCollection;
	CIndex objIndex;

	int temp_ret = -1;
	//if(NodeType == 1)
		temp_ret = IfNodeExists(NodeID, NodeType, ErrStr);
	//else if(NodeType == 0)
		//temp_ret = IfNodeExists(NodeID, NodeType, ErrStr);
	//else
	//{
	//	printf("Invalid Node Type");
	//	return -4;
	//}
	if(temp_ret < 0)
	{
		printf("\n\n\nErr:%s!!\n\n\n", ErrStr);
		// Node Doesn't Exist
		return -1;
	}
	else
	{	
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
			return -2;
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
					return tmpIndexcount;
				}
				else if(tmpIndexcount == (objIndexCollection->getNumberofIndexes() - 1))
				{
					printf("Index Doesn't exist \n\n");
					// Index Doesn't Exist
					return -2;
				}
			}
		}
	}
	// Index Doesn't Exist
	printf("\n\nIndex Doesn't Exist\n\n\n");
	return -2;
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
int IfSubIndexExists(int NodeID, ENodeType NodeType, char* IndexID, char* SubIndexID, char* ErrStr)
{
		int count;
		CNode objNode;		
		CNodeCollection *objNodeCollection;
		CIndexCollection *objIndexCollection;
		CIndex objIndex;
		CIndex* objSubIndex;
		
		int IndexPos = -1;
		//if(NodeType == 1)
			IndexPos = IfIndexExists(NodeID, NodeType, IndexID, ErrStr);
		//else if(NodeType == 0)
			//temp_ret = IfIndexExists(NodeID, MN, IndexID, ErrStr);
		//else
		//{
		//	printf("Invalid Node Type");
		//	return -4;
		//}
		if(IndexPos < 0)
		{
			printf("\n\n\nErr:%s!!\n\n\n", ErrStr);
			// Node Doesn't Exist
			return -1;
		}
		else
		{	
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
				return -3;
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
						return tmpSubIndexcount;
					}
					else if(tmpSubIndexcount == (objSubIndex->getNumberofSubIndexes() - 1))
					{
						printf("SubIndex Doesn't exist \n\n");
						// SubIndex Doesn't Exist
						return -3;
					}
				}
			}
		
		}
	return -3;
}
