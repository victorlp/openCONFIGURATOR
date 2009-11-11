%module openConfiguratorWrapper
#define DllExport
/* Windows Platform */
#if defined(_WIN32) && defined(_MSC_VER)
	 #undef DllExport
	 #define DllExport  __declspec(dllexport)
	 
#endif

%{
#include "../openCONFIGURATOR/Include/ApplicationProcess.h"
#include "../openCONFIGURATOR/Include/BaseIndex.h"
#include "../openCONFIGURATOR/Include/ComplexDataType.h"
#include "../openCONFIGURATOR/Include/Error.h"

#include "../openCONFIGURATOR/Include/Exports.h"
#include "../openCONFIGURATOR/Include/DataTypeCollection.h"
#include "../openCONFIGURATOR/Include/Declarations.h"
#include "../openCONFIGURATOR/Include/globals.h"
#include "../openCONFIGURATOR/Include/IndexCollection.h"
#include "../openCONFIGURATOR/Include/Index.h"
#include "../openCONFIGURATOR/Include/NodeCollection.h"
#include "../openCONFIGURATOR/Include/Node.h"
#include "../openCONFIGURATOR/Include/SubIndex.h"


%}

%include cpointer.i
%include cstring.i
%pointer_functions(char, charp)
%pointer_functions(long, longp)
%pointer_functions(int, intp)
%pointer_functions(bool, boolp)
%pointer_functions(EAutoGenerate, EAutoGeneratep)
%pointer_functions(EAutoSave, EAutoSavep)
%pointer_functions(EViewMode, EViewModep)
%pointer_functions(EStationType, EStationTypep)
%cstring_bounded_output(char *Out_AttributeValue, 512);
%cstring_bounded_output(char *Out_IndexID, 100);
%cstring_bounded_output(char *Out_SubIndexID, 100);
%cstring_bounded_output(char *Out_NodeName, 100);
%cstring_bounded_output(char *Out_FeatureValue, 512);
%cstring_bounded_output(char *Out_DataTypes, 1000);

%include typemaps.i
%include exception.i
%include std_except.i

/* Let's just grab the original header file here */

%include "../openCONFIGURATOR/Include/ApplicationProcess.h"
%include "../openCONFIGURATOR/Include/BaseIndex.h"
%include "../openCONFIGURATOR/Include/ComplexDataType.h"
%include "../openCONFIGURATOR/Include/Error.h"

%include "../openCONFIGURATOR/Include/Exports.h"
%include "../openCONFIGURATOR/Include/DataTypeCollection.h"
%include "../openCONFIGURATOR/Include/Declarations.h"
%include "../openCONFIGURATOR/Include/globals.h"
%include "../openCONFIGURATOR/Include/IndexCollection.h"
%include "../openCONFIGURATOR/Include/Index.h"
%include "../openCONFIGURATOR/Include/NodeCollection.h"
%include "../openCONFIGURATOR/Include/Node.h"
%include "../openCONFIGURATOR/Include/SubIndex.h"


using namespace std; 


