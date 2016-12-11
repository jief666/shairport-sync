//*************************************************************************************************
//
//                                          Buffer
//
//*************************************************************************************************

#if !defined(__XRBUFFER_CPP__)
#define __XRBUFFER_CPP__

#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <errno.h>

//#include "XToolsCommon.h"
#include "XString.h"
#include "XStrings.h"
//#include "XFILE.h"
#include "XRBuffer.h"
//#include "ToolsException.h"

const XRBuffer NullXRBuffer;


//*************************************************************************************************
//
//                                          RBuffer (ConstBuffer)
//
//*************************************************************************************************
void XRBuffer::Initialize(const void *p, xsize length)
{
	_RData = (unsigned char*)p;
	_Len = length;
	_Index = 0;
}

XRBuffer::XRBuffer(const XRBuffer &aXRBuffer, xsize pos, xsize max_length)
{
	if ( pos >= aXRBuffer.Length() ) {
		Initialize(NULL, 0);
	}else{
		if ( max_length > aXRBuffer.Length()-pos ) max_length = aXRBuffer.Length()-pos;
		Initialize(aXRBuffer.Data(pos), max_length);
	}
}
/*
XRBuffer::XRBuffer(const XBuffer &aXBuffer, xsize pos, xsize count)
{
	if ( count > aXBuffer.Length() ) count = aXBuffer.Length();
	_Data = aXBuffer.UCData();
	_Len = count;
	_Index = 0;
}
*/

//-------------------------------------------------------------------------------------------------
//                                               
//-------------------------------------------------------------------------------------------------
bool XRBuffer::Get(void *buf, xsize count)
{
	if ( Length() - Index() >= count ) {
		memcpy(buf, Data(Index()), count);
		SetIndex(Index()+count);
		return true;
	}
	return false;
}

xsize XRBuffer::IdxOf(const XString& aString, xsize Pos) const
{
	if ( aString.Length() > Length()-Pos ) return MAX_XSIZE;
	xsize nb = Length()-aString.Length()+1;
	for ( xsize ui=Pos ; ui<nb ; ui+=1 ) {
		if ( strncmp(CData(ui), aString, aString.Length()) == 0 ) return ui;
	}
	return  MAX_XSIZE;
}

xsize XRBuffer::IdxOf(const XStrings& aStrings, xsize Pos, xsize *n) const
{
  xsize pos;

	for (xsize ui=0 ; ui<aStrings.Length() ; ui+=1 ) {
		pos = IdxOf(aStrings[ui], Pos);
		if ( pos != MAX_XSIZE ) {
			if ( n != NULL ) *n = ui;
			return pos;
		}
	}
	return MAX_XSIZE;
}

size_t XBuffer::Sizeof() const
{
	return sizeof(unsigned int)+Length();
}
/*
bool XRBuffer::WriteToBuf(char *buf, size_t *idx, size_t count) const
{
  unsigned int longueur;

	if ( count-*idx < sizeof(longueur) + Length() ) return false;
	longueur = Length();
	memcpy(buf+*idx, &longueur, sizeof(longueur));
	*idx += sizeof(longueur);
	memcpy(buf+*idx, _Data, Length());
	*idx += Length();
	return true;
}
*/
//void XRBuffer::WriteToFileNameT(const char* FileName) const
//{
//  XFILE f;
//
//	f.OpenT(FileName, "wb");
//	f.WriteT(Data(), Length());
//	f.CloseT();
//}
//
//bool XRBuffer::WriteToFileName(const char* FileName) const
//{
//	try {
//		WriteToFileNameT(FileName);
//		SetLastErrorFlag(false);
//	}
//	StdCatch();
//	return !LastErrorFlag();
//}
//
//bool XRBuffer::WriteToFILE(FILE *fp) const
//{
//  xsize longueur;
//
//	longueur = Length();
//	if ( fwrite(&longueur, sizeof(longueur), 1, fp) != 1 ) return false;
//	if ( longueur > 0  &&  fwrite(Data(), longueur, 1, fp) != 1 ) return false;
//	return true;
//}
//
//void XRBuffer::WriteToXFILET(XFILE *f) const
//{
//  xsize longueur;
//
//	longueur = Length();
//	f->WriteT(&longueur, sizeof(longueur));
//	if ( longueur > 0 ) f->WriteT(Data(), longueur);
//}
//
//bool XRBuffer::WriteToXFILE(XFILE *f) const
//{
//	try {
//		WriteToXFILET(f);
//		SetLastErrorFlag(false);
//	}
//	StdCatch();
//	return !LastErrorFlag();
//}

#ifdef __DEVTOOLS_SOCKETS__
void XRBuffer::WriteToSOCKETT(SOCKET Sock, unsigned int TO,const char *ErrMsg) const
{
  unsigned int longueur;

	longueur = Length();
	SockSendT(Sock, &longueur, sizeof(longueur), TO, ErrMsg);
	if ( longueur > 0 ) SockSendT(Sock, Data(), longueur, TO, ErrMsg);
}

bool XRBuffer::WriteToSOCKET(SOCKET Sock, unsigned int TO,const char *ErrMsg) const
{
	try
	{
		WriteToSOCKETT(Sock, TO, ErrMsg);
		SetLastErrorFlag(false);
	}
	StdCatch();
	return !LastErrorMsg();
}
#endif

#endif
