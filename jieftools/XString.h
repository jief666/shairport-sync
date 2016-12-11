//*************************************************************************************************
//*************************************************************************************************
//
//                                          STRING
//
//*************************************************************************************************
//*************************************************************************************************

#if !defined(__XSTRING_H__)
#define __XSTRING_H__

#include <stdarg.h>

//#include "XToolsCommon.h"
#include "XConstString.h"
//#include "XStringW.h"

#if defined(__APPLE__) && defined(__OBJC__)
    #include <Foundation/NSString.h>
#endif

class XBuffer;
class XRBuffer;

extern Dll_IE xsize XStringGrowByDefault;

class Dll_IE XStringData
{
  public:

	xsize Len;
	xsize Size;
};

class XConstString;

class Dll_IE XString
{
  protected:
//	char *_Data;

  public:
  	const char *c;
	void Init(xsize aSize=0);
	XString();
	XString(const char*, const char* Format, ...) __attribute__((format (printf, 3, 4))); // 3 and 4 because of hidden parameter. Le premier paramËtre ne sert ‡ rien, c'est pour le diffÈrencier des autres constructeurs
	XString(const XString &aString);
#ifdef __AFXWIN_H__
	XString(const CString &aCString);
#endif
#ifdef __BORLANDC_VCL__
	XString(const AnsiString &aAString);
#endif
#ifdef _WX_WXSTRINGH__
	XString(const wxString &aWxString);
#endif
#if defined(__APPLE__) && defined(__OBJC__)
	XString(const NSString* const);
//	XString(const NSString* const aNSString, NSStringEncoding encoding);
#endif
	XString(const XConstString &aConstString);
	XString(const char *);
	XString(const char* S, xsize count);
//	XString(unsigned char);
	XString(char);
	XString(int);
	XString(unsigned int);
	XString(long);
	XString(unsigned long);

	~XString();

  protected:
	char *CheckSize(xsize nNewSize, xsize nGrowBy = XStringGrowByDefault);

	const XStringData *SData() const { return (((XStringData *)c)-1); }
	XStringData *SData() { return (((XStringData *)c)-1); }

  public:
	const char *Data(xsize ui=0) const { return c+ui; }
	char *Data(xsize ui=0) { return (char*)(c+ui); }
	char *DataWithSizeMin(xsize ui, xsize size, xsize nGrowBy=XStringGrowByDefault) { CheckSize(size, nGrowBy); return Data(ui); }

	xsize Length() const { return (xsize)SData()->Len; }
	//xsize length() const { return (xsize)SData()->Len; }
	xsize Size() const { return (xsize)SData()->Size; }
	void SetLength(xsize len);

	/* IsNull ? */
	bool IsNull() const { return Length() == 0 ; }
	bool NotNull() const { return Length() > 0 ; }
//	bool operator ()() const { return NotNull(); }
//	bool operator !() const { return IsNull(); }
//	operator bool() const { return NotNull(); }

	/* Cast */
	operator const char *() const { return Data(); }
//	operator char *() { return Data(); }
	#ifdef NOTESAPI_VERSION
//		operator LNString() const { if ( Length()==0 ) return ""; else return LNString(Data(), Length()); }
		operator LNString() const { return LNString(Data(), Length()); }
	#endif
	#ifdef __AFXWIN_H__
//		operator CString() const { return CString(Data()); }
	#endif
	#ifdef __BORLANDC_VCL__
		operator AnsiString() const { return AnsiString(Data()); }
	#endif
	#ifdef _WX_WXSTRINGH__
		operator wxString() const { return wxString(Data(), wxConvLocal, Length()); }
	#endif
	#if defined(__APPLE__) && defined(__OBJC__)
		operator NSString*() const { return [NSString stringWithCString:Data() encoding:NSUTF8StringEncoding]; }
	#endif
	
	int ToInt() const;
	unsigned int ToUInt() const;

	XStringW wcs();

	/* char [] */
	char operator [](int i) const {
		#if defined __XTOOLS_INT_CHECK__
			if ( i < 0 ) throw("XString index cannot < 0");
		#endif
		return *Data(i);
	}
	char operator [](xsize i) const { return *Data(i); }

	/* char& [] */
	char& operator [](int i) {
		#if defined __XTOOLS_INT_CHECK__
			if ( i < 0 ) throw("XString index cannot < 0");
		#endif
		return *Data(i);
	}
	char& operator [](xsize i) { return *Data(i); }

