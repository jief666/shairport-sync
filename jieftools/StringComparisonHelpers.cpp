/*
 * StringComparisonHelpers.cpp
 *
 *  Created on: Nov 11, 2016
 *      Author: jief
 */

#include "StringComparisonHelpers.h"
#include <stddef.h>
#include <string.h>
#include <wchar.h>

/*------------------------------------------------*/

Dll_IE bool strequal(const char *s1, const char *s2)
{
	if ( s1 == s2 ) return true;  /* notamment vrai si s1==NULL && s2==NULL */
	if ( s1 == NULL  ||  s2 == NULL ) return false;
	return ( strcmp(s1, s2) == 0 );
}

Dll_IE bool strnequal(const char *s1, const char *s2, size_t count)
{
	if ( s1 == s2 ) return true;  /* notamment vrai si s1==NULL && s2==NULL */
	if ( s1 == NULL  ||  s2 == NULL ) return false;
	return ( strncmp(s1, s2, count) == 0 );
}

/*------------------------------------------------*/
/*
Return Value

If buf1 is...	return value is...

less than buf2	< 0
the same as buf2	== 0
greater than buf2	> 0
*/

/*-------------------------------------------------------------------------------------------------*/

int StringWCompare(const wchar_t* str1, const wchar_t* str2)
{
	if ( str1 == str2 ) return 0;  // EGAL notamment vrai si s1==NULL && s2==NULL
	if ( str1 == NULL ) return -1;  // str1 < str2
	if ( str2 == NULL ) return 1;  // str1 > str2
	return wcscmp(str1, str2);
}

bool StringEqual(const wchar_t *str1, const wchar_t *str2)
{
	if ( str1 == str2 ) return true;  // EGAL notamment vrai si s1==NULL && s2==NULL
	if ( str1 == NULL  ||  str2 == NULL ) return false;

	while ( *str2 ) {
		if ( *str1 != *str2 ) return false;
		str1++;
		str2++;
	}
	return *str1 == 0; // Si str1 est plus long, str1!=str2
}

// compare le début de str1 et str2.
bool StringWBeginEqual(const wchar_t* str1, const wchar_t* str2)
{
	if ( str1 == str2 ) return true;  // EGAL notamment vrai si s1==NULL && s2==NULL
	if ( str1 == NULL  ||  str2 == NULL ) return false;

	while ( *str2 ) {
		if ( *str1 != *str2 ) return false;
		str1++;
		str2++;
	}
	return true;
}

/*-------------------------------------------------------------------------------------------------*/
int StringCompare(const char *str1, const char *str2)
{
	if ( str1 == str2 ) return 0;  // EGAL notamment vrai si s1==NULL && s2==NULL
	if ( str1 == NULL ) return -1;  // str1 < str2
	if ( str2 == NULL ) return 1;  // str1 > str2

	int ret;

	while( (( ret = *str1 - *str2 )==0)  &&  *str1) ++str1, ++str2;

	if ( ret < 0 ) ret = -1;
	else if ( ret > 0 ) ret = 1;
	return( ret );
}

bool StringEqual(const char *str1, const char *str2)
{
	if ( str1 == str2 ) return true;  // EGAL notamment vrai si s1==NULL && s2==NULL
	if ( str1 == NULL  ||  str2 == NULL ) return false;

	while ( *str2 ) {
		if ( *str1 != *str2 ) return false;
		str1++;
		str2++;
	}
	return *str1 == 0; // Si str1 est plus long, str1!=str2
}

// compare le début de str1 et str2.
bool StringBeginEqual(const char *str1, const char *str2)
{
	if ( str1 == str2 ) return true;  // EGAL notamment vrai si s1==NULL && s2==NULL
	if ( str1 == NULL  ||  str2 == NULL ) return false;

	while ( *str2 ) {
		if ( *str1 != *str2 ) return false;
		str1++;
		str2++;
	}
	return true;
}

