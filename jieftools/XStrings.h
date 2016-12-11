//*************************************************************************************************
//*************************************************************************************************
//
//                                          STRINGS
//
//*************************************************************************************************
//*************************************************************************************************

#if !defined(__XSTRINGS_H__)
#define __XSTRINGS_H__

//#include "XToolsCommon.h"
#include "XObjArray.h"
#include "XString.h"

#ifndef Dll_IE
#define Dll_IE
#endif

#ifndef xsize
#define xsize unsigned int
#define MAX_XSIZE UINT_MAX
#define XSIZE_PRINTF_FORMAT "%d"
#endif

#ifndef XARRAY_GROW_BY_DEFAULT
#define XARRAY_GROW_BY_DEFAULT 512
#endif

//#pragma warning(push)
//#pragma warning(disable:4231)
//ObjArray_Dll_IE(XString);
//#pragma warning(pop)

#define XStringsSuper ObjArray<XString>
class Dll_IE XStrings : public XStringsSuper
{
  public:

	XStrings();

	#ifdef NOTESAPI_VERSION
		XStrings(const LNText &TextItem); // ImplÈmentation dans ToolsNotes.h
		const XStrings &operator =(const LNText &TextItem);
		operator LNText() const;
	#endif

	void SetNull() { Empty(); }
	bool IsNull() const { return Length() == 0 ; }
	bool NotNull() const { return Length() > 0 ; }
//	bool operator ()(void) const { return NotNull(); }
//	operator bool() const { return NotNull(); }

#ifdef _DEBUG
	const char *DbgAt(unsigned int nIndex) const { if (nIndex<Length()) return ElementAt(nIndex).Data(); else return "index >= longueur"; };// pour debug
#endif
	XString ConcatAll(XString Separator = ", ", XString Prefix = "", XString Suffix = "") const;

	bool Equal(const XStrings &aStrings) const;
	bool operator ==(const XStrings &aXStrings) const { return Equal(aXStrings); }
//	bool EqualIC(const XStrings &aStrings) const;
	xsize IdxOf(const XString &S) const;
	bool ExistIn(const XString &S) const { return IdxOf(S) != MAX_XSIZE; }
#ifdef NOT_USED_ANYMORE_skqdjfhksqjhfksjqdf
	bool ExistInIC(const XString &S) const;
	bool ExistInIAC(const XString &S) const;
#endif
	bool Same(const XStrings &aStrings) const;
#ifdef NOT_USED_ANYMORE_skqdjfhksqjhfksjqdf
	bool SameIC(const XStrings &aStrings) const;
#endif
	void AddStrings(const char *Val1, ...);

	void AddNoNull(const XString &aString) { if ( aString.NotNull() ) AddCopy(aString); }
	void AddEvenNull(const XString &aString) { AddCopy(aString); }

	void Add(const XString &aString) { AddCopy(aString); }
	void Add(const XStrings &aStrings);

	void AddID(const XString &aString); /* ignore Duplicate */
	void AddID(const XStrings &aStrings); /* ignore Duplicate */

#ifdef NOT_USED_ANYMORE_skqdjfhksqjhfksjqdf
	void AddICD(const XString &aString); /* ignore Case & Duplicate */
	void AddICD(const XStrings &aStrings); /* ignore Case & Duplicate */

	void AddIACD(const XString &aString); /* ignore Accent & Case & Duplicate */
	void AddIACD(const XStrings &aStrings); /* ignore Accent & Case & Duplicate */
#endif

	size_t Sizeof() const;
	bool ReadFromBuf(const char *buf, size_t *idx, size_t count);
	bool ReadFromBuf(const char *buf, size_t count) { size_t idx = 0; return ReadFromBuf(buf, &idx, count); }
	bool WriteToBuf(char *buf, size_t *idx, size_t count) const;
	bool WriteToBuf(char *buf, size_t count) { size_t idx = 0; return WriteToBuf(buf, &idx, count); }
	bool ReadFromFILE(FILE *fp);
	bool WriteToFILE(FILE *fp) const;
	//
	bool ReadFromXBuffer(XRBuffer &unXBuffer);
//	void WriteToXBuffer(XBuffer *unXBuffer) const;
	void CatToXBuffer(XBuffer *unXBuffer) const;

	void ReadFromString(const char* buf);
};

extern Dll_IE const XStrings NullXStrings;
Dll_IE XStrings Split(const XString &S, const XString &Separator = ", ");

#endif
