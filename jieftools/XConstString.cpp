//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
//                                      CONSTSTRING
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx

#if !defined(__XCONSTSTRING_CPP__)
#define __XCONSTSTRING_CPP__

#if defined(__APPLE__)  &&  defined(__OBJC__)
    #import <Foundation/NSString.h>
#endif

//#include "ToolsCommon.h"
#include "XConstString.h"
//#include "XStringW.h"
#include "XBuffer.h"

//XStringW XConstString::wcs() const
//{
//  xsize l = Length();
//  XBuffer xbuf;
////  wchar_t buf[l + 1];
//
//	size_t nbchar = mbstowcs((wchar_t*)xbuf.DataWithSizeMin(0, sizeof(wchar_t)*l), _Data, l);
//	if ( nbchar == (size_t)-1 ) throw("XConstString::wcs() -> Conversion error");
//	return XStringW((wchar_t*)xbuf.Data(), (xsize)nbchar); // overflow ?
//}
//

#endif //__XCONSTSTRING_CPP__
