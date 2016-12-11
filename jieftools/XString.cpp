//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
//                                      STRING
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx

#if !defined(__XSTRING_CPP__)
#define __XSTRING_CPP__

#include <stdlib.h>
#include <errno.h>

#if defined(__APPLE__) && defined(__OBJC__)
	#import <Foundation/NSData.h>
	#import <Foundation/NSString.h>
#endif

//#include "XToolsCommon.h"
#include "XString.h"
#include "XBuffer.h"
#include "XRBuffer.h"
#ifdef __DEVTOOLS_SOCKETS__
	// Socket functions use exceptions
	#include "ToolsException.h"
#endif

#define PATH_SEPARATOR '/'

Dll_IE xsize XStringGrowByDefault = 1024;
Dll_IE const XString NullXString;

#define PRINTF_FORMAT "XString::Init(unsigned int="  XSIZE_PRINTF_FORMAT  ") -> malloc errno=%d\n"

void XString::Init(xsize aSize)
{
  XStringData *newSData;
//TRACE("Init\n");
	newSData = (XStringData *)malloc(sizeof(XStringData)+aSize+1); /* le 0 terminal n'est pas compté dans Size */
	if ( !newSData ) {
		printf(PRINTF_FORMAT, aSize, errno);
		throw 0xC0000017;
	}
	newSData->Size = aSize;
	newSData->Len = 0;
	c = (char *)(newSData + 1);
	((char *)c)[0] = 0;
}

XString::XString()
{
//Debugf("Construteur\n");
	Init();
}

XString::~XString()
{
//Debugf("Destructeur :%s\n", Data());
	free(SData());
}

void XString::SetLength(xsize len)
{
	SData()->Len = len;
	((char *)c)[len] = 0;
	#ifdef _DEBUG
		if ( strlen(Data()) != len ) {
			throw "XString SetLength error";
		}
	#endif
}

#undef PRINTF_FORMAT
#define PRINTF_FORMAT "XString::CheckSize(xsize="  XSIZE_PRINTF_FORMAT  ", xsize=" XSIZE_PRINTF_FORMAT ") -> realloc errno=%d\n"

/* CheckSize() */
char *XString::CheckSize(xsize nNewSize, xsize nGrowBy)
{
  XStringData *_SData = SData();

	if ( _SData->Size < nNewSize )
	{

		nNewSize += nGrowBy;
		_SData = (XStringData *)realloc((void *)_SData, sizeof(XStringData) + nNewSize + 1);
		if ( !_SData ) {
			printf(PRINTF_FORMAT, nNewSize, nGrowBy, errno);
			throw 0xC0000017;
		}
		_SData->Size = nNewSize;
		c = (char *)(_SData + 1);
//		c = _Data;
	}
	return ((char *)c);
}

void XString::StrnCpy(const char *buf, xsize len)
{
	if ( buf && *buf && len > 0 ) {
		CheckSize(len, 0);
		xsize idx = 0;
		char* p = Data();
		while ( idx++ < len  &&  (*p++ = *buf++));
		SetLength(idx-1); /* SetLength fait _Data[len]=0 */
	}else{
		SetLength(0); /* SetLength fait _Data[len]=0 */
	}
}

void XString::StrnCpy(const char *buf)
{
	if ( buf && *buf ) {
		StrnCpy(buf, (xsize)strlen(buf)); // overflow ?
	}else{
		SetLength(0); /* SetLength fait _Data[len]=0 */
	}
}

//inline
void XString::StrnCat(const char *buf, xsize len)
{
  xsize NewLen;

	if ( buf && *buf && len > 0 ) {
		NewLen = Length()+len;
		CheckSize(NewLen, 0);
		memcpy(Data()+Length(), buf, len);
		SetLength(NewLen); /* SetLength fait Data()[len]=0 */
	}
}

void XString::Cat(const XString &uneXString)
{
	CheckSize(Length()+uneXString.Length());
	memcpy(Data()+Length(), uneXString.c, uneXString.Length());
	SetLength(Length() + uneXString.Length());
}

