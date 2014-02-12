/**
 *****************************************************************************
 \file		Utility.cpp

 \brief		This file contains the definitions that are used as utility functions to support operations such as string manipulation, number to string conversions, etc
 *****************************************************************************
 */

/*
 � Kalycito Infotech Private Limited

 License:

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions
 are met:

 1. Redistributions of source code must retain the above copyright
 notice, this list of conditions and the following disclaimer.

 2. Redistributions in binary form must reproduce the above copyright
 notice, this list of conditions and the following disclaimer in the
 documentation and/or other materials provided with the distribution.

 3. Neither the name of Kalycito Infotech Private Limited nor the names of 
 its contributors may be used to endorse or promote products derived
 from this software without prior written permission. For written
 permission, please contact info@kalycito.com.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 COPYRIGHT HOLDERS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 POSSIBILITY OF SUCH DAMAGE.

 Severability Clause:

 If a provision of this License is or becomes illegal, invalid or
 unenforceable in any jurisdiction, that shall not affect:
 1. the validity or enforceability in that jurisdiction of any other
 provision of this License; or
 2. the validity or enforceability in other jurisdictions of that or
 any other provision of this License.

 ****************************************************************************/

/****************************************************************************/
/* Includes */

#include <iostream>
#include <stdlib.h>
#include <time.h>
#include "../Include/Declarations.h"
#include "../Include/Internal.h"
#include "../Include/ProcessImage.h"

using namespace std;

//==========================================================================//
// 				F U N C T I O N  D E F I N I T I O N S  					//
//==========================================================================//

char* ConvertToUpper(char* str)
{
	if (NULL == str)
	{
		ocfmException objException;
		objException.OCFMException(OCFM_ERR_INVALID_PARAMETER);
		cout << "INVALID_PARAMETER:" << __FUNCTION__ << __LINE__ << endl;
		throw objException;
	}
	UINT32 loopCount = 0;

	for (loopCount = 0; str[loopCount]; ++loopCount)
	{
		str[loopCount] = toupper(str[loopCount]);
	}
	return str;
}

//TODO: To add a parameter for destination to avoid new delete memory leak.
char* StringToUpper(char* srcStr)
{
	if (NULL == srcStr)
	{
		ocfmException objException;
		objException.OCFMException(OCFM_ERR_INVALID_PARAMETER);
		cout << "INVALID_PARAMETER:" << __FUNCTION__ << __LINE__ << endl;
		throw objException;
	}
	UINT32 uiLoopCount;
	UINT32 uiStringLen = strlen(srcStr);

	char* strUpp = new char[uiStringLen + STR_ALLOC_BUFFER];

	for (uiLoopCount = 0; uiLoopCount < uiStringLen; ++uiLoopCount)
	{
		strUpp[uiLoopCount] = toupper(srcStr[uiLoopCount]);
	}

	strUpp[uiLoopCount] = '\0';
	return strUpp;
}

char* Reverse(char* str)
{
	if (NULL == str)
	{
		ocfmException objException;
		objException.OCFMException(OCFM_ERR_INVALID_PARAMETER);
		cout << "INVALID_PARAMETER:" << __FUNCTION__ << __LINE__ << endl;
		throw objException;
	}
	char* charLeft = str;
	char* charRight = charLeft + strlen(str) - 1;
	char cTemp;

	while (charLeft < charRight)
	{
		cTemp = *charLeft;
		*charLeft++ = *charRight;
		*charRight-- = cTemp;
	}
	return str;
}

char* PadLeft(char* str, char padChar, INT32 padLength)
{
	if (NULL == str)
	{
		ocfmException objException;
		objException.OCFMException(OCFM_ERR_INVALID_PARAMETER);
		cout << "INVALID_PARAMETER:" << __FUNCTION__ << __LINE__ << endl;
		throw objException;
	}

	INT32 iStringLen = strlen(str);

	if (iStringLen < padLength)
	{
		char abTempBuff[50];
		INT32 iLoopCount = 0;
		str = Reverse(str);
		while (iLoopCount < (padLength - iStringLen))
		{
			abTempBuff[iLoopCount] = padChar;
			iLoopCount++;
		}

		abTempBuff[iLoopCount] = '\0';
		strcat(str, abTempBuff);
		str = Reverse(str);
	}
	return str;
}

