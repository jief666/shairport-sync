//*************************************************************************************************
//*************************************************************************************************
//
//                                          OBJARRAY
//
//*************************************************************************************************
//*************************************************************************************************

#if !defined(__XOBJARRAY_H__)
#define __XOBJARRAY_H__
//#include "XToolsCommon.h"

#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <errno.h>

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

//template<class TYPE>
/*struct ObjArrayEntry
{
	void *Object;
	bool  FreeIt;
};
*/


template <class TYPE>
class ObjArrayEntry
{
  public:
	TYPE* Object;
	bool  FreeIt;
};

template<class TYPE>
class Dll_IE ObjArrayNC
{
  public:
	ObjArrayEntry<TYPE> *_Data;
	xsize _Len;
	xsize _Size;

  public:
	void Init();
	ObjArrayNC() { Init(); }
	virtual ~ObjArrayNC();

  protected:
	ObjArrayNC(const ObjArrayNC<TYPE> &anObjArrayNC) { throw "pas défini"; }
	const ObjArrayNC<TYPE> &operator =(const ObjArrayNC<TYPE> &anObjArrayNC) { throw "pas défini"; }
	xsize _getLen() const { return _Len; }

  public:
	xsize Size() const { return _Size; }
	xsize Length() const { return _Len; }

	bool NotNull() const { return Length() > 0; }
	bool IsNull() const { return Length() == 0; }

	const TYPE &ElementAt(xsize nIndex) const;
	TYPE &ElementAt(xsize nIndex);
	
	#ifdef _DEBUG
		const TYPE *DbgAt(int i) const { if ( i >= 0 && (xsize)i < _Len ) return &ElementAt ((xsize) i); else return NULL; }
	#endif

	const TYPE &operator[](xsize nIndex) const { return ElementAt(nIndex); }
	TYPE &operator[](xsize nIndex) { return ElementAt(nIndex); }

//	xsize AddRef(TYPE *newElement, bool FreeIt = false);
	xsize AddRef(TYPE *newElement, bool FreeIt); // enlevé le paramËtre par défaut -> source d'erreur

//	xsize InsertRef(TYPE *newElement, xsize pos, bool FreeIt = false);
	xsize InsertRef(TYPE *newElement, xsize pos, bool FreeIt);

	void SetFreeIt(xsize nIndex, bool Flag);
	void SetFreeIt(const TYPE *Element, bool Flag);

	void Remove(const TYPE *Element);
	void RemoveWithoutFreeing(const TYPE *Element);
	void Remove(const TYPE &Element);
	void Remove(xsize nIndex);
	void RemoveWithoutFreeing(xsize nIndex);
	//void Remove(int nIndex);
	void RemoveAllBut(const TYPE *Element);

	void Empty();

  public:
	void CheckSize(xsize nNewSize, xsize nGrowBy = XARRAY_GROW_BY_DEFAULT);

};

template<class TYPE>
class Dll_IE ObjArray : public ObjArrayNC<TYPE>
{
  public:
	ObjArray() : ObjArrayNC<TYPE>() {}
	ObjArray(const ObjArray<TYPE> &anObjArray);
	const ObjArray<TYPE> &operator =(const ObjArray<TYPE> &anObjArray);