inline void XString::StrnCat(const char *buf)
{
	if ( buf && *buf ) {
		StrnCat(buf, (xsize)strlen(buf)); // overflow ?
	}
}

void XString::Delete(xsize pos, xsize count)
{
	if ( pos < Length() ) {
		if ( count != MAX_XSIZE  &&  pos + count < Length() ) {
			memmove( Data()+pos, Data()+pos+count, Length()-pos-count);
			SetLength(Length()-count);
//			Data()[Length()] = 0; fait dans setlength();
		}else{
			SetLength(pos);
//			Data()[Length()] = 0; fait dans setlength();
		}
	}
}

void XString::Insert(xsize pos, const XString& Str)
{
	if ( pos < Length() ) {
		CheckSize(Length()+Str.Length());
		memmove(Data()+pos+Str.Length(), Data()+pos, Length()-pos);
		memcpy(Data()+pos, Str.Data(), Str.Length());
		/**/ // Modification par Hassan le 10/09/2001 16:40:16
		SetLength(Length()+Str.Length());
		/**/
	}else{
		StrnCat(Str);
	}
}

void XString::Replace(char c1, char c2)
{
  char* p;

	p = Data();
	while ( *p ) {
		if ( *p == c1 ) *p = c2;
		p += 1;
	}
}

XString XString::SubStringReplace(char c1, char c2)
{
  char* p;
  XString Result;

	p = Data();
	while ( *p  ) {
		if ( *p == c1 ) Result += c2;
		else Result += *p;
		p++;
	}
	return Result;
}

void XString::vSPrintf(const char *Format, va_list va)
{
  char buf[16384]; // faire mieux un jour...
  int nb;

  	#if defined(__DEV_BORLANDC__) || defined(__DEV_UNIX__)
		nb = vsprintf(buf, Format, va);
    #elif defined(__DEV_WIN32__)
		nb = _vsnprintf(buf, sizeof(buf), Format, va);
	#elif defined(__APPLE__)
		nb = vsnprintf(buf, sizeof(buf), Format, va);
	#elif defined(_GNU_SOURCE)
		nb = vsnprintf(buf, sizeof(buf), Format, va);
	#else
		#error Plateforme non supportee
    #endif
	if ( nb > 0 ) StrnCpy(buf, nb);
	else SetNull();
}

void XString::SPrintf(const char *Format, ...)
{
  va_list va;

	va_start(va, Format);
	vSPrintf(Format, va);
	va_end(va);
}


//XStringW XString::wcs()
//{
//  XBuffer xbuf;
////  wchar_t buf[Length() + 1];
//	size_t nbchar = mbstowcs((wchar_t *)xbuf.DataWithSizeMin(0, sizeof(wchar_t)*Length()), c, Length());
//	if ( nbchar == (size_t)-1 ) throw("XString::wcs() -> Conversion error");
//	return XStringW((wchar_t *)xbuf.Data(), (xsize)nbchar); // safe cast : nbchar cannot be > than Length
//}

XString XString::basename() const
{
	if ( LastChar() == PATH_SEPARATOR ) throw "XString::basename() -> LastChar() == PATH_SEPARATOR";
	xsize idx = RIdxOf(PATH_SEPARATOR);
	if ( idx == MAX_XSIZE ) return NullXString;
	return SubString(idx+1, Length()-idx-1);
}

XString XString::dirname() const
{
	xsize idx = RIdxOf(PATH_SEPARATOR);
	if ( idx == MAX_XSIZE ) return NullXString;
	#ifdef __DEV_WIN32__
		if ( idx == 1  &&  *Data(0) == PATH_SEPARATOR ) {
			// this string is an icomplete UNC name : \\server
			return NullXString;
		}
	#endif
	return SubString(0, idx);
}

