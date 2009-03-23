///////////////////////////////////////////////////////////////////////////////////////////////
//
//  $Header: $
//
// NAME:  BaseIndex.cpp
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
#include "../Include/NodeCollection.h"
#include "../Include/BaseIndex.h"
#include "../Include/Declarations.h"
#include "../Include/Internal.h"

/****************************************************************************************************
* FUNCTION DEFINITIONS
****************************************************************************************************/

/****************************************************************************************************
* Constructor
****************************************************************************************************/
CBaseIndex::CBaseIndex(void)
{
		
		m_Index         =  NULL;
		m_Name          =  NULL;
		m_HighLimit     =  NULL;		
		m_dataType.Name =  NULL;
		m_dataTypeValue = NULL;
		m_DefaultValue  =  NULL;		
		m_ActualValue   =  NULL;		
		m_accessType    =  NULL;
		m_LowLimit	    =  NULL;
		m_UniqueIDRef	  =  NULL;
		m_dataTypeValue = NULL;
		m_dataType.DataTypeValue = NULL;
		m_dataType.DataSize = NULL;
		m_IsIncludedInCDC = FALSE;

}

/****************************************************************************************************
* Destructor
****************************************************************************************************/
CBaseIndex::~CBaseIndex(void)
{
	//Add destructor code here	
	//delete m_DefaultValue;
}
#pragma region Properties
/****************************************************************************************************
* Function Name: CBaseIndex::getName
* Description: Returns the Name of the Index Object
* Return value: const char*
****************************************************************************************************/

const char* CBaseIndex::getName()
{
		return m_Name;
}

/****************************************************************************************************
* Function Name: CBaseIndex::setName
* Description: sets the Name of the Index Object
* Return value: void
****************************************************************************************************/
void CBaseIndex::setName(char* pbName)
{
		if(pbName != NULL)
		{
		m_Name = new char[strlen(pbName) + STR_ALLOC_BUFFER];
		strcpy((char*)m_Name, pbName);
		}
		else
		{
				m_Name = new char[1 + ALLOC_BUFFER];
				strcpy((char*)m_Name,"");
		}
}
/****************************************************************************************************
* Function Name: CBaseIndex::getIndexValue
* Description: Returns the Index of the Index Object
* Return value: const char*
****************************************************************************************************/

const char* CBaseIndex::getIndexValue()
{
		return m_Index;
}

/****************************************************************************************************
* Function Name: CBaseIndex::setIndexValue
* Description: sets the Index of the Index Object
* Return value: void
****************************************************************************************************/
void CBaseIndex::setIndexValue(char* pbIndex)
{
		m_Index = new char[strlen(pbIndex) + STR_ALLOC_BUFFER];
		strcpy((char*)m_Index, pbIndex);
}
/****************************************************************************************************
* Function Name: CBaseIndex::getLowLimit
* Description: Returns the LowLimit of the Index Object
* Return value: const char*
****************************************************************************************************/

const char* CBaseIndex::getLowLimit()
{
		if(m_LowLimit != NULL)
		{
			return m_LowLimit;
		}
		else
		{
			return NULL;
		}
}

/****************************************************************************************************
* Function Name: CBaseIndex::setLowLimit
* Description: sets the LowLimit of the Index Object
* Return value: void
****************************************************************************************************/
void CBaseIndex::setLowLimit(char* pbLowLimit)
{
		m_LowLimit = new char[strlen(pbLowLimit) + STR_ALLOC_BUFFER];
		strcpy((char*)m_LowLimit, pbLowLimit);
}
/****************************************************************************************************
* Function Name: CBaseIndex::getHighLimit
* Description: Returns the LowLimit of the Index Object
* Return value: const char*
****************************************************************************************************/
const char* CBaseIndex::getHighLimit()
{
		if(m_HighLimit != NULL)
		{
			return m_HighLimit;
		}
		else
		{
			return NULL;
		}
}

/****************************************************************************************************
* Function Name: CBaseIndex::setHighLimit
* Description: sets the LowLimit of the Index Object
* Return value: void
****************************************************************************************************/
void CBaseIndex::setHighLimit(char* pbHighLimit)
{
		m_HighLimit = new char[strlen(pbHighLimit) + STR_ALLOC_BUFFER];
		strcpy((char*)m_HighLimit, pbHighLimit);
}

/****************************************************************************************************
* Function Name: CBaseIndex::getDefaultValue
* Description: Returns the Default Value of the Index Object
* Return value: const char*
****************************************************************************************************/
const char* CBaseIndex::getDefaultValue()
{
		return m_DefaultValue;
}

