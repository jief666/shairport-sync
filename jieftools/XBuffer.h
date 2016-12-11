//*************************************************************************************************
//*************************************************************************************************
//
//                                          BUFFER
//
//*************************************************************************************************
//*************************************************************************************************
#if !defined(__XBUFFER_H__)
#define __XBUFFER_H__

#include <stdio.h> // for FILE


#if defined(NO_XBUFFER)
	#error "Inclure XBuffer.h avant XString.h"
#endif


//#include "XToolsCommon.h"
#include "XRBuffer.h"


#ifndef XBUFFER_GROW_BY_DEFAULT
#define XBUFFER_GROW_BY_DEFAULT 512
#endif

#define XBuffer_Super XRBuffer
class Dll_IE XBuffer : public XBuffer_Super
{
  protected:
	unsigned char *_WData; // pointe vers la même zone mémoire que _RData (voir XRBuffer)
	xsize _Size;

	void Initialize(const unsigned char* p, xsize count, xsize index);
  public:
	XBuffer();
	XBuffer(XRBuffer &aBuffer, xsize pos, xsize count);
	XBuffer(XBuffer &aBuffer, xsize pos, xsize count);
	XBuffer(void *p, xsize count);
	const XBuffer &operator =(const XRBuffer &aBuffer);
	const XBuffer &operator =(const XBuffer &aBuffer);

	~XBuffer();

  public:
	void CheckSize(xsize nNewSize, xsize nGrowBy = XBUFFER_GROW_BY_DEFAULT);

	// La red�finition de la m�thode suivante ne devrait pas �tre n�cessaire (je crois !). Et pourtant, si on l'enleve, �a ne compile pas...
	const void *Data(xsize ui=0) const { return XBuffer_Super::Data(ui); }
	void *Data(xsize ui=0) { return _WData+ui; }
	void *DataWithSizeMin(xsize ui, xsize size, xsize nGrowBy=XBUFFER_GROW_BY_DEFAULT) { CheckSize(size, nGrowBy); return Data(ui); }

	char *CData(xsize ui=0) { return (char *)(_WData+ui); }
	char *CDataWithSizeMin(xsize ui, xsize size, xsize nGrowBy=XBUFFER_GROW_BY_DEFAULT) { CheckSize(size, nGrowBy); return CData(ui); }

	unsigned char *UCData(xsize ui=0) { return _WData+ui; }
	void *UCDataWithSizeMin(xsize ui, unsigned int size, xsize nGrowBy=XBUFFER_GROW_BY_DEFAULT) { CheckSize(size, nGrowBy); return UCData(ui); }

	xsize Size() const { return _Size; }
	void SetLength(xsize len) { _Len = len; };

	/* IsNull ? */
	void SetNull() { SetLength(0); };

	/* [] */
	unsigned char& operator [](int i) { return UCData()[i]; } // underflow ? overflow ?
	unsigned char& operator [](xsize i) { return UCData()[i]; }

	void Cpy(const void *buf, xsize len);
	// Cat
	void Cat(const void *buf, xsize len);
	void Cat(char c) { Cat(&c, sizeof(c)); };
	void Cat(unsigned char b) { Cat(&b, sizeof(b)); };
	void Cat(bool b) { Cat(&b, sizeof(b)); };
	void Cat(short s) { Cat(&s, sizeof(s)); };
	void Cat(unsigned short us) { Cat(&us, sizeof(us)); };
	void Cat(int i) { Cat(&i, sizeof(i)); };
	void Cat(unsigned int ui) { Cat(&ui, sizeof(ui)); };
	void Cat(long l) { Cat(&l, sizeof(l)); };
	void Cat(unsigned long ul) { Cat(&ul, sizeof(ul)); };
	void Cat(unsigned long long ull) { Cat(&ull, sizeof(ull)); };
	void Cat(double d) { Cat(&d, sizeof(d)); };
	void Cat(const XConstString &aXConstString);
	void Cat(const XString &aXString);
	void Cat(const XBuffer &unXBuffer) { Cat(unXBuffer.Length()); Cat(unXBuffer.Data(), unXBuffer.Length()); }
	void Delete(unsigned int pos, unsigned int count=1);

	const XBuffer &operator += (const XRBuffer &aBuffer);

	size_t Sizeof() const;
	bool ReadFileName(const char* FileName, unsigned int AddZeros=0);
	bool ReadFromBuf(const char *buf, size_t *idx, xsize count);
	bool ReadFromXBuffer(XRBuffer &unXBuffer);
	bool ReadFromFILE(FILE *fp);

	#ifdef __DEVTOOLS_SOCKETS__
		void ReadFromSOCKETT(SOCKET Sock, unsigned int LenMax, unsigned int TO, unsigned int AddZeros, const char *ErrMsg=NULL);
		void ReadFromSOCKETT(SOCKET Sock, unsigned int LenMax, unsigned int TO, const char *ErrMsg=NULL) { ReadFromSOCKETT(Sock, LenMax, TO, 0, ErrMsg); }
		bool ReadFromSOCKET(SOCKET Sock, unsigned int LenMax, unsigned int TO, unsigned int AddZeros, const char *ErrMsg=NULL);
		bool ReadFromSOCKET(SOCKET Sock, unsigned int LenMax, unsigned int TO, const char *ErrMsg=NULL) { return ReadFromSOCKET(Sock, LenMax, TO, 0, ErrMsg); }
	#endif
  public:

};

#endif
