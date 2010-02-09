///////////////////////////////////////////////////////////////////////////////////////////////
//
//  $Source: $
//
// NAME:  Node.cpp
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
#include <stdio.h>
#include "../Include/Node.h"
#include "../Include/Internal.h"

/****************************************************************************************************
* FUNCTION DEFINITIONS
****************************************************************************************************/

/****************************************************************************************************
* Constructor
****************************************************************************************************/
CNode::CNode(void)
{
	m_NodeName 				= NULL;		
	m_IndexCollection 		= NULL;
	m_DataTypeCollection 	=  NULL;	 
	m_IndexCollection 		= NULL;
	m_ApplicationProcess	= NULL;
	m_NetworkManagement		= NULL;
	m_HasPdoObjects 		= false;
	m_StationType			= NORMAL;
	m_ForcedCycle			= NULL;
	m_PollResponseTimeout		= NULL;
	m_ForcedCycleFlag		= false;
}

/****************************************************************************************************
* Destructor
****************************************************************************************************/
CNode::~CNode(void)
{
	//Add destrucor code here
}

#pragma region Properties

/**************************************************************************************************
* Function Name: CNode::getNodeType
* Description: Returns the NodeType of the Node
* Return value: ENodeType
****************************************************************************************************/
ENodeType CNode:: getNodeType()
{
	return m_NodeType;
}

/**************************************************************************************************
* Function Name: CNode::setNodeType
* Description: sets the Node Type of the Node.Value can be either CN or MN
* Return value: void
****************************************************************************************************/
void CNode::setNodeType(ENodeType objNodeType)
{
	m_NodeType = objNodeType; 
}
	
/**************************************************************************************************
* Function Name: CNode::getNodeId
* Description: Returns the Index of the Index Object
* Return value: INT32
****************************************************************************************************/
INT32 CNode::getNodeId()
{
	return m_NodeId;
}

/**************************************************************************************************
* Function Name: CNode::setIndex
* Description: sets the Index of the Index Object
* Return value: void
****************************************************************************************************/
void CNode::setNodeId(int iNodeId)
{
	m_NodeId = iNodeId;
		
}

/**************************************************************************************************
* Function Name: CNode::getNodeIndex
* Description: Returns the Index of the Index Object
* Return value:
****************************************************************************************************/
INT32 CNode::getNodeIndex()
{
	return m_NodeIndex;
}

/**************************************************************************************************
* Function Name: CNode::setNodeIndex
* Description: sets the Index of the Index Object
* Return value:
****************************************************************************************************/
void CNode::setNodeIndex(int iNodeIndex)
{
	m_NodeIndex = iNodeIndex; 
}

/**************************************************************************************************
* Function Name: CNode::getNodeName
* Description: Returns the Nama of the Node
* Return value:
****************************************************************************************************/
char* CNode::getNodeName()
{
	return m_NodeName;
}

/**************************************************************************************************
* Function Name: CNode::setNodeName
* Description: sets the Name of the Node
* Return value: void
****************************************************************************************************/
void CNode::setNodeName(char* pbNodeName)
{	
	m_NodeName = new char[strlen(pbNodeName) + ALLOC_BUFFER];
	strcpy((char*)m_NodeName, pbNodeName);
}

/**************************************************************************************************
* Function Name: CNode::HasPdoObjects
* Description: Returns true if PDO Objects are present else false
* Return value: bool
****************************************************************************************************/
bool CNode::HasPdoObjects()
{
	return m_HasPdoObjects;
}

/**************************************************************************************************
* Function Name: CNode::setFlagForPdoObjects
* Description: sets the Name of the Node
* Return value: void
****************************************************************************************************/
void CNode::setFlagForPdoObjects(bool bFlag)
{	
	m_HasPdoObjects = bFlag;
}
	
/**************************************************************************************************
* Function Name: getIndexCollection
* Description: Returns the IndexCollection of the Node
* Return value: CIndexCollection*
****************************************************************************************************/
CIndexCollection* CNode::getIndexCollection()
{
	return m_IndexCollection;
}
	
/**************************************************************************************************
* Function Name: CNode::getApplicationProcess
* Description: Returns the Application Process of the Node
* Return value: CApplicationProcess*
****************************************************************************************************/
CApplicationProcess* CNode::getApplicationProcess()
{
	return m_ApplicationProcess;
}
/**************************************************************************************************
* Function Name: CNode::getNetworkManagement
* Description: Returns the NetworkManagement of the Node
* Return value: CNetworkManagement*
****************************************************************************************************/
CNetworkManagement* CNode::getNetworkManagement()
{	
	return m_NetworkManagement;
}

