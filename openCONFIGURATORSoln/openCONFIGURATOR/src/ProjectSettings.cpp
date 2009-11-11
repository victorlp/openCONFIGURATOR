///////////////////////////////////////////////////////////////////////////////////////////////
//
//  $Source: $
//
// NAME:  ProjectSettings.cpp
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
#include "../Include/ProjectSettings.h"
#include "../Include/Internal.h"

using namespace std;

/****************************************************************************************************
* Global Variables
****************************************************************************************************/
bool CPjtSettings::instanceFlag=false;
CPjtSettings* CPjtSettings::objPjtSettings = NULL;

/****************************************************************************************************
* FUNCTION DEFINITIONS
****************************************************************************************************/

/****************************************************************************************************
* Constructor
****************************************************************************************************/
CPjtSettings::CPjtSettings(void)
{		
	#if defined DEBUG
			cout << "\nPjtSettings Objects Initialized!\n" << endl;	
	#endif
		m_saveMode 			= DISCARD_AS;
		m_generateMode 		= NO_AG;
		m_IP_openPOWERLINK 	= NULL;	
	#if defined DEBUG
		//cout << "\n Returning m_saveMode:-" << m_saveMode <<endl;	
	#endif
}

/****************************************************************************************************
* Destructor
****************************************************************************************************/
CPjtSettings::~CPjtSettings(void)
{		
	instanceFlag = false;
	#if defined DEBUG
		cout << "\n\n\n\nPjtSettings Object Deleted!\n" << endl;	
	#endif
}

/****************************************************************************************************
* Function Name: CPjtSettings::getPjtSettingsPtr
* Description:
* Return value: CPjtSettings*
****************************************************************************************************/
CPjtSettings* CPjtSettings::getPjtSettingsPtr()
{
	if(!instanceFlag)
	{
		objPjtSettings 	= 	new CPjtSettings();
		instanceFlag	=	true;
		#if defined DEBUG
			cout << "\n\n\n\nNew PjtSettings Object Created!\n" << endl;	
		#endif
	}
	return objPjtSettings;	
}

/****************************************************************************************************
* Function Name: CPjtSettings::getSaveAttr
* Description:
* Return value: EAutoSave
****************************************************************************************************/
EAutoSave CPjtSettings::getSaveAttr()
{
	return m_saveMode;
}

/****************************************************************************************************
* Function Name: CPjtSettings::setSaveAttr
* Description:
* Return value: void
****************************************************************************************************/
void CPjtSettings::setSaveAttr(EAutoSave enumAutoSaveMode)
{
	m_saveMode = enumAutoSaveMode;
}

/****************************************************************************************************
* Function Name: CPjtSettings::getGenerateAttr
* Description:
* Return value: EAutoGenerate
****************************************************************************************************/
EAutoGenerate CPjtSettings::getGenerateAttr()
{
	#if defined DEBUG
		//cout << "\n Returning m_generateMode:-" << m_generateMode <<endl;	
	#endif	
	return m_generateMode;		
}

/****************************************************************************************************
* Function Name: CPjtSettings::setGenerateAttr
* Description:
* Return value: void
****************************************************************************************************/
void CPjtSettings::setGenerateAttr(EAutoGenerate enumAutoGenerateMode)
{
	m_generateMode = enumAutoGenerateMode;
}

/****************************************************************************************************
* Function Name: CPjtSettings::setPOWERLINK_IP
* Description:
* Return value: void
****************************************************************************************************/
void CPjtSettings::setPOWERLINK_IP(char* pbIPAddr)
{	
	m_IP_openPOWERLINK = new char[strlen(pbIPAddr) + ALLOC_BUFFER];
	strcpy((char*)m_IP_openPOWERLINK, pbIPAddr);
}

/****************************************************************************************************
* Function Name: CPjtSettings::getPOWERLINK_IP
* Description:
* Return value: char*
****************************************************************************************************/
const char* CPjtSettings::getPOWERLINK_IP()
{
	if(m_IP_openPOWERLINK != NULL)
	{
		return m_IP_openPOWERLINK;
	}
	else
	{
		return NULL;
	}
}
/****************************************************************************************************
* Function Name: CPjtSettings::getViewMode
* Description:
* Return value: EViewMode
****************************************************************************************************/
EViewMode CPjtSettings::getViewMode()
{
	return m_viewMode;
}

/****************************************************************************************************
* Function Name: CPjtSettings::setViewMode
* Description:
* Return value: void
****************************************************************************************************/
void CPjtSettings::setViewMode(EViewMode enumViewMode)
{
	m_viewMode = enumViewMode;
}