//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
// Constructeurs Chaines
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
XString::XString(const char*, const char* Format, ...) // le premier paramètre ne sert à rien, c'est pour le différencier des autres constructeurs
{
  va_list va;
  char buf[16384]; // faire mieux un jour...
  int nb;

	va_start(va, Format);

  	#if defined(__DEV_BORLANDC__) || defined(__DEV_UNIX__)
		nb = vsprintf(buf, Format, va);
	#elif defined(__DEV_WIN32__)
		nb = _vsnprintf(buf, sizeof(buf), Format, va);
	#elif defined(__APPLE__)
		nb = vsnprintf(buf, sizeof(buf), Format, va);
	#elif defined(_GNU_SOURCE)
		nb = vsnprintf(buf, sizeof(buf), Format, va);
	#else
		#error Plateforme non supportee
    #endif
	if ( nb > 0 ) {
		Init(nb);
		StrnCpy(buf, nb);
	}else{
		Init(0);
	}
	va_end(va);
}

XString::XString(const XString &aString)
{
//Debugf("Construteur const String & : %s\n", aString);
	Init(aString.Length());
	StrnCpy(aString.Data(), aString.Length());
}

#ifdef __AFXWIN_H__
XString::XString(const CString &aCString)
{
//Debugf("Construteur const CString & : %s\n", aCString);
	Init(aCString.GetLength());
	StrnCpy(aCString, aCString.GetLength());
}
#endif

#ifdef __BORLANDC_VCL__
XString::XString(const AnsiString &aAString)
{
//Debugf("Construteur const AnsiString & : %s\n", aAString.c_str());
	Init(aAString.Length());
	StrnCpy(aAString.c_str(), aAString.Length());
}
#endif

#ifdef _WX_WXSTRINGH__
XString::XString(const wxString &aWxString)
{
	Init(aWxString.length());
	StrnCpy(aWxString.mb_str(), (unsigned int)(aWxString.length()));
}
#endif

#if defined(__APPLE__) && defined(__OBJC__)
XString::XString(const NSString* aNSString)
{
	NSData *nsData = [aNSString dataUsingEncoding: NSUTF8StringEncoding];
	if ( nsData != nil )

//	char buf[ [aNSString length]*MB_CUR_MAX ];
//	if ( [aNSString getCString:buf maxLength:[aNSString length]*MB_CUR_MAX encoding:NSUTF8StringEncoding] )
	{
		Init( (xsize)()[nsData length]+1) ); // overflow ?
		StrnCpy((const char *)[nsData bytes], (xsize)[nsData length]); // overflow ?
	}
	else {
#ifdef _DEBUG
		if ( [aNSString canBeConvertedToEncoding:NSUTF8StringEncoding] ) {
			NSLog(@"toto");
		}
#endif
		throw "convert NSString to XString failed";
		//Init(0);
	}
}
/*
XString::XString(const NSString* const aNSString, NSStringEncoding encoding)
{
	char buf[ [aNSString length]*MB_CUR_MAX+MB_CUR_MAX ];
	if ( [aNSString getCString:buf maxLength:[aNSString length]*MB_CUR_MAX+MB_CUR_MAX encoding:encoding] )
	{
		Init(strlen(buf));
		StrnCpy(buf);
	}
	else {
		Init(0);
	}
}
*/
#endif

XString::XString(const XConstString &aConstString)
{
//Debugf("Construteur const ConstString & : %s\n", aConstString);
	Init( aConstString.Length() );
	StrnCpy( aConstString.Data() );
}

XString::XString(const char *S)
{
//Debugf("Construteur const char * : %s\n", S);
	if ( S == NULL ) {
		Init(0);
	}else{
	  Init((xsize)strlen(S)); // overflow ?
	  if ( S ) StrnCpy(S);
	}
}

XString::XString(const char *S, xsize count)
{
//Debugf("Construteur const char *, unsigned int :%s   %d\n", S, count);
	Init(count);
	StrnCpy(S, count);
}

//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
// Constructeurs Caractères
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx

