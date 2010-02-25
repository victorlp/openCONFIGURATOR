#ifndef Internal_h
#define Internal_h
///////////////////////////////////////////////////////////////////////////////////////////////
//
// $Header: $
//
// NAME:  Internal.h
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

/************************************************************************************************
* Includes
************************************************************************************************/
#include <string>
#include <libxml/xmlwriter.h>
#include <libxml/xmlreader.h>
#include "Exports.h"
#include "Index.h"
#include "ProjectSettings.h"
#include "ProcessImage.h"
#include "Declarations.h"
#include "Node.h"
#include "IndexCollection.h"
#include "NetworkManagement.h"

/*******************************************************************e *****************************
* Defines
************************************************************************************************/
#define TOOL_VERSION "1.1.0"
#define LAST_TOOL_VERSION "1.0.1"
#define PREV_TOOL_VERSION_1 "1.0.0"
#define PREV_TOOL_VERSION_2 "0.9.1"
#define ALLOC_BUFFER 5
#define INDEX_LEN 5
#define SUBINDEX_LEN 3
#define RANGE_INDEX 3
#define STR_ALLOC_BUFFER 1
#define MAX_PI_SIZE 4064
#define PROJECT_FILE_NAME 500
#define LINUX_INSTALL_DIR "/usr/share/openCONFIGURATOR-"TOOL_VERSION
#define LINUX_INSTALL_DIR_LEN 50

#define EPL_NODEASSIGN_NODE_EXISTS      0x00000001L // Bit 0
#define EPL_NODEASSIGN_NODE_IS_CN       0x00000002L // Bit 1
#define EPL_NODEASSIGN_START_CN         0x00000004L // Bit 2
#define EPL_NODEASSIGN_MANDATORY_CN     0x00000008L // Bit 3
#define EPL_NODEASSIGN_KEEPALIVE        0x00000010L //currently not used in EPL V2 standard
#define EPL_NODEASSIGN_SWVERSIONCHECK   0x00000020L // Bit 5
#define EPL_NODEASSIGN_SWUPDATE         0x00000040L // Bit 6
#define EPL_NODEASSIGN_ASYNCONLY_NODE   0x00000100L // Bit 8
#define EPL_NODEASSIGN_MULTIPLEXED_CN   0x00000200L // Bit 9
#define EPL_NODEASSIGN_RT1              0x00000400L // Bit 10
#define EPL_NODEASSIGN_RT2              0x00000800L // Bit 11
#define EPL_NODEASSIGN_MN_PRES          0x00001000L // Bit 12
#define EPL_NODEASSIGN_CHAINED_CN       0x00800000L // Bit 14
#define EPL_NODEASSIGN_VALID            0x80000000L // Bit 31
/************************************************************************************************
* Enumerations
************************************************************************************************/
typedef enum{
	ADD = 0,
	DELETE
}EOperation;

/************************************************************************************************
* Externs
************************************************************************************************/
extern int iConfigDate;
extern int iConfigTime;
extern UINT32 uiCycleNumber;
/************************************************************************************************
* Global variables
************************************************************************************************/
static const int g_simple_arr_size = 19;
static bool ObjectDictLoaded = false;
//static UINT32 uiCycleNumber = 0;
static const UINT32 uiPreScalar =0;

