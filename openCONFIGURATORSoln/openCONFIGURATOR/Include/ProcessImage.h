#ifndef processImage_h
#define processImage_h
///////////////////////////////////////////////////////////////////////////////////////////////
//
//  $Header: $
//
// NAME:  ProcessImage.h
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
//

/************************************************************************************************
* Includes
************************************************************************************************/
#include <iostream>
#include <fstream>
#include "Declarations.h"

/************************************************************************************************
* Enumerations
************************************************************************************************/
typedef enum EPIDirectionType
{
	INPUT 		= 0,
	OUTPUT
}EPIDirectionType;

typedef enum ePDODataType
{
	UNSIGNED8	= 0,
	INTEGER8,
	UNSIGNED16,
	INTEGER16,
	UNSIGNED32,
	INTEGER32
}PDODataType;

typedef struct PIDataInfo
{
	IEC_Datatype _dt_enum;
	int			DataSize;
	char*		_dt_Name;
}PIDataInfo;

/************************************************************************************************
* Structures
************************************************************************************************/
struct ProcessImage
{
	char*					Name;
	char					Direction[5];
	/*char*					DataType;
	char					DataSize[5];*/
	PIDataInfo DataInfo;
	int								ByteOffset;
	char*					Value;
	char*					subindex;
	char*					Index;
	int							BitOffset;
	int							ParametrIndex;
	char*					ValueFromPDO;
	char*					ModuleName;
	int							CNNodeID;
	char*					VarName;
	char*					ModuleIndex;
	void Initialize()
	{
		ParametrIndex = 0;
	}
	EPIDirectionType			DirectionType;
};

struct NETProcessImage
{
	char*					Name;
	char*					LastName;
	char*					ModuleName;
	PIDataInfo DataInfo;
	int					CNNodeID;
	void Initialize()
	{
		count = 0;
		iTotalDataSize = 0;
	}
	EPIDirectionType			DirectionType;
	int					iTotalDataSize;
	int count;
};

typedef struct tAddressTable
{
	char Address[10];
	PDODataType dt;
	EPIDirectionType Direction;	
}tADDRESSTABLE;

struct stOffsets
{
	int prevOffset;
	int currOffset;
};
		
struct PIObject
{
	char* Index;
	char* SubIndex;
	
};
struct ModuleCol
{
	int ModuleNo;
	char ModuleName[50];
};

/************************************************************************************************
* Externs
************************************************************************************************/
extern int iInVars;
extern int iOutVars;

/************************************************************************************************
* Constants
************************************************************************************************/
static const int NO_OF_PI_ENTERIES = 12;

/****************************************************************************************************
* Function Declarations
****************************************************************************************************/
void GroupInOutPIVariables(ProcessImage PIInCol[], ProcessImage PIOutCol[]);
void CalculateOffsets(int VarCount,  EPIDirectionType type);
PIDataInfo* getIECDT(char* dtStr, int dataSize);
int ComputeINOffset(int dataSize, EPDOType pdoType);
int ComputeOUTOffset(int dataSize, EPDOType pdoType);
void SetPIOffsets(ProcessImage* pi,int& StartingByteOffset, int pos, int& StartBitOffset);
void GenerateXAPHeaderFile(char* fileName, ProcessImage PI_IN[], ProcessImage PI_OUT[], int InVar, int OutVar);
void AddPDOIndexsToMN(char* Index, char* SubIndex, EPDOType pdoType);
PIObject getPIAddress(PDODataType dt,  EPIDirectionType dirType, int Offset, int dataSize);
char* getPIDataTypeName(char* Address);
char* getPIName(char* Address);
bool CheckIfProcessImageIdx(char* Index);
void WriteXAPHeaderContents(ProcessImage PI[], int NumberOfVars, EPIDirectionType dirType, FILE* fileptr);
void GenerateNETHeaderFile(char* pbFileName, ProcessImage objPIInCol[], ProcessImage objPIOutCol[], INT32 iInVar, INT32 iOutVar);
void WriteNETHeaderContents(ProcessImage objProcessImage[], INT32 iNumberOfVars, EPIDirectionType enumDirType, FILE* fpNetHeader);
INT32 SearchModuleNameNETProcessImageCollection(int CNNodeID, INT32 iItemLoopCount, char* schModuleName );
INT32 GroupNETPIVariables( EPIDirectionType DirectionType, ProcessImage aobjPICol[]);
void CopyPItoNETPICollection(ProcessImage objProcessImage, NETProcessImage objNETProcessImage, char* ModuleName);
INT32 GroupNETHeaderContents(ProcessImage objProcessImage[], INT32 iNumberOfVars, EPIDirectionType enumDirType, FILE* fpNetHeader);
char* GetDatatypeNETPI(IEC_Datatype dt_enum);
int GetDatasizeNETPI(IEC_Datatype dt_enum);
#endif // processImage_h
