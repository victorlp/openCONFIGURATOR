#include "../Include/NodeCollection.h"
#include "../Include/BaseIndex.h"
#include "../Include/Declarations.h"
#include "../Include/Internal.h"


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

CBaseIndex::~CBaseIndex(void)
	{
		//delete m_DefaultValue;
	}
#pragma region Properties
/****************************************************************************************************
* Function Name: getName
* Description: Returns the Name of the Index Object
****************************************************************************************************/

const char* CBaseIndex::getName()
	{return m_Name;}

/****************************************************************************************************
* Function Name: setName
* Description: sets the Name of the Index Object
****************************************************************************************************/
void CBaseIndex::setName(char* Name)
	{
		m_Name = new char[strlen(Name) + 1];
		strcpy((char*)m_Name, Name);
	}
/****************************************************************************************************
* Function Name: getIndexValue
* Description: Returns the Index of the Index Object
****************************************************************************************************/

const char* CBaseIndex::getIndexValue()
	{return m_Index;}

/****************************************************************************************************
* Function Name: setIndex
* Description: sets the Index of the Index Object
****************************************************************************************************/
void CBaseIndex::setIndexValue(char* Index)
	{
		m_Index = new char[strlen(Index)];
		strcpy((char*)m_Index, Index);
	}
/****************************************************************************************************
* Function Name: getLowLimit
* Description: Returns the LowLimit of the Index Object
****************************************************************************************************/

const char* CBaseIndex::getLowLimit()
	{
		if(m_LowLimit != NULL)
			return m_LowLimit;
		else
			return NULL;
	}

/****************************************************************************************************
* Function Name: setLowLimit
* Description: sets the LowLimit of the Index Object
****************************************************************************************************/
void CBaseIndex::setLowLimit(char* LowLimit)
	{
		m_LowLimit = new char[strlen(LowLimit)];
		strcpy((char*)m_LowLimit,LowLimit);
 }
/****************************************************************************************************
* Function Name: getLowLimit
* Description: Returns the LowLimit of the Index Object
****************************************************************************************************/

const char* CBaseIndex::getHighLimit()
	{
		if(m_HighLimit != NULL)
			return m_HighLimit;
		else
			return NULL;
	}

/****************************************************************************************************
* Function Name: setLowLimit
* Description: sets the LowLimit of the Index Object
****************************************************************************************************/
void CBaseIndex::setHighLimit(char* HighLimit)
	{
		m_HighLimit = new char[strlen(HighLimit)];
		strcpy((char*)m_HighLimit,HighLimit);
	}
/****************************************************************************************************
* Function Name: getDefaultValue
* Description: Returns the Default Value of the Index Object
****************************************************************************************************/

const char* CBaseIndex::getDefaultValue()
	{return m_DefaultValue;}

/****************************************************************************************************
* Function Name: setDefaultValue
* Description: sets the Default Value of the Index Object
****************************************************************************************************/
void CBaseIndex::setDefaultValue(char* Value)
	{				
		m_DefaultValue = new char[strlen(Value)];
		strcpy((char*)m_DefaultValue,Value);
	}
/****************************************************************************************************
* Function Name: getActualValue
* Description: Returns the Actual Value of the Index Object
****************************************************************************************************/

const char* CBaseIndex::getActualValue()
	{
		if(m_ActualValue == NULL)
			m_ActualValue = NULL;
		return m_ActualValue;
	}

/****************************************************************************************************
* Function Name: setActualValue
* Description: sets the Actual Value of the Index Object
****************************************************************************************************/
void CBaseIndex::setActualValue(char* Value)
	{
		m_ActualValue = new char[strlen(Value) + ALLOC_BUFFER];		
		strcpy((char*)m_ActualValue,Value);
	}
/****************************************************************************************************
* Function Name: getAccessType
* Description: Returns the Access Type of the Index Object
****************************************************************************************************/

	const char* CBaseIndex::getAccessType()
	{return m_accessType;}

/****************************************************************************************************
* Function Name: setAccessType
* Description: sets the Access Type of the Index Object
****************************************************************************************************/
void CBaseIndex::setAccessType(char* accessType)
	{	
		m_accessType = new char[strlen(accessType)];	
		strcpy((char*)m_accessType,accessType);
	}	
