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
#include "../Include/ProjectSettings.h"
#include "../Include/Internal.h"
#include <iostream>
using namespace std;

CPjtSettings::CPjtSettings(void)
	{		
		#if defined DEBUG
			cout << "\nPjtSettings Objects Initialized!\n" << endl;	
		#endif
		m_saveMode = DISCARD_AS;
		m_generateMode = NO_AG;
		m_IP_openPOWERLINK = NULL;
		TestVar = 0;
			#if defined DEBUG
		//cout << "\n Returning m_saveMode:-" << m_saveMode <<endl;	
	#endif
		//getGenerateAttr();
	}

CPjtSettings::~CPjtSettings(void)
	{		
		instanceFlag = false;
		#if defined DEBUG
			cout << "\n\n\n\nPjtSettings Object Deleted!\n" << endl;	
		#endif
	}

bool CPjtSettings::instanceFlag=false;
CPjtSettings* CPjtSettings::objPjtSettings = NULL;
CPjtSettings* CPjtSettings::getPjtSettingsPtr()
{
	if(!instanceFlag)
	{
		objPjtSettings = new CPjtSettings();
		instanceFlag=true;
		#if defined DEBUG
			cout << "\n\n\n\nNew PjtSettings Object Created!\n" << endl;	
		#endif
	}
	return objPjtSettings;	
}

EAutoSave CPjtSettings::getSaveAttr()
{
	return m_saveMode;
}

void CPjtSettings::setSaveAttr(EAutoSave AutoSaveMode)
{
	m_saveMode = AutoSaveMode;
}

EAutoGenerate CPjtSettings::getGenerateAttr()
{
	m_generateMode = YES_AG;
	m_saveMode = YES_AS;
	#if defined DEBUG
		cout << "\n Returning m_saveMode:-" << m_saveMode <<endl;	
	#endif
	cout << "\n$$$" << endl;
	return m_generateMode;
		
}

void CPjtSettings::setGenerateAttr(EAutoGenerate AutoGenerateMode)
{
	m_generateMode = AutoGenerateMode;
}

void CPjtSettings::setPOWERLINK_IP(char* IPAddr)
{	
	m_IP_openPOWERLINK = new char[strlen(IPAddr) + ALLOC_BUFFER];
	strcpy((char*)m_IP_openPOWERLINK, IPAddr);
}

const char* CPjtSettings::getPOWERLINK_IP()
{
	if(m_IP_openPOWERLINK != NULL)
		return m_IP_openPOWERLINK;
	else
		return NULL;
}

void CPjtSettings::setTestVar(int temp)
{
	TestVar = temp;
}

int CPjtSettings::getTestVar()
{
	return TestVar;
}