/****************************************************************************************************
* Function Name: CBaseIndex::setDefaultValue
* Description: sets the Default Value of the Index Object
* Return value: void
****************************************************************************************************/
void CBaseIndex::setDefaultValue(char* pbValue)
{				
		m_DefaultValue = new char[strlen(pbValue) + STR_ALLOC_BUFFER];
		strcpy((char*)m_DefaultValue, pbValue);
}
/****************************************************************************************************
* Function Name: CBaseIndex::getActualValue
* Description: Returns the Actual Value of the Index Object
* Return value: const char*
****************************************************************************************************/

const char* CBaseIndex::getActualValue()
{
		if(m_ActualValue == NULL)
		{
			m_ActualValue = NULL;
		}
		return m_ActualValue;
}

/****************************************************************************************************
* Function Name: CBaseIndex::setActualValue
* Description: sets the Actual Value of the Index Object
* Return value: void
****************************************************************************************************/
void CBaseIndex::setActualValue(char* pbValue)
{
		m_ActualValue = new char[strlen(pbValue) + STR_ALLOC_BUFFER];		
		strcpy((char*)m_ActualValue, pbValue);
}
/****************************************************************************************************
* Function Name: CBaseIndex::getAccessType
* Description: Returns the Access Type of the Index Object
* Return value: const char*
****************************************************************************************************/
const char* CBaseIndex::getAccessType()
{
		return m_accessType;
}

/****************************************************************************************************
* Function Name: CBaseIndex::setAccessType
* Description: sets the Access Type of the Index Object
* Return value: void
****************************************************************************************************/
void CBaseIndex::setAccessType(char* pbAccessType)
{	
		m_accessType = new char[strlen(pbAccessType) + STR_ALLOC_BUFFER];	
		strcpy((char*)m_accessType, pbAccessType);
}	
/****************************************************************************************************
* Function Name: CBaseIndex::getObjectType
* Description: Returns the Object Type of the Index Object as const char*
* Return value: const char*
****************************************************************************************************/

const char* CBaseIndex::getObjectType ()
{
		switch (m_objectType)
			{
			case DEFTYPE:
				return "DEFTYPE";
				break;
			case DEFSTRUCT:
				return "DEFSTRUCT";
				break;
			case VAR:
				return "VAR";
				break;
			case ARRAY:
				return "ARRAY";
				break;
			case RECORD:
				return "RECORD";
				break;
			default:
				return NULL;
				break;
			}
	
}
	
/****************************************************************************************************
* Function Name: CBaseIndex::getEObjectType
* Description: Returns the Object Type of the Index Object as Enum
* Return value: EObjectType
****************************************************************************************************/
EObjectType	CBaseIndex::getEObjectType()
{
		return m_objectType;
}

/****************************************************************************************************
* Function Name: CBaseIndex::setObjectType
* Description: sets the Object of the Index Object
* Return value: void
****************************************************************************************************/
void CBaseIndex::setObjectType(char* pbObjectType)
{	
		char *pbStrBuff = new char[strlen(pbObjectType) + STR_ALLOC_BUFFER];

		strcpy(pbStrBuff, pbObjectType);
		//if(strcmp(pbObjectType, "5") == 0 || strcmp(ConvertToUpper(pbObjectType), "DEFTYPE") == 0 )
		if(strcmp(pbStrBuff, "5") == 0 || strcmp(ConvertToUpper(pbStrBuff), "DEFTYPE") == 0 )
		{
			m_objectType = DEFTYPE;
		}
		
		else if(strcmp(pbStrBuff, "6") == 0 || strcmp(pbStrBuff, "DEFSTRUCT") == 0)
		{
			m_objectType = DEFSTRUCT;
		}
		
		else if(strcmp(pbStrBuff, "7") == 0 || strcmp(pbStrBuff, "VAR") == 0)
		{
			m_objectType = VAR;
		}
		
		else if(strcmp(pbStrBuff, "8") == 0 || strcmp(pbStrBuff, "ARRAY") == 0)
		{
			m_objectType = ARRAY;
		}
		
		else if(strcmp(pbStrBuff, "9") == 0 || strcmp(pbStrBuff, "RECORD") == 0)
		{
			m_objectType = RECORD;
		}
		else
		{
			#ifdef DEBUG
				cout << "Error! setObjectType failed\n" << endl;
			#endif
		}
		
		delete [] pbStrBuff;			
}	
/****************************************************************************************************
* Function Name: CBaseIndex::getPDOMapping
* Description: Returns the PDOMapping of the Index Object
* Return value: const char*
****************************************************************************************************/

