///////////////////////////////////////////////////////////////////////////////////////////////
//
//  $Header: $
//
// NAME:  ProcessImage.cpp
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
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include "../Include/openCONFIGURATOR.h"
#include "../Include/Internal.h"
#include "../Include/Exception.h"
#include "../Include/ProcessImage.h"

/****************************************************************************************************
* Defines
****************************************************************************************************/
#define HEADER_FILE_BUFFER 5000000
#define TOTAL_MODULES 10000

/****************************************************************************************************
* Global Variables
****************************************************************************************************/
INT32 iInVars =0;
INT32 iOutVars = 0;

static tADDRESSTABLE AddressTable[NO_OF_PI_ENTERIES] = {
														{"A000", INTEGER8,		INPUT},
														{"A040", UNSIGNED8,			INPUT},
														{"A0C0", INTEGER16, INPUT},
														{"A100", UNSIGNED16,		INPUT},
														{"A1C0", INTEGER32, INPUT},
														{"A200", UNSIGNED32,		INPUT},
														{"A480", INTEGER8,		OUTPUT},
														{"A4C0", UNSIGNED8,			OUTPUT},
														{"A540", INTEGER16, OUTPUT},
														{"A580", UNSIGNED16,		OUTPUT},
														{"A640", INTEGER32, OUTPUT},
														{"A680", UNSIGNED32,			OUTPUT},
																												
													   };
ModuleCol astModuleInfo[TOTAL_MODULES]; 

/****************************************************************************************************
* FUNCTION DEFINITIONS
***************************************************************************************************/

/****************************************************************************************************
* Function Name: SetPIOffsets
* Description:
* Return value: void
****************************************************************************************************/
void SetPIOffsets(ProcessImage* pobjProcessImage, INT32& iStartByteOffset, INT32 iPosition, INT32& iStartBitOffset  )
{
	if(pobjProcessImage->DataInfo.DataSize == 1 )
	{
		if(iStartBitOffset  == -1)
		{		
			//StartByteOffset = StartByteOffset + 1;
			pobjProcessImage->ByteOffset = iStartByteOffset;		
		}
		iStartBitOffset				 	= iStartBitOffset + 1;
		pobjProcessImage->BitOffset 	= iStartBitOffset;
		pobjProcessImage->ByteOffset 	= iStartByteOffset;
		
		if(iStartBitOffset == 7)
		{
			iStartByteOffset = iStartByteOffset + 1;
		}	
	}
	else
	{		
		//StartByteOffset = StartByteOffset + iPosition*(pobjProcessImage->DataInfo.DataSize);
		pobjProcessImage->ByteOffset 	= iStartByteOffset;
		iStartByteOffset 				=  iStartByteOffset + (pobjProcessImage->DataInfo.DataSize) / 8;
	}	
}
//void CalculateOffsets(INT32 VarCount,  EPIDirectionType type)
//{
//	INT32 arrOfOffsets[6][2] = {{-1, -1}, {-1, -1},{-1, -1},{-1,-1},{-1, -1}, {-1, -1}};			/* Contain last offsets of size 1Bytes, 2 bytes, 4 bytes and 8 bytes*/	
//	for(INT32 i=0; i<VarCount; i++)
//	{
//
//		ProcessImage* pi;
//		if(type==INPUT)
//		pi = &PIInCol[i];
//		else if(type == OUTPUT)
//		pi = &PIOutCol[i];	
//		INT32 Offset;
//		INT32 BitOffset ;
//		char* PIByteOffset = new char[7];
//		char* PIBitOffset		= new char[5];
//	
//		/*if(strcmp(pi.DataType == "USINT"))*/
//	
//		
//		if(pi->DataInfo._dt_enum ==BITSTRING)
//		{
//			if(arrOfOffsets[1][1] == 7)
//			{
//				Offset = arrOfOffsets[1][0] + 1;
//				BitOffset = 0;				
//				
//			}
//			else
//			{
//				if(arrOfOffsets[1][1] ==-1)
//				Offset  = arrOfOffsets[1][0] + 1;
//				else
//				Offset  = arrOfOffsets[1][0];
//				BitOffset = arrOfOffsets[1][1] + 1;				
//			}		
//			arrOfOffsets[1][1] = BitOffset;
//			pi->BitOffset =  BitOffset;
//			//printf("\nName: %s", pi->Name);
//			//printf("\nBit Offset: %s", pi->BitOffset);
//		}
//			
//			else	Offset = arrOfOffsets[1][0] + (pi->DataInfo.DataSize)/8;
//			pi->ByteOffset = Offset;
//						//printf("\nName: %s", pi->Name);
//			//printf("\nByte Offset: %s", pi->ByteOffset);
//			arrOfOffsets[1][0] = Offset;		
//	}
//}
//INT32 TotalPIVarsCount()
//{
//	CNodeCollection* objNodeCol;
//	objNodeCol =  CNodeCollection::getNodeColObjectPointer();
//	INT32 PIVarsCount =0;
//	for(INT32 i=0; i< objNodeCol->getCNNodesCount(); i++)
//	{
//		CNode objNode;
//		objNode = objNodeCol->getNodebyCollectionIndex(i);	
//		PIVarsCount = objNode.ProcessImageCollection.Count() + PIVarsCount;		
//	}
//	return PIVarsCount;
//}

