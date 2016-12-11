//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
//
//                                           STRINGS
//
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx

#if !defined(__XSTRINGS_CPP__)
#define __XSTRINGS_CPP__

//include "XToolsCommon.h"
#include "XStrings.h"
//#include "XString.h"
//#include "XFILE.h"
//#include "XRBuffer.h"
//#include "ToolsException.h"

Dll_IE const XStrings NullXStrings;


XStrings::XStrings() : XStringsSuper()
{
	#if defined(_DEBUG) && defined(__DEV_VISUALC__)
		const char* tmp = DbgAt(0);
	#endif
}

size_t XStrings::Sizeof() const
{
  size_t l;
  xsize ui;

	l = sizeof(unsigned int);
	for ( ui = 0 ; ui < Length() ; ui += 1 ) {
		l += ElementAt(ui).Sizeof(); // overflow ?
	}
	return l;
}

const char *XStrings_SkipArg(const char *p)
{
	while ( *p == ' ' ) p += 1;
	if ( *p == 0 ) return NULL;
	if ( *p == '"' ) {
		p += 1;
		while ( *p  &&  *p != '"' ) p += 1;
		if ( *p == '"' ) p += 1;
		return p; /* si on est au bout sans trouver de '"', p pointe sur 0 */
	}else
	if ( *p == '\'' ) {
		p += 1;
		while ( *p  &&  *p != '\'' ) p += 1;
		if ( *p == '\'' ) p += 1;
		return p; /* si on est au bout sans trouver de '"', p pointe sur 0 */

	}else{
		while ( *p  &&  *p != ' ' ) p += 1;
		return p; /* si on est au bout sans trouver de ' ', p pointe sur 0 */
	}
}

void XStrings::ReadFromString(const char* buf)
{
  const char *p;
  const char *s;

	Empty();
	p = buf;
	while ( *p == ' ' ) p += 1;
	while ( *p ) {
		s = p;
		if ( *s == '\"' ) {
			p = XStrings_SkipArg(p);
			if ( p-s >= 2 ) AddCopy(SubString(s+1, 0, (xsize)(p-s-2))); // overflow ?
		}else
		if ( *s == '\'' ) {
			p = XStrings_SkipArg(p);
			if ( p-s >= 2 ) AddCopy(SubString(s+1, 0, (xsize)(p-s-2))); // overflow ?
		}else{
			p = XStrings_SkipArg(p);
			if ( p-s >= 0 ) AddCopy(SubString(s, 0, (xsize)(p-s))); // overflow ?
		}
		while ( *p == ' ' ) p += 1;
	}
}

//void mylog(const char* format, ...)
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

bool XStrings::ReadFromBuf(const char *buf, size_t *idx, size_t count)
{
  xsize longueur, ui;

//mylog(SPrintf("XStrings::ReadFromBuf *idx=%d count=%d - %d %d %d %d\n", *idx, count, buf[*idx+0], buf[*idx+1], buf[*idx+2], buf[*idx+3]).c);
	if ( count-*idx < sizeof(longueur) ) goto fin;

	longueur = *((xsize *)(buf+*idx));
	*idx += sizeof(longueur);
//mylog(SPrintf("XStrings::ReadFromBuf *idx=%d count=%d longueur=%d\n", *idx, count, longueur).c);

	for ( ui = 0 ; ui < longueur ; ui += 1 )
	{
	  XString *NewS;

		NewS = new XString;
		if ( !NewS->ReadFromBuf(buf, idx, count) ) goto fin;
//mylog("XStrings::ReadFromBuf add %s\n", NewS->c);
		AddRef(NewS, true);
	}
	return true;
  fin:
	SetNull();
	return false;
}

bool XStrings::WriteToBuf(char *buf, size_t *idx, size_t count) const
{
  xsize longueur;

	if ( count-*idx < sizeof(longueur) ) return false;
	longueur = Length();
	memcpy(buf+*idx, &longueur, sizeof(longueur));
	*idx += sizeof(longueur);
	for ( xsize ui = 0 ; ui < longueur ; ui += 1 ) {
		if ( !ElementAt(ui).WriteToBuf(buf, idx, count) ) return false;
	}
	return true;
}

bool XStrings::ReadFromFILE(FILE *fp)
{
  unsigned int longueur, ui;

	if ( fread(&longueur, sizeof(longueur), 1, fp) != 1 ) goto fin;
	for ( ui = 0 ; ui < longueur ; ui += 1 )
	{
	  XString *NewS;

		NewS = new XString;
		if ( !NewS->ReadFromFILE(fp) ) goto fin;
		AddRef(NewS, true);
	}
	return true;
  fin:
	SetNull();
	return false;
}

bool XStrings::WriteToFILE(FILE *fp) const
{
  xsize longueur, ui;

	longueur = Length();
	if ( fwrite(&longueur, sizeof(longueur), 1, fp) != 1 ) return false;
	for ( ui = 0 ; ui < longueur ; ui += 1 ) {
		if ( !ElementAt(ui).WriteToFILE(fp) ) return false;
	}
	return true;
}

bool XStrings::ReadFromXBuffer(XRBuffer &unXBuffer)
{
  xsize longueur, ui;

	if ( !unXBuffer.GetXSize(&longueur) ) goto fin;

	for ( ui = 0 ; ui < longueur ; ui += 1 )
	{
	  XString *NewS;

		NewS = new XString;
		if ( !NewS->ReadFromXBuffer(unXBuffer) ) goto fin;
		AddRef(NewS, true);
	}
	return true;
  fin:
	SetNull();
	return false;
}

void XStrings::CatToXBuffer(XBuffer *unXBuffer) const
{
	(*unXBuffer).Cat(Length());
	for ( unsigned int ui = 0 ; ui < Length() ; ui += 1 ) {
		ElementAt(ui).CatToXBuffer(unXBuffer);
	}
}

