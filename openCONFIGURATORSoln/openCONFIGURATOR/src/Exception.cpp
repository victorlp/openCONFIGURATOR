///////////////////////////////////////////////////////////////////////////////////////////////
//
//  $Source: $
//
// NAME:  Exception.cpp
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
#include <exception>
#include <libxml/xmlerror.h>
#include "../Include/Exception.h"

/*
class ocfmException: public exception
{
	  virtual const char* what() const throw()
  {
    return "Node Exception Handled: Node does not exsist";
  }
} NodeEx;
*/

/****************************************************************************************************
* FUNCTION DEFINITIONS
****************************************************************************************************/

/****************************************************************************************************
* Constructor
****************************************************************************************************/
ocfmException::ocfmException(void)
{
	_ocfmRetCode.code = OCFM_ERR_SUCCESS;
}

/****************************************************************************************************
* Destructor
****************************************************************************************************/
ocfmException::~ocfmException(void) throw()
{
	//Add destructor code here	
}

/****************************************************************************************************
* Function Name: ocfmException::ocfm_Excpetion
* Description:
* Return value: void
****************************************************************************************************/
void ocfmException::ocfm_Excpetion(EConfiuguratorErrors enumErrcode)
{	
	_ocfmRetCode.code 		 = enumErrcode;
	_ocfmRetCode.errorString = new char[ERR_STRING_LEN];
	
	switch(enumErrcode)
	{
		case OCFM_ERR_FILE_NOT_PRESENT:
				strcpy(_ocfmRetCode.errorString,"File not present");
				break;
		case OCFM_ERR_FILE_CANNOT_OPEN:
				strcpy(_ocfmRetCode.errorString,"File cannot open");
				break;
		case OCFM_ERR_INVALID_NODEID:
				strcpy(_ocfmRetCode.errorString,"Invalid Node ID");
				break;
		case OCFM_ERR_INVALID_NODEPOS:
				strcpy(_ocfmRetCode.errorString,"Invalid Node position");
				break;
		case OCFM_ERR_INVALID_NODETYPE:
				strcpy(_ocfmRetCode.errorString,"Invalid Node type");
				break;
		case OCFM_ERR_DATATYPE_NOT_FOUND:
				strcpy(_ocfmRetCode.errorString, "DataType Not Found in the DataTypeList of XDC");
				break;
		case OCFM_ERR_INVALID_INDEXID:
				strcpy(_ocfmRetCode.errorString,"Invalid Index ID");
				break;
		case OCFM_ERR_INVALID_INDEXPOS:
				strcpy(_ocfmRetCode.errorString,"Invalid Index position");
				break;
		case OCFM_ERR_INVALID_SUBINDEXID:
				strcpy(_ocfmRetCode.errorString,"Invalid SubIndex ID");
				break;
		case OCFM_ERR_INVALID_SUBINDEXPOS:
				strcpy(_ocfmRetCode.errorString,"Invalid SubIndex position");
				break;
		case OCFM_ERR_INVALID_ATTRIBUTETYPE:
				strcpy(_ocfmRetCode.errorString,"Invalid Attribute type");
				break;												
		case OCFM_ERR_NO_NODES_FOUND:
				strcpy(_ocfmRetCode.errorString,"No Nodes found");
				break;
		case OCFM_ERR_NO_INDEX_FOUND:
				strcpy(_ocfmRetCode.errorString,"No Indexes found");
				break;
		case OCFM_ERR_NO_SUBINDEXS_FOUND:
				strcpy(_ocfmRetCode.errorString,"No SubIndexes found");
				break;
		case OCFM_ERR_NODEID_NOT_FOUND:
				strcpy(_ocfmRetCode.errorString,"Node ID not found");
				break;
		case OCFM_ERR_INDEXID_NOT_FOUND:
				strcpy(_ocfmRetCode.errorString,"Index ID not found");
				break;
		case OCFM_ERR_SUBINDEXID_NOT_FOUND:
				strcpy(_ocfmRetCode.errorString,"SubIndex ID not found");
				break;
		case OCFM_ERR_NODE_ALREADY_EXISTS:
				strcpy(_ocfmRetCode.errorString,"Node Already Exists");				
				break;						
		case OCFM_ERR_INDEX_ALREADY_EXISTS:
				strcpy(_ocfmRetCode.errorString,"Index Already Exists");
				break;
		case OCFM_ERR_SUBINDEX_ALREADY_EXISTS:
				strcpy(_ocfmRetCode.errorString,"SubIndex Already Exists");
				break;						
		case OCFM_ERR_INVALID_VALUE:
				strcpy(_ocfmRetCode.errorString,"Invalid value");
				break;
		case OCFM_ERR_INVALID_NAME:
				strcpy(_ocfmRetCode.errorString,"Invalid name");
				break;
		case OCFM_ERR_XML_FILE_CORRUPTED:
				strcpy(_ocfmRetCode.errorString,"XML file corrupted");
				break;
		case OCFM_ERR_CANNOT_OPEN_FILE:
				strcpy(_ocfmRetCode.errorString,"Cannot open file");
				break;
		case OCFM_ERR_PARSE_XML:
				strcpy(_ocfmRetCode.errorString,"Cannot parse XML");
				break;
		case OCFM_ERR_MODULE_INDEX_NOT_FOUND:
				strcpy(_ocfmRetCode.errorString,"PDO Mapped Module Index Not Found");
				break;						
		case OCFM_ERR_MODULE_SUBINDEX_NOT_FOUND:
				strcpy(_ocfmRetCode.errorString,"PDO Mapped Module SubIndex Not Found");
				break;						
		case OCFM_ERR_UNIQUE_ID_REF_NOT_FOUND:
				strcpy(_ocfmRetCode.errorString, "Unique ID reference not found");
				break;
		case OCFM_ERR_STRUCT_DATATYPE_NOT_FOUND:
				strcpy(_ocfmRetCode.errorString, "Struct Datatype not found");
				break;				
		case OCFM_ERR_NO_CN_NODES_FOUND:
				strcpy(_ocfmRetCode.errorString, "NO CN Nodes are found");
				break;
		case OCFM_ERR_UNKNOWN:
				strcpy(_ocfmRetCode.errorString, "Unknown error");
				break;
		case OCFM_ERR_VALUE_NOT_WITHIN_RANGE:
				strcpy(_ocfmRetCode.errorString, "Value out of range");
				break;
		case OCFM_ERR_MN_NODE_DOESNT_EXIST:
				strcpy(_ocfmRetCode.errorString, "MN Node doesnt exist");
				break;					
		case OCFM_ERR_CREATE_XML_WRITER_FAILED:
				strcpy(_ocfmRetCode.errorString, "Error creating the xml writer");
				break;
		case OCFM_ERR_XML_WRITER_START_ELT_FAILED:
				strcpy(_ocfmRetCode.errorString, "Error at xmlTextWriterStartElement");
				break;
		case OCFM_ERR_XML_WRITER_END_ELT_FAILED:
				strcpy(_ocfmRetCode.errorString, "Error at xmlTextWriterEndElement");
				break;
		case OCFM_ERR_XML_START_DOC_FAILED:
				strcpy(_ocfmRetCode.errorString, "Error at xmlTextWriterStartDocument");
				break;
		case OCFM_ERR_XML_END_DOC_FAILED:
				strcpy(_ocfmRetCode.errorString, "Error at xmlTextWriterEndDocument");
				break;		
		case OCFM_ERR_CANNOT_OPEN_PROJECT_VER_MISMATCH:
				strcpy(_ocfmRetCode.errorString, "Cannot open project: Tool-Project version mismatch");
				break;
		case OCFM_ERR_INVALID_PJTXML:
				strcpy(_ocfmRetCode.errorString, "Cannot open project: Invalid Project XML");		
				break;
		case OCFM_ERR_PROJECT_SETTINGS:
				strcpy(_ocfmRetCode.errorString, "Project Settings are NULL");
				break;
		case OCFM_ERR_INVALID_DATATYPE_FOR_PDO:
				strcpy(_ocfmRetCode.errorString, "Invalid Datatype for Mapped Object");
				break;
		case OCFM_ERR_XAP_FILE_NOT_WRITTEN:
				strcpy(_ocfmRetCode.errorString, "XAP File Not Written");	
				break;
		case OCFM_ERR_MAX_PI_SIZE:
				strcpy(_ocfmRetCode.errorString, "MAX PI Size(4064 bytes) crossed");
				break;
		default:
				strcpy(_ocfmRetCode.errorString, "Unhandled Error");
				break;
	}
}
