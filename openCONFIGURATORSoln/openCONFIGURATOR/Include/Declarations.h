#ifndef declarations_h
#define declarations_h
///////////////////////////////////////////////////////////////////////////////////////////////
//
// $Header: $
//
// NAME:  
//
// PURPOSE:  purpose description
//
// AUTHOR:   Kalycito Powerlink Team
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
#include <string.h>
#include <stdlib.h>
#include "Exports.h"
#include "Error.h"

//using namespace std;

/************************************************************************************************
* Constants
************************************************************************************************/
#define MN_NODEID 240
const int OFFSET_INTEGER_8 = 0;
const int OFFSET_UNSIGNED_INTEGER_8 = 0;

/****************************************************************************************************
* Defines
****************************************************************************************************/
#define OFFSET_FIRST_INDEX_IN_MN_OBD	4

#define OFFSET_NUM_ENTRIES_MN 0
#define OFFSET_NUM_ENTRIES_CN 7

#define OFFSET_INDEX	0
#define OFFSET_SUBINDEX 2
#define OFFSET_SIZE		3
#define OFFSET_DATA		7
#define	INDEX_SIZE		4
#define	SUBINDEX_SIZE	2
#define NODE_ID			3
#define ERR_STRING_LEN	100
#define OFFSET_FIRST_INDEX_IN_1F22 11

/****************************************************************************************************
* Typedefs
****************************************************************************************************/

//typedef	signed 	 char	CHAR;
typedef unsigned char	UINT8;
typedef unsigned short	UINT16;
typedef unsigned int	UINT32;
typedef signed int		INT32;
/************************************************************************************************
Enumerations
************************************************************************************************/

typedef enum 
{	
	DEFTYPE		= 5,
	DEFSTRUCT	= 6,
	VAR			= 7,
	ARRAY		= 8,
	RECORD		= 9	
}EObjectType;

DllExport typedef enum 
{
	NO,
	DEFAULT,
	OPTIONAL,
	RPDO,
	TPDO
}EPDOMapping;

typedef EPDOMapping *e_PDOMapping;

typedef enum 
{
	MN			= 0,
	CN			= 1
}ENodeType;

typedef enum
{
	PDO_TPDO	= 1,
	PDO_RPDO	= 2
}EPDOType;

typedef enum 
{
	GENERAL_FEATURES = 0,
	MN_FEATURES,
	CN_FEATURES
}EFeatureType;

typedef enum
{	
	 constant,
	 ro,			 /* read access only (default value)*/
	 wr,			 /* write access only */
	 rw,	 /* both read and write access*/
	 readWriteInput, /*both read and write access, but represents process input data*/
	 readWriteOutput, /*both read and write, access, but represents process output data*/
	 noAccess				 /*access denied				*/
}EParameterAccess;

typedef enum IEC_Datatype
{
	BITSTRING = 0,
	BOOL,
	BYTE,
	_CHAR,
	DWORD,
	LWORD,
	SINT,
	INT,
	DINT,
	LINT,
	USINT,
	UINT,
	UDINT,
	ULINT,
	REAL,
	LREAL,
	STRING,
	WSTRING
}IEC_Datatype;

struct DataType
{
	char*		Name;
	char*		DataTypeValue;
	int				DataSize;
	IEC_Datatype IEC_dt;
	char* getName()
	{
		return Name;
	}
	
	void setName(char* DataTypeName)
	{
		Name = new char[strlen(DataTypeName) + 1];
		strcpy(Name,DataTypeName);	
	}
};

typedef struct appProcessCommon
{
	char*						m_Name;
	char*						UniqueId;
	char*						dataType;
	char*						dataTypeUniqueIDRef;

	char* getUniqueID()
	{
		return UniqueId;
	}
	
	void  setUniqueID(char* UniqueID)
	{	
		UniqueId = new char[strlen(UniqueID) + 1];
		strcpy(UniqueId, UniqueID);
	}
	
	char* getName()
	{
		return m_Name;
	}
	
	void  setName(char* attrname)
	{
		m_Name = new char[strlen(attrname) + 1];
		strcpy(m_Name, attrname);
	}
	
	char* getDataType()
	{
		return dataType;
	}
	
	void setDataType(char* dt)
	{
		dataType = new char[strlen(dt) + 1];
		strcpy(dataType, dt );
	}
	
	char* getDtUniqueRefId()
	{
		return dataTypeUniqueIDRef; 
	}
	
	void setDtUniqueRefId(char* UniqueRefID)
	{
		dataTypeUniqueIDRef = new char[strlen(UniqueRefID) + 1];
		strcpy(dataTypeUniqueIDRef, UniqueRefID);
	}
}appProcessCommon;

