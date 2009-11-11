
///////////////////////////////////////////////////////////////////////////////////////////////
//
//  $Source: $
//
// NAME:  XdcOperations.cpp
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
#include <libxml/xmlreader.h>
#include <errno.h>
#include <string.h>
#include "../Include/openCONFIGURATOR.h"
#include "../Include/Declarations.h"
#include "../Include/Exception.h"

/****************************************************************************************************
* Defines
****************************************************************************************************/
#define MY_ENCODING "UTF-8"

/****************************************************************************************************
* Global Variables
****************************************************************************************************/
INT32 LastIndexParsed = 0;
static const char *gpa2bSimple[][2] = {
										{"BOOL" ,    "1"},
										{"BITSTRING","1"},
										{"BYTE",     "8"},
										{"CHAR",     "1"},
										{"WORD",     "16"},
										{"DWORD",    "32"},
										{"LWORD",    "64"},
										{"SINT",     "8"},
										{"INT",      "16"},
										{"DINT",     "32"},
										{"LINT",     "64"},
										{"USINT",    "8"},
										{"UINT",     "16	"},
										{"UDINT",    "32"},
										{"ULINT",    "64"},
										{"REAL",     "16"},
										{"LREAL",    "64"},
										{"STRING",   "1"},
										{"WSTRING",  "1"}
									};
																										
/****************************************************************************************************
* FUNCTION DEFINITIONS
****************************************************************************************************/

/**************************************************************************************************
* Function Name: setIndexAttributes
* Description:	
* Return value: void
****************************************************************************************************/
void setIndexAttributes(xmlTextReaderPtr pxtrReader, CIndex *pobjIndex, bool& bhasPDO)
{
	const xmlChar *pxcName  = NULL;
	const xmlChar *pxcValue = NULL;
	// For holding temp data to print
	char *pbPrintData	= NULL;
	
	//Retrieve the pxcName and Value of an attribute
	pxcValue = xmlTextReaderConstValue(pxtrReader);
	pxcName  = xmlTextReaderConstName(pxtrReader);				
	
	if(!strcmp(ConvertToUpper((char*)pxcName), "INDEX"))
	{			
		// Setting the Index Value
		pobjIndex->setIndexValue((char*)pxcValue);
		
		// For Testing. 	
		pbPrintData = new char[sizeof(pobjIndex->getIndexValue())];
		strcpy(pbPrintData, pobjIndex->getIndexValue());
		
		/*$S Commented out the TPDO and RPDO grouping $S*/
		//TODO: Check point 1 
		char *pbTempValue = (char*) pxcValue;

		if( (!strncmp(pbTempValue,"14",2)) || (!strncmp(pbTempValue,"16", 2)))
		{
			pobjIndex->setPDOType(PDO_RPDO);
			bhasPDO  = true;
		}
		else if((!strncmp(pbTempValue, "18", 2 ))|| (!strncmp(pbTempValue,"1A", 2)))
		{
			pobjIndex->setPDOType(PDO_TPDO);
			bhasPDO		= true;
		}
		delete [] pbPrintData;
	}
	else if( !(strcmp(ConvertToUpper((char*)pxcName), "NAME")) )
	{	
		pobjIndex->setName((char*)pxcValue);
	}
	else if(!strcmp(ConvertToUpper((char*)pxcName), "OBJECTTYPE"))
	{
		pobjIndex->setObjectType((char*)pxcValue);
	}
	else if(!strcmp(ConvertToUpper((char*)pxcName), "LOWLIMIT"))
	{
		pobjIndex->setLowLimit((char*)pxcValue);		
	}
	else if(!strcmp(ConvertToUpper((char*)pxcName), "HIGHLIMIT"))
	{
		pobjIndex->setHighLimit((char*)pxcValue);
	}
	else if(!strcmp(ConvertToUpper((char*)pxcName), "ACCESSTYPE"))
	{
		pobjIndex->setAccessType((char*)pxcValue);
	}
	else if(!strcmp(ConvertToUpper((char*)pxcName), "PDOMAPPING"))
	{
		pobjIndex->setPDOMapping((char*)pxcValue);
	}
	else if(!strcmp(ConvertToUpper((char*)pxcName), "DEFAULTVALUE"))					
	{
		pobjIndex->setDefaultValue((char*)pxcValue);
	}
	else if(!strcmp(ConvertToUpper((char*)pxcName), "ACTUALVALUE"))					
	{
		pobjIndex->setActualValue((char*)pxcValue);
	}
	else if(!strcmp(ConvertToUpper((char*)pxcName), "DATATYPE"))					
	{
		if(CheckIfDataTypeExists((char*)pxcValue, pobjIndex->getNodeID()))
		{
			pobjIndex->setDataType((char*)pxcValue);		
		}
		else
		{				
			ocfmException ex;	
			ex.ocfm_Excpetion(OCFM_ERR_DATATYPE_NOT_FOUND);
		}
	}
	else if(!strcmp(ConvertToUpper((char*)pxcName), "UNIQUEIDREF"))					
	{
		pobjIndex->setUniqueIDRef((char*)pxcValue);		
	}
	else if(!strcmp(ConvertToUpper((char*)pxcName), "CDCFLAG"))
	{
		if(!strcmp(ConvertToUpper((char*)pxcValue), "FALSE"))
		{
			pobjIndex->setFlagIfIncludedCdc(FALSE);
		}
		else if(!strcmp(ConvertToUpper((char*)pxcValue), "TRUE"))
		{
			pobjIndex->setFlagIfIncludedCdc(TRUE);
		}
	}		
}
	
/**************************************************************************************************
* Function Name: setSubIndexAttributes
* Description:	
* Return Value: void
****************************************************************************************************/
void setSubIndexAttributes(xmlTextReaderPtr pxtrReader, CSubIndex *pobjSubIndex)
{
	const xmlChar *pxcName  = NULL;
	const xmlChar *pxcValue = NULL;
	//Retrieve the pxcName and Value of an attribute
	pxcValue = xmlTextReaderConstValue(pxtrReader);
	pxcName  = xmlTextReaderConstName(pxtrReader);

	if(!strcmp(ConvertToUpper((char*)pxcName), "SUBINDEX"))
	{
		pobjSubIndex->setIndexValue((char*)pxcValue);
	}
	else if(!strcmp(ConvertToUpper((char*)pxcName), "NAME"))					
	{
		pobjSubIndex->setName((char*)pxcValue);
	}
	else if(!strcmp(ConvertToUpper((char*)pxcName), "OBJECTTYPE"))					
	{
		pobjSubIndex->setObjectType((char*)pxcValue);
	}
	else if(!strcmp(ConvertToUpper((char*)pxcName), "LOWLIMIT"))
	{
		pobjSubIndex->setLowLimit((char*)pxcValue);	
	}
	else if(!strcmp(ConvertToUpper((char*)pxcName), "HIGHLIMIT"))					
	{
		pobjSubIndex->setHighLimit((char*)pxcValue);
	}
	else if(!strcmp(ConvertToUpper((char*)pxcName), "ACCESSTYPE"))					
	{
		pobjSubIndex->setAccessType((char*)pxcValue);
	}
	else if(!strcmp(ConvertToUpper((char*)pxcName), "PDOMAPPING"))					
	{
		pobjSubIndex->setPDOMapping((char*)pxcValue);
	}
	else if(!strcmp(ConvertToUpper((char*)pxcName), "DEFAULTVALUE"))
	{
		pobjSubIndex->setDefaultValue((char*)pxcValue);
	}
	else if(!strcmp(ConvertToUpper((char*)pxcName), "ACTUALVALUE"))					
	{
		pobjSubIndex->setActualValue((char*)pxcValue);
	}
	else if(!strcmp(ConvertToUpper((char*)pxcName), "DATATYPE"))
	{
		pobjSubIndex->setDataType((char*)pxcValue);		
	}
	else if(!strcmp(ConvertToUpper((char*)pxcName), "UNIQUEIDREF"))					
	{
		pobjSubIndex->setUniqueIDRef((char*)pxcValue);
	}
	else if(!strcmp(ConvertToUpper((char*)pxcName), "CDCFLAG"))
	{
		if(!strcmp(ConvertToUpper((char*)pxcValue), "FALSE"))
		{
			pobjSubIndex->setFlagIfIncludedCdc(FALSE);
		}
		else if(!strcmp(ConvertToUpper((char*)pxcValue), "TRUE"))
		{
			pobjSubIndex->setFlagIfIncludedCdc(TRUE);
		}
	}
	else
	{
		#ifdef DEBUF
		cout <<"Error! setSubIndexAttributes fails\n"<<endl;
		#endif
	}													
}

/**************************************************************************************************
* Function Name: setDataTypeAttributes
* Description:	
* Return Value: void
****************************************************************************************************/
void setDataTypeAttributes(xmlTextReaderPtr pxtrReader ,DataType *objDataType)
{
	const xmlChar *pxcName  = NULL;
	const xmlChar *pxcValue = NULL;		
	INT32 iRetVal;

	objDataType->Name 			= NULL;
	objDataType->DataTypeValue 	= NULL;
	objDataType->DataSize 		= NULL;
		
	//Retrieve the pxcName and Value of an attribute
	pxcValue = xmlTextReaderConstValue(pxtrReader);
	pxcName  = xmlTextReaderConstName(pxtrReader);

	if(!strcmp(ConvertToUpper((char*)pxcName), "DATATYPE"))						
	{
		objDataType->DataTypeValue = new char[strlen((char*)pxcValue) + 1];
		strcpy(objDataType->DataTypeValue, (char*)pxcValue);
		//Read the Equivalent pxcName of a datatype
		iRetVal = xmlTextReaderRead(pxtrReader);
		if(iRetVal != 1)
		{			
			ocfmException objException;						
			objException.ocfm_Excpetion(OCFM_ERR_XML_FILE_CORRUPTED);
			throw objException;				
		}
		while(xmlTextReaderNodeType(pxtrReader) != XML_READER_TYPE_ELEMENT)
		{			
			iRetVal = xmlTextReaderRead(pxtrReader);			
			if(iRetVal!=1)
			{			
				ocfmException objException;						
				objException.ocfm_Excpetion(OCFM_ERR_XML_FILE_CORRUPTED);
				throw objException;
			}			
		}
		pxcValue 	= xmlTextReaderConstValue(pxtrReader);
		pxcName 	= xmlTextReaderConstName(pxtrReader);
						
		objDataType->Name = new char[strlen((char*)pxcName) + ALLOC_BUFFER];		
		strcpy(objDataType->Name, (char*)pxcName);
		
		objDataType->DataSize = getDataSize(objDataType->Name);
		if(strcmp(objDataType->Name,"Unsigned8")==0 || strcmp(objDataType->Name,"UNSIGNED8")==0 )
		{
			objDataType->IEC_dt = USINT;			
		}
		else if(strcmp(objDataType->Name,"Boolean")==0 || strcmp(objDataType->Name,"BOOLEAN")==0)
		{
			objDataType->IEC_dt = BOOL;			
		}
		else if(strcmp(objDataType->Name,"Integer8")==0 || strcmp(objDataType->Name,"INTEGER8")==0)
		{
			objDataType->IEC_dt = SINT;	
		}		
		else if(strcmp(objDataType->Name,"Unsigned16")==0 || strcmp(objDataType->Name,"UNSIGNED16")==0)
		{
			objDataType->IEC_dt = UINT;		
		}
		else if(strcmp(objDataType->Name,"Integer16")==0 || strcmp(objDataType->Name,"INTEGER16")==0)
		{
			objDataType->IEC_dt = INT;
		}
		else if(strcmp(objDataType->Name,"Unsigned32")==0 || strcmp(objDataType->Name,"UNSIGNED32")==0)
		{
			objDataType->IEC_dt = UDINT;
		}
		else if(strcmp(objDataType->Name,"Integer32")==0 || strcmp(objDataType->Name,"INTEGER32")==0)
		{
			objDataType->IEC_dt = DINT;		
		}
		else if(strcmp(objDataType->Name,"Real32")==0 || strcmp(objDataType->Name,"REAL32")==0)
		{
			objDataType->IEC_dt = REAL;		
		}
		
		/*if((strcmp(objDataType->Name,"Unsigned8")==0) ||
			(strcmp(objDataType->Name,"Boolean")==0 ) ||
			(strcmp(objDataType->Name,"Integer8")==0 ))
				strcpy(objDataType->DataSize,"00000001");

		if((strcmp(objDataType->Name,"Unsigned16")==0) ||
			(strcmp(objDataType->Name,"Integer16")==0 ))
			strcpy(objDataType->DataSize,"00000002");

		if((strcmp(objDataType->Name,"Unsigned32")==0) ||
			(strcmp(objDataType->Name,"Integer32")==0 ) ||
			(strcmp(objDataType->Name,"Real32")==0 ))
			strcpy(objDataType->DataSize,"00000004");*/
			
		else if(strcmp(objDataType->Name,"Unsigned64")==0 || strcmp(objDataType->Name,"UNSIGNED64")==0)
		{
			objDataType->IEC_dt = LREAL;
		}		
	}
}

