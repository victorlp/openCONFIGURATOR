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
#include "../Include/IndexCollection.h"
#include "../Include/Internal.h"
#include <stdio.h>

CIndexCollection::CIndexCollection(void)
	{
		//IndexList.size =0;
		m_IndexCount = 0;
	}

CIndexCollection::~CIndexCollection(void)
	{
	}
void CIndexCollection::addIndex(CIndex objIndex)
	{
		
		int i = collectionObj.Add();
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
		collectionObj[i] = objIndex;
		m_IndexCount= collectionObj.Count();

	}
void CIndexCollection::deleteIndex(int IndexID)
	{
	   collectionObj.Remove(IndexID);
	   m_IndexCount = collectionObj.Count();
	   //printf("\n\nAfter Delete - Count:%d\n\n", m_IndexCount);

	}
	
void CIndexCollection::DeleteIndexCollection() 
	{
		collectionObj.Clear();
		m_IndexCount = collectionObj.Count();
	}
	void CIndexCollection::DeletePDOs() 
	{
		int i;
		char* substr= new char[3];
		
		for(i =0; i<m_IndexCount;i++)
		{
			CIndex objIndex;
			objIndex = collectionObj[i];
			substr = subString((char*)objIndex.getIndexValue(), 0, 2);
			if(strcmp(substr, "1A")==0 || strcmp(substr, "1a")==0 ||
					strcmp(substr, "14")==0 || strcmp(substr, "16")==0   ||
					strcmp(substr, "18")==0 )
			{
			/*if(!(CheckIfNotPDO((char*)objIndex.getIndexValue())));
			{*/
				printf("\n objIndex.getIndexValue() : %s", objIndex.getIndexValue());
				collectionObj.Remove(i);
	   m_IndexCount = collectionObj.Count();
	   i = 0;
			}
			
		}		
	}
	void CIndexCollection::DeletePIObjects()
	{
		int i;
		char* substr= new char[2];
		
		for(i =0; i<m_IndexCount;i++)
		{
			CIndex objIndex;
			objIndex = collectionObj[i];
				substr = subString((char*)objIndex.getIndexValue(), 0, 1);
			if(strcmp(substr,"A")==0  ||  strcmp(substr,"a")==0 )
			{
				collectionObj.Remove(i);
	   m_IndexCount = collectionObj.Count();
	   i= 0;
			}
			
		}		
	}
CIndex* CIndexCollection::getIndex(int Count)
	{
		return &collectionObj[Count];
	}
CIndex* CIndexCollection::getIndexbyIndexValue(char* Index)
	{
		int i;
		
		for(i =0; i<m_IndexCount;i++)
		{
			CIndex objIndex;
			objIndex = collectionObj[i];
			if(strcmp(objIndex.getIndexValue(),Index)==0)
				return &collectionObj[i];
		}		
		return NULL;
	}
int CIndexCollection::getNumberofIndexes()
	{return m_IndexCount;}