XString::XString(char aChar)
{
//TRACE("Construteur char \n");
	Init(1);
	StrnCpy(&aChar, 1);
}

//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
// Constructeurs numériques
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx

XString::XString(int i)
{
  char buf[1024];
  unsigned int len;

//TRACE("Copy Constructor(int) -> (%lu) %d\n", this, i);
	len = sprintf(buf,"%d", i);
	Init(len);
	StrnCpy(buf, len);
}

XString::XString(unsigned int ui)
{
  char buf[1024];
  unsigned int len;

//TRACE("Copy Constructor(unsigned int) -> (%lu) %u\n", this, ui);
	len = sprintf(buf,"%u", ui);
	Init(len);
	StrnCpy(buf, len);
}

XString::XString(long l)
{
  char buf[1024];
  unsigned int len;

//TRACE("Copy Constructor(long) -> (%lu) %ld\n", this, l);
	len = sprintf(buf,"%ld", l);
	Init(len);
	StrnCpy(buf, len);
}

XString::XString(unsigned long ul)
{
  char buf[1024];
  unsigned int len;

//TRACE("Copy Constructor(unsigned long) -> (%lu) %lu\n", this, ul);
	len = sprintf(buf,"%lu", ul);
	Init(len);
	StrnCpy(buf, len);
}


//-------------------------------------------------------------------------------------------------
// StringCompare
//-------------------------------------------------------------------------------------------------
// -1 si buf1 est plus grand
//  0 si égal
//  1 si buf2 est plus grand
//-------------------------------------------------------------------------------------------------


XString XString::SubString(xsize pos, xsize count) const
{
	if ( count > Length()-pos ) count = Length()-pos;
	return XString( &(Data()[pos]), count);
}

xsize XString::IdxOf(char aChar, xsize Pos) const
{
  xsize Idx;

	for ( Idx=Pos ; Idx<Length() ; Idx+=1 ) {
        if ( Data()[Idx] == aChar ) return Idx;
	}
	return MAX_XSIZE;
}

xsize XString::IdxOf(const XString &S, xsize Pos) const
{
  xsize i;
  xsize Idx;

	if ( Length() < S.Length() ) return MAX_XSIZE;
	for ( Idx=Pos ; Idx<=Length()-S.Length() ; Idx+=1 ) {
		i = 0;
	    while( i<S.Length()  &&  ( Data()[Idx+i] - S[i] ) == 0 ) i += 1;
		if ( i == S.Length() ) return Idx;
	}
	return MAX_XSIZE;
}

#ifdef NOT_USED_ANYMORE_skqdjfhksqjhfksjqdf
unsigned int XString::IdxOfIC(const XString &S, xsize Pos) const
{
  xsize i;
  xsize Idx;

	if ( Length() < S.Length() ) return MAX_XSIZE;
	for ( Idx=Pos ; Idx<=Length()-S.Length() ; Idx+=1 ) {
		i = 0;
	    while( i<S.Length()  &&  ( Minuscule(Data()[Idx+i]) - Minuscule(S[i]) ) == 0 ) i += 1;
		if ( i == S.Length() ) return Idx;
	}
	return MAX_XSIZE;
}

unsigned int XString::IdxOfIAC(const XString &S, xsize Pos) const
{
  xsize i;
  xsize Idx;

	if ( Length() < S.Length() ) return MAX_XSIZE;
	for ( Idx=Pos ; Idx<=Length()-S.Length() ; Idx+=1 ) {
		i = 0;
		while( i<S.Length()  &&  ( MinusculeSansAccent(Data()[Idx+i]) - MinusculeSansAccent(S[i]) ) == 0 ) i += 1;
		if ( i == S.Length() ) return Idx;
	}
	return MAX_XSIZE;
}
#endif