/**************************************************************************************************
* Function Name: setParameterAttributes
* Description:	
* Return Value: void
****************************************************************************************************/
void setParameterAttributes(xmlTextReaderPtr pxtrReader, Parameter *pstParameter)
{
		const xmlChar *pxcName  = NULL;
		const xmlChar *pxcValue = NULL;
		//Retrieve the pxcName and Value of an attribute
		pxcValue = xmlTextReaderConstValue(pxtrReader);
		pxcName  = xmlTextReaderConstName(pxtrReader);					

		if(!strcmp(ConvertToUpper((char*)pxcName), "UNIQUEID"))
		{						
			pstParameter->name_id_dt_attr.setUniqueID((char*)pxcValue);
		}
		else if(!strcmp(ConvertToUpper((char*)pxcName), "NAME"))					
		{									
			pstParameter->name_id_dt_attr.setName((char*)pxcValue);		
		}
		else if(!strcmp(ConvertToUpper((char*)pxcName), "ACCESS"))					
		{						
			pstParameter->access  = new char[strlen((const char*)pxcValue) + 1];			
			strcpy(pstParameter->access, (const char*)pxcValue);
		}

		/* TO DO: DATATYPE..There is no tag for it..need to check after how many reads datatype is define </label>
            <USINT/>*/	
}

/**************************************************************************************************
* Function Name: getParaDT
* Description:	
* Return Value: void
****************************************************************************************************/
void getParaDT(xmlTextReaderPtr pxtrReader, Parameter* pstParameter)
{
	const xmlChar *pxcName  = NULL;
	const xmlChar *pxcValue = NULL;
	INT32 iRetVal;
  
	iRetVal = xmlTextReaderRead(pxtrReader);
  
	if(iRetVal!=1)
	{
		ocfmException objException;						
		objException.ocfm_Excpetion(OCFM_ERR_XML_FILE_CORRUPTED);
		throw objException;
	}
  
	pxcName = xmlTextReaderConstName(pxtrReader);
	pxcValue = xmlTextReaderConstValue(pxtrReader);
	
	while(!(CheckEndElement(xmlTextReaderNodeType(pxtrReader),(char*)pxcName, (char*)"parameter")))
	{
		iRetVal = xmlTextReaderRead(pxtrReader);
	
		if(iRetVal!=1)
		{
			ocfmException objException;						
			objException.ocfm_Excpetion(OCFM_ERR_XML_FILE_CORRUPTED);
			throw objException;
		}
		
		pxcValue = xmlTextReaderConstValue(pxtrReader);
		pxcName =xmlTextReaderConstName(pxtrReader);			
		
		char abSize[3];
		
		if(CheckifSimpleDT((char*)pxcName, abSize))
		{
				pstParameter->name_id_dt_attr.setDataType((char*)pxcName);
		}		
		if(CheckStartElement(xmlTextReaderNodeType(pxtrReader),(char*)pxcName, (char*)"dataTypeIDRef"))
		{
			if (xmlTextReaderHasAttributes(pxtrReader) == TRUE)
			{
			
				xmlTextReaderMoveToNextAttribute(pxtrReader);
				pxcValue = xmlTextReaderConstValue(pxtrReader);
				pxcName =xmlTextReaderConstName(pxtrReader);							

				if(strcmp(ConvertToUpper((char*)pxcName), "UNIQUEIDREF") == FALSE)
				{
					pstParameter->name_id_dt_attr.setDtUniqueRefId((char*)pxcValue);
				}
			}		
		}
	}
}
	
/**************************************************************************************************
* Function Name: setCDTAttributes
* Description:	
* Return pxcValue: void
****************************************************************************************************/
static void setCDTAttributes(xmlTextReaderPtr pxtrReader, CComplexDataType *pobjCDT)
{
	const xmlChar *pxcName  = NULL;
	const xmlChar *pxcValue = NULL;
	
	//Retrieve the pxcName and Value of an attribute
	pxcValue = xmlTextReaderConstValue(pxtrReader);
	pxcName  = xmlTextReaderConstName(pxtrReader);							

	if(strcmp(ConvertToUpper((char*)pxcName), "UNIQUEID")==0)
	{						
		pobjCDT->name_id_attr->setUniqueID((char*)pxcValue);
	}

	else if(strcmp(ConvertToUpper((char*)pxcName), "NAME")==0)					
	{						
		pobjCDT->name_id_attr->setName((char*)pxcValue);			
	}		
}
	
/**************************************************************************************************
* Function Name: CheckifSimpleDT
* Description:	
* Return pxcValue: bool
****************************************************************************************************/
bool CheckifSimpleDT(char *pbName, char *pbSize)
{
	INT32 iLoopCount 		= 0;
	char *pbSimpleElement	= NULL;
	
	while(iLoopCount < g_simple_arr_size )
	{
		pbSimpleElement = (char*)gpa2bSimple[iLoopCount][0];
		
		if(!strcmp(pbSimpleElement, pbName))
		{
			strcpy(pbSize, (char*)gpa2bSimple[iLoopCount][1]);
			return true;
		}
		
		iLoopCount++; 	 
	}
	return false;	
}

/**************************************************************************************************
* Function Name: setVarDecAttributes
* Description:	
* Return pxcValue: void
****************************************************************************************************/
void setVarDecAttributes(xmlTextReaderPtr pxtrReader, varDeclaration& vdecl)
{
	const xmlChar *pxcName  = NULL;
	const xmlChar *pxcValue = NULL;
	
	//Retrieve the pxcName and Value of an attribute
	pxcValue = xmlTextReaderConstValue(pxtrReader);
	pxcName =xmlTextReaderConstName(pxtrReader);		
	//bool vardecCompleted = false;
						
	if(!strcmp(ConvertToUpper((char*)pxcName), "UNIQUEID"))
	{						
		vdecl.nam_id_dt_attr->setUniqueID((char*)pxcValue);
	}
	else if(!strcmp(ConvertToUpper((char*)pxcName), "NAME"))					
	{						
		vdecl.nam_id_dt_attr->setName((char*)pxcValue);			
	}
	if(!strcmp(ConvertToUpper((char*)pxcName), "SIZE"))					
	{						
		strcpy((char*)vdecl.size, (const char*)pxcValue);
	}
	/*	vdecl->nam_id_dt_attr = app;*/
}

/**************************************************************************************************
* Function Name: CheckEndElement
* Description:	
* Return pxcValue: bool
****************************************************************************************************/
bool CheckEndElement(INT32 iNodeType, char *pbElement, char *pbCompareWith)
{
	if((iNodeType == XML_READER_TYPE_END_ELEMENT) && (!strcmp(pbElement, pbCompareWith)))
	{
		return true;
	}
	else
	{
		return false;
	}
} 
	
/**************************************************************************************************
* Function Name: CheckStartElement
* Description:	
* Return pxcValue: bool
****************************************************************************************************/
bool CheckStartElement(INT32 iNodeType, char *pbElement, char *pbCompareWith)
{
	if((iNodeType == XML_READER_TYPE_ELEMENT) && (!strcmp(pbElement, pbCompareWith)))
	{
		return true;
	}
	else
	{
		return false;
	}
} 
	
/**************************************************************************************************
* Function Name: getVarDeclaration
* Description:	
* Return pxcValue: void
****************************************************************************************************/
static void getVarDeclaration(xmlTextReaderPtr pxtrReader, CComplexDataType *pobjCDT)
{
	const xmlChar *pxcName  = NULL;
	const xmlChar *pxcValue = NULL;
	INT32 iRetVal;
	varDeclaration ObjVarDecl;
  
	ObjVarDecl.Initialize();
	iRetVal = xmlTextReaderRead(pxtrReader);
  
	if(iRetVal != 1)
	{
		ocfmException objException;						
		objException.ocfm_Excpetion(OCFM_ERR_XML_FILE_CORRUPTED);
		throw objException;
	}
  
	pxcName  = xmlTextReaderConstName(pxtrReader);
	pxcValue = xmlTextReaderConstValue(pxtrReader);
   
	while(!(CheckEndElement(xmlTextReaderNodeType(pxtrReader),(char*)pxcName, (char*)"struct")))
	{
		varDeclaration objTempVarDecl;
		objTempVarDecl.Initialize();
			
		try
		{
			iRetVal = xmlTextReaderRead(pxtrReader);
			
			if (iRetVal != 1) 
			{
				ocfmException objException;// = new ocfmException;
				objException.ocfm_Excpetion(OCFM_ERR_XML_FILE_CORRUPTED);
				throw &objException;
			}		  
		}
		catch(ocfmException *ex)
		{
			throw ex;
		}
		pxcName = xmlTextReaderConstName(pxtrReader);
		pxcValue = xmlTextReaderConstValue(pxtrReader);
		//printf("\nName:%s",pxcName);
		//printf("\n NodeType: %d",xmlTextReaderNodeType(pxtrReader));
			
		if(CheckEndElement(xmlTextReaderNodeType(pxtrReader),(char*)pxcName, (char*)"varDeclaration"))
		{
			ObjVarDecl.StructUniqueId = (char*)malloc(strlen(pobjCDT->name_id_attr->getUniqueID() +1));
			strcpy(ObjVarDecl.StructUniqueId , pobjCDT->name_id_attr->getUniqueID());
			pobjCDT->addVarDeclaration(ObjVarDecl);
			ObjVarDecl = objTempVarDecl;
		}
		if(CheckStartElement(xmlTextReaderNodeType(pxtrReader),(char*)pxcName, (char*)"varDeclaration"))
		{
			//printf("\nELEMENT----- Name = %s ",pxcName);
			if (pxcValue==NULL)
			{
				//printf("Value = NULL \n");  
			}
			else
			{
				//printf("Value= %s",pxcValue);
			}
			if (xmlTextReaderHasAttributes(pxtrReader)==1)
			{						
				while(xmlTextReaderMoveToNextAttribute(pxtrReader))
				{
					setVarDecAttributes(pxtrReader, ObjVarDecl);
					pxcValue = xmlTextReaderConstValue(pxtrReader);
					pxcName =xmlTextReaderConstName(pxtrReader);					
				}
			}
		}						
		/*	iRetVal = xmlTextReaderRead(pxtrReader);
		pxcName = xmlTextReaderConstName(pxtrReader);
		pxcValue = xmlTextReaderConstValue(pxtrReader);*/
		char abSize[3];
		
		if(CheckifSimpleDT((char*)pxcName, abSize))
		{
			ObjVarDecl.nam_id_dt_attr->setDataType((char*)pxcName);
			
			if(!strcmp(ObjVarDecl.size,""))
			{
				strcpy(ObjVarDecl.size, abSize);
			}
		}		
		if(CheckStartElement(xmlTextReaderNodeType(pxtrReader), (char*)pxcName, (char*)"dataTypeIDRef"))
		{
			if (xmlTextReaderHasAttributes(pxtrReader)==1)
			{
			
				xmlTextReaderMoveToNextAttribute(pxtrReader);
				pxcValue = xmlTextReaderConstValue(pxtrReader);
				pxcName =xmlTextReaderConstName(pxtrReader);							

				if(!strcmp(ConvertToUpper((char*)pxcName), "UNIQUEIDREF"))
				{
					ObjVarDecl.nam_id_dt_attr->setDtUniqueRefId((char*)pxcValue);
				}
			}		
		}											
	}
}
	
/**************************************************************************************************
* Function Name: ImportXML
* Description: Imports the XML file
* Return pxcValue: ocfmRetCode
****************************************************************************************************/
ocfmRetCode ImportXML(char *pbFileName, INT32 iNodeID, ENodeType enumNodeType)
{	
	ocfmRetCode stErrStruct;	
	try
	{
		stErrStruct = parseFile(pbFileName, iNodeID, enumNodeType);
		
		if(stErrStruct.code != 0)
		{
			return stErrStruct;
		}
		xmlCleanupParser();
		/*
		* this is to debug memory for regression tests
		*/
		xmlMemoryDump();
	
		/* Add other required index*/		
		if(enumNodeType == CN)
		{
			setFlagForRequiredCNIndexes(iNodeID);
			/* Not required only in case of autogenartion 1020 of CN should be updated else it shud be as it is imported*/
			//AddOtherRequiredCNIndexes(iNodeID);
		}		
		if(enumNodeType == MN)
		{
			setFlagForRequiredMNIndexes(iNodeID);
			
		}		
		/*if(enumNodeType == MN)
		{
			AddOtherMNIndexes(iNodeID);
		}*/
		
		/* Copy default value of pdos to act value*/
		copyPDODefToAct(iNodeID, enumNodeType);	
		
		stErrStruct.code = OCFM_ERR_SUCCESS;
		return stErrStruct;
	}
	catch(ocfmException& ex)
	{
		return ex._ocfmRetCode;
	}	
}