const char* CBaseIndex::getPDOMapping ()
{
		switch(m_pdoMapping)
		{
			case NO:
				//cout << "\nNO" << endl;
				return "NO";				
			case DEFAULT:
				//cout << "\nDEFAULT" << endl;
				return "DEFAULT";				
			case OPTIONAL:
				//cout << "\nOPTIONAL" << endl;
				return "OPTIONAL";
			case RPDO:
				//cout << "\nRPDO" << endl;
				return "RPDO";
			case TPDO:
				//cout << "\nTPDO" << endl;
				return "TPDO";
			default:
				return NULL;			
		}
		//return m_pdoMapping;
}

/****************************************************************************************************
* Function Name: CBaseIndex::setPDOMapping
* Description: sets the PDOMapping of the Index Object
* Return value: void
****************************************************************************************************/
void CBaseIndex::setPDOMapping(char* pbPdoMapping)
{		
		char *pbStrBuff = new char[strlen(pbPdoMapping) + STR_ALLOC_BUFFER];

		strcpy(pbStrBuff, ConvertToUpper((char*)pbPdoMapping));	
		//if(strcmp(ConvertToUpper((char*)pdoMapping), "DEFAULT")==0)
		if(strcmp(ConvertToUpper(pbStrBuff), "DEFAULT") == 0)
		{
			m_pdoMapping = DEFAULT;
		}
		//else if(strcmp(ConvertToUpper((char*)pdoMapping), "NO")==0)
		else if(strcmp(ConvertToUpper(pbStrBuff), "NO") == 0)
		{
			m_pdoMapping = NO;	 
		}
		//else if(strcmp(ConvertToUpper((char*)pdoMapping), "OPTIONAL")==0)
		else if(strcmp(ConvertToUpper(pbStrBuff), "OPTIONAL") == 0)
		{
			m_pdoMapping = OPTIONAL;
		}
		else if(strcmp(ConvertToUpper(pbStrBuff), "RPDO") == 0)
		{
			m_pdoMapping = RPDO;
		}
		else if(strcmp(ConvertToUpper(pbStrBuff), "TPDO") == 0)
		{
			m_pdoMapping = TPDO;
		}
		else
		{
			#ifdef DEBUG
				cout << "Error! setPDOMapping failed\n" << endl;
			#endif
		}
		delete [] pbStrBuff;
}
/****************************************************************************************************
* Function Name: CBaseIndex::getDataType
* Description: Returns the DataType of the Index Object
* Return value: DataType
****************************************************************************************************/
DataType CBaseIndex::getDataType ()
{
		return m_dataType;
}

/****************************************************************************************************
* Function Name: CBaseIndex::setDataType
* Description: sets the DataType of the Index Object
* Return value: void
****************************************************************************************************/
void CBaseIndex::setDataType(char* pbDataTypeName)
{	
		DataType* pobjDataType;
		CNodeCollection* pobjNodeCol;
		CDataTypeCollection* pobjDataCol;

		pobjNodeCol		= CNodeCollection::getNodeColObjectPointer();
		CNode objNode 	= pobjNodeCol->getNode(m_NodeID);
		pobjDataCol		= objNode.getDataTypeCollection();
		pobjDataType 	= pobjDataCol->getDataType(pbDataTypeName);

		if(pobjDataType != NULL)
		{
			m_dataType= *pobjDataType;
		}
		/*m_dataType.DataTypeValue =pobjDataType.DataTypeValue;
		m_dataType.Name =pobjDataType.Name ;*/
}
/****************************************************************************************************
* Function Name: CBaseIndex::setDataType
* Description: sets the DataType of the Index Object
* Return value: void
****************************************************************************************************/
void CBaseIndex::setDataType(char* pbDataTypeName, int iNodeID)
	{	
		DataType* pobjDataType;
		CNodeCollection* pobjNodeCol;
		CDataTypeCollection* pobjDataCol;

		pobjNodeCol		= CNodeCollection::getNodeColObjectPointer();
		CNode objNode 	= pobjNodeCol->getNode(iNodeID);
		
		pobjDataCol 	= objNode.getDataTypeCollection();
		pobjDataType	= pobjDataCol->getDataTypeByName(pbDataTypeName);	
		m_dataType		= *pobjDataType;
		/*m_dataType.DataTypeValue = pobjDataType.DataTypeValue;
		m_dataType.Name = pobjDataType.Name ;*/
	}
/****************************************************************************************************
* Function Name: CBaseIndex::getNodeID
* Description: Returns the NodeID of the Node of the Index Object
* Return value: int
****************************************************************************************************/

