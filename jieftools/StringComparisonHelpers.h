/*
 * StringComparisonHelpers.cpp
 *
 *  Created on: Nov 11, 2016
 *      Author: jief
 */

#include <stddef.h>


#ifndef Dll_IE
#define Dll_IE
#endif


/*------------------------------------------------*/

Dll_IE bool strequal(const char *s1, const char *s2);
Dll_IE bool strnequal(const char *s1, const char *s2, size_t count);

/*------------------------------------------------*/
/*
Return Value

If buf1 is...	return value is...

less than buf2	< 0
the same as buf2	== 0
greater than buf2	> 0
*/

/*-------------------------------------------------------------------------------------------------*/

int StringWCompare(const wchar_t* str1, const wchar_t* str2);
bool StringEqual(const wchar_t *str1, const wchar_t *str2);

// compare le début de str1 et str2.
bool StringWBeginEqual(const wchar_t* str1, const wchar_t* str2);

/*-------------------------------------------------------------------------------------------------*/
int StringCompare(const char *str1, const char *str2);
bool StringEqual(const char *str1, const char *str2);

// compare le début de str1 et str2.
bool StringBeginEqual(const char *str1, const char *str2);