	char LastChar() const { if ( Length() > 0 ) return Data()[Length()-1]; else return 0; }
	void RemoveLastEspCtrl();

	void SetNull() { SetLength(0); };
	void StrnCpy(const char *buf, xsize len);
	void StrnCpy(const char *buf);
	void StrnCat(const char *buf, xsize len);
	void StrnCat(const char *buf);
	void Delete(xsize pos, xsize count=1);

	void Insert(xsize pos, const XString& Str);

	void Cat(const XString &uneXString);

	void vSPrintf(const char *Format, va_list va);
	void SPrintf(const char *format, ...) __attribute__((format (printf, 2, 3))); // 3 and 4 because of hidden parameter.

	const XString &operator =(const XString &aString);
#ifdef __AFXWIN_H__
	const XString &operator =(const CString &aCString);
#endif
#ifdef _WX_WXSTRINGH__
	const XString &operator =(const wxString &awxString);
#endif
	const XString &operator =(const XConstString &aConstString);
	const XString &operator =(const char* S);
	const XString &operator =(char);
	const XString &operator =(int);
	const XString &operator =(unsigned int);
	const XString &operator =(long);
	const XString &operator =(unsigned long);

	const XString &operator += (const XString &);
	const XString &operator += (const XConstString &aConstString);
	const XString &operator += (const char* S);
	const XString &operator += (char);
	const XString &operator += (int);
	const XString &operator += (unsigned int);
	const XString &operator += (long);
	const XString &operator += (unsigned long);

	/* ajout pour Notes (a mettre aprËs la dÈfinition de StrnCpy) */
	#ifdef NOTESAPI_VERSION
		XString(const LNString &aLNString) { Init(); StrnCpy((const char *)aLNString, aLNString.GetLength()); }
		const XString &operator =(const LNString &aLNString) { StrnCpy((const char *)aLNString, aLNString.GetLength());	return *this; }
	#endif

	XString SubString(xsize pos, xsize count) const;
    xsize IdxOf(char c, xsize Pos = 0) const;
	xsize IdxOf(const XString &S, xsize Pos = 0) const;
#ifdef NOT_USED_ANYMORE_skqdjfhksqjhfksjqdf
	xsize IdxOfIC(const XString &S, xsize Pos = 0) const;
	xsize IdxOfIAC(const XString &S, xsize Pos = 0) const;
#endif
	xsize RIdxOf(const XString &S, xsize Pos = MAX_XSIZE) const;

	void ToLower(bool FirstCharIsCap = false);
	bool IsLetters() const;
	bool IsLettersNoAccent() const;
	bool IsDigits() const;
	bool IsDigits(xsize pos, xsize count) const;

	bool ExistIn(const XString &S) const { return IdxOf(S) != MAX_XSIZE; }
#ifdef NOT_USED_ANYMORE_skqdjfhksqjhfksjqdf
	bool ExistInIC(const XString &S) const { return IdxOfIC(S) != MAX_XSIZE; }
	bool ExistInIAC(const XString &S) const { return IdxOfIC(S) != MAX_XSIZE; }
	bool DeleteIC(const XString &S);
#endif
	void Replace(char c1, char c2);
	XString SubStringReplace(char c1, char c2);

	int Compare(const char* S) const { return StringCompare(Data(), (S ? S : "")); }
#ifdef NOT_USED_ANYMORE_skqdjfhksqjhfksjqdf
	//IC
	int CompareIC(const char* S) const { return StringCompareIC(Data(), (S ? S : "")); }
	// IA
	int CompareIA(const char* S) const { return StringCompareIA(Data(), (S ? S : "")); }
	// IAC
	int CompareIAC(const char* S) const { return StringCompareIAC(Data(), (S ? S : "")); }
/*
	int CompareIACSubString(const char* S, size_t LenS) const { return SubStringCompareIAC(Data(), Length(), S, LenS); } //SubStringCompareIC renvoi 0 (ÈgalitÈ) si Len1 ou Len2 == 0
	int CompareIACSubString(xsize Pos, const char* S, size_t LenS) const { return SubStringCompareIAC(Data(Pos), Length()-Pos, S, LenS); } //SubStringCompareIC renvoi 0 (ÈgalitÈ) si Len1 ou Len2 == 0
	int CompareIACSubString(xsize Pos, xsize Len, const char* S, size_t LenS) const { return SubStringCompareIAC(Data(Pos), Len, S, LenS); }
*/
#endif