struct varDeclaration
{
	appProcessCommon* nam_id_dt_attr;
	char		size[5];
	char*		InitialValue;
	char*		StructUniqueId;
	void Initialize()
	{
		nam_id_dt_attr = new appProcessCommon;	
		nam_id_dt_attr->dataType = NULL;
		nam_id_dt_attr->dataTypeUniqueIDRef = NULL;
		nam_id_dt_attr->m_Name = NULL;
		nam_id_dt_attr->UniqueId = NULL;
		strcpy(size, "");
	}	
};

typedef struct Parameter
{
		appProcessCommon name_id_dt_attr;
		char*	dataType;
		int		StructIndex;
		char*	access;
		int		ParaIndex;
		//char*	dataTypeUniqueIDRef;			
}Parameter;

typedef struct Feature
{
	char*			m_Name;
	//DataType		m_dataType;
	char*			m_Value;	
	EFeatureType	m_featureType;
			//char*	dataTypeUniqueIDRef;			
}Feature;

typedef struct MNPdoVariable
{
	char* Index;
	char* SubIndex;
	int Offset;
	char*	 Value;
	int			DataSize;
	EPDOType pdoType;
}MNPdoVariable;

typedef enum
{
	NAME	= 0,	// Name of the Attribute
	OBJECTTYPE,		// Object Type of the Attribute
	DATATYPE,		// Data Type of the Attribute
	ACCESSTYPE,		// Access Type of the Attribute
	DEFAULTVALUE,	// Default value of the Attribute
	ACTUALVALUE,	// Actual value of the Attribute
	PDOMAPPING,		// PDO Mapping of the Attribute
	LOWLIMIT,
	HIGHLIMIT,
	FLAGIFINCDC,	
}EAttributeType;

typedef enum
{
	FALSE = 0,
	TRUE
}EFlag;

typedef enum
{
	DATE = 0,
	TIME
}EDateTime;

typedef enum 
{
	NO_AG = 0,
	YES_AG = 1
}EAutoGenerate;

typedef enum 
{
	YES_AS = 0,
	PROMPT_AS,
	DISCARD_AS
}EAutoSave;
typedef enum 
{
	SIMPLE = 0,
	EXPERT
	
}EViewMode;

typedef enum
{
	AUTOGENERATE = 0,
	AUTOSAVE
}EProjectSettings;
typedef enum
{
	NORMAL = 0,
	MULTIPLEXED,
	CHAINED
}EStationType;
/************************************************************************************************
* Function Declarations
****************************************************************************************************/
DllExport ocfmRetCode ImportXML(char* fileName, int NodeID, ENodeType NodeType);
DllExport ocfmRetCode ReImportXML(char* fileName, int NodeID, ENodeType NodeType);

//DllExport ocfmRetCode ImportObjDictXML(char* fileName);
DllExport ocfmRetCode GenerateXAP(char* fileName);
DllExport ocfmRetCode GenerateCDC(char* CDCLocation);
//void GenerateMNOBD();

DllExport ocfmRetCode CreateNode(int NodeID, ENodeType NodeType, char* NodeName);
DllExport ocfmRetCode parseFile(char* filename, int NodeID, ENodeType  NodeType);
DllExport ocfmRetCode DeleteNode(int NodeID, ENodeType NodeType);
DllExport ocfmRetCode DeleteNodeObjDict(int NodeID, ENodeType NodeType);

DllExport ocfmRetCode DeleteIndex(int NodeID, ENodeType NodeType, char* IndexID);
DllExport ocfmRetCode DeleteSubIndex(int NodeID, ENodeType NodeType, char* IndexID, char* SubIndexID);
DllExport ocfmRetCode AddIndex(int NodeID, ENodeType NodeType, char* IndexID);
DllExport ocfmRetCode AddSubIndex(int NodeID, ENodeType NodeType, char* IndexID, char* SubIndexID);
DllExport ocfmRetCode SetIndexAttributes(int NodeID, ENodeType NodeType, char* IndexID, char* IndexValue, char* IndexName, EFlag flagIfInCdc);
//DllExport ocfmRetCode SetIndexAttributes(int NodeID, ENodeType NodeType, char* IndexID, EAttributeType AttributeType, char* AttributeValue);
DllExport ocfmRetCode SetSubIndexAttributes(int NodeID, ENodeType NodeType, char* IndexID, char* SubIndexID, char* IndexValue, char* IndexName, EFlag flagIfInCdc);

//$S Test API. Should be deleted in the release code
DllExport void DisplayNodeTree(void);

