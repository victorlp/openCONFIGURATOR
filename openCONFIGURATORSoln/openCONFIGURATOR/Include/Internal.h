#ifndef Internal_h
#define Internal_h
///////////////////////////////////////////////////////////////////////////////////////////////
//
// $Header: $
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
#include <string>
#include "Exports.h"
#include "Index.h"
#include "ProjectSettings.h"
#include "ProcessImage.h"
#include "Declarations.h"
#include "Node.h"
#include "IndexCollection.h"
#include <libxml/xmlwriter.h>
#include <libxml/xmlreader.h>
#define TOOL_VERSION "1.0.0"
#define ALLOC_BUFFER 5
#define INDEX_LEN 5
#define SUBINDEX_LEN 3
#define RANGE_INDEX 3
extern int ConfigDate;
extern int ConfigTime;
static const int g_simple_arr_size = 19;
typedef enum{
ADD = 0,
DELETE
}EOperation;

/****************************************************************************************************
* Function Declarations
****************************************************************************************************/

//void LoadObjectDictionary(char* fileName);
char* ConvertToUpper(char* str);
char* _IntToAscii( int value, char* result, int base );
bool CheckIfNotPDO(char* Index);
void CreateTree();
bool CheckIfNotPDO(char* Index);
void ProcessUniqueIDRefs();
char* subString(char* str, int startpos, int len);
char* reverse(char* str);
ocfmRetCode ProcessPDONodes();
bool CheckIfManufactureSpecificObject(char* Index);
bool IsAscii(char c);
int lenOfCNBuffer(char* Buffer);
char* padLeft(char* str, char padChar, int padLength);
unsigned long hex2int(char *a);
CIndex* getMNIndexValues(char* Index);
bool CheckAllowedCNIndexes(char* IndexValue);
int ConvertCdcToBinary(char* fileName,char* tempFile);
char* getParameterAccess(char* access);
bool CheckEndElement(int NodeType, char* element, char* comparewith);
bool CheckStartElement(int NodeType, char* element, char* comparewith);
bool CheckifSimpleDT(char* Name, char* size);
char* getPIName(int NodeID);
void setIndexAttributes(xmlTextReaderPtr reader, CIndex* objIndex, bool& hasPDO);
void setSubIndexAttributes(xmlTextReaderPtr reader, CSubIndex* objSubIndex);
void setDataTypeAttributes(xmlTextReaderPtr reader ,DataType* objDataType);
char* ConvertToHexformat(char* hexValue, int padlength, bool DoPadding);
int getCNDataLen(char* Buffer);
bool CheckIfDataTypeExists(char* dtVal, int NodeID);
bool CheckIfMappingPDO(char* Index);
bool CheckIfHex(char* value);
void WriteXAPElements(ProcessImage piCol[], xmlTextWriterPtr& writer,int VarCount, EPIDirectionType piType);
void StartXAPxml(xmlTextWriterPtr& writer,  xmlDocPtr& doc);
void EndWrtitingXAP( xmlTextWriterPtr& writer, char* fileName, xmlDocPtr& doc);
bool CheckIfSubIndexExists(int NodeID, ENodeType NodeType, char* IndexID, char* SubIndexID);
ocfmRetCode processProjectXML(xmlTextReaderPtr reader, char* PjtPath);
bool IfVersionNumberMatches(xmlTextReaderPtr reader);
bool setProjectSettings_Auto(xmlTextReaderPtr reader);
bool setProjectSettings_Communication(xmlTextReaderPtr reader);
int getConfigDate();
int getConfigTime();
bool getandCreateNode(xmlTextReaderPtr reader, char* PjtPath);
bool saveProjectXML(char* ProjectPath, char* ProjectName);
void SetSIdxValue(char* Idx, char* SIdx,
																	 char* value, CIndexCollection * objIdxCol,
																		int NodeId, ENodeType NodeType, bool setDefaultValue);
ocfmRetCode AddOtherRequiredCNIndexes(int NodeId);
void CreateMNPDOVar(int Offset, int dataSize,IEC_Datatype dtenum, EPDOType pdoType, CNode *objNode);
int getCNsTotalIndexSubIndex(int NodeID);
CHAR toupper (CHAR ch);
CHAR tobin(CHAR ch);
int reversedata(UINT8 *actemp1, UINT8 *actemp2, UINT32 size);
char* StringToUpper(char* str);
void setFlagForRequiredIndexes(int NodeId);
void UpdateCNCycleTime(CIndexCollection  *objIdxCol,char* cycleTime);
void	UpdateNumberOfEnteriesSIdx(CIndex *objIndex, ENodeType NodeType);
int getDataSize(char* dataTypeVal);
bool checkIfStringDatatypes(char* datatypeValue);
#endif // internal_h