%inline %{

ocfmRetCode ImportXML(char* fileName, int NodeID, ENodeType NodeType);
ocfmRetCode ReImportXML(char* fileName, int NodeID, ENodeType NodeType);

ocfmRetCode GenerateXAP(char* fileName);
ocfmRetCode GenerateCDC(char* fileName);

ocfmRetCode CreateNode(int NodeID, ENodeType NodeType, char* NodeName);
ocfmRetCode parseFile(char* filename, int NodeID, ENodeType  NodeType);
ocfmRetCode DeleteNode(int NodeID, ENodeType NodeType);
ocfmRetCode DeleteNodeObjDict(int NodeID, ENodeType NodeType);

ocfmRetCode DeleteIndex(int NodeID, ENodeType NodeType, char* IndexID);
ocfmRetCode DeleteSubIndex(int NodeID, ENodeType NodeType, char* IndexID, char* SubIndexID);
ocfmRetCode AddIndex(int NodeID, ENodeType NodeType, char* IndexID);
ocfmRetCode AddSubIndex(int NodeID, ENodeType NodeType, char* IndexID, char* SubIndexID);
ocfmRetCode SetIndexAttributes(int NodeID, ENodeType NodeType, char* IndexID, char* IndexValue, char* IndexName, EFlag flagIfInCdc);
//ocfmRetCode SetSubIndexAttributes(int NodeID, ENodeType NodeType, char* IndexID, char* SubIndexID, char* IndexValue, char* IndexName);
ocfmRetCode SetSubIndexAttributes(int NodeID, ENodeType NodeType, char* IndexID, char* SubIndexID, char* IndexValue, char* IndexName, EFlag flagIfInCdc);

ocfmRetCode IfNodeExists(int NodeID, ENodeType NodeType, int* NodePos, bool& ExistfFlag);
ocfmRetCode IfIndexExists(int NodeID, ENodeType NodeType, char* IndexID, int* IndexPos);
ocfmRetCode IfSubIndexExists(int NodeID, ENodeType NodeType, char* IndexID, char* SubIndexID, int* SubIndexPos, int* IndexPos);

ocfmRetCode GetIndexAttributes(int NodeID, ENodeType NodeType, char* IndexID, EAttributeType AttributeType, char* Out_AttributeValue);
ocfmRetCode GetSubIndexAttributes(int NodeID, ENodeType NodeType, char* IndexID, char* SubIndexID, EAttributeType AttributeType, char* Out_AttributeValue);
ocfmRetCode GetNodeCount(int MNID, int* Out_NodeCount);
ocfmRetCode GetIndexCount(int NodeID, ENodeType NodeType, int* Out_IndexCount);
ocfmRetCode GetSubIndexCount(int NodeID, ENodeType NodeType, char* IndexID, int* Out_SubIndexCount);

ocfmRetCode GetNodeAttributesbyNodePos(int NodePos, int* Out_NodeID, char* Out_NodeName, EStationType* Out_eStationType);
ocfmRetCode GetIndexIDbyIndexPos(int NodeID, ENodeType NodeType, int IndexPos, char* Out_IndexID);
ocfmRetCode GetSubIndexIDbySubIndexPos(int NodeID, ENodeType NodeType, char* IndexID, int SubIndexPos, char* Out_SubIndexID);

ocfmRetCode GetIndexIDbyPositions(int NodePos, int IndexPos, char* Out_IndexID);
ocfmRetCode GetSubIndexIDbyPositions(int NodePos, int IndexPos, int SubIndexPos, char* Out_SubIndexID);
ocfmRetCode GetIndexAttributesbyPositions(int NodePos, int IndexPos, EAttributeType AttributeType, char* Out_AttributeValue);
ocfmRetCode GetSubIndexAttributesbyPositions(int NodePos, int IndexPos, int SubIndexPos, EAttributeType AttributeType, char* Out_AttributeValue);

void LoadObjectDictionary(char* fileName);
ocfmRetCode GenerateMNOBD();

ocfmRetCode SetAllIndexAttributes(int NodeID, ENodeType NodeType, 
char* IndexID, char* ActualValue,
char* IndexName, char* Access, char* dataTypeValue,
char* pdoMappingVal, char* defaultValue, char* highLimit,
char* lowLimit, char* objType, EFlag flagIfIncludedInCdc);

ocfmRetCode SetAllSubIndexAttributes(int NodeID, ENodeType NodeType, 
char* IndexID, char* SubIndexID, char* ActualValue,
char* IndexName, char* Access, char* dataTypeValue,
char* pdoMappingVal, char* defaultValue, char* highLimit,
char* lowLimit, char* objType, EFlag flagIfIncludedInCdc);

ocfmRetCode GetProjectSettings(EAutoGenerate* autoGen, EAutoSave* autoSave, EViewMode* viewMode);
ocfmRetCode SetProjectSettings(EAutoGenerate autoGen, EAutoSave autoSave, EViewMode viewMode);
ocfmRetCode FreeProjectMemory();
ocfmRetCode GetFeatureValue(INT32 iNodeId, ENodeType eNodeType, EFeatureType eFeatureType, char* FeatureName, char* Out_FeatureValue);
ocfmRetCode UpdateNodeParams(INT32 iCurrNodeId, INT32 iNewNodeID, ENodeType eNodeType, char* NodeName, EStationType eStationType, char* ForcedCycle);
ocfmRetCode GetNodeDataTypes(INT32 iNodeId, ENodeType eNodeType, char* Out_DataTypes);

%}

