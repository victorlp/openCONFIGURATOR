/*//////////////////////////////////////////////////////////////////////////////////////////////
//
//  $Source: $
//
// NAME:  Utility.cpp
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
***********************************************************************************************/
/*
//  REVISION HISTORY:
// $Log:      $
/////////////////////////////////////////////////////////////////////////////////////////////*/

/****************************************************************************************************
* Includes
****************************************************************************************************/
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include "../Include/Declarations.h"
#include "../Include/Internal.h"
#include "../Include/ProcessImage.h"


using namespace std;

/****************************************************************************************************
* FUNCTION DEFINITIONS
****************************************************************************************************/

/****************************************************************************************************
* Function Name: ConvertToUpper
* Description:
* Return value: char*
****************************************************************************************************/
char* ConvertToUpper(char* pbStrBuffer)
{
	UINT32 uiLoopCount;

	for(uiLoopCount = 0; pbStrBuffer[uiLoopCount]; ++uiLoopCount)
	{
		pbStrBuffer[uiLoopCount] = toupper(pbStrBuffer[uiLoopCount]);
	}
	return pbStrBuffer;
	//int t;
	//int len = strlen(str);
	//char* str1;
	//str1 = new char[len];
	//for(t=0; t<strlen(str); ++t)
	//{
	//	str1[t] = toupper(str[t]);
	//}		
	//str1[t] = '\0';
	//return str1;
}
	
/****************************************************************************************************
* Function Name: StringToUpper
* Description:
* Return value: char*
****************************************************************************************************/
char* StringToUpper(char* pbStrBuffer)
{
	UINT32 uiLoopCount;
	UINT32 uiStringLen = strlen(pbStrBuffer);
	
	char* pbStrUpp = new char[uiStringLen + ALLOC_BUFFER];

	for(uiLoopCount = 0; uiLoopCount < uiStringLen; ++uiLoopCount)
	{
		pbStrUpp[uiLoopCount] = toupper(pbStrBuffer[uiLoopCount]);
	}
	
	pbStrUpp[uiLoopCount] = '\0';
	return pbStrUpp;
}
	
/****************************************************************************************************
* Function Name: reverse
* Description:
* Return value: char*
****************************************************************************************************/
char* reverse(char* pbStrBuffer)
{
    char* pbCharLeft  = pbStrBuffer;
    char* pbCharRight = pbCharLeft + strlen(pbStrBuffer) - 1;
    char  cTemp;
	
    while (pbCharLeft < pbCharRight) 
	{
        cTemp			= *pbCharLeft;
        *pbCharLeft++  	= *pbCharRight;
        *pbCharRight-- 	= cTemp;
    }
    return pbStrBuffer;
}

/****************************************************************************************************
* Function Name: padLeft
* Description:
* Return value: char*
****************************************************************************************************/
char* padLeft(char* pbStrBuffer, char cPadChar, int iPadLength)
{
	char abTempBuff[50];
	int iLoopCount = 0;
	int iStringLen = strlen(pbStrBuffer);
	
	if (iStringLen < iPadLength)
	{
		//temp = new char[iPadLength-iStringLen+1];		
		pbStrBuffer = reverse(pbStrBuffer);
		while(iLoopCount < (iPadLength-iStringLen))
		{
			abTempBuff[iLoopCount] = cPadChar;
			iLoopCount++;
		}
		
		abTempBuff[iLoopCount] = '\0';
		strcat(pbStrBuffer, abTempBuff);				
		pbStrBuffer = reverse(pbStrBuffer);
	}
	return pbStrBuffer;
}

/****************************************************************************************************
* Function Name: subString
* Description:
* Return value: char*
****************************************************************************************************/
char* subString(char* pbStrBuffer, int iStartPos, int iStringLen)
{
	char* pbSubStrBuffer;

	pbSubStrBuffer = new char[iStringLen + 1];
	strncpy(pbSubStrBuffer, (const char*)(pbStrBuffer + iStartPos), iStringLen);
	pbSubStrBuffer[iStringLen]='\0';
	return pbSubStrBuffer;
}