/**************************************************************************************************
* Function Name: CNode::getDataTypeCollection
* Description: returns the DataType Collection
* Return value: CDataTypeCollection*
****************************************************************************************************/
CDataTypeCollection* CNode::getDataTypeCollection()
{
	return m_DataTypeCollection;
}

/**************************************************************************************************
* Function Name: CNode::CreateIndexCollection
* Description: 
* Return value: void
****************************************************************************************************/
void CNode::CreateIndexCollection() 
{
	CIndexCollection* pobjIndexCollection;
	
	pobjIndexCollection = new CIndexCollection();
	m_IndexCollection 	= pobjIndexCollection ;
}
	
/**************************************************************************************************
* Function Name: CNode::CreateDataTypeCollection
* Description: 
* Return value: void
****************************************************************************************************/
void CNode::CreateDataTypeCollection() 
{
	CDataTypeCollection* pobjDataTypeCollection;
	pobjDataTypeCollection = new CDataTypeCollection();
	m_DataTypeCollection 	= pobjDataTypeCollection ;
}

/**************************************************************************************************
* Function Name: CNode::addProcessImage
* Description: 
* Return value: void
****************************************************************************************************/
void CNode::addProcessImage(ProcessImage objProcessImage)
{
	INT32 iItemPosition = ProcessImageCollection.Add();
	//*(ProcessImageCollection + i) = processImage;	
	ProcessImageCollection[iItemPosition] 	  = objProcessImage;
}

/**************************************************************************************************
* Function Name: CNode::addNETProcessImage
* Description: 
* Return value: void
****************************************************************************************************/
void CNode::addNETProcessImage(NETProcessImage objNETProcessImage )
{
	INT32 iItemPosition = NETProcessImageCollection.Add();
	NETProcessImageCollection[iItemPosition] 	  = objNETProcessImage;
}
/**************************************************************************************************
* Function Name: CNode::addMNPDOvar
* Description: 
* Return value: void
****************************************************************************************************/
void CNode::addMNPDOvar(MNPdoVariable stPdoVar, EPDOType enumEpdoType)
{
	if(enumEpdoType == PDO_RPDO)
	{
		INT32 iItemPosition = MNPDOOUTVarCollection.Add();
		//*(ProcessImageCollection + i) = processImage;	
		MNPDOOUTVarCollection[iItemPosition] = stPdoVar;
	}
	else if(enumEpdoType == PDO_TPDO)
	{
		INT32 iItemPosition = MNPDOINVarCollection.Add();
		//*(ProcessImageCollection + i) = processImage;	
		MNPDOINVarCollection[iItemPosition] = stPdoVar;
	}
}
 
/**************************************************************************************************
* Function Name: CNode::CreateApplicationProcess
* Description: 
* Return value: void
****************************************************************************************************/
void CNode::CreateApplicationProcess()
{
	CApplicationProcess* pobjAppliProcess;
	
	pobjAppliProcess 	 = new CApplicationProcess();
	m_ApplicationProcess =  pobjAppliProcess;
}
/**************************************************************************************************
* Function Name: CNode::CreateApplicationProcess
* Description: 
* Return value: void
****************************************************************************************************/
void CNode::CreateNetworkManagament()
{
	CNetworkManagement* pobjNwManagement;
	
	pobjNwManagement 	 = new CNetworkManagement();
	m_NetworkManagement =  pobjNwManagement;
}

/**************************************************************************************************
* Function Name: CNode::getPDOIndexCollection
* Description: 
* Return value: CIndexCollection*
****************************************************************************************************/
CIndexCollection* CNode::getPDOIndexCollection(EPDOType enumEpdoType)
{
	CIndexCollection* pobjPdoIndexCollection = new CIndexCollection();
	
	for(INT32 iLoopCount=0; iLoopCount < m_IndexCollection->getNumberofIndexes(); iLoopCount++)
	{
		CIndex* pobjIndex;
		
		pobjIndex = m_IndexCollection->getIndex(iLoopCount);
		
		if(pobjIndex->getPDOType() == enumEpdoType)
		{
			pobjPdoIndexCollection->addIndex(*pobjIndex);					
		}
	}
	return pobjPdoIndexCollection;
}
	
