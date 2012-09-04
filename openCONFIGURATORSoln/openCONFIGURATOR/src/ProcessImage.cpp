///////////////////////////////////////////////////////////////////////////////////////////////
//
//  $Source: $
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
                                                        {"A000", INTEGER8, OUTPUT},
                                                        {"A040", UNSIGNED8,	OUTPUT},
                                                        {"A0C0", INTEGER16, OUTPUT},
                                                        {"A100", UNSIGNED16, OUTPUT},
                                                        {"A1C0", INTEGER32, OUTPUT},
                                                        {"A200", UNSIGNED32, OUTPUT},
                                                        {"A480", INTEGER8, INPUT},
                                                        {"A4C0", UNSIGNED8,	INPUT},
                                                        {"A540", INTEGER16, INPUT},
                                                        {"A580", UNSIGNED16, INPUT},
                                                        {"A640", INTEGER32, INPUT},
                                                        {"A680", UNSIGNED32, INPUT},
																												
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
		pobjProcessImage->ByteOffset 	= iStartByteOffset;
		iStartByteOffset 				=  iStartByteOffset + (pobjProcessImage->DataInfo.DataSize) / 8;
	}	
}

/****************************************************************************************************
* Function Name: GroupInOutPIVariables
* Description:
* Return value: void
****************************************************************************************************/
void GroupInOutPIVariables(ProcessImage aobjPIInCol[], ProcessImage aobjPIOutCol[])
{
	
	CNodeCollection* pobjNodeCol = NULL;
	CNode*  objNode;
	pobjNodeCol =  CNodeCollection::getNodeColObjectPointer();
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
			}
			else if(objNode->ProcessImageCollection[iInLoopCount].DirectionType == OUTPUT)
			{			
				aobjPIOutCol[iOutVars]  = objNode->ProcessImageCollection[iInLoopCount];
				iOutVars++;
			}
		}
	}
}

// /****************************************************************************************************
// * Function Name: GroupNETPIVariables
// * Description:
// * Return value: void
// ****************************************************************************************************/
INT32 GroupNETPIVariables( EPIDirectionType DirectionType, NETProcessImage aobjPICol[])
{
	CNodeCollection* pobjNodeCol = NULL;
	CNode*  objNode;
	pobjNodeCol =  CNodeCollection::getNodeColObjectPointer();
	INT32 NetPIVarsCount =0;
	for(INT32 iOutLoopCount=0; iOutLoopCount < pobjNodeCol->getNumberOfNodes(); iOutLoopCount++)
	{
		objNode = pobjNodeCol->getNodebyColIndex(iOutLoopCount);	
		
		for(INT32 iInLoopCount=0; iInLoopCount < objNode->NETProcessImageCollection.Count(); iInLoopCount++)
		{			
			if(objNode->NETProcessImageCollection[iInLoopCount].DirectionType == DirectionType)
			{
				aobjPICol[NetPIVarsCount]  = objNode->NETProcessImageCollection[iInLoopCount];
				NetPIVarsCount++;
			}
		}
	}
	return NetPIVarsCount;
}