/****************************************************************************************************
* Function Name: utoa
* Description:
* Return value: char *
****************************************************************************************************/
char *utoa(unsigned uValue, char *pbDigits, int iBaseValue)
{
    char *pbAlphNum, *pbTempBuffer;
	
    pbAlphNum = (char *) "0123456789abcdefghijklmnopqrstuvwxyz"; 
    if (iBaseValue == 0)
	{
        iBaseValue = 10;
	}
    if (pbDigits == NULL || iBaseValue < 2 || iBaseValue > 36)
	{
        return NULL;
	}
    if (uValue < (unsigned) iBaseValue) 
	{
        pbDigits[0] = pbAlphNum[uValue];
        pbDigits[1] = '\0';
    }
	else 
	{
        for (pbTempBuffer = utoa(uValue / ((unsigned)iBaseValue), pbDigits, iBaseValue);
             *pbTempBuffer;
             pbTempBuffer++);
        utoa( uValue % ((unsigned)iBaseValue), pbTempBuffer, iBaseValue);
    }	
    return pbDigits;
	
}

/****************************************************************************************************
* Function Name: _IntToAscii
* Description:
* Return value: char *
****************************************************************************************************/
char *_IntToAscii(long iValue, char *pbDigits, int iBaseValue)
{
    char *pbDigit;
    unsigned long uiValue; /* assume unsigned is big enough to hold all the
                 * unsigned values -x could possibly be -- don't
                 * know how well this assumption holds on the
                 * DeathStation 9000, so beware of nasal demons
                 */

    pbDigit = pbDigits;
    if (iBaseValue == 0)
	{
        iBaseValue = 10;
	}
    if (pbDigits == NULL || iBaseValue < 2 || iBaseValue > 36)
	{
        return NULL;
	}
    if (iValue < 0) {
        //*pbDigit++ = '-';
        uiValue = -((unsigned)iValue);
		uiValue = ~uiValue + 1; // two's complement
		
    }
	else
	{
        uiValue = iValue;
	}
	
    utoa(uiValue, pbDigit, iBaseValue);
	
    return pbDigits;
}

/**************************************************************************************************
* Function Name: _IntToAscii
* Description: Perform C++ style "_IntToAscii"
* Return value: char*
***************************************************************************************************/

//char* _IntToAscii( int value, char* result, int base ) 
//{
//	// check that the base if valid
//
//	if (base < 2 || base > 16) 
//	{ 
//		*result = 0; return result; 
//	}
//	char* out = result;
//	int quotient = value;
//	do 
//	{
//		*out = "0123456789abcdef"[ abs( quotient % base ) ];
//		++out;
//		quotient /= base;
//	} while ( quotient );
//
//	// Only apply negative sign for base 10
//	
//	if ( value < 0 && base == 10) *out++ = '-';
//	result = reverse(out);
//	printf("\nResult%s",result);
//	*out = 0;
//	return result;
//}

/**************************************************************************************************
* Function Name: hex2int
* Description: 
* Return value: unsigned long
***************************************************************************************************/
unsigned long hex2int(char *pbStrBuffer)
{
    UINT32 uiLoopCount;
    UINT32 uiValue = 0;
    UINT32 uiStrLen = strlen(pbStrBuffer);
				
    for(uiLoopCount = 0; uiLoopCount < uiStrLen; uiLoopCount++)
    {
		if(IsAscii(pbStrBuffer[uiLoopCount]))
		{
			pbStrBuffer[uiLoopCount] = toupper(pbStrBuffer[uiLoopCount]);	
		}
		if(pbStrBuffer[uiLoopCount] <= 57)
		{
			uiValue += (pbStrBuffer[uiLoopCount] - 48)*(1<<(4*(uiStrLen-1-uiLoopCount)));
		}
		else
		{
			uiValue += (pbStrBuffer[uiLoopCount] - 55)*(1<<(4*(uiStrLen-1-uiLoopCount)));
		}
    }
    return uiValue;
}

/**************************************************************************************************
* Function Name: IsAscii
* Description: 
* Return value: bool
***************************************************************************************************/
bool IsAscii(char cArg)
{
	UINT32 uiCount;
	
	uiCount = cArg;
	
	if (uiCount >= 48 && uiCount <= 57)
	{
		return false;
	}
	else 
	{
		return true;
	}
}

/**************************************************************************************************
* Function Name: CheckIfNotPDO
* Description:
* Return value: bool
***************************************************************************************************/
bool CheckIfNotPDO(char* pbIndex)
{
	if(strncmp(pbIndex, "14xx",2) == 0 || strncmp(pbIndex, "16xx",2) == 0 || strncmp(pbIndex, "18xx",2) == 0 ||
				strncmp(pbIndex,"1Axx",2) == 0)
	{
		return false;
	}
	else 
	{
		return true;
	}
}
	