/****************************************************************************************************
* Function Name: GroupInOutPIVariables
* Description:
* Return value: void
****************************************************************************************************/
void GroupInOutPIVariables(ProcessImage aobjPIInCol[], ProcessImage aobjPIOutCol[])
{
	//INT32 count =  TotalPIVarsCount();

	//aobjPIInCol = (ProcessImage*)malloc(1*sizeof(ProcessImage));
	//aobjPIOutCol = (ProcessImage*)malloc(1*sizeof(ProcessImage));
	
	CNodeCollection* pobjNodeCol = NULL;
	CNode*  objNode;
	pobjNodeCol =  CNodeCollection::getNodeColObjectPointer();
	//INT32 PIVarsCount =0;
	iInVars = 0;
	iOutVars = 0;
	
	for(INT32 iOutLoopCount=0; iOutLoopCount < pobjNodeCol->getNumberOfNodes(); iOutLoopCount++)
	{
		objNode = pobjNodeCol->getNodebyColIndex(iOutLoopCount);	
		
		for(INT32 iInLoopCount=0; iInLoopCount < objNode->ProcessImageCollection.Count(); iInLoopCount++)
		{			
			if(objNode->ProcessImageCollection[iInLoopCount].DirectionType == INPUT)
			{
				aobjPIInCol[iInVars]  = objNode->ProcessImageCollection[iInLoopCount];
				iInVars++;
				//aobjPIInCol = (ProcessImage*)realloc(aobjPIInCol,(size + 1)*sizeof(ProcessImage));			
			}
			else if(objNode->ProcessImageCollection[iInLoopCount].DirectionType == OUTPUT)
			{			
				aobjPIOutCol[iOutVars]  = objNode->ProcessImageCollection[iInLoopCount];
				iOutVars++;
				//aobjPIOutCol = (aobjPIOutCol *)realloc(aobjPIOutCol,(size + 1)*sizeof(ProcessImage));
			}
		}
	}
}