	bool Equal(const char* S) const { return Compare(S) == 0; };
	bool BeginEqual(const char* S) const { return StringBeginEqual(Data(), S); }
	bool SubStringEqual(xsize Pos, const char* S) const { return StringBeginEqual(Data(Pos), S); }
#ifdef NOT_USED_ANYMORE_skqdjfhksqjhfksjqdf
	// IC
	bool EqualIC(const char* S) const { return StringEqualIC(Data(), S); }
	bool BeginEqualIC(const char* S) const { return StringBeginEqualIC(Data(), S); }
	bool SubStringEqualIC(xsize Pos, const char* S) const { return StringBeginEqualIC(Data(Pos), S); }
	bool EqualSubStringIC(const char* S) const { return StringBeginEqualIC(S, Data()); }
	// IA
	bool EqualIA(const char* S) const { return CompareIA(S) == 0; };
	// IAC
	bool EqualIAC(const char* S) const { return CompareIAC(S) == 0; };
	bool BeginEqualIAC(const char* S) const { return StringBeginEqualIAC(Data(), S); }
	bool SubStringEqualIAC(xsize Pos, const char* S) const { return StringBeginEqualIAC(Data(Pos), S); }
	bool EqualSubStringIAC(const char* S) const { return StringBeginEqualIAC(S, Data()); }


/*	bool EqualIACSubString(const char* S, size_t LenS) const { return CompareIACSubString(S, LenS) == 0; }
	bool EqualIACSubString(xsize Pos, const char* S, size_t LenS) const { return CompareIACSubString(Pos, S, LenS) == 0; }
	bool EqualIACSubString(xsize Pos, xsize Len, const char* S, size_t LenS) const { return CompareIACSubString(Pos, Len, S, LenS) == 0; }
*/
#endif

	XString basename() const;
	XString dirname() const;

	size_t Sizeof() const { return size_t(sizeof(xsize)+Length()); } // overflow ? underflow ?
	bool ReadFromBuf(const char *buf, size_t *idx, size_t count);
	bool WriteToBuf(char *buf, size_t *idx, size_t count) const;
	bool ReadFromFILE(FILE *fp);
	bool WriteToFILE(FILE *fp) const;
	//
	bool ReadFromXBuffer(XRBuffer &unXBuffer); // Impossible de mettre le XBuffer en const car il y a une variable d'instance de XBuffer incrÈmentÈe par ReadFromXBuffer
	void CatToXBuffer(XBuffer *unXBuffer) const;
//	void WriteToXBuffer(XBuffer *unXBuffer, xsize *idx) const;

	#ifdef __DEVTOOLS_SOCKETS__
		void ReadFromSOCKETT(SOCKET Sock, unsigned int LenMax, unsigned int TO, const char *ErrMsg=NULL);
		bool ReadFromSOCKET(SOCKET Sock, unsigned int LenMax, unsigned int TO, const char *ErrMsg=NULL);
		void WriteToSOCKETT(SOCKET Sock, unsigned int TO,const char *ErrMsg = NULL) const;
		bool WriteToSOCKET(SOCKET Sock, unsigned int TO,const char *ErrMsg = NULL) const;
	#endif
  public:
	// OpÈrateur +
	// Chaines
	friend XString operator + (const XString& p1, const XString& p2) { XString s; s=p1; s+=p2; return s; }
	friend XString operator + (const XString& p1, const char *p2  ) { XString s; s=p1; s+=p2; return s; }
	friend XString operator + (const char *p1,   const XString& p2) { XString s; s=p1; s+=p2; return s; }
	friend XString operator + (const XConstString& p1,   const XString& p2) { XString s; s=p1; s+=p2; return s; }
	friend XString operator + (const XString& p1,   const XConstString& p2) { XString s; s=p1; s+=p2; return s; }
	friend XString operator + (const XConstString& p1,   const XConstString& p2) { XString s; s=p1; s+=p2; return s; }
	friend XString operator + (const XConstString &p1, const char *p2  ) { XString s; s=p1; s+=p2; return s; }
	friend XString operator + (const char *p1,   const XConstString &p2) { XString s; s=p1; s+=p2; return s; }
	// Char
	friend XString operator + (const XString& p1, char p2         ) { XString s; s=p1; s+=p2; return s; }
	friend XString operator + (char p1,   const XString& p2       ) { XString s; s=p1; s+=p2; return s; }
	// NumÈrique
	friend XString operator + (const XString& p1, int p2          ) { XString s; s=p1; s+=p2; return s; }
	friend XString operator + (int p1,   const XString& p2        ) { XString s; s=p1; s+=p2; return s; }
	friend XString operator + (const XString& p1, unsigned int p2         ) { XString s; s=p1; s+=p2; return s; }
	friend XString operator + (unsigned int p1,   const XString& p2       ) { XString s; s=p1; s+=p2; return s; }
	friend XString operator + (const XString& p1, long p2         ) { XString s; s=p1; s+=p2; return s; }
	friend XString operator + (long p1,   const XString& p2       ) { XString s; s=p1; s+=p2; return s; }
	friend XString operator + (const XString& p1, unsigned long p2        ) { XString s; s=p1; s+=p2; return s; }
	friend XString operator + (unsigned long p1,   const XString& p2      ) { XString s; s=p1; s+=p2; return s; }

