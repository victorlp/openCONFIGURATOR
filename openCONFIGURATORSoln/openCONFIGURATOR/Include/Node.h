#ifndef Node_h
#define Node_h
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

#pragma once

/************************************************************************************************
* Includes
************************************************************************************************/
#include "DataTypeCollection.h"
#include "IndexCollection.h"
#include "Declarations.h"
#include "ApplicationProcess.h"
#include "ProcessImage.h"

class DllExport CNode
{
	public:
		CNode(void);
		~CNode(void);
	private:
		CDataTypeCollection *m_DataTypeCollection;
		CIndexCollection *m_IndexCollection;
		CApplicationProcess* m_ApplicationProcess;
				
		int				m_NodeId;
		ENodeType		m_NodeType;
		int				m_NodeIndex;
		bool			m_HasPdoObjects;
		char*			m_NodeName;
		/*	typedef TCollection<ProcessImage> PICollection;*/
	public:
		/*typedef _PICollection PICollection;*/
		TCollection<ProcessImage> ProcessImageCollection;
		TCollection<MNPdoVariable> MNPDOINVarCollection, MNPDOOUTVarCollection ;
	public:
		CDataTypeCollection* getDataTypeCollection();
		CIndexCollection* getIndexCollection();
		CApplicationProcess* getApplicationProcess();
		
		int getNodeId();
		void setNodeId(int NodeId);

		int getNodeIndex();
		void setNodeIndex(int NodeIndex);

		char* getNodeName();
		void setNodeName(char* NodeName);

		ENodeType getNodeType();
		void setNodeType(ENodeType NodeType);
		
		bool HasPdoObjects();
		void setFlagForPdoObjects(bool flag);

		void CreateIndexCollection();
		void CreateDataTypeCollection();
		void CreateApplicationProcess();
		
		void addProcessImage(ProcessImage processImage);
		void addMNPDOvar(MNPdoVariable variable, EPDOType pdoType);

		CIndexCollection* getPDOIndexCollection(EPDOType PDOType);
		CIndexCollection* getIndexCollectionWithoutPDO();
		CIndexCollection*getPDOIndexCollection();
		
		ProcessImage* getPIbyParaIndex(int paraIndex);
		void DeleteCollectionsForPI();
		/*void UpdatePIOffset(char* ByteOffset);*/				
		bool isNull();			
};
#endif // Node_h