/**************************************************************************************************
* Function Name: processNode
* Description: Process the Node pxcValue,Name and its attributes
* Return pxcValue: void
****************************************************************************************************/
void processNode(xmlTextReaderPtr pxtrReader, ENodeType enumNodeType, INT32 iNodeIndex)
{
	const xmlChar 	*pxcName  			= NULL;
	const xmlChar 	*pxcValue 			= NULL;
	CNodeCollection *pobjNodeCollection	= NULL;
	CNode 			*pobjNode			= NULL;
	
    pxcName = xmlTextReaderConstName(pxtrReader);

	pxcValue = xmlTextReaderConstValue(pxtrReader);
	try
	{ 
		//If the NodeTYPE is ELEMENT
		if( xmlTextReaderNodeType(pxtrReader) == TRUE)
		{
			if(!strcmp(((char*)pxcName),"defType"))
			{
				pobjNodeCollection= CNodeCollection::getNodeColObjectPointer();
				CDataTypeCollection* pobjDataTypeCollection;			
				//printf("createed Nodecol\n");
				DataType objDataType;
				
				if (xmlTextReaderHasAttributes(pxtrReader)==1)
				{						
					while(xmlTextReaderMoveToNextAttribute(pxtrReader))
					{
						//printf("Calling setDataTypeAttributes..\n ");
						setDataTypeAttributes(pxtrReader, &objDataType);
					}
				}
				pobjNode = pobjNodeCollection->getNodePtr(enumNodeType, iNodeIndex);							
				pobjDataTypeCollection = pobjNode->getDataTypeCollection();
				pobjDataTypeCollection->addDataType(objDataType);
			}
			else if(!strcmp(((char*)pxcName),"parameter"))
			{
				pobjNodeCollection = CNodeCollection::getNodeColObjectPointer();	
				CApplicationProcess* pobjApplicationProcess;				
				Parameter stParameter;
				
				pobjNode = pobjNodeCollection->getNodePtr(enumNodeType, iNodeIndex);				
				if (xmlTextReaderHasAttributes(pxtrReader)==1)
				{						
					while(xmlTextReaderMoveToNextAttribute(pxtrReader))
					{
							setParameterAttributes(pxtrReader,&stParameter);																															
					}
				}						
				getParaDT(pxtrReader, &stParameter);
				// Add parameter to the parameter collection of a node
				pobjApplicationProcess = pobjNode->getApplicationProcess();
				pobjApplicationProcess->addParameter(stParameter);
			}
			else if(!strcmp(((char*)pxcName),"struct"))
			{
				pobjNodeCollection= CNodeCollection::getNodeColObjectPointer();	
				CApplicationProcess* pobjApplicationProcess;		
				CComplexDataType objCDT;				
		
				pobjNode = pobjNodeCollection->getNodePtr(enumNodeType, iNodeIndex);	
				
				if (xmlTextReaderHasAttributes(pxtrReader) == 1)
				{						
					while(xmlTextReaderMoveToNextAttribute(pxtrReader))
					{
						setCDTAttributes(pxtrReader,&objCDT);																															
					}
				}
					
				getVarDeclaration(pxtrReader,&objCDT);
				// Add parameter to the parameter collection of a node
				pobjApplicationProcess = pobjNode->getApplicationProcess();
				pobjApplicationProcess->addComplexDataType(objCDT);				
			}
			else if(!strcmp(((char*)pxcName),"Object"))
			{
				pobjNodeCollection = CNodeCollection::getNodeColObjectPointer();
				CIndexCollection* pobjIndexCollection;
				CIndex objIndex;
				
				pobjNode = pobjNodeCollection->getNodePtr(enumNodeType, iNodeIndex);	
				//Set the NodeID
				objIndex.setNodeID(pobjNode->getNodeId());
				bool bhasPDO = false;

				if (xmlTextReaderHasAttributes(pxtrReader) == 1)
				{						
					while(xmlTextReaderMoveToNextAttribute(pxtrReader))
					{
						setIndexAttributes(pxtrReader, &objIndex, bhasPDO);	
					}
				}
				if(bhasPDO)
				{
					pobjNode->setFlagForPdoObjects(bhasPDO);
				}
				pobjIndexCollection = pobjNode->getIndexCollection();
				
				//Add Index object to the IndexCollection
				pobjIndexCollection->addIndex(objIndex);				
				LastIndexParsed = pobjIndexCollection->getNumberofIndexes()-1;		
			}
			else if(!strcmp(((char*)pxcName),"SubObject"))
			{
				pobjNodeCollection = CNodeCollection::getNodeColObjectPointer();
				CIndexCollection* pobjIndexCollection;
				CSubIndex objSubIndex;
				
				pobjNode = pobjNodeCollection->getNodePtr(enumNodeType,iNodeIndex);
				//Set the NodeID
				objSubIndex.setNodeID(pobjNode->getNodeId());		

				if (xmlTextReaderHasAttributes(pxtrReader) == 1)
				{			
					while(xmlTextReaderMoveToNextAttribute(pxtrReader))
					{
						setSubIndexAttributes(pxtrReader,&objSubIndex);				
					}
				}
				CIndex* pobjIndexPtr;
						
				pobjIndexCollection = pobjNode->getIndexCollection();
				pobjIndexPtr		= pobjIndexCollection->getIndex(LastIndexParsed);
				//				
				//if(strcmp(objIndexPtr->getIndexValue(),"2103")==0)
				//	{
				//		if(strcmp(objSubIndex.getIndexValue(),"17")==0)
				//			{	
				//			printf("%d",objIndexPtr->getNumberofSubIndexes());
				//			}
				//	}
				//printf("LastIndexParsed: %d-%s\n",LastIndexParsed, objIndexPtr->getIndexValue());
				//printf("SubIndex pxcValue: %s \n",objSubIndex.getIndexValue());
				pobjIndexPtr->addSubIndex(objSubIndex);
				////free memory
				//objIndexPtr->~CIndex();
				//delete objIndexPtr;
				/*printf("subindex deleted:%s",objIndexPtr->getIndexValue());*/
				//printf("Index added\n");			
			}
			else if((!strcmp(((char*)pxcName),"GeneralFeatures")) || 
					(!strcmp(((char*)pxcName),"MNFeatures")) || 
					(!strcmp(((char*)pxcName),"CNFeatures")))
			{
				pobjNodeCollection= CNodeCollection::getNodeColObjectPointer();	
				CNetworkManagement* pobjNwManagement;									
				Feature stFeature;

				pobjNode = pobjNodeCollection->getNodePtr(enumNodeType, iNodeIndex);	
				
				if (xmlTextReaderHasAttributes(pxtrReader) == 1)
				{						
					while(xmlTextReaderMoveToNextAttribute(pxtrReader))
					{
						setFeatures(pxtrReader, &stFeature);																															
					}
				}
				if(!strcmp(((char*)pxcName),"GeneralFeatures"))
				{
					stFeature.m_featureType = GENERAL_FEATURES;
					printf("General Features\n");	
				}
				else if(!strcmp(((char*)pxcName),"MNFeatures"))
				{
					stFeature.m_featureType = MN_FEATURES;
					if((!strcmp(stFeature.m_Name, "DLLMNFeatureMultiplex")) && (!strcmp(stFeature.m_Name, "true")))
					{
						pobjNode->setStationType(MULTIPLEXED);
					}
					if((!strcmp(stFeature.m_Name, "DLLMNFeatureChaining")) && (!strcmp(stFeature.m_Name, "true")))
					{
						pobjNode->setStationType(MULTIPLEXED);
					}
					printf("MN Features\n");	
				}
				else if(!strcmp(((char*)pxcName),"CNFeatures"))
				{
					stFeature.m_featureType = CN_FEATURES;
					if((!strcmp(stFeature.m_Name, "DLLCNFeatureMultiplex")) && (!strcmp(stFeature.m_Name, "true")))
					{
						pobjNode->setStationType(MULTIPLEXED);
					}
					if((!strcmp(stFeature.m_Name, "DLLCNFeatureChaining")) && (!strcmp(stFeature.m_Name, "true")))
					{
						pobjNode->setStationType(CHAINED);
					}
					printf("CN Features\n");	
				}
				// Add parameter to the parameter collection of a node
				pobjNwManagement = pobjNode->getNetworkManagement();

				printf("\n Noded %d", pobjNode->getNodeId());
				pobjNwManagement->addFeature(stFeature);
				printf("\n feature added %d", pobjNode->getNodeId());
			}
		}		
	}
	catch(ocfmException* ex)
	{		
		throw ex;
	}	
}

/**************************************************************************************************
* Function Name: parseFile
* Description: Parses the XML file
* Return pxcValue: ocfmRetCode
****************************************************************************************************/
ocfmRetCode parseFile(char* pbFileName, INT32 iNodeIndex, ENodeType  enumNodeType) 
{
    xmlTextReaderPtr pxtrReader;
    INT32 iRetVal;

    pxtrReader = xmlReaderForFile(pbFileName, NULL, 0);
    try
    {
		if (pxtrReader != NULL)
		{
			iRetVal = xmlTextReaderRead(pxtrReader);
			while (iRetVal == 1)
			{		
				processNode(pxtrReader,enumNodeType, iNodeIndex);
				iRetVal = xmlTextReaderRead(pxtrReader);
			}
			if(iRetVal!=0)
			{
				ocfmException objException;
				/*objException->ocfm_Excpetion(o, true);*/
				objException.ocfm_Excpetion(OCFM_ERR_PARSE_XML);
				throw objException;
			}
		}
		else 
		{
			ocfmException objException;
			objException.ocfm_Excpetion(OCFM_ERR_CANNOT_OPEN_FILE);
			throw objException;
		}
	}									
	catch(ocfmException& ex)
	{
		DeleteNodeObjDict(iNodeIndex, enumNodeType);
		return ex._ocfmRetCode;
	}
	ocfmRetCode ErrStruct;		 
	ErrStruct.code = OCFM_ERR_SUCCESS;
	return ErrStruct;
}
 
/**************************************************************************************************
* Function Name: ReImportXML
* Description: Parses the XML file
* Return pxcValue: ocfmRetCode
****************************************************************************************************/
ocfmRetCode ReImportXML(char* pbFileName, INT32 iNodeID, ENodeType enumNodeType)
{
	INT32 iNodePos;
	//if((IfNodeExists(NodeID, enumNodeType, errorString)) >= 0)
	//ocfmRetValError retPos;
	ocfmRetCode ErrStruct;
	try
	{
		bool bFlag = false;
		ErrStruct = IfNodeExists(iNodeID, enumNodeType, &iNodePos, bFlag);
		if(ErrStruct.code == 0 && bFlag == true)
		{
			CNode objNode;
			CNodeCollection *pobjNodeCollection;
			CIndexCollection *pobjIndexCollection;
			CDataTypeCollection *pobjDataTypeCollection;
			CIndex objIndex;

			objIndex.setNodeID(objNode.getNodeId());
			pobjNodeCollection= CNodeCollection::getNodeColObjectPointer();
			objNode = pobjNodeCollection->getNode(enumNodeType, iNodeID);
			
			pobjDataTypeCollection = objNode.getDataTypeCollection();

			pobjIndexCollection = objNode.getIndexCollection();
			// Delete IndexCollection
			pobjIndexCollection->DeleteIndexCollection();
			// Delete DataTypeCollection
			pobjDataTypeCollection->DeleteDataTypeCollection();
			ErrStruct = parseFile(pbFileName, iNodeID, enumNodeType);
			if(ErrStruct.code != OCFM_ERR_SUCCESS)
			{
				#if defined DEBUG
					cout << "\nparseFile in ReImport failed\n" << endl;
				#endif
				ErrStruct = DeleteNodeObjDict(iNodeID, enumNodeType);
				if(ErrStruct.code != OCFM_ERR_SUCCESS)
				{
					#if defined DEBUG
						cout << "\nDeleteNodeObjDict in ReImport failed\n" << endl;
					#endif
				}
			}
			/* Add other required index*/		
			if(enumNodeType == CN)
			{
				setFlagForRequiredCNIndexes(iNodeID);
				/* Not required only in case of autogenartion 1020 of CN should be updated else it shud be as it is imported*/
				//AddOtherRequiredCNIndexes(iNodeID);
			}			
			if(enumNodeType == MN)
			{
				setFlagForRequiredMNIndexes(iNodeID);			
		}		
			/* Copy default value of pdos to act value*/
			copyPDODefToAct(iNodeID, enumNodeType);	
		
			ErrStruct.code = OCFM_ERR_SUCCESS;
			return ErrStruct;
		}
		else
		{
			ocfmException objException;				
			objException.ocfm_Excpetion(OCFM_ERR_INVALID_NODEID);
			throw objException;
		}
	}
	catch(ocfmException* ex)
	{
		return ex->_ocfmRetCode;
	}
}
 
