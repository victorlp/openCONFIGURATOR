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
	#include <time.h>
#else
	#include <sys/stat.h>
#endif
#include "../Include/openCONFIGURATOR.h"
#include "../Include/Exception.h"
#include "../Include/Internal.h"
#include "../Include/ObjectDictionary.h"
#include "../Include/ProcessImage.h"
#include "../Include/Declarations.h"

/****************************************************************************************************
* Defines
****************************************************************************************************/

#define MY_ENCODING "UTF-8"
#define CDC_BUFFER 5000
#define CDC_MN_BUFFER 200000
//#define PI_VAR_COUNT 4000
#define MAX_FILE_PATH_SIZE 500

/****************************************************************************************************
* Global Variables
****************************************************************************************************/
INT32 iLastVarIndex = -1;
static bool bCDTCompleted = false;

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

static sBuildTime sBuildTime_g;
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
	}
		

	
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
			char tmpCmdBuffer[LINUX_INSTALL_DIR_LEN];
			if(!ObjectDictLoaded)
			{
				#if defined(_WIN32) && defined(_MSC_VER)
					LoadObjectDictionary((char*) "od.xml");
				#else
				{
					sprintf(tmpCmdBuffer, "%s/od.xml", LINUX_INSTALL_DIR);
					LoadObjectDictionary(tmpCmdBuffer);
				}
				#endif
				ObjectDictLoaded = true;
			}
			uiCycleNumber = 0;
		}
		if(enumNodeType == CN)
		{
			bool bFlag = false;
			stErrorInfo = IfNodeExists(iNodeID, enumNodeType, &iNodePos, bFlag );
								
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
		objNode.setNodeId(iNodeID);
		objNode.setNodeType(enumNodeType);
		objNode.setNodeName(pbNodeName);
		
		objNode.CreateIndexCollection();
		objNode.CreateDataTypeCollection();
		objNode.CreateApplicationProcess();
		objNode.CreateNetworkManagament();

		pobjNodeCollection = CNodeCollection::getNodeColObjectPointer();
		pobjNodeCollection->addNode(objNode);
		
		
		stErrorInfo.code = OCFM_ERR_SUCCESS;
		return stErrorInfo;
	}
	catch(ocfmException* ex)
	{
		stErrorInfo = ex->_ocfmRetCode;
		return stErrorInfo;
	}
}

/****************************************************************************************************
* Function Name: NewProjectNode
* Description:
* Return value: ocfmRetCode
****************************************************************************************************/
ocfmRetCode NewProjectNode(INT32 iNodeID, ENodeType enumNodeType, char* pbNodeName, char * pbImportXmlFile)
{
    ocfmRetCode stErrorInfo;
    CNode objNode;
    CNodeCollection *pobjNodeCollection;
    INT32 iNodePos;
    
    try
    {
        stErrorInfo = CreateNode(iNodeID, enumNodeType, pbNodeName);
        if(OCFM_ERR_SUCCESS != stErrorInfo.code)
            return stErrorInfo;


        stErrorInfo = ImportXML(pbImportXmlFile, iNodeID, enumNodeType);
        if(OCFM_ERR_SUCCESS != stErrorInfo.code)
            return stErrorInfo;

        if(MN == enumNodeType)
        {
            //set the loss of SoC tolerance to 50 ms
            INT32 iIndexPos;
            stErrorInfo = IfIndexExists(iNodeID, enumNodeType, (char *)"1C14", &iIndexPos);
            if(OCFM_ERR_SUCCESS == stErrorInfo.code)
            {
                CIndex* pobjIndex;
        
                pobjIndex = getMNIndexValues((char*) "1C14");
                if( (NULL != pobjIndex) && (NULL == pobjIndex->getActualValue()))
                {
                    pobjIndex->setActualValue((char *)"50000000");
                }
            }
        }
        stErrorInfo.code = OCFM_ERR_SUCCESS;
        return stErrorInfo;

    }
    catch(ocfmException* ex)
    {
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
				// Node Doesn't Exist
				
			}
			return stErrorInfo;
		}
		

		CNode objNode;		
		CNodeCollection *pobjNodeCollection;
		pobjNodeCollection= CNodeCollection::getNodeColObjectPointer();	
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
				SetSubIndexAttributesByAttribute(MN_NODEID, MN, (char *)MULTIPL_CYCLE_ASSIGN_OBJECT, cSIdx, ACTUALVALUE, (char*)"");
            }
            stErrStructInfo = IfSubIndexExists(MN_NODEID, MN, (char *)MNCN_POLLRESPONSE_TIMEOUT_OBJECT, cSIdx, &subIndexPos, &IndexPos);
            if(stErrStructInfo.code == OCFM_ERR_SUCCESS)
            {
				SetSubIndexAttributesByAttribute(MN_NODEID, MN, (char *)MULTIPL_CYCLE_ASSIGN_OBJECT, cSIdx, ACTUALVALUE, (char*)"");
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
				//continue with process
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
			
			
			pobjIndex->deleteSubIndex(iSubIndexPos);
			
				/* Update subindex "00"*/
			if(pobjSIDx != NULL)
			{
				int iTotalSIdxs = 0;
				/* subindexes excluding "00"*/
				iTotalSIdxs = pobjIndex->getNumberofSubIndexes() - 1;
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
cout<<"----------AddSubIndex--------"<<endl;
		CNode objNode;		
		CNodeCollection *pobjNodeCollection;
		CIndexCollection *pobjIndexCollection;
		CIndex objIndex;
		INT32 iSubIndexPos;
		INT32 iIndexPos;
		ocfmRetCode stErrorInfo;
		
		try
		{

			stErrorInfo = IfSubIndexExists(iNodeID, enumNodeType, pbIndexID, pbSubIndexID, &iSubIndexPos, &iIndexPos);
			if( stErrorInfo.code == OCFM_ERR_SUCCESS )
			{
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

				/* Get the SubIndex from ObjectDictionary*/
				CObjectDictionary* pobjOBD;
				pobjOBD = CObjectDictionary::getObjDictPtr();
				

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
        stErrorInfo = AddSubIndex(iNodeID, enumNodeType, pbIndexID, (char*)"00");
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
            
        
            stErrorInfo = IfSubIndexExists(iNodeID, enumNodeType, pbIndexID, (char*)"00", &iSubIndexPos, &iIndexPos);
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
			stErrorInfo = IfIndexExists(iNodeID, enumNodeType, pbIndexID, &iIndexPos);
			
			if( stErrorInfo.code == OCFM_ERR_SUCCESS )
			{
				ocfmException objException;				
				objException.ocfm_Excpetion(OCFM_ERR_INDEX_ALREADY_EXISTS);
				throw objException;
			}
			
			if( (stErrorInfo.code == OCFM_ERR_NO_INDEX_FOUND) || (stErrorInfo.code == OCFM_ERR_INDEXID_NOT_FOUND) )
			{	
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
					//update the no of entries for the subindex added
					UpdateNumberOfEnteriesSIdx(&objIndex, enumNodeType);
					pobjIndexCollection->addIndex(objIndex);
				}
				else if((enumNodeType == MN) && CheckIfProcessImageIdx(pbIndexID))
				{	
						CIndex objIndex;
						objIndex.setNodeID(iNodeID);
						objIndex.setIndexValue(pbIndexID);
						pobjIndexCollection->addIndex(objIndex);
					 
				}
				else if(CheckIfManufactureSpecificObject(pbIndexID))
				{	
						CIndex objIndex;
						objIndex.setNodeID(iNodeID);
						objIndex.setIndexValue(pbIndexID);
						objIndex.setFlagIfIncludedCdc(TRUE);
						pobjIndexCollection->addIndex(objIndex);
				}
				else 
				{
					ocfmException objException;				
					objException.ocfm_Excpetion(OCFM_ERR_INVALID_INDEXID);
					throw objException;
				}
								
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
			//continue wih operation
		}
		else
		{
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
			if(pobjIndexPtr->IsIndexVaueValid(pbIndexValue))
			{
				pobjIndexPtr->setActualValue(pbIndexValue);
				
				stErrorInfo.code = OCFM_ERR_SUCCESS;
			}
			else
			{
					ocfmException objException;				
					objException.ocfm_Excpetion(OCFM_ERR_VALUE_NOT_WITHIN_RANGE);
					throw objException;
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
		
		try
		{
			stErrorInfo = IfSubIndexExists(iNodeID, enumNodeType, pbIndexID, pbSubIndexID, &iSubIndexPos, &iIndexPos);
			if(stErrorInfo.code == OCFM_ERR_SUCCESS)
			{
				//continue with process
			}
			else
			{
				ocfmException objException;				
				objException.ocfm_Excpetion(OCFM_ERR_INVALID_SUBINDEXID);
				throw objException;
			}
			
            if((iNodeID == 240) && (enumNodeType == MN ) && (strcmp(pbIndexID,"1F92") == 0) && (strcmp(pbSubIndexID,"00") != 0) )
            {
///////////////////////////////////////////////////////////////////////////
cout<<"--------------SetSubIndexAttributes-----------1f92"<<endl;
                if(false == ValidateCNPresTimeout(pbSubIndexID, pbIndexValue))
                {
cout<<"--------------SetSubIndexAttributes-----------1f92---false"<<endl;
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
			
			if(pbIndexName !=NULL)
			{
				pobjSubIndexPtr->setName(pbIndexName);
			}
			
			pobjSubIndexPtr->setFlagIfIncludedCdc(enumIsIncludedInCdc);
			
			if(pbIndexValue != NULL)
			{
			if(pobjSubIndexPtr->IsIndexVaueValid(pbIndexValue))		
				{
				pobjSubIndexPtr->setActualValue(pbIndexValue);
				stErrorInfo.code = OCFM_ERR_SUCCESS;
				}
			}
			else
			{
				ocfmException objException;				
				objException.ocfm_Excpetion(OCFM_ERR_VALUE_NOT_WITHIN_RANGE);
				throw objException;
			}
			if((iNodeID == 240) && (enumNodeType == MN ) && (strcmp(pbIndexID,"1F98") == 0) && (strcmp(pbSubIndexID,"07") == 0) )
			{
				RecalculateMultiplex();
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
			//continue with process
		}
		else
		{
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
			pobjIndexPtr->setActualValue(pbActualValue);
			
			stErrorInfo.code = OCFM_ERR_SUCCESS;
		}
		else
		{
				ocfmException objException;				
				objException.ocfm_Excpetion(OCFM_ERR_VALUE_NOT_WITHIN_RANGE);
				throw objException;
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
	* Function Name: SetAllSubIndexAttributes
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
				//continue with process
			}
			else
			{
				ocfmException objException;				
				objException.ocfm_Excpetion(OCFM_ERR_INVALID_SUBINDEXID);
				throw objException;
			}
			
			CSubIndex* pobjSubIndexPtr;
				
			objIndex.setNodeID(objNode.getNodeId());
			pobjNodeCollection= CNodeCollection::getNodeColObjectPointer();
			objNode = pobjNodeCollection->getNode(enumNodeType, iNodeID);

			pobjIndexCollection = objNode.getIndexCollection();
			pobjSubIndex =pobjIndexCollection->getIndex(iIndexPos);
			
			pobjSubIndexPtr = pobjSubIndex->getSubIndex(iSubIndexPos);						
			
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
					pobjSubIndexPtr->setActualValue(pbActualValue);
					stErrorInfo.code = OCFM_ERR_SUCCESS;
				}
				else
				{
					ocfmException objException;				
					objException.ocfm_Excpetion(OCFM_ERR_VALUE_NOT_WITHIN_RANGE);
					throw objException;
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
			

			return stErrorInfo;
		}
		catch(ocfmException& ex)
		{
			return ex._ocfmRetCode;
		}
}
/**************************************************************************************************
* Function Name: SetSubIndexAttributesByAttribute
* Description:
* Return value: ocfmRetCode
****************************************************************************************************/
ocfmRetCode SetSubIndexAttributesByAttribute(
	INT32 			iNodeID, 
	ENodeType 		enumNodeType, 
	char* 			pbIndexID, 
	char* 			pbSubIndexID, 
	EAttributeType 	enumAttributeType, 
	char* 			pbInAttributeValue)
{
		CNode objNode;		
		CNodeCollection *pobjNodeCollection;
		CIndexCollection *pobjIndexCollection;
		CIndex* pobjIndex;
		INT32 iSubIndexPos;
		INT32 iIndexPos;
		ocfmRetCode stErrorInfo;
		
		try
		{
		
			stErrorInfo = IfSubIndexExists(iNodeID, enumNodeType, pbIndexID, pbSubIndexID, &iSubIndexPos, &iIndexPos);
			if(stErrorInfo.code == OCFM_ERR_SUCCESS)
			{
				//continue with process
			}
			else
			{
				return stErrorInfo;
			}

			pobjNodeCollection= CNodeCollection::getNodeColObjectPointer();
			objNode = pobjNodeCollection->getNode(enumNodeType, iNodeID);

			pobjIndexCollection = objNode.getIndexCollection();
			pobjIndex =pobjIndexCollection->getIndex(iIndexPos);
			

			CSubIndex* pobjSubIndexPtr;
			pobjSubIndexPtr = pobjIndex->getSubIndex(iSubIndexPos);	
			
			switch(enumAttributeType)
			{
				case NAME:						
						// No Operation
						break;
				case OBJECTTYPE:
						// No Operation
						break;
				case DATATYPE:
						// No Operation
						break;		
				case ACCESSTYPE:
						// No Operation
						break;
				case DEFAULTVALUE:
						// No Operation
						break;
				case ACTUALVALUE:
						if(NULL != pbInAttributeValue)
							pobjSubIndexPtr->setActualValue(pbInAttributeValue);
						else
							pobjSubIndexPtr->setActualValue((char *)"");
						break;
				case PDOMAPPING:
						// No Operation
						break;
				case LOWLIMIT:
						// No Operation
						break;
				case HIGHLIMIT:
						// No Operation
						break;
				case FLAGIFINCDC:
						// No Operation
						break;							
				default:
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
					}						
			}
		}
		else
		{
				return;
		}
			
	}	
	
/****************************************************************************************************
* Function Name: EnableDisableMappingPDO
* Description:
* Return value: void
****************************************************************************************************/
void EnableDisableMappingPDO(CIndexCollection* pobjIdxCol, CIndex* objIndex, char* Buffer, bool EnableFlag)
{
            int len;
            bool IfStringDT = false;
            //Get the Index Value       
            
            strcpy(Buffer,"");
            if(objIndex->getNumberofSubIndexes() != 0 )
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
                            noOfSubIndexes = hex2int(subString((char*)objSubIndex->getActualValue(), 2, strlen(objSubIndex->getActualValue()) -2));
                            
                        }
                        else
                            noOfSubIndexes = atoi(objSubIndex->getActualValue());

                        /* No need to reinitailize mapping pdo to zero again */
                        if(noOfSubIndexes ==0 && true == EnableFlag)
                            return;

                        noOfSubIndexes = noOfSubIndexes + 1;
                    }
                }
                int noOfValidSubIndexes = 0;
                int noOFPdoAddedSubindex = 0;
                    
                if( NULL == objSubIndex )
                    return;                

                if((objSubIndex->getActualValue() != NULL) && (objSubIndex->getFlagIfIncludedCdc() == TRUE)  && ((true == ReactivateMappingPDO(pobjIdxCol, objIndex)) || (true == IsDefaultActualNotEqual(objSubIndex))) )
                {

                        strcat(Buffer,objIndex->getIndexValue());
                        //Place a tab           
                        len = strlen(Buffer);
                        strcat(Buffer,"\t");                
    
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
                        else strcat(Buffer,"00000000");
                            delete[] size;
                        strcat(Buffer,"\t");
            
                    if(false == EnableFlag)
                    {
                        if (objSubIndex->getActualValue()!=NULL)
                        {

                                char actvalue[20];
                                strcpy(actvalue,"0");
                                strcat(Buffer,padLeft(actvalue,'0',padLength));
                                resetValueAdded = true;
                        }
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
                                            
                                                strcat(Buffer,padLeft(actvalue,'0',padLength));
                                            
                                            }
                                                else
                                                {               
                                                    strcpy(actvalue, _IntToAscii(atoi(objSubIndex->getActualValue()),actvalue,16));
                                        
                                                    strcat(Buffer,padLeft(actvalue, '0', padLength));
                                                }
                                            }
                    }
                        strcat(Buffer,"\n");
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
* Function Name: UpdateCNSoCTolerance
* Description: Updates the Loss SoC Tolerance(1C14) of the CN
* Return value: void
****************************************************************************************************/

    void UpdateCNSoCTolerance(CIndexCollection  *pobjIdxCol,char* pbSocTolerance)
    {
        CIndex* pobjIndex;

        pobjIndex = pobjIdxCol->getIndexbyIndexValue((char*) "1C14");
        if(pobjIndex!= NULL)
        {
            pobjIndex->setActualValue(pbSocTolerance);
			pobjIndex->setFlagIfIncludedCdc(TRUE);
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

    bCopy1F81 = ISCNNodeAssignmentValid(pobjNode);
        
    if(true == bCopy1F81)
    {
        CIndexCollection* objCNIndexCollection;
        CIndex* objCNIndex;
        objCNIndexCollection = pobjNode->getIndexCollection();
        objCNIndex = objCNIndexCollection->getIndexbyIndexValue((char*)"1F81");
        if(objCNIndex == NULL)
        {
            return;
        }
        objCNIndex->setFlagIfIncludedCdc(TRUE);

        CIndex* objMNIndex;
        objMNIndex = getMNIndexValues((char*)"1F81");
        CSubIndex* objCNSubIndex;
        CSubIndex* objMNSubIndex;

        for(INT32 iLoopCount = 0; iLoopCount < objCNIndex->getNumberofSubIndexes(); iLoopCount++)
        {           
            objCNSubIndex = objCNIndex -> getSubIndex(iLoopCount);
            if(NULL == objCNSubIndex)
                continue;
            
            objMNSubIndex = objMNIndex -> getSubIndexbyIndexValue ( (char *)objCNSubIndex -> getIndexValue() );

            if(NULL == objMNSubIndex)
                continue;
            
            if(NULL != objMNSubIndex->getActualValue() )
            {
                objCNSubIndex->setActualValue((char*)objMNSubIndex->getActualValue());
            }
            else
            {
                objCNSubIndex -> setActualValue((char*)"");
            }
			objCNSubIndex->setFlagIfIncludedCdc(TRUE);
          
        }
    }
}

/****************************************************************************************************
* Function Name: ISCNNodeAssignmentValid
* Description: checks whether the CN can implement 1F81 object
* Return value: bool
****************************************************************************************************/
bool ISCNNodeAssignmentValid(CNode*  pobjNode)
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
        if( OCFM_ERR_SUCCESS == (GetSubIndexAttributes(iNodeId, enumNodeType, (char*)"1F98", (char*)"07", ACTUALVALUE, pcMultiplCycleCnt)).code )
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
    
    return bCopy1F81;
}
/****************************************************************************************************
* Function Name: UpdateCNMultipleCycleAssign
* Description: copies all the subobjects of 1F9B in MN to CN if conditions are satisfied
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
        return;
    }
    objCNIndex->setFlagIfIncludedCdc(TRUE);

    CIndex* objMNIndex;
    objMNIndex = getMNIndexValues((char*)"1F9B");
    CSubIndex *objCNSubIndex, *objMNSubIndex;

    for(INT32 iLoopCount = 0; iLoopCount < objCNIndex->getNumberofSubIndexes(); iLoopCount++)
    {
      
        objCNSubIndex = objCNIndex -> getSubIndex(iLoopCount);
        if(NULL == objCNSubIndex)
            continue;
                
        objMNSubIndex = objMNIndex -> getSubIndexbyIndexValue ( (char *)objCNSubIndex -> getIndexValue() );
    
        if(NULL == objMNSubIndex)
            continue;
                               
        if(NULL != objMNSubIndex -> getActualValue() )
        {
            objCNSubIndex -> setActualValue((char*)objMNSubIndex -> getActualValue());
        }
        else
        {
            objCNSubIndex -> setActualValue((char*)"");
        }
		objCNSubIndex -> setFlagIfIncludedCdc(TRUE);
    }
}

/****************************************************************************************************
* Function Name: UpdateCNPresMNActLoad
* Description:  set the calculated PresMN load value in CN 1F8D object
* Return value: void
****************************************************************************************************/
void UpdateCNPresMNActLoad(CNode*  pobjNode)
{
        ocfmRetCode stErrStructInfo;
        CIndex *pobjIndex;
        CSubIndex *pobjSubIndex;
        CIndexCollection *pobjIndexColl;
        INT32 IndexPos=0, subIndexPos=0;
        
        CNodeCollection* objNodeCol;
        objNodeCol = CNodeCollection::getNodeColObjectPointer();

        INT32 iNodeId = pobjNode->getNodeId();
        ENodeType eNodeType =  pobjNode->getNodeType();
        stErrStructInfo = IfSubIndexExists(iNodeId, eNodeType, (char *)"1F8D", (char *)"F0", &subIndexPos, &IndexPos);
        if(OCFM_ERR_SUCCESS != stErrStructInfo.code)
        {
          return;
        }

        char * pcSubindexId = new char[SUBINDEX_LEN+1];
        strcpy(pcSubindexId, (char *)"F0");
        
        pobjIndexColl = pobjNode->getIndexCollection();
        pobjIndex = pobjIndexColl->getIndexbyIndexValue((char *)"1F8D");
        if(NULL != pobjIndex)
        {
            pobjSubIndex = pobjIndex->getSubIndexbyIndexValue((char *)"F0");
            if(NULL != pobjSubIndex)
            {
                if(CHAINED == pobjNode->getStationType())
                {
                    char convalue[20];
                    memset(convalue, 0, 20*sizeof(char));
                    char actvalue[22];
                    memset(actvalue, 0, 22*sizeof(char));

					if( PRES_DEFAULT_PAYLOAD > (objNodeCol->getMNNode()).getPResActPayloadValue() )
					{
						_IntToAscii( PRES_DEFAULT_PAYLOAD , convalue, 16);
					}
					else
					{
						_IntToAscii( (objNodeCol->getMNNode()).getPResActPayloadValue() , convalue, 16);
					}
                    strcpy(actvalue, (char *)"0x");
                    strcat(actvalue, convalue);
                    pobjSubIndex->setActualValue(actvalue);
                    
                    pobjIndex->setFlagIfIncludedCdc(TRUE);
                    pobjSubIndex->setFlagIfIncludedCdc(TRUE);
                }
                else
                {
                    pobjSubIndex->setActualValue((char *)"");
                        
                    pobjIndex->setFlagIfIncludedCdc(FALSE);
                    pobjSubIndex->setFlagIfIncludedCdc(FALSE);
                }
            }
        }
        delete[] pcSubindexId;
}

/****************************************************************************************************
* Function Name: UpdatePreqActLoad
* Description: set the calculated Preq load value in CN 
* Return value: void
****************************************************************************************************/
void UpdatePreqActLoad(CNode*  pobjNode)
{
    ocfmRetCode stErrStructInfo;
    CIndex *pobjIndex;
    CSubIndex *pobjSubIndex;
    CIndexCollection *pobjIndexColl;
    INT32 IndexPos=0, subIndexPos=0;
        
    CNodeCollection* objNodeCol;
    objNodeCol = CNodeCollection::getNodeColObjectPointer();
    
    char convalue[20];
    memset(convalue, 0, 20*sizeof(char));
    char actvalue[22];
    memset(actvalue, 0, 22*sizeof(char));
    
    INT32 iNodeId = pobjNode->getNodeId();
    ENodeType eNodeType =  pobjNode->getNodeType();
    stErrStructInfo = IfSubIndexExists(iNodeId, eNodeType, (char *)"1F98", (char *)"04", &subIndexPos, &IndexPos);
    if(OCFM_ERR_SUCCESS != stErrStructInfo.code)
    {
      return;
    }
    
    char * pcSubindexId = new char[SUBINDEX_LEN+1];
    strcpy(pcSubindexId, (char *)"04");
    pobjIndexColl = pobjNode->getIndexCollection();
    pobjIndex = pobjIndexColl->getIndexbyIndexValue((char *)"1F98");
    if(NULL != pobjIndex)
    {
      pobjSubIndex = pobjIndex->getSubIndexbyIndexValue((char *)"04");
      if(NULL != pobjSubIndex)
      {
		  if( PREQ_DEFAULT_PAYLOAD > pobjNode->getPReqActPayloadValue() )
		  {
			  _IntToAscii( PREQ_DEFAULT_PAYLOAD , convalue, 16);
		  }
		  else
		  {
			  _IntToAscii( pobjNode->getPReqActPayloadValue() , convalue, 16);
		  }
		
          
          strcpy(actvalue, (char *)"0x");
          strcat(actvalue, convalue);
          pobjSubIndex->setActualValue(actvalue);
          pobjIndex->setFlagIfIncludedCdc(TRUE);
          pobjSubIndex->setFlagIfIncludedCdc(TRUE);
          
          //set the value in MN
          if( CN == pobjNode->getNodeType() )
          {
            _IntToAscii( pobjNode->getNodeId(), pcSubindexId, 16);
            pcSubindexId = padLeft(pcSubindexId, '0', 2);

            stErrStructInfo = IfSubIndexExists(MN_NODEID, MN, (char *)"1F8B", pcSubindexId, &subIndexPos, &IndexPos);
            if(OCFM_ERR_SUCCESS != stErrStructInfo.code)
            {
              return;
            }
            pobjIndex = getMNIndexValues((char *)"1F8B");
            if(NULL != pobjIndex)
            {
                pobjSubIndex = pobjIndex->getSubIndexbyIndexValue(pcSubindexId);
                if(NULL != pobjSubIndex)
                {
                  pobjSubIndex->setActualValue(actvalue);
                  pobjIndex->setFlagIfIncludedCdc(TRUE);
                  pobjSubIndex->setFlagIfIncludedCdc(TRUE);
                }
             }
          }
      }
    }
    delete[] pcSubindexId;
}

/****************************************************************************************************
* Function Name: UpdatePresActLoad
* Description: set the calculated Pres load value in CN 
* Return value: void
****************************************************************************************************/
void UpdatePresActLoad(CNode*  pobjNode)
{
    ocfmRetCode stErrStructInfo;
    CIndex *pobjIndex;
    CSubIndex *pobjSubIndex;
    CIndexCollection *pobjIndexColl;
    INT32 IndexPos=0, subIndexPos=0;
        
    CNodeCollection* objNodeCol;
    objNodeCol = CNodeCollection::getNodeColObjectPointer();
    
    char convalue[20];
    memset(convalue, 0, 20*sizeof(char));
    char actvalue[22];
    memset(actvalue, 0, 22*sizeof(char));
    
    INT32 iNodeId = pobjNode->getNodeId();
    ENodeType eNodeType =  pobjNode->getNodeType();
    stErrStructInfo = IfSubIndexExists(iNodeId, eNodeType, (char *)"1F98", (char *)"05", &subIndexPos, &IndexPos);
    if(OCFM_ERR_SUCCESS != stErrStructInfo.code)
    {
      return;
    }
    
    char * pcSubindexId = new char[SUBINDEX_LEN+1];
    strcpy(pcSubindexId, (char *)"05");
    pobjIndexColl = pobjNode->getIndexCollection();
    pobjIndex = pobjIndexColl->getIndexbyIndexValue((char *)"1F98");
    if(NULL != pobjIndex)
    {
      pobjSubIndex = pobjIndex->getSubIndexbyIndexValue((char *)"05");
      if(NULL != pobjSubIndex)
      {
		  if( PRES_DEFAULT_PAYLOAD > pobjNode->getPResActPayloadValue() )
		  {
			  _IntToAscii( PRES_DEFAULT_PAYLOAD , convalue, 16);
		  }
		  else
		  {
			  _IntToAscii( pobjNode->getPResActPayloadValue() , convalue, 16);
		  }
        
        strcpy(actvalue, (char *)"0x");
        strcat(actvalue, convalue);
        pobjSubIndex->setActualValue(actvalue);
        pobjIndex->setFlagIfIncludedCdc(TRUE);
        pobjSubIndex->setFlagIfIncludedCdc(TRUE);
          
          //set the value in MN
        if( CN == pobjNode->getNodeType() )
        {
          _IntToAscii( pobjNode->getNodeId(), pcSubindexId, 16);
          pcSubindexId = padLeft(pcSubindexId, '0', 2);

          
          stErrStructInfo = IfSubIndexExists(MN_NODEID, MN, (char *)"1F8D", pcSubindexId, &subIndexPos, &IndexPos);
          if(OCFM_ERR_SUCCESS != stErrStructInfo.code)
          {
            return;
          }
          pobjIndex = getMNIndexValues((char *)"1F8D");
          if(NULL != pobjIndex)
          {
            pobjSubIndex = pobjIndex->getSubIndexbyIndexValue(pcSubindexId);
            if(NULL != pobjSubIndex)
            {
              pobjSubIndex->setActualValue(actvalue);
              pobjIndex->setFlagIfIncludedCdc(TRUE);
              pobjSubIndex->setFlagIfIncludedCdc(TRUE);
            }
          }
        }
      }
    }
    delete[] pcSubindexId;
}

/****************************************************************************************************
* Function Name: UpdateCNVisibleNode
* Description: Updates 1F81 and 1F8D of cross trafficked station 
* Return value: void
****************************************************************************************************/
void UpdateCNVisibleNode(CNode*  pobjNode) throw(ocfmException)
{
    CIndexCollection* objPDOCollection;
    objPDOCollection = pobjNode->getPDOIndexCollection();
    
    if(NULL == objPDOCollection)
    {
      return;
    }

    CIndex *pobjIndex, *pobjCommIndex;
    CSubIndex *pobjEntriesSubIndex, *pobjNodeIDSubIndex;
    char *pcIdx;
    char *pcCommIdx = new char[INDEX_SIZE];
    char *pcMappedNodeId;
    
    int iCrossStnCnt = 0;
	ResetAllSubIndexFlag ( (pobjNode -> getIndexCollection()) -> getIndexbyIndexValue( (char *)"1F81" ) );
	ResetAllSubIndexFlag ( (pobjNode -> getIndexCollection()) -> getIndexbyIndexValue( (char *)"1F8D" ) );

    
    for(INT32 iLoopCount = 0; iLoopCount < objPDOCollection -> getNumberofIndexes(); iLoopCount++)
    {
        pobjIndex = objPDOCollection -> getIndex(iLoopCount);
        if( (NULL != pobjIndex) && (0 == strncmp(pobjIndex->getIndexValue(), "16", 2)) )
        {
            pobjEntriesSubIndex = pobjIndex -> getSubIndexbyIndexValue( (char *)"00" );
            
            if(NULL != pobjEntriesSubIndex)
            {
                if ( (NULL != pobjEntriesSubIndex -> getActualValue()) && (0 != strcmp( pobjEntriesSubIndex -> getActualValue(), "" )) && !( checkIfValueZero((char*)pobjEntriesSubIndex -> getActualValue())) )
                {   
                    pcIdx = subString((char *)pobjIndex->getIndexValue(), 2, 2);
                    strcpy(pcCommIdx, (char *)"14");
                    strcat(pcCommIdx, pcIdx);
                    delete [] pcIdx;
                    
                    pobjCommIndex = objPDOCollection->getIndexbyIndexValue(pcCommIdx);
                    if(NULL != pobjCommIndex)
                    {
                        pobjNodeIDSubIndex = pobjCommIndex->getSubIndexbyIndexValue((char *)"01");
                        if(NULL != pobjNodeIDSubIndex)
                        {
                            if ( (NULL != pobjNodeIDSubIndex -> getActualValue()) && (0 != strcmp( pobjNodeIDSubIndex -> getActualValue(), "" )) && !(checkIfValueZero ( (char*)pobjNodeIDSubIndex -> getActualValue())) )
                            {   
                                iCrossStnCnt++;
                                if(MAX_CN_CROSS_TRAFFIC_STN < iCrossStnCnt)
                                {
                                    ocfmException objocfmException;
                                    objocfmException._ocfmRetCode.code = OCFM_ERR_CN_EXCEEDS_CROSS_TRAFFIC_STN;
                                    char acCustomError[200] = {0};
                                    sprintf(acCustomError, "The cross trafficking in CN Node ID:%d excceds the maximum permissible station %d",  pobjNode->getNodeId(), MAX_CN_CROSS_TRAFFIC_STN);
                                    CopyCustomErrorString(&(objocfmException._ocfmRetCode), acCustomError);
    
                                    throw objocfmException;
                                }
                                  
                                //copy the MN objects
                                if(CheckIfHex((char*)pobjNodeIDSubIndex -> getActualValue()))
                                {
                                    pcMappedNodeId = subString((char *)pobjNodeIDSubIndex -> getActualValue(), 2, 2);
                                }
                                else
                                {
                                    pcMappedNodeId = new char[SUBINDEX_LEN];
                                    strcpy(pcMappedNodeId, pobjNodeIDSubIndex -> getActualValue());
                                }
                                pcMappedNodeId = padLeft(pcMappedNodeId, '0', 2);
                                
                                if( true == ISCNNodeAssignmentValid( pobjNode ))
                                {
                                    if( true == CopyMNSubindexToCN( pobjNode, (char *)"1F81", pcMappedNodeId))
                                    {
                                        CopyMNSubindexToCN( pobjNode, (char *)"1F8D", pcMappedNodeId);
                                    }
                                }
                                
                                delete [] pcMappedNodeId;
                            }
                        }
                    }
                }
            }
        }
    }

}

/****************************************************************************************************
* Function Name: CopyMNSubindexToCN
* Description: copies the MN subindex value into corresponding CN subindex
* Return value: bool
****************************************************************************************************/
bool CopyMNSubindexToCN(CNode*  pobjNode, char *pcIndex, char *pcSubIndex)
{
    bool bCopied = false;
    
    CIndexCollection *pobjIndexCollection;
    CIndex *pobjMNIndex, *pobjCNIndex;
    CSubIndex *pobjMNSubindex, *pobjCNSubindex;
    
    pobjIndexCollection = pobjNode->getIndexCollection();
    pobjCNIndex = pobjIndexCollection -> getIndexbyIndexValue(pcIndex);
    if(NULL != pobjCNIndex)
    {
        pobjCNSubindex = pobjCNIndex -> getSubIndexbyIndexValue( pcSubIndex );
        if(NULL != pobjCNSubindex)
        {
            
        }
        else
        {
            return bCopied;
        }
      
    }
    else
    {
        return bCopied;
    }
    
    pobjMNIndex = getMNIndexValues(pcIndex);
    if(NULL != pobjMNIndex)
    {
        pobjMNSubindex = pobjMNIndex -> getSubIndexbyIndexValue( pcSubIndex );
        if(NULL != pobjMNSubindex)
        {
                
        }
        else
        {
            return bCopied;
        }
      
    }
    else
    {
        return bCopied;
    }
    
    if(NULL != pobjMNSubindex->getActualValue())
    {
      pobjCNSubindex -> setActualValue( (char *)pobjMNSubindex -> getActualValue());
      pobjCNSubindex -> setFlagIfIncludedCdc(TRUE);
      pobjCNIndex -> setFlagIfIncludedCdc(TRUE);
      return true;
    }
    return bCopied;
}

/****************************************************************************************************
* Function Name: ResetAllSubIndexFlag
* Description: 
* Return value: void
****************************************************************************************************/
void ResetAllSubIndexFlag(CIndex *pobjIndex)
{
	if(NULL == pobjIndex)
	{
		return;
	}
	CSubIndex *pobjSubIndex;
	for (INT32 iLoopCount = 0; iLoopCount < pobjIndex->getNumberofSubIndexes();  iLoopCount++)
	{
		pobjSubIndex = pobjIndex->getSubIndex(iLoopCount);
		if(NULL != pobjSubIndex)
		{
			pobjSubIndex -> setFlagIfIncludedCdc(FALSE);
		}
	}
	pobjIndex -> setFlagIfIncludedCdc(FALSE);
}
/****************************************************************************************************
* Function Name: GetIndexData
* Description: 
* Return value: void
****************************************************************************************************/
void GetIndexData(CIndex* objIndex, char* Buffer)
    {
            int len;
            bool IfStringDT = false;
            //Get the Index Value       
            
            strcpy(Buffer,"");
            if(objIndex->getNumberofSubIndexes()==0 && objIndex->getActualValue()!=NULL)
            {
                if(true == IsDefaultActualNotEqual(objIndex))
                {
                    strcpy(Buffer,objIndex->getIndexValue());

                    //Place a tab           
                    len = strlen(Buffer);
                    strcat(Buffer,"\t");

                    //If subindexes are none, add "00"
                        strcat(Buffer,"00");
                        strcat(Buffer,"\t");

                            //Add datatype
                            DataType dt;        
                            dt = objIndex->getDataType();
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
                                            {
                                                int len = strlen((char*)objIndex->getActualValue());
                                                strncpy(actvalue,(objIndex->getActualValue()+ 2),len-2 );
                                                actvalue[len -2] ='\0';                             
                                                strcat(Buffer,padLeft(actvalue,'0',padLength));
                                                
                                            }                                       
                        
                                        else
                                        {
                                     
                                            strcpy(actvalue, _IntToAscii(atoi(objIndex->getActualValue()),actvalue,16));
                                            strcat(Buffer,padLeft(actvalue, '0', padLength));
                                        
                                    }
                                }
                            }
                    
                                    strcat(Buffer,"\n");
                }
            }
            /* If Subobjects present*/
            else
            {
                int noOfSubIndexes = 0; 
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
                            noOfSubIndexes = hex2int(subString((char*)objSubIndex->getActualValue(), 2, strlen(objSubIndex->getActualValue()) -2));
                            
                        }
                        else
                            noOfSubIndexes = atoi(objSubIndex->getActualValue());
                            if(noOfSubIndexes ==0)
                            return;
                            noOfSubIndexes = noOfSubIndexes + 1;
                    }
                }

                if(CheckIfMappingPDO((char*)objIndex->getIndexValue()))
                {
                    if(objSubIndex->getActualValue() != NULL)
                    {
                            noOfSubIndexes = noOfTotalSubIndexes ;
                    }
                    else
                    {
                            noOfSubIndexes = noOfTotalSubIndexes + 1;
                    }
                }

                int noOfValidSubIndexes = 0;
                
                for(i=0; i<noOfTotalSubIndexes ; i++)
                {
                    
                    objSubIndex = objIndex->getSubIndex(i);
                
                    if((objSubIndex->getActualValue() != NULL) && (objSubIndex->getFlagIfIncludedCdc() == TRUE) &&  ( true == CheckAccessTypeForInclude((char*)objSubIndex->getAccessType()) || CheckIfMappingPDO((char*)objIndex->getIndexValue()) ) && (true == IsDefaultActualNotEqual(objSubIndex))  )
                    {
                        noOfValidSubIndexes = noOfValidSubIndexes + 1;
                        
                        if(noOfValidSubIndexes == noOfSubIndexes)
                        {
                            i= noOfTotalSubIndexes -1;
                        
                        }
                        if(CheckIfMappingPDO((char*)objIndex->getIndexValue()))
                        {
                            if(0 == strcmp((char*)objSubIndex->getIndexValue() ,"00"))
                            {
                                continue;
                            }  
                            if(0 == GetDecimalValue((char*)objSubIndex->getActualValue()))
                            {
                                if(  NULL == objSubIndex->getDefaultValue() || 0 == GetDecimalValue((char*)objSubIndex->getDefaultValue()))
                                {
                                    continue;
                                }
                            }
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
                    
                                else strcat(Buffer,"00000000");
                                delete[] size;
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
                                            
                                                strcat(Buffer,padLeft(actvalue,'0',padLength));
                                            
                                            }
                                                else
                                                {               
                                                    strcpy(actvalue, _IntToAscii(atoi(objSubIndex->getActualValue()),actvalue,16));
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
                                    i = -1; 
                                    flag_No_of_enteriesAdded = true;                            
                                }
                            }
                        }                   
                        
                }
    }

