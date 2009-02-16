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
#include "../Include/Node.h"
#include "../Include/Internal.h"
#include <stdio.h>

CNode::CNode(void)
	{
		m_NodeName = NULL;		
		m_IndexCollection = NULL;
		m_DataTypeCollection =  NULL;	 
		m_IndexCollection = NULL;
		m_ApplicationProcess = NULL;
		m_HasPdoObjects = false;
	}

CNode::~CNode(void)
	{
	}

#pragma region Properties
/**************************************************************************************************
	* Function Name: getNodeType
    * Description: Returns the NodeType of the Node
/****************************************************************************************************/

ENodeType CNode:: getNodeType()
	{return m_NodeType;}

/**************************************************************************************************
	* Function Name: setNodeType
    * Description: sets the Node Type of the Node.Value can be either CN or MN
/****************************************************************************************************/
void CNode::setNodeType(ENodeType NodeType)
	{m_NodeType = NodeType; }
/**************************************************************************************************
	* Function Name: getNodeId
    * Description: Returns the Index of the Index Object
/****************************************************************************************************/

int CNode::getNodeId()
	{return m_NodeId;}

/**************************************************************************************************
	* Function Name: setIndex
    * Description: sets the Index of the Index Object
/****************************************************************************************************/
void CNode::setNodeId(int NodeId)
	{
		m_NodeId = NodeId;
		
	}
/**************************************************************************************************
	* Function Name: getNodeIndex
    * Description: Returns the Index of the Index Object
/****************************************************************************************************/

int CNode::getNodeIndex()
	{return m_NodeIndex;}

/**************************************************************************************************
	* Function Name: setNodeIndex
    * Description: sets the Index of the Index Object
/****************************************************************************************************/
void CNode::setNodeIndex(int NodeIndex)
	{m_NodeIndex = NodeIndex; }
/**************************************************************************************************
	* Function Name: getNodeName
    * Description: Returns the Nama of the Node
/****************************************************************************************************/

char* CNode::getNodeName()
	{return m_NodeName;}

/**************************************************************************************************
	* Function Name: setNodeName
    * Description: sets the Name of the Node
/****************************************************************************************************/
void CNode::setNodeName(char* NodeName)
	{	
		m_NodeName = new char[strlen(NodeName)];
		strcpy((char*)m_NodeName, NodeName);
	}
	/**************************************************************************************************
	* Function Name: HasPdoObjects
    * Description: Returns true if PDO Objects are present else false
/****************************************************************************************************/

bool CNode::HasPdoObjects()
	{return m_HasPdoObjects;}

/**************************************************************************************************
	* Function Name: setNodeName
    * Description: sets the Name of the Node
/****************************************************************************************************/
void CNode::setFlagForPdoObjects(bool flag)
	{	
		m_HasPdoObjects = flag;
	}
/**************************************************************************************************
	* Function Name: getIndexCollection
    * Description: Returns the IndexCollection of the Node
/****************************************************************************************************/

CIndexCollection* CNode::getIndexCollection()
	{
		return m_IndexCollection;
	}
/**************************************************************************************************
	* Function Name: getApplicationProcess
    * Description: Returns the Application Process of the Node
/****************************************************************************************************/

CApplicationProcess* CNode::getApplicationProcess()
	{
		return m_ApplicationProcess;
	}
/**************************************************************************************************
	* Function Name: getDataTypeCollection
    * Description: returns the DataType Collection
/****************************************************************************************************/
CDataTypeCollection* CNode::getDataTypeCollection()
	{return m_DataTypeCollection;}
void CNode::CreateIndexCollection() 
	{
		CIndexCollection* objIndexCollection;
		objIndexCollection = new CIndexCollection();
		m_IndexCollection =objIndexCollection ;
	}
void CNode::CreateDataTypeCollection() 
	{
		CDataTypeCollection* objDataTypeCollection;
		objDataTypeCollection = new CDataTypeCollection();
		m_DataTypeCollection = objDataTypeCollection ;
	}

 void CNode::addProcessImage(ProcessImage processImage)
 {
	 int i = ProcessImageCollection.Add();
  //*(ProcessImageCollection + i) = processImage;	
  ProcessImageCollection[i] = processImage;
 }
void CNode::CreateApplicationProcess()
{
	CApplicationProcess* objAPPProcess;
	objAPPProcess = new CApplicationProcess();
	m_ApplicationProcess =  objAPPProcess;
}


CIndexCollection* CNode::getPDOIndexCollection(EPDOType PDOType)
	{
		printf("\nGetPDOCollection starts");
		CIndexCollection* PDOCollection = new CIndexCollection();
		for(int count=0;count<m_IndexCollection->getNumberofIndexes();count++)
			{
				CIndex* objIndex;
				objIndex = m_IndexCollection->getIndex(count);
				if(objIndex->getPDOType() ==PDOType)
					PDOCollection->addIndex(*objIndex);					
			}
			printf("\nGetPDOCollection ends");
		return PDOCollection;
	}
	CIndexCollection* CNode::getPDOIndexCollection()
	{
		printf("\nGetPDOCollection starts");
		CIndexCollection* PDOCollection = new CIndexCollection();
		for(int count=0;count<m_IndexCollection->getNumberofIndexes();count++)
			{
				CIndex* objIndex;
				objIndex = m_IndexCollection->getIndex(count);
				if(objIndex->getPDOType() ==  PDO_TPDO || objIndex->getPDOType() ==  PDO_RPDO)
					PDOCollection->addIndex(*objIndex);					
			}
			printf("\nGetPDOCollection ends");
		return PDOCollection;
	}
CIndexCollection* CNode::getIndexCollectionWithoutPDO()
	{
		CIndexCollection* objIndexCol = new CIndexCollection();
		for(int count=0;count<m_IndexCollection->getNumberofIndexes();count++)
			{
				CIndex* objIndex;
				objIndex = m_IndexCollection->getIndex(count);
				if(CheckIfNotPDO((char*)objIndex->getIndexValue()))
						objIndexCol->addIndex(*objIndex);			
			}			
		return objIndexCol;
	}
//CDTCollection* CNode::getComplexDTCollection()
// {
//	return collectionObjCDT;
// }
bool CNode::isNull()
{
	if (m_IndexCollection==NULL)
	return true;
	else
	return false;
}
//CNode CNode::newNull()
//{return new CNullObjectClass();}
ProcessImage* CNode::getPIbyParaIndex(int paraIndex)
{
	ProcessImage* pi =NULL;
	for(int count=0; count<ProcessImageCollection.Count(); count++)
		{
			pi = ProcessImageCollection.GetAddress(count);
			if(pi->ParametrIndex = paraIndex)
			return pi;			
		}			
	return pi;
}
#pragma endregion Properties