/****************************************************************************************************
* Function Declarations
****************************************************************************************************/
char* ConvertToUpper(char* str);
char* _IntToAscii( long value, char* result, int base );
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
bool CheckBlockedMNIndexes(char* IndexValue);
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
//CHAR toupper (CHAR ch);
//CHAR tobin(CHAR ch);
int reversedata(UINT8 *actemp1, UINT8 *actemp2, UINT32 size);
char* StringToUpper(char* str);
void setFlagForRequiredCNIndexes(int NodeId);
void UpdateCNCycleTime(CIndexCollection  *objIdxCol,char* cycleTime);
void	UpdateNumberOfEnteriesSIdx(CIndex *objIndex, ENodeType NodeType);
int getDataSize(char* dataTypeVal);
bool checkIfStringDatatypes(char* datatypeValue);
bool CheckIfDataTypeByNameExists(char* dtName, int NodeID);
ocfmRetCode AddOtherMNIndexes(int NodeID);
void AuotgenerateOtherIndexs(CNode* objNode);
void UpdatedCNDateORTime(CIndex* pobjMNIndex, int iNodeId, EDateTime eDT);
void copyPDODefToAct(int iNodeID, ENodeType enumNodeType);
void copyMNPropDefToAct(int iNodeID, ENodeType enumNodeType);
void copySubIndexDefToAct(int iNodeID, ENodeType enumNodeType, char *indexId, char *subIndexId );
void copyIndexDefToAct(int iNodeID, ENodeType enumNodeType, char *indexId );
void setFlagForRequiredMNIndexes(INT32 iNodeId);
bool CheckAllowedDTForMapping(char* dtName);
void setFeatures(xmlTextReaderPtr reader, Feature* stFeature);
CIndex getPDOIndexByOffset(CIndex* objIndex);
char* setNodeAssigmentBits(CNode* objNode);
char* getLastAvailableCycleNumber();
ocfmRetCode RecalculateMultiplex();
void checkAndCorrectName(char* checkName);
void CheckAndReAssignMultiplex(int iCNNodeId, char* CycleValue );
ocfmRetCode CheckMutliplexAssigned();
UINT32 getFreeCycleNumber(UINT32 uiCycleNumber);
bool IsMultiplexCycleNumberContinuous(UINT32 uiCycleNumber);
void calculateCNPollResponse(int iNodeID, ENodeType enumNodeType);
ocfmRetCode CheckUpperAndLowerLimits(char* pcLowLimit, char* pcHighLimit);
ocfmRetCode AddSubobject(INT32 iNodeID, ENodeType enumNodeType, char* pbIndexID);
CSubIndex* getMNSubIndexValues(char* pbIndex, char* pbSubIndex);
void UpdateCNAsyncMTUsize(CIndexCollection  *pobjIdxCol,char* pbAsyncMTUsize);
void UpdateCNMultiPrescal(CIndexCollection  *pobjIdxCol,char* pbMultiPrescal);
bool checkIfValueZero(char* pcValue);
INT32 GetDecimalValue(char* pcValue);
void UpdateCNNodeAssignment(CNode*  pobjNode);
void ResetMultiplexedCNForceCycle();
bool CheckIfMultiplexedCNExist();
void CopyCustomErrorString(ocfmRetCode* stRet, char* pcCustomErrString);
void UpdateCNMultipleCycleAssign(CNode*  pobjNode);
void UpdateMNNodeAssignmentIndex(CNode *pobjNode, INT32 CNsCount, char* pcIndex, bool allowMNSubindex);
void RecalculateCNPresTimeout(char* pbSubIndexId);
bool ValidateCNPresTimeout(char* pbSubIndexId, char* pcCheckValue);
void CopyOldNodeIdAssignmentObject(CNode* pobjNode, INT32 iOldNodeId);
void CopyOldNodeIdAssignmentObjectSubindex(CNode* pobjNode, INT32 iOldNodeId, char* pcIndex);
bool CheckAccessTypeForInclude(char* pcAccesstype);
CSubIndex* DuplicateSubIndexObject(CSubIndex* pobjSubindex);
void EnableDisableMappingPDO(CIndexCollection* pobjIdxCol, CIndex* objIndex, char* Buffer, bool EnableFlag);
void FormatCdc(CIndexCollection *objIndexCollection, char* Buffer1, FILE* fileptr, ENodeType eNodeType );
bool IsDefaultActualNotEqual(CBaseIndex* pBaseIndexObject);
bool ReactivateMappingPDO(CIndexCollection* pobjIndexCol, CIndex* pobjIndex);
#endif // internal_h