/****************************************************************************************************
* Function Name: BRSpecificGetIndexData
* Description: write the index data into cdc as per specific format
* Return value: void
****************************************************************************************************/
void BRSpecificGetIndexData(CIndex* objIndex, char* Buffer, int iNodeId )
    {
            int len;
            bool IfStringDT = false;
	    	char pb1F81nodeid[10];
            //Get the Index Value
            strcpy(Buffer,"");
            if(objIndex->getNumberofSubIndexes()==0 && objIndex->getActualValue()!=NULL)
            {
                if(true == IsDefaultActualNotEqual(objIndex))
                {
	                strcpy(Buffer,objIndex->getIndexValue());

	                len = strlen(Buffer);
	                //Place a tab
	                strcat(Buffer,"\t");
	                strcat(Buffer,"00");
	                strcat(Buffer,"\t");

                    //Add datatype
                    DataType dt;
                    dt = objIndex->getDataType();
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
                                    {
                                        int len = strlen((char*)objIndex->getActualValue());
                                        strncpy(actvalue,(objIndex->getActualValue()+ 2),len-2 );
                                        actvalue[len -2] ='\0';                             
                                        strcat(Buffer,padLeft(actvalue,'0',padLength));
                                                
                                    }                                       
                        
                                else
                                {
                                     
                                    strcpy(actvalue, _IntToAscii(atoi(objIndex->getActualValue()),actvalue,16));                                                            
                                    strcat(Buffer,padLeft(actvalue, '0', padLength));
                                        
                            }
                        }
                    }
                    
                            strcat(Buffer,"\n");
                }
            }
            /* If Subobjects present*/
            else
            {
                int noOfSubIndexes = 0;
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
                            noOfSubIndexes = hex2int(subString((char*)objSubIndex->getActualValue(), 2, strlen(objSubIndex->getActualValue()) -2));
                            
                        }
                        else
                            noOfSubIndexes = atoi(objSubIndex->getActualValue());               
                            if(noOfSubIndexes ==0)
                            return;
                            noOfSubIndexes = noOfSubIndexes + 1;
                    }
                }

                if(CheckIfMappingPDO((char*)objIndex->getIndexValue()))
                {
                    if(objSubIndex->getActualValue() != NULL)
                    {
                            noOfSubIndexes = noOfTotalSubIndexes ;
                    }
                    else
                    {
                            noOfSubIndexes = noOfTotalSubIndexes + 1;
                    }
                }

                int noOfValidSubIndexes = 0;
                
				if(0 == strcmp(objIndex->getIndexValue(),(char*)"1F81"))
				{
					
					_IntToAscii(iNodeId, pb1F81nodeid, 16); 
					padLeft(pb1F81nodeid, '0', 2);
				}

                for(i=0; i<noOfTotalSubIndexes ; i++)
                {
                    
                    objSubIndex = objIndex->getSubIndex(i);
                
                    if((objSubIndex->getActualValue() != NULL) && (objSubIndex->getFlagIfIncludedCdc() == TRUE) &&  ( true == CheckAccessTypeForInclude((char*)objSubIndex->getAccessType()) || CheckIfMappingPDO((char*)objIndex->getIndexValue()) ) && (true == IsDefaultActualNotEqual(objSubIndex))  )
                    {
						if(0 == strcmp(objIndex->getIndexValue(),(char*)"1F81"))
						{
							
							if(0 == strcmp(objSubIndex->getIndexValue(),"00") || 0 == strcmp(objSubIndex->getIndexValue(),pb1F81nodeid))
							{
								continue;
							}
							else
							{
							}
						}
                        noOfValidSubIndexes = noOfValidSubIndexes + 1;
                        
                        if(noOfValidSubIndexes == noOfSubIndexes)
                        {
                            i= noOfTotalSubIndexes -1;
                        
                        }

                        if(CheckIfMappingPDO((char*)objIndex->getIndexValue()))
                        {
                            if(0 == strcmp((char*)objSubIndex->getIndexValue() ,"00"))
                            {
                                continue;
                            }  
                            if(0 == GetDecimalValue((char*)objSubIndex->getActualValue()))
                            {
                                if(  NULL == objSubIndex->getDefaultValue() || 0 == GetDecimalValue((char*)objSubIndex->getDefaultValue()))
                                {
                                    continue;
                                }
                            }
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
                    
                                else strcat(Buffer,"00000000");
                                delete[] size;
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
                                            
                                                strcat(Buffer,padLeft(actvalue,'0',padLength));
                                            
                                            }
                                                else
                                                {               
                                                    strcpy(actvalue, _IntToAscii(atoi(objSubIndex->getActualValue()),actvalue,16));
                                        
                                                    strcat(Buffer,padLeft(actvalue, '0', padLength));
                                                }
                                            }
                                        
                                        }
                                        
                                }
                    
                                strcat(Buffer,"\n");
                                if(i == 0 && (CheckIfMappingPDO((char*)objIndex->getIndexValue()) &&(flag_No_of_enteriesAdded==true)))
                                {
                                    i = noOfTotalSubIndexes-1;                              
                                }
                                if(i == noOfTotalSubIndexes-1 && (CheckIfMappingPDO((char*)objIndex->getIndexValue()) && (flag_No_of_enteriesAdded==false) && resetValueAdded == true))
                                {
                                    i = -1; 
                                    flag_No_of_enteriesAdded = true;                            
                                }
                            }
                        }                   
                        
                }
    }

