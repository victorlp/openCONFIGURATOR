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

#include "../Include/Declarations.h"
#include "../Include/Internal.h"
#include <iostream>
#include <stdlib.h>
using namespace std;
char* ConvertToUpper(char* str)
	{
		int t;

		for(t=0; str[t]; ++t)
		{
			str[t] = toupper(str[t]);
		}
		return str;
	}
char* reverse(char* str)
{
    char* left  = str;
    char* right = left + strlen(str) - 1;
    char  tmp;
    while (left < right) {
        tmp      = *left;
        *left++  = *right;
        *right-- = tmp;
    }
    return str;
}
char* padLeft(char* str, char padChar, int padLength)
{
			char* temp;
			int i=0;
			int len = strlen(str);
			
			if (len < padLength)
			{
				temp = new char[padLength-len+1];		
				str = reverse(str);
				while(i<(padLength-len))
				{
					temp[i] = padChar;
					i++;
				}
				temp[i] = '\0';
				strcat(str, temp);
				str = reverse(str);
			}
			return str;
	}
char* subString(char* str, int startpos, int len)
{
		char* substr;
	
		substr = (char*)malloc(len+1);	
		strncpy(substr,(const char*)(str+startpos),len);
		substr[len]='\0';
		return substr;
}
char *utoa(unsigned value, char *digits, int base)
{
    char *s, *p;

    s = "0123456789abcdefghijklmnopqrstuvwxyz"; /* don't care if s is in

                                                 * read-only memory
                                                 */
    if (base == 0)
        base = 10;
    if (digits == NULL || base < 2 || base > 36)
        return NULL;
    if (value < (unsigned) base) {
        digits[0] = s[value];
        digits[1] = '\0';
    } else {
        for (p = utoa(value / ((unsigned)base), digits, base);
             *p;
             p++);
        utoa( value % ((unsigned)base), p, base);
    }
    return digits;
}

char *itoa(int value, char *digits, int base)
{
    char *d;
    unsigned u; /* assume unsigned is big enough to hold all the
                 * unsigned values -x could possibly be -- don't
                 * know how well this assumption holds on the
                 * DeathStation 9000, so beware of nasal demons
                 */

    d = digits;
    if (base == 0)
        base = 10;
    if (digits == NULL || base < 2 || base > 36)
        return NULL;
    if (value < 0) {
        *d++ = '-';
        u = -((unsigned)value);
    } else
        u = value;
    utoa(u, d, base);
    return digits;
}


/**************************************************************************************************
* Function Name: itoa
* Description: Perform C++ style "itoa"
/****************************************************************************************************/

//char* itoa( int value, char* result, int base ) 
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
unsigned long hex2int(char *a)
{
    int i;
    unsigned long val = 0;
    unsigned int len = strlen(a);
				
    for(i=0;i<len;i++)
    {
							if(IsAscii(a[i]))
							a[i] = toupper(a[i]);	
       if(a[i] <= 57)
        val += (a[i]-48)*(1<<(4*(len-1-i)));
       else
        val += (a[i]-55)*(1<<(4*(len-1-i)));
    }
    return val;
}
bool IsAscii(char c)
{
	int i;
	i = c;
	if (i >= 48 && i<=57)
		return false;
	else return true;
}
bool CheckIfNotPDO(char* Index)
	{
		if(strncmp(Index, "14",2)==0 || strncmp(Index, "16",2)==0 || strncmp(Index, "18",2)==0 ||
				strncmp(Index,"1A",2)==0)
			return false;
		else return true;
	}
bool CheckIfManufactureSpecificObject(char* Index)
{
		unsigned long _Device_Index;
		_Device_Index = hex2int("2000");
		if(hex2int(Index) >= _Device_Index )
			return true;
		else return false;
}
bool CheckIfMappingPDO(char* Index)
{
		if(strncmp(Index, "1A",2)==0 || strncmp(Index, "16",2)==0)
		return true;
		else return false;
}
bool CheckAllowedCNIndexes(char* IndexValue)
{
	if((CheckIfNotPDO((char*)IndexValue)==false)  || CheckIfManufactureSpecificObject((char*)IndexValue) ||
					strcmp(IndexValue,"1F98")==0)
	return true;
	else return false;							
}
