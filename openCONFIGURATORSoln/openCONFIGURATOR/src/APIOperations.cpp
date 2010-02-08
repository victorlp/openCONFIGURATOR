///////////////////////////////////////////////////////////////////////////////////////////////
//
//  $Source: $
//
// NAME:  APIOperations.cpp
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
#include <fstream>
#include <stdio.h>
#include <libxml/xmlreader.h>
#include <errno.h>
#include <string.h>
#include <libxml/xmlwriter.h>
#include <libxml/encoding.h>
#include <sys/stat.h> 
#if defined(_WIN32) && defined(_MSC_VER)
	#include <direct.h>
#else
	#include <sys/stat.h>
#endif
#include "../Include/openCONFIGURATOR.h"
#include "../Include/Internal.h"
#include "../Include/Exception.h"
#include "../Include/ObjectDictionary.h"
#include "../Include/ProcessImage.h"
#include "../Include/Declarations.h"

/****************************************************************************************************
* Defines
****************************************************************************************************/

#define MY_ENCODING "UTF-8"
#define CDC_BUFFER 5000
//#define PI_VAR_COUNT 4000
#define MAX_FILE_PATH_SIZE 500

/****************************************************************************************************
* Global Variables
****************************************************************************************************/
INT32 iLastVarIndex = -1;
static bool bCDTCompleted = false;
//static bool NotLoadedOBD = true;
//static stOffsets size8Offset;
//static stOffsets size16Offset;
//static stOffsets size32Offset;
//static stOffsets size64Offset;
static stOffsets stSize8INOffset;
static stOffsets stSize16INOffset;
static stOffsets stSize32INOffset;
static stOffsets stSize64INOffset;
static stOffsets stSize8OUTOffset;
static stOffsets stSize16OUTOffset;
static stOffsets stSize32OUTOffset;
static stOffsets stSize64OUTOffset;

INT32 iNo8Offsets  = 0; 
INT32 iNo16Offsets = 0;
INT32 iNo32Offsets = 0; 
INT32 iNo64Offsets = 0;

char abC_DLL_ISOCHR_MAX_PAYL[5] = "1490";
char abC_DLL_MIN_ASYNC_MTU[4] = "300";
INT32 iConfigDate;
INT32 iConfigTime;
UINT32 uiCycleNumber;
/****************************************************************************************************
* FUNCTION DEFINITIONS
****************************************************************************************************/

/****************************************************************************************************
* Function Name: AddIndexAttributes
* Description: Adds the default attributes to the Index, when addded.
* Return value: void
****************************************************************************************************/
static void AddIndexAttributes(char* pIndexID, CIndex* pobjIndex, CIndex* pobjDictIndex)
{
	// Setting the Index Value
	//
	//if(strcmp((char*) pobjIndex->getIndexValue(), pIndexID) != 0)
	//{
	//		exit;
	//}
	
	//cout << "INDEXID" << pobjIndex->getIndexValue() << endl;
	//cout << "RX pIndexID" << IndexID << endl;
	pobjIndex->setIndexValue(pIndexID);
	
	//$S The Value for all the attributes has to come from ObjDict.xml
	if(pobjDictIndex->getName() != NULL)
	{
		pobjIndex->setName((char*) pobjDictIndex->getName());
	}
	else
	{
		pobjIndex->setName((char*) "");
	}
	
	if(pobjDictIndex->getObjectType() != NULL)		
	{
		pobjIndex->setObjectType((char*) pobjDictIndex->getObjectType());
	}
	else
	{
		//Setting "0", so default case is hit, when setting
		pobjIndex->setObjectType((char*) "");
	}
			
	if( pobjDictIndex->getLowLimit() != NULL)
	{
		pobjIndex->setLowLimit((char*) pobjDictIndex->getLowLimit());
	}
	else
	{
		pobjIndex->setLowLimit((char*) "");
	}
	
	if(pobjDictIndex->getHighLimit() != NULL)
	{
		pobjIndex->setHighLimit((char*) pobjDictIndex->getHighLimit());
	}
	else
	{
		pobjIndex->setHighLimit((char*) "");
	}
		
	if(pobjDictIndex->getAccessType() != NULL)
	{
		pobjIndex->setAccessType((char*) pobjDictIndex->getAccessType());
	}
	else
	{
		pobjIndex->setAccessType((char*) "");
	}
		
	if(pobjDictIndex->getPDOMapping() != NULL)
	{
		char *pbPdoMapStr = new char[50];
		strcpy(pbPdoMapStr, (char*) pobjDictIndex->getPDOMapping());
		pobjIndex->setPDOMapping(pbPdoMapStr);
		delete [] pbPdoMapStr;
	}
	else
	{
		char *pbPdoMapStr = new char[5];
		strcpy(pbPdoMapStr, (char*) "");
		pobjIndex->setPDOMapping(pbPdoMapStr);
		delete [] pbPdoMapStr;
		//pobjIndex->setPDOMapping("NO");
	}
		
	////if(pobjIndex->getPDOMapping() != NULL)
	////	ppobjIndex->setPDOMapping((char*) objIndex->getPDOMapping());
	////else
	////	pobjIndex->setPDOMapping("");
	
	if(pobjDictIndex->getDefaultValue() != NULL)
	{
		pobjIndex->setDefaultValue((char*) pobjDictIndex->getDefaultValue());
	}
	else
	{
		pobjIndex->setDefaultValue((char*) "");
	}
	
	if(pobjDictIndex->getActualValue() != NULL)
	{
		pobjIndex->setActualValue((char*) pobjDictIndex->getActualValue());
	}
	else
	{
		pobjIndex->setActualValue((char*) "");
	}
	//$STODO:
	if(pobjDictIndex->getDataType().getName() != NULL)
	{
		//pobjIndex->setDataType((char*) objIndex->getDataTypeValue(), objIndex->getNodeID());
		pobjIndex->setDataTypeST(pobjDictIndex->getDataType());
	}
	else
	{
		pobjIndex->setDataType((char*) "");
	}

	return;
}

/****************************************************************************************************
* Function Name: AddSubIndexAttributes
* Description: Adds the default attributes to the Index, when addded.
* Return value: void
****************************************************************************************************/
static void AddSubIndexAttributes (char* pSubIndexID, CSubIndex* pobjSubIndex, CSubIndex* pobjDictSIndex)
{
		
	// Setting the Index Value--
	pobjSubIndex->setIndexValue(pSubIndexID);
	//$S The Value for all the attributes has to come from ObjDict.xml
	if(pobjDictSIndex->getName() != NULL)
	{
		pobjSubIndex->setName((char*) pobjDictSIndex->getName());
	}
	else
	{
		pobjSubIndex->setName((char*) "");
	}
	if(pobjDictSIndex->getObjectType() != NULL)
	{
		//ppobjSubIndex->setObjectType(atoi(objSubIndex->getObjectType()));
		pobjSubIndex->setObjectType((char*) (pobjDictSIndex->getObjectType()));
	}
	else
	{
		//Setting "0", so default case is hit, when setting
		pobjSubIndex->setObjectType((char*) "");
	}
	
	if( pobjDictSIndex->getLowLimit() != NULL)
	{
		pobjSubIndex->setLowLimit((char*) pobjDictSIndex->getLowLimit());
	}
	else
	{
		pobjSubIndex->setLowLimit((char*) "");
	}
	
	if(pobjDictSIndex->getHighLimit() != NULL)
	{
		pobjSubIndex->setHighLimit((char*) pobjDictSIndex->getHighLimit());
	}
	else
	{
		pobjSubIndex->setHighLimit((char*) "");
	}
	
	if(pobjDictSIndex->getAccessType() != NULL)
	{
		pobjSubIndex->setAccessType((char*) pobjDictSIndex->getAccessType());
	}
	else
	{
		pobjSubIndex->setAccessType((char*) "");
	}
			
	//if(pobjSubIndex->getPDOMapping() != NULL)
	//{
	//	char *str = new char[50];
	//	strcpy(str, (char*) pobjSubIndex->getPDOMapping());
	//	pobjSubIndex->setPDOMapping(str);
	//	delete [] str;
	//}
	//else
	//	pobjSubIndex->setPDOMapping("");
			
	if(pobjDictSIndex->getDefaultValue() != NULL)
	{
		pobjSubIndex->setDefaultValue((char*) pobjDictSIndex->getDefaultValue());
	}
	else
	{
		pobjSubIndex->setDefaultValue((char*) "");
	}
	
	if(pobjDictSIndex->getActualValue() != NULL)
	{
		pobjSubIndex->setActualValue((char*) pobjDictSIndex->getActualValue());
	}
	else
	{
		pobjSubIndex->setActualValue((char*) "");
	}
	
	if(pobjDictSIndex->getDataType().getName() != NULL)
	{
		//ppobjSubIndex->setDataType(objSubIndex->getDataTypeValue());
		pobjSubIndex->setDataTypeST(pobjDictSIndex->getDataType());
	}
	else
	{
		pobjSubIndex->setDataType((char*) "");
	}
	return;
}

 
/****************************************************************************************************
* Function Name: CreateNode
* Description:
* Return value: ocfmRetCode
****************************************************************************************************/
ocfmRetCode CreateNode(INT32 iNodeID, ENodeType enumNodeType, char* pbNodeName)
{
	ocfmRetCode stErrorInfo;
	CNode objNode;
	CNodeCollection *pobjNodeCollection;
	INT32 iNodePos;
	
	try
	{
		// TODO:If Check is made when Zero nodes present, Seg Fault is happening
		if(enumNodeType ==MN)
		{
			//cout << "loading od.xml"<< endl;
			//printf("\n ObjectDictLoaded %d", ObjectDictLoaded);
			char tmpCmdBuffer[LINUX_INSTALL_DIR_LEN];
			if(!ObjectDictLoaded)
			{
				#if defined(_WIN32) && defined(_MSC_VER)
					LoadObjectDictionary((char*) "od.xml");
				#else
				{
					sprintf(tmpCmdBuffer, "%s/od.xml", LINUX_INSTALL_DIR);
					//printf("\n command Buffer %s", tmpCmdBuffer);
					LoadObjectDictionary(tmpCmdBuffer);
				}
				#endif
				ObjectDictLoaded = true;
			//cout << "loaded xml" << endl;
			}
			uiCycleNumber = 0;
		}
		if(enumNodeType == CN)
		{
			bool bFlag = false;
			stErrorInfo = IfNodeExists(iNodeID, enumNodeType, &iNodePos, bFlag );
								
			if(stErrorInfo.code == OCFM_ERR_SUCCESS && bFlag == true)								
			{
				//cout << "from CreateNode OCFM_ERR_NODE_ALREADY_EXISTS:" << OCFM_ERR_NODE_ALREADY_EXISTS << endl;
				ocfmException objException ;//= new ocfmException;
				objException.ocfm_Excpetion(OCFM_ERR_NODE_ALREADY_EXISTS);		
				throw &objException;
			}
			else
			{
				#if defined DEBUG
					cout<< "OCFM_ERR_NODEID_NOT_FOUND" << OCFM_ERR_NODEID_NOT_FOUND << endl;;
				#endif
			}
		}
		objNode.setNodeId(iNodeID);
		objNode.setNodeType(enumNodeType);
		objNode.setNodeName(pbNodeName);
		
		objNode.CreateIndexCollection();
		objNode.CreateDataTypeCollection();
		objNode.CreateApplicationProcess();
		objNode.CreateNetworkManagament();

		pobjNodeCollection = CNodeCollection::getNodeColObjectPointer();
		pobjNodeCollection->addNode(objNode);
		
		//cout << "\nOut of Create Node\n" << endl;
		
		stErrorInfo.code = OCFM_ERR_SUCCESS;
		return stErrorInfo;
	}
	catch(ocfmException* ex)
	{
		//cout << "\nOut of Create Node\n" << endl;
		stErrorInfo = ex->_ocfmRetCode;
		return stErrorInfo;
	}
}

/****************************************************************************************************
* Function Name: DeleteNode
* Description:
* Return value: ocfmRetCode
****************************************************************************************************/
ocfmRetCode DeleteNode(INT32 iNodeID, ENodeType enumNodeType)
	{
		INT32 iNodePos;
		ocfmRetCode stErrorInfo;
		bool bFlag = false;
		stErrorInfo = IfNodeExists(iNodeID, enumNodeType, &iNodePos, bFlag);
							
		if(bFlag!= true)
		{
			/* Function didnt throw any exception but Node doesnt exist */
			if(stErrorInfo.code == OCFM_ERR_SUCCESS)								
			{
				stErrorInfo.code = OCFM_ERR_NODEID_NOT_FOUND;
			}
			/* Function threw exception*/
			else
			{
				//cout << "\n\nErrStruct.code:" << stErrorInfo.code << "\n\n!!!" << endl;
				// Node Doesn't Exist
				
			}
			return stErrorInfo;
		}
		

		CNode objNode;		
		CNodeCollection *pobjNodeCollection;
		pobjNodeCollection= CNodeCollection::getNodeColObjectPointer();	
		//cout<< "Inside DeleteNode: \n" <<pobjNodeCollection->getNumberOfNodes()<<endl;
		pobjNodeCollection = CNodeCollection::getNodeColObjectPointer();
        objNode = pobjNodeCollection->getNodebyCollectionIndex(iNodePos);

        char* cSIdx = new char[SUBINDEX_LEN];
        cSIdx = _IntToAscii(objNode.getNodeId(), cSIdx, 16);
        cSIdx = padLeft(cSIdx, '0', 2);
        ocfmRetCode stErrStructInfo;
        int IndexPos;
        int subIndexPos;
        if(objNode.getNodeType() == CN)
        {
            if(objNode.getStationType() == MULTIPLEXED)
            {
                //Deleted MN's 1F9B and Suindex = old node id
                CheckAndReAssignMultiplex(objNode.getNodeId(), objNode.getForcedCycle() );
            }
            stErrStructInfo = IfSubIndexExists(MN_NODEID, MN, (char *)MULTIPL_CYCLE_ASSIGN_OBJECT, cSIdx, &subIndexPos, &IndexPos);
            if(stErrStructInfo.code == OCFM_ERR_SUCCESS)
            {
                DeleteSubIndex(MN_NODEID, MN, (char *)MULTIPL_CYCLE_ASSIGN_OBJECT, cSIdx);
            }
            stErrStructInfo = IfSubIndexExists(MN_NODEID, MN, (char *)MNCN_POLLRESPONSE_TIMEOUT_OBJECT, cSIdx, &subIndexPos, &IndexPos);
            if(stErrStructInfo.code == OCFM_ERR_SUCCESS)
            {
                DeleteSubIndex(MN_NODEID, MN, (char *)MNCN_POLLRESPONSE_TIMEOUT_OBJECT, cSIdx);
            }
        }
		pobjNodeCollection->deleteNode(iNodePos);
		stErrorInfo.code = OCFM_ERR_SUCCESS;
		return stErrorInfo;		
	}
	
/****************************************************************************************************
* Function Name: DeleteIndex
* Description: Deletes the Index after performing check for Index Existanse
* Return value: ocfmRetCode
****************************************************************************************************/
ocfmRetCode DeleteIndex(INT32 iNodeID, ENodeType enumNodeType, char* pIndexID)
	{
		
		INT32 iIndexPos;
		try
		{
			ocfmRetCode stErrorInfo;
			stErrorInfo = IfIndexExists(iNodeID, enumNodeType, pIndexID, &iIndexPos);
			if(stErrorInfo.code == OCFM_ERR_SUCCESS)
			{
				//iIndexPos = stErrorInfo.returnValue;
			}
			else
			{
				ocfmException objException;				
				objException.ocfm_Excpetion(OCFM_ERR_INDEXID_NOT_FOUND);
				throw objException;
			}
			CNode objNode;		
			CNodeCollection *pobjNodeCollection;
			CIndexCollection *pobjIndexCollection;
			CIndex objIndex;

			objIndex.setNodeID(objNode.getNodeId());
			pobjNodeCollection= CNodeCollection::getNodeColObjectPointer();
			objNode = pobjNodeCollection->getNode(enumNodeType, iNodeID);

			pobjIndexCollection = objNode.getIndexCollection();
			//cout<< "Inside DeleteIndex: \n" << atoi(pIndexID) <<endl;
			//cout<< "Inside DeleteIndex: \n" << iIndexPos <<endl;
			pobjIndexCollection->deleteIndex(iIndexPos);
			stErrorInfo.code = OCFM_ERR_SUCCESS;
			return stErrorInfo;
		}	
		catch(ocfmException& ex)
		{
			return ex._ocfmRetCode;
		}
	}
	
/****************************************************************************************************
* Function Name: DeleteSubIndex
* Description: Deletes the Index after performing check for Index Existanse
* Return value: ocfmRetCode
****************************************************************************************************/
ocfmRetCode DeleteSubIndex(INT32 iNodeID, ENodeType enumNodeType, char* pbIndexID, char* pbSubIndexID)
	{
		CNode objNode;		
		CNodeCollection *pobjNodeCollection;
		CIndexCollection *pobjIndexCollection;
		CIndex* pobjIndex;
		CSubIndex* pobjSIDx;
		INT32 iSubIndexPos;
		INT32 iIndexPos;
		ocfmRetCode stErrorInfo;

		try
		{
			stErrorInfo = IfSubIndexExists(iNodeID, enumNodeType, pbIndexID, pbSubIndexID, &iSubIndexPos, &iIndexPos);
			if(stErrorInfo.code == OCFM_ERR_SUCCESS)
			{
				//iSubIndexPos = Tmp_stErrorInfo.returnValue;
			}
			else
			{
					ocfmException objException;				
					objException.ocfm_Excpetion(OCFM_ERR_SUBINDEXID_NOT_FOUND);
					throw objException;
			}

			pobjNodeCollection= CNodeCollection::getNodeColObjectPointer();
			objNode = pobjNodeCollection->getNode(enumNodeType, iNodeID);


			pobjIndexCollection = objNode.getIndexCollection();
			pobjIndex =pobjIndexCollection->getIndex(iIndexPos);
			pobjSIDx = pobjIndex->getSubIndex(iSubIndexPos);
			
			
			//cout << "NumberofSubIndexes:" << pobjIndex.getNumberofSubIndexes()<< endl;
			pobjIndex->deleteSubIndex(iSubIndexPos);
			
				/* Update subindex "00"*/
			if(pobjSIDx != NULL)
			{
				int iTotalSIdxs = 0;
				/* subindexes excluding "00"*/
				iTotalSIdxs = pobjIndex->getNumberofSubIndexes() - 1;

					
	
				/*if(strcmp(pobjSIDx->getIndexValue(), "00")!=0)
				{
					pobjSIDx = pobjIndex->getSubIndexbyIndexValue((char*)"00");
					SetSubIndexAttributes(iNodeID, CN, (char*)pobjIndex->getIndexValue(), (char*)pobjSIDx->getIndexValue(), _IntToAscii(iTotalSIdxs, pbAsciBuff, 16), (char*)pobjSIDx->getName(), TRUE);
				}*/
				UpdateNumberOfEnteriesSIdx(pobjIndex, enumNodeType);
			}
		
			stErrorInfo.code = OCFM_ERR_SUCCESS;
			return stErrorInfo;
		}
		catch(ocfmException& ex)
		{
			return ex._ocfmRetCode;
		}		
	}
/****************************************************************************************************
* Function Name: AddSubIndex
* Description:
* Return value: ocfmRetCode
****************************************************************************************************/
ocfmRetCode AddSubIndex(INT32 iNodeID, ENodeType enumNodeType, char* pbIndexID, char* pbSubIndexID)
	{
	//cout << "\nInside AddSubIndex\n" << endl;
		CNode objNode;		
		CNodeCollection *pobjNodeCollection;
		CIndexCollection *pobjIndexCollection;
		CIndex objIndex;
		INT32 iSubIndexPos;
		INT32 iIndexPos;
		ocfmRetCode stErrorInfo;
		
		try
		{
			/*if(NotLoadedOBD)
			{
			cout << "loading od.xml..." << endl;
				LoadObjectDictionary("od.xml");
				cout << "loaded od.xml..." << endl;
				NotLoadedOBD = false;
			}*/
			stErrorInfo = IfSubIndexExists(iNodeID, enumNodeType, pbIndexID, pbSubIndexID, &iSubIndexPos, &iIndexPos);
			if( stErrorInfo.code == OCFM_ERR_SUCCESS )
			{
				//stErrorInfo.code = OCFM_ERR_SUBINDEX_ALREADY_EXISTS;
				//return stErrorInfo;
				ocfmException objException;				
				objException.ocfm_Excpetion(OCFM_ERR_SUBINDEX_ALREADY_EXISTS);
				throw objException;
			}
			
			if( (stErrorInfo.code == OCFM_ERR_NO_SUBINDEXS_FOUND) || (stErrorInfo.code == OCFM_ERR_SUBINDEXID_NOT_FOUND))
			{
				
				pobjNodeCollection= CNodeCollection::getNodeColObjectPointer();
				objNode = pobjNodeCollection->getNode(enumNodeType, iNodeID);
				pobjIndexCollection = objNode.getIndexCollection();
				CIndex* pobjIndexPtr;
				pobjIndexPtr =pobjIndexCollection->getIndex(iIndexPos);
				CSubIndex* pobjSubIndex;
				CSubIndex* objDictSIdx;
				
				pobjSubIndex = new CSubIndex;
				//Set the iNodeID
				//pobjSubIndex.setNodeID(objNode.getNodeId());
				
				/* Get the SubIndex from ObjectDictionary*/
				CObjectDictionary* pobjOBD;
				pobjOBD = CObjectDictionary::getObjDictPtr();
				
				//cout << "\n\nobjOBD->ifObjectDictSubIndexExists(ppIndexID, ppSubIndexID):" << pobjOBD->ifObjectDictSubIndexExists(IndexID, SubIndexID) << endl;
				//if(pobjOBD->ifObjectDictSubIndexExists(pbIndexID, pbSubIndexID) == 1)
				//{
				//	//cout<< "\n\n\n$OCFM_ERR_INVALID_SUBINDEXID \n"<<endl;
				//	//stErrorInfo.code = OCFM_ERR_INVALID_SUBINDEXID;
				//	//return stErrorInfo;
				//	ocfmException objException;				
				//	objException.ocfm_Excpetion(OCFM_ERR_INVALID_SUBINDEXID);
				//	throw objException;
				//}	
				objDictSIdx = pobjOBD->getObjectDictSubIndex(pbIndexID, pbSubIndexID);
				if(objDictSIdx != NULL)
				{
					pobjSubIndex->setNodeID(iNodeID);								
					AddSubIndexAttributes(pbSubIndexID, pobjSubIndex, objDictSIdx);
					pobjIndexPtr->addSubIndex(*pobjSubIndex);					
				
				}
				else if( (enumNodeType == MN) && CheckIfProcessImageIdx(pbIndexID))
				{
					
						pobjSubIndex = new CSubIndex();
						pobjSubIndex->setNodeID(iNodeID);
						pobjSubIndex->setIndexValue(pbSubIndexID);
						if(pobjIndexPtr != NULL)
							pobjIndexPtr->addSubIndex(*pobjSubIndex);
				}
				else if(CheckIfManufactureSpecificObject(pbIndexID))
				{		
						pobjSubIndex = new CSubIndex;					
						pobjSubIndex->setNodeID(iNodeID);							
						pobjSubIndex->setIndexValue(pbSubIndexID);
											
						if(pobjIndexPtr != NULL)
						{
							if((pobjIndexPtr->getEObjectType() == ARRAY) && (strcmp(pbSubIndexID, "00") != 0))
							{
								//If objectType='ARRAY', all subobjects (except 0x00) have got the same dataType as the object
								pobjSubIndex->setDataTypeST(pobjIndexPtr->getDataType());
							}
                            //all the subobjects is of type VAR
                            pobjSubIndex->setObjectType((char*)"VAR");
							pobjSubIndex->setFlagIfIncludedCdc(TRUE);
							//printf(pobjIndexPtr->getIndexValue());
							pobjIndexPtr->addSubIndex(*pobjSubIndex);
						}
				}
				else
				{
						ocfmException objException;				
						objException.ocfm_Excpetion(OCFM_ERR_INVALID_SUBINDEXID);
						throw objException;
				}
				/* Update subindex "00"*/
					if(pobjSubIndex != NULL)
					{
						if(strcmp(pobjSubIndex->getIndexValue(), "00") != 0)
						UpdateNumberOfEnteriesSIdx(pobjIndexPtr, enumNodeType);
					}
			
				//AddSubIndexAttributes(pbSubIndexID, &pobjSubIndex);									
				
				//printf("Added SubIndex \n\n");
				stErrorInfo.code = OCFM_ERR_SUCCESS;
				return stErrorInfo;						
			}
			return stErrorInfo;
		}
		catch(ocfmException& ex)
		{
			return ex._ocfmRetCode;
		}
	}	

ocfmRetCode AddSubobject(INT32 iNodeID, ENodeType enumNodeType, char* pbIndexID)
{
    INT32 iSubIndexPos;
    INT32 iIndexPos;
    ocfmRetCode stErrorInfo;
    
    try
    {
        stErrorInfo = AddSubIndex(iNodeID, enumNodeType, pbIndexID, "00");
        if( (stErrorInfo.code == OCFM_ERR_SUCCESS) && (CheckIfManufactureSpecificObject(pbIndexID)) )
        {
            CNode objNode;      
            CNodeCollection *pobjNodeCollection;
            CIndexCollection *pobjIndexCollection;
            CIndex objIndex;
            CIndex* pobjSubIndex;
            INT32 iIndexPos;
            INT32 iSubIndexPos;
            ocfmRetCode stErrorInfo;
            
        
            stErrorInfo = IfSubIndexExists(iNodeID, enumNodeType, pbIndexID, "00", &iSubIndexPos, &iIndexPos);
            if(stErrorInfo.code == OCFM_ERR_SUCCESS)
            {
                
            }
            else
            {
                
                ocfmException objException;             
                objException.ocfm_Excpetion(OCFM_ERR_INVALID_SUBINDEXID);
                throw objException;
            }
                
            CSubIndex* pobjSubIndexPtr;
                
            pobjNodeCollection= CNodeCollection::getNodeColObjectPointer();
            objNode = pobjNodeCollection->getNode(enumNodeType, iNodeID);
    
            pobjIndexCollection = objNode.getIndexCollection();
            pobjSubIndex =pobjIndexCollection->getIndex(iIndexPos);
            
            pobjSubIndexPtr = pobjSubIndex->getSubIndex(iSubIndexPos);
            pobjSubIndexPtr->setName((char*)"NumberOfEntries");
            pobjSubIndexPtr->setObjectType((char*)"VAR");
            char* pbDataTypeName = new char[15];
            strcpy(pbDataTypeName, "UNSIGNED8");
            if( (CheckIfDataTypeByNameExists(pbDataTypeName, pobjSubIndexPtr->getNodeID())) == true)
            {
                pobjSubIndexPtr->setDataType(pbDataTypeName, iNodeID);
            }
            else
            {
                ocfmException objException;             
                objException.ocfm_Excpetion(OCFM_ERR_DATATYPE_NOT_FOUND);
                throw objException;
            }
        }
        stErrorInfo.code = OCFM_ERR_SUCCESS;
        return stErrorInfo;
    }
    catch(ocfmException& ex)
    {
        return ex._ocfmRetCode;
    }
}

char* getIndexName(char* ObjectIndex, char* ObjectName)
{
	char* Name = NULL;// = new char[100];
	char* ModifiedName = new char[strlen(ObjectName)];
	
	int len;
	//printf("\nObjectName %s", ObjectName);
	Name = strchr(ObjectName, 'X')	;
	if(Name != NULL)
	{
		len = 1;
		if(strcmp(subString(Name,1,1), "X") ==0)
		len++;
	
		int pos = strlen(Name);
		int i = 0;
		while(i < len)
		{
			//if(Name[0] == 'X' && len==2)
			if(Name[0] == 'X')
			{
				Name[0] = *(ObjectIndex + i);
			}
			Name++;
			i++;	
		}
		Name = Name-i;
		strcpy(ModifiedName, subString(ObjectName, 0, strlen(ObjectName)-pos));		
		strcat(ModifiedName, Name);	
		//printf("\n ModifiedName %s",ModifiedName);
		return ModifiedName;
	}
	else
	{ return ObjectName;}
}
/****************************************************************************************************
* Function Name: AddIndex
* Description:
* Return value: ocfmRetCode
****************************************************************************************************/
ocfmRetCode AddIndex(INT32 iNodeID, ENodeType enumNodeType, char* pbIndexID)
	{
		INT32 iIndexPos = 0;
		CNode objNode;		
		CNodeCollection *pobjNodeCollection;
		CIndexCollection *pobjIndexCollection;
		//CIndex* objIndex;
		CIndex* pobjDictIndex;
		ocfmRetCode stErrorInfo;
		stErrorInfo.code = OCFM_ERR_UNKNOWN;
		stErrorInfo.errorString = NULL;
		
		try 
		{		
			//cout<< "Inside AddIndex \n"<<endl;
			stErrorInfo = IfIndexExists(iNodeID, enumNodeType, pbIndexID, &iIndexPos);
			
			
			//cout << "\n\nErrStruct.code from IfIndexExists:" << stErrorInfo.code << endl;
			
			if( stErrorInfo.code == OCFM_ERR_SUCCESS )
			{
				//stErrorInfo.code = OCFM_ERR_INDEX_ALREADY_EXISTS;
				//return stErrorInfo;
				ocfmException objException;				
				objException.ocfm_Excpetion(OCFM_ERR_INDEX_ALREADY_EXISTS);
				throw objException;
			}
			
			if( (stErrorInfo.code == OCFM_ERR_NO_INDEX_FOUND) || (stErrorInfo.code == OCFM_ERR_INDEXID_NOT_FOUND) )
			{	
				//objIndex = new CIndex();
				CIndex objIndex;
				
				pobjNodeCollection= CNodeCollection::getNodeColObjectPointer();
				objNode = pobjNodeCollection->getNode(enumNodeType, iNodeID);
				pobjIndexCollection = objNode.getIndexCollection();
				/* Get the Index from ObjectDictionary*/
				CObjectDictionary* pobjOBD;
				pobjOBD = CObjectDictionary::getObjDictPtr();
				pobjDictIndex = pobjOBD->getObjectDictIndex(pbIndexID);				
				if(pobjDictIndex!=NULL)
				{
					objIndex.setNodeID(iNodeID);
					AddIndexAttributes(pbIndexID, &objIndex, pobjDictIndex);
					for(INT32 iLoopCount= 0; iLoopCount<pobjDictIndex->getNumberofSubIndexes(); iLoopCount++)
					{
						CSubIndex* objSIdx;
						objSIdx = pobjDictIndex->getSubIndex(iLoopCount);
						objIndex.addSubIndex(*objSIdx);
					}
	
					objIndex.setName(pobjOBD->getIndexName(subString(pbIndexID,2,4),(char*)objIndex.getName()));
					pobjIndexCollection->addIndex(objIndex);
				}
				else if((enumNodeType == MN) && CheckIfProcessImageIdx(pbIndexID))
				{	
						//objIndex = new CIndex();
						CIndex objIndex;
						objIndex.setNodeID(iNodeID);
						objIndex.setIndexValue(pbIndexID);
						pobjIndexCollection->addIndex(objIndex);
					 
				}
				else if(CheckIfManufactureSpecificObject(pbIndexID))
				{	
						//objIndex = new CIndex();
						CIndex objIndex;
						objIndex.setNodeID(iNodeID);
						objIndex.setIndexValue(pbIndexID);
						objIndex.setFlagIfIncludedCdc(TRUE);
						pobjIndexCollection->addIndex(objIndex);
						//SetAllIndexAttributes(iNodeID, enumNodeType, pbIndexID, NULL ,NULL,"rw", NULL, NULL, NULL, NULL, NULL,"8",FALSE);
						//stErrorInfo = AddSubIndex(iNodeID, enumNodeType, pbIndexID, "00");						
						//SetAllSubIndexAttributes(iNodeID, enumNodeType, pbIndexID, "00","0","NumberOfEntries", "rw", "unsigned8" ,NULL,"0",NULL,NULL, "7", FALSE);
				}
				else 
				{
					//stErrorInfo.code = OCFM_ERR_INVALID_INDEXID;
					//printf("\nAdded NOT Index \n\n");
					//return stErrorInfo;
					ocfmException objException;				
					objException.ocfm_Excpetion(OCFM_ERR_INVALID_INDEXID);
					throw objException;
				}
				//printf("Added Index \n\n");
				//return OCFM_ERR_SUCCESS;
								
				stErrorInfo.code = OCFM_ERR_SUCCESS;
				return stErrorInfo;
			}			
			return stErrorInfo;
		}
		catch(ocfmException& ex)
		{
			return ex._ocfmRetCode;
		}
	}	
/****************************************************************************************************
* Function Name: SetIndexAttributes
* Description:
* Return value: ocfmRetCode
****************************************************************************************************/
ocfmRetCode SetIndexAttributes(INT32 iNodeID, ENodeType enumNodeType,
 char* pbIndexID, char* pbIndexValue,
 char* pbIndexName, EFlag enumIsIncludedInCdc)
{
	INT32 iIndexPos;
	ocfmRetCode stErrorInfo;
	try 
	{
		stErrorInfo = IfIndexExists(iNodeID, enumNodeType, pbIndexID, &iIndexPos);
		if(stErrorInfo.code == OCFM_ERR_SUCCESS)
		{
			//iIndexPos = stErrorInfo.returnValue;
		}
		else
		{
			//OCFM_ERR_INDEXID_NOT_FOUND
			//return stErrorInfo;
			ocfmException objException;				
			objException.ocfm_Excpetion(OCFM_ERR_INDEXID_NOT_FOUND);
			throw objException;
		}
		CIndex* pobjIndexPtr;

		CNode objNode;		
		CNodeCollection *pobjNodeCollection;
		CIndexCollection *pobjIndexCollection;
		CIndex objIndex;
		
		objIndex.setNodeID(objNode.getNodeId());
		pobjNodeCollection= CNodeCollection::getNodeColObjectPointer();
		objNode = pobjNodeCollection->getNode(enumNodeType, iNodeID);
		pobjIndexCollection = objNode.getIndexCollection();
		pobjIndexPtr =pobjIndexCollection->getIndex(iIndexPos);			
		//cout << "EditIndexValue:Index Actual Value:" << pobjIndexPtr->getActualValue() << pbIndexValue << endl;
		/* Check if the value is valid*/
		if(pbIndexName != NULL)
			pobjIndexPtr->setName(pbIndexName);
		else
		{
			pobjIndexPtr->setName('\0');
		}
		
		pobjIndexPtr->setFlagIfIncludedCdc(enumIsIncludedInCdc);
		
		if(pbIndexValue != NULL)
		{
			//cout << "\npbIndexValue:" << pbIndexValue << endl;
			if(pobjIndexPtr->IsIndexVaueValid(pbIndexValue))
			{
				//printf("\nIndex value%s",pbIndexValue);
				pobjIndexPtr->setActualValue(pbIndexValue);
				
			//printf("EditIndexValue:Index Actual Value:%s-%s\n", pobjIndexPtr->getActualValue(), pbIndexValue);
				stErrorInfo.code = OCFM_ERR_SUCCESS;
			}
			else
			{
					ocfmException objException;				
					objException.ocfm_Excpetion(OCFM_ERR_VALUE_NOT_WITHIN_RANGE);
					throw objException;
					//stErrorInfo.code = OCFM_ERR_VALUE_NOT_WITHIN_RANGE;
			}
			return stErrorInfo;
		}
	}
	catch(ocfmException& ex)
	{
		return ex._ocfmRetCode;
	}
}

/****************************************************************************************************
* Function Name: SetSubIndexAttributes
* Description:
* Return value: ocfmRetCode
****************************************************************************************************/
ocfmRetCode SetSubIndexAttributes(INT32 iNodeID, 
								  ENodeType enumNodeType, 
								  char* pbIndexID, 
								  char* pbSubIndexID, 
								  char* pbIndexValue, 
								  char* pbIndexName, 
								  EFlag enumIsIncludedInCdc)
{
		CNode objNode;		
		CNodeCollection *pobjNodeCollection;
		CIndexCollection *pobjIndexCollection;
		CIndex objIndex;
		CIndex* pobjSubIndex;
		INT32 iIndexPos;
		INT32 iSubIndexPos;
		ocfmRetCode stErrorInfo;
		//CSubIndex* pobjSubIndex;
		
		try
		{
			stErrorInfo = IfSubIndexExists(iNodeID, enumNodeType, pbIndexID, pbSubIndexID, &iSubIndexPos, &iIndexPos);
			if(stErrorInfo.code == OCFM_ERR_SUCCESS)
			{
				//iSubIndexPos = Tmp_stErrorInfo.returnValue;
			}
			else
			{
				//OCFM_ERR_INVALID_SUBINDEXID
				ocfmException objException;				
				objException.ocfm_Excpetion(OCFM_ERR_INVALID_SUBINDEXID);
				throw objException;
//				return stErrorInfo;
			}
			
            if((iNodeID == 240) && (enumNodeType == MN ) && (strcmp(pbIndexID,"1F92") == 0) && (strcmp(pbSubIndexID,"00") != 0) )
            {
                if(false == ValidateCNPresTimeout(pbSubIndexID, pbIndexValue))
                {
                    ocfmException objException;             
                    objException.ocfm_Excpetion(OCFM_ERR_LOW_CNPRESTIMEOUT);
                    throw objException;
                }
            }

			CSubIndex* pobjSubIndexPtr;
				
			objIndex.setNodeID(objNode.getNodeId());
			pobjNodeCollection= CNodeCollection::getNodeColObjectPointer();
			objNode = pobjNodeCollection->getNode(enumNodeType, iNodeID);

			pobjIndexCollection = objNode.getIndexCollection();
			pobjSubIndex =pobjIndexCollection->getIndex(iIndexPos);
			
			pobjSubIndexPtr = pobjSubIndex->getSubIndex(iSubIndexPos);						
			//printf("SubIndexValue:%s-%s\n", pobjSubIndexPtr->getName(), pbSubIndexID);
			
			if(pbIndexName !=NULL)
			{
				pobjSubIndexPtr->setName(pbIndexName);
			}
			
			pobjSubIndexPtr->setFlagIfIncludedCdc(enumIsIncludedInCdc);
			
			if(pbIndexValue != NULL)
			{
			if(pobjSubIndexPtr->IsIndexVaueValid(pbIndexValue))		
				{
				//printf("\nIndex value%s",pbIndexValue);
				pobjSubIndexPtr->setActualValue(pbIndexValue);
				stErrorInfo.code = OCFM_ERR_SUCCESS;
				}
			}
			else
			{
				ocfmException objException;				
				objException.ocfm_Excpetion(OCFM_ERR_VALUE_NOT_WITHIN_RANGE);
				throw objException;
				//stErrorInfo.code =  OCFM_ERR_VALUE_NOT_WITHIN_RANGE;
			}
			//pobjSubIndexPtr->setName(pbIndexName);
			//printf("SubIndexValue:%s-%s\n", objSubIndexPtr->getName(), SubIndexID);
			/*ErrStruct.code = OCFM_ERR_SUCCESS;*/
			if((iNodeID == 240) && (enumNodeType == MN ) && (strcmp(pbIndexID,"1F98") == 0) && (strcmp(pbSubIndexID,"07") == 0) )
			{
				RecalculateMultiplex();
			}
            if((iNodeID == 240) && (enumNodeType == MN ) && (strcmp(pbIndexID,"1F92") == 0) && (strcmp(pbSubIndexID,"00") != 0) )
            {
                RecalculateCNPresTimeout(pbSubIndexID);
            }
			return stErrorInfo;
		}
		catch(ocfmException& ex)
		{
			return ex._ocfmRetCode;
		}
}
/**************************************************************************************************
* Function Name: SetALLIndexAttributes
* Description:
* Return value: ocfmRetCode
****************************************************************************************************/
ocfmRetCode SetAllIndexAttributes(INT32 iNodeID, 
									ENodeType enumNodeType, 
									char* pbIndexID, 
									char* pbActualValue,
									char* pbIndexName, 
									char* pbAccess, 
									char* pbDataTypeName,
									char* pdoMappingVal, 
									char* defaultValue, 
									char* highLimit,
									char* lowLimit, 
									char* objType, 
									EFlag enumIsIncludedInCdc)
{
	INT32 iIndexPos;
	ocfmRetCode stErrorInfo;
	try 
	{
		stErrorInfo = IfIndexExists(iNodeID, enumNodeType, pbIndexID, &iIndexPos);
		if(stErrorInfo.code == OCFM_ERR_SUCCESS)
		{
			//iIndexPos = stErrorInfo.returnValue;
		}
		else
		{
			//OCFM_ERR_INDEXID_NOT_FOUND
			//return stErrorInfo;
			ocfmException objException;				
			objException.ocfm_Excpetion(OCFM_ERR_INDEXID_NOT_FOUND);
			throw objException;
		}
		CIndex* pobjIndexPtr;

		CNode objNode;		
		CNodeCollection *pobjNodeCollection;
		CIndexCollection *pobjIndexCollection;
		CIndex objIndex;
		
		objIndex.setNodeID(objNode.getNodeId());
		pobjNodeCollection= CNodeCollection::getNodeColObjectPointer();
		objNode = pobjNodeCollection->getNode(enumNodeType, iNodeID);
		pobjIndexCollection = objNode.getIndexCollection();
		pobjIndexPtr =pobjIndexCollection->getIndex(iIndexPos);			
		//cout << "EditIndexValue:Index Actual Value:" << pobjIndexPtr->getActualValue() << pbIndexValue << endl;
		/* Check if the value is valid*/
		if(pbIndexName!= NULL)
		{
			if(strcmp(pbIndexName, "") != 0)
				pobjIndexPtr->setName(pbIndexName);
		}
		
		if(pbAccess != NULL)
		{
			if(strcmp(pbAccess, "") != 0)
				pobjIndexPtr->setAccessType(pbAccess);
		}
		
		if(pdoMappingVal != NULL)
		{
			pobjIndexPtr->setPDOMapping(pdoMappingVal);
		}
		if(defaultValue != NULL)
		{
			pobjIndexPtr->setDefaultValue(defaultValue);
		}

        ocfmRetCode stErrorLimitInfo;
		stErrorLimitInfo = CheckUpperAndLowerLimits(lowLimit, highLimit);
        if(stErrorLimitInfo.code == OCFM_ERR_SUCCESS)
        {
            //
        }
        else
        {
            return stErrorLimitInfo;
        }
		if(highLimit != NULL)
		pobjIndexPtr->setHighLimit(highLimit);
		
		if(lowLimit != NULL)
		pobjIndexPtr->setLowLimit(lowLimit);
		
		if(objType != NULL)
		{
			pobjIndexPtr->setObjectType(objType);
			if(pobjIndexPtr->getEObjectType() ==  ARRAY)
			{
				pobjIndexPtr->UpdateArraySubObjects();
			}
		}
		
		pobjIndexPtr->setFlagIfIncludedCdc(enumIsIncludedInCdc);
		
        if((pobjIndexPtr->getEObjectType() ==  ARRAY) || (pobjIndexPtr->getEObjectType() ==  RECORD) )
        {
            AddSubobject(iNodeID, enumNodeType, pbIndexID);
        }

		if(pbDataTypeName != NULL)
		{
			if(strcmp(pbDataTypeName, "") !=0)
			{
				if( (CheckIfDataTypeByNameExists(pbDataTypeName, pobjIndexPtr->getNodeID())) == true)
				{
					pobjIndexPtr->setDataType(pbDataTypeName, iNodeID);
					DataType objDataType;
					objDataType = pobjIndexPtr->getDataType();
					if(pobjIndexPtr->getEObjectType() ==  ARRAY)
					{
						pobjIndexPtr->UpdateArraySubObjects();
					}
					//printf("\n name %s", objDataType.getName());
				}
				else
				{
					ocfmException objException;				
					objException.ocfm_Excpetion(OCFM_ERR_DATATYPE_NOT_FOUND);
					throw objException;
				}
			}
		}
		if(pbActualValue != NULL)
		{		
		if(pobjIndexPtr->IsIndexVaueValid(pbActualValue))
		{
			//printf("\nIndex value%s",pbIndexValue);
			pobjIndexPtr->setActualValue(pbActualValue);
			
		//printf("EditIndexValue:Index Actual Value:%s-%s\n", pobjIndexPtr->getActualValue(), pbIndexValue);
			stErrorInfo.code = OCFM_ERR_SUCCESS;
		}
		else
		{
				ocfmException objException;				
				objException.ocfm_Excpetion(OCFM_ERR_VALUE_NOT_WITHIN_RANGE);
				throw objException;
				//stErrorInfo.code = OCFM_ERR_VALUE_NOT_WITHIN_RANGE;
		}
	}
		return stErrorInfo;
	}
	catch(ocfmException& ex)
	{
		return ex._ocfmRetCode;
	}
}

/****************************************************************************************************
	* Function Name: EditSubIndex
    * Description:
	* Return value: ocfmRetCode
****************************************************************************************************/
ocfmRetCode SetAllSubIndexAttributes(INT32 iNodeID, 
									ENodeType enumNodeType, 
									char* pbIndexID, 
									char* pbSubIndexID,
									char* pbActualValue, 
									char* pbIndexName,
									char* pbAccess, 
									char* pbDataTypeName,
									char* pbPdoMappingVal, 
									char* pbDefaultValue, 
									char* pbHighLimit,
									char* pbLowLimit, 
									char* pbObjType, 
									EFlag enumIsIncludedInCdc)
{
		CNode objNode;		
		CNodeCollection *pobjNodeCollection;
		CIndexCollection *pobjIndexCollection;
		CIndex objIndex;
		CIndex* pobjSubIndex;
		INT32 iIndexPos;
		INT32 iSubIndexPos;
		ocfmRetCode stErrorInfo;
		//CSubIndex* pobjSubIndex;
		
		try
		{			
			stErrorInfo = IfSubIndexExists(iNodeID, enumNodeType, pbIndexID, pbSubIndexID, &iSubIndexPos, &iIndexPos);
			
			if(stErrorInfo.code == OCFM_ERR_SUCCESS)
			{
				//iSubIndexPos = Tmp_stErrorInfo.returnValue;
			}
			else
			{
				//OCFM_ERR_INVALID_SUBINDEXID
				ocfmException objException;				
				objException.ocfm_Excpetion(OCFM_ERR_INVALID_SUBINDEXID);
				throw objException;
//				return stErrorInfo;
			}
			
			CSubIndex* pobjSubIndexPtr;
				
			objIndex.setNodeID(objNode.getNodeId());
			pobjNodeCollection= CNodeCollection::getNodeColObjectPointer();
			objNode = pobjNodeCollection->getNode(enumNodeType, iNodeID);

			pobjIndexCollection = objNode.getIndexCollection();
			pobjSubIndex =pobjIndexCollection->getIndex(iIndexPos);
			
			pobjSubIndexPtr = pobjSubIndex->getSubIndex(iSubIndexPos);						
			//printf("SubIndexValue:%s-%s\n", pobjSubIndexPtr->getName(), pbSubIndexID);
			
			if(pbIndexName!= NULL)
			{
				pobjSubIndexPtr->setName(pbIndexName);
			}
			
			if(pbAccess != NULL)
			{
				pobjSubIndexPtr->setAccessType(pbAccess);	
			}
			
			
			if(pbPdoMappingVal != NULL)
			{
				pobjSubIndexPtr->setPDOMapping(pbPdoMappingVal);
			}
			
			if(pbDefaultValue != NULL)
			{
				pobjSubIndexPtr->setDefaultValue(pbDefaultValue);
			}
			
            ocfmRetCode stErrorLimitInfo;
            stErrorLimitInfo = CheckUpperAndLowerLimits(pbLowLimit, pbHighLimit);
            if(stErrorLimitInfo.code == OCFM_ERR_SUCCESS)
            {
                //
            }
            else
            {
                return stErrorLimitInfo;
            }
			if(pbHighLimit != NULL)
			{
				pobjSubIndexPtr->setHighLimit(pbHighLimit);
			}
			
			if(pbLowLimit != NULL)
			{
				pobjSubIndexPtr->setLowLimit(pbLowLimit);
			}
			
			if(pbObjType != NULL)
			{
				pobjSubIndexPtr->setObjectType(pbObjType);
			}
			
			pobjSubIndexPtr->setFlagIfIncludedCdc(enumIsIncludedInCdc);
			
			if(pbActualValue != NULL)
			{
				if(pobjSubIndexPtr->IsIndexVaueValid(pbActualValue))		
				{
					//printf("\nIndex value%s",pbActualValue);
					pobjSubIndexPtr->setActualValue(pbActualValue);
					stErrorInfo.code = OCFM_ERR_SUCCESS;
				}
				else
				{
					ocfmException objException;				
					objException.ocfm_Excpetion(OCFM_ERR_VALUE_NOT_WITHIN_RANGE);
					throw objException;
					//stErrorInfo.code =  OCFM_ERR_VALUE_NOT_WITHIN_RANGE;
				}
			}
			if(pbDataTypeName != NULL)
			{
			if(strcmp(pbDataTypeName, "") !=0)
			{
					if( (CheckIfDataTypeByNameExists(pbDataTypeName, pobjSubIndexPtr->getNodeID())) == true)
					{
						pobjSubIndexPtr->setDataType(pbDataTypeName, iNodeID);
					}
					else
					{
						ocfmException objException;				
						objException.ocfm_Excpetion(OCFM_ERR_DATATYPE_NOT_FOUND);
						throw objException;
					}
				}
			}
			

			//pobjSubIndexPtr->setName(pbIndexName);
			//printf("SubIndexValue:%s-%s\n", pobjSubIndexPtr->getName(), pbSubIndexID);
			/*stErrorInfo.code = OCFM_ERR_SUCCESS;*/
			return stErrorInfo;
		}
		catch(ocfmException& ex)
		{
			return ex._ocfmRetCode;
		}
}


ocfmRetCode CheckUpperAndLowerLimits(char* pcLowLimit, char* pcHighLimit)
{
    ocfmRetCode stError;
    stError.code = OCFM_ERR_SUCCESS;

    unsigned long ulLowlimit;
    unsigned long ulHighLimit;

    if(pcLowLimit != NULL && pcHighLimit!= NULL)
    {       
        if( (strcmp(pcLowLimit,"") != 0) && (strcmp(pcHighLimit,"") != 0) )
        {
            if(CheckIfHex((char*)pcLowLimit))
            {
                ulLowlimit = hex2int(subString((char*)pcLowLimit, 2, strlen(pcLowLimit) -2));       
            }
            else
            {
                ulLowlimit = atoi(pcLowLimit);
            }
            if(CheckIfHex((char*)pcHighLimit))
            {
                ulHighLimit = hex2int(subString((char*)pcHighLimit, 2, strlen(pcHighLimit) -2));       
            }
            else
            {
                ulHighLimit = atoi(pcHighLimit);
            }

            if(ulHighLimit >= ulLowlimit)
            {
                return stError;
            }
            else
            {
                stError.code = OCFM_ERR_INVALID_UPPERLOWER_LIMITS;    
                stError.errorString = new char[150];
                stError.errorString[0] = 0;
                sprintf(stError.errorString, "The lower limit(%s) is greater than upperlimit(%s)", pcLowLimit, pcHighLimit);
                return stError;
            }
        }
    }
    return stError;
}
/****************************************************************************************************
* Function Name: DisplayNodeTree
* Description:
* Return value: void
****************************************************************************************************/
void DisplayNodeTree()
	{
		CNode objNode;		
		CNodeCollection *pobjNodeCollection;
		CIndexCollection *pobjIndexCollection;
		
		pobjNodeCollection= CNodeCollection::getNodeColObjectPointer();
		pobjIndexCollection = objNode.getIndexCollection();
		
		if( pobjNodeCollection->getNumberOfNodes() > 0)
		{
			for(INT32 iLoopCount = 0; iLoopCount < pobjNodeCollection->getNumberOfNodes(); iLoopCount++)
			{
					objNode = pobjNodeCollection->getNodebyCollectionIndex(iLoopCount);
					if (objNode.getNodeType() == CN)
					{						
						//INT32 tmp_NodeID = objNode.getNodeId();
						//objNode =pobjNodeCollection->getNode(CN,tmp_NodeID);
						//printf("iNodePos:%d, iNodeID:%d\n", iLoopCount, objNode.getNodeId());
					}						
			}
		}
		else
		{
				//printf("No Nodes found!\n");
				return;
		}
			
	}	
	
/****************************************************************************************************
* Function Name: GetIndexData
* Description: 
* Return value: void
****************************************************************************************************/
//void GetIndexData(CIndex* pobjIndex, char* pbBuffer)
//	{
//			INT32 iLength;
//			bool IfStringDT = false;
//			//Get the Index Value		
//			
//			/*if(ppobjIndex->getNumberofSubIndexes()==0 &&(objIndex->getDefaultValue()!= NULL ||
//																																													 pobjIndex->getActualValue()!=NULL))*/
//			strcpy(pbBuffer,"");
//			if(pobjIndex->getNumberofSubIndexes()==0 && pobjIndex->getActualValue()!=NULL)
//			
//			{
//					//pbBuffer = (char*)malloc(sizeof(pobjIndex->getIndexValue()+1));
//					strcpy(pbBuffer,pobjIndex->getIndexValue());

//					//Place a tab			
//					iLength = strlen(pbBuffer);
//					//pbBuffer =(char*)realloc(pbBuffer,2);
//					strcat(pbBuffer,"\t");
//					//printf("\n%s",pbBuffer);

//					//If subindexes are none, add "00"
//					//	pbBuffer =(char*)realloc(pbBuffer,4);
//						strcat(pbBuffer,"00");
//						strcat(pbBuffer,"\t");

//							//Add datatype
//							DataType objDataType;		
//							//pbBuffer =(char*)realloc(pbBuffer,6);	
//							objDataType = pobjIndex->getDataType();
//							INT32 iPadLength=0;
//							char* size = new char[8 + ALLOC_BUFFER];
//							if(objDataType.Name != NULL)
//							{
//			//					objDataType.DataSize = padLeft(objDataType.DataSize, '0', 4);
//								//strcat(Buffer ,dt.DataSize);	
//								if(!checkIfStringDatatypes(objDataType.Name)) 
//								{
//									size  = _IntToAscii(objDataType.DataSize, size, 16); 
//									size = padLeft(size, '0', 8);
//									strcat(pbBuffer, size);	
//									iPadLength = objDataType.DataSize*2;
//									IfStringDT = false;
//									}
//									else
//									{			
//										int iLength = strlen(pobjIndex->getActualValue());							
//										size = _IntToAscii(iLength, size, 16);
//										size = padLeft(size, '0', 8);
//										strcat(pbBuffer, size);	
//										iPadLength = iLength*2;
//										IfStringDT = true;
//									}
//							}
//							
//							else strcat(pbBuffer,"00000000");
//							delete[] size;
//							
//							strcat(pbBuffer,"\t");
//						
//							if (pobjIndex->getActualValue()!=NULL)
//							{	
//										char abActvalue[20];
//										abActvalue[0]  = '\0';
//										if(IfStringDT)
//										{
//											strcat(pbBuffer,(char*)pobjIndex->getActualValue());
//										}
//										else
//										{
//										if(CheckIfHex((char*)pobjIndex->getActualValue()))
//										//if(abActvalue!=NULL)
//										{
//											INT32 iLength = strlen((char*)pobjIndex->getActualValue());
//											strncpy(abActvalue,(pobjIndex->getActualValue()+ 2),iLength-2 );
//											abActvalue[iLength -2] ='\0';
//											strcat(pbBuffer,padLeft(abActvalue,'0',iPadLength));
//												
//										}
//						
//										else
//										{
//									 
//										strcpy(abActvalue, _IntToAscii(atoi(pobjIndex->getActualValue()),abActvalue,16));
//										//printf("\n ACT Value%s",abActvalue);
//									
//												//strcpy(abActvalue, _IntToAscii(atoi(pobjIndex->getActualValue()),abActvalue,16));
//										strcat(pbBuffer,padLeft(abActvalue, '0', iPadLength));
//									
//								}
//							}
//					
//									strcat(pbBuffer,"\n");
//				}	
//		}
//			else
//			{
//				INT32 iNoOfSubIndexes 		= pobjIndex->getNumberofSubIndexes();
//				bool bIsIndexAdded  		= false;				
//				bool bIsResetValueAdded 	= false;
//				bool bIsNoOfEnteriesAdded 	= false;
//				INT32 iLoopCount;
//				CSubIndex* pobjSubIndex;

//				for(iLoopCount=0; iLoopCount < iNoOfSubIndexes ; iLoopCount++)
//				{
//					pobjSubIndex = pobjIndex->getSubIndex(iLoopCount);
//				
//					/*if(strcmp(ppobjSubIndex->getIndexValue(),"00")!=0 && objSubIndex->getActualValue() != NULL)*/
//					if(pobjSubIndex->getActualValue() != NULL)
//			
//					{
//						if (bIsIndexAdded)
//						{
//							strcat(pbBuffer,pobjIndex->getIndexValue());
//						}
//						else
//						{
//							strcpy(pbBuffer,pobjIndex->getIndexValue());
//							bIsIndexAdded = true;
//						}
//											
//						//Place a tab			
//						iLength = strlen(pbBuffer);
//						strcat(pbBuffer,"\t");
//						//printf("%s",pbBuffer);

//							/*if(ppobjSubIndex->getActualValue() != NULL || objSubIndex->getDefaultValue()!=NULL)*/
//								strcat(pbBuffer, pobjSubIndex->getIndexValue());
//								strcat(pbBuffer,"\t");
//									//Add datatype
//								DataType objDataType;			
//								objDataType = pobjSubIndex->getDataType();
//								INT32 iPadLength=0;
//								char* size = new char[8 + ALLOC_BUFFER];
//							
//								if(objDataType.Name != NULL)
//								{
//									if(!checkIfStringDatatypes(objDataType.Name)) 
//								{
//									size  = _IntToAscii(objDataType.DataSize, size, 16); 
//									size = padLeft(size, '0', 8);
//									strcat(pbBuffer, size);	
//									iPadLength = objDataType.DataSize*2;
//									IfStringDT = false;
//									}
//									else
//									{			
//										int iLength = strlen(pobjIndex->getActualValue());							
//										size = _IntToAscii(iLength, size, 16);
//										size = padLeft(size, '0', 8);
//										strcat(pbBuffer, size);	
//										iPadLength = iLength*2;
//										IfStringDT = true;
//									}
//								}
//							
//								/*if(dt.Name != NULL)
//								{
//									strcat(Buffer ,dt.DataSize);				
//									padLength = hex2int(dt.DataSize)*2;
//								}*/
//								else strcat(pbBuffer,"0000");
//								delete[] size;
//								strcat(pbBuffer,"\t");
//							
//								if (pobjSubIndex->getActualValue()!=NULL)
//								{

//									// Add the reset value for that Index,SubIndex
//									if(strcmp(pobjSubIndex->getIndexValue(),"00")==0 && CheckIfMappingPDO((char*)pobjIndex->getIndexValue()) &&
//												bIsResetValueAdded==false )
//									{
//										char abActvalue[20];

//										strcpy(abActvalue,"0");
//										strcat(pbBuffer,padLeft(abActvalue,'0',iPadLength));
//										bIsResetValueAdded = true;
//										//iLoopCount--;
//									}
//									else
//									{
//										
//										char abActvalue[20];
//										abActvalue[0]  = '\0';
//										if(IfStringDT)
//										{
//											strcat(pbBuffer,(char*)pobjSubIndex->getActualValue());
//										}
//										else
//										{
//										if(CheckIfHex((char*)pobjSubIndex->getActualValue()))
//										//if(abActvalue!=NULL)
//										{
//											INT32 iLength = strlen((char*)pobjSubIndex->getActualValue());
//											strncpy(abActvalue,(pobjSubIndex->getActualValue()+ 2),iLength-2 );
//											abActvalue[iLength -2] ='\0';
//										
//											//printf("\n ACT Value%s",abActvalue);
//											strcat(pbBuffer,padLeft(abActvalue,'0',iPadLength));
//										
//										}
//											else
//											{				
//												//abActvalue = new char[50];									
//												strcpy(abActvalue, _IntToAscii(atoi(pobjSubIndex->getActualValue()),abActvalue,16));
//												//printf("\n ACT Value%s",abActvalue);
//									
//												strcat(pbBuffer,padLeft(abActvalue, '0', iPadLength));
//											}
//										
//										}
//										
//								}
//									/*else strcat(pbBuffer,pobjSubIndex->getDefaultValue());*/
//								strcat(pbBuffer,"\n");
//								
//								if(iLoopCount == 0 && (CheckIfMappingPDO((char*)pobjIndex->getIndexValue()) &&(bIsNoOfEnteriesAdded==true)))
//								{
//									iLoopCount = iNoOfSubIndexes-1;								
//								}
//								if(iLoopCount == iNoOfSubIndexes-1 && (CheckIfMappingPDO((char*)pobjIndex->getIndexValue()) && (bIsNoOfEnteriesAdded==false)))
//								{
//									iLoopCount = -1;	
//									bIsNoOfEnteriesAdded = true;							
//								}
//							}
//						}					
//						
//				}
//	}
//}

void GetIndexData(CIndex* objIndex, char* Buffer)
	{
			int len;
			bool IfStringDT = false;
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
							char* size = new char[8 + ALLOC_BUFFER];
							if(dt.Name != NULL)
							{
								//dt.DataSize = padLeft(dt.DataSize, '0', 4);
								//strcat(Buffer ,dt.DataSize);	
								if(!checkIfStringDatatypes(dt.Name)) 
								{
									size  = _IntToAscii(dt.DataSize, size, 16); 
									size = padLeft(size, '0', 8);
									strcat(Buffer, size);	
									padLength = dt.DataSize*2;
									IfStringDT = false;
									}
									else
									{			
										int len = strlen(objIndex->getActualValue());							
										size = _IntToAscii(len, size, 16);
										size = padLeft(size, '0', 8);
										strcat(Buffer, size);	
										padLength = len*2;
										IfStringDT = true;
									}
							}
							
							else strcat(Buffer,"00000000");
							delete[] size;
							
							strcat(Buffer,"\t");
						
							if (objIndex->getActualValue()!=NULL)
							{	
										char actvalue[20];
										actvalue[0]  = '\0';
										if(IfStringDT)
										{
											strcat(Buffer,(char*)objIndex->getActualValue());
										}
										else
										{
											if(CheckIfHex((char*)objIndex->getActualValue()))
											//if(actvalue!=NULL)
											{
												int len = strlen((char*)objIndex->getActualValue());
												strncpy(actvalue,(objIndex->getActualValue()+ 2),len-2 );
												actvalue[len -2] ='\0';								
												strcat(Buffer,padLeft(actvalue,'0',padLength));
												
											}										
						
										else
										{
									 
											strcpy(actvalue, _IntToAscii(atoi(objIndex->getActualValue()),actvalue,16));
											//printf("\n ACT Value%s",actvalue);															
											strcat(Buffer,padLeft(actvalue, '0', padLength));
										
									}
								}
							}
					
									strcat(Buffer,"\n");
				}	
			/* If Subobjects present*/
			else
			{
				int noOfSubIndexes = 0; //= objIndex->getNumberofSubIndexes();	
				int noOfTotalSubIndexes = objIndex->getNumberofSubIndexes();
			
				bool Indexadded  = false;				
				bool resetValueAdded = false;
				bool flag_No_of_enteriesAdded = false;
				int i;
				CSubIndex* objSubIndex;
				objSubIndex = objIndex->getSubIndexbyIndexValue((char*)"00");
				if(objSubIndex != NULL)
				{
					if(objSubIndex->getActualValue() != NULL)
					{
						if(CheckIfHex((char*)objSubIndex->getActualValue()))
						{
						//	cout << "hex value" << objSubIndex->getActualValue();
							noOfSubIndexes = hex2int(subString((char*)objSubIndex->getActualValue(), 2, strlen(objSubIndex->getActualValue()) -2));
							
						}
						else
							noOfSubIndexes = atoi(objSubIndex->getActualValue());
                            //printf("\n GetIndexdata Indxeid=%s noOfSubIndexes=%d\n", objIndex->getIndexValue(), noOfSubIndexes);
                            if(noOfSubIndexes ==0)
                            return;
							noOfSubIndexes = noOfSubIndexes + 1;
					}
				}
				int noOfValidSubIndexes = 0;
				
				for(i=0; i<noOfTotalSubIndexes ; i++)
				{
					
					objSubIndex = objIndex->getSubIndex(i);
				
					/*if(strcmp(objSubIndex->getIndexValue(),"00")!=0 && objSubIndex->getActualValue() != NULL)*/
					//$S_:TODO
					if((objSubIndex->getActualValue() != NULL) && (objSubIndex->getFlagIfIncludedCdc() == TRUE))			
					//if((objSubIndex->getActualValue() != NULL))
					{

						noOfValidSubIndexes = noOfValidSubIndexes + 1;
						
						if(noOfValidSubIndexes == noOfSubIndexes)
						{
							i= noOfTotalSubIndexes -1;
						
						}
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
	
							/*if(objSubIndex->getActualValue() != NULL || objSubIndex->getDefaultValue()!=NULL)*/
								strcat(Buffer, objSubIndex->getIndexValue());
								strcat(Buffer,"\t");
									//Add datatype
								DataType dt;			
								dt = objSubIndex->getDataType();
								int padLength=0;
							char* size = new char[8 + ALLOC_BUFFER];
							
								if(dt.Name != NULL)
								{
									if(!checkIfStringDatatypes(dt.Name)) 
								{
									size  = _IntToAscii(dt.DataSize, size, 16); 
									size = padLeft(size, '0', 8);
									strcat(Buffer, size);	
									padLength = dt.DataSize*2;
									IfStringDT = false;
									}
									else
									{			
										int len = strlen(objIndex->getActualValue());							
										size = _IntToAscii(len, size, 16);
										size = padLeft(size, '0', 8);
										strcat(Buffer, size);	
										padLength = len*2;
											IfStringDT = true;
									}
								}
					
								/*if(dt.Name != NULL)
								{
									strcat(Buffer ,dt.DataSize);				
									padLength = hex2int(dt.DataSize)*2;
								}*/
								else strcat(Buffer,"00000000");
								delete[] size;
								strcat(Buffer,"\t");
			
//                                    if(i ==0 && (CheckIfMappingPDO((char*)objIndex->getIndexValue()) ) ) 
//                                    printf("\n******subindex 00 %s\n",objSubIndex->getActualValue());
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
										if(IfStringDT)
										{
											strcat(Buffer,(char*)objSubIndex->getActualValue());
										}
										else
										{
											if(CheckIfHex((char*)objSubIndex->getActualValue()))										
											{
												int len = strlen((char*)objSubIndex->getActualValue());
												strncpy(actvalue,(objSubIndex->getActualValue()+ 2),len-2 );
												actvalue[len -2] ='\0';
											
												//printf("\n ACT Value%s",actvalue);
												strcat(Buffer,padLeft(actvalue,'0',padLength));
											
											}
												else
												{				
													//actvalue = new char[50];									
													strcpy(actvalue, _IntToAscii(atoi(objSubIndex->getActualValue()),actvalue,16));
													//printf("\n ACT Value%s",actvalue);
										
													strcat(Buffer,padLeft(actvalue, '0', padLength));
												}
											}
										
										}
										
								}
					
									/*else strcat(Buffer,objSubIndex->getDefaultValue());*/
								strcat(Buffer,"\n");
								
								if(i == 0 && (CheckIfMappingPDO((char*)objIndex->getIndexValue()) &&(flag_No_of_enteriesAdded==true)))
								{
									i = noOfTotalSubIndexes-1;								
								}
								if(i == noOfTotalSubIndexes-1 && (CheckIfMappingPDO((char*)objIndex->getIndexValue()) && (flag_No_of_enteriesAdded==false) && resetValueAdded == true))
								{
                                        //printf("\n*set for adding actual entereis");
									i = -1;	
									flag_No_of_enteriesAdded = true;							
								}
							}
						}					
						
				}
	}
		

/****************************************************************************************************
* Function Name: UpdateCNCycleTime
* Description: Updates the cycle time(1006) of the CN
* Return value: void
****************************************************************************************************/

	void UpdateCNCycleTime(CIndexCollection  *pobjIdxCol,char* pbCycleTime)
	{
		CIndex* pobjIndex;

		pobjIndex = pobjIdxCol->getIndexbyIndexValue((char*) "1006");
		if(pobjIndex!= NULL)
		{
			pobjIndex->setActualValue(pbCycleTime);
		}
		
	}

/****************************************************************************************************
* Function Name: UpdateCNAsyncMTUsize
* Description: Updates the Asynchronous MTU size(1F98/08) of the CN
* Return value: void
****************************************************************************************************/

    void UpdateCNAsyncMTUsize(CIndexCollection  *pobjIdxCol,char* pbAsyncMTUsize)
    {
        CIndex* pobjIndex;

        pobjIndex = pobjIdxCol->getIndexbyIndexValue((char*) "1F98");
        if(pobjIndex!= NULL && NULL != pbAsyncMTUsize )
        {   
            CSubIndex* pobjSubIndex;
            pobjSubIndex = pobjIndex->getSubIndexbyIndexValue((char*) "08");
            pobjSubIndex->setActualValue(pbAsyncMTUsize);
        }
        
    }

/****************************************************************************************************
* Function Name: UpdateCNMultiPrescal
* Description: Updates the Multilexing prescaler (1F98/07) of the CN
* Return value: void
****************************************************************************************************/

    void UpdateCNMultiPrescal(CIndexCollection  *pobjIdxCol,char* pbMultiPrescal)
    {
        CIndex* pobjIndex;

        pobjIndex = pobjIdxCol->getIndexbyIndexValue((char*) "1F98");
        if(pobjIndex!= NULL && NULL != pbMultiPrescal)
        {   
            CSubIndex* pobjSubIndex;
            pobjSubIndex = pobjIndex->getSubIndexbyIndexValue((char*) "07");
            pobjSubIndex->setActualValue(pbMultiPrescal);
        }
        
    }

/****************************************************************************************************
* Function Name: UpdateCNNodeAssignment
* Description: copies all the subobjects of 1F81 in MN to CN if conditions are satisfied
* Return value: void
****************************************************************************************************/

void UpdateCNNodeAssignment(CNode*  pobjNode)
{
    ocfmRetCode stErrorInfo;
    INT32 iIndexPos, iSubIndexPos;
    bool bCopy1F81 = false;
    int iNodeId = pobjNode->getNodeId();
    ENodeType enumNodeType = pobjNode->getNodeType();

    stErrorInfo = IfIndexExists(iNodeId, enumNodeType, (char*)"1F9B", &iIndexPos);
    if( OCFM_ERR_SUCCESS == stErrorInfo.code )
    {
        stErrorInfo = IfSubIndexExists(iNodeId, enumNodeType, (char*)"1F98", (char*)"07", &iSubIndexPos, &iIndexPos);
        if( OCFM_ERR_SUCCESS == stErrorInfo.code )
        {
            char* pcMultiplCycleCnt = new char[20];
            if( OCFM_ERR_SUCCESS == (GetSubIndexAttributes(iNodeId, enumNodeType, (char*)"1F98", (char*)"07", ACTUALVALUE, pcMultiplCycleCnt)).code );
            {
                if( ( pcMultiplCycleCnt != NULL ) && ( 0 != strcmp(pcMultiplCycleCnt,"") ) && !(checkIfValueZero(pcMultiplCycleCnt)) )
                {
                    bCopy1F81 = true;
                }
            }
        }
    } 
    if( OCFM_ERR_SUCCESS == (IfIndexExists(iNodeId, enumNodeType, (char*)"1016", &iIndexPos)).code)
    {
        bCopy1F81 = true;
    }
    if( OCFM_ERR_SUCCESS == (IfIndexExists(iNodeId, enumNodeType, (char*)"1F8D", &iIndexPos)).code)
    {
        bCopy1F81 = true;
    }

    if(true == bCopy1F81)
    {
        CIndexCollection* objCNIndexCollection;
        CIndex* objCNIndex;
        objCNIndexCollection = pobjNode->getIndexCollection();
        objCNIndex = objCNIndexCollection->getIndexbyIndexValue((char*)"1F81");
        if(objCNIndex == NULL)
        {
            AddIndex(iNodeId, enumNodeType, (char*)"1F81");
            objCNIndex = objCNIndexCollection->getIndexbyIndexValue((char*)"1F81");
        }
        objCNIndex->setFlagIfIncludedCdc(TRUE);

        CIndex* objMNIndex;
        objMNIndex = getMNIndexValues((char*)"1F81");
        CSubIndex* objCNSubIndex, objMNSubIndex;
        objCNIndex->deleteSubIndexCollection();

        for(INT32 iLoopCount = 0; iLoopCount < objMNIndex->getNumberofSubIndexes(); iLoopCount++)
        {
            objCNIndex->addSubIndex(*objMNIndex->getSubIndex(iLoopCount));
//             objCNSubIndex = NULL;
            objCNSubIndex = objCNIndex->getSubIndex(iLoopCount);
            if(NULL != objCNSubIndex)
                objCNSubIndex->setFlagIfIncludedCdc(TRUE);
                
//             if(NULL == objCNSubIndex)
//             {
//                 AddSubIndex(iNodeId, enumNodeType, (char*)"1F81", (char*)objMNIndex->getSubIndex(iLoopCount)->getIndexValue());
//                 objCNSubIndex = objCNIndex->getSubIndexbyIndexValue((char*)objMNIndex->getSubIndex(iLoopCount)->getIndexValue());
//             }
//             if(NULL != (char*)objMNIndex->getSubIndex(iLoopCount)->getActualValue())
//             {
//                 objCNSubIndex->setActualValue((char*)objMNIndex->getSubIndex(iLoopCount)->getActualValue());
//                 printf("\niLoopCount=%d subindex value=%s\n", iLoopCount, (char*)objMNIndex->getSubIndex(iLoopCount)->getActualValue());
//             }
            
        }
    }
}

/****************************************************************************************************
* Function Name: UpdateCNNodeAssignment
* Description: copies all the subobjects of 1F81 in MN to CN if conditions are satisfied
* Return value: void
****************************************************************************************************/

void UpdateCNMultipleCycleAssign(CNode*  pobjNode)
{
    ocfmRetCode stErrorInfo;
    INT32 iIndexPos, iSubIndexPos;
    bool bCopy1F81 = false;
    int iNodeId = pobjNode->getNodeId();
    ENodeType enumNodeType = pobjNode->getNodeType();

    CIndexCollection* objCNIndexCollection;
    CIndex* objCNIndex;
    objCNIndexCollection = pobjNode->getIndexCollection();
    objCNIndex = objCNIndexCollection->getIndexbyIndexValue((char*)"1F9B");
    if(objCNIndex == NULL)
    {
        AddIndex(iNodeId, enumNodeType, (char*)"1F9B");
        objCNIndex = objCNIndexCollection->getIndexbyIndexValue((char*)"1F9B");
    }
    objCNIndex->setFlagIfIncludedCdc(TRUE);

    CIndex* objMNIndex;
    objMNIndex = getMNIndexValues((char*)"1F9B");
    CSubIndex* objCNSubIndex, objMNSubIndex;
    objCNIndex->deleteSubIndexCollection();

    for(INT32 iLoopCount = 0; iLoopCount < objMNIndex->getNumberofSubIndexes(); iLoopCount++)
    {
        objCNIndex->addSubIndex(*objMNIndex->getSubIndex(iLoopCount));
        objCNSubIndex = objCNIndex->getSubIndex(iLoopCount);
        if(NULL != objCNSubIndex)
            objCNSubIndex->setFlagIfIncludedCdc(TRUE);
    }
}

/****************************************************************************************************
* Function Name: WriteCNsData
* Description: Writes all CNS data to CDC file
* Return value: void
****************************************************************************************************/

//	void WriteCNsData(char* pbFileName)
//{
//		char* pbBuffer2;
//		char* pbBuffer3;
//		INT32 iLength;
//		CIndexCollection* pobjIndexCollection;
//	
//			//printf("\n\n\n CN-S Data Started**********************");
//		//pbBuffer2 = NULL;
////		strcpy(pbBuffer3,"");
//		char* pbCNNodeId = (char*)malloc(50);	
//		FILE* fpCdcFile = new FILE();
//		CNode objNode;	
//		CNodeCollection* pobjNodeCollection;
//		pobjNodeCollection = CNodeCollection::getNodeColObjectPointer();		
//		
//		INT32 iCNCount=0;
//		//if (( fpCdcFile = fopen(pbFileName,"a+")) == NULL)
//		//{
//		//	//cout << "Problem" <<endl;
//		//}
//		for(INT32 iLoopCount = 0; iLoopCount < pobjNodeCollection->getNumberOfNodes(); iLoopCount++)
//			{
//				objNode = pobjNodeCollection->getNodebyCollectionIndex(iLoopCount);
//				if (objNode.getNodeType() == CN)
//					{
//						if (( fpCdcFile = fopen(pbFileName,"a+")) == NULL)
//						{
//							//cout << "Problem" <<endl;
//						}
//						
//							AddOtherRequiredCNIndexes(objNode.getNodeId());
//							pobjIndexCollection = objNode.getIndexCollection();
//							char* comment= (char*)malloc(50);
//							_IntToAscii(iCNCount+1,pbCNNodeId,10);
//							iCNCount++;
//							
//							strcpy(comment,"////Configuration Data for CN-");
//							comment = strcat(comment,pbCNNodeId);
//							comment = strcat(comment,"\n");
//							iLength = strlen(comment);
//							if((fwrite(comment, sizeof(char),iLength,fpCdcFile))!=NULL)
//							{
//								printf("\nComments for CN-%d",iLoopCount);
//								fclose(fpCdcFile);
//							}
//							
//							free(comment);
//							//comment = strcat(comment,(char*)iLoopCount);
//							/*strcpy(pbBuffer2, comment);*/
//							INT32 iNumberOfIndexes;
//							//bool firstBuffer = true;																								
//							iNumberOfIndexes = pobjIndexCollection->getNumberofIndexes();
//							
//							CIndex* pobjIndex;
//							char* pbBuffer4;
//									
//						/*************WRITE MN'S 1006,1020 Indexes Values *******************************/			
//							pbBuffer4 = new char[10000];	
//							pobjIndex = getMNIndexValues((char*)"1006");
//							pbBuffer2 = new char[20000];		
//						
//							if(pobjIndex!=NULL)
//							{
//								//GetIndexData(pobjIndex,pbBuffer4);
//								//strcat(pbBuffer2, pbBuffer4);
//								if((char*)pobjIndex->getActualValue() != NULL)
//								UpdateCNCycleTime(pobjIndexCollection,(char*) pobjIndex->getActualValue());
//							}
//							
//							strcpy(pbBuffer2, "");
//							strcpy(pbBuffer4, "");
//							
//							char* pbNoOfenteries = new char[10];
//							pbNoOfenteries = _IntToAscii(getCNsTotalIndexSubIndex(objNode.getNodeId()), pbNoOfenteries, 16);
//							pbNoOfenteries =  padLeft(pbNoOfenteries, '0', 8);
//							strcpy(pbBuffer4, pbNoOfenteries);
//							strcat(pbBuffer4, "\n");
//							strcpy(pbBuffer2, pbBuffer4);
//						
//							
//						
//							/*************WRITE Required CN Indexes in CDC *******************************/
//								for(INT32 iLoopCount = 0; iLoopCount < iNumberOfIndexes; iLoopCount++)
//								{
//		
//									
//									pobjIndex = pobjIndexCollection->getIndex(iLoopCount);
//									//const char* pbIndexValue = pobjIndex->getIndexValue();
//									
//								//if(CheckAllowedCNIndexes((char*)pbIndexValue) && (pobjIndex->getFlagIfIncludedCdc() == true))
//								if(pobjIndex->getFlagIfIncludedCdc() == TRUE)
//								{																						
//										GetIndexData(pobjIndex, pbBuffer4);
//										strcat(pbBuffer2, pbBuffer4);
//										
//									}									
//								}	
//								delete[] pbBuffer4;
//										
//							
//							//Convert CN iNodeID to Hex
//							_IntToAscii(objNode.getNodeId(),pbCNNodeId,16);	
//							char abCNDataLength[50];
//							strcpy(abCNDataLength, "1F22\t");
//							pbCNNodeId = padLeft(pbCNNodeId, '0', 2);
//					
//							///write CN-n iNodeID  in the next to 1F22
//							strcat(abCNDataLength, pbCNNodeId);
//							strcat(abCNDataLength, "\t");
//							
//							//write the size of CN-n pbBuffer
//							INT32 iLength = getCNDataLen(pbBuffer2);

//							//Convert length to Hex
//							_IntToAscii(iLength,pbCNNodeId,16);
//							//printf("pbCNNodeId%s",pbCNNodeId);
//							
//							pbCNNodeId = padLeft(pbCNNodeId, '0', 8);
//							strcat(abCNDataLength, pbCNNodeId);				
//							
//							// First write the IF22 data in a pbBuffer and then CN-ns Object Dictionary
//							pbBuffer3 = new char [strlen(pbBuffer2)+50];
//							strcpy(pbBuffer3, abCNDataLength);
//							strcat(pbBuffer3, "\n");
//							strcat(pbBuffer3, pbBuffer2);
//															
//						
//							//strcpy(pbBuffer2,GenerateCNOBD(pobjNodeCollection));
//						
//							//write all CNs data in the file
//							iLength = strlen(pbBuffer3);
//							if (( fpCdcFile = fopen(pbFileName,"a+")) == NULL)
//							{
//								//cout << "Problem" <<endl;
//							}
//							if((fwrite(pbBuffer3, sizeof(char),iLength,fpCdcFile))!=NULL)
//								{
//									fclose(fpCdcFile);																							
//								}
//					delete [] pbBuffer2;
//					delete [] pbBuffer3;
//					}
//			
//			}
//		
//	}

	void WriteCNsData(char* fileName)
{
		char* Buffer2;
		char* Buffer3;
		int len;
		CIndexCollection* objIndexCollection;
	
			//printf("\n\n\n CN-S Data Started**********************");
		//Buffer2 = NULL;
//		strcpy(Buffer3,"");
		char* c = (char*)malloc(50);	
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
						
							//AddOtherRequiredCNIndexes(objNode.getNodeId());
							objIndexCollection = objNode.getIndexCollection();
							char* comment= (char*)malloc(50);
							_IntToAscii(CNCount+1,c,10);
							CNCount++;
							
							strcpy(comment,"////Configuration Data for CN-");
							comment = strcat(comment,c);
							comment = strcat(comment,"\n");
							len = strlen(comment);
							if((fwrite(comment, sizeof(char),len,fileptr))!=NULL)
							{
								//printf("\nComments for CN-%d",count);
								fclose(fileptr);
							}
							
							free(comment);
							//comment = strcat(comment,(char*)count);
							/*strcpy(Buffer2, comment);*/
							int NumberOfIndexes;
							//bool firstBuffer = true;																								
							
							CIndex* objIndex;
							char* Buffer4;
									
						/*************WRITE MN'S 1006,1020 Indexes Values *******************************/			
							//Buffer4 = (char*)malloc(10000);	
							Buffer4 = new char[15000];	
							objIndex = getMNIndexValues((char*)"1006");
							//Buffer2 = (char*)malloc(20000);		
							Buffer2 = new char[60000];		
						
							if(objIndex!=NULL)
							{
								//GetIndexData(objIndex,Buffer4);
								//strcat(Buffer2, Buffer4);
								if((char*)objIndex->getActualValue() != NULL)
								UpdateCNCycleTime(objIndexCollection,(char*) objIndex->getActualValue());
							}
							
							objIndex = getMNIndexValues((char*)"1F26");
							if(objIndex != NULL)
							UpdatedCNDateORTime(objIndex, objNode.getNodeId(), DATE);
							
							objIndex = getMNIndexValues((char*)"1F27");
							if(objIndex != NULL)
							UpdatedCNDateORTime(objIndex, objNode.getNodeId(), TIME);
							
                            objIndex = getMNIndexValues((char*)"1F98");
                            if(objIndex != NULL)
                            {
                                CSubIndex* pobjSubIndex;
                                pobjSubIndex = getMNSubIndexValues((char*)"1F98", (char*)"08");
                                if( (pobjSubIndex != NULL) && (pobjSubIndex->getActualValue() != NULL) )
                                    UpdateCNAsyncMTUsize( objIndexCollection, (char*)pobjSubIndex->getActualValue());

                                pobjSubIndex = getMNSubIndexValues((char*)"1F98", (char*)"07");
                                if( (pobjSubIndex != NULL) && (pobjSubIndex->getActualValue() != NULL) )
                                    UpdateCNMultiPrescal( objIndexCollection,  (char*)pobjSubIndex->getActualValue());
                            }

                            objIndex = getMNIndexValues((char*)"1F81");
                            if(objIndex != NULL)
                            {
                                    UpdateCNNodeAssignment( &objNode);
                            }

                            objIndex = getMNIndexValues((char*)"1F9B");
                            if(objIndex != NULL)
                            {
                                    UpdateCNMultipleCycleAssign( &objNode);
                            }

							strcpy(Buffer2, "");
							strcpy(Buffer4, "");
							
							char* NoOfenteries = new char[10];
							NoOfenteries = _IntToAscii(getCNsTotalIndexSubIndex(objNode.getNodeId())+1, NoOfenteries, 16);
                            /*1 is added for the size*/
							NoOfenteries =  padLeft(NoOfenteries, '0', 8);
							strcpy(Buffer4, NoOfenteries);
							strcat(Buffer4, "\n");
							strcpy(Buffer2, Buffer4);
						
							NumberOfIndexes = objIndexCollection->getNumberofIndexes();
						
							/*************WRITE Required CN Indexes in CDC *******************************/
								for(int i=0; i<NumberOfIndexes; i++)
								{
		
									
									objIndex = objIndexCollection->getIndex(i);
									//const char* IndexValue = objIndex->getIndexValue();
									
								//if(CheckAllowedCNIndexes((char*)IndexValue) && (objIndex->getFlagIfIncludedCdc() == true))
								if(objIndex->getFlagIfIncludedCdc() == TRUE)
								{	
										//printf("\nobjindex value %s\n",objIndex->getIndexValue());
										GetIndexData(objIndex, Buffer4);
										strcat(Buffer2, Buffer4);
										
									}
								}	
								delete[] Buffer4;
										
							
							//Convert CN NodeID to Hex
							_IntToAscii(objNode.getNodeId(),c,16);	
							char CNDataLength[50];
							strcpy(CNDataLength, "1F22\t");
							c = padLeft(c, '0', 2);
					
							///write CN-n NodeID  in the next to 1F22
							strcat(CNDataLength, c);
							strcat(CNDataLength, "\t");
							
							//write the size of CN-n Buffer
							int len = getCNDataLen(Buffer2);

							//Convert length to Hex
							_IntToAscii(len,c,16);
							//printf("c%s",c);
							
							c = padLeft(c, '0', 8);
							strcat(CNDataLength, c);				
							
							// First write the IF22 data in a Buffer and then CN-ns Object Dictionary
							//Buffer3 = (char*)malloc(strlen(Buffer2)+50);
							Buffer3 = new char [strlen(Buffer2)+50];
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
									fclose(fileptr);																							
								}
					//free(Buffer2);
					delete [] Buffer2;
					//free(Buffer3);
					delete [] Buffer3;
					}
			
			}
		
	}
	
	
/****************************************************************************************************
* Function Name: getMNsTotalIndexSubIndex
* Description: 
* Return value: INT32
****************************************************************************************************/
	INT32 getMNsTotalIndexSubIndex()
	{
		CNodeCollection *objNodeCol;
		objNodeCol =  CNodeCollection::getNodeColObjectPointer();
		CNode objNode;		
		INT32 iNumberOfEntries = 0;
		CIndexCollection* pobjIdxCol;
		objNode = objNodeCol->getMNNode();
		/* Number of 1F81 enteries is twice the number of CN's*/
		iNumberOfEntries =  2 * objNodeCol->getCNNodesCount();
        /* include the number of CN's*/
        iNumberOfEntries =  iNumberOfEntries + objNodeCol->getCNNodesCount();

		pobjIdxCol =  objNode.getIndexCollection();

		for(INT32 iLoopCount = 0; iLoopCount < pobjIdxCol->getNumberofIndexes(); iLoopCount++)
        {
            
            CIndex *pobjIndex;
            pobjIndex = pobjIdxCol->getIndex(iLoopCount);
            
    
            if( pobjIndex->getFlagIfIncludedCdc() == TRUE)
            {
		if(CheckBlockedMNIndexes((char*)pobjIndex->getIndexValue()))
		{
			continue;
		}
                    if(pobjIndex->getNumberofSubIndexes() ==0)
                    {
                        if(pobjIndex->getActualValue() != NULL)
                        {
                            iNumberOfEntries =  iNumberOfEntries + 1;
							//	printf("\nIndexid=%s iNumberOfEntries=%d\n", (char*)pobjIndex->getIndexValue(), iNumberOfEntries);
                        }
                    }
                        
                    else 
                {
                        if(CheckIfMappingPDO((char*)pobjIndex->getIndexValue()))
                        {
                            CSubIndex* pobjSubIndex;
                            pobjSubIndex = pobjIndex->getSubIndexbyIndexValue((char*)"00");

                            if((NULL != pobjSubIndex) && (NULL != pobjSubIndex->getActualValue()) && ( 0 != strcmp(pobjSubIndex->getActualValue(),"")) && !(checkIfValueZero((char*)pobjSubIndex->getActualValue())) )
                            {
								//printf("\nIndexid=%s subindex=%s before iNumberOfEntries=%d\t", (char*)pobjIndex->getIndexValue(), (char*)pobjSubIndex->getIndexValue(), iNumberOfEntries);
                                iNumberOfEntries =  iNumberOfEntries + GetDecimalValue((char*)pobjSubIndex->getActualValue());
                                iNumberOfEntries =  iNumberOfEntries + 2; /* to initalize and reinitialize 00 entry subindex */
								//printf("After iNumberOfEntries=%d\n", iNumberOfEntries);
                            }
                            continue;
                        }

                        CSubIndex* pobjSubIndex;
                        pobjSubIndex = pobjIndex->getSubIndexbyIndexValue((char*)"00");
                        if((NULL != pobjSubIndex) && (NULL != pobjSubIndex->getActualValue()) && ( 0 != strcmp(pobjSubIndex->getActualValue(),"")) )
                        {
							//printf("\nIndexid=%s subindex=%s before iNumberOfEntries=%d\t", (char*)pobjIndex->getIndexValue(), (char*)pobjSubIndex->getIndexValue(), iNumberOfEntries);
                            if(checkIfValueZero((char*)pobjSubIndex->getActualValue()))
                                continue;
                            /*
                            if(TRUE == pobjSubIndex->getFlagIfIncludedCdc())
                            {
                                iNumberOfEntries = iNumberOfEntries + 1;
                            }
                            iNumberOfEntries =  iNumberOfEntries + GetDecimalValue((char*)pobjSubIndex->getActualValue());
							//printf("After iNumberOfEntries=%d\n", iNumberOfEntries);
                        continue;
                            */
                        }

                        for(INT32 iLoopCount = 0; iLoopCount < pobjIndex->getNumberofSubIndexes(); iLoopCount++)
                        {
                            //printf("\n SubIndex iLoopCount%d",pobjIndex->getNumberofSubIndexes());
                            if(pobjIndex->getSubIndex(iLoopCount)->getActualValue() !=NULL && TRUE == pobjIndex->getSubIndex(iLoopCount)->getFlagIfIncludedCdc())
                            {
                                iNumberOfEntries =  iNumberOfEntries + 1;
								//printf("\nIndexid=%s subindex=%s before iNumberOfEntries=%d\t", (char*)pobjIndex->getIndexValue(), (char*)pobjIndex->getSubIndex(iLoopCount)->getIndexValue(), iNumberOfEntries);
                            }
                        }               
                    }
                   
                }
            }
		/* Add + 1 for the number of enteries line*/
		//iNumberOfEntries = iNumberOfEntries + 1;		

		return iNumberOfEntries ;
	}
	
/****************************************************************************************************
* Function Name: getCNsTotalIndexSubIndex
* Description: 
* Return value: INT32
****************************************************************************************************/
		INT32 getCNsTotalIndexSubIndex(INT32 iNodeID)
	{
		CNodeCollection *objNodeCol;
		objNodeCol =  CNodeCollection::getNodeColObjectPointer();
		CNode objNode;		
		INT32 iNumberOfEntries = 0;
		CIndexCollection* pobjIdxCol;
		
		objNode    = objNodeCol->getNode(iNodeID);
		pobjIdxCol =  objNode.getIndexCollection();
//printf("\ngetCNsTotalIndexSubIndex iNodeID=%d\n",iNodeID);
		for(INT32 iLoopCount = 0; iLoopCount < pobjIdxCol->getNumberofIndexes(); iLoopCount++)
		{
			
			CIndex *pobjIndex;
			pobjIndex = pobjIdxCol->getIndex(iLoopCount);
			
	
			if( pobjIndex->getFlagIfIncludedCdc() == TRUE)
			{
				if(CheckAllowedCNIndexes((char*)pobjIndex->getIndexValue()))
				{
					if(pobjIndex->getNumberofSubIndexes() ==0)
					{
						if(pobjIndex->getActualValue() != NULL)
						{
							iNumberOfEntries =  iNumberOfEntries + 1;
						}
					}
						
					else 
				{
						if(CheckIfMappingPDO((char*)pobjIndex->getIndexValue()))
						{
							CSubIndex* pobjSubIndex;
                            pobjSubIndex = pobjIndex->getSubIndexbyIndexValue((char*)"00");

                            if((NULL != pobjSubIndex) && (NULL != pobjSubIndex->getActualValue()) && ( 0 != strcmp(pobjSubIndex->getActualValue(),"")) && !(checkIfValueZero((char*)pobjSubIndex->getActualValue())) )
                            {
                                iNumberOfEntries =  iNumberOfEntries + GetDecimalValue((char*)pobjSubIndex->getActualValue());
                                iNumberOfEntries =  iNumberOfEntries + 2; /* to initalize and reinitialize 00 entry subindex */
                            }
                            continue;
						}



                        CSubIndex* pobjSubIndex;
                        pobjSubIndex = pobjIndex->getSubIndexbyIndexValue((char*)"00");
                        if((NULL != pobjSubIndex) && (NULL != pobjSubIndex->getActualValue()) && ( 0 != strcmp(pobjSubIndex->getActualValue(),"")) )
                        {
                            if(checkIfValueZero((char*)pobjSubIndex->getActualValue()))
                                continue;
                            /*if(TRUE == pobjSubIndex->getFlagIfIncludedCdc())
                            {
                                iNumberOfEntries = iNumberOfEntries + 1;
                            }
                            
                            iNumberOfEntries =  iNumberOfEntries + GetDecimalValue((char*)pobjSubIndex->getActualValue());
                        continue;*/
                        }
                        
						for(INT32 iLoopCount = 0; iLoopCount < pobjIndex->getNumberofSubIndexes(); iLoopCount++)
						{
							if(pobjIndex->getSubIndex(iLoopCount)->getActualValue() !=NULL && TRUE == pobjIndex->getSubIndex(iLoopCount)->getFlagIfIncludedCdc())
							{
                                /* if index is a mapping pdo then dont count if value is zero */
                                  //if( CheckIfMappingPDO((char*)pobjIndex->getIndexValue()) && ( strcmp(pobjIndex->getSubIndex(iLoopCount)->getActualValue(),"") == 0 ||  checkIfValueZero((char*)pobjIndex->getSubIndex(iLoopCount)->getActualValue()) ) )
                                  //  continue;
 								iNumberOfEntries =  iNumberOfEntries + 1;
// // 
// //                             CSubIndex* pobjSubIndex;
// //                             pobjSubIndex = pobjIndex->getSubIndexbyIndexValue((char*)"00");
// // printf("\nIndexid=%s tsubIndexid=%s value=%s iNumberOfEntries=%d flag=%d", (char*)pobjIndex->getIndexValue(), (char*)pobjIndex->getSubIndex(iLoopCount)->getIndexValue(), pobjIndex->getSubIndex(iLoopCount)->getActualValue(), iNumberOfEntries, pobjIndex->getSubIndex(iLoopCount)->getFlagIfIncludedCdc()); 
// //                             if((NULL != pobjSubIndex) && (NULL != pobjSubIndex->getActualValue()) && ( 0 != strcmp(pobjSubIndex->getActualValue(),"")) && !(checkIfValueZero((char*)pobjSubIndex->getActualValue())) )
// //                             {
// //                                 iNumberOfEntries =  iNumberOfEntries + GetDecimalValue((char*)pobjSubIndex->getActualValue());
// // printf(" after iNumberOfEntries=%d\n", iNumberOfEntries);
// //                             }
                            //continue;
							}
						}				
					}
					}
					
				}
			}

		return iNumberOfEntries ;
	}
/****************************************************************************************************
* Function Name: GenerateCDC
* Description: Generates the CDC file
* Return value: ocfmRetCode
****************************************************************************************************/
//ocfmRetCode GenerateCDC(char* CDCLocation)
//	{
	//	CNode objNode;	
	//	CIndexCollection* pobjIndexCollection;
	//	//ofstream file;
	//	char *pbBuffer1 = NULL;
	//	UINT32 uiLength;
	//	ocfmRetCode retCode;
	//	//const char tempFileName[9] = "temp.txt";
	//	char* pbTempFileName;
	//	pbTempFileName = new char[strlen(CDCLocation) + 10 + 10];		
	//	sprintf(pbTempFileName, "%s/%s.txt", CDCLocation, "mnobd");

	//	char* pbTempOutFileName;
	//	//pbTempOutFileName = new char [strlen(CDCLocation + 10 + 10)];
	//	pbTempOutFileName = new char [1000];
	//	sprintf(pbTempOutFileName, "%s/%s.cdc", CDCLocation, "mnobd");		

	//	//sprintf(pbTempFileName, "../%s.txt", "mnobd");
	//	cout << "\n\ntempFileName:" << pbTempFileName << endl;
	//	cout << "\n\ntempOutputFileName:" << pbTempOutFileName << endl;
	//	//printf("Inside GenerateCDC");
	//	
	//	try
	//	{							
	//		//get the MN Node object from the NodeCollection
	//		CNodeCollection* pobjNodeCollection;
	//		pobjNodeCollection = CNodeCollection::getNodeColObjectPointer();		
	//		
	//	/******************************* Write MN's Indexes ******************************************************************/
	//	
	//		
	//		objNode = pobjNodeCollection->getMNNode();
	//		if(pobjNodeCollection->getNumberOfNodes()==0)
	//		{
	//			ocfmException ex;
	//			ex.ocfm_Excpetion(OCFM_ERR_NO_NODES_FOUND);
	//			throw ex;
	//		}
	//		if(objNode.isNull())
	//		{			
	//			ocfmException ex;
	//			ex.ocfm_Excpetion(OCFM_ERR_MN_NODE_DOESNT_EXIST);
	//			throw ex;			
	//		}
	//		if(pobjNodeCollection->getCNNodesCount()==0)
	//		{
	//			ocfmException ex;
	//			ex.ocfm_Excpetion(OCFM_ERR_NO_CN_NODES_FOUND);
	//			throw ex;
	//		}
	//		//printf("Node id %d",objNode.getNodeId());

	//		/* Check if autogeneration of MN's Object Dicitonary is set to true */
	//		
	//		CPjtSettings* pobjPjtSettings;
	//		pobjPjtSettings = CPjtSettings::getPjtSettingsPtr();
	//		bool bCheckFlag = true;
	//		("\n autogenerate %d",pobjPjtSettings->getGenerateAttr());
	//		if(pobjPjtSettings->getGenerateAttr() == YES_AG)
	//		{	
	//			cout << "\n\n\n\n Generating MN OBD\n\n\n\n" << endl;
	//			retCode = GenerateMNOBD();
	//			if(retCode.code != OCFM_ERR_SUCCESS)
	//				return retCode;
	//			/*else
	//				bcheckFlag = false;*/
	//		}
	//	

	//		FILE* fpCdcFile = new FILE();
	//		if (( fpCdcFile = fopen(pbTempFileName,"w+")) == NULL)
	//		{
	//				ocfmException ex;
	//				ex.ocfm_Excpetion(OCFM_ERR_CANNOT_OPEN_FILE);
	//				throw ex;						
	//			}
	//			pbBuffer1 = new char[CDC_BUFFER];
	//			char* pbNoOfenteries = new char[10];
	//			pbNoOfenteries = _IntToAscii(getMNsTotalIndexSubIndex(), pbNoOfenteries, 16);
	//			pbNoOfenteries =  padLeft(pbNoOfenteries, '0', 8);
	//			strcpy(pbBuffer1, pbNoOfenteries);
	//			strcat(pbBuffer1, "\n");
	//			uiLength =  strlen(pbBuffer1);
	//	
	//			
	//			/* Write number of enteries */
	//			if((uiLength != (fwrite(pbBuffer1, sizeof(char),uiLength,fpCdcFile))))
	//			{
	//				delete[] pbBuffer1;
	//			}
	//			for(INT32 iLoopCount = 0; iLoopCount < pobjNodeCollection->getNumberOfNodes(); iLoopCount++)
	//			{
	//					CNode objNode;
	//					objNode = pobjNodeCollection->getNodebyCollectionIndex(iLoopCount);
	//					if(objNode.getNodeType() ==CN)
	//					{
	//							
	//							//printf("\nInside 1F81");
	//													
	//							//pbBuffer1 = (char*)malloc(CDC_BUFFER);
	//							pbBuffer1 = new char[CDC_BUFFER];
	//							//strcpy(pbBuffer1, pbNoOfenteries);
	//							strcpy(pbBuffer1, "//// NodeId Assignment\n");
	//							strcat(pbBuffer1, "1F81");
	//							strcat(pbBuffer1, "\t");
	//							INT32 iNodeID = objNode.getNodeId();		
	//							char* pbHexData = (char*)malloc(10);					
	//							pbHexData = _IntToAscii(iNodeID,pbHexData,16);
	//							pbHexData = padLeft(pbHexData,'0',2);
	//							strcat(pbBuffer1, pbHexData);								
	//							strcat(pbBuffer1, "\t00000004\t00000007\n");						
	//							strcat(pbBuffer1, "\n");	
	//							uiLength = strlen(pbBuffer1);
	//							// write 1F81 entry in MN text file
	//							if((uiLength != (fwrite(pbBuffer1, sizeof(char),uiLength,fpCdcFile))))
	//							{
	//								//fclose(fpCdcFile);
	//								//printf("pbBuffer1 written");
	//							
	//							}
	//						
	//							delete[] pbBuffer1;
	//					}
	//				}
	//				/*	}*/
	//			fclose(fpCdcFile);
	//		if (( fpCdcFile = fopen(pbTempFileName,"a+")) == NULL)
	//			{
	//				printf ( "Cannot open file you have named...!\n" );
	//				ocfmException ex;
	//				ex.ocfm_Excpetion(OCFM_ERR_CANNOT_OPEN_FILE);
	//				throw ex;							
	//			}
	//			pobjIndexCollection = objNode.getIndexCollection();
	//		

	//		//Get all the MN's Default Data in pbBuffer1
	//		INT32 iNumberOfIndexes = pobjIndexCollection->getNumberofIndexes();
	//		for(INT32 iLoopCount=0; iLoopCount < iNumberOfIndexes; iLoopCount++)
	//			{
	//				CIndex* pobjIndex;
	//				pobjIndex = pobjIndexCollection->getIndex(iLoopCount);
	//				//if((!checkFlag) || (checkFlag && (objIndex->getFlagIfIncludedCdc() == TRUE)))
	//				if(pobjIndex->getFlagIfIncludedCdc() == TRUE)
	//				{
	//						//pbBuffer1 = (char*)malloc(CDC_BUFFER);
	//						pbBuffer1 = new char[CDC_BUFFER];
	//						uiLength = strlen(pbBuffer1);
	//						
	//						GetIndexData(pobjIndex,pbBuffer1);
	//						uiLength = strlen(pbBuffer1);
	//						if((uiLength != (fwrite(pbBuffer1, sizeof(char),uiLength,fpCdcFile))))
	//						{
	//							//printf("pbBuffer1 written");
	//						
	//						}					
	//						
	//						delete[] pbBuffer1;
	//					}
	//						
	//			}
	//			fclose(fpCdcFile);
	//			
	//		/*************************Write CN's Data in pbBuffer2***************************************************/
	//		WriteCNsData((char*)pbTempFileName);
	//		INT32 iRetVal;
	//		
	//		//Get all the IF81 ENTRY in pbBuffer1
	//		if(pobjNodeCollection->getNumberOfNodes()!=0)
	//		{
	//					
	//			for(INT32 iLoopCount = 0; iLoopCount < pobjNodeCollection->getNumberOfNodes(); iLoopCount++)
	//			{
	//					CNode objNode;
	//					objNode = pobjNodeCollection->getNodebyCollectionIndex(iLoopCount);
	//					if(objNode.getNodeType() ==CN)
	//					{
	//							if (( fpCdcFile = fopen(pbTempFileName,"a+")) == NULL)
	//							{
	//								ocfmException ex;
	//								ex.ocfm_Excpetion(OCFM_ERR_CANNOT_OPEN_FILE);
	//								throw ex;						
	//							}
	//							//printf("\nInside 1F81");
	//							//pbBuffer1 = (char*)malloc(CDC_BUFFER);
	//							pbBuffer1 = new char[CDC_BUFFER];
	//							uiLength = strlen(pbBuffer1);
	//							strcpy(pbBuffer1, "//// NodeId Reassignment\n");
	//							strcat(pbBuffer1, "1F81");
	//							strcat(pbBuffer1, "\t");
	//							INT32 iNodeID = objNode.getNodeId();		
	//							char* pbHexData = (char*)malloc(10);					
	//							pbHexData = _IntToAscii(iNodeID,pbHexData,16);
	//							pbHexData = padLeft(pbHexData,'0',2);
	//							strcat(pbBuffer1, pbHexData);								
	//							strcat(pbBuffer1, "\t00000004\t80000007\n");																						
	//							uiLength = strlen(pbBuffer1);
	//							if((uiLength != (fwrite(pbBuffer1, sizeof(char),uiLength,fpCdcFile))))
	//							{
	//							
	//								iRetVal = fclose(fpCdcFile);
	//								printf("\n iRetVal code %d", iRetVal);
	//								//printf("pbBuffer1 written");
	//							
	//							}
	//							else
	//							{
	//								iRetVal = fclose(fpCdcFile);
	//								printf("\n Return code %d", iRetVal);
	//							}
	//							/*else
	//							{
	//								ocfmException ex;
	//								ex.ocfm_Excpetion(OCFM_ERR_CANNOT_OPEN_FILE);
	//								throw ex;		
	//							}*/
	//							delete[] pbBuffer1;
	//					}
	//				}
	//				/*	}*/
	//						
	//			}
	//		
	//				
	//		//printf("\nText cdc generated");
	//		// Convert CDC txt file to Binary
	//		//ConvertCdcToBinary(pbFileName,(char*) pbTempFileName);			
	//			
	//		#if defined(_WIN32) && defined(_MSC_VER)
	//		char* cmdBuffer;
	//		printf("\ntxt2cdc.exe \"%s\" \"%s\"", pbTempFileName, pbTempOutFileName);
	//		cmdBuffer = new char[(2 * (strlen(CDCLocation) + 10 + 10)) + 25];		
	//		sprintf(cmdBuffer, "txt2cdc.exe \"%s\" \"%s\"", pbTempFileName, pbTempOutFileName);
	//		printf("\n command Buffer %s",cmdBuffer);
	//		system(cmdBuffer);
	//		delete [] cmdBuffer;
	//		#else
	//		char* cmdBuffer;
	//		printf("\n./txt2cdc \"%s\" \"%s\"", pbTempFileName, pbTempOutFileName);
	//		cmdBuffer = new char[(2 * (strlen(CDCLocation) + 10 + 10)) + 25];		
	//		sprintf(cmdBuffer, "./txt2cdc \"%s\" \"%s\"", pbTempFileName, pbTempOutFileName);
	//		printf("\n command Buffer %s",cmdBuffer);
	//		system(cmdBuffer);
	//		delete [] cmdBuffer;
	//		#endif
	//		retCode.code =  OCFM_ERR_SUCCESS ;
	//		delete [] pbTempFileName;
	//		delete [] pbTempOutFileName;
	//		return retCode;
	//	}
	//	catch(ocfmException & ex)
	//	{
	//		delete [] pbTempFileName;
	//		delete [] pbTempOutFileName;
	//		return ex._ocfmRetCode;
	//	}
	//}
	
	
	ocfmRetCode GenerateCDC(char* CDCLocation)
	{
		CNode objNode;	
		CIndexCollection* objIndexCollection;
		//ofstream file;
		char *Buffer1 = NULL;
		unsigned int len;
		ocfmRetCode retCode, ret1F81Code;
        int iSubIndexPos, iIndexPos;
		//const char tempFileName[9] = "temp.txt";
		char* tempFileName;
		tempFileName = new char[strlen(CDCLocation) + 10 + 10];		
		sprintf(tempFileName, "%s/%s.txt", CDCLocation, "mnobd");

		char* tempOutputFileName;
		//tempOutputFileName = new char [strlen(CDCLocation + 10 + 10)];
		tempOutputFileName = new char [1000];
		sprintf(tempOutputFileName, "%s/%s.cdc", CDCLocation, "mnobd");		

		//sprintf(tempFileName, "../%s.txt", "mnobd");
		//cout << "\n\ntempFileName:" << tempFileName << endl;
		//cout << "\n\ntempOutputFileName:" << tempOutputFileName << endl;
		//printf("Inside GenerateCDC");
		
		try
		{							
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

			/* Check if autogeneration of MN's Object Dicitonary is set to true */
			
			CPjtSettings* stPjtSettings;
			stPjtSettings = CPjtSettings::getPjtSettingsPtr();
			bool checkFlag = true;
		
			if(stPjtSettings->getGenerateAttr() == YES_AG)
			{	
				//cout << "\n\n\n\n Generating MN OBD\n\n\n\n" << endl;
                retCode = CheckMutliplexAssigned();
                if(retCode.code != OCFM_ERR_SUCCESS)
                {
                    return retCode;
                }
				retCode = GenerateMNOBD();
				if(retCode.code != OCFM_ERR_SUCCESS)
					return retCode;
				/*else
					checkFlag = false;*/
			}
		

			FILE* fileptr = new FILE();
			if (( fileptr = fopen(tempFileName,"w+")) == NULL)
			{
					ocfmException ex;
					ex.ocfm_Excpetion(OCFM_ERR_CANNOT_OPEN_FILE);
					throw ex;						
				}
				//Buffer1 = (char*)malloc(CDC_BUFFER);
				Buffer1 = new char[CDC_BUFFER];
				char* NoOfenteries = new char[10];
				NoOfenteries = _IntToAscii(getMNsTotalIndexSubIndex(), NoOfenteries, 16);
				NoOfenteries =  padLeft(NoOfenteries, '0', 8);
				strcpy(Buffer1, NoOfenteries);
				strcat(Buffer1, "\n");
				len =  strlen(Buffer1);
		
				
				/* Write number of enteries */
				if((len != (fwrite(Buffer1, sizeof(char),len,fileptr))))
				{
					delete[] Buffer1;
				}

				// Add 1F81
				//UpdateMNNodeAssignmentIndex(&objNodeCollection->getMNNode(), objNodeCollection->getCNNodesCount());
				UpdateMNNodeAssignmentIndex(&objNodeCollection->getMNNode(), objNodeCollection->getCNNodesCount(), (char*)"1F81", true );
				UpdateMNNodeAssignmentIndex(&objNodeCollection->getMNNode(), objNodeCollection->getCNNodesCount(), (char*)"1F92", false );
							
				for(int i=0;i < objNodeCollection->getNumberOfNodes();i++)
				{
						CNode objNode;
						objNode = objNodeCollection->getNodebyCollectionIndex(i);
						if(objNode.getNodeType() ==CN)
						{
								
								//printf("\nInside 1F81");
														
								//Buffer1 = (char*)malloc(CDC_BUFFER);
								Buffer1 = new char[CDC_BUFFER];
								//strcpy(Buffer1, NoOfenteries);
								strcpy(Buffer1, "//// NodeId Assignment\n");
								strcat(Buffer1, "1F81");
								strcat(Buffer1, "\t");
								int NodeID = objNode.getNodeId();		
								char* hex = (char*)malloc(10);					
								hex = _IntToAscii(NodeID,hex,16);
								hex = padLeft(hex,'0',2);
								strcat(Buffer1, hex);								
								strcat(Buffer1, "\t00000004\t00000007\n");

                                char* pcTemp1F81Data = setNodeAssigmentBits(&objNode);
                                char* pb1F81Data = new char[ strlen(pcTemp1F81Data) + ALLOC_BUFFER + 2];
                                sprintf(pb1F81Data, "0X%s", pcTemp1F81Data);
                                ret1F81Code = IfSubIndexExists(240, MN, (char*)"1F81", hex, &iSubIndexPos, &iIndexPos);
                                if( OCFM_ERR_INDEXID_NOT_FOUND == ret1F81Code.code)
                                {
                                    AddIndex(240, MN, (char*)"1F81");
                                    AddSubIndex(240, MN, (char*)"1F81", hex);
                                }
                                else if(OCFM_ERR_SUBINDEXID_NOT_FOUND == ret1F81Code.code)
                                {
                                    AddSubIndex(240, MN, (char*)"1F81", hex);
                                }
 
								SetSIdxValue((char*)"1F81",hex, pb1F81Data,objNodeCollection->getMNNode().getIndexCollection(), MN_NODEID, MN, false); 
                              /*  char* pcSubIndName = new char[50];
                                pcSubIndName[0] = 0;
                                INT32 iSubIndFlag = 0;
                                GetSubIndexAttributes(240, MN, (char*)"1F81", hex, NAME, pcSubIndName);
                                char* pcSubIndFlag = new char[10];
                                GetSubIndexAttributes(240, MN, (char*)"1F81", hex, FLAGIFINCDC, pcSubIndFlag);
                                iSubIndFlag  = atoi(pcSubIndFlag);
                                SetSubIndexAttributes(240, MN, (char*)"1F81", hex, pb1F81Data, pcSubIndName, (EFlag)iSubIndFlag);
                                delete [] pcSubIndName;
                                delete [] pcSubIndFlag;*/
                                delete [] pb1F81Data;

								strcat(Buffer1, "\n");	
								len = strlen(Buffer1);
								// write 1F81 entry in MN text file
								if((len != (fwrite(Buffer1, sizeof(char),len,fileptr))))
								{
									//fclose(fileptr);
									//printf("Buffer1 written");
								
								}
							
								delete[] Buffer1;
						}
					}
					/*	}*/
				fclose(fileptr);
				
			if (( fileptr = fopen(tempFileName,"a+")) == NULL)
				{
					//printf ( "Cannot open file you have named...!\n" );
					ocfmException ex;
					ex.ocfm_Excpetion(OCFM_ERR_CANNOT_OPEN_FILE);
					throw ex;							
				}
				objIndexCollection = objNode.getIndexCollection();
			

			//Get all the MN's Default Data in Buffer1
			int NumberOfIndexes = objIndexCollection->getNumberofIndexes();
			for(int i=0;i < NumberOfIndexes; i++)
				{
					CIndex* objIndex;
					objIndex = objIndexCollection->getIndex(i);
					//if((!checkFlag) || (checkFlag && (objIndex->getFlagIfIncludedCdc() == TRUE)))
					
					if(strcmp(objIndex->getIndexValue(), "1F81") != 0)
					{
						if(objIndex->getFlagIfIncludedCdc() == TRUE )
						{
								//Buffer1 = (char*)malloc(CDC_BUFFER);
								Buffer1 = new char[CDC_BUFFER];
								len = strlen(Buffer1);		
								GetIndexData(objIndex,Buffer1);
								len = strlen(Buffer1);
								if((len != (fwrite(Buffer1, sizeof(char),len,fileptr))))
								{
									//printf("Buffer1 written");
								
								}					
								
								delete[] Buffer1;
						}
								
					}
			
			}
				fclose(fileptr);
	
			/*************************Write CN's Data in Buffer2***************************************************/
			WriteCNsData((char*)tempFileName);
			int ret;
			
			//Get all the IF81 ENTRY in Buffer1
			if(objNodeCollection->getNumberOfNodes()!=0)
			{
						
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
								//Buffer1 = (char*)malloc(CDC_BUFFER);
								Buffer1 = new char[CDC_BUFFER];
								len = strlen(Buffer1);
								strcpy(Buffer1, "//// NodeId Reassignment\n");
								strcat(Buffer1, "1F81");
								strcat(Buffer1, "\t");
								int NodeID = objNode.getNodeId();		
								char* hex = (char*)malloc(10);					
								hex = _IntToAscii(NodeID,hex,16);
								hex = padLeft(hex,'0',2);
								strcat(Buffer1, hex);								
								//strcat(Buffer1, "\t00000004\t80000007\n");			
								//setNodeAssigmentBits(&objNode);

								strcat(Buffer1, "\t00000004\t");
                                char* pcTemp1F81Data = setNodeAssigmentBits(&objNode);
//                                 char* pb1F81Data = new char[ strlen(pcTemp1F81Data) + ALLOC_BUFFER + 2];
//                                 sprintf(pb1F81Data, "0X%s", pcTemp1F81Data);
								strcat(Buffer1, pcTemp1F81Data);

//                                 AddSubIndex(240, MN, (char*)"1F81", hex);
//                                 char* pcSubIndName = new char[50];
//                                 pcSubIndName[0] = 0;
//                                 INT32 iSubIndFlag = 0;
//                                 GetSubIndexAttributes(240, MN, (char*)"1F81", hex, NAME, pcSubIndName);
//                                 char* pcSubIndFlag = new char[10];
//                                 GetSubIndexAttributes(240, MN, (char*)"1F81", hex, FLAGIFINCDC, pcSubIndFlag);
//                                 iSubIndFlag  = atoi(pcSubIndFlag);
//                                 SetSubIndexAttributes(240, MN, (char*)"1F81", hex, pb1F81Data, pcSubIndName, (EFlag)iSubIndFlag);
//                                 delete [] pcSubIndName;
//                                 delete [] pcSubIndFlag;
//                                 delete [] pb1F81Data;

								strcat(Buffer1, "\n");

								len = strlen(Buffer1);
								if((len != (fwrite(Buffer1, sizeof(char),len,fileptr))))
								{
								
									ret = fclose(fileptr);
									//printf("\n ret code %d", ret);
									//printf("Buffer1 written");
								
								}
								else
								{
									ret = fclose(fileptr);
									//printf("\n ret code %d", ret);
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
			//ConvertCdcToBinary(fileName,(char*) tempFileName);			
				
			#if defined(_WIN32) && defined(_MSC_VER)
			char* cmdBuffer;
			//printf("\ntxt2cdc.exe \"%s\" \"%s\"", tempFileName, tempOutputFileName);
			cmdBuffer = new char[(2 * (strlen(CDCLocation) + 10 + 10)) + 25];		
			sprintf(cmdBuffer, "txt2cdc.exe \"%s\" \"%s\"", tempFileName, tempOutputFileName);
			//printf("\n command Buffer %s",cmdBuffer);
			system(cmdBuffer);
			delete [] cmdBuffer;
			#else
			char* cmdBuffer;
			//printf("\n./txt2cdc \"%s\" \"%s\"", tempFileName, tempOutputFileName);
			cmdBuffer = new char[LINUX_INSTALL_DIR_LEN + (2 * (strlen(CDCLocation) + 10 + 10)) + 25];		
			sprintf(cmdBuffer, "%s/txt2cdc \"%s\" \"%s\"", LINUX_INSTALL_DIR, tempFileName, tempOutputFileName);
			//printf("\n command Buffer %s",cmdBuffer);
			system(cmdBuffer);
			delete [] cmdBuffer;
			#endif
			retCode.code =  OCFM_ERR_SUCCESS ;
			delete [] tempFileName;
			delete [] tempOutputFileName;
			return retCode;
		}
		catch(ocfmException & ex)
		{
			delete [] tempFileName;
			delete [] tempOutputFileName;
			return ex._ocfmRetCode;
		}
	}
	
/****************************************************************************************************
* Function Name: GenerateMNOBD
* Description: Generates the MN Object Dictionary
* Return value: void
****************************************************************************************************/
//void GenerateMNOBD()
//	{
//		CNodeCollection *pobjNodeCollection;
//		pobjNodeCollection= CNodeCollection::getNodeColObject();
//
//		//get the MN Node
//		CNode objMNNode;
//		
//		objMNNode=pobjNodeCollection->getNode(MN,249);
//
//		//get the Index collection of MN Object
//		CIndexCollection *pobjIndexCollection;
//		pobjIndexCollection = objMNNode->getIndexCollection();
//
//		//loop through the Indexes of IndexCollection
//		INT32 iLoopCount=0;
//		for (iLoopCount=0;iLoopCount < pobjIndexCollection->getNumberofIndexes(); iLoopCount++)
//			{
//				CIndex objIndex;
//				objIndex = pobjIndexCollection->getIndex(iLoopCount);
//				objIndex.getIndex();
//				
//			}
//
//
//
//
//	}

/****************************************************************************************************
* Function Name: ProcessCDT
* Description: 
* Return value: void
****************************************************************************************************/
ocfmRetCode  ProcessCDT(CComplexDataType* pobjCDT,CApplicationProcess* pobjAppProc,
																 CNode* pobjNode, Parameter* pobjParameter, EPDOType enumPdoType,
																 char* pbModuleName, char* pbModuleIndex)
{

	/*INT32 StartByteOffset = startingOffset ;*/
	ocfmException objocfmException;
	INT32 iStartBitOffset =  0;
	INT32 iOffset;
	bool bIsNewBitStringVar = true;
	INT32 iDataSize = 0;
	INT32 iTotalBytesMapped = 0;
		
	{
		if(pobjCDT ==NULL)
		{
			objocfmException.ocfm_Excpetion(OCFM_ERR_STRUCT_DATATYPE_NOT_FOUND);
			throw objocfmException;		
		}

		for(INT32 iLoopCount= 0 ; iLoopCount<pobjCDT->varCollection.Count(); iLoopCount++)
		{
	
			varDeclaration objVarDecl;

			objVarDecl.Initialize();
			objVarDecl = pobjCDT->varCollection[iLoopCount];			
			if(objVarDecl.nam_id_dt_attr->getDtUniqueRefId() != NULL)
			{
				pobjCDT = pobjAppProc->getCDTbyUniqueID(objVarDecl.nam_id_dt_attr->getDtUniqueRefId());	
				pobjAppProc->updatePreviousCDT_UId(objVarDecl.StructUniqueId, pobjCDT->Index);		
				/*pobjCDT->previousCDT_UId = (char*)malloc(strlen(objVarDecl.StructUniqueId)+1);
				strcpy(pobjCDT->previousCDT_UId, objVarDecl.StructUniqueId);*/
				iLastVarIndex = iLoopCount;
			/*	printf("\n previousCDT_UId : %s",pobjCDT->previousCDT_UId);
				printf("\n DataRefID : %s",objVarDecl.nam_id_dt_attr->getDtUniqueRefId());
		*/		ProcessCDT(pobjCDT, pobjAppProc, pobjNode, pobjParameter, enumPdoType, pbModuleName, pbModuleIndex );
			}
		if(!bCDTCompleted)
		{	
			// add rest of the contents
			ProcessImage objProcessImage;

			objProcessImage.BitOffset = -1;
			objProcessImage.ByteOffset = NULL;
			if(pobjParameter->access !=NULL)
			strcpy(objProcessImage.Direction,getParameterAccess(pobjParameter->access));
				
			if(enumPdoType == PDO_TPDO)
			{
				objProcessImage.DirectionType = INPUT ;
			}
			else if(enumPdoType == PDO_RPDO)
			{
 				objProcessImage.DirectionType = OUTPUT;
			}
			
				if(objVarDecl.size != NULL)			
				{
				/*	objProcessImage.DataSize = (char*)malloc(5);*/
					//strcpy(objProcessImage.DataInfo.DataSize, objVarDecl.size);
					objProcessImage.DataInfo.DataSize = atoi(objVarDecl.size);					
				}
			else
			{
			/*		objProcessImage.DataSize = (char*)malloc(5);*/
					//strcpy(objProcessImage.DataSize,"000x");
			}
				if(objVarDecl.nam_id_dt_attr->getName()!=NULL)
				{
					objProcessImage.Name = (char*)malloc(strlen(objVarDecl.nam_id_dt_attr->getName()) + strlen(pbModuleName) + 6 + ALLOC_BUFFER);
					strcpy(objProcessImage.Name,getPIName(pobjNode->getNodeId()));
					strcat(objProcessImage.Name, pbModuleName);
					strcat(objProcessImage.Name, ".");
					strcat(objProcessImage.Name,objVarDecl.nam_id_dt_attr->getName());
					
					objProcessImage.ModuleName = (char*)malloc(strlen(pbModuleName) + ALLOC_BUFFER);
					strcpy(objProcessImage.ModuleName, pbModuleName);		
					
						objProcessImage.ModuleIndex = (char*)malloc(strlen(pbModuleIndex) + ALLOC_BUFFER);
					strcpy(objProcessImage.ModuleIndex, pbModuleIndex);	
					
								
					objProcessImage.VarName = (char*)malloc(strlen(objVarDecl.nam_id_dt_attr->getName()) + ALLOC_BUFFER);
					strcpy(objProcessImage.VarName, objVarDecl.nam_id_dt_attr->getName());		
					//printf("\n PI Name: %s",objProcessImage.Name);
				}
				
				objProcessImage.CNNodeID = pobjNode->getNodeId();
					
				if(objVarDecl.nam_id_dt_attr->getDataType()!=NULL)
				{
					objProcessImage.DataInfo._dt_Name = (char*)malloc(strlen(objVarDecl.nam_id_dt_attr->getDataType()) + ALLOC_BUFFER);
					strcpy(objProcessImage.DataInfo._dt_Name ,(const char*)objVarDecl.nam_id_dt_attr->getDataType());
				}
		
			
				/* Set the IEC DT*/
			objProcessImage.DataInfo = *(getIECDT(objVarDecl.nam_id_dt_attr->getDataType(), objProcessImage.DataInfo.DataSize));
			
			/* Calculate Offset*/
			if((objProcessImage.DataInfo.DataSize >= 8 ) || ((objProcessImage.DataInfo.DataSize == 1) && (iStartBitOffset == 0 || iStartBitOffset ==8)))
			{
				
				bIsNewBitStringVar =  true;
				if((objProcessImage.DataInfo.DataSize == 1) && (iStartBitOffset == 0 || iStartBitOffset ==8))
				{
					iDataSize = 8;
					if(iStartBitOffset ==8)
					{						
						iStartBitOffset = 0;					
					}
				}
				else
				{
				 iDataSize =  objProcessImage.DataInfo.DataSize;
				 
				}
			
				if(enumPdoType == PDO_RPDO)
				{
					iOffset =  ComputeOUTOffset(iDataSize, enumPdoType);			
				}
				else if(enumPdoType == PDO_TPDO)
				{
					iOffset =  ComputeINOffset(iDataSize, enumPdoType);					
				}
			}
			/* Set the Byte Offet*/
			objProcessImage.ByteOffset = iOffset;
			if(bIsNewBitStringVar)
			{
				/* Total bytes Mapped */
			 iTotalBytesMapped = iTotalBytesMapped + iDataSize;
				if(iTotalBytesMapped >  MAX_PI_SIZE)
				{
					ocfmException objex;
					objex.ocfm_Excpetion(OCFM_ERR_MAX_PI_SIZE);
					throw objex;
				}
				CreateMNPDOVar(iOffset, iDataSize, objProcessImage.DataInfo._dt_enum, enumPdoType, pobjNode);
			}
			
			if((objProcessImage.DataInfo.DataSize >= 8) && (iStartBitOffset!= 0 ))
			{
				iStartBitOffset = 0;
			}
			else if(objProcessImage.DataInfo._dt_enum  == BITSTRING)			
			{
				objProcessImage.BitOffset = iStartBitOffset;
				iStartBitOffset = iStartBitOffset + objProcessImage.DataInfo.DataSize;
				objProcessImage.ByteOffset = iOffset;
				bIsNewBitStringVar =  false;
			}
		/*	else if
			SetPIOffsets(&objProcessImage, iOffset, iLoopCount,iStartBitOffset); */
		pobjNode->addProcessImage(objProcessImage);
		}
		
	}

		/*if(pobjCDT->VarIndex != -1)
		{
			varDeclaration objVarDecl;
			objVarDecl = pobjCDT->
			pobjCDT
		}*/
		if(pobjCDT->previousCDT_UId !=NULL)
		{
			pobjCDT = pobjAppProc->getCDTbyUniqueID(pobjCDT->previousCDT_UId);
		
			for(INT32 iLoopCount = (iLastVarIndex + 1) ; iLoopCount < pobjCDT->varCollection.Count(); iLoopCount++)
			{
				if(!bCDTCompleted)
				{
					varDeclaration objVarDecl;
					//printf("\n iLoopCount : %d",iLoopCount);
					objVarDecl = pobjCDT->varCollection[iLoopCount];
					//printf("\nVar Count: %d",pobjCDT->varCollection.Count());
					if(objVarDecl.nam_id_dt_attr->getDtUniqueRefId() != NULL)
					{
						/*pobjCDT->previousCDT_UId = (char*)malloc(strlen(objVarDecl.StructUniqueId)+1);
						strcpy(pobjCDT->previousCDT_UId, objVarDecl.StructUniqueId);	*/	
						pobjCDT = pobjAppProc->getCDTbyUniqueID(objVarDecl.nam_id_dt_attr->getDtUniqueRefId());
						pobjAppProc->updatePreviousCDT_UId(objVarDecl.StructUniqueId, pobjCDT->Index);
						//printf("\n previousCDT_UId : %s",pobjCDT->previousCDT_UId);
						//printf("\n DataRefID : %s",objVarDecl.nam_id_dt_attr->getDtUniqueRefId());
				
						iLastVarIndex = iLoopCount;
						ProcessCDT(pobjCDT, pobjAppProc, pobjNode, pobjParameter, enumPdoType, pbModuleName,pbModuleIndex);
					
					}						
				}
			}
		}
		
	}
	bCDTCompleted = true;
	//printf("exiting");
}

/****************************************************************************************************
* Function Name: DecodeUniqiueIDRef
* Description: 
* Return value: void
****************************************************************************************************/
void DecodeUniqiueIDRef(char* uniquedIdref, CNode* pobjNode, EPDOType enumPdoType, char* pbModuleName, char* pbModuleIndex)
{
	ocfmException objocfmException;
	Parameter* pobjParameter;
	CApplicationProcess* pobjAppProc;
	CComplexDataType* pobjCDT;
	
	try
	{
		if(pobjNode->getApplicationProcess()!=NULL)
		{
			
			pobjAppProc = pobjNode->getApplicationProcess();
			if(pobjAppProc->ParameterCollection.Count()!=0)
			{
				
				pobjParameter = pobjAppProc->get_Parameterby_UniqueIDRef(uniquedIdref);
				if(pobjParameter == NULL)
				{
					objocfmException.ocfm_Excpetion(OCFM_ERR_UNIQUE_ID_REF_NOT_FOUND);
					throw objocfmException;
				}
				
				//if(pobjParameter->access != NULL)
				//{
				//	
				//	//procImage.Direction = getParameterAccess(pobjParameter->access);
				//	//strcpy(procImage.Direction, getParameterAccess(pobjParameter->access));
				//	strcpy(procImage.Direction, "rw");
				//	procImage.ParametrIndex = pobjParameter->ParaIndex;

				//}
				// Check if DataTypeUniqueIDref exists
				if(pobjParameter->name_id_dt_attr.dataTypeUniqueIDRef !=NULL)
				{
							
						pobjCDT = pobjAppProc->getCDTbyUniqueID(pobjParameter->name_id_dt_attr.dataTypeUniqueIDRef);
						if(pobjCDT ==NULL)
						{
							objocfmException.ocfm_Excpetion(OCFM_ERR_STRUCT_DATATYPE_NOT_FOUND);
							throw objocfmException;
						}
						ProcessCDT(pobjCDT, pobjAppProc, pobjNode, pobjParameter, enumPdoType, pbModuleName, pbModuleIndex); 
						iLastVarIndex = -1;
						bCDTCompleted = false;
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
/****************************************************************************************************
* Function Name: ProcessPDONodes
* Description: Processes the Node
* Return value: ocfmRetCode
****************************************************************************************************/
ocfmRetCode ProcessPDONodes()
{
		CNodeCollection* objNodeCol;
		ocfmException objocfmException;
		objNodeCol = CNodeCollection::getNodeColObjectPointer();
		CNode* pobjNode;
		INT32 iTotalBytesMapped = 0;
			
		CIndexCollection* objPDOCollection;
		CIndexCollection* pobjIndexCollection;
		/* Check RPDO Mapped objects*/
		INT32 iNodesCount = 0;
		ocfmRetCode stRetInfo;
		
		iNodesCount = objNodeCol->getCNNodesCount();
		
			#if defined DEBUG	
						cout<< "Nodes cunt"<<iNodesCount<< endl;
					#endif
		try
		{
			/*if(NodesCount == 0)
			{				
				objex.ocfm_Excpetion(OCFM_ERR_NO_CN_NODES_FOUND);
				throw objex;
			}*/
			if(iNodesCount == 0)
			{				
				exit;
			}

				stSize8INOffset.currOffset  = 0; stSize8INOffset.prevOffset = 0;
				stSize16INOffset.currOffset  = 0; stSize16INOffset.prevOffset = 0;
				stSize32INOffset.currOffset  = 0; stSize32INOffset.prevOffset = 0;
				stSize64INOffset.currOffset  = 0; stSize64INOffset.prevOffset = 0;
			
				stSize8OUTOffset.currOffset  = 0; stSize8OUTOffset.prevOffset = 0;
				stSize16OUTOffset.currOffset  = 0; stSize16OUTOffset.prevOffset = 0;
				stSize32OUTOffset.currOffset  = 0; stSize32OUTOffset.prevOffset = 0;
				stSize64OUTOffset.currOffset  = 0; stSize64OUTOffset.prevOffset = 0;

			for(INT32 iLoopCount = 0; iLoopCount < objNodeCol->getNumberOfNodes(); iLoopCount++)
			{
			//printf("\n iiNodeID %d",NodeID);
				pobjNode = objNodeCol->getNodebyColIndex(iLoopCount);
				/* Process PDO Objects for CN*/
			
					if (pobjNode->getNodeType() != MN )
					{
						if(!(pobjNode->HasPdoObjects()))	
						{
					
							continue;
						}
					

							/* Empty ProcessImage collection*/
							pobjNode->DeleteCollectionsForPI();
							
							
							objPDOCollection = pobjNode->getPDOIndexCollection();
						
							if(objPDOCollection!= NULL)
							{
									
								pobjIndexCollection = pobjNode->getIndexCollection();
								
								for(INT32 iLoopCount = 0; iLoopCount<objPDOCollection->getNumberofIndexes(); iLoopCount++)
								{
										CIndex* pobjBforeSortIndex;
										CIndex objIndex;
										pobjBforeSortIndex = objPDOCollection->getIndex(iLoopCount);
										if(!(CheckIfMappingPDO((char*)pobjBforeSortIndex->getIndexValue())))
										{
											continue;
										}
										else
										{
												#if defined DEBUG	
									cout<< "index:"<<(char*)pobjIndex->getIndexValue() << endl;
								#endif
										}
										if(pobjBforeSortIndex->getNumberofSubIndexes() > 0)
										{
										
													/* Sort the pdo collection */
											objIndex = getPDOIndexByOffset(pobjBforeSortIndex);
											
											INT32 iSiCount = 1;
											INT32 iSiTotal = objIndex.getNumberofSubIndexes();
									
												#if defined DEBUG	
									cout<< "iSiTotal:"<<iSiTotal << endl;
								#endif
											while(iSiCount< iSiTotal)
											{
												CSubIndex* pobjSubIdx;
												pobjSubIdx = objIndex.getSubIndex(iSiCount);
													#if defined DEBUG	
									cout<< "\n pobjSubIdx->getIndexValue():"<<pobjSubIdx->getIndexValue() << endl;
										cout<< "\n pobjSubIdx->getName():"<<pobjSubIdx->getName() << endl;
									
								

								#endif
												if ( (pobjSubIdx->getActualValue()!=NULL) && (0 != strcmp(pobjSubIdx->getActualValue(),"")) && !(checkIfValueZero((char*)pobjSubIdx->getActualValue())) )
												{
														
														const char* pbActualVal = pobjSubIdx->getActualValue();
														INT32 iLength = strlen(pbActualVal);
														//char* reverseValue = (char*)malloc(iLength);
														/* Reverse the actual pbActualVal to get Index / subindex*/
												/*		reverseValue = reverse((char*)pbActualVal);*/
														
														/* Get the Index*/
														//char* strModuleIndex = (char*)malloc(INDEX_SIZE + ALLOC_BUFFER);
														char* pbModuleIndex = NULL;// = new char[INDEX_SIZE + ALLOC_BUFFER];
														pbModuleIndex = subString((char*)pbActualVal, iLength-4,4);
														pbModuleIndex[5] ='\0';
												/*		pbModuleIndex = strncpy(pbModuleIndex,reverseValue,4);
												*/				#if defined DEBUG	
									cout<< "pbModuleIndex:"<<pbModuleIndex << endl;
								#endif
												
												/* Get the SubIndex*/
														//char* strSubIndex = (char*)malloc(SUBINDEX_SIZE + ALLOC_BUFFER);
														char* pbSubIndex = NULL;// = new char[SUBINDEX_SIZE + ALLOC_BUFFER];
														//pbSubIndex = subString(reverseValue,2,2);
														pbSubIndex = subString((char*)pbActualVal, iLength-6,2);
														pbSubIndex[3] ='\0';
														
														#if defined DEBUG	
									cout<< "pbSubIndex:"<<pbSubIndex << endl;
								#endif
													
												
														CIndex* pobjModuleIndex;
														CSubIndex* pobjModuleSIndex;
														char* uniqueidRefID = NULL;			
														char* pbSIdxName = NULL;
														char* Access = NULL;
														char *pbModuleName = NULL;
														DataType dt;	
														dt.Name =  NULL;				
														bool bObjectMapped = false;	
												
																		
															pobjModuleIndex = pobjIndexCollection->getIndexbyIndexValue(pbModuleIndex);
															if(pobjModuleIndex==NULL)
															{								
																objocfmException.ocfm_Excpetion(OCFM_ERR_MODULE_INDEX_NOT_FOUND);
																throw objocfmException;
															}
															
															if(pobjModuleIndex->getName() != NULL)
															{
																	
																		pbModuleName = new char[strlen(pobjModuleIndex->getName()) + ALLOC_BUFFER];
																		strcpy(pbModuleName, pobjModuleIndex->getName());
															}
															if(pobjModuleIndex->getNumberofSubIndexes() == 0 && (strcmp(pbSubIndex, "00")==0))
															{
																bObjectMapped = true;
																if(pobjModuleIndex->getUniqueIDRef()!= NULL)
																{
																	uniqueidRefID =  new char[strlen(pobjModuleIndex->getUniqueIDRef()) +  ALLOC_BUFFER];
																	strcpy(uniqueidRefID, pobjModuleIndex->getUniqueIDRef());
																}				
																else
																{
																		if(pobjModuleIndex->getAccessType() != NULL)
																		{
																			Access = new char[strlen(pobjModuleIndex->getAccessType()) + ALLOC_BUFFER];
																			strcpy(Access, pobjModuleIndex->getAccessType());
																			
																		}
																	dt = pobjModuleIndex->getDataType();
																}				
																					
															}
															else
															{
 															pobjModuleSIndex = pobjModuleIndex->getSubIndexbyIndexValue(pbSubIndex);
															if(pobjModuleSIndex==NULL)
															{								
																objocfmException.ocfm_Excpetion(OCFM_ERR_MODULE_SUBINDEX_NOT_FOUND);
																throw objocfmException;
																}
																else
																{
																	if(pobjModuleSIndex->getUniqueIDRef() != NULL)
																	{
																		uniqueidRefID =  new char[strlen(pobjModuleSIndex->getUniqueIDRef()) +  ALLOC_BUFFER];
																		strcpy(uniqueidRefID, pobjModuleSIndex->getUniqueIDRef());
																	}
																	else
																	{
																		if(pobjModuleSIndex->getName() != NULL)
																		{
																		pbSIdxName = new char[strlen(pobjModuleSIndex->getName()) + ALLOC_BUFFER];
																		strcpy(pbSIdxName, pobjModuleSIndex->getName());
																	
																			if(pobjModuleSIndex->getAccessType() != NULL)
																			{
																		Access = new char[strlen(pobjModuleSIndex->getAccessType()) + ALLOC_BUFFER];
																		strcpy(Access, pobjModuleSIndex->getAccessType());
																			}
																		dt = pobjModuleSIndex->getDataType();
																		}
																	}
																}
																
															}
															
															
															EPDOType pdoType = objIndex.getPDOType();							
															
														//printf("\n NodeID %d",objNode->getNodeId());
															if(uniqueidRefID != NULL)
															{
																//DecodeUniqiueIDRef(objSIndex->getUniqueIDRef(), objNode, pdoType, (char*) objModuleIndex->getName(), (char*)objModuleIndex->getIndexValue());
																DecodeUniqiueIDRef(uniqueidRefID, pobjNode, pdoType, (char*) pobjModuleIndex->getName(), (char*)pobjModuleIndex->getIndexValue());
																//printf("\n size8 prev offset: %d",size8INOffset.prevOffset);
																//printf("\n size8 curr offset: %d",size8INOffset.currOffset);
															}
 															else
															{
																ProcessImage objProcessImage;
																
																if(dt.getName() == NULL )
																{																
																	objocfmException.ocfm_Excpetion(OCFM_ERR_INVALID_DATATYPE_FOR_PDO);
																	throw objocfmException;
																}
																else if(!CheckAllowedDTForMapping(dt.getName()))
																{
																		objocfmException.ocfm_Excpetion(OCFM_ERR_INVALID_DATATYPE_FOR_PDO);
																		throw objocfmException;
																}
																		objProcessImage.CNNodeID = pobjNode->getNodeId();
																		/* Name of the Process Image variable*/
																	objProcessImage.Name = (char*)malloc(6 + ALLOC_BUFFER);
																	strcpy(objProcessImage.Name, getPIName(pobjNode->getNodeId()));
																																	
																	if(pbModuleName != NULL)
																	{
																		objProcessImage.Name = (char*)realloc(objProcessImage.Name, strlen(objProcessImage.Name)+ strlen(pbModuleName) + ALLOC_BUFFER);																		
																		strcat(objProcessImage.Name, pbModuleName);
																		
																/*	objProcessImage.Name = (char*)malloc(strlen(Name) + 6 + ALLOC_BUFFER);
																	strcpy(objProcessImage.Name,getPIName(pobjNode->getNodeId()));*/
																	objProcessImage.ModuleName = (char*)malloc(strlen(pbModuleName) + ALLOC_BUFFER);
																	strcpy(objProcessImage.ModuleName, pbModuleName);		
																	}
																	else
																	{
																		/*If the object name is empty, Ixxxx (xxxx is the index number in hex) */
																	//	cout << "\nObjectName empty" << endl;
																		pbModuleName = new char[INDEX_LEN + 1 + ALLOC_BUFFER];
																		strcpy(pbModuleName, "I");
																		strcat(pbModuleName, objIndex.getIndexValue());
																		
																		//cout << "pbModuleName" << pbModuleName;
																		objProcessImage.Name = (char*)realloc(objProcessImage.Name, strlen(objProcessImage.Name)+ strlen(pbModuleName) + ALLOC_BUFFER);																		
																		strcat(objProcessImage.Name, pbModuleName);
																	
																		objProcessImage.ModuleName = (char*)malloc(strlen(pbModuleName) + ALLOC_BUFFER);
																		strcpy(objProcessImage.ModuleName, pbModuleName);		
																	}
																
											
																	//strcat(pi.Name, ModuleName);
																	//strcat(pi.Name,objSIndex->getName());
																	
																	//pi.ModuleName = (char*)malloc(strlen(ModuleName) + ALLOC_BUFFER);
																	//strcpy(pi.ModuleName, ModuleName);		
																	
																		objProcessImage.ModuleIndex = (char*)malloc(strlen(pbModuleIndex) + ALLOC_BUFFER);
																	strcpy(objProcessImage.ModuleIndex, pbModuleIndex);	
																	
																				
																	//pi.VarName = (char*)malloc(strlen(objSIndex->getName()) + ALLOC_BUFFER);
																	if(bObjectMapped)
																	{
																	//	cout << "\n objectmapped";																			
																		objProcessImage.VarName = (char*)malloc(strlen(pbModuleName) + ALLOC_BUFFER);
																		strcpy(objProcessImage.VarName, pbModuleName);
																	}
																	else
																	{
																		if(pbSIdxName != NULL)
																		{
																			objProcessImage.VarName = (char*)malloc(strlen(pbSIdxName) + ALLOC_BUFFER);
																			strcpy(objProcessImage.VarName, pbSIdxName);		
																//			cout << "\n SubObjectName" << pbSIdxName;
																		
																			objProcessImage.Name = (char*)realloc(objProcessImage.Name,(strlen(objProcessImage.Name) + strlen(pbSIdxName) + 1+ ALLOC_BUFFER));
																			strcat(objProcessImage.Name, ".");
																			strcat(objProcessImage.Name, pbSIdxName);
																		}
																		else
																		{
																				
																			/*If the subobject name is empty, use Sxx (xx is the subindex number in hex) */
																			pbSIdxName = new char[SUBINDEX_LEN + 1 + ALLOC_BUFFER];
																			
																			strcpy(pbSIdxName, "S");
																			strcat(pbSIdxName, pobjModuleSIndex->getIndexValue());
																			
																		
																			objProcessImage.VarName = (char*)malloc(strlen(pbSIdxName) + ALLOC_BUFFER);
																			strcpy(objProcessImage.VarName, pbSIdxName);		
																			
																			objProcessImage.Name = (char*)realloc(objProcessImage.Name,(strlen(objProcessImage.Name) + strlen(pbSIdxName) + 1+ ALLOC_BUFFER));
																			strcat(objProcessImage.Name, ".");
																			strcat(objProcessImage.Name, pbSIdxName);
																			
															
																		
																		}
																	}
															 
													
																
																/* Access of the Process Image variable*/
																//strcpy(pi.Direction, objSIndex->getAccessType());
														
																//DataType dt;
																//dt = objSIndex->getDataType();
																/* Data Size in hex of the Process Image variable*/
															/*	objProcessImage.DataSize = (char*)malloc(strlen(dt.DataSize+1));*/
																//strcpy(objProcessImage.DataSize, dt.DataSize);
																//printf("\n Datasize %d", dt.DataSize);
																
																
																//pi.DataInfo.DataSize = atoi(dt.DataSize);
																objProcessImage.DataInfo.DataSize = dt.DataSize *8;
																
																/* Total bytes Mapped */
																iTotalBytesMapped = iTotalBytesMapped +  dt.DataSize *8;;
																if(iTotalBytesMapped >  MAX_PI_SIZE)
																{
																	ocfmException objex;
																	objex.ocfm_Excpetion(OCFM_ERR_MAX_PI_SIZE);
																	throw objex;
																}
																/* Datatype in hex of the Process Image variable*/
																objProcessImage.DataInfo._dt_Name = (char*)malloc(strlen(dt.Name) + ALLOC_BUFFER);
																strcpy(objProcessImage.DataInfo ._dt_Name, dt.Name);	
																objProcessImage.DataInfo._dt_enum = dt.IEC_dt;
																
																	if( pdoType == PDO_TPDO)
																	{
																		objProcessImage.ByteOffset = ComputeINOffset(objProcessImage.DataInfo.DataSize, pdoType);			
																		objProcessImage.DirectionType = INPUT ;
																	}
																	else if( pdoType == PDO_RPDO)
																	{
																		objProcessImage.ByteOffset = ComputeOUTOffset(objProcessImage.DataInfo.DataSize, pdoType);			
																		objProcessImage.DirectionType = OUTPUT;
																	}
		
																objProcessImage.BitOffset = 0;
																CreateMNPDOVar(objProcessImage.ByteOffset, objProcessImage.DataInfo.DataSize, objProcessImage.DataInfo._dt_enum, pdoType, pobjNode);
																pobjNode->addProcessImage(objProcessImage);
																delete[] pbModuleName ;
																delete[] pbSIdxName;
															}															
													}
													iSiCount++;
												}
											}
										}
									}
								}
							}
			stRetInfo.code = OCFM_ERR_SUCCESS ;
			return stRetInfo;
		}
		catch(ocfmException& ex)
		{
			return ex._ocfmRetCode;
		}
	}
/*	
INT32 ConvertCdcToBinary(char* cdcfileName,char* temptxtFileName)
{
		FILE		*fptrInputTxt	= NULL; 
	FILE		*fptrOutputCdc	= NULL;
	errno_t		ret;
	CHAR		ch			= 0;

	UINT32		u32Ctr		= 0;
	UINT32		u32Ctr1		= 0;

	UINT32		u32Length;
	UINT32		u32LengthMNObj = 0;
	UINT32		u32LengthObj;

	UINT8 acOddEntries[10000];
	UINT8 acOddEntriesTemp[10000];

	//if(argc<=1 || argc>2)
	//
	//	printf("\n Invalid argument \n");
	//	goto Exit;
	//}
	//else
	//{

	 //ret = fopen_s(&fptrInputTxt, (const char *)argv[1], "r");
#if defined(_WIN32) && defined(_MSC_VER)
	ret = fopen_s(&fptrInputTxt,temptxtFileName, "r");
#else
	//Write Linux code here
#endif
	 if (ret != 0)
	 {
		 printf("Error opening input file\n");
		 return -1;
	 }
	 ret = fopen_s(&fptrOutputCdc, cdcfileName, "wb");
	 if (ret != 0)
	 {
		 printf("Error opening cdc file\n");
		 return -1;
	 }



		while(ch != EOF)
		{
			ch = fgetc(fptrInputTxt);
			if(ch == '/') 
		    {
				while(fgetc(fptrInputTxt) != '\n');
		    }
			else if(!(ch == '\t' || ch == '\n' || ch == ' '))
			{
				//printf("%c, ", ch);
				// Convert to Upper case
				ch = toupper(ch);
				//Convert Char to Binary Value
				ch = tobin(ch);

				if (ch >= 0 && ch <= 0xF)
				{
					acOddEntries[u32Ctr] = ch;
					u32Ctr++;
				}
			}
		}
		//Store the number of entries (characters in the file that of interest to us)
		u32Length = u32Ctr;

		
		//For testing purpose.
		//for ( u32Ctr = 0; u32Ctr < (u32Length - 1); u32Ctr++)
		//{
		//	printf("%x, ",acOddEntries[u32Ctr]);
		//}

		//For Byte Packing
		for (u32Ctr = 0 , u32Ctr1 = 0; u32Ctr < (u32Length - 1); u32Ctr++, u32Ctr1++ )
		{
			u32Ctr = u32Ctr1 * 2;
			acOddEntriesTemp[u32Ctr1] = (UINT8)( ( acOddEntries[ u32Ctr ] << 4 ) | acOddEntries[ u32Ctr + 1 ] );
			//printf("%.2x, ",acOddEntriesTemp[u32Ctr1]);
		}
		
		//Store the number of entries (characters in the file that of interest to us)
		u32Length = u32Ctr1;

		//Copy content into 
		//for (u32Ctr = 0; u32Ctr < (u32Length - 1); u32Ctr++)		
		//{
		//	acOddEntries[u32Ctr] = acOddEntriesTemp[u32Ctr];
		//}

		printf("Size : %x\n", u32Length);

		reversedata(&acOddEntries[OFFSET_NUM_ENTRIES_MN], &acOddEntriesTemp[OFFSET_NUM_ENTRIES_MN], sizeof(UINT32));

		printf("Number of Entries: %x\n", * (UINT32 *)&acOddEntries[0]);

		u32Ctr = OFFSET_FIRST_INDEX_IN_MN_OBD; //Skip the number of elements field

		//For Byte Ordering 
		while ( u32Ctr < (u32Length - 1))
		{

			//Index
			reversedata(&acOddEntries[u32Ctr + OFFSET_INDEX], &acOddEntriesTemp[u32Ctr + OFFSET_INDEX], sizeof(UINT16));
			//SubIndex
			acOddEntries[u32Ctr + OFFSET_SUBINDEX] = acOddEntriesTemp[u32Ctr + OFFSET_SUBINDEX];
			//DataSize
			reversedata(&acOddEntries[u32Ctr + OFFSET_SIZE], &acOddEntriesTemp[u32Ctr + OFFSET_SIZE], sizeof(UINT32));

			u32LengthMNObj = * ((UINT32 *)&acOddEntries[u32Ctr + OFFSET_SIZE]);

			//Data should also be reversed correctly and also if its 1F22's data it needs special treatment
			if (* (UINT16*) &acOddEntries[u32Ctr] == 0x1F22)
			{
				//printf("Size : %x\n", u32LengthMNObj);

				//Just after the index, subindex and size we will have the number of entries
				reversedata(&acOddEntries[u32Ctr + OFFSET_NUM_ENTRIES_CN], &acOddEntriesTemp[u32Ctr + OFFSET_NUM_ENTRIES_CN], sizeof(UINT32));

				printf("Number of Entries: %x\n", * (UINT32 *)&acOddEntries[u32Ctr + OFFSET_NUM_ENTRIES_CN]);

				u32Ctr1 = u32Ctr + OFFSET_FIRST_INDEX_IN_1F22; //first index within the 1F22 object
				
				while ( u32Ctr1 < (u32Ctr + OFFSET_DATA + u32LengthMNObj - 1))
				{

					//Index
					reversedata(&acOddEntries[u32Ctr1 + OFFSET_INDEX], &acOddEntriesTemp[u32Ctr1 + OFFSET_INDEX], sizeof(UINT16));
					//SubIndex
					acOddEntries[u32Ctr1 + OFFSET_SUBINDEX] = acOddEntriesTemp[u32Ctr1 + OFFSET_SUBINDEX];
					//DataSize
					reversedata(&acOddEntries[u32Ctr1 + OFFSET_SIZE], &acOddEntriesTemp[u32Ctr1 + OFFSET_SIZE], sizeof(UINT32));

					u32LengthObj = * (UINT32 *) &acOddEntries[u32Ctr1 + OFFSET_SIZE];

					//Data should also be reversed correctly
					reversedata(&acOddEntries[u32Ctr1 + OFFSET_DATA], &acOddEntriesTemp[u32Ctr1 + OFFSET_DATA], u32LengthObj);
					//printf("\tIndex: %x SubIndex: %x Size %x\n", * (UINT16*) &acOddEntries[u32Ctr1 + OFFSET_INDEX], * (UINT8 *) &acOddEntries[u32Ctr1 + OFFSET_SUBINDEX], * (UINT32 *) &acOddEntries[u32Ctr1 + OFFSET_SIZE] );
					u32Ctr1 = u32Ctr1 + OFFSET_DATA + u32LengthObj;
				}				

			}
			else
			{
				//printf("Index: %x SubIndex: %x Size %x\n", * (UINT16*) &acOddEntries[u32Ctr + OFFSET_INDEX], * (UINT8 *) &acOddEntries[u32Ctr + OFFSET_SUBINDEX], u32LengthMNObj );
				//Get the length of the data to be reversed				
				reversedata(&acOddEntries[u32Ctr + OFFSET_DATA], &acOddEntriesTemp[u32Ctr + OFFSET_DATA], u32LengthMNObj);
			}

			//printf("Index: %x SubIndex: %x Size %x\n", * (UINT16*) &acOddEntries[u32Ctr + OFFSET_INDEX], * (UINT8 *) &acOddEntries[u32Ctr + OFFSET_SUBINDEX], u32LengthMNObj );
			u32Ctr = u32Ctr + OFFSET_DATA + u32LengthMNObj ;
		}

		// Write to Binary file
		fwrite(&acOddEntries,1,u32Length,fptrOutputCdc);

	fclose(fptrInputTxt);
	fclose(fptrOutputCdc);

	return 0;
	
}
*/
//void ConvertCdcToBinary(char* pbFileName,char* tempFile)
//{
//	FILE *fin, *fout;
//	INT32 iLoopCount=0;
//	INT32 num=0;
//	char ch=0;
//	INT32 iCtr;
//	INT32 iLength;
//	unsigned char ca_cn1obd[10000];
//	unsigned char tempCn1Obd[10000];
//
//	fin = fopen(tempFile, "r");
//	//filePath = (char*)malloc(80);
//	//filePath = strchr(reverse(pbFileName),'\\');
//	//filePath = subString(pbFileName, 0, strlen(pbFileName) - strlen(filePath) +1);
//	//strcat(filePath,"config_data.cdc");
//	////fout = fopen("config_data.cdc", "wb");
//	fout = fopen(pbFileName, "wb");
//	
//	
//		while(ch != EOF)
//		{
//			ch = fgetc(fin);
//			if(ch == '/') 
//		    {
//				while(fgetc(fin) != '\n');
//		    }
//			else if( ch != '\t' || ch != '\n' || ch != ' ')
//			{
//				// Convert to Upper case
//				ch = toupper(ch);
//				if((ch >= 65 && ch <= 70) || (ch >= 97 && ch <=102))
//				{
//					ca_cn1obd[num] = ch - 55;
//					num++;
//				}
//				else if (( ch >=48 && ch <= 57))
//				{
//					ca_cn1obd[num] = ch - 48;
//					num++;
//				}
//		}
//		//ca_cn1obd[num] = '\0';
//		// For testing purpose.
//		/*for ( iLoopCount = 0; iLoopCount<num ; iLoopCount++)
//		{
//			printf("%x",ca_cn1obd[iLoopCount]);
//
//		}*/
//		iLength = num;
//		//printf( "\niLength = %d\n",iLength);
//		//For Byte Packing
//		for (iCtr = 0 , iLoopCount = 0; iCtr < iLength; iCtr++, iLoopCount++ )
//		{
//			tempCn1Obd[iLoopCount] = (unsigned char)( ( ca_cn1obd[ iCtr ] << 4 ) | ca_cn1obd[ ++iCtr ] );
//			//printf("0x%2x\t",tempCn1Obd[iLoopCount]);
//		}
//		//printf("Size : %d\n", iLoopCount);
//		iLoopCount = iLoopCount -1;
//		// Write to Binary file
//		fwrite(&tempCn1Obd,1,iLoopCount,fout);
//	
//	}
//	
//	fclose(fin);
//	fclose(fout);	
//}

/****************************************************************************************************
* Function Name: getCNDataLen
* Description: 
* Return value: INT32
****************************************************************************************************/
INT32 getCNDataLen(char* pbBuffer)
{
	INT32 iCount = 0;
	INT32 iNoOfChars = 0;
	char cCharRead = 0;
	INT32 iCtr;
	INT32 iLength;
	INT32 iLoopCount =0;
	unsigned char abCnObd[60000];
	unsigned char abTempCnObd[60000];
	int i =0;
	
	cCharRead = *(pbBuffer);

	while(cCharRead !='\0')
	{
		cCharRead = *(pbBuffer + iLoopCount);
		if(cCharRead == '/') 
		{
			while(cCharRead != '\n')
			{
				cCharRead = *(pbBuffer + iLoopCount);
				iLoopCount++;
			}
				/*while(fgetc(fin) != '\n');*/
		}
		else if( cCharRead != '\t' || cCharRead != '\n' || cCharRead != ' ')
		{
				// Convert to Upper case
				cCharRead = toupper(cCharRead);
				if((cCharRead >= 65 && cCharRead <= 70) || (cCharRead >= 97 && cCharRead <=102))
				{
					abCnObd[iNoOfChars] = cCharRead - 55;
					iNoOfChars++;
				}
				else if (( cCharRead >=48 && cCharRead <= 57))
				{
					abCnObd[iNoOfChars] = cCharRead - 48;
					iNoOfChars++;
				}
				iLoopCount++;
			}
		
		iLength = iNoOfChars;
		//printf( "\niLength = %d\n",iLength);
		//For Byte Packing
		for (iCtr = 0 , iCount = 0; iCtr < iLength; iCtr++, iCount++ )
		{
			abTempCnObd[iCount] = (unsigned char)( ( abCnObd[ iCtr ] << 4 ) | abCnObd[ iCtr + 1 ] );
			iCtr++;
			//printf("0x%2x\t",abTempCnObd[iCount]);
		}
		//printf("Size : %d\n", iCount);
		/*iCount = iCount -1;*/
		}
		return iCount;
}

/****************************************************************************************************
* Function Name: lenOfCNBuffer
* Description: 
* Return value: INT32
****************************************************************************************************/
 INT32 lenOfCNBuffer(char* pbBuffer)
 {
		char cTempChar;
		INT32 iLength = strlen(pbBuffer);
		INT32 iLoopCount = 0;
		INT32 iActualLen = 0;

		while(iLoopCount < iLength)
		{
			cTempChar = *(pbBuffer + iLoopCount);
			if(cTempChar!='\n' && cTempChar != '\t')
			iActualLen++;
			iLoopCount++;
		}
		return iActualLen;

 }

/****************************************************************************************************
* Function Name: getMNIndexValues
* Description: 
* Return value: CIndex*
****************************************************************************************************/
CIndex* getMNIndexValues(char* pbIndex)
{
		CNodeCollection* pobjNodeCollection;
		pobjNodeCollection = CNodeCollection::getNodeColObjectPointer();		
		CIndexCollection* objIndexCol;
		CNode objNode;

		objNode = pobjNodeCollection->getMNNode();
		objIndexCol = objNode.getIndexCollection();
		CIndex* pobjIndex = NULL;
		pobjIndex = objIndexCol->getIndexbyIndexValue(pbIndex);
		return pobjIndex;
}

/****************************************************************************************************
* Function Name: getMNSubIndexValues
* Description: 
* Return value: CSubIndex*
****************************************************************************************************/
CSubIndex* getMNSubIndexValues(char* pbIndex, char* pbSubIndex)
{
        CNodeCollection* pobjNodeCollection;
        pobjNodeCollection = CNodeCollection::getNodeColObjectPointer();        
        CIndexCollection* objIndexCol;
        CNode objNode;

        objNode = pobjNodeCollection->getMNNode();
        objIndexCol = objNode.getIndexCollection();
        CIndex* pobjIndex = NULL;
        pobjIndex = objIndexCol->getIndexbyIndexValue(pbIndex);
        if(NULL == pobjIndex)
            return NULL;

        CSubIndex* pobjSubIndex = NULL;
        pobjSubIndex = pobjIndex->getSubIndexbyIndexValue(pbSubIndex);
        return pobjSubIndex;
}

/****************************************************************************************************
* Function Name: ImportObjDictXML
* Description: Imports the Object Dictionary XML file. Objects from this Object dictionary file 
	will be used when Adding Index and/or Adding SubIndex
* Return value: ocfmRetCode
****************************************************************************************************/
//ocfmRetCode ImportObjDictXML(char* pbFileName)
//{
//	xmlTextReaderPtr pxReader;
//	INT32 ret;
//	ocfmRetCode stErrorInfo;
//	CIndex objIndex;
//
//	cout << "ImportObjDictXML:" << pbFileName << endl;
//	
//	CObjectDictionary* ObjDictObject;
//	ObjDictObject = CObjectDictionary::getObjDictObjectPointer();
//	//char* pbIndexID = "1006";
//	
//	//objIndex = ObjDictObject->CreateIndexCollection();
//	
//	//pobjNodeCollection= CNodeCollection::getNodeColObjectPointer();
//	//objNode = pobjNodeCollection->getNode(enumNodeType, iNodeID);
//	//pobjIndexCollection = objNode.getIndexCollection();
//			
//	//AddIndexAttributes(pbIndexID, &objIndex);
//	//ObjDictObject->addIndex(objIndex);
//
//	//pxReader = xmlReaderForFile(pbFileName, NULL, 0);
//	//if (pxReader != NULL)
//	//{
//	//	ret = xmlTextReaderRead(pxReader);
//	//	while (ret == 1)
//	//	{		
//	//		processObjDict(pxReader);
//	//		ret = xmlTextReaderRead(pxReader);
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

/****************************************************************************************************
* Function Name: WriteXAPElements
* Description: 
* Return value: void
****************************************************************************************************/
void WriteXAPElements(ProcessImage aobjPICol[], xmlTextWriterPtr& pxtwWriter,INT32 iVarCount, EPIDirectionType piType)
{
	INT32 iBytesWritten;

	if (iVarCount!=0)
	{
	/* Start an element named "Channel". Since thist is the first
     * element, this will be the root element of the document. */
    iBytesWritten = xmlTextWriterStartElement(pxtwWriter, BAD_CAST "ProcessImage");
    if (iBytesWritten < 0)
    {
        //printf("testXmlwriterDoc: Error at xmlTextWriterStartElement\n");
        return;
    }
    if(piType ==INPUT)
					iBytesWritten = xmlTextWriterWriteAttribute(pxtwWriter, BAD_CAST "type",      
                                   BAD_CAST "input");
				else if(piType ==OUTPUT)
					iBytesWritten = xmlTextWriterWriteAttribute(pxtwWriter, BAD_CAST "type",      
                                   BAD_CAST "output");
				
                                  
    if (iBytesWritten < 0)
    {
        //printf("testXmlwriterDoc: Error at xmlTextWriterWriteAttribute\n");
        return;
    }
    
	for(INT32 iLoopCount =0 ; iLoopCount < iVarCount ; iLoopCount++)
			{
					ProcessImage objProcessImage;
					objProcessImage = aobjPICol[iLoopCount];
	 /* Start an element named "Channel". Since thist is the first
     * element, this will be the root element of the document. */
    iBytesWritten = xmlTextWriterStartElement(pxtwWriter, BAD_CAST "Channel");
    if (iBytesWritten < 0)
    {
        //printf("testXmlwriterDoc: Error at xmlTextWriterStartElement\n");
        return;
    }
			


    

    /* Add an attribute with name "Name" and value  to channel. */
     //printf("\n%s",objProcessImage.Name);
    iBytesWritten = xmlTextWriterWriteAttribute(pxtwWriter, BAD_CAST "Name",      
                                   BAD_CAST objProcessImage.Name);
                                  
    if (iBytesWritten < 0)
    {
        //printf("testXmlwriterDoc: Error at xmlTextWriterWriteAttribute\n");
        return;
    }
    
    

    ///* Add an attribute with name "direction" and value to channel */
    //  printf("\n%s",objProcessImage.Direction);
    //iBytesWritten = xmlTextWriterWriteAttribute(pxtwWriter, BAD_CAST "Direction",
    //                                 BAD_CAST objProcessImage.Direction );
    //if (iBytesWritten < 0)
    //{
    //    printf("testXmlwriterDoc: Error at xmlTextWriterWriteAttribute\n");
    //    return;
    //}

				/* Add an attribute with name "DataType" and value to channel */
				 //printf("\n%s",objProcessImage.DataType);
    iBytesWritten = xmlTextWriterWriteAttribute(pxtwWriter, BAD_CAST "dataType",
                                     BAD_CAST objProcessImage.DataInfo._dt_Name );
    if (iBytesWritten < 0)
    {
        //printf("testXmlwriterDoc: Error at xmlTextWriterWriteAttribute\n");
        return;
    }
    
    ///* Add an attribute with name "dataSize" and value to channel */
    //printf("\n%s",objProcessImage.DataSize);
    char * pbDataSize = new char[20];
    pbDataSize = _IntToAscii(objProcessImage.DataInfo.DataSize,pbDataSize,10);
    
    iBytesWritten = xmlTextWriterWriteAttribute(pxtwWriter, BAD_CAST "dataSize",
                                     BAD_CAST pbDataSize );
    if (iBytesWritten < 0)
    {
        //printf("testXmlwriterDoc: Error at xmlTextWriterWriteAttribute\n");
        return;
    }
			
   
     ///* Add an attribute with name "dataSize" and value to channel */
    //printf("\n%s",objProcessImage.ByteOffset);
  		char* pbByteOffset = new char[6];
				pbByteOffset = _IntToAscii(objProcessImage.ByteOffset, pbByteOffset, 16);
				pbByteOffset = ConvertToHexformat(pbByteOffset, 4, 1);
			
    iBytesWritten = xmlTextWriterWriteAttribute(pxtwWriter, BAD_CAST "PIOffset",
                                     BAD_CAST pbByteOffset );
    if (iBytesWritten < 0)
    {
        //printf("testXmlwriterDoc: Error at xmlTextWriterWriteAttribute\n");
        return;
    }
    
    if(objProcessImage.BitOffset!=-1)
    {
						char* pbBitOffset = new char[4];
						pbBitOffset = _IntToAscii(objProcessImage.BitOffset, pbBitOffset, 16);
						pbBitOffset = ConvertToHexformat(pbBitOffset, 2, 1);
			
					iBytesWritten = xmlTextWriterWriteAttribute(pxtwWriter, BAD_CAST "BitOffset",
                                     BAD_CAST pbBitOffset );
    }
    if (iBytesWritten < 0)
    {
        //printf("testXmlwriterDoc: Error at xmlTextWriterWriteAttribute\n");
        return;
    }
    ///* Close the element named Channel. */
    iBytesWritten = xmlTextWriterEndElement(pxtwWriter);
    if (iBytesWritten < 0)
    {
        //printf("testXmlwriterDoc: Error at xmlTextWriterEndElement\n");
        return;
    }
			}
	 ///* Close the element named ProcessImage. */
   iBytesWritten = xmlTextWriterEndElement(pxtwWriter);
   if (iBytesWritten < 0)
   {
       //printf("testXmlwriterDoc: Error at xmlTextWriterEndElement\n");
       return;
   }

  }
}
/****************************************************************************************************
* Function Name: StartXAPxml
* Description: 
* Return value: void
****************************************************************************************************/
void StartXAPxml(xmlTextWriterPtr& pxtwWriter,  xmlDocPtr& pxdDoc)

{
	 INT32 iBytesWritten;
		//xmlTextWriterPtr pxtwWriter =NULL;


    /* Create a new XmlWriter for DOM, with no compression. */
    pxtwWriter = xmlNewTextWriterDoc(&pxdDoc, 0);
    if (pxtwWriter == NULL) {
        //printf("testXmlwriterDoc: Error creating the xml pxtwWriter\n");
        return;
    }

    /* Start the document with the xml default for the version,
     * encoding UTF-8 and the default for the standalone
     * declaration. */
    iBytesWritten = xmlTextWriterStartDocument(pxtwWriter, NULL, MY_ENCODING, NULL);
    if (iBytesWritten < 0) {
       //printf("testXmlwriterDoc: Error at xmlTextWriterStartDocument\n");
        return;
    }

			/* Write a comment as child of ORDER */
		 iBytesWritten = xmlTextWriterWriteComment(pxtwWriter,BAD_CAST "This file was autogenerated by openCONFIGURATOR");
    if (iBytesWritten < 0)
     {
        //printf("testXmlwriterDoc: Error at xmlTextWriterWriteFormatComment\n");
        return;
    }
    iBytesWritten = xmlTextWriterStartElement(pxtwWriter, BAD_CAST "ApplicationProcess");
    if (iBytesWritten < 0) {
        //printf("testXmlwriterDoc: Error at xmlTextWriterStartElement\n");
        return;
    }
}

/****************************************************************************************************
* Function Name: EndWrtitingXAP
* Description: 
* Return value: void
****************************************************************************************************/
void EndWrtitingXAP( xmlTextWriterPtr& pxtwWriter, char* pbFileName, xmlDocPtr& pxdDoc)
{ 
	INT32 iBytesWritten;

   
	// Close the element named ApplicationProcess.
    iBytesWritten = xmlTextWriterEndElement(pxtwWriter);
    if (iBytesWritten < 0)
    {
       // printf("testXmlwriterDoc: Error at xmlTextWriterEndElement\n");
        return;
    }
    iBytesWritten = xmlTextWriterEndDocument(pxtwWriter);
    if (iBytesWritten < 0) {
        //printf("testXmlwriterDoc: Error at xmlTextWriterEndDocument\n");
        return;
    }

    xmlFreeTextWriter(pxtwWriter);

				xmlSaveFormatFile ((const char*)pbFileName, pxdDoc, 1);

    xmlFreeDoc(pxdDoc);
}

/****************************************************************************************************
* Function Name: GenerateXAP
* Description: 
* Return value: ocfmRetCode
****************************************************************************************************/
ocfmRetCode GenerateXAP(char* pbFileName)
{
		CNodeCollection* pobjNodeCollection;
		ocfmException objException;
		pobjNodeCollection = CNodeCollection::getNodeColObjectPointer();		
		ocfmRetCode stRetInfo;
		char* xapFileName = new char[strlen(pbFileName) + 4 + ALLOC_BUFFER];
		try
		{
			if(pobjNodeCollection->getNumberOfNodes()==0)
			{
				objException.ocfm_Excpetion(OCFM_ERR_NO_NODES_FOUND);
				throw objException;
			}
			if(pobjNodeCollection->getCNNodesCount() ==0)
			{
				objException.ocfm_Excpetion(OCFM_ERR_NO_CN_NODES_FOUND);
				throw objException;
			}
			
					/*Process PDO Nodes*/
				stRetInfo = ProcessPDONodes();
				
				if(stRetInfo.code != OCFM_ERR_SUCCESS)
				return stRetInfo;
			CNode objNode;
			xmlTextWriterPtr pxtwWriter = NULL;
			xmlDocPtr pxdDoc = NULL;
			//INT32 picount = 0;
			//INT32 iLoopCount=10;
			ProcessImage aobjPiInCol[PI_VAR_COUNT] = {};
			ProcessImage aobjPiOutCol[PI_VAR_COUNT] = {};
			GroupInOutPIVariables(aobjPiInCol, aobjPiOutCol);
			/* Calculate Offsets for Input Variable*/
			//CalculateOffsets(InVars, INPUT);
			
				/* Calculate Offsets for Input Variable*/
			//CalculateOffsets(OutVars, OUTPUT);
			
			
			StartXAPxml(pxtwWriter, pxdDoc);	
			if(iInVars !=0)
			WriteXAPElements(aobjPiInCol, pxtwWriter, iInVars, INPUT);
			
			if(iOutVars !=0)			
			WriteXAPElements(aobjPiOutCol, pxtwWriter, iOutVars, OUTPUT);


			//printf("\n pbFileName%s",pbFileName); 
			strcpy(xapFileName, pbFileName);
			strcat(xapFileName, ".xml");
			//printf("\n xapFileName%s",xapFileName); 
			EndWrtitingXAP(pxtwWriter, xapFileName, pxdDoc);
			
			/*Generate Header file */			
			GenerateXAPHeaderFile (pbFileName, aobjPiInCol, aobjPiOutCol, iInVars, iOutVars);
			return objException._ocfmRetCode;
		}
		catch(ocfmException& ex)
		{
			return ex._ocfmRetCode ;
		}
}

/****************************************************************************************************
* Function Name: GenerateNET
* Description: 
* Return value: ocfmRetCode
****************************************************************************************************/
ocfmRetCode GenerateNET(char* pbFileName)
{
		CNodeCollection* pobjNodeCollection;
		ocfmException objException;
		pobjNodeCollection = CNodeCollection::getNodeColObjectPointer();		
		ocfmRetCode stRetInfo;
		//char* xapFileName = new char[strlen(pbFileName) + 4 + ALLOC_BUFFER];
		try
		{
			if(pobjNodeCollection->getNumberOfNodes()==0)
			{
				objException.ocfm_Excpetion(OCFM_ERR_NO_NODES_FOUND);
				throw objException;
			}
			if(pobjNodeCollection->getCNNodesCount() ==0)
			{
				objException.ocfm_Excpetion(OCFM_ERR_NO_CN_NODES_FOUND);
				throw objException;
			}
			
					/*Process PDO Nodes*/
				//stRetInfo = ProcessPDONodes();
				
				//if(stRetInfo.code != OCFM_ERR_SUCCESS)
				//return stRetInfo;
			CNode objNode;
			//xmlTextWriterPtr pxtwWriter = NULL;
			//xmlDocPtr pxdDoc = NULL;
			//INT32 picount = 0;
			//INT32 iLoopCount=10;
			ProcessImage aobjPiInCol[PI_VAR_COUNT] = {};
			ProcessImage aobjPiOutCol[PI_VAR_COUNT] = {};
			GroupInOutPIVariables(aobjPiInCol, aobjPiOutCol);
			/*Generate Dot NET Header file */			
			GenerateNETHeaderFile (pbFileName, aobjPiInCol, aobjPiOutCol, iInVars, iOutVars);
			return objException._ocfmRetCode;
		}
		catch(ocfmException& ex)
		{
			return ex._ocfmRetCode ;
		}
}


/***************************************************************************
*************************
* Function Name: GetIndexAttributes
* Description:
* Return value: ocfmRetCode
****************************************************************************************************/

ocfmRetCode GetIndexAttributes(
	INT32				iNodeID, 
	ENodeType		enumNodeType, 
	char* 			pbIndexID, 
	EAttributeType 	enumAttributeType, 
	char* 			pbOutAttributeValue)
{
	INT32 iIndexPos;
	ocfmRetCode stErrorInfo;
	
	try
	{
		stErrorInfo = IfIndexExists(iNodeID, enumNodeType, pbIndexID, &iIndexPos);
		if(stErrorInfo.code == OCFM_ERR_SUCCESS)
		{
			//cout << "iIndexPos:" << iIndexPos << endl;
		}
		else
		{
			 //OCFM_ERR_INDEXID_NOT_FOUND;
			//return stErrorInfo;
			ocfmException objException;				
			objException.ocfm_Excpetion(OCFM_ERR_INDEXID_NOT_FOUND);
			throw objException;
		}
		CNode objNode;		
		CNodeCollection *pobjNodeCollection;
		CIndexCollection *pobjIndexCollection;
		CIndex objIndex;
		CIndex* pobjIndexPtr;
		
		objIndex.setNodeID(objNode.getNodeId());
		pobjNodeCollection= CNodeCollection::getNodeColObjectPointer();
		objNode = pobjNodeCollection->getNode(enumNodeType, iNodeID);

		pobjIndexCollection = objNode.getIndexCollection();
		pobjIndexPtr = pobjIndexCollection->getIndex(iIndexPos);
		
		switch(enumAttributeType)
		{
			case NAME:									
					if(pobjIndexPtr->getName() != NULL)
						strcpy(pbOutAttributeValue, (char *)pobjIndexPtr->getName());
					else
						strcpy(pbOutAttributeValue, "");
					break;
			case OBJECTTYPE:
					if(pobjIndexPtr->getObjectType() != NULL)
						strcpy(pbOutAttributeValue, (char *)pobjIndexPtr->getObjectType());
					else
						strcpy(pbOutAttributeValue, "");
					break;
			case DATATYPE:
					DataType objTmpDataType;
					objTmpDataType = pobjIndexPtr->getDataType();
					if(objTmpDataType.Name != NULL)
						strcpy(pbOutAttributeValue, objTmpDataType.Name);
					else
						strcpy(pbOutAttributeValue, "");					
					break;		
			case ACCESSTYPE:
					if(pobjIndexPtr->getAccessType() != NULL)
						strcpy(pbOutAttributeValue, (char *)pobjIndexPtr->getAccessType());
					else
						strcpy(pbOutAttributeValue, "");
					break;
			case DEFAULTVALUE:
					if(pobjIndexPtr->getDefaultValue() != NULL)
						strcpy(pbOutAttributeValue, (char *) pobjIndexPtr->getDefaultValue());
					else
						strcpy(pbOutAttributeValue, "");
					break;
			case ACTUALVALUE:
					if(pobjIndexPtr->getActualValue() != NULL)
						strcpy(pbOutAttributeValue, (char *) pobjIndexPtr->getActualValue());
					else
						strcpy(pbOutAttributeValue, "");
					break;
			case PDOMAPPING:
					if(pobjIndexPtr->getPDOMapping() != NULL)
						strcpy(pbOutAttributeValue, (char *) pobjIndexPtr->getPDOMapping());
					else
						strcpy(pbOutAttributeValue, "");
					break;
			case LOWLIMIT:
					if(pobjIndexPtr->getLowLimit() != NULL)
						strcpy(pbOutAttributeValue, (char *) pobjIndexPtr->getLowLimit());
					else
						strcpy(pbOutAttributeValue, "");
					break;
			case HIGHLIMIT:
					if(pobjIndexPtr->getHighLimit() != NULL)
						strcpy(pbOutAttributeValue, (char *) pobjIndexPtr->getHighLimit());
					else
						strcpy(pbOutAttributeValue, "");
					break;
			/* Flag if it should be included in cdc*/
			case FLAGIFINCDC:
					if(pobjIndexPtr->getFlagIfIncludedCdc() == TRUE)
					strcpy(pbOutAttributeValue, "1");
					else
					strcpy(pbOutAttributeValue, "0");			
					break;					
			default:
					//cout << "invalid Attribute Type" << endl;
					//stErrorInfo.code = OCFM_ERR_INVALID_ATTRIBUTETYPE;
					//return stErrorInfo;
					ocfmException objException;				
					objException.ocfm_Excpetion(OCFM_ERR_INVALID_ATTRIBUTETYPE);
					throw objException;
		}
		//cout << "\n\n\n$SOut_AttributeValue:" << pbOutAttributeValue << endl;
		stErrorInfo.code = OCFM_ERR_SUCCESS;
		return stErrorInfo;
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
****************************************************************************************************/
ocfmRetCode GetIndexAttributesbyPositions(
	INT32 			iNodePos, 
	INT32 			iIndexPos, 
	EAttributeType 	enumAttributeType, 
	char* 			pbOutAttributeValue)
{
	ocfmRetCode stErrorInfo;
	try
	{
		// Check for the Existance of the Node in the iNodePos
		CNode objNode;		
		CNodeCollection *pobjNodeCollection;
		CIndexCollection *pobjIndexCollection;
		CIndex* pobjIndexPtr;

		pobjNodeCollection= CNodeCollection::getNodeColObjectPointer();
		
		//pobjIndexCollection = objNode.getIndexCollection();
		
		INT32 iTempNodeCount = pobjNodeCollection->getNumberOfNodes();		
		//cout << "\n\ntmp_NodeCount:" << iTempNodeCount << endl;
		if(iNodePos >= iTempNodeCount)
		{
			ocfmException objException;//new ocfmException;
			objException.ocfm_Excpetion(OCFM_ERR_INVALID_NODEPOS);		
			throw &objException;
		}
		else if(iTempNodeCount == 0)
		{
			ocfmException objException;// = new ocfmException;
			objException.ocfm_Excpetion(OCFM_ERR_NO_NODES_FOUND);		
			throw &objException;
		}
	
		objNode = pobjNodeCollection->getNodebyCollectionIndex(iNodePos);
		pobjIndexCollection = objNode.getIndexCollection();
		
		INT32 iTempIndexCount = pobjIndexCollection->getNumberofIndexes();
		//cout << "\n\iTempIndexCount:" << iTempIndexCount << endl;
		if(iTempIndexCount == 0)
		{
			ocfmException objException;// = new ocfmException;
			objException.ocfm_Excpetion(OCFM_ERR_NO_INDEX_FOUND);
			throw &objException;
		}
		else if(iIndexPos >= iTempIndexCount)
		{
			ocfmException objException;// = new ocfmException;
			objException.ocfm_Excpetion(OCFM_ERR_INVALID_INDEXPOS);
			throw &objException;
		}
		
		pobjIndexPtr = pobjIndexCollection->getIndex(iIndexPos);
		
			switch(enumAttributeType)
			{
				case NAME:									
						if(pobjIndexPtr->getName() != NULL)
							strcpy(pbOutAttributeValue, (char *)pobjIndexPtr->getName());
						else
							strcpy(pbOutAttributeValue, "");
						break;
				case OBJECTTYPE:
						if(pobjIndexPtr->getObjectType() != NULL)
							strcpy(pbOutAttributeValue, (char *)pobjIndexPtr->getObjectType());
						else
							strcpy(pbOutAttributeValue, "");
						break;
				case DATATYPE:
						DataType objTmpDataType;
						objTmpDataType = pobjIndexPtr->getDataType();
						if(objTmpDataType.Name != NULL)
						{
							strcpy(pbOutAttributeValue, objTmpDataType.Name);
						}
						else
						{
							strcpy(pbOutAttributeValue, "");					
						}
						break;		
				case ACCESSTYPE:
						if(pobjIndexPtr->getAccessType() != NULL)
							strcpy(pbOutAttributeValue, (char *)pobjIndexPtr->getAccessType());
						else
							strcpy(pbOutAttributeValue, "");
						break;
				case DEFAULTVALUE:
						if(pobjIndexPtr->getDefaultValue() != NULL)
							strcpy(pbOutAttributeValue, (char *) pobjIndexPtr->getDefaultValue());
						else
							strcpy(pbOutAttributeValue, "");
						break;
				case ACTUALVALUE:
						if(pobjIndexPtr->getActualValue() != NULL)
							strcpy(pbOutAttributeValue, (char *) pobjIndexPtr->getActualValue());
						else
							strcpy(pbOutAttributeValue, "");
						break;
				case PDOMAPPING:
						if(pobjIndexPtr->getPDOMapping() != NULL)
							strcpy(pbOutAttributeValue, (char *) pobjIndexPtr->getPDOMapping());
						else
							strcpy(pbOutAttributeValue, "");
						break;
				case LOWLIMIT:
						if(pobjIndexPtr->getLowLimit() != NULL)
							strcpy(pbOutAttributeValue, (char *) pobjIndexPtr->getLowLimit());
						else
							strcpy(pbOutAttributeValue, "");
						break;
				case HIGHLIMIT:
						if(pobjIndexPtr->getHighLimit() != NULL)
							strcpy(pbOutAttributeValue, (char *) pobjIndexPtr->getHighLimit());
						else
							strcpy(pbOutAttributeValue, "");
						break;			
				/* Flag if it should be included in cdc*/
				case FLAGIFINCDC:
						if(pobjIndexPtr->getFlagIfIncludedCdc() ==  TRUE)
							strcpy(pbOutAttributeValue, "1");
						else
							strcpy(pbOutAttributeValue, "0");	
						break;					
				default:
						//cout << "invalid Attribute Type" << endl;
						stErrorInfo.code = OCFM_ERR_INVALID_ATTRIBUTETYPE;
						return stErrorInfo;
			}
	}
	catch(ocfmException* ex)
	{
		//cout << "\n\nCatch...\n" << endl;
		return ex->_ocfmRetCode;
	}
	//cout << "\n\n\n$SOut_AttributeValue:" << pbOutAttributeValue << endl;
	stErrorInfo.code = OCFM_ERR_SUCCESS;
	return stErrorInfo;
}

/**************************************************************************************************
* Function Name: GetSubIndexAttributes
* Description:
* Return value: ocfmRetCode
****************************************************************************************************/
ocfmRetCode GetSubIndexAttributes(
	INT32 			iNodeID, 
	ENodeType 		enumNodeType, 
	char* 			pbIndexID, 
	char* 			pbSubIndexID, 
	EAttributeType 	enumAttributeType, 
	char* 			pbOutAttributeValue)
{
		CNode objNode;		
		CNodeCollection *pobjNodeCollection;
		CIndexCollection *pobjIndexCollection;
		CIndex objIndex;
		CIndex* pobjSubIndex;
		INT32 iSubIndexPos;
		INT32 iIndexPos;
		ocfmRetCode stErrorInfo;
		
		try
		{
		
			stErrorInfo = IfSubIndexExists(iNodeID, enumNodeType, pbIndexID, pbSubIndexID, &iSubIndexPos, &iIndexPos);
			if(stErrorInfo.code == OCFM_ERR_SUCCESS)
			{
				//iSubIndexPos = Tmp_stErrorInfo.returnValue;
			}
			else
			{
				//OCFM_ERR_INVALID_SUBINDEXID;
				//return stErrorInfo;
				ocfmException objException;				
				objException.ocfm_Excpetion(OCFM_ERR_INVALID_SUBINDEXID);
				throw objException;
			}

			objIndex.setNodeID(objNode.getNodeId());
			pobjNodeCollection= CNodeCollection::getNodeColObjectPointer();
			objNode = pobjNodeCollection->getNode(enumNodeType, iNodeID);

			pobjIndexCollection = objNode.getIndexCollection();
			pobjSubIndex =pobjIndexCollection->getIndex(iIndexPos);
			
			//cout << "NumberofSubIndexes:" << objIndex.getNumberofSubIndexes()<< endl;

			CSubIndex* pobjSubIndexPtr;
			pobjSubIndexPtr = pobjSubIndex->getSubIndex(iSubIndexPos);	
			
			switch(enumAttributeType)
			{
				case NAME:						
						if(pobjSubIndexPtr->getName() != NULL)
							strcpy(pbOutAttributeValue, (char *)pobjSubIndexPtr->getName());
						else
							strcpy(pbOutAttributeValue, "");
						break;
				case OBJECTTYPE:
						if(pobjSubIndexPtr->getObjectType() != NULL)
							strcpy(pbOutAttributeValue, (char *)pobjSubIndexPtr->getObjectType());
						else
							strcpy(pbOutAttributeValue, "");
						break;
				case DATATYPE:
						DataType objTmpDataType;
						objTmpDataType = pobjSubIndexPtr->getDataType();										
						if(objTmpDataType.Name != NULL)
							strcpy(pbOutAttributeValue, objTmpDataType.Name);
						else
							strcpy(pbOutAttributeValue, "");					
						break;		
				case ACCESSTYPE:
						if(pobjSubIndexPtr->getAccessType() != NULL)
							strcpy(pbOutAttributeValue, (char *)pobjSubIndexPtr->getAccessType());
						else
							strcpy(pbOutAttributeValue, "");
						break;
				case DEFAULTVALUE:
						if(pobjSubIndexPtr->getDefaultValue() != NULL)
							strcpy(pbOutAttributeValue, (char *) pobjSubIndexPtr->getDefaultValue());
						else
							strcpy(pbOutAttributeValue, "");
						break;
				case ACTUALVALUE:
						if(pobjSubIndexPtr->getActualValue() != NULL)
							strcpy(pbOutAttributeValue, (char *) pobjSubIndexPtr->getActualValue());
						else
							strcpy(pbOutAttributeValue, "");
						break;
				case PDOMAPPING:
						if(pobjSubIndexPtr->getPDOMapping() != NULL)
							strcpy(pbOutAttributeValue, (char *) pobjSubIndexPtr->getPDOMapping());
						else
							strcpy(pbOutAttributeValue, "");
						break;
				case LOWLIMIT:
						if(pobjSubIndexPtr->getLowLimit() != NULL)
							strcpy(pbOutAttributeValue, (char *) pobjSubIndexPtr->getLowLimit());
						else
							strcpy(pbOutAttributeValue, "");
						break;
				case HIGHLIMIT:
						if(pobjSubIndexPtr->getHighLimit() != NULL)
							strcpy(pbOutAttributeValue, (char *) pobjSubIndexPtr->getHighLimit());
						else
							strcpy(pbOutAttributeValue, "");
						break;
				case FLAGIFINCDC:
						if(pobjSubIndexPtr->getFlagIfIncludedCdc() == TRUE)
							strcpy(pbOutAttributeValue, "1");
						else
							strcpy(pbOutAttributeValue, "0");			
						break;							
				default:
						//cout << "invalid Attribute Type" << endl;
						//stErrorInfo.code = OCFM_ERR_INVALID_ATTRIBUTETYPE;
						//return stErrorInfo;
						ocfmException objException;				
						objException.ocfm_Excpetion(OCFM_ERR_INVALID_SUBINDEXID);
						throw objException;
			}		
			
			stErrorInfo.code = OCFM_ERR_SUCCESS;
			return stErrorInfo;
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
****************************************************************************************************/
ocfmRetCode GetSubIndexAttributesbyPositions(
	INT32 			iNodePos, 
	INT32 			iIndexPos,
	INT32				iSubIndexPos,
	EAttributeType 	enumAttributeType, 
	char* 			pbOutAttributeValue)
{
	ocfmRetCode stErrorInfo;
	try
	{
				// Check for the Existance of the Node in the iNodePos
		CNode objNode;		
		CNodeCollection *pobjNodeCollection;
		CIndexCollection *pobjIndexCollection;
		CIndex* pobjIndexPtr;

		pobjNodeCollection= CNodeCollection::getNodeColObjectPointer();
		
		//pobjIndexCollection = objNode.getIndexCollection();
		
		INT32 iTempNodeCount = pobjNodeCollection->getNumberOfNodes();		
		//cout << "\n\ntmp_NodeCount:" << iTempNodeCount << endl;
		if(iNodePos >= iTempNodeCount)
		{
			ocfmException objException;// = new ocfmException;
			objException.ocfm_Excpetion(OCFM_ERR_INVALID_NODEPOS);		
			throw &objException;
		}
		else if(iTempNodeCount == 0)
		{
			ocfmException objException;// = new ocfmException;
			objException.ocfm_Excpetion(OCFM_ERR_NO_NODES_FOUND);		
			throw &objException;
		}
	
		objNode = pobjNodeCollection->getNodebyCollectionIndex(iNodePos);
		pobjIndexCollection = objNode.getIndexCollection();
		
		INT32 iTempIndexCount = pobjIndexCollection->getNumberofIndexes();
		//cout << "\n\iTempIndexCount:" << iTempIndexCount << endl;
		if(iTempIndexCount == 0)
		{
			ocfmException objException;// = new ocfmException;
			objException.ocfm_Excpetion(OCFM_ERR_NO_INDEX_FOUND);
			throw &objException;
		}
		else if(iIndexPos >= iTempIndexCount)
		{
			ocfmException objException;// = new ocfmException;
			objException.ocfm_Excpetion(OCFM_ERR_INVALID_INDEXPOS);
			throw &objException;
		}
		
		pobjIndexPtr = pobjIndexCollection->getIndex(iIndexPos);
		
		INT32 iTempSubIndexCount = pobjIndexPtr->getNumberofSubIndexes();
		//cout << "\n\ntmp_SubIndexCount:" << iTempSubIndexCount << endl;

		if(iSubIndexPos >= iTempSubIndexCount)
		{
			ocfmException objException;// = new ocfmException;
			objException.ocfm_Excpetion(OCFM_ERR_INVALID_SUBINDEXPOS);		
			throw &objException;
		}
		else if(iTempSubIndexCount == 0)
		{
			ocfmException objException;// = new ocfmException;
			objException.ocfm_Excpetion(OCFM_ERR_NO_SUBINDEXS_FOUND);		
			throw &objException;
		}
		
		CSubIndex* pobjSubIndexPtr;
		pobjSubIndexPtr = pobjIndexPtr->getSubIndex(iSubIndexPos);			
		
		switch(enumAttributeType)
		{
			case NAME:						
					if(pobjSubIndexPtr->getName() != NULL)
						strcpy(pbOutAttributeValue, (char *)pobjSubIndexPtr->getName());
					else
						strcpy(pbOutAttributeValue, "");
					break;
			case OBJECTTYPE:
					if(pobjSubIndexPtr->getObjectType() != NULL)
						strcpy(pbOutAttributeValue, (char *)pobjSubIndexPtr->getObjectType());
					else
						strcpy(pbOutAttributeValue, "");
					break;
			case DATATYPE:
					DataType objTmpDataType;
					objTmpDataType = pobjSubIndexPtr->getDataType();										
					if(objTmpDataType.Name != NULL)
						strcpy(pbOutAttributeValue, objTmpDataType.Name);
					else
						strcpy(pbOutAttributeValue, "");					
					break;		
			case ACCESSTYPE:
					if(pobjSubIndexPtr->getAccessType() != NULL)
						strcpy(pbOutAttributeValue, (char *)pobjSubIndexPtr->getAccessType());
					else
						strcpy(pbOutAttributeValue, "");
					break;
			case DEFAULTVALUE:
					if(pobjSubIndexPtr->getDefaultValue() != NULL)
						strcpy(pbOutAttributeValue, (char *) pobjSubIndexPtr->getDefaultValue());
					else
						strcpy(pbOutAttributeValue, "");
					break;
			case ACTUALVALUE:
					if(pobjSubIndexPtr->getActualValue() != NULL)
						strcpy(pbOutAttributeValue, (char *) pobjSubIndexPtr->getActualValue());
					else
						strcpy(pbOutAttributeValue, "");
					break;
			case PDOMAPPING:
					if(pobjSubIndexPtr->getPDOMapping() != NULL)
						strcpy(pbOutAttributeValue, (char *) pobjSubIndexPtr->getPDOMapping());
					else
						strcpy(pbOutAttributeValue, "");
					break;
			case LOWLIMIT:
					if(pobjSubIndexPtr->getLowLimit() != NULL)
						strcpy(pbOutAttributeValue, (char *) pobjSubIndexPtr->getLowLimit());
					else
						strcpy(pbOutAttributeValue, "");
					break;
			case HIGHLIMIT:
					if(pobjSubIndexPtr->getHighLimit() != NULL)
						strcpy(pbOutAttributeValue, (char *) pobjSubIndexPtr->getHighLimit());
					else
						strcpy(pbOutAttributeValue, "");
					break;
			case FLAGIFINCDC:
					if(pobjSubIndexPtr->getFlagIfIncludedCdc() == TRUE)
						strcpy(pbOutAttributeValue, "1");
					else
						strcpy(pbOutAttributeValue, "0");			
					break;	
					
			default:
					//cout << "invalid Attribute Type" << endl;
					stErrorInfo.code = OCFM_ERR_INVALID_ATTRIBUTETYPE;
					return stErrorInfo;
		}			
	}
	catch(ocfmException* ex)
	{
		//cout << "\n\nCatch...\n" << endl;
		return ex->_ocfmRetCode;
	}
	//cout << "\n\n\n$SOut_AttributeValue:" << pbOutAttributeValue << endl;
	stErrorInfo.code = OCFM_ERR_SUCCESS;
	return stErrorInfo;

}
/**************************************************************************************************
* Function Name: GetNodeCount
* Description:
* Return value: ocfmRetCode
****************************************************************************************************/
ocfmRetCode GetNodeCount(
	INT32 		iMnId, 
	INT32* 		piOutNodeCount)
{
	ocfmRetCode stErrorInfo;
	CNode objNode;
	CNodeCollection *pobjNodeCollection;

	try
	{
		pobjNodeCollection = CNodeCollection::getNodeColObjectPointer();
		if(pobjNodeCollection->getNumberOfNodes() < 0)
		{
			ocfmException objException;				
			objException.ocfm_Excpetion(OCFM_ERR_NO_NODES_FOUND);
			throw objException;
		}
		
		*piOutNodeCount = pobjNodeCollection->getNumberOfNodes();
		
		stErrorInfo.code = OCFM_ERR_SUCCESS;
		return stErrorInfo;
	}
	catch(ocfmException* ex)
	{
		return ex->_ocfmRetCode;
	}
}
/**************************************************************************************************
* Function Name: getPIName
* Description:
* Return value: char*
****************************************************************************************************/
char* getPIName(INT32 iNodeID)
{
	char* pbNodeIdStr;
	char* pbIdAsci = new char[2];

	pbNodeIdStr =(char*)malloc(5);
	strcpy(pbNodeIdStr, "CN");
	pbIdAsci =  _IntToAscii(iNodeID, pbIdAsci, 10);
	strcat(pbNodeIdStr, pbIdAsci);
	strcat(pbNodeIdStr, ".");
	strcat(pbNodeIdStr,"\0");
	return pbNodeIdStr;
}
/**************************************************************************************************
* Function Name: getParameterAccess
* Description:
* Return value: char*
****************************************************************************************************/
char* getParameterAccess(char* pbAccess)
{
	if (!strcmp(ConvertToUpper(pbAccess), "READ"))
	return (char*)"ro\0";
	else if(!strcmp(ConvertToUpper(pbAccess),"READWRITE"))
	return (char*)"rw\0";
	else return (char*)"xx";

}

/**************************************************************************************************
* Function Name: GetIndexCount
* Description:
* Return value: ocfmRetCode
****************************************************************************************************/
ocfmRetCode GetIndexCount(
	INT32 		iNodeID, 
	ENodeType 	enumNodeType, 
	INT32* 		piOutIndexCount)
{
	ocfmRetCode stErrorInfo;
		
	try
	{
		CNode objNode;		
		CNodeCollection *pobjNodeCollection;
		CIndexCollection *pobjIndexCollection;
		CIndex objIndex;
		bool bFlag = false;;
		
		INT32 iNodePos;
		stErrorInfo = IfNodeExists(iNodeID, enumNodeType, &iNodePos, bFlag);
		if (stErrorInfo.code == 0 && bFlag==true) 
		{		
			//retPos = stErrorInfo.returnValue;
		}
		else
		{	
			//cout << "\n\nErrStruct.errCode.code:" << stErrorInfo.code << "\n\n!!!" << endl;
			// Node Doesn't Exist
			//stErrorInfo.code = OCFM_ERR_INVALID_NODEID;
			//return stErrorInfo;
			ocfmException objException;				
			objException.ocfm_Excpetion(OCFM_ERR_INVALID_NODEID);
			throw objException;
		}

		pobjNodeCollection = CNodeCollection::getNodeColObjectPointer();
		objNode = pobjNodeCollection->getNode(enumNodeType, iNodeID);
		pobjIndexCollection = objNode.getIndexCollection();
		
		*piOutIndexCount = pobjIndexCollection->getNumberofIndexes();

		stErrorInfo.code = OCFM_ERR_SUCCESS;
		return stErrorInfo;
	}
	catch(ocfmException* ex)
	{
		return ex->_ocfmRetCode;
	}
}

/****************************************************************************************************
* Function Name: GetSubIndexCount
* Description:
* Return value: ocfmRetCode
****************************************************************************************************/
ocfmRetCode GetSubIndexCount(
	INT32 		iNodeID,
	ENodeType 	enumNodeType,
	char* 		pbIndexID,
	INT32* 		piOutSubIndexCount) 
{
	CNode objNode;		
	CNodeCollection *pobjNodeCollection;
	CIndexCollection *pobjIndexCollection;
	CIndex objIndex;
	CIndex* pobjSubIndex;
	ocfmRetCode stErrorInfo;
	
	INT32 iIndexPos;
	
	try
	{
		stErrorInfo = IfIndexExists(iNodeID, enumNodeType, pbIndexID, &iIndexPos);
		if (stErrorInfo.code == 0)
		{				
			//iIndexPos = stErrorInfo.returnValue;
		}
		else
		{	
			//stErrorInfo.code = OCFM_ERR_INDEXID_NOT_FOUND;
			//return stErrorInfo;
			ocfmException objException;				
			objException.ocfm_Excpetion(OCFM_ERR_INDEXID_NOT_FOUND);
			throw objException;
		}
		
		objIndex.setNodeID(objNode.getNodeId());
		pobjNodeCollection= CNodeCollection::getNodeColObjectPointer();
		objNode = pobjNodeCollection->getNode(enumNodeType, iNodeID);

		pobjIndexCollection = objNode.getIndexCollection();
		pobjSubIndex =pobjIndexCollection->getIndex(iIndexPos);
		
		*piOutSubIndexCount = pobjSubIndex->getNumberofSubIndexes();
		
		stErrorInfo.code = OCFM_ERR_SUCCESS;
		return stErrorInfo;
	}
	catch(ocfmException* ex)
	{
		return ex->_ocfmRetCode;
	}
}

/****************************************************************************************************
* Function Name: LoadObjectDictionary
* Description: 
* Return value: void
****************************************************************************************************/
void LoadObjectDictionary(char* pbFileName)
	{
	
		//printf("Inside ObjectDictionary");
  xmlTextReaderPtr pxReader;
  INT32 iRetVal;
		CObjectDictionary* objDict;
		//printf("Inside ObjectDictionary3");
		objDict = CObjectDictionary::getObjDictPtr();
		//if(objDict==NULL)
		////printf("NULL");
		pxReader = xmlReaderForFile(pbFileName, NULL, 0);
  if (pxReader != NULL)
		{
			 iRetVal = xmlTextReaderRead(pxReader);
    while (iRetVal == 1)
				{		
							const xmlChar* pxcName	=	NULL;
	const xmlChar* pxcValue	=	NULL;	
						pxcName = xmlTextReaderConstName(pxReader);
						pxcValue = xmlTextReaderConstValue(pxReader);
						objDict->ProcessObjectDictionary(pxReader);						
						iRetVal = xmlTextReaderRead(pxReader);					
				}
				if(iRetVal!=0)
				{
					//printf("Error parsing file");
				}
			}
				//printf("\n Processing completed");
			//cout << "file parsed" <<endl;		
		//printf("Parsing ObjectDictionary5 Done");
		//objDict->printall();
		 xmlCleanupParser();
		/*
		* this is to debug memory for regression tests
		*/
		xmlMemoryDump();

	}
/****************************************************************************************************
* Function Name: GetNodeIDbyNodePos
* Description: Fills the NodeID and NodeType for the NodeCount
* Return value: ocfmRetCode
****************************************************************************************************/

ocfmRetCode GetNodeAttributesbyNodePos(
	INT32 iNodePos,
	INT32* piOutNodeID,
	char* piOutNodeName,
	EStationType* eOutStationType,
	char* pbOutForcedCycle,
	bool* bForcedCycleFlag)
{
	//cout<< "Inside GetNodeIDbyNodeCount" << endl;
	ocfmRetCode stErrorInfo;
	
	INT32 iTempNodeCount;
	try
	{
		// We do not have support for multiple MNs in this version.
		GetNodeCount( MN_NODEID, &iTempNodeCount);
		
		if(iNodePos > iTempNodeCount)
		{
			ocfmException objException;// = new ocfmException;
			objException.ocfm_Excpetion(OCFM_ERR_INVALID_NODEPOS);		
			throw &objException;
		}
		else if(iTempNodeCount == 0)
		{
			ocfmException objException;// = new ocfmException;
			objException.ocfm_Excpetion(OCFM_ERR_NO_NODES_FOUND);		
			throw &objException;
		}
		CNode objNode;		
		CNodeCollection *pobjNodeCollection;
		CIndexCollection *pobjIndexCollection;

		pobjNodeCollection= CNodeCollection::getNodeColObjectPointer();
		
		pobjIndexCollection = objNode.getIndexCollection();

		objNode = pobjNodeCollection->getNodebyCollectionIndex(iNodePos);
		
		//if(objNode.getNodeType() == NULL)
		//{
		//	cout << "OCFM_ERR_INVALID_NODETYPE" << endl;
		//	ocfmException* objException = new ocfmException;
		//	objException->ocfm_Excpetion(OCFM_ERR_INVALID_NODETYPE);
		//	throw objException;
		//}
		*piOutNodeID = objNode.getNodeId();
		if(objNode.getNodeName() != NULL)
			strcpy(piOutNodeName, objNode.getNodeName());
		else
			piOutNodeName = NULL;

		if(objNode.getForcedCycle() != NULL)
			strcpy(pbOutForcedCycle, objNode.getForcedCycle());
		else
			pbOutForcedCycle = NULL;

		*eOutStationType = objNode.getStationType();
		*bForcedCycleFlag = objNode.getForceCycleFlag();
				
		stErrorInfo.code = OCFM_ERR_SUCCESS;
		return stErrorInfo;
	}
	catch(ocfmException* ex)
	{
		return ex->_ocfmRetCode;
	}
}

/****************************************************************************************************
* Function Name: GetIndexIDbyIndexPos
* Description: Fills the IndexID for the IndexCount, given iNodeID and the enumNodeType
* Return value: ocfmRetCode
****************************************************************************************************/

ocfmRetCode GetIndexIDbyIndexPos(
	INT32 iNodeID, 
	ENodeType enumNodeType, 
	INT32 iIndexPos, 
	char* pbOutIndexID)
{	
	ocfmRetCode stErrorInfo;
		
	CNode objNode;		
	CNodeCollection *pobjNodeCollection;
	CIndexCollection *pobjIndexCollection;
	CIndex* pobjIndex;
	
	INT32 iNodePos;
	try
	{
		bool bFlag = false;
		stErrorInfo = IfNodeExists(iNodeID, enumNodeType, &iNodePos, bFlag);
		if (stErrorInfo.code == 0 && bFlag==true) 
		{		
			//retPos = stErrorInfo.returnValue;
		}
		else
		{	
			//cout << "$S\n\nErrStruct.errCode.code:" << stErrorInfo.code << "\n\n!!!" << endl;
			// Node Doesn't Exist
			ocfmException objException;// = new ocfmException;
			objException.ocfm_Excpetion(OCFM_ERR_INVALID_NODEID);
			throw &objException;
		}

		pobjNodeCollection = CNodeCollection::getNodeColObjectPointer();
		//objNode = pobjNodeCollection->getNode(enumNodeType, iNodeID);
		objNode = pobjNodeCollection->getNodebyCollectionIndex(iNodePos);
		pobjIndexCollection = objNode.getIndexCollection();
		
		INT32 iTempIndexCount = pobjIndexCollection->getNumberofIndexes();
		
		if(iTempIndexCount == 0)
		{
			ocfmException objException;// = new ocfmException;
			objException.ocfm_Excpetion(OCFM_ERR_NO_INDEX_FOUND);
			throw &objException;
		}
		else if(iTempIndexCount < iIndexPos)
		{
			ocfmException objException;// = new ocfmException;
			objException.ocfm_Excpetion(OCFM_ERR_INVALID_INDEXPOS);
			throw &objException;
		}
		
		pobjIndex = pobjIndexCollection->getIndex(iIndexPos);
		
		if(pobjIndex->getIndexValue() != NULL)
			strcpy(pbOutIndexID, (char *) pobjIndex->getIndexValue());
		
		//cout << "$SOut_IndexID:" << Out_IndexID << endl;

		stErrorInfo.code = OCFM_ERR_SUCCESS;
		return stErrorInfo;
	}
	catch(ocfmException* ex)
	{
		return ex->_ocfmRetCode;
	}
}

/****************************************************************************************************
* Function Name: GetIndexIDbyPositions
* Description: Fills the IndexID for the IndexCount,
* Return value: ocfmRetCode
****************************************************************************************************/

ocfmRetCode GetIndexIDbyPositions(
	INT32 iNodePos, 
	INT32 iIndexPos, 
	char* pbOutIndexID)
{
	ocfmRetCode stErrorInfo;
	CNode objNode;		
	CNodeCollection *pobjNodeCollection;
	CIndexCollection *pobjIndexCollection;
	CIndex* pobjIndex;
		
	try
	{
		pobjNodeCollection= CNodeCollection::getNodeColObjectPointer();

		INT32 iTempNodeCount = pobjNodeCollection->getNumberOfNodes();
		
		if(iTempNodeCount == 0)
		{
			ocfmException objException;// = new ocfmException;			
			objException.ocfm_Excpetion(OCFM_ERR_NO_NODES_FOUND);
			throw &objException;
		}
		else if(iTempNodeCount < iNodePos)
		{
			ocfmException objException;// = new ocfmException;
			objException.ocfm_Excpetion(OCFM_ERR_INVALID_NODEPOS);
			throw &objException;
		}
		
		objNode = pobjNodeCollection->getNodebyCollectionIndex(iNodePos);
		pobjIndexCollection = objNode.getIndexCollection();
		
		INT32 iTempIndexCount = pobjIndexCollection->getNumberofIndexes();
		
		if(iTempIndexCount == 0)
		{
			ocfmException objException;// = new ocfmException;
			objException.ocfm_Excpetion(OCFM_ERR_NO_INDEX_FOUND);
			throw &objException;
		}
		else if(iTempIndexCount < iIndexPos)
		{
			ocfmException objException;// = new ocfmException;
			objException.ocfm_Excpetion(OCFM_ERR_INVALID_INDEXPOS);
			throw &objException;
		}
		
		pobjIndex = pobjIndexCollection->getIndex(iIndexPos);
		
		if(pobjIndex->getIndexValue() != NULL)
			strcpy(pbOutIndexID, (char *) pobjIndex->getIndexValue());
		
		//cout << "Out_IndexID:" << Out_IndexID << endl;

		stErrorInfo.code = OCFM_ERR_SUCCESS;
		return stErrorInfo;
	}
	catch(ocfmException* ex)
	{
		return ex->_ocfmRetCode;
	}
	
}

/****************************************************************************************************
* Function Name: GetSubIndexIDbySubIndexPos
* Description: Fills the SubpIndexID for the iSubIndexPos, given iNodeID, enumNodeType and the IndexID
* Return value: ocfmRetCode
****************************************************************************************************/

ocfmRetCode GetSubIndexIDbySubIndexPos(
	INT32 iNodeID, 
	ENodeType enumNodeType, 
	char* pbIndexID, 
	INT32 iSubIndexPos, 
	char* pbOutSubIndexID)
{
	CNode objNode;		
	CNodeCollection *pobjNodeCollection;
	CIndexCollection *pobjIndexCollection;
	CIndex objIndex;
	CIndex* pobjSubIndex;
	ocfmRetCode stErrorInfo;
	
	INT32 iIndexPos;
	try
	{
		
		stErrorInfo = IfIndexExists(iNodeID, enumNodeType, pbIndexID, &iIndexPos);
		if (stErrorInfo.code == 0)
		{				
			//iIndexPos = stErrorInfo.returnValue;
		}
		else
		{	
			ocfmException objException;// = new ocfmException;
			objException.ocfm_Excpetion(OCFM_ERR_INDEXID_NOT_FOUND);
			throw &objException;
		}
		
		//objIndex.setNodeID(objNode.getNodeId());
		pobjNodeCollection= CNodeCollection::getNodeColObjectPointer();
		objNode = pobjNodeCollection->getNode(enumNodeType, iNodeID);

		pobjIndexCollection = objNode.getIndexCollection();
		pobjSubIndex = pobjIndexCollection->getIndex(iIndexPos);
			
		INT32 iTempSubIndexCount = pobjSubIndex->getNumberofSubIndexes();
		if(iTempSubIndexCount == 0)
		{		
			ocfmException objException;// = new ocfmException;			
			objException.ocfm_Excpetion(OCFM_ERR_NO_SUBINDEXS_FOUND);
			throw &objException;			
		}
		else if(iTempSubIndexCount < iSubIndexPos)
		{
			ocfmException objException;// = new ocfmException;
			objException.ocfm_Excpetion(OCFM_ERR_INVALID_SUBINDEXPOS);
			throw &objException;	
		}
	
		CSubIndex* pobjSubIndexPtr;
		pobjSubIndexPtr = pobjSubIndex->getSubIndex(iSubIndexPos);
		
		if(pobjSubIndexPtr->getIndexValue() != NULL)
			strcpy(pbOutSubIndexID, (char *) pobjSubIndexPtr->getIndexValue());	
		
		stErrorInfo.code = OCFM_ERR_SUCCESS;
		return stErrorInfo;
	}
	catch(ocfmException* ex)
	{
		return ex->_ocfmRetCode;
	}
}

/****************************************************************************************************
* Function Name: GetSubIndexIDbyPositions
* Description: Fills the SubIndexID for the iSubIndexPos, given iNodePos and the iIndexPos
* Return value: ocfmRetCode
****************************************************************************************************/

ocfmRetCode GetSubIndexIDbyPositions(
	INT32 iNodePos, 
	INT32 iIndexPos, 
	INT32 iSubIndexPos, 
	char* pbOutSubIndexID)

{
	CNode objNode;		
	CNodeCollection *pobjNodeCollection;
	CIndexCollection *pobjIndexCollection;
	CIndex objIndex;
	CIndex* pobjSubIndex;
	ocfmRetCode stErrorInfo;

	try
	{		
		pobjNodeCollection= CNodeCollection::getNodeColObjectPointer();

		INT32 iTempNodeCount = pobjNodeCollection->getNumberOfNodes();
		
		if(iTempNodeCount == 0)
		{
			ocfmException objException;// = new ocfmException;			
			objException.ocfm_Excpetion(OCFM_ERR_NO_NODES_FOUND);
			throw &objException;
		}
		else if(iTempNodeCount < iNodePos)
		{
			ocfmException objException;// = new ocfmException;	
			objException.ocfm_Excpetion(OCFM_ERR_INVALID_NODEPOS);
			throw &objException;
		}
		
		objNode = pobjNodeCollection->getNodebyCollectionIndex(iNodePos);		
		pobjIndexCollection = objNode.getIndexCollection();
		
		INT32 iTempIndexCount = pobjIndexCollection->getNumberofIndexes();		
		if(iTempIndexCount == 0)
		{
			ocfmException objException; // = new ocfmException;
			objException.ocfm_Excpetion(OCFM_ERR_NO_INDEX_FOUND);
			throw &objException;
		}
		else if(iTempIndexCount < iIndexPos)
		{
			ocfmException objException;// = new ocfmException;
			objException.ocfm_Excpetion(OCFM_ERR_INVALID_INDEXPOS);
			throw &objException;
		}
		
		pobjSubIndex = pobjIndexCollection->getIndex(iIndexPos);
			
		INT32 iTempSubIndexCount = pobjSubIndex->getNumberofSubIndexes();
		if(iTempSubIndexCount == 0)
		{		
			ocfmException objException;// = new ocfmException;			
			objException.ocfm_Excpetion(OCFM_ERR_NO_SUBINDEXS_FOUND);
			throw &objException;			
		}
		else if(iTempSubIndexCount < iSubIndexPos)
		{
			ocfmException objException; //= new ocfmException;
			objException.ocfm_Excpetion(OCFM_ERR_INVALID_SUBINDEXPOS);
			throw &objException;	
		}
	
		CSubIndex* pobjSubIndexPtr;
		pobjSubIndexPtr = pobjSubIndex->getSubIndex(iSubIndexPos);
		
		if(pobjSubIndexPtr->getIndexValue() != NULL)
			strcpy(pbOutSubIndexID, (char *) pobjSubIndexPtr->getIndexValue());	
		
		stErrorInfo.code = OCFM_ERR_SUCCESS;
		return stErrorInfo;
	}
	catch(ocfmException* ex)
	{
		return ex->_ocfmRetCode;
	}
}

/****************************************************************************************************
* Function Name: DeleteNodeObjDict
* Description: Deletes the Node Object Dictinary.
* Return value: ocfmRetCode
****************************************************************************************************/

ocfmRetCode DeleteNodeObjDict(
	INT32 iNodeID, ENodeType enumNodeType)
{
	ocfmRetCode stErrorInfo;
	INT32 iNodePos;
	//cout << "\n$SInside DeleteMNObjDict..\n" << endl;
	try
	{	
		bool bFlag = false;
		stErrorInfo = IfNodeExists(iNodeID, enumNodeType, &iNodePos, bFlag);
		
		if(stErrorInfo.code == 0 && bFlag ==true)
		{
		}
		else
		{
			ocfmException objException;// = new ocfmException;
			objException.ocfm_Excpetion(OCFM_ERR_INVALID_NODEID);
			throw &objException;
		}
		
		CNode objNode;
		CNodeCollection *pobjNodeCollection;
		CIndexCollection *pobjIndexCollection;
		CDataTypeCollection *objDataTypeCollection;
		CIndex objIndex;

		objIndex.setNodeID(objNode.getNodeId());
		pobjNodeCollection= CNodeCollection::getNodeColObjectPointer();
		objNode = pobjNodeCollection->getNode(enumNodeType, iNodeID);
		
		objDataTypeCollection = objNode.getDataTypeCollection();

		pobjIndexCollection = objNode.getIndexCollection();

		// Delete IndexCollection
		pobjIndexCollection->DeleteIndexCollection();
		// Delete DataTypeCollection
		objDataTypeCollection->DeleteDataTypeCollection();
		//cout << "\n\n$SDelete MN OBD Success" << endl;
		stErrorInfo.code = OCFM_ERR_SUCCESS;
		return stErrorInfo;
	}
	catch(ocfmException* ex)
	{
		return ex->_ocfmRetCode;
	}
}

/****************************************************************************************************
* Function Name: SaveProject
* Description: Saves all the Nodes into the Project location
* Return value: ocfmRetCode
****************************************************************************************************/

ocfmRetCode SaveProject(char* pbProjectPath, char* pbProjectName)
{
	CNode objNode;		
	CNodeCollection *pobjNodeCollection = NULL;	
	ocfmRetCode stErrorInfo;
	char* pbTempPath;
	pbTempPath = new char[(strlen(pbProjectPath)) + (strlen(pbProjectName)) + strlen("cdc_xap") + ALLOC_BUFFER];
	
	try
	{	
		char* pbTempPjtName;
		struct stat stFileInfo;		
		INT32 iIntStat;

		//pbTempPjtName = new char[strlen(pbTempPath) + strlen(pbProjectName) + strlen(pbProjectName) + 3 + 2];
		pbTempPjtName = new char[300];
		//sprintf(pbTempPjtName, "%s/%s/%s.oct", pbProjectPath, pbProjectName, pbProjectName);
		sprintf(pbTempPjtName, "%s%s/%s.oct", pbProjectPath, pbProjectName, pbProjectName);
		//sprintf(pbTempPjtName, "%s/%s.oct", pbProjectPath, pbProjectName);
		//cout << "\n\ntmp_PjtName:" << pbTempPjtName << endl;

		iIntStat = stat(pbTempPjtName,&stFileInfo);
		if(iIntStat == 0) {
			#if defined DEBUG
				cout << "\n\n\nProject File Already exists\n" << endl;
			#endif
		} 
		else 
		{ 				
  			//cout << "\n\ntmp_PjtName:" << pbTempPjtName << endl;
			#if defined(_WIN32) && defined(_MSC_VER)
			{
				sprintf(pbTempPath, "%s\\%s", pbProjectPath, pbProjectName);
				//cout << "\npath:" << pbTempPath <<endl;
				mkdir(pbTempPath);	
				//cout << "mkdir success"<<endl;
			}
			#else
			{
				sprintf(pbTempPath, "%s/%s", pbProjectPath, pbProjectName);				
				mkdir(pbTempPath, S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH);
			}
			#endif
		}	
		
		#if defined(_WIN32) && defined(_MSC_VER)
		{
			sprintf(pbTempPath, "%s\\%s\\%s", pbProjectPath, pbProjectName, "cdc_xap");
			//cout << "\npath:" << path <<endl;
			mkdir(pbTempPath);	
			//cout << "mkdir success"<<endl;
		}
		#else
		{
			sprintf(pbTempPath, "%s/%s/%s", pbProjectPath, pbProjectName, "cdc_xap");				
			mkdir(pbTempPath, S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH);
		}
		#endif
		
		saveProjectXML(pbProjectPath, pbProjectName);
		pobjNodeCollection = CNodeCollection::getNodeColObjectPointer();	
		if(pobjNodeCollection == NULL)
		{
			ocfmException objException;
			objException.ocfm_Excpetion(OCFM_ERR_NO_NODES_FOUND);
			throw &objException;
		}
		if( pobjNodeCollection->getNumberOfNodes() > 0)
		{
			for(INT32 iLoopCount = 0; iLoopCount < pobjNodeCollection->getNumberOfNodes(); iLoopCount++)
			{				
				objNode = pobjNodeCollection->getNodebyCollectionIndex(iLoopCount);
				//char *pbFileName;	
				//pbFileName = new char[80];
				char* pbFileName;
				//pbFileName = new char[(strlen(pbTempPath) + 4 + 5)];
				pbFileName  =  new char[MAX_FILE_PATH_SIZE];
				#if defined(_WIN32) && defined(_MSC_VER)
				{
					sprintf(pbTempPath, "%s\\%s\\octx", pbProjectPath, pbProjectName);				
					struct stat stFileInfo;
					INT32 iIntStat;
					iIntStat = stat(pbTempPath,&stFileInfo);
					if(iIntStat == 0) 
					{

						#if defined DEBUG
							cout << "Folder Already Exists\n" << endl;
						#endif
					}
					else
					{
						mkdir(pbTempPath);
					}
					// Saves the nodes with their nodeId as the name
					sprintf(pbFileName, "%s\\%d.octx", pbTempPath, objNode.getNodeId());
					//cout << "\nSave Pjt FileName:" << pbFileName << endl;
				}
				#else
				{
					struct stat stFileInfo;
					INT32 iIntStat;
					sprintf(pbTempPath, "%s/%s/octx", pbProjectPath, pbProjectName);				
					iIntStat = stat(pbTempPath,&stFileInfo);
					if(iIntStat == 0) 
					{
						#if defined DEBUG
							cout << "Folder Already Exists\n" << endl;
						#endif
					}
					else
					{					
						mkdir(pbTempPath, S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH);
					}
					// Saves the nodes with their nodeId as the name
					sprintf(pbFileName, "%s/%d.octx", pbTempPath, objNode.getNodeId());
				}
				#endif				
				//cout << "\nfileName:" << pbFileName << endl;
				//cout << "\ngetNodeId-getNodeType:" << objNode.getNodeId() << objNode.getNodeType() << endl;
				SaveNode(pbFileName, objNode.getNodeId(), objNode.getNodeType());	
				//cout << "After savenode"<<endl;
				//cout << "Trace_8" <<endl;
				delete [] pbFileName;			

			}
			stErrorInfo.code = OCFM_ERR_SUCCESS;
			delete [] pbTempPath;
			return stErrorInfo;
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
		delete [] pbTempPath;
		return ex->_ocfmRetCode;
	}
	
}

/****************************************************************************************************
* Function Name: GetMNPDOSubIndex
* Description: 
* Return value: void
****************************************************************************************************/
void GetMNPDOSubIndex(MNPdoVariable stMNPdoVar, INT32& iPrevSubIndex, CIndex* pobjIdx, char* pbMNIndex, INT32 iPrevSize)
{
			CSubIndex* pobjSubIndex;
			char* pbIdx =  new char[2];
		/* Set the MN's PDO subIndex*/
			iPrevSubIndex =  iPrevSubIndex + 1;
			pbIdx = _IntToAscii(iPrevSubIndex, pbIdx, 16);
			pbIdx = padLeft(pbIdx, '0', 2);
			
			#if defined DEBUG	
				cout<< " idx" << pbIdx << endl;
				cout<< " prevsubindex" << iPrevSubIndex << endl;
			#endif			
			if(CheckIfSubIndexExists(MN_NODEID, MN, pbMNIndex,pbIdx))
			{
				pobjSubIndex = pobjIdx->getSubIndexbyIndexValue(pbIdx);
			}			
			else
			{
				AddSubIndex(MN_NODEID, MN,  pbMNIndex,pbIdx);
				pobjSubIndex = pobjIdx->getSubIndexbyIndexValue(pbIdx);
			}
			
			/* Calculate the actual value of MN PDO */
			char* pbActValue = new char[20];
			strcpy(pbActValue, subString(stMNPdoVar.Value,0,6));
			char* pbOffset = new char[5];

			pbOffset = _IntToAscii(iPrevSize, pbOffset, 16);
			pbOffset = padLeft(pbOffset, '0', 4);
			
			strcat(pbActValue, pbOffset);
			/* Add reserve byte*/
			strcat(pbActValue, "00");
			
			strcat(pbActValue, stMNPdoVar.SubIndex);
			strcat(pbActValue, stMNPdoVar.Index);
			
			pobjSubIndex->setActualValue(pbActValue);
			delete[] pbActValue;
			
			AddPDOIndexsToMN(stMNPdoVar.Index,stMNPdoVar.SubIndex, stMNPdoVar.pdoType);
		/*	return *pobjSubIndex;*/
}

/****************************************************************************************************
* Function Name: SetSIdxValue
* Description: 
* Return value: void
****************************************************************************************************/
void SetSIdxValue(char* pbIdx, char* pbSIdx,
																	 char* pbvalue, CIndexCollection * pobjIdxCol,
																		INT32 iNodeId, ENodeType enumNodeType, bool setDefaultValue)
{
		CIndex *pobjIndex;
		CSubIndex* pobjSIdx;
		ocfmRetCode stRetInfo;

		pobjIndex = pobjIdxCol->getIndexbyIndexValue(pbIdx);
					#if defined DEBUG	
						cout << "sidx idx" << pbIdx << pbSIdx << endl;;
						
					#endif
		if(CheckIfSubIndexExists(iNodeId, enumNodeType, pbIdx, pbSIdx))
		{			
			#if defined DEBUG	
						cout << "subindex fetched" << endl;;
						
					#endif			
			pobjSIdx = pobjIndex->getSubIndexbyIndexValue(pbSIdx);
			if(setDefaultValue)
			pobjSIdx->setActualValue((char*)pobjSIdx->getDefaultValue());
			else
			pobjSIdx->setActualValue(pbvalue);
			pobjSIdx->setFlagIfIncludedCdc(TRUE);
		}
		else
		{
			#if defined DEBUG	
						cout << "call addindex" << endl;
						
					#endif
			stRetInfo = AddSubIndex(iNodeId, enumNodeType, pbIdx, pbSIdx);
				#if defined DEBUG	
						cout << "retcode" << stRetInfo.code<<endl;
						cout<< "Index sidx added"<< pbIdx << pbSIdx;
					#endif
			pobjSIdx = pobjIndex->getSubIndexbyIndexValue(pbSIdx);
			pobjSIdx->setFlagIfIncludedCdc(TRUE);
			if(setDefaultValue)
			pobjSIdx->setActualValue((char*)pobjSIdx->getDefaultValue());
			else
			{
				#if defined DEBUG	
						cout << "value" << pbvalue<<endl;	
						cout << "subindex index"<< pobjSIdx->getIndexValue();
					#endif
				pobjSIdx->setActualValue(pbvalue);
			}
		}
}

/****************************************************************************************************
* Function Name: AddForEachSIdx
* Description: 
* Return value: void
****************************************************************************************************/
void AddForEachSIdx (char* pbIdx,CIndexCollection * pobjIdxCol, INT32 iMNNodeID, char* pbValue, bool bIsDefaultValueSet)
{
		CNodeCollection* objNodeCol;
		CNode objCNNode;
		CIndex *pobjIndex;
		char* pbSIdx =  new char[3];
		char* pbIndexNo = new char[3];
		ocfmRetCode stRetInfo;
		
		stRetInfo.code = OCFM_ERR_SUCCESS;
		pobjIndex = pobjIdxCol->getIndexbyIndexValue(pbIdx);
		objNodeCol = CNodeCollection::getNodeColObjectPointer();
		
		// to doStack wasnt booting up with sub index 00 for 1C09, 1F26
		if((strcmp("1C09",pbIdx)==0) || (strcmp("1F26", pbIdx) == 0)
																															|| (strcmp("1F8B",pbIdx)  == 0)
																															|| (strcmp("1F8D",pbIdx)		==	0)
																															|| (strcmp("1F27",pbIdx)		==	0)
																															|| (strcmp("1F84",pbIdx)		==	0))
	{
		strcpy(pbSIdx, "00");
		DeleteSubIndex(iMNNodeID, MN, pbIdx, pbSIdx);
	}
	else
	{
			if(objNodeCol->getCNNodesCount() !=0)
			{
				strcpy(pbSIdx, "00");
			
				pbIndexNo = _IntToAscii(objNodeCol->getCNNodesCount(), pbIndexNo, 16);
				pbIndexNo = padLeft(pbIndexNo, '0', 2);
				
				SetSIdxValue(pbIdx, pbSIdx, pbIndexNo , pobjIdxCol, iMNNodeID, MN, false);
			}
	}
				
		for(INT32 iLoopCount = 0; iLoopCount < objNodeCol->getNumberOfNodes(); iLoopCount++)
		{
			objCNNode = objNodeCol->getNodebyCollectionIndex(iLoopCount);	
		
			if(objCNNode.getNodeType() == CN)
			{
				pbSIdx = _IntToAscii(objCNNode.getNodeId(), pbSIdx, 16);
				pbSIdx = padLeft(pbSIdx, '0', 2);	
				
				
				if(strcmp("1F84",pbIdx)==0)
				{
					CIndex* objCNIndex;
					CSubIndex objCNSIdx;
					CIndexCollection* pobjCNIdxCol;
					char* pbMIndex = new char[INDEX_LEN];
					pobjCNIdxCol = objCNNode.getIndexCollection();
					
					strcpy(pbMIndex, "1000");
					objCNIndex = pobjCNIdxCol->getIndexbyIndexValue(pbMIndex);
					
					if(objCNIndex != NULL)
					{
						if(objCNIndex->getActualValue() != NULL)
						{
							SetSIdxValue(pbIdx, pbSIdx, (char*)objCNIndex->getActualValue(), pobjIdxCol, iMNNodeID, MN, false);
						}
						else
						{
							SetSIdxValue(pbIdx, pbSIdx, (char*)objCNIndex->getActualValue(), pobjIdxCol, iMNNodeID, MN, true);
						}
					}
										
				}
				else
				{
					//cout << "\nVal:" << pbValue <<endl;
					SetSIdxValue(pbIdx,pbSIdx,pbValue, pobjIdxCol, iMNNodeID, MN, bIsDefaultValueSet);
				}
		
				
				}
		
	}
}

/****************************************************************************************************
* Function Name: AddOtherMNIndexes
* Description: 
* Return value: ocfmRetCode
****************************************************************************************************/
ocfmRetCode AddOtherMNIndexes(int NodeID)
{
			ocfmRetCode stRetInfo;
			char* pbMNIndex = new char[INDEX_LEN];		
			char* pbSidx =  new char[SUBINDEX_LEN];
			CIndex* pobjIndex;
			CIndexCollection* pobjIdxCol;
			
			CNode *pobjNode;
			CNodeCollection *objNodeCollection = NULL;
		
			try
			{
				objNodeCollection = CNodeCollection::getNodeColObjectPointer();
				pobjNode = objNodeCollection->getNodePtr(MN, NodeID);
				pobjIdxCol = pobjNode->getIndexCollection();
					
				/* Add 1006*/
					strcpy(pbMNIndex, "1006");
							#if defined DEBUG	
						cout << "string copied" << endl;
					
					#endif
					stRetInfo = AddIndex(MN_NODEID, MN, pbMNIndex);
						#if defined DEBUG	
						cout << "retcode" << stRetInfo.code<<endl;
						cout<< "1006 added"<<endl;
					#endif
					
					/* Set 5ms value*/	
					SetIndexAttributes(MN_NODEID, MN, pbMNIndex, (char*)"50000",(char*)"NMT_CycleLen_U32", TRUE);				
					
						/* Add 1020*/
					strcpy(pbMNIndex, "1020");
					stRetInfo = AddIndex(MN_NODEID, MN, pbMNIndex);
					
							/* Add 1020*/
					strcpy(pbMNIndex, "1300");
					stRetInfo = AddIndex(MN_NODEID, MN, pbMNIndex);
					/* $:To do by M hard coded*/
					SetIndexAttributes(MN_NODEID, MN, pbMNIndex, (char*)"5000",(char*)"SDO_SequLayerTimeout_U32", TRUE);
					
					
					/* Add 1C02*/
					strcpy(pbMNIndex, "1C02");
					stRetInfo = AddIndex(MN_NODEID, MN, pbMNIndex);
				
					if(stRetInfo.code == OCFM_ERR_SUCCESS || stRetInfo.code == OCFM_ERR_INDEX_ALREADY_EXISTS)
					{				
						pobjIndex = pobjIdxCol->getIndexbyIndexValue(pbMNIndex);
							/* $:set Flag to true*/
						pobjIndex->setFlagIfIncludedCdc(TRUE);
						/* Set subindex value 40 or 0000028 */
						strcpy(pbSidx, "00");
						SetSIdxValue(pbMNIndex, pbSidx, (char*)"3", pobjIdxCol, pobjNode->getNodeId(), MN, false);
								#if defined DEBUG	
						cout<< "1c02 subidex added"<<endl;
					#endif
						
						strcpy(pbSidx, "01");
						SetSIdxValue(pbMNIndex, pbSidx, (char*)"40", pobjIdxCol, pobjNode->getNodeId(), MN,  false);
								#if defined DEBUG	
						cout<< "1c02 subidex 01 added"<<endl;
					#endif
						strcpy(pbSidx, "02");
						SetSIdxValue(pbMNIndex, pbSidx, (char*)"40", pobjIdxCol, pobjNode->getNodeId(), MN, false);
						
						strcpy(pbSidx, "03");
						SetSIdxValue(pbMNIndex, pbSidx, (char*)"40", pobjIdxCol, pobjNode->getNodeId(), MN, false);

					}
					
					/* Add 1C09*/
					strcpy(pbMNIndex, "1C09");
					stRetInfo = AddIndex(MN_NODEID, MN, pbMNIndex);
					if(stRetInfo.code == OCFM_ERR_SUCCESS || stRetInfo.code == OCFM_ERR_INDEX_ALREADY_EXISTS)
					{				
						pobjIndex = pobjIdxCol->getIndexbyIndexValue(pbMNIndex);
								/* $:set Flag to true*/
						pobjIndex->setFlagIfIncludedCdc(TRUE);
			
						AddForEachSIdx(pbMNIndex, pobjIdxCol, pobjNode->getNodeId(), (char*)"40",false);					
					
					}
					
							/* Add 1F26*/
					strcpy(pbMNIndex, "1F26");
					stRetInfo = AddIndex(MN_NODEID, MN, pbMNIndex);
					if(stRetInfo.code == OCFM_ERR_SUCCESS || stRetInfo.code == OCFM_ERR_INDEX_ALREADY_EXISTS)
					{				
						pobjIndex = pobjIdxCol->getIndexbyIndexValue(pbMNIndex);
								/* $:set Flag to true*/
						pobjIndex->setFlagIfIncludedCdc(TRUE);
			
						char* pbVal = new char[8];
						iConfigDate = getConfigDate();
						pbVal = _IntToAscii(iConfigDate, pbVal, 10);
						//hexVal = padLeft(hexVal, '0' , 8);
						AddForEachSIdx(pbMNIndex, pobjIdxCol, pobjNode->getNodeId(), pbVal, false);		
											
										//AddForEachSIdx(pbMNIndex, pobjIdxCol, pobjNode->getNodeId(), hexVal, false);		
	
					}
					
							#if defined DEBUG	
						cout<< "1F26 subidex added"<<endl;
					#endif
							/* Add 1F27*/
					strcpy(pbMNIndex, "1F27");
					stRetInfo = AddIndex(MN_NODEID, MN, pbMNIndex);
					if(stRetInfo.code == OCFM_ERR_SUCCESS || stRetInfo.code == OCFM_ERR_INDEX_ALREADY_EXISTS)
					{				
						pobjIndex = pobjIdxCol->getIndexbyIndexValue(pbMNIndex);		
								/* $:set Flag to true*/
						pobjIndex->setFlagIfIncludedCdc(TRUE);
			
						char* pbVal = new char[50];
						iConfigTime = getConfigTime();
						//cout << "\n\n\nConfigTime:" << iConfigTime << endl;																		
						pbVal = _IntToAscii(iConfigTime, pbVal, 10);
						//Val = padLeft(hexVal, '0' , 8);

						AddForEachSIdx(pbMNIndex, pobjIdxCol, pobjNode->getNodeId(), pbVal, false);										
					}
					
					
					/* Add 1F84*/
					strcpy(pbMNIndex, "1F84");
					stRetInfo = AddIndex(MN_NODEID, MN, pbMNIndex);
					if(stRetInfo.code == OCFM_ERR_SUCCESS || stRetInfo.code == OCFM_ERR_INDEX_ALREADY_EXISTS)
					{				
						pobjIndex = pobjIdxCol->getIndexbyIndexValue(pbMNIndex);
								/* $:set Flag to true*/
						pobjIndex->setFlagIfIncludedCdc(TRUE);
						
						AddForEachSIdx(pbMNIndex, pobjIdxCol, pobjNode->getNodeId(), (char*)"", true);			
			
					}
					
							/* Add 1F89*/
					strcpy(pbMNIndex, "1F89");
					stRetInfo = AddIndex(MN_NODEID, MN, pbMNIndex);
					if(stRetInfo.code == OCFM_ERR_SUCCESS || stRetInfo.code == OCFM_ERR_INDEX_ALREADY_EXISTS)
					{
						pobjIndex = pobjIdxCol->getIndexbyIndexValue(pbMNIndex);
								/* $:set Flag to true*/
						pobjIndex->setFlagIfIncludedCdc(TRUE);
					/*	strcpy(pbSidx, "00");
						SetSIdxValue(pbMNIndex, pbSidx, "", pobjIdxCol, pobjNode->getNodeId(),true);
										
						strcpy(pbSidx, "01");
						SetSIdxValue(pbMNIndex, pbSidx, "", pobjIdxCol, pobjNode->getNodeId(),true);
				*/	
						strcpy(pbSidx, "02");
						SetSIdxValue(pbMNIndex, pbSidx, (char*)"", pobjIdxCol, pobjNode->getNodeId(), MN, true);
				
					/*		strcpy(pbSidx, "03");
						SetSIdxValue(pbMNIndex, pbSidx, "", pobjIdxCol, pobjNode->getNodeId(), true);
						
						strcpy(pbSidx, "04");
						SetSIdxValue(pbMNIndex, pbSidx, "", pobjIdxCol, pobjNode->getNodeId(), true);
						
						strcpy(pbSidx, "05");
						SetSIdxValue(pbMNIndex, pbSidx, "", pobjIdxCol, pobjNode->getNodeId(), true);
						
						strcpy(pbSidx, "06");
						SetSIdxValue(pbMNIndex, pbSidx, "", pobjIdxCol, pobjNode->getNodeId(), true);
						
						strcpy(pbSidx, "07");
						SetSIdxValue(pbMNIndex, pbSidx, "", pobjIdxCol, pobjNode->getNodeId(), true);
						
						strcpy(pbSidx, "08");
						SetSIdxValue(pbMNIndex, pbSidx, "", pobjIdxCol, pobjNode->getNodeId(), true);
						
						strcpy(pbSidx, "09");
						SetSIdxValue(pbMNIndex, pbSidx, "", pobjIdxCol, pobjNode->getNodeId(), true);
			*/
					}
			
					/* Add 1F8A*/
					strcpy(pbMNIndex, "1F8A");
					stRetInfo = AddIndex(MN_NODEID, MN, pbMNIndex);
					if(stRetInfo.code == OCFM_ERR_SUCCESS || stRetInfo.code == OCFM_ERR_INDEX_ALREADY_EXISTS)
					{				
						pobjIndex = pobjIdxCol->getIndexbyIndexValue(pbMNIndex);
								/* $:set Flag to true*/
						pobjIndex->setFlagIfIncludedCdc(TRUE);
						strcpy(pbSidx, "00");
						SetSIdxValue(pbMNIndex, pbSidx, (char*)"", pobjIdxCol, pobjNode->getNodeId(), MN, true);
						
						//$:To do By M as subindex 01 shud be equal to 02, need to find the reason 
						strcpy(pbSidx, "01");
						SetSIdxValue(pbMNIndex, pbSidx, (char*)"100000", pobjIdxCol, pobjNode->getNodeId(), MN, false);
					
						strcpy(pbSidx, "02");
						SetSIdxValue(pbMNIndex, pbSidx, (char*)"", pobjIdxCol, pobjNode->getNodeId(), MN, true);
						
					/*	strcpy(pbSidx, "03");
						SetSIdxValue(pbMNIndex, pbSidx, "", pobjIdxCol, pobjNode->getNodeId(), true);
						
						strcpy(pbSidx, "04");
						SetSIdxValue(pbMNIndex, pbSidx, "", pobjIdxCol, pobjNode->getNodeId(), true);
						
						strcpy(pbSidx, "05");
						SetSIdxValue(pbMNIndex, pbSidx, "", pobjIdxCol, pobjNode->getNodeId(), true);
						
						strcpy(pbSidx, "06");
						SetSIdxValue(pbMNIndex, pbSidx, "", pobjIdxCol, pobjNode->getNodeId(), true);
						
						strcpy(pbSidx, "07");
						SetSIdxValue(pbMNIndex, pbSidx, "", pobjIdxCol, pobjNode->getNodeId(), true);
						
						strcpy(pbSidx, "08");
						SetSIdxValue(pbMNIndex, pbSidx, "", pobjIdxCol, pobjNode->getNodeId(), true);
						
						strcpy(pbSidx, "09");
						SetSIdxValue(pbMNIndex, pbSidx, "", pobjIdxCol, pobjNode->getNodeId(), true);*/
					
						}
	
							/* Add 1F98*/
					strcpy(pbMNIndex, "1F98");
					
					stRetInfo = AddIndex(MN_NODEID, MN, pbMNIndex);
					if(stRetInfo.code == OCFM_ERR_SUCCESS)
					{				
					
							pobjIndex = pobjIdxCol->getIndexbyIndexValue(pbMNIndex);
								/* $:set Flag to true*/
						pobjIndex->setFlagIfIncludedCdc(TRUE);
						
						strcpy(pbSidx, "05");
						SetSIdxValue(pbMNIndex, pbSidx, abC_DLL_ISOCHR_MAX_PAYL, pobjIdxCol, pobjNode->getNodeId(), MN, false);
						
						/*strcpy(Sidx, "00");
						SetSIdxValue(pbMNIndex, pbSidx, "", pobjIdxCol, pobjNode->getNodeId(),true);
										
						strcpy(pbSidx, "01");
						SetSIdxValue(pbMNIndex, pbSidx, abC_DLL_ISOCHR_MAX_PAYL, pobjIdxCol, pobjNode->getNodeId(), false);
					
						strcpy(pbSidx, "02");
						SetSIdxValue(pbMNIndex, pbSidx, abC_DLL_ISOCHR_MAX_PAYL, pobjIdxCol, pobjNode->getNodeId(), false);
						
						
						strcpy(pbSidx, "07");
						SetSIdxValue(pbMNIndex, pbSidx, "", pobjIdxCol, pobjNode->getNodeId(), true);
						
						strcpy(pbSidx, "08");
						SetSIdxValue(pbMNIndex, pbSidx, abC_DLL_MIN_ASYNC_MTU, pobjIdxCol, pobjNode->getNodeId(), false);
						
						strcpy(pbSidx, "09");
						SetSIdxValue(pbMNIndex, pbSidx, "", pobjIdxCol, pobjNode->getNodeId(), true);*/
					}
					
					delete [] pbMNIndex;
					delete [] pbSidx;
					return stRetInfo;
		}
		catch(ocfmException & ex)
		{
			delete [] pbMNIndex;
			delete [] pbSidx;
			return ex._ocfmRetCode;
		}

}
/****************************************************************************************************
* Function Name: GenerateMNOBD
* Description: 
* Return value: ocfmRetCode
****************************************************************************************************/
ocfmRetCode GenerateMNOBD()
	{
		CNode objNode;		
		CNode *pobjMNNode;
		CNodeCollection *pobjNodeCollection = NULL;
		CIndexCollection * objMNIndexCol;
		CSubIndex * objSubIdex;
		CIndex* pobjIndex;			
		char* pbMNIndex = new char[INDEX_LEN];								
		char* pbIdx =  new char[SUBINDEX_LEN];
		ocfmRetCode stRetInfo;
		
		ocfmException objocfmException;
		INT32 iPrevSubIndex = 0 ;
		INT32 iPrevSize = 0;

		try
		{		
			pobjNodeCollection = CNodeCollection::getNodeColObjectPointer();	
			pobjMNNode  = pobjNodeCollection->getNodePtr(MN, MN_NODEID);
			if(pobjMNNode == NULL)
			{			
				objocfmException.ocfm_Excpetion(OCFM_ERR_MN_NODE_DOESNT_EXIST);
				throw objocfmException;
			}
			else
			{
				/*Process PDO Nodes*/
				stRetInfo = ProcessPDONodes();
				#if defined DEBUG	
						cout<< "Nodes Processed"<<endl;
					#endif
				if(stRetInfo.code != OCFM_ERR_SUCCESS)
				{
					return stRetInfo;
				}
				//char* pbTempCycleTime;
				//pbTempCycleTime = new char[20];
				
				CNode objNode;		
				CNodeCollection *pobjNodeCollection;
				CIndexCollection *pobjIndexCollection;
				CIndex objIndex;
				
				pobjNodeCollection = CNodeCollection::getNodeColObjectPointer();
				objNode = pobjNodeCollection->getNode(MN, MN_NODEID);
				pobjIndexCollection = objNode.getIndexCollection();
				//Check for existance of the Index
				
				#if defined DEBUG	
						cout<< "N1006"<<endl;
					#endif
				/* Delete the MN's old object dictionary*/

				pobjIndexCollection = pobjMNNode->getIndexCollection();	

					//cout << "Indexes deleted";
					// Delete IndexCollection
					//pobjIndexCollection->DeleteIndexCollection();
				pobjIndexCollection->DeletePDOs();
				// Delete Process Image Objects
				pobjIndexCollection->DeletePIObjects();
	
				// Autogenertate other indexs 
				AuotgenerateOtherIndexs(pobjMNNode);
				//DeleteNodeObjDict(240, MN);		
						#if defined DEBUG	
						cout<< "MN Node Object dictionary deleted"<<endl;
					#endif
				/* Add other Indexes than PDO*/
					//AddOtherMNIndexes(pobjMNNode, pbTempCycleTime);
					#if defined DEBUG	
						cout<< "Index added"<<endl;
					#endif
						/*if(stRetInfo.code != OCFM_ERR_SUCCESS)
						{
							objocfmException.ocfm_Excpetion(stRetInfo.code);
							throw objocfmException;
						}*/
					#if defined DEBUG	
						cout<< "Deleted"<<endl;
					#endif
			}
			for(INT32 iLoopCount =0; iLoopCount<pobjNodeCollection->getNumberOfNodes(); iLoopCount++)
			{
				objNode = pobjNodeCollection->getNodebyCollectionIndex(iLoopCount);			
				if(objNode.getNodeType() == CN)
				{
					iPrevSubIndex = 0;
					iPrevSize = 0;
					objMNIndexCol =  pobjMNNode->getIndexCollection();
									
					if(objNode.MNPDOOUTVarCollection.Count()!=0)
					{
												
						/* Create PDO_TxCommParam_XXh_REC 1800 INdex*/
						CIndex* pobjIndex;	
						strcpy(pbMNIndex, "18");
						pbIdx = _IntToAscii((objNode.getNodeId()-1), pbIdx, 16);
						pbIdx = padLeft(pbIdx, '0', 2);
						pbMNIndex =strcat(pbMNIndex, pbIdx);
					
						stRetInfo = AddIndex(MN_NODEID, MN, pbMNIndex);
							/* set bFlag to true for 1800*/
						pobjIndex = objMNIndexCol->getIndexbyIndexValue(pbMNIndex);
						if(pobjIndex != NULL)
						
							
						if(stRetInfo.code != OCFM_ERR_SUCCESS)
						{
							objocfmException.ocfm_Excpetion(stRetInfo.code);
							throw objocfmException;
						}
						
						pbIdx = _IntToAscii(objNode.getNodeId(), pbIdx, 10);
						char* pbSidx =  new char[SUBINDEX_LEN];
						strcpy(pbSidx, "01");
						SetSubIndexAttributes(MN_NODEID, MN, pbMNIndex, pbSidx, pbIdx,(char*)"NodeID_U8", TRUE);
									
						strcpy(pbMNIndex, "1A");
						pbIdx = _IntToAscii((objNode.getNodeId()-1), pbIdx, 16);
						pbIdx = padLeft(pbIdx, '0', 2);
						pbMNIndex =strcat(pbMNIndex, pbIdx);
						/* Set the MN's PDO Index*/
						stRetInfo = AddIndex(MN_NODEID, MN, pbMNIndex);
						
					
						pobjIndex->setFlagIfIncludedCdc(TRUE);
							
						if(stRetInfo.code != OCFM_ERR_SUCCESS)
						{
							objocfmException.ocfm_Excpetion(stRetInfo.code);
							throw objocfmException;
						}
					
						//pobjIndex = objMNIndexCol->getIndexbyIndexValue(pbMNIndex);
						for(INT32 iLoopCount=0; iLoopCount<objNode.MNPDOOUTVarCollection.Count() ; iLoopCount++)
						{
							MNPdoVariable stMNPdoVar;
							stMNPdoVar = objNode.MNPDOOUTVarCollection[iLoopCount];	
							pobjIndex = objMNIndexCol->getIndexbyIndexValue(pbMNIndex);

							if(pobjIndex !=NULL)
							{
							 pobjIndex->setFlagIfIncludedCdc(TRUE);				
								GetMNPDOSubIndex(stMNPdoVar, iPrevSubIndex, pobjIndex, pbMNIndex, iPrevSize);
								iPrevSize = iPrevSize + stMNPdoVar.DataSize;
							}
					
						}
						char* pbActVal = new char[4];
						pbActVal = _IntToAscii(objNode.MNPDOOUTVarCollection.Count(), pbActVal, 16);
						pbActVal = ConvertToHexformat(pbActVal, 2, true);	
						objSubIdex = pobjIndex->getSubIndexbyIndexValue((char*)"00");
						objSubIdex->setActualValue(pbActVal);
		
					}
											
				if(objNode.MNPDOINVarCollection.Count()!=0)
					{
												
						/* Create PDO_TxCommParam_XXh_REC 1800 INdex*/
						strcpy(pbMNIndex, "14");
						pbIdx = _IntToAscii((objNode.getNodeId()-1), pbIdx, 16);
						pbIdx = padLeft(pbIdx, '0', 2);
						pbMNIndex =strcat(pbMNIndex, pbIdx);
										
						stRetInfo = AddIndex(MN_NODEID, MN, pbMNIndex);
						/* set bFlag to true for 1800*/
						pobjIndex = objMNIndexCol->getIndexbyIndexValue(pbMNIndex);
						if(pobjIndex != NULL)
							pobjIndex->setFlagIfIncludedCdc(TRUE);
						
						iPrevSubIndex = 0;
						iPrevSize = 0;
						if(stRetInfo.code != OCFM_ERR_SUCCESS)
						{
							objocfmException.ocfm_Excpetion(stRetInfo.code);
							throw objocfmException;
						}
						
						pbIdx = _IntToAscii(objNode.getNodeId(), pbIdx, 10);
						char* pbSidx =  new char[SUBINDEX_LEN];
						strcpy(pbSidx, "01");
						SetSubIndexAttributes(MN_NODEID, MN, pbMNIndex, pbSidx, pbIdx,(char*)"NodeID_U8", TRUE);
				
							
						strcpy(pbMNIndex, "16");
						pbIdx = _IntToAscii((objNode.getNodeId()-1), pbIdx, 16);
						pbIdx = padLeft(pbIdx, '0', 2);
						pbMNIndex =strcat(pbMNIndex, pbIdx);
						/* Set the MN's PDO Index*/
						stRetInfo = AddIndex(MN_NODEID, MN, pbMNIndex);
						
						
						if(stRetInfo.code != OCFM_ERR_SUCCESS)
						{
							objocfmException.ocfm_Excpetion(stRetInfo.code);
							throw objocfmException;
						}
					
						
						pobjIndex = objMNIndexCol->getIndexbyIndexValue(pbMNIndex);
						for(INT32 iLoopCount=0; iLoopCount<objNode.MNPDOINVarCollection.Count() ; iLoopCount++)
						{
							MNPdoVariable stMNPdoVar;
							stMNPdoVar = objNode.MNPDOINVarCollection[iLoopCount];		
							pobjIndex = objMNIndexCol->getIndexbyIndexValue(pbMNIndex);					
							pobjIndex->setFlagIfIncludedCdc(TRUE);
							GetMNPDOSubIndex(stMNPdoVar, iPrevSubIndex, pobjIndex, pbMNIndex, iPrevSize);				
							iPrevSize = iPrevSize + stMNPdoVar.DataSize ;	
						}
						char* pbActVal = new char[4];
						pbActVal = _IntToAscii(objNode.MNPDOINVarCollection.Count(), pbActVal, 16);
						//pbActVal = padLeft(pbActVal, '0', 4);
						pbActVal = ConvertToHexformat(pbActVal, 2, true);						
						objSubIdex = pobjIndex->getSubIndexbyIndexValue((char*)"00");
						objSubIdex->setActualValue(pbActVal);
					
					}
				
			}
		}
		setFlagForRequiredMNIndexes(MN_NODEID);
	}
	
	catch(ocfmException& objocfmException)
	{
		return objocfmException._ocfmRetCode;
	}
	//cout << "generation of MN OBDD completed" << endl;
	/* Testing Code*/
		/*		pobjNodeCollection = CNodeCollection::getNodeColObjectPointer();	
			pobjMNNode  = pobjNodeCollection->getNodePtr(MN, 240);
		CIndexCollection * objIndexCol;
			objIndexCol = pobjMNNode->getIndexCollection();
			for(INT32 iLoopCount = 0; iLoopCount< objIndexCol->getNumberofIndexes(); iLoopCount++)
			{
				CIndex* pobjIndex;
				pobjIndex = objIndexCol->getIndex(iLoopCount);
				//printf("\n**************************");
				//printf("\n Index value :%s", pobjIndex->getIndexValue());
				for(INT32 y = 0; y<pobjIndex->getNumberofSubIndexes(); y++)
				{
					CSubIndex* pobjSubIndex;
					DataType objDataType;
					pobjSubIndex = pobjIndex->getSubIndex(y);
					objDataType = pobjSubIndex->getDataType();
					//printf("\n SubIndex: %s \n DataType = %s \n Actual Value = %s",pobjSubIndex->getIndexValue(),objDataType.DataSize,
																																																																				pobjSubIndex->getActualValue());
				}
			}*/

	stRetInfo.code = OCFM_ERR_SUCCESS;
	return stRetInfo;
}

/****************************************************************************************************
* Function Name: checkIfOffsetUsed
* Description: 
* Return value: void
****************************************************************************************************/
void checkIfOffsetUsed(INT32& iOffset, EPDOType enumPdoType)
{
	CNodeCollection* pobjNodeCol;
	pobjNodeCol =  CNodeCollection::getNodeColObjectPointer();
	CNode objNode;
	
	for(INT32 iNodeIndex = 0 ; iNodeIndex < pobjNodeCol->getNumberOfNodes(); iNodeIndex++)
	{
		objNode = pobjNodeCol->getNodebyCollectionIndex(iNodeIndex);
		if(objNode.getNodeType() ==CN )
		{
			if(enumPdoType == PDO_RPDO)
			{
				for(INT32 iLoopCount =0 ; iLoopCount < objNode.MNPDOOUTVarCollection.Count(); iLoopCount++)
				{
					if(objNode.MNPDOOUTVarCollection[iLoopCount].Offset == iOffset)
						iOffset = iOffset + objNode.MNPDOOUTVarCollection[iLoopCount].DataSize/8;
				}
			}
			else	if(enumPdoType == PDO_TPDO)
			{
				for(INT32 iLoopCount =0 ; iLoopCount < objNode.MNPDOINVarCollection.Count(); iLoopCount++)
				{
					if(objNode.MNPDOINVarCollection[iLoopCount].Offset == iOffset)
						iOffset = iOffset + objNode.MNPDOINVarCollection[iLoopCount].DataSize/8;
				}
			}
		}
	}
	
}

/****************************************************************************************************
* Function Name: ComputeOUTOffset
* Description: 
* Return value: INT32
****************************************************************************************************/
INT32 ComputeOUTOffset(INT32 iDataSize, EPDOType enumPdoType)
{
	//INT32 arrOfOffsets[4][2] = {{-1, -1}, {-1, -1},{-1, -1},{-1,-1}};			/* Contain prev and current offsets of size 1Bytes, 2 bytes, 4 bytes and 8 bytes*/	
	INT32 iOffset = 0;
							

		switch(iDataSize)
		{
			
			/*Unsigned8, Int8*/
			case 8:
						stSize8OUTOffset.prevOffset = stSize8OUTOffset.currOffset ;
						iOffset = stSize8OUTOffset.currOffset ;
						stSize8OUTOffset.currOffset =	stSize8OUTOffset.currOffset + 1;
						
						/* Set other DataType Offsets*/
						/* if greater no change*/
						if(stSize16OUTOffset.currOffset >= stSize8OUTOffset.currOffset)
						{}
						else
						{
								stSize16OUTOffset.prevOffset = stSize16OUTOffset.currOffset ;
								stSize16OUTOffset.currOffset = stSize16OUTOffset.currOffset + 2;
						}
						
						/* if greater no change*/
						if(stSize32OUTOffset.currOffset >= stSize8OUTOffset.currOffset)
						{}
						else
						{
								stSize32OUTOffset.prevOffset = stSize32OUTOffset.currOffset ;
								stSize32OUTOffset.currOffset = stSize32OUTOffset.currOffset + 4;
						}
						
						/* if greater no change*/
						if(stSize64OUTOffset.currOffset >= stSize8OUTOffset.currOffset)
						{}
						else
						{
								stSize64OUTOffset.prevOffset = stSize64OUTOffset.currOffset ;
								stSize64OUTOffset.currOffset = stSize64OUTOffset.currOffset + 8;
						}
						break;
						
			/*Unsigned16, Int16*/
			case 16:
						stSize16OUTOffset.prevOffset = stSize16OUTOffset.currOffset ;
						iOffset = stSize16OUTOffset.currOffset ;
						stSize16OUTOffset.currOffset =	stSize16OUTOffset.currOffset + 2;
				
							/* Set other DataType Offsets*/
							
							/* if greater no change*/
						if(stSize8OUTOffset.currOffset >= stSize16OUTOffset.currOffset)
						{}
						else
						{
								stSize8OUTOffset.prevOffset = stSize8OUTOffset.currOffset ;
									stSize8OUTOffset.currOffset = stSize16OUTOffset.currOffset;
							/*	if((stSize8OUTOffset.currOffset >= stSize16OUTOffset.prevOffset) && (stSize8OUTOffset.currOffset <= stSize16OUTOffset.currOffset))
								{
									stSize8OUTOffset.prevOffset = stSize8OUTOffset.currOffset ;
									stSize8OUTOffset.currOffset = stSize16OUTOffset.currOffset;
								}
								else
								{
									stSize8OUTOffset.prevOffset = stSize8OUTOffset.currOffset;
									newOffset = stSize8OUTOffset.currOffset;
									checkIfOffsetUsed(newOffset, enumPdoType);				
									stSize8OUTOffset.currOffset = newOffset;
								}*/
						}
						
						/* if greater no change*/
						if(stSize32OUTOffset.currOffset >= stSize16OUTOffset.currOffset)
						{
							//
						}
						else
						{
								stSize32OUTOffset.prevOffset = stSize32OUTOffset.currOffset ;
								stSize32OUTOffset.currOffset = stSize32OUTOffset.currOffset + 4;
						}
						
						/* if greater no change*/
						if(stSize64OUTOffset.currOffset >= stSize16OUTOffset.currOffset)
						{}
						else
						{
								stSize64OUTOffset.prevOffset = stSize64OUTOffset.currOffset ;
								stSize64OUTOffset.currOffset = stSize64OUTOffset.currOffset + 8;
						}						
					break;
			/*Unsigned32, Int32*/
			case 32:
						stSize32OUTOffset.prevOffset = stSize32OUTOffset.currOffset ;
						iOffset = stSize32OUTOffset.currOffset ;
						stSize32OUTOffset.currOffset = stSize32OUTOffset.currOffset + 4;
					
						
			
							/* Set other DataType Offsets*/
								/* if greater no change*/
						if(stSize8OUTOffset.currOffset >= stSize32OUTOffset.currOffset)
						{
							//
						}
						else
						{
								stSize8OUTOffset.prevOffset = stSize8OUTOffset.currOffset ;
								stSize8OUTOffset.currOffset = stSize32OUTOffset.currOffset;
								//if((stSize8OUTOffset.currOffset >= stSize32OUTOffset.prevOffset) && (stSize8OUTOffset.currOffset <= stSize32OUTOffset.currOffset))
								//{
								//	stSize8OUTOffset.prevOffset = stSize8OUTOffset.currOffset ;
								//	stSize8OUTOffset.currOffset = stSize32OUTOffset.currOffset;
								//}
								//else
								//{
								//	stSize8OUTOffset.prevOffset = stSize8OUTOffset.currOffset;
								//		newOffset = stSize8OUTOffset.currOffset;
								//	
								//	checkIfOffsetUsed(newOffset, enumPdoType);
								//	
								//	/*while(checkIfOffsetUsed(iOffset))
								//	{
								//		iOffset = iOffset + 1;
								//	}*/
								//	stSize8OUTOffset.currOffset = newOffset;
								//}
						}
						
							/* if greater no change*/
						if(stSize16OUTOffset.currOffset >= stSize32OUTOffset.currOffset)
						{}
						else
						{
								stSize16OUTOffset.prevOffset = stSize16OUTOffset.currOffset ;
								stSize16OUTOffset.currOffset = stSize32OUTOffset.currOffset;
								//if((stSize16OUTOffset.currOffset >= stSize32OUTOffset.prevOffset) && (stSize16OUTOffset.currOffset < stSize32OUTOffset.currOffset))
								//{
								//	stSize16OUTOffset.prevOffset = stSize16OUTOffset.currOffset ;
								//	stSize16OUTOffset.currOffset = stSize32OUTOffset.currOffset;
								//}
								//else
								//{
								//	stSize16OUTOffset.prevOffset = stSize16OUTOffset.currOffset;
								//	newOffset = stSize16OUTOffset.currOffset;
								//		checkIfOffsetUsed(newOffset, enumPdoType);
								///*	while(checkIfOffsetUsed(iOffset))
								//	{
								//		iOffset = iOffset + 1;
								//	}*/
								//	stSize16OUTOffset.currOffset = newOffset;
								//
								//}
						}
						/* if greater no change*/
						if(stSize64OUTOffset.currOffset >= stSize8OUTOffset.currOffset)
						{}
						else
						{
								stSize64OUTOffset.prevOffset = stSize64OUTOffset.currOffset ;
								stSize64OUTOffset.currOffset = stSize64OUTOffset.currOffset + 8;
						}
						break;
			/*Unsigned64, Int64*/
			case 64:
						stSize64OUTOffset.prevOffset = stSize64OUTOffset.currOffset ;
						iOffset = stSize64OUTOffset.currOffset ;
						stSize64OUTOffset.currOffset = stSize64OUTOffset.currOffset + 8;
			
			break;
		}
		return iOffset;
	}

/****************************************************************************************************
* Function Name: ComputeINOffset
* Description: 
* Return value: INT32
****************************************************************************************************/
INT32 ComputeINOffset(INT32 iDataSize, EPDOType enumPdoType)
{
	//INT32 arrOfOffsets[4][2] = {{-1, -1}, {-1, -1},{-1, -1},{-1,-1}};			/* Contain prev and current offsets of size 1Bytes, 2 bytes, 4 bytes and 8 bytes*/	
	INT32 iOffset = 0;
							

		switch(iDataSize)
		{
			
			/*Unsigned8, Int8*/
			case 8:
						stSize8INOffset.prevOffset = stSize8INOffset.currOffset ;
						iOffset = stSize8INOffset.currOffset ;
						stSize8INOffset.currOffset =	stSize8INOffset.currOffset + 1;
						/* Set other DataType Offsets*/
						/* if greater no change*/
						if(stSize16INOffset.currOffset >= stSize8INOffset.currOffset)
						{}
						else
						{
								stSize16INOffset.prevOffset = stSize16INOffset.currOffset ;
								stSize16INOffset.currOffset = stSize16INOffset.currOffset + 2;
						}
						
						/* if greater no change*/
						if(stSize32INOffset.currOffset >= stSize8INOffset.currOffset)
						{}
						else
						{
						
								
								stSize32INOffset.prevOffset = stSize32INOffset.currOffset ;
								stSize32INOffset.currOffset = stSize32INOffset.currOffset + 4;
							
						}
						
						/* if greater no change*/
						if(stSize64INOffset.currOffset >= stSize8INOffset.currOffset)
						{}
						else
						{
								stSize64INOffset.prevOffset = stSize64INOffset.currOffset ;
								stSize64INOffset.currOffset = stSize64INOffset.currOffset + 8;
						}
						break;
						
			/*Unsigned16, Int16*/
			case 16:
						stSize16INOffset.prevOffset = stSize16INOffset.currOffset ;
						iOffset = stSize16INOffset.currOffset ;
						stSize16INOffset.currOffset =	stSize16INOffset.currOffset + 2;
					
			
							/* Set other DataType Offsets*/
							
							/* if greater no change*/
						if(stSize8INOffset.currOffset >= stSize16INOffset.currOffset)
						{}
						else
						{
							stSize8INOffset.prevOffset = stSize8INOffset.currOffset ;
							stSize8INOffset.currOffset = stSize16INOffset.currOffset;
								//if((stSize8INOffset.currOffset >= stSize16INOffset.prevOffset) && (stSize8INOffset.currOffset <= stSize16INOffset.currOffset))
								//{
								//	stSize8INOffset.prevOffset = stSize8INOffset.currOffset ;
								//	stSize8INOffset.currOffset = stSize16INOffset.currOffset;
								//}
								//else
								//{
								//	stSize8INOffset.prevOffset = stSize8INOffset.currOffset;
								//	/*newOffset = stSize8INOffset.currOffset;
								//	checkIfOffsetUsed(newOffset, enumPdoType);				
								//	stSize8INOffset.currOffset = newOffset;*/
								//}
						}
						
						/* if greater no change*/
						if(stSize32INOffset.currOffset >= stSize16INOffset.currOffset)
						{}
						else
						{
								stSize32INOffset.prevOffset = stSize32INOffset.currOffset ;
								stSize32INOffset.currOffset = stSize32INOffset.currOffset + 4;
						}
						
						/* if greater no change*/
						if(stSize64INOffset.currOffset >= stSize16INOffset.currOffset)
						{}
						else
						{
								stSize64INOffset.prevOffset = stSize64INOffset.currOffset ;
								stSize64INOffset.currOffset = stSize64INOffset.currOffset + 8;
						}						
					break;
			/*Unsigned32, Int32*/
			case 32:
						stSize32INOffset.prevOffset = stSize32INOffset.currOffset ;
						iOffset = stSize32INOffset.currOffset ;
						stSize32INOffset.currOffset = stSize32INOffset.currOffset + 4;
			
							/* Set other DataType Offsets*/
								/* if greater no change*/
						if(stSize8INOffset.currOffset >= stSize32INOffset.currOffset)
						{}
						else
						{
									stSize8INOffset.prevOffset = stSize8INOffset.currOffset ;
									stSize8INOffset.currOffset = stSize32INOffset.currOffset;
								//if((stSize8INOffset.currOffset >= stSize32INOffset.prevOffset) && (stSize8INOffset.currOffset <= stSize32INOffset.currOffset))
								//{
								//	stSize8INOffset.prevOffset = stSize8INOffset.currOffset ;
								//	stSize8INOffset.currOffset = stSize32INOffset.currOffset;
								//}
								//else
								//{
								//	stSize8INOffset.prevOffset = stSize8INOffset.currOffset;
								//		newOffset = stSize8INOffset.currOffset;
								//	
								//	checkIfOffsetUsed(newOffset, enumPdoType);
								//	
								//	/*while(checkIfOffsetUsed(iOffset))
								//	{
								//		iOffset = iOffset + 1;
								//	}*/
								//	stSize8INOffset.currOffset = newOffset;
								//}
						}
						
							/* if greater no change*/
						if(stSize16INOffset.currOffset >= stSize32INOffset.currOffset)
						{}
						else
						{
								stSize16INOffset.prevOffset = stSize16INOffset.currOffset ;
								stSize16INOffset.currOffset = stSize32INOffset.currOffset;
								//if((stSize16INOffset.currOffset >= stSize32INOffset.prevOffset) && (stSize16INOffset.currOffset < stSize32INOffset.currOffset))
								//{
								//	stSize16INOffset.prevOffset = stSize16INOffset.currOffset ;
								//	stSize16INOffset.currOffset = stSize32INOffset.currOffset;
								//}
								//else
								//{
								//	stSize16INOffset.prevOffset = stSize16INOffset.currOffset;
								//	newOffset = stSize16INOffset.currOffset;
								//	checkIfOffsetUsed(newOffset, enumPdoType);
								///*	while(checkIfOffsetUsed(iOffset))
								//	{
								//		iOffset = iOffset + 1;
								//	}*/
								//	stSize16INOffset.currOffset = newOffset;
								//
								//}
						}
						/* if greater no change*/
						if(stSize64INOffset.currOffset >= stSize8INOffset.currOffset)
						{}
						else
						{
								stSize64INOffset.prevOffset = stSize64INOffset.currOffset ;
								stSize64INOffset.currOffset = stSize64INOffset.currOffset + 8;
						}
						break;
			/*Unsigned64, Int64*/
			case 64:
						stSize64INOffset.prevOffset = stSize64INOffset.currOffset ;
						iOffset = stSize64INOffset.currOffset ;
			
			break;
		}
		return iOffset;
	}

	/*INT32 CheckAvailableOffset(PDODataType pdt)
	{
		INT32 iOffset;
	
		switch(pdt)
		{
			case INTEGER8:
					iOffset = size8Offsets[iNo8Offsets-1] + 1;
					if(iNo16Offsets ==0)
					{break;}
					if(size16Offsets[0] > iOffset)
					{break;}
					for(INT32 iLoopCount= 0 ;iLoopCount < iNo16Offsets; iLoopCount++)
					{
						if(size16Offsets[iLoopCount] == iOffset)
						{
							iOffset =  iOffset + 2;
						}														
					} 
					
					if(no32Offets == 0)
					{break;}
					if(size32Offsets[0] != iOffset)
					{break;}
					if(iNo64Offsets==0)
					{break;}
						for(INT32 iLoopCount= 0 ;iLoopCount < iNo32Offsets; iLoopCount++)
						{
							if(size32Offsets[iLoopCount] == iOffset)
								iOffset =  iOffset + 2;
							else
							break;
						} 
			case INTEGER16:
					iOffset = size16Offsets[iNo16Offsets-1] + 1;
					if(iNo8Offsets ==0)
					{break;}
					if(size16Offsets[0] > iOffset)
					{break;}
					for(INT32 iLoopCount= 0 ;iLoopCount < iNo16Offsets; iLoopCount++)
					{
						if(size16Offsets[iLoopCount] == iOffset)
						{
							iOffset =  iOffset + 2;
						}														
					} 
					
					if(no32Offets == 0)
					{break;}
					if(size32Offsets[0] != iOffset)
					{break;}
					if(iNo64Offsets==0)
					{break;}
						for(INT32 iLoopCount= 0 ;iLoopCount < iNo32Offsets; iLoopCount++)
						{
							if(size32Offsets[iLoopCount] == iOffset)
								iOffset =  iOffset + 2;
							else
							break;
						} 
			case INTEGER32:
			case INTEGER64:
		}
	}*/

/****************************************************************************************************
* Function Name: FreeProjectMemory
* Description: Close the current project and free the memory
* Return value: ocfmRetCode
****************************************************************************************************/
ocfmRetCode FreeProjectMemory()
{
	CNodeCollection *pobjNodeCollection = NULL;
	pobjNodeCollection = CNodeCollection::getNodeColObjectPointer();
	delete pobjNodeCollection;
	ocfmRetCode stErrorInfo;		 
	stErrorInfo.code = OCFM_ERR_SUCCESS;
	return stErrorInfo;
}
	
/****************************************************************************************************
* Function Name: OpenProject
* Description: Saves all the Nodes into the Project location
* Return value: ocfmRetCode
****************************************************************************************************/

ocfmRetCode OpenProject(char* pbPjtPath, char* pbProjectXmlFileName)
{
	CNodeCollection *pobjNodeCollection;
	pobjNodeCollection = CNodeCollection::getNodeColObjectPointer();
	xmlTextReaderPtr pxReader;
    	INT32 iRetVal;
	char *pbFileName;	

	//delete pobjNodeCollection;
	
	#if defined DEBUG
	cout << "\nStrLen for FileName:" << (strlen(pbPjtPath) + strlen(pbProjectXmlFileName) + 1) << endl;
	#endif
	
	pbFileName = new char[(strlen(pbPjtPath) + strlen(pbProjectXmlFileName) + 5)];
	#if defined(_WIN32) && defined(_MSC_VER)
	{		
		sprintf(pbFileName, "%s\\%s", pbPjtPath, pbProjectXmlFileName);	
	}
	#else
	{
		sprintf(pbFileName, "%s/%s", pbPjtPath, pbProjectXmlFileName);
	}
	#endif			
    pxReader = xmlReaderForFile(pbFileName, NULL, 0);
    try
    {
		if (pxReader != NULL)
		{
			iRetVal = xmlTextReaderRead(pxReader);
			while (iRetVal == 1)
			{		
				processProjectXML(pxReader, pbPjtPath);
				iRetVal = xmlTextReaderRead(pxReader);
			}
			if(iRetVal!=0)
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
        
        pobjNodeCollection = CNodeCollection::getNodeColObjectPointer();
        CNode objNode;
        INT32 iNodeID;
        ENodeType iNodeType;
        for(INT32 iLoopCount = 0; iLoopCount < pobjNodeCollection->getNumberOfNodes(); iLoopCount++)
        {
            objNode = pobjNodeCollection->getNodebyCollectionIndex(iLoopCount);
            
            iNodeType = objNode.getNodeType();
            iNodeID = objNode.getNodeId();
            copyPDODefToAct(iNodeID, iNodeType);
            copyMNPropDefToAct(iNodeID, iNodeType);
            calculateCNPollResponse(iNodeID, iNodeType);
        }
        //RecalculateMultiplex();
	}								
	
	catch(ocfmException& objocfmException)
	{
		return objocfmException._ocfmRetCode;
	}
	ocfmRetCode stErrorInfo;		 
	stErrorInfo.code = OCFM_ERR_SUCCESS;
	return stErrorInfo;
}


/****************************************************************************************************
* Function Name: processProjectXML
* Description: Process the Node value,Name and its attributes
* Return value: ocfmRetCode
****************************************************************************************************/
ocfmRetCode processProjectXML(xmlTextReaderPtr pxReader, char* pbPjtPath)
{
	const xmlChar *pxcName;
	const xmlChar *value;
	CPjtSettings* pobjPjtSettings;
	pobjPjtSettings = CPjtSettings::getPjtSettingsPtr();
	pxcName = xmlTextReaderConstName(pxReader);
	if (pxcName == NULL)
	{
		#if defined DEBUG
			cout << "\nGot NULL for Name\n" << endl;	
		#endif
	}
    value = xmlTextReaderConstValue(pxReader);
	try
	{ 
		if( xmlTextReaderNodeType(pxReader)==1)
		{
			// Check for openCONFIGURATOR Tag
			if(strcmp(((char*)pxcName),"openCONFIGURATOR")==0)
			{
				#if defined DEBUG
					cout << "openCONFIGURATOR Tag present\n" << endl;	
				#endif
				if (xmlTextReaderHasAttributes(pxReader) == 1)
				{
					while(xmlTextReaderMoveToNextAttribute(pxReader))
					{
						// Call Check Version number Fn
						if(IfVersionNumberMatches(pxReader) == false)
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
			else if (strcmp(((char*)pxcName),"profile")==0)
			{
				#if defined DEBUG
					cout << "profile Tag present\n" << endl;	
				#endif
				if (xmlTextReaderHasAttributes(pxReader) == 1)
				{	
					#if defined DEBUG
						cout << "Cannot open project: Invalid Project XML\n" << endl;	
					#endif
					ocfmException objException;
					objException.ocfm_Excpetion(OCFM_ERR_INVALID_PJTXML);
					throw objException;								
				}
			}
			else if (strcmp(((char*)pxcName),"Auto")==0)
			{
				#if defined DEBUG
					cout << "Auto Tag present\n" << endl;	
				#endif
				if (xmlTextReaderHasAttributes(pxReader) == 1)
				{	
					if(setProjectSettings_Auto(pxReader) == false)
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
			else if (strcmp(((char*)pxcName),"Communication")==0)
			{
				#if defined DEBUG
					cout << "Communication Tag present\n" << endl;	
				#endif
				if (xmlTextReaderHasAttributes(pxReader) == 1)
				{	
					if(setProjectSettings_Communication(pxReader) == false)
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
			else if (strcmp(((char*)pxcName),"NodeCollection")==0)
			{
				#if defined DEBUG
					cout << "NodeCollection Tag present\n" << endl;	
				#endif
				if (xmlTextReaderHasAttributes(pxReader) == 1)
				{
					#if defined DEBUG
						cout << "Cannot open project: Invalid Project XML\n" << endl;	
					#endif
					ocfmException objException;
					objException.ocfm_Excpetion(OCFM_ERR_INVALID_PJTXML);
					throw objException;	
				}
			}
			else if (strcmp(((char*)pxcName),"Node") == 0)
			{
				if (xmlTextReaderHasAttributes(pxReader) == 1)
				{
					if(getandCreateNode(pxReader, pbPjtPath) == false)
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
						ocfmRetCode stErrorInfo;
						stErrorInfo.code = OCFM_ERR_SUCCESS;
						return stErrorInfo;
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
	catch(ocfmException* objocfmException)
	{		
		 return objocfmException->_ocfmRetCode;
	}	
	#if defined DEBUG
		//cout << "\nstPjtSettings.getGenerateAttr():" << pobjPjtSettings->getGenerateAttr() << endl;
		//cout << "\nstPjtSettings.getSaveAttr():" << pobjPjtSettings->getSaveAttr() << endl;
		//if(pobjPjtSettings->getPOWERLINK_IP() != NULL)
		//	cout << "\nstPjtSettings.getPOWERLINK_IP():" << pobjPjtSettings->getPOWERLINK_IP() << endl;
	#endif
	ocfmRetCode stErrorInfo;
	stErrorInfo.code = OCFM_ERR_SUCCESS;
	return stErrorInfo;
}

/****************************************************************************************************
* Function Name: setProjectSettings_Auto
* Description: Gets the Auto info from the Pjt xml and stores in the object
* Return value: bool[True/False]
****************************************************************************************************/
bool setProjectSettings_Auto(xmlTextReaderPtr pxReader)
{
	const xmlChar* pxcName;
	const xmlChar* pxcValue;
	CPjtSettings* pobjPjtSettings;
	pobjPjtSettings = CPjtSettings::getPjtSettingsPtr();
	
	while(xmlTextReaderMoveToNextAttribute(pxReader))
	{	
		//Retrieve the pxcName and Value of an attribute	
		pxcValue = xmlTextReaderConstValue(pxReader);
		pxcName  = xmlTextReaderConstName(pxReader);	

		if(pxcValue == NULL || pxcName == NULL)
			return false;		
		#if defined DEBUG
			cout << "\nName:" << pxcName << endl;	
			cout << "\nValue:" << pxcValue << endl;
		#endif
		if (strcmp(((char*)pxcName),"Generate") == 0)
		{
			if(strcmp(((char*)pxcValue),"YES") == 0)
				pobjPjtSettings->setGenerateAttr(YES_AG);
			else if(strcmp(((char*)pxcValue),"NO") == 0)
				pobjPjtSettings->setGenerateAttr(NO_AG);
			else
			{
				#if defined DEBUG
					cout << "\nsetProjectSettings_Auto returning false" << endl;
				#endif
				return false;
			}
		}
		else if (strcmp(((char*)pxcName),"Save") == 0)
		{
			if(strcmp(((char*)pxcValue),"YES") == 0)
				pobjPjtSettings->setSaveAttr(YES_AS);
			else if(strcmp(((char*)pxcValue),"PROMPT") == 0)
				pobjPjtSettings->setSaveAttr(PROMPT_AS);
			else if(strcmp(((char*)pxcValue),"DISCARD") == 0)
				pobjPjtSettings->setSaveAttr(DISCARD_AS);
			else
			{
				#if defined DEBUG
					cout << "\nsetProjectSettings_Auto returning false" << endl;
				#endif
				return false;
			}
		}
		else if (strcmp(((char*)pxcName),"View") == 0)
		{
			if(strcmp(((char*)pxcValue),"General") == 0)
				pobjPjtSettings->setViewMode(SIMPLE);
			else if(strcmp(((char*)pxcValue),"Advanced") == 0)
				pobjPjtSettings->setViewMode(EXPERT);			
			else
			{
				#if defined DEBUG
					cout << "\nsetProjectSettings_Auto returning false" << endl;
				#endif
				return false;
			}
		}
		else if (strcmp(((char*)pxcName),"ExpertViewSelected") == 0)
		{
			if(strcmp(((char*)pxcValue),"true") == 0)
				pobjPjtSettings->setExpertViewSelectedFlag(true);
			else if(strcmp(((char*)pxcValue),"false") == 0)
				pobjPjtSettings->setExpertViewSelectedFlag(false);			
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

/****************************************************************************************************
* Function Name: setProjectSettings_Communication
* Description: Gets the communication info from the Pjt xml and stores in the object
* Return value: bool[True/False]
****************************************************************************************************/
bool setProjectSettings_Communication(xmlTextReaderPtr pxReader)
{
	const xmlChar* pxcName;
	const xmlChar* pxcValue;
	CPjtSettings* pobjPjtSettings;
	pobjPjtSettings = CPjtSettings::getPjtSettingsPtr();
	
	while(xmlTextReaderMoveToNextAttribute(pxReader))
	{	
		//Retrieve the pxcName and Value of an attribute	
		pxcValue = xmlTextReaderConstValue(pxReader);
		pxcName = xmlTextReaderConstName(pxReader);	

		if(pxcValue == NULL || pxcName == NULL)
			return false;		
		#if defined DEBUG
			cout << "\nName:" << pxcName << endl;	
			cout << "\nValue:" << pxcValue << endl;
		#endif
		if (strcmp(((char*)pxcName),"IP") == 0)
		{
			if((char*)pxcValue != NULL)		
			{
				pobjPjtSettings->setPOWERLINK_IP((char*)pxcValue);
				//cout << "\nstPjtSettings->getPOWERLINK_IP():" << pobjPjtSettings->getPOWERLINK_IP() << endl;
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

/****************************************************************************************************
* Function Name: getandCreateNode
* Description: Gets the Node properties from the Pjt xml and Creates the Nodes
* Return value: bool[True/False]
****************************************************************************************************/
bool getandCreateNode(xmlTextReaderPtr pxReader, char* pbPjtPath)
{
	const xmlChar *pxcName;
	const xmlChar *pxcValue;
	ocfmRetCode stErrorInfo;
	
	char* pbNodeName;
	INT32 iNodeID;
	ENodeType enumNodeType;
	char* pbXdcPath;
	char* pbFileName;
	bool bForceCycleFlag;
    EStationType eStationType = NORMAL;
    char* pbForceCycleValue = NULL;
	CPjtSettings* pobjPjtSettings;
	pobjPjtSettings = CPjtSettings::getPjtSettingsPtr();
	
	while(xmlTextReaderMoveToNextAttribute(pxReader))
	{	
		//Retrieve the pxcName and Value of an attribute	
		pxcValue = xmlTextReaderConstValue(pxReader);
		pxcName = xmlTextReaderConstName(pxReader);	

		if(pxcValue == NULL || pxcName == NULL)
			return false;
			
		if (strcmp(((char*)pxcName),"name") == 0)
		{
			if((char*)pxcValue != NULL)		
			{
				pbNodeName = new char[strlen((char*)pxcValue) + ALLOC_BUFFER];
				strcpy((char*)pbNodeName, (char*)pxcValue);
				//cout << "\nnodeName:" << pbNodeName << endl;
				//pobjPjtSettings->setPOWERLINK_IP((char*)pxcValue);
				//cout << "\nstPjtSettings->getPOWERLINK_IP():" << pobjPjtSettings->getPOWERLINK_IP() << endl;
			}
			else
			{
				#if defined DEBUG
					cout << "\ngetandCreateNode returning false" << endl;
				#endif
				return false;
			}
			
		}
		else if (strcmp(((char*)pxcName),"NodeId") == 0)
		{
			if((char*)pxcValue != NULL)		
			{
				iNodeID = atoi((char*)pxcValue);
				//cout << "\niNodeID:" << iNodeID << endl;
				//pobjPjtSettings->setPOWERLINK_IP((char*)pxcValue);
				//cout << "\nstPjtSettings->getPOWERLINK_IP():" << pobjPjtSettings->getPOWERLINK_IP() << endl;
			}
			else
			{
				#if defined DEBUG
					cout << "\ngetandCreateNode returning false" << endl;
				#endif
				return false;
			}
			
		}
		else if (strcmp(((char*)pxcName),"NodeType") == 0)
		{
			if(strcmp(((char*)pxcValue), "MN") == 0)		
			{
				enumNodeType = MN;
				#if defined DEBUG
				cout << "\nnodeType:" << enumNodeType << endl;				
				#endif
			}
			else if(strcmp(((char*)pxcValue), "CN") == 0)		
			{
				enumNodeType = CN;
				#if defined DEBUG
				cout << "\nnodeType:" << enumNodeType << endl;
				#endif
				//pobjPjtSettings->setPOWERLINK_IP((char*)pxcValue);
				//cout << "\nstPjtSettings->getPOWERLINK_IP():" << pobjPjtSettings->getPOWERLINK_IP() << endl;
			}
			else
			{
				#if defined DEBUG
					cout << "\ngetandCreateNode returning false" << endl;
				#endif
				return false;
			}
			
		}
		else if (strcmp(((char*)pxcName),"xdc") == 0)
		{
			if((char*)pxcValue != NULL)		
			{
				pbXdcPath = new char[strlen((char*)pxcValue) + 1];
				strcpy((char*)pbXdcPath, (char*)pxcValue);
				#if defined DEBUG
				cout << "\nxdcPath:" << pbXdcPath << endl;
				#endif
				//pobjPjtSettings->setPOWERLINK_IP((char*)pxcValue);
				//cout << "\nstPjtSettings->getPOWERLINK_IP():" << pobjPjtSettings->getPOWERLINK_IP() << endl;
			}
			else
			{
				#if defined DEBUG
					cout << "\ngetandCreateNode returning false" << endl;
				#endif
				pbXdcPath = NULL;
				//return false;
			}
		}
		else if (strcmp(((char*)pxcName),"ForceCycleFlag") == 0)
		{
			if(strcmp(((char*)pxcValue), "true") == 0)		
			{
				bForceCycleFlag = true;
				#if defined DEBUG
				cout << "\nForceCycleFlag:" << bForceCycleFlag << endl;				
				#endif
			}
			else if(strcmp(((char*)pxcValue), "false") == 0)		
			{
				bForceCycleFlag = false;
				#if defined DEBUG
				cout << "\nForceCycleFlag:" << bForceCycleFlag << endl;
				#endif
			}
			else
			{
				bForceCycleFlag = false;
				#if defined DEBUG
					cout << "\ngetandCreateNode returning false" << endl;
				#endif
			}
		}	
        else if(strcmp(((char*)pxcName),"ForceCycle") == 0)
        {
            if( (char*)pxcValue != NULL && strcmp((char*)pxcValue,"") != 0 )
            {
                pbForceCycleValue = new char[strlen((char*)pxcValue) + ALLOC_BUFFER];
                strcpy((char*)pbForceCycleValue, (char*)pxcValue);
            }
        }
        else if (strcmp(((char*)pxcName),"StationType") == 0)
        {
            if(strcmp(((char*)pxcValue), "Multiplexed") == 0)      
            {
                eStationType = MULTIPLEXED;
                #if defined DEBUG
                cout << "\nStationType:" << eStationType << endl;             
                #endif
            }
            else if(strcmp(((char*)pxcValue), "Chained") == 0)        
            {
                eStationType = CHAINED;
                #if defined DEBUG
                cout << "\nStationType:" << eStationType << endl;
                #endif
            }
            else if(strcmp(((char*)pxcValue), "Normal") == 0)        
            {
                eStationType = NORMAL;
                #if defined DEBUG
                cout << "\nStationType:" << eStationType << endl;
                #endif
            }
            else
            {
                #if defined DEBUG
                    cout << "\ngetandCreateNode returning false" << endl;
                #endif
            }
        }   
	}
	#if defined DEBUG
		cout << "\n\n\nCan Create Node\n\n"  << endl;
		cout << "\n enumNodeType:" << enumNodeType << endl;
	#endif
	if(enumNodeType == 1)
	{
		stErrorInfo = CreateNode(iNodeID, CN, pbNodeName);
	}
	else if(enumNodeType == 0)
	{
		stErrorInfo = CreateNode(iNodeID, MN, pbNodeName);
	}
	
	if(stErrorInfo.code != OCFM_ERR_SUCCESS)
		return false;

	CNode* objNode;		
	CNodeCollection *pobjNodeCollection;
	pobjNodeCollection = CNodeCollection::getNodeColObjectPointer();
	objNode = pobjNodeCollection->getNodePtr(enumNodeType,iNodeID);
	objNode->setForceCycleFlag(bForceCycleFlag);
	
	#if defined DEBUG
	cout << "\nCreateNode - stErrorInfo.code:" << stErrorInfo.code << endl;
	#endif
	
	pbFileName = new char[(strlen(pbPjtPath) + strlen(pbXdcPath) + 5)];
	sprintf(pbFileName, "%s/%s", pbPjtPath, pbXdcPath);
	if(enumNodeType == 1)
		stErrorInfo = parseFile(pbFileName, iNodeID, CN);
	else if(enumNodeType == 0)
		stErrorInfo = parseFile(pbFileName, iNodeID, MN);

	if(stErrorInfo.code != OCFM_ERR_SUCCESS)
		return false;

    if(enumNodeType == 1 && pbForceCycleValue != NULL)
        objNode->setForcedCycle(pbForceCycleValue);

    objNode->setStationType(eStationType);

//    copyDefToAct(iNodeID, enumNodeType);
	delete [] pbFileName;
	delete [] pbNodeName;
	return true;
}
/****************************************************************************************************
* Function Name: saveProjectXML
* Description: Saves the project details into the Project location
* Return value: bool[True/False]
****************************************************************************************************/
bool saveProjectXML(char* pbProjectPath, char* pbProjectName)
{
	
CPjtSettings* pobjPjtSettings;
pobjPjtSettings = CPjtSettings::getPjtSettingsPtr();
//pobjPjtSettings = new CPjtSettings();
//cout << " \nPrjt settings Address:" << pobjPjtSettings;

xmlTextWriterPtr pxtwWriter;
xmlDocPtr pxdDoc;
INT32 iBytesWritten;
char* pbFileName;
//pbFileName = new char[strlen(pbProjectPath) + strlen(pbProjectName) + strlen(pbProjectName) + 10];
pbFileName = new char[MAX_FILE_PATH_SIZE];
//pbFileName = new char[(strlen(pbProjectPath) + strlen(pbProjectName) + strlen(pbProjectName) + ALLOC_BUFFER)];
//pbFileName = new char[100];
//INT32 iLength = strlen(pbProjectPath);
//cout << "length " << iLength;

#if defined(_WIN32) && defined(_MSC_VER)
{

	sprintf(pbFileName, "%s\\%s\\%s.oct", pbProjectPath, pbProjectName, pbProjectName);

}
#else
{
	sprintf(pbFileName, "%s/%s/%s.oct", pbProjectPath, pbProjectName, pbProjectName);
}
#endif

/* Create a new XmlWriter for DOM, with no compression. */
pxtwWriter = xmlNewTextWriterDoc(&pxdDoc, 0);
if (pxtwWriter == NULL) 
{
	printf("testXmlwriterDoc: Error creating the xml pxtwWriter\n");
	//return;
	ocfmException objException;// = new ocfmException;
	objException.ocfm_Excpetion(OCFM_ERR_CREATE_XML_WRITER_FAILED);
	throw &objException;
}
/* Start the document with the xml default for the version,
* encoding UTF-8 and the default for the standalone
* declaration. */
iBytesWritten = xmlTextWriterStartDocument(pxtwWriter, NULL, MY_ENCODING, NULL);
if (iBytesWritten < 0) 
{
	printf("testXmlwriterDoc: Error at xmlTextWriterStartDocument\n");
	ocfmException objException;// = new ocfmException;
	objException.ocfm_Excpetion(OCFM_ERR_XML_START_DOC_FAILED);
	throw &objException;
}	
iBytesWritten = xmlTextWriterWriteComment(pxtwWriter,BAD_CAST "This file was autogenerated by openCONFIGURATOR");
xmlTextWriterSetIndent(pxtwWriter, 1);
// Start openCONFIGURATOR Tag		
iBytesWritten = xmlTextWriterStartElement(pxtwWriter, BAD_CAST "openCONFIGURATOR");
if (iBytesWritten < 0) 
{
	printf("testXmlwriterMemory: Error at xmlTextWriterStartElement\n");
	ocfmException objException;// = new ocfmException;
	objException.ocfm_Excpetion(OCFM_ERR_XML_WRITER_START_ELT_FAILED);
	throw &objException;
}
	iBytesWritten = xmlTextWriterWriteAttribute(pxtwWriter, BAD_CAST "Version", BAD_CAST TOOL_VERSION);
	
xmlTextWriterSetIndent(pxtwWriter, 1);
// Start profile Tag
iBytesWritten = xmlTextWriterStartElement(pxtwWriter, BAD_CAST "profile");
if (iBytesWritten < 0) 
{
	printf("testXmlwriterMemory: Error at xmlTextWriterStartElement\n");
	ocfmException objException;// = new ocfmException;
	objException.ocfm_Excpetion(OCFM_ERR_XML_WRITER_START_ELT_FAILED);
	throw &objException;
}

	xmlTextWriterSetIndent(pxtwWriter, 1);
	// Start Auto Tag		
	iBytesWritten = xmlTextWriterStartElement(pxtwWriter, BAD_CAST "Auto");
	if (iBytesWritten < 0) 
	{
		printf("testXmlwriterMemory: Error at xmlTextWriterStartElement\n");
		ocfmException objException;// = new ocfmException;
		objException.ocfm_Excpetion(OCFM_ERR_XML_WRITER_START_ELT_FAILED);
		throw &objException;
	}

	//pobjPjtSettings->setTestVar(5);

	//cout << "\nstPjtSettings->getTestVar():" << pobjPjtSettings->getTestVar() << endl;

	//pobjPjtSettings->setGenerateAttr(YES_AG);
	//if(0 != strlen(pobjPjtSettings->getGenerateAttr()))
	{
		//cout << "\n\n\nstPjtSettings->getGenerateAttr():" << pobjPjtSettings->getGenerateAttr() << endl;
		if(pobjPjtSettings->getGenerateAttr() == AUTOGENERATE)
		{
			iBytesWritten = xmlTextWriterWriteAttribute(pxtwWriter, BAD_CAST "Generate", BAD_CAST "NO");
		}
		else if(pobjPjtSettings->getGenerateAttr() == AUTOSAVE)
		{
			iBytesWritten = xmlTextWriterWriteAttribute(pxtwWriter, BAD_CAST "Generate", BAD_CAST "YES");
		}
	}
	
	//cout << "\n\n\nstPjtSettings->getSaveAttr():" << pobjPjtSettings->getSaveAttr() << endl;
	if(pobjPjtSettings->getSaveAttr() == YES_AS)
		iBytesWritten = xmlTextWriterWriteAttribute(pxtwWriter, BAD_CAST "Save", BAD_CAST "YES");
	else if(pobjPjtSettings->getSaveAttr() == PROMPT_AS)
		iBytesWritten = xmlTextWriterWriteAttribute(pxtwWriter, BAD_CAST "Save", BAD_CAST "PROMPT");
	else if(pobjPjtSettings->getSaveAttr() == DISCARD_AS)
		iBytesWritten = xmlTextWriterWriteAttribute(pxtwWriter, BAD_CAST "Save", BAD_CAST "DISCARD");

	if(pobjPjtSettings->getViewMode() == SIMPLE)
	{
		iBytesWritten = xmlTextWriterWriteAttribute(pxtwWriter, BAD_CAST "View", BAD_CAST "General");
	}
	else if(pobjPjtSettings->getViewMode()== EXPERT)
	{
		iBytesWritten = xmlTextWriterWriteAttribute(pxtwWriter, BAD_CAST "View", BAD_CAST "Advanced");
	}

	if(pobjPjtSettings->getExpertViewSelectedFlag() == true)
	{
		iBytesWritten = xmlTextWriterWriteAttribute(pxtwWriter, BAD_CAST "ExpertViewSelected", BAD_CAST "true");
	}
	else if(pobjPjtSettings->getExpertViewSelectedFlag()== false)
	{
		iBytesWritten = xmlTextWriterWriteAttribute(pxtwWriter, BAD_CAST "ExpertViewSelected", BAD_CAST "false");
	}


	//cout << "\n1" << endl;
	// End Auto Tag
	iBytesWritten = xmlTextWriterEndElement(pxtwWriter);
	if (iBytesWritten < 0)
	{		
		printf("testXmlwriterDoc: Error at xmlTextWriterEndElement\n");
		ocfmException objException;// = new ocfmException;
		objException.ocfm_Excpetion(OCFM_ERR_XML_WRITER_END_ELT_FAILED);
		throw &objException;
	}
	
	xmlTextWriterSetIndent(pxtwWriter, 1);
	// Start Communication Tag		
	iBytesWritten = xmlTextWriterStartElement(pxtwWriter, BAD_CAST "Communication");
	if (iBytesWritten < 0) 
	{
		printf("testXmlwriterMemory: Error at xmlTextWriterStartElement\n");
		ocfmException objException;// = new ocfmException;
		objException.ocfm_Excpetion(OCFM_ERR_XML_WRITER_START_ELT_FAILED);
		throw &objException;
	}
	//cout << "\n3" << endl;
	/*
	Connection between openCONFIGURATOR and openPOWERLINK via socket is not implented in this version.
	*/
	iBytesWritten = xmlTextWriterWriteAttribute(pxtwWriter, BAD_CAST "IP", BAD_CAST "0.0.0.0");
		
	// End Communication Tag
	iBytesWritten = xmlTextWriterEndElement(pxtwWriter);
	if (iBytesWritten < 0)
	{
		printf("testXmlwriterDoc: Error at xmlTextWriterEndElement\n");
		ocfmException objException;// = new ocfmException;
		objException.ocfm_Excpetion(OCFM_ERR_XML_WRITER_END_ELT_FAILED);
		throw &objException;
	}

// End profile Tag
iBytesWritten = xmlTextWriterEndElement(pxtwWriter);
if (iBytesWritten < 0)
{
	printf("testXmlwriterDoc: Error at xmlTextWriterEndElement\n");
	ocfmException objException;// = new ocfmException;
	objException.ocfm_Excpetion(OCFM_ERR_XML_WRITER_END_ELT_FAILED);
	throw &objException;
}
//cout << "\n4" << endl;
xmlTextWriterSetIndent(pxtwWriter, 1);
// Start NodeCollection Tag
iBytesWritten = xmlTextWriterStartElement(pxtwWriter, BAD_CAST "NodeCollection");
if (iBytesWritten < 0) 
{
	printf("testXmlwriterMemory: Error at xmlTextWriterStartElement\n");
	ocfmException objException;// = new ocfmException;
	objException.ocfm_Excpetion(OCFM_ERR_XML_WRITER_START_ELT_FAILED);
	throw &objException;
}
//cout << "\n5" << endl;
		CNode objNode;		
		CNodeCollection *pobjNodeCollection = NULL;
		//CIndexCollection *pobjIndexCollection = NULL;
		CIndex objIndex;
		//CIndex* pobjIndexPtr =  NULL;
		//cout << "\n5_0" << endl;
		//Parameter* pobjParameter = NULL;
		//CApplicationProcess* pobjAppProc = NULL;
		//INT32 iIndexPos = 0;
		//cout << "\n5_1" << endl;
		objIndex.setNodeID(objNode.getNodeId());
		//cout << "\n5_2" << endl;
		pobjNodeCollection = CNodeCollection::getNodeColObjectPointer();
		//cout << "\n5_3" << endl;
		//objNode = pobjNodeCollection->getNode(enumNodeType, iNodeID);
//cout << "\n6" << endl;
		//pobjIndexCollection = objNode.getIndexCollection();
		


	for(INT32 nodeCount = 0; nodeCount < pobjNodeCollection->getNumberOfNodes(); nodeCount++)
	{
		CNode* pobjNode;

		pobjNode = pobjNodeCollection->getNodebyColIndex(nodeCount);
		
		xmlTextWriterSetIndent(pxtwWriter, 1);
		
		
		// Start Node Tag		
		iBytesWritten = xmlTextWriterStartElement(pxtwWriter, BAD_CAST "Node");

		if (iBytesWritten < 0) 
		{
			printf("testXmlwriterMemory: Error at xmlTextWriterStartElement\n");
			ocfmException objException;// = new ocfmException;
			objException.ocfm_Excpetion(OCFM_ERR_XML_WRITER_START_ELT_FAILED);
			throw &objException;
		}

		char* pbTempNodeName;
		//cout << "before nodename" << endl;
		//printf("\n pobjNode pxcName %s ",pobjNode->getNodeName());
		//pbTempNodeName = new char[(strlen(pobjNode->getNodeName()) + 10)];
		pbTempNodeName = new char[50];
		//cout << "after indeent" << endl;
		strcpy(pbTempNodeName, (char*) pobjNode->getNodeName());
		iBytesWritten = xmlTextWriterWriteAttribute(pxtwWriter, BAD_CAST "name", BAD_CAST pbTempNodeName);
		
		char* pbTempNodeID;
		pbTempNodeID = new char[20];
		
		_IntToAscii(pobjNode->getNodeId(), pbTempNodeID, 0);

		iBytesWritten = xmlTextWriterWriteAttribute(pxtwWriter, BAD_CAST "NodeId", BAD_CAST pbTempNodeID);
		
		ENodeType tmp_NodeType;
		tmp_NodeType = pobjNode->getNodeType();
		
		if(tmp_NodeType == 0)
			iBytesWritten = xmlTextWriterWriteAttribute(pxtwWriter, BAD_CAST "NodeType", BAD_CAST "MN");
		if(tmp_NodeType == 1)
			iBytesWritten = xmlTextWriterWriteAttribute(pxtwWriter, BAD_CAST "NodeType", BAD_CAST "CN");

		char* pbTempXdcName;
		pbTempXdcName = new char[20];
		
		sprintf(pbTempXdcName, "%s/%s.octx", "octx", pbTempNodeID);		
		iBytesWritten = xmlTextWriterWriteAttribute(pxtwWriter, BAD_CAST "xdc", BAD_CAST pbTempXdcName);
		if(pobjNode->getForceCycleFlag() == true)
		{
			iBytesWritten = xmlTextWriterWriteAttribute(pxtwWriter, BAD_CAST "ForceCycleFlag", BAD_CAST "true");
		}
		else if(pobjNode->getForceCycleFlag() == false)
		{
			iBytesWritten = xmlTextWriterWriteAttribute(pxtwWriter, BAD_CAST "ForceCycleFlag", BAD_CAST "false");
		}

        if(pobjNode->getForcedCycle() != NULL && strcmp(pobjNode->getForcedCycle(),""))
        {
            iBytesWritten = xmlTextWriterWriteAttribute(pxtwWriter, BAD_CAST "ForceCycle", BAD_CAST pobjNode->getForcedCycle());
        }

        if(pobjNode->getStationType() == NORMAL)
        {
            iBytesWritten = xmlTextWriterWriteAttribute(pxtwWriter, BAD_CAST "StationType", BAD_CAST "Normal");
        }
        else if(pobjNode->getStationType() == MULTIPLEXED)
        {
            iBytesWritten = xmlTextWriterWriteAttribute(pxtwWriter, BAD_CAST "StationType", BAD_CAST "Multiplexed");
        }
        else if(pobjNode->getStationType() == CHAINED)
        {
            iBytesWritten = xmlTextWriterWriteAttribute(pxtwWriter, BAD_CAST "StationType", BAD_CAST "Chained");
        }
		// End Node Tag
		iBytesWritten = xmlTextWriterEndElement(pxtwWriter);
		if (iBytesWritten < 0)
		{
			printf("testXmlwriterDoc: Error at xmlTextWriterEndElement\n");
			ocfmException objException;// = new ocfmException;
			objException.ocfm_Excpetion(OCFM_ERR_XML_WRITER_END_ELT_FAILED);
			throw &objException;
		}
	
		delete [] pbTempNodeName;
		delete [] pbTempNodeID;
		delete [] pbTempXdcName;
	}

// End NodeCollection Tag
iBytesWritten = xmlTextWriterEndElement(pxtwWriter);
if (iBytesWritten < 0)
{
	printf("testXmlwriterDoc: Error at xmlTextWriterEndElement\n");
	ocfmException objException;// = new ocfmException;
	objException.ocfm_Excpetion(OCFM_ERR_XML_WRITER_END_ELT_FAILED);
	throw &objException;
}

// End openCONFIGURATOR Tag
iBytesWritten = xmlTextWriterEndElement(pxtwWriter);
if (iBytesWritten < 0)
{
	printf("testXmlwriterDoc: Error at xmlTextWriterEndElement\n");
	ocfmException objException;// = new ocfmException;
	objException.ocfm_Excpetion(OCFM_ERR_XML_WRITER_END_ELT_FAILED);
	throw &objException;
}

iBytesWritten = xmlTextWriterEndDocument(pxtwWriter);
if (iBytesWritten < 0) 
{
	printf("testXmlwriterDoc: Error at xmlTextWriterEndDocument\n");
	ocfmException objException;// = new ocfmException;
	objException.ocfm_Excpetion(OCFM_ERR_XML_END_DOC_FAILED);
	throw &objException;
}

xmlFreeTextWriter(pxtwWriter);

xmlSaveFileEnc(pbFileName, pxdDoc, MY_ENCODING);

xmlFreeDoc(pxdDoc);	

delete [] pbFileName;
return true;
}

/****************************************************************************************************
* Function Name: CreateMNPDOVar
* Description: 
* Return value: void
****************************************************************************************************/
void CreateMNPDOVar(INT32 iOffset, INT32 iDataSize, IEC_Datatype enumDataType, EPDOType enumPdoType, CNode *pobjNode)
{
		MNPdoVariable objPDOvar;
		CNodeCollection* pobjNodeCol;
		PIObject objpi;
		
		pobjNodeCol =  CNodeCollection::getNodeColObjectPointer();		
		
		/* Assign the PDO type*/
		objPDOvar.pdoType = enumPdoType;
		objPDOvar.DataSize = iDataSize;
	/* Assign Index*/
			objPDOvar.Index = new char[4 + ALLOC_BUFFER];
			objPDOvar.SubIndex = new char[SUBINDEX_LEN + ALLOC_BUFFER];
			switch(enumDataType)
			{
			
				case USINT:
				case BITSTRING :						
					if(enumPdoType == PDO_TPDO)
					{
						objpi =  getPIAddress(UNSIGNED8, INPUT, iOffset, iDataSize);
					
					}
					else if(enumPdoType == PDO_RPDO)
					{
						objpi =  getPIAddress(UNSIGNED8, OUTPUT, iOffset, iDataSize);
						//strcpy(objPDOvar.Index, getPIAddress(UNSIGNED8, OUTPUT, Offset, dataSize));	
					}
					
					
					break;					
				case SINT  :						
					if(enumPdoType == PDO_TPDO)
					{
							objpi =  getPIAddress(INTEGER8, INPUT, iOffset, iDataSize);
						//strcpy(objPDOvar.Index, getPIAddress(INTEGER8, INPUT, Offset, dataSize));	
					}
					
					else if(enumPdoType == PDO_RPDO)
					{
							objpi =  getPIAddress(INTEGER8, OUTPUT, iOffset, iDataSize);
						//strcpy(objPDOvar.Index, getPIAddress(INTEGER8, OUTPUT, Offset, dataSize));	
					}
					break;	
				case UINT :											
					if(enumPdoType == PDO_TPDO)
					{
							objpi =  getPIAddress(UNSIGNED16, INPUT, iOffset, iDataSize);
						//strcpy(objPDOvar.Index, getPIAddress(UNSIGNED16, INPUT , Offset, dataSize));	
					}
					else if(enumPdoType == PDO_RPDO)
					{
						objpi =  getPIAddress(UNSIGNED16, OUTPUT, iOffset, iDataSize);
						//strcpy(objPDOvar.Index, getPIAddress(UNSIGNED16, OUTPUT, Offset, dataSize));	
					}
					break;									
				case INT :											
					if(enumPdoType == PDO_TPDO)
					{
						objpi =  getPIAddress(INTEGER16, INPUT, iOffset, iDataSize);
						//strcpy(objPDOvar.Index, getPIAddress(INTEGER16, INPUT, Offset, dataSize));	
					}
					else if(enumPdoType == PDO_RPDO)
					{
						objpi =  getPIAddress(INTEGER16, OUTPUT, iOffset, iDataSize);
						//strcpy(objPDOvar.Index, getPIAddress(INTEGER16, OUTPUT, Offset, dataSize));	
					}
					break;
				case UDINT:											
					if(enumPdoType == PDO_TPDO)
					{
						objpi =  getPIAddress(UNSIGNED32, INPUT, iOffset, iDataSize);
						//strcpy(objPDOvar.Index, getPIAddress(UNSIGNED32, INPUT, Offset, dataSize));	
					}
					else if(enumPdoType == PDO_RPDO)
					{
						objpi =  getPIAddress(UNSIGNED32, OUTPUT, iOffset, iDataSize);
						//strcpy(objPDOvar.Index, getPIAddress(UNSIGNED32, OUTPUT, Offset, dataSize));	
					}
					break;		
				case DINT:
				case REAL:											
					if(enumPdoType == PDO_TPDO)
					{
						objpi =  getPIAddress(INTEGER32, INPUT, iOffset, iDataSize);
						//strcpy(objPDOvar.Index, getPIAddress(INTEGER32, INPUT, Offset, dataSize));	
					}
					else if(enumPdoType == PDO_RPDO)
					{
						objpi =  getPIAddress(INTEGER32, OUTPUT, iOffset, iDataSize);
						//strcpy(objPDOvar.Index, getPIAddress(INTEGER32, OUTPUT, Offset, dataSize));		
					}
					break;																			
				case LINT:
				case LREAL:
					/*if(enumPdoType == PDO_TPDO)
					strcpy(objPDOvar.Index, "A071");	
					
					else if(enumPdoType == PDO_RPDO)
					strcpy(objPDOvar.Index, "A061");*/	
					break;
				// Handled all values 
				case BOOL:
					cout << "Data type BOOL not handled" << endl;
					break;
				case BYTE:
					cout << "Data type BYTE not handled" << endl;
					break;
				case _CHAR:
					cout << "Data type _CHAR not handled" << endl;
					break;
				case DWORD:
					cout << "Data type DWORD not handled" << endl;
					break;
				case LWORD:
					cout << "Data type LWORD not handled" << endl;
					break;
				case ULINT:
					cout << "Data type ULINT not handled" << endl;
					break;
				case STRING:
					cout << "Data type STRING not handled" << endl;
					break;
				case WSTRING:
					cout << "Data type WSTRING not handled" << endl;
					break;
			}
	
			strcpy(objPDOvar.Index,objpi.Index);	
			strcpy(objPDOvar.SubIndex, objpi.SubIndex);		
		//printf("\n objPDOvar.Index%s", objPDOvar.Index);
	
	
	/* Assign SubIndex*/							
		/*INT32 iSIdx;
		INT32 iTempDataSize = iDataSize/8;
		iSIdx 		    = (iOffset)/iTempDataSize + 1;
		objPDOvar.SubIndex  = new char[2 + ALLOC_BUFFER];
		objPDOvar.SubIndex  =  _IntToAscii(iSIdx, objPDOvar.SubIndex, 16);
		objPDOvar.SubIndex  = padLeft(objPDOvar.SubIndex, '0', 2);
	*/					
		/* Assign the value*/
		objPDOvar.Value = new char[10 + ALLOC_BUFFER];
		char* pbPadBuff = new char[5 + ALLOC_BUFFER];		
		pbPadBuff 	= _IntToAscii(iDataSize, pbPadBuff, 16);
		pbPadBuff	= padLeft(pbPadBuff, '0', 4);
		strcpy(objPDOvar.Value, "0x");
		strcat(objPDOvar.Value, pbPadBuff);
		/* Set the Offset*/
		strcat(objPDOvar.Value , "0000");
		/* Set the Reserved*/
		strcat(objPDOvar.Value , "00");
		/* Set the subIndex*/
//		strcat(objPDOvar.Value , objPDOvar.SubIndex);
		/* Set the Reserved*/
	//	strcat(objPDOvar.Value , objPDOvar.Index);
	
		
		pobjNode->addMNPDOvar(objPDOvar, enumPdoType);
		
		
}
/**************************************************************************************************
* Function Name: GetProjectSettings
* Description: Gets the Project Settings of the tool
* Return value: ocfmRetCode
****************************************************************************************************/

ocfmRetCode GetProjectSettings(EAutoGenerate *enumAutoGen, EAutoSave *enumAutoSave, EViewMode *enumviewMode, bool* bExpertViewAlreadySet)
{
 
		ocfmRetCode stRetInfo;
		CPjtSettings* pobjPrjtSettings;
		pobjPrjtSettings =  CPjtSettings::getPjtSettingsPtr();
		
		try
		{
			if(pobjPrjtSettings == NULL)
			{
				ocfmException objException;// = new ocfmException;
				objException.ocfm_Excpetion(OCFM_ERR_PROJECT_SETTINGS);
				throw &objException;
			}
			
			*enumAutoGen = pobjPrjtSettings->getGenerateAttr();
			*enumAutoSave = pobjPrjtSettings->getSaveAttr();
			*enumviewMode = pobjPrjtSettings->getViewMode();
			*bExpertViewAlreadySet = pobjPrjtSettings->getExpertViewSelectedFlag();

			stRetInfo.code =  OCFM_ERR_SUCCESS;
			return stRetInfo;
		}
		catch(ocfmException& objocfmException)
		{
			return objocfmException._ocfmRetCode;
		}
}


/****************************************************************************************************
* Function Name: SetProjectSettings
* Description: Sets the Project Settings of the tool
* Return value: ocfmRetCode
****************************************************************************************************/

ocfmRetCode SetProjectSettings(EAutoGenerate enumAutoGen, EAutoSave enumAutoSave, EViewMode enumViewMode,bool bExpertViewAlreadySet)
{
 
		ocfmRetCode stRetInfo;
		CPjtSettings* pobjPrjtSettings;
		pobjPrjtSettings =  CPjtSettings::getPjtSettingsPtr();
		
		try
		{
			if(pobjPrjtSettings == NULL)
			{
				ocfmException objException;// = new ocfmException;
				objException.ocfm_Excpetion(OCFM_ERR_PROJECT_SETTINGS);
				throw &objException;
			}
			
			//printf("\n autogenerate %d",enumAutoGen);
			pobjPrjtSettings->setGenerateAttr(enumAutoGen);
			pobjPrjtSettings->setSaveAttr(enumAutoSave);
			pobjPrjtSettings->setViewMode(enumViewMode);
			pobjPrjtSettings->setExpertViewSelectedFlag(bExpertViewAlreadySet);
			stRetInfo.code =  OCFM_ERR_SUCCESS;
			return stRetInfo;
		}
		catch(ocfmException& objocfmException)
		{
			return objocfmException._ocfmRetCode;
		}
}
/****************************************************************************************************
* Function Name: UpdateNumberOfEnteriesSIdx
* Description: Updtade subindex "00"/NumberofEnteries
* Return value: void
****************************************************************************************************/

void	UpdateNumberOfEnteriesSIdx(CIndex *pobjIndex, ENodeType enumNodeType)
{
	CSubIndex *pobjSIdx;
	INT32 iTotalSIdxs;
	char pbAsciBuff[10];
	
	pobjSIdx = pobjIndex->getSubIndexbyIndexValue((char*)"00");
	
	/* subindexes excluding "00"*/
	iTotalSIdxs = pobjIndex->getNumberofSubIndexes() - 1;
	
	/*if(pobjSIdx ==NULL)	
	{
		AddSubIndex(ppobjIndex->getNodeID(), enumNodeType, (char *)objIndex->getIndexValue(), "00");
		pobjSIdx = pobjIndex->getSubIndexbyIndexValue("00");
	}*/
	
	if(pobjSIdx!=NULL)
    {
        char* buffer = new char[10];
        strcpy(buffer, "0x");
        strcat(buffer, _IntToAscii(iTotalSIdxs, pbAsciBuff, 16));
	    pobjSIdx->setActualValue(buffer); 
        delete [] buffer;
    }
}

void AuotgenerateOtherIndexs(CNode* pobjNode)
{

			ocfmRetCode retCode;
			char* pbMNIndex = new char[INDEX_LEN + ALLOC_BUFFER];		
			//char pbMNIndex[INDEX_LEN + ALLOC_BUFFER];		
			char* pbSidx =  new char[SUBINDEX_LEN + ALLOC_BUFFER];
			CIndex* pobjIndex;
			//CNode* pobjNode;
			//char pbSidx[SUBINDEX_LEN + ALLOC_BUFFER];
			CIndexCollection* pobjIdxCol;
			INT32 iConfigDate;
			INT32 iConfigTime;
			
			pobjIdxCol = pobjNode->getIndexCollection();
			/* Add 1006*/
				strcpy(pbMNIndex, "1006");
						#if defined DEBUG	
					cout << "string copied" << endl;
				
				#endif
				retCode = AddIndex(MN_NODEID, MN, pbMNIndex);
					#if defined DEBUG	
					cout << "retcode" << retCode.code<<endl;
					cout<< "1006 added"<<endl;
				#endif
				
				/* Set 5ms value*/	
				if(retCode.code ==  OCFM_ERR_INDEX_ALREADY_EXISTS)
				{
					pobjIndex = pobjIdxCol->getIndexbyIndexValue(pbMNIndex);
					if(pobjIndex->getActualValue() ==NULL)
					SetIndexAttributes(MN_NODEID, MN, pbMNIndex, (char*)"50000",(char*)"NMT_CycleLen_U32", TRUE);				
				}
				else
				{
						SetIndexAttributes(MN_NODEID, MN, pbMNIndex, (char*)"50000",(char*)"NMT_CycleLen_U32", TRUE);				
				}
			/* Add 1020*/
				strcpy(pbMNIndex, "1020");
				retCode = AddIndex(MN_NODEID, MN, pbMNIndex);
				
						/* Add 1300*/
				strcpy(pbMNIndex, "1300");
				retCode = AddIndex(MN_NODEID, MN, pbMNIndex);				
				SetIndexAttributes(MN_NODEID, MN, pbMNIndex, (char*)"5000",(char*)"SDO_SequLayerTimeout_U32", TRUE);

				/* Add 1C02*/
					strcpy(pbMNIndex, "1C02");
					retCode = AddIndex(MN_NODEID, MN, pbMNIndex);
					if(retCode.code == OCFM_ERR_SUCCESS || retCode.code == OCFM_ERR_INDEX_ALREADY_EXISTS)
					{		
							pobjIndex = pobjIdxCol->getIndexbyIndexValue(pbMNIndex);
							/* $:set Flag to true*/
						pobjIndex->setFlagIfIncludedCdc(TRUE);
						/* Set subindex value 40 or 0000028 */
						strcpy(pbSidx, "00");
						SetSIdxValue(pbMNIndex, pbSidx, (char*)"3", pobjIdxCol, pobjNode->getNodeId(), MN, false);
								#if defined DEBUG	
						cout<< "1c02 subidex added"<<endl;
					#endif
						
						strcpy(pbSidx, "01");
						SetSIdxValue(pbMNIndex, pbSidx, (char*)"40", pobjIdxCol, pobjNode->getNodeId(), MN,  false);
								#if defined DEBUG	
						cout<< "1c02 subidex 01 added"<<endl;
					#endif
						strcpy(pbSidx, "02");
						SetSIdxValue(pbMNIndex, pbSidx, (char*)"40", pobjIdxCol, pobjNode->getNodeId(), MN, false);
						
						strcpy(pbSidx, "03");
						SetSIdxValue(pbMNIndex, pbSidx, (char*)"40", pobjIdxCol, pobjNode->getNodeId(), MN, false);
	
					}
					
		/* Add 1C09*/
					strcpy(pbMNIndex, "1C09");
					retCode = AddIndex(MN_NODEID, MN, pbMNIndex);
					if(retCode.code == OCFM_ERR_SUCCESS || retCode.code == OCFM_ERR_INDEX_ALREADY_EXISTS)
					{				
						pobjIndex = pobjIdxCol->getIndexbyIndexValue(pbMNIndex);
								/* $:set Flag to true*/
						pobjIndex->setFlagIfIncludedCdc(TRUE);
			
						AddForEachSIdx(pbMNIndex, pobjIdxCol, pobjNode->getNodeId(), (char*)"40",false);					
					
					}
					
							/* Add 1F26*/
					strcpy(pbMNIndex, "1F26");
					retCode = AddIndex(MN_NODEID, MN, pbMNIndex);
					if(retCode.code == OCFM_ERR_SUCCESS || retCode.code == OCFM_ERR_INDEX_ALREADY_EXISTS)
					{				
						pobjIndex = pobjIdxCol->getIndexbyIndexValue(pbMNIndex);
								/* $:set Flag to true*/
						pobjIndex->setFlagIfIncludedCdc(TRUE);
			
						char* Val = new char[8];
						iConfigDate = getConfigDate();
						Val = _IntToAscii(iConfigDate, Val, 10);
						//hexVal = padLeft(hexVal, '0' , 8);
						AddForEachSIdx(pbMNIndex, pobjIdxCol, pobjNode->getNodeId(), Val, false);		
											
										//AddForEachSIdx(MNIndex, pobjIdxCol, objNode->getNodeId(), hexVal, false);		
	
					}
					
							#if defined DEBUG	
						cout<< "1F26 subidex added"<<endl;
					#endif
							/* Add 1F27*/
					strcpy(pbMNIndex, "1F27");
					retCode = AddIndex(MN_NODEID, MN, pbMNIndex);
					if(retCode.code == OCFM_ERR_SUCCESS || retCode.code == OCFM_ERR_INDEX_ALREADY_EXISTS)
					{				
						pobjIndex = pobjIdxCol->getIndexbyIndexValue(pbMNIndex);		
								/* $:set Flag to true*/
						pobjIndex->setFlagIfIncludedCdc(TRUE);
			
						char* Val = new char[50];
						iConfigTime = getConfigTime();
						//cout << "\n\n\nConfigTime:" << iConfigTime << endl;																		
						Val = _IntToAscii(iConfigTime,Val, 10);
						//Val = padLeft(hexVal, '0' , 8);

						AddForEachSIdx(pbMNIndex, pobjIdxCol, pobjNode->getNodeId(), Val, false);										
					}
					
					
					/* Add 1F84*/
					strcpy(pbMNIndex, "1F84");
					retCode = AddIndex(MN_NODEID, MN, pbMNIndex);
					if(retCode.code == OCFM_ERR_SUCCESS || retCode.code == OCFM_ERR_INDEX_ALREADY_EXISTS)
					{				
						pobjIndex = pobjIdxCol->getIndexbyIndexValue(pbMNIndex);
								/* $:set Flag to true*/
						pobjIndex->setFlagIfIncludedCdc(TRUE);
						
						AddForEachSIdx(pbMNIndex, pobjIdxCol, pobjNode->getNodeId(), (char*)"", true);			
			
					}
								/* Add 1F8B*/
					strcpy(pbMNIndex, "1F8B");
					retCode = AddIndex(MN_NODEID, MN, pbMNIndex);
					if(retCode.code == OCFM_ERR_SUCCESS || retCode.code == OCFM_ERR_INDEX_ALREADY_EXISTS)
					{				
						pobjIndex = pobjIdxCol->getIndexbyIndexValue(pbMNIndex);
								/* $:set Flag to true*/
						pobjIndex->setFlagIfIncludedCdc(TRUE);
			
						AddForEachSIdx(pbMNIndex, pobjIdxCol, pobjNode->getNodeId(), (char*)"1490", false);			
											
					
					}
							/* Add 1F8D*/
					strcpy(pbMNIndex, "1F8D");
					retCode = AddIndex(MN_NODEID, MN, pbMNIndex);
					if(retCode.code == OCFM_ERR_SUCCESS || retCode.code == OCFM_ERR_INDEX_ALREADY_EXISTS)
					{				
						pobjIndex = pobjIdxCol->getIndexbyIndexValue(pbMNIndex);
								/* $:set Flag to true*/
						pobjIndex->setFlagIfIncludedCdc(TRUE);
			
						AddForEachSIdx(pbMNIndex, pobjIdxCol, pobjNode->getNodeId(), (char*)"1490", false);			
											
					
					}
							/* Add 1F92*/
					strcpy(pbMNIndex, "1F92");
					retCode = AddIndex(MN_NODEID, MN, pbMNIndex);
					if(retCode.code == OCFM_ERR_SUCCESS)
					{				
						pobjIndex = pobjIdxCol->getIndexbyIndexValue(pbMNIndex);
								/* $:set Flag to true*/
						pobjIndex->setFlagIfIncludedCdc(TRUE);
			
						AddForEachSIdx(pbMNIndex, pobjIdxCol, pobjNode->getNodeId(), (char*)"2000000",false);			
											
					
					}
					else if (retCode.code == OCFM_ERR_INDEX_ALREADY_EXISTS)
					{
						/* $:set Flag to true*/
						pobjIndex->setFlagIfIncludedCdc(TRUE);
					}
					
					
							/* Add 1F98*/
					strcpy(pbMNIndex, "1F98");
					
					retCode = AddIndex(MN_NODEID, MN, pbMNIndex);
					if(retCode.code == OCFM_ERR_SUCCESS)
					{				
					
						pobjIndex = pobjIdxCol->getIndexbyIndexValue(pbMNIndex);
								/* $:set Flag to true*/
						pobjIndex->setFlagIfIncludedCdc(TRUE);
						
						strcpy(pbSidx, "05");
						SetSIdxValue(pbMNIndex, pbSidx, abC_DLL_ISOCHR_MAX_PAYL, pobjIdxCol, pobjNode->getNodeId(), MN, false);
						
						/*strcpy(Sidx, "00");
						SetSIdxValue(pbMNIndex, pbSidx, "", pobjIdxCol, pobjNode->getNodeId(),true);
										
						strcpy(pbSidx, "01");
						SetSIdxValue(pbMNIndex, pbSidx, abC_DLL_ISOCHR_MAX_PAYL, pobjIdxCol, pobjNode->getNodeId(), false);
					
						strcpy(pbSidx, "02");
						SetSIdxValue(pbMNIndex, pbSidx, abC_DLL_ISOCHR_MAX_PAYL, pobjIdxCol, pobjNode->getNodeId(), false);
						
						
						strcpy(pbSidx, "07");
						SetSIdxValue(pbMNIndex, pbSidx, "", pobjIdxCol, pobjNode->getNodeId(), true);
						
						strcpy(pbSidx, "08");
						SetSIdxValue(pbMNIndex, pbSidx, abC_DLL_MIN_ASYNC_MTU, pobjIdxCol, pobjNode->getNodeId(), false);
						
						strcpy(pbSidx, "09");
						SetSIdxValue(pbMNIndex, pbSidx, "", pobjIdxCol, pobjNode->getNodeId(), true);*/
					}
					delete [] pbMNIndex;
					delete [] pbSidx;
}

void UpdatedCNDateORTime(CIndex* pobjMNIndex, int iNodeId, EDateTime enumDT)
{

	ocfmRetCode stRetCode;
	CSubIndex *pobjSIdx;
	//char* Index 	= new char[INDEX_LEN];		
	char Index[INDEX_LEN];
	//char* Sidx 		=  new char[SUBINDEX_LEN];
	char Sidx[SUBINDEX_LEN];
	CIndexCollection* pobjIdxCol;
	CNodeCollection* pobjNodeCol;
	CNode* pobjNode;
	char* subIndexHexValue = new char[3 + ALLOC_BUFFER];

		pobjNodeCol =  CNodeCollection::getNodeColObjectPointer();
		pobjNode =  pobjNodeCol->getNodePtr(CN, iNodeId);
		pobjIdxCol = pobjNode->getIndexCollection();
		
		subIndexHexValue = _IntToAscii(iNodeId, subIndexHexValue, 16);
		subIndexHexValue = padLeft(subIndexHexValue, '0', 2);
		pobjSIdx = pobjMNIndex->getSubIndexbyIndexValue(subIndexHexValue);
		
		/* Add 1006*/
			strcpy(Index, "1020");
				
			stRetCode = AddIndex(pobjNode->getNodeId(), CN, Index);
				#if defined DEBUG	
				cout << "stRetCode" << stRetCode.code<<endl;
				cout<< "1020 added"<<endl;
			#endif
			if((stRetCode.code != 0) && (stRetCode.code != OCFM_ERR_INDEX_ALREADY_EXISTS ))
			exit;
			
			/*char* Val = new char[16];
			Val = _IntToAscii(iConfigDate,Val, 10);
		*/
			/* Set 5ms pxcValue*/
				/* Set subindex pxcValue 40 or 0000028 */
				
				if(pobjSIdx != NULL)
				{
					if(pobjSIdx->getActualValue() != NULL)
					{	
						if(enumDT == DATE)
						{
				
							strcpy(Sidx, "01");
							SetSIdxValue(Index, Sidx,(char*)pobjSIdx->getActualValue() , pobjIdxCol, pobjNode->getNodeId(), CN,  false);
						}
						else if(enumDT == TIME)
						{					
						
							strcpy(Sidx, "02");
							SetSIdxValue(Index, Sidx,(char*) pobjSIdx->getActualValue(), pobjIdxCol, pobjNode->getNodeId(), CN, false);
						}
					}
				}
								
}
/**************************************************************************************************
* Function Name: CNode::Copy pdos default value to actual value
* Description: 
* Return value: void
****************************************************************************************************/
void copyPDODefToAct(int iNodeID, ENodeType enumNodeType)
{


	CSubIndex* pobjSIndex;
	CIndexCollection* pobjIdxCol;
			
	CNode *pobjNode;
	CNodeCollection *objNodeCollection = NULL;

	
	objNodeCollection = CNodeCollection::getNodeColObjectPointer();
	pobjNode = objNodeCollection->getNodePtr(enumNodeType, iNodeID);
	pobjIdxCol = pobjNode->getIndexCollection();
	
	for(INT32 iIndexLoopCount=0; iIndexLoopCount < pobjIdxCol->getNumberofIndexes(); iIndexLoopCount++)
	{
			CIndex* pobjIndex;
			
			pobjIndex = pobjIdxCol->getIndex(iIndexLoopCount);
			
			if( !(CheckIfNotPDO((char*)pobjIndex->getIndexValue())) )
			{
									
				for(INT32 iSIdxLoopCount=0; iSIdxLoopCount < pobjIndex->getNumberofSubIndexes(); iSIdxLoopCount++)
				{
					pobjSIndex = pobjIndex->getSubIndex(iSIdxLoopCount);
					if(pobjSIndex->getActualValue() == 		NULL)
					{							
						if(pobjSIndex->getDefaultValue() != NULL)
						{							
							pobjSIndex->setActualValue((char*)pobjSIndex->getDefaultValue());
						}
						
					}
				}
			}
	}
	}
/**************************************************************************************************
* Function Name: getPDOIndexByOffset
* Description: Sort the pdos by offset
* Return value: Object of CIndex class
****************************************************************************************************/

CIndex getPDOIndexByOffset(CIndex* pobjIndex)
{
	//CIndexCollection* pPdoColByOffset;
	CIndex objIndex; 
	objIndex = *pobjIndex;
	CSubIndex* pObjSIdx1, *pObjSIdx2;
			
	for(INT32 iIndexLoopCount=1; iIndexLoopCount <= objIndex.getNumberofSubIndexes() - 1; iIndexLoopCount++)
	{
		for(INT32 iIdxLoopCount=1; iIdxLoopCount <= objIndex.getNumberofSubIndexes() -1 -iIndexLoopCount ; iIdxLoopCount++)
		{
					
			pObjSIdx1 = objIndex.getSubIndex(iIdxLoopCount);
			if(pObjSIdx1->getActualValue()!= NULL)
			{
				const char* pbActualVal1 = pObjSIdx1->getActualValue();
				INT32 iLength1 = strlen(pbActualVal1);
				char* pbOffset1 = NULL;
				INT32 iOffset1 = 0;
				//cout << "\n pbActualVal1" << pbActualVal1;
				
				pbOffset1 = subString((char*)pbActualVal1, iLength1-12,4);
				iOffset1 = hex2int(pbOffset1);
				//cout << "\n iOffset1" << iOffset1;
				pObjSIdx2 = objIndex.getSubIndex(iIdxLoopCount + 1);
				if(pObjSIdx2->getActualValue()!= NULL)
				{
					const char* pbActualVal2 = pObjSIdx2->getActualValue();
					
				
					
					INT32 iLength2 = strlen(pbActualVal2);
					char* pbOffset2 = NULL;
					INT32 iOffset2 = 0;
					
					pbOffset2 = subString((char*)pbActualVal2, iLength1-12,4);
					iOffset2  = hex2int(pbOffset2);
					CSubIndex* pbTempIdx = 0;
					if(iOffset1 >  iOffset2)
					{					
						/*pbTempIdx = pObjSIdx1;
						pObjSIdx1  = pObjSIdx2;
						pObjSIdx2 = pbTempIdx;	*/
						objIndex.SwapSubObjects(iIdxLoopCount, iIdxLoopCount + 1);
				
					}
				}
			}
		}
	}

	return objIndex;
}
/****************************************************************************************************
* Function Name: GetFeatureValue
* Description:
* Return value: ocfmRetCode
****************************************************************************************************/

ocfmRetCode GetFeatureValue(							
							INT32 iNodeID,
							ENodeType eNodeType, 
							EFeatureType eFeatureType,
							char* FeatureName, 
							char* Out_FeatureValue)
{
	ocfmRetCode stErrorInfo;
	INT32 iNodePos;
	char* pbRetString = NULL;
	try
	{
		bool bFlag = false;
		stErrorInfo = IfNodeExists(iNodeID, eNodeType, &iNodePos, bFlag);
		if (stErrorInfo.code == 0 && bFlag==true) 
		{		
			//retPos = stErrorInfo.returnValue;
		}
		else
		{	
			//cout << "$S\n\nErrStruct.errCode.code:" << stErrorInfo.code << "\n\n!!!" << endl;
			// Node Doesn't Exist
			ocfmException objException;// = new ocfmException;
			objException.ocfm_Excpetion(OCFM_ERR_INVALID_NODEID);
			throw &objException;
		}

		CNode objNode;		
		CNodeCollection *pobjNodeCollection;
		CNetworkManagement *pobjNwManagement;

		pobjNodeCollection= CNodeCollection::getNodeColObjectPointer();		
		objNode = pobjNodeCollection->getNode(iNodeID);
		
		pobjNwManagement = objNode.getNetworkManagement();
		strcpy(Out_FeatureValue, pobjNwManagement->getFeatureValue(eFeatureType, FeatureName));
		/*pbRetString = pobjNwManagement->getFeatureValue(eFeatureType, FeatureName);
		if(pbRetString != NULL)
		{
			Out_FeatureValue = new char[strlen(pbRetString) + STR_ALLOC_BUFFER + 1];
			strcpy(Out_FeatureValue, pbRetString);
			strcat(Out_FeatureValue, "\0");
			printf("\n Out_FeatureValue %s\n", Out_FeatureValue);			
		}
		else
			Out_FeatureValue = NULL;*/
			
		stErrorInfo.code = OCFM_ERR_SUCCESS;
		return stErrorInfo;
	}
	catch(ocfmException* ex)
	{
		return ex->_ocfmRetCode;
	}
}
/****************************************************************************************************
* Function Name: GetNodeIDbyNodePos
* Description: Fills the NodeID and NodeType for the NodeCount
* Return value: ocfmRetCode
****************************************************************************************************/

ocfmRetCode UpdateNodeParams(INT32 iCurrNodeId, INT32 iNewNodeID, ENodeType eNodeType, 
							 char* NodeName, EStationType eStationType, char* ForcedCycle, bool ForcedCycleFlag, char* PollResponseTimeout)
{
	//cout<< "Inside GetNodeIDbyNodeCount" << endl;
	ocfmRetCode stErrorInfo;
	INT32 iNodePos;
	try
	{
		bool bFlag = false;
		
		if(iNewNodeID == iCurrNodeId)
		{
			stErrorInfo = IfNodeExists(iNewNodeID, eNodeType, &iNodePos, bFlag);
			if(stErrorInfo.code == 0 && bFlag ==true)
			{
				
			}
			else
			{
				ocfmException objException;// = new ocfmException;
				objException.ocfm_Excpetion(OCFM_ERR_INVALID_NODEID);
				throw &objException;
			}

		}
		else if(iNewNodeID != iCurrNodeId)
		{
			stErrorInfo = IfNodeExists(iNewNodeID, eNodeType, &iNodePos, bFlag);
			if(stErrorInfo.code == OCFM_ERR_SUCCESS && bFlag == true)								
			{		
				ocfmException objException ;//= new ocfmException;
				objException.ocfm_Excpetion(OCFM_ERR_NODE_ALREADY_EXISTS);		
				throw &objException;
			}
			else
			{				
				#if defined DEBUG
					cout<< "OCFM_ERR_NODEID_NOT_FOUND" << OCFM_ERR_NODEID_NOT_FOUND << endl;;
				#endif
			}
		}
		if(eStationType != MULTIPLEXED ||( iNewNodeID != iCurrNodeId && eStationType == MULTIPLEXED))
		{	
			char* cSIdx = new char[SUBINDEX_LEN];
			cSIdx = _IntToAscii(iCurrNodeId, cSIdx, 16);
			cSIdx = padLeft(cSIdx, '0', 2);
			//Deleted MN's 1F9B and Suindex = old node id
			DeleteSubIndex(MN_NODEID, MN, (char *)MULTIPL_CYCLE_ASSIGN_OBJECT, cSIdx);
		}

		CNode* pobjNode;		
		CNodeCollection *pobjNodeCollection;
		
		

		pobjNodeCollection= CNodeCollection::getNodeColObjectPointer();
		pobjNode = pobjNodeCollection->getNodePtr(eNodeType, iCurrNodeId);
		if(eNodeType == CN)
		{
			pobjNode->setNodeId(iNewNodeID);
			EStationType eNodePrevStationType = pobjNode->getStationType();
			pobjNode->setStationType(eStationType);
            if( (eNodePrevStationType == MULTIPLEXED) && (eStationType != eNodePrevStationType) )
            {
                CheckAndReAssignMultiplex(iNewNodeID, pobjNode->getForcedCycle());
            }
            if(eStationType != MULTIPLEXED)
            {
                pobjNode->resetForcedCycleValue();
            }
			if(eStationType == MULTIPLEXED)
			{
                bool bCalcForceCycle = true;
                bool bSetForceCycle = true;
                if(ForcedCycleFlag == true)
                {
                    bCalcForceCycle = false;
                }

				if( (ForcedCycle == NULL || strcmp(ForcedCycle,"") ==0) && ForcedCycleFlag == false )
				{
                    if(pobjNode->getForceCycleFlag() == true )
                    {
                        
                    }
                    else if(pobjNode->getForcedCycle() == NULL || strcmp(pobjNode->getForcedCycle(),"") == 0 )
                    {

                    } 
                    else
                    {
//                         char* actValue = new char[50];
//                         actValue[0] = 0;
//                         stErrorInfo = GetSubIndexAttributes(MN_NODEID, MN, (char*)"1F98", (char*)"07", ACTUALVALUE, actValue); 
//                         if(stErrorInfo.code == OCFM_ERR_SUCCESS)
//                         {
//                             if( !(actValue == NULL || strcmp(actValue,"") ==0) )
//                             {
//                                 INT32 iMNMultiActualValue = 0;
//                                 if (strncmp(actValue,"0x",2) == 0 || strncmp(actValue,"0X",2) == 0)
//                                     iMNMultiActualValue  = hex2int(subString(actValue, 2, strlen(actValue) -2));
//                                 else
//                                     iMNMultiActualValue  = atoi(actValue);
    
                                INT32 iCNActualValue = 0;
                                if (strncmp(pobjNode->getForcedCycle(),"0x",2) == 0 || strncmp(pobjNode->getForcedCycle(),"0X",2) == 0)
                                    iCNActualValue  = hex2int(subString(pobjNode->getForcedCycle(), 2, strlen(pobjNode->getForcedCycle()) -2));
                                else
                                    iCNActualValue  = atoi(pobjNode->getForcedCycle());

                                if( true == IsMultiplexCycleNumberContinuous(iCNActualValue) )
                                {
                                    bCalcForceCycle = false;
                                    bSetForceCycle = false;
                                }
//                             }
//                             else
//                             {
//                             }
//                         }
//                         else
//                         {
//                         }
                    }
				}
                if(bCalcForceCycle == true)
                {
//                         ForcedCycle = new char[20];
//                         UINT32 uiCycleno =  getLastAvailableCycleNumber();
//                         //uiCycleno = getFreeCycleNumber(uiCycleno, pobjNode->getNodeId());
//                         //uiCycleNumber = uiCycleno;
//                         ForcedCycle = _IntToAscii(uiCycleno, ForcedCycle, 16);
                         ForcedCycle = getLastAvailableCycleNumber();
                }
				if(bSetForceCycle == true)
				    pobjNode->setForcedCycle(ForcedCycle);
//                 if(ForcedCycleFlag == false)
//                 {
//                         ForcedCycle = new char[20];
//                         UINT32 uiCycleno =  getLastAvailableCycleNumber();
//                         ForcedCycle = _IntToAscii(uiCycleno, ForcedCycle, 16);
//                 }
//                 pobjNode->setForcedCycle(ForcedCycle);
				pobjNode->setForceCycleFlag(ForcedCycleFlag);
			}
		}

		if(eNodeType == CN && !(PollResponseTimeout == NULL || strcmp(PollResponseTimeout,"") == 0) )
		{
			pobjNode->setPollResponseTimeout(PollResponseTimeout);
		}
		pobjNode->setNodeName(NodeName);
		
				
		stErrorInfo.code = OCFM_ERR_SUCCESS;
		return stErrorInfo;
	}
	catch(ocfmException* ex)
	{
		return ex->_ocfmRetCode;
	}
}
/****************************************************************************************************
* Function Name: GetNodeDataTypes
* Description:Returns the Data Type Available for a prticular node
* Return value: ocfmRetCode
****************************************************************************************************/

ocfmRetCode GetNodeDataTypes(INT32 iNodeId, ENodeType eNodeType, char* pbOutDataTypes)
{
	//cout<< "Inside GetNodeIDbyNodeCount" << endl;
	ocfmRetCode stErrorInfo;
	INT32 iNodePos;
	try
	{
		bool bFlag = false;
		stErrorInfo = IfNodeExists(iNodeId, eNodeType, &iNodePos, bFlag);
		if (stErrorInfo.code == 0 && bFlag==true) 
		{		
			
		}
		else
		{	
			//cout << "$S\n\nErrStruct.errCode.code:" << stErrorInfo.code << "\n\n!!!" << endl;
			// Node Doesn't Exist
			ocfmException objException;// = new ocfmException;
			objException.ocfm_Excpetion(OCFM_ERR_NODE_ALREADY_EXISTS);
			throw &objException;
		}

		CNode* pobjNode;		
		CNodeCollection *pobjNodeCollection;
		CDataTypeCollection *pobjDtCol;
		

		pobjNodeCollection= CNodeCollection::getNodeColObjectPointer();
		pobjNode = pobjNodeCollection->getNodePtr(eNodeType, iNodeId);
		pobjDtCol = pobjNode->getDataTypeCollection();
		
		for(UINT16 uiLoopCount = 0; uiLoopCount<pobjDtCol->getNumberOfDataTypes() ; uiLoopCount++)
		{
			DataType *dt = NULL;
			char* strdtName = NULL;

			dt = pobjDtCol->getDataTypeElement(uiLoopCount);			
			strdtName = new char[strlen(dt->getName()) + STR_ALLOC_BUFFER];
			
			strcpy(strdtName, dt->getName());			
			if(uiLoopCount == 0)
			{
				//pbOutDataTypes = (char*)realloc(pbOutDataTypes,  strlen(strdtName) + STR_ALLOC_BUFFER +  2);						
				strcpy(pbOutDataTypes, strdtName);				
			}
			else 
			{
				//printf("%d Datatypes string length %d", strlen(pbOutDataTypes));
				//pbOutDataTypes = (char*)realloc(pbOutDataTypes, strlen(pbOutDataTypes) + strlen(strdtName) + STR_ALLOC_BUFFER +  2);				
				strcat(pbOutDataTypes, strdtName);
				
			}

			/*if(pbOutDataTypes == NULL)
			{
				pbOutDataTypes = (char*)malloc(strlen(strdtName) + STR_ALLOC_BUFFER +  2);
				strcpy(pbOutDataTypes, strdtName);				
			}
			else 
			{
				pbOutDataTypes = (char*)realloc(pbOutDataTypes, strlen(pbOutDataTypes) + strlen(strdtName) + STR_ALLOC_BUFFER +  3);				
				strcat(pbOutDataTypes, strdtName);				
				
			}*/
			
			strcat(pbOutDataTypes, "\n");					
		}
		
		stErrorInfo.code = OCFM_ERR_SUCCESS;
		return stErrorInfo;
	}
	catch(ocfmException* ex)
	{
		return ex->_ocfmRetCode;
	}
}
/***********************************************************************************************************************************************
* Function Name: setNodeAssigmentBits
* Description:Returns the 1F81 object data depending upon the  if Multiplex set bit 8, if chained set bit 14
* Return value: char*
***********************************************************************************************************************************************/
char* setNodeAssigmentBits(CNode* pobjNode)
{
	char* pb1F81Data = NULL;
	unsigned long ulValue;
	pb1F81Data = new char[8 + STR_ALLOC_BUFFER];
	ulValue =  EPL_NODEASSIGN_VALID | EPL_NODEASSIGN_NODE_EXISTS | EPL_NODEASSIGN_NODE_IS_CN | EPL_NODEASSIGN_START_CN;
	switch(pobjNode->getStationType())
	{
		case NORMAL:			
		break;

		case MULTIPLEXED:
			ulValue =  ulValue | EPL_NODEASSIGN_MULTIPLEXED_CN;
			break;

		case CHAINED:
			ulValue =  ulValue | EPL_NODEASSIGN_CHAINED_CN; 
			break;
	};

	pb1F81Data = _IntToAscii(ulValue,pb1F81Data, 16); 
	return pb1F81Data;
}

/****************************************************************************************************
* Function Name: RecalculateMultiplex
* Description: recalculates the multiplex cycle for CNs
* Return value: ocfmRetCode
****************************************************************************************************/

ocfmRetCode RecalculateMultiplex()
{
	ocfmRetCode stErrorInfo;
	CNodeCollection *objNodeCol;
	objNodeCol= CNodeCollection::getNodeColObjectPointer();
	INT32 iNodesCount = 0;
	iNodesCount = objNodeCol->getCNNodesCount();

	CNode* objNode;

	try
	{
        if(false == CheckIfMultiplexedCNExist())
        {
            stErrorInfo.code == OCFM_ERR_SUCCESS;
            return stErrorInfo;
        }
		char* actValue = new char[50];
		actValue[0] = 0;
        int IndexPos;
        int subIndexPos;
        stErrorInfo = IfSubIndexExists(MN_NODEID, MN, (char*)"1F98", (char*)"07", &subIndexPos, &IndexPos);
        if(stErrorInfo.code == OCFM_ERR_SUCCESS)        
        {
        }
        else
        {
            //reset automatically assigned cn force cycle
            ResetMultiplexedCNForceCycle();
            stErrorInfo.code == OCFM_ERR_SUCCESS;
            return stErrorInfo;
        }
		stErrorInfo = GetSubIndexAttributes(MN_NODEID, MN, (char*)"1F98", (char*)"07", ACTUALVALUE, actValue); 
		if(stErrorInfo.code == OCFM_ERR_SUCCESS)		
		{
		}
		else
		{
			ocfmException objException;// = new ocfmException;
			objException.ocfm_Excpetion(stErrorInfo.code);
			delete[] actValue;
			throw &objException;
		}

		for(INT32 iLoopCount = 0; iLoopCount < objNodeCol->getNumberOfNodes(); iLoopCount++)
		{
			objNode = objNodeCol->getNodebyColIndex(iLoopCount);
			if (objNode->getNodeType() == CN )
			{
				ocfmRetCode stErrorInfoSubindexMultiplCycl;
				static char* strConvertedValue = NULL;
				if(strConvertedValue != NULL)
				{
					delete[] strConvertedValue;
					strConvertedValue = NULL;
				}
				strConvertedValue = new char[SUBINDEX_LEN];
				char acMultiCycleAssignObj[] = MULTIPL_CYCLE_ASSIGN_OBJECT;
				strConvertedValue = _IntToAscii(objNode->getNodeId(), strConvertedValue, 16);
				strConvertedValue = padLeft(strConvertedValue, '0', 2);
				char* subIndActValue = new char[20];
				subIndActValue[0] = 0;
				try
				{
					stErrorInfoSubindexMultiplCycl = GetSubIndexAttributes(240, MN, acMultiCycleAssignObj, strConvertedValue, ACTUALVALUE, subIndActValue);
				} 
				catch(...)
				{
					delete[] subIndActValue;
					//delete[] strConvertedValue;
					continue;
				}
 				if(stErrorInfoSubindexMultiplCycl.code == OCFM_ERR_SUCCESS)
 				{
 				}
 				else
 				{
					continue;
 				}
				if (objNode->getStationType() == MULTIPLEXED)
				{
                    if(objNode->getForceCycleFlag() == 1)
                    {
                        continue;
                    }
					else if( objNode->getForcedCycle() == NULL || strcmp(objNode->getForcedCycle(),"") == 0 )
					{
						//continue down the loop
					}
					else if ( actValue == NULL || strcmp(actValue,"") == 0 )
					{
						char* subIndName = new char[50];
						subIndName[0] = 0;
						GetSubIndexAttributes(240, MN, acMultiCycleAssignObj, strConvertedValue, NAME, subIndName);
						char* subIndFlag = new char[10];
						GetSubIndexAttributes(240, MN, acMultiCycleAssignObj, strConvertedValue, FLAGIFINCDC, subIndFlag);
						INT32 iCNsubIndFlag = 0;
						iCNsubIndFlag  = atoi(subIndFlag);
						SetSubIndexAttributes(MN_NODEID, MN, acMultiCycleAssignObj, strConvertedValue, (char*)"",subIndName, (EFlag)iCNsubIndFlag);
						delete[] subIndName;
						delete[] subIndFlag;
						//delete[] strConvertedValue;
						continue;
					}
					else
					{
						if(stErrorInfoSubindexMultiplCycl.code == OCFM_ERR_SUCCESS)
						{
							INT32 iMNMultiActualValue = 0;
							if (strncmp(actValue,"0x",2) == 0 || strncmp(actValue,"0X",2) == 0)
								iMNMultiActualValue  = hex2int(subString(actValue, 2, strlen(actValue) -2));
							else
								iMNMultiActualValue  = atoi(actValue);

							char* ForcedCycleValue = new char[50];
							strcpy(ForcedCycleValue,objNode->getForcedCycle());
							INT32 iCNActualValue = 0;
							if (strncmp(ForcedCycleValue,"0x",2) == 0 || strncmp(ForcedCycleValue,"0X",2) == 0)
								iCNActualValue  = hex2int(subString(ForcedCycleValue, 2, strlen(ForcedCycleValue) -2));
							else
								iCNActualValue  = atoi(ForcedCycleValue);

							if (iMNMultiActualValue == 0)
							{

								char* subIndName = new char[50];
								subIndName[0] = 0;
								GetSubIndexAttributes(240, MN, acMultiCycleAssignObj, strConvertedValue, NAME, subIndName);
								char* subIndFlag = new char[10];
								GetSubIndexAttributes(240, MN, acMultiCycleAssignObj, strConvertedValue, FLAGIFINCDC, subIndFlag);
								INT32 iCNsubIndFlag = 0;
								iCNsubIndFlag  = atoi(subIndFlag);
								SetSubIndexAttributes(MN_NODEID, MN, acMultiCycleAssignObj, strConvertedValue, (char*)"",subIndName, (EFlag)iCNsubIndFlag);
								delete[] subIndName;
								delete[] subIndFlag;
								continue;
							}
							else if(iCNActualValue <= iMNMultiActualValue)
							{
								continue;
							}
							else
							{
								//continue the function
							}
						}
						else
						{

//need to foce the cycle


// 							ocfmException objException;// = new ocfmException;
// 							objException.ocfm_Excpetion(stErrorInfoSubindexMultiplCycl.code);
// 							delete[] subIndActValue;
// 							throw &objException;
						}
					}

// 					char* ForcedCycle = new char[20];
// 					UINT32 uiCycleno = getLastAvailableCycleNumber();
//                     //uiCycleno = getFreeCycleNumber(uiCycleno, objNode->getNodeId());
//                     //uiCycleNumber = uiCycleno;
// 					ForcedCycle = _IntToAscii(uiCycleno, ForcedCycle, 16);
                    char* ForcedCycle = getLastAvailableCycleNumber();
					objNode->setForcedCycle(ForcedCycle);
					delete[] ForcedCycle;
				}
				else 
				{
				// station other than multiplexed
					char* subIndName = new char[50];
					subIndName[0] = 0;
					GetSubIndexAttributes(240, MN, acMultiCycleAssignObj, strConvertedValue, NAME, subIndName);

					char* subIndFlag = new char[10];
					GetSubIndexAttributes(240, MN, acMultiCycleAssignObj, strConvertedValue, FLAGIFINCDC, subIndFlag);

					INT32 iCNsubIndFlag = 0;
					iCNsubIndFlag  = atoi(subIndFlag);
					SetSubIndexAttributes(MN_NODEID, MN, acMultiCycleAssignObj, strConvertedValue, (char*)"",subIndName, (EFlag)iCNsubIndFlag);
					delete[] subIndName;
					delete[] subIndFlag;
				}
				//delete[] strConvertedValue;
			} // end of if loop 1
		}//end of for loop
		delete[] actValue;
	}//end of try
	catch(ocfmException* ex)
	{
		return ex->_ocfmRetCode;
	}
}

void ResetMultiplexedCNForceCycle()
{
    CNodeCollection *objNodeCol;
    objNodeCol= CNodeCollection::getNodeColObjectPointer();
    CNode* objNode;

    for(INT32 iLoopCount = 0; iLoopCount < objNodeCol->getNumberOfNodes(); iLoopCount++)
    {
        objNode = objNodeCol->getNodebyColIndex(iLoopCount);
        if (objNode->getNodeType() == CN )
        {
            ocfmRetCode stErrorInfoSubindexMultiplCycl;
            static char* strConvertedValue = NULL;
            if(strConvertedValue != NULL)
            {
                delete[] strConvertedValue;
                strConvertedValue = NULL;
            }
            strConvertedValue = new char[SUBINDEX_LEN];
            char acMultiCycleAssignObj[] = MULTIPL_CYCLE_ASSIGN_OBJECT;
            strConvertedValue = _IntToAscii(objNode->getNodeId(), strConvertedValue, 16);
            strConvertedValue = padLeft(strConvertedValue, '0', 2);
            char* subIndActValue = new char[20];
            subIndActValue[0] = 0;
            try
            {
                stErrorInfoSubindexMultiplCycl = GetSubIndexAttributes(240, MN, acMultiCycleAssignObj, strConvertedValue, ACTUALVALUE, subIndActValue);
            } 
            catch(...)
            {
                delete[] subIndActValue;
                //delete[] strConvertedValue;
                continue;
            }
            if(stErrorInfoSubindexMultiplCycl.code == OCFM_ERR_SUCCESS)
            {
            }
            else
            {
                continue;
            }
            if (objNode->getStationType() == MULTIPLEXED)
            {
                if(objNode->getForceCycleFlag() == 1)
                {
                    continue;
                }
                objNode->setForcedCycle((char*)"");
                objNode->setStationType(NORMAL);
            }
        }
    }
}

/**************************************************************************************************
* Function Name: CNode::copyMNPropDefToAct
* Description: 
* Return value: void
****************************************************************************************************/
void copyMNPropDefToAct(int iNodeID, ENodeType enumNodeType)
{   
    if ( enumNodeType != MN ) 
    {
        return;
    }
    copyIndexDefToAct(iNodeID, enumNodeType, (char *)"1006");
    copySubIndexDefToAct(iNodeID, enumNodeType, (char *)"1F8A", (char *)"02" );
    copySubIndexDefToAct(iNodeID, enumNodeType, (char *)"1F98", (char *)"07" );
    copySubIndexDefToAct(iNodeID, enumNodeType, (char *)"1F98", (char *)"08" );
}

/**************************************************************************************************
* Function Name: CNode::copyIndexDefToAct
* Description: 
* Return value: void
****************************************************************************************************/
void copyIndexDefToAct(int iNodeID, ENodeType enumNodeType, char *indexId )
{
    CSubIndex* pobjSIndex;
    CIndexCollection* pobjIdxCol;
            
    CNode *pobjNode;
    CNodeCollection *objNodeCollection = NULL;

    
    objNodeCollection = CNodeCollection::getNodeColObjectPointer();
    pobjNode = objNodeCollection->getNodePtr(enumNodeType, iNodeID);
    pobjIdxCol = pobjNode->getIndexCollection();
    
    CIndex* pobjIndex;
    
    pobjIndex = pobjIdxCol->getIndexbyIndexValue(indexId);
    if(pobjIndex == NULL)
    {
        return;
    }
    if(pobjIndex->getActualValue() == NULL)
    {
        if(pobjIndex->getDefaultValue() != NULL)
        {
            pobjIndex->setActualValue((char*)pobjIndex->getDefaultValue());
        }
    }
}

/**************************************************************************************************
* Function Name: CNode::copySubIndexDefToAct
* Description: 
* Return value: void
****************************************************************************************************/
void copySubIndexDefToAct(int iNodeID, ENodeType enumNodeType, char *indexId, char *subIndexId )
{
    CSubIndex* pobjSIndex;
    CIndexCollection* pobjIdxCol;
            
    CNode *pobjNode;
    CNodeCollection *objNodeCollection = NULL;

    
    objNodeCollection = CNodeCollection::getNodeColObjectPointer();
    pobjNode = objNodeCollection->getNodePtr(enumNodeType, iNodeID);
    pobjIdxCol = pobjNode->getIndexCollection();
    
    CIndex* pobjIndex;
    
    pobjIndex = pobjIdxCol->getIndexbyIndexValue(indexId);
    if(pobjIndex == NULL)
    {
        return;
    }


    pobjSIndex = pobjIndex->getSubIndexbyIndexValue(subIndexId);
    if(pobjSIndex == NULL)
    {
        return;
    }
    if(pobjSIndex->getActualValue() ==      NULL)
    {
        if(pobjSIndex->getDefaultValue() != NULL)
        {
            pobjSIndex->setActualValue((char*)pobjSIndex->getDefaultValue());
        }
    }
}

// /****************************************************************************************************
// * Function Name: CheckAndReAssignMultiplex
// * Description: -
// * Return value: ocfmRetCode
// ****************************************************************************************************/

void CheckAndReAssignMultiplex(int iCNNodeId, char* CycleValue )
{
	ocfmRetCode stErrorInfo;
	if(CycleValue == NULL || strcmp(CycleValue,"") == 0 )
	{
		stErrorInfo.code = OCFM_ERR_SUCCESS;
		//return stErrorInfo;
        return;
	}

	INT32 iCycleValue = 0;
	if (strncmp(CycleValue,"0x",2) == 0 || strncmp(CycleValue,"0X",2) == 0)
		iCycleValue  = hex2int(subString(CycleValue, 2, strlen(CycleValue) -2));
	else
		iCycleValue  = atoi(CycleValue);

	CNodeCollection *objNodeCol;
	objNodeCol= CNodeCollection::getNodeColObjectPointer();
	INT32 iNodesCount = 0;
	iNodesCount = objNodeCol->getCNNodesCount();

	CNode* objNode;

	try
	{
		if(iNodesCount == 0)
		{				
			//exit;
            return;
		}
		char* actValue = new char[50];
		actValue[0] = 0;
        try 
        {
        
		    stErrorInfo = GetSubIndexAttributes(MN_NODEID, MN, (char*)"1F98", (char*)"07", ACTUALVALUE, actValue); 
		}
		catch(...)
		{
			delete[] actValue;
            stErrorInfo.code = OCFM_ERR_SUCCESS;
            //return stErrorInfo;
            return;
		}

		for(INT32 iLoopCount = 0; iLoopCount < objNodeCol->getNumberOfNodes(); iLoopCount++)
		{
			objNode = objNodeCol->getNodebyColIndex(iLoopCount);
			if (objNode->getNodeType() == CN && objNode->getNodeId() != iCNNodeId && objNode->getStationType() == MULTIPLEXED)
			{
                    if(objNode->getForcedCycle() == NULL || strcmp(objNode->getForcedCycle(),"") == 0 )
                    {
                        continue;
                    }
                    else
                    {
                            char* ForcedCycleValue = new char[strlen(objNode->getForcedCycle()) + ALLOC_BUFFER];
                            strcpy(ForcedCycleValue,objNode->getForcedCycle());
                            INT32 iCNActualValue = 0;
                            if (strncmp(ForcedCycleValue,"0x",2) == 0 || strncmp(ForcedCycleValue,"0X",2) == 0)
                                iCNActualValue  = hex2int(subString(ForcedCycleValue, 2, strlen(ForcedCycleValue) -2));
                            else
                                iCNActualValue  = atoi(ForcedCycleValue);

                            delete[] ForcedCycleValue;
                            if (iCycleValue == iCNActualValue)
                            {
                                return;
                            }
                            else
                            {
                                //continue the function
                            }
                    }
            } // end of if loop 1
		}//end of for loop
        for(INT32 iLoopCount = 0; iLoopCount < objNodeCol->getNumberOfNodes(); iLoopCount++)
        {
            objNode = objNodeCol->getNodebyColIndex(iLoopCount);
            if (objNode->getNodeType() == CN && objNode->getNodeId() != iCNNodeId && objNode->getStationType() == MULTIPLEXED && objNode->getForceCycleFlag() == false)
            {
                if(objNode->getForcedCycle() == NULL || strcmp(objNode->getForcedCycle(),"") == 0 )
                {
                    continue;
                }
                else
                {
                }

                char* ForcedCycleValue = new char[strlen(objNode->getForcedCycle()) + ALLOC_BUFFER];
                strcpy(ForcedCycleValue,objNode->getForcedCycle());
                INT32 iCNActualValue = 0;
                if (strncmp(ForcedCycleValue,"0x",2) == 0 || strncmp(ForcedCycleValue,"0X",2) == 0)
                    iCNActualValue  = hex2int(subString(ForcedCycleValue, 2, strlen(ForcedCycleValue) -2));
                else
                    iCNActualValue  = atoi(ForcedCycleValue);

                delete[] ForcedCycleValue;
                if (iCycleValue >= iCNActualValue || iCNActualValue == 1)
                {
                    continue;
                }
                else
                {
                            //continue the function
                }
                iCNActualValue--;
                uiCycleNumber = iCNActualValue;

                ocfmRetCode stErrorInfoSubindexMultiplCycl;
                static char* strCNActualValue = NULL;
                if(strCNActualValue != NULL)
                {
                    delete[] strCNActualValue;
                    strCNActualValue = NULL;
                }
                strCNActualValue = new char[50];
                strCNActualValue = _IntToAscii(iCNActualValue, strCNActualValue, 10);

                objNode->setForcedCycle(strCNActualValue);
            } //end of if loop
        } //end of for loop
		delete[] actValue;
	}//end of try
 	catch(ocfmException* ex)
 	{
 		//return ex->_ocfmRetCode;
        return;
 	}
}

/****************************************************************************************************
// * Function Name: CheckMutliplexAssigned
// * Description: -
// * Return value: ocfmRetCode
// ****************************************************************************************************/
ocfmRetCode CheckMutliplexAssigned()
{
    ocfmRetCode stErrorInfo;
    CNodeCollection *objNodeCol;
    objNodeCol= CNodeCollection::getNodeColObjectPointer();
    INT32 iNodesCount = 0;
    iNodesCount = objNodeCol->getCNNodesCount();
    stErrorInfo.code = OCFM_ERR_SUCCESS;

    bool bErrorFlag = false;
    CNode* objNode;

    try
    {
        if(iNodesCount == 0)
        {
            stErrorInfo.code = OCFM_ERR_NO_CN_NODES_FOUND;
            return stErrorInfo;
        }
        if(false == CheckIfMultiplexedCNExist())
        {
            return stErrorInfo;
        }
        char* actValue = new char[50];
        actValue[0] = 0;
        int subIndexPos, IndexPos;
        stErrorInfo = IfSubIndexExists(MN_NODEID, MN, (char*)"1F98", (char*)"07", &subIndexPos, &IndexPos);
        if(stErrorInfo.code == OCFM_ERR_SUCCESS)        
        {
        }
        else
        {
            stErrorInfo.code = OCFM_ERR_MULTIPLEX_ASSIGN_ERROR;
            CopyCustomErrorString(&stErrorInfo, (char*)"The Subindex 07 of Index 1F98 does not exist Multiplexing not supported");
            return stErrorInfo;
        }
        stErrorInfo = GetSubIndexAttributes(MN_NODEID, MN, (char*)"1F98", (char*)"07", ACTUALVALUE, actValue); 
        if(stErrorInfo.code == OCFM_ERR_SUCCESS)        
        {
        }
        else
        {
            return stErrorInfo;

        }
        stErrorInfo.errorString = new char[500];
        stErrorInfo.errorString[0] = 0;
        strcpy(stErrorInfo.errorString,"CN with nodeid ");
        INT32 iMNMultiActualValue = 0;
        if (strncmp(actValue,"0x",2) == 0 || strncmp(actValue,"0X",2) == 0)
            iMNMultiActualValue  = hex2int(subString(actValue, 2, strlen(actValue) -2));
        else
            iMNMultiActualValue  = atoi(actValue);

        for(INT32 iLoopCount = 0; iLoopCount < objNodeCol->getNumberOfNodes(); iLoopCount++)
        {
            objNode = objNodeCol->getNodebyColIndex(iLoopCount);
            if (objNode->getNodeType() == CN )
            {
                if (objNode->getStationType() == MULTIPLEXED)
                {
    
                    ocfmRetCode stErrorInfoSubindexMultiplCycl;
                    static char* strConvertedValue = NULL;
                    if(strConvertedValue != NULL)
                    {
                        delete[] strConvertedValue;
                        strConvertedValue = NULL;
                    }
                    strConvertedValue = new char[SUBINDEX_LEN];
                    char acMultiCycleAssignObj[] = MULTIPL_CYCLE_ASSIGN_OBJECT;
                    strConvertedValue = _IntToAscii(objNode->getNodeId(), strConvertedValue, 16);
                    strConvertedValue = padLeft(strConvertedValue, '0', 2);
                    char* subIndActValue = new char[20];
                    subIndActValue[0] = 0;
                    try
                    {
                        stErrorInfoSubindexMultiplCycl = GetSubIndexAttributes(240, MN, acMultiCycleAssignObj, strConvertedValue, ACTUALVALUE, subIndActValue);
                    } 
                    catch(...)
                    {
                        delete[] subIndActValue;
                        //delete[] strConvertedValue;
                        continue;
                    }
                    if(stErrorInfoSubindexMultiplCycl.code == OCFM_ERR_SUCCESS)
                    {
                    }
                    else
                    {
                        continue;
                    }

                    if(objNode->getForcedCycle() == NULL || strcmp(objNode->getForcedCycle(),"") == 0)
                    {
                        //continue down the loop
                    }
                    else if ( actValue == NULL || strcmp(actValue,"") == 0 )
                    {
                        continue;
                    }
                    else
                    {
                        //if(stErrorInfoSubindexMultiplCycl.code == OCFM_ERR_SUCCESS)
                        //{
                            char* ForcedCycleValue = new char[50];
                            strcpy(ForcedCycleValue,objNode->getForcedCycle());
                            INT32 iCNActualValue = 0;
                            if (strncmp(ForcedCycleValue,"0x",2) == 0 || strncmp(ForcedCycleValue,"0X",2) == 0)
                                iCNActualValue  = hex2int(subString(ForcedCycleValue, 2, strlen(ForcedCycleValue) -2));
                            else
                                iCNActualValue  = atoi(ForcedCycleValue);

                            if(iCNActualValue <= iMNMultiActualValue)
                            {
                                continue;
                            }
                            else
                            {
//                                 strcat(stErrorInfo.errorString, "Nodeid:");
//                                 char* pCNnodeid = new char[SUBINDEX_LEN];
//                                 pCNnodeid = _IntToAscii(objNode->getNodeId(), pCNnodeid, 10);
//                                 strcat(stErrorInfo.errorString, pCNnodeid);
//                                 strcat(stErrorInfo.errorString, " ");
//                                 delete[] pCNnodeid;
                                sprintf(stErrorInfo.errorString,"%s%d, ", stErrorInfo.errorString, objNode->getNodeId());
                                bErrorFlag = true;
                            }
                        //}
                        //else
                        //{
                        //}
                    }
                }
                else 
                {
                }
                //delete[] strConvertedValue;
            } // end of if loop 1
        }//end of for loop
        if(bErrorFlag == true)
        {
            stErrorInfo.code = OCFM_ERR_MULTIPLEX_ASSIGN_ERROR;
            stErrorInfo.errorString[strlen(stErrorInfo.errorString)-2] = 0;
            strcat(stErrorInfo.errorString, " multiplex cycle value exceeds the multiplex prescalar");
        }
        delete[] actValue;
        return stErrorInfo;
    }//end of try
    catch(ocfmException* ex)
    {
        return ex->_ocfmRetCode;
    }
}

/****************************************************************************************************
// * Function Name: getFreeCycleNumber
// * Description: -
// * Return value: UINT32
// ****************************************************************************************************/
UINT32 getFreeCycleNumber(UINT32 uiCycleNumber)
{
    ocfmRetCode stErrorInfo;
    CNodeCollection *objNodeCol;
    objNodeCol= CNodeCollection::getNodeColObjectPointer();
    INT32 iNodesCount = 0;
    iNodesCount = objNodeCol->getCNNodesCount();

    CNode* objNode;
    UINT32 uiFreeCycleNumber = 0;

        if(iNodesCount == 0)
        {
            stErrorInfo.code = OCFM_ERR_NO_CN_NODES_FOUND;
            ocfmException objException;// = new ocfmException;
            objException.ocfm_Excpetion(stErrorInfo.code);
            return uiCycleNumber;
        }
        for(UINT32 uiCycleNumberCount = 1; uiCycleNumberCount < uiCycleNumber; uiCycleNumberCount++)
        {
            for(INT32 iLoopCount = 0; iLoopCount < objNodeCol->getNumberOfNodes(); iLoopCount++)
            {
                objNode = objNodeCol->getNodebyColIndex(iLoopCount);
                if (objNode->getNodeType() == CN && objNode->getStationType() == MULTIPLEXED)
                {
                    if(objNode->getForcedCycle() == NULL || strcmp(objNode->getForcedCycle(),"") == 0 )
                    {
                        //continue;
                    }
                    else
                    {
                        char* ForcedCycleValue = new char[strlen(objNode->getForcedCycle()) + ALLOC_BUFFER];
                        strcpy(ForcedCycleValue,objNode->getForcedCycle());
                        INT32 iCNActualValue = 0;
                        if (strncmp(ForcedCycleValue,"0x",2) == 0 || strncmp(ForcedCycleValue,"0X",2) == 0)
                            iCNActualValue  = hex2int(subString(ForcedCycleValue, 2, strlen(ForcedCycleValue) -2));
                        else
                            iCNActualValue  = atoi(ForcedCycleValue);

                        delete[] ForcedCycleValue;
                        if (uiCycleNumberCount == iCNActualValue)
                        {
                            //uiFreeCycleNumber = uiCycleNumberCount;
                            break;
                        }
                        else
                        {
                            //continue the function
                        }
                    }
                } // end of if loop 1
                if(iLoopCount == objNodeCol->getNumberOfNodes()-1)
                {
                    return uiCycleNumberCount;
                }
            }//end of for loop
            
        }//end of for loop
        return uiCycleNumber;
}

/****************************************************************************************************
// * Function Name: IsMultiplexCycleNumberContinuous
// * Description: -
// * Return value: bool
// ****************************************************************************************************/
bool IsMultiplexCycleNumberContinuous(UINT32 uiCycleNumber)
{
    ocfmRetCode stErrorInfo;
    CNodeCollection *objNodeCol;
    objNodeCol= CNodeCollection::getNodeColObjectPointer();
    INT32 iNodesCount = 0;
    iNodesCount = objNodeCol->getCNNodesCount();

    CNode* objNode;
    UINT32 uiFreeCycleNumber = 0;

        if(iNodesCount == 0)
        {
            return false;
        }
        for(UINT32 uiCycleNumberCount = 1; uiCycleNumberCount < uiCycleNumber; uiCycleNumberCount++)
        {

            for(INT32 iLoopCount = 0; iLoopCount < objNodeCol->getNumberOfNodes(); iLoopCount++)
            {

                objNode = objNodeCol->getNodebyColIndex(iLoopCount);
                if (objNode->getNodeType() == CN && objNode->getStationType() == MULTIPLEXED)
                {

                        if(objNode->getForcedCycle() == NULL || strcmp(objNode->getForcedCycle(),"") == 0 )
                        {
                            //continue;
                        }
                        else
                        {
                                char* ForcedCycleValue = new char[strlen(objNode->getForcedCycle()) + ALLOC_BUFFER];
                                strcpy(ForcedCycleValue,objNode->getForcedCycle());
                                INT32 iCNActualValue = 0;
                                if (strncmp(ForcedCycleValue,"0x",2) == 0 || strncmp(ForcedCycleValue,"0X",2) == 0)
                                    iCNActualValue  = hex2int(subString(ForcedCycleValue, 2, strlen(ForcedCycleValue) -2));
                                else
                                    iCNActualValue  = atoi(ForcedCycleValue);
    
                                delete[] ForcedCycleValue;
                                if (uiCycleNumberCount == iCNActualValue)
                                {
                                    //uiFreeCycleNumber = uiCycleNumberCount;
                                    break;
                                }
                                else
                                {
                                    //continue the function
                                }
                        }
                } // end of if loop 1
                if(iLoopCount == objNodeCol->getNumberOfNodes()-1)
                {
                    return false;
                }
            }//end of for loop
            
        }//end of for loop
        return true;
}

/****************************************************************************************************
// * Function Name: calculateCNPollResponse
// * Description: -
// * Return value: void
// ****************************************************************************************************/
void calculateCNPollResponse(int iNodeID, ENodeType enumNodeType)
{
    if(enumNodeType != CN)
    {
        return;
    }
    int SubIndexPos;
    int IndexPos;
    ocfmRetCode stErrorInfo;
    stErrorInfo = IfSubIndexExists(iNodeID, enumNodeType, (char*)"1F98", (char*)"03", &SubIndexPos, &IndexPos);
    if(stErrorInfo.code != OCFM_ERR_SUCCESS)
    {
        return;
    }

    CSubIndex* pobjSIndex;
    CIndexCollection* pobjIdxCol;
            
    CNode *pobjNode;
    CNodeCollection *objNodeCollection = NULL;

    
    objNodeCollection = CNodeCollection::getNodeColObjectPointer();
    pobjNode = objNodeCollection->getNodePtr(enumNodeType, iNodeID);
    pobjIdxCol = pobjNode->getIndexCollection();
    
    CIndex* pobjIndex;
    
    pobjIndex = pobjIdxCol->getIndexbyIndexValue((char*)"1F98");
    if(pobjIndex == NULL)
    {
        return;
    }


    pobjSIndex = pobjIndex->getSubIndexbyIndexValue((char*)"03");
    if(pobjSIndex == NULL)
    {
        return;
    }
    char *pcValue;
    INT32 iValue = 0;
    bool add25microsec = false;
    if(pobjSIndex->getActualValue() == NULL || strcmp(pobjSIndex->getActualValue(),"") == 0)
    {
        if(pobjSIndex->getDefaultValue() == NULL || strcmp(pobjSIndex->getDefaultValue(),"") == 0)
        {
            pcValue = new char[strlen("250000") + ALLOC_BUFFER];
            strcpy(pcValue, "25000");
        }
        else
        {
            pcValue = new char[strlen(pobjSIndex->getDefaultValue()) + ALLOC_BUFFER];
            strcpy(pcValue, pobjSIndex->getDefaultValue());
            add25microsec = true;
        }
    }
    else
    {
        pcValue = new char[strlen(pobjSIndex->getActualValue()) + ALLOC_BUFFER];
        strcpy(pcValue, pobjSIndex->getActualValue());
    }

    if (strncmp(pcValue,"0x",2) == 0 || strncmp(pcValue,"0X",2) == 0)
        iValue  = hex2int(subString(pcValue, 2, strlen(pcValue) -2));
    else
        iValue  = atoi(pcValue);

    if(true == add25microsec)
        iValue += 25000;

    //add with 25 micro sec
    char *convValue = new char[30];
    
    convValue = _IntToAscii(iValue, convValue, 10);
    pobjNode->setPollResponseTimeout(convValue);
    delete [] pcValue;
    delete [] convValue;
}

/****************************************************************************************************
* Function Name: CopyCustomErrorString
* Description:
* Return value: void
****************************************************************************************************/
void CopyCustomErrorString(ocfmRetCode* stRet, char* pcCustomErrString)
{
    if(NULL != pcCustomErrString)
        return;

    stRet->errorString = new char[strlen(pcCustomErrString) + ALLOC_BUFFER];
    strcpy(stRet->errorString, pcCustomErrString);
}

/****************************************************************************************************
* Function Name: CheckIfMultiplexedCNExist
* Description:
* Return value: void
****************************************************************************************************/
bool CheckIfMultiplexedCNExist()
{
    CNodeCollection *objNodeCol;
    objNodeCol= CNodeCollection::getNodeColObjectPointer();
    CNode* objNode;
    for(INT32 iLoopCount = 0; iLoopCount < objNodeCol->getNumberOfNodes(); iLoopCount++)
    {
        objNode = objNodeCol->getNodebyColIndex(iLoopCount);
        if (objNode->getNodeType() == CN )
        {
            if (objNode->getStationType() == MULTIPLEXED)
            {
                return true;
            }
        }
    }
    return false;
}

/****************************************************************************************************
* Function Name: RecalculateCNPresTimeout
* Description:
* Return value: void
****************************************************************************************************/
void RecalculateCNPresTimeout(char* pbSubIndexId)
{
    ocfmRetCode stErrorInfo;
    int iSubIndexPos, iIndexPos;

    stErrorInfo = IfSubIndexExists(MN_NODEID, MN, (char*)"1F92", pbSubIndexId, &iSubIndexPos, &iIndexPos);
    if(stErrorInfo.code == OCFM_ERR_SUCCESS)
    {
            //iSubIndexPos = Tmp_stErrorInfo.returnValue;
    }
    else
    {
        return;
    }
    char* pcMNIndValue = new char[50];
    GetSubIndexAttributes(MN_NODEID, MN, (char*)"1F92", pbSubIndexId, ACTUALVALUE, pcMNIndValue);

    if( NULL == pcMNIndValue )
        return;

    INT32 iCNNodeId = hex2int(pbSubIndexId);
//printf("\npbSubIndexId=%s iCNNodeId=%d\n", pbSubIndexId, iCNNodeId);
    stErrorInfo = IfIndexExists(iCNNodeId, CN, (char*)"1F98", &iIndexPos);
    char* strConvertedValue;
    strConvertedValue = new char[SUBINDEX_LEN];
    strcpy(strConvertedValue, pbSubIndexId);

    char* subIndName = new char[50];
    subIndName[0] = 0;
    INT32 iCNsubIndFlag = 0;
    if(stErrorInfo.code == OCFM_ERR_SUCCESS)
    {
        //Index exists
        int subIndexPos;        
        stErrorInfo = IfSubIndexExists(iCNNodeId, CN, (char*)"1F98", (char*)"03",  &iSubIndexPos, &iIndexPos);
        if(stErrorInfo.code == OCFM_ERR_SUCCESS)
        {

        }
        else
        {   
            stErrorInfo = AddSubIndex(iCNNodeId, CN, (char*)"1F98", (char*)"03");
        }
    }
    else
    {
        stErrorInfo = AddIndex(iCNNodeId, CN, (char*)"1F98");
        stErrorInfo = AddSubIndex(iCNNodeId, CN, (char*)"1F98", (char*)"03");      
    }

    GetSubIndexAttributes(iCNNodeId, CN, (char*)"1F98", (char*)"03", NAME, subIndName);
    char* subIndFlag = new char[10];
    GetSubIndexAttributes(iCNNodeId, CN, (char*)"1F98", (char*)"03", FLAGIFINCDC, subIndFlag);
    iCNsubIndFlag  = atoi(subIndFlag);

    SetSubIndexAttributes(iCNNodeId, CN, (char*)"1F98", (char*)"03", pcMNIndValue, subIndName, (EFlag)iCNsubIndFlag);
    delete[] subIndName;
    delete[] subIndFlag;
    delete[] pcMNIndValue;

}

void UpdateMNNodeAssignmentIndex(CNode *pobjNode, INT32 CNsCount, char* pcIndex, bool allowMNSubindex)
{ 	
	if(NULL == pcIndex)
		return;

	CIndexCollection *pobjIdxCol = NULL;
	ocfmRetCode retCode;
	CIndex *pobjIndex;
	char* pbIndexNo = new char[3];
				
	pobjIdxCol = pobjNode->getIndexCollection();
	char* pbMNIndex = new char[INDEX_LEN + ALLOC_BUFFER];
	char* pbSidx =  new char[SUBINDEX_LEN + ALLOC_BUFFER];
		/* Add 1F81*/
	strcpy(pbMNIndex, pcIndex);
	retCode = AddIndex(MN_NODEID, MN, pbMNIndex);
	//printf("cn count %d", CNsCount);

	if(retCode.code == OCFM_ERR_SUCCESS || retCode.code == OCFM_ERR_INDEX_ALREADY_EXISTS)
	{				
		pobjIndex = pobjIdxCol->getIndexbyIndexValue(pcIndex);
				/* $:set Flag to true*/
		pobjIndex->setFlagIfIncludedCdc(TRUE);	
		//pobjIndex->deleteSubIndexCollection();
		CSubIndex* pobjSubIndex;
		for(INT32 iSidxCount = pobjIndex->getNumberofSubIndexes()-1; iSidxCount >= 0; iSidxCount--)
		{
			pobjSubIndex = pobjIndex->getSubIndex(iSidxCount);
			if(NULL == pobjSubIndex)
				continue;

			if(0 == strcmp((char*)pobjSubIndex->getIndexValue(), "00"))
				continue;

			try
            {
                //INT32 iValue = 0;
                INT32 iNodeidValue  = hex2int((char*)pobjSubIndex->getIndexValue());
                ENodeType iNodeType;
                if(MN_NODEID == iNodeidValue)
                {
                    iNodeType = MN;
                } 
                else
                {
                    iNodeType = CN;
                }
                INT32 iNodePos;
                bool bFlag = false;
                retCode = IfNodeExists(iNodeidValue, iNodeType, &iNodePos, bFlag);
                if(OCFM_ERR_SUCCESS == retCode.code && true == bFlag && ((CN == iNodeType) || (true == allowMNSubindex)) )
                {
                }
                else
                {
                    //pobjSubIndex->setActualValue("0");
                    pobjIndex->deleteSubIndex(iSidxCount);
                }
            }
            catch(ocfmException* ex)
            {
                //return ex->_ocfmRetCode;
                //pobjSubIndex->setActualValue("0");
                pobjIndex->deleteSubIndex(iSidxCount);
            }
			
		}
		strcpy(pbSidx, "00");
		pbIndexNo = _IntToAscii(CNsCount, pbIndexNo, 16);
		pbIndexNo = padLeft(pbIndexNo, '0', 2);		
		SetSIdxValue(pbMNIndex, pbSidx, pbIndexNo,pobjIdxCol, pobjNode->getNodeId(), MN, false);					
	}	
}

bool ValidateCNPresTimeout(char* pbSubIndexId, char* pcCheckValue)
{
//printf("\nValidateCNPresTimeout pbSubIndexId=%s pcCheckValue=%s\n", pbSubIndexId, pcCheckValue);
    ocfmRetCode stErrorInfo, stRet;
    int iSubIndexPos, iIndexPos;

    //stRet.code = OCFM_ERR_LOW_CNPRESTIMEOUT;
    stErrorInfo = IfSubIndexExists(MN_NODEID, MN, (char*)"1F92", pbSubIndexId, &iSubIndexPos, &iIndexPos);
    if(stErrorInfo.code == OCFM_ERR_SUCCESS)
    {
            //iSubIndexPos = Tmp_stErrorInfo.returnValue;
    }
    else
    {
        return false;
    }

    INT32 iCNNodeId = hex2int(pbSubIndexId);
    stErrorInfo = IfSubIndexExists(iCNNodeId, CN, (char*)"1F98", (char*)"03",  &iSubIndexPos, &iIndexPos);
    char* strConvertedValue;
    strConvertedValue = new char[SUBINDEX_LEN];
    strcpy(strConvertedValue, pbSubIndexId);

    char* pcCNDefaultValue = new char[50];
    if(stErrorInfo.code == OCFM_ERR_SUCCESS)
    {
        GetSubIndexAttributes(iCNNodeId, CN, (char*)"1F98", (char*)"03", DEFAULTVALUE, pcCNDefaultValue);
    }
    else
    {
        return false; 
    }

    if( !( NULL == pcCheckValue || strcmp(pcCheckValue, "") == 0 ) ) 
    {
        if( !( NULL == pcCNDefaultValue || strcmp(pcCNDefaultValue, "") == 0 ) )
        {
            INT32 minimumDefaultValue = GetDecimalValue(pcCNDefaultValue);
            INT32 iCheckValue = GetDecimalValue(pcCheckValue);
            if(iCheckValue >= minimumDefaultValue)
            {
                return true;
            }
            else
            {
                return false;
            }
        }
        else
        {
            return false;
        }
    }
    else
    {
        return false;
    }

}