/****************************************************************************************************
* Function Name: WriteCNsData
* Description: write the index data into cdc
* Return value: void
****************************************************************************************************/
	void WriteCNsData(char* fileName)
{
		char* Buffer2;
		char* Buffer3;
		int len;
		CIndexCollection* objIndexCollection;
	
		char* c = (char*)malloc(50);	
		FILE* fileptr = new FILE();
		CNode objNode;	
		CNodeCollection* objNodeCollection;
		objNodeCollection = CNodeCollection::getNodeColObjectPointer();		
		
		int CNCount = 0;
        
        CPjtSettings* stPjtSettings;
        EAutoGenerate ePjtSetting;
        ePjtSetting = CPjtSettings::getPjtSettingsPtr()->getGenerateAttr();
                
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
							if((fwrite(comment, sizeof(char),len,fileptr)) != 0)
							{
								fclose(fileptr);
							}
							
							free(comment);
							int NumberOfIndexes;
							
							CIndex* objIndex;
							char* Buffer4;
									
						/*************WRITE MN'S 1006,1020 Indexes Values *******************************/			
                            Buffer4 = new char[30000];
							objIndex = getMNIndexValues((char*)"1006");
							Buffer2 = new char[60000];		
						
							if(objIndex!=NULL)
							{
								if((char*)objIndex->getActualValue() != NULL)
								UpdateCNCycleTime(objIndexCollection,(char*) objIndex->getActualValue());
							}
							
                            objIndex = getMNIndexValues((char*)"1C14");
                            if(objIndex != NULL)
                            {
                                if((char*)objIndex->getActualValue() != NULL)
                                UpdateCNSoCTolerance(objIndexCollection,(char*) objIndex->getActualValue());
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

                            objIndex = getMNIndexValues((char*)"1F9B");
                            if(objIndex != NULL)
                            {
                                    UpdateCNMultipleCycleAssign( &objNode);
                            }

                            UpdateCNVisibleNode( &objNode);
                            UpdateCNPresMNActLoad(&objNode);

							strcpy(Buffer2, "");
							strcpy(Buffer4, "");
							
							char* NoOfenteries = new char[10];
							//workaround for B&R Bus Controller stack
							//NoOfenteries = _IntToAscii(getCNsTotalIndexSubIndex(objNode.getNodeId()), NoOfenteries, 16);
							NoOfenteries = _IntToAscii(BRSpecificgetCNsTotalIndexSubIndex(objNode.getNodeId()), NoOfenteries, 16);
                            /*1 is added for the size*/
							NoOfenteries =  padLeft(NoOfenteries, '0', 8);
							strcpy(Buffer4, NoOfenteries);
							strcat(Buffer4, "\n");
							strcpy(Buffer2, Buffer4);
						
							NumberOfIndexes = objIndexCollection->getNumberofIndexes();
								

								////workaround for B&R Bus Controller stack
                                //FormatCdc(objIndexCollection, Buffer4, fileptr, CN);
								BRSpecificFormatCdc(objIndexCollection, Buffer4, fileptr, CN, objNode.getNodeId() );
                                strcat(Buffer2, Buffer4);
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
							Buffer3 = new char [strlen(Buffer2)+50];
							strcpy(Buffer3, CNDataLength);
							strcat(Buffer3, "\n");
							strcat(Buffer3, Buffer2);
															
						
						
							//write all CNs data in the file
							len = strlen(Buffer3);
							if (( fileptr = fopen(fileName,"a+")) == NULL)
							{
								//cout << "Problem" <<endl;
							}
							if((fwrite(Buffer3, sizeof(char),len,fileptr))!=0)
								{
									fclose(fileptr);																							
								}
					delete [] Buffer2;
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
            
    
            if( pobjIndex->getFlagIfIncludedCdc() == TRUE && ( true == CheckAccessTypeForInclude((char*)pobjIndex->getAccessType()) || CheckIfMappingPDO((char*)pobjIndex->getIndexValue()) ) )
            {
				if(pobjIndex->getNumberofSubIndexes() ==0)
				{
					if(pobjIndex->getActualValue() != NULL && true == IsDefaultActualNotEqual(pobjIndex) )
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

                        if((NULL != pobjSubIndex) && (NULL != pobjSubIndex->getActualValue()) && ( 0 != strcmp(pobjSubIndex->getActualValue(),"")) )
						{
                            if(true == ReactivateMappingPDO(pobjIdxCol, pobjIndex) || true == IsDefaultActualNotEqual(pobjSubIndex))
                            {
    							iNumberOfEntries =  iNumberOfEntries + 1; /* to initalize 00 entry subindex */
                            }
                            if(checkIfValueZero((char*)pobjSubIndex->getActualValue()))
                            {
                                continue;
                            }
                            if(true == ReactivateMappingPDO(pobjIdxCol, pobjIndex) || true == IsDefaultActualNotEqual(pobjSubIndex))
                            {
                                iNumberOfEntries =  iNumberOfEntries + 1; /* to reinitalize 00 entry subindex */
                            }
                            for(INT32 iLoopCount = 0; iLoopCount < pobjIndex->getNumberofSubIndexes(); iLoopCount++)
                            {
                                if( pobjIndex->getSubIndex(iLoopCount)->getActualValue() !=NULL && TRUE == pobjIndex->getSubIndex(iLoopCount)->getFlagIfIncludedCdc() && true == IsDefaultActualNotEqual(pobjIndex->getSubIndex(iLoopCount)) )
                                {
                                    if(0 == strcmp((char*)pobjIndex->getSubIndex(iLoopCount)->getIndexValue() ,"00"))
                                    {
                                        continue;
                                    }  
                                    if(0 == GetDecimalValue((char*)pobjIndex->getSubIndex(iLoopCount)->getActualValue()))
                                    {
                                        if(  NULL == pobjIndex->getSubIndex(iLoopCount)->getDefaultValue() || 0 == GetDecimalValue((char*)pobjIndex->getSubIndex(iLoopCount)->getDefaultValue()))
                                        {
                                            continue;
                                        }
                                    }
                                    iNumberOfEntries =  iNumberOfEntries + 1;

                                }
                            }
						}
						continue;
					}

                    if(0 == strcmp((char*)pobjIndex->getIndexValue(), "1F81"))
                    {
                        CSubIndex* pobjSubIndex;
                        pobjSubIndex = pobjIndex->getSubIndexbyIndexValue((char*)"F0");
                        if( NULL != pobjSubIndex && pobjSubIndex->getActualValue() !=NULL && 0 != strcmp((char*)pobjSubIndex->getActualValue(), "") && true == CheckAccessTypeForInclude((char*)pobjSubIndex->getAccessType()) && true == IsDefaultActualNotEqual(pobjSubIndex) )
                        {
                            iNumberOfEntries =  iNumberOfEntries + 1;
                        }
                        continue;

                    }


					CSubIndex* pobjSubIndex;
					pobjSubIndex = pobjIndex->getSubIndexbyIndexValue((char*)"00");
					if((NULL != pobjSubIndex) && (NULL != pobjSubIndex->getActualValue()) && ( 0 != strcmp(pobjSubIndex->getActualValue(),"")) )
					{
						if(checkIfValueZero((char*)pobjSubIndex->getActualValue()))
							continue;
					}

					for(INT32 iLoopCount = 0; iLoopCount < pobjIndex->getNumberofSubIndexes(); iLoopCount++)
					{
						if(pobjIndex->getSubIndex(iLoopCount)->getActualValue() !=NULL && TRUE == pobjIndex->getSubIndex(iLoopCount)->getFlagIfIncludedCdc() && true == CheckAccessTypeForInclude((char*)pobjIndex->getSubIndex(iLoopCount)->getAccessType()) && true == IsDefaultActualNotEqual(pobjIndex->getSubIndex(iLoopCount)) )
						{
								iNumberOfEntries =  iNumberOfEntries + 1;
						}
					}
				}
                   
			}
		}
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
	for(INT32 iLoopCount = 0; iLoopCount < pobjIdxCol->getNumberofIndexes(); iLoopCount++)
	{
			
		CIndex *pobjIndex;
		pobjIndex = pobjIdxCol->getIndex(iLoopCount);
			
	
		if( pobjIndex->getFlagIfIncludedCdc() == TRUE && ( true == CheckAccessTypeForInclude((char*)pobjIndex->getAccessType()) || CheckIfMappingPDO((char*)pobjIndex->getIndexValue()) ) )
		{
			{
				if(pobjIndex->getNumberofSubIndexes() ==0)
				{
					if(pobjIndex->getActualValue() != NULL && true == IsDefaultActualNotEqual(pobjIndex) )
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

                        if((NULL != pobjSubIndex) && (NULL != pobjSubIndex->getActualValue()) && ( 0 != strcmp(pobjSubIndex->getActualValue(),"")) )
                        {
                            if(true == ReactivateMappingPDO(pobjIdxCol, pobjIndex) || true == IsDefaultActualNotEqual(pobjSubIndex))
                            {
                                iNumberOfEntries =  iNumberOfEntries + 1; /* to initalize 00 entry subindex */
                            }
                            if(checkIfValueZero((char*)pobjSubIndex->getActualValue()))
                            {
                                continue;
                            }
                            if(true == ReactivateMappingPDO(pobjIdxCol, pobjIndex) || true == IsDefaultActualNotEqual(pobjSubIndex))
                            {
                                iNumberOfEntries =  iNumberOfEntries + 1; /* to reinitalize 00 entry subindex */
                            }
                            for(INT32 iLoopCount = 0; iLoopCount < pobjIndex->getNumberofSubIndexes(); iLoopCount++)
                            {
                                if(pobjIndex->getSubIndex(iLoopCount)->getActualValue() !=NULL && TRUE == pobjIndex->getSubIndex(iLoopCount)->getFlagIfIncludedCdc() && true == IsDefaultActualNotEqual(pobjIndex->getSubIndex(iLoopCount)) )
                                {
                                    if(0 == strcmp((char*)pobjIndex->getSubIndex(iLoopCount)->getIndexValue() ,"00"))
                                    {
                                        continue;
                                    }  
                                    if(0 == GetDecimalValue((char*)pobjIndex->getSubIndex(iLoopCount)->getActualValue()))
                                    {
                                        if(  NULL == pobjIndex->getSubIndex(iLoopCount)->getDefaultValue() || 0 == GetDecimalValue((char*)pobjIndex->getSubIndex(iLoopCount)->getDefaultValue()))
                                        {
                                            continue;
                                        }
                                    }
                                    iNumberOfEntries =  iNumberOfEntries + 1;

                                }
                            }
                        }
                        continue;
                    }

                    CSubIndex* pobjSubIndex;
                    pobjSubIndex = pobjIndex->getSubIndexbyIndexValue((char*)"00");
                    if((NULL != pobjSubIndex) && (NULL != pobjSubIndex->getActualValue()) && ( 0 != strcmp(pobjSubIndex->getActualValue(),"")) )
                    {
                        if(checkIfValueZero((char*)pobjSubIndex->getActualValue()))
                            continue;
                    }

                    for(INT32 iLoopCount = 0; iLoopCount < pobjIndex->getNumberofSubIndexes(); iLoopCount++)
                    {
                        if(pobjIndex->getSubIndex(iLoopCount)->getActualValue() !=NULL && TRUE == pobjIndex->getSubIndex(iLoopCount)->getFlagIfIncludedCdc() && true == CheckAccessTypeForInclude((char*)pobjIndex->getSubIndex(iLoopCount)->getAccessType()) && true == IsDefaultActualNotEqual(pobjIndex->getSubIndex(iLoopCount)) )
                        {
                                iNumberOfEntries =  iNumberOfEntries + 1;
                        }
			        }
		        }
			}
		}
	}
	return iNumberOfEntries ;
}
/****************************************************************************************************
* Function Name: BRSpecificgetCNsTotalIndexSubIndex
* Description: 
* Return value: INT32
****************************************************************************************************/
INT32 BRSpecificgetCNsTotalIndexSubIndex(INT32 iNodeID)
{
	CNodeCollection *objNodeCol;
	objNodeCol =  CNodeCollection::getNodeColObjectPointer();
	CNode objNode;		
	INT32 iNumberOfEntries = 0;
	CIndexCollection* pobjIdxCol;
	char pb1F81nodeid[10];
	_IntToAscii(iNodeID, pb1F81nodeid, 16); 
	padLeft(pb1F81nodeid, '0', 2);
			
	objNode    = objNodeCol->getNode(iNodeID);
	pobjIdxCol =  objNode.getIndexCollection();
	for(INT32 iLoopCount = 0; iLoopCount < pobjIdxCol->getNumberofIndexes(); iLoopCount++)
	{
			
		CIndex *pobjIndex;
		pobjIndex = pobjIdxCol->getIndex(iLoopCount);
			
	
		if( pobjIndex->getFlagIfIncludedCdc() == TRUE && ( true == CheckAccessTypeForInclude((char*)pobjIndex->getAccessType()) || CheckIfMappingPDO((char*)pobjIndex->getIndexValue()) ) )
		{
			{
				if(pobjIndex->getNumberofSubIndexes() ==0)
				{
					if(pobjIndex->getActualValue() != NULL && true == IsDefaultActualNotEqual(pobjIndex) )
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
                        
                        if((NULL != pobjSubIndex) && (NULL != pobjSubIndex->getActualValue()) && ( 0 != strcmp(pobjSubIndex->getActualValue(),"")) )
                        {
                            if(true == ReactivateMappingPDO(pobjIdxCol, pobjIndex) || true == IsDefaultActualNotEqual(pobjSubIndex))
                            {
                                iNumberOfEntries =  iNumberOfEntries + 1; /* to initalize 00 entry subindex */
                            }
                            if(checkIfValueZero((char*)pobjSubIndex->getActualValue()))
                            {
                                continue;
                            }
                            if(true == ReactivateMappingPDO(pobjIdxCol, pobjIndex) || true == IsDefaultActualNotEqual(pobjSubIndex))
                            {
                                iNumberOfEntries =  iNumberOfEntries + 1; /* to reinitalize 00 entry subindex */
                            }
                            for(INT32 iLoopCount = 0; iLoopCount < pobjIndex->getNumberofSubIndexes(); iLoopCount++)
                            {
                                if(pobjIndex->getSubIndex(iLoopCount)->getActualValue() !=NULL && TRUE == pobjIndex->getSubIndex(iLoopCount)->getFlagIfIncludedCdc() && true == IsDefaultActualNotEqual(pobjIndex->getSubIndex(iLoopCount)) )
                                {
                                    if(0 == strcmp((char*)pobjIndex->getSubIndex(iLoopCount)->getIndexValue() ,"00"))
                                    {
                                        continue;
                                    }  
                                    if(0 == GetDecimalValue((char*)pobjIndex->getSubIndex(iLoopCount)->getActualValue()))
                                    {
                                        if(  NULL == pobjIndex->getSubIndex(iLoopCount)->getDefaultValue() || 0 == GetDecimalValue((char*)pobjIndex->getSubIndex(iLoopCount)->getDefaultValue()))
                                        {
                                            continue;
                                        }
                                    }
                                    iNumberOfEntries =  iNumberOfEntries + 1;

                                }
                            }
                        }
                        continue;
                    }

                    CSubIndex* pobjSubIndex;
                    pobjSubIndex = pobjIndex->getSubIndexbyIndexValue((char*)"00");
                    if((NULL != pobjSubIndex) && (NULL != pobjSubIndex->getActualValue()) && ( 0 != strcmp(pobjSubIndex->getActualValue(),"")) )
                    {
                        if(checkIfValueZero((char*)pobjSubIndex->getActualValue()))
                            continue;
                    }

                    for(INT32 iLoopCount = 0; iLoopCount < pobjIndex->getNumberofSubIndexes(); iLoopCount++)
                    {
						if(pobjIndex->getSubIndex(iLoopCount)->getActualValue() !=NULL && TRUE == pobjIndex->getSubIndex(iLoopCount)->getFlagIfIncludedCdc() && true == CheckAccessTypeForInclude((char*)pobjIndex->getSubIndex(iLoopCount)->getAccessType()) && true == IsDefaultActualNotEqual(pobjIndex->getSubIndex(iLoopCount)) )
                        {
							if(0 == strcmp(pobjIndex->getIndexValue(),(char*)"1F81"))
							{
								
								if(0 == strcmp(pobjIndex->getSubIndex(iLoopCount)->getIndexValue(),"00") || 0 == strcmp(pobjIndex->getSubIndex(iLoopCount)->getIndexValue(),pb1F81nodeid))
								{
									continue;
								}
								else
								{
								}
							}
                                iNumberOfEntries =  iNumberOfEntries + 1;
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
	ocfmRetCode GenerateCDC(char* CDCLocation)
	{
///////////////////////////////
cout<<"------GenerateCDC-------------------------------------------"<<endl;
		CNode objNode;	
		CIndexCollection* objIndexCollection;
		//ofstream file;
		char *Buffer1 = NULL;
		unsigned int len;
		ocfmRetCode retCode, ret1F81Code;
        int iSubIndexPos, iIndexPos;
		char* tempFileName;
		tempFileName = new char[strlen(CDCLocation) + 10 + 10];		
		sprintf(tempFileName, "%s/%s.txt", CDCLocation, "mnobd");

		char* tempOutputFileName;
		tempOutputFileName = new char [1000];
		sprintf(tempOutputFileName, "%s/%s.cdc", CDCLocation, "mnobd");		

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

			/* Check if autogeneration of MN's Object Dicitonary is set to true */
			
			CPjtSettings* stPjtSettings;
			stPjtSettings = CPjtSettings::getPjtSettingsPtr();
			bool checkFlag = true;
		
			if(stPjtSettings->getGenerateAttr() == YES_AG)
			{	
                retCode = CheckMutliplexAssigned();
                if(retCode.code != OCFM_ERR_SUCCESS)
                {
                    return retCode;
                }
				retCode = GenerateMNOBD(true);			////////////////Generate MNOBD for Auto Generate On
				if(retCode.code != OCFM_ERR_SUCCESS)
					return retCode;
			}
		

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
                                int NodeID = objNode.getNodeId();       
                                char* hex = (char*)malloc(10);                  
                                hex = _IntToAscii(NodeID,hex,16);
                                hex = padLeft(hex,'0',2);

                                char* pcTemp1F81Data = setNodeAssigmentBits(&objNode);
                                char* pb1F81Data = new char[ strlen(pcTemp1F81Data) + ALLOC_BUFFER + 2];
                                sprintf(pb1F81Data, "0X%s", pcTemp1F81Data);
                                ret1F81Code = IfSubIndexExists(240, MN, (char*)"1F81", hex, &iSubIndexPos, &iIndexPos);
                                if( OCFM_ERR_SUCCESS != ret1F81Code.code)
                                {
                                    continue;
                                }
 
                                SetSIdxValue((char*)"1F81",hex, pb1F81Data,objNodeCollection->getMNNode().getIndexCollection(), MN_NODEID, MN, false); 
                                delete [] pb1F81Data;
                        }
                    }
//////////////////////////////////////////
cout<<"--UpdateMNNodeAssignmentIndex--Called from GenerateCDC--"<<endl;

                UpdateMNNodeAssignmentIndex(&objNodeCollection->getMNNode(), objNodeCollection->getCNNodesCount(), (char*)"1F81", true );
                UpdateMNNodeAssignmentIndex(&objNodeCollection->getMNNode(), objNodeCollection->getCNNodesCount(), (char*)"1F92", false );
                UpdateMNNodeAssignmentIndex(&objNodeCollection->getMNNode(), objNodeCollection->getCNNodesCount(), (char*)"1F8D", true );
                
            /*BUG FIX #27 - START*/
               if(stPjtSettings->getGenerateAttr() == YES_AG)
				{	
                	CalculatePayload();
				} 
            /*BUG FIX #27 -END*/

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
					cout << "Write Error in CDC" << endl;
				}
                delete[] Buffer1;
				
				// Add 1F81		
				for(int i=0;i < objNodeCollection->getNumberOfNodes();i++)
				{
						CNode objNode;
						objNode = objNodeCollection->getNodebyCollectionIndex(i);
						if(objNode.getNodeType() ==CN)
						{
						
								Buffer1 = new char[CDC_BUFFER];
								strcpy(Buffer1, "//// NodeId Assignment\n");
								strcat(Buffer1, "1F81");
								strcat(Buffer1, "\t");
								int NodeID = objNode.getNodeId();		
								char* hex = (char*)malloc(10);					
								hex = _IntToAscii(NodeID,hex,16);
								hex = padLeft(hex,'0',2);
								strcat(Buffer1, hex);								
								strcat(Buffer1, "\t00000004\t00000007\n");
								strcat(Buffer1, "\n");	
								len = strlen(Buffer1);
								// write 1F81 entry in MN text file
								if((len != (fwrite(Buffer1, sizeof(char),len,fileptr))))
								{
									cout << "Write Error in CDC _1" << endl;
								}
							
								delete[] Buffer1;
						}
					}

				fclose(fileptr);
				
			if (( fileptr = fopen(tempFileName,"a+")) == NULL)
				{
					ocfmException ex;
					ex.ocfm_Excpetion(OCFM_ERR_CANNOT_OPEN_FILE);
					throw ex;							
				}
				objIndexCollection = objNode.getIndexCollection();
			

			//Get all the MN's Default Data in Buffer1
			int NumberOfIndexes = objIndexCollection->getNumberofIndexes();
            Buffer1 = new char[CDC_MN_BUFFER];
            memset(Buffer1, 0, CDC_MN_BUFFER*sizeof(char));
            FormatCdc(objIndexCollection, Buffer1, fileptr, MN);
            
            len = strlen(Buffer1);
            if((len != (fwrite(Buffer1, sizeof(char),len,fileptr))))
            {
				cout << "Write Error in CDC _2" << endl;
            }                   
            delete[] Buffer1;
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

								strcat(Buffer1, "\t00000004\t");
                                char* pcTemp1F81Data = setNodeAssigmentBits(&objNode);
								strcat(Buffer1, pcTemp1F81Data);

								strcat(Buffer1, "\n");

								len = strlen(Buffer1);
								if((len != (fwrite(Buffer1, sizeof(char),len,fileptr))))
								{
								
									ret = fclose(fileptr);
								
								}
								else
								{
									ret = fclose(fileptr);
								}
								delete[] Buffer1;
						}
					}
							
				}

			// Convert CDC txt file to Binary
			#if defined(_WIN32) && defined(_MSC_VER)
			char* cmdBuffer;
			cmdBuffer = new char[(2 * (strlen(CDCLocation) + 10 + 10)) + 25];		
			sprintf(cmdBuffer, "txt2cdc.exe \"%s\" \"%s\"", tempFileName, tempOutputFileName);
			system(cmdBuffer);
			delete [] cmdBuffer;
			#else
			char* cmdBuffer;
			cmdBuffer = new char[LINUX_INSTALL_DIR_LEN + (2 * (strlen(CDCLocation) + 10 + 10)) + 25];		
			sprintf(cmdBuffer, "%s/txt2cdc \"%s\" \"%s\"", LINUX_INSTALL_DIR, tempFileName, tempOutputFileName);
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
* Function Name: FormatCdc
* Description: extracts the index data as per specified format
* Return value: void
****************************************************************************************************/
void FormatCdc(CIndexCollection *objIndexCollection, char* Buffer1, FILE* fileptr, ENodeType eNodeType )
{

    char *TempBuffer1 = NULL;
    strcpy(Buffer1, "");
    unsigned int len;
    int NumberOfIndexes = objIndexCollection->getNumberofIndexes();
	//disable mapping pdo
    for(int i = 0;i < NumberOfIndexes; i++)
    {
        CIndex* objIndex;
        objIndex = objIndexCollection->getIndex(i);
        if(objIndex->getFlagIfIncludedCdc() == TRUE )
        {
                if(CheckIfMappingPDO((char*)objIndex->getIndexValue()))
                {
                TempBuffer1 = new char[CDC_BUFFER];
                len = strlen(Buffer1);      
                //GetIndexData(objIndex,Buffer1);
                EnableDisableMappingPDO(objIndexCollection, objIndex, TempBuffer1, false);
                strcat(Buffer1, TempBuffer1);
                 delete[] TempBuffer1;
                 }
        }
    }
            // write all objects except pdo
            for(int i=0;i < NumberOfIndexes; i++)
            {
                CIndex* objIndex;
                objIndex = objIndexCollection->getIndex(i);
                

                if(objIndex->getFlagIfIncludedCdc() == TRUE && true == CheckAccessTypeForInclude((char*)objIndex->getAccessType()) && CheckIfNotPDO((char*)objIndex->getIndexValue()))
                {
                        if((CN == eNodeType) ||  (strcmp(objIndex->getIndexValue(), "1F81") != 0 && MN == eNodeType))
                        {
                        TempBuffer1 = new char[15000];
                        GetIndexData(objIndex,TempBuffer1);
                        strcat(Buffer1, TempBuffer1);
                        
                        delete[] TempBuffer1;
                        }
                        else
                        {
                            CSubIndex* objSubIndex = objIndex->getSubIndexbyIndexValue((char*)"F0");
                            if(NULL != objSubIndex && TRUE == objSubIndex->getFlagIfIncludedCdc() && true == CheckAccessTypeForInclude((char*)objSubIndex->getAccessType())  && NULL != objSubIndex->getActualValue() && 0 != strcmp((char*)objSubIndex->getActualValue(), "") && true == IsDefaultActualNotEqual(objSubIndex))
                            {
                                    TempBuffer1 = new char[CDC_BUFFER];
                                    strcpy(TempBuffer1, "1F81");
                                    strcat(TempBuffer1, "\t");
                                    strcat(TempBuffer1, "F0");
                                    strcat(TempBuffer1, "\t00000004\t");
    
                                    char actvalue[20];
                                    actvalue[0]  = '\0';
                                    if(CheckIfHex((char*)objSubIndex->getActualValue()))
                                    {
                                        int len = strlen((char*)objSubIndex->getActualValue());

                                        strncpy(actvalue,(objSubIndex->getActualValue()+ 2),len-2 );
                                        actvalue[len -2] ='\0';
                                        strcat(TempBuffer1,padLeft(actvalue,'0',8));
                                    }
                                    else
                                    {
                                        strcpy(actvalue, _IntToAscii(atoi(objSubIndex->getActualValue()),actvalue,16));
                                        strcat(TempBuffer1,padLeft(actvalue, '0', 8));
                                    }
    
                                    strcat(TempBuffer1, "\n"); 
                                    len = strlen(TempBuffer1);
                                    strcat(Buffer1, TempBuffer1);
                                    delete[] TempBuffer1;
                        }
                }
            }           
        }
        //Write the pdo configuration
        for(int i=0;i < NumberOfIndexes; i++)
            {
                CIndex* objIndex;
                objIndex = objIndexCollection->getIndex(i);
                if(objIndex->getFlagIfIncludedCdc() == TRUE && ( true == CheckAccessTypeForInclude((char*)objIndex->getAccessType()) || CheckIfMappingPDO((char*)objIndex->getIndexValue()) ) && !CheckIfNotPDO((char*)objIndex->getIndexValue()))
                {
                        TempBuffer1 = new char[3 * CDC_BUFFER];
                        GetIndexData(objIndex,TempBuffer1);
                        strcat(Buffer1, TempBuffer1);
                        
                        delete[] TempBuffer1;
                    }
            }           
            //reenable the pdos
            for(int i=0;i < NumberOfIndexes; i++)
            {
                CIndex* objIndex;
                objIndex = objIndexCollection->getIndex(i);
                if(objIndex->getFlagIfIncludedCdc() == TRUE )
                {
                        if(CheckIfMappingPDO((char*)objIndex->getIndexValue()))
                        {
                        TempBuffer1 = new char[CDC_BUFFER];
                        EnableDisableMappingPDO(objIndexCollection, objIndex, TempBuffer1, true);
                        strcat(Buffer1, TempBuffer1);
                         delete[] TempBuffer1;
                         }
                }
            }
}

/****************************************************************************************************
* Function Name: BRSpecificFormatCdc
* Description: write the cdc as per BR specific format
* Return value: void
****************************************************************************************************/
void BRSpecificFormatCdc(CIndexCollection *objIndexCollection, char* Buffer1, FILE* fileptr, ENodeType eNodeType, int iNodeId )
{
    char *TempBuffer1 = NULL;
    strcpy(Buffer1, "");
    unsigned int len;
    int NumberOfIndexes = objIndexCollection->getNumberofIndexes();
//disable mapping pdo
    for(int i=0;i < NumberOfIndexes; i++)
    {
        CIndex* objIndex;
        objIndex = objIndexCollection->getIndex(i);
        if(objIndex->getFlagIfIncludedCdc() == TRUE )
        {
                if(CheckIfMappingPDO((char*)objIndex->getIndexValue()))
                {
                TempBuffer1 = new char[CDC_BUFFER];
                len = strlen(Buffer1);      
                EnableDisableMappingPDO(objIndexCollection, objIndex, TempBuffer1, false);
                strcat(Buffer1, TempBuffer1);
                 delete[] TempBuffer1;
                 }
        }
    }
            // write all objects except pdo
            for(int i=0;i < NumberOfIndexes; i++)
            {
                CIndex* objIndex;
                objIndex = objIndexCollection->getIndex(i);
                if(objIndex->getFlagIfIncludedCdc() == TRUE && true == CheckAccessTypeForInclude((char*)objIndex->getAccessType()) && CheckIfNotPDO((char*)objIndex->getIndexValue()))
                {
                        if((CN == eNodeType) ||  (strcmp(objIndex->getIndexValue(), "1F81") != 0 && MN == eNodeType))
                        {
                        TempBuffer1 = new char[15000];
						//commented the GetIndexData fn and BRSpecificGetIndexData is used
						//GetIndexData(objIndex,TempBuffer1);
                        BRSpecificGetIndexData(objIndex,TempBuffer1,iNodeId);
                        strcat(Buffer1, TempBuffer1);
                        
                        delete[] TempBuffer1;
                        }
                        else
                        {
                            CSubIndex* objSubIndex = objIndex->getSubIndexbyIndexValue((char*)"F0");
                            if(NULL != objSubIndex && TRUE == objSubIndex->getFlagIfIncludedCdc() && true == CheckAccessTypeForInclude((char*)objSubIndex->getAccessType())  && NULL != objSubIndex->getActualValue() && 0 != strcmp((char*)objSubIndex->getActualValue(), "") && true == IsDefaultActualNotEqual(objSubIndex))
                            {
                                    TempBuffer1 = new char[CDC_BUFFER];
                                    strcpy(TempBuffer1, "1F81");
                                    strcat(TempBuffer1, "\t");
                                    strcat(TempBuffer1, "F0");
                                    strcat(TempBuffer1, "\t00000004\t");
    
                                    char actvalue[20];
                                    actvalue[0]  = '\0';
                                    if(CheckIfHex((char*)objSubIndex->getActualValue()))
                                    {
                                        int len = strlen((char*)objSubIndex->getActualValue());
                                        strncpy(actvalue,(objSubIndex->getActualValue()+ 2),len-2 );
                                        actvalue[len -2] ='\0';
                                        strcat(TempBuffer1,padLeft(actvalue,'0',8));
                                    }
                                    else
                                    {
                                        strcpy(actvalue, _IntToAscii(atoi(objSubIndex->getActualValue()),actvalue,16));
                                        strcat(TempBuffer1,padLeft(actvalue, '0', 8));
                                    }
    
                                    strcat(TempBuffer1, "\n"); 
                                    len = strlen(TempBuffer1);
                                    strcat(Buffer1, TempBuffer1);
                                    delete[] TempBuffer1;
                        }
                }
            }           
        }
        //Write the pdo configuration
        for(int i=0;i < NumberOfIndexes; i++)
            {
                CIndex* objIndex;
                objIndex = objIndexCollection->getIndex(i);
                if(objIndex->getFlagIfIncludedCdc() == TRUE && ( true == CheckAccessTypeForInclude((char*)objIndex->getAccessType()) || CheckIfMappingPDO((char*)objIndex->getIndexValue()) ) && !CheckIfNotPDO((char*)objIndex->getIndexValue()))
                {
                        TempBuffer1 = new char[3 * CDC_BUFFER];
                        GetIndexData(objIndex,TempBuffer1);
                        strcat(Buffer1, TempBuffer1);
                        
                        delete[] TempBuffer1;
                    }
            }           
            //reenable the pdos
            for(int i=0;i < NumberOfIndexes; i++)
            {
                CIndex* objIndex;
                objIndex = objIndexCollection->getIndex(i);
                if(objIndex->getFlagIfIncludedCdc() == TRUE )
                {
                        if(CheckIfMappingPDO((char*)objIndex->getIndexValue()))
                        {
                        TempBuffer1 = new char[CDC_BUFFER];
                        EnableDisableMappingPDO(objIndexCollection, objIndex, TempBuffer1, true);
                        strcat(Buffer1, TempBuffer1);
 
                         delete[] TempBuffer1;
                         }
                }
            }
}

/****************************************************************************************************
* Function Name: ProcessCDT
* Description: 
* Return value: void
****************************************************************************************************/
ocfmRetCode  ProcessCDT(CComplexDataType* pobjCDT,CApplicationProcess* pobjAppProc,
																 CNode* pobjNode, Parameter* pobjParameter, EPDOType enumPdoType,
																 char* pbModuleName, char* pbModuleIndex)
{
cout<<"---------ProcessCDT--------"<<endl;
	ocfmException objocfmException;
	INT32 iStartBitOffset =  0;
	INT32 iOffset;
	bool bIsNewBitStringVar = false;
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
				iLastVarIndex = iLoopCount;
			ProcessCDT(pobjCDT, pobjAppProc, pobjNode, pobjParameter, enumPdoType, pbModuleName, pbModuleIndex );
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
					objProcessImage.DataInfo.DataSize = atoi(objVarDecl.size);					
				}
			else
			{
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
            if(((objProcessImage.DataInfo._dt_enum  != BITSTRING) && (objProcessImage.DataInfo.DataSize >= 8 )) || ((objProcessImage.DataInfo._dt_enum  == BITSTRING) && (iStartBitOffset ==0 || iStartBitOffset ==8 || iStartBitOffset == 16 || iStartBitOffset == 32 || iStartBitOffset == 64)))
			{
				
				bIsNewBitStringVar =  true;
                if(objProcessImage.DataInfo._dt_enum  == BITSTRING) 
                {
			        iStartBitOffset = 0;					
                    iDataSize =  0;
                    for(INT32 iBitStringCount = iLoopCount; iBitStringCount<pobjCDT->varCollection.Count(); iBitStringCount++)
                    {
                        varDeclaration objVarDecl;
            
                        objVarDecl.Initialize();
                        objVarDecl = pobjCDT->varCollection[iBitStringCount];            
                        if( BITSTRING == ((*(getIECDT(objVarDecl.nam_id_dt_attr->getDataType(), objProcessImage.DataInfo.DataSize)))._dt_enum) )
                        {
                            iDataSize +=  atoi(objVarDecl.size);
                            if(8 == iDataSize || 16 == iDataSize || 32 == iDataSize || 64 == iDataSize)
                            {
                                break;
                            }
                        }
                        else
                        {
                            break;
                        }
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
			/*BUG FIX #10 - START*/
			 iTotalBytesMapped = iTotalBytesMapped + iDataSize/8;
			/*BUG FIX #10 - END*/
				if(iTotalBytesMapped >  MAX_PI_SIZE)
				{
					ocfmException objex;
					objex.ocfm_Excpetion(OCFM_ERR_MAX_PI_SIZE);
					throw objex;
				}
				CreateMNPDOVar(iOffset, iDataSize, objProcessImage.DataInfo._dt_enum, enumPdoType, pobjNode);
			}
			
			if((objProcessImage.DataInfo.DataSize >= 8) && (iStartBitOffset!= 0 ) && (objProcessImage.DataInfo._dt_enum  != BITSTRING))
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
		pobjNode->addProcessImage(objProcessImage);
		}
		
	}

		if(pobjCDT->previousCDT_UId !=NULL)
		{
			pobjCDT = pobjAppProc->getCDTbyUniqueID(pobjCDT->previousCDT_UId);
		
			for(INT32 iLoopCount = (iLastVarIndex + 1) ; iLoopCount < pobjCDT->varCollection.Count(); iLoopCount++)
			{
				if(!bCDTCompleted)
				{
					varDeclaration objVarDecl;
					objVarDecl = pobjCDT->varCollection[iLoopCount];
					if(objVarDecl.nam_id_dt_attr->getDtUniqueRefId() != NULL)
					{
						pobjCDT = pobjAppProc->getCDTbyUniqueID(objVarDecl.nam_id_dt_attr->getDtUniqueRefId());
						pobjAppProc->updatePreviousCDT_UId(objVarDecl.StructUniqueId, pobjCDT->Index);
				
						iLastVarIndex = iLoopCount;
						ProcessCDT(pobjCDT, pobjAppProc, pobjNode, pobjParameter, enumPdoType, pbModuleName,pbModuleIndex);
					
					}						
				}
			}
		}
		
	}
	bCDTCompleted = true;
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

ocfmRetCode ProcessPDONodes()
{
  return (ProcessPDONodes(false));
}
/****************************************************************************************************
* Function Name: ProcessPDONodes
* Description: Processes the Node
* Return value: ocfmRetCode
****************************************************************************************************/
ocfmRetCode ProcessPDONodes(bool IsBuild)
{
cout<<"--------ProcessPDONodes----------"<<endl;
	CNodeCollection* objNodeCol;
	ocfmException objocfmException;
	objNodeCol = CNodeCollection::getNodeColObjectPointer();
	CNode *pobjNode, *pobjMNNode = NULL;
	INT32 iTotalBytesMapped = 0;
	INT32 iTotalChainedBytesMapped = 0;
    INT32 iNodeMappedTotalBytes = 0;
	INT32 iNodeRPDOMappedNodeID = 0;
		
	CIndexCollection* objPDOCollection;
	CIndexCollection* pobjIndexCollection;
	/* Check RPDO Mapped objects*/
	INT32 iNodesCount = 0;
	ocfmRetCode stRetInfo;
    
    bool bChangeOffset = false;
	
	iNodesCount = objNodeCol->getCNNodesCount();
	
		#if defined DEBUG	
					cout<< "Nodes count"<<iNodesCount<< endl;
				#endif
                    
    
	try
	{
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

        INT32* pArrangedNodeIDbyStation;
        pArrangedNodeIDbyStation = ArrangeNodeIDbyStation();
		for(INT32 iLoopCount = 0; iLoopCount < objNodeCol->getNumberOfNodes(); iLoopCount++)
		{

			pobjNode = objNodeCol->getNodebyColIndex(iLoopCount);
			/* Process PDO Objects for CN*/
		
				if (pobjNode->getNodeType() == MN )
				{
                    pobjMNNode = pobjNode;
                }
                else
                {
					if(!(pobjNode->HasPdoObjects()))	
					{
				
						continue;
					}
                    EStationType eNodeStation = pobjNode->getStationType();

					/* Empty ProcessImage collection*/
					pobjNode->DeleteCollectionsForPI();
					
					
					objPDOCollection = pobjNode->getPDOIndexCollection();
				
					if(objPDOCollection!= NULL)
					{
							
						pobjIndexCollection = pobjNode->getIndexCollection();
						
                        pobjNode->setPReqActPayloadValue(0);
                        pobjNode->setPResActPayloadValue(0);
                        
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
                              cout<< "index:"<<(char*)pobjBforeSortIndex->getIndexValue() << endl;
					            #endif
							}
                            iNodeMappedTotalBytes = 0;
                            bChangeOffset = false;
                            
                           
							if(pobjBforeSortIndex->getNumberofSubIndexes() > 0)
							{
							
								/* Sort the pdo collection */
								objIndex = getPDOIndexByOffset(pobjBforeSortIndex);
								
								INT32 iSiCount = 1;
								INT32 iSiTotal = objIndex.getNumberofSubIndexes();
						
								#if defined DEBUG	
						            cout<< "iSiTotal:"<<iSiTotal << endl;
					            #endif
								//check whether the channel is activated

									CSubIndex *pobjNoofEntriesSubIndex;
									pobjNoofEntriesSubIndex = pobjBforeSortIndex->getSubIndexbyIndexValue((char *)"00");
									if(NULL != pobjNoofEntriesSubIndex)
									{
										if ( (pobjNoofEntriesSubIndex->getActualValue()!=NULL) 
										&& (0 != strcmp(pobjNoofEntriesSubIndex->getActualValue(),"")) 
										&& !(checkIfValueZero((char*)pobjNoofEntriesSubIndex->getActualValue())) )
										{
											//value is not zero the channel is activated
										}
										else
										{
											// PDO channel is deactivated
											continue;
										}
										
									}
									else
									{
										//no of entries index does not exist
										continue;
									}


								//set the correponding 14xx/01 to f0
								if((true == IsBuild) && (strncmp(objIndex.getIndexValue(), "16", 2) == 0))
								{
								  CIndex *pobjCommIndex;
								  CSubIndex *pobjNodeIDSubIndex;
								  char *pcIdx = subString((char *)objIndex.getIndexValue(), 2, 4);
								  char *pcCommIdx = new char[INDEX_SIZE];
								  strcpy(pcCommIdx, (char *)"14");
								  strcat(pcCommIdx, pcIdx);
								  pobjCommIndex = pobjIndexCollection->getIndexbyIndexValue(pcCommIdx);
								  if(NULL != pobjCommIndex)
								  {
									pobjNodeIDSubIndex = pobjCommIndex->getSubIndexbyIndexValue((char *)"01");
									if(NULL != pobjNodeIDSubIndex)
									{
									  if((NULL != pobjNodeIDSubIndex->getActualValue()) && (0 != strcmp(pobjNodeIDSubIndex->getActualValue(), "")))
									  {
										iNodeRPDOMappedNodeID = GetDecimalValue( (char *) pobjNodeIDSubIndex -> getActualValue());
										if( (CHAINED == eNodeStation) && (BROADCAST_NODEID ==  iNodeRPDOMappedNodeID) )
										{
										  pobjNodeIDSubIndex->setActualValue((char *)"0xF0");
										  bChangeOffset = true;
										}
	                                                
										if((CHAINED != eNodeStation) && (MN_NODEID == iNodeRPDOMappedNodeID ))
										{
										  pobjNodeIDSubIndex->setActualValue((char *)"0x0");
										  bChangeOffset = true;
										}
									  }
									}
								  }
								  delete [] pcCommIdx;
								  delete [] pcIdx;
								}


								while(iSiCount< iSiTotal)
								{
									CSubIndex* pobjSubIdx;
									pobjSubIdx = objIndex.getSubIndex(iSiCount);
									#if defined DEBUG	
						                cout<< "\n pobjSubIdx->getIndexValue():"<<pobjSubIdx->getIndexValue() << endl;
							            cout<< "\n pobjSubIdx->getName():"<<pobjSubIdx->getName() << endl;
					                #endif
									if ( (pobjSubIdx->getActualValue()!=NULL) 
										&& (0 != strcmp(pobjSubIdx->getActualValue(),"")) 
										&& !(checkIfValueZero((char*)pobjSubIdx->getActualValue())) )
								    {
										
										const char* pbActualVal = pobjSubIdx->getActualValue();
										INT32 iLength = strlen(pbActualVal);
										char* pbModuleIndex = NULL;// = new char[INDEX_SIZE + ALLOC_BUFFER];
										pbModuleIndex = subString((char*)pbActualVal, iLength-4,4);
										pbModuleIndex[5] ='\0';
										#if defined DEBUG	
					                        cout<< "pbModuleIndex:"<<pbModuleIndex << endl;
				                        #endif
								
								/* Get the SubIndex*/
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
                                            char acCustomError[200] = {0};
                                            sprintf(acCustomError, "PDO Mapped Module Index Not Found, Index:%s in Node ID:%d", pbModuleIndex, pobjNode->getNodeId());
                                            CopyCustomErrorString(&(objocfmException._ocfmRetCode), acCustomError);

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
                                                char acCustomError[200] = {0};
                                                sprintf(acCustomError, "PDO Mapped Module SubIndex Not Found, Index:%s Subindex:%s in Node ID:%d", pbModuleIndex, pbSubIndex, pobjNode->getNodeId());
                                                CopyCustomErrorString(&(objocfmException._ocfmRetCode), acCustomError);

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
											
											if(uniqueidRefID != NULL)
											{
												DecodeUniqiueIDRef(uniqueidRefID, pobjNode, pdoType, (char*) pobjModuleIndex->getName(), (char*)pobjModuleIndex->getIndexValue());
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
													
												objProcessImage.ModuleName = (char*)malloc(strlen(pbModuleName) + ALLOC_BUFFER);
												strcpy(objProcessImage.ModuleName, pbModuleName);		
												}
												else
												{
													/*If the object name is empty, Ixxxx (xxxx is the index number in hex) */
													pbModuleName = new char[INDEX_LEN + 1 + ALLOC_BUFFER];
													strcpy(pbModuleName, "I");
													strcat(pbModuleName, objIndex.getIndexValue());
													
													objProcessImage.Name = (char*)realloc(objProcessImage.Name, strlen(objProcessImage.Name)+ strlen(pbModuleName) + ALLOC_BUFFER);																		
													strcat(objProcessImage.Name, pbModuleName);
												
													objProcessImage.ModuleName = (char*)malloc(strlen(pbModuleName) + ALLOC_BUFFER);
													strcpy(objProcessImage.ModuleName, pbModuleName);		
												}
												
							
												objProcessImage.ModuleIndex = (char*)malloc(strlen(pbModuleIndex) + ALLOC_BUFFER);
												strcpy(objProcessImage.ModuleIndex, pbModuleIndex);	
												
															
												if(bObjectMapped)
												{
													objProcessImage.VarName = (char*)malloc(strlen(pbModuleName) + ALLOC_BUFFER);
													strcpy(objProcessImage.VarName, pbModuleName);
												}
												else
												{
													if(pbSIdxName != NULL)
													{
														objProcessImage.VarName = (char*)malloc(strlen(pbSIdxName) + ALLOC_BUFFER);
														strcpy(objProcessImage.VarName, pbSIdxName);		
													
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
												
									
												
												objProcessImage.DataInfo.DataSize = dt.DataSize *8;
cout<<"ProcessPDONodes--"<<	dt.DataSize<<"iTotalBytesMapped"<<iTotalBytesMapped<<endl;											
												/* Total bytes Mapped */
												/* PATCH For BUG #10 - START */
												iTotalBytesMapped = iTotalBytesMapped +  dt.DataSize;
												/* PATCH For BUG #10 - END */
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
                                            
                                            if( \
												(true == IsBuild) \
												&& (strncmp(objIndex.getIndexValue(), "16", 2) == 0) \
												&& ( (MN_NODEID == iNodeRPDOMappedNodeID ) || (BROADCAST_NODEID ==  iNodeRPDOMappedNodeID) ) \
												)
											{
                                                char* pbModOffset = new char[strlen(pbActualVal) + 1];
                                                strcpy(pbModOffset, pbActualVal);
                                                INT32 iLength = 0;
                                                char* pcLength = NULL;
                                                //extract the length mapped 1AXX
                                                pcLength = subString((char *)pbActualVal, 2, 4);
                                                iLength = hex2int(pcLength);
                                                
												char* offset = new char[5];
                                                memset(offset, 0, 5*sizeof(char));
                                                if(CHAINED == eNodeStation)
                                                {
                                                    offset  = _IntToAscii(iTotalChainedBytesMapped, &(offset[0]), 16); 
                                                }
                                                else
                                                {
                                                    offset  = _IntToAscii(iNodeMappedTotalBytes, &(offset[0]), 16); 
                                                }
												offset = padLeft(&(offset[0]), '0', 4);
												INT32 iOffsetCopyCount;
                                                for(iOffsetCopyCount = 0; iOffsetCopyCount <= 3; iOffsetCopyCount++)
												{
													pbModOffset[iOffsetCopyCount+2+4] = offset[iOffsetCopyCount];
												}
                                                
                                                pobjIndexCollection->getIndexbyIndexValue((char *)pobjBforeSortIndex->getIndexValue())->getSubIndexbyIndexValue((char *)pobjSubIdx->getIndexValue())->setActualValue(pbModOffset);
                                                
                                                iNodeMappedTotalBytes = iNodeMappedTotalBytes + iLength;
                                                
                                                if(CHAINED == eNodeStation)
                                                {
                                                    iTotalChainedBytesMapped =  iTotalChainedBytesMapped +  iLength;
                                                    
                                                }
                                                
												delete[] pbModOffset;
                                                delete[] pcLength;
                                                delete[] offset;
											}
									}
									iSiCount++;
									}
								}
							}
						}
					}
				}
		delete[] pArrangedNodeIDbyStation;
		//The PI variable name should be unique
		SetUniquePIVarName();
		//find the time of build
		SetBuildTime();

        stRetInfo.code = OCFM_ERR_SUCCESS ;
		return stRetInfo;
	}
	catch(ocfmException& ex)
	{
		return ex._ocfmRetCode;
	}
}

/****************************************************************************************************
* Function Name: CalculatePayload
* Description: calculates the PReq and PRes payload for node
* Return value: void
****************************************************************************************************/
void CalculatePayload()
{
	//printf("CalculatePayload \n");
    CNodeCollection* objNodeCol;
    ocfmException objocfmException;
    objNodeCol = CNodeCollection::getNodeColObjectPointer();
    CNode *pobjNode, *pobjMNNode = NULL;
    INT32 iTotalBytesMapped = 0;
    INT32 iTotalChainedBytesMapped = 0;
    INT32 iNodeMappedTotalBytes = 0;
            
    CIndexCollection* objPDOCollection;
    CIndexCollection* pobjIndexCollection;
    /* Check RPDO Mapped objects*/
    INT32 iNodesCount = 0;
    ocfmRetCode stRetInfo;
        
    iNodesCount = objNodeCol->getCNNodesCount();
                        
        
        if(iNodesCount == 0)
        {               
        exit;
        }

        INT32* pArrangedNodeIDbyStation;
        pArrangedNodeIDbyStation = ArrangeNodeIDbyStation();
        for(INT32 iLoopCount = 0; iLoopCount < objNodeCol->getNumberOfNodes(); iLoopCount++)
        {
        
            pobjNode = objNodeCol->getNodebyColIndex(iLoopCount);
            /* Process PDO Objects for CN*/
                
            if (pobjNode->getNodeType() == MN )
            {
                pobjMNNode = pobjNode;
            }
            else
            {
                if(!(pobjNode->HasPdoObjects()))    
                {
                    continue;
                }
                
                EStationType eNodeStation = pobjNode->getStationType();

                objPDOCollection = pobjNode->getPDOIndexCollection();

                if(objPDOCollection!= NULL)
                {
                            
                    pobjIndexCollection = pobjNode->getIndexCollection();
                                    
                    pobjNode->setPReqActPayloadValue(0);
                    pobjNode->setPResActPayloadValue(0);
                                    
                    for(INT32 iLoopCount = 0; iLoopCount<objPDOCollection->getNumberofIndexes(); iLoopCount++)
                    {
                        CIndex* pobjBforeSortIndex;
                        CIndex objIndex;
                        pobjBforeSortIndex = objPDOCollection->getIndex(iLoopCount);
                        if(!(CheckIfMappingPDO((char*)pobjBforeSortIndex->getIndexValue())))
                        {
                            continue;
                        }
                        iNodeMappedTotalBytes = 0;
                        
                        if(pobjBforeSortIndex->getNumberofSubIndexes() > 0)
                        {
                            /* Sort the pdo collection */
                            objIndex = getPDOIndexByOffset(pobjBforeSortIndex);
                            INT32 iSiCount = 1;
                            INT32 iSiTotal = objIndex.getNumberofSubIndexes();

							INT32 iNodeRPDOMappedNodeID = -1;
                            if(strncmp(objIndex.getIndexValue(), "16", 2) == 0)
                            {
                                CIndex *pobjCommIndex;
                                CSubIndex *pobjNodeIDSubIndex;
                                char *pcIdx = subString((char *)objIndex.getIndexValue(), 2, 4);
                                char *pcCommIdx = new char[INDEX_SIZE];
                                strcpy(pcCommIdx, (char *)"14");
                                strcat(pcCommIdx, pcIdx);
                                pobjCommIndex = pobjIndexCollection->getIndexbyIndexValue(pcCommIdx);
                                if(NULL != pobjCommIndex)
                                {
                                    pobjNodeIDSubIndex = pobjCommIndex->getSubIndexbyIndexValue((char *)"01");
                                    if(NULL != pobjNodeIDSubIndex)
                                    {
										iNodeRPDOMappedNodeID = GetDecimalValue((char*)pobjNodeIDSubIndex->getActualValue());
									}
                                }
                                delete [] pcCommIdx;
                                delete [] pcIdx;
                            }

                            
                            while(iSiCount< iSiTotal)
                            {
                                
                                CSubIndex* pobjSubIdx;
                                pobjSubIdx = objIndex.getSubIndex(iSiCount);
                                
                                iSiCount++;
                                
                                
                                if ( \
									(pobjSubIdx->getActualValue()==NULL) \
									|| (0 == strcmp(pobjSubIdx->getActualValue(),"")) \
									|| (checkIfValueZero((char*)pobjSubIdx->getActualValue())) \
									)
                                {
                                    continue;
                                }
                                
                                const char* pbActualVal = pobjSubIdx->getActualValue();
                                

                                if( \
									(strncmp(objIndex.getIndexValue(), "16", 2) == 0) \
									&& ((MN_NODEID == iNodeRPDOMappedNodeID) || (BROADCAST_NODEID == iNodeRPDOMappedNodeID)) \
									) 
                                {
                                    char* pbModOffset = new char[strlen(pbActualVal) + 1];
                                    strcpy(pbModOffset, pbActualVal);
                                    INT32 iLength = 0, iOffset = 0;
    
                                    char* pcLength = NULL;
                                    pcLength = subString((char *)pbActualVal, 2, 4);
                                    iLength = hex2int(pcLength);
    
                                    char* offset = NULL;
                                    offset = subString((char *)pbActualVal, 6, 4);
                                    iOffset = hex2int(offset);
    
                                    iNodeMappedTotalBytes = iOffset + iLength;
    
                                    if(CHAINED == eNodeStation)
                                    {
                                        iTotalChainedBytesMapped =  iOffset +  iLength;
                                    }
                                    if(BROADCAST_NODEID == iNodeRPDOMappedNodeID)
                                    {
                                        pobjNode->setPReqActPayloadValue( (iOffset +  iLength) / 8);
                                    }
                    
                                    delete[] pbModOffset;
                                    delete[] pcLength;
                                    delete[] offset;
                
                                }
                                if(strncmp(objIndex.getIndexValue(), "1A", 2) == 0)
                                {
                                    char* pbModOffset = new char[strlen(pbActualVal) + 1];
                                    strcpy(pbModOffset, pbActualVal);
                                    INT32 iLength = 0;
                                    char* pcLength = NULL;
                                    pcLength = subString((char *)pbActualVal, 2, 4);
                                    iLength = hex2int(pcLength);
                        
                                    char* pcOffset = NULL;
                                    pcOffset = subString((char *)pbActualVal, 6, 4);
                                    INT32 iOffset = 0;
                                    iOffset = hex2int(pcOffset);
                                    
                                    pobjNode->setPResActPayloadValue((iOffset + iLength) / 8);
                                                                    
                                    delete[] pbModOffset;
                                    delete[] pcLength;
                                    delete[] pcOffset;
                                }
                            }
                        }
                    }
                }
                UpdatePreqActLoad(pobjNode);
                UpdatePresActLoad(pobjNode);
            }
        }
        if( (NULL != pobjMNNode) && (true == IsPresMN()) )
        {
            pobjMNNode->setPResActPayloadValue(iTotalChainedBytesMapped / 8);
            UpdatePresActLoad(pobjMNNode);
        }
        delete[] pArrangedNodeIDbyStation;
}
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
		//For Byte Packing
		for (iCtr = 0 , iCount = 0; iCtr < iLength; iCtr++, iCount++ )
		{
			abTempCnObd[iCount] = (unsigned char)( ( abCnObd[ iCtr ] << 4 ) | abCnObd[ iCtr + 1 ] );
			iCtr++;
		}
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
* Function Name: WriteXAPElements
* Description: 
* Return value: void
****************************************************************************************************/
void WriteXAPElements(ProcessImage aobjPICol[], xmlTextWriterPtr& pxtwWriter,INT32 iVarCount, EPIDirectionType piType)
{
	INT32 iBytesWritten;
	INT32 iHighBitOffset = 0;
	INT32 iHighBitOffsetDatasize = 0;

	if (iVarCount!=0)
	{
	/* Start an element named "Channel". Since thist is the first
     * element, this will be the root element of the document. */
    iBytesWritten = xmlTextWriterStartElement(pxtwWriter, BAD_CAST "ProcessImage");
    if (iBytesWritten < 0)
    {
        return;
    }
    if(piType ==INPUT)
					iBytesWritten = xmlTextWriterWriteAttribute(pxtwWriter, BAD_CAST "type",      
                    BAD_CAST "output");
				else if(piType ==OUTPUT)
					iBytesWritten = xmlTextWriterWriteAttribute(pxtwWriter, BAD_CAST "type",      
                    BAD_CAST "input");
				
                                  
    if (iBytesWritten < 0)
    {
        return;
    }
    
    //find the size and write it to attribute
	for(INT32 iLoopCount =0 ; iLoopCount < iVarCount ; iLoopCount++)
			{
					ProcessImage objProcessImage;
					objProcessImage = aobjPICol[iLoopCount];

					if(iHighBitOffset < ((objProcessImage.ByteOffset * 8) + objProcessImage.BitOffset ) )
					{
						//save the higher offset of process image and its datasize
						iHighBitOffset = (objProcessImage.ByteOffset * 8) + objProcessImage.BitOffset ; //multiply by 8 for byte to bit conversion
						iHighBitOffsetDatasize = objProcessImage.DataInfo.DataSize;
					}
			}
		char * pbPISize = new char[20];
		pbPISize = _IntToAscii((iHighBitOffset + iHighBitOffsetDatasize)/8, pbPISize, 10); //divide by 8 for bit to bytt conversion
	    iBytesWritten = xmlTextWriterWriteAttribute(pxtwWriter, BAD_CAST "size",
	                                     BAD_CAST pbPISize );
	    delete[] pbPISize;
	    if (iBytesWritten < 0)
	    {
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
        return;
    }
			


    

    /* Add an attribute with name "Name" and value  to channel. */
    iBytesWritten = xmlTextWriterWriteAttribute(pxtwWriter, BAD_CAST "Name",      
                                   BAD_CAST objProcessImage.Name);
                                  
    if (iBytesWritten < 0)
    {
        return;
    }
    
    


				/* Add an attribute with name "DataType" and value to channel */
    iBytesWritten = xmlTextWriterWriteAttribute(pxtwWriter, BAD_CAST "dataType",
                                     BAD_CAST objProcessImage.DataInfo._dt_Name );
    if (iBytesWritten < 0)
    {
        return;
    }
    
    ///* Add an attribute with name "dataSize" and value to channel */
    char * pbDataSize = new char[20];
    pbDataSize = _IntToAscii(objProcessImage.DataInfo.DataSize,pbDataSize,10);
    
    iBytesWritten = xmlTextWriterWriteAttribute(pxtwWriter, BAD_CAST "dataSize",
                                     BAD_CAST pbDataSize );
    if (iBytesWritten < 0)
    {
        return;
    }
			
   
     ///* Add an attribute with name "dataSize" and value to channel */
  		char* pbByteOffset = new char[6];
				pbByteOffset = _IntToAscii(objProcessImage.ByteOffset, pbByteOffset, 16);
				pbByteOffset = ConvertToHexformat(pbByteOffset, 4, 1);
			
    iBytesWritten = xmlTextWriterWriteAttribute(pxtwWriter, BAD_CAST "PIOffset",
                                     BAD_CAST pbByteOffset );
    if (iBytesWritten < 0)
    {
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
        return;
    }
    ///* Close the element named Channel. */
    iBytesWritten = xmlTextWriterEndElement(pxtwWriter);
    if (iBytesWritten < 0)
    {
        return;
    }
			}
	 ///* Close the element named ProcessImage. */
   iBytesWritten = xmlTextWriterEndElement(pxtwWriter);
   if (iBytesWritten < 0)
   {
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

    /* Create a new XmlWriter for DOM, with no compression. */
    pxtwWriter = xmlNewTextWriterDoc(&pxdDoc, 0);
    if (pxtwWriter == NULL) {
        return;
    }

    /* Start the document with the xml default for the version,
     * encoding UTF-8 and the default for the standalone
     * declaration. */
    iBytesWritten = xmlTextWriterStartDocument(pxtwWriter, NULL, MY_ENCODING, NULL);
    if (iBytesWritten < 0) {
        return;
    }

			/* Write a comment as child of ORDER */
    char* pcComment = new char[strlen(BUILD_COMMENT) + BUILDTIME_BUF_LEN];
    strcpy(pcComment,  BUILD_COMMENT);
    strcat(pcComment, GetBuildTime());
		 iBytesWritten = xmlTextWriterWriteComment(pxtwWriter, BAD_CAST pcComment);
		 delete[] pcComment;
    if (iBytesWritten < 0)
     {
        return;
    }

    iBytesWritten = xmlTextWriterStartElement(pxtwWriter, BAD_CAST "ApplicationProcess");
    if (iBytesWritten < 0) {
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
        return;
    }
    iBytesWritten = xmlTextWriterEndDocument(pxtwWriter);
    if (iBytesWritten < 0) {
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
			ProcessImage aobjPiInCol[PI_VAR_COUNT] = {};
			ProcessImage aobjPiOutCol[PI_VAR_COUNT] = {};
			GroupInOutPIVariables(aobjPiInCol, aobjPiOutCol);
			
			StartXAPxml(pxtwWriter, pxdDoc);	
			if(iInVars !=0)
			WriteXAPElements(aobjPiInCol, pxtwWriter, iInVars, INPUT);
			
			if(iOutVars !=0)			
			WriteXAPElements(aobjPiOutCol, pxtwWriter, iOutVars, OUTPUT);


			strcpy(xapFileName, pbFileName);
			strcat(xapFileName, ".xml");
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
			
			CNode objNode;
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
			//continue with process
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
					ocfmException objException;				
					objException.ocfm_Excpetion(OCFM_ERR_INVALID_ATTRIBUTETYPE);
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
		
		INT32 iTempNodeCount = pobjNodeCollection->getNumberOfNodes();		
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
		if(iTempIndexCount == 0)
		{
			ocfmException objException;
			objException.ocfm_Excpetion(OCFM_ERR_NO_INDEX_FOUND);
			throw &objException;
		}
		else if(iIndexPos >= iTempIndexCount)
		{
			ocfmException objException;
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
						stErrorInfo.code = OCFM_ERR_INVALID_ATTRIBUTETYPE;
						return stErrorInfo;
			}
	}
	catch(ocfmException* ex)
	{
		return ex->_ocfmRetCode;
	}
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
				//continue with process
			}
			else
			{
				ocfmException objException;				
				objException.ocfm_Excpetion(OCFM_ERR_INVALID_SUBINDEXID);
				throw objException;
			}

			objIndex.setNodeID(objNode.getNodeId());
			pobjNodeCollection= CNodeCollection::getNodeColObjectPointer();
			objNode = pobjNodeCollection->getNode(enumNodeType, iNodeID);

			pobjIndexCollection = objNode.getIndexCollection();
			pobjSubIndex =pobjIndexCollection->getIndex(iIndexPos);
			
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
		
		INT32 iTempNodeCount = pobjNodeCollection->getNumberOfNodes();		
		if(iNodePos >= iTempNodeCount)
		{
			ocfmException objException;

			objException.ocfm_Excpetion(OCFM_ERR_INVALID_NODEPOS);		
			throw &objException;
		}
		else if(iTempNodeCount == 0)
		{
			ocfmException objException;
			objException.ocfm_Excpetion(OCFM_ERR_NO_NODES_FOUND);		
			throw &objException;
		}
	
		objNode = pobjNodeCollection->getNodebyCollectionIndex(iNodePos);
		pobjIndexCollection = objNode.getIndexCollection();
		
		INT32 iTempIndexCount = pobjIndexCollection->getNumberofIndexes();
		if(iTempIndexCount == 0)
		{
			ocfmException objException;
			objException.ocfm_Excpetion(OCFM_ERR_NO_INDEX_FOUND);
			throw &objException;
		}
		else if(iIndexPos >= iTempIndexCount)
		{
			ocfmException objException;
			objException.ocfm_Excpetion(OCFM_ERR_INVALID_INDEXPOS);
			throw &objException;
		}
		
		pobjIndexPtr = pobjIndexCollection->getIndex(iIndexPos);
		
		INT32 iTempSubIndexCount = pobjIndexPtr->getNumberofSubIndexes();

		if(iSubIndexPos >= iTempSubIndexCount)
		{
			ocfmException objException;
			objException.ocfm_Excpetion(OCFM_ERR_INVALID_SUBINDEXPOS);		
			throw &objException;
		}
		else if(iTempSubIndexCount == 0)
		{
			ocfmException objException;
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
					stErrorInfo.code = OCFM_ERR_INVALID_ATTRIBUTETYPE;
					return stErrorInfo;
		}			
	}
	catch(ocfmException* ex)
	{
		return ex->_ocfmRetCode;
	}
/////////////////////////////////////////////////////////////
//if(iIndexPos == 36)
//{
//cout<<"---GetSubIndexAttributesbyPositions--------"<<iNodePos<<iIndexPos<<iSubIndexPos<<pbOutAttributeValue<<endl;
//}
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
cout<<"-------getPIName from Apioperations-------------"<<endl;
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
			//continue with process
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
			//continue with process
		}
		else
		{	
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
	
  		xmlTextReaderPtr pxReader;
		INT32 iRetVal;
		CObjectDictionary* objDict;
		objDict = CObjectDictionary::getObjDictPtr();
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
					//continue with process
				}
			}
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
/////////////////////////////////////////
cout<<"------GetNodeAttributesbyNodePos--------"<<endl;
	ocfmRetCode stErrorInfo;
	
	INT32 iTempNodeCount;
	try
	{
		// We do not have support for multiple MNs in this version.
		GetNodeCount( MN_NODEID, &iTempNodeCount);
		
		if(iNodePos > iTempNodeCount)
		{
			ocfmException objException;
			objException.ocfm_Excpetion(OCFM_ERR_INVALID_NODEPOS);		
			throw &objException;
		}
		else if(iTempNodeCount == 0)
		{
			ocfmException objException;
			objException.ocfm_Excpetion(OCFM_ERR_NO_NODES_FOUND);		
			throw &objException;
		}
		CNode objNode;		
		CNodeCollection *pobjNodeCollection;
		CIndexCollection *pobjIndexCollection;

		pobjNodeCollection= CNodeCollection::getNodeColObjectPointer();
		
		pobjIndexCollection = objNode.getIndexCollection();

		objNode = pobjNodeCollection->getNodebyCollectionIndex(iNodePos);
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
			//continue with process
		}
		else
		{	
			// Node Doesn't Exist
			ocfmException objException;
			objException.ocfm_Excpetion(OCFM_ERR_INVALID_NODEID);
			throw &objException;
		}

		pobjNodeCollection = CNodeCollection::getNodeColObjectPointer();
		objNode = pobjNodeCollection->getNodebyCollectionIndex(iNodePos);
		pobjIndexCollection = objNode.getIndexCollection();
		
		INT32 iTempIndexCount = pobjIndexCollection->getNumberofIndexes();
		
		if(iTempIndexCount == 0)
		{
			ocfmException objException;
			objException.ocfm_Excpetion(OCFM_ERR_NO_INDEX_FOUND);
			throw &objException;
		}
		else if(iTempIndexCount < iIndexPos)
		{
			ocfmException objException;
			objException.ocfm_Excpetion(OCFM_ERR_INVALID_INDEXPOS);
			throw &objException;
		}
		
		pobjIndex = pobjIndexCollection->getIndex(iIndexPos);
		
		if(pobjIndex->getIndexValue() != NULL)
			strcpy(pbOutIndexID, (char *) pobjIndex->getIndexValue());
		
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
			ocfmException objException;
			objException.ocfm_Excpetion(OCFM_ERR_NO_NODES_FOUND);
			throw &objException;
		}
		else if(iTempNodeCount < iNodePos)
		{
			ocfmException objException;
			objException.ocfm_Excpetion(OCFM_ERR_INVALID_NODEPOS);
			throw &objException;
		}
		
		objNode = pobjNodeCollection->getNodebyCollectionIndex(iNodePos);
		pobjIndexCollection = objNode.getIndexCollection();
		
		INT32 iTempIndexCount = pobjIndexCollection->getNumberofIndexes();
		
		if(iTempIndexCount == 0)
		{
			ocfmException objException;
			objException.ocfm_Excpetion(OCFM_ERR_NO_INDEX_FOUND);
			throw &objException;
		}
		else if(iTempIndexCount < iIndexPos)
		{
			ocfmException objException;
			objException.ocfm_Excpetion(OCFM_ERR_INVALID_INDEXPOS);
			throw &objException;
		}
		
		pobjIndex = pobjIndexCollection->getIndex(iIndexPos);
		
		if(pobjIndex->getIndexValue() != NULL)
			strcpy(pbOutIndexID, (char *) pobjIndex->getIndexValue());
		

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
			//continue with process
		}
		else
		{	
			ocfmException objException;
			objException.ocfm_Excpetion(OCFM_ERR_INDEXID_NOT_FOUND);
			throw &objException;
		}
		
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
			ocfmException objException;
			objException.ocfm_Excpetion(OCFM_ERR_NO_NODES_FOUND);
			throw &objException;
		}
		else if(iTempNodeCount < iNodePos)
		{
			ocfmException objException;
			objException.ocfm_Excpetion(OCFM_ERR_INVALID_NODEPOS);
			throw &objException;
		}
		
		objNode = pobjNodeCollection->getNodebyCollectionIndex(iNodePos);		
		pobjIndexCollection = objNode.getIndexCollection();
		
		INT32 iTempIndexCount = pobjIndexCollection->getNumberofIndexes();		
		if(iTempIndexCount == 0)
		{
			ocfmException objException;
			objException.ocfm_Excpetion(OCFM_ERR_NO_INDEX_FOUND);
			throw &objException;
		}
		else if(iTempIndexCount < iIndexPos)
		{
			ocfmException objException;
			objException.ocfm_Excpetion(OCFM_ERR_INVALID_INDEXPOS);
			throw &objException;
		}
		
		pobjSubIndex = pobjIndexCollection->getIndex(iIndexPos);
			
		INT32 iTempSubIndexCount = pobjSubIndex->getNumberofSubIndexes();
		if(iTempSubIndexCount == 0)
		{		
			ocfmException objException;
			objException.ocfm_Excpetion(OCFM_ERR_NO_SUBINDEXS_FOUND);
			throw &objException;			
		}
		else if(iTempSubIndexCount < iSubIndexPos)
		{
			ocfmException objException;
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
	try
	{	
		bool bFlag = false;
		stErrorInfo = IfNodeExists(iNodeID, enumNodeType, &iNodePos, bFlag);
		
		if(stErrorInfo.code == 0 && bFlag ==true)
		{
		}
		else
		{
			ocfmException objException;
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

		pbTempPjtName = new char[300];
		sprintf(pbTempPjtName, "%s%s/%s.oct", pbProjectPath, pbProjectName, pbProjectName);

		iIntStat = stat(pbTempPjtName,&stFileInfo);
		if(iIntStat == 0) {
			#if defined DEBUG
				cout << "\n\n\nProject File Already exists\n" << endl;
			#endif
		} 
		else 
		{ 				
			#if defined(_WIN32) && defined(_MSC_VER)
			{
				sprintf(pbTempPath, "%s\\%s", pbProjectPath, pbProjectName);
				mkdir(pbTempPath);	
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
			mkdir(pbTempPath);	
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
				char* pbFileName;
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
				SaveNode(pbFileName, objNode.getNodeId(), objNode.getNodeType());	
				delete [] pbFileName;			

			}
			stErrorInfo.code = OCFM_ERR_SUCCESS;
			delete [] pbTempPath;
			return stErrorInfo;
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
        char* pbHexIndexNo = new char[5];
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
                strcpy(pbHexIndexNo, "0x");
			
				pbIndexNo = _IntToAscii(objNodeCol->getCNNodesCount(), pbIndexNo, 16);
				pbIndexNo = padLeft(pbIndexNo, '0', 2);
				strcat(pbHexIndexNo, pbIndexNo);
                SetSIdxValue(pbIdx, pbSIdx, pbHexIndexNo, pobjIdxCol, iMNNodeID, MN, false);
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
						pbVal = _IntToAscii(iConfigTime, pbVal, 10);

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
						strcpy(pbSidx, "02");
						SetSIdxValue(pbMNIndex, pbSidx, (char*)"", pobjIdxCol, pobjNode->getNodeId(), MN, true);
				
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
  return (GenerateMNOBD(false));
}

/****************************************************************************************************
* Function Name: GenerateMNOBD
* Description: 
* Return value: ocfmRetCode
****************************************************************************************************/
ocfmRetCode GenerateMNOBD(bool IsBuild)
	{
////////////////////////////////////////////////
cout<<"----------GenerateMNOBD----------"<<endl;
		CNode objNode;		
		CNode *pobjMNNode;
		CNodeCollection *pobjNodeCollection = NULL;
		CIndexCollection * objMNIndexCol;
		CSubIndex * objSubIdex;
		CIndex* pobjIndex;			
		char* pbMNIndex = new char[INDEX_LEN];
		char* pbIdx =  new char[SUBINDEX_LEN];
		char* pbMappIdx =  new char[SUBINDEX_LEN];
        char* pbMappNodeID =  new char[SUBINDEX_LEN];
		ocfmRetCode stRetInfo;
		
		ocfmException objocfmException;
        INT32 iOutPrevSubIndex = 0 ;
        INT32 iOutPrevSize = 0;
        INT32 iChainOutPrevSubIndex = 0 ;
        INT32 iChainOutPrevSize = 0;
		INT32 iInPrevSubIndex = 0 ;
		INT32 iInPrevSize = 0;
        INT32 iIndexPos = 0;
        INT32 iRxChannelCount = 0;
        INT32 iTxChannelCount = 0;

        EStationType eCurrCNStation;
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
              stRetInfo = ProcessPDONodes(IsBuild);
				#if defined DEBUG	
						cout<< "Nodes Processed"<<endl;
					#endif
				if(stRetInfo.code != OCFM_ERR_SUCCESS)
				{
					return stRetInfo;
				}
				
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

				pobjIndexCollection->DeletePDOs();
				// Delete Process Image Objects
				pobjIndexCollection->DeletePIObjects();
	
				// Autogenertate other indexs 
				AuotgenerateOtherIndexs(pobjMNNode);
						#if defined DEBUG	
						cout<< "MN Node Object dictionary deleted"<<endl;
					#endif
				/* Add other Indexes than PDO*/
					#if defined DEBUG	
						cout<< "Index added"<<endl;
					#endif
					#if defined DEBUG	
						cout<< "Deleted"<<endl;
					#endif
			}
            
            bool bIsPresMN = false;
            bIsPresMN = IsPresMN();
            
            if(true == bIsPresMN)
            {
                iTxChannelCount = 1;
            }

            INT32* pArrangedNodeIDbyStation;
            pArrangedNodeIDbyStation = ArrangeNodeIDbyStation();
			for(INT32 iLoopCount =0; iLoopCount<pobjNodeCollection->getNumberOfNodes(); iLoopCount++)
			{
              objNode = pobjNodeCollection->getNodebyCollectionIndex(iLoopCount);         			
				if(objNode.getNodeType() == CN)
				{
                    eCurrCNStation = objNode.getStationType();

					objMNIndexCol =  pobjMNNode->getIndexCollection();
									
					if(objNode.MNPDOOUTVarCollection.Count()!=0)
					{
                        if(CHAINED != eCurrCNStation)
                        {
                            iOutPrevSubIndex = 0;
                            iOutPrevSize = 0;
                        }
                        else
                        {
                            iOutPrevSubIndex = iChainOutPrevSubIndex;
                            iOutPrevSize = iChainOutPrevSize;
                        }
						/* Create PDO_TxCommParam_XXh_REC 1800 INdex*/
						CIndex* pobjIndex;	
						strcpy(pbMNIndex, "18");
                        if(CHAINED != eCurrCNStation)
                        {
                            pbIdx = _IntToAscii(iTxChannelCount, pbIdx, 16);
                            iTxChannelCount++;

						    pbIdx = padLeft(pbIdx, '0', 2);
						    pbMNIndex =strcat(pbMNIndex, pbIdx);
						    stRetInfo = AddIndex(MN_NODEID, MN, pbMNIndex);

                            //to write cn node id in 18XX/01
                            pbMappNodeID = _IntToAscii(objNode.getNodeId(), pbMappNodeID, 10);//###not adding the cn node id here
                        }
                        else
                        {
                            //1800 is used of PRes chained station
                            iIndexPos = 0;
                            strcpy(pbMNIndex, (char *)"1800");
                            strcpy(pbIdx, (char *)"00");
                            stRetInfo = IfIndexExists(MN_NODEID, MN, pbMNIndex, &iIndexPos);
                            if(stRetInfo.code != OCFM_ERR_SUCCESS)
                            {
                                stRetInfo = AddIndex(MN_NODEID, MN, pbMNIndex);
                            }

                            //to write 0 in 18XX/01 to indicate PRes MN
                            strcpy(pbMappNodeID, (char *)"0x0");
                        }
							/* set bFlag to true for 1800*/
						pobjIndex = objMNIndexCol->getIndexbyIndexValue(pbMNIndex);
							
						if(stRetInfo.code != OCFM_ERR_SUCCESS)
						{
							objocfmException.ocfm_Excpetion(stRetInfo.code);
							throw objocfmException;
						}
						
						char* pbSidx =  new char[SUBINDEX_LEN];
						strcpy(pbSidx, "01");
						SetSubIndexAttributes(MN_NODEID, MN, pbMNIndex, pbSidx, pbMappNodeID,(char*)"NodeID_U8", TRUE);
									
						strcpy(pbMNIndex, "1A");
						pbMNIndex =strcat(pbMNIndex, pbIdx);
						/* Set the MN's PDO Index*/
                        iIndexPos = 0;
                        stRetInfo = IfIndexExists(MN_NODEID, MN, pbMNIndex, &iIndexPos);
                        if(stRetInfo.code != OCFM_ERR_SUCCESS)
                        {
                            stRetInfo = AddIndex(MN_NODEID, MN, pbMNIndex);
                        }
					
						pobjIndex->setFlagIfIncludedCdc(TRUE);
							
						if(stRetInfo.code != OCFM_ERR_SUCCESS)
						{
							objocfmException.ocfm_Excpetion(stRetInfo.code);
							throw objocfmException;
						}
					
						for(INT32 iLoopCount=0; iLoopCount<objNode.MNPDOOUTVarCollection.Count() ; iLoopCount++)
						{
							MNPdoVariable stMNPdoVar;
							stMNPdoVar = objNode.MNPDOOUTVarCollection[iLoopCount];	
							pobjIndex = objMNIndexCol->getIndexbyIndexValue(pbMNIndex);

							if(pobjIndex !=NULL)
							{
							 pobjIndex->setFlagIfIncludedCdc(TRUE);				
								GetMNPDOSubIndex(stMNPdoVar, iOutPrevSubIndex, pobjIndex, pbMNIndex, iOutPrevSize);
								iOutPrevSize = iOutPrevSize + stMNPdoVar.DataSize;
							}
					
						}
                        
                        if(CHAINED == eCurrCNStation)
                        {
                          iChainOutPrevSubIndex = iOutPrevSubIndex;
                          iChainOutPrevSize = iOutPrevSize;
                        }
		
					}
											
				if(objNode.MNPDOINVarCollection.Count()!=0)
					{
												
						/* Create PDO_TxCommParam_XXh_REC 1800 INdex*/
						strcpy(pbMNIndex, "14");
                        pbIdx = _IntToAscii(iRxChannelCount, pbIdx, 16);
                        iRxChannelCount++;
						
						pbIdx = padLeft(pbIdx, '0', 2);
						pbMNIndex =strcat(pbMNIndex, pbIdx);
                        
						stRetInfo = AddIndex(MN_NODEID, MN, pbMNIndex);
						/* set bFlag to true for 1800*/
						pobjIndex = objMNIndexCol->getIndexbyIndexValue(pbMNIndex);
						if(pobjIndex != NULL)
							pobjIndex->setFlagIfIncludedCdc(TRUE);
						
						iInPrevSubIndex = 0;
						iInPrevSize = 0;
						if(stRetInfo.code != OCFM_ERR_SUCCESS)
						{
							objocfmException.ocfm_Excpetion(stRetInfo.code);
							throw objocfmException;
						}
						
						pbMappIdx = _IntToAscii((objNode.getNodeId()), pbMappIdx, 10);
						char* pbSidx =  new char[SUBINDEX_LEN];
						strcpy(pbSidx, "01");
						SetSubIndexAttributes(MN_NODEID, MN, pbMNIndex, pbSidx, pbMappIdx,(char*)"NodeID_U8", TRUE);
						delete[] pbSidx;
				
							
						strcpy(pbMNIndex, "16");
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
							GetMNPDOSubIndex(stMNPdoVar, iInPrevSubIndex, pobjIndex, pbMNIndex, iInPrevSize);				
							iInPrevSize = iInPrevSize + stMNPdoVar.DataSize ;	

						}
					
					}
				
			}
		}
        setPresMNNodeAssigmentBits();
        
        delete[] pArrangedNodeIDbyStation;
		setFlagForRequiredMNIndexes(MN_NODEID);
	}
	
	catch(ocfmException& objocfmException)
	{
		return objocfmException._ocfmRetCode;
	}

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

						}
						
							/* if greater no change*/
						if(stSize16OUTOffset.currOffset >= stSize32OUTOffset.currOffset)
						{}
						else
						{
								stSize16OUTOffset.prevOffset = stSize16OUTOffset.currOffset ;
								stSize16OUTOffset.currOffset = stSize32OUTOffset.currOffset;

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

						}
						
							/* if greater no change*/
						if(stSize16INOffset.currOffset >= stSize32INOffset.currOffset)
						{}
						else
						{
								stSize16INOffset.prevOffset = stSize16INOffset.currOffset ;
								stSize16INOffset.currOffset = stSize32INOffset.currOffset;

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
//////////////////////////////////////////////////////
cout<<"----------OpenProject-------------------------------"<<endl;
	CNodeCollection *pobjNodeCollection;
	pobjNodeCollection = CNodeCollection::getNodeColObjectPointer();
	xmlTextReaderPtr pxReader;
    	INT32 iRetVal;
	char *pbFileName;	
	
	
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
		/* PATCH For BUG #7 - START */
		char* pbPresTimeoutVal = new char[50];
		pbPresTimeoutVal[0] = 0;
		/* PATCH For BUG #7 - END */
        for(INT32 iLoopCount = 0; iLoopCount < pobjNodeCollection->getNumberOfNodes(); iLoopCount++)
        {
            objNode = pobjNodeCollection->getNodebyCollectionIndex(iLoopCount);
            
            iNodeType = objNode.getNodeType();
            iNodeID = objNode.getNodeId();
			/* PATCH For BUG #7 - START */
			#if 0
			if(iNodeID == 240)
			{
				GetSubIndexAttributes(iNodeID,iNodeType,"1f92",	"02",ACTUALVALUE,pbPresTimeoutVal);
				cout<<"Actual Value"<<iNodeID<<pbPresTimeoutVal<<endl;
				GetSubIndexAttributes(iNodeID,iNodeType,"1f92",	"02",DEFAULTVALUE,pbPresTimeoutVal);
				cout<<"Default Value"<<iNodeID<<pbPresTimeoutVal<<endl;
			}
			#endif	
			/* PATCH For BUG #7 - END */
            copyPDODefToAct(iNodeID, iNodeType);
            copyMNPropDefToAct(iNodeID, iNodeType);
			/* PATCH For BUG #7 - START */
			if((iNodeID != 240) && (iNodeType != MN))
			{
				char* strConvertedValue = NULL;
				strConvertedValue = new char[SUBINDEX_LEN];
				strConvertedValue = _IntToAscii(iNodeID, strConvertedValue, 16);
				strConvertedValue = padLeft(strConvertedValue, '0', 2);
				GetSubIndexAttributes(240,MN,"1f92",strConvertedValue,ACTUALVALUE,pbPresTimeoutVal);
				cout<<"Actual Value"<<iNodeID<<pbPresTimeoutVal<<endl;	
				if(((pbPresTimeoutVal == NULL) || (strcmp(pbPresTimeoutVal, "") == 0)) || (!(ValidateCNPresTimeout(strConvertedValue, pbPresTimeoutVal))))
				{
			          calculateCNPollResponse(iNodeID, iNodeType);				//
				}

				delete[] strConvertedValue;
			}
        }
		delete[] pbPresTimeoutVal;
	}
	/* PATCH For BUG #7 - END */

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
///////////////////////////////////////////////////
cout<<"-------------------processProjectXML--------------"<<endl;
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
//////////////////////////////////////////
cout<<"---------------getandCreateNode-------------"<<endl;
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
			}
			else
			{
				#if defined DEBUG
					cout << "\ngetandCreateNode returning false" << endl;
				#endif
				pbXdcPath = NULL;
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

xmlTextWriterPtr pxtwWriter;
xmlDocPtr pxdDoc;
INT32 iBytesWritten;
char* pbFileName;
pbFileName = new char[MAX_FILE_PATH_SIZE];

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
	ocfmException objException;
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
	ocfmException objException;
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
	ocfmException objException;
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
	ocfmException objException;
	objException.ocfm_Excpetion(OCFM_ERR_XML_WRITER_START_ELT_FAILED);
	throw &objException;
}

	xmlTextWriterSetIndent(pxtwWriter, 1);
	// Start Auto Tag		
	iBytesWritten = xmlTextWriterStartElement(pxtwWriter, BAD_CAST "Auto");
	if (iBytesWritten < 0) 
	{
		printf("testXmlwriterMemory: Error at xmlTextWriterStartElement\n");
		ocfmException objException;
		objException.ocfm_Excpetion(OCFM_ERR_XML_WRITER_START_ELT_FAILED);
		throw &objException;
	}

	{
        if(pobjPjtSettings->getGenerateAttr() == NO_AG)
		{
			iBytesWritten = xmlTextWriterWriteAttribute(pxtwWriter, BAD_CAST "Generate", BAD_CAST "NO");
		}
        else if(pobjPjtSettings->getGenerateAttr() == YES_AG)
		{
			iBytesWritten = xmlTextWriterWriteAttribute(pxtwWriter, BAD_CAST "Generate", BAD_CAST "YES");
		}
	}
	
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
		CNode objNode;		
		CNodeCollection *pobjNodeCollection = NULL;
		CIndex objIndex;
		objIndex.setNodeID(objNode.getNodeId());
		pobjNodeCollection = CNodeCollection::getNodeColObjectPointer();

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
		pbTempNodeName = new char[50];
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
cout<<"---------CreateMNPDOVar--------"<<endl;
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
                    PDODataType dt;
                    switch (iDataSize)
                    {
                        case 8:
                            dt = UNSIGNED8;
                            break;
                        case 16:
                            dt = UNSIGNED16;
                            break;
                        case 32:
                            dt = UNSIGNED32;
                            break;
                        case 64:
                            break;
                        default:
                            break;
                    }					
					if(enumPdoType == PDO_TPDO)
					{
						objpi =  getPIAddress(dt, INPUT, iOffset, iDataSize);
					
					}
					else if(enumPdoType == PDO_RPDO)
					{
						objpi =  getPIAddress(dt, OUTPUT, iOffset, iDataSize);
					}
					
					
					break;					
				case SINT  :						
					if(enumPdoType == PDO_TPDO)
					{
							objpi =  getPIAddress(INTEGER8, INPUT, iOffset, iDataSize);
					}
					
					else if(enumPdoType == PDO_RPDO)
					{
							objpi =  getPIAddress(INTEGER8, OUTPUT, iOffset, iDataSize);
					}
					break;	
				case UINT :											
					if(enumPdoType == PDO_TPDO)
					{
							objpi =  getPIAddress(UNSIGNED16, INPUT, iOffset, iDataSize);
					}
					else if(enumPdoType == PDO_RPDO)
					{
						objpi =  getPIAddress(UNSIGNED16, OUTPUT, iOffset, iDataSize);
					}
					break;									
				case INT :											
					if(enumPdoType == PDO_TPDO)
					{
						objpi =  getPIAddress(INTEGER16, INPUT, iOffset, iDataSize);
					}
					else if(enumPdoType == PDO_RPDO)
					{
						objpi =  getPIAddress(INTEGER16, OUTPUT, iOffset, iDataSize);
					}
					break;
				case UDINT:											
					if(enumPdoType == PDO_TPDO)
					{
						objpi =  getPIAddress(UNSIGNED32, INPUT, iOffset, iDataSize);
					}
					else if(enumPdoType == PDO_RPDO)
					{
						objpi =  getPIAddress(UNSIGNED32, OUTPUT, iOffset, iDataSize);
					}
					break;		
				case DINT:
				case REAL:											
					if(enumPdoType == PDO_TPDO)
					{
						objpi =  getPIAddress(INTEGER32, INPUT, iOffset, iDataSize);
					}
					else if(enumPdoType == PDO_RPDO)
					{
						objpi =  getPIAddress(INTEGER32, OUTPUT, iOffset, iDataSize);
					}
					break;																			
				case LINT:
				case LREAL:
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
				ocfmException objException;
				objException.ocfm_Excpetion(OCFM_ERR_PROJECT_SETTINGS);
				throw &objException;
			}
			
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
			char* pbSidx =  new char[SUBINDEX_LEN + ALLOC_BUFFER];
			CIndex* pobjIndex;
			CIndexCollection* pobjIdxCol;
			INT32 iConfigDate;
			INT32 iConfigTime;
            int IndexPos;
			
			pobjIdxCol = pobjNode->getIndexCollection();
			/* 1006*/
				strcpy(pbMNIndex, "1006");
						#if defined DEBUG	
					cout << "string copied" << endl;
				
				#endif
                    retCode = IfIndexExists(MN_NODEID, MN, pbMNIndex, &IndexPos);
					#if defined DEBUG	
					cout << "retcode" << retCode.code<<endl;
					cout<< "1006 added"<<endl;
				#endif
				

						/*  1300*/
				strcpy(pbMNIndex, "1300");
                retCode = IfIndexExists(MN_NODEID, MN, pbMNIndex, &IndexPos);
                if(OCFM_ERR_SUCCESS == retCode.code)
                {
                  SetIndexAttributes(MN_NODEID, MN, pbMNIndex, (char*)"5000",(char*)"SDO_SequLayerTimeout_U32", TRUE);
                }

				/* 1C02*/
					strcpy(pbMNIndex, "1C02");
                    retCode = IfIndexExists(MN_NODEID, MN, pbMNIndex, &IndexPos);
                    if(OCFM_ERR_SUCCESS == retCode.code)
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
					
		/*  1C09*/
					strcpy(pbMNIndex, "1C09");
                    retCode = IfIndexExists(MN_NODEID, MN, pbMNIndex, &IndexPos);
                    if(OCFM_ERR_SUCCESS == retCode.code)
					{				
						pobjIndex = pobjIdxCol->getIndexbyIndexValue(pbMNIndex);
								/* $:set Flag to true*/
						pobjIndex->setFlagIfIncludedCdc(TRUE);
			
						AddForEachSIdx(pbMNIndex, pobjIdxCol, pobjNode->getNodeId(), (char*)"40",false);					
					
					}
					
							/*  1F26*/
					strcpy(pbMNIndex, "1F26");
                    retCode = IfIndexExists(MN_NODEID, MN, pbMNIndex, &IndexPos);
                    if(OCFM_ERR_SUCCESS == retCode.code)
					{				
						pobjIndex = pobjIdxCol->getIndexbyIndexValue(pbMNIndex);
								/* $:set Flag to true*/
						pobjIndex->setFlagIfIncludedCdc(TRUE);
			
						char* Val = new char[8];
						iConfigDate = getConfigDate();
						Val = _IntToAscii(iConfigDate, Val, 10);
						AddForEachSIdx(pbMNIndex, pobjIdxCol, pobjNode->getNodeId(), Val, false);		
											
	
					}
					
							#if defined DEBUG	
						cout<< "1F26 subidex added"<<endl;
					#endif
							/*  1F27*/
					strcpy(pbMNIndex, "1F27");
                    retCode = IfIndexExists(MN_NODEID, MN, pbMNIndex, &IndexPos);
                    if(OCFM_ERR_SUCCESS == retCode.code)
					{				
						pobjIndex = pobjIdxCol->getIndexbyIndexValue(pbMNIndex);		
								/* $:set Flag to true*/
						pobjIndex->setFlagIfIncludedCdc(TRUE);
			
						char* Val = new char[50];
						iConfigTime = getConfigTime();
						Val = _IntToAscii(iConfigTime,Val, 10);

						AddForEachSIdx(pbMNIndex, pobjIdxCol, pobjNode->getNodeId(), Val, false);										
					}
					
					
					/* 1F84*/
					strcpy(pbMNIndex, "1F84");
                    retCode = IfIndexExists(MN_NODEID, MN, pbMNIndex, &IndexPos);
                    if(OCFM_ERR_SUCCESS == retCode.code)
					{				
						pobjIndex = pobjIdxCol->getIndexbyIndexValue(pbMNIndex);
								/* $:set Flag to true*/
						pobjIndex->setFlagIfIncludedCdc(TRUE);
						
						AddForEachSIdx(pbMNIndex, pobjIdxCol, pobjNode->getNodeId(), (char*)"", true);			
			
					}

					delete [] pbMNIndex;
					delete [] pbSidx;
}