//void ComputeCompactPI(INT32 VarCount,  EPIDirectionType type)
//{
//	//INT32 arrOfOffsets[4][2] = {{-1, -1}, {-1, -1},{-1, -1},{-1,-1}};			/* Contain prev and current offsets of size 1Bytes, 2 bytes, 4 bytes and 8 bytes*/	
//	stOffsets size8Offet			= {0, 0};
//	stOffsets size16Offset = {0, 0};
//	stOffsets size32Offset = {0, 0};
//	stOffsets size64Offset = {0, 0};
//	for(INT32 i=0; i<VarCount; i++)
//	{
//
//		ProcessImage* pi;
//		ProcessImage piPrev;
//		INT32 Offset;
//		INT32 BitOffset ;
//		char* PIByteOffset = new char[7];
//		char* PIBitOffset		= new char[5];
//		
//		
//		if(type==INPUT)
//		{
//			pi = &PIInCol[i];
//			piPrev = PIInCol[i-1];
//		}
//		else if(type == OUTPUT)
//		{
//			pi = &PIOutCol[i];	
//			piPrev = PIOutCol[i-1];
//		}
//		
//		INT32 dataSize;
//		dataSize =  pi->DataInfo.DataSize;
//		
//		switch(dataSize)
//		{
//			/* Bitstring*/
//			case 1:
//				
//				if(piPrev.DataInfo._dt_enum  == BITSTRING)
//				{
//					if(piPrev.BitOffset == 7)
//					{
//						pi->BitOffset = 0;
//						pi->ByteOffset = piPrev.ByteOffset + 1;
//					}
//					else
//					{
//						pi->BitOffset = piPrev.BitOffset + 1;
//						pi->ByteOffset = piPrev.ByteOffset;
//					}
//				}
//				else
//				{
//						size8Offet.prevOffset = size8Offet.currOffset ;
//					 pi->ByteOffset = size8Offet.currOffset;											
//				}
//				
//			/*Unsigned8, Int8*/
//			case 8:
//						size8Offet.prevOffset = size8Offet.currOffset ;
//						pi->ByteOffset = size8Offet.currOffset ;
//						size8Offet.currOffset =	size8Offet.currOffset + 1;
//						
//						/* Set other DataType Offsets*/
//						/* if greater no change*/
//						if(size16Offset.currOffset >= size8Offet.currOffset)
//						else
//						{
//								size16Offet.prevOffset = size16Offet.currOffset ;
//								size16Offset.currOffset = size16Offset.currOffset + 2;
//						}
//						
//						/* if greater no change*/
//						if(size32.currOffset >= size8.currOffset)
//						else
//						{
//								size32.prevOffset = size32.currOffset ;
//								size32.currOffset = size32.currOffset + 4;
//						}
//						
//						/* if greater no change*/
//						if(size64Offset.currOffset >= size8Offet.currOffset)
//						else
//						{
//								size64Offset.prevOffset = size64Offset.currOffset ;
//								size64Offset.currOffset = size64Offset.currOffset + 8;
//						}
//						
//			/*Unsigned16, Int16*/
//			case 16:
//						size16Offet.prevOffset = size16Offet.currOffset ;
//						pi->ByteOffset = size16Offet.currOffset ;
//						size8Offet.currOffset =	size8Offet.currOffset + 2;
//					
//			
//							/* Set other DataType Offsets*/
//							
//							/* if greater no change*/
//						if(size8Offset.currOffset >= size16Offet.currOffset)
//						else
//						{
//								if((size8Offet.currOffset >= size16Offset.prevOffset) || (size8Offet.currOffset <= size16Offset.currOffset)
//								{
//									size8Offet.prevOffset = size8Offet.currOffset ;
//									size8Offset.currOffset = size16Offset.currOffset;
//								}
//								else
//								{
//									size8Offet.prevOffset = size8Offet.currOffset;
//									size8Offet.currOffset = size8Offet.currOffset + 1;
//								}
//						}
//						
//						/* if greater no change*/
//						if(size32Offset.currOffset >= size16Offet.currOffset)
//						else
//						{
//								size32Offset.prevOffset = size32Offset.currOffset ;
//								size32Offset.currOffset = size32Offset.currOffset + 4;
//						}
//						
//						/* if greater no change*/
//						if(size64.currOffset >= size16.currOffset)
//						else
//						{
//								size64Offset.prevOffset = size64Offset.currOffset ;
//								size64Offset.currOffset = size64Offset.currOffset + 8;
//						}						
//					
//			/*Unsigned32, Int32*/
//			case 32:
//						size32Offet.prevOffset = size32Offet.currOffset ;
//						pi->ByteOffset = size32Offet.currOffset ;
//						size32Offset.currOffset = size32Offset.currOffset + 4;
//			
//							/* Set other DataType Offsets*/
//								/* if greater no change*/
//						if(size8Offset.currOffset >= size32Offet.currOffset)
//						else
//						{
//								if((size8Offet.currOffset >= size32Offset.prevOffset) || (size8Offet.currOffset <= size32Offset.currOffset)
//								{
//									size8Offet.prevOffset = size8Offet.currOffset ;
//									size8Offset.currOffset = size32Offset.currOffset;
//								}
//								else
//								{
//									size8Offet.prevOffset = size8Offet.currOffset;
//									size8Offet.currOffset = size8Offet.currOffset + 1;
//								}
//						}
//						
//							/* if greater no change*/
//						if(size16Offset.currOffset >= size32Offet.currOffset)
//						else
//						{
//								if((size16Offet.currOffset >= size32Offset.prevOffset) || (size16Offet.currOffset <= size32Offset.currOffset)
//								{
//									size16Offet.prevOffset = size16Offet.currOffset ;
//									size16Offset.currOffset = size16Offset.currOffset;
//								}
//								else
//								{
//									size16Offet.prevOffset = size16Offet.currOffset;
//									size16Offet.currOffset = size16Offet.currOffset + 2;
//								}
//						}
//						/* if greater no change*/
//						if(size64Offset.currOffset >= size8Offet.currOffset)
//						else
//						{
//								size64Offset.prevOffset = size64Offset.currOffset ;
//								size64Offset.currOffset = size64Offset.currOffset + 8;
//						}
//			/*Unsigned64, Int64*/
//			case 64:
//						size64Offet.prevOffset = size64Offet.currOffset ;
//						pi->ByteOffset = size64Offet.currOffset ;
//			
//			break;
//		}
//	
//		}
//	}

