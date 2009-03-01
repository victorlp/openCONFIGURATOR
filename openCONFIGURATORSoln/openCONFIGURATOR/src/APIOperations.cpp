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
#include "../Include/Exception.h"
#include "../Include/ObjectDictionary.h"
#include "../Include/ProcessImage.h"
#include "../Include/Declarations.h"
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <libxml/xmlreader.h>
#include <errno.h>
#include <string.h>
#include <libxml/xmlwriter.h>
#include <libxml/encoding.h>

#if defined(_WIN32) && defined(_MSC_VER)
	#include <direct.h>
#else
	#include <sys/stat.h>
#endif


#define MY_ENCODING "ISO-8859-1"

int lastVarIndex = -1;
static bool CDTCompleted = false;
static bool NotLoadedOBD = true;
//static stOffsets size8Offset;
//static stOffsets size16Offset;
//static stOffsets size32Offset;
//static stOffsets size64Offset;
static stOffsets size8INOffset;
static stOffsets size16INOffset;
static stOffsets size32INOffset;
static stOffsets size64INOffset;
static stOffsets size8OUTOffset;
static stOffsets size16OUTOffset;
static stOffsets size32OUTOffset;
static stOffsets size64OUTOffset;

int no8Offsets, no16Offsets, no32Offsets, no64Offsets = 0;
char C_DLL_ISOCHR_MAX_PAYL[5] = "1490";
char C_DLL_MIN_ASYNC_MTU[4] = "300";
int ConfigDate;
int ConfigTime;

/**************************************************************************************************
	* Function Name: AddIndexAttributes
    * Description: Adds the default attributes to the Index, when addded.
/****************************************************************************************************/
static void AddIndexAttributes(char* IndexID, CIndex* objIndex, CIndex* objDictIndex)
	{
		// Setting the Index Value
		//
		//if(strcmp((char*) objIndex->getIndexValue(), IndexID) != 0)
		//{
		//		exit;
		//}
		
		//cout << "INDEXID" << objIndex->getIndexValue() << endl;
		//cout << "RX INDEXID" << IndexID << endl;
		objIndex->setIndexValue(IndexID);
		
		//$S The Value for all the attributes has to come from ObjDict.xml
		if(objDictIndex->getName() != NULL)
			objIndex->setName((char*) objDictIndex->getName());
		else
			objIndex->setName("");

		if(objDictIndex->getObjectType() != NULL)		
			objIndex->setObjectType((char*) objDictIndex->getObjectType());
		else
			//Setting "0", so default case is hit, when setting
			objIndex->setObjectType((char*) "");
				
		if( objDictIndex->getLowLimit() != NULL)
			objIndex->setLowLimit((char*) objDictIndex->getLowLimit());
		else
			objIndex->setLowLimit("");
			
		if(objDictIndex->getHighLimit() != NULL)
			objIndex->setHighLimit((char*) objDictIndex->getHighLimit());
		else
			objIndex->setHighLimit("");
			
		if(objDictIndex->getAccessType() != NULL)
			objIndex->setAccessType((char*) objDictIndex->getAccessType());
		else
			objIndex->setAccessType("");
		
		////$S To be checked by $M
		if(objDictIndex->getPDOMapping() != NULL)
		{
			char *str = new char[50];
			strcpy(str, (char*) objDictIndex->getPDOMapping());
			objIndex->setPDOMapping(str);
			delete [] str;
		}
		else
		{
			char *str = new char[5];
			strcpy(str, (char*) "");
			objIndex->setPDOMapping(str);
			delete [] str;
			//objIndex->setPDOMapping("NO");
		}
		//$S To be checked by $M
		
		////if(objIndex->getPDOMapping() != NULL)
		////	objIndex->setPDOMapping((char*) objIndex->getPDOMapping());
		////else
		////	objIndex->setPDOMapping("");
		
		if(objDictIndex->getDefaultValue() != NULL)
			objIndex->setDefaultValue((char*) objDictIndex->getDefaultValue());
		else
			objIndex->setDefaultValue("");
			
		if(objDictIndex->getActualValue() != NULL)
			objIndex->setActualValue((char*) objDictIndex->getActualValue());
		else
			objIndex->setActualValue("");
		//$STODO:
		if(objDictIndex->getDataType().getName() != NULL)
			//objIndex->setDataType((char*) objIndex->getDataTypeValue(), objIndex->getNodeID());
			objIndex->setDataTypeST(objDictIndex->getDataType());
		else
			objIndex->setDataType("");

		return;
	}

/**************************************************************************************************
	* Function Name: AddSubIndexAttributes
    * Description: Adds the default attributes to the Index, when addded.
/****************************************************************************************************/
static void AddSubIndexAttributes(char* SubIndexID, CSubIndex* objSubIndex, CSubIndex* objDictSIndex)
	{
			
		// Setting the Index Value
		objSubIndex->setIndexValue(SubIndexID);
		//$S The Value for all the attributes has to come from ObjDict.xml
		if(objDictSIndex->getName() != NULL)
			objSubIndex->setName((char*) objDictSIndex->getName());
		else
			objSubIndex->setName("");
		if(objDictSIndex->getObjectType() != NULL)
			//objSubIndex->setObjectType(atoi(objSubIndex->getObjectType()));
			objSubIndex->setObjectType((char*) (objDictSIndex->getObjectType()));
		else
			//Setting "0", so default case is hit, when setting
			objSubIndex->setObjectType("");
				
		if( objDictSIndex->getLowLimit() != NULL)
			objSubIndex->setLowLimit((char*) objDictSIndex->getLowLimit());
		else
			objSubIndex->setLowLimit("");
			
		if(objDictSIndex->getHighLimit() != NULL)
			objSubIndex->setHighLimit((char*) objDictSIndex->getHighLimit());
		else
			objSubIndex->setHighLimit("");
			
		if(objDictSIndex->getAccessType() != NULL)
			objSubIndex->setAccessType((char*) objDictSIndex->getAccessType());
		else
			objSubIndex->setAccessType("");
				
		//if(objSubIndex->getPDOMapping() != NULL)
		//{
		//	char *str = new char[50];
		//	strcpy(str, (char*) objSubIndex->getPDOMapping());
		//	objSubIndex->setPDOMapping(str);
		//	delete [] str;
		//}
		//else
		//	objSubIndex->setPDOMapping("");
				
		if(objDictSIndex->getDefaultValue() != NULL)
			objSubIndex->setDefaultValue((char*) objDictSIndex->getDefaultValue());
		else
			objSubIndex->setDefaultValue("");
			
		if(objDictSIndex->getActualValue() != NULL)
			objSubIndex->setActualValue((char*) objDictSIndex->getActualValue());
		else
			objSubIndex->setActualValue("");
		
		if(objDictSIndex->getDataType().getName() != NULL)
			//objSubIndex->setDataType(objSubIndex->getDataTypeValue());
			objSubIndex->setDataTypeST(objDictSIndex->getDataType());
		else
			objSubIndex->setDataType("");
		return;
	}

 
/**************************************************************************************************
	* Function Name: CreateNode
    * Description:
/****************************************************************************************************/
ocfmRetCode CreateNode(int NodeID, ENodeType NodeType, char* NodeName)
{
	ocfmRetCode ErrStruct;
	CNode objNode;
	CNodeCollection *objNodeCollection;
	int NodePos;
	try
	{
		// TODO:If Check is made when Zero nodes present, Seg Fault is happening
		if(NodeType ==MN)
		{
			//cout << "loading od.xml"<< endl;
			LoadObjectDictionary("od.xml");
			//cout << "loaded xml" << endl;
		}
		if(NodeType == CN)
		{
			bool flag = false;
			ErrStruct = IfNodeExists(NodeID, NodeType, &NodePos, flag );
								
			if(ErrStruct.code == OCFM_ERR_SUCCESS && flag == true)								
			{
				//cout << "from CreateNode OCFM_ERR_NODE_ALREADY_EXISTS:" << OCFM_ERR_NODE_ALREADY_EXISTS << endl;
				ocfmException* objException = new ocfmException;
				objException->ocfm_Excpetion(OCFM_ERR_NODE_ALREADY_EXISTS);		
				throw objException;
				//ErrStruct.code = OCFM_ERR_NODE_ALREADY_EXISTS;
				//return ErrStruct;
			}
			else
			{
				//cout<< "OCFM_ERR_NODEID_NOT_FOUND" << OCFM_ERR_NODEID_NOT_FOUND << endl;;
				
				
			}
		}
		objNode.setNodeId(NodeID);
		objNode.setNodeType(NodeType);
		objNode.setNodeName(NodeName);
		
		objNode.CreateIndexCollection();
		objNode.CreateDataTypeCollection();
		objNode.CreateApplicationProcess();

		objNodeCollection = CNodeCollection::getNodeColObjectPointer();
		objNodeCollection->addNode(objNode);
		//cout<< "\n\nNode Created!!\n\n\n" << endl;
		ErrStruct.code = OCFM_ERR_SUCCESS;
		return ErrStruct;
	}
	catch(ocfmException* ex)
	{
		//cout<< "\n\nNode Not Created!!\n\n\n" << endl;
		return ex->_ocfmRetCode;
	}
}

/**************************************************************************************************
	* Function Name: DeleteNode
    * Description:
	* Return value: ocfmRetCode
/****************************************************************************************************/
ocfmRetCode DeleteNode(int NodeID, ENodeType NodeType)
	{
		int NodePos;
		ocfmRetCode ErrStruct;
		bool flag = false;
		ErrStruct = IfNodeExists(NodeID, NodeType, &NodePos, flag);
							
		if(flag!= true)
		{
			/* Function didnt throw any exception but Node doesnt exist */
			if(ErrStruct.code == OCFM_ERR_SUCCESS)								
			{
				ErrStruct.code = OCFM_ERR_NODEID_NOT_FOUND;
			}
			/* Function threw exception*/
			else
			{
				//cout << "\n\nErrStruct.code:" << ErrStruct.code << "\n\n!!!" << endl;
				// Node Doesn't Exist
				
			}
			return ErrStruct;
		}
		

		CNode objNode;		
		CNodeCollection *objNodeCollection;
		objNodeCollection= CNodeCollection::getNodeColObjectPointer();	
		//cout<< "Inside DeleteNode: \n" <<objNodeCollection->getNumberOfNodes()<<endl;
		objNodeCollection = CNodeCollection::getNodeColObjectPointer();
		objNodeCollection->deleteNode(NodePos);
		ErrStruct.code = OCFM_ERR_SUCCESS;
		return ErrStruct;		
	}
	
/**************************************************************************************************
	* Function Name: DeleteIndex
    * Description: Deletes the Index after performing check for Index Existanse
	* Return value: ocfmRetCode
/****************************************************************************************************/
ocfmRetCode DeleteIndex(int NodeID, ENodeType NodeType, char* IndexID)
	{
		
		int IndexPos;
		try
		{
			ocfmRetCode ErrStruct;
			ErrStruct = IfIndexExists(NodeID, NodeType, IndexID, &IndexPos);
			if(ErrStruct.code == OCFM_ERR_SUCCESS)
			{
				//IndexPos = ErrStruct.returnValue;
			}
			else
			{
				ocfmException objException;				
				objException.ocfm_Excpetion(OCFM_ERR_INDEXID_NOT_FOUND);
				throw objException;
			}
			CNode objNode;		
			CNodeCollection *objNodeCollection;
			CIndexCollection *objIndexCollection;
			CIndex objIndex;

			objIndex.setNodeID(objNode.getNodeId());
			objNodeCollection= CNodeCollection::getNodeColObjectPointer();
			objNode = objNodeCollection->getNode(NodeType, NodeID);

			objIndexCollection = objNode.getIndexCollection();
			//cout<< "Inside DeleteIndex: \n" << atoi(IndexID) <<endl;
			//cout<< "Inside DeleteIndex: \n" << IndexPos <<endl;
			objIndexCollection->deleteIndex(IndexPos);
			ErrStruct.code = OCFM_ERR_SUCCESS;
			return ErrStruct;
		}	
		catch(ocfmException& ex)
		{
			return ex._ocfmRetCode;
		}
	}
	
/**************************************************************************************************
	* Function Name: DeleteSubIndex
    * Description: Deletes the Index after performing check for Index Existanse
	* Return value: ocfmRetCode
/****************************************************************************************************/
ocfmRetCode DeleteSubIndex(int NodeID, ENodeType NodeType, char* IndexID, char* SubIndexID)
	{
		CNode objNode;		
		CNodeCollection *objNodeCollection;
		CIndexCollection *objIndexCollection;
		CIndex objIndex;
		CIndex* objSubIndex;
		int SubIndexPos;
		int IndexPos;
		ocfmRetCode ErrStruct;
		try
		{
			ErrStruct = IfSubIndexExists(NodeID, NodeType, IndexID, SubIndexID, &SubIndexPos, &IndexPos);
			if(ErrStruct.code == OCFM_ERR_SUCCESS)
			{
				//SubIndexPos = Tmp_ErrStruct.returnValue;
			}
			else
			{
					ocfmException objException;				
					objException.ocfm_Excpetion(OCFM_ERR_SUBINDEXID_NOT_FOUND);
					throw objException;
			}

			objIndex.setNodeID(objNode.getNodeId());
			objNodeCollection= CNodeCollection::getNodeColObjectPointer();
			objNode = objNodeCollection->getNode(NodeType, NodeID);


			objIndexCollection = objNode.getIndexCollection();
			objSubIndex =objIndexCollection->getIndex(IndexPos);
			
			//cout << "NumberofSubIndexes:" << objIndex.getNumberofSubIndexes()<< endl;
			objSubIndex->deleteSubIndex(SubIndexPos);
			ErrStruct.code = OCFM_ERR_SUCCESS;
			return ErrStruct;
		}
		catch(ocfmException& ex)
		{
			return ex._ocfmRetCode;
		}		
	}
/**************************************************************************************************
	* Function Name: AddSubIndex
    * Description:
	* Return value: ocfmRetCode
/****************************************************************************************************/
ocfmRetCode AddSubIndex(int NodeID, ENodeType NodeType, char* IndexID, char* SubIndexID)
	{
	//cout << "\nInside AddSubIndex\n" << endl;
		CNode objNode;		
		CNodeCollection *objNodeCollection;
		CIndexCollection *objIndexCollection;
		CIndex objIndex;
		int SubIndexPos;
		int IndexPos;
		ocfmRetCode ErrStruct;
		
		try
		{
			/*if(NotLoadedOBD)
			{
			cout << "loading od.xml..." << endl;
				LoadObjectDictionary("od.xml");
				cout << "loaded od.xml..." << endl;
				NotLoadedOBD = false;
			}*/
			ErrStruct = IfSubIndexExists(NodeID, NodeType, IndexID, SubIndexID, &SubIndexPos, &IndexPos);
		//	cout << "1\n" << endl;
			if( ErrStruct.code == OCFM_ERR_SUCCESS )
			{
				//ErrStruct.code = OCFM_ERR_SUBINDEX_ALREADY_EXISTS;
				//return ErrStruct;
				ocfmException objException;				
				objException.ocfm_Excpetion(OCFM_ERR_SUBINDEX_ALREADY_EXISTS);
		//		cout << "2\n" << endl;
				throw objException;
			}
			
			if( (ErrStruct.code == OCFM_ERR_NO_SUBINDEXS_FOUND) || (ErrStruct.code == OCFM_ERR_SUBINDEXID_NOT_FOUND))
			{
			//cout << "3\n" << endl;
				
				objNodeCollection= CNodeCollection::getNodeColObjectPointer();
				objNode = objNodeCollection->getNode(NodeType, NodeID);
				objIndexCollection = objNode.getIndexCollection();
				CIndex* objIndexPtr;
				objIndexPtr =objIndexCollection->getIndex(IndexPos);
				CSubIndex* objSubIndex;
				CSubIndex* objDictSIdx;
				
				objSubIndex = new CSubIndex;
				//Set the NodeID
				//objSubIndex.setNodeID(objNode.getNodeId());
				
				/* Get the SubIndex from ObjectDictionary*/
				CObjectDictionary* objOBD;
				objOBD = CObjectDictionary::getObjDictPtr();
				
				//cout << "\n\nobjOBD->ifObjectDictSubIndexExists(IndexID, SubIndexID):" << objOBD->ifObjectDictSubIndexExists(IndexID, SubIndexID) << endl;
				//if(objOBD->ifObjectDictSubIndexExists(IndexID, SubIndexID) == 1)
				//{
				//	//cout<< "\n\n\n$OCFM_ERR_INVALID_SUBINDEXID \n"<<endl;
				//	//ErrStruct.code = OCFM_ERR_INVALID_SUBINDEXID;
				//	//return ErrStruct;
				//	ocfmException objException;				
				//	objException.ocfm_Excpetion(OCFM_ERR_INVALID_SUBINDEXID);
				//	throw objException;
				//}	
				
				objDictSIdx = objOBD->getObjectDictSubIndex(IndexID, SubIndexID);
				
				if(objSubIndex != NULL)
				{
					objSubIndex->setNodeID(NodeID);
				//	cout << "4\n" << endl;
					AddSubIndexAttributes(SubIndexID, objSubIndex, objDictSIdx);
					objIndexPtr->addSubIndex(*objSubIndex);
				}
				else
				{
			//	cout << "5\n" << endl;
						ocfmException objException;				
						objException.ocfm_Excpetion(OCFM_ERR_INVALID_SUBINDEXID);
						throw objException;
				}
				
				//AddSubIndexAttributes(SubIndexID, &objSubIndex);									
				
				//printf("Added SubIndex \n\n");
				ErrStruct.code = OCFM_ERR_SUCCESS;
				return ErrStruct;						
			}
			return ErrStruct;
		}
		catch(ocfmException& ex)
		{
			return ex._ocfmRetCode;
		}
	}	

/**************************************************************************************************
	* Function Name: AddIndex
    * Description:
	* Return value: ocfmRetCode
/****************************************************************************************************/
ocfmRetCode AddIndex(int NodeID, ENodeType NodeType, char* IndexID)
	{
		int IndexPos = 0;
		CNode objNode;		
		CNodeCollection *objNodeCollection;
		CIndexCollection *objIndexCollection;
		CIndex* objIndex;
		CIndex* objDictIndex;
		ocfmRetCode ErrStruct;
		ErrStruct.code = OCFM_ERR_UNKNOWN;
		ErrStruct.errorString = NULL;
		
		try 
		{
			/*if(NotLoadedOBD)
			{
				LoadObjectDictionary("od.xml");
				NotLoadedOBD = false;
			}*/
			
			//cout<< "Inside AddIndex \n"<<endl;
			ErrStruct = IfIndexExists(NodeID, NodeType, IndexID, &IndexPos);
			
			
			//cout << "\n\nErrStruct.code from IfIndexExists:" << ErrStruct.code << endl;
			
			if( ErrStruct.code == OCFM_ERR_SUCCESS )
			{
				//ErrStruct.code = OCFM_ERR_INDEX_ALREADY_EXISTS;
				//return ErrStruct;
				ocfmException objException;				
				objException.ocfm_Excpetion(OCFM_ERR_INDEX_ALREADY_EXISTS);
				throw objException;
			}
			
			if( (ErrStruct.code == OCFM_ERR_NO_INDEX_FOUND) || (ErrStruct.code == OCFM_ERR_INDEXID_NOT_FOUND) )
			{	
				objIndex = new CIndex();
				
				objNodeCollection= CNodeCollection::getNodeColObjectPointer();
				objNode = objNodeCollection->getNode(NodeType, NodeID);
				objIndexCollection = objNode.getIndexCollection();
				/* Get the Index from ObjectDictionary*/
				CObjectDictionary* objOBD;
				objOBD = CObjectDictionary::getObjDictPtr();
				//if(objOBD->ifObjectDictIndexExists(IndexID) == 1)
				//{
				//	//cout<< "\n\n\n$OCFM_ERR_INVALID_INDEXID \n"<<endl;
				//	//ErrStruct.code = OCFM_ERR_INVALID_INDEXID;
				//	//return ErrStruct;
				//	ocfmException objException;				
				//	objException.ocfm_Excpetion(OCFM_ERR_INVALID_INDEXID);
				//	throw objException;
				//}
				objDictIndex = objOBD->getObjectDictIndex(IndexID);				
				if(objIndex!=NULL)
				{
					objIndex->setNodeID(NodeID);
					AddIndexAttributes(IndexID, objIndex, objDictIndex);
					for(int i= 0; i<objDictIndex->getNumberofSubIndexes(); i++)
					{
						CSubIndex* objSIdx;
						objSIdx = objDictIndex->getSubIndex(i);
						objIndex->addSubIndex(*objSIdx);
					}
					objIndexCollection->addIndex(*objIndex);
				}
				else
				{
					//ErrStruct.code = OCFM_ERR_INVALID_INDEXID;
					//printf("\nAdded NOT Index \n\n");
					//return ErrStruct;
					ocfmException objException;				
					objException.ocfm_Excpetion(OCFM_ERR_INVALID_INDEXID);
					throw objException;
				}
				//printf("Added Index \n\n");
				//return OCFM_ERR_SUCCESS;
				ErrStruct.code = OCFM_ERR_SUCCESS;
				return ErrStruct;
			}			
			return ErrStruct;
		}
		catch(ocfmException& ex)
		{
			return ex._ocfmRetCode;
		}
	}	
/**************************************************************************************************
	* Function Name: EditIndex
    * Description:
	* Return value: ocfmRetCode
/****************************************************************************************************/
ocfmRetCode SetIndexAttributes(int NodeID, ENodeType NodeType, char* IndexID, char* IndexValue, char* IndexName)
{
	int IndexPos;
	ocfmRetCode ErrStruct;
	try 
	{
		ErrStruct = IfIndexExists(NodeID, NodeType, IndexID, &IndexPos);
		if(ErrStruct.code == OCFM_ERR_SUCCESS)
		{
			//IndexPos = ErrStruct.returnValue;
		}
		else
		{
			//OCFM_ERR_INDEXID_NOT_FOUND
			//return ErrStruct;
			ocfmException objException;				
			objException.ocfm_Excpetion(OCFM_ERR_INDEXID_NOT_FOUND);
			throw objException;
		}
		CIndex* objIndexPtr;

		CNode objNode;		
		CNodeCollection *objNodeCollection;
		CIndexCollection *objIndexCollection;
		CIndex objIndex;
		
		objIndex.setNodeID(objNode.getNodeId());
		objNodeCollection= CNodeCollection::getNodeColObjectPointer();
		objNode = objNodeCollection->getNode(NodeType, NodeID);
		objIndexCollection = objNode.getIndexCollection();
		objIndexPtr =objIndexCollection->getIndex(IndexPos);			
		//cout << "EditIndexValue:Index Actual Value:" << objIndexPtr->getActualValue() << IndexValue << endl;
		/* Check if the value is valid*/
		objIndexPtr->setName(IndexName);
		if(objIndexPtr->IsIndexVaueValid(IndexValue))
		{
			//printf("\nIndex value%s",IndexValue);
			objIndexPtr->setActualValue(IndexValue);
			
		//printf("EditIndexValue:Index Actual Value:%s-%s\n", objIndexPtr->getActualValue(), IndexValue);
			ErrStruct.code = OCFM_ERR_SUCCESS;
		}
		else
		{
				ocfmException objException;				
				objException.ocfm_Excpetion(OCFM_ERR_VALUE_NOT_WITHIN_RANGE);
				throw objException;
				//ErrStruct.code = OCFM_ERR_VALUE_NOT_WITHIN_RANGE;
		}
		return ErrStruct;
	}
	catch(ocfmException& ex)
	{
		return ex._ocfmRetCode;
	}
}