/**************************************************************************************************
* Function Name: CreateTree
* Description:
* Return pxcValue: void
****************************************************************************************************/
void CreateTree()
{
	//objNodeCollection= CNodeCollection::getNodeColObject();	
}

/**************************************************************************************************
* Function Name: ConvertCdcToBinary
* Description:
* Return pxcValue: void
****************************************************************************************************/
//void ConvertCdcToBinary(char* pbFileName)
//{
//	FILE *fin, *fout;
//	char* filePath;
//	INT32 count=0;
//	INT32 num=0;
//	char ch=0;
//	INT32 iCtr;
//	INT32 iLength;
//	unsigned char ca_cn1obd[10000];
//	unsigned char tempCn1Obd[10000];

//	fin = fopen(pbFileName, "r");
//	filePath = new char[80];
//	filePath = strchr(pbFileName,'\\');
//	filePath = subString(pbpbFileName, 0, strlen(fileName) - strlen(filePath) +1);
//	strcat(filePath,"config_data.cdc");
//	//fout = fopen("config_data.cdc", "wb");
//	fout = fopen(filePath, "wb");
//	
//	
//		while(ch != EOF)
//		{
//			ch = fgetc(fin);
//			if(ch == '/') 
//		    {
//				while(fgetc(fin) != '\n');
//		    }
//			else if( ch != '\t' || ch != '\n' || ch != ' ')
//			{
//				// Convert to Upper case
//				ch = toupper(ch);
//				if((ch >= 65 && ch <= 70) || (ch >= 97 && ch <=102))
//				{
//					ca_cn1obd[num] = ch - 55;
//					num++;
//				}
//				else if (( ch >=48 && ch <= 57))
//				{
//					ca_cn1obd[num] = ch - 48;
//					num++;
//				}
//		}
//		//ca_cn1obd[num] = '\0';
//		// For testing purpose.
//		/*for ( count = 0; count<num ; count++)
//		{
//			printf("%x",ca_cn1obd[count]);

//		}*/
//		iLength = num;
//		//printf( "\niLength = %d\n",iLength);
//		//For Byte Packing
//		for (iCtr = 0 , count = 0; iCtr < iLength; iCtr++, count++ )
//		{
//			tempCn1Obd[count] = (unsigned char)( ( ca_cn1obd[ iCtr ] << 4 ) | ca_cn1obd[iCtr + 1 ] );
//			iCtr++;
//			//printf("0x%2x\t",tempCn1Obd[count]);
//		}
//		//printf("Size : %d\n", count);
//		count = count -1;
//		// Write to Binary file
//		fwrite(&tempCn1Obd,1,count,fout);
//	}

//	delete [] filePath;
//	fclose(fin);
//	fclose(fout);	
//}
/**************************************************************************************************
* Function Name: ProcessUniqueIDRefs
* Description: 
* Return Value: void
****************************************************************************************************/
void ProcessUniqueIDRefs(CNode* objNode)
{
	//Add code here
}
 
/**************************************************************************************************
* Function Name: ImportObjDictXML
* Description: Imports the Object Dictionary XML file. Objects from this Object dictionary file 
  will be used when Adding Index and/or Adding SubIndex
* Return pxcValue: ocfmRetCode
****************************************************************************************************/
//ocfmRetCode ImportObjDictXML(char* pbFileName)
//{
//	xmlTextReaderPtr pxtrReader;
//	INT32 iRetVal;
//	ocfmRetCode ErrStruct;
//	CIndex objIndex;
//
//	cout << "ImportObjDictXML:" << pbFileName << endl;
//	
//	CObjectDictionary* ObjDictObject;
//	ObjDictObject = CObjectDictionary::getObjDictObjectPointer();
//	//char* IndexID = "1006";
//	
//	//objIndex = ObjDictObject->CreateIndexCollection();
//	
//	//objNodeCollection= CNodeCollection::getNodeColObjectPointer();
//	//objNode = objNodeCollection->getNode(enumNodeType, NodeID);
//	//pobjIndexCollection = objNode.getIndexCollection();
//			
//	//AddIndexAttributes(IndexID, &objIndex);
//	//ObjDictObject->addIndex(objIndex);
//
//	//pxtrReader = xmlReaderForFile(pbFileName, NULL, 0);
//	//if (pxtrReader != NULL)
//	//{
//	//	iRetVal = xmlTextReaderRead(pxtrReader);
//	//	while (iRetVal == 1)
//	//	{		
//	//		processObjDict(pxtrReader);
//	//		iRetVal = xmlTextReaderRead(pxtrReader);
//	//	}
//	//}
//	//else
//	//{
//	//	fprintf(stderr, "Unable to open %s\n", pbFileName);
//	//}
//
//	//printf("\n\n\nCalling xmlFreeTextReader\n\n\n\n");	
//	
//}
/**************************************************************************************************
* Function Name: processObjDict
* Description: Process the Node value,Name and its attributes
* Return pxcValue: void
****************************************************************************************************/
void processObjDict(xmlTextReaderPtr pxtrReader)
{
	//const xmlChar *pxcName, *pxcValue;
	
    //pxcName = xmlTextReaderConstName(pxtrReader);
    //if (pxcName == NULL)
	//	printf("\n\n\n\nGot NULL for Name\n\n\n\n");
	////	pxcName = BAD_CAST "--";		
    //pxcValue = xmlTextReaderConstValue(pxtrReader);
    
	////If the NodeTYPE is ELEMENT
	//if( xmlTextReaderNodeType(pxtrReader)==1)
	//	{
	//		if(strcmp(((char*)pxcName),"defType")==0)
	//			{
	//				//objNodeCollection= CNodeCollection::getNodeColObjectPointer();
	//				//CDataTypeCollection* pobjDataTypeCollection;
	//				
	//				DataType objDataType;
	//				if (xmlTextReaderHasAttributes(pxtrReader)==1)
	//					{						
	//						while(xmlTextReaderMoveToNextAttribute(pxtrReader))
	//							{
	//								//printf("Calling setDataTypeAttributes..\n ");
	//								setDataTypeAttributes(pxtrReader, &objDataType);
	//								//$Svalue = xmlTextReaderConstValue(pxtrReader);
	//								//$Sprintf("Value_1:%s\n", pxcValue);
	//								//$Sname =xmlTextReaderConstName(pxtrReader);																									
	//							}
	//					}

	//				//objNode = objNodeCollection->getNode(enumNodeType,NodeIndex);							
	//				//pobjDataTypeCollection = objNode.getDataTypeCollection();
	//				//pobjDataTypeCollection->addDataType(objDataType);
	//		

	//			}
	//		if(strcmp(((char*)pxcName),"Parameter")==0)
	//			{				
	//				Parameter stParameter;

	//				
	//				//objNode = objNodeCollection->getNode(enumNodeType, NodeIndex);	
	//				
	//				if (xmlTextReaderHasAttributes(pxtrReader)==1)
	//					{						
	//						while(xmlTextReaderMoveToNextAttribute(pxtrReader))
	//							{
	//									setParameterAttributes(pxtrReader,&stParameter);																															
	//							}
	//					}
	//					
	//					// Add parameter to the parameter collection of a node
	//					objNode.addParameter(stParameter);				
	//				
	//			}
	//		if(strcmp(((char*)pxcName),"Object")==0)
	//			{
	//				//objNodeCollection= CNodeCollection::getNodeColObjectPointer();
	//				//CIndexCollection* pobjIndexCollection;
	//				//CIndex objIndex;
	//				//$S
	//				CObjectDictionary* ObjDictObject;
	//				ObjDictObject = CObjectDictionary::getObjDictObjectPointer();

	//				
	//				objNode = objNodeCollection->getNode(enumNodeType, NodeIndex);	
	//					//Set the NodeID
	//					objIndex.setNodeID(objNode.getNodeId());

	//				if (xmlTextReaderHasAttributes(pxtrReader)==1)
	//					{						
	//						while(xmlTextReaderMoveToNextAttribute(pxtrReader))
	//							{
	//								//printf("Calling setIndexAttributes..\n ");
	//								setIndexAttributes(pxtrReader, &objIndex);	
	//								//$Svalue = xmlTextReaderConstValue(pxtrReader);
	//								//$Sprintf("\tValue_2:%s\n", pxcValue);
	//								//$Sname =xmlTextReaderConstName(pxtrReader);																								
	//							}
	//					}
	//					
	//					pobjIndexCollection = objNode.getIndexCollection();
	//					
	//					//Add Index object to the IndexCollection
	//					pobjIndexCollection->addIndex(objIndex);				
	//					LastIndexParsed = pobjIndexCollection->getNumberofIndexes()-1;
	//					
	//			}
	//		if(strcmp(((char*)pxcName),"SubObject")==0 )
	//			{
	//				objNodeCollection = CNodeCollection::getNodeColObjectPointer();
	//				CIndexCollection* pobjIndexCollection;
	//				CSubIndex objSubIndex;
	//				objNode =objNodeCollection->getNode(enumNodeType,NodeIndex);
	//					//Set the NodeID
	//					objSubIndex.setNodeID(objNode.getNodeId());		

	//				if (xmlTextReaderHasAttributes(pxtrReader) ==1)
	//					{
	//						
	//						while(xmlTextReaderMoveToNextAttribute(pxtrReader))
	//							{
	//								setSubIndexAttributes(pxtrReader,&objSubIndex);
	//								//pxcValue = xmlTextReaderConstValue(pxtrReader);
	//								//printf("\t\tValue_3:%s\n", pxcValue);
	//								//pxcName =xmlTextReaderConstName(pxtrReader);																									
	//							}
	//					}
	//					CIndex* objIndexPtr;
	//							
	//			
	//					pobjIndexCollection =objNode.getIndexCollection();
	//					objIndexPtr =pobjIndexCollection->getIndex(LastIndexParsed);
	//									
	//					if(strcmp(objIndexPtr->getIndexValue(),"2103")==0)
	//						{
	//							if(strcmp(objSubIndex.getIndexValue(),"17")==0)
	//								{	
	//								printf("%d",objIndexPtr->getNumberofSubIndexes());
	//								}
	//						}
	//					printf("LastIndexParsed: %d-%s\n",LastIndexParsed, objIndexPtr->getIndexValue());
	//					printf("SubIndex pxcValue: %s \n",objSubIndex.getIndexValue());
	//					objIndexPtr->addSubIndex(objSubIndex);						
	//					
	//			}	
	//		}
	//		
}