/****************************************************************************************************
* Function Name: getIECDT
* Description:
* Return value: PIDataInfo*
****************************************************************************************************/
PIDataInfo* getIECDT(char* pbDataStr, INT32 iDataSize)
{
	PIDataInfo *pstDataInfo = new PIDataInfo;
	pstDataInfo->_dt_Name = new char[strlen(pbDataStr)+1];
	strcpy(pstDataInfo->_dt_Name, pbDataStr);
	
	if(pbDataStr != NULL)
	{
		if(!strcmp(pbDataStr, "BITSTRING"))
		{
			pstDataInfo->DataSize = iDataSize;
			pstDataInfo->_dt_enum = BITSTRING;
		}
		else if(!strcmp(pbDataStr, "BOOL"))
		{
			pstDataInfo->DataSize = iDataSize;
			pstDataInfo->_dt_enum = BOOL;
		}
		else if(!strcmp(pbDataStr, "_CHAR"))
		{
			pstDataInfo->DataSize = iDataSize;
			pstDataInfo->_dt_enum = _CHAR;
		}
		else if(!strcmp(pbDataStr, "WORD"))
		{
			pstDataInfo->DataSize = iDataSize;	
			pstDataInfo->_dt_enum = BYTE;
		}		
		else if(!strcmp(pbDataStr, "DWORD"))
		{
			pstDataInfo->DataSize = iDataSize;
			pstDataInfo->_dt_enum = DWORD;
		}	
		else if(!strcmp(pbDataStr, "LWORD"))
		{
			pstDataInfo->DataSize = iDataSize;
			pstDataInfo->_dt_enum = LWORD;
		}		
		else if(!strcmp(pbDataStr, "SINT"))
		{
			pstDataInfo->DataSize = iDataSize;
			pstDataInfo->_dt_enum = SINT;
		}	
		else if(!strcmp(pbDataStr, "INT"))
		{
			pstDataInfo->DataSize = iDataSize;
			pstDataInfo->_dt_enum = INT;
		}	
		else if(!strcmp(pbDataStr, "DINT"))
		{
			pstDataInfo->DataSize = iDataSize;
			pstDataInfo->_dt_enum = DINT;
		}		
		else if(!strcmp(pbDataStr, "LINT"))
		{
			pstDataInfo->DataSize = iDataSize;
			pstDataInfo->_dt_enum = LINT;
		}		
		else if(!strcmp(pbDataStr, "USINT"))
		{
			pstDataInfo->DataSize = iDataSize;
			pstDataInfo->_dt_enum = USINT;
		}		
		else if(!strcmp(pbDataStr, "UINT"))
		{
			pstDataInfo->DataSize = iDataSize;
			pstDataInfo->_dt_enum = UINT;
		}	
		else if(strcmp(pbDataStr, "UDINT")==0)
		{
			pstDataInfo->DataSize = iDataSize;
			pstDataInfo->_dt_enum = UDINT;
		}	
		else if(!strcmp(pbDataStr, "ULINT"))
		{
			pstDataInfo->DataSize = iDataSize;
			pstDataInfo->_dt_enum = ULINT;
		}		
		else if(!strcmp(pbDataStr, "REAL"))
		{
			pstDataInfo->DataSize = iDataSize;
			pstDataInfo->_dt_enum = REAL;
		}	
		else if(!strcmp(pbDataStr, "LREAL"))
		{
			pstDataInfo->DataSize = iDataSize;
			pstDataInfo->_dt_enum = LREAL;
		}	
		else if(!strcmp(pbDataStr, "STRING"))
		{
			pstDataInfo->DataSize = iDataSize;
			pstDataInfo->_dt_enum = STRING;
		}		
		else if(!strcmp(pbDataStr, "WSTRING"))
		{
			pstDataInfo->DataSize = iDataSize;
			pstDataInfo->_dt_enum = WSTRING;
		}	
	}
	return pstDataInfo;
}

/****************************************************************************************************
* Function Name: CheckIfModuleExists
* Description:
* Return value: bool
****************************************************************************************************/
bool CheckIfModuleExists(char* pbModuleName, INT32 & ModuleNo, INT32 iNoOfModules, ModuleCol astModCol[])
{
	for(INT32 iLoopCount = 0; iLoopCount <= iNoOfModules; iLoopCount++)
	{
		if(!strcmp(pbModuleName, astModuleInfo[iLoopCount].ModuleName))
		{
			ModuleNo = astModCol[iLoopCount].ModuleNo;
			return true;
		}		
	}
	return false;
}