xsize XString::RIdxOf(const XString &S, xsize Pos) const
{
  xsize i;
  xsize Idx;

	if ( S.Length() == 0 ) return MAX_XSIZE;
	if ( Pos > Length() ) Pos = Length();
	if ( Pos < S.Length() ) return MAX_XSIZE;
	Pos -= S.Length();
	for ( Idx=Pos+1 ; Idx-- > 0 ; ) {
		i = 0;
		while( i<S.Length()  &&  Data()[Idx+i] == S[i] ) i += 1;
		if ( i == S.Length() ) return Idx;
	}
	return MAX_XSIZE;
}

#ifdef NOT_USED_ANYMORE_skqdjfhksqjhfksjqdf
bool XString::DeleteIC(const XString &S)
{
  xsize Idx;

	Idx = IdxOfIC(S);
	if ( Idx == MAX_XSIZE ) return false;
	Delete(Idx, S.Length());
	return true;
}
#endif

#ifdef NOT_USED_ANYMORE_skqdjfhksqjhfksjqdf
void XString::ToLower(bool FirstCharIsCap)
{

	if ( Length() > 0 )
	{
	  unsigned int ui;

		if ( FirstCharIsCap ) {
			Data()[0] = Majuscule(Data()[0]);
			ui = 1;
		}else{
			ui = 0;
		}
		for ( ; ui < Length() ; ui+=1 ) {
			Data()[ui] = Minuscule(Data()[ui]);
		}
	}
}

bool XString::IsLetters() const
{
  const char *p;
  char aChar;

	p = Data();
	if ( !*p ) return false;
	for ( ; *p ; p+=1 ) {
		aChar = MinusculeSansAccent(*p);  // toutes les lettres, avec accent ou pas, seront dans l'intervalle 'a'..'z'
		if ( aChar < 'a' ) return false;
		if ( aChar > 'z' ) return false;
	}
	return true;
}
#endif

bool XString::IsDigits() const
{
  const char *p;

	p = Data();
	if ( !*p ) return false;
	for ( ; *p ; p+=1 ) {
		if ( *p < '0' ) return false;
		if ( *p > '9' ) return false;
	}
	return true;
}

bool XString::IsDigits(xsize pos, xsize count) const
{
  const char *p;
  const char *q;

	if ( pos >= Length() ) {
		#ifdef _DEBUG
			throw "XString::IsDigits pos >= Length()";
		#else
			return false;
		#endif
	}
	if ( pos+count > Length() ) {
		#ifdef _DEBUG
			throw "XString::IsDigits pos+count > Length()";
		#else
			return false;
		#endif
	}
	p = Data() + pos;
	q = p + count;
	for ( ; p < q ; p+=1 ) {
		if ( *p < '0' ) return false;
		if ( *p > '9' ) return false;
	}
	return true;
}

#ifdef NOT_USED_ANYMORE_skqdjfhksqjhfksjqdf
bool XString::IsLettersNoAccent() const
{
  const char *p;
  char aChar;

	p = Data();
	if ( !*p ) return false;
	for ( ; *p ; p+=1 ) {
		aChar = Minuscule(*p); // Uniquement les lettres maj et min sans accent seront dans l'intervalle 'a'..'z'
		if ( aChar < 'a' ) return false;
		if ( aChar > 'z' ) return false;
	}
	return true;
}
#endif

void XString::RemoveLastEspCtrl()
{
  char *p;

	if ( Length() > 0 ) {
		p = Data() + Length() - 1;
		if ( *p >= 0 && *p <= ' ' ) {
			p -= 1;
			while ( p>Data() && *p >= 0 && *p <= ' ' ) p -= 1;
			if ( p>Data() ) {
				SetLength( (xsize)(p-Data()+1) ); // safe (p-Data()+1 < Length()
			}else{
				if ( *p >= 0 && *p <= ' ' ) SetLength(0);
				else SetLength(1);
			}
		}
	}
}

int XString::ToInt() const
{
  int i;

	if ( sscanf(Data(), "%d", &i) != 1 ) return INT_MIN;
	return i;
}