/**************************************************************************************************
* Function Name: CNode::getPDOIndexCollection
* Description: 
* Return value: CIndexCollection*
****************************************************************************************************/
CIndexCollection* CNode::getPDOIndexCollection()
{
	CIndexCollection* pobjPdoIndexCollection = new CIndexCollection();
	
	for(INT32 iLoopCount=0; iLoopCount < m_IndexCollection->getNumberofIndexes(); iLoopCount++)
	{
		CIndex* pobjIndex;
		
		pobjIndex = m_IndexCollection->getIndex(iLoopCount);
		if(pobjIndex->getPDOType() ==  PDO_TPDO || pobjIndex->getPDOType() ==  PDO_RPDO)
		{
			pobjPdoIndexCollection->addIndex(*pobjIndex);					
			//printf("\nIndex:%s", pobjIndex->getIndexValue());
		}
	}
	return pobjPdoIndexCollection;
}

/**************************************************************************************************
* Function Name: CNode::getIndexCollectionWithoutPDO
* Description: 
* Return value: CIndexCollection*
****************************************************************************************************/
CIndexCollection* CNode::getIndexCollectionWithoutPDO()
{
	CIndexCollection* pobjIndexCol = new CIndexCollection();
	
	for(INT32 iLoopCount=0; iLoopCount < m_IndexCollection->getNumberofIndexes(); iLoopCount++)
	{
		CIndex* pobjIndex;
		
		pobjIndex = m_IndexCollection->getIndex(iLoopCount);
		if(CheckIfNotPDO((char*)pobjIndex->getIndexValue()))
		{
			pobjIndexCol->addIndex(*pobjIndex);			
		}
	}			
	return pobjIndexCol;
}
	
//CDTCollection* CNode::getComplexDTCollection()
// {
//	return collectionObjCDT;
// }

/**************************************************************************************************
* Function Name: CNode::isNull
* Description: 
* Return value: bool
****************************************************************************************************/
bool CNode::isNull()
{
	if (m_IndexCollection == NULL)
	{
		return true;
	}
	else
	{
		return false;
	}
}

//CNode CNode::newNull()
//{return new CNullObjectClass();}

/**************************************************************************************************
* Function Name: CNode::getPIbyParaIndex
* Description: 
* Return value: ProcessImage*
****************************************************************************************************/
ProcessImage* CNode::getPIbyParaIndex(int iParaIndex)
{
	ProcessImage* pobjProcessImage = NULL;
	
	for(INT32 iLoopCount=0; iLoopCount < ProcessImageCollection.Count(); iLoopCount++)
	{
		pobjProcessImage = ProcessImageCollection.GetAddress(iLoopCount);
		
		if(pobjProcessImage->ParametrIndex == iParaIndex)
		{
			return pobjProcessImage;			
		}
	}			
	return pobjProcessImage;
}

/**************************************************************************************************
* Function Name: CNode::DeleteCollectionsForPI
* Description: 
* Return value: void
****************************************************************************************************/
void CNode::DeleteCollectionsForPI()
{
	if(MNPDOINVarCollection.Count() != 0)
	{
		MNPDOINVarCollection.Clear();
	}
	if(MNPDOOUTVarCollection.Count()!=0)	
	{
		MNPDOOUTVarCollection.Clear();
	}
	if(ProcessImageCollection.Count()!=0)
	{
		ProcessImageCollection.Clear();
	}
}

/********************
* Function Name: CNode::DeleteCollectionsForPI
* Description: 
* Return value: void
****************************************************************************************************/
void CNode::DeleteCollectionsForNETPI()
{
	
	if(NETProcessImageCollection.Count() != 0)
	{
		NETProcessImageCollection.Clear();
	}
}

/**************************************************************************************************
* Function Name: CNode::getStationType
* Description: Returns the stattion type of the Node
* Return value: EStationType
****************************************************************************************************/
EStationType CNode::getStationType()
{
	return m_StationType;
}

/**************************************************************************************************
* Function Name: CNode::setNodeType
* Description: sets the Node Type of the Node.Value can be either CN or MN
* Return value: void
****************************************************************************************************/
void CNode::setStationType(EStationType StationType)
{
	m_StationType = StationType; 
}
/**************************************************************************************************
* Function Name: CNode::getForcedCycle
* Description: Returns the forced cycle of the Node
* Return value:
****************************************************************************************************/
char* CNode::getForcedCycle()
{
	return m_ForcedCycle;
}