char* SubString(char* destStr, const char* srcStr, UINT32 startPos, UINT32 len)
{
	if ((NULL != srcStr) && (NULL != destStr))
	{
		if ((startPos + len) > strlen(srcStr))
		{
			//cout<<"Error: "<< strlen(destStr) << " " << __FUNCTION__ <<" wrong inputs. startPos:"<<startPos<<" Len:"<<len <<" Total available:"<<strlen(srcStr)<<endl;
		}

		strncpy(destStr, (const char*) (srcStr + startPos), len);
		destStr[len] = '\0';
		#if defined DEBUG
			cout << "src:" << srcStr << " DestStr: "<< destStr << endl;
		#endif
	}
	else
	{
		cout<<"Error: SubString Returning NULL"<<endl;
	}
	return destStr;
}

char* UnsignedToAlphaNumeric(unsigned srcValue, char *destStr, INT32 baseValue)
{
	char *alphNum = NULL;

	alphNum = (char *) "0123456789abcdefghijklmnopqrstuvwxyz";
	if (0 == baseValue)
	{
		baseValue = 10;
	}
	if ((NULL == destStr) || (2 > baseValue) || (36 < baseValue))
	{
		return NULL;
	}
	if (srcValue < (unsigned) baseValue)
	{
		destStr[0] = alphNum[srcValue];
		destStr[1] = '\0';
	}
	else
	{
		char *tempBuffer = NULL;
		for (tempBuffer = UnsignedToAlphaNumeric(
				srcValue / ((unsigned) baseValue), destStr, baseValue);
				*tempBuffer; tempBuffer++)
			;
		{
			UnsignedToAlphaNumeric(srcValue % ((unsigned) baseValue),
					tempBuffer, baseValue);
		}
	}
	return destStr;

}

char* IntToAscii(LONG srcValue, char *destStr, INT32 baseValue)
{
	char *tempStr = NULL;
	ULONG uiValue;

	tempStr = destStr;
	if (0 == baseValue)
	{
		baseValue = 10;
	}
	if ((NULL == destStr) || (2 > baseValue) || (36 < baseValue))
	{
		return NULL;
	}
	if (srcValue < 0)
	{
		uiValue = -(srcValue);
		//Two's complement of that number
		uiValue = ~uiValue + 1;
	}
	else
	{
		uiValue = srcValue;
	}

	UnsignedToAlphaNumeric(uiValue, tempStr, baseValue);

	return destStr;
}

ULONG HexToInt(char *hexStr)
{
	UINT32 loopCount = 0;
	ULONG retValue = 0;
	UINT32 hexStrLen = strlen(hexStr);

	for (loopCount = 0; loopCount < hexStrLen; loopCount++)
	{
		if (IsAscii(hexStr[loopCount]))
		{
			hexStr[loopCount] = toupper(hexStr[loopCount]);
		}
		if (hexStr[loopCount] <= 57)
		{
			retValue += (hexStr[loopCount] - 48)
					* (1 << (4 * ((hexStrLen - 1) - loopCount)));
		}
		else
		{
			retValue += (hexStr[loopCount] - 55)
					* (1 << (4 * ((hexStrLen - 1) - loopCount)));
		}
	}
	return retValue;
}

bool IsAscii(char argVar)
{
	UINT32 uiCount;

	uiCount = argVar;

	if (48 <= uiCount && 57 >= uiCount)
	{
		return false;
	}
	else
	{
		return true;
	}
}

bool CheckIfNotPDO(char* indexId)
{
	if ((0 == strncmp(indexId, "14xx", 2)) || (0 == strncmp(indexId, "16xx", 2))
			|| (0 == strncmp(indexId, "18xx", 2))
			|| (0 == strncmp(indexId, "1Axx", 2))
			|| (0 == strncmp(indexId, "1axx", 2)))
	{
		return false;
	}
	else
	{
		return true;
	}
}