	xsize AddCopy(const TYPE &newElement, bool FreeIt = true);
	xsize AddCopies(const TYPE &n1, bool FreeIt = true);
	xsize AddCopies(const TYPE &n1, const TYPE &n2, bool FreeThem = true);
	xsize AddCopies(const TYPE &n1, const TYPE &n2, const TYPE &n3, bool FreeThem = true);
	xsize AddCopies(const TYPE &n1, const TYPE &n2, const TYPE &n3, const TYPE &n4, bool FreeThem = true);
	xsize AddCopies(const TYPE &n1, const TYPE &n2, const TYPE &n3, const TYPE &n4, const TYPE &n5, bool FreeThem = true);
	xsize AddCopies(const TYPE &n1, const TYPE &n2, const TYPE &n3, const TYPE &n4, const TYPE &n5, const TYPE &n6, bool FreeThem = true);
	xsize AddCopies(const TYPE &n1, const TYPE &n2, const TYPE &n3, const TYPE &n4, const TYPE &n5, const TYPE &n6, const TYPE &n7, bool FreeThem = true);
	xsize AddCopies(const TYPE &n1, const TYPE &n2, const TYPE &n3, const TYPE &n4, const TYPE &n5, const TYPE &n6, const TYPE &n7, const TYPE &n8, bool FreeThem = true);
	xsize AddCopies(const TYPE &n1, const TYPE &n2, const TYPE &n3, const TYPE &n4, const TYPE &n5, const TYPE &n6, const TYPE &n7, const TYPE &n8, const TYPE &n9, bool FreeThem = true);
	xsize AddCopies(const TYPE &n1, const TYPE &n2, const TYPE &n3, const TYPE &n4, const TYPE &n5, const TYPE &n6, const TYPE &n7, const TYPE &n8, const TYPE &n9, const TYPE &n10, bool FreeThem = true);
	xsize AddCopies(const TYPE &n1, const TYPE &n2, const TYPE &n3, const TYPE &n4, const TYPE &n5, const TYPE &n6, const TYPE &n7, const TYPE &n8, const TYPE &n9, const TYPE &n10, const TYPE &n11, bool FreeThem = true);
	xsize AddCopies(const TYPE &n1, const TYPE &n2, const TYPE &n3, const TYPE &n4, const TYPE &n5, const TYPE &n6, const TYPE &n7, const TYPE &n8, const TYPE &n9, const TYPE &n10, const TYPE &n11, const TYPE &n12, bool FreeThem = true);
	xsize AddCopies(const TYPE &n1, const TYPE &n2, const TYPE &n3, const TYPE &n4, const TYPE &n5, const TYPE &n6, const TYPE &n7, const TYPE &n8, const TYPE &n9, const TYPE &n10, const TYPE &n11, const TYPE &n12, const TYPE &n13, bool FreeThem = true);
	xsize AddCopies(const TYPE &n1, const TYPE &n2, const TYPE &n3, const TYPE &n4, const TYPE &n5, const TYPE &n6, const TYPE &n7, const TYPE &n8, const TYPE &n9, const TYPE &n10, const TYPE &n11, const TYPE &n12, const TYPE &n13, const TYPE &n14, bool FreeThem = true);
	//TYPE &       AddNew(bool FreeIt = true);

	xsize InsertCopy(const TYPE &newElement, xsize pos);

};

//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
//
//                                          ObjArray
//
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx

//#ifndef DllIm // Si on a importé cette class, ne pas définir les méthodes

/* Constructeur */
template<class TYPE>
void ObjArrayNC<TYPE>::Init()
{
	_Data = NULL;
	_Size = 0;
	_Len = 0;
	#ifdef _DEBUG
	{
		const TYPE *tmp;
		tmp = DbgAt(0);
	}
	#endif
}

/* Constructeur */
template<class TYPE>
ObjArray<TYPE>::ObjArray(const ObjArray<TYPE> &anObjArray)
{
  xsize ui;

  	ObjArrayNC<TYPE>::Init();
	this->CheckSize(anObjArray.Length(), (xsize)0);
	for ( ui=0 ; ui<anObjArray.Length() ; ui+=1 ) AddCopy(anObjArray.ElementAt(ui));
}

/* operator = */
template<class TYPE>
const ObjArray<TYPE> &ObjArray<TYPE>::operator =(const ObjArray<TYPE> &anObjArray)
{
  xsize ui;

  	ObjArrayNC<TYPE>::Empty();
	CheckSize(anObjArray.Length(), 0);
	for ( ui=0 ; ui<anObjArray.Length() ; ui+=1 ) AddCopy(anObjArray.ElementAt(ui));
	return *this;
}

/* Destructeur */
template<class TYPE>
ObjArrayNC<TYPE>::~ObjArrayNC()
{
//printf("ObjArray Destructor\n");
	Empty();
	if ( _Data ) free(_Data);
}

#define PRINTF_FORMAT "ObjArrayNC<TYPE>::CheckSize(xsize="  XSIZE_PRINTF_FORMAT  ", xsize=" XSIZE_PRINTF_FORMAT ") -> realloc errno=%d\n"

