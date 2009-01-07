#include "../Include/BaseIndex.h"

CBaseIndex::CBaseIndex(void)
	{
		m_DefaultValue = new char;
		m_ActualValue= new char;
		m_Index = new char;
		m_Name = new char;
		m_LowLimit= new char;
		m_HighLimit= new char;
		m_accessType= new char;
	}

CBaseIndex::~CBaseIndex(void)
	{
	}
#pragma region Properties
/**************************************************************************************************
	* Function Name: getName
    * Description: Returns the Name of the Index Object
/****************************************************************************************************/

char* CBaseIndex::getName()
	{return m_Name;}

/**************************************************************************************************
	* Function Name: setName
    * Description: sets the Name of the Index Object
/****************************************************************************************************/
void CBaseIndex::setName(char* Name)
	{strcpy(m_Name, Name); }
/**************************************************************************************************
	* Function Name: getIndexValue
    * Description: Returns the Index of the Index Object
/****************************************************************************************************/

char* CBaseIndex::getIndexValue()
	{return m_Index;}

/**************************************************************************************************
	* Function Name: setIndex
    * Description: sets the Index of the Index Object
/****************************************************************************************************/
void CBaseIndex::setIndexValue(char* Index)
	{strcpy(m_Index, Index); }
/**************************************************************************************************
	* Function Name: getLowLimit
    * Description: Returns the LowLimit of the Index Object
/****************************************************************************************************/

char* CBaseIndex::getLowLimit()
	{return m_LowLimit;}

/**************************************************************************************************
	* Function Name: setLowLimit
    * Description: sets the LowLimit of the Index Object
/****************************************************************************************************/
void CBaseIndex::setLowLimit(char* LowLimit)
	{strcpy(m_LowLimit,LowLimit); }
/**************************************************************************************************
	* Function Name: getLowLimit
    * Description: Returns the LowLimit of the Index Object
/****************************************************************************************************/

char* CBaseIndex::getHighLimit()
	{return m_HighLimit;}

/**************************************************************************************************
	* Function Name: setLowLimit
    * Description: sets the LowLimit of the Index Object
/****************************************************************************************************/
void CBaseIndex::setHighLimit(char* HighLimit)
	{strcpy(m_HighLimit,HighLimit); }
/**************************************************************************************************
	* Function Name: getDefaultValue
    * Description: Returns the Default Value of the Index Object
/****************************************************************************************************/

char* CBaseIndex::getDefaultValue()
	{return m_DefaultValue;}

/**************************************************************************************************
	* Function Name: setDefaultValue
    * Description: sets the Default Value of the Index Object
/****************************************************************************************************/
void CBaseIndex::setDefaultValue(char* Value)
	{		
		strcpy(m_DefaultValue,Value);
	}
/**************************************************************************************************
	* Function Name: getActualValue
    * Description: Returns the Actual Value of the Index Object
/****************************************************************************************************/

char* CBaseIndex::getActualValue()
	{return m_ActualValue;}

/**************************************************************************************************
	* Function Name: setActualValue
    * Description: sets the Actual Value of the Index Object
/****************************************************************************************************/
void CBaseIndex::setActualValue(char* Value)
	{		
		strcpy(m_ActualValue,Value);
	}
/**************************************************************************************************
	* Function Name: getAccessType
    * Description: Returns the Access Type of the Index Object
/****************************************************************************************************/

	char* CBaseIndex::getAccessType()
	{return m_accessType;}

/**************************************************************************************************
	* Function Name: setAccessType
    * Description: sets the Access Type of the Index Object
/****************************************************************************************************/
void CBaseIndex::setAccessType(char* accessType)
	{		
		strcpy(m_accessType,accessType);
	}	
/**************************************************************************************************
	* Function Name: getObjectType
    * Description: Returns the Object Type of the Index Object
/****************************************************************************************************/

	EObjectType CBaseIndex::getObjectType ()
	{return m_objectType;}

/**************************************************************************************************
	* Function Name: setObjectType
    * Description: sets the Object of the Index Object
/****************************************************************************************************/
	void CBaseIndex::setObjectType(int objectType)
	{	
	
		switch (objectType)
			{
			case 5:
				m_objectType=DEFTYPE;
				break;
			case 6:
				m_objectType=DEFSTRUCT;
				break;
			case 7:
				m_objectType=VAR;
				break;
			case 8:
				m_objectType=ARRAY;
				break;
			case 9:
				m_objectType=RECORD;
				break;
			default:				
				break;
			}

		
	}	
/**************************************************************************************************
	* Function Name: getPDOMapping
    * Description: Returns the PDOMapping of the Index Object
/****************************************************************************************************/

EPDOMapping CBaseIndex::getPDOMapping ()
	{return m_pdoMapping;}

/**************************************************************************************************
	* Function Name: setPDOMapping
    * Description: sets the PDOMapping of the Index Object
/****************************************************************************************************/
void CBaseIndex::setPDOMapping(char* pdoMapping)
	{		
		if(strcmp(ConvertToUpper((char*)pdoMapping), "DEFAULT")==0)
		m_pdoMapping=DEFAULT;

		if(strcmp(ConvertToUpper((char*)pdoMapping), "NO")==0)
		m_pdoMapping = NO;
	}
/**************************************************************************************************
	* Function Name: getDataType
    * Description: Returns the DataType of the Index Object
/****************************************************************************************************/

DataType CBaseIndex::getDataType ()
	{return m_dataType;}

/**************************************************************************************************
	* Function Name: setDataType
    * Description: sets the DataType of the Index Object
/****************************************************************************************************/
void CBaseIndex::setDataType(char* dataType)
	{	
		
	}

#pragma endregion Properties

#pragma region MemberFunctions
/**************************************************************************************************
	* Function Name: SaveChanges
    * Description: Saves the changes of the Index properties
/****************************************************************************************************/
void CBaseIndex::SaveChanges(char* Index,char* Name)
	{
		m_Index = Index;
		m_Name  = Name;
	}
#pragma endregion MemberFunctions