bool CheckIfManufactureSpecificObject(char* indexId)
{
	UINT32 manufactureIndexid;

	manufactureIndexid = HexToInt((char *) "2000");
	if (HexToInt(indexId) >= manufactureIndexid)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool CheckIfMappingPDO(char* indexId)
{
	if ((0 == strncmp(indexId, "1A", 2)) || (0 == strncmp(indexId, "16", 2)))
	{
		return true;
	}
	else
	{
		return false;
	}
}

//TODO: Function is not used.
bool CheckAllowedCNIndexes(char* indexId)
{
	if ((false == CheckIfNotPDO((char*) indexId))
			|| CheckIfManufactureSpecificObject((char*) indexId)
			|| (0 == strcmp(indexId, "1F98")) || (0 == strcmp(indexId, "1020"))
			|| (0 == strcmp(indexId, "1F9B")) || (0 == strcmp(indexId, "1F81"))
			|| (0 == strcmp(indexId, "1006")))
	{
		return true;
	}
	else
	{
		return false;
	}
}

//TODO: function is not used.
bool CheckBlockedMNIndexes(char* indexId)
{
	if (0 == strcmp(indexId, "1F81"))
	{
		return true;
	}
	else
	{
		return false;
	}
}

char* ConvertStringToHex(char* srcStr)
{
	INT32 srcStrLen = 0;

	if (NULL == srcStr)
	{
		cout << "INVALID_PARAMETER:" << __FUNCTION__ << __LINE__ << endl;
		return NULL;
	}
	srcStrLen = strlen(srcStr);
	char* tempSrcStr = new char[srcStrLen + 1];
	char* tempHexStr = new char[(srcStrLen * 2) + 1];
	if ((NULL == tempSrcStr) || (NULL == tempHexStr))
	{
#if defined DEBUG
		cout << "Memory allocation error" << __FUNCTION__ << endl;
#endif

		ocfmException ex;
		ex.OCFMException(OCFM_ERR_MEMORY_ALLOCATION_ERROR);
		throw ex;
	}
	strcpy(tempSrcStr, (char*) srcStr);
	strcpy(srcStr, "");
	for (INT32 iloopCntr = 0; iloopCntr < srcStrLen; iloopCntr++)
	{
		sprintf(tempHexStr, "%X", tempSrcStr[iloopCntr]);
		strcat(srcStr, (char*) tempHexStr);
	}
	delete[] tempSrcStr;
	delete[] tempHexStr;
	return srcStr;
}

char* ConvertToHexformat(char* hexValue, INT32 padLength, bool doPadding)
{
	char* hexTemp = NULL;

	hexTemp = new char[padLength + 2 + STR_ALLOC_BUFFER];

	strcpy(hexTemp, "0x");

	if (doPadding)
	{
		hexValue = PadLeft(hexValue, '0', padLength);
	}
	strcat(hexTemp, hexValue);

	return hexTemp;
}
//TODO: change parameter as const char*
bool CheckIfHex(char* srcStr)
{
	if (NULL == strchr(srcStr, 'x'))
	{
		if (NULL == strchr(srcStr, 'X'))
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

INT32 GetConfigDate()
{
//Time & date are calculated since 1984
	INT32 daysCount = 0;
	INT32 yearsSince = 0;

	time_t rawTime;
	struct tm *timeInfo;

	time(&rawTime);
	timeInfo = localtime(&rawTime);

	yearsSince = ((timeInfo->tm_year) - 84);

	//Except the curent year calculate all the values
	for (INT32 loopCount = 0; loopCount < yearsSince; loopCount++)
	{
		INT32 tempYear = 0;
		INT32 tempDays = 0;
		tempYear = (1984 + loopCount);
		if ((0 == tempYear % 4) && !(0 == tempYear % 100)
				&& (0 != tempYear % 400))
		{
			tempDays = 366;
		}
		else
		{
			tempDays = 365;
		}
		daysCount += tempDays;
	}
	//Calculate the number of days from January 1 of current year
	daysCount += timeInfo->tm_yday;
	return daysCount;
}

INT32 GetConfigTime()
{
	time_t rawtime;
	struct tm *timeInfo;
	INT32 milliSeconds = 0;

	time(&rawtime);
	timeInfo = localtime(&rawtime);

	milliSeconds += (timeInfo->tm_hour) * 3600000;
	milliSeconds += (timeInfo->tm_min) * 60000;
	milliSeconds += (timeInfo->tm_sec) * 1000;

	return milliSeconds;
}

//TODO: function is not used.
INT32 ReverseData(UINT8 *destData, UINT8 *srcData, UINT32 srcLen)
{
	UINT32 uiLoopCount;

	if (NULL == destData || NULL == srcData)
	{
		return -1;
	}

	for (uiLoopCount = 0; uiLoopCount < srcLen; uiLoopCount++)
	{
		*(destData + uiLoopCount) = *(srcData + (srcLen - 1) - uiLoopCount);
	}

	return 0;
}

bool CheckAllowedDTForMapping(char* dataTypeName)
{
	bool retVal = false;
	if( NULL != dataTypeName)
	{
		char *dtNameUpper = new char[strlen(dataTypeName) + STR_ALLOC_BUFFER];
		strcpy(dtNameUpper, dataTypeName);
		dtNameUpper = ConvertToUpper(dtNameUpper);

		if (0 == strcmp(dtNameUpper, "INTEGER8")
			|| 0 == strcmp(dtNameUpper, "INTEGER16")
			|| 0 == strcmp(dtNameUpper, "INTEGER32")
			|| 0 == strcmp(dtNameUpper, "INTEGER64")
			|| 0 == strcmp(dtNameUpper, "UNSIGNED8")
			|| 0 == strcmp(dtNameUpper, "UNSIGNED16")
			|| 0 == strcmp(dtNameUpper, "UNSIGNED32")
			|| 0 == strcmp(dtNameUpper, "UNSIGNED64")
			|| 0 == strcmp(dtNameUpper, "DOMAIN"))
		{
			retVal = true;
		}
		else
		{
			retVal = false;
		}
		delete[] dtNameUpper;
	}
	else
	{
		retVal = false;
	}
	return retVal;
}

//TODO: Add a parameter to return the value to avoid new delete memory issues
char* GetLastAvailableCycleNumber()
{
	char* retForcedCycleValue = new char[20];
	ocfmRetCode retCode;
	INT32 indexPos;
	INT32 subIndexPos;
	retCode = IfSubIndexExists(240, MN, (char*) "1F98", (char*) "07",
			&subIndexPos, &indexPos);
	if (OCFM_ERR_SUCCESS != retCode.code)
	{
		strcpy(retForcedCycleValue, "");
		return retForcedCycleValue;
	}
	char* actValue = new char[20];
	retCode = GetSubIndexAttributes(240, MN, (char*) "1F98", (char*) "07",
			ACTUALVALUE, actValue);
	UINT32 tempCycleNumber = cycleNumberGlobal;
	if (OCFM_ERR_SUCCESS != retCode.code)
	{

		UINT32 cycleValue;
		if (0 == strncmp(actValue, "0x", 2) || 0 == strncmp(actValue, "0X", 2))
		{
			char *tempCycleVal = new char[strlen(actValue)];
			SubString(tempCycleVal, (const char*)actValue, 2, strlen(actValue) - 2);
			cycleValue = (UINT32) HexToInt(tempCycleVal);
			delete[] tempCycleVal;
		}
		else
			cycleValue = atoi(actValue);

		if (cycleValue > tempCycleNumber + 1)
		{
			tempCycleNumber += 1;
		}
		else
		{
			tempCycleNumber += 1;
			while (cycleValue < tempCycleNumber)
			{
				tempCycleNumber = tempCycleNumber - cycleValue;
			}

		}

	}
	delete[] actValue;

	UINT32 freeCycleNumber = GetFreeCycleNumber(tempCycleNumber);
	if (freeCycleNumber == tempCycleNumber)
	{
		cycleNumberGlobal = tempCycleNumber;
		retForcedCycleValue = IntToAscii(tempCycleNumber, retForcedCycleValue,
				16);
	}
	else
	{
		retForcedCycleValue = IntToAscii(freeCycleNumber, retForcedCycleValue,
				16);
	}
	return retForcedCycleValue;
}

void CheckAndCorrectName(char* srcStr)
{
	if (NULL == srcStr)
	{
		return;
	}

	UINT32 srcStrLen = strlen(srcStr);
	for (UINT32 loopCount = 0; loopCount < srcStrLen; loopCount++)
	{
		if ((48 <= srcStr[loopCount] && 57 >= srcStr[loopCount])
				|| (65 <= srcStr[loopCount] && 90 >= srcStr[loopCount])
				|| (97 <= srcStr[loopCount] && 122 >= srcStr[loopCount])
				|| 95 == srcStr[loopCount])
		{
		}
		else
		{
			srcStr[loopCount] = '_';
		}
	}
}

bool CheckIfValueZero(char* srcStr)
{
	if (NULL == srcStr || 0 == strcmp(srcStr, ""))
	{
		return false;
	}

	INT32 srcValue = 0;
	if (0 == strncmp(srcStr, "0x", 2) || 0 == strncmp(srcStr, "0X", 2))
	{
		char* tempVal = new char[strlen(srcStr)];
		SubString(tempVal, (const char*) srcStr, 2, strlen(srcStr) - 2);
		srcValue = HexToInt(tempVal);
		delete[] tempVal;
	}
	else
	{
		srcValue = atoi(srcStr);
	}

	if (0 == srcValue)
	{
		return true;
	}
	else
	{
		return false;
	}
}

//TODO: Change as const char*
INT32 GetDecimalValue(char* srcStr)
{
	if (NULL == srcStr || 0 == strcmp(srcStr, ""))
	{
		return 0;
	}

	INT32 srcValue = 0;
	if (0 == strncmp(srcStr, "0x", 2) || 0 == strncmp(srcStr, "0X", 2))
	{
		char* tempVal = new char[strlen(srcStr)];
		SubString(tempVal, (const char*) srcStr, 2, strlen(srcStr) - 2);
		srcValue = (INT32) HexToInt(tempVal);
		delete[] tempVal;
	}
	else
	{
		srcValue = atoi(srcStr);
	}

	return srcValue;
}

bool CheckAccessTypeForInclude(char* accessType)
{
	bool retInclude;
	if (NULL == accessType)
	{
		//TODO: To check with the functionality. Return true or false?
		retInclude = true;
	}
	else
	{
		char* tempAccesstype = new char[strlen(accessType) + STR_ALLOC_BUFFER];
		strcpy(tempAccesstype, accessType);
		tempAccesstype = ConvertToUpper(tempAccesstype);
		if ((0 == strcmp(tempAccesstype, "CONST"))
			|| (0 == strcmp(tempAccesstype, "RO")))
		{
			retInclude = false;
		}
		else
		{
			retInclude = true;
		}
		delete[] tempAccesstype;
	}
	return retInclude;
}

bool CheckToolVersion(char* currentToolVersion)
{
	char* tempToolVersion = ConvertToUpper(currentToolVersion);
	if ((0 == strcmp(tempToolVersion, TOOL_VERSION))
		|| (0 == strcmp(tempToolVersion, PREV_TOOL_VERSION_1))
		|| (0 == strcmp(tempToolVersion, PREV_TOOL_VERSION_2))
		|| (0 == strcmp(tempToolVersion, PREV_TOOL_VERSION_3))
		|| (0 == strcmp(tempToolVersion, PREV_TOOL_VERSION_4))
		|| (0 == strcmp(tempToolVersion, PREV_TOOL_VERSION_5))
		|| (0 == strcmp(tempToolVersion, PREV_TOOL_VERSION_6))
		|| (0 == strcmp(tempToolVersion, PREV_TOOL_VERSION_7))
		|| (0 == strcmp(tempToolVersion, LAST_TOOL_VERSION))
		|| (0 == strcmp(tempToolVersion, TOOL_INTER_REL_VERSION_1))
		|| (0 == strcmp(tempToolVersion, TOOL_INTER_REL_VERSION_2))
		|| (0 == strcmp(tempToolVersion, TOOL_INTER_REL_VERSION_3))
		|| (0 == strcmp(tempToolVersion, TOOL_INTER_REL_VERSION_4))
		|| (0 == strcmp(tempToolVersion, TOOL_INTER_REL_VERSION_5)))
	{
		return true;
	}
	else
	{
		return false;
	}
}
