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

int InVars =0;
int OutVars = 0;
#define HEADER_FILE_BUFFER 500000
#define TOTAL_MODULES 10000

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


ModuleCol modCol[TOTAL_MODULES]; 


//extern ProcessImage PIInCol[4000] = {};
//extern ProcessImage PIOutCol[4000]= {};

void SetPIOffsets(ProcessImage* pi,int& StartByteOffset, int pos, int& StartBitOffset  )
{
	if(pi->DataInfo.DataSize == 1 )
	{
		if(StartBitOffset  == -1)
		{		
		//	StartByteOffset = StartByteOffset + 1;
			pi->ByteOffset = StartByteOffset;
		
		}
		StartBitOffset = StartBitOffset + 1;
		pi->BitOffset = StartBitOffset;
		pi->ByteOffset = StartByteOffset;
		if(StartBitOffset ==7)
		StartByteOffset = StartByteOffset + 1;
	
	}
	else
	{
		
		//StartByteOffset = StartByteOffset + pos*(pi->DataInfo.DataSize);
		pi->ByteOffset = StartByteOffset;
		StartByteOffset =  StartByteOffset + (pi->DataInfo.DataSize)/8;
	}	
}
//void CalculateOffsets(int VarCount,  EPIDirectionType type)
//{
//	int arrOfOffsets[6][2] = {{-1, -1}, {-1, -1},{-1, -1},{-1,-1},{-1, -1}, {-1, -1}};			/* Contain last offsets of size 1Bytes, 2 bytes, 4 bytes and 8 bytes*/	
//	for(int i=0; i<VarCount; i++)
//	{
//
//		ProcessImage* pi;
//		if(type==INPUT)
//		pi = &PIInCol[i];
//		else if(type == OUTPUT)
//		pi = &PIOutCol[i];	
//		int Offset;
//		int BitOffset ;
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
//int TotalPIVarsCount()
//{
//	CNodeCollection* objNodeCol;
//	objNodeCol =  CNodeCollection::getNodeColObjectPointer();
//	int PIVarsCount =0;
//	for(int i=0; i< objNodeCol->getCNNodesCount(); i++)
//	{
//		CNode objNode;
//		objNode = objNodeCol->getNodebyCollectionIndex(i);	
//		PIVarsCount = objNode.ProcessImageCollection.Count() + PIVarsCount;		
//	}
//	return PIVarsCount;
//}
void GroupInOutPIVariables(ProcessImage PIInCol[], ProcessImage PIOutCol[])
{

	//int count =  TotalPIVarsCount();

	//PIInCol = (ProcessImage*)malloc(1*sizeof(ProcessImage));
	//PIOutCol = (ProcessImage*)malloc(1*sizeof(ProcessImage));
	
	CNodeCollection* objNodeCol;
	objNodeCol =  CNodeCollection::getNodeColObjectPointer();
	//int PIVarsCount =0;
	InVars = 0;
	OutVars = 0;
	for(int i=0; i< objNodeCol->getNumberOfNodes(); i++)
	{
		CNode objNode;
		objNode = objNodeCol->getNodebyCollectionIndex(i);	
		
		for(int i=0; i<objNode.ProcessImageCollection.Count(); i++)
		{
			
			if(objNode.ProcessImageCollection[i].DirectionType == INPUT)
			{
				PIInCol[InVars]  = objNode.ProcessImageCollection[i];
				InVars++;
				//PIInCol = (ProcessImage*)realloc(PIInCol,(size + 1)*sizeof(ProcessImage));			
			}
			else if(objNode.ProcessImageCollection[i].DirectionType == OUTPUT)
			{			
				PIOutCol[OutVars]  = objNode.ProcessImageCollection[i];
					OutVars++;
				//PIOutCol = (PIOutCol *)realloc(PIOutCol,(size + 1)*sizeof(ProcessImage));
			}
		}
}
}
//void ComputeCompactPI(int VarCount,  EPIDirectionType type)
//{
//	//int arrOfOffsets[4][2] = {{-1, -1}, {-1, -1},{-1, -1},{-1,-1}};			/* Contain prev and current offsets of size 1Bytes, 2 bytes, 4 bytes and 8 bytes*/	
//	stOffsets size8Offet			= {0, 0};
//	stOffsets size16Offset = {0, 0};
//	stOffsets size32Offset = {0, 0};
//	stOffsets size64Offset = {0, 0};
//	for(int i=0; i<VarCount; i++)
//	{
//
//		ProcessImage* pi;
//		ProcessImage piPrev;
//		int Offset;
//		int BitOffset ;
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
//		int dataSize;
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
PIDataInfo* getIECDT(char* dtStr, int dataSize)
{
	PIDataInfo *di = new PIDataInfo;
	di->_dt_Name = new char[strlen(dtStr)+1];
	strcpy(di->_dt_Name, dtStr);
	
	if(dtStr!=NULL)
	{
		if(strcmp(dtStr, "BITSTRING")==0)
		{
			di->DataSize = dataSize;
			di->_dt_enum = BITSTRING;
		}
		else if(strcmp(dtStr, "BOOL")==0)
		{
			di->DataSize = dataSize;
			di->_dt_enum = BOOL;
		}
		else if(strcmp(dtStr, "_CHAR")==0)
		{
			di->DataSize = dataSize;
			di->_dt_enum = _CHAR;
		}
		else if(strcmp(dtStr, "WORD")==0)
		{
			di->DataSize = dataSize;	
			di->_dt_enum = BYTE;
		}		
		else if(strcmp(dtStr, "DWORD")==0)
			{
			di->DataSize = dataSize;
			di->_dt_enum = DWORD;
		}
	
		else if(strcmp(dtStr, "LWORD")==0)
			{
			di->DataSize = dataSize;
			di->_dt_enum = LWORD;
		}
		
		else if(strcmp(dtStr, "SINT")==0)
			{
			di->DataSize = dataSize;
			di->_dt_enum = SINT;
		}
	
		else if(strcmp(dtStr, "INT")==0)
		{
			di->DataSize = dataSize;
			di->_dt_enum = INT;
		}
	
		else if(strcmp(dtStr, "DINT")==0)
		{
			di->DataSize = dataSize;
			di->_dt_enum = DINT;
		}
		
		else if(strcmp(dtStr, "LINT")==0)
		{
			di->DataSize = dataSize;
			di->_dt_enum = LINT;
		}
		
		else if(strcmp(dtStr, "USINT")==0)
		{
			di->DataSize = dataSize;
			di->_dt_enum = USINT;
		}
		
		else if(strcmp(dtStr, "UINT")==0)
		{
			di->DataSize = dataSize;
			di->_dt_enum = UINT;
		}
	
		else if(strcmp(dtStr, "UDINT")==0)
		{
			di->DataSize = dataSize;
			di->_dt_enum = UDINT;
		}
	
		else if(strcmp(dtStr, "ULINT")==0)
		{
			di->DataSize = dataSize;
			di->_dt_enum = ULINT;
		}
		
		else if(strcmp(dtStr, "REAL")==0)
		{
			di->DataSize = dataSize;
			di->_dt_enum = REAL;
		}
	
		else if(strcmp(dtStr, "LREAL")==0)
		{
			di->DataSize = dataSize;
			di->_dt_enum = LREAL;
		}
	
		else if(strcmp(dtStr, "STRING")==0)
		{
			di->DataSize = dataSize;
			di->_dt_enum = STRING;
		}
		
		else if(strcmp(dtStr, "WSTRING")==0)
		{
			di->DataSize = dataSize;
			di->_dt_enum = WSTRING;
		}
	
	}
	return di;
}
bool CheckIfModuleExists(char* ModuleName, int & ModuleNo, int NoOfModules, ModuleCol modCol[])
{
	for(int i=0; i<= NoOfModules; i++)
	{
		if(strcmp(ModuleName, modCol[i].ModuleName) ==0)
		{
			ModuleNo = modCol[i].ModuleNo ;
			return true;
		}
			
	}
	return false;
}
void GenerateXAPHeaderFile(char* fileName, ProcessImage PI_IN[], ProcessImage PI_OUT[], int InVar, int OutVar)
{

		char* strFileName  = new char[strlen(fileName) + ALLOC_BUFFER];
		strcpy(strFileName, fileName);
		strcat(strFileName, ".h");
		FILE* fileptr = new FILE();
	
		/* write Input structure */
		if (( fileptr = fopen(strFileName,"w+")) == NULL)
		{
					ocfmException ex;
					ex.ocfm_Excpetion(OCFM_ERR_CANNOT_OPEN_FILE);
					delete [] strFileName;
					throw ex;						
		}			
		if(InVar !=0)
		{			
			WriteXAPHeaderContents(PI_IN, InVar, INPUT, fileptr);			
		}
	
		fclose(fileptr);
		
			/* write Output structure */
		if (( fileptr = fopen(strFileName,"a+")) == NULL)
		{
					ocfmException ex;
					ex.ocfm_Excpetion(OCFM_ERR_CANNOT_OPEN_FILE);
					delete [] strFileName;
					throw ex;						
		}
		if(OutVar !=0)
		{	
			WriteXAPHeaderContents(PI_OUT, OutVar, OUTPUT, fileptr );
		}		
		fclose(fileptr);
		delete [] strFileName;
}
void WriteXAPHeaderContents(ProcessImage PI[], int NumberOfVars, EPIDirectionType dirType, FILE* fileptr)
{


		char* Buffer = new char[HEADER_FILE_BUFFER];
		char* Buffer1 = new char[200];
			
		
		//char* strCNID = new char[NODE_ID + ALLOC_BUFFER];
		int ModuleNo = 0;
		//int LastModuleNo = 0;
		
		int totalsize = 0;
		int DataSize  = 0;
		int holeFilledIdNo = 1;									
		
		
		if(NumberOfVars!=0 )
			{
				strcpy(Buffer, "\ntypedef struct { \n");	
				ModuleNo = 0;
				for(int i = 0; i<NumberOfVars ; i++)
				{					
					int NodeId;
					char* strCNID = new char[50];
					char* ModName =  new char[50];
					char* strModuleNo = new char[20];
					//char* varNo = new char[10];
					
					
					DataSize  = PI[i].DataInfo.DataSize;
						/* Check if 8, 16, 32 bit aligned*/
					if((DataSize % 32 == 0 ) || 
								(DataSize % 16 == 0 ) ||
								(DataSize % 8 == 0 ))
					{
						
						if(totalsize % DataSize !=0)
						{
							char holeid[20];
							int filledBits = DataSize - (totalsize % DataSize);
							totalsize =  totalsize + filledBits;
							strcat(Buffer,"unsigned");
							strcat(Buffer," ");
							strcat(Buffer,"PADDING_VAR_");		
							strcat(Buffer,_IntToAscii(holeFilledIdNo, holeid, 10));												
							strcat(Buffer,":");
							char* fbits  = new char[2 + ALLOC_BUFFER];
							fbits =  _IntToAscii(filledBits, fbits, 10);							
							strcat(Buffer,fbits);
							strcat(Buffer,";\n");
							holeFilledIdNo =  holeFilledIdNo + 1;
							delete[] fbits;
						}
						
					}
					
					NodeId = PI[i].CNNodeID;
				
					strCNID = _IntToAscii(PI[i].CNNodeID, strCNID, 10); 
					strcpy(strModuleNo, subString( PI[i].ModuleIndex,2, 2));
					strcpy(ModName, PI[i].ModuleName);
					
			
					//printf("\n Module Name: %s",ModName);
					strcat(Buffer,"unsigned");
					strcat(Buffer," ");
					char* varName = new char[100];
					strcpy(varName, "CN");
					strCNID = _IntToAscii(PI[i].CNNodeID, strCNID, 10); 
					strcat(varName, strCNID);
					strcat(varName, "_");
					
					/* Add Mod NO*/
					strcat(varName, "M");
					strcat(varName, strModuleNo);
					strcat(varName, "_");
					
					strcat(varName, ModName);
					strcat(varName, "_");
					strcat(varName, PI[i].VarName);
						
			/*		if(strcmp(subString(PI[i].VarName,0, 8), "Reserved")== 0)
					{
						varNo =  _IntToAscii(i, varNo, 10);
						strcat(varName, "_");
						strcat(varName, varNo);
					}*/
					 
					strcat(Buffer, varName);									
					strcat(Buffer, ":");
					
					char* str = new char[50];												
					str = _IntToAscii(DataSize, str, 10); 
					totalsize = DataSize  + totalsize;
					strcat(Buffer, str);
					strcat(Buffer, ";");
					
					strcat(Buffer, "\n");
					delete[] varName;				
					delete[] ModName;
					delete[] strModuleNo;
					delete[] strCNID;	
										
			}
			/* Check if the whole struct is 32 bit aligned*/
			if(totalsize % 32 !=0)
			{
					char holeid[20];
					int filledBits = 32 - (totalsize % 32);
					totalsize =  totalsize + filledBits;
					strcat(Buffer,"unsigned");
					strcat(Buffer," ");
					strcat(Buffer,"PADDING_VAR_");		
					strcat(Buffer,_IntToAscii(holeFilledIdNo, holeid, 10));												
					strcat(Buffer,":");
					char* fbits  = new char[2 + ALLOC_BUFFER];
					fbits =  _IntToAscii(filledBits, fbits, 10);							
					strcat(Buffer,fbits);
					strcat(Buffer,";\n");
					holeFilledIdNo =  holeFilledIdNo + 1;
					delete[] fbits;
			}
			strcat(Buffer, "}");
			if(dirType == INPUT)
			{
				strcpy(Buffer1, "# define COMPUTED_PI_IN_SIZE ");			
				strcat(Buffer, " PI_IN;");	
			}
			
			else if(dirType == OUTPUT)
			{				
				
				strcpy(Buffer1, "\n\n# define COMPUTED_PI_OUT_SIZE ");		
				strcat(Buffer, " PI_OUT;");	
			}
		}
		
		
			char* strsize = new char[50];
			printf("\n Totalsize %d",totalsize);
			
			/* write the size in bytes*/
			totalsize =  totalsize/8;	
			strsize =  _IntToAscii(totalsize, strsize, 10);
			strcat(Buffer1, strsize);
			
			unsigned int len =  strlen(Buffer1);
			printf("\n Length of Buffer1 %d", len);
			printf(" Buffer1 :%s", Buffer1);
			if((len != fwrite(Buffer1, sizeof(char),len,fileptr)))
			{	
				printf("\n xap.h buffer1 not  written");
				ocfmException ex;
				ex.ocfm_Excpetion(OCFM_ERR_FILE_CANNOT_OPEN);
				delete[] Buffer;
				delete[] Buffer1;
				throw ex;
			}
			else
			{
			printf("\n buffer1 written");
			/////fclose(fileptr);
			}
		
			len  =  strlen(Buffer);
			printf("\n\nlen of Buffer :%d", len);

			if((len != fwrite(Buffer, sizeof(char),len,fileptr)))
			{	
				printf("\n xap.h buffer not written");
				ocfmException ex;
				ex.ocfm_Excpetion(OCFM_ERR_FILE_CANNOT_OPEN);
				throw ex;
			}
			else
			{
			printf("\n done");
			}
			delete[] Buffer;
			delete[] Buffer1;
	
	
}
void SetSIdxDataType(DataType *dt, char* Idx, char* SIdx)
{
		CNodeCollection* objNodeCollection;
		CIndexCollection *objIndexCol;
		CIndex *objIndex;
		CSubIndex *objSIdx;
		CNode objNode;
		objNodeCollection = CNodeCollection::getNodeColObjectPointer();				
				
		objNode = objNodeCollection->getMNNode();
		objIndexCol = objNode.getIndexCollection();
		
		objIndex = objIndexCol->getIndexbyIndexValue(Idx);
		objSIdx = objIndex->getSubIndexbyIndexValue(SIdx);
		
		objSIdx->setDataTypeST(*dt);
		
	
}
void AddPDOIndexsToMN(char* Index, char* SubIndex, EPDOType pdoType)
{
	ocfmRetCode retCode;
	DataType* dt;
	CIndex *objIndex;
	CNodeCollection *objNodeCol;
	CIndexCollection *objIdxCol;
	CNode objMNNode;
	CDataTypeCollection *objDTCol;
	CSubIndex *objSIdx;
	char* Name = new char[50];
	char* pdoMap = new char[4 + ALLOC_BUFFER];
	char* ObjectName = new char[14 + ALLOC_BUFFER];
	
	objNodeCol =  CNodeCollection::getNodeColObjectPointer();
	objMNNode = objNodeCol->getNode(MN, MN_NODEID);
	objDTCol = objMNNode.getDataTypeCollection();
	
	
	retCode = AddIndex(MN_NODEID, MN, Index);	
	objIdxCol = objMNNode.getIndexCollection();
	if(objIdxCol != NULL)
	{
		objIndex = objIdxCol->getIndexbyIndexValue(Index);
		if(objIndex != NULL && (retCode.code != OCFM_ERR_INDEX_ALREADY_EXISTS) )
		{
			objIndex->setObjectType((char*)"ARRAY");
			if(pdoType == PDO_RPDO)
			{				
				strcpy(ObjectName, "PI_OUTPUTS_A");
				strcat(ObjectName, getPIName(Index));		
			}
			else if(pdoType == PDO_TPDO)
			{							
				strcpy(ObjectName, "PI_INPUTS_A");
				strcat(ObjectName, getPIName(Index));	
			}
			objIndex->setName(ObjectName);
			/* Add subindex 00 */
			
			retCode = AddSubIndex(MN_NODEID, MN, Index, "00");
			if(retCode.code  ==  OCFM_ERR_SUCCESS)
			{
				//SetSubIndexAttributes(MN_NODEID, MN, Index, "00", NULL, "NumberOfEntries");
			}
		}
	}
	
	//printf("\n after Add Index before subIndex : %s", Index);
	retCode = AddSubIndex(240, MN, Index, SubIndex);
	
	if(objIndex != NULL)
	{
		objSIdx = objIndex->getSubIndexbyIndexValue(SubIndex);
		if(objSIdx != NULL)
		{
			
			objSIdx->setObjectType((char*)"VAR");
			
			/* Its reversed because CN's RPDO is MN's TPDO*/
			
			if(pdoType == PDO_RPDO)
			{				
				strcpy(pdoMap, "TPDO");
				strcpy(ObjectName, "PI_OUTPUTS_");
				strcat(ObjectName, getPIName(Index));								
				objSIdx->setAccessType((char*)"wo");
				}
			else if(pdoType == PDO_TPDO)
			{				
				strcpy(pdoMap, "RPDO");				
				strcpy(ObjectName, "PI_INPUTS_");
				strcat(ObjectName, getPIName(Index));	
				
				objSIdx->setAccessType((char*)"ro");
			}
				objSIdx->setName(ObjectName);
				objSIdx->setPDOMapping(pdoMap);
				delete[] pdoMap;
				delete[] Name;
		}
	}
	
	
		
	Name = getPIDataTypeName(Index);
		
	dt = objDTCol->getDataTypeByName(Name);
	//dt->setName(Name);
	

	SetSIdxDataType(dt, Index, SubIndex);
	
	UpdateNumberOfEnteriesSIdx(objIndex, MN);
	
	//delete[] Name;
	//delete[] pdoMap;
	//delete[] ObjectName;
	
 }