/**************************************************************************************************
* Function Name: CNode::setForcedCycle
* Description: sets the cycle of the Node
* Return value: void
****************************************************************************************************/
void CNode::setForcedCycle(char* pbForcedCycle)
{	
	m_ForcedCycle = new char[strlen(pbForcedCycle) + ALLOC_BUFFER];
	strcpy((char*)m_ForcedCycle, pbForcedCycle);	

	//add or update 1f9b
	ocfmRetCode stErrorInfo;
	int IndexPos;
	char* strConvertedValue;
	strConvertedValue = new char[SUBINDEX_LEN];
	char acMultiCycleAssignObj[] = MULTIPL_CYCLE_ASSIGN_OBJECT;
	stErrorInfo = IfIndexExists(240, MN, acMultiCycleAssignObj, &IndexPos);
	strConvertedValue = _IntToAscii(this->getNodeId(), strConvertedValue, 16);
	strConvertedValue = padLeft(strConvertedValue, '0', 2);

    char* subIndName = new char[50];
    subIndName[0] = 0;
    INT32 iCNsubIndFlag = 0;
	if(stErrorInfo.code == OCFM_ERR_SUCCESS)
	{
		//Index exists
		int subIndexPos;		
		stErrorInfo = IfSubIndexExists(MN_NODEID, MN, acMultiCycleAssignObj, strConvertedValue,  &subIndexPos, &IndexPos);
		if(stErrorInfo.code == OCFM_ERR_SUCCESS)
		{			

		}
		else
		{			
			stErrorInfo = AddSubIndex(MN_NODEID, MN, (char*)"1F9B", strConvertedValue);
		}
		
	}
	else
	{
		stErrorInfo = AddIndex(MN_NODEID, MN, acMultiCycleAssignObj);
		stErrorInfo = AddSubIndex(MN_NODEID, MN, acMultiCycleAssignObj, strConvertedValue);		
		
	}
    GetSubIndexAttributes(240, MN, acMultiCycleAssignObj, strConvertedValue, NAME, subIndName);
    //char* subIndFlag = new char[10];
    //GetSubIndexAttributes(240, MN, acMultiCycleAssignObj, strConvertedValue, FLAGIFINCDC, subIndFlag);
    //iCNsubIndFlag  = atoi(subIndFlag);
    SetSubIndexAttributes(240, MN, acMultiCycleAssignObj, strConvertedValue, pbForcedCycle, subIndName, TRUE);

    CIndex* objMN1F9BIndex;
    objMN1F9BIndex = getMNIndexValues(acMultiCycleAssignObj);
    if(NULL != objMN1F9BIndex)
    {
        //UpdateNumberOfEnteriesSIdx(objMN1F92Index, MN);
        objMN1F9BIndex->setFlagIfIncludedCdc(TRUE);
        if( NULL != objMN1F9BIndex->getSubIndexbyIndexValue((char*)"00"))
        {
            objMN1F9BIndex->getSubIndexbyIndexValue((char*)"00")->setFlagIfIncludedCdc(TRUE);
        }
    }

    strcpy(subIndName,"");
    //strcpy(subIndFlag,"");
    stErrorInfo = IfIndexExists(this->getNodeId(), CN, acMultiCycleAssignObj, &IndexPos);
    if(stErrorInfo.code == OCFM_ERR_SUCCESS)
    {
        //Index exists
        int subIndexPos;        
        stErrorInfo = IfSubIndexExists(this->getNodeId(), CN, acMultiCycleAssignObj, strConvertedValue,  &subIndexPos, &IndexPos);
        if(stErrorInfo.code == OCFM_ERR_SUCCESS)
        {           

        }
        else
        {           
            stErrorInfo = AddSubIndex(this->getNodeId(), CN, (char*)"1F9B", strConvertedValue);
        }
        
    }
    else
    {
        stErrorInfo = AddIndex(this->getNodeId(), CN, acMultiCycleAssignObj);
        stErrorInfo = AddSubIndex(this->getNodeId(), CN, acMultiCycleAssignObj, strConvertedValue);     
        
    }
    GetSubIndexAttributes(this->getNodeId(), CN, acMultiCycleAssignObj, strConvertedValue, NAME, subIndName);
    //GetSubIndexAttributes(this->getNodeId(), CN, acMultiCycleAssignObj, strConvertedValue, FLAGIFINCDC, subIndFlag);
    //iCNsubIndFlag  = atoi(subIndFlag);

    SetSubIndexAttributes(this->getNodeId(), CN, acMultiCycleAssignObj, strConvertedValue, pbForcedCycle, subIndName, TRUE);

    CIndex* objThis1F9BIndex;
    objThis1F9BIndex = this->m_IndexCollection->getIndexbyIndexValue( acMultiCycleAssignObj );
    if(NULL != objThis1F9BIndex)
    {
        //UpdateNumberOfEnteriesSIdx(objMN1F92Index, MN);
        objThis1F9BIndex->setFlagIfIncludedCdc(TRUE);
        if( NULL != objThis1F9BIndex->getSubIndexbyIndexValue((char*)"00"))
        {
            objThis1F9BIndex->getSubIndexbyIndexValue((char*)"00")->setFlagIfIncludedCdc(TRUE);
        }
    }

    delete[] subIndName;
    //delete[] subIndFlag;
}

