//*************************************************************************************************
//*************************************************************************************************
//
//                                          CONST STRING
//
//*************************************************************************************************
//*************************************************************************************************

#if !defined(__XCONSTSTRING_H__)
#define __XCONSTSTRING_H__

#include <string.h>
#include "StringComparisonHelpers.h"


//#include "XToolsCommon.h"
#include "XBuffer.h"

class XStringW;

class Dll_IE XConstString
{
  protected:
	const char *_Data;
  public:
	XConstString(const char *S) { _Data = S; }
	XConstString(const XConstString &S) { _Data = S.Data(); }
	#ifdef NOTESAPI_VERSION
//		XConstString(const LNString &aString) { _Data = (const char *)aString; }
	#endif
	#ifdef __BORLANDC_VCL__
		XConstString(const AnsiString &aAString) { _Data = aAString.c_str(); }
	#endif

	const char *Data() const { return _Data; }
	operator const char *() const { return _Data; }

	xsize Length() const { return (xsize)strlen(Data()); }

	bool IsNull() const { return  (_Data == NULL) || (*_Data == 0) ; }
	bool NotNull() const { return _Data  &&  *_Data ; }

	size_t Sizeof() const { return size_t(sizeof(xsize)+Length()); } // overflow ? underflow ?
	void CatToXBuffer(XBuffer *unXBuffer) const	{ xsize ui=Length(); (*unXBuffer).Cat(ui); (*unXBuffer).Cat(Data(), ui); }

	XStringW wcs() const;

//	operator bool() const { return NotNull(); }
/*
	friend XString operator + (const XConstString &p1, const XConstString &p2) { XString s; s=p1; s+=p2; return s; }
	friend XString operator + (const XConstString &p1, const XString &p2  ) { XString s; s=p1; s+=p2; return s; }
	friend XString operator + (const XString &p1, const XConstString &p2  ) { XString s; s=p1; s+=p2; return s; }
	friend XString operator + (const XConstString &p1, const char *p2  ) { XString s; s=p1; s+=p2; return s; }
	friend XString operator + (const char *p1,   const XConstString &p2) { XString s; s=p1; s+=p2; return s; }
*/
//  jief : VC7 06/2004


	int Compare(const char *S) const { return StringCompare(Data(), S); }
#ifdef NOT_USED_ANYMORE_skqdjfhksqjhfksjqdf
	int CompareIC(const char *S) const { return StringCompareIC(Data(), S); }
	int CompareIA(const char *S) const { return StringCompareIA(Data(), S); }
	int CompareIAC(const char *S) const { return StringCompareIAC(Data(), S); }
#endif

	bool Equal(const char *S) const { return Compare(S) == 0; };
	bool BeginEqual(const char* S) const { return StringBeginEqual(Data(), S); }
#ifdef NOT_USED_ANYMORE_skqdjfhksqjhfksjqdf
	bool EqualIC(const char *S) const { return CompareIC(S) == 0; };
	bool EqualIA(const char *S) const { return CompareIA(S) == 0; };
	bool EqualIAC(const char *S) const { return CompareIAC(S) == 0; };
#endif
/*
	bool operator ==(const XConstString& s2) { return StringCompare(this->Data(), s2) == 0; }
	bool operator !=(const XConstString& s2) { return ! (*this == s2); }
	bool operator ==(const XString& s2) { return StringCompare(this->Data(), s2) == 0; }
	bool operator !=(const XString& s2) { return ! (*this == s2); }

	friend bool operator == (const XString& s1, const XConstString& s2) { return StringCompare(s1, s2) == 0; }
*//*
	friend bool operator == (const XConstString& s1, const XConstString& s2) { return StringCompare(s1, s2) == 0; }
	friend bool operator == (const XConstString& s1, const char *s2  ) { return StringCompare(s1, s2) == 0; }
*/
//	friend bool operator == (const char *s1,   const XConstString& s2) { return StringCompare(s1, s2) == 0; }
//	friend bool operator ^= (const XConstString& s1, const XConstString& s2) { return StringCompareIC(s1, s2) == 0; }
//	friend bool operator *= (const XConstString& s1, const XConstString& s2) { return StringCompareIAC(s1, s2) == 0; }
};

extern Dll_IE const XConstString NullXConstString;

#endif
