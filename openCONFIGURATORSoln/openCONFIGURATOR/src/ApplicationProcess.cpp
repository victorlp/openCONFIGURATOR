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
#include "../Include/ApplicationProcess.h"
#include <iostream>
#include <fstream>

CApplicationProcess::CApplicationProcess(void)
	{
	}

CApplicationProcess::~CApplicationProcess(void)
	{
	}
void CApplicationProcess::ParseXDDfile(char* filename)
	{
	}
int CApplicationProcess::checkFileStatus(char* filename) 
	{
		ifstream file (filename, ios::in|ios::binary|ios::ate);
		if (file.is_open()) return 1;
		else return 0;

	}
	void CApplicationProcess::addComplexDataType(CComplexDataType complexDT)
 {
	 	int i = CDTCollection.Add();
	 	complexDT.Index = i;
		CDTCollection[i] = complexDT;		
 }
void CApplicationProcess::addParameter(Parameter  parameter)
 {
	 	int i = ParameterCollection.Add();
	 	parameter.ParaIndex = i;
			ParameterCollection[i] = parameter;
			
 }
int CApplicationProcess::get_ParameterIndexby_UniqueIDRef(char *UniqueIdRef)
{

	for(int count = 0; count<ParameterCollection.Count(); count++)
	{
		Parameter objPara;
		objPara = ParameterCollection[count];
		if(strcmp(UniqueIdRef, objPara.name_id_dt_attr.getUniqueID())==0)
		return count;
	}
	return NULL;
}
Parameter* CApplicationProcess::get_Parameterby_UniqueIDRef(char *UniqueIdRef)
{

	for(int count = 0; count<ParameterCollection.Count(); count++)
	{
		Parameter objPara;
		objPara = ParameterCollection[count];
		if(strcmp(UniqueIdRef, objPara.name_id_dt_attr.getUniqueID())==0)
		return &ParameterCollection[count];
	}
	return NULL;
}
int CApplicationProcess::get_CDT_UniqueIDRef(char *UniqueIdRef)
{

	for(int count = 0; count<CDTCollection.Count(); count++)
	{
		CComplexDataType objCDT;
		objCDT = CDTCollection[count];
		if(strcmp(UniqueIdRef, objCDT.name_id_attr->getUniqueID())==0)
		return count;
	}
	return NULL;
}
CComplexDataType* CApplicationProcess::getCDTbyUniqueID(char *UniqueId)
{
		for(int count = 0; count<CDTCollection.Count(); count++)
	{
		CComplexDataType objCDT;
		objCDT = CDTCollection[count];
			//printf("\nUniqueID : %s",UniqueId);
		//printf("\n%s",objCDT.name_id_attr->getUniqueID());
		if(strcmp(UniqueId, objCDT.name_id_attr->getUniqueID())==0)
		return &CDTCollection[count];
	}
	return NULL;
}
CComplexDataType* CApplicationProcess::getCDTbydt_UniqueRefID(char *UniqueRefId)
{
		for(int count = 0; count<CDTCollection.Count(); count++)
	{
		CComplexDataType objCDT;
		objCDT = CDTCollection[count];
		if( objCDT.name_id_attr->getDtUniqueRefId()!= NULL)
		{
			if(strcmp(UniqueRefId, objCDT.name_id_attr->getDtUniqueRefId())==0)
			return &CDTCollection[count];
		}
	}
	return NULL;
}
CComplexDataType* CApplicationProcess::getCDTbydtIndex(int Index)
{
		return &CDTCollection[Index];
}
void CApplicationProcess::updatePreviousCDT_UId(char *UniqueID, int Index)
{
		CComplexDataType* objCDT;
		objCDT = CDTCollection.GetAddress(Index);
		objCDT->previousCDT_UId = (char*)malloc(strlen(UniqueID)+1);
		strcpy(objCDT->previousCDT_UId, UniqueID);
}

Parameter CApplicationProcess::get_UniqueIDRef_by_ParameterIndex(int ParameterIndex)
{
	Parameter objPara;
	objPara = ParameterCollection[ParameterIndex];
	return objPara;
}

int CApplicationProcess::getCDTCount()
{
	return CDTCollection.Count();
}

CComplexDataType* CApplicationProcess::getCDTbyCount(int count)
{
	//CComplexDataType objCDT;
	//objCDT = CDTCollection[count];
	return &CDTCollection[count];
}
