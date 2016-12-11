//*************************************************************************************************
//
//                                          Buffer
//
//*************************************************************************************************

#if !defined(__XBUFFER_CPP__)
#define __XBUFFER_CPP__

#include <stdlib.h> // for malloc

//#include "XToolsCommon.h"
#include "XConstString.h"
#include "XString.h"
#include "XBuffer.h"
//#include "ToolsException.h"

Dll_IE xsize XBufferGrowByDefault = 128;
const XBuffer NullXBuffer;

//*************************************************************************************************
void XBuffer::Initialize(const unsigned char* p, xsize count, xsize index)
{
	if ( p!=NULL && count>0 )
	{
		_Size = count;
		_WData = (unsigned char*)malloc(_Size);
		if ( !_WData ) throw 0xC0000017;
		memcpy(_WData, p, count);
		_RData = _WData;
		_Len = count;
		_Index = index;
	}
	else{
		_Size = 0;
		_WData = NULL;
		_RData = NULL;
		_Len = 0;
		_Index = 0;
	}
}

XBuffer::XBuffer()
{
	Initialize(NULL, 0, 0);
}

XBuffer::XBuffer(XRBuffer &aXRBuffer, xsize pos, xsize count)
{
	if ( pos >= aXRBuffer.Length() ) {
		Initialize(NULL, 0, 0);
	}else{
		if ( count > aXRBuffer.Length()-pos ) count = aXRBuffer.Length()-pos;
		Initialize(aXRBuffer.UCData(pos), count, aXRBuffer.Index());
	}
}

XBuffer::XBuffer(XBuffer &aXBuffer, xsize pos, xsize count)
{
	if ( pos >= aXBuffer.Length() ) {
		Initialize(NULL, 0, 0);
	}else{
		if ( count > aXBuffer.Length()-pos ) count = aXBuffer.Length()-pos;
		Initialize(aXBuffer.UCData(pos), count, aXBuffer.Index());
	}
}

XBuffer::XBuffer(void *p, xsize count)
{
	Initialize((const unsigned char*)p, count, 0);
}

XBuffer::~XBuffer()
{
	free(_WData);
}

//-------------------------------------------------------------------------------------------------
//                                               operator =, +=
//-------------------------------------------------------------------------------------------------
const XBuffer &XBuffer::operator =(const XRBuffer &aBuffer)
{
//TRACE("Operator =const XBuffer&\n");
	Cpy(aBuffer.Data(), aBuffer.Length());
	SetIndex(aBuffer.Index());
	return *this;
}

const XBuffer &XBuffer::operator =(const XBuffer &aBuffer)
{
//TRACE("Operator =const XBuffer&\n");
	Cpy(aBuffer.Data(), aBuffer.Length());
	SetIndex(aBuffer.Index());
	return *this;
}

const XBuffer &XBuffer::operator +=(const XRBuffer &aBuffer)
{
//TRACE("Operator +=const XBuffer&\n");
	Cat(aBuffer.Data(), aBuffer.Length());
	return *this;
}

//-------------------------------------------------------------------------------------------------
//                                               CheckSize
//-------------------------------------------------------------------------------------------------
void XBuffer::CheckSize(xsize nNewSize, xsize nGrowBy)
{
	if ( _Size < nNewSize )
	{
		nNewSize += nGrowBy;
		_WData = (unsigned char*)realloc(_WData, nNewSize);
		if ( !_WData ) throw 0xC0000017;
		_RData = _WData;
		_Size = nNewSize;
	}
}

//-------------------------------------------------------------------------------------------------
//                                               Cpy, Cat, Delete
//-------------------------------------------------------------------------------------------------
void XBuffer::Cpy(const void *buf, xsize len)
{
	if ( buf && len > 0 ) {
		CheckSize(len, 0);         // GrowBy � 0
		memcpy(Data(), buf, len);
		SetLength(len);
	}
}

void XBuffer::Cat(const void *buf, xsize len)
{
	if ( buf && len > 0 ) {
		CheckSize(Length()+len);
		memcpy(Data(Length()), buf, len);
		SetLength(Length()+len);
	}
}

/* impossible de mettre les m�thodes suivantes en inline dans le .h pour des probl�mes d'interd�pendance */
void XBuffer::Cat(const XConstString &aXConstString)
{
	xsize ui = aXConstString.Length(); // Optimize -> on met la longueur dans une variable car XConstString::Length() utilise strlen
	Cat(ui);
	Cat(aXConstString.Data(), ui);
}

void XBuffer::Cat(const XString &aXString)
{
	Cat(aXString.Length());
	Cat(aXString.Data(), aXString.Length());
};