/* CheckSize() */
template<class TYPE>
void ObjArrayNC<TYPE>::CheckSize(xsize nNewSize, xsize nGrowBy)
{
	if ( _Size < nNewSize ) {
		nNewSize += nGrowBy + 1;
		_Data = (ObjArrayEntry<TYPE> *)realloc((void *)_Data, sizeof(ObjArrayEntry<TYPE>) * nNewSize );
		if ( !_Data ) {
//#define PASTER(x,y) #x ## X ## #y
//#define EVALUATOR(x,y)  PASTER(x,y)
//
//#define STRINGIZE_NX(A) #A
//#define STRINGIZE(A) STRINGIZE_NX(A)
//
//#define PRINTF_FORMAT STRINGIZE("ObjArrayNC<TYPE>::CheckSize(xsize="  XSIZE_PRINTF_FORMAT  ", xsize=%lu) -> realloc errno=%d\n")
//#define PRINTF_FORMAT EVALUATOR("ObjArrayNC<TYPE>::CheckSize(xsize=","%lu")
			printf(PRINTF_FORMAT, nNewSize, nGrowBy, errno);
			throw 0xC0000017;
		}
//		memset(&_Data[_Size], 0, (nNewSize-_Size) * sizeof(ObjArrayEntry<TYPE>));
		_Size = nNewSize;
	}
}

/* ElementAt() */
template<class TYPE>
TYPE &ObjArrayNC<TYPE>::ElementAt(xsize index)
{
	#ifdef _DEBUG
		if ( index >= _Len ) {
			throw "ObjArray<TYPE>::ElementAt(xsize) -> operator []  -  index greater than length";
		}
	#endif
	return  *((TYPE *)(_Data[index].Object));
}

/* ElementAt() */
template<class TYPE>
const TYPE &ObjArrayNC<TYPE>::ElementAt(xsize index) const
{
	#ifdef _DEBUG
		if ( index >= _Len ) {
			throw "ObjArray<TYPE>::ElementAt(xsize) const -> operator []  -  index greater than length";
		}
	#endif
	return  *((TYPE *)(_Data[index].Object));
}

///* Add() */
//template<class TYPE>
//TYPE &ObjArray<TYPE>::AddNew(bool FreeIt)
//{
//	ObjArrayNC<TYPE>::CheckSize(ObjArray<TYPE>::_getLen()+1);
//	ObjArray<TYPE>::_Data[ObjArray<TYPE>::_Len].Object = new TYPE;
//	ObjArray<TYPE>::_Data[ObjArray<TYPE>::_Len].FreeIt = FreeIt;
//	ObjArray<TYPE>::_Len += 1;
//	return *((TYPE *)(ObjArray<TYPE>::_Data[ObjArray<TYPE>::_Len-1].Object));
//}

/* Add(TYPE &, xsize) */
template<class TYPE>
xsize ObjArray<TYPE>::AddCopy(const TYPE &newElement, bool FreeIt)
{
	ObjArrayNC<TYPE>::CheckSize(ObjArray<TYPE>::_Len+1);
	ObjArray<TYPE>::_Data[ObjArray<TYPE>::_Len].Object = new TYPE(newElement);
	ObjArray<TYPE>::_Data[ObjArray<TYPE>::_Len].FreeIt = FreeIt;
	ObjArray<TYPE>::_Len += 1;
	return ObjArray<TYPE>::_Len-1;
}

template<class TYPE>
xsize ObjArray<TYPE>::AddCopies(const TYPE &n1, bool FreeIt)
{
	return AddCopy(n1, FreeIt);
}

template<class TYPE>
xsize ObjArray<TYPE>::AddCopies(const TYPE &n1, const TYPE &n2, bool FreeThem)
{
	xsize ui = AddCopies(n1, FreeThem);
	AddCopy(n2, FreeThem);
	return ui;
}

template<class TYPE>
xsize ObjArray<TYPE>::AddCopies(const TYPE &n1, const TYPE &n2, const TYPE &n3, bool FreeThem)
{
	xsize ui = AddCopies(n1, n2, FreeThem);
	AddCopy(n3, FreeThem);
	return ui;
}

template<class TYPE>
xsize ObjArray<TYPE>::AddCopies(const TYPE &n1, const TYPE &n2, const TYPE &n3, const TYPE &n4, bool FreeThem)
{
	xsize ui = AddCopies(n1, n2, n3, FreeThem);
	AddCopy(n4, FreeThem);
	return ui;
}

template<class TYPE>
xsize ObjArray<TYPE>::AddCopies(const TYPE &n1, const TYPE &n2, const TYPE &n3, const TYPE &n4, const TYPE &n5, bool FreeThem)
{
	xsize ui = AddCopies(n1, n2, n3, n4, FreeThem);
	AddCopy(n5, FreeThem);
	return ui;
}