/****************************************************************************************************
* Function Name: SetUniquePIVarName
* Description:
* Return value: void
****************************************************************************************************/
void SetUniquePIVarName()
{
	CNodeCollection* pobjNodeCol = NULL;
	CNode*  objNode;
	pobjNodeCol =  CNodeCollection::getNodeColObjectPointer();
	iInVars = 0;
	iOutVars = 0;
	ProcessImage* pPIObjTemp = NULL;
	ProcessImage* pPIObjTempChk = NULL;
	const int iUniqNameLen = 2;


	for(INT32 iOutLoopCount=0; iOutLoopCount < pobjNodeCol->getNumberOfNodes(); iOutLoopCount++)
	{
		objNode = pobjNodeCol->getNodebyColIndex(iOutLoopCount);

		for(INT32 iInLoopCount=0; iInLoopCount < objNode->ProcessImageCollection.Count(); iInLoopCount++)
		{
			int iUniqNameCnt = 1;
			bool bMatchFound = false;

			pPIObjTemp = &objNode->ProcessImageCollection[iInLoopCount];

			//it is possible that the changed var name matching a previous entry
		    for(INT32 iInChkLoopCount=0; iInChkLoopCount < objNode->ProcessImageCollection.Count() ; iInChkLoopCount++)
			{
		    	if(iInChkLoopCount == iInLoopCount)
		    	{
		    		//Both the index are same, do not check same object
		    		continue;
		    	}

		    	pPIObjTempChk = &objNode->ProcessImageCollection[iInChkLoopCount];

		    	//check module index, module name, directiontype and variable name
		    	//if all are same then append count variable to variable name
		    	if(\
		    			(strcmp(pPIObjTemp->ModuleIndex,pPIObjTempChk->ModuleIndex) == 0) \
		    			&& (strcmp(pPIObjTemp->ModuleName,pPIObjTempChk->ModuleName) == 0) \
		    			&& (pPIObjTemp->DirectionType == pPIObjTempChk->DirectionType) \
		    			&& (strcmp(pPIObjTemp->VarName,pPIObjTempChk->VarName) == 0) \
		    	  )
		    	{
		    		//change the name of VarName
		    		iUniqNameCnt++; //1 is reserved for first matched entry
		    		if(NULL != pPIObjTempChk->VarName) free(pPIObjTempChk->VarName);
		    		pPIObjTempChk->VarName = (char *)malloc(strlen(pPIObjTemp->VarName) + iUniqNameLen + ALLOC_BUFFER);
		    		sprintf(pPIObjTempChk->VarName, "%s%02d", pPIObjTemp->VarName, iUniqNameCnt);
		    		bMatchFound = true;

		    		//change the name of Name
		    		if(NULL != pPIObjTempChk->Name) free(pPIObjTempChk->Name);
		    		pPIObjTempChk->Name = (char*)malloc(strlen(pPIObjTempChk->VarName) + strlen(pPIObjTempChk->ModuleName) + 6 + ALLOC_BUFFER);
					strcpy(pPIObjTempChk->Name,getPIName(objNode->getNodeId()));
					strcat(pPIObjTempChk->Name, pPIObjTempChk->ModuleName);
					strcat(pPIObjTempChk->Name, ".");
					strcat(pPIObjTempChk->Name,pPIObjTempChk->VarName);

		    	}
			}

		    if(true == bMatchFound)
		    {
		    	char* pcTempVarName;
		    	pcTempVarName = (char*)malloc(strlen(pPIObjTemp->VarName) + ALLOC_BUFFER);
		    	strcpy(pcTempVarName, pPIObjTemp->VarName);
		    	if(NULL != pPIObjTemp->VarName) delete[] pPIObjTemp->VarName;
				pPIObjTemp->VarName = (char*)malloc(strlen(pcTempVarName) + iUniqNameLen + ALLOC_BUFFER);
		    	sprintf(pPIObjTemp->VarName, "%s%02d", pcTempVarName, 1);
		    	free(pcTempVarName);

				//change the name of Name
				if(NULL != pPIObjTemp->Name) free(pPIObjTemp->Name);
				pPIObjTemp->Name = (char*)malloc(strlen(pPIObjTemp->VarName) + strlen(pPIObjTemp->ModuleName) + 6 + ALLOC_BUFFER);
				strcpy(pPIObjTemp->Name,getPIName(objNode->getNodeId()));
				strcat(pPIObjTemp->Name, pPIObjTemp->ModuleName);
				strcat(pPIObjTemp->Name, ".");
				strcat(pPIObjTemp->Name,pPIObjTemp->VarName);
		    }
		}
	}
	return;
}

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
	
	if (( fpXapFile = fopen(pbXapFileName,"w+")) == NULL)
	{
		ocfmException ex;
		ex.ocfm_Excpetion(OCFM_ERR_CANNOT_OPEN_FILE);
		delete [] pbXapFileName;
		throw ex;
	}			

	//write comment
    char* pcComment = new char[strlen(BUILD_COMMENT) + BUILDTIME_BUF_LEN + ALLOC_BUFFER + 8];// 8 is for comment lines
    strcpy(pcComment, "/* ");
    strcat(pcComment,  BUILD_COMMENT);
    strcat(pcComment, GetBuildTime());
    strcat(pcComment, " */\n");
    UINT32 uiStrLength =  strlen(pcComment);
    if((uiStrLength != fwrite(pcComment, sizeof(char), uiStrLength, fpXapFile)))
	{
		ocfmException ex;
		ex.ocfm_Excpetion(OCFM_ERR_FILE_CANNOT_OPEN);
		throw ex;
	}
	delete[] pcComment;
	/*BUG #6 - START*/
	//write include guard for the headerfile
	char* pbXapHeaderIncludeGuard  = new char[strlen(INCLUDE_GUARD_CHECK) + strlen(INCLUDE_GUARD_START) + ALLOC_BUFFER];
	strcpy(pbXapHeaderIncludeGuard, INCLUDE_GUARD_CHECK);
	strcat(pbXapHeaderIncludeGuard, "\n");
	strcat(pbXapHeaderIncludeGuard, INCLUDE_GUARD_START);
	strcat(pbXapHeaderIncludeGuard, "\n\n");
	uiStrLength =  strlen(pbXapHeaderIncludeGuard);
	
	if((uiStrLength != fwrite(pbXapHeaderIncludeGuard, sizeof(char), uiStrLength, fpXapFile)))
	{
		ocfmException ex;
		ex.ocfm_Excpetion(OCFM_ERR_FILE_CANNOT_OPEN);
		throw ex;
	}
	delete[] pbXapHeaderIncludeGuard;
	/*BUG #6 - END*/
	/* write Input structure */
	if(iInVar !=0)
	{			
		WriteXAPHeaderContents(objPIInCol, iInVar, INPUT, fpXapFile);			
	}

	fclose(fpXapFile);
	/* write Output structure */
	if (( fpXapFile = fopen(pbXapFileName,"a+")) == NULL)
	{
		ocfmException ex;
		ex.ocfm_Excpetion(OCFM_ERR_CANNOT_OPEN_FILE);
		delete [] pbXapFileName;
		throw ex;
	}
	if(iOutVar !=0)
	{
		WriteXAPHeaderContents(objPIOutCol, iOutVar, OUTPUT, fpXapFile ); 
	}
	delete [] pbXapFileName;
	/*BUG #6 - START*/
	//close include guard for the XAP header file
	 pbXapHeaderIncludeGuard  = new char[strlen(INCLUDE_GUARD_END) + ALLOC_BUFFER];
	 strcpy(pbXapHeaderIncludeGuard, "\n");
	 strcat(pbXapHeaderIncludeGuard, INCLUDE_GUARD_END);
	uiStrLength =  strlen(pbXapHeaderIncludeGuard);	
	if((uiStrLength != fwrite(pbXapHeaderIncludeGuard, sizeof(char), uiStrLength, fpXapFile)))
	{
		ocfmException ex;
		ex.ocfm_Excpetion(OCFM_ERR_FILE_CANNOT_OPEN);
		throw ex;
	}
	fclose(fpXapFile);
	delete[] pbXapHeaderIncludeGuard;
	/*BUG #6 - END*/
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
	INT32 iModuleNo 		= 0;
	INT32 iTotalsize 		= 0;
	INT32 iDataSize  		= 0;
	INT32 iHoleFilledIdNo 	= 1;									


	if(iNumberOfVars != 0 )
	{
		strcpy(pbBuffer, "\ntypedef struct \n{\n");	
		iModuleNo = 0;
		for(INT32 iLoopCount = 0; iLoopCount<iNumberOfVars ; iLoopCount++)
		{					
			INT32 iNodeId;
			char* pbStrCNID 	= new char[50];
			char* pbModName		= new char[50];
			char* pbStrModuleNo = new char[20];
			
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
					strcat(pbBuffer,"\tunsigned");
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
				
			strcat(pbBuffer,"\tunsigned");
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
			strcat(pbBuffer,"\tunsigned");
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
            strcpy(pbBuffer1, "# define COMPUTED_PI_OUT_SIZE ");			
            strcat(pbBuffer, " PI_OUT;");   
		}	
		else if(enumDirType == OUTPUT)
		{				
			
            strcpy(pbBuffer1, "\n\n# define COMPUTED_PI_IN_SIZE ");		
            strcat(pbBuffer, " PI_IN;");    
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
* Function Name: GenerateNETHeaderFile
* Description:
* Return value: void
****************************************************************************************************/
void GenerateNETHeaderFile(char* pbFileName, ProcessImage objPIInCol[], ProcessImage objPIOutCol[], INT32 iInVar, INT32 iOutVar)
{
	char* pbNetFileName  = new char[strlen(pbFileName) + ALLOC_BUFFER];
	FILE* fpNetFile 	 = new FILE();

	strcpy(pbNetFileName, pbFileName);
	strcat(pbNetFileName, ".cs");
	/* write Input structure */
	if (( fpNetFile = fopen(pbNetFileName,"w+")) == NULL)
	{
		ocfmException ex;
		ex.ocfm_Excpetion(OCFM_ERR_CANNOT_OPEN_FILE);
		delete [] pbNetFileName;
		throw ex;
	}
	CNodeCollection* pobjNodeCol = NULL;
	CNode*  objNode;
	pobjNodeCol =  CNodeCollection::getNodeColObjectPointer();
	for(INT32 iNodeColLoopCount=0; iNodeColLoopCount < pobjNodeCol->getNumberOfNodes(); iNodeColLoopCount++)
	{
		objNode = pobjNodeCol->getNodebyColIndex(iNodeColLoopCount);	
		objNode->DeleteCollectionsForNETPI();	
	}
	if( (iInVar != 0) || (iOutVar != 0) )
	{
		 /*Writng Header of .NET Interface*/
		char* pbBuffer 		= new char[HEADER_FILE_BUFFER];
		strcpy(pbBuffer, "using System;\n");
		strcat(pbBuffer, "using System.Runtime.InteropServices;\n");
		//writing comments
		strcat(pbBuffer, "/// <summary>\n");
		strcat(pbBuffer, "/// ");
	    strcat(pbBuffer, BUILD_COMMENT);
	    strcat(pbBuffer, GetBuildTime());
	    strcat(pbBuffer, "\n");
	    strcat(pbBuffer, "/// </summary>\n");

		strcat(pbBuffer, "\nnamespace openPOWERLINK\n");
		strcat(pbBuffer, "{\n");
		UINT32 uiStrLength =  strlen(pbBuffer);
		
		if((uiStrLength != fwrite(pbBuffer, sizeof(char),uiStrLength,fpNetFile)))
		{	
			ocfmException ex;
			ex.ocfm_Excpetion(OCFM_ERR_FILE_CANNOT_OPEN);
            fclose(fpNetFile);
			delete[] pbBuffer;
			throw ex;
		}
		delete[] pbBuffer;
	}		
	if(iInVar !=0)
	{			
		WriteNETHeaderContents(objPIInCol, iInVar, INPUT, fpNetFile);
	}

	/* write Output structure */
	if(iOutVar !=0)
	{
		WriteNETHeaderContents(objPIOutCol, iOutVar, OUTPUT, fpNetFile);	
	}
	if( (iInVar !=0) || (iOutVar !=0) )
	{
		char* pbBuffer = new char[10];
		strcpy(pbBuffer, "}\n");
		UINT32 uiStrLength =  strlen(pbBuffer);
		
		if((uiStrLength != fwrite(pbBuffer, sizeof(char),uiStrLength,fpNetFile)))
		{	
			ocfmException ex;
			ex.ocfm_Excpetion(OCFM_ERR_FILE_CANNOT_OPEN);
			fclose(fpNetFile);
			delete[] pbBuffer;
			throw ex;
		}
		delete[] pbBuffer;
	}
    fclose(fpNetFile);
	delete [] pbNetFileName;
}

/****************************************************************************************************
* Function Name: WriteNETHeaderContents
* Description:
* Return value: void
****************************************************************************************************/
void WriteNETHeaderContents(ProcessImage objProcessImage[], INT32 iNumberOfVars, EPIDirectionType enumDirType, FILE* fpNetHeader)
{

	char* pbBuffer 		= new char[HEADER_FILE_BUFFER];
	pbBuffer[0] = 0;
	char* pbBuffer1 	= new char[200];		
	char* pbBuffer2 	= new char[500];
    char pcOffset[10];
    int iOffset = 0;

	INT32 iTotalsize = GroupNETHeaderContents(objProcessImage, iNumberOfVars, enumDirType, fpNetHeader);
	NETProcessImage aobjNetPiCol[PI_VAR_COUNT] = {};
	INT32 NetPIVarsCount = GroupNETPIVariables( enumDirType, aobjNetPiCol);
	for(INT32 iLoopCount = 0; iLoopCount<NetPIVarsCount ; iLoopCount++)
	{
        strcat(pbBuffer , "\t\t[FieldOffset(");
        memset(pcOffset, 0, 10);
        _IntToAscii(iOffset, pcOffset, 10);
        strcat(pbBuffer , pcOffset);
        strcat(pbBuffer , ")]\n");
		strcat(pbBuffer , "\t\tpublic ");
		strcat(pbBuffer , GetDatatypeNETPI(aobjNetPiCol[iLoopCount].DataInfo._dt_enum));
        iOffset += GetDatasizeNETPI(aobjNetPiCol[iLoopCount].DataInfo._dt_enum);
		strcat(pbBuffer , " ");
		strcat(pbBuffer , aobjNetPiCol[iLoopCount].ModuleName);
		strcat(pbBuffer , "_");
		strcat(pbBuffer , aobjNetPiCol[iLoopCount].Name);
		if(aobjNetPiCol[iLoopCount].count > 0)
		{
			strcat(pbBuffer , "_to_");
			strcat(pbBuffer , aobjNetPiCol[iLoopCount].LastName);
		}
		strcat(pbBuffer , ";\n");

	}

    char* abTotalsize = new char[20];
    _IntToAscii(iTotalsize, abTotalsize, 10);

	if(enumDirType == INPUT)
	{
		strcpy(pbBuffer1, "\n\t/// <summary>\n");
        strcat(pbBuffer1, "\t/// Struct : ProcessImage Out\n");
		strcat(pbBuffer1, "\t/// </summary>\n");
		strcat(pbBuffer1, "\t[StructLayout(LayoutKind.Explicit, Pack = 1, Size = ");
        strcat(pbBuffer1, abTotalsize);
        strcat(pbBuffer1, ")]\n");
        strcat(pbBuffer1, "\tpublic struct AppProcessImageOut\n");
		strcat(pbBuffer1, "\t{\n");

        strcpy(pbBuffer2, "\t}\n");
	}	
	else if(enumDirType == OUTPUT)
	{				
		
		strcpy(pbBuffer1, "\n\t/// <summary>\n");
        strcat(pbBuffer1, "\t/// Struct : ProcessImage In\n");
		strcat(pbBuffer1, "\t/// </summary>\n");
		strcat(pbBuffer1, "\t[StructLayout(LayoutKind.Explicit, Pack = 1, Size = ");
        strcat(pbBuffer1, abTotalsize);
        strcat(pbBuffer1, ")]\n");
        strcat(pbBuffer1, "\tpublic struct AppProcessImageIn\n");
		strcat(pbBuffer1, "\t{\n");

        strcpy(pbBuffer2, "\t}\n");
	}	
	delete[] abTotalsize;

	UINT32 uiStrLength =  strlen(pbBuffer1);
	
	if((uiStrLength != fwrite(pbBuffer1, sizeof(char),uiStrLength,fpNetHeader)))
	{	
		ocfmException ex;
		ex.ocfm_Excpetion(OCFM_ERR_FILE_CANNOT_OPEN);
		delete[] pbBuffer;
		delete[] pbBuffer1;
		throw ex;
	}
 	delete[] pbBuffer1;

	uiStrLength  =  strlen(pbBuffer);

	if((uiStrLength != fwrite(pbBuffer, sizeof(char), uiStrLength, fpNetHeader)))
	{	
		ocfmException ex;
		ex.ocfm_Excpetion(OCFM_ERR_FILE_CANNOT_OPEN);
		throw ex;
	}
 	delete[] pbBuffer;

	uiStrLength  =  strlen(pbBuffer2);

	if((uiStrLength != fwrite(pbBuffer2, sizeof(char), uiStrLength, fpNetHeader)))
	{	
		ocfmException ex;
		ex.ocfm_Excpetion(OCFM_ERR_FILE_CANNOT_OPEN);
		throw ex;
	}
 	delete[] pbBuffer2;

}

/****************************************************************************************************
* Function Name: GroupNETHeaderContents
* Description:
* Return value: INT32
****************************************************************************************************/
INT32 GroupNETHeaderContents(ProcessImage objProcessImage[], INT32 iNumberOfVars, EPIDirectionType enumDirType, FILE* fpNetHeader)
{

	INT32 iModuleNo 		= 0;
	INT32 iDataSize  		= 0;
	INT32 iHoleFilledIdNo 	= 1;
	INT32 iTotalsize = 0;

	if(iNumberOfVars != 0 )
	{
		iModuleNo = 0;
		CNodeCollection* pobjNodeCol = NULL;
		CNode*  objNode;
		pobjNodeCol =  CNodeCollection::getNodeColObjectPointer();

		for(INT32 iLoopCount = 0; iLoopCount<iNumberOfVars ; iLoopCount++)
		{
			objNode = pobjNodeCol->getNodePtr(CN, objProcessImage[iLoopCount].CNNodeID);
			INT32 iNodeId;
			char* pbStrCNID 	= new char[50];
			char* pbModName		= new char[50];
			char* pbStrModuleNo = new char[20];
			
			iDataSize  = objProcessImage[iLoopCount].DataInfo.DataSize;
			/* Check if 8, 16, 32 bit aligned*/
			if((iDataSize % 32 == 0 ) || 
			(iDataSize % 16 == 0 ) ||
			(iDataSize % 8 == 0 ))
			{				
				if(iTotalsize % iDataSize !=0)
				{
					char* abHoleid = new char[20];
					char* pbFbits  = new char[2 + ALLOC_BUFFER];
					INT32 iFilledBits = iDataSize - (iTotalsize % iDataSize);
					
					iTotalsize =  iTotalsize + iFilledBits;
					for(INT32 iLoopCount = 1 ; iLoopCount <= (iFilledBits / 8) ; iLoopCount++ )
                    {
                        strcpy(abHoleid, "" );
                        strcpy(pbFbits, "" );
                        NETProcessImage objNETProcessImage;
                        abHoleid = _IntToAscii(iHoleFilledIdNo, abHoleid, 10);
                        objNETProcessImage.Name = new char[ strlen(abHoleid) + ALLOC_BUFFER];
                        strcpy(objNETProcessImage.Name , abHoleid);
                        objNETProcessImage.ModuleName = new char[ strlen("PADDING_VAR") + ALLOC_BUFFER];
                        strcpy(objNETProcessImage.ModuleName , "PADDING_VAR");
            
                        objNETProcessImage.count = 0;
                        objNETProcessImage.DataInfo._dt_enum = BYTE;
                        objNETProcessImage.DataInfo.DataSize = 8;
                        objNETProcessImage.DataInfo._dt_Name = new char[strlen("byte") + ALLOC_BUFFER];
                        strcpy(objNETProcessImage.DataInfo._dt_Name, "byte");
                        objNETProcessImage.iTotalDataSize = 8;
                        objNETProcessImage.DirectionType= enumDirType;
                        objNode->addNETProcessImage(objNETProcessImage);
                        iHoleFilledIdNo =  iHoleFilledIdNo + 1;
                    }
					delete[] pbFbits;
					delete[] abHoleid;
				}				
			}
			
			iNodeId = objProcessImage[iLoopCount].CNNodeID;
		
			pbStrCNID = _IntToAscii(objProcessImage[iLoopCount].CNNodeID, pbStrCNID, 10); 
			strcpy(pbStrModuleNo, subString( objProcessImage[iLoopCount].ModuleIndex, 2, 2));
			strcpy(pbModName, objProcessImage[iLoopCount].ModuleName);
				
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
			INT32 iNETPIIndex;
			bool NetPICreated = false;
			if(objProcessImage[iLoopCount].DataInfo._dt_enum == BITSTRING)
			{
				INT32 iPIIndexCount = 0;
				do {
					iNETPIIndex = SearchModuleNameNETProcessImageCollection(objProcessImage[iLoopCount].CNNodeID, iPIIndexCount, pbVarName);
					if(iNETPIIndex == -1)
					{
						//create new
						NetPICreated = true;
						NETProcessImage objNETProcessImage;
						CopyPItoNETPICollection(objProcessImage[iLoopCount], objNETProcessImage, pbVarName);
						
					}
					else if(objNode->NETProcessImageCollection[iNETPIIndex].iTotalDataSize < 8)
					{
						NetPICreated = true;
						objNode->NETProcessImageCollection[iNETPIIndex].iTotalDataSize = objNode->NETProcessImageCollection[iNETPIIndex].iTotalDataSize + iDataSize;
						objNode->NETProcessImageCollection[iNETPIIndex].count++;
						objNode->NETProcessImageCollection[iNETPIIndex].LastName = new char[strlen(objProcessImage[iLoopCount].VarName) + ALLOC_BUFFER];
						strcpy( objNode->NETProcessImageCollection[iNETPIIndex].LastName , objProcessImage[iLoopCount].VarName );
						if(objNode->NETProcessImageCollection[iNETPIIndex].iTotalDataSize == 8)
						{
							objNode->NETProcessImageCollection[iNETPIIndex].DataInfo._dt_enum = BYTE;
							objNode->NETProcessImageCollection[iNETPIIndex].DataInfo._dt_Name = new char[strlen("byte") + ALLOC_BUFFER];
							strcpy( objNode->NETProcessImageCollection[iNETPIIndex].DataInfo._dt_Name ,"byte");
						}
                        else if(objNode->NETProcessImageCollection[iNETPIIndex].iTotalDataSize == 16)
                        {
                            objNode->NETProcessImageCollection[iNETPIIndex].DataInfo._dt_enum = UINT;
                            objNode->NETProcessImageCollection[iNETPIIndex].DataInfo._dt_Name = new char[strlen("UInt16") + ALLOC_BUFFER];
                            strcpy( objNode->NETProcessImageCollection[iNETPIIndex].DataInfo._dt_Name ,"UInt16");
                        }
                        else if(objNode->NETProcessImageCollection[iNETPIIndex].iTotalDataSize == 32)
                        {
                            objNode->NETProcessImageCollection[iNETPIIndex].DataInfo._dt_enum = UDINT;
                            objNode->NETProcessImageCollection[iNETPIIndex].DataInfo._dt_Name = new char[strlen("UInt32") + ALLOC_BUFFER];
                            strcpy( objNode->NETProcessImageCollection[iNETPIIndex].DataInfo._dt_Name ,"UInt32");
                        }
					}
					else
					{
						iPIIndexCount = iNETPIIndex+1;
					}
				} while (NetPICreated == false);
			} else {
				//copy as it is
				NETProcessImage objNETProcessImage;
				CopyPItoNETPICollection(objProcessImage[iLoopCount], objNETProcessImage, pbVarName);
			}

			iTotalsize 		= iDataSize  + iTotalsize;

			delete[] pbVarName;				
			delete[] pbModName;
			delete[] pbStrModuleNo;
			delete[] pbStrCNID;									
		}
		if(iTotalsize % 32 !=0)
		{
			char* abHoleid = new char[20];
			char* pbFbits  = new char[2 + ALLOC_BUFFER];
			INT32 iFilledBits = 32 - (iTotalsize % 32);
			
			iTotalsize =  iTotalsize + iFilledBits;
            for(INT32 iLoopCount = 1 ; iLoopCount <= (iFilledBits / 8) ; iLoopCount++ )
            {
                strcpy(abHoleid, "" );
                strcpy(pbFbits, "" );
			    NETProcessImage objNETProcessImage;
			    abHoleid = _IntToAscii(iHoleFilledIdNo, abHoleid, 10);
			    objNETProcessImage.Name = new char[ strlen(abHoleid) + ALLOC_BUFFER];
			    strcpy(objNETProcessImage.Name , abHoleid);
			    objNETProcessImage.ModuleName = new char[ strlen("PADDING_VAR") + ALLOC_BUFFER];
			    strcpy(objNETProcessImage.ModuleName , "PADDING_VAR");
    
			    objNETProcessImage.count = 0;
			    objNETProcessImage.DataInfo._dt_enum = BYTE;
			    objNETProcessImage.DataInfo.DataSize = 8;
			    objNETProcessImage.DataInfo._dt_Name = new char[strlen("byte") + ALLOC_BUFFER];
			    strcpy(objNETProcessImage.DataInfo._dt_Name, "byte");
			    objNETProcessImage.iTotalDataSize = 8;
                objNETProcessImage.DirectionType= enumDirType;
			    objNode->addNETProcessImage(objNETProcessImage);
			    iHoleFilledIdNo =  iHoleFilledIdNo + 1;
            }
            delete[] pbFbits;
            delete[] abHoleid;
		}
 	}

	iTotalsize 	=  iTotalsize / 8;	

	return iTotalsize;
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
	#ifdef DEBUG_FUNCTION_ENTRY
		cout<<"---------AddPDOIndexesToMN---------"<<endl;
	#endif
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
			//name from network point of view
			if(enumPdoType == PDO_RPDO)
			{			
				strcpy(abObjectName, "PI_INPUTS_A");
				strcat(abObjectName, getPIName(pbIndex));		
				pobjIndex->setName(abObjectName);
			}
			else if(enumPdoType == PDO_TPDO)
			{							
				strcpy(abObjectName, "PI_OUTPUTS_A");
				strcat(abObjectName, getPIName(pbIndex));	
				pobjIndex->setName(abObjectName);
			}
			else
			{
				cout << "enumPdoType is not TPDO or RPDO _1\n" << endl;
			}
			/* Add subindex 00 */
			stRetCode = AddSubIndex(MN_NODEID, MN, pbIndex, (char*) "00");
			if(stRetCode.code  ==  OCFM_ERR_SUCCESS)
			{
			}
		}
	}
	
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
				strcpy(abObjectName, "PI_INPUTS_");
				strcat(abObjectName, getPIName(pbIndex));								
				pobjSIdx->setAccessType((char*)"ro");
				
				pobjSIdx->setName(abObjectName);
				pobjSIdx->setPDOMapping(pbPdoMap);
			}
			else if(enumPdoType == PDO_TPDO)
			{				
				strcpy(pbPdoMap, "RPDO");

				strcpy(abObjectName, "PI_OUTPUTS_");
				strcat(abObjectName, getPIName(pbIndex));	
				pobjSIdx->setAccessType((char*)"rw");
				
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
	#ifdef DEBUG_FUNCTION_ENTRY
		cout<<"--------------GetPIAddress----------------"<<endl;	
	#endif
	stPIObject.Index = new char[INDEX_LEN];
	stPIObject.SubIndex = new char[SUBINDEX_LEN];

	for(i = 0; i< NO_OF_PI_ENTERIES; i++)
	{
		if((AddressTable[i].dt == dt) && (AddressTable[i].Direction == dirType))
		{
			INT32 iTempDataSize = iDataSize/8;
			subIndex = (iOffset)/ iTempDataSize + 1;
			#ifdef DEBUG_INDEX_LEVEL
				cout<<"GetPIAddress--subIndex: "<<subIndex<<" iTempDataSize: "<<iTempDataSize<<" iDatasize: "<<iDataSize<<endl;
			#endif
			if(subIndex > 254)
			{
				int div = subIndex / 254;
				int mod = subIndex % 254;
				int iAddress;
				/* PATCH For BUG #11 - START */
				iAddress = hex2int(AddressTable[i].Address);
				/* PATCH For BUG #11 - END */
				#ifdef DEBUG_INDEX_LEVEL
					cout<<"GetPIAddress iAddress: "<<iAddress<<" ,the value: "<<AddressTable[i].Address<<endl;
				#endif
				iAddress = iAddress + div;
				stPIObject.Index  = _IntToAscii(iAddress, stPIObject.Index, 16);								
				stPIObject.SubIndex =  _IntToAscii(mod, 	stPIObject.SubIndex, 16);
				stPIObject.SubIndex = padLeft(	stPIObject.SubIndex, '0', 2);
				#ifdef DEBUG_FUNCTION_ENTRY
					cout<<"GetPIAddress---stPIObject.Index: "<<stPIObject.Index <<" ,the value: "<<stPIObject.SubIndex<<endl;
				#endif
			}
			else
			{			
				strcpy(stPIObject.Index, AddressTable[i].Address);
				stPIObject.SubIndex =  _IntToAscii(subIndex, 	stPIObject.SubIndex, 16);
				stPIObject.SubIndex = padLeft(	stPIObject.SubIndex, '0', 2);
				#ifdef DEBUG_FUNCTION_ENTRY
					cout<<"GetPIAddress: "<<AddressTable[i].Address<<endl;						
					cout<<"GetPIAddress: "<<stPIObject.Index <<" ,the value: "<<stPIObject.SubIndex<<endl;
				#endif
			}			
		}		
	}
	//Handled error case and returned dummy value to avoid warning
	return stPIObject;
}