unsigned int XString::ToUInt() const
{
  unsigned int u;

	if ( sscanf(Data(), "%u", &u) != 1 ) return UINT_MAX;
	return u;
}

//-------------------------------------------------------------------------------------------------
//
//-------------------------------------------------------------------------------------------------
//void mylog2(const char* format, ...)
//{
//  va_list va;
//  char buf[16384];
//
//	va_start(va, format);
//	int nb = vsprintf_s(buf, sizeof(buf), format, va);
//	va_end(va);
//
//	FILE* fp;
//	fopen_s(&fp, "C:\\jieflog.txt", "a");
//	if ( fp != NULL ) {
//		fwrite(buf, 1, nb, fp);
//		fclose(fp);
//	}
//}

bool XString::ReadFromBuf(const char *buf, size_t *idx, size_t count)
{
  xsize longueur;

//mylog2(::SPrintf("XString::ReadFromBuf *idx=%d count=%d - %d %d %d %d\n", *idx, count, buf[*idx+0], buf[*idx+1], buf[*idx+2], buf[*idx+3]).c);
	if ( count-*idx >= sizeof(longueur) ) {
		longueur = *((xsize *)(buf+*idx));
//mylog2(::SPrintf("XString::ReadFromBuf *idx=%d count=%d longueur=%d\n", *idx, count, longueur).c);
		*idx += sizeof(longueur);
		if (  longueur > 0  &&  count-*idx>=longueur  ) memcpy(DataWithSizeMin(0, longueur), buf+*idx, longueur);
		*idx += longueur;
		SetLength(longueur);
		return true;
	}else{
		SetNull();
		return false;
	}
}

bool XString::WriteToBuf(char *buf, size_t *idx, size_t count) const
{
  xsize longueur;

	if ( count-*idx < sizeof(longueur) + Length() ) return false;
	longueur = Length();
	memcpy(buf+*idx, &longueur, sizeof(longueur));
	*idx += sizeof(longueur);
	memcpy(buf+*idx, Data(), Length());
	*idx += Length();
	return true;
}

bool XString::ReadFromFILE(FILE *fp)
{
  unsigned int longueur;

	if ( fread(&longueur, sizeof(longueur), 1, fp) != 1 ) goto fin;
	if ( longueur > 0  &&  fread(DataWithSizeMin(0, longueur), longueur, 1, fp) != 1 ) goto fin;
	SetLength(longueur);
	return true;
  fin:
	SetNull();
	return false;
}

bool XString::WriteToFILE(FILE *fp) const
{
  xsize longueur;

	longueur = Length();
	if ( fwrite(&longueur, sizeof(longueur), 1, fp) != 1 ) return false;
	if ( longueur > 0  &&  fwrite(Data(), longueur, 1, fp) != 1 ) return false;
	return true;
}

#ifdef __DEVTOOLS_SOCKETS__
void XString::ReadFromSOCKETT(SOCKET Sock, unsigned int LenMax, unsigned int TO, const char *ErrMsg)
{
  unsigned int longueur;

	SockReceiveT(Sock, &longueur, sizeof(longueur), TO, ErrMsg);
	if ( longueur > LenMax ) Throw("Longueur reÁue (%d) supérieure ‡ la longueur max (%d)", longueur, LenMax);
	if ( longueur > 0 ) SockReceiveT(Sock, DataWithSizeMin(0, longueur, 0), longueur, TO, ErrMsg);
	SetLength(longueur);
}

bool XString::ReadFromSOCKET(SOCKET Sock, unsigned int LenMax, unsigned int TO, const char *ErrMsg)
{
	try
	{
		ReadFromSOCKETT(Sock, LenMax, TO, ErrMsg);
		SetLastErrorFlag(false);
	}
	StdCatch();
	return !LastErrorFlag();
}