/****************************************************************************************************
* Function Name: GenerateXAPHeaderFile
* Description:
* Return value: void
****************************************************************************************************/
void GenerateXAPHeaderFile(char* pbFileName, ProcessImage objPIInCol[], ProcessImage objPIOutCol[], INT32 iInVar, INT32 iOutVar)
{
	char* pbXapFileName  = new char[strlen(pbFileName) + ALLOC_BUFFER];
	FILE* fpXapFile 	 = new FILE();

	strcpy(pbXapFileName, pbFileName);
	strcat(pbXapFileName, ".h");
	
	/* write Input structure */
	if (( fpXapFile = fopen(pbXapFileName,"w+")) == NULL)
	{
		ocfmException ex;
		ex.ocfm_Excpetion(OCFM_ERR_CANNOT_OPEN_FILE);
		delete [] pbXapFileName;
		throw ex;
	}			
	if(iInVar !=0)
	{			
		WriteXAPHeaderContents(objPIInCol, iInVar, INPUT, fpXapFile);			
	}

	fclose(fpXapFile);
	
	/* write Output structure */
	if(iOutVar !=0)
	{
		if (( fpXapFile = fopen(pbXapFileName,"a+")) == NULL)
		{
			ocfmException ex;
			ex.ocfm_Excpetion(OCFM_ERR_CANNOT_OPEN_FILE);
			delete [] pbXapFileName;
			throw ex;
		}
		WriteXAPHeaderContents(objPIOutCol, iOutVar, OUTPUT, fpXapFile );			
		fclose(fpXapFile); 
	}
	delete [] pbXapFileName;
}

/****************************************************************************************************
* Function Name: WriteXAPHeaderContents
* Description:
* Return value: void
****************************************************************************************************/
void WriteXAPHeaderContents(ProcessImage objProcessImage[], INT32 iNumberOfVars, EPIDirectionType enumDirType, FILE* fpXapHeader)
{
	char* pbBuffer 		= new char[HEADER_FILE_BUFFER];
	char* pbBuffer1 	= new char[200];		
	//char* strCNID = new char[NODE_ID + ALLOC_BUFFER];
	INT32 iModuleNo 		= 0;
	//INT32 LastModuleNo = 0;
	INT32 iTotalsize 		= 0;
	INT32 iDataSize  		= 0;
	INT32 iHoleFilledIdNo 	= 1;									


	if(iNumberOfVars != 0 )
	{
		strcpy(pbBuffer, "\ntypedef struct { \n");	
		iModuleNo = 0;
		for(INT32 iLoopCount = 0; iLoopCount<iNumberOfVars ; iLoopCount++)
		{					
			INT32 iNodeId;
			char* pbStrCNID 	= new char[50];
			char* pbModName		= new char[50];
			char* pbStrModuleNo = new char[20];
			//char* varNo = new char[10];				
			
			iDataSize  = objProcessImage[iLoopCount].DataInfo.DataSize;
			/* Check if 8, 16, 32 bit aligned*/
			if((iDataSize % 32 == 0 ) || 
			(iDataSize % 16 == 0 ) ||
			(iDataSize % 8 == 0 ))
			{				
				if(iTotalsize % iDataSize !=0)
				{
					char abHoleid[20];
					char* pbFbits  = new char[2 + ALLOC_BUFFER];
					INT32 iFilledBits = iDataSize - (iTotalsize % iDataSize);
					
					iTotalsize =  iTotalsize + iFilledBits;
					strcat(pbBuffer,"unsigned");
					strcat(pbBuffer," ");
					strcat(pbBuffer,"PADDING_VAR_");		
					strcat(pbBuffer,_IntToAscii(iHoleFilledIdNo, abHoleid, 10));												
					strcat(pbBuffer,":");
					pbFbits =  _IntToAscii(iFilledBits, pbFbits, 10);							
					strcat(pbBuffer,pbFbits);
					strcat(pbBuffer,";\n");
					iHoleFilledIdNo =  iHoleFilledIdNo + 1;
					delete[] pbFbits;
				}				
			}
			
			iNodeId = objProcessImage[iLoopCount].CNNodeID;
		
			pbStrCNID = _IntToAscii(objProcessImage[iLoopCount].CNNodeID, pbStrCNID, 10); 
			strcpy(pbStrModuleNo, subString( objProcessImage[iLoopCount].ModuleIndex, 2, 2));
			strcpy(pbModName, objProcessImage[iLoopCount].ModuleName);
				
			//printf("\n Module Name: %s",pbModName);
			strcat(pbBuffer,"unsigned");
			strcat(pbBuffer," ");
			char* pbVarName = new char[100];
			strcpy(pbVarName, "CN");
			pbStrCNID = _IntToAscii(objProcessImage[iLoopCount].CNNodeID, pbStrCNID, 10); 
			strcat(pbVarName, pbStrCNID);
			strcat(pbVarName, "_");
			
			/* Add Mod NO*/
			strcat(pbVarName, "M");
			strcat(pbVarName, pbStrModuleNo);
			strcat(pbVarName, "_");
			
			strcat(pbVarName, pbModName);
			strcat(pbVarName, "_");
			strcat(pbVarName, objProcessImage[iLoopCount].VarName);
				
			//if(strcmp(subString(objProcessImage[iLoopCount].VarName,0, 8), "Reserved")== 0)
			//{
			//	varNo =  _IntToAscii(iLoopCount, varNo, 10);
			//	strcat(pbVarName, "_");
			//	strcat(pbVarName, varNo);
			//}
			 
			strcat(pbBuffer, pbVarName);									
			strcat(pbBuffer, ":");
			
			char* pbBuff 	= new char[50];												
			pbBuff 			= _IntToAscii(iDataSize, pbBuff, 10); 
			iTotalsize 		= iDataSize  + iTotalsize;
			strcat(pbBuffer, pbBuff);
			strcat(pbBuffer, ";");
			
			strcat(pbBuffer, "\n");
			delete[] pbVarName;				
			delete[] pbModName;
			delete[] pbStrModuleNo;
			delete[] pbStrCNID;									
		}
		/* Check if the whole struct is 32 bit aligned*/
		if(iTotalsize % 32 !=0)
		{
			char abHoleid[20];
			char* pbFbits  = new char[2 + ALLOC_BUFFER];
			INT32 iFilledBits = 32 - (iTotalsize % 32);
			
			iTotalsize =  iTotalsize + iFilledBits;
			strcat(pbBuffer,"unsigned");
			strcat(pbBuffer," ");
			strcat(pbBuffer,"PADDING_VAR_");		
			strcat(pbBuffer,_IntToAscii(iHoleFilledIdNo, abHoleid, 10));												
			strcat(pbBuffer,":");
			pbFbits =  _IntToAscii(iFilledBits, pbFbits, 10);							
			strcat(pbBuffer,pbFbits);
			strcat(pbBuffer,";\n");
			iHoleFilledIdNo =  iHoleFilledIdNo + 1;
			delete[] pbFbits;
		}
		strcat(pbBuffer, "}");
		
		if(enumDirType == INPUT)
		{
			strcpy(pbBuffer1, "# define COMPUTED_PI_IN_SIZE ");			
			strcat(pbBuffer, " PI_IN;");	
		}	
		else if(enumDirType == OUTPUT)
		{				
			
			strcpy(pbBuffer1, "\n\n# define COMPUTED_PI_OUT_SIZE ");		
			strcat(pbBuffer, " PI_OUT;");	
			strcat(pbBuffer, "\n");
		}
	}

	char* pbStrSize = new char[50];
	
	/* write the size in bytes*/
	iTotalsize 	=  iTotalsize / 8;	
	pbStrSize 	=  _IntToAscii(iTotalsize, pbStrSize, 10);
	strcat(pbBuffer1, pbStrSize);
	
	UINT32 uiStrLength =  strlen(pbBuffer1);
	
	if((uiStrLength != fwrite(pbBuffer1, sizeof(char),uiStrLength,fpXapHeader)))
	{	
		ocfmException ex;
		ex.ocfm_Excpetion(OCFM_ERR_FILE_CANNOT_OPEN);
		delete[] pbBuffer;
		delete[] pbBuffer1;
		throw ex;
	}

	uiStrLength  =  strlen(pbBuffer);

	if((uiStrLength != fwrite(pbBuffer, sizeof(char), uiStrLength, fpXapHeader)))
	{	
		ocfmException ex;
		ex.ocfm_Excpetion(OCFM_ERR_FILE_CANNOT_OPEN);
		throw ex;
	}
	
	delete[] pbBuffer;
	delete[] pbBuffer1;	
}

