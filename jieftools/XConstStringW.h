//*************************************************************************************************
//*************************************************************************************************
//
//                                          CONST STRING
//
//*************************************************************************************************
//*************************************************************************************************

#if !defined(__XCONSTSTRINGW_H__)
#define __XCONSTSTRINGW_H__

#include "XToolsCommon.h"
#include "XBuffer.h"

class XString;

class Dll_IE XConstStringW
{
  protected:
	const wchar_t *_Data;
  public:
	XConstStringW(const wchar_t *S) { _Data = S; }
	XConstStringW(const XConstStringW &S) { _Data = S.Data(); }
	#ifdef NOTESAPI_VERSION
//		XConstStringW(const LNString &aString) { _Data = (const wchar_t *)aString; }
	#endif
	#ifdef __BORLANDC_VCL__
		XConstStringW(const AnsiString &aAString) { _Data = aAString.c_str(); }
	#endif

	const wchar_t *Data() const { return _Data; }
	operator const wchar_t *() const { return _Data; }

	xsize Length() const { return (xsize)wcslen(Data()); } // overflow ?

	bool IsNull() { return  (_Data == NULL) || (*_Data == 0) ; }
	bool NotNull() const { return _Data  &&  *_Data ; }

	XString mbs() const;

	size_t Sizeof() const { return size_t(sizeof(xsize)+Length()); } // overflow ? underflow ?
	void CatToXBuffer(XBuffer *unXBuffer) const	{ xsize ui=Length(); (*unXBuffer).Cat(ui); (*unXBuffer).Cat(Data(), ui); }

//	operator bool() const { return NotNull(); }
/*
	friend XStringW operator + (const XConstStringW &p1, const XConstStringW &p2) { XStringW s; s=p1; s+=p2; return s; }
	friend XStringW operator + (const XConstStringW &p1, const XStringW &p2  ) { XStringW s; s=p1; s+=p2; return s; }
	friend XStringW operator + (const XStringW &p1, const XConstStringW &p2  ) { XStringW s; s=p1; s+=p2; return s; }
	friend XStringW operator + (const XConstStringW &p1, const wchar_t *p2  ) { XStringW s; s=p1; s+=p2; return s; }
	friend XStringW operator + (const wchar_t *p1,   const XConstStringW &p2) { XStringW s; s=p1; s+=p2; return s; }
*/
//  jief : VC7 06/2004


	int Compare(const wchar_t *S) const { return wcscmp(Data(), S); }
#ifdef NOT_USED_ANYMORE_skqdjfhksqjhfksjqdf
	int CompareIC(const wchar_t *S) const { return StringCompareIC(Data(), S); }
	int CompareIA(const wchar_t *S) const { return StringCompareIA(Data(), S); }
	int CompareIAC(const wchar_t *S) const { return StringCompareIAC(Data(), S); }
#endif
	bool Equal(const wchar_t *S) const { return Compare(S) == 0; };
#ifdef NOT_USED_ANYMORE_skqdjfhksqjhfksjqdf
	bool EqualIC(const wchar_t *S) const { return CompareIC(S) == 0; };
	bool EqualIA(const wchar_t *S) const { return CompareIA(S) == 0; };
	bool EqualIAC(const wchar_t *S) const { return CompareIAC(S) == 0; };
#endif
/*
	bool operator ==(const XConstStringW& s2) { return StringCompare(this->Data(), s2) == 0; }
	bool operator !=(const XConstStringW& s2) { return ! (*this == s2); }
	bool operator ==(const XStringW& s2) { return StringCompare(this->Data(), s2) == 0; }
	bool operator !=(const XStringW& s2) { return ! (*this == s2); }

	friend bool operator == (const XStringW& s1, const XConstStringW& s2) { return StringCompare(s1, s2) == 0; }
*//*
	friend bool operator == (const XConstStringW& s1, const XConstStringW& s2) { return StringCompare(s1, s2) == 0; }
	friend bool operator == (const XConstStringW& s1, const wchar_t *s2  ) { return StringCompare(s1, s2) == 0; }
*/
//	friend bool operator == (const wchar_t *s1,   const XConstStringW& s2) { return StringCompare(s1, s2) == 0; }
//	friend bool operator ^= (const XConstStringW& s1, const XConstStringW& s2) { return StringCompareIC(s1, s2) == 0; }
//	friend bool operator *= (const XConstStringW& s1, const XConstStringW& s2) { return StringCompareIAC(s1, s2) == 0; }
};

extern Dll_IE const XConstStringW NullXConstStringW;

#endif