/**************************************************************************************************
	* Function Name: EditSubIndex
    * Description:
	* Return value: ocfmRetCode
/****************************************************************************************************/
ocfmRetCode SetSubIndexAttributes(int NodeID, ENodeType NodeType, char* IndexID, char* SubIndexID, char* IndexValue, char* IndexName)
{
		CNode objNode;		
		CNodeCollection *objNodeCollection;
		CIndexCollection *objIndexCollection;
		CIndex objIndex;
		CIndex* objSubIndex;
		int IndexPos;
		int SubIndexPos;
		ocfmRetCode ErrStruct;
		//CSubIndex* objSubIndex;
		
		try
		{
			ErrStruct = IfSubIndexExists(NodeID, NodeType, IndexID, SubIndexID, &SubIndexPos, &IndexPos);
			if(ErrStruct.code == OCFM_ERR_SUCCESS)
			{
				//SubIndexPos = Tmp_ErrStruct.returnValue;
			}
			else
			{
				//OCFM_ERR_INVALID_SUBINDEXID
				ocfmException objException;				
				objException.ocfm_Excpetion(OCFM_ERR_INVALID_SUBINDEXID);
				throw objException;
//				return ErrStruct;
			}
			
			CSubIndex* objSubIndexPtr;
				
			objIndex.setNodeID(objNode.getNodeId());
			objNodeCollection= CNodeCollection::getNodeColObjectPointer();
			objNode = objNodeCollection->getNode(NodeType, NodeID);

			objIndexCollection = objNode.getIndexCollection();
			objSubIndex =objIndexCollection->getIndex(IndexPos);
			
			objSubIndexPtr = objSubIndex->getSubIndex(SubIndexPos);						
			//printf("SubIndexValue:%s-%s\n", objSubIndexPtr->getName(), SubIndexID);
			objSubIndexPtr->setName(IndexName);
			if(objSubIndexPtr->IsIndexVaueValid(IndexValue))		
			{
				printf("\nIndex value%s",IndexValue);
				objSubIndexPtr->setActualValue(IndexValue);
				ErrStruct.code = OCFM_ERR_SUCCESS;
			}
			else
			{
				ocfmException objException;				
				objException.ocfm_Excpetion(OCFM_ERR_VALUE_NOT_WITHIN_RANGE);
				throw objException;
				//ErrStruct.code =  OCFM_ERR_VALUE_NOT_WITHIN_RANGE;
			}
			objSubIndexPtr->setName(IndexName);
			//printf("SubIndexValue:%s-%s\n", objSubIndexPtr->getName(), SubIndexID);
			/*ErrStruct.code = OCFM_ERR_SUCCESS;*/
			return ErrStruct;
		}
		catch(ocfmException& ex)
		{
			return ex._ocfmRetCode;
		}
}

/**************************************************************************************************
	* Function Name: DisplayNodeTree
    * Description:
/****************************************************************************************************/
void DisplayNodeTree()
	{
		CNode objNode;		
		CNodeCollection *objNodeCollection;
		CIndexCollection *objIndexCollection;
		
		objNodeCollection= CNodeCollection::getNodeColObjectPointer();
		objIndexCollection = objNode.getIndexCollection();
		
		if( objNodeCollection->getNumberOfNodes() > 0)
		{
			for(int count = 0; count < objNodeCollection->getNumberOfNodes(); count++)
			{
					objNode = objNodeCollection->getNodebyCollectionIndex(count);
					if (objNode.getNodeType() == CN)
					{						
						//int tmp_NodeID = objNode.getNodeId();
						//objNode =objNodeCollection->getNode(CN,tmp_NodeID);
						//printf("NodePos:%d, NodeID:%d\n", count, objNode.getNodeId());
					}						
			}
		}
		else
		{
				//printf("No Nodes found!\n");
				return;
		}
			
	}	
	