void UpdatedCNDateORTime(CIndex* pobjMNIndex, int iNodeId, EDateTime enumDT)
{

	ocfmRetCode stRetCode;
	CSubIndex *pobjSIdx;
	char Index[INDEX_LEN];
    int IndexPos;
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
		
			strcpy(Index, "1020");
				
            if(OCFM_ERR_SUCCESS != (IfIndexExists(pobjNode->getNodeId(), CN, Index, &IndexPos)).code)
            {
              return;
            }

				
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
/////////////////////////////////////////////////
cout<<"--------copyPDODefToAct----------------------"<<endl;

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
	CIndex objIndex; 
	objIndex = *pobjIndex;
	CSubIndex* pObjSIdx1, *pObjSIdx2;
			
	for(INT32 iIndexLoopCount=1; iIndexLoopCount <= objIndex.getNumberofSubIndexes() - 1; iIndexLoopCount++)
	{
		for(INT32 iIdxLoopCount=1; iIdxLoopCount <= objIndex.getNumberofSubIndexes() -1 -iIndexLoopCount ; iIdxLoopCount++)
		{
					
			pObjSIdx1 = objIndex.getSubIndex(iIdxLoopCount);
			if ( (pObjSIdx1->getActualValue()!=NULL) 
				&& (0 != strcmp(pObjSIdx1->getActualValue(),"")) 
				&& !(checkIfValueZero((char*)pObjSIdx1->getActualValue())) )
			{
				const char* pbActualVal1 = pObjSIdx1->getActualValue();

				INT32 iLength1 = strlen(pbActualVal1);
				char* pbOffset1 = NULL;
				INT32 iOffset1 = 0;
				
				pbOffset1 = subString((char*)pbActualVal1, iLength1-12,4);
				iOffset1 = hex2int(pbOffset1);
				pObjSIdx2 = objIndex.getSubIndex(iIdxLoopCount + 1);
				if ( (pObjSIdx2->getActualValue()!=NULL) 
					&& (0 != strcmp(pObjSIdx2->getActualValue(),"")) 
					&& !(checkIfValueZero((char*)pObjSIdx2->getActualValue())) )
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
			//continue with process
		}
		else
		{	
			// Node Doesn't Exist
			ocfmException objException;
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
////////////////////////////////////////
cout<<"--------------UpdateNodeParams------------"<<endl;
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
				ocfmException objException;
				objException.ocfm_Excpetion(OCFM_ERR_INVALID_NODEID);
				throw &objException;
			}

		}
		else if(iNewNodeID != iCurrNodeId)
		{
			stErrorInfo = IfNodeExists(iNewNodeID, eNodeType, &iNodePos, bFlag);
			if(stErrorInfo.code == OCFM_ERR_SUCCESS && bFlag == true)								
			{		
				ocfmException objException ;
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
		

		CNode* pobjNode;		
		CNodeCollection *pobjNodeCollection;
		
		pobjNodeCollection= CNodeCollection::getNodeColObjectPointer();
		pobjNode = pobjNodeCollection->getNodePtr(eNodeType, iCurrNodeId);

		if(eNodeType == CN)
		{
            if(iNewNodeID != iCurrNodeId)
            {
			    pobjNode->setNodeId(iNewNodeID);
			    CopyOldNodeIdAssignmentObject(pobjNode, iCurrNodeId);
            }
		}	

        if((eStationType != MULTIPLEXED) ||( (iNewNodeID != iCurrNodeId) && (eStationType == MULTIPLEXED)))
        {   
            char* cSIdx = new char[SUBINDEX_LEN];
            cSIdx = _IntToAscii(iCurrNodeId, cSIdx, 16);
            cSIdx = padLeft(cSIdx, '0', 2);
            pobjNode->resetForcedCycleValue();
        }

        if((eNodeType == CN) && !((PollResponseTimeout == NULL) || (strcmp(PollResponseTimeout,"") == 0)) )
        {
          pobjNode->setPollResponseTimeout(PollResponseTimeout);
        }
        pobjNode->setNodeName(NodeName);
        
		if(eNodeType == CN)
		{
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
                    }
				}
                if(bCalcForceCycle == true)
                {
                         ForcedCycle = getLastAvailableCycleNumber();
                }
				if(bSetForceCycle == true)
                  stErrorInfo = pobjNode->setForcedCycle(ForcedCycle);
				  
                  if( OCFM_ERR_SUCCESS != stErrorInfo.code )
                  {
                    return stErrorInfo;
                  }
                  pobjNode->setForceCycleFlag(ForcedCycleFlag);
			}
		}
				
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
			// Node Doesn't Exist
			ocfmException objException;
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
				strcpy(pbOutDataTypes, strdtName);				
			}
			else 
			{
				strcat(pbOutDataTypes, strdtName);
				
			}


			
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