/****************************************************************************************************
* Function Name: getObjectType
* Description: Returns the Object Type of the Index Object as const char*
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
* Function Name: getEObjectType
* Description: Returns the Object Type of the Index Object as Enum
****************************************************************************************************/
	
	EObjectType	CBaseIndex::getEObjectType()
	{
		return m_objectType;
	}

/****************************************************************************************************
* Function Name: setObjectType
* Description: sets the Object of the Index Object
****************************************************************************************************/
	void CBaseIndex::setObjectType(char* objectType)
	{	
		char *str = new char[50];
		strcpy(str, objectType);
		//if(strcmp(objectType, "5")==0 || strcmp(ConvertToUpper(objectType), "DEFTYPE")==0 )
		if(strcmp(str, "5")==0 || strcmp(ConvertToUpper(str), "DEFTYPE")==0 )
		m_objectType = DEFTYPE;
		
		else if(strcmp(str, "6")==0 || strcmp(str, "DEFSTRUCT")==0)
		m_objectType = DEFSTRUCT;
		
		else if(strcmp(str, "7")==0 || strcmp(str, "VAR")==0)
		m_objectType = VAR;
		
		else if(strcmp(str, "8")==0 || strcmp(str, "ARRAY")==0)
		m_objectType = ARRAY;
		
		else if(strcmp(str, "9")==0 || strcmp(str, "RECORD")==0)
		m_objectType = RECORD;
		delete [] str;			
	}	
/****************************************************************************************************
* Function Name: getPDOMapping
* Description: Returns the PDOMapping of the Index Object
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
* Function Name: setPDOMapping
* Description: sets the PDOMapping of the Index Object
****************************************************************************************************/
void CBaseIndex::setPDOMapping(char* pdoMapping)
	{		
		char *str = new char[50];
		strcpy(str, ConvertToUpper((char*)pdoMapping));	
		//if(strcmp(ConvertToUpper((char*)pdoMapping), "DEFAULT")==0)
		if(strcmp(ConvertToUpper(str), "DEFAULT")==0)
		m_pdoMapping = DEFAULT;
		//else if(strcmp(ConvertToUpper((char*)pdoMapping), "NO")==0)
		else if(strcmp(ConvertToUpper(str), "NO")==0)
		m_pdoMapping = NO;	 
		//else if(strcmp(ConvertToUpper((char*)pdoMapping), "OPTIONAL")==0)
		else if(strcmp(ConvertToUpper(str), "OPTIONAL")==0)
		m_pdoMapping = OPTIONAL;
		else if(strcmp(ConvertToUpper(str), "RPDO")==0)
		m_pdoMapping = RPDO;
		else if(strcmp(ConvertToUpper(str), "TPDO")==0)
		m_pdoMapping = TPDO;
		delete [] str;
	}
/****************************************************************************************************
* Function Name: getDataType
* Description: Returns the DataType of the Index Object
****************************************************************************************************/

DataType CBaseIndex::getDataType ()
	{return m_dataType;}

/****************************************************************************************************
* Function Name: setDataType
* Description: sets the DataType of the Index Object
****************************************************************************************************/
void CBaseIndex::setDataType(char* dataTypeName)
	{	
		DataType* dt;
		CNodeCollection* objNodeCol;
		objNodeCol= CNodeCollection::getNodeColObjectPointer();
		CNode objNode = objNodeCol->getNode(m_NodeID);
		CDataTypeCollection* dtcol;
		dtcol=objNode.getDataTypeCollection();
		dt = dtcol->getDataType(dataTypeName);	
		if(dt != NULL)
		{
			m_dataType= *dt;
		}		
		cout << "Out of CBaseIndex::setDataType\n\n" << endl;
		/*m_dataType.DataTypeValue =dt.DataTypeValue;
		m_dataType.Name =dt.Name ;*/
	}