char* getPIAddress(PDODataType dt,  EPIDirectionType dirType)
{
	int i;
	for(i = 0; i< NO_OF_PI_ENTERIES; i++)
	{
		if((AddressTable[i].dt == dt) && (AddressTable[i].Direction == dirType))
		return AddressTable[i].Address;
	}

	//Handled error case and returned dummy value to avoid warning
	cout << "Error in returning getPIAddress" << endl;
	return (char*) "Error";
}
char* getPIDataTypeName(char* Address)
{
	char *RetString = NULL;
	
	for(int i = 0; i< NO_OF_PI_ENTERIES; i++)
	{		
		if(strcmp(AddressTable[i].Address,  Address) ==0)
		{
			switch(AddressTable[i].dt)
			{
				case UNSIGNED8 :
				{
					RetString = (char*) "Unsigned8";
					break;
				}
				case INTEGER8 :
				{
					RetString = (char*) "Integer8";
					break;
				}
				case UNSIGNED16 :
				{
					RetString = (char*) "Unsigned16";
					break;
				}
				case INTEGER16 :
				{
					RetString = (char*) "Integer8";
					break;
				}
				case UNSIGNED32 :
				{
					RetString = (char*) "Unsigned32";
					break;
				}
				case INTEGER32 :
				{
					RetString = (char*) "Integer32";
					break;
				}
				default:
				{
					//Handled error case and returned dummy value to avoid warning
					cout << "Error in returning getPIDataTypeName" << endl;
					RetString = (char*) "Error";
					break;
				}
			}
		}
	}
	return RetString;	
}
char* getPIName(char* Address)
{
	char *RetString = NULL;
	
	for(int i = 0; i< NO_OF_PI_ENTERIES; i++)
	{		
		if(strcmp(AddressTable[i].Address,  Address) ==0)
		{
			switch(AddressTable[i].dt)
			{
				case UNSIGNED8 :
				{
					RetString = (char*) "U8";
					break;
				}
				case INTEGER8 :
				{
					RetString = (char*) "I8";
					break;
				}
				case UNSIGNED16 :
				{
					RetString = (char*) "U16";
					break;
				}
				case INTEGER16 :
				{
					RetString = (char*) "I16";
					break;
				}
				case UNSIGNED32 :
				{
					RetString = (char*) "U32";
					break;
				}
				case INTEGER32 :
				{
					RetString = (char*) "I32";
					break;
				}
				default:
				{
					//Handled error case and returned dummy value to avoid warning
					cout << "Error in returning getPIDataTypeName" << endl;
					RetString = (char*) "Error";
					break;
				}
			}
		}
	}
	
	return RetString;
}
bool CheckIfProcessImageIdx(char* Index)
{
	for(int i = 0; i< NO_OF_PI_ENTERIES; i++)
	{		
		if(strcmp(AddressTable[i].Address,  Index) ==0)
		return true;
	}
	
	return false;
}