template<class TYPE>
xsize ObjArray<TYPE>::AddCopies(const TYPE &n1, const TYPE &n2, const TYPE &n3, const TYPE &n4, const TYPE &n5, const TYPE &n6, bool FreeThem)
{
	xsize ui = AddCopies(n1, n2, n3, n4, n5, FreeThem);
	AddCopy(n6, FreeThem);
	return ui;
}

template<class TYPE>
xsize ObjArray<TYPE>::AddCopies(const TYPE &n1, const TYPE &n2, const TYPE &n3, const TYPE &n4, const TYPE &n5, const TYPE &n6, const TYPE &n7, bool FreeThem)
{
	xsize ui = AddCopies(n1, n2, n3, n4, n5, n6, FreeThem);
	AddCopy(n7, FreeThem);
	return ui;
}

template<class TYPE>
xsize ObjArray<TYPE>::AddCopies(const TYPE &n1, const TYPE &n2, const TYPE &n3, const TYPE &n4, const TYPE &n5, const TYPE &n6, const TYPE &n7, const TYPE &n8, bool FreeThem)
{
	xsize ui = AddCopies(n1, n2, n3, n4, n5, n6, n7, FreeThem);
	AddCopy(n8, FreeThem);
	return ui;
}

template<class TYPE>
xsize ObjArray<TYPE>::AddCopies(const TYPE &n1, const TYPE &n2, const TYPE &n3, const TYPE &n4, const TYPE &n5, const TYPE &n6, const TYPE &n7, const TYPE &n8, const TYPE &n9, bool FreeThem)
{
	xsize ui = AddCopies(n1, n2, n3, n4, n5, n6, n7, n8, FreeThem);
	AddCopy(n9, FreeThem);
	return ui;
}

template<class TYPE>
xsize ObjArray<TYPE>::AddCopies(const TYPE &n1, const TYPE &n2, const TYPE &n3, const TYPE &n4, const TYPE &n5, const TYPE &n6, const TYPE &n7, const TYPE &n8, const TYPE &n9, const TYPE &n10, bool FreeThem)
{
	xsize ui = AddCopies(n1, n2, n3, n4, n5, n6, n7, n8, n9, FreeThem);
	AddCopy(n10, FreeThem);
	return ui;
}

template<class TYPE>
xsize ObjArray<TYPE>::AddCopies(const TYPE &n1, const TYPE &n2, const TYPE &n3, const TYPE &n4, const TYPE &n5, const TYPE &n6, const TYPE &n7, const TYPE &n8, const TYPE &n9, const TYPE &n10, const TYPE &n11, bool FreeThem)
{
	xsize ui = AddCopies(n1, n2, n3, n4, n5, n6, n7, n8, n9, n10, FreeThem);
	AddCopy(n11, FreeThem);
	return ui;
}

template<class TYPE>
xsize ObjArray<TYPE>::AddCopies(const TYPE &n1, const TYPE &n2, const TYPE &n3, const TYPE &n4, const TYPE &n5, const TYPE &n6, const TYPE &n7, const TYPE &n8, const TYPE &n9, const TYPE &n10, const TYPE &n11, const TYPE &n12, bool FreeThem)
{
	xsize ui = AddCopies(n1, n2, n3, n4, n5, n6, n7, n8, n9, n10, n11, FreeThem);
	AddCopy(n12, FreeThem);
	return ui;
}

template<class TYPE>
xsize ObjArray<TYPE>::AddCopies(const TYPE &n1, const TYPE &n2, const TYPE &n3, const TYPE &n4, const TYPE &n5, const TYPE &n6, const TYPE &n7, const TYPE &n8, const TYPE &n9, const TYPE &n10, const TYPE &n11, const TYPE &n12, const TYPE &n13, bool FreeThem)
{
	xsize ui = AddCopies(n1, n2, n3, n4, n5, n6, n7, n8, n9, n10, n11, n12, FreeThem);
	AddCopy(n13, FreeThem);
	return ui;
}