/**************************************************************************************************
* Function Name: CNode::resetForcedCycleValue
* Description: sets the cycle of the Node
* Return value: void
****************************************************************************************************/
void CNode::resetForcedCycleValue()
{
    m_ForcedCycle = new char[strlen("") + ALLOC_BUFFER];
    strcpy((char*)m_ForcedCycle, "");
}
/**************************************************************************************************
* Function Name: CNode::setPollResponseTimeout
* Description: sets the poll response timeout value in MN
* Return value: void
****************************************************************************************************/
void CNode::setPollResponseTimeout(char* pbPollResponseTimeout)
{	
	m_PollResponseTimeout = new char[strlen(pbPollResponseTimeout) + ALLOC_BUFFER];
	strcpy((char*)m_PollResponseTimeout, pbPollResponseTimeout);	

	//add or update 1f92 subobjects in MN
	ocfmRetCode stErrorInfo;
	int IndexPos;
	char* strConvertedValue;
	strConvertedValue = new char[SUBINDEX_LEN];
	char acMNCNPollresponseTimeoutObj[] = MNCN_POLLRESPONSE_TIMEOUT_OBJECT;
	stErrorInfo = IfIndexExists(MN_NODEID, MN, acMNCNPollresponseTimeoutObj, &IndexPos);
	strConvertedValue = _IntToAscii(this->getNodeId(), strConvertedValue, 16);

	strConvertedValue = padLeft(strConvertedValue, '0', 2);

    char* subIndName = new char[50];
    subIndName[0] = 0;
    INT32 iCNsubIndFlag = 0;
	if(stErrorInfo.code == OCFM_ERR_SUCCESS)
	{
		//Index exists
		int subIndexPos;		
		stErrorInfo = IfSubIndexExists(MN_NODEID, MN, acMNCNPollresponseTimeoutObj, strConvertedValue,  &subIndexPos, &IndexPos);
		if(stErrorInfo.code == OCFM_ERR_SUCCESS)
		{

		}
		else
		{	
			stErrorInfo = AddSubIndex(MN_NODEID, MN, acMNCNPollresponseTimeoutObj, strConvertedValue);
		}
	}
	else
	{
		stErrorInfo = AddIndex(MN_NODEID, MN, acMNCNPollresponseTimeoutObj);
		stErrorInfo = AddSubIndex(MN_NODEID, MN, acMNCNPollresponseTimeoutObj, strConvertedValue);		
	}

    GetSubIndexAttributes(240, MN, acMNCNPollresponseTimeoutObj, strConvertedValue, NAME, subIndName);
    //char* subIndFlag = new char[10];
    //GetSubIndexAttributes(240, MN, acMNCNPollresponseTimeoutObj, strConvertedValue, FLAGIFINCDC, subIndFlag);
    //iCNsubIndFlag  = atoi(subIndFlag);

    SetSubIndexAttributes(MN_NODEID, MN, acMNCNPollresponseTimeoutObj, strConvertedValue, pbPollResponseTimeout, subIndName, TRUE);


    CIndex* objMN1F92Index;
    objMN1F92Index = getMNIndexValues(acMNCNPollresponseTimeoutObj);
    if(NULL != objMN1F92Index)
    {
        UpdateNumberOfEnteriesSIdx(objMN1F92Index, MN);
        objMN1F92Index->setFlagIfIncludedCdc(TRUE);
    }

    delete[] subIndName;
    //delete[] subIndFlag;
}

bool CNode::getForceCycleFlag()
{
	return m_ForcedCycleFlag;
}

void CNode::setForceCycleFlag(bool bForceCycleFlag)
{
	m_ForcedCycleFlag = bForceCycleFlag;
}
#pragma endregion Properties