/****************************************************************************************************
* Function Name: SetSIdxDataType
* Description:
* Return value: void
****************************************************************************************************/
void SetSIdxDataType(DataType *pobjDataType, char* pbIdx, char* pbSIdx)
{
	CNodeCollection *pobjNodeCollection = NULL;
	CIndexCollection *pobjIndexCol      = NULL;
	CIndex *pobjIndex					= NULL;
	CSubIndex *pobjSIdx					= NULL;
	CNode objNode;
	
	pobjNodeCollection = CNodeCollection::getNodeColObjectPointer();				
			
	objNode = pobjNodeCollection->getMNNode();
	pobjIndexCol = objNode.getIndexCollection();
	
	pobjIndex = pobjIndexCol->getIndexbyIndexValue(pbIdx);
	pobjSIdx = pobjIndex->getSubIndexbyIndexValue(pbSIdx);
	
	pobjSIdx->setDataTypeST(*pobjDataType);	
}

/****************************************************************************************************
* Function Name: AddPDOIndexsToMN
* Description:
* Return value: void
****************************************************************************************************/
void AddPDOIndexsToMN(char* pbIndex, char* pbSubIndex, EPDOType enumPdoType)
{
	ocfmRetCode stRetCode;
	CNode objMNNode;
	DataType 			*pobjDataType 	= NULL;
	CIndex 				*pobjIndex	   	= NULL;
	CNodeCollection 	*pobjNodeCol 	= NULL;
	CIndexCollection 	*pobjIdxCol 	= NULL;
	CDataTypeCollection *pobjDTCol 		= NULL;
	CSubIndex 			*pobjSIdx 		= NULL;
	char				*pbName 		= NULL;
	char				*pbPdoMap 		= new char[4 + ALLOC_BUFFER];
	
	pobjNodeCol =  CNodeCollection::getNodeColObjectPointer();
	objMNNode 	= pobjNodeCol->getNode(MN, MN_NODEID);
	pobjDTCol 	= objMNNode.getDataTypeCollection();
		
	stRetCode 	= AddIndex(MN_NODEID, MN, pbIndex);	
	pobjIdxCol 	= objMNNode.getIndexCollection();
	if(pobjIdxCol != NULL)
	{
		char abObjectName[14 + ALLOC_BUFFER];
		pobjIndex = pobjIdxCol->getIndexbyIndexValue(pbIndex);
		if(pobjIndex != NULL && (stRetCode.code != OCFM_ERR_INDEX_ALREADY_EXISTS) )
		{
			pobjIndex->setObjectType((char*)"ARRAY");
			if(enumPdoType == PDO_RPDO)
			{			
				strcpy(abObjectName, "PI_OUTPUTS_A");
				strcat(abObjectName, getPIName(pbIndex));		
				pobjIndex->setName(abObjectName);
			}
			else if(enumPdoType == PDO_TPDO)
			{							
				strcpy(abObjectName, "PI_INPUTS_A");
				strcat(abObjectName, getPIName(pbIndex));	
				pobjIndex->setName(abObjectName);
			}
			else
			{
				cout << "enumPdoType is not TPDO or RPDO _1\n" << endl;
			}
			//$SpobjIndex->setName(pbObjectName);
			/* Add subindex 00 */
			stRetCode = AddSubIndex(MN_NODEID, MN, pbIndex, (char*) "00");
			if(stRetCode.code  ==  OCFM_ERR_SUCCESS)
			{
				//SetSubIndexAttributes(MN_NODEID, MN, Index, "00", NULL, "NumberOfEntries");
			}
		}
	}
	
	//printf("\n after Add Index before subIndex : %s", pbIndex);
	stRetCode = AddSubIndex(MN_NODEID, MN, pbIndex, pbSubIndex);
	if(pobjIndex != NULL)
	{
		pobjSIdx = pobjIndex->getSubIndexbyIndexValue(pbSubIndex);
		if(pobjSIdx != NULL)
		{
			char abObjectName[14 + ALLOC_BUFFER];
			pobjSIdx->setObjectType((char*)"VAR");			
			/* Its reversed because CN's RPDO is MN's TPDO*/			
			if(enumPdoType == PDO_RPDO)
			{								
				strcpy(pbPdoMap, "TPDO");
				strcpy(abObjectName, "PI_OUTPUTS_");
				strcat(abObjectName, getPIName(pbIndex));								
				pobjSIdx->setAccessType((char*)"wo");
				
				pobjSIdx->setName(abObjectName);
				pobjSIdx->setPDOMapping(pbPdoMap);
			}
			else if(enumPdoType == PDO_TPDO)
			{				
				strcpy(pbPdoMap, "RPDO");				
				strcpy(abObjectName, "PI_INPUTS_");
				strcat(abObjectName, getPIName(pbIndex));	
				pobjSIdx->setAccessType((char*)"ro");
				
				pobjSIdx->setName(abObjectName);
				pobjSIdx->setPDOMapping(pbPdoMap);
			}
			else
			{
				cout << "enumPdoType is not TPDO or RPDO _2\n" << endl;
			}
		}
	}
	pbName = getPIDataTypeName(pbIndex);	
	
	if(pbName != NULL)	
	{
		pobjDataType = pobjDTCol->getDataTypeByName(pbName);		
	}
	
	if((pobjDataType != NULL) && (pbIndex != NULL) && (pbSubIndex != NULL))
	{
		SetSIdxDataType(pobjDataType, pbIndex, pbSubIndex);
	}
	
	if(pobjIndex != NULL)
	{
		UpdateNumberOfEnteriesSIdx(pobjIndex, MN);
	}
	delete [] pbPdoMap;
 }
 