/***********************************************************************************************************************************************
* Function Name: setPresMNNodeAssigmentBits
* Description:Returns the 1F81 object data depending upon the  if Multiplex set bit 8, if chained set bit 14
* Return value: void
***********************************************************************************************************************************************/
void setPresMNNodeAssigmentBits()
{
    int IndexPos;
    int subIndexPos;
    ocfmRetCode stErrStructInfo;
    stErrStructInfo = IfSubIndexExists(MN_NODEID, MN, (char *)"1F81", (char *)"F0", &subIndexPos, &IndexPos);
    
    if(OCFM_ERR_SUCCESS != stErrStructInfo.code)
    {
        return;
    }

	bool bIsPresMN = false;
	bool bIsMNBroadcastingPRes = false;
	INT32 iNodeTPDOMappedNodeID;
	
    bIsPresMN = IsPresMN();
    
	//check whether Pres activated in MN
	CNodeCollection* pobjNodeCol;
	CNode objMNNode;
	CIndexCollection* objPDOCollection;
	CIndexCollection* pobjIndexCollection;

    pobjNodeCol = CNodeCollection::getNodeColObjectPointer();
	objMNNode = pobjNodeCol->getMNNode();

	objPDOCollection = objMNNode.getPDOIndexCollection();
	if(objPDOCollection!= NULL)
	{
		pobjIndexCollection = objMNNode.getIndexCollection();
		CIndex* pobjIndex;
		for(INT32 iLoopCount = 0; iLoopCount<objPDOCollection->getNumberofIndexes(); iLoopCount++)
	    {
			pobjIndex = objPDOCollection->getIndex(iLoopCount);
			if(0 == strncmp(pobjIndex->getIndexValue(), "1A", 2))
			{
				CIndex *pobjCommIndex;
				CSubIndex *pobjNodeIDSubIndex;
				char *pcIdx = subString((char *)pobjIndex->getIndexValue(), 2, 4);
				char *pcCommIdx = new char[INDEX_SIZE];
				strcpy(pcCommIdx, (char *)"18");
				strcat(pcCommIdx, pcIdx);
				pobjCommIndex = pobjIndexCollection->getIndexbyIndexValue(pcCommIdx);
				if(NULL != pobjCommIndex)
				{
					pobjNodeIDSubIndex = pobjCommIndex->getSubIndexbyIndexValue((char *)"01");
					if(NULL != pobjNodeIDSubIndex)
					{
					  if((NULL != pobjNodeIDSubIndex->getActualValue()) && (0 != strcmp(pobjNodeIDSubIndex->getActualValue(), "")))
					  {
						iNodeTPDOMappedNodeID = GetDecimalValue( (char *) pobjNodeIDSubIndex -> getActualValue());
						if(BROADCAST_NODEID ==  iNodeTPDOMappedNodeID)
						{
							bIsMNBroadcastingPRes = true;
							break;
						}
					  }
					}
				  }
				  delete [] pcCommIdx;
				  delete [] pcIdx;
			}
		}
	}


	//check whether MN is transmitting PRes
	if( (false == bIsPresMN) \
		&& (false == bIsMNBroadcastingPRes) \
	  )
	{
		return;
	}


    char *pcSubIndex = new char[SUBINDEX_LEN];
    strcpy(pcSubIndex, (char*)"F0");
    
    CSubIndex* pobjSubindex;
    pobjSubindex = getMNSubIndexValues((char*)"1F81", (char*)"F0");
    if(NULL != pobjSubindex)
    {
        if( (true == bIsPresMN) \
			|| (true == bIsMNBroadcastingPRes) \
			)
        {
            char* pb1F81Data = NULL;
            unsigned long ulValue;
            pb1F81Data = new char[8 + STR_ALLOC_BUFFER];
            ulValue =  EPL_NODEASSIGN_VALID | EPL_NODEASSIGN_NODE_EXISTS | EPL_NODEASSIGN_MN_PRES;
            strcpy(pb1F81Data, (char *)"0x");
            _IntToAscii(ulValue, &pb1F81Data[2], 16);
            
            pobjSubindex->setActualValue(pb1F81Data);
            pobjSubindex->setFlagIfIncludedCdc(TRUE);
            delete[] pb1F81Data;
        }
        else
        {
            // no operation
        }
    }
    
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
            stErrorInfo.code = OCFM_ERR_SUCCESS;
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
            stErrorInfo.code = OCFM_ERR_SUCCESS;
            return stErrorInfo;
        }
		stErrorInfo = GetSubIndexAttributes(MN_NODEID, MN, (char*)"1F98", (char*)"07", ACTUALVALUE, actValue); 
		if(stErrorInfo.code == OCFM_ERR_SUCCESS)		
		{
		}
		else
		{
			ocfmException objException;
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

						}
					}

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
                objNode->resetForcedCycleValue();
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
    copySubIndexDefToAct(iNodeID, enumNodeType, false, (char *)"1F8A", (char *)"02" );
    copySubIndexDefToAct(iNodeID, enumNodeType, false, (char *)"1F98", (char *)"07" );
    copySubIndexDefToAct(iNodeID, enumNodeType, false, (char *)"1F98", (char *)"08" );
}

