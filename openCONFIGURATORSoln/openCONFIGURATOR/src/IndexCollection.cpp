///////////////////////////////////////////////////////////////////////////////////////////////
//
//  $Header: $
//
// NAME:  IndexCollection.cpp
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
#include "../Include/IndexCollection.h"
#include "../Include/Internal.h"

/****************************************************************************************************
* FUNCTION DEFINITIONS
****************************************************************************************************/

/****************************************************************************************************
* Constructor
****************************************************************************************************/
CIndexCollection::CIndexCollection(void)
{
	//IndexList.size =0;
	m_IndexCount = 0;
}

/****************************************************************************************************
* Destructor
****************************************************************************************************/
CIndexCollection::~CIndexCollection(void)
{
	//Add destructor code here
}

/****************************************************************************************************
* Function Name: CIndexCollection::addIndex
* Description:
* Return value: void
****************************************************************************************************/
void CIndexCollection::addIndex(CIndex objIndex)
{		
	INT32 iItemPosition = collectionObj.Add();
	char* substr= new char[3];
	substr = subString((char*)objIndex.getIndexValue(), 0, 2);
	if(strcmp(substr,"14")==0 ||strcmp(substr,"16") ==0)
	{
		objIndex.setPDOType(PDO_RPDO);			
	}
	else if(strcmp(substr,"18")==0 ||strcmp(substr,"1A") ==0)
	{
		objIndex.setPDOType(PDO_TPDO);		
	}
	collectionObj[iItemPosition] = objIndex;
	m_IndexCount= collectionObj.Count();
}
	
/****************************************************************************************************
* Function Name: CIndexCollection::deleteIndex
* Description:
* Return value: void
****************************************************************************************************/
void CIndexCollection::deleteIndex(int iIndexID)
{
   collectionObj.Remove(iIndexID);
   m_IndexCount = collectionObj.Count();
   //printf("\n\nAfter Delete - Count:%d\n\n", m_IndexCount);
}
	
/****************************************************************************************************
* Function Name: CIndexCollection::DeleteIndexCollection
* Description:
* Return value: void
****************************************************************************************************/
void CIndexCollection::DeleteIndexCollection() 
{
	collectionObj.Clear();
	m_IndexCount = collectionObj.Count();
}

/****************************************************************************************************
* Function Name: CIndexCollection::DeletePDOs
* Description:
* Return value: void
****************************************************************************************************/
void CIndexCollection::DeletePDOs() 
{
	int iLoopCount;
	char* substr= new char[3];
	
	for(iLoopCount =0; iLoopCount < m_IndexCount; iLoopCount++)
	{
		CIndex objIndex;
		objIndex = collectionObj[iLoopCount];
		substr = subString((char*)objIndex.getIndexValue(), 0, 2);
		if(strcmp(substr, "1A")==0 || strcmp(substr, "1a")==0 ||
		strcmp(substr, "14")==0 || strcmp(substr, "16")==0   ||
		strcmp(substr, "18")==0 )
		{	
			collectionObj.Remove(iLoopCount);
			m_IndexCount = collectionObj.Count();
			iLoopCount = 0;
		}		
	}		
}

/****************************************************************************************************
* Function Name: CIndexCollection::DeletePIObjects
* Description:
* Return value: void
****************************************************************************************************/
void CIndexCollection::DeletePIObjects()
{
	int iLoopCount;
	char* substr= new char[2];
	
	for(iLoopCount =0; iLoopCount < m_IndexCount; iLoopCount++)
	{
		CIndex objIndex;
		objIndex = collectionObj[iLoopCount];
			substr = subString((char*)objIndex.getIndexValue(), 0, 1);
		if(strcmp(substr,"A")==0  ||  strcmp(substr,"a")==0 )
		{
			collectionObj.Remove(iLoopCount);
			m_IndexCount = collectionObj.Count();
			iLoopCount = 0;
		}		
	}		
}
	
/****************************************************************************************************
* Function Name: CIndexCollection::getIndex
* Description:
* Return value: CIndex*
****************************************************************************************************/
CIndex* CIndexCollection::getIndex(int iCount)
{
	return &collectionObj[iCount];
}
	
/****************************************************************************************************
* Function Name: CIndexCollection::getIndexbyIndexValue
* Description:
* Return value: CIndex*
****************************************************************************************************/
CIndex* CIndexCollection::getIndexbyIndexValue(char* pbIndex)
{
	INT32 iLoopCount;
	
	for(iLoopCount =0; iLoopCount < m_IndexCount; iLoopCount++)
	{
		CIndex objIndex;
		
		objIndex = collectionObj[iLoopCount];
		if(strcmp(objIndex.getIndexValue(),pbIndex) == 0)
		{
			return &collectionObj[iLoopCount];
		}
	}		
	return NULL;
}
	
/****************************************************************************************************
* Function Name: CIndexCollection::getNumberofIndexes
* Description:
* Return value: int
****************************************************************************************************/
int CIndexCollection::getNumberofIndexes()
{
	return m_IndexCount;
}