/****************************************************************************************************
* Function Name: getPIAddress
* Description:
* Return value: char*
****************************************************************************************************/
PIObject getPIAddress(PDODataType dt,  EPIDirectionType dirType, int iOffset, int iDataSize)
{
	int i;
	int subIndex;
	PIObject stPIObject;
	
	stPIObject.Index = new char[INDEX_LEN];
	stPIObject.SubIndex = new char[SUBINDEX_LEN];

	for(i = 0; i< NO_OF_PI_ENTERIES; i++)
	{
		if((AddressTable[i].dt == dt) && (AddressTable[i].Direction == dirType))
		{
			INT32 iTempDataSize = iDataSize/8;
			subIndex = (iOffset)/ iTempDataSize + 1;
				//printf("\n subindex %d", subIndex);
			if(subIndex > 254)
			{
				int div = subIndex / 254;
				int mod = subIndex % 254;
				int iAddress;
				iAddress = atoi(AddressTable[i].Address);
				iAddress = iAddress + div;
				stPIObject.Index  = _IntToAscii(iAddress, stPIObject.Index, 16);								
				stPIObject.SubIndex =  _IntToAscii(mod, 	stPIObject.SubIndex, 16);
				stPIObject.SubIndex = padLeft(	stPIObject.SubIndex, '0', 2);
				return stPIObject;
			}
			else
			{			
				strcpy(stPIObject.Index, AddressTable[i].Address);
				stPIObject.SubIndex =  _IntToAscii(subIndex, 	stPIObject.SubIndex, 16);
				stPIObject.SubIndex = padLeft(	stPIObject.SubIndex, '0', 2);
				return stPIObject;			
			}			
		}		
	}
	//Handled error case and returned dummy value to avoid warning
	//cout << "Error in returning getPIAddress" << endl;
	//return (char*) "Error";
}