/**************************************************************************************************
	* Function Name: GetIndexData
   * Description: 
/****************************************************************************************************/
void GetIndexData(CIndex* objIndex, char* Buffer)
	{
			int len;
			
			//Get the Index Value		
			
			/*if(objIndex->getNumberofSubIndexes()==0 &&(objIndex->getDefaultValue()!= NULL ||
																																													 objIndex->getActualValue()!=NULL))*/
			strcpy(Buffer,"");
			if(objIndex->getNumberofSubIndexes()==0 && objIndex->getActualValue()!=NULL)
			
			{
					//Buffer = (char*)malloc(sizeof(objIndex->getIndexValue()+1));
					strcpy(Buffer,objIndex->getIndexValue());

					//Place a tab			
					len = strlen(Buffer);
					//Buffer =(char*)realloc(Buffer,2);
					strcat(Buffer,"\t");
					//printf("\n%s",Buffer);

					//If subindexes are none, add "00"
					//	Buffer =(char*)realloc(Buffer,4);
						strcat(Buffer,"00");
						strcat(Buffer,"\t");

							//Add datatype
							DataType dt;		
							//Buffer =(char*)realloc(Buffer,6);	
							dt = objIndex->getDataType();
							int padLength=0;
							if(dt.Name != NULL)
							{
								strcat(Buffer ,dt.DataSize);				
								padLength = hex2int(dt.DataSize)*2;
							}
							else strcat(Buffer,"0000");
							
							strcat(Buffer,"\t");
						
							if (objIndex->getActualValue()!=NULL)
							{	
										char actvalue[20];
										actvalue[0]  = '\0';
										//actvalue = new char[50];							
										//actvalue = strchr((char*)objSubIndex->getActualValue(),'x');
										/*strcpy(actvalue, strchr((char*)objSubIndex->getActualValue(),'x'));*/
										if(CheckIfHex((char*)objIndex->getActualValue()))
										//if(actvalue!=NULL)
										{
											int len = strlen((char*)objIndex->getActualValue());
											//char substr[18];
											//substr =  (char*)objSubIndex->getActualValue(),2,len -2);
											strncpy(actvalue,(objIndex->getActualValue()+ 2),len-2 );
											actvalue[len -2] ='\0';
											//delete[] substr;
											printf("\n ACT Value%s",actvalue);
										
											//strcpy(actvalue, subString(actvalue,1,strlen(actvalue)-1));
											strcat(Buffer,padLeft(actvalue,'0',padLength));
											//delete[] actvalue;
											//actvalue = '\0';
											//int l = strlen(actvalue);
										}
												
								/*char* actvalue = new char[30];												
								if(strchr((char*)objIndex->getActualValue(),'x')!=NULL )
								{
									strcpy(actvalue,strchr((char*)objIndex->getActualValue(),'x'));
									strcpy(actvalue, subString(actvalue,1,strlen(actvalue)-1));
									strcat(Buffer,padLeft(actvalue,'0',padLength));
								}*/
								else
								{
								 
									//actvalue =  itoa(atoi(objIndex->getActualValue()),actvalue,16);
									///*printf("\n actval%s",actvalue);*/
									////printf("Index:% s actvalue: %s padlength:%d",objIndex->getIndexValue(),objIndex->getActualValue(),padLength);
									////strcat(Buffer,padLeft((char*)objIndex->getActualValue(),'0',padLength));								
									//strcat(Buffer,padLeft(actvalue, '0', padLength));
										strcpy(actvalue, itoa(atoi(objIndex->getActualValue()),actvalue,16));
										printf("\n ACT Value%s",actvalue);
									
												//strcpy(actvalue, itoa(atoi(objIndex->getActualValue()),actvalue,16));
										strcat(Buffer,padLeft(actvalue, '0', padLength));
									
								}
							}
					
									strcat(Buffer,"\n");
				}	
			
			else
			{
				int noOfSubIndexes = objIndex->getNumberofSubIndexes();
				bool Indexadded  = false;				
				bool resetValueAdded = false;
				bool flag_No_of_enteriesAdded = false;
				int i;
				CSubIndex* objSubIndex;
				for(i=0; i<noOfSubIndexes ; i++)
				{
					objSubIndex = objIndex->getSubIndex(i);
				
					/*if(strcmp(objSubIndex->getIndexValue(),"00")!=0 && objSubIndex->getActualValue() != NULL)*/
					if(objSubIndex->getActualValue() != NULL)
			
					{
						if (Indexadded)
						strcat(Buffer,objIndex->getIndexValue());
						else
						{
							strcpy(Buffer,objIndex->getIndexValue());
							Indexadded = true;
						}
											
						//Place a tab			
						len = strlen(Buffer);
						strcat(Buffer,"\t");
						//printf("%s",Buffer);

							/*if(objSubIndex->getActualValue() != NULL || objSubIndex->getDefaultValue()!=NULL)*/
								strcat(Buffer, objSubIndex->getIndexValue());
								strcat(Buffer,"\t");
									//Add datatype
								DataType dt;			
								dt = objSubIndex->getDataType();
								int padLength=0;
								if(dt.Name != NULL)
								{
									strcat(Buffer ,dt.DataSize);				
									padLength = hex2int(dt.DataSize)*2;
								}
								else strcat(Buffer,"0000");
								
								strcat(Buffer,"\t");
							
								if (objSubIndex->getActualValue()!=NULL)
								{

									// Add the reset value for that Index,SubIndex
									if(strcmp(objSubIndex->getIndexValue(),"00")==0 && CheckIfMappingPDO((char*)objIndex->getIndexValue()) &&
												resetValueAdded==false )
									{
										char actvalue[20];
										strcpy(actvalue,"0");
										strcat(Buffer,padLeft(actvalue,'0',padLength));
										resetValueAdded = true;
										//i--;
									}
									else
									{
										
										char actvalue[20];
										actvalue[0]  = '\0';
										if(CheckIfHex((char*)objSubIndex->getActualValue()))
										//if(actvalue!=NULL)
										{
											int len = strlen((char*)objSubIndex->getActualValue());
											strncpy(actvalue,(objSubIndex->getActualValue()+ 2),len-2 );
											actvalue[len -2] ='\0';
										
											printf("\n ACT Value%s",actvalue);
											strcat(Buffer,padLeft(actvalue,'0',padLength));
										
										}
											else
											{				
												//actvalue = new char[50];									
												strcpy(actvalue, itoa(atoi(objSubIndex->getActualValue()),actvalue,16));
												printf("\n ACT Value%s",actvalue);
									
												strcat(Buffer,padLeft(actvalue, '0', padLength));
											}
										
										}
										
								}
									/*else strcat(Buffer,objSubIndex->getDefaultValue());*/
								strcat(Buffer,"\n");
								
								if(i == 0 && (CheckIfMappingPDO((char*)objIndex->getIndexValue()) &&(flag_No_of_enteriesAdded==true)))
								{
									i = noOfSubIndexes-1;								
								}
								if(i == noOfSubIndexes-1 && (CheckIfMappingPDO((char*)objIndex->getIndexValue()) && (flag_No_of_enteriesAdded==false)))
								{
									i = -1;	
									flag_No_of_enteriesAdded = true;							
								}
							}
						}					
						
				}
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
	* Function Name: WriteCNsData
 * Description: Writes all CNS data to CDC file
/****************************************************************************************************/

	void WriteCNsData(char* fileName)
{
		char* Buffer2;
		char* Buffer3;
		int len;
		CIndexCollection* objIndexCollection;
	
			//printf("\n\n\n CN-S Data Started**********************");
		//Buffer2 = NULL;
//		strcpy(Buffer3,"");
		char* c = (char*)malloc(4);	
		FILE* fileptr = new FILE();
		CNode objNode;	
		CNodeCollection* objNodeCollection;
		objNodeCollection = CNodeCollection::getNodeColObjectPointer();		
		
		int CNCount=0;
		//if (( fileptr = fopen(fileName,"a+")) == NULL)
		//{
		//	//cout << "Problem" <<endl;
		//}
		for(int count=0; count<objNodeCollection->getNumberOfNodes(); count++)
			{
				objNode = objNodeCollection->getNodebyCollectionIndex(count);
				if (objNode.getNodeType() == CN)
					{
						if (( fileptr = fopen(fileName,"a+")) == NULL)
						{
							//cout << "Problem" <<endl;
						}
						
							AddOtherRequiredCNIndexes(objNode.getNodeId());
							objIndexCollection = objNode.getIndexCollection();
							char* comment= (char*)malloc(30);
							itoa(CNCount+1,c,10);
							CNCount++;
							
							strcpy(comment,"////Configuration Data for CN-");
							comment = strcat(comment,c);
							comment = strcat(comment,"\n");
							len = strlen(comment);
							if((fwrite(comment, sizeof(char),len,fileptr))!=NULL)
							{
								printf("\nComments for CN-%d",count);
								fclose(fileptr);
							}
							//comment = strcat(comment,(char*)count);
							/*strcpy(Buffer2, comment);*/
							int NumberOfIndexes;
							bool firstBuffer = true;																								
							NumberOfIndexes = objIndexCollection->getNumberofIndexes();
							
							CIndex* objIndex;
							char* Buffer4;
									
						/*************WRITE MN'S 1006,1020 Indexes Values *******************************/			
							Buffer4 = (char*)malloc(2000);	
							objIndex = getMNIndexValues("1006");
							Buffer2 = (char*)malloc(2000);		
							strcpy(Buffer2, "");
							strcpy(Buffer4, "");
									
							if(objIndex!=NULL)
							{
								GetIndexData(objIndex,Buffer4);
								strcpy(Buffer2, Buffer4);
							}
							
							
							objIndex = getMNIndexValues("1");
							if(objIndex!=NULL)
							{
								GetIndexData(objIndex,Buffer4);
								strcat(Buffer2, Buffer4);
							}
							
							/*************WRITE Other Required CN Indexes in CDC *******************************/
								for(int i=0; i<NumberOfIndexes; i++)
								{
		
									
									objIndex = objIndexCollection->getIndex(i);
									const char* IndexValue = objIndex->getIndexValue();
									
									/*if((CheckIfNotPDO((char*)IndexValue)==false)  || CheckIfManufactureSpecificObject((char*)IndexValue))
								*/
								if(CheckAllowedCNIndexes((char*)IndexValue))
								{
																								
											GetIndexData(objIndex, Buffer4);
											strcat(Buffer2, Buffer4);
										
									}									
								}	
								delete[] Buffer4;
										
							
							//Convert CN NodeID to Hex
							itoa(objNode.getNodeId(),c,16);	
							char CNDataLength[18];
							strcpy(CNDataLength, "1F22\t");
							c = padLeft(c, '0', 2);
					
							///write CN-n NodeID  in the next to 1F22
							strcat(CNDataLength, c);
							strcat(CNDataLength, "\t");
							
							//write the size of CN-n Buffer
							//int len = lenOfCNBuffer(Buffer2);
							int len = getCNDataLen(Buffer2);

							//Convert length to Hex
							itoa(len,c,16);
							//printf("c%s",c);
							
							c = padLeft(c, '0', 4);
							strcat(CNDataLength, c);				
							
							// First write the IF22 data in a Buffer and then CN-ns Object Dictionary
							Buffer3 = (char*)malloc(strlen(Buffer2)+50);
							strcpy(Buffer3, CNDataLength);
							strcat(Buffer3, "\n");
							strcat(Buffer3, Buffer2);
															
						
							//strcpy(Buffer2,GenerateCNOBD(objNodeCollection));
						
							//write all CNs data in the file
							len = strlen(Buffer3);
							if (( fileptr = fopen(fileName,"a+")) == NULL)
							{
								//cout << "Problem" <<endl;
							}
							if((fwrite(Buffer3, sizeof(char),len,fileptr))!=NULL)
								{
									//printf("success");				
									fclose(fileptr);	
									/*delete[] Buffer2;
									delete[] Buffer3;		*/
															
								}
					delete[] Buffer2;
					delete[] Buffer3;
					}
			
			}
		
	}
/**************************************************************************************************
	* Function Name: GenerateCDC
    * Description: Generates the CDC file
/****************************************************************************************************/
ocfmRetCode GenerateCDC(char* fileName)
	{
		CNode objNode;	
		CIndexCollection* objIndexCollection;
		//ofstream file;
		char *Buffer1;
		int len;
		ocfmRetCode retCode;
		const char tempFileName[9] = "temp.txt";

		//printf("Inside GenerateCDC");
		
		try
		{
				
			Buffer1 = NULL;		

			//get the MN Node object from the NodeCollection
			CNodeCollection* objNodeCollection;
			objNodeCollection = CNodeCollection::getNodeColObjectPointer();		
			
		/******************************* Write MN's Indexes ******************************************************************/
		
			
			objNode = objNodeCollection->getMNNode();
			if(objNodeCollection->getNumberOfNodes()==0)
			{
				ocfmException ex;
				ex.ocfm_Excpetion(OCFM_ERR_NO_NODES_FOUND);
				throw ex;
			}
			if(objNode.isNull())
			{			
				ocfmException ex;
				ex.ocfm_Excpetion(OCFM_ERR_MN_NODE_DOESNT_EXIST);
				throw ex;			
			}
			if(objNodeCollection->getCNNodesCount()==0)
			{
				ocfmException ex;
				ex.ocfm_Excpetion(OCFM_ERR_NO_CN_NODES_FOUND);
				throw ex;
			}
			//printf("Node id %d",objNode.getNodeId());

			/* Generate MN'S Object Dictionary*/
			GenerateMNOBD();
			
			
			FILE* fileptr = new FILE();
			if (( fileptr = fopen(tempFileName,"w+")) == NULL)
				{
					ocfmException ex;
					ex.ocfm_Excpetion(OCFM_ERR_CANNOT_OPEN_FILE);
					throw ex;						
				}
				for(int i=0;i < objNodeCollection->getNumberOfNodes();i++)
				{
						CNode objNode;
						objNode = objNodeCollection->getNodebyCollectionIndex(i);
						if(objNode.getNodeType() ==CN)
						{
								if (( fileptr = fopen(tempFileName,"a+")) == NULL)
								{
									ocfmException ex;
									ex.ocfm_Excpetion(OCFM_ERR_CANNOT_OPEN_FILE);
									throw ex;						
								}
								//printf("\nInside 1F81");
								Buffer1 = (char*)malloc(500);
								len = strlen(Buffer1);
								strcpy(Buffer1, "//// NodeId Assignment\n");
								strcat(Buffer1, "1F81");
								strcat(Buffer1, "\t");
								int NodeID = objNode.getNodeId();		
								char* hex = (char*)malloc(3);					
								hex = itoa(NodeID,hex,16);
								hex = padLeft(hex,'0',2);
								strcat(Buffer1, hex);								
								strcat(Buffer1, "\t0004\t00000007\n");						
								strcat(Buffer1, "\n");	
								len = strlen(Buffer1);
								if((fwrite(Buffer1, sizeof(char),len,fileptr))!=NULL)
								{
									fclose(fileptr);
									//printf("Buffer1 written");
								
								}
							
								delete[] Buffer1;
						}
					}
					/*	}*/
			if (( fileptr = fopen(tempFileName,"a+")) == NULL)
				{
					printf ( "Cannot open file you have named...!\n" );
					ocfmException ex;
					ex.ocfm_Excpetion(OCFM_ERR_CANNOT_OPEN_FILE);
					throw ex;							
				}
				objIndexCollection = objNode.getIndexCollection();
			

			//Get all the MN's Default Data in Buffer1
			for(int i=0;i < objIndexCollection->getNumberofIndexes ();i++)
				{
					CIndex* objIndex;
					objIndex = objIndexCollection->getIndex(i);
				/*	if(CheckIfNotPDO((char*)objIndex->getIndexValue()))
						{	*/					
							//printf("Index%s",objIndex->getIndexValue());						
							Buffer1 = (char*)malloc(5000);
							len = strlen(Buffer1);
							
							GetIndexData(objIndex,Buffer1);
							len = strlen(Buffer1);
							if((fwrite(Buffer1, sizeof(char),len,fileptr))!=NULL)
							{
								//printf("Buffer1 written");
							
							}
							/*else
							{
								ocfmException ex;
								ex.ocfm_Excpetion(OCFM_ERR_CANNOT_OPEN_FILE);
								throw ex;		
							}*/
							
							delete[] Buffer1;
					/*	}*/
							
				}
				fclose(fileptr);
				
			/*************************Write CN's Data in Buffer2***************************************************/
			WriteCNsData((char*)tempFileName);
			
			//Get all the IF81 ENTRY in Buffer1
			if(objNodeCollection->getNumberOfNodes()!=0)
			{
				if (( fileptr = fopen(tempFileName,"a+")) == NULL)
				{
					ocfmException ex;
					ex.ocfm_Excpetion(OCFM_ERR_CANNOT_OPEN_FILE);
					throw ex;						
				}
			
				for(int i=0;i < objNodeCollection->getNumberOfNodes();i++)
				{
						CNode objNode;
						objNode = objNodeCollection->getNodebyCollectionIndex(i);
						if(objNode.getNodeType() ==CN)
						{
								if (( fileptr = fopen(tempFileName,"a+")) == NULL)
								{
									ocfmException ex;
									ex.ocfm_Excpetion(OCFM_ERR_CANNOT_OPEN_FILE);
									throw ex;						
								}
								//printf("\nInside 1F81");
								Buffer1 = (char*)malloc(500);
								len = strlen(Buffer1);
								strcpy(Buffer1, "//// NodeId Reassignment\n");
								strcat(Buffer1, "1F81");
								strcat(Buffer1, "\t");
								int NodeID = objNode.getNodeId();		
								char* hex = (char*)malloc(3);					
								hex = itoa(NodeID,hex,16);
								hex = padLeft(hex,'0',2);
								strcat(Buffer1, hex);								
								strcat(Buffer1, "\t0004\t80000007\n");																						
								len = strlen(Buffer1);
								if((fwrite(Buffer1, sizeof(char),len,fileptr))!=NULL)
								{
									fclose(fileptr);
									//printf("Buffer1 written");
								
								}
								/*else
								{
									ocfmException ex;
									ex.ocfm_Excpetion(OCFM_ERR_CANNOT_OPEN_FILE);
									throw ex;		
								}*/
								delete[] Buffer1;
						}
					}
					/*	}*/
							
				}
			
			//printf("\nText cdc generated");
			// Convert CDC txt file to Binary
			ConvertCdcToBinary(fileName,(char*) tempFileName);
			retCode.code =  OCFM_ERR_SUCCESS ;
			return retCode;
		}
		catch(ocfmException & ex)
		{
			return ex._ocfmRetCode;
		}
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

void  ProcessCDT(CComplexDataType* objCDT,CApplicationProcess* objAppProc,
																 CNode* objNode, Parameter* para, EPDOType pdoType,
																 int startingOffset, char* ModuleName)
{

	int StartByteOffset = startingOffset ;
	ocfmException objex;
	int StartBitOffset =  -1;
	
		
	{
		if(objCDT ==NULL)
		{
			objex.ocfm_Excpetion(OCFM_ERR_STRUCT_DATATYPE_NOT_FOUND);
			throw objex;		
		}

		for(int i= 0 ; i<objCDT->varCollection.Count(); i++)
		{
	
			varDeclaration vd;
			vd.Initialize();
			vd = objCDT->varCollection[i];			
			if(vd.nam_id_dt_attr->getDtUniqueRefId() != NULL)
			{
				objCDT = objAppProc->getCDTbyUniqueID(vd.nam_id_dt_attr->getDtUniqueRefId());	
				objAppProc->updatePreviousCDT_UId(vd.StructUniqueId, objCDT->Index);		
				/*objCDT->previousCDT_UId = (char*)malloc(strlen(vd.StructUniqueId)+1);
				strcpy(objCDT->previousCDT_UId, vd.StructUniqueId);*/
				lastVarIndex = i;
			/*	printf("\n previousCDT_UId : %s",objCDT->previousCDT_UId);
				printf("\n DataRefID : %s",vd.nam_id_dt_attr->getDtUniqueRefId());
		*/		ProcessCDT(objCDT, objAppProc, objNode, para, pdoType,StartByteOffset, ModuleName );
			}
		if(!CDTCompleted)
		{	
			// add rest of the contents
			ProcessImage pi;
			pi.BitOffset = -1;
			pi.ByteOffset = NULL;
			if(para->access !=NULL)
			strcpy(pi.Direction,getParameterAccess(para->access));
				
			if(pdoType == PDO_TPDO)
			pi.DirectionType = INPUT ;
			else if(pdoType == PDO_RPDO)
 			pi.DirectionType = OUTPUT;
			
				if(vd.size != NULL)			
				{
				/*	pi.DataSize = (char*)malloc(5);*/
					//strcpy(pi.DataInfo.DataSize, vd.size);
					pi.DataInfo.DataSize = atoi(vd.size);
				}
			else
			{
			/*		pi.DataSize = (char*)malloc(5);*/
					//strcpy(pi.DataSize,"000x");
			}
				if(vd.nam_id_dt_attr->getName()!=NULL)
				{
					pi.Name = (char*)malloc(strlen(vd.nam_id_dt_attr->getName()) + strlen(ModuleName) + 6);
					strcpy(pi.Name,getPIName(objNode->getNodeId()));
					strcat(pi.Name, ModuleName);
					strcat(pi.Name, ".");
					strcat(pi.Name,vd.nam_id_dt_attr->getName());
					
					pi.ModuleName = (char*)malloc(strlen(ModuleName) + 1);
					strcpy(pi.ModuleName, ModuleName);		
					
					pi.VarName = (char*)malloc(strlen(vd.nam_id_dt_attr->getName()) + 1);
					strcpy(pi.VarName, vd.nam_id_dt_attr->getName());		
					//printf("\n PI Name: %s",pi.Name);
				}
				
				pi.CNNodeID = objNode->getNodeId();
					
				if(vd.nam_id_dt_attr->getDataType()!=NULL)
				{
					pi.DataInfo._dt_Name = (char*)malloc(strlen(vd.nam_id_dt_attr->getDataType()) +1);
					strcpy(pi.DataInfo._dt_Name ,(const char*)vd.nam_id_dt_attr->getDataType());
				}
		
				/* Set the IEC DT*/
			pi.DataInfo = *(getIECDT(vd.nam_id_dt_attr->getDataType()));
			if((pi.DataInfo.DataSize == 1) && (StartBitOffset == 7))
			{
				StartBitOffset = -1;
			}
			SetPIOffsets(&pi, StartByteOffset, i,StartBitOffset); 
			
			objNode->addProcessImage(pi);
		}
		
	}

		/*if(objCDT->VarIndex != -1)
		{
			varDeclaration vd;
			vd = objCDT->
			objCDT
		}*/
		if(objCDT->previousCDT_UId !=NULL)
		{
			objCDT = objAppProc->getCDTbyUniqueID(objCDT->previousCDT_UId);
		
			for(int i=(lastVarIndex + 1) ; i<objCDT->varCollection.Count(); i++)
			{
				if(!CDTCompleted)
				{
					varDeclaration vd;
					//printf("\n i : %d",i);
					vd = objCDT->varCollection[i];
					//printf("\nVar Count: %d",objCDT->varCollection.Count());
					if(vd.nam_id_dt_attr->getDtUniqueRefId() != NULL)
					{
						/*objCDT->previousCDT_UId = (char*)malloc(strlen(vd.StructUniqueId)+1);
						strcpy(objCDT->previousCDT_UId, vd.StructUniqueId);	*/	
						objCDT = objAppProc->getCDTbyUniqueID(vd.nam_id_dt_attr->getDtUniqueRefId());
						objAppProc->updatePreviousCDT_UId(vd.StructUniqueId, objCDT->Index);
						//printf("\n previousCDT_UId : %s",objCDT->previousCDT_UId);
						//printf("\n DataRefID : %s",vd.nam_id_dt_attr->getDtUniqueRefId());
				
						lastVarIndex = i;
						ProcessCDT(objCDT, objAppProc, objNode, para, pdoType, StartByteOffset, ModuleName);
					
					}						
				}
			}
		}
		
	}
	CDTCompleted = true;
	//printf("exiting");
}

void DecodeUniqiueIDRef(char* uniquedIdref, CNode* objNode, EPDOType pdoType, int startingOffset,char* ModuleName)
{
	ocfmException objex;
	Parameter* para;
	CApplicationProcess* objAppProc;
	CComplexDataType* objCDT;
	
	try
	{
		if(objNode->getApplicationProcess()!=NULL)
		{
			
			objAppProc = objNode->getApplicationProcess();
			if(objAppProc->ParameterCollection.Count()!=0)
			{
				
				para = objAppProc->get_Parameterby_UniqueIDRef(uniquedIdref);
				if(para == NULL)
				{
					objex.ocfm_Excpetion(OCFM_ERR_UNIQUE_ID_REF_NOT_FOUND);
					throw objex;
				}
				
				//if(para->access != NULL)
				//{
				//	
				//	//procImage.Direction = getParameterAccess(para->access);
				//	//strcpy(procImage.Direction, getParameterAccess(para->access));
				//	strcpy(procImage.Direction, "rw");
				//	procImage.ParametrIndex = para->ParaIndex;

				//}
				// Check if DataTypeUniqueIDref exists
				if(para->name_id_dt_attr.dataTypeUniqueIDRef !=NULL)
				{
							
						objCDT = objAppProc->getCDTbyUniqueID(para->name_id_dt_attr.dataTypeUniqueIDRef);
						if(objCDT ==NULL)
						{
							objex.ocfm_Excpetion(OCFM_ERR_STRUCT_DATATYPE_NOT_FOUND);
							throw objex;
						}
						ProcessCDT(objCDT, objAppProc, objNode, para, pdoType, startingOffset, ModuleName); 
						lastVarIndex = -1;
						CDTCompleted = false;
				}
				
				else
				{
					
				}
			}
			
		}
		
	}
	catch(ocfmException& ex)
	{
		throw ex;
	}
}
/**************************************************************************************************
	* Function Name: ProcessPDONodes
    * Description: Processes the Node
/****************************************************************************************************/
ocfmRetCode ProcessPDONodes()
{
		CNodeCollection* objNodeCol;
		ocfmException* objex = NULL;
		objNodeCol = CNodeCollection::getNodeColObjectPointer();
		CNode* objNode;
	
		CIndexCollection* objPDOCollection;
		CIndexCollection* objIndexCollection;
		/* Check RPDO Mapped objects*/
		int NodesCount = 0;
		ocfmRetCode retCode;
		
		NodesCount = objNodeCol->getCNNodesCount();
		
			#if defined DEBUG	
						cout<< "Nodes cunt"<<NodesCount<< endl;
					#endif
		if(NodesCount == 0)
		{
			retCode.code = OCFM_ERR_NO_CN_NODES_FOUND;
			return retCode;
		}

			size8INOffset.currOffset  = 0; size8INOffset.prevOffset = 0;
			size16INOffset.currOffset  = 0; size16INOffset.prevOffset = 0;
			size32INOffset.currOffset  = 0; size32INOffset.prevOffset = 0;
			size64INOffset.currOffset  = 0; size64INOffset.prevOffset = 0;
		
			size8OUTOffset.currOffset  = 0; size8OUTOffset.prevOffset = 0;
			size16OUTOffset.currOffset  = 0; size16OUTOffset.prevOffset = 0;
			size32OUTOffset.currOffset  = 0; size32OUTOffset.prevOffset = 0;
			size64OUTOffset.currOffset  = 0; size64OUTOffset.prevOffset = 0;
		for(int i = 0; i< objNodeCol->getNumberOfNodes() ;i++)
		{
		//printf("\n NodeID %d",NodeID);
			objNode = objNodeCol->getNodebyColIndex(i);
			/* Process PDO Objects for CN*/
		
				if (objNode->getNodeType() != MN )
				{
					if(!(objNode->HasPdoObjects()))	
					{
				
						return retCode;
					}
				

						/* Empty ProcessImage collection*/
						objNode->DeleteCollectionsForPI();
						
						objPDOCollection = objNode->getPDOIndexCollection();
						if(objPDOCollection!= NULL)
						{
								
							objIndexCollection = objNode->getIndexCollection();
							
							for(int count = 0; count<objPDOCollection->getNumberofIndexes(); count++)
							{
									CIndex* objIndex;
									objIndex = objPDOCollection->getIndex(count);
									if(!(CheckIfMappingPDO((char*)objIndex->getIndexValue())))
									continue;
									else
											#if defined DEBUG	
								cout<< "index:"<<(char*)objIndex->getIndexValue() << endl;
							#endif
									if(objIndex->getNumberofSubIndexes() > 0)
									{
										int sicount = 1;
										int siTotal = objIndex->getNumberofSubIndexes();
								
											#if defined DEBUG	
								cout<< "siTotal:"<<siTotal << endl;
							#endif
										while(sicount< siTotal)
										{
											CSubIndex* objSI;
											objSI = objIndex->getSubIndex(sicount);
												#if defined DEBUG	
								cout<< "objSI->getIndexValue():"<<objSI->getIndexValue() << endl;
									cout<< "objSI->getName():"<<objSI->getName() << endl;
								
														cout<< "objSI->getActualValue():"<<objSI->getActualValue() << endl;

							#endif
											if (objSI->getActualValue()!=NULL)
											{
														#if defined DEBUG	
								cout<< "objSI->getActualValue():"<<objSI->getActualValue() << endl;
							#endif
													const char* value = objSI->getActualValue();
													int len = strlen(value);
													char* reverseValue = (char*)malloc(len);
													/* Reverse the actual value to get Index / subindex*/
											/*		reverseValue = reverse((char*)value);*/
													
													/* Get the Index*/
													char* strModuleIndex = (char*)malloc(5);
													strModuleIndex = subString((char*)value, len-4,4);
													strModuleIndex[5] ='\0';
											/*		strModuleIndex = strncpy(strModuleIndex,reverseValue,4);
											*/				#if defined DEBUG	
								cout<< "strModuleIndex:"<<strModuleIndex << endl;
							#endif
											
											/* Get the SubIndex*/
													char* strSubIndex = (char*)malloc(3);
													//strSubIndex = subString(reverseValue,2,2);
													strSubIndex = subString((char*)value, len-6,2);
													strSubIndex[3] ='\0';
													
													#if defined DEBUG	
								cout<< "strSubIndex:"<<strSubIndex << endl;
							#endif
													char*	ds = new char[5];
													ds =  subString((char*)value, 2, 4);
											
													CIndex* objModuleIndex;
													CSubIndex* objSIndex;
													
													MNPdoVariable objPDOvar;
													int Offset;
													int datasize;
													datasize =  hex2int(ds);
													
													/* Assign the PDO type*/
													objPDOvar.pdoType = objIndex->getPDOType();
													
													if(objIndex->getPDOType() == PDO_RPDO)											
													Offset = ComputeOUTOffset(objNode->getNodeId(), datasize, objIndex->getPDOType());
													else if(objIndex->getPDOType() == PDO_TPDO)											
													Offset = ComputeINOffset(objNode->getNodeId(), datasize, objIndex->getPDOType());
											
													
													
													/* Assign the value*/
													objPDOvar.Value = new char[10];
													strcpy(objPDOvar.Value, subString((char*)value, 0, 10));
													/* Assign the Offset*/
													objPDOvar.Offset = Offset;
													printf("\n Offset for bit variable : %d", Offset);
													
													/* Assign Index*/
													objPDOvar.Index = new char[5];
													switch(datasize)
													{
														case 8:						
															if(objIndex->getPDOType() == PDO_TPDO)
															strcpy(objPDOvar.Index, "A001");	
															
															else if(objIndex->getPDOType() == PDO_RPDO)
															strcpy(objPDOvar.Index, "A031");	
															
															break;										
														case 16:											
															if(objIndex->getPDOType() == PDO_TPDO)
															strcpy(objPDOvar.Index, "A011");	
															
															else if(objIndex->getPDOType() == PDO_RPDO)
															strcpy(objPDOvar.Index, "A041");																
															
															break;
														case 32:											
															if(objIndex->getPDOType() == PDO_TPDO)
															strcpy(objPDOvar.Index, "A021");	
															
															else if(objIndex->getPDOType() == PDO_RPDO)
															strcpy(objPDOvar.Index, "A051");	
															
															break;																			
														case 64:
															if(objIndex->getPDOType() == PDO_TPDO)
															strcpy(objPDOvar.Index, "A071");	
															
															else if(objIndex->getPDOType() == PDO_RPDO)
															strcpy(objPDOvar.Index, "A061");	
															break;		
														
													}
															/* Assign SubIndex*/							
															int SIdx;
															int d = datasize/8;
															SIdx = Offset/d + 1;
															objPDOvar.SubIndex = new char[2];
															objPDOvar.SubIndex =  itoa(SIdx, objPDOvar.SubIndex, 16);
															objPDOvar.SubIndex = padLeft(objPDOvar.SubIndex, '0', 2);
															
															objNode->addMNPDOvar(objPDOvar, objIndex->getPDOType());
																	
														objModuleIndex = objIndexCollection->getIndexbyIndexValue(strModuleIndex);
														if(objModuleIndex==NULL)
														{								
															objex->ocfm_Excpetion(OCFM_ERR_MODULE_INDEX_NOT_FOUND);
															throw objex;
														}
 														objSIndex = objModuleIndex->getSubIndexbyIndexValue(strSubIndex);
														if(objSIndex==NULL)
														{								
															objex->ocfm_Excpetion(OCFM_ERR_MODULE_SUBINDEX_NOT_FOUND);
															throw objex;
														}
														
																								
														
													//printf("\n NodeID %d",objNode->getNodeId());
														if(objSIndex->getUniqueIDRef()!=NULL)
														{
															DecodeUniqiueIDRef(objSIndex->getUniqueIDRef(), objNode, objIndex->getPDOType(), Offset,(char*) objModuleIndex->getName());
															
														}
														else
														{
															ProcessImage objProcessImage;
								
															/* Name of the Process Image variable*/
															objProcessImage.Name = (char*)malloc(strlen(objSIndex->getName())+1);
															strcpy(objProcessImage.Name, objSIndex->getName());
															/* Access of the Process Image variable*/
															strcpy(objProcessImage.Direction, objSIndex->getAccessType());
															DataType dt;
															dt = objSIndex->getDataType();
															/* Data Size in hex of the Process Image variable*/
														/*	objProcessImage.DataSize = (char*)malloc(strlen(dt.DataSize+1));*/
															//strcpy(objProcessImage.DataSize, dt.DataSize);
															objProcessImage.DataInfo.DataSize = atoi(dt.DataSize);
															/* Datatype in hex of the Process Image variable*/
															objProcessImage.DataInfo._dt_Name = (char*)malloc(strlen(dt.Name+1));
															strcpy(objProcessImage.DataInfo ._dt_Name, dt.Name);
															
															objNode->addProcessImage(objProcessImage);
														}
												}
												sicount++;
											}
										}
									}
								}
							}
						}
		retCode.code = OCFM_ERR_SUCCESS ;
		return retCode;
	}

void ConvertCdcToBinary(char* fileName,char* tempFile)
{
	FILE *fin, *fout;
	int count=0;
	int num=0;
	char ch=0;
	int iCtr;
	int iLength;
	unsigned char ca_cn1obd[10000];
	unsigned char tempCn1Obd[10000];

	fin = fopen(tempFile, "r");
	//filePath = (char*)malloc(80);
	//filePath = strchr(reverse(fileName),'\\');
	//filePath = subString(fileName, 0, strlen(fileName) - strlen(filePath) +1);
	//strcat(filePath,"config_data.cdc");
	////fout = fopen("config_data.cdc", "wb");
	fout = fopen(fileName, "wb");
	
	
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
	
	fclose(fin);
	fclose(fout);	
}
int getCNDataLen(char* Buffer)
{
	int count=0;
	int num=0;
	char ch=0;
	int iCtr;
	int iLength;
	unsigned char ca_cn1obd[10000];
	unsigned char tempCn1Obd[10000];
	int i =0;
	
	ch = *(Buffer);
	while(ch !='\0')
	{
		ch = *(Buffer + i);
		if(ch == '/') 
		{
			while(ch != '\n')
			{
				ch = *(Buffer + i);
				i++;
			}
				/*while(fgetc(fin) != '\n');*/
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
				i++;
			}
		
		iLength = num;
		//printf( "\niLength = %d\n",iLength);
		//For Byte Packing
		for (iCtr = 0 , count = 0; iCtr < iLength; iCtr++, count++ )
		{
			tempCn1Obd[count] = (unsigned char)( ( ca_cn1obd[ iCtr ] << 4 ) | ca_cn1obd[ ++iCtr ] );
			//printf("0x%2x\t",tempCn1Obd[count]);
		}
		//printf("Size : %d\n", count);
		/*count = count -1;*/
		}
		return count;
}
 int lenOfCNBuffer(char* Buffer)
 {
		char tempchar;
		int len = strlen(Buffer);
		int counter =0;
		int actualLength=0;
		while(counter<len)
		{
			tempchar = *(Buffer+counter);
			if(tempchar!='\n' && tempchar != '\t')
			actualLength++;
			counter++;
		}
		return actualLength;

 }
CIndex* getMNIndexValues(char* Index)
{
		CNodeCollection* objNodeCollection;
		objNodeCollection = CNodeCollection::getNodeColObjectPointer();		
		CIndexCollection* objIndexCol;
		CNode objNode;
		objNode = objNodeCollection->getMNNode();
		objIndexCol = objNode.getIndexCollection();
		CIndex* objIndex = NULL;
		objIndex = objIndexCol->getIndexbyIndexValue(Index);
		return objIndex;
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
void WriteXAPElements(ProcessImage piCol[], xmlTextWriterPtr& writer,int VarCount, EPIDirectionType piType)
{
	int rc;
	if (VarCount!=0)
	{
	/* Start an element named "Channel". Since thist is the first
     * element, this will be the root element of the document. */
    rc = xmlTextWriterStartElement(writer, BAD_CAST "ProcessImage");
    if (rc < 0)
    {
        //printf("testXmlwriterDoc: Error at xmlTextWriterStartElement\n");
        return;
    }
    if(piType ==INPUT)
					rc = xmlTextWriterWriteAttribute(writer, BAD_CAST "type",      
                                   BAD_CAST "input");
				else if(piType ==OUTPUT)
					rc = xmlTextWriterWriteAttribute(writer, BAD_CAST "type",      
                                   BAD_CAST "output");
				
                                  
    if (rc < 0)
    {
        //printf("testXmlwriterDoc: Error at xmlTextWriterWriteAttribute\n");
        return;
    }
    
	for(int count =0 ; count<VarCount ; count++)
			{
					ProcessImage pi;
					pi = piCol[count];
	 /* Start an element named "Channel". Since thist is the first
     * element, this will be the root element of the document. */
    rc = xmlTextWriterStartElement(writer, BAD_CAST "Channel");
    if (rc < 0)
    {
        //printf("testXmlwriterDoc: Error at xmlTextWriterStartElement\n");
        return;
    }
			


    

    /* Add an attribute with name "Name" and value  to channel. */
     //printf("\n%s",pi.Name);
    rc = xmlTextWriterWriteAttribute(writer, BAD_CAST "Name",      
                                   BAD_CAST pi.Name);
                                  
    if (rc < 0)
    {
        //printf("testXmlwriterDoc: Error at xmlTextWriterWriteAttribute\n");
        return;
    }
    
    

    ///* Add an attribute with name "direction" and value to channel */
    //  printf("\n%s",pi.Direction);
    //rc = xmlTextWriterWriteAttribute(writer, BAD_CAST "Direction",
    //                                 BAD_CAST pi.Direction );
    //if (rc < 0)
    //{
    //    printf("testXmlwriterDoc: Error at xmlTextWriterWriteAttribute\n");
    //    return;
    //}

				/* Add an attribute with name "DataType" and value to channel */
				 //printf("\n%s",pi.DataType);
    rc = xmlTextWriterWriteAttribute(writer, BAD_CAST "dataType",
                                     BAD_CAST pi.DataInfo._dt_Name );
    if (rc < 0)
    {
        //printf("testXmlwriterDoc: Error at xmlTextWriterWriteAttribute\n");
        return;
    }
    
    ///* Add an attribute with name "dataSize" and value to channel */
    //printf("\n%s",pi.DataSize);
    char * size = new char[20];
    size = itoa(pi.DataInfo.DataSize,size,10);
    
    rc = xmlTextWriterWriteAttribute(writer, BAD_CAST "dataSize",
                                     BAD_CAST size );
    if (rc < 0)
    {
        //printf("testXmlwriterDoc: Error at xmlTextWriterWriteAttribute\n");
        return;
    }
			
   
     ///* Add an attribute with name "dataSize" and value to channel */
    //printf("\n%s",pi.ByteOffset);
  		char* byteOffset = new char[6];
				byteOffset = itoa(pi.ByteOffset, byteOffset, 16);
				byteOffset = ConvertToHexformat(byteOffset, 4, 1);
			
    rc = xmlTextWriterWriteAttribute(writer, BAD_CAST "PIOffset",
                                     BAD_CAST byteOffset );
    if (rc < 0)
    {
        //printf("testXmlwriterDoc: Error at xmlTextWriterWriteAttribute\n");
        return;
    }
    
    if(pi.BitOffset!=-1)
    {
						char* bitoffset = new char[4];
						bitoffset = itoa(pi.BitOffset, bitoffset, 16);
						bitoffset = ConvertToHexformat(bitoffset, 2, 1);
			
					rc = xmlTextWriterWriteAttribute(writer, BAD_CAST "BitOffset",
                                     BAD_CAST bitoffset );
    }
    if (rc < 0)
    {
        //printf("testXmlwriterDoc: Error at xmlTextWriterWriteAttribute\n");
        return;
    }
    ///* Close the element named Channel. */
    rc = xmlTextWriterEndElement(writer);
    if (rc < 0)
    {
        //printf("testXmlwriterDoc: Error at xmlTextWriterEndElement\n");
        return;
    }
			}
	 ///* Close the element named ProcessImage. */
   rc = xmlTextWriterEndElement(writer);
   if (rc < 0)
   {
       //printf("testXmlwriterDoc: Error at xmlTextWriterEndElement\n");
       return;
   }

  }
}
void StartXAPxml(xmlTextWriterPtr& writer,  xmlDocPtr& doc)

{
	 int rc;
		//xmlTextWriterPtr writer =NULL;


    /* Create a new XmlWriter for DOM, with no compression. */
    writer = xmlNewTextWriterDoc(&doc, 0);
    if (writer == NULL) {
        //printf("testXmlwriterDoc: Error creating the xml writer\n");
        return;
    }

    /* Start the document with the xml default for the version,
     * encoding ISO 8859-1 and the default for the standalone
     * declaration. */
    rc = xmlTextWriterStartDocument(writer, NULL, MY_ENCODING, NULL);
    if (rc < 0) {
       //printf("testXmlwriterDoc: Error at xmlTextWriterStartDocument\n");
        return;
    }

			/* Write a comment as child of ORDER */
		 rc = xmlTextWriterWriteComment(writer,BAD_CAST "This file was autogenerated by openCONFIGURATOR");
    if (rc < 0)
     {
        //printf("testXmlwriterDoc: Error at xmlTextWriterWriteFormatComment\n");
        return;
    }
    rc = xmlTextWriterStartElement(writer, BAD_CAST "ApplicationProcess");
    if (rc < 0) {
        //printf("testXmlwriterDoc: Error at xmlTextWriterStartElement\n");
        return;
    }
}
 
void EndWrtitingXAP( xmlTextWriterPtr& writer, char* fileName, xmlDocPtr& doc)
{ 
	int rc;

   
		 /*///* Close the element named ApplicationProcess. */
    rc = xmlTextWriterEndElement(writer);
    if (rc < 0)
    {
       // printf("testXmlwriterDoc: Error at xmlTextWriterEndElement\n");
        return;
    }
    rc = xmlTextWriterEndDocument(writer);
    if (rc < 0) {
        //printf("testXmlwriterDoc: Error at xmlTextWriterEndDocument\n");
        return;
    }

    xmlFreeTextWriter(writer);

				xmlSaveFormatFile ((const char*)fileName, doc, 1);

    xmlFreeDoc(doc);
}
ocfmRetCode GenerateXAP(char* fileName)
{
		CNodeCollection* objNodeCollection;
		ocfmException objException;
		objNodeCollection = CNodeCollection::getNodeColObjectPointer();		
		ocfmRetCode retCode;
		try
		{
			if(objNodeCollection->getNumberOfNodes()==0)
			{
				objException.ocfm_Excpetion(OCFM_ERR_NO_NODES_FOUND);
				throw objException;
			}
			if(objNodeCollection->getCNNodesCount() ==0)
			{
				objException.ocfm_Excpetion(OCFM_ERR_NO_CN_NODES_FOUND);
				throw objException;
			}
			
					/*Process PDO Nodes*/
				retCode = ProcessPDONodes();
				
				if(retCode.code != OCFM_ERR_SUCCESS)
				return retCode;
			CNode objNode;
			xmlTextWriterPtr writer = NULL;
			xmlDocPtr doc = NULL;
			int picount = 0;
			int i=10;
			ProcessImage PIInCol[4000] = {};
			ProcessImage PIOutCol[4000] = {};
			GroupInOutPIVariables(PIInCol, PIOutCol);
			/* Calculate Offsets for Input Variable*/
			//CalculateOffsets(InVars, INPUT);
			
				/* Calculate Offsets for Input Variable*/
			//CalculateOffsets(OutVars, OUTPUT);
			
			
			StartXAPxml(writer, doc);	
			
			WriteXAPElements(PIInCol, writer, InVars, INPUT);
			WriteXAPElements(PIOutCol, writer, OutVars, OUTPUT);

			EndWrtitingXAP(writer, fileName, doc);
			
			/*Generate Header file */			
			
			GenerateXAPHeaderFile (fileName, PIInCol, PIOutCol, InVars, OutVars);
			return objException._ocfmRetCode;
		}
		catch(ocfmException& ex)
		{
			return ex._ocfmRetCode ;
		}
}

/**************************************************************************************************
	* Function Name: GetIndexAttributes
    * Description:
	* Return value: ocfmRetCode
/****************************************************************************************************/

ocfmRetCode GetIndexAttributes(
	int				NodeID, 
	ENodeType		NodeType, 
	char* 			IndexID, 
	EAttributeType 	AttributeType, 
	char* 			Out_AttributeValue)
{
	int IndexPos;
	ocfmRetCode ErrStruct;
	
	try
	{
		ErrStruct = IfIndexExists(NodeID, NodeType, IndexID, &IndexPos);
		if(ErrStruct.code == OCFM_ERR_SUCCESS)
		{
			//cout << "IndexPos:" << IndexPos << endl;
		}
		else
		{
			 //OCFM_ERR_INDEXID_NOT_FOUND;
			//return ErrStruct;
			ocfmException objException;				
			objException.ocfm_Excpetion(OCFM_ERR_INDEXID_NOT_FOUND);
			throw objException;
		}
		CNode objNode;		
		CNodeCollection *objNodeCollection;
		CIndexCollection *objIndexCollection;
		CIndex objIndex;
		CIndex* objIndexPtr;
		
		objIndex.setNodeID(objNode.getNodeId());
		objNodeCollection= CNodeCollection::getNodeColObjectPointer();
		objNode = objNodeCollection->getNode(NodeType, NodeID);

		objIndexCollection = objNode.getIndexCollection();
		objIndexPtr = objIndexCollection->getIndex(IndexPos);
		
		switch(AttributeType)
		{
			case 0:									
					if(objIndexPtr->getName() != NULL)
						strcpy(Out_AttributeValue, (char *)objIndexPtr->getName());
					else
						strcpy(Out_AttributeValue, "");
					break;
			case 1:
					if(objIndexPtr->getObjectType() != NULL)
						strcpy(Out_AttributeValue, (char *)objIndexPtr->getObjectType());
					else
						strcpy(Out_AttributeValue, "");
					break;
			case 2:
					DataType tmpDataType;
					tmpDataType = objIndexPtr->getDataType();
					if(tmpDataType.Name != NULL)
						strcpy(Out_AttributeValue, tmpDataType.Name);
					else
						strcpy(Out_AttributeValue, "");					
					break;		
			case 3:
					if(objIndexPtr->getAccessType() != NULL)
						strcpy(Out_AttributeValue, (char *)objIndexPtr->getAccessType());
					else
						strcpy(Out_AttributeValue, "");
					break;
			case 4:
					if(objIndexPtr->getDefaultValue() != NULL)
						strcpy(Out_AttributeValue, (char *) objIndexPtr->getDefaultValue());
					else
						strcpy(Out_AttributeValue, "");
					break;
			case 5:
					if(objIndexPtr->getActualValue() != NULL)
						strcpy(Out_AttributeValue, (char *) objIndexPtr->getActualValue());
					else
						strcpy(Out_AttributeValue, "");
					break;
			case 6:
					if(objIndexPtr->getPDOMapping() != NULL)
						strcpy(Out_AttributeValue, (char *) objIndexPtr->getPDOMapping());
					else
						strcpy(Out_AttributeValue, "");
					break;
			case 7:
					if(objIndexPtr->getLowLimit() != NULL)
						strcpy(Out_AttributeValue, (char *) objIndexPtr->getLowLimit());
					else
						strcpy(Out_AttributeValue, "");
					break;
			case 8:
					if(objIndexPtr->getHighLimit() != NULL)
						strcpy(Out_AttributeValue, (char *) objIndexPtr->getHighLimit());
					else
						strcpy(Out_AttributeValue, "");
					break;									
			default:
					//cout << "invalid Attribute Type" << endl;
					//ErrStruct.code = OCFM_ERR_INVALID_ATTRIBUTETYPE;
					//return ErrStruct;
					ocfmException objException;				
					objException.ocfm_Excpetion(OCFM_ERR_INVALID_ATTRIBUTETYPE);
					throw objException;
		}
		//cout << "\n\n\n$SOut_AttributeValue:" << Out_AttributeValue << endl;
		ErrStruct.code = OCFM_ERR_SUCCESS;
		return ErrStruct;
	}
	catch(ocfmException* ex)
	{
		return ex->_ocfmRetCode;
	}
}

/**************************************************************************************************
	* Function Name: GetIndexAttributesbyPositions
    * Description:
	* Return value: ocfmRetCode
/****************************************************************************************************/
ocfmRetCode GetIndexAttributesbyPositions(
	int 			NodePos, 
	int 			IndexPos, 
	EAttributeType 	AttributeType, 
	char* 			Out_AttributeValue)
{
	ocfmRetCode ErrStruct;
	try
	{
		// Check for the Existance of the Node in the NodePos
		CNode objNode;		
		CNodeCollection *objNodeCollection;
		CIndexCollection *objIndexCollection;
		CIndex* objIndexPtr;

		objNodeCollection= CNodeCollection::getNodeColObjectPointer();
		
		//objIndexCollection = objNode.getIndexCollection();
		
		int tmp_NodeCount = objNodeCollection->getNumberOfNodes();		
		//cout << "\n\ntmp_NodeCount:" << tmp_NodeCount << endl;
		if(NodePos >= tmp_NodeCount)
		{
			ocfmException* objException = new ocfmException;
			objException->ocfm_Excpetion(OCFM_ERR_INVALID_NODEPOS);		
			throw objException;
		}
		else if(tmp_NodeCount == 0)
		{
			ocfmException* objException = new ocfmException;
			objException->ocfm_Excpetion(OCFM_ERR_NO_NODES_FOUND);		
			throw objException;
		}
	
		objNode = objNodeCollection->getNodebyCollectionIndex(NodePos);
		objIndexCollection = objNode.getIndexCollection();
		
		int tmp_IndexCount = objIndexCollection->getNumberofIndexes();
		//cout << "\n\tmp_IndexCount:" << tmp_IndexCount << endl;
		if(tmp_IndexCount == 0)
		{
			ocfmException* objException = new ocfmException;
			objException->ocfm_Excpetion(OCFM_ERR_NO_INDEX_FOUND);
			throw objException;
		}
		else if(IndexPos >= tmp_IndexCount)
		{
			ocfmException* objException = new ocfmException;
			objException->ocfm_Excpetion(OCFM_ERR_INVALID_INDEXPOS);
			throw objException;
		}
		
		objIndexPtr = objIndexCollection->getIndex(IndexPos);
		
			switch(AttributeType)
			{
				case 0:									
						if(objIndexPtr->getName() != NULL)
							strcpy(Out_AttributeValue, (char *)objIndexPtr->getName());
						else
							strcpy(Out_AttributeValue, "");
						break;
				case 1:
						if(objIndexPtr->getObjectType() != NULL)
							strcpy(Out_AttributeValue, (char *)objIndexPtr->getObjectType());
						else
							strcpy(Out_AttributeValue, "");
						break;
				case 2:
						DataType tmpDataType;
						tmpDataType = objIndexPtr->getDataType();
						if(tmpDataType.Name != NULL)
						{
							strcpy(Out_AttributeValue, tmpDataType.Name);
						}
						else
						{
							strcpy(Out_AttributeValue, "");					
						}
						break;		
				case 3:
						if(objIndexPtr->getAccessType() != NULL)
							strcpy(Out_AttributeValue, (char *)objIndexPtr->getAccessType());
						else
							strcpy(Out_AttributeValue, "");
						break;
				case 4:
						if(objIndexPtr->getDefaultValue() != NULL)
							strcpy(Out_AttributeValue, (char *) objIndexPtr->getDefaultValue());
						else
							strcpy(Out_AttributeValue, "");
						break;
				case 5:
						if(objIndexPtr->getActualValue() != NULL)
							strcpy(Out_AttributeValue, (char *) objIndexPtr->getActualValue());
						else
							strcpy(Out_AttributeValue, "");
						break;
				case 6:
						if(objIndexPtr->getPDOMapping() != NULL)
							strcpy(Out_AttributeValue, (char *) objIndexPtr->getPDOMapping());
						else
							strcpy(Out_AttributeValue, "");
						break;
				case 7:
						if(objIndexPtr->getLowLimit() != NULL)
							strcpy(Out_AttributeValue, (char *) objIndexPtr->getLowLimit());
						else
							strcpy(Out_AttributeValue, "");
						break;
				case 8:
						if(objIndexPtr->getHighLimit() != NULL)
							strcpy(Out_AttributeValue, (char *) objIndexPtr->getHighLimit());
						else
							strcpy(Out_AttributeValue, "");
						break;									
				default:
						//cout << "invalid Attribute Type" << endl;
						ErrStruct.code = OCFM_ERR_INVALID_ATTRIBUTETYPE;
						return ErrStruct;
			}
	}
	catch(ocfmException* ex)
	{
		//cout << "\n\nCatch...\n" << endl;
		return ex->_ocfmRetCode;
	}
	//cout << "\n\n\n$SOut_AttributeValue:" << Out_AttributeValue << endl;
	ErrStruct.code = OCFM_ERR_SUCCESS;
	return ErrStruct;
}

/**************************************************************************************************
	* Function Name: GetSubIndexAttributes
    * Description:
	* Return value: ocfmRetCode
/****************************************************************************************************/
ocfmRetCode GetSubIndexAttributes(
	int 			NodeID, 
	ENodeType 		NodeType, 
	char* 			IndexID, 
	char* 			SubIndexID, 
	EAttributeType 	AttributeType, 
	char* 			Out_AttributeValue)
{
		CNode objNode;		
		CNodeCollection *objNodeCollection;
		CIndexCollection *objIndexCollection;
		CIndex objIndex;
		CIndex* objSubIndex;
		int SubIndexPos;
		int IndexPos;
		ocfmRetCode ErrStruct;
		
		try
		{
		
			ErrStruct = IfSubIndexExists(NodeID, NodeType, IndexID, SubIndexID, &SubIndexPos, &IndexPos);
			if(ErrStruct.code == OCFM_ERR_SUCCESS)
			{
				//SubIndexPos = Tmp_ErrStruct.returnValue;
			}
			else
			{
				//OCFM_ERR_INVALID_SUBINDEXID;
				//return ErrStruct;
				ocfmException objException;				
				objException.ocfm_Excpetion(OCFM_ERR_INVALID_SUBINDEXID);
				throw objException;
			}

			objIndex.setNodeID(objNode.getNodeId());
			objNodeCollection= CNodeCollection::getNodeColObjectPointer();
			objNode = objNodeCollection->getNode(NodeType, NodeID);

			objIndexCollection = objNode.getIndexCollection();
			objSubIndex =objIndexCollection->getIndex(IndexPos);
			
			//cout << "NumberofSubIndexes:" << objIndex.getNumberofSubIndexes()<< endl;

			CSubIndex* objSubIndexPtr;
			objSubIndexPtr = objSubIndex->getSubIndex(SubIndexPos);	
			
			switch(AttributeType)
			{
				case 0:						
						if(objSubIndexPtr->getName() != NULL)
							strcpy(Out_AttributeValue, (char *)objSubIndexPtr->getName());
						else
							strcpy(Out_AttributeValue, "");
						break;
				case 1:
						if(objSubIndexPtr->getObjectType() != NULL)
							strcpy(Out_AttributeValue, (char *)objSubIndexPtr->getObjectType());
						else
							strcpy(Out_AttributeValue, "");
						break;
				case 2:
						DataType tmpDataType;
						tmpDataType = objSubIndexPtr->getDataType();										
						if(tmpDataType.Name != NULL)
							strcpy(Out_AttributeValue, tmpDataType.Name);
						else
							strcpy(Out_AttributeValue, "");					
						break;		
				case 3:
						if(objSubIndexPtr->getAccessType() != NULL)
							strcpy(Out_AttributeValue, (char *)objSubIndexPtr->getAccessType());
						else
							strcpy(Out_AttributeValue, "");
						break;
				case 4:
						if(objSubIndexPtr->getDefaultValue() != NULL)
							strcpy(Out_AttributeValue, (char *) objSubIndexPtr->getDefaultValue());
						else
							strcpy(Out_AttributeValue, "");
						break;
				case 5:
						if(objSubIndexPtr->getActualValue() != NULL)
							strcpy(Out_AttributeValue, (char *) objSubIndexPtr->getActualValue());
						else
							strcpy(Out_AttributeValue, "");
						break;
				case 6:
						if(objSubIndexPtr->getPDOMapping() != NULL)
							strcpy(Out_AttributeValue, (char *) objSubIndexPtr->getPDOMapping());
						else
							strcpy(Out_AttributeValue, "");
						break;
				case 7:
						if(objSubIndexPtr->getLowLimit() != NULL)
							strcpy(Out_AttributeValue, (char *) objSubIndexPtr->getLowLimit());
						else
							strcpy(Out_AttributeValue, "");
						break;
				case 8:
						if(objSubIndexPtr->getHighLimit() != NULL)
							strcpy(Out_AttributeValue, (char *) objSubIndexPtr->getHighLimit());
						else
							strcpy(Out_AttributeValue, "");
						break;
				default:
						//cout << "invalid Attribute Type" << endl;
						//ErrStruct.code = OCFM_ERR_INVALID_ATTRIBUTETYPE;
						//return ErrStruct;
						ocfmException objException;				
						objException.ocfm_Excpetion(OCFM_ERR_INVALID_SUBINDEXID);
						throw objException;
			}		
			
			ErrStruct.code = OCFM_ERR_SUCCESS;
			return ErrStruct;
		}
		catch(ocfmException* ex)
		{
			return ex->_ocfmRetCode;
		}
}

/**************************************************************************************************
	* Function Name: GetSubIndexAttributesbyPositions
    * Description:
	* Return value: ocfmRetCode
/****************************************************************************************************/
ocfmRetCode GetSubIndexAttributesbyPositions(
	int 			NodePos, 
	int 			IndexPos,
	int				SubIndexPos,
	EAttributeType 	AttributeType, 
	char* 			Out_AttributeValue)
{
	ocfmRetCode ErrStruct;
	try
	{
				// Check for the Existance of the Node in the NodePos
		CNode objNode;		
		CNodeCollection *objNodeCollection;
		CIndexCollection *objIndexCollection;
		CIndex* objIndexPtr;

		objNodeCollection= CNodeCollection::getNodeColObjectPointer();
		
		//objIndexCollection = objNode.getIndexCollection();
		
		int tmp_NodeCount = objNodeCollection->getNumberOfNodes();		
		//cout << "\n\ntmp_NodeCount:" << tmp_NodeCount << endl;
		if(NodePos >= tmp_NodeCount)
		{
			ocfmException* objException = new ocfmException;
			objException->ocfm_Excpetion(OCFM_ERR_INVALID_NODEPOS);		
			throw objException;
		}
		else if(tmp_NodeCount == 0)
		{
			ocfmException* objException = new ocfmException;
			objException->ocfm_Excpetion(OCFM_ERR_NO_NODES_FOUND);		
			throw objException;
		}
	
		objNode = objNodeCollection->getNodebyCollectionIndex(NodePos);
		objIndexCollection = objNode.getIndexCollection();
		
		int tmp_IndexCount = objIndexCollection->getNumberofIndexes();
		//cout << "\n\tmp_IndexCount:" << tmp_IndexCount << endl;
		if(tmp_IndexCount == 0)
		{
			ocfmException* objException = new ocfmException;
			objException->ocfm_Excpetion(OCFM_ERR_NO_INDEX_FOUND);
			throw objException;
		}
		else if(IndexPos >= tmp_IndexCount)
		{
			ocfmException* objException = new ocfmException;
			objException->ocfm_Excpetion(OCFM_ERR_INVALID_INDEXPOS);
			throw objException;
		}
		
		objIndexPtr = objIndexCollection->getIndex(IndexPos);
		
		int tmp_SubIndexCount = objIndexPtr->getNumberofSubIndexes();
		//cout << "\n\ntmp_SubIndexCount:" << tmp_SubIndexCount << endl;

		if(SubIndexPos >= tmp_SubIndexCount)
		{
			ocfmException* objException = new ocfmException;
			objException->ocfm_Excpetion(OCFM_ERR_INVALID_SUBINDEXPOS);		
			throw objException;
		}
		else if(tmp_SubIndexCount == 0)
		{
			ocfmException* objException = new ocfmException;
			objException->ocfm_Excpetion(OCFM_ERR_NO_SUBINDEXS_FOUND);		
			throw objException;
		}
		
		CSubIndex* objSubIndexPtr;
		objSubIndexPtr = objIndexPtr->getSubIndex(SubIndexPos);			
		
		switch(AttributeType)
		{
			case 0:						
					if(objSubIndexPtr->getName() != NULL)
						strcpy(Out_AttributeValue, (char *)objSubIndexPtr->getName());
					else
						strcpy(Out_AttributeValue, "");
					break;
			case 1:
					if(objSubIndexPtr->getObjectType() != NULL)
						strcpy(Out_AttributeValue, (char *)objSubIndexPtr->getObjectType());
					else
						strcpy(Out_AttributeValue, "");
					break;
			case 2:
					DataType tmpDataType;
					tmpDataType = objSubIndexPtr->getDataType();										
					if(tmpDataType.Name != NULL)
						strcpy(Out_AttributeValue, tmpDataType.Name);
					else
						strcpy(Out_AttributeValue, "");					
					break;		
			case 3:
					if(objSubIndexPtr->getAccessType() != NULL)
						strcpy(Out_AttributeValue, (char *)objSubIndexPtr->getAccessType());
					else
						strcpy(Out_AttributeValue, "");
					break;
			case 4:
					if(objSubIndexPtr->getDefaultValue() != NULL)
						strcpy(Out_AttributeValue, (char *) objSubIndexPtr->getDefaultValue());
					else
						strcpy(Out_AttributeValue, "");
					break;
			case 5:
					if(objSubIndexPtr->getActualValue() != NULL)
						strcpy(Out_AttributeValue, (char *) objSubIndexPtr->getActualValue());
					else
						strcpy(Out_AttributeValue, "");
					break;
			case 6:
					if(objSubIndexPtr->getPDOMapping() != NULL)
						strcpy(Out_AttributeValue, (char *) objSubIndexPtr->getPDOMapping());
					else
						strcpy(Out_AttributeValue, "");
					break;
			case 7:
					if(objSubIndexPtr->getLowLimit() != NULL)
						strcpy(Out_AttributeValue, (char *) objSubIndexPtr->getLowLimit());
					else
						strcpy(Out_AttributeValue, "");
					break;
			case 8:
					if(objSubIndexPtr->getHighLimit() != NULL)
						strcpy(Out_AttributeValue, (char *) objSubIndexPtr->getHighLimit());
					else
						strcpy(Out_AttributeValue, "");
					break;
			default:
					//cout << "invalid Attribute Type" << endl;
					ErrStruct.code = OCFM_ERR_INVALID_ATTRIBUTETYPE;
					return ErrStruct;
		}			
	}
	catch(ocfmException* ex)
	{
		//cout << "\n\nCatch...\n" << endl;
		return ex->_ocfmRetCode;
	}
	//cout << "\n\n\n$SOut_AttributeValue:" << Out_AttributeValue << endl;
	ErrStruct.code = OCFM_ERR_SUCCESS;
	return ErrStruct;

}
/**************************************************************************************************
	* Function Name: GetNodeCount
    * Description:
	* Return value: ocfmRetCode
/****************************************************************************************************/
ocfmRetCode GetNodeCount(
	int 		MNID, 
	int* 		Out_NodeCount)
{
	ocfmRetCode ErrStruct;
	CNode objNode;
	CNodeCollection *objNodeCollection;
	try
	{
		objNodeCollection= CNodeCollection::getNodeColObjectPointer();
		if(objNodeCollection->getNumberOfNodes() < 0)
		{
			ocfmException objException;				
			objException.ocfm_Excpetion(OCFM_ERR_NO_NODES_FOUND);
			throw objException;
		}
		
		*Out_NodeCount = objNodeCollection->getNumberOfNodes();
		
		ErrStruct.code = OCFM_ERR_SUCCESS;
		return ErrStruct;
	}
	catch(ocfmException* ex)
	{
		return ex->_ocfmRetCode;
	}
}

char* getPIName(int NodeID)
{
	char* str;
	char* id = new char[2];
	str =(char*)malloc(5);
	strcpy(str, "CN");
	id =  itoa(NodeID,id, 10);
	strcat(str, id);
	strcat(str, ".");
	strcat(str,"\0");
	return str;
}

char* getParameterAccess(char* access)
{
	if (!strcmp(ConvertToUpper(access), "READ"))
	return "ro\0";
	else if(!strcmp(ConvertToUpper(access),"READWRITE"))
	return "rw\0";
	else return "xx";

}

/**************************************************************************************************
	* Function Name: GetIndexCount
    * Description:
	* Return value: ocfmRetCode
/****************************************************************************************************/
ocfmRetCode GetIndexCount(
	int 		NodeID, 
	ENodeType 	NodeType, 
	int* 		Out_IndexCount)
{
	ocfmRetCode ErrStruct;
		
	try
	{
		CNode objNode;		
		CNodeCollection *objNodeCollection;
		CIndexCollection *objIndexCollection;
		CIndex objIndex;
		bool flag = false;;
		
		int NodePos;
		ErrStruct = IfNodeExists(NodeID, NodeType, &NodePos, flag);
		if (ErrStruct.code == 0 && flag==true) 
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
		
		*Out_IndexCount = objIndexCollection->getNumberofIndexes();

		ErrStruct.code = OCFM_ERR_SUCCESS;
		return ErrStruct;
	}
	catch(ocfmException* ex)
	{
		return ex->_ocfmRetCode;
	}
}

/**************************************************************************************************
	* Function Name: GetSubIndexCount
    * Description:
	* Return value: ocfmRetCode
/****************************************************************************************************/
ocfmRetCode GetSubIndexCount(
	int 		NodeID,
	ENodeType 	NodeType,
	char* 		IndexID,
	int* 		Out_SubIndexCount) 
{
	CNode objNode;		
	CNodeCollection *objNodeCollection;
	CIndexCollection *objIndexCollection;
	CIndex objIndex;
	CIndex* objSubIndex;
	ocfmRetCode ErrStruct;
	
	int IndexPos;
	
	try
	{
		ErrStruct = IfIndexExists(NodeID, NodeType, IndexID, &IndexPos);
		if (ErrStruct.code == 0)
		{				
			//IndexPos = ErrStruct.returnValue;
		}
		else
		{	
			//ErrStruct.code = OCFM_ERR_INDEXID_NOT_FOUND;
			//return ErrStruct;
			ocfmException objException;				
			objException.ocfm_Excpetion(OCFM_ERR_INDEXID_NOT_FOUND);
			throw objException;
		}
		
		objIndex.setNodeID(objNode.getNodeId());
		objNodeCollection= CNodeCollection::getNodeColObjectPointer();
		objNode = objNodeCollection->getNode(NodeType, NodeID);

		objIndexCollection = objNode.getIndexCollection();
		objSubIndex =objIndexCollection->getIndex(IndexPos);
		
		*Out_SubIndexCount = objSubIndex->getNumberofSubIndexes();
		
		ErrStruct.code = OCFM_ERR_SUCCESS;
		return ErrStruct;
	}
	catch(ocfmException* ex)
	{
		return ex->_ocfmRetCode;
	}
}


void LoadObjectDictionary(char* fileName)
	{
	
		printf("Inside ObjectDictionary");
  xmlTextReaderPtr reader;
  int ret;
		CObjectDictionary* objDict;
		//printf("Inside ObjectDictionary3");
		objDict = CObjectDictionary::getObjDictPtr();
		if(objDict==NULL)
		printf("NULL");
		//printf("Inside ObjectDictionary1");
  reader = xmlReaderForFile(fileName, NULL, 0);
  //printf("Inside ObjectDictionary2");
		if (reader != NULL)
		{
			 ret = xmlTextReaderRead(reader);
    while (ret == 1)
				{		
						objDict->ProcessObjectDictionary(reader);						
						ret = xmlTextReaderRead(reader);
				}
				if(ret!=0)
				{
					printf("Error parsing file");
				}
			}
			//cout << "file parsed" <<endl;		
		//printf("Parsing ObjectDictionary5 Done");
		//objDict->printall();
		 xmlCleanupParser();
		/*
		* this is to debug memory for regression tests
		*/
		xmlMemoryDump();

	}
/**************************************************************************************************
	* Function Name: GetNodeIDbyNodePos
    * Description: Fills the NodeID and NodeType for the NodeCount
	* Return value: ocfmRetCode
/****************************************************************************************************/

ocfmRetCode GetNodeAttributesbyNodePos(
	int NodePos,
	int* Out_NodeID,
	char* Out_NodeName)
{
	//cout<< "Inside GetNodeIDbyNodeCount" << endl;
	ocfmRetCode ErrStruct;
	
	int tmp_NodeCount;
	try
	{
		// We do not have support for multiple MNs in this version.
		GetNodeCount( 240, &tmp_NodeCount);
		
		if(NodePos > tmp_NodeCount)
		{
			ocfmException* objException = new ocfmException;
			objException->ocfm_Excpetion(OCFM_ERR_INVALID_NODEPOS);		
			throw objException;
		}
		else if(tmp_NodeCount == 0)
		{
			ocfmException* objException = new ocfmException;
			objException->ocfm_Excpetion(OCFM_ERR_NO_NODES_FOUND);		
			throw objException;
		}
		CNode objNode;		
		CNodeCollection *objNodeCollection;
		CIndexCollection *objIndexCollection;

		objNodeCollection= CNodeCollection::getNodeColObjectPointer();
		
		objIndexCollection = objNode.getIndexCollection();

		objNode = objNodeCollection->getNodebyCollectionIndex(NodePos);
		
		//if(objNode.getNodeType() == NULL)
		//{
		//	cout << "OCFM_ERR_INVALID_NODETYPE" << endl;
		//	ocfmException* objException = new ocfmException;
		//	objException->ocfm_Excpetion(OCFM_ERR_INVALID_NODETYPE);
		//	throw objException;
		//}
		*Out_NodeID = objNode.getNodeId();
		if(objNode.getNodeName() != NULL)
			strcpy(Out_NodeName, objNode.getNodeName());
		else
			Out_NodeName = NULL;
				
		ErrStruct.code = OCFM_ERR_SUCCESS;
		return ErrStruct;
	}
	catch(ocfmException* ex)
	{
		return ex->_ocfmRetCode;
	}
}

/**************************************************************************************************
	* Function Name: GetIndexIDbyIndexPos
    * Description: Fills the IndexID for the IndexCount, given NodeID and the NodeType
	* Return value: ocfmRetCode
/****************************************************************************************************/

ocfmRetCode GetIndexIDbyIndexPos(
	int NodeID, 
	ENodeType NodeType, 
	int IndexPos, 
	char* Out_IndexID)
{	
	ocfmRetCode ErrStruct;
		
	CNode objNode;		
	CNodeCollection *objNodeCollection;
	CIndexCollection *objIndexCollection;
	CIndex* objIndex;
	
	int NodePos;
	try
	{
		bool flag = false;
		ErrStruct = IfNodeExists(NodeID, NodeType, &NodePos, flag);
		if (ErrStruct.code == 0 && flag==true) 
		{		
			//retPos = ErrStruct.returnValue;
		}
		else
		{	
			//cout << "$S\n\nErrStruct.errCode.code:" << ErrStruct.code << "\n\n!!!" << endl;
			// Node Doesn't Exist
			ocfmException* objException = new ocfmException;
			objException->ocfm_Excpetion(OCFM_ERR_INVALID_NODEID);
			throw objException;
		}

		objNodeCollection = CNodeCollection::getNodeColObjectPointer();
		//objNode = objNodeCollection->getNode(NodeType, NodeID);
		objNode = objNodeCollection->getNodebyCollectionIndex(NodePos);
		objIndexCollection = objNode.getIndexCollection();
		
		int tmp_IndexCount = objIndexCollection->getNumberofIndexes();
		
		if(tmp_IndexCount == 0)
		{
			ocfmException* objException = new ocfmException;
			objException->ocfm_Excpetion(OCFM_ERR_NO_INDEX_FOUND);
			throw objException;
		}
		else if(tmp_IndexCount < IndexPos)
		{
			ocfmException* objException = new ocfmException;
			objException->ocfm_Excpetion(OCFM_ERR_INVALID_INDEXPOS);
			throw objException;
		}
		
		objIndex = objIndexCollection->getIndex(IndexPos);
		
		if(objIndex->getIndexValue() != NULL)
			strcpy(Out_IndexID, (char *) objIndex->getIndexValue());
		
		//cout << "$SOut_IndexID:" << Out_IndexID << endl;

		ErrStruct.code = OCFM_ERR_SUCCESS;
		return ErrStruct;
	}
	catch(ocfmException* ex)
	{
		return ex->_ocfmRetCode;
	}
}

/**************************************************************************************************
	* Function Name: GetIndexIDbyPositions
    * Description: Fills the IndexID for the IndexCount,
	* Return value: ocfmRetCode
/****************************************************************************************************/

ocfmRetCode GetIndexIDbyPositions(
	int NodePos, 
	int IndexPos, 
	char* Out_IndexID)
{
	ocfmRetCode ErrStruct;
	CNode objNode;		
	CNodeCollection *objNodeCollection;
	CIndexCollection *objIndexCollection;
	CIndex* objIndex;
		
	try
	{
		objNodeCollection= CNodeCollection::getNodeColObjectPointer();

		int tmp_NodeCount = objNodeCollection->getNumberOfNodes();
		
		if(tmp_NodeCount == 0)
		{
			ocfmException* objException = new ocfmException;			
			objException->ocfm_Excpetion(OCFM_ERR_NO_NODES_FOUND);
			throw objException;
		}
		else if(tmp_NodeCount < NodePos)
		{
			ocfmException* objException = new ocfmException;
			objException->ocfm_Excpetion(OCFM_ERR_INVALID_NODEPOS);
			throw objException;
		}
		
		objNode = objNodeCollection->getNodebyCollectionIndex(NodePos);
		objIndexCollection = objNode.getIndexCollection();
		
		int tmp_IndexCount = objIndexCollection->getNumberofIndexes();
		
		if(tmp_IndexCount == 0)
		{
			ocfmException* objException = new ocfmException;
			objException->ocfm_Excpetion(OCFM_ERR_NO_INDEX_FOUND);
			throw objException;
		}
		else if(tmp_IndexCount < IndexPos)
		{
			ocfmException* objException = new ocfmException;
			objException->ocfm_Excpetion(OCFM_ERR_INVALID_INDEXPOS);
			throw objException;
		}
		
		objIndex = objIndexCollection->getIndex(IndexPos);
		
		if(objIndex->getIndexValue() != NULL)
			strcpy(Out_IndexID, (char *) objIndex->getIndexValue());
		
		//cout << "Out_IndexID:" << Out_IndexID << endl;

		ErrStruct.code = OCFM_ERR_SUCCESS;
		return ErrStruct;
	}
	catch(ocfmException* ex)
	{
		return ex->_ocfmRetCode;
	}
	
}

/**************************************************************************************************
	* Function Name: GetSubIndexIDbySubIndexPos
    * Description: Fills the SubIndexID for the SubIndexPos, given NodeID, NodeType and the IndexID
	* Return value: ocfmRetCode
/****************************************************************************************************/

ocfmRetCode GetSubIndexIDbySubIndexPos(
	int NodeID, 
	ENodeType NodeType, 
	char* IndexID, 
	int SubIndexPos, 
	char* Out_SubIndexID)
{
	CNode objNode;		
	CNodeCollection *objNodeCollection;
	CIndexCollection *objIndexCollection;
	CIndex objIndex;
	CIndex* objSubIndex;
	ocfmRetCode ErrStruct;
	
	int IndexPos;
	try
	{
		
		ErrStruct = IfIndexExists(NodeID, NodeType, IndexID, &IndexPos);
		if (ErrStruct.code == 0)
		{				
			//IndexPos = ErrStruct.returnValue;
		}
		else
		{	
			ocfmException* objException = new ocfmException;
			objException->ocfm_Excpetion(OCFM_ERR_INDEXID_NOT_FOUND);
			throw objException;
		}
		
		//objIndex.setNodeID(objNode.getNodeId());
		objNodeCollection= CNodeCollection::getNodeColObjectPointer();
		objNode = objNodeCollection->getNode(NodeType, NodeID);

		objIndexCollection = objNode.getIndexCollection();
		objSubIndex = objIndexCollection->getIndex(IndexPos);
			
		int tmp_SubIndexCount = objSubIndex->getNumberofSubIndexes();
		if(tmp_SubIndexCount == 0)
		{		
			ocfmException* objException = new ocfmException;			
			objException->ocfm_Excpetion(OCFM_ERR_NO_SUBINDEXS_FOUND);
			throw objException;			
		}
		else if(tmp_SubIndexCount < SubIndexPos)
		{
			ocfmException* objException = new ocfmException;
			objException->ocfm_Excpetion(OCFM_ERR_INVALID_SUBINDEXPOS);
			throw objException;	
		}
	
		CSubIndex* objSubIndexPtr;
		objSubIndexPtr = objSubIndex->getSubIndex(SubIndexPos);
		
		if(objSubIndexPtr->getIndexValue() != NULL)
			strcpy(Out_SubIndexID, (char *) objSubIndexPtr->getIndexValue());	
		
		ErrStruct.code = OCFM_ERR_SUCCESS;
		return ErrStruct;
	}
	catch(ocfmException* ex)
	{
		return ex->_ocfmRetCode;
	}
}

/**************************************************************************************************
	* Function Name: GetSubIndexIDbyPositions
    * Description: Fills the SubIndexID for the SubIndexPos, given NodePos and the IndexPos
	* Return value: ocfmRetCode
/****************************************************************************************************/

ocfmRetCode GetSubIndexIDbyPositions(
	int NodePos, 
	int IndexPos, 
	int SubIndexPos, 
	char* Out_SubIndexID)

{
	CNode objNode;		
	CNodeCollection *objNodeCollection;
	CIndexCollection *objIndexCollection;
	CIndex objIndex;
	CIndex* objSubIndex;
	ocfmRetCode ErrStruct;

	try
	{		
		objNodeCollection= CNodeCollection::getNodeColObjectPointer();

		int tmp_NodeCount = objNodeCollection->getNumberOfNodes();
		
		if(tmp_NodeCount == 0)
		{
			ocfmException* objException = new ocfmException;			
			objException->ocfm_Excpetion(OCFM_ERR_NO_NODES_FOUND);
			throw objException;
		}
		else if(tmp_NodeCount < NodePos)
		{
			ocfmException* objException = new ocfmException;
			objException->ocfm_Excpetion(OCFM_ERR_INVALID_NODEPOS);
			throw objException;
		}
		
		objNode = objNodeCollection->getNodebyCollectionIndex(NodePos);		
		objIndexCollection = objNode.getIndexCollection();
		
		int tmp_IndexCount = objIndexCollection->getNumberofIndexes();		
		if(tmp_IndexCount == 0)
		{
			ocfmException* objException = new ocfmException;
			objException->ocfm_Excpetion(OCFM_ERR_NO_INDEX_FOUND);
			throw objException;
		}
		else if(tmp_IndexCount < IndexPos)
		{
			ocfmException* objException = new ocfmException;
			objException->ocfm_Excpetion(OCFM_ERR_INVALID_INDEXPOS);
			throw objException;
		}
		
		objSubIndex = objIndexCollection->getIndex(IndexPos);
			
		int tmp_SubIndexCount = objSubIndex->getNumberofSubIndexes();
		if(tmp_SubIndexCount == 0)
		{		
			ocfmException* objException = new ocfmException;			
			objException->ocfm_Excpetion(OCFM_ERR_NO_SUBINDEXS_FOUND);
			throw objException;			
		}
		else if(tmp_SubIndexCount < SubIndexPos)
		{
			ocfmException* objException = new ocfmException;
			objException->ocfm_Excpetion(OCFM_ERR_INVALID_SUBINDEXPOS);
			throw objException;	
		}
	
		CSubIndex* objSubIndexPtr;
		objSubIndexPtr = objSubIndex->getSubIndex(SubIndexPos);
		
		if(objSubIndexPtr->getIndexValue() != NULL)
			strcpy(Out_SubIndexID, (char *) objSubIndexPtr->getIndexValue());	
		
		ErrStruct.code = OCFM_ERR_SUCCESS;
		return ErrStruct;
	}
	catch(ocfmException* ex)
	{
		return ex->_ocfmRetCode;
	}
}

/**************************************************************************************************
	* Function Name: DeleteNodeObjDict
    * Description: Deletes the Node Object Dictinary.
	* Return value: ocfmRetCode
/****************************************************************************************************/

ocfmRetCode DeleteNodeObjDict(
	int NodeID, ENodeType NodeType)
{
	ocfmRetCode ErrStruct;
	int NodePos;
	//cout << "\n$SInside DeleteMNObjDict..\n" << endl;
	try
	{	
		bool flag = false;
		ErrStruct = IfNodeExists(NodeID, NodeType, &NodePos, flag);
		
		if(ErrStruct.code == 0 && flag ==true)
		{
		}
		else
		{
			ocfmException* objException = new ocfmException;
			objException->ocfm_Excpetion(OCFM_ERR_INVALID_NODEID);
			throw objException;
		}
		
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

		// Delete IndexCollection
		objIndexCollection->DeleteIndexCollection();
		// Delete DataTypeCollection
		objDataTypeCollection->DeleteDataTypeCollection();
		//cout << "\n\n$SDelete MN OBD Success" << endl;
		ErrStruct.code = OCFM_ERR_SUCCESS;
		return ErrStruct;
	}
	catch(ocfmException* ex)
	{
		return ex->_ocfmRetCode;
	}
}

/**************************************************************************************************
	* Function Name: SaveProject
    * Description: Saves all the Nodes into the Project location
	* Return value: ocfmRetCode
/****************************************************************************************************/

ocfmRetCode SaveProject(char* ProjectPath, char* ProjectName)
{
	CNode objNode;		
	CNodeCollection *objNodeCollection = NULL;	
	ocfmRetCode ErrStruct;
	
	char* path;
	path = new char[(strlen(ProjectPath) + strlen(ProjectName) + 5)];
	
	try
	{	
		#if defined(_WIN32) && defined(_MSC_VER)
		{
			sprintf(path, "%s\%s", ProjectPath, ProjectName);				
			mkdir(path);
		}
		#else
		{
			sprintf(path, "%s/%s", ProjectPath, ProjectName);				
			mkdir(path, S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH);
		}
		#endif
		
		saveProjectXML(ProjectPath, ProjectName);
		
		objNodeCollection = CNodeCollection::getNodeColObjectPointer();	
		if(objNodeCollection == NULL)
		{
			ocfmException* objException = new ocfmException;
			objException->ocfm_Excpetion(OCFM_ERR_NO_NODES_FOUND);
			throw objException;
		}
		
		if( objNodeCollection->getNumberOfNodes() > 0)
		{
			for(int count = 0; count < objNodeCollection->getNumberOfNodes(); count++)
			{				
				objNode = objNodeCollection->getNodebyCollectionIndex(count);
				
				//char *fileName;	
				//fileName = new char[80];
				char* fileName;
				fileName = new char[(strlen(path) + 4 + 5)];
												
				#if defined(_WIN32) && defined(_MSC_VER)
				{
					//sprintf(path, "%s\%s", ProjectPath, ProjectName);				
					//mkdir(path);
					// Saves the nodes with their nodeId as the name
					sprintf(fileName, "%s\%d.xdc", path, objNode.getNodeId());
				}
				#else
				{
					//sprintf(path, "%s/%s", ProjectPath, ProjectName);				
					//mkdir(path, S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH);
					// Saves the nodes with their nodeId as the name
					sprintf(fileName, "%s/%d.xdc", path, objNode.getNodeId());
				}
				#endif
										
				//cout << "\fileName:" << fileName << endl;
				//cout << "\ngetNodeId-getNodeType:" << objNode.getNodeId() << objNode.getNodeType() << endl;
				SaveNode(fileName, objNode.getNodeId(), objNode.getNodeType());	
				delete [] fileName;			
			}
			ErrStruct.code = OCFM_ERR_SUCCESS;
			return ErrStruct;
		}
		else
		{
			ocfmException* objException = new ocfmException;
			objException->ocfm_Excpetion(OCFM_ERR_NO_NODES_FOUND);
			throw objException;			
		}	
	}
	catch(ocfmException* ex)
	{
		return ex->_ocfmRetCode;
	}
	delete [] path;
}
void GetMNPDOSubIndex(MNPdoVariable var, int& prevSubIndex, CIndex* objIdx,char* MNIndex)
{
			CSubIndex* objSubIndex;
			char* Idx =  new char[2];
		/* Set the MN's PDO subIndex*/
			prevSubIndex =  prevSubIndex + 1;
			Idx = itoa(prevSubIndex, Idx, 16);
			Idx = padLeft(Idx, '0', 2);
			
				#if defined DEBUG	
						cout<< " idx" << Idx << endl;
						cout<< " prevsubindex" << prevSubIndex << endl;
						#endif
			if(CheckIfSubIndexExists(240, MN, MNIndex,Idx))
			{
				objSubIndex = objIdx->getSubIndexbyIndexValue(Idx);
			}
			else
			{
				AddSubIndex(240, MN,  MNIndex,Idx);
				objSubIndex = objIdx->getSubIndexbyIndexValue(Idx);
			}
			//objSubIndex.setIndexValue(Idx);
			//objSubIndex.setNodeID(240);
			//
			///* Set the datatype having datasize = 0008 which is 001B*/			
			//DataType dt;		
			//dt.setName("Unsigned64");
			//
			//dt.DataSize = new char[4];
			//strcpy(dt.DataSize, "0008");
			//
			//dt.DataTypeValue = new char[4];
			//strcpy(dt.DataTypeValue, "001B");
			//objSubIndex.setDataTypeST(dt);																		
			//objSubIndex.setDataType("001B");				
			
			/* Calculate the actual value of MN PDO */
			char* actValue = new char[18];
			strcpy(actValue, var.Value);
			strcat(actValue, "00");
			strcat(actValue, var.SubIndex);
			strcat(actValue, var.Index);
			
			objSubIndex->setActualValue(actValue);
		/*	return *objSubIndex;*/
}
void SetSIdxValue(char* Idx, char* SIdx,
																	 char* value, CIndexCollection * objIdxCol,
																		int NodeId, ENodeType NodeType, bool setDefaultValue)
{
		CIndex *objIndex;
		CSubIndex* objSIdx;
		ocfmRetCode retCode;
		objIndex = objIdxCol->getIndexbyIndexValue(Idx);
					#if defined DEBUG	
						cout << "sidx idx" << Idx << SIdx << endl;;
						
					#endif
		if(CheckIfSubIndexExists(NodeId, NodeType, Idx, SIdx))
		{			
			#if defined DEBUG	
						cout << "subindex fetched" << endl;;
						
					#endif			
			objSIdx = objIndex->getSubIndexbyIndexValue(SIdx);
			if(setDefaultValue)
			objSIdx->setActualValue((char*)objSIdx->getDefaultValue());
			else
			objSIdx->setActualValue(value);
		}
		else
		{
			#if defined DEBUG	
						cout << "call addindex" << endl;
						
					#endif
			retCode = AddSubIndex(NodeId, NodeType, Idx, SIdx);
				#if defined DEBUG	
						cout << "retcode" << retCode.code<<endl;
						cout<< "Index sidx added"<< Idx << SIdx;
					#endif
			objSIdx = objIndex->getSubIndexbyIndexValue(SIdx);
			if(setDefaultValue)
			objSIdx->setActualValue((char*)objSIdx->getDefaultValue());
			else
			objSIdx->setActualValue(value);
		}
}

void AddForEachSIdx(char* Idx,CIndexCollection * objIdxCol, int MNNodeID,char* Value, bool setDefaultValue)
{
		CNodeCollection* objNodeCol;
		CNode objCNNode;
		CIndex *objIndex;
		CSubIndex* objSIdx;
		char* SIdx =  new char[2];
		char* IndexNo = new char[2];
		ocfmRetCode retCode;
		
		retCode.code = OCFM_ERR_SUCCESS;
		objIndex = objIdxCol->getIndexbyIndexValue(Idx);
		objNodeCol = CNodeCollection::getNodeColObjectPointer();
		
		// to doStack wasnt booting up with sub index 00 for 1C09, 1F26
		if((strcmp("1C09",Idx)==0) || (strcmp("1F26", Idx) == 0)
																															|| (strcmp("1F8B",Idx)  == 0)
																															|| (strcmp("1F8D",Idx)		==	0)
																															|| (strcmp("1F27",Idx)		==	0)
																															|| (strcmp("1F84",Idx)		==	0)
																														 || (strcmp("1F92",Idx)		==	0))
	{
		strcpy(SIdx, "00");
		DeleteSubIndex(MNNodeID, MN, Idx, SIdx);
	}
	else
	{
			if(objNodeCol->getNumberOfNodes() !=0)
			strcpy(SIdx, "00");
			IndexNo = itoa(objNodeCol->getCNNodesCount(), IndexNo, 16);
			IndexNo = padLeft(IndexNo, '0', 2);
			
			SetSIdxValue(Idx, SIdx, IndexNo , objIdxCol, MNNodeID, MN, false);
		}
				
		for(int i = 0; i< objNodeCol->getNumberOfNodes(); i++)
		{
			objCNNode = objNodeCol->getNodebyCollectionIndex(i);			
			if(objCNNode.getNodeType() == CN)
			{
				SIdx = itoa(objCNNode.getNodeId(), SIdx, 16);
				SIdx = padLeft(SIdx, '0', 2);	
				
				
				if(strcmp("1F84",Idx)==0)
				{
					CIndex* objCNIndex;
					CSubIndex objCNSIdx;
					CIndexCollection* objCNIdxCol;
					objCNIdxCol = objCNNode.getIndexCollection();
					char* MIndex = new char[4];
					strcpy(MIndex, "1000");
					objCNIndex = objCNIdxCol->getIndexbyIndexValue(MIndex);
					
					if(objCNIndex->getActualValue() != NULL)
					{
						SetSIdxValue(Idx,SIdx,(char*)objCNIndex->getActualValue(), objIdxCol, MNNodeID, MN, false);
					}
					else
					{
						SetSIdxValue(Idx,SIdx,(char*)objCNIndex->getActualValue(), objIdxCol, MNNodeID, MN, true);
					}
										
				}
				else
				{
					SetSIdxValue(Idx,SIdx,Value, objIdxCol, MNNodeID, MN, setDefaultValue);
				}
			/*	if(CheckIfSubIndexExists(MNNodeID, MN, Idx, SIdx))
				{						
					objSIdx = objIndex->getSubIndexbyIndexValue(SIdx);			
					objSIdx->setActualValue(Value);		
				}
				else
				{
					retCode = AddSubIndex(MNNodeID, MN, Idx, SIdx);						
					if(retCode.code != 0)
					{
						ocfmException ex;
						ex.ocfm_Excpetion(retCode.code);
						throw ex;
					}
					objSIdx = objIndex->getSubIndexbyIndexValue(SIdx);			
					if(strcmp("1F84",Idx)==0)
					{
						CIndex objCNIndex;
						CSubIndex objCNSIdx;
						CIndexCollection objCNIdxCol;
						objCNIdxCol = objCNNode.getIndexCollection();
						objCNIndex = objCNIdxCol.getIndexbyIndexValue(Idx);
						if(objCNIndex.getActualValue() != NULL)
						{
							SetSIdxValue(Idx,SIdx,objCNIndex.getActualValue(), objIdxCol, MNNodeID);
						}
						
					}
					else	
					objSIdx->setActualValue(Value);*/
				
				}
		
	}
}
ocfmRetCode AddOtherMNIndexes(CNode *objNode)
{
			ocfmRetCode retCode;
			char* MNIndex = new char[4];		
			char* Sidx =  new char[2];
			CIndex* objIndex;
			CSubIndex* objSIdx;
			CIndexCollection* objIdxCol;
			
			try
			{
				objIdxCol = objNode->getIndexCollection();
					
				/* Add 1006*/
					strcpy(MNIndex, "1006");
							#if defined DEBUG	
						cout << "string copied" << endl;
					
					#endif
					retCode = AddIndex(240, MN, MNIndex);
						#if defined DEBUG	
						cout << "retcode" << retCode.code<<endl;
						cout<< "1006 added"<<endl;
					#endif
					
					/* Set 5ms value*/
					SetIndexAttributes(240, MN, MNIndex, "5000","NMT_CycleLen_U32");
					
						/* Add 1020*/
					strcpy(MNIndex, "1020");
					retCode = AddIndex(240, MN, MNIndex);
					
							/* Add 1020*/
					strcpy(MNIndex, "1300");
					retCode = AddIndex(240, MN, MNIndex);
					/* $:To do by M hard coded*/
					SetIndexAttributes(240, MN, MNIndex, "5000","SDO_SequLayerTimeout_U32");
					
					
					/* Add 1C02*/
					strcpy(MNIndex, "1C02");
					retCode = AddIndex(240, MN, MNIndex);
					if(retCode.code == OCFM_ERR_SUCCESS)
					{				
						objIndex = objIdxCol->getIndexbyIndexValue(MNIndex);
						
						/* Set subindex value 40 or 0000028 */
						strcpy(Sidx, "00");
						SetSIdxValue(MNIndex, Sidx, "3", objIdxCol, objNode->getNodeId(), MN, false);
								#if defined DEBUG	
						cout<< "1c02 subidex added"<<endl;
					#endif
						
						strcpy(Sidx, "01");
						SetSIdxValue(MNIndex, Sidx, "40", objIdxCol, objNode->getNodeId(), MN,  false);
								#if defined DEBUG	
						cout<< "1c02 subidex 01 added"<<endl;
					#endif
						strcpy(Sidx, "02");
						SetSIdxValue(MNIndex, Sidx, "40", objIdxCol, objNode->getNodeId(), MN, false);
						
						strcpy(Sidx, "03");
						SetSIdxValue(MNIndex, Sidx, "40", objIdxCol, objNode->getNodeId(), MN, false);

					}
					
					/* Add 1C09*/
					strcpy(MNIndex, "1C09");
					retCode = AddIndex(240, MN, MNIndex);
					if(retCode.code == OCFM_ERR_SUCCESS)
					{				
						objIndex = objIdxCol->getIndexbyIndexValue(MNIndex);
						
						AddForEachSIdx(MNIndex, objIdxCol, objNode->getNodeId(), "40",false);					
					
					}
					
							/* Add 1F26*/
					strcpy(MNIndex, "1F26");
					retCode = AddIndex(240, MN, MNIndex);
					if(retCode.code == OCFM_ERR_SUCCESS)
					{				
						objIndex = objIdxCol->getIndexbyIndexValue(MNIndex);
						char* Val = new char[8];
						ConfigDate = getConfigDate();
						Val = itoa(ConfigDate,Val, 10);
						//hexVal = padLeft(hexVal, '0' , 8);
						AddForEachSIdx(MNIndex, objIdxCol, objNode->getNodeId(), Val, false);		
											
										//AddForEachSIdx(MNIndex, objIdxCol, objNode->getNodeId(), hexVal, false);		
	
					}
					
							#if defined DEBUG	
						cout<< "1F26 subidex added"<<endl;
					#endif
							/* Add 1F27*/
					strcpy(MNIndex, "1F27");
					retCode = AddIndex(240, MN, MNIndex);
					if(retCode.code == OCFM_ERR_SUCCESS)
					{				
						objIndex = objIdxCol->getIndexbyIndexValue(MNIndex);		
						char* Val = new char[8];
						ConfigTime = getConfigTime();
						Val = itoa(ConfigTime,Val, 10);
						//Val = padLeft(hexVal, '0' , 8);

						AddForEachSIdx(MNIndex, objIdxCol, objNode->getNodeId(), Val, false);														
					
					//AddForEachSIdx(MNIndex, objIdxCol, objNode->getNodeId(), getConfigTime(), false);		
					}
					
					
					/* Add 1F84*/
					strcpy(MNIndex, "1F84");
					retCode = AddIndex(240, MN, MNIndex);
					if(retCode.code == OCFM_ERR_SUCCESS)
					{				
						objIndex = objIdxCol->getIndexbyIndexValue(MNIndex);
									
						AddForEachSIdx(MNIndex, objIdxCol, objNode->getNodeId(), "", true);			
			
					}
					
							/* Add 1F89*/
					strcpy(MNIndex, "1F89");
					retCode = AddIndex(240, MN, MNIndex);
					if(retCode.code == OCFM_ERR_SUCCESS)
					{
					/*	strcpy(Sidx, "00");
						SetSIdxValue(MNIndex, Sidx, "", objIdxCol, objNode->getNodeId(),true);
										
						strcpy(Sidx, "01");
						SetSIdxValue(MNIndex, Sidx, "", objIdxCol, objNode->getNodeId(),true);
				*/	
						strcpy(Sidx, "02");
						SetSIdxValue(MNIndex, Sidx, "", objIdxCol, objNode->getNodeId(), MN, true);
				
					/*		strcpy(Sidx, "03");
						SetSIdxValue(MNIndex, Sidx, "", objIdxCol, objNode->getNodeId(), true);
						
						strcpy(Sidx, "04");
						SetSIdxValue(MNIndex, Sidx, "", objIdxCol, objNode->getNodeId(), true);
						
						strcpy(Sidx, "05");
						SetSIdxValue(MNIndex, Sidx, "", objIdxCol, objNode->getNodeId(), true);
						
						strcpy(Sidx, "06");
						SetSIdxValue(MNIndex, Sidx, "", objIdxCol, objNode->getNodeId(), true);
						
						strcpy(Sidx, "07");
						SetSIdxValue(MNIndex, Sidx, "", objIdxCol, objNode->getNodeId(), true);
						
						strcpy(Sidx, "08");
						SetSIdxValue(MNIndex, Sidx, "", objIdxCol, objNode->getNodeId(), true);
						
						strcpy(Sidx, "09");
						SetSIdxValue(MNIndex, Sidx, "", objIdxCol, objNode->getNodeId(), true);
			*/
					}
					
					/* Add 1F8A*/
					strcpy(MNIndex, "1F8A");
					retCode = AddIndex(240, MN, MNIndex);
					if(retCode.code == OCFM_ERR_SUCCESS)
					{				
						strcpy(Sidx, "00");
						SetSIdxValue(MNIndex, Sidx, "", objIdxCol, objNode->getNodeId(), MN, true);
						
						//$:To do By M as subindex 01 shud be equal to 02, need to find the reason 
						strcpy(Sidx, "01");
						SetSIdxValue(MNIndex, Sidx, "100000", objIdxCol, objNode->getNodeId(), MN, false);
					
						strcpy(Sidx, "02");
						SetSIdxValue(MNIndex, Sidx, "", objIdxCol, objNode->getNodeId(), MN, true);
						
					/*	strcpy(Sidx, "03");
						SetSIdxValue(MNIndex, Sidx, "", objIdxCol, objNode->getNodeId(), true);
						
						strcpy(Sidx, "04");
						SetSIdxValue(MNIndex, Sidx, "", objIdxCol, objNode->getNodeId(), true);
						
						strcpy(Sidx, "05");
						SetSIdxValue(MNIndex, Sidx, "", objIdxCol, objNode->getNodeId(), true);
						
						strcpy(Sidx, "06");
						SetSIdxValue(MNIndex, Sidx, "", objIdxCol, objNode->getNodeId(), true);
						
						strcpy(Sidx, "07");
						SetSIdxValue(MNIndex, Sidx, "", objIdxCol, objNode->getNodeId(), true);
						
						strcpy(Sidx, "08");
						SetSIdxValue(MNIndex, Sidx, "", objIdxCol, objNode->getNodeId(), true);
						
						strcpy(Sidx, "09");
						SetSIdxValue(MNIndex, Sidx, "", objIdxCol, objNode->getNodeId(), true);*/
					
						}
					
								/* Add 1F8B*/
					strcpy(MNIndex, "1F8B");
					retCode = AddIndex(240, MN, MNIndex);
					if(retCode.code == OCFM_ERR_SUCCESS)
					{				
						objIndex = objIdxCol->getIndexbyIndexValue(MNIndex);
						
						AddForEachSIdx(MNIndex, objIdxCol, objNode->getNodeId(), "1490", false);			
											
					
					}
							/* Add 1F8D*/
					strcpy(MNIndex, "1F8D");
					retCode = AddIndex(240, MN, MNIndex);
					if(retCode.code == OCFM_ERR_SUCCESS)
					{				
						objIndex = objIdxCol->getIndexbyIndexValue(MNIndex);
						
						AddForEachSIdx(MNIndex, objIdxCol, objNode->getNodeId(), "1490", false);			
											
					
					}
							/* Add 1F92*/
					strcpy(MNIndex, "1F92");
					retCode = AddIndex(240, MN, MNIndex);
					if(retCode.code == OCFM_ERR_SUCCESS)
					{				
						objIndex = objIdxCol->getIndexbyIndexValue(MNIndex);
						
						AddForEachSIdx(MNIndex, objIdxCol, objNode->getNodeId(), "2000000",false);			
											
					
					}
					
							/* Add 1F98*/
					strcpy(MNIndex, "1F98");
					
					retCode = AddIndex(240, MN, MNIndex);
					if(retCode.code == OCFM_ERR_SUCCESS)
					{				
					
						strcpy(Sidx, "05");
						SetSIdxValue(MNIndex, Sidx, C_DLL_ISOCHR_MAX_PAYL, objIdxCol, objNode->getNodeId(), MN, false);
						
						/*strcpy(Sidx, "00");
						SetSIdxValue(MNIndex, Sidx, "", objIdxCol, objNode->getNodeId(),true);
										
						strcpy(Sidx, "01");
						SetSIdxValue(MNIndex, Sidx, C_DLL_ISOCHR_MAX_PAYL, objIdxCol, objNode->getNodeId(), false);
					
						strcpy(Sidx, "02");
						SetSIdxValue(MNIndex, Sidx, C_DLL_ISOCHR_MAX_PAYL, objIdxCol, objNode->getNodeId(), false);
						
						
						strcpy(Sidx, "07");
						SetSIdxValue(MNIndex, Sidx, "", objIdxCol, objNode->getNodeId(), true);
						
						strcpy(Sidx, "08");
						SetSIdxValue(MNIndex, Sidx, C_DLL_MIN_ASYNC_MTU, objIdxCol, objNode->getNodeId(), false);
						
						strcpy(Sidx, "09");
						SetSIdxValue(MNIndex, Sidx, "", objIdxCol, objNode->getNodeId(), true);*/
					}
					
					return retCode;
		}
		catch(ocfmException & ex)
		{
			return ex._ocfmRetCode;
		}

}
/**************************************************************************************************
	* Function Name: GenerateMNOBD
   * Description: 
/****************************************************************************************************/
ocfmRetCode GenerateMNOBD()
	{
		CNode objNode;		
		CNode *objMNNode;
		CNodeCollection *objNodeCollection = NULL;
		CIndexCollection * objMNIndexCol;
		CSubIndex * objSubIdex;
		char* MNIndex = new char[4];								
		char* Idx =  new char[2];
		ocfmRetCode retCode;
		
		ocfmException ex;
		int prevSubIndex = 0 ;
		try
		{		
			cout << "Inside GenerateMN's OBD" << endl;
			objNodeCollection = CNodeCollection::getNodeColObjectPointer();	
			objMNNode  = objNodeCollection->getNodePtr(MN, 240);
			if(objMNNode == NULL)
			{			
				ex.ocfm_Excpetion(OCFM_ERR_MN_NODE_DOESNT_EXIST);
				throw ex;
			}
			else
			{
				/*Process PDO Nodes*/
				retCode = ProcessPDONodes();
				#if defined DEBUG	
						cout<< "Nodes Processed"<<endl;
					#endif
				if(retCode.code != OCFM_ERR_SUCCESS)
				return retCode;
				/* Delete the MN's old object dictionary*/
				DeleteNodeObjDict(240, MN);		
						#if defined DEBUG	
						cout<< "MN Node Object dictionary deleted"<<endl;
					#endif
				/* Add other Indexes than PDO*/
					AddOtherMNIndexes(objMNNode);
					#if defined DEBUG	
						cout<< "Index added"<<endl;
					#endif
						if(retCode.code != OCFM_ERR_SUCCESS)
						{
							ex.ocfm_Excpetion(retCode.code);
							throw ex;
						}
					#if defined DEBUG	
						cout<< "Deleted"<<endl;
					#endif
			}
			for(int i =0; i<objNodeCollection->getNumberOfNodes(); i++)
			{
				objNode = objNodeCollection->getNodebyCollectionIndex(i);			
				if(objNode.getNodeType() == CN)
				{
					prevSubIndex = 0;
					objMNIndexCol =  objMNNode->getIndexCollection();
									
					if(objNode.MNPDOOUTVarCollection.Count()!=0)
					{
												
						/* Create PDO_TxCommParam_XXh_REC 1800 INdex*/
						CIndex* objIndex;	
						strcpy(MNIndex, "18");
						Idx = itoa((objNode.getNodeId()-1), Idx, 16);
						Idx = padLeft(Idx, '0', 2);
						MNIndex =strcat(MNIndex, Idx);
					
						retCode = AddIndex(240, MN, MNIndex);
								#if defined DEBUG	
						cout<< "Index added"<<endl;
					#endif
						if(retCode.code != OCFM_ERR_SUCCESS)
						{
							ex.ocfm_Excpetion(retCode.code);
							throw ex;
						}
						
						Idx = itoa(objNode.getNodeId(), Idx, 10);
						char* Sidx =  new char[2];
						strcpy(Sidx, "01");
						SetSubIndexAttributes(240, MN, MNIndex, Sidx, Idx,"NodeID_U8");
									
						strcpy(MNIndex, "1A");
						Idx = itoa((objNode.getNodeId()-1), Idx, 16);
						Idx = padLeft(Idx, '0', 2);
						MNIndex =strcat(MNIndex, Idx);
						/* Set the MN's PDO Index*/
						retCode = AddIndex(240, MN, MNIndex);
						if(retCode.code != OCFM_ERR_SUCCESS)
						{
							ex.ocfm_Excpetion(retCode.code);
							throw ex;
						}
						//objIndex.setIndexValue(MNIndex);			
						//objIndex.setNodeID(240);
					
						#if defined DEBUG	
						cout<< " MN Index value" << MNIndex << endl;
						cout<< " Out var count" << objNode.MNPDOOUTVarCollection.Count() << endl;
						#endif
						
						objIndex = objMNIndexCol->getIndexbyIndexValue(MNIndex);
						for(int i=0; i<objNode.MNPDOOUTVarCollection.Count() ; i++)
						{
							MNPdoVariable var;
							var = objNode.MNPDOOUTVarCollection[i];	
							#if defined DEBUG	
							cout << "VAR" << var.Value << endl;	
							#endif
							
							GetMNPDOSubIndex(var, prevSubIndex, objIndex, MNIndex);
						/*	objIndex->addSubIndex(objSubIdex);
						*/		#if defined DEBUG	
							//cout << "SubIndex value" << objSubIdex->getIndexValue() << endl;	
							#endif
						}
						char* actval = new char[4];
						actval = itoa(objNode.MNPDOOUTVarCollection.Count(), actval, 16);
						actval = padLeft(actval, '0', 4);
						objSubIdex = objIndex->getSubIndexbyIndexValue("00");
						objSubIdex->setActualValue(actval);
						//objSubIdex.setIndexValue("00");
					//	objSubIdex.setActualValue(actval);
						
						/*	DataType dt;		
						dt.setName("Unsigned32");
			
					dt.DataSize = new char[4];
					strcpy(dt.DataSize, "0001");
					
					dt.DataTypeValue = new char[4];
					strcpy(dt.DataTypeValue, "0005");
					objSubIdex.setDataTypeST(dt);					*/													

						
						//objIndex->addSubIndex(objSubIdex);
						/*objMNIndexCol->addIndex(objIndex);*/
					}
											
				if(objNode.MNPDOINVarCollection.Count()!=0)
					{
												
						/* Create PDO_TxCommParam_XXh_REC 1800 INdex*/
						strcpy(MNIndex, "14");
						Idx = itoa((objNode.getNodeId()-1), Idx, 16);
						Idx = padLeft(Idx, '0', 2);
						MNIndex =strcat(MNIndex, Idx);
					
					
						retCode = AddIndex(240, MN, MNIndex);
						prevSubIndex = 0;
						if(retCode.code != OCFM_ERR_SUCCESS)
						{
							ex.ocfm_Excpetion(retCode.code);
							throw ex;
						}
						
						Idx = itoa(objNode.getNodeId(), Idx, 10);
						char* Sidx =  new char[2];
						strcpy(Sidx, "01");
						SetSubIndexAttributes(240, MN, MNIndex, Sidx, Idx,"NodeID_U8");
				
						CIndex* objIndex;				
						strcpy(MNIndex, "16");
						Idx = itoa((objNode.getNodeId()-1), Idx, 16);
						Idx = padLeft(Idx, '0', 2);
						MNIndex =strcat(MNIndex, Idx);
						/* Set the MN's PDO Index*/
						retCode = AddIndex(240, MN, MNIndex);
						if(retCode.code != OCFM_ERR_SUCCESS)
						{
							ex.ocfm_Excpetion(retCode.code);
							throw ex;
						}
						//objIndex.setIndexValue(MNIndex);			
						//objIndex.setNodeID(240);
					
						#if defined DEBUG	
						cout<< " MN Index value" << MNIndex << endl;
						cout<< " IN var count" << objNode.MNPDOINVarCollection.Count() << endl;
						#endif
						
						objIndex = objMNIndexCol->getIndexbyIndexValue(MNIndex);
						for(int i=0; i<objNode.MNPDOINVarCollection.Count() ; i++)
						{
							MNPdoVariable var;
							var = objNode.MNPDOINVarCollection[i];	
							#if defined DEBUG	
							cout << "VAR" << var.Value << endl;	
							#endif
							
							GetMNPDOSubIndex(var, prevSubIndex, objIndex, MNIndex);
						/*	objIndex->addSubIndex(objSubIdex);
						*/		#if defined DEBUG	
						//	cout << "SubIndex value" << objSubIdex->getIndexValue() << endl;	
							#endif
						}
						char* actval = new char[4];
						actval = itoa(objNode.MNPDOINVarCollection.Count(), actval, 16);
						actval = padLeft(actval, '0', 4);
						objSubIdex = objIndex->getSubIndexbyIndexValue("00");
						objSubIdex->setActualValue(actval);
						//objSubIdex.setIndexValue("00");
					//	objSubIdex.setActualValue(actval);
						
						/*	DataType dt;		
						dt.setName("Unsigned32");
			
					dt.DataSize = new char[4];
					strcpy(dt.DataSize, "0001");
					
					dt.DataTypeValue = new char[4];
					strcpy(dt.DataTypeValue, "0005");
					objSubIdex.setDataTypeST(dt);					*/													

						
						//objIndex->addSubIndex(objSubIdex);
						/*objMNIndexCol->addIndex(objIndex);*/
					}
				
			}
		}
	}
	catch(ocfmException& ex)
	{
		return ex._ocfmRetCode;
	}
	/* Testing Code*/
				objNodeCollection = CNodeCollection::getNodeColObjectPointer();	
			objMNNode  = objNodeCollection->getNodePtr(MN, 240);
		CIndexCollection * objIndexCol;
			objIndexCol = objMNNode->getIndexCollection();
			for(int i = 0; i< objIndexCol->getNumberofIndexes(); i++)
			{
				CIndex* objIndex;
				objIndex = objIndexCol->getIndex(i);
				printf("\n**************************");
				printf("\n Index value :%s", objIndex->getIndexValue());
				for(int y = 0; y<objIndex->getNumberofSubIndexes(); y++)
				{
					CSubIndex* objSubIndex;
					DataType dt;
					objSubIndex = objIndex->getSubIndex(y);
					dt = objSubIndex->getDataType();
					printf("\n SubIndex: %s \n DataType = %s \n Actual Value = %s",objSubIndex->getIndexValue(),dt.DataSize,
																																																																				objSubIndex->getActualValue());
				}
			}

	retCode.code = OCFM_ERR_SUCCESS;
	return retCode;
}


void checkIfOffsetUsed(int& Offset, int NodeID, EPDOType pdoType)
{
	CNodeCollection* objNodeCol;
	objNodeCol =  CNodeCollection::getNodeColObjectPointer();
	CNode objNode;
	objNode = objNodeCol->getNode(CN, NodeID);
	if(pdoType == PDO_RPDO)
	{
		for(int i =0 ;i< objNode.MNPDOOUTVarCollection.Count() ;i++)
		{
			if(objNode.MNPDOOUTVarCollection[i].Offset == Offset)
				Offset = Offset + objNode.MNPDOOUTVarCollection[i].DataSize/8;
		}
	}
	else	if(pdoType == PDO_TPDO)
	{
		for(int i =0 ;i< objNode.MNPDOINVarCollection.Count() ;i++)
		{
			if(objNode.MNPDOINVarCollection[i].Offset == Offset)
				Offset = Offset + objNode.MNPDOINVarCollection[i].DataSize/8;
		}
	}
	
}
int ComputeOUTOffset(int NodeID, int dataSize, EPDOType pdoType)
{
	//int arrOfOffsets[4][2] = {{-1, -1}, {-1, -1},{-1, -1},{-1,-1}};			/* Contain prev and current offsets of size 1Bytes, 2 bytes, 4 bytes and 8 bytes*/	
	int Offset = 0;
	int newOffset;
							

		switch(dataSize)
		{
			
			/*Unsigned8, Int8*/
			case 8:
						size8OUTOffset.prevOffset = size8OUTOffset.currOffset ;
						Offset = size8OUTOffset.currOffset ;
						size8OUTOffset.currOffset =	size8OUTOffset.currOffset + 1;
						
						/* Set other DataType Offsets*/
						/* if greater no change*/
						if(size16OUTOffset.currOffset >= size8OUTOffset.currOffset)
						{}
						else
						{
								size16OUTOffset.prevOffset = size16OUTOffset.currOffset ;
								size16OUTOffset.currOffset = size16OUTOffset.currOffset + 2;
						}
						
						/* if greater no change*/
						if(size32OUTOffset.currOffset >= size8OUTOffset.currOffset)
						{}
						else
						{
								size32OUTOffset.prevOffset = size32OUTOffset.currOffset ;
								size32OUTOffset.currOffset = size32OUTOffset.currOffset + 4;
						}
						
						/* if greater no change*/
						if(size64INOffset.currOffset >= size8OUTOffset.currOffset)
						{}
						else
						{
								size64OUTOffset.prevOffset = size64OUTOffset.currOffset ;
								size64OUTOffset.currOffset = size64OUTOffset.currOffset + 8;
						}
						break;
						
			/*Unsigned16, Int16*/
			case 16:
						size16OUTOffset.prevOffset = size16OUTOffset.currOffset ;
						Offset = size16OUTOffset.currOffset ;
						size16OUTOffset.currOffset =	size16OUTOffset.currOffset + 2;
					
			
							/* Set other DataType Offsets*/
							
							/* if greater no change*/
						if(size8OUTOffset.currOffset >= size16OUTOffset.currOffset)
						{}
						else
						{
								if((size8OUTOffset.currOffset >= size16OUTOffset.prevOffset) && (size8OUTOffset.currOffset <= size16OUTOffset.currOffset))
								{
									size8OUTOffset.prevOffset = size8OUTOffset.currOffset ;
									size8OUTOffset.currOffset = size16OUTOffset.currOffset;
								}
								else
								{
									size8OUTOffset.prevOffset = size8OUTOffset.currOffset;
									newOffset = size8OUTOffset.currOffset;
									checkIfOffsetUsed(newOffset, NodeID, pdoType);				
									size8OUTOffset.currOffset = newOffset;
								}
						}
						
						/* if greater no change*/
						if(size32OUTOffset.currOffset >= size16OUTOffset.currOffset)
						{}
						else
						{
								size32OUTOffset.prevOffset = size32OUTOffset.currOffset ;
								size32OUTOffset.currOffset = size32OUTOffset.currOffset + 4;
						}
						
						/* if greater no change*/
						if(size64OUTOffset.currOffset >= size16OUTOffset.currOffset)
						{}
						else
						{
								size64OUTOffset.prevOffset = size64OUTOffset.currOffset ;
								size64OUTOffset.currOffset = size64OUTOffset.currOffset + 8;
						}						
					break;
			/*Unsigned32, Int32*/
			case 32:
						size32OUTOffset.prevOffset = size32OUTOffset.currOffset ;
						Offset = size32OUTOffset.currOffset ;
						size32OUTOffset.currOffset = size32OUTOffset.currOffset + 4;
			
							/* Set other DataType Offsets*/
								/* if greater no change*/
						if(size8OUTOffset.currOffset >= size32OUTOffset.currOffset)
						{}
						else
						{
								if((size8OUTOffset.currOffset >= size32OUTOffset.prevOffset) && (size8OUTOffset.currOffset <= size32OUTOffset.currOffset))
								{
									size8OUTOffset.prevOffset = size8OUTOffset.currOffset ;
									size8OUTOffset.currOffset = size32OUTOffset.currOffset;
								}
								else
								{
									size8OUTOffset.prevOffset = size8OUTOffset.currOffset;
										newOffset = size8OUTOffset.currOffset;
									
									checkIfOffsetUsed(newOffset, NodeID, pdoType);
									
									/*while(checkIfOffsetUsed(Offset))
									{
										Offset = Offset + 1;
									}*/
									size8OUTOffset.currOffset = newOffset;
								}
						}
						
							/* if greater no change*/
						if(size16OUTOffset.currOffset >= size32OUTOffset.currOffset)
						{}
						else
						{
								if((size16OUTOffset.currOffset >= size32OUTOffset.prevOffset) && (size16OUTOffset.currOffset < size32OUTOffset.currOffset))
								{
									size16OUTOffset.prevOffset = size16OUTOffset.currOffset ;
									size16OUTOffset.currOffset = size32OUTOffset.currOffset;
								}
								else
								{
									size16OUTOffset.prevOffset = size16OUTOffset.currOffset;
									newOffset = size16OUTOffset.currOffset;
										checkIfOffsetUsed(newOffset, NodeID, pdoType);
								/*	while(checkIfOffsetUsed(Offset))
									{
										Offset = Offset + 1;
									}*/
									size16OUTOffset.currOffset = newOffset;
								
								}
						}
						/* if greater no change*/
						if(size64OUTOffset.currOffset >= size8OUTOffset.currOffset)
						{}
						else
						{
								size64OUTOffset.prevOffset = size64OUTOffset.currOffset ;
								size64OUTOffset.currOffset = size64OUTOffset.currOffset + 8;
						}
						break;
			/*Unsigned64, Int64*/
			case 64:
						size64OUTOffset.prevOffset = size64OUTOffset.currOffset ;
						Offset = size64OUTOffset.currOffset ;
						size64OUTOffset.currOffset = size64OUTOffset.currOffset + 8;
			
			break;
		}
		return Offset;
	}

int ComputeINOffset(int NodeID, int dataSize, EPDOType pdoType)
{
	//int arrOfOffsets[4][2] = {{-1, -1}, {-1, -1},{-1, -1},{-1,-1}};			/* Contain prev and current offsets of size 1Bytes, 2 bytes, 4 bytes and 8 bytes*/	
	int Offset = 0;
	int newOffset;
							

		switch(dataSize)
		{
			
			/*Unsigned8, Int8*/
			case 8:
						size8INOffset.prevOffset = size8INOffset.currOffset ;
						Offset = size8INOffset.currOffset ;
						size8INOffset.currOffset =	size8INOffset.currOffset + 1;
						
						/* Set other DataType Offsets*/
						/* if greater no change*/
						if(size16INOffset.currOffset >= size8INOffset.currOffset)
						{}
						else
						{
								size16INOffset.prevOffset = size16INOffset.currOffset ;
								size16INOffset.currOffset = size16INOffset.currOffset + 2;
						}
						
						/* if greater no change*/
						if(size32OUTOffset.currOffset >= size8INOffset.currOffset)
						{}
						else
						{
								size32INOffset.prevOffset = size32INOffset.currOffset ;
								size32INOffset.currOffset = size32INOffset.currOffset + 4;
						}
						
						/* if greater no change*/
						if(size64INOffset.currOffset >= size8INOffset.currOffset)
						{}
						else
						{
								size64INOffset.prevOffset = size64INOffset.currOffset ;
								size64INOffset.currOffset = size64INOffset.currOffset + 8;
						}
						break;
						
			/*Unsigned16, Int16*/
			case 16:
						size16INOffset.prevOffset = size16INOffset.currOffset ;
						Offset = size16INOffset.currOffset ;
						size16INOffset.currOffset =	size16INOffset.currOffset + 2;
					
			
							/* Set other DataType Offsets*/
							
							/* if greater no change*/
						if(size8INOffset.currOffset >= size16INOffset.currOffset)
						{}
						else
						{
								if((size8INOffset.currOffset >= size16INOffset.prevOffset) && (size8INOffset.currOffset <= size16INOffset.currOffset))
								{
									size8INOffset.prevOffset = size8INOffset.currOffset ;
									size8INOffset.currOffset = size16INOffset.currOffset;
								}
								else
								{
									size8INOffset.prevOffset = size8INOffset.currOffset;
									newOffset = size8INOffset.currOffset;
									checkIfOffsetUsed(newOffset, NodeID, pdoType);				
									size8INOffset.currOffset = newOffset;
								}
						}
						
						/* if greater no change*/
						if(size32INOffset.currOffset >= size16INOffset.currOffset)
						{}
						else
						{
								size32INOffset.prevOffset = size32INOffset.currOffset ;
								size32INOffset.currOffset = size32INOffset.currOffset + 4;
						}
						
						/* if greater no change*/
						if(size64INOffset.currOffset >= size16INOffset.currOffset)
						{}
						else
						{
								size64INOffset.prevOffset = size64INOffset.currOffset ;
								size64INOffset.currOffset = size64INOffset.currOffset + 8;
						}						
					break;
			/*Unsigned32, Int32*/
			case 32:
						size32INOffset.prevOffset = size32INOffset.currOffset ;
						Offset = size32INOffset.currOffset ;
						size32INOffset.currOffset = size32INOffset.currOffset + 4;
			
							/* Set other DataType Offsets*/
								/* if greater no change*/
						if(size8INOffset.currOffset >= size32INOffset.currOffset)
						{}
						else
						{
								if((size8INOffset.currOffset >= size32INOffset.prevOffset) && (size8INOffset.currOffset <= size32INOffset.currOffset))
								{
									size8INOffset.prevOffset = size8INOffset.currOffset ;
									size8INOffset.currOffset = size32INOffset.currOffset;
								}
								else
								{
									size8INOffset.prevOffset = size8INOffset.currOffset;
										newOffset = size8INOffset.currOffset;
									
									checkIfOffsetUsed(newOffset, NodeID, pdoType);
									
									/*while(checkIfOffsetUsed(Offset))
									{
										Offset = Offset + 1;
									}*/
									size8INOffset.currOffset = newOffset;
								}
						}
						
							/* if greater no change*/
						if(size16INOffset.currOffset >= size32INOffset.currOffset)
						{}
						else
						{
								if((size16INOffset.currOffset >= size32INOffset.prevOffset) && (size16INOffset.currOffset < size32INOffset.currOffset))
								{
									size16INOffset.prevOffset = size16INOffset.currOffset ;
									size16INOffset.currOffset = size32INOffset.currOffset;
								}
								else
								{
									size16INOffset.prevOffset = size16INOffset.currOffset;
									newOffset = size16INOffset.currOffset;
										checkIfOffsetUsed(newOffset, NodeID, pdoType);
								/*	while(checkIfOffsetUsed(Offset))
									{
										Offset = Offset + 1;
									}*/
									size16INOffset.currOffset = newOffset;
								
								}
						}
						/* if greater no change*/
						if(size64INOffset.currOffset >= size8INOffset.currOffset)
						{}
						else
						{
								size64INOffset.prevOffset = size64INOffset.currOffset ;
								size64INOffset.currOffset = size64INOffset.currOffset + 8;
						}
						break;
			/*Unsigned64, Int64*/
			case 64:
						size64INOffset.prevOffset = size64INOffset.currOffset ;
						Offset = size64INOffset.currOffset ;
			
			break;
		}
		return Offset;
	}

	/*int CheckAvailableOffset(PDODataType pdt)
	{
		int Offset;
	
		switch(pdt)
		{
			case INTEGER8:
					Offset = size8Offsets[no8Offsets-1] + 1;
					if(no16Offsets ==0)
					{break;}
					if(size16Offsets[0] > Offset)
					{break;}
					for(int i= 0 ;i < no16Offsets; i++)
					{
						if(size16Offsets[i] == Offset)
						{
							Offset =  Offset + 2;
						}														
					} 
					
					if(no32Offets == 0)
					{break;}
					if(size32Offsets[0] != Offset)
					{break;}
					if(no64Offsets==0)
					{break;}
						for(int i= 0 ;i < no32Offsets; i++)
						{
							if(size32Offsets[i] == Offset)
								Offset =  Offset + 2;
							else
							break;
						} 
			case INTEGER16:
					Offset = size16Offsets[no16Offsets-1] + 1;
					if(no8Offsets ==0)
					{break;}
					if(size16Offsets[0] > Offset)
					{break;}
					for(int i= 0 ;i < no16Offsets; i++)
					{
						if(size16Offsets[i] == Offset)
						{
							Offset =  Offset + 2;
						}														
					} 
					
					if(no32Offets == 0)
					{break;}
					if(size32Offsets[0] != Offset)
					{break;}
					if(no64Offsets==0)
					{break;}
						for(int i= 0 ;i < no32Offsets; i++)
						{
							if(size32Offsets[i] == Offset)
								Offset =  Offset + 2;
							else
							break;
						} 
			case INTEGER32:
			case INTEGER64:
		}
	}*/

/**************************************************************************************************
* Function Name: OpenProject
* Description: Saves all the Nodes into the Project location
* Return value: ocfmRetCode
/****************************************************************************************************/

ocfmRetCode OpenProject(char* PjtPath, char* projectXmlFileName)
{
	CNodeCollection *objNodeCollection;
	objNodeCollection = CNodeCollection::getNodeColObjectPointer();
	delete objNodeCollection;
	xmlTextReaderPtr reader;
    	int ret;
	char *fileName;	
	
	#if defined DEBUG
	cout << "\nStrLen for FileName:" << (strlen(PjtPath) + strlen(projectXmlFileName) + 1) << endl;
	#endif
	
	fileName = new char[(strlen(PjtPath) + strlen(projectXmlFileName) + 5)];
	#if defined(_WIN32) && defined(_MSC_VER)
	{		
		sprintf(fileName, "%s\%s", PjtPath, projectXmlFileName);	
	}
	#else
	{
		sprintf(fileName, "%s/%s", PjtPath, projectXmlFileName);
	}
	#endif			
    reader = xmlReaderForFile(fileName, NULL, 0);
    try
    {
		if (reader != NULL)
		{
			ret = xmlTextReaderRead(reader);
			while (ret == 1)
			{		
				processProjectXML(reader, PjtPath);
				ret = xmlTextReaderRead(reader);
			}
			if(ret!=0)
			{
				ocfmException objException;
				/*objException->ocfm_Excpetion(o, true);*/
				#if defined DEBUG 
					cout << "\nOCFM_ERR_PARSE_XML\n" << endl;
				#endif
				objException.ocfm_Excpetion(OCFM_ERR_PARSE_XML);
				throw objException;
			}
		}
		else 
		{
			#if defined DEBUG 
				cout << "\nOCFM_ERR_CANNOT_OPEN_FILE\n" << endl;
			#endif
			ocfmException objException;
			objException.ocfm_Excpetion(OCFM_ERR_CANNOT_OPEN_FILE);
			throw objException;
		}
	}								
	
	catch(ocfmException& ex)
	{
		return ex._ocfmRetCode;
	}
	ocfmRetCode ErrStruct;		 
	ErrStruct.code = OCFM_ERR_SUCCESS;
	return ErrStruct;
}


/**************************************************************************************************
	* Function Name: processProjectXML
    * Description: Process the Node value,Name and its attributes
/****************************************************************************************************/
ocfmRetCode processProjectXML(xmlTextReaderPtr reader, char* PjtPath)
{
	const xmlChar *name, *value;
	CNodeCollection *objNodeCollection;
	CNode *objNode;
	CPjtSettings* stPjtSettings;
	stPjtSettings = CPjtSettings::getPjtSettingsPtr();
	name = xmlTextReaderConstName(reader);
	if (name == NULL)
	{
		#if defined DEBUG
			cout << "\nGot NULL for Name\n" << endl;	
		#endif
	}
    value = xmlTextReaderConstValue(reader);
	try
	{ 
		if( xmlTextReaderNodeType(reader)==1)
		{
			// Check for openCONFIGURATOR Tag
			if(strcmp(((char*)name),"openCONFIGURATOR")==0)
			{
				#if defined DEBUG
					cout << "openCONFIGURATOR Tag present\n" << endl;	
				#endif
				if (xmlTextReaderHasAttributes(reader) == 1)
				{
					while(xmlTextReaderMoveToNextAttribute(reader))
					{
						// Call Check Version number Fn
						if(IfVersionNumberMatches(reader) == false)
						{
							#if defined DEBUG
								cout << "openCONFIGURATOR Tag present\n" << endl;	
							#endif
							ocfmException objException;
							objException.ocfm_Excpetion(OCFM_ERR_CANNOT_OPEN_PROJECT_VER_MISMATCH);
							throw objException;
						}
					}
				}
			}
			else if (strcmp(((char*)name),"profile")==0)
			{
				#if defined DEBUG
					cout << "profile Tag present\n" << endl;	
				#endif
				if (xmlTextReaderHasAttributes(reader) == 1)
				{	
					#if defined DEBUG
						cout << "Cannot open project: Invalid Project XML\n" << endl;	
					#endif
					ocfmException objException;
					objException.ocfm_Excpetion(OCFM_ERR_INVALID_PJTXML);
					throw objException;								
				}
			}
			else if (strcmp(((char*)name),"Auto")==0)
			{
				#if defined DEBUG
					cout << "Auto Tag present\n" << endl;	
				#endif
				if (xmlTextReaderHasAttributes(reader) == 1)
				{	
					if(setProjectSettings_Auto(reader) == false)
					{
						#if defined DEBUG
							cout << "Cannot open project: Invalid Project XML\n" << endl;	
						#endif
						ocfmException objException;
						objException.ocfm_Excpetion(OCFM_ERR_INVALID_PJTXML);
						throw objException;		
					}
				}
				else
				{
					#if defined DEBUG
						cout << "Cannot open project: Invalid Project XML\n" << endl;	
					#endif
					ocfmException objException;
					objException.ocfm_Excpetion(OCFM_ERR_INVALID_PJTXML);
					throw objException;
				}
			}
			else if (strcmp(((char*)name),"Communication")==0)
			{
				#if defined DEBUG
					cout << "Communication Tag present\n" << endl;	
				#endif
				if (xmlTextReaderHasAttributes(reader) == 1)
				{	
					if(setProjectSettings_Communication(reader) == false)
					{
						#if defined DEBUG
							cout << "Cannot open project: Invalid Project XML\n" << endl;	
						#endif
						ocfmException objException;
						objException.ocfm_Excpetion(OCFM_ERR_INVALID_PJTXML);
						throw objException;		
					}
				}
				else
				{
					#if defined DEBUG
						cout << "Cannot open project: Invalid Project XML\n" << endl;	
					#endif
					ocfmException objException;
					objException.ocfm_Excpetion(OCFM_ERR_INVALID_PJTXML);
					throw objException;
				}
			}
			else if (strcmp(((char*)name),"NodeCollection")==0)
			{
				#if defined DEBUG
					cout << "NodeCollection Tag present\n" << endl;	
				#endif
				if (xmlTextReaderHasAttributes(reader) == 1)
				{
					#if defined DEBUG
						cout << "Cannot open project: Invalid Project XML\n" << endl;	
					#endif
					ocfmException objException;
					objException.ocfm_Excpetion(OCFM_ERR_INVALID_PJTXML);
					throw objException;	
				}
			}
			else if (strcmp(((char*)name),"Node") == 0)
			{
				if (xmlTextReaderHasAttributes(reader) == 1)
				{
					if(getandCreateNode(reader, PjtPath) == false)
					{
						#if defined DEBUG
							cout << "Cannot open project: Invalid Project XML\n" << endl;	
						#endif
						ocfmException objException;
						objException.ocfm_Excpetion(OCFM_ERR_INVALID_PJTXML);
						throw objException;	
					}
					else
					{
						ocfmRetCode ErrStruct;
						ErrStruct.code = OCFM_ERR_SUCCESS;
						return ErrStruct;
					}
				}
				else
				{
					#if defined DEBUG
						cout << "Cannot open project: Invalid Project XML\n" << endl;	
					#endif
					ocfmException objException;
					objException.ocfm_Excpetion(OCFM_ERR_INVALID_PJTXML);
					throw objException;	
				}
			}
		}
			
	}	
	catch(ocfmException* ex)
	{		
		 return ex->_ocfmRetCode;
	}	
	#if defined DEBUG
		cout << "\nstPjtSettings.getGenerateAttr():" << stPjtSettings->getGenerateAttr() << endl;
		cout << "\nstPjtSettings.getSaveAttr():" << stPjtSettings->getSaveAttr() << endl;
		if(stPjtSettings->getPOWERLINK_IP() != NULL)
			cout << "\nstPjtSettings.getPOWERLINK_IP():" << stPjtSettings->getPOWERLINK_IP() << endl;
	#endif
	ocfmRetCode ErrStruct;
	ErrStruct.code = OCFM_ERR_SUCCESS;
	return ErrStruct;
}

/**************************************************************************************************
	* Function Name: setProjectSettings_Auto
    * Description: Gets the Auto info from the Pjt xml and stores in the object
/****************************************************************************************************/
bool setProjectSettings_Auto(xmlTextReaderPtr reader)
{
	const xmlChar* name,*value;
	CPjtSettings* stPjtSettings;
	stPjtSettings = CPjtSettings::getPjtSettingsPtr();
	
	while(xmlTextReaderMoveToNextAttribute(reader))
	{	
		//Retrieve the name and Value of an attribute	
		value = xmlTextReaderConstValue(reader);
		name = xmlTextReaderConstName(reader);	

		if(value == NULL || name == NULL)
			return false;		
		#if defined DEBUG
			cout << "\nName:" << name << endl;	
			cout << "\nValue:" << value << endl;
		#endif
		if (strcmp(((char*)name),"Generate") == 0)
		{
			if(strcmp(((char*)value),"YES") == 0)
				stPjtSettings->setGenerateAttr(YES_AG);
			else if(strcmp(((char*)value),"NO") == 0)
				stPjtSettings->setGenerateAttr(NO_AG);
			else
			{
				#if defined DEBUG
					cout << "\nsetProjectSettings_Auto returning false" << endl;
				#endif
				return false;
			}
		}
		else if (strcmp(((char*)name),"Save") == 0)
		{
			if(strcmp(((char*)value),"YES") == 0)
				stPjtSettings->setSaveAttr(YES_AS);
			else if(strcmp(((char*)value),"PROMPT") == 0)
				stPjtSettings->setSaveAttr(PROMPT_AS);
			else if(strcmp(((char*)value),"DISCARD") == 0)
				stPjtSettings->setSaveAttr(DISCARD_AS);
			else
			{
				#if defined DEBUG
					cout << "\nsetProjectSettings_Auto returning false" << endl;
				#endif
				return false;
			}
		}
		else
		{
			#if defined DEBUG
				cout << "\nsetProjectSettings_Auto returning false" << endl;
			#endif
			return false;
		}
	}
	return true;
}

/**************************************************************************************************
	* Function Name: setProjectSettings_Communication
    * Description: Gets the communication info from the Pjt xml and stores in the object
/****************************************************************************************************/
bool setProjectSettings_Communication(xmlTextReaderPtr reader)
{
	const xmlChar* name,*value;
	CPjtSettings* stPjtSettings;
	stPjtSettings = CPjtSettings::getPjtSettingsPtr();
	
	while(xmlTextReaderMoveToNextAttribute(reader))
	{	
		//Retrieve the name and Value of an attribute	
		value = xmlTextReaderConstValue(reader);
		name = xmlTextReaderConstName(reader);	

		if(value == NULL || name == NULL)
			return false;		
		#if defined DEBUG
			cout << "\nName:" << name << endl;	
			cout << "\nValue:" << value << endl;
		#endif
		if (strcmp(((char*)name),"IP") == 0)
		{
			if((char*)value != NULL)		
			{
				stPjtSettings->setPOWERLINK_IP((char*)value);
				cout << "\nstPjtSettings->getPOWERLINK_IP():" << stPjtSettings->getPOWERLINK_IP() << endl;
			}
			else
			{
				#if defined DEBUG
					cout << "\nsetProjectSettings_Communication returning false" << endl;
				#endif
				return false;
			}
			
		}
		else
		{
			#if defined DEBUG
				cout << "\nsetProjectSettings_Communication returning false" << endl;
			#endif
			return false;
		}
	}
	return true;
}

/**************************************************************************************************
	* Function Name: getandCreateNode
    * Description: Gets the Node properties from the Pjt xml and Creates the Nodes
/****************************************************************************************************/
bool getandCreateNode(xmlTextReaderPtr reader, char* PjtPath)
{
	const xmlChar* name,*value;
	int ret;
	ocfmRetCode ErrStruct;
	
	char* nodeName;
	int nodeID;
	ENodeType nodeType;
	char* xdcPath;
	char* fileName;
	
	CPjtSettings* stPjtSettings;
	stPjtSettings = CPjtSettings::getPjtSettingsPtr();
	
	while(xmlTextReaderMoveToNextAttribute(reader))
	{	
		//Retrieve the name and Value of an attribute	
		value = xmlTextReaderConstValue(reader);
		name = xmlTextReaderConstName(reader);	

		if(value == NULL || name == NULL)
			return false;
			
		if (strcmp(((char*)name),"name") == 0)
		{
			if((char*)value != NULL)		
			{
				nodeName = new char[strlen((char*)value) + 1];
				strcpy((char*)nodeName, (char*)value);
				cout << "\nnodeName:" << nodeName << endl;
				//stPjtSettings->setPOWERLINK_IP((char*)value);
				//cout << "\nstPjtSettings->getPOWERLINK_IP():" << stPjtSettings->getPOWERLINK_IP() << endl;
			}
			else
			{
				#if defined DEBUG
					cout << "\ngetandCreateNode returning false" << endl;
				#endif
				return false;
			}
			
		}
		else if (strcmp(((char*)name),"NodeId") == 0)
		{
			if((char*)value != NULL)		
			{
				nodeID = atoi((char*)value);
				cout << "\nnodeID:" << nodeID << endl;
				//stPjtSettings->setPOWERLINK_IP((char*)value);
				//cout << "\nstPjtSettings->getPOWERLINK_IP():" << stPjtSettings->getPOWERLINK_IP() << endl;
			}
			else
			{
				#if defined DEBUG
					cout << "\ngetandCreateNode returning false" << endl;
				#endif
				return false;
			}
			
		}
		else if (strcmp(((char*)name),"NodeType") == 0)
		{
			if(strcmp(((char*)value), "MN") == 0)		
			{
				nodeType = MN;
				#if defined DEBUG
				cout << "\nnodeType:" << nodeType << endl;				
				#endif
			}
			else if(strcmp(((char*)value), "CN") == 0)		
			{
				nodeType = CN;
				#if defined DEBUG
				cout << "\nnodeType:" << nodeType << endl;
				#endif
				//stPjtSettings->setPOWERLINK_IP((char*)value);
				//cout << "\nstPjtSettings->getPOWERLINK_IP():" << stPjtSettings->getPOWERLINK_IP() << endl;
			}
			else
			{
				#if defined DEBUG
					cout << "\ngetandCreateNode returning false" << endl;
				#endif
				return false;
			}
			
		}
		else if (strcmp(((char*)name),"xdc") == 0)
		{
			if((char*)value != NULL)		
			{
				xdcPath = new char[strlen((char*)value) + 1];
				strcpy((char*)xdcPath, (char*)value);
				#if defined DEBUG
				cout << "\nxdcPath:" << xdcPath << endl;
				#endif
				//stPjtSettings->setPOWERLINK_IP((char*)value);
				//cout << "\nstPjtSettings->getPOWERLINK_IP():" << stPjtSettings->getPOWERLINK_IP() << endl;
			}
			else
			{
				#if defined DEBUG
					cout << "\ngetandCreateNode returning false" << endl;
				#endif
				xdcPath = NULL;
				//return false;
			}			
		}
	}
	#if defined DEBUG
		cout << "\n\n\nCan Create Node\n\n"  << endl;
		cout << "\nCreateNode:" << nodeID << nodeType << nodeName << endl;
	#endif
	if(nodeType == 1)
	{
		ErrStruct = CreateNode(nodeID, CN, nodeName);
	}
	else if(nodeType == 0)
	{
		ErrStruct = CreateNode(nodeID, MN, nodeName);
	}
	
	if(ErrStruct.code != OCFM_ERR_SUCCESS)
		return false;
	
	#if defined DEBUG
	cout << "\nCreateNode - ErrStruct.code:" << ErrStruct.code << endl;
	#endif
	
	fileName = new char[(strlen(PjtPath) + strlen(xdcPath) + 5)];
	sprintf(fileName, "%s/%s", PjtPath, xdcPath);
	if(nodeType == 1)
		ErrStruct = parseFile(fileName, nodeID, CN);
	else if(nodeType == 0)
		ErrStruct = parseFile(fileName, nodeID, MN);

	return true;
}
/**************************************************************************************************
* Function Name: saveProjectXML
* Description: Saves the project details into the Project location
* Return value: bool[True/False]
/****************************************************************************************************/
bool saveProjectXML(char* ProjectPath, char* ProjectName)
{
	
CPjtSettings* stPjtSettings;
stPjtSettings = CPjtSettings::getPjtSettingsPtr();

xmlTextWriterPtr writer;
xmlDocPtr doc;
int rc;

char* fileName;
fileName = new char[(strlen(ProjectPath) + strlen(ProjectName) + strlen(ProjectName) + 5)];

#if defined(_WIN32) && defined(_MSC_VER)
{
	sprintf(fileName, "%s\%s\%s.oct", ProjectPath, ProjectName, ProjectName);
}
#else
{
	sprintf(fileName, "%s/%s/%s.oct", ProjectPath, ProjectName, ProjectName);
}
#endif

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
xmlTextWriterSetIndent(writer, 1);
// Start openCONFIGURATOR Tag		
rc = xmlTextWriterStartElement(writer, BAD_CAST "openCONFIGURATOR");
if (rc < 0) 
{
	printf("testXmlwriterMemory: Error at xmlTextWriterStartElement\n");
	ocfmException* objException = new ocfmException;
	objException->ocfm_Excpetion(OCFM_ERR_XML_WRITER_START_ELT_FAILED);
	throw objException;
}
	rc = xmlTextWriterWriteAttribute(writer, BAD_CAST "Version", BAD_CAST TOOL_VERSION);
	
xmlTextWriterSetIndent(writer, 1);
// Start profile Tag
rc = xmlTextWriterStartElement(writer, BAD_CAST "profile");
if (rc < 0) 
{
	printf("testXmlwriterMemory: Error at xmlTextWriterStartElement\n");
	ocfmException* objException = new ocfmException;
	objException->ocfm_Excpetion(OCFM_ERR_XML_WRITER_START_ELT_FAILED);
	throw objException;
}

	xmlTextWriterSetIndent(writer, 1);
	// Start Auto Tag		
	rc = xmlTextWriterStartElement(writer, BAD_CAST "Auto");
	if (rc < 0) 
	{
		printf("testXmlwriterMemory: Error at xmlTextWriterStartElement\n");
		ocfmException* objException = new ocfmException;
		objException->ocfm_Excpetion(OCFM_ERR_XML_WRITER_START_ELT_FAILED);
		throw objException;
	}

	cout << "\n\n\nstPjtSettings->getGenerateAttr():" << stPjtSettings->getGenerateAttr() << endl;
	if(stPjtSettings->getGenerateAttr() == 0)
		rc = xmlTextWriterWriteAttribute(writer, BAD_CAST "Generate", BAD_CAST "NO");
	else if(stPjtSettings->getGenerateAttr() == 1)
		rc = xmlTextWriterWriteAttribute(writer, BAD_CAST "Generate", BAD_CAST "YES");
	
	cout << "\n\n\nstPjtSettings->getSaveAttr():" << stPjtSettings->getSaveAttr() << endl;
	if(stPjtSettings->getSaveAttr() == 0)
		rc = xmlTextWriterWriteAttribute(writer, BAD_CAST "Save", BAD_CAST "YES");
	else if(stPjtSettings->getSaveAttr() == 1)
		rc = xmlTextWriterWriteAttribute(writer, BAD_CAST "Save", BAD_CAST "PROMPT");
	else if(stPjtSettings->getSaveAttr() == 2)
		rc = xmlTextWriterWriteAttribute(writer, BAD_CAST "Save", BAD_CAST "DISCARD");

	// End Auto Tag
	rc = xmlTextWriterEndElement(writer);
	if (rc < 0)
	{
		printf("testXmlwriterDoc: Error at xmlTextWriterEndElement\n");
		ocfmException* objException = new ocfmException;
		objException->ocfm_Excpetion(OCFM_ERR_XML_WRITER_END_ELT_FAILED);
		throw objException;
	}
	
	xmlTextWriterSetIndent(writer, 1);
	// Start Communication Tag		
	rc = xmlTextWriterStartElement(writer, BAD_CAST "Communication");
	if (rc < 0) 
	{
		printf("testXmlwriterMemory: Error at xmlTextWriterStartElement\n");
		ocfmException* objException = new ocfmException;
		objException->ocfm_Excpetion(OCFM_ERR_XML_WRITER_START_ELT_FAILED);
		throw objException;
	}
		rc = xmlTextWriterWriteAttribute(writer, BAD_CAST "IP", BAD_CAST stPjtSettings->getPOWERLINK_IP());
		
	// End Communication Tag
	rc = xmlTextWriterEndElement(writer);
	if (rc < 0)
	{
		printf("testXmlwriterDoc: Error at xmlTextWriterEndElement\n");
		ocfmException* objException = new ocfmException;
		objException->ocfm_Excpetion(OCFM_ERR_XML_WRITER_END_ELT_FAILED);
		throw objException;
	}

// End profile Tag
rc = xmlTextWriterEndElement(writer);
if (rc < 0)
{
	printf("testXmlwriterDoc: Error at xmlTextWriterEndElement\n");
	ocfmException* objException = new ocfmException;
	objException->ocfm_Excpetion(OCFM_ERR_XML_WRITER_END_ELT_FAILED);
	throw objException;
}

xmlTextWriterSetIndent(writer, 1);
// Start NodeCollection Tag
rc = xmlTextWriterStartElement(writer, BAD_CAST "NodeCollection");
if (rc < 0) 
{
	printf("testXmlwriterMemory: Error at xmlTextWriterStartElement\n");
	ocfmException* objException = new ocfmException;
	objException->ocfm_Excpetion(OCFM_ERR_XML_WRITER_START_ELT_FAILED);
	throw objException;
}

		CNode objNode;		
		CNodeCollection *objNodeCollection;
		CIndexCollection *objIndexCollection;
		CIndex objIndex;
		CIndex* objIndexPtr;
		Parameter* para;
		CApplicationProcess* objAppProc;
		int IndexPos = 0;
		
		objIndex.setNodeID(objNode.getNodeId());
		objNodeCollection = CNodeCollection::getNodeColObjectPointer();
		//objNode = objNodeCollection->getNode(NodeType, NodeID);

		//objIndexCollection = objNode.getIndexCollection();
		cout << "\nobjNodeCollection.getNumberOfNodes():" << objNodeCollection->getNumberOfNodes() << endl;
		


	for(int nodeCount = 0; nodeCount < objNodeCollection->getNumberOfNodes(); nodeCount++)
	{
		CNode* objNode;

		objNode = objNodeCollection->getNodebyColIndex(nodeCount);
		xmlTextWriterSetIndent(writer, 1);
		// Start Node Tag		
		rc = xmlTextWriterStartElement(writer, BAD_CAST "Node");

		if (rc < 0) 
		{
			printf("testXmlwriterMemory: Error at xmlTextWriterStartElement\n");
			ocfmException* objException = new ocfmException;
			objException->ocfm_Excpetion(OCFM_ERR_XML_WRITER_START_ELT_FAILED);
			throw objException;
		}

		char* tmp_NodeName;
		tmp_NodeName = new char[(strlen(objNode->getNodeName()) + 2)];
		strcpy(tmp_NodeName, (char*) objNode->getNodeName());
		rc = xmlTextWriterWriteAttribute(writer, BAD_CAST "name", BAD_CAST tmp_NodeName);
		
		char* tmp_NodeID;
		tmp_NodeID = new char[20];
		
		itoa(objNode->getNodeId(), tmp_NodeID, 0);

		rc = xmlTextWriterWriteAttribute(writer, BAD_CAST "NodeId", BAD_CAST tmp_NodeID);
		
		ENodeType tmp_NodeType;
		tmp_NodeType = objNode->getNodeType();
		
		if(tmp_NodeType == 0)
			rc = xmlTextWriterWriteAttribute(writer, BAD_CAST "NodeType", BAD_CAST "MN");
		if(tmp_NodeType == 1)
			rc = xmlTextWriterWriteAttribute(writer, BAD_CAST "NodeType", BAD_CAST "CN");

		char* tmp_XdcName;
		tmp_XdcName = new char[20];
		
		sprintf(tmp_XdcName, "%s.xdc", tmp_NodeID);		
		rc = xmlTextWriterWriteAttribute(writer, BAD_CAST "xdc", BAD_CAST tmp_XdcName);

		// End Node Tag
		rc = xmlTextWriterEndElement(writer);
		if (rc < 0)
		{
			printf("testXmlwriterDoc: Error at xmlTextWriterEndElement\n");
			ocfmException* objException = new ocfmException;
			objException->ocfm_Excpetion(OCFM_ERR_XML_WRITER_END_ELT_FAILED);
			throw objException;
		}
		delete [] tmp_NodeName;
		delete [] tmp_NodeID;
		delete [] tmp_XdcName;
	}

// End NodeCollection Tag
rc = xmlTextWriterEndElement(writer);
if (rc < 0)
{
	printf("testXmlwriterDoc: Error at xmlTextWriterEndElement\n");
	ocfmException* objException = new ocfmException;
	objException->ocfm_Excpetion(OCFM_ERR_XML_WRITER_END_ELT_FAILED);
	throw objException;
}

// End openCONFIGURATOR Tag
rc = xmlTextWriterEndElement(writer);
if (rc < 0)
{
	printf("testXmlwriterDoc: Error at xmlTextWriterEndElement\n");
	ocfmException* objException = new ocfmException;
	objException->ocfm_Excpetion(OCFM_ERR_XML_WRITER_END_ELT_FAILED);
	throw objException;
}

rc = xmlTextWriterEndDocument(writer);
if (rc < 0) 
{
	printf("testXmlwriterDoc: Error at xmlTextWriterEndDocument\n");
	ocfmException* objException = new ocfmException;
	objException->ocfm_Excpetion(OCFM_ERR_XML_END_DOC_FAILED);
	throw objException;
}
cout << "\n5" << endl;
xmlFreeTextWriter(writer);

xmlSaveFileEnc(fileName, doc, MY_ENCODING);

xmlFreeDoc(doc);	

cout << "\nsaveProjectXML:\n" << fileName <<endl;
delete [] fileName;
return true;
}