/**************************************************************************************************
* Function Name: SaveNode
* Description: Saves the content of a Node into a XML in XDC format
* Return Value: ocfmRetCode
****************************************************************************************************/
ocfmRetCode SaveNode(const char* pbFileName, INT32 NodeID, ENodeType enumNodeType)
{	
	INT32 iBytesWritten;
	xmlTextWriterPtr pxtwWriter;
    xmlDocPtr pxdDoc;
	ocfmRetCode stErrInfo;

	try 
	{
		CNode objNode;		
		CNodeCollection *pobjNodeCollection;
		CIndexCollection *pobjIndexCollection;
		CIndex objIndex;
		CIndex* pobjIndexPtr;
		CApplicationProcess* pobjAppProc;
		INT32 iIndexPos = 0;
		
		objIndex.setNodeID(objNode.getNodeId());
		pobjNodeCollection= CNodeCollection::getNodeColObjectPointer();
		objNode = pobjNodeCollection->getNode(enumNodeType, NodeID);

		pobjIndexCollection = objNode.getIndexCollection();
		
		if(pobjIndexCollection->getNumberofIndexes() == 0)
		{
			//OCFM_ERR_NO_INDEX_FOUND;
			stErrInfo.code = OCFM_ERR_SUCCESS;
			return stErrInfo;	
		}

		/* Create a new XmlWriter for DOM, with no compression. */
		pxtwWriter = xmlNewTextWriterDoc(&pxdDoc, 0);
		if (pxtwWriter == NULL) 
		{
			printf("testXmlpxtwWriterDoc: Error creating the xml pxtwWriter\n");
			//return;
			ocfmException objException;// = new ocfmException;
			objException.ocfm_Excpetion(OCFM_ERR_CREATE_XML_WRITER_FAILED);
			throw &objException;
		}
		/* Start the document with the xml default for the version,
		* encoding UTF-8 and the default for the standalone
		* declaration. */
		iBytesWritten = xmlTextWriterStartDocument(pxtwWriter, NULL, MY_ENCODING, NULL);
		if (iBytesWritten < 0) 
		{
			printf("testXmlwriterDoc: Error at xmlTextWriterStartDocument\n");
			ocfmException objException;// = new ocfmException;
			objException.ocfm_Excpetion(OCFM_ERR_XML_START_DOC_FAILED);
			throw &objException;
		}	
		iBytesWritten = xmlTextWriterWriteComment(pxtwWriter,BAD_CAST "This file was autogenerated by openCONFIGURATOR");
		
		// Start ISO15745ProfileContainer Tag		
		iBytesWritten = xmlTextWriterStartElement(pxtwWriter, BAD_CAST "ISO15745ProfileContainer");
		if (iBytesWritten < 0) 
		{
			printf("testXmlwriterMemory: Error at xmlTextWriterStartElement\n");
			ocfmException objException;// = new ocfmException;
			objException.ocfm_Excpetion(OCFM_ERR_XML_WRITER_START_ELT_FAILED);
			throw &objException;
		}
		
		// Start ProfileBody Tag
		iBytesWritten = xmlTextWriterStartElement(pxtwWriter, BAD_CAST "ProfileBody");
		if (iBytesWritten < 0) 
		{
			printf("testXmlwriterMemory: Error at xmlTextWriterStartElement\n");
			ocfmException objException;// = new ocfmException;
			objException.ocfm_Excpetion(OCFM_ERR_XML_WRITER_START_ELT_FAILED);
			throw &objException;
		}
		
		// Start ApplicationProcess Tag	
		iBytesWritten = xmlTextWriterStartElement(pxtwWriter, BAD_CAST "ApplicationProcess");
		if (iBytesWritten < 0) 
		{
			printf("testXmlwriterMemory: Error at xmlTextWriterStartElement\n");
			ocfmException objException;// = new ocfmException;
			objException.ocfm_Excpetion(OCFM_ERR_XML_WRITER_START_ELT_FAILED);
			throw &objException;
		}
		xmlTextWriterSetIndent(pxtwWriter, 1);
					
		// Start dataTypeList Tag
		iBytesWritten = xmlTextWriterStartElement(pxtwWriter, BAD_CAST "dataTypeList");
		if (iBytesWritten < 0) 
		{
			printf("testXmlwriterMemory: Error at xmlTextWriterStartElement\n");
			ocfmException objException;// = new ocfmException;
			objException.ocfm_Excpetion(OCFM_ERR_XML_WRITER_START_ELT_FAILED);
			throw &objException;
		}

		CComplexDataType* objCDT;
		pobjAppProc = objNode.getApplicationProcess();							
								
		if(pobjAppProc->getCDTCount() > 0)
		{
			//cout << "\nobjAppProc->getCDTCount():" << pobjAppProc->getCDTCount() << endl;
			for(INT32 iLoopCount = 0; iLoopCount < pobjAppProc->getCDTCount(); iLoopCount++)
			{
				// Start struct Tag
				iBytesWritten = xmlTextWriterStartElement(pxtwWriter, BAD_CAST "struct");
				if (iBytesWritten < 0) 
				{
					printf("testXmlwriterMemory: Error at xmlTextWriterStartElement\n");
					ocfmException objException;// = new ocfmException;
					objException.ocfm_Excpetion(OCFM_ERR_XML_WRITER_START_ELT_FAILED);
					throw &objException;
				}
				objCDT = pobjAppProc->getCDTbyCount(iLoopCount);
				//cout << "\nobjCDT.pxcName_id_attr.getUniqueID():" << objCDT->pxcName_id_attr->UniqueId << endl;
				iBytesWritten = xmlTextWriterWriteAttribute(pxtwWriter, BAD_CAST "name", BAD_CAST objCDT->name_id_attr->getName());
				iBytesWritten = xmlTextWriterWriteAttribute(pxtwWriter, BAD_CAST "uniqueID", BAD_CAST objCDT->name_id_attr->UniqueId);
				
				//cout << "objCDT->getVarDeclarationCount():" << objCDT->varCollection.Count() << endl;
				for(INT32 tempVarCount = 0; tempVarCount < objCDT->varCollection.Count(); tempVarCount++)
				{
					varDeclaration vd;
					vd.Initialize();
					vd = objCDT->varCollection[tempVarCount];
					// Start varDeclaration Tag
					iBytesWritten = xmlTextWriterStartElement(pxtwWriter, BAD_CAST "varDeclaration");
					if (iBytesWritten < 0) 
					{
						printf("testXmlwriterMemory: Error at xmlTextWriterStartElement\n");
						ocfmException objException;// = new ocfmException;
						objException.ocfm_Excpetion(OCFM_ERR_XML_WRITER_START_ELT_FAILED);
						throw &objException;
					}
					iBytesWritten = xmlTextWriterWriteAttribute(pxtwWriter, BAD_CAST "name", BAD_CAST vd.nam_id_dt_attr->getName());
					iBytesWritten = xmlTextWriterWriteAttribute(pxtwWriter, BAD_CAST "uniqueID", BAD_CAST vd.nam_id_dt_attr->UniqueId);
					iBytesWritten = xmlTextWriterWriteAttribute(pxtwWriter, BAD_CAST "size", BAD_CAST vd.size);
					
					//$STODO:
					//iBytesWritten = xmlTextWriterWriteAttribute(pxtwWriter, BAD_CAST "size", BAD_CAST vd.size);
					// Start DataType Tag							
					if(vd.nam_id_dt_attr->getDataType() != NULL)
					{
						//cout << "\nvd.nam_id_dt_attr->dataTypeUniqueIDRef:" << vd.nam_id_dt_attr->dataTypeUniqueIDRef << endl;
						//cout << "\nvd.nam_id_dt_attr->dataType:" << vd.nam_id_dt_attr->getDataType() << endl;
						iBytesWritten = xmlTextWriterStartElement(pxtwWriter, BAD_CAST vd.nam_id_dt_attr->getDataType());
							
						if (iBytesWritten < 0) 
						{
							printf("testXmlwriterMemory: Error at xmlTextWriterStartElement\n");
							ocfmException objException;// = new ocfmException;
							objException.ocfm_Excpetion(OCFM_ERR_XML_WRITER_START_ELT_FAILED);
							throw &objException;
						}
						// End varDeclaration Tag
						iBytesWritten = xmlTextWriterEndElement(pxtwWriter);
						if (iBytesWritten < 0)
						{
							printf("testXmlwriterDoc: Error at xmlTextWriterEndElement\n");
							ocfmException objException;// = new ocfmException;
							objException.ocfm_Excpetion(OCFM_ERR_XML_WRITER_END_ELT_FAILED);
							throw &objException;
						}
					}
					else if(vd.nam_id_dt_attr->getDtUniqueRefId() != NULL)
					{
						//cout << "\nvd.nam_id_dt_attr->getDtUniqueRefId():" << vd.nam_id_dt_attr->getDtUniqueRefId() << endl;
						// Start dataTypeIDRef Tag
						iBytesWritten = xmlTextWriterStartElement(pxtwWriter, BAD_CAST "dataTypeIDRef");							
						if (iBytesWritten < 0) 
						{
							printf("testXmlwriterMemory: Error at xmlTextWriterStartElement\n");
							ocfmException objException;// = new ocfmException;
							objException.ocfm_Excpetion(OCFM_ERR_XML_WRITER_START_ELT_FAILED);
							throw &objException;
						}
						
						iBytesWritten = xmlTextWriterWriteAttribute(pxtwWriter, BAD_CAST "uniqueIDRef", BAD_CAST vd.nam_id_dt_attr->getDtUniqueRefId());
						
						// End dataTypeIDRef Tag
						iBytesWritten = xmlTextWriterEndElement(pxtwWriter);
						if (iBytesWritten < 0)
						{
							printf("testXmlwriterDoc: Error at xmlTextWriterEndElement\n");
							ocfmException objException;// = new ocfmException;
							objException.ocfm_Excpetion(OCFM_ERR_XML_WRITER_END_ELT_FAILED);
							throw &objException;
						}
					}
							
					// End varDeclaration Tag
					iBytesWritten = xmlTextWriterEndElement(pxtwWriter);
					if (iBytesWritten < 0)
					{
						printf("testXmlwriterDoc: Error at xmlTextWriterEndElement\n");
						ocfmException objException;// = new ocfmException;
						objException.ocfm_Excpetion(OCFM_ERR_XML_WRITER_END_ELT_FAILED);
						throw &objException;
					}						
				}
						
				// End struct Tag
				iBytesWritten = xmlTextWriterEndElement(pxtwWriter);
				if (iBytesWritten < 0)
				{
					printf("testXmlwriterDoc: Error at xmlTextWriterEndElement\n");
					ocfmException objException;// = new ocfmException;
					objException.ocfm_Excpetion(OCFM_ERR_XML_WRITER_END_ELT_FAILED);
					throw &objException;
				}
			}
		}
									
		// End dataTypeList Tag
		iBytesWritten = xmlTextWriterEndElement(pxtwWriter);
		if (iBytesWritten < 0)
		{
			printf("testXmlwriterDoc: Error at xmlTextWriterEndElement\n");
			ocfmException objException;// = new ocfmException;
			objException.ocfm_Excpetion(OCFM_ERR_XML_WRITER_END_ELT_FAILED);
			throw &objException;
		}
		
		// Start parameterList Tag
		iBytesWritten = xmlTextWriterStartElement(pxtwWriter, BAD_CAST "parameterList");
		if (iBytesWritten < 0) 
		{
			printf("testXmlwriterMemory: Error at xmlTextWriterStartElement\n");
			ocfmException objException;// = new ocfmException;
			objException.ocfm_Excpetion(OCFM_ERR_XML_WRITER_START_ELT_FAILED);
			throw &objException;
		}			
		
		if(objNode.getApplicationProcess()!=NULL)
		{			
			pobjAppProc = objNode.getApplicationProcess();								
			
			for(INT32 iLoopCount = 0; iLoopCount < pobjAppProc->ParameterCollection.Count(); iLoopCount++)
			{
				Parameter Param;
				Param = pobjAppProc->get_UniqueIDRef_by_ParameterIndex(iLoopCount);		
				// Start parameter Tag
				iBytesWritten = xmlTextWriterStartElement(pxtwWriter, BAD_CAST "parameter");
				if (iBytesWritten < 0) 
				{
					printf("testXmlwriterMemory: Error at xmlTextWriterStartElement\n");
					ocfmException objException;// = new ocfmException;
					objException.ocfm_Excpetion(OCFM_ERR_XML_WRITER_START_ELT_FAILED);
					throw &objException;
				}
				//cout << "\nParam.pxcName_id_dt_attr.getUniqueID():" << Param.pxcName_id_dt_attr.getUniqueID() << endl;
				iBytesWritten = xmlTextWriterWriteAttribute(pxtwWriter, BAD_CAST "uniqueID", BAD_CAST Param.name_id_dt_attr.getUniqueID());
				//cout << "\nParam.access:" << Param.access << endl;
				iBytesWritten = xmlTextWriterWriteAttribute(pxtwWriter, BAD_CAST "access", BAD_CAST Param.access);
										
				// Start dataTypeIDRef Tag
				iBytesWritten = xmlTextWriterStartElement(pxtwWriter, BAD_CAST "dataTypeIDRef");
				if (iBytesWritten < 0) 
				{
					printf("testXmlwriterMemory: Error at xmlTextWriterStartElement\n");
					ocfmException objException;// = new ocfmException;
					objException.ocfm_Excpetion(OCFM_ERR_XML_WRITER_START_ELT_FAILED);
					throw &objException;
				}
				//cout << "\nParam.pxcName_id_dt_attr.getDtUniqueRefId():" << Param.pxcName_id_dt_attr.getDtUniqueRefId() << endl;
				iBytesWritten = xmlTextWriterWriteAttribute(pxtwWriter, BAD_CAST "uniqueIDRef", BAD_CAST Param.name_id_dt_attr.getDtUniqueRefId());
				// End dataTypeIDRef Tag
				iBytesWritten = xmlTextWriterEndElement(pxtwWriter);
				if (iBytesWritten < 0)
				{
					printf("testXmlwriterDoc: Error at xmlTextWriterEndElement\n");
					ocfmException objException;// = new ocfmException;
					objException.ocfm_Excpetion(OCFM_ERR_XML_WRITER_END_ELT_FAILED);
					throw &objException;
				}
				
				// End parameter Tag
				iBytesWritten = xmlTextWriterEndElement(pxtwWriter);
				if (iBytesWritten < 0)
				{
					printf("testXmlwriterDoc: Error at xmlTextWriterEndElement\n");
					ocfmException objException;// = new ocfmException;
					objException.ocfm_Excpetion(OCFM_ERR_XML_WRITER_END_ELT_FAILED);
					throw &objException;
				}
			}
		}
		// End parameterList Tag
		iBytesWritten = xmlTextWriterEndElement(pxtwWriter);
		if (iBytesWritten < 0)
		{
			printf("testXmlwriterDoc: Error at xmlTextWriterEndElement\n");
			ocfmException objException;// = new ocfmException;
			objException.ocfm_Excpetion(OCFM_ERR_XML_WRITER_END_ELT_FAILED);
			throw &objException;
		}
		
		// End ApplicationProcess Tag
		iBytesWritten = xmlTextWriterEndElement(pxtwWriter);
		if (iBytesWritten < 0)
		{
			printf("testXmlwriterDoc: Error at xmlTextWriterEndElement\n");
			ocfmException objException;// = new ocfmException;
			objException.ocfm_Excpetion(OCFM_ERR_XML_WRITER_END_ELT_FAILED);
			throw &objException;
		}

		// End ProfileBody Tag
		iBytesWritten = xmlTextWriterEndElement(pxtwWriter);
		if (iBytesWritten < 0)
		{
			printf("testXmlwriterDoc: Error at xmlTextWriterEndElement\n");
			ocfmException objException;// = new ocfmException;
			objException.ocfm_Excpetion(OCFM_ERR_XML_WRITER_END_ELT_FAILED);
			throw &objException;
		}
		
		// Start ProfileBody Tag
		iBytesWritten = xmlTextWriterStartElement(pxtwWriter, BAD_CAST "ProfileBody");
		if (iBytesWritten < 0) 
		{
			printf("testXmlwriterMemory: Error at xmlTextWriterStartElement\n");
			ocfmException objException;// = new ocfmException;
			objException.ocfm_Excpetion(OCFM_ERR_XML_WRITER_START_ELT_FAILED);
			throw &objException;
		}
		
		// Start ApplicationLayers Tag
		iBytesWritten = xmlTextWriterStartElement(pxtwWriter, BAD_CAST "ApplicationLayers");		
		if (iBytesWritten < 0) 
		{
			printf("testXmlwriterMemory: Error at xmlTextWriterStartElement\n");
			ocfmException objException;// = new ocfmException;
			objException.ocfm_Excpetion(OCFM_ERR_XML_WRITER_START_ELT_FAILED);
			throw &objException;
		}
		
		// Start DataTypeList Tag
		iBytesWritten = xmlTextWriterStartElement(pxtwWriter, BAD_CAST "DataTypeList");
		if (iBytesWritten < 0) 
		{
			printf("testXmlwriterMemory: Error at xmlTextWriterStartElement\n");
			ocfmException objException;// = new ocfmException;
			objException.ocfm_Excpetion(OCFM_ERR_XML_WRITER_START_ELT_FAILED);
			throw &objException;
		}
			
		//pobjIndexPtr = pobjIndexCollection->getNumberOfDataTypes;
		
		CDataTypeCollection *pobjDataTypeCollection;
		
		pobjDataTypeCollection = objNode.getDataTypeCollection();
		
		for(INT32 iLoopCount = 0; iLoopCount < pobjDataTypeCollection->getNumberOfDataTypes(); iLoopCount++)
		{		
			DataType* dt = NULL;
			dt = pobjDataTypeCollection->getDataTypeElement(iLoopCount);
			//char temp_DTValue[50] = dt->DataTypeValue;
			//char temp_DTName[50] = dt->getName();			
			
			// Start defType Tag
			iBytesWritten = xmlTextWriterStartElement(pxtwWriter, BAD_CAST "defType");	
			if (iBytesWritten < 0) 
			{
				printf("testXmlwriterMemory: Error at xmlTextWriterStartElement\n");
				ocfmException objException;// = new ocfmException;
				objException.ocfm_Excpetion(OCFM_ERR_XML_WRITER_START_ELT_FAILED);
				throw &objException;
			}
			
			iBytesWritten = xmlTextWriterWriteAttribute(pxtwWriter, BAD_CAST "dataType", BAD_CAST dt->DataTypeValue);			
			// Start DataType Tag
			iBytesWritten = xmlTextWriterStartElement(pxtwWriter, BAD_CAST dt->getName());			
			if (iBytesWritten < 0) 
			{
				printf("testXmlwriterMemory: Error at xmlTextWriterStartElement\n");
				ocfmException objException;// = new ocfmException;
				objException.ocfm_Excpetion(OCFM_ERR_XML_WRITER_START_ELT_FAILED);
				throw &objException;
			}
			xmlTextWriterSetIndent(pxtwWriter, 1);
			// End DataType Tag
			iBytesWritten = xmlTextWriterEndElement(pxtwWriter);
			if (iBytesWritten < 0)
			{
				printf("testXmlwriterDoc: Error at xmlTextWriterEndElement\n");
				ocfmException objException;// = new ocfmException;
				objException.ocfm_Excpetion(OCFM_ERR_XML_WRITER_END_ELT_FAILED);
				throw &objException;				
			}
			xmlTextWriterSetIndent(pxtwWriter, 1);
			// End defType Tag
			iBytesWritten = xmlTextWriterEndElement(pxtwWriter);
			if (iBytesWritten < 0)
			{
				printf("testXmlwriterDoc: Error at xmlTextWriterEndElement\n");
				ocfmException objException;// = new ocfmException;
				objException.ocfm_Excpetion(OCFM_ERR_XML_WRITER_END_ELT_FAILED);
				throw &objException;				
			}			
		}	
		xmlTextWriterSetIndent(pxtwWriter, 1);
		// End DataTypeList Tag
		iBytesWritten = xmlTextWriterEndElement(pxtwWriter);
		if (iBytesWritten < 0)
		{
			printf("testXmlwriterDoc: Error at xmlTextWriterEndElement\n");
			ocfmException objException;// = new ocfmException;
			objException.ocfm_Excpetion(OCFM_ERR_XML_WRITER_END_ELT_FAILED);
			throw &objException;
		}
				
		if(pobjIndexCollection->getNumberofIndexes() == 0)
		{
			printf("SaveNode: No Indexes Found\n");
			ocfmException objException;				
			objException.ocfm_Excpetion(OCFM_ERR_NO_INDEX_FOUND);
			throw &objException;
		}
		
		// Start ObjectList Tag
		iBytesWritten = xmlTextWriterStartElement(pxtwWriter, BAD_CAST "ObjectList");
		if (iBytesWritten < 0) 
		{
			printf("testXmlwriterMemory: Error at xmlTextWriterStartElement\n");
			ocfmException objException;// = new ocfmException;
			objException.ocfm_Excpetion(OCFM_ERR_XML_WRITER_START_ELT_FAILED);
			throw &objException;
		}
						
		for(iIndexPos = 0; iIndexPos < pobjIndexCollection->getNumberofIndexes(); iIndexPos++)		
		{		
			// Start Object Tag
			iBytesWritten = xmlTextWriterStartElement(pxtwWriter, BAD_CAST "Object");
			if (iBytesWritten < 0) 
			{
				printf("testXmlwriterMemory: Error at xmlTextWriterStartElement\n");
				ocfmException objException;// = new ocfmException;
				objException.ocfm_Excpetion(OCFM_ERR_XML_WRITER_START_ELT_FAILED);
				throw &objException;
			}
			
			pobjIndexPtr = pobjIndexCollection->getIndex(iIndexPos);
			
			if(pobjIndexPtr->getIndexValue() != NULL)
			{
				iBytesWritten = xmlTextWriterWriteAttribute(pxtwWriter, BAD_CAST "index", BAD_CAST pobjIndexPtr->getIndexValue());
				//cout << "\nobjIndexPtr->getIndexValue():" << pobjIndexPtr->getIndexValue() << endl;				
			}
			if(pobjIndexPtr->getName() != NULL)
				iBytesWritten = xmlTextWriterWriteAttribute(pxtwWriter, BAD_CAST "name", BAD_CAST pobjIndexPtr->getName());
			if(pobjIndexPtr->getObjectType() != NULL)
			{
				char *str_EObjectType = new char[10];
				str_EObjectType = _IntToAscii(pobjIndexPtr->getEObjectType(), str_EObjectType, 10);
				iBytesWritten = xmlTextWriterWriteAttribute(pxtwWriter, BAD_CAST "objectType", BAD_CAST str_EObjectType);
				delete [] str_EObjectType;
				//iBytesWritten = xmlTextWriterWriteAttribute(pxtwWriter, BAD_CAST "objectType", BAD_CAST pobjIndexPtr->getObjectType());
			}
			DataType objTmpDataType;
			objTmpDataType = pobjIndexPtr->getDataType();		
			if(objTmpDataType.DataTypeValue != NULL)// TODO:$S
			if(strlen(objTmpDataType.DataTypeValue) != 0)
			{				
				iBytesWritten = xmlTextWriterWriteAttribute(pxtwWriter, BAD_CAST "dataType", BAD_CAST objTmpDataType.DataTypeValue);
			}
			if((pobjIndexPtr->getAccessType() != NULL))
			if( strlen(pobjIndexPtr->getAccessType()) != 0 )
				iBytesWritten = xmlTextWriterWriteAttribute(pxtwWriter, BAD_CAST "accessType", BAD_CAST pobjIndexPtr->getAccessType());
			if((pobjIndexPtr->getDefaultValue() != NULL)) 
			if(strlen(pobjIndexPtr->getDefaultValue()) != 0)
				iBytesWritten = xmlTextWriterWriteAttribute(pxtwWriter, BAD_CAST "defaultValue", BAD_CAST pobjIndexPtr->getDefaultValue());
			if((pobjIndexPtr->getActualValue() != NULL))
			if(strlen(pobjIndexPtr->getActualValue()) != 0)
				iBytesWritten = xmlTextWriterWriteAttribute(pxtwWriter, BAD_CAST "actualValue", BAD_CAST pobjIndexPtr->getActualValue());
			if(pobjIndexPtr->getLowLimit() != NULL)
			if(strlen(pobjIndexPtr->getLowLimit()) != 0)
			{
				//cout << "\nWhen saving pobjIndexPtr->getLowLimit():" << pobjIndexPtr->getLowLimit() << endl;
				iBytesWritten = xmlTextWriterWriteAttribute(pxtwWriter, BAD_CAST "lowLimit", BAD_CAST pobjIndexPtr->getLowLimit());
			}
			if(pobjIndexPtr->getHighLimit() != NULL)
			if(strlen(pobjIndexPtr->getHighLimit()) != 0)
			{
				//cout << "\nWhen saving pobjIndexPtr->getHighLimit():" << pobjIndexPtr->getHighLimit() << endl;
				iBytesWritten = xmlTextWriterWriteAttribute(pxtwWriter, BAD_CAST "highLimit", BAD_CAST pobjIndexPtr->getHighLimit());
			}
			if((pobjIndexPtr->getPDOMapping() != NULL))
			if(strlen(pobjIndexPtr->getPDOMapping()) != 0)
				iBytesWritten = xmlTextWriterWriteAttribute(pxtwWriter, BAD_CAST "PDOmapping", BAD_CAST pobjIndexPtr->getPDOMapping());
			if(pobjIndexPtr->getUniqueIDRef() != NULL)
			if(strlen(pobjIndexPtr->getUniqueIDRef()) != 0)
				iBytesWritten = xmlTextWriterWriteAttribute(pxtwWriter, BAD_CAST "uniqueIDRef", BAD_CAST pobjIndexPtr->getUniqueIDRef());
			if(pobjIndexPtr->getFlagIfIncludedCdc() == 0)
				iBytesWritten = xmlTextWriterWriteAttribute(pxtwWriter, BAD_CAST "CDCFlag", BAD_CAST "FALSE");
			else if(pobjIndexPtr->getFlagIfIncludedCdc() == 1)
				iBytesWritten = xmlTextWriterWriteAttribute(pxtwWriter, BAD_CAST "CDCFlag", BAD_CAST "TRUE");
				
			xmlTextWriterSetIndent(pxtwWriter, 1);
								
			INT32 SubIndexPos = 0;
			
			if(!(pobjIndexPtr->getNumberofSubIndexes() <= 0))
			{					
				for(SubIndexPos = 0; SubIndexPos < pobjIndexPtr->getNumberofSubIndexes(); SubIndexPos++)
				{					
					// Start SubObject Tag
					iBytesWritten = xmlTextWriterStartElement(pxtwWriter, BAD_CAST "SubObject");
					if (iBytesWritten < 0) 
					{
						printf("testXmlwriterMemory: Error at xmlTextWriterStartElement\n");
						ocfmException objException;// = new ocfmException;
						objException.ocfm_Excpetion(OCFM_ERR_XML_WRITER_START_ELT_FAILED);
						throw &objException;
					}						
					CSubIndex* objSubIndexPtr;
					objSubIndexPtr = pobjIndexPtr->getSubIndex(SubIndexPos);	
					//SubObject subIndex="01" name="NodeID_U8" objectType="7" dataType="0005" accessType="rw" defaultValue="0"
					if(objSubIndexPtr->getIndexValue() != NULL)
						iBytesWritten = xmlTextWriterWriteAttribute(pxtwWriter, BAD_CAST "subIndex", BAD_CAST objSubIndexPtr->getIndexValue());
					if(objSubIndexPtr->getName() != NULL)
						iBytesWritten = xmlTextWriterWriteAttribute(pxtwWriter, BAD_CAST "name", BAD_CAST objSubIndexPtr->getName());
					if(objSubIndexPtr->getObjectType() != NULL)
					{
						char *str_EObjectType = new char[10];
						str_EObjectType = _IntToAscii(objSubIndexPtr->getEObjectType(), str_EObjectType, 10);
						iBytesWritten = xmlTextWriterWriteAttribute(pxtwWriter, BAD_CAST "objectType", BAD_CAST str_EObjectType);
						delete [] str_EObjectType;
					}
					//iBytesWritten = xmlTextWriterWriteAttribute(pxtwWriter, BAD_CAST "objectType", BAD_CAST objSubIndexPtr->getEObjectType());
					DataType objTmpDataType;
					objTmpDataType = objSubIndexPtr->getDataType();		
					if(objTmpDataType.DataTypeValue != NULL)
					if(strlen(objTmpDataType.DataTypeValue) != 0)
						iBytesWritten = xmlTextWriterWriteAttribute(pxtwWriter, BAD_CAST "dataType", BAD_CAST objTmpDataType.DataTypeValue);
					if(objSubIndexPtr->getAccessType() != NULL)
						iBytesWritten = xmlTextWriterWriteAttribute(pxtwWriter, BAD_CAST "accessType", BAD_CAST objSubIndexPtr->getAccessType());
					if(objSubIndexPtr->getDefaultValue() != NULL)
						iBytesWritten = xmlTextWriterWriteAttribute(pxtwWriter, BAD_CAST "defaultValue", BAD_CAST objSubIndexPtr->getDefaultValue());
					if(objSubIndexPtr->getActualValue() != NULL)
						iBytesWritten = xmlTextWriterWriteAttribute(pxtwWriter, BAD_CAST "actualValue", BAD_CAST objSubIndexPtr->getActualValue());
					if(objSubIndexPtr->getLowLimit() != NULL)
					if(strlen(objSubIndexPtr->getLowLimit()) != 0)
					{
						//cout << "\nWhen saving SI pobjIndexPtr->getLowLimit():" << objSubIndexPtr->getLowLimit() << endl;
						iBytesWritten = xmlTextWriterWriteAttribute(pxtwWriter, BAD_CAST "lowLimit", BAD_CAST objSubIndexPtr->getLowLimit());
					}
					if(objSubIndexPtr->getHighLimit() != NULL)
					if(strlen(objSubIndexPtr->getHighLimit()) != 0)
					{
						//cout << "\nWhen saving SI pobjIndexPtr->getHighLimit():" << objSubIndexPtr->getHighLimit() << endl;
						iBytesWritten = xmlTextWriterWriteAttribute(pxtwWriter, BAD_CAST "highLimit", BAD_CAST objSubIndexPtr->getHighLimit());
					}
					if((pobjIndexPtr->getPDOMapping() != NULL))
					if(objSubIndexPtr->getPDOMapping() != NULL)
						iBytesWritten = xmlTextWriterWriteAttribute(pxtwWriter, BAD_CAST "PDOmapping", BAD_CAST objSubIndexPtr->getPDOMapping());
					if(objSubIndexPtr->getUniqueIDRef() != NULL)
						iBytesWritten = xmlTextWriterWriteAttribute(pxtwWriter, BAD_CAST "uniqueIDRef", BAD_CAST objSubIndexPtr->getUniqueIDRef());
					if(objSubIndexPtr->getFlagIfIncludedCdc() == 0)
						iBytesWritten = xmlTextWriterWriteAttribute(pxtwWriter, BAD_CAST "CDCFlag", BAD_CAST "FALSE");
					else if(objSubIndexPtr->getFlagIfIncludedCdc() == 1)
						iBytesWritten = xmlTextWriterWriteAttribute(pxtwWriter, BAD_CAST "CDCFlag", BAD_CAST "TRUE");
					// End SubObject Tag
					iBytesWritten = xmlTextWriterEndElement(pxtwWriter);
					if (iBytesWritten < 0)
					{
						printf("testXmlwriterDoc: Error at xmlTextWriterEndElement\n");
						ocfmException objException;// = new ocfmException;
						objException.ocfm_Excpetion(OCFM_ERR_XML_WRITER_END_ELT_FAILED);
						throw &objException;				
					}
						
				}
			}
				
			// End Object Tag
			iBytesWritten = xmlTextWriterEndElement(pxtwWriter);
			if (iBytesWritten < 0)
			{
				printf("testXmlwriterDoc: Error at xmlTextWriterEndElement\n");
				ocfmException objException;// = new ocfmException;
				objException.ocfm_Excpetion(OCFM_ERR_XML_WRITER_END_ELT_FAILED);
				throw &objException;				
			}
				
		}
		
		xmlTextWriterSetIndent(pxtwWriter, 1);
		// End ObjectList Tag
		iBytesWritten = xmlTextWriterEndElement(pxtwWriter);
		if (iBytesWritten < 0)
		{
			printf("testXmlwriterDoc: Error at xmlTextWriterEndElement\n");
			ocfmException objException;// = new ocfmException;
			objException.ocfm_Excpetion(OCFM_ERR_XML_WRITER_END_ELT_FAILED);
			throw &objException;
		}
		
		xmlTextWriterSetIndent(pxtwWriter, 1);
		// End ApplicationLayers Tag
		iBytesWritten = xmlTextWriterEndElement(pxtwWriter);
		if (iBytesWritten < 0)
		{
			printf("testXmlwriterDoc: Error at xmlTextWriterEndElement\n");
			ocfmException objException;// = new ocfmException;
			objException.ocfm_Excpetion(OCFM_ERR_XML_WRITER_END_ELT_FAILED);
			throw &objException;
		}
		// Start Network Management Tag
		iBytesWritten = xmlTextWriterStartElement(pxtwWriter, BAD_CAST "NetworkManagement");		
		if (iBytesWritten < 0) 
		{
			printf("testXmlwriterMemory: Error at xmlTextWriterStartElement\n");
			ocfmException objException;// = new ocfmException;
			objException.ocfm_Excpetion(OCFM_ERR_XML_WRITER_START_ELT_FAILED);
			throw &objException;
		}

		xmlTextWriterSetIndent(pxtwWriter, 1);
					
		// Start General Features Tag
		iBytesWritten = xmlTextWriterStartElement(pxtwWriter, BAD_CAST "General Features");
		if (iBytesWritten < 0) 
		{
			printf("testXmlwriterMemory: Error at xmlTextWriterStartElement\n");
			ocfmException objException;// = new ocfmException;
			objException.ocfm_Excpetion(OCFM_ERR_XML_WRITER_START_ELT_FAILED);
			throw &objException;
		}
		CNetworkManagement *pobjNwManagement;
		pobjNwManagement = objNode.getNetworkManagement();
		
		for(UINT32 uiLoopCount = 0; uiLoopCount < pobjNwManagement->getNumberOfFeatures(); uiLoopCount++)
		{		
			Feature* feature = NULL;
			feature = pobjNwManagement->getFeature(uiLoopCount);
			
			if(feature->m_featureType == GENERAL_FEATURES)
			{
				iBytesWritten = xmlTextWriterWriteAttribute(pxtwWriter, BAD_CAST feature->m_Name, BAD_CAST feature->Value);		
				if (iBytesWritten < 0) 
				{
					printf("testXmlwriterMemory: Error at xmlTextWriterStartElement\n");
					ocfmException objException;// = new ocfmException;
					objException.ocfm_Excpetion(OCFM_ERR_XML_WRITER_START_ELT_FAILED);
					throw &objException;
				}
			}
		}
		xmlTextWriterSetIndent(pxtwWriter, 1);
		// End General Features Tag
		iBytesWritten = xmlTextWriterEndElement(pxtwWriter);

		//////////////Write MN Features for MN Node////////////////////////////////
		if(objNode.getNodeType() == MN)
		{
			//Start MN Features Tag			
			iBytesWritten = xmlTextWriterStartElement(pxtwWriter, BAD_CAST "MNFeatures");
			if (iBytesWritten < 0) 
			{
				printf("testXmlwriterMemory: Error at xmlTextWriterStartElement\n");
				ocfmException objException;// = new ocfmException;
				objException.ocfm_Excpetion(OCFM_ERR_XML_WRITER_START_ELT_FAILED);
				throw &objException;
			}
			CNetworkManagement *pobjNwManagement;
			pobjNwManagement = objNode.getNetworkManagement();
			
			for(UINT32 uiLoopCount = 0; uiLoopCount < pobjNwManagement->getNumberOfFeatures(); uiLoopCount++)	
			{		
				Feature* feature = NULL;
				feature = pobjNwManagement->getFeature(uiLoopCount);
			
				if(feature->m_featureType == MN_FEATURES)
				{
					iBytesWritten = xmlTextWriterWriteAttribute(pxtwWriter, BAD_CAST feature->m_Name, BAD_CAST feature->Value);		
					if (iBytesWritten < 0) 
					{
						printf("testXmlwriterMemory: Error at xmlTextWriterStartElement\n");
						ocfmException objException;// = new ocfmException;
						objException.ocfm_Excpetion(OCFM_ERR_XML_WRITER_START_ELT_FAILED);
						throw &objException;
					}
				}
			}
			xmlTextWriterSetIndent(pxtwWriter, 1);
			// End MN Features Tag
			iBytesWritten = xmlTextWriterEndElement(pxtwWriter);
				

		}
		//////////////Write CN Features for CN Node////////////////////////////////
		else if(objNode.getNodeType() == CN)
		{
			//Start CN Features Tag
			//Start MN Features Tag			
			iBytesWritten = xmlTextWriterStartElement(pxtwWriter, BAD_CAST "CNFeatures");
			if (iBytesWritten < 0) 
			{
				printf("testXmlwriterMemory: Error at xmlTextWriterStartElement\n");
				ocfmException objException;// = new ocfmException;
				objException.ocfm_Excpetion(OCFM_ERR_XML_WRITER_START_ELT_FAILED);
				throw &objException;
			}
			CNetworkManagement *pobjNwManagement;
			pobjNwManagement = objNode.getNetworkManagement();
			
			for(UINT32 uiLoopCount = 0; uiLoopCount < pobjNwManagement->getNumberOfFeatures(); uiLoopCount++)	
			{		
				Feature* feature = NULL;
				feature = pobjNwManagement->getFeature(uiLoopCount);
			
				if(feature->m_featureType == CN_FEATURES)
				{
					iBytesWritten = xmlTextWriterWriteAttribute(pxtwWriter, BAD_CAST feature->m_Name, BAD_CAST feature->Value);		
					if (iBytesWritten < 0) 
					{
						printf("testXmlwriterMemory: Error at xmlTextWriterStartElement\n");
						ocfmException objException;// = new ocfmException;
						objException.ocfm_Excpetion(OCFM_ERR_XML_WRITER_START_ELT_FAILED);
						throw &objException;
					}
				}
			}
			xmlTextWriterSetIndent(pxtwWriter, 1);
			//End CN Features Tag
			iBytesWritten = xmlTextWriterEndElement(pxtwWriter);			
		}

		//End Network Management Tag
		xmlTextWriterSetIndent(pxtwWriter, 1);
		// End ProfileBody Tag
		iBytesWritten = xmlTextWriterEndElement(pxtwWriter);
		if (iBytesWritten < 0)
		{
			printf("testXmlwriterDoc: Error at xmlTextWriterEndElement\n");
			ocfmException objException;// = new ocfmException;
			objException.ocfm_Excpetion(OCFM_ERR_XML_WRITER_END_ELT_FAILED);
			throw &objException;
		}
		
		xmlTextWriterSetIndent(pxtwWriter, 1);
		// End ISO15745ProfileContainer Tag
		iBytesWritten = xmlTextWriterEndElement(pxtwWriter);
		if (iBytesWritten < 0)
		{
			printf("testXmlwriterDoc: Error at xmlTextWriterEndElement\n");
			ocfmException objException;// = new ocfmException;
			objException.ocfm_Excpetion(OCFM_ERR_XML_WRITER_END_ELT_FAILED);
			throw &objException;
		}
				
		//cout << "\nEnd Write Doc\n" <<endl;
		iBytesWritten = xmlTextWriterEndDocument(pxtwWriter);
		if (iBytesWritten < 0) 
		{
			printf("testXmlwriterDoc: Error at xmlTextWriterEndDocument\n");
			ocfmException objException;// = new ocfmException;
			objException.ocfm_Excpetion(OCFM_ERR_XML_END_DOC_FAILED);
			throw &objException;
		}

		xmlFreeTextWriter(pxtwWriter);

		xmlSaveFileEnc(pbFileName, pxdDoc, MY_ENCODING);

		xmlFreeDoc(pxdDoc);
	}
	catch(ocfmException *ex)
	{
		throw ex;
	}

	stErrInfo.code = OCFM_ERR_SUCCESS;
	return stErrInfo;	
}