/**************************************************************************************************
* Function Name: CNode::copyIndexDefToAct
* Description: 
* Return value: void
****************************************************************************************************/
void copyIndexDefToAct(int iNodeID, ENodeType enumNodeType, char *indexId )
{
////////////////////////////////////////////
cout<<"--------------copyIndexDefToAct---------"<<indexId<<endl;
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
void copySubIndexDefToAct(int iNodeID, ENodeType enumNodeType, bool bForce, char *indexId, char *subIndexId )
{
///////////////////////////////////////////
cout<<"-------------------copySubIndexDefToAct------------"<<endl;
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
    if((pobjSIndex->getActualValue() == NULL) || (true == bForce))
    {
        if(pobjSIndex->getDefaultValue() != NULL)
        {
            pobjSIndex->setActualValue((char*)pobjSIndex->getDefaultValue());
        }
        else
        {
            if(true == bForce)
            {
              pobjSIndex->setActualValue((char*)"");
            }
        }
    }
}

/****************************************************************************************************
* Function Name: CheckAndReAssignMultiplex
* Description: -
* Return value: ocfmRetCode
****************************************************************************************************/

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
                                sprintf(stErrorInfo.errorString,"%s%d, ", stErrorInfo.errorString, objNode->getNodeId());
                                bErrorFlag = true;
                            }
                    }
                }
                else 
                {
                }
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
 * Function Name: getFreeCycleNumber
 * Description: -
 * Return value: UINT32
 ****************************************************************************************************/
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
 * Function Name: IsMultiplexCycleNumberContinuous
 * Description: -
 * Return value: bool
 ****************************************************************************************************/