void XString::WriteToSOCKETT(SOCKET Sock, unsigned int TO,const char *ErrMsg) const
{
  unsigned int longueur;

	longueur = Length();
	SockSendT(Sock, &longueur, sizeof(longueur), TO, ErrMsg);
	if ( longueur > 0 ) SockSendT(Sock, Data(), longueur, TO, ErrMsg);
}

bool XString::WriteToSOCKET(SOCKET Sock, unsigned int TO,const char *ErrMsg) const
{
	try
	{
		WriteToSOCKET(Sock, TO, ErrMsg);
		SetLastErrorFlag(false);
	}
	StdCatch();
	return !LastErrorFlag();
}
#endif

bool XString::ReadFromXBuffer(XRBuffer &unXBuffer)
{
  xsize longueur;

	if ( !unXBuffer.GetXSize(&longueur) ) goto fin;
	if ( longueur>0  &&  !unXBuffer.Get(DataWithSizeMin(0, longueur), longueur) ) goto fin;
	SetLength(longueur);
	return true;
fin:
	SetNull();
	return false;
}

void XString::CatToXBuffer(XBuffer *unXBuffer) const
{
	(*unXBuffer).Cat(Length());
	(*unXBuffer).Cat(Data(), Length());
}

//*************************************************************************************************
//
//                                       Opérateurs =
//
//*************************************************************************************************


//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
// Opérateur = CaractËres
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx

const XString &XString::operator =(char aChar)
{
//TRACE("Operator =char \n");
	StrnCpy(&aChar, 1);
	return *this;
}


//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
// Opérateur = Chaines
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx

const XString &XString::operator =(const XString &aString)
{
//TRACE("Operator =const XString&\n");
	StrnCpy(aString.Data(), aString.Length());
	return *this;
}

#ifdef __AFXWIN_H__
const XString &XString::operator =(const CString &aCString)
{
//TRACE("Operator =const CString&\n");
	StrnCpy(aCString, aCString.GetLength());
	return *this;
}
#endif

#ifdef _WX_WXSTRINGH__
const XString &XString::operator =(const wxString &awxString)
{
//TRACE("Operator =const wxString&\n");
	StrnCpy(awxString.mb_str(), awxString.length());
	return *this;
}
#endif

const XString &XString::operator =(const XConstString &aConstString)
{
//TRACE("Operator =const XString&\n");
	StrnCpy(aConstString.Data());
	return *this;
}

const XString &XString::operator =(const char *S)
{
//TRACE("Operator =const char *\n");
	StrnCpy(S);
	return *this;
}


//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
// Opérateur = numériques
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx

const XString &XString::operator =(int i)
{
  char buf[1024];
  unsigned int len;

//TRACE("Opérateur =(int) -> (%lu) %d\n", this, i);
	len = sprintf(buf,"%d", i);
	StrnCpy(buf, len);
	return *this;
}

const XString &XString::operator =(unsigned int ui)
{
  char buf[1024];
  unsigned int len;

//TRACE("Opérateur =(long) -> (%lu) %u\n", this, ui);
	len = sprintf(buf,"%u", ui);
	StrnCpy(buf, len);
	return *this;
}

const XString &XString::operator =(long l)
{
  char buf[1024];
  unsigned int len;

//TRACE("Opérateur =(long) -> (%lu) %ld\n", this, l);
	len = sprintf(buf,"%ld", l);
	StrnCpy(buf, len);
	return *this;
}

const XString &XString::operator =(unsigned long ul)
{
  char buf[1024];
  unsigned int len;

//TRACE("Opérateur =(long) -> (%lu) %lu\n", this, ul);
	len = sprintf(buf,"%lu", ul);
	StrnCpy(buf, len);
	return *this;
}



//*************************************************************************************************
//
//                                       Opérateurs +=
//
//*************************************************************************************************


//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
// Opérateur = CaractËres
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx

const XString &XString::operator +=(char aChar)
{
//TRACE("Operator +=char \n");
	StrnCat(&aChar, 1);
	return *this;
}


//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
// Opérateur = Chaines
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx

