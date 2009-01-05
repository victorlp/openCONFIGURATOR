#include "../Include/BaseIndex.h"

CBaseIndex::CBaseIndex(void)
	{
	}

CBaseIndex::~CBaseIndex(void)
	{
	}
#pragma region Properties
/**************************************************************************************************
	* Function Name: getName
    * Description: Returns the Name of the Index Object
/****************************************************************************************************/

string CBaseIndex::getName()
	{return m_Name;}

/**************************************************************************************************
	* Function Name: setName
    * Description: sets the Name of the Index Object
/****************************************************************************************************/
void CBaseIndex::setName(string Name)
	{m_Name = Name; }
/**************************************************************************************************
	* Function Name: getIndex
    * Description: Returns the Index of the Index Object
/****************************************************************************************************/

string CBaseIndex::getIndex()
	{return m_Index;}

/**************************************************************************************************
	* Function Name: setIndex
    * Description: sets the Index of the Index Object
/****************************************************************************************************/
void CBaseIndex::setIndex(string Index)
	{m_Index = Index; }

#pragma endregion Properties

#pragma region MemberFunctions
/**************************************************************************************************
	* Function Name: SaveChanges
    * Description: Saves the changes of the Index properties
/****************************************************************************************************/
void CBaseIndex::SaveChanges(string Index,string Name)
	{
		m_Index = Index;
		m_Name  = Name;
	}
#pragma endregion MemberFunctions