INT32 CBaseIndex::getNodeID()
{
		return m_NodeID;
}

/****************************************************************************************************
* Function Name: CBaseIndex::setNodeID
* Description: sets the NodeID of the Index Object
* Return value: void
****************************************************************************************************/
void CBaseIndex::setNodeID(int NodeID)
{
		m_NodeID = NodeID;
}
/****************************************************************************************************
* Function Name: CBaseIndex::getUniqueIDRef
* Description: Returns the UniqueIDRef of the Node of the Index Object
* Return value: char*
****************************************************************************************************/

char* CBaseIndex::getUniqueIDRef()
{
		return m_UniqueIDRef;
}

/****************************************************************************************************
* Function Name: CBaseIndex::setUniqueIDRef
* Description: sets the UniqueIDRef of the Index Object
* Return value: void
****************************************************************************************************/
void CBaseIndex::setUniqueIDRef(char* pbUniqueID)
{
		m_UniqueIDRef = new char[strlen(pbUniqueID) + STR_ALLOC_BUFFER];
		strcpy((char*)m_UniqueIDRef, pbUniqueID);
}
/****************************************************************************************************
* Function Name: CBaseIndex::getDataTypeValue
* Description: Returns only datatype value of the Node of the Index Object
* Return value: char*
****************************************************************************************************/

char* CBaseIndex::getDataTypeValue()
{
		if(m_dataTypeValue != NULL)
		{
			return m_dataTypeValue;
		}
		else
		{
			return NULL;
		}
}

/****************************************************************************************************
* Function Name: CBaseIndex::setDataTypeValue
* Description: sets the datatype value  of the Index Object
* Return value: void
****************************************************************************************************/
void CBaseIndex::setDataTypeValue(char* pbValue)
{	
		m_dataTypeValue = new char[strlen(pbValue) + STR_ALLOC_BUFFER];
		strcpy((char*)m_dataTypeValue, pbValue);
}
/****************************************************************************************************
* Function Name: CBaseIndex::setDataTypeST
* Description: sets the datatype value  of the Index Object
* Return value: void
****************************************************************************************************/
void CBaseIndex::setDataTypeST(DataType objDataType)
{
		m_dataType = objDataType;
}
#pragma endregion Properties

#pragma region MemberFunctions
/****************************************************************************************************
* Function Name: CBaseIndex::SaveChanges
* Description: Saves the changes of the Index properties
* Return value: void
****************************************************************************************************/
void CBaseIndex::SaveChanges(char* pbIndex,char* pbName)
{
		m_Index = pbIndex;
		m_Name  = pbName;
}
/****************************************************************************************************
* Function Name: CBaseIndex::IsIndexVaueValid
* Description: Saves the changes of the Index properties
* Return value: bool
****************************************************************************************************/
bool CBaseIndex::IsIndexVaueValid(char* pbHexValue)
{
		unsigned long ulValue;
		unsigned long ulLowlimit;
		unsigned long ulHighLimit;
		bool bFlag = true;

		if(strcmp(pbHexValue, "") == 0)
			return true;
		
		ulValue = hex2int(pbHexValue);
		if(this->m_LowLimit != NULL)
		{		
			if(strcmp(this->m_LowLimit,"")!=0)
			{
				ulLowlimit = hex2int((char *)m_LowLimit);	
				
				if(ulValue>= ulLowlimit)
				{
					bFlag = true;
				}
				else
				{
					bFlag = false;	 
				}
			}
		}
		if(this->m_HighLimit!= NULL)
		{
			if(strcmp(this->m_HighLimit,"")!=0)
			{
				ulHighLimit = hex2int((char *)m_HighLimit);
				
				if(ulValue<= ulHighLimit)
				{
					bFlag = true;
				}
				else
				{
					bFlag = false;
				}
			}
		} 
		return bFlag;
}
	
/****************************************************************************************************
* Function Name: CBaseIndex::setFlagIfIncludedCdc
* Description: sets the flag whether the Index should be included in cdc
* Return value: void
****************************************************************************************************/

void CBaseIndex::setFlagIfIncludedCdc(EFlag enumFlag)
{
		m_IsIncludedInCDC = enumFlag;
}

/****************************************************************************************************
* Function Name: CBaseIndex::getFlagIfIncludedCdc
* Description: Returns whether the Index should be included in cdc
* Return value: EFlag
****************************************************************************************************/

EFlag CBaseIndex::getFlagIfIncludedCdc()
{
		return m_IsIncludedInCDC;
}
#pragma endregion MemberFunctions