const XString &XString::operator +=(const XString &aString)
{
//TRACE("Operator +=const XString&\n");
	StrnCat(aString.Data(), aString.Length());
	return *this;
}

const XString &XString::operator +=(const XConstString &aConstString)
{
//TRACE("Operator +=const EConstString&\n");
	StrnCat(aConstString.Data(), aConstString.Length());
	return *this;
}

const XString &XString::operator +=(const char *S)
{
//TRACE("operator +=const char *\n");
	StrnCat(S);
	return *this;
}


//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
// Opérateur = numériques
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx

const XString &XString::operator +=(int i)
{
  char buf[1024];
  unsigned int len;

//TRACE("Opérateur =(int) -> (%lu) %d\n", this, i);
	len = sprintf(buf,"%d", i);
	StrnCat(buf, len);
	return *this;
}

const XString &XString::operator +=(unsigned int ui)
{
  char buf[1024];
  unsigned int len;

//TRACE("Opérateur =(unsigned int) -> (%lu) %u\n", this, ui);
	len = sprintf(buf,"%u", ui);
	StrnCat(buf, len);
	return *this;
}

const XString &XString::operator +=(long l)
{
  char buf[1024];
  unsigned int len;

//TRACE("Opérateur =(long) -> (%lu) %ld\n", this, l);
	len = sprintf(buf,"%ld", l);
	StrnCat(buf, len);
	return *this;
}

const XString &XString::operator +=(unsigned long ul)
{
  char buf[1024];
  unsigned int len;

//TRACE("Opérateur =(unsigned long) -> (%lu) %lu\n", this, ul);
	len = sprintf(buf,"%lu", ul);
	StrnCat(buf, len);
	return *this;
}


//-----------------------------------------------------------------------------
//                                 Fonction
//-----------------------------------------------------------------------------

XString SPrintf(const char *format, ...)
{
  va_list va;
  char buf[16384];

	va_start(va, format);
	vsprintf(buf, format, va);
	va_end(va);
	return XString(buf);
}

XString SubString(const char *S, xsize pos, xsize count)
{
	if ( strlen(S)-pos < count ) count = (xsize)(strlen(S)-pos); // overflow ?
	return ( XString(S+pos, count) );
}

#ifdef NOT_USED_ANYMORE_skqdjfhksqjhfksjqdf
XString ToAlpha(const char *S)
{
  XString ReturnValue;
  unsigned int i;

	for ( i=0 ; S[i] ; i+=1 ) {
		ReturnValue += ToAlpha(S[i]);
	}
	return ReturnValue;
}

XString ToAlpha(const XString &S)
{
  XString ReturnValue;
  unsigned int i;

	for ( i=0 ; i<S.Length() ; i+=1 ) {
		ReturnValue += ToAlpha(S[i]);
	}
	return ReturnValue;
}

XString ToLower(const char *S, bool FirstCharIsCap)
{
  XString ReturnValue;
  unsigned int ui;

	if ( S && *S ) {
		if ( FirstCharIsCap ) ReturnValue = Majuscule(S[0]);
		else ReturnValue = Minuscule(S[0]);

		for ( ui=1 ; S[ui] ; ui+=1 ) {
			ReturnValue += Minuscule(S[ui]);
		}
	}
	return ReturnValue;
}

XString ToUpper(const char *S)
{
  XString ReturnValue;
  unsigned int ui;

	if ( S && *S ) {
		for ( ui=0 ; S[ui] ; ui+=1 ) {
			ReturnValue += Majuscule(S[ui]);
		}
	}
	return ReturnValue;
}
#endif

XString CleanCtrl(const XString &S)
{
  XString ReturnValue;
  xsize i;

	for ( i=0 ; i<S.Length() ; i+=1 ) {
		if ( S[i] >=0  &&  S[i] < ' ' ) ReturnValue += 'x'; /* Les char sont signés !!! */
		else ReturnValue += S[i];
	}
	return ReturnValue;
}

#endif