/**************************************************************************************************
* Function Name: setFlagForRequiredCNIndexes
* Description: 
* Return Value: void
****************************************************************************************************/
void setFlagForRequiredCNIndexes(INT32 iNodeId)
{
	CIndex* pobjIndex;
	CIndexCollection* pobjIdxCol;
	CNodeCollection* pobjNodeCol;
	CNode* pobjNode;
	
	pobjNodeCol =  CNodeCollection::getNodeColObjectPointer();
	pobjNode 	=  pobjNodeCol->getNodePtr(CN, iNodeId);
	pobjIdxCol	= pobjNode->getIndexCollection();
	
	INT32 iIndexCount = pobjIdxCol->getNumberofIndexes();
	
	for(INT32 iLoopCount = 0; iLoopCount < iIndexCount; iLoopCount++)
	{
		pobjIndex = pobjIdxCol->getIndex(iLoopCount);
		
		if(CheckIfNotPDO((char*)pobjIndex->getIndexValue()) == false ||
		strcmp((char*)pobjIndex->getIndexValue(),"1F98") == 0 ||
		strcmp((char*)pobjIndex->getIndexValue(),"1020") == 0 ||
		strcmp((char*)pobjIndex->getIndexValue(),"1006") == 0 ||
		CheckIfManufactureSpecificObject((char*)pobjIndex->getIndexValue()))
		{
			pobjIndex->setFlagIfIncludedCdc(TRUE);
			for(INT32 iSIDxCount = 0; iSIDxCount < pobjIndex->getNumberofSubIndexes(); iSIDxCount++)
			{
				CSubIndex* pobjSIdx;
				pobjSIdx = pobjIndex->getSubIndex(iSIDxCount);
				pobjSIdx->setFlagIfIncludedCdc(TRUE);				
			}
		}
	}				
}
/**************************************************************************************************
* Function Name: setFlagForRequiredMNIndexes
* Description: 
* Return Value: void
****************************************************************************************************/
void setFlagForRequiredMNIndexes(INT32 iNodeId)
{
	CIndex* pobjIndex;
	CIndexCollection* pobjIdxCol;
	CNodeCollection* pobjNodeCol;
	CNode* pobjNode;
	
	pobjNodeCol =  CNodeCollection::getNodeColObjectPointer();
	pobjNode 	=  pobjNodeCol->getNodePtr(MN, iNodeId);
	pobjIdxCol	= pobjNode->getIndexCollection();
	
	INT32 iIndexCount = pobjIdxCol->getNumberofIndexes();
	
	for(INT32 iLoopCount = 0; iLoopCount < iIndexCount; iLoopCount++)
	{
		pobjIndex = pobjIdxCol->getIndex(iLoopCount);
		
		if(CheckIfNotPDO((char*)pobjIndex->getIndexValue()) == false ||
		strcmp((char*)pobjIndex->getIndexValue(),"1006") == 0 ||
		strcmp((char*)pobjIndex->getIndexValue(),"1020") == 0 ||
		strcmp((char*)pobjIndex->getIndexValue(),"1300") == 0 ||
		strcmp((char*)pobjIndex->getIndexValue(),"1C02") == 0 ||
		strcmp((char*)pobjIndex->getIndexValue(),"1C09") == 0 ||
		strcmp((char*)pobjIndex->getIndexValue(),"1F26") == 0 ||
		strcmp((char*)pobjIndex->getIndexValue(),"1F27") == 0 ||
		strcmp((char*)pobjIndex->getIndexValue(),"1F84") == 0 ||
		strcmp((char*)pobjIndex->getIndexValue(),"1F8B") == 0 ||
		strcmp((char*)pobjIndex->getIndexValue(),"1F8D") == 0 ||
		strcmp((char*)pobjIndex->getIndexValue(),"1F92") == 0 ||
		strcmp((char*)pobjIndex->getIndexValue(),"1F98") == 0)
		{
			pobjIndex->setFlagIfIncludedCdc(TRUE);
			for(INT32 iSIDxCount = 0; iSIDxCount < pobjIndex->getNumberofSubIndexes(); iSIDxCount++)
			{
				CSubIndex* pobjSIdx;
				pobjSIdx = pobjIndex->getSubIndex(iSIDxCount);
				pobjSIdx->setFlagIfIncludedCdc(TRUE);				
			}
		}
	}				
}

