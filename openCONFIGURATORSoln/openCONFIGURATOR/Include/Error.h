#ifndef error_h
#define error_h
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

typedef enum
{
					OCFM_ERR_SUCCESS					= 0,
					OCFM_ERR_FILE_NOT_PRESENT,
					OCFM_ERR_FILE_CANNOT_OPEN,			
					OCFM_ERR_INVALID_NODEID,
					OCFM_ERR_INVALID_INDEXID,
					OCFM_ERR_INVALID_SUBINDEXID,
					OCFM_ERR_NO_NODES_FOUND,
					OCFM_ERR_NO_INDEX_FOUND,
					OCFM_ERR_NO_SUBINDEXS_FOUND,
					OCFM_ERR_NODEID_NOT_FOUND,
					OCFM_ERR_INDEXID_NOT_FOUND,
					OCFM_ERR_SUBINDEXID_NOT_FOUND,
					OCFM_ERR_NODE_ALREADY_EXISTS,
					OCFM_ERR_INDEX_ALREADY_EXISTS,
					OCFM_ERR_SUBINDEX_ALREADY_EXISTS,					
					OCFM_ERR_INVALID_VALUE,
					OCFM_ERR_INVALID_NAME,
					OCFM_ERR_XML_FILE_CORRUPTED,
					OCFM_ERR_CANNOT_OPEN_FILE,
					OCFM_ERR_PARSE_XML,
					OCFM_ERR_UNKNOWN					

	
}EConfiuguratorErrors;

typedef struct ocfmRetCode
{
				EConfiuguratorErrors				code;/* Error code from EConfiuguratorErrors*/
				char*		errorString; /* String Describes the error */
				
}ocfmRetCode;

typedef struct ocfmRetValError
{
	ocfmRetCode errCode;
	int returnValue;
}ocfmRetValError;

#endif // Error_h