bool IsMultiplexCycleNumberContinuous(UINT32 uiCycleNumber)
{
//////////////////////////////////////////////////
cout<<"--------------------IsMultiplexCycleNumberContinuous--------------"<<endl;
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
///////////////////////////////////////////////////////
cout<<"--------------------calculateCNPollResponse-------------"<<endl;
    if(enumNodeType != CN)
    {
cout<<"--------------------calculateCNPollResponse0-------------"<<endl;
        return;
    }
    int SubIndexPos;
    int IndexPos;
    ocfmRetCode stErrorInfo;
    stErrorInfo = IfSubIndexExists(iNodeID, enumNodeType, (char*)"1F98", (char*)"03", &SubIndexPos, &IndexPos);
    if(stErrorInfo.code != OCFM_ERR_SUCCESS)
    {
cout<<"--------------------calculateCNPollResponse1-------------"<<endl;
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
cout<<"--------------------calculateCNPollResponse2-------------"<<endl;
        return;
    }


    pobjSIndex = pobjIndex->getSubIndexbyIndexValue((char*)"03");
    if(pobjSIndex == NULL)
    {
cout<<"--------------------calculateCNPollResponse3-------------"<<endl;
        return;
    }
    char *pcValue;
    INT32 iValue = 0;
    bool add25microsec = false;
    if(pobjSIndex->getActualValue() == NULL || strcmp(pobjSIndex->getActualValue(),"") == 0)
    {
cout<<"Actual Value 0"<<pobjSIndex->getActualValue()<< endl;
        if(pobjSIndex->getDefaultValue() == NULL || strcmp(pobjSIndex->getDefaultValue(),"") == 0)
        {
cout<<"Deafult Value 0"<<pobjSIndex->getDefaultValue()<< endl;
			/* PATCH For BUG #7 - START */
            pcValue = new char[strlen("25000") + ALLOC_BUFFER];
			/* PATCH For BUG #7 - END */
            strcpy(pcValue, "25000");
        }
        else
        {
            pcValue = new char[strlen(pobjSIndex->getDefaultValue()) + ALLOC_BUFFER];
cout<<"pcValue: "<<pcValue<<endl;
            strcpy(pcValue, pobjSIndex->getDefaultValue());
            add25microsec = true;
        }
    }
    else
    {
cout<<"Actual Value "<<pobjSIndex->getActualValue()<< endl;
        pcValue = new char[strlen(pobjSIndex->getActualValue()) + ALLOC_BUFFER];
        strcpy(pcValue, pobjSIndex->getActualValue());
    }

    if (strncmp(pcValue,"0x",2) == 0 || strncmp(pcValue,"0X",2) == 0)
    {
cout<<"pcValue: "<<pcValue<<endl;
        iValue  = hex2int(subString(pcValue, 2, strlen(pcValue) -2));
    }
    else
    {
cout<<"pcValue: "<<pcValue<<endl;
        iValue  = atoi(pcValue);
cout<<"iValue "<<iValue<<endl;
    }

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
    if(0 == strlen(pcCustomErrString))
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
///////////////////////////////////////////
cout<<"-------------RecalculateCNPresTimeout------------"<<endl;
    stErrorInfo = IfSubIndexExists(MN_NODEID, MN, (char*)"1F92", pbSubIndexId, &iSubIndexPos, &iIndexPos);
    if(stErrorInfo.code == OCFM_ERR_SUCCESS)
    {
            //continue with process
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

    stErrorInfo = IfSubIndexExists(iCNNodeId, CN, (char*)"1F98", (char*)"03",  &iSubIndexPos, &iIndexPos);
    if(OCFM_ERR_SUCCESS != stErrorInfo.code)
    {
        return;
    }
    
                                     
    char* strConvertedValue;
    strConvertedValue = new char[SUBINDEX_LEN];
    strcpy(strConvertedValue, pbSubIndexId);

    char* subIndName = new char[50];
    subIndName[0] = 0;
    INT32 iCNsubIndFlag = 0;
    
    GetSubIndexAttributes(iCNNodeId, CN, (char*)"1F98", (char*)"03", NAME, subIndName);
    char* subIndFlag = new char[10];
    GetSubIndexAttributes(iCNNodeId, CN, (char*)"1F98", (char*)"03", FLAGIFINCDC, subIndFlag);
    iCNsubIndFlag  = atoi(subIndFlag);

    SetSubIndexAttributes(iCNNodeId, CN, (char*)"1F98", (char*)"03", pcMNIndValue, subIndName, (EFlag)iCNsubIndFlag);
    delete[] subIndName;
    delete[] subIndFlag;
    delete[] pcMNIndValue;

}

/****************************************************************************************************
* Function Name: UpdateMNNodeAssignmentIndex
* Description:
* Return value: void
****************************************************************************************************/
void UpdateMNNodeAssignmentIndex(CNode *pobjNode, INT32 CNsCount, char* pcIndex, bool allowMNSubindex)
{ 	
	if(NULL == pcIndex)
		return;
	CIndexCollection *pobjIdxCol = NULL;
	ocfmRetCode retCode;
	CIndex *pobjIndex;
	char* pbIndexNo = new char[3];
    char* pbHexIndexNo = new char[5];
//////////////////////////////////////////////
cout<<"------UpdateMNNodeAssignmentIndex----------"<<endl;				
	pobjIdxCol = pobjNode->getIndexCollection();
	char* pbMNIndex = new char[INDEX_LEN + ALLOC_BUFFER];
	char* pbSidx =  new char[SUBINDEX_LEN + ALLOC_BUFFER];
	strcpy(pbMNIndex, pcIndex);
    int IndexPos;
    retCode = IfIndexExists(MN_NODEID, MN, pbMNIndex, &IndexPos);
    if(retCode.code == OCFM_ERR_SUCCESS)
	{				
		pobjIndex = pobjIdxCol->getIndexbyIndexValue(pcIndex);
				/* $:set Flag to true*/
		pobjIndex->setFlagIfIncludedCdc(TRUE);	
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
/////////////////////////////////////
//cout<<pobjSubIndex->getActualValue()<<endl;
                if(OCFM_ERR_SUCCESS == retCode.code && true == bFlag && ((CN == iNodeType) || (true == allowMNSubindex)) )
                {
                }
                else
                {
                    if(NULL == pobjSubIndex -> getDefaultValue())
                        pobjSubIndex->setActualValue((char *)"");
                    else
                        pobjSubIndex->setActualValue((char *)pobjSubIndex -> getDefaultValue());
                }
            }
            catch(ocfmException* ex)
            {
                if(NULL == pobjSubIndex -> getDefaultValue())
                    pobjSubIndex->setActualValue((char *)"");
                else
                    pobjSubIndex->setActualValue((char *)pobjSubIndex -> getDefaultValue());
            }
			
		}
	}	
}

