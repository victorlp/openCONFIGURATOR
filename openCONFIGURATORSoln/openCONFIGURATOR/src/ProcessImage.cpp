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

#include "../Include/openCONFIGURATOR.h"
#include "../Include/Internal.h"
#include "../Include/Exception.h"
#include "../Include/ProcessImage.h"
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <errno.h>
#include <string.h>


void CalculateOffsets(int VarCount,  EPIDirectionType type)
{
	int arrOfOffsets[6][2] = {{-1, -1}, {-1, -1},{-1, -1},{-1,-1},{-1, -1}, {-1, -1}};			/* Contain last offsets of Int8,Uint8,Int16, UInt16, Int32,UInt32*/	
	for(int i=0; i<VarCount; i++)
	{

		ProcessImage* pi;
		if(type==INPUT)
		pi = &PIInCol[i];
		else if(type == OUTPUT)
		pi = &PIOutCol[i];	
		int dataTypeSize;
		int Offset;
		int BitOffset ;
		char* PIByteOffset = new char[7];
		char* PIBitOffset		= new char[5];
	
		/*if(strcmp(pi.DataType == "USINT"))*/
	
		
		if(strcmp(pi->DataType,"BITSTRING")==0)
		{
			if(arrOfOffsets[1][1] == 7)
			{
				Offset = arrOfOffsets[1][0] + 1;
				BitOffset = 0;				
				
			}
			else
			{
				if(arrOfOffsets[1][1] ==-1)
				Offset  = arrOfOffsets[1][0] + 1;
				else
				Offset  = arrOfOffsets[1][0];
				BitOffset = arrOfOffsets[1][1] + 1;				
			}		
			arrOfOffsets[1][1] = BitOffset;
			PIBitOffset = itoa(BitOffset, PIBitOffset, 16);
			pi->BitOffset = (char*)malloc(2);
			strcpy(pi->BitOffset, ConvertToHexformat(PIBitOffset, 2, 1));
			//printf("\nName: %s", pi->Name);
			//printf("\nBit Offset: %s", pi->BitOffset);
		}
			
			else	Offset = arrOfOffsets[1][0] + atoi(pi->DataSize)/8;
			PIByteOffset = itoa(Offset, PIByteOffset, 16);
			pi->ByteOffset = (char*)malloc(6);
			strcpy(pi->ByteOffset, ConvertToHexformat(PIByteOffset, 4, 1));
			//printf("\nName: %s", pi->Name);
			//printf("\nByte Offset: %s", pi->ByteOffset);
			arrOfOffsets[1][0] = Offset;		
	}
}
int TotalPIVarsCount()
{
	CNodeCollection* objNodeCol;
	objNodeCol =  CNodeCollection::getNodeColObjectPointer();
	int PIVarsCount =0;
	for(int i=0; i< objNodeCol->getCNNodesCount(); i++)
	{
		CNode objNode;
		objNode = objNodeCol->getNodebyCollectionIndex(i);	
		PIVarsCount = objNode.ProcessImageCollection.Count() + PIVarsCount;		
	}
	return PIVarsCount;
}
void GroupInOutPIVariables()
{

	int count =  TotalPIVarsCount();
	//PIInCol = (ProcessImage*)malloc(1*sizeof(ProcessImage));
	//PIOutCol = (ProcessImage*)malloc(1*sizeof(ProcessImage));
	
	CNodeCollection* objNodeCol;
	objNodeCol =  CNodeCollection::getNodeColObjectPointer();
	int PIVarsCount =0;
	for(int i=0; i< objNodeCol->getNumberOfNodes(); i++)
	{
		CNode objNode;
		objNode = objNodeCol->getNodebyCollectionIndex(i);	
		
		for(int i=0; i<objNode.ProcessImageCollection.Count(); i++)
		{
			
			if(objNode.ProcessImageCollection[i].DirectionType == INPUT)
			{
				InVars++;
				PIInCol[InVars]  = objNode.ProcessImageCollection[i];
				//PIInCol = (ProcessImage*)realloc(PIInCol,(size + 1)*sizeof(ProcessImage));			
			}
			else if(objNode.ProcessImageCollection[i].DirectionType == OUTPUT)
			{
				OutVars++;
				PIOutCol[OutVars]  = objNode.ProcessImageCollection[i];
				//PIOutCol = (PIOutCol *)realloc(PIOutCol,(size + 1)*sizeof(ProcessImage));
			}
		}
}
}