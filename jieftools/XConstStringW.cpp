//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
//                                      CONSTSTRING
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx

#if !defined(__XCONSTSTRING_CPP__)
#define __XCONSTSTRING_CPP__

#if defined(__APPLE__)  &&  defined(__OBJC__)
    #import <Foundation/NSString.h>
#endif

#include "ToolsCommon.h"
#include "XConstString.h"
#include "XString.h"
#include "XBuffer.h"

/* Convert to multi byte system.
     To avoid allocating too much memory on the heap, we use an intermediate buffer with the size max (Length() * MB_LEN_MAX).
*/
XString XConstStringW::mbs() const
{
  const xsize len = Length() * MB_LEN_MAX + 1;
  XBuffer xbuf;
//  char buf[len];

	size_t nbchar = wcstombs(xbuf.CDataWithSizeMin(0, len), Data(), len);
	if ( nbchar == (size_t)-1 ) throw("XStringW::mbs() -> Conversion error");
	return XString(xbuf.CData(), (xsize)nbchar); // overflow ?
}


#endif //__XCONSTSTRING_CPP__