//-------------------------------------------------------------------------------------------------
//
//-------------------------------------------------------------------------------------------------
void XStrings::AddStrings(const char *Val1, ...)
{
  va_list va;
  const char *p;

	AddCopy(Val1);
	va_start(va, Val1);
	p = va_arg(va, const char *);
	while ( p != NULL ) {
		AddCopy(p);
		p = va_arg(va, const char *);
	}
	va_end(va);
}

XString XStrings::ConcatAll(XString Separator, XString Prefix, XString Suffix) const
{
  unsigned int i;
  XString s;

	if ( Length() > 0 ) {
		s = Prefix;
		s += ElementAt(0);
		for ( i=1 ; i<Length() ; i+=1 ) {
			s += Separator;
			s += ElementAt(i);
		}
		s += Suffix;
	}
	return s;
}

bool XStrings::Equal(const XStrings &aStrings) const
{
  unsigned int ui;

	if ( Length() != aStrings.Length() ) return false;
	for ( ui=0 ; ui<Length() ; ui+=1 ) {
		if ( ElementAt(ui) != aStrings[ui] ) return false;
	}
	return true;
}
/*
bool XStrings::EqualIC(const XStrings &aStrings) const
{
  unsigned int i;

	for ( i=0 ; i<Length() ; i+=1 ) {
		if ( !aStrings.ExistInIC(ElementAt(i)) ) return false;
	}
	for ( i=0 ; i<aStrings.Length() ; i+=1 ) {
		if ( !ExistInIC(aStrings.ElementAt(i)) ) return false;
	}
	return true;
}
*/
bool XStrings::Same(const XStrings &aStrings) const
{
  unsigned int i;

	for ( i=0 ; i<Length() ; i+=1 ) {
		if ( !aStrings.ExistIn(ElementAt(i)) ) return false;
	}
	for ( i=0 ; i<aStrings.Length() ; i+=1 ) {
		if ( !ExistIn(aStrings.ElementAt(i)) ) return false;
	}
	return true;
}

#ifdef NOT_USED_ANYMORE_skqdjfhksqjhfksjqdf
bool XStrings::SameIC(const XStrings &aStrings) const
{
  unsigned int i;

	for ( i=0 ; i<Length() ; i+=1 ) {
		if ( !aStrings.ExistInIC(ElementAt(i)) ) return false;
	}
	for ( i=0 ; i<aStrings.Length() ; i+=1 ) {
		if ( !ExistInIC(aStrings.ElementAt(i)) ) return false;
	}
	return true;
}
#endif

xsize XStrings::IdxOf(const XString &S) const
{
  unsigned int i;

	for ( i=0 ; i<Length() ; i+=1 ) {
		if ( StringCompare(ElementAt(i), S) == 0 ) return i;
	}
	return MAX_XSIZE;
}

#ifdef NOT_USED_ANYMORE_skqdjfhksqjhfksjqdf
bool XStrings::ExistInIC(const XString &S) const
{
  unsigned int i;

	for ( i=0 ; i<Length() ; i+=1 ) {
		if ( StringCompareIC(ElementAt(i), S) == 0 ) return true;
	}
	return false;
}

bool XStrings::ExistInIAC(const XString &S) const
{
  unsigned int i;

	for ( i=0 ; i<Length() ; i+=1 ) {
		if ( StringCompareIAC(ElementAt(i), S) == 0 ) return true;
	}
	return false;
}
#endif

//-------------------------------------------------------------------------------------------------
// Add
//-------------------------------------------------------------------------------------------------
void XStrings::Add(const XStrings &aStrings)
{
  unsigned int i;

	for ( i=0 ; i<aStrings.Length() ; i+=1 ) {
		AddCopy(aStrings[i]);
	}
}

void XStrings::AddID(const XString &aString)
{
	if ( !ExistIn(aString) ) AddCopy(aString);
}

void XStrings::AddID(const XStrings &aStrings)
{
  unsigned int i;

	for ( i=0 ; i<aStrings.Length() ; i+=1 ) {
		if ( !ExistIn(aStrings[i]) ) AddCopy(aStrings[i]);
	}
}

#ifdef NOT_USED_ANYMORE_skqdjfhksqjhfksjqdf
void XStrings::AddICD(const XString &aString)
{
	if ( !ExistInIC(aString) ) AddCopy(aString);
}

void XStrings::AddICD(const XStrings &aStrings)
{
  unsigned int i;

	for ( i=0 ; i<aStrings.Length() ; i+=1 ) {
		if ( !ExistInIC(aStrings[i]) ) AddCopy(aStrings[i]);
	}
}

void XStrings::AddIACD(const XString &aString)
{
	if ( !ExistInIAC(aString) ) AddCopy(aString);
}

void XStrings::AddIACD(const XStrings &aStrings)
{
  unsigned int i;

	for ( i=0 ; i<aStrings.Length() ; i+=1 ) {
		if ( !ExistInIAC(aStrings[i]) ) AddCopy(aStrings[i]);
	}
}
#endif

//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
// Divers
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx

XStrings Split(const XString &S, const XString &Separator)
{
  XStrings Ss;
  xsize idxB, idxE;

	idxB = 0;
	idxE = S.IdxOf(Separator, idxB);
	while ( idxE != MAX_XSIZE ) {
		Ss.AddCopy(SubString(S, idxB, idxE-idxB));
		idxB = idxE + Separator.Length();
		idxE = S.IdxOf(Separator, idxB);
	}
	if ( idxB < S.Length() ) Ss.AddCopy(SubString(S, idxB, S.Length()-idxB));
	return Ss;
}



#endif