DllExport ocfmRetCode IfNodeExists(int NodeID, ENodeType NodeType, int* NodePos, bool& ExistfFlag);
DllExport ocfmRetCode IfIndexExists(int NodeID, ENodeType NodeType, char* IndexID, int* IndexPos);
DllExport ocfmRetCode IfSubIndexExists(int NodeID, ENodeType NodeType, char* IndexID, char* SubIndexID, int* SubIndexPos, int* IndexPos);

DllExport ocfmRetCode GetIndexAttributes(int NodeID, ENodeType NodeType, char* IndexID, EAttributeType AttributeType, char* Out_AttributeValue);
DllExport ocfmRetCode GetSubIndexAttributes(int NodeID, ENodeType NodeType, char* IndexID, char* SubIndexID, EAttributeType AttributeType, char* Out_AttributeValue);
DllExport ocfmRetCode GetNodeCount(int MNID, int* Out_NodeCount);
DllExport ocfmRetCode GetIndexCount(int NodeID, ENodeType NodeType, int* Out_IndexCount);
DllExport ocfmRetCode GetSubIndexCount(int NodeID, ENodeType NodeType, char* IndexID, int* Out_SubIndexCount);

DllExport ocfmRetCode GetNodeAttributesbyNodePos(int NodePos, int* Out_NodeID, char* Out_NodeName, EStationType *Out_eStationType);
DllExport ocfmRetCode GetIndexIDbyIndexPos(int NodeID, ENodeType NodeType, int IndexPos, char* Out_IndexID);
DllExport ocfmRetCode GetSubIndexIDbySubIndexPos(int NodeID, ENodeType NodeType, char* IndexID, int SubIndexPos, char* Out_SubIndexID);

DllExport ocfmRetCode GetIndexIDbyPositions(int NodePos, int IndexPos, char* Out_IndexID);
DllExport ocfmRetCode GetSubIndexIDbyPositions(int NodePos, int IndexPos, int SubIndexPos, char* Out_SubIndexID);

//DllExport ocfmRetCode GetSubIndexAttributesbyPositions(int 			NodePos, int 			IndexPos,	int				SubIndexPos,EAttributeType 	AttributeType, 	char* 			Out_AttributeValue);

DllExport ocfmRetCode GetIndexAttributesbyPositions(int NodePos, int IndexPos, EAttributeType AttributeType, char* Out_AttributeValue);
DllExport ocfmRetCode GetSubIndexAttributesbyPositions(int NodePos, int IndexPos, int SubIndexPos, EAttributeType AttributeType, char* Out_AttributeValue);
DllExport void LoadObjectDictionary(char* fileName);

DllExport ocfmRetCode SaveNode(const char* fileName, int NodeID, ENodeType NodeType);
DllExport ocfmRetCode SaveProject(char* ProjectPath, char* ProjectName);
DllExport ocfmRetCode OpenProject(char* PjtPath, char* projectXmlFileName);
DllExport ocfmRetCode FreeProjectMemory();
DllExport ocfmRetCode GetProjectSettings(EAutoGenerate* autoGen, EAutoSave* autoSave, EViewMode* viewMode);
DllExport ocfmRetCode SetProjectSettings(EAutoGenerate autoGen, EAutoSave autoSave, EViewMode viewMode);

//DllExport ocfmRetCode GetSubIndexCount(char* IndexID, int* IndexCount);
DllExport ocfmRetCode GenerateMNOBD();
DllExport ocfmRetCode SetAllIndexAttributes(int NodeID, ENodeType NodeType, 
											char* IndexID, char* ActualValue,
											char* IndexName, char* Access, char* dataTypeValue,
											char* pdoMappingVal, char* defaultValue, char* highLimit,
											char* lowLimit, char* objType, EFlag flagIfIncludedInCdc);
DllExport ocfmRetCode SetAllSubIndexAttributes(int NodeID, ENodeType NodeType, 
												char* IndexID, char* SubIndexID, char* ActualValue,
												char* IndexName, char* Access, char* dataTypeValue,
												char* pdoMappingVal, char* defaultValue, char* highLimit,
												char* lowLimit, char* objType, EFlag flagIfIncludedInCdc);
DllExport ocfmRetCode GetFeatureValue(INT32 iNodeId, ENodeType eNodeType, EFeatureType eFeatureType, char* FeatureName, char* Out_FeatureValue);
DllExport ocfmRetCode UpdateNodeParams(INT32 iCurrNodeId, INT32 iNewNodeID, ENodeType eNodeType, char* NodeName, EStationType eStationType, char* ForcedCycle); 
DllExport ocfmRetCode GetNodeDataTypes(INT32 iNodeId, ENodeType eNodeType, char* Out_DataTypes);

#pragma warning( disable: 4251 )
#endif // declarations_h