/****************************************************************************************************
* Function Name: getPIDataTypeName
* Description:
* Return value: char*
****************************************************************************************************/
char* getPIDataTypeName(char* pbAddress)
{
	char *pbRetString = NULL;
	
	for(INT32 iLoopCount = 0; iLoopCount < NO_OF_PI_ENTERIES; iLoopCount++)
	{		
		if(strcmp(AddressTable[iLoopCount].Address,  pbAddress) ==0)
		{
			switch(AddressTable[iLoopCount].dt)
			{
				case UNSIGNED8 :
				{
					pbRetString = (char*) "Unsigned8";
					break;
				}
				case INTEGER8 :
				{
					pbRetString = (char*) "Integer8";
					break;
				}
				case UNSIGNED16 :
				{
					pbRetString = (char*) "Unsigned16";
					break;
				}
				case INTEGER16 :
				{
					pbRetString = (char*) "Integer8";
					break;
				}
				case UNSIGNED32 :
				{
					pbRetString = (char*) "Unsigned32";
					break;
				}
				case INTEGER32 :
				{
					pbRetString = (char*) "Integer32";
					break;
				}
				default:
				{
					//Handled error case and returned dummy value to avoid warning
					//cout << "Error in returning getPIDataTypeName" << endl;
					pbRetString = (char*) "Error";
					break;
				}
			}
		}
	}
	return pbRetString;	
}

/****************************************************************************************************
* Function Name: getPIName
* Description:
* Return value: char*
****************************************************************************************************/
char* getPIName(char* pbAddress)
{
	char *pbRetString = NULL;
	
	for(INT32 iLoopCount = 0; iLoopCount < NO_OF_PI_ENTERIES; iLoopCount++)
	{		
		if(strcmp(AddressTable[iLoopCount].Address,  pbAddress) ==0)
		{
			switch(AddressTable[iLoopCount].dt)
			{
				case UNSIGNED8 :
				{
					pbRetString = (char*) "U8";
					break;
				}
				case INTEGER8 :
				{
					pbRetString = (char*) "I8";
					break;
				}
				case UNSIGNED16 :
				{
					pbRetString = (char*) "U16";
					break;
				}
				case INTEGER16 :
				{
					pbRetString = (char*) "I16";
					break;
				}
				case UNSIGNED32 :
				{
					pbRetString = (char*) "U32";
					break;
				}
				case INTEGER32 :
				{
					pbRetString = (char*) "I32";
					break;
				}
				default:
				{
					//Handled error case and returned dummy value to avoid warning
					//cout << "Error in returning getPIDataTypeName" << endl;
					pbRetString = (char*) "Err";
					break;
				}
			}
		}
	}
	
	return pbRetString;
}

/****************************************************************************************************
* Function Name: CheckIfProcessImageIdx
* Description:
* Return value: bool
****************************************************************************************************/
bool CheckIfProcessImageIdx(char* pbIndex)
{
	for(INT32 iLoopCount = 0; iLoopCount< NO_OF_PI_ENTERIES; iLoopCount++)
	{		
		if(!strcmp(AddressTable[iLoopCount].Address,  pbIndex))
		{
			return true;
		}
	}
	return false;
}