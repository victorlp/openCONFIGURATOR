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
ModuleCol modCol[2000]; 
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
	int PIVarsCount =0;
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

	try
	{
		char Buffer[10000];
			
		char* strCNID = new char[7];
		int ModuleNo = 0;
		int LastModuleNo = 0;
		char* ModName =  new char[50];
		char* strModuleNo = new char[16];
		char* varNo = new char[10];
		int totalsize = 0;
						
		char* strFileName  = new char[strlen(fileName) + 2];
		strcpy(strFileName, fileName);
		strcat(strFileName, ".h");
		
  ModuleCol	modCol[1000];
		//strcpy(strFileName, fileName);
		FILE* fileptr = new FILE();
		
		if (( fileptr = fopen(strFileName,"w+")) == NULL)
			{
				ocfmException ex;
				ex.ocfm_Excpetion(OCFM_ERR_CANNOT_OPEN_FILE);
				throw ex;						
			}
			
			if(InVar!=0 )
			{
				strcpy(Buffer, "typedef struct { \n");	
				ModuleNo = 0;
				for(int i = 0; i<InVar ; i++)
				{					
					int NodeId;
					char* str = new char[4];				
					
					
					NodeId = PI_IN[i].CNNodeID;
				/*	if((i!=0) && (NodeId == PI_IN[i-1].CNNodeID))
					totalsize = PI_IN[i].DataInfo.DataSize + totalsize;
					else
					totalsize = 0;*/
			
					/*strcat(Buffer, "struct");				
					strcat(Buffer, " CN");*/
					strCNID = IntToAscii(PI_IN[i].CNNodeID, strCNID, 10); 
					/*strcat(Buffer,strCNID);*/
							/* Add Module No*/
					
					/*if(i != 0)
					{*/
					/*	if(strcmp(PI_IN[i].ModuleName, ModName) !=0)
						{
							
							if(CheckIfModuleExists(PI_IN[i].ModuleName, ModuleNo, LastModuleNo, modCol))
							{
								strModuleNo = IntToAscii(ModuleNo, strModuleNo, 10);
							}
							else
							{
								LastModuleNo = LastModuleNo  + 1;	
								strcpy(modCol[LastModuleNo].ModuleName, PI_IN[i].ModuleName);
								modCol[LastModuleNo].ModuleNo = LastModuleNo; 	
								strModuleNo = IntToAscii(LastModuleNo, strModuleNo, 10);					
								}
							}
					}
					else
					{
							strcpy(modCol[LastModuleNo].ModuleName, PI_IN[i].ModuleName);
							modCol[LastModuleNo].ModuleNo = LastModuleNo; 
							strModuleNo = IntToAscii(ModuleNo, strModuleNo, 10);
					}*/
					//char* ModName =  new char[strlen(PI_IN[i].ModuleName) + 1];
					strcpy(strModuleNo, subString( PI_IN[i].ModuleIndex,2, 2));
					strcpy(ModName, PI_IN[i].ModuleName);
					
			
					//printf("\n Module Name: %s",ModName);
					strcat(Buffer,"unsigned");
					strcat(Buffer," ");
					char* varName = new char[100];
					strcpy(varName, "CN");
					strCNID = IntToAscii(PI_IN[i].CNNodeID, strCNID, 10); 
					strcat(varName, strCNID);
					strcat(varName, "_");
					
					/* Add Mod NO*/
					strcat(varName, "M");
					strcat(varName, strModuleNo);
					strcat(varName, "_");
					
					strcat(varName, ModName);
					strcat(varName, "_");
					strcat(varName, PI_IN[i].VarName);
						
					if(strcmp(subString(PI_IN[i].VarName,0, 8), "Reserved")== 0)
					{
						varNo =  IntToAscii(i, varNo, 10);
						strcat(varName, "_");
						strcat(varName, varNo);
					}
					 
					strcat(Buffer, varName);
					//strcat(Buffer, PI_IN[i].VarName);
					strcat(Buffer, ":");
					str = IntToAscii(PI_IN[i].DataInfo.DataSize, str, 10); 
					strcat(Buffer, str);
					strcat(Buffer, ";");
					
					strcat(Buffer, "\n");
					//i++;
					//printf("\nI:%d",i);					
										
			}
			strcat(Buffer, "}");
			strcat(Buffer, " PI_IN;");	
		}
			int len  =  strlen(Buffer);
			if((fwrite(Buffer, sizeof(char),len,fileptr))!=NULL)
			{
				//fclose(fileptr);
				//printf("Buffer1 written");
			
			}
			
				if(OutVar!=0 )
			{
				strcpy(Buffer, "\ntypedef struct { \n");	
				ModuleNo = 0;
				//LastModuleNo = 0;
				for(int i = 0; i<OutVar ; i++)
				{					
						int NodeId;
					char* str = new char[4];
					NodeId = PI_OUT[i].CNNodeID;
					
					strcpy(ModName, "");
					/*strcat(Buffer, "struct");				
					strcat(Buffer, " CN");
					
				*/	strCNID = IntToAscii(PI_OUT[i].CNNodeID, strCNID, 10); 
				//strcat(Buffer,strCNID);
				/*		if(strcmp(PI_OUT[i].ModuleName, ModName) !=0)
						{							
								if(CheckIfModuleExists(PI_OUT[i].ModuleName, ModuleNo, LastModuleNo, modCol))
								{
									strModuleNo = IntToAscii(ModuleNo, strModuleNo, 10);
								}
								else
								{
									LastModuleNo = LastModuleNo  + 1;	
									strcpy(modCol[ModuleNo].ModuleName, PI_OUT[i].ModuleName);
									modCol[ModuleNo].ModuleNo = LastModuleNo; 	
									strModuleNo = IntToAscii(LastModuleNo, strModuleNo, 10);					
								}
							}
							else							
							strModuleNo = IntToAscii(LastModuleNo, strModuleNo, 10);
			*/	
					//char* ModName =  new char[strlen(PI_OUT[i].ModuleName) + 1];
					strcpy(strModuleNo, subString( PI_OUT[i].ModuleIndex,2, 2));
					strcpy(ModName, PI_OUT[i].ModuleName);
					//printf("\n Module Name: %s",ModName);
					strcat(Buffer,"unsigned");
					strcat(Buffer," ");
					char* varName = new char[100];
					strcpy(varName, "CN");
					strCNID = IntToAscii(PI_OUT[i].CNNodeID, strCNID, 10); 
					strcat(varName, strCNID);
					strcat(varName, "_");
					
						/* Add Mod NO*/
					strcat(varName, "M");
					strcat(varName, strModuleNo);
					strcat(varName, "_");
					
					strcat(varName, ModName);
					strcat(varName, "_");
					strcat(varName, PI_OUT[i].VarName);
					
						if(strcmp(subString(PI_OUT[i].VarName,0, 8), "Reserved")== 0)
					{
						varNo =  IntToAscii(i, varNo, 10);
						strcat(varName, "_");
						strcat(varName, varNo);
					}
					
					strcat(Buffer, varName);
					//strcat(Buffer, PI_IN[i].VarName);
					strcat(Buffer, ":");
					str = IntToAscii(PI_OUT[i].DataInfo.DataSize, str, 10); 
					strcat(Buffer, str);
						strcat(Buffer, ";");
						
					strcat(Buffer, "\n");
					//i++;
							//printf("\nI:%d",i);
								
			}
				strcat(Buffer, "}");
			strcat(Buffer, " PI_OUT;");	
	
		}
			len  =  strlen(Buffer);
			if((fwrite(Buffer, sizeof(char),len,fileptr))!=NULL)
			{
				fclose(fileptr);
				//printf("Buffer1 written");
			
			}
		}
		

		catch(ocfmException& ex)
		{
		}
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
void AddPDOIndexsToMN(char* Index, char* SubIndex)
{
	ocfmRetCode retCode;
	DataType* dt;
	CNodeCollection *objNodeCol;
	CNode objMNNode;
	CDataTypeCollection *objDTCol;
	char* Name = new char[50];
	
	objNodeCol =  CNodeCollection::getNodeColObjectPointer();
	objMNNode = objNodeCol->getNode(MN, 240);
	objDTCol = objMNNode.getDataTypeCollection();
	
	printf("\n Index: %s",Index);
	
	retCode = AddIndex(240, MN, Index);	
	//printf("\n after Add Index before subIndex : %s", Index);
	retCode = AddSubIndex(240, MN, Index, SubIndex);
	
	Name = getPIDataTypeName(Index);
		
	dt = objDTCol->getDataTypeByName(Name);
	//dt->setName(Name);
	

	SetSIdxDataType(dt, Index, SubIndex);
	
 }
char* getPIAddress(PDODataType dt,  EPIDirectionType dirType)
{
	
	for(int i = 0; i< NO_OF_PI_ENTERIES; i++)
	{
		if((AddressTable[i].dt == dt) && (AddressTable[i].Direction == dirType))
		return AddressTable[i].Address;
	}
}
char* getPIDataTypeName(char* Address)
{
	
	for(int i = 0; i< NO_OF_PI_ENTERIES; i++)
	{		
		if(strcmp(AddressTable[i].Address,  Address) ==0)
		{
			switch(AddressTable[i].dt)
			{
				case UNSIGNED8 :
				{
					return "Unsigned8";
					break;
				}
				case INTEGER8 :
				{
					return "Integer8";
					break;
				}
				case UNSIGNED16 :
				{
					return "Unsigned16";
					break;
				}
				case INTEGER16 :
				{
					return "Integer8";
					break;
				}
				case UNSIGNED32 :
				{
					return "Unsigned32";
					break;
				}
				case INTEGER32 :
				{
					return "Integer32";
					break;
				}
				
			}
		}
	}
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