/**************************************************************************************************
* Function Name: CheckIfManufactureSpecificObject
* Description: 
* Return value: bool
***************************************************************************************************/
bool CheckIfManufactureSpecificObject(char* pbIndex)
{
	UINT32 uiDeviceIndex;
	
	uiDeviceIndex = hex2int( (char *) "2000");
	if(hex2int(pbIndex) >= uiDeviceIndex )
	{
		return true;
	}
	else
	{
		return false;
	}
}

/**************************************************************************************************
* Function Name: CheckIfMappingPDO
* Description: 
* Return value: bool
***************************************************************************************************/
bool CheckIfMappingPDO(char* pbIndex)
{
	if(strncmp(pbIndex, "1A", 2) == 0 || strncmp(pbIndex, "16", 2) == 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}

/**************************************************************************************************
* Function Name: CheckAllowedCNIndexes
* Description: 
* Return value: bool
***************************************************************************************************/
bool CheckAllowedCNIndexes(char* pbIndexValue)
{
	if((CheckIfNotPDO((char*)pbIndexValue) == false)  || CheckIfManufactureSpecificObject((char*)pbIndexValue) ||
	strcmp(pbIndexValue,"1F98") == 0 
	||	strcmp(pbIndexValue,"1020") == 0
	||	strcmp(pbIndexValue,"1006") == 0)
	{
		return true;
	}
	else
	{
		return false;							
	}
}

/**************************************************************************************************
* Function Name: ConvertToHexformat
* Description: 
* Return value: char*
***************************************************************************************************/
char* ConvertToHexformat(char* pbHexValue, int iPadLength, bool bDoPadding)
{
	char* pbHexTemp;

	pbHexTemp = (char*)malloc(iPadLength + 3);
	strcpy(pbHexTemp, "0x");
	
	if(bDoPadding)
	{
		pbHexValue = padLeft(pbHexValue, '0', iPadLength);
	}
	strcat(pbHexTemp, pbHexValue);
	
	return pbHexTemp;
}

/**************************************************************************************************
* Function Name: CheckIfHex
* Description: 
* Return value: bool
***************************************************************************************************/
bool CheckIfHex(char* pbValue)
{
	if(strchr(pbValue,'x') == NULL)
	{
		if(strchr(pbValue,'X') == NULL)
		{
			return false;
		}
		else
		{
			return true;
		}
	}
	else
	{
		return true;
	}
		
}
//char* ReplaceChars(char ch, int num, char* str)
//{
//		int t;
//		char st[2];
//		for(t=0; str[t] ; ++t)
//		{
//			for(int i =0; i<num ;i++)
//			{
//				strcpy(st[i], 
//			}
//			if(str[t] == ch)
//			
//		}
//		return str;
//}

/**************************************************************************************************
* Function Name: getConfigDate
* Description: 
* Return value: int
***************************************************************************************************/
int getConfigDate()
{
  // to have the total number of days Since 1984
  INT32 iDays = 0;
  // to have the total number of years Since 1984  
  INT32 iYearSince = 0;
  // to have the current Year
  INT32 iCurrentYear = 0;
  time_t rawtime;
  struct tm *pstTimeInfo;

  time ( &rawtime );
  pstTimeInfo = localtime ( &rawtime );

  iYearSince = ((pstTimeInfo->tm_year)- 84);
  iCurrentYear = ((pstTimeInfo->tm_year) + 1900);
  
  // Loops thru all the years except the curent year
  for(INT32 iLoopCount = 0; iLoopCount < iYearSince; iLoopCount++)
  {
  	INT32 iTmpYear = 0;
  	INT32 iTmpDays = 0;
  	iTmpYear = (1984 + iLoopCount);
  	if (iTmpYear % 4 == 0 && !(iTmpYear % 100 == 0 && iTmpYear % 400 != 0))
	{
  		iTmpDays = 366;
	}
  	else
	{
  		iTmpDays = 365;
	}
  	iDays += iTmpDays;
  }
  // To calculate number of days since Jan 1 of current year
  iDays += pstTimeInfo->tm_yday;
  return iDays;
}

/**************************************************************************************************
* Function Name: getConfigTime
* Description: 
* Return value: int
***************************************************************************************************/
int getConfigTime()
{
  time_t rawtime;
  struct tm *pstTimeInfo;
  INT32 iMSeconds = 0;

  time ( &rawtime );
  pstTimeInfo = localtime ( &rawtime );
  
  iMSeconds += (pstTimeInfo->tm_hour) * 3600000;
  iMSeconds += (pstTimeInfo->tm_min) * 60000;
  iMSeconds += (pstTimeInfo->tm_sec) * 1000;
  
  return iMSeconds;
}

/**************************************************************************************************
* Function Name: toupper
* Description: 
* Return value: CHAR
***************************************************************************************************/
//CHAR toupper (CHAR cArg)
//{
//	if (cArg >= 97 && cArg <= 122)
//	{
//		cArg = cArg - 32;
//	}
//	return cArg;
//}

/**************************************************************************************************
* Function Name: tobin
* Description: 
* Return value: CHAR
***************************************************************************************************/
//CHAR tobin(CHAR cArg)
//{
//	if(cArg >= 65 && cArg <= 70)
//	{
//		cArg = cArg - 55;
//	}
//	else if ( cArg >=48 && cArg <= 57)
//	{
//		cArg = cArg - 48;
//	}
//	return cArg;
//}

/**************************************************************************************************
* Function Name: reversedata
* Description: 
* Return value: int
***************************************************************************************************/
int reversedata(UINT8 *pbArg1, UINT8 *pbArg2, UINT32 uiSize)
{
	UINT32 uiLoopCount;
	
	if (pbArg1 == NULL || pbArg2 == NULL)
	{
		return -1;
	}	
	
	for (uiLoopCount = 0; uiLoopCount < uiSize; uiLoopCount++)
	{
		*(pbArg1 + uiLoopCount) = *(pbArg2 + uiSize - 1 - uiLoopCount);
	}
		
	return 0;
}
bool CheckAllowedDTForMapping(char* dtName)
{
	if(strcmp(StringToUpper(dtName), "INTEGER8") == 0 ||
				strcmp(StringToUpper(dtName), "INTEGER16") == 0 ||
				strcmp(StringToUpper(dtName), "INTEGER32") == 0 ||							
				strcmp(StringToUpper(dtName), "UNSIGNED8") == 0 ||
				strcmp(StringToUpper(dtName), "UNSIGNED16") == 0 ||
				strcmp(StringToUpper(dtName), "UNSIGNED32") == 0)
				
				
				return true;
	else
		return false;
}
UINT32 getLastAvailableCycleNumber()
{
	char* actValue = new char[20];
	ocfmRetCode Ret;
	Ret = GetSubIndexAttributes(240, MN, "1F98", "07", ACTUALVALUE, actValue);
	if(Ret.code == OCFM_ERR_SUCCESS)
	{	

		//printf(" \nactvalue %s",actValue);
		
		//printf("\ncycle %d", uiCycleNumber);
		int iCycleValue;
		if (strncmp(actValue,"0x",2) == 0 || strncmp(actValue,"0X",2) == 0)
			iCycleValue  = hex2int(subString(actValue, 2,strlen(actValue) -2 ));
		else
			iCycleValue  = hex2int(actValue);

		if(iCycleValue > uiCycleNumber+1)
		{
			uiCycleNumber += 1;
		}
		else
		{			
			uiCycleNumber += 1;
			while(iCycleValue < uiCycleNumber)
			{
				uiCycleNumber = uiCycleNumber - iCycleValue;
			}
			
		}
		
	}
	delete[] actValue;
	return uiCycleNumber;
}

void checkAndCorrectName(char* checkName)
{
    if(checkName == NULL)
        return;

	UINT32 uiStrLen = strlen(checkName);
    for(UINT32 uiLoopCount = 0; uiLoopCount < uiStrLen; uiLoopCount++)
    {
		if( (checkName[uiLoopCount] >= 48 && checkName[uiLoopCount] <= 57) || (checkName[uiLoopCount] >= 65 && checkName[uiLoopCount] <= 90) || (checkName[uiLoopCount] >= 97 && checkName[uiLoopCount] <= 122) || checkName[uiLoopCount] == 95)
		{
		}
		else
		{
			checkName[uiLoopCount] = '_';
		}
    }
}