/****************************************************************************************************
* Function Name: setDataType
* Description: sets the DataType of the Index Object
****************************************************************************************************/
void CBaseIndex::setDataType(char* dataTypeName, int NodeID)
	{	
		DataType* dt;
		CNodeCollection* objNodeCol;
		objNodeCol= CNodeCollection::getNodeColObjectPointer();
		CNode objNode = objNodeCol->getNode(NodeID);
		CDataTypeCollection* dtcol;
		dtcol=objNode.getDataTypeCollection();
		dt = dtcol->getDataType(dataTypeName);	
		m_dataType= *dt;
		/*m_dataType.DataTypeValue =dt.DataTypeValue;
		m_dataType.Name =dt.Name ;*/
	}
/****************************************************************************************************
* Function Name: getNodeID
* Description: Returns the NodeID of the Node of the Index Object
****************************************************************************************************/

int CBaseIndex::getNodeID()
	{return m_NodeID;}

/****************************************************************************************************
* Function Name: setNodeID
* Description: sets the NodeID of the Index Object
****************************************************************************************************/
void CBaseIndex::setNodeID(int NodeID)
	{	
		m_NodeID = NodeID;
	}
/****************************************************************************************************
* Function Name: getNodeID
* Description: Returns the NodeID of the Node of the Index Object
****************************************************************************************************/

char* CBaseIndex::getUniqueIDRef()
	{return m_UniqueIDRef;}

/****************************************************************************************************
* Function Name: setNodeID
* Description: sets the NodeID of the Index Object
****************************************************************************************************/
void CBaseIndex::setUniqueIDRef(char* UniqueID)
	{	
		m_UniqueIDRef = new char[strlen(UniqueID)];
		strcpy((char*)m_UniqueIDRef,UniqueID);
	}
/****************************************************************************************************
* Function Name: getDataTypeValue
* Description: Returns only datatype value of the Node of the Index Object
****************************************************************************************************/

char* CBaseIndex::getDataTypeValue()
	{
		if(m_dataTypeValue != NULL)
			return m_dataTypeValue;
		else
			return NULL;
	}

/****************************************************************************************************
* Function Name: setDataTypeValue
* Description: sets the datatype value  of the Index Object
****************************************************************************************************/
void CBaseIndex::setDataTypeValue(char* value)
	{	
		m_dataTypeValue = new char[strlen(value)];
		strcpy((char*)m_dataTypeValue,value);
	}
/****************************************************************************************************
* Function Name: setDefaultValue
* Description: sets the datatype value  of the Index Object
****************************************************************************************************/
void CBaseIndex::setDataTypeST(DataType dt)
	{	
		m_dataType = dt;
	}
#pragma endregion Properties

#pragma region MemberFunctions
/****************************************************************************************************
* Function Name: SaveChanges
* Description: Saves the changes of the Index properties
****************************************************************************************************/
void CBaseIndex::SaveChanges(char* Index,char* Name)
	{
		m_Index = Index;
		m_Name  = Name;
	}
/****************************************************************************************************
* Function Name: ValidateIndex
* Description: Saves the changes of the Index properties
****************************************************************************************************/
bool CBaseIndex::IsIndexVaueValid(char* hexValue)
	{
		unsigned long l,lowlimit,highLimit;
		bool flag = true;
		return flag;
		l = hex2int(hexValue);
		if(this->m_LowLimit != NULL)
		{		
		 lowlimit = hex2int((char *)m_LowLimit);	
		 if(l>= lowlimit)
			flag = true;
		 else flag = false;	 
		}
		if(this->m_HighLimit!= NULL)
		{
			highLimit = hex2int((char *)m_HighLimit);
			if(l<= highLimit)
			flag = true;
			else flag = false;
		} 
		return flag;
	}
	
/****************************************************************************************************
* Function Name: setFlagIfIncludedCdc
* Description: sets the flag whether the Index should be included in cdc
****************************************************************************************************/
void CBaseIndex::setFlagIfIncludedCdc(EFlag flag)
	{
		m_IsIncludedInCDC = flag;
	}
/****************************************************************************************************
* Function Name: getFlagIfIncludedCdc
* Description: Returns whether the Index should be included in cdc
****************************************************************************************************/

EFlag CBaseIndex::getFlagIfIncludedCdc()
	{return m_IsIncludedInCDC;}
#pragma endregion MemberFunctions