/****************************************************************************************************
* Function Name: ValidateCNPresTimeout
* Description:	/////////////////////////////////////////////////////////////////////////////////////////////////////validates the user input for the cdc generation; but only considers the default latency value.
* Return value: bool
****************************************************************************************************/
bool ValidateCNPresTimeout(char* pbSubIndexId, char* pcCheckValue)
{
    ocfmRetCode stErrorInfo, stRet;
    int iSubIndexPos, iIndexPos;
////////////////////////////////////////////
cout<<"---------ValidateCNPresTimeout---------"<<endl;
    stErrorInfo = IfSubIndexExists(MN_NODEID, MN, (char*)"1F92", pbSubIndexId, &iSubIndexPos, &iIndexPos);
    if(stErrorInfo.code == OCFM_ERR_SUCCESS)
    {
            //continue with process
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

/****************************************************************************************************
* Function Name: CopyOldNodeIdAssignmentObject
* Description:
* Return value: void
****************************************************************************************************/
void CopyOldNodeIdAssignmentObject(CNode* pobjNode, INT32 iOldNodeId)
{
     CopyOldNodeIdAssignmentObjectSubindex(pobjNode, iOldNodeId, (char*)"1F81");
     CopyOldNodeIdAssignmentObjectSubindex(pobjNode, iOldNodeId, (char*)"1F92");
     CopyOldNodeIdAssignmentObjectSubindex(pobjNode, iOldNodeId, (char*)"1F9B");

}

/****************************************************************************************************
* Function Name: CopyOldNodeIdAssignmentObjectSubindex
* Description:
* Return value: void
****************************************************************************************************/
void CopyOldNodeIdAssignmentObjectSubindex(CNode* pobjNode, INT32 iOldNodeId, char* pcIndex)
{
/////////////////////////////////////////////
cout<<"--------------CopyOldNodeIdAssignmentObjectSubindex-------------"<<endl;
    if(NULL == pcIndex)
    {
        return;
    }
    INT32 iCNNodeId = pobjNode->getNodeId();
    ocfmRetCode stErrorInfo, stRet;
    int iSubIndexPos, iIndexPos;
    char* pcCNNodeId = new char[10];
    pcCNNodeId = _IntToAscii(iCNNodeId, pcCNNodeId, 16);
    pcCNNodeId = padLeft(pcCNNodeId, '0', 2);
    char* pcCNOldNodeId = new char[10];
    pcCNOldNodeId = _IntToAscii(iOldNodeId, pcCNOldNodeId, 16);
    pcCNOldNodeId = padLeft(pcCNOldNodeId, '0', 2);
    char* TempOldActualValue = NULL;
    EFlag TempOldFlg;

    CIndexCollection* pobjMNIdxCol = NULL;
    pobjMNIdxCol = CNodeCollection::getNodeColObjectPointer()->getNodePtr(MN, MN_NODEID)->getIndexCollection();
    CIndex *pobjMNIndex;
    CSubIndex* pobjMNOldCNNodeIdSubIndex;
    CSubIndex* pobjMNCNNodeIdSubIndex;

    stErrorInfo = IfSubIndexExists(MN_NODEID, MN, pcIndex, pcCNOldNodeId, &iSubIndexPos, &iIndexPos);
    if(OCFM_ERR_SUCCESS == stErrorInfo.code)
    {
            pobjMNIndex = pobjMNIdxCol->getIndexbyIndexValue(pcIndex);
            pobjMNOldCNNodeIdSubIndex = pobjMNIndex->getSubIndexbyIndexValue(pcCNOldNodeId);
            if (NULL != pobjMNOldCNNodeIdSubIndex)
            {   
                if(NULL != (char*)pobjMNOldCNNodeIdSubIndex->getActualValue())
                {
                    TempOldActualValue = new char[strlen((char*)pobjMNOldCNNodeIdSubIndex->getActualValue()) + STR_ALLOC_BUFFER];
					strcpy((char*)TempOldActualValue, (char*)pobjMNOldCNNodeIdSubIndex->getActualValue());
                    TempOldFlg = pobjMNOldCNNodeIdSubIndex->getFlagIfIncludedCdc();
                }
                stErrorInfo = IfSubIndexExists(MN_NODEID, MN, pcIndex, pcCNNodeId, &iSubIndexPos, &iIndexPos);
                if(OCFM_ERR_SUCCESS == stErrorInfo.code)
                {
                    //set the value alone
                }
                else
                {
                    return;
                }

                pobjMNCNNodeIdSubIndex = pobjMNIndex->getSubIndexbyIndexValue(pcCNNodeId);
                if (NULL != pobjMNCNNodeIdSubIndex)
                {
                pobjMNCNNodeIdSubIndex->setActualValue((char*)"");
                }

                 if ((NULL != pobjMNCNNodeIdSubIndex) && (NULL != TempOldActualValue ))
                 {
                    //copy the actual value to here
                    pobjMNCNNodeIdSubIndex->setActualValue(TempOldActualValue);
                    pobjMNCNNodeIdSubIndex->setFlagIfIncludedCdc(TempOldFlg);
                 }
                 // reset the actual value of the old node id subindex
				 SetSubIndexAttributesByAttribute(MN_NODEID, MN, pcIndex, pcCNOldNodeId, ACTUALVALUE, (char*)"");
            }
    }
    else
    {
        //no operation
    }
}

CSubIndex* DuplicateSubIndexObject(CSubIndex* pobjSubindex)
{
    if(NULL == pobjSubindex)
        return NULL;

    CSubIndex* pobjDupliSubindex = new CSubIndex();

    if(NULL != pobjSubindex->getIndexValue())
        pobjDupliSubindex->setIndexValue((char*)pobjSubindex->getIndexValue());

    pobjDupliSubindex->setName((char*)pobjSubindex->getName());

    if(NULL != pobjSubindex->getHighLimit())
        pobjDupliSubindex->setHighLimit((char*)pobjSubindex->getHighLimit());

     if(NULL != pobjSubindex->getDataTypeValue())
        pobjDupliSubindex->setDataTypeValue(pobjSubindex->getDataTypeValue());

    if(NULL != pobjSubindex->getDefaultValue())
        pobjDupliSubindex->setDefaultValue((char*)pobjSubindex->getDefaultValue());

    if(NULL != pobjSubindex->getActualValue())
        pobjDupliSubindex->setActualValue((char*)pobjSubindex->getActualValue());

    if(NULL != pobjSubindex->getAccessType())
        pobjDupliSubindex->setAccessType((char*)pobjSubindex->getAccessType());

    if(NULL != pobjSubindex->getLowLimit())
        pobjDupliSubindex->setLowLimit((char*)pobjSubindex->getLowLimit());

    if(NULL != pobjSubindex->getUniqueIDRef())
        pobjDupliSubindex->setUniqueIDRef((char*)pobjSubindex->getUniqueIDRef());

    pobjDupliSubindex->setDataTypeST(pobjSubindex->getDataType());

    pobjDupliSubindex->setFlagIfIncludedCdc(pobjSubindex->getFlagIfIncludedCdc());
        
    if(NULL != pobjSubindex->getObjectType())
        pobjDupliSubindex->setObjectType((char*)pobjSubindex->getObjectType());

    if(NULL != pobjSubindex->getPDOMapping())
        pobjDupliSubindex->setPDOMapping((char*)pobjSubindex->getPDOMapping());

    pobjDupliSubindex->setNodeID(pobjSubindex->getNodeID());

    return pobjDupliSubindex;

}

/****************************************************************************************************
* Function Name: IsDefaultActualNotEqual
* Description:
* Return value: bool
****************************************************************************************************/
bool IsDefaultActualNotEqual(CBaseIndex* pBaseIndexObject)
{
    if(NULL == pBaseIndexObject || NULL == pBaseIndexObject->getActualValue() )
        return false;

    if( NULL == pBaseIndexObject->getDefaultValue())
        return true;

    DataType dt = pBaseIndexObject->getDataType();
    if(dt.Name != NULL)
    {
        if(!checkIfStringDatatypes(dt.Name)) 
        {
            INT32 iActualValue, iDefaultValue;
            iDefaultValue = GetDecimalValue((char*)pBaseIndexObject->getDefaultValue());
            iActualValue = GetDecimalValue((char*)pBaseIndexObject->getActualValue());
            if(iActualValue == iDefaultValue)
                return false;
            else
                return true;
        }
        else
        {
            if( 0 == strcmp( pBaseIndexObject->getDefaultValue(), pBaseIndexObject->getActualValue()) )
                return false;
            else
                return true;
        }
    }
    else
    {
        if( 0 == strcmp( pBaseIndexObject->getDefaultValue(), pBaseIndexObject->getActualValue()) )
            return false;
        else
            return true;
    }
}

/****************************************************************************************************
* Function Name: ReactivateMappingPDO
* Description:
* Return value: bool
****************************************************************************************************/
bool ReactivateMappingPDO(CIndexCollection* pobjIndexCol, CIndex* pobjIndex)
{
    if(false == CheckIfMappingPDO((char*)pobjIndex->getIndexValue()))
        return false;

    CSubIndex* pobjSubIndex;
    pobjSubIndex = pobjIndex->getSubIndexbyIndexValue((char*)"00");
    if((NULL != pobjSubIndex) && (NULL != pobjSubIndex->getActualValue()) && ( 0 != strcmp(pobjSubIndex->getActualValue(),"")) )
    {
        if(false == checkIfValueZero((char*)pobjSubIndex->getActualValue()))
        {
            for(INT32 iLoopCount = 0; iLoopCount < pobjIndex->getNumberofSubIndexes(); iLoopCount++)
            {
                if(pobjIndex->getSubIndex(iLoopCount)->getActualValue() !=NULL && TRUE == pobjIndex->getSubIndex(iLoopCount)->getFlagIfIncludedCdc() && true == IsDefaultActualNotEqual(pobjIndex->getSubIndex(iLoopCount)) )
                {
                    if(0 == strcmp(pobjIndex->getSubIndex(iLoopCount)->getIndexValue(),"00"))
                    {
                            continue;
                    }
        
                    return true;
                }
            }
        }
        else
        {
            if(false == IsDefaultActualNotEqual(pobjSubIndex))
                return false;
                
        }
    }


    char aCommParam[5];
    strcpy(aCommParam, (char*)pobjIndex->getIndexValue());
    
    if(strncmp((char*)pobjIndex->getIndexValue(), "1A", 2) == 0)
    {
        aCommParam[1] = '8';
    }
    else if(strncmp((char*)pobjIndex->getIndexValue(), "16", 2) == 0)
    {
        aCommParam[1] = '4';
    }
    else    
    {
        return false;
    }
    
    CIndex* pObjCommParam;
    pObjCommParam = pobjIndexCol->getIndexbyIndexValue(aCommParam);
    if(NULL == pObjCommParam)
    {
        return false;
    }

    pobjSubIndex = pObjCommParam->getSubIndexbyIndexValue((char*)"00");
    if((NULL != pobjSubIndex) && (NULL != pobjSubIndex->getActualValue()) && ( 0 != strcmp(pobjSubIndex->getActualValue(),"")) )
    {
        if(checkIfValueZero((char*)pobjSubIndex->getActualValue()))
            return false;
    }

    for(INT32 iLoopCount = 0; iLoopCount < pObjCommParam->getNumberofSubIndexes(); iLoopCount++)
    {
        if(pObjCommParam->getSubIndex(iLoopCount)->getActualValue() !=NULL && TRUE == pObjCommParam->getSubIndex(iLoopCount)->getFlagIfIncludedCdc() && true == CheckAccessTypeForInclude((char*)pObjCommParam->getSubIndex(iLoopCount)->getAccessType()) && true == IsDefaultActualNotEqual(pObjCommParam->getSubIndex(iLoopCount)) )
        {
                return true;
        }
    }

    return false;
}

/****************************************************************************************************
* Function Name: SortNodeIDbyStation
* Description:
* Return value: INT32*
****************************************************************************************************/
INT32* ArrangeNodeIDbyStation()
{
    CNodeCollection* objNodeCol;
    objNodeCol = CNodeCollection::getNodeColObjectPointer();
    CNode* pobjNode;
        
    /* Check RPDO Mapped objects*/
    INT32 iCNNodesCount = 0;
    ocfmRetCode stRetInfo;
    
    iCNNodesCount = objNodeCol->getCNNodesCount();
    
    if(iCNNodesCount == 0)
    {               
        exit;
    }

    INT32 *piNodeIDColl = new INT32[iCNNodesCount];
    INT32 iArrangedNodeIDCount = 0;
    INT32 *piArrangedNodeIDColl = new INT32[iCNNodesCount];
    EStationType *piStationTypeColl = new EStationType[iCNNodesCount];
    INT32 iNodesCount = objNodeCol->getNumberOfNodes();
    INT32 iLoopCount, iCNNodeLoopCnt=0;
    for(iLoopCount = 0; iLoopCount < iNodesCount; iLoopCount++)
    {
        pobjNode = objNodeCol->getNodebyColIndex(iLoopCount);
        if (MN == pobjNode->getNodeType())
        {
            continue;
        }
        piNodeIDColl[iCNNodeLoopCnt] = pobjNode->getNodeId();
        piStationTypeColl[iCNNodeLoopCnt] = pobjNode->getStationType();
        iCNNodeLoopCnt++;
    }
    //get the number of poll response station and other station
    INT32 iChainStnCnt = 0,iOtherStnCnt =0;
    for(iLoopCount = 0; iLoopCount < iCNNodesCount ; iLoopCount++)
    {
        if(CHAINED == piStationTypeColl[iLoopCount])
            iChainStnCnt++;
    }
    iOtherStnCnt = iCNNodesCount - iChainStnCnt;

    //create arrays to store nodeid for chained station and other station
    if(0 != iChainStnCnt)
    {
        INT32 *piChainStnColl = new INT32[iChainStnCnt];
        INT32 iChainStnLoopCnt = 0;
        for(iLoopCount = 0; iLoopCount < iCNNodesCount ; iLoopCount++)
        {
            if(CHAINED == piStationTypeColl[iLoopCount])
            {
                piChainStnColl[iChainStnLoopCnt] = piNodeIDColl[iLoopCount];
                iChainStnLoopCnt++;
            }
        }
        //sort by station no
        SortNodeID(piChainStnColl, iChainStnCnt);

        //copy the poll response staion sorted in asscending order
        for(iLoopCount = 0; iLoopCount < iChainStnCnt ; iLoopCount++)
        {
            piArrangedNodeIDColl[iArrangedNodeIDCount] = piChainStnColl[iLoopCount];
            iArrangedNodeIDCount++;
        }

        delete[] piChainStnColl;
    }

    if(0 != iOtherStnCnt)
    {
        INT32 *piOtherStnColl = new INT32[iOtherStnCnt];
        INT32 iOtherStnLoopCnt = 0;
        for(iLoopCount = 0; iLoopCount < iCNNodesCount ; iLoopCount++)
        {
            if(CHAINED != piStationTypeColl[iLoopCount])
            {
                piOtherStnColl[iOtherStnLoopCnt] = piNodeIDColl[iLoopCount];
                iOtherStnLoopCnt++;
            }
        }
        //copy the other station after the poll response staion
        for(iLoopCount = 0; iLoopCount < iOtherStnCnt ; iLoopCount++)
        {
            piArrangedNodeIDColl[iArrangedNodeIDCount] = piOtherStnColl[iLoopCount];
            iArrangedNodeIDCount++;
        }
        delete[] piOtherStnColl;
    }
    delete[] piNodeIDColl;
    delete[] piStationTypeColl;

    return piArrangedNodeIDColl;
}

/****************************************************************************************************
* Function Name: SortNodeID
* Description:
* Return value: void
****************************************************************************************************/
void SortNodeID(INT32 *piNodeIDColl, INT32 iColSize)
{
    INT32 iTemp =0;
    for(INT32 iLoopCount = 0; iLoopCount < iColSize; iLoopCount++)
    {
        for(INT32 iSortCount=iLoopCount+1; iSortCount<=iLoopCount; iSortCount++)
        {
            if(piNodeIDColl[iLoopCount] > piNodeIDColl[iSortCount])
            {
                iTemp = piNodeIDColl[iLoopCount];
                piNodeIDColl[iLoopCount] = piNodeIDColl[iSortCount];
                piNodeIDColl[iSortCount] = iTemp;
            }
        }
    }
}

/****************************************************************************************************
* Function Name: IsPresMN
* Description:
* Return value: bool
****************************************************************************************************/
bool IsPresMN()
{
    CNodeCollection* objNodeCol;
    objNodeCol = CNodeCollection::getNodeColObjectPointer();
    CNode* pobjNode;
    INT32 iLoopCount;
    INT32 iNodesCount = objNodeCol->getNumberOfNodes();
    for(iLoopCount = 0; iLoopCount < iNodesCount; iLoopCount++)
    {
        pobjNode = objNodeCol->getNodebyColIndex(iLoopCount);
        if (MN == pobjNode->getNodeType())
        {
            continue;
        }
        if(CHAINED ==  pobjNode->getStationType())
        {
            return true;
        }
    }
    return false;
}

/****************************************************************************************************
* Function Name: SetCNLossObjects
* Description: sets value to 1COB, 1C0C and 1C0D objects
* Return value: 
****************************************************************************************************/
void SetCNLossObjects(int iNodeID, ENodeType enumNodeType)
{
    if(enumNodeType != CN)
    {
        return;
    }

    CNode *pobjNode;
    CNodeCollection *objNodeCollection = NULL;
    CIndexCollection* pobjIdxCol;
    CIndex* pobjIndex;
    CSubIndex* pobjSIndex;
    
    objNodeCollection = CNodeCollection::getNodeColObjectPointer();
    pobjNode = objNodeCollection->getNodePtr(enumNodeType, iNodeID);
    pobjIdxCol = pobjNode->getIndexCollection();
        
    
    //loss of SoC
    pobjIndex = pobjIdxCol->getIndexbyIndexValue((char*)"1C0B");
    if(NULL != pobjIndex)
    {
        pobjSIndex = pobjIndex->getSubIndexbyIndexValue((char*)"03");
        if(NULL != pobjSIndex)
        {
            if(pobjSIndex->getActualValue() == NULL || strcmp(pobjSIndex->getActualValue(),"") == 0)
            { 
              pobjSIndex->setActualValue((char*)"0x50");
              pobjSIndex->setFlagIfIncludedCdc(TRUE);
              pobjIndex->setFlagIfIncludedCdc(TRUE);
            }
        }
    }
    
    //loss of SoA
    pobjIndex = pobjIdxCol->getIndexbyIndexValue((char*)"1C0C");
    if(NULL != pobjIndex)
    {
      pobjSIndex = pobjIndex->getSubIndexbyIndexValue((char*)"03");
      if(NULL != pobjSIndex)
      {
        if(pobjSIndex->getActualValue() == NULL || strcmp(pobjSIndex->getActualValue(),"") == 0)
        { 
          pobjSIndex->setActualValue((char*)"0x50");
          pobjSIndex->setFlagIfIncludedCdc(TRUE);
          pobjIndex->setFlagIfIncludedCdc(TRUE);
        }
      }
    }
    
    //loss of PReq
    pobjIndex = pobjIdxCol->getIndexbyIndexValue((char*)"1C0D");
    if(NULL != pobjIndex)
    {
      pobjSIndex = pobjIndex->getSubIndexbyIndexValue((char*)"03");
      if(NULL != pobjSIndex)
      {
        if(pobjSIndex->getActualValue() == NULL || strcmp(pobjSIndex->getActualValue(),"") == 0)
        { 
          pobjSIndex->setActualValue((char*)"0x50");
          pobjSIndex->setFlagIfIncludedCdc(TRUE);
          pobjIndex->setFlagIfIncludedCdc(TRUE);
        }
      }
    }
}

/****************************************************************************************************
* Function Name: SetBuildTime
* Description: Gets the system time during time of build - to be called from ProcessPDOnodes
* Return value: void
****************************************************************************************************/
void SetBuildTime()
{
	time ( &sBuildTime_g.rawtime );
	sBuildTime_g.timeinfo = localtime ( &sBuildTime_g.rawtime );
    strftime (sBuildTime_g.buffer, BUILDTIME_BUF_LEN, "%d-%b-%Y %H:%M:%S", sBuildTime_g.timeinfo);
}

/****************************************************************************************************
* Function Name: GetBuildTime
* Description: Gets the calculated system time during time of build
*              - to be called from API generating interface files
* Return value: const char*
****************************************************************************************************/
const char* GetBuildTime()
{
	return (const char*)&sBuildTime_g.buffer;
}