void XBuffer::Delete(unsigned int pos, unsigned int count)
{
	if ( pos < Length() ) {
		if ( pos + count < Length() ) {
			memmove( _WData+pos, _WData+pos+count, Length()-pos-count);
			SetLength(Length()-count);
		}else{
			SetLength(pos);
		}
	}
}

//-------------------------------------------------------------------------------------------------
//                                              ReadFrom
//-------------------------------------------------------------------------------------------------
//bool XBuffer::ReadFileName(const char* FileName, unsigned int AddZeros)
//{
//  FILE *fp;
//  long int ulen;
//  unsigned int len;
//
//	fp = fopen(FileName, "rb");
//	if ( fp == NULL ) {
//		SetLastErrorMsgf("Impossible d'ouvrir le fichier en lecture '%s'", FileName);
//		goto finally;
//	}
//
//	fseek(fp, 0, 2); // Seek to end of file
//	ulen = ftell(fp);
//	if ( ulen <= 0 ) {
//		SetLastErrorMsgf("Impossible d'avoir la longueur du fichier '%s'", FileName);
//		goto finally;
//	}
//
//	len = (unsigned int)ulen;
//	fseek(fp, 0, 0);
//
//	if ( fread(CDataWithSizeMin(0, len+AddZeros), 1, len, fp) != len ) {
//		SetLastErrorMsgf("Impossible de lire %d octets du fichier '%s'", len, FileName);
//		goto finally;
//	}
//	SetLength(len);
//	SetIndex(0);
//	if ( fclose(fp) != 0 ) {
//		SetLastErrorMsgf("Impossible de fermer les fichier '%s'", FileName);
//		goto finally;
//	}
//	{for ( unsigned int ui=len ; ui<len+AddZeros ; ui+=1 ) *CData(ui) = 0;}
//	return true;
//  finally:
//	if ( fp != NULL ) fclose(fp);
//	SetLength(0);
//	return false;
//
//}

bool XBuffer::ReadFromXBuffer(XRBuffer &unXBuffer)
{
  xsize longueur;
	
	if ( !unXBuffer.GetXSize(&longueur) ) return false;
	if ( !unXBuffer.Get(DataWithSizeMin(0, longueur, 0), longueur) ) return false; // GrowBy (param 3 de DataWithSizeMin est � 0 pour eviter du gachis m�moire
	SetLength(longueur);
	SetIndex(0);
	return true;
}

bool XBuffer::ReadFromBuf(const char *buf, size_t *idx, xsize count)
{
  xsize longueur;

	if ( count-*idx >= sizeof(longueur) ) {
		longueur = *((xsize *)(buf+*idx));
		*idx += sizeof(longueur);
		if (  longueur > 0  &&  count-*idx>=longueur  ) memcpy(DataWithSizeMin(0, longueur, 0), buf+*idx, longueur);
		*idx += longueur;
		SetLength(longueur);
		SetIndex(0);
		return true;
	}else{
		SetNull();
		return false;
	}
}

bool XBuffer::ReadFromFILE(FILE *fp)
{
  unsigned int longueur;

	if ( fread(&longueur, sizeof(longueur), 1, fp) != 1 ) goto fin;
	if ( longueur > 0  &&  fread(DataWithSizeMin(0, longueur, 0), longueur, 1, fp) != 1 ) goto fin;
	SetLength(longueur);
	SetIndex(0);
	return true;
  fin:
	SetNull();
	return false;
}

#ifdef __DEVTOOLS_SOCKETS__
void XBuffer::ReadFromSOCKETT(SOCKET Sock, unsigned int LenMax, unsigned int TO, unsigned int AddZeros, const char *ErrMsg)
{
  unsigned int longueur;

	SockReceiveT(Sock, &longueur, sizeof(longueur), TO, ErrMsg);
	if ( longueur > LenMax ) Throw("Longueur re�ue (%d) sup�rieure � la longueur max (%d)", longueur, LenMax);
	if ( longueur > 0 ) SockReceiveT(Sock, DataWithSizeMin(0, longueur+AddZeros, 0), longueur, TO, ErrMsg);
	SetLength(longueur);
	{for ( unsigned int ui=longueur ; ui<longueur+AddZeros ; ui+=1 ) *CData(ui) = 0;}
	SetIndex(0);
}

bool XBuffer::ReadFromSOCKET(SOCKET Sock, unsigned int LenMax, unsigned int TO, unsigned int AddZeros, const char *ErrMsg)
{
	try
	{
		ReadFromSOCKETT(Sock, LenMax, TO, AddZeros, ErrMsg);
		SetLastErrorFlag(false);
	}
	StdCatch();
	return !LastErrorFlag();
}
#endif

#endif
