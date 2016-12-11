//*************************************************************************************************
//*************************************************************************************************
//
//                                          RBUFFER
//
//*************************************************************************************************
//*************************************************************************************************

#if !defined(__XRBUFFER_H__)
#define __XRBUFFER_H__
//#include "XToolsCommon.h"

#include <stddef.h>
#include <limits.h>


#ifndef Dll_IE
#define Dll_IE
#endif

#ifndef xsize
#define xsize unsigned int
#define MAX_XSIZE UINT_MAX
#define XSIZE_PRINTF_FORMAT "%d"
#endif


class XConstStringW;
class XStringW;
class XStringws;


class XConstString;
class XString;
class XStrings;
class XBuffer;
class XFILE;

class Dll_IE XRBuffer
{
  protected:
	const unsigned char *_RData; // on utilise le type unsigned char pour pouvoir ajouter un ofs a _Data
	xsize _Len;
	xsize _Index;

  public:
	void Initialize(const void *p, xsize count);
    XRBuffer() { Initialize(NULL, 0); }
	XRBuffer(const void *p, xsize count) { Initialize(p, count); }
	XRBuffer(const XRBuffer& aXRBuffer, xsize pos, xsize max_length);


//	XRBuffer(const XBuffer &aBuffer, unsigned int pos = 0, unsigned int count = MAX_XSIZE);

  public:

	const void *Data(xsize ui=0) const { return _RData+ui; }
	const char *CData(xsize ui=0) const { return (char *)(_RData+ui); }
	const unsigned char *UCData(xsize ui=0) const { return _RData+ui; }

	xsize Length() const { return _Len; }
	xsize Index() const { return _Index; }
	void SetIndex(xsize Idx) { _Index = Idx; };

	// IsNull ? //
	bool IsNull() const { return Length() == 0 ; }
	bool NotNull() const { return Length() > 0 ; }

	// Cast //
	operator const char *() const { return CData(); }

	// [] //
	unsigned char operator [](int i) const { return UCData()[i]; } // underflow ? overflow ?
	unsigned char operator [](unsigned int i) const { return UCData()[i]; } // overflow ?

	bool Get(void *buf, xsize count);

	bool Get(char *c) { return Get(c, sizeof(*c)); }
	bool Get(unsigned char *b) { return Get(b, sizeof(*b)); }
	bool Get(bool *b) { return Get(b, sizeof(*b)); }
	bool Get(short *s) { return Get(s, sizeof(*s)); }
	bool Get(unsigned short *us) { return Get(us, sizeof(*us)); }
	bool Get(int *i) { return Get(i, sizeof(*i)); };
	bool Get(unsigned int *ui) { return Get(ui, sizeof(*ui)); };
	bool Get(long *l) { return Get(l, sizeof(*l)); };
	bool Get(unsigned long *ul) { return Get(ul, sizeof(*ul)); };
	bool Get(double *d) { return Get(d, sizeof(*d)); };

	bool GetChar(char *c) { return Get(c, sizeof(*c)); }
	bool GetByte(unsigned char *b) { return Get(b, sizeof(*b)); }
	bool GetBool(bool *b) { return Get(b, sizeof(*b)); }
	bool GetUShort(unsigned short *us) { return Get(us, sizeof(*us)); }
	bool GetUInt(unsigned int *ui) { return Get(ui, sizeof(*ui)); };
	bool GetULong(unsigned long *ul) { return Get(ul, sizeof(*ul)); };
	bool GetXSize(xsize *size) { return Get(size, sizeof(*size)); };

	xsize IdxOf(const XString& aString, xsize Pos = 0) const;
	xsize IdxOf(const XStrings& aStrings, xsize Pos = 0, xsize *n = NULL) const;

//	bool WriteToBuf(char *buf, size_t *idx, size_t count) const;
	void WriteToFileNameT(const char* FileName) const;
	bool WriteToFileName(const char* FileName) const;

//	// a supprimer quand tous seras passé avec des XFILE
//	bool WriteToFILE(FILE *fp) const;
//
//	void WriteToXFILET(XFILE *f) const;
//	bool WriteToXFILE(XFILE *f) const;
//	#ifdef __DEVTOOLS_SOCKETS__
//		void WriteToSOCKETT(SOCKET Sock, unsigned int TO, const char *ErrMsg = NULL) const;
//		bool WriteToSOCKET(SOCKET Sock, unsigned int TO, const char *ErrMsg = NULL) const;
//	#endif
};

#endif