/**************************************************************************************************
* Function Name: AddOtherRequiredCNIndexes
* Description: 
* Return Value: ocfmRetCode
****************************************************************************************************/
ocfmRetCode AddOtherRequiredCNIndexes(INT32 iNodeId)
{
	ocfmRetCode stRetCode;
	char* MNIndex 	= new char[INDEX_LEN];		
	char* Sidx 		=  new char[SUBINDEX_LEN];
	CIndexCollection* pobjIdxCol;
	CNodeCollection* pobjNodeCol;
	CNode* pobjNode;
	
	try
	{
		pobjNodeCol =  CNodeCollection::getNodeColObjectPointer();
		pobjNode =  pobjNodeCol->getNodePtr(CN, iNodeId);
		pobjIdxCol = pobjNode->getIndexCollection();
			
		/* Add 1006*/
		strcpy(MNIndex, "1020");
		#if defined DEBUG	
			cout << "string copied" << endl;			
		#endif
		stRetCode = AddIndex(pobjNode->getNodeId(), CN, MNIndex);
		#if defined DEBUG	
			cout << "stRetCode" << stRetCode.code<<endl;
			cout<< "1020 added"<<endl;
		#endif
		if((stRetCode.code != 0) && (stRetCode.code != OCFM_ERR_INDEX_ALREADY_EXISTS ))
			return stRetCode;
			
		char* Val = new char[16];
		Val = _IntToAscii(iConfigDate,Val, 10);
		
		/* Set 5ms pxcValue*/
		/* Set subindex pxcValue 40 or 0000028 */					
		strcpy(Sidx, "01");
		SetSIdxValue(MNIndex, Sidx, Val, pobjIdxCol, pobjNode->getNodeId(), CN,  false);
			
		Val = _IntToAscii(iConfigTime,Val, 10);
		
		strcpy(Sidx, "02");
		SetSIdxValue(MNIndex, Sidx, Val, pobjIdxCol, pobjNode->getNodeId(), CN, false);				
	}
	catch(ocfmException& ex)
	{
		return ex._ocfmRetCode;
	}	
	stRetCode.code = OCFM_ERR_SUCCESS;
	return stRetCode;
}