	// OpÈrateur ==
	// Chaines
	friend bool operator == (const XString& s1,        const XString& s2)      { return s1.Compare(s2) == 0; }
	friend bool operator == (const XString& s1,        const char* s2  )        { return s1.Compare(s2) == 0; }
	friend bool operator == (const char* s1,            const XString& s2)      { return s2.Compare(s1) == 0; }
	friend bool operator == (const XConstString &s1,   const XString& s2)      { return s1.Compare(s2) == 0; }
	friend bool operator == (const XString &s1,        const XConstString& s2) { return s1.Compare(s2) == 0; }
	friend bool operator == (const XConstString &s1,   const XConstString& s2) { return s1.Compare(s2) == 0; }

	friend bool operator != (const XString& s1,        const XString& s2)      { return s1.Compare(s2) != 0; }
	friend bool operator != (const XString& s1,        const char* s2  )        { return s1.Compare(s2) != 0; }
	friend bool operator != (const char* s1,            const XString& s2)      { return s2.Compare(s1) != 0; }
	friend bool operator != (const XConstString &s1,   const XString& s2)      { return s1.Compare(s2) != 0; }
	friend bool operator != (const XString &s1,        const XConstString& s2) { return s1.Compare(s2) != 0; }
	friend bool operator != (const XConstString &s1,   const XConstString& s2) { return s1.Compare(s2) != 0; }

	friend bool operator <  (const XString& s1, const XString& s2) { return s1.Compare(s2) < 0; }
	friend bool operator <  (const XString& s1, const char* s2  ) { return s1.Compare(s2) < 0; }
	friend bool operator <  (const char* s1,   const XString& s2) { return s2.Compare(s1) > 0; }

	friend bool operator >  (const XString& s1, const XString& s2) { return s1.Compare(s2) > 0; }
	friend bool operator >  (const XString& s1, const char* s2  ) { return s1.Compare(s2) > 0; }
	friend bool operator >  (const char* s1,   const XString& s2) { return s2.Compare(s1) < 0; }

	friend bool operator <= (const XString& s1, const XString& s2) { return s1.Compare(s2) <= 0; }
	friend bool operator <= (const XString& s1, const char* s2  ) { return s1.Compare(s2) <= 0; }
	friend bool operator <= (const char* s1,   const XString& s2) { return s2.Compare(s1) >= 0; }

	friend bool operator >= (const XString& s1, const XString& s2) { return s1.Compare(s2) >= 0; }
	friend bool operator >= (const XString& s1, const char* s2  ) { return s1.Compare(s2) >= 0; }
	friend bool operator >= (const char* s1,   const XString& s2) { return s2.Compare(s1) <= 0; }

};

extern Dll_IE const XString NullXString;

Dll_IE XString SPrintf(const char *format, ...) __attribute__ ((format (printf, 1, 2)));;
Dll_IE XString SubString(const char *S, xsize pos, xsize count);
#ifdef NOT_USED_ANYMORE_skqdjfhksqjhfksjqdf
Dll_IE XString ToAlpha(const char *S);
Dll_IE XString ToAlpha(const XString &S);
Dll_IE XString ToLower(const char *S, bool FirstCharIsCap = false);
Dll_IE XString ToUpper(const char *S);
#endif
Dll_IE XString CleanCtrl(const XString &S);

#endif