/****************************************************************************************************
* Function Name: getPIDataTypeName
* Description:
* Return value: char*
****************************************************************************************************/
char* getPIDataTypeName(char* pbAddress)
{
	#ifdef DEBUG_FUNCTION_ENTRY
		cout<<"--------------getPIDataTypeName----------------"<<endl;	
	#endif
	char *pbRetString = NULL;
	PDODataType dt;
	for(INT32 iLoopCount = 0; iLoopCount < NO_OF_PI_ENTERIES; iLoopCount++)
	{		
		/* PATCH For BUG #18 - START */
		if(strcmp(AddressTable[iLoopCount].Address,  pbAddress) == 0)
		{
			dt = AddressTable[iLoopCount].dt;
		}		
		/* PATCH For BUG #18 - END */
			/* PATCH For BUG #11 and 12 - START */
		else if(strcmp(AddressTable[iLoopCount].Address,  pbAddress) > 0)
		{
			 dt = (iLoopCount > 0) ? AddressTable[iLoopCount - 1].dt : static_cast<PDODataType>(-1);
		}
		else
		{
			// TODO:
		}	
			switch(dt)
			/* PATCH For BUG #11 and 12 - END */
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
					pbRetString = (char*) "Error";
					break;
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
	#ifdef DEBUG_FUNCTION_ENTRY
		cout<<"--------------getPIName----------------"<<pbAddress<<endl;	
	#endif
	char *pbRetString = NULL;
	PDODataType dt;
	for(INT32 iLoopCount = 0; iLoopCount < NO_OF_PI_ENTERIES; iLoopCount++)
	{		
		/* PATCH For BUG #18 - START */
		if(strcmp(AddressTable[iLoopCount].Address,  pbAddress) == 0)
		{
			dt = AddressTable[iLoopCount].dt;
		}		
		/* PATCH For BUG #18 - END */		
			/* PATCH For BUG #11 and 12 - START */
		else if(strcmp(AddressTable[iLoopCount].Address,  pbAddress) > 0)
		{
			 dt = (iLoopCount > 0) ? AddressTable[iLoopCount - 1].dt : static_cast<PDODataType>(-1);
		}
		else
		{
			// TODO:
		}	
		switch(dt)
		/* PATCH For BUG #11 and 12 - END */
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
				pbRetString = (char*) "Err";
				break;
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
	#ifdef DEBUG_FUNCTION_ENTRY
		cout<<"--------------CheckIfProcessImageIdx----------------"<<endl;	
	#endif
	/* PATCH For BUG #12 - START */
	return ((strcmp(pbIndex, "A000") >= 0) && (strcmp(pbIndex, "AFFF") <= 0));
	/* PATCH For BUG #12 - END */	

/*
	for(INT32 iLoopCount = 0; iLoopCount< NO_OF_PI_ENTERIES; iLoopCount++)
	{		
		if(!strcmp(AddressTable[iLoopCount].Address,  pbIndex))
		{
			return true;
		}
	}
	return false; */
}

/**************************************************************************************************
* Function Name: SearchModuleNameNETProcessImageCollection
* Description: 
* Return value: INT32
****************************************************************************************************/
INT32 SearchModuleNameNETProcessImageCollection(int CNNodeID, INT32 iItemLoopCount, char* schModuleName )
{

	CNodeCollection* pobjNodeCol = NULL;
	CNode*  objNode;
	pobjNodeCol =  CNodeCollection::getNodeColObjectPointer();
	objNode = pobjNodeCol->getNodePtr(CN, CNNodeID);

	INT32 iItemCount = objNode->NETProcessImageCollection.Count();
	for(; iItemLoopCount < iItemCount; iItemLoopCount++)
	{
		if(objNode->NETProcessImageCollection[iItemLoopCount].ModuleName == NULL)
			continue;

		if(strcmp(objNode->NETProcessImageCollection[iItemLoopCount].ModuleName, schModuleName) == 0)
			return iItemLoopCount;

	}
	return -1;
}

// /**************************************************************************************************
// * Function Name: CopyPItoNETPICollection
// * Description: 
// * Return value: void
// ****************************************************************************************************/
void CopyPItoNETPICollection(ProcessImage objProcessImage, NETProcessImage objNETProcessImage, char* ModuleName)
{
	CNodeCollection* pobjNodeCol = NULL;
	CNode*  objNode;
	pobjNodeCol =  CNodeCollection::getNodeColObjectPointer();
	objNode = pobjNodeCol->getNodePtr(CN, objProcessImage.CNNodeID);

	//NETProcessImage objNETProcessImage;
	if(objProcessImage.VarName != NULL)
	{
		objNETProcessImage.Name = new char[strlen(objProcessImage.VarName)+ALLOC_BUFFER];
		strcpy(objNETProcessImage.Name, objProcessImage.VarName);
	}
	objNETProcessImage.ModuleName = new char[strlen(ModuleName)+ALLOC_BUFFER];
	strcpy(objNETProcessImage.ModuleName, ModuleName);
	objNETProcessImage.DataInfo._dt_enum = objProcessImage.DataInfo._dt_enum;
	objNETProcessImage.DataInfo.DataSize = objProcessImage.DataInfo.DataSize;
	if(objProcessImage.DataInfo._dt_Name != NULL)
	{
		objNETProcessImage.DataInfo._dt_Name = new char[strlen(objProcessImage.DataInfo._dt_Name) + ALLOC_BUFFER];
		strcpy(objNETProcessImage.DataInfo._dt_Name, objProcessImage.DataInfo._dt_Name);
	}
	objNETProcessImage.CNNodeID = objProcessImage.CNNodeID;
	objNETProcessImage.DirectionType = objProcessImage.DirectionType;
	objNETProcessImage.count = 0;
	objNETProcessImage.iTotalDataSize = objProcessImage.DataInfo.DataSize;

	objNode->addNETProcessImage(objNETProcessImage);
}

char* GetDatatypeNETPI(IEC_Datatype dt_enum)
{
    char* pcDTNetString;
    switch(dt_enum)
    {
        case BITSTRING:
            pcDTNetString = (char*)"";
            break;
        case BOOL:
            pcDTNetString = (char*)"byte";
            break;
        case BYTE:
            pcDTNetString = (char*)"byte";
            break;
        case _CHAR:
            pcDTNetString = (char*)"byte";
            break;
        case DWORD:
            pcDTNetString = (char*)"UInt32";
            break;
        case LWORD:
            pcDTNetString = (char*)"UInt64";
            break;
        case SINT:
            pcDTNetString = (char*)"sbyte";
            break;
        case INT:
            pcDTNetString = (char*)"Int16";
            break;
        case DINT:
            pcDTNetString = (char*)"Int32";
            break;
        case LINT:
            pcDTNetString = (char*)"Int64";
            break;
        case USINT:
            pcDTNetString = (char*)"byte";
            break;
        case UINT:
            pcDTNetString = (char*)"UInt16";
            break;
        case UDINT:
            pcDTNetString = (char*)"UInt32";
            break;
        case ULINT:
            pcDTNetString = (char*)"UInt64";
            break;
        case REAL:
            pcDTNetString = (char*)"";
            break;
        case LREAL:
            pcDTNetString = (char*)"";
            break;
        case STRING:
            pcDTNetString = (char*)"";
            break;
        case WSTRING:
            pcDTNetString = (char*)"";
            break;
        default:
            pcDTNetString = (char*)"";
            break;
    }
    return pcDTNetString;
}

int GetDatasizeNETPI(IEC_Datatype dt_enum)
{
    int iDSNetString = 0;
    switch(dt_enum)
    {
        case BOOL:
        case BYTE:
        case _CHAR:
        case SINT:
        case USINT:
            iDSNetString = 1;
            break;
        case INT:
        case UINT:
            iDSNetString = 2;
            break;
        case DINT:
        case DWORD:
        case UDINT:
            iDSNetString = 4;
            break;
        case LINT:
        case LWORD:
        case ULINT:
            iDSNetString = 8;
            break;
        default:
            break;
    }
    return iDSNetString;
}