template<class TYPE>
xsize ObjArray<TYPE>::AddCopies(const TYPE &n1, const TYPE &n2, const TYPE &n3, const TYPE &n4, const TYPE &n5, const TYPE &n6, const TYPE &n7, const TYPE &n8, const TYPE &n9, const TYPE &n10, const TYPE &n11, const TYPE &n12, const TYPE &n13, const TYPE &n14, bool FreeThem)
{
	xsize ui = AddCopies(n1, n2, n3, n4, n5, n6, n7, n8, n9, n10, n11, n12, n13, FreeThem);
	AddCopy(n14, FreeThem);
	return ui;
}

/* Add(TYPE *, xsize) */
template<class TYPE>
xsize ObjArrayNC<TYPE>::AddRef(TYPE *newElement, bool FreeIt)
{
	ObjArrayNC<TYPE>::CheckSize(ObjArrayNC<TYPE>::_Len+1);
	ObjArrayNC<TYPE>::_Data[ObjArrayNC<TYPE>::_Len].Object = newElement;
	ObjArrayNC<TYPE>::_Data[ObjArrayNC<TYPE>::_Len].FreeIt = FreeIt;
	ObjArrayNC<TYPE>::_Len += 1;
	return ObjArrayNC<TYPE>::_Len-1;
}

/* Insert(TYPE &, xsize) */
template<class TYPE>
xsize ObjArray<TYPE>::InsertCopy(const TYPE &newElement, xsize pos)
{
	if ( pos  < ObjArray<TYPE>::_Len ) {
		ObjArrayNC<TYPE>::CheckSize(ObjArray<TYPE>::_Len+1);
		memmove(&ObjArray<TYPE>::_Data[pos+1], &ObjArray<TYPE>::_Data[pos], (ObjArray<TYPE>::_Len-pos)*sizeof(ObjArrayEntry<TYPE>));
		ObjArray<TYPE>::_Data[pos].Object = new TYPE(newElement);
		ObjArray<TYPE>::_Data[pos].FreeIt = true;
		ObjArray<TYPE>::_Len += 1;
		return pos;
	}else{
		return AddCopy(newElement);
	}
}

/* Insert(TYPE &, xsize) */
template<class TYPE>
xsize ObjArrayNC<TYPE>::InsertRef(TYPE *newElement, xsize pos, bool FreeIt)
{
	if ( pos  < ObjArrayNC<TYPE>::_Len ) {
		CheckSize(ObjArrayNC<TYPE>::_Len+1);
		memmove(&ObjArrayNC<TYPE>::_Data[pos+1], &ObjArrayNC<TYPE>::_Data[pos], (ObjArrayNC<TYPE>::_Len-pos)*sizeof(ObjArrayEntry<TYPE>));
		_Data[pos].Object = newElement;
		_Data[pos].FreeIt = FreeIt;
		ObjArrayNC<TYPE>::_Len += 1;
		return pos;
	}else{
		return AddRef(newElement, FreeIt);
	}
}

/* SetFreeIt(xsize, bool) */
template<class TYPE>
void ObjArrayNC<TYPE>::SetFreeIt(xsize nIndex, bool Flag)
{
	if ( nIndex  < ObjArrayNC<TYPE>::_Len )
	{
		ObjArrayNC<TYPE>::_Data[nIndex].FreeIt = Flag;
	}
	else{
		#if defined(_DEBUG)
			throw "ObjArray::SetFreeIt(xsize) -> Impossible\n";
		#endif
	}
}

/* SetFreeIt(const TYPE *Element, bool) */
template<class TYPE>
void ObjArrayNC<TYPE>::SetFreeIt(const TYPE *Element, bool Flag)
{
 xsize i;

	for ( i=0 ; i < ObjArrayNC<TYPE>::_Len ; i+= 1) {
		if ( ((TYPE *)ObjArrayNC<TYPE>::_Data[i].Object) == Element ) {
			SetFreeIt(i, Flag);
			return ;
		}
	}
	#if defined(_DEBUG)
		throw "ObjArray::SetFreeIt(const TYPE *) -> Impossible\n";
	#endif
}

/* Remove(xsize) */
template<class TYPE>
void ObjArrayNC<TYPE>::Remove(xsize nIndex)
{
	if ( nIndex  < ObjArrayNC<TYPE>::_Len )
	{
		if ( _Data[nIndex].FreeIt )
		{
		  TYPE *TmpObject; // Obligé de passer par une variable temporaire pour faire le delete (en BCB 4)

		  TmpObject = (TYPE *)(_Data[nIndex].Object);
			delete TmpObject;
		}
		if ( nIndex<ObjArrayNC<TYPE>::_Len-1 ) memcpy(&_Data[nIndex], &_Data[nIndex+1], (_Len-nIndex-1)*sizeof(ObjArrayEntry<TYPE>));
		_Len -= 1;
		return;
	}
	#if defined(_DEBUG)
		throw "ObjArray::Remove(xsize) -> Impossible\n";
	#endif
}

