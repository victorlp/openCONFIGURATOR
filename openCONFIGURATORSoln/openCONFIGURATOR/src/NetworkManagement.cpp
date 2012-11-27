///////////////////////////////////////////////////////////////////////////////////////////////
//
//  $Source: $
//
// NAME:  NetworkManagement.cpp
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

#include "../Include/NetworkManagement.h"
#include "../Include/Internal.h"

CNetworkManagement::CNetworkManagement(void)
{	
	m_MaxPDOCount = 0;
}

CNetworkManagement::~CNetworkManagement(void)
{
}
/****************************************************************************************************
* Function Name: CApplicationProcess::addParameter
* Description:
* Return value: void
****************************************************************************************************/
void CNetworkManagement::addFeature(Feature stfeature)
{
	
	INT32 iItemPosition = FeatureCollection.Add();
	FeatureCollection[iItemPosition] = stfeature;		
}
/****************************************************************************************************
* Function Name: CNetworkManagement::getNodeID
* Description: Returns the NodeID of the Node of the NetworkManagement Object
* Return value: int
****************************************************************************************************/
INT32 CNetworkManagement::getNodeID()
{
	return m_NodeID;
}

/****************************************************************************************************
* Function Name: CNetworkManagement::setNodeID
* Description: sets the NodeID of the NetworkManagement Object
* Return value: void
****************************************************************************************************/
void CNetworkManagement::setNodeID(int NodeID)
{
	m_NodeID = NodeID;
}
/****************************************************************************************************
* Function Name: CNetworkManagement::getFeatureValue
* Description: gets the network Management feature value
* Return value: char*
****************************************************************************************************/
char* CNetworkManagement::getFeatureValue(EFeatureType featureType, char *featureName)
{
	int iLoopCount;
	char *pbRetString = NULL;
	

	for(iLoopCount =0; iLoopCount < FeatureCollection.Count() ; iLoopCount++)
	{
		Feature stFeature;		
		stFeature = FeatureCollection[iLoopCount];	
		if( stFeature.m_featureType == featureType && (!strcmp(featureName, stFeature.m_Name)))
		{		
			pbRetString = new char[strlen(stFeature.m_Value) + STR_ALLOC_BUFFER];
			strcpy(pbRetString, stFeature.m_Value);
			return pbRetString;
		}
	}
	pbRetString = new char[1 + ALLOC_BUFFER];
	strcpy((char*)pbRetString,"");
	return pbRetString;
}
/****************************************************************************************************
* Function Name: CNetworkManagement::getNumberOfFeatures
* Description: returns the Number of Features
* Return value: UINT32
****************************************************************************************************/
UINT32 CNetworkManagement::getNumberOfFeatures()
{	
	return FeatureCollection.Count();
}
/****************************************************************************************************
* Function Name: CNetworkManagement::getNumberOfFeatures
* Description: returns the Number of Features
* Return value: UINT32
****************************************************************************************************/
Feature* CNetworkManagement::getFeature(UINT32 uiCount)
{	
	return &FeatureCollection[uiCount];
}

/********************
* Function Name: CNetworkManagement::DeleteNetworkManagementCollections
* Description: 
* Return value: void
****************************************************************************************************/
void CNetworkManagement::DeleteFeatureCollections()
{
    
    if(FeatureCollection.Count() != 0)
    {
        FeatureCollection.Clear();
    }
}
/****************************************************************************************************
* Function Name: CIndexCollection::getMaxPDOCount
* Description:
* Return value: INT32
****************************************************************************************************/
INT32 CNetworkManagement::getMaxPDOCount() 
{
	return m_MaxPDOCount;
}
/****************************************************************************************************
* Function Name: CIndexCollection::calculateMaxPDOCount
* Description:
* Return value: void
****************************************************************************************************/
void CNetworkManagement::calculateMaxPDOCount() 
{	
	char* pbMaxPDOCount = new char[3];
	char* featureName = new char[20];
	m_MaxPDOCount = 0;
	if(( NULL == pbMaxPDOCount ) || ( NULL == featureName ))
	{
		#if defined DEBUG
		cout<<"Memory allocation error"<<endl;
		#endif
		
		ocfmException ex;
		ex.ocfm_Excpetion(OCFM_ERR_MEMORY_ALLOCATION_ERROR);
		throw ex;
	}
	else
	{
		strcpy(featureName, "PDOTPDOChannels");
		strcpy(pbMaxPDOCount, getFeatureValue(MN_FEATURES, featureName));
		m_MaxPDOCount = atoi((char*)pbMaxPDOCount);
		delete [] pbMaxPDOCount;
		delete [] featureName;
	}
}