/**************************************************************************************************
* Function Name: getDataSize
* Description: 
* Return Value: int
****************************************************************************************************/										
int getDataSize(char* dataTypeVal)
{
		if(strcmp(StringToUpper(dataTypeVal),"UNSIGNED8")==0 || 
		strcmp(StringToUpper(dataTypeVal),"BOOLEAN")==0			|| 
		strcmp(StringToUpper(dataTypeVal),"INTEGER8")==0)					
		{
			return 1;					
		}			
		else if(strcmp(StringToUpper(dataTypeVal),"UNSIGNED16")==0 ||
		strcmp(StringToUpper(dataTypeVal), "INTEGER16")==0)		
		{
			return 2;		
		}			
			
		else if(strcmp(StringToUpper(dataTypeVal),"INTEGER24")==0 || 
		strcmp(StringToUpper(dataTypeVal),"UNSIGNED24")==0)
		{
			return 3;			
		}	
		else if(strcmp(StringToUpper(dataTypeVal),"UNSIGNED32")==0 ||
		strcmp(StringToUpper(dataTypeVal),"INTEGER32")==0 ||
		strcmp(StringToUpper(dataTypeVal),"REAL32")==0)
		{
			return 4;		
		}			
		else if(strcmp(StringToUpper(dataTypeVal),"INTEGER40")==0 ||
		strcmp(StringToUpper(dataTypeVal),"UNSIGNED40")==0)
		{
			return 5;
		}	
		else if(strcmp(StringToUpper(dataTypeVal),"INTEGER48")==0 || 
		strcmp(StringToUpper(dataTypeVal),"UNSIGNED48")==0 )
		{
			return 6;			
		}	
		else if(strcmp(StringToUpper(dataTypeVal),"INTEGER56")==0 || 
		strcmp(StringToUpper(dataTypeVal),"UNSIGNED56")==0)
		{
			return 7;			
		}	
		else if(strcmp(StringToUpper(dataTypeVal),"UNSIGNED64")==0 || 
		strcmp(StringToUpper(dataTypeVal),"INTEGER64")==0 ||
		strcmp(StringToUpper(dataTypeVal),"REAL64")==0)
		{
			return 8;
		}	
		else if(strcmp(StringToUpper(dataTypeVal),"MAC_ADDRESS")==0)
		{
			return 6;		
		}	
		else if(strcmp(StringToUpper(dataTypeVal),"IP_ADDRESS")==0)
		{
			return 4;			
		}	
		/* NETTIME is composed as follows:
									STRUCT OF
									UNSIGNED32 seconds
									UNSIGNED32 nanoseconds */
		else if(strcmp(StringToUpper(dataTypeVal),"NETTIME")==0)
		{
			return 8;		
		}			
		/*STRUCT OF
							UNSIGNED28 ms,
							VOID4 reserved,
							UNSIGNED16 days
						TIME_DIFFERENCE*/			
		else if(strcmp(StringToUpper(dataTypeVal),"TIME_DIFF")==0)
		{
			return 6;		
		}	
		/*STRUCT OF
						UNSIGNED28 ms,
						VOID4 reserved,
						UNSIGNED16 days
					TIME_DIFFERENCE*/
		else if(strcmp(StringToUpper(dataTypeVal),"TIME_OF_DAY")==0)
		{
			return 6;			
		}	
	return 1; //by default
}

/**************************************************************************************************
* Function Name: checkIfStringDatatypes
* Description: 
* Return Value: bool
****************************************************************************************************/
bool checkIfStringDatatypes(char* datatypeValue)
{
	if(strcmp(StringToUpper(datatypeValue), "VISIBLE_STRING") == 0 ||
	strcmp(StringToUpper(datatypeValue), "OCTET_STRING") == 0 ||
	strcmp(StringToUpper(datatypeValue), "UNICODE_STRING") == 0)
		return true;
	else 
		return false;
}
/**************************************************************************************************
* Function Name: setFeature
* Description:	
* Return pxcValue: void
***************************************************************e*************************************/
void setFeatures(xmlTextReaderPtr pxtrReader, Feature *pstFeature)
{
	const xmlChar *pxcName  = NULL;
	const xmlChar *pxcValue = NULL;

	
	//Retrieve the pxcName and Value of an attribute
	pxcValue = xmlTextReaderConstValue(pxtrReader);
	pxcName  = xmlTextReaderConstName(pxtrReader);							

	pstFeature->m_Name = new char[strlen((char*)pxcName) + STR_ALLOC_BUFFER];
	strcpy((char*)pstFeature->m_Name, (char*)pxcName);

	pstFeature->m_Name = new char[strlen((char*)pxcName) + STR_ALLOC_BUFFER];
	strcpy((char*)pstFeature->Value, (char*)pxcValue);


}