//-------------------------------------------------------------------------------------------------
//                                               
//-------------------------------------------------------------------------------------------------
/* RemoveWithoutFreeing(xsize) */
template<class TYPE>
void ObjArrayNC<TYPE>::RemoveWithoutFreeing(xsize nIndex)
{
	if ( nIndex  < _Len )
	{
		if ( nIndex<_Len-1 ) memcpy(&_Data[nIndex], &_Data[nIndex+1], (_Len-nIndex-1)*sizeof(ObjArrayEntry<TYPE>));
		_Len -= 1;
		return;
	}
	#if defined(_DEBUG)
		throw "ObjArray::RemoveWithoutFreeing(xsize) -> Impossible\n";
	#endif
}

//-------------------------------------------------------------------------------------------------
//                                               
//-------------------------------------------------------------------------------------------------
/* Remove(int) */
//template<class TYPE>
//void ObjArrayNC<TYPE>::Remove(int nIndex)
//{
//	if ( nIndex >= 0 ) {
//		Remove( (xsize)nIndex ); // Le test de validité de nIndex est fait dans Remove(xsize)
//		return;
//	}
//	#if defined(_DEBUG)
//		throw "ObjArray::Remove(int) -> Impossible (nIndex < 0)\n";
//	#endif
//}

/* Remove(const TYPE &) */
template<class TYPE>
void ObjArrayNC<TYPE>::Remove(const TYPE &Element)
{
  xsize i;

	for ( i=0 ; i<_Len ; i+= 1) {
		if ( *((TYPE *)(_Data[i].Object)) == Element ) {
			Remove(i);
			return ;
		}
	}
	#if defined(_DEBUG)
		throw "ObjArray::Remove(TYPE &) -> Impossible\n";
	#endif
}

//-------------------------------------------------------------------------------------------------
//                                               
//-------------------------------------------------------------------------------------------------
/* Remove(const TYPE *) */
template<class TYPE>
void ObjArrayNC<TYPE>::Remove(const TYPE *Element)
{
  xsize i;

	for ( i=0 ; i<_Len ; i+= 1) {
		if ( ((TYPE *)_Data[i].Object) == Element ) {
			Remove(i);
			return ;
		}
	}
	#if defined(_DEBUG)
		throw "ObjArray::Remove(TYPE *) -> Impossible\n";
	#endif
}

//-------------------------------------------------------------------------------------------------
//                                               
//-------------------------------------------------------------------------------------------------
/* RemoveWithoutFreeing(const TYPE *) */
template<class TYPE>
void ObjArrayNC<TYPE>::RemoveWithoutFreeing(const TYPE *Element)
{
  xsize i;

	for ( i=0 ; i<_Len ; i+= 1) {
		if ( ((TYPE *)_Data[i].Object) == Element ) {
			RemoveWithoutFreeing(i);
			return ;
		}
	}
	#if defined(_DEBUG)
		throw "ObjArray::RemoveWithoutFreeing(TYPE *) -> Impossible\n";
	#endif
}

//-------------------------------------------------------------------------------------------------
//                                               
//-------------------------------------------------------------------------------------------------
template<class TYPE>
void ObjArrayNC<TYPE>::RemoveAllBut(const TYPE *Element)
{
  xsize i;

	for ( i=_Len ; i-- ; ) {
		if ( ((TYPE *)_Data[i].Object) != Element ) {
			Remove(i);
		}
	}
}

/* Empty() */
template<class TYPE>
void ObjArrayNC<TYPE>::Empty()
{
  xsize i;

	if ( _Len > 0 ) {
		for ( i=0 ; i<_Len ; i+= 1) {
			if ( _Data[i].FreeIt )
			{
			  TYPE *TmpObject; // Obligé de passer par une variable temporaire pour faire le delete (en BCB 4)

				TmpObject = (TYPE *)(_Data[i].Object);
				delete TmpObject;
			}
		}
		_Len = 0;
	}
}

#endif
