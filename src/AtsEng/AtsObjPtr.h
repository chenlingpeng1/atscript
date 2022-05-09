#ifndef __ATSObjPtr
#define __ATSObjPtr

#include "Atlbase.h"

template <class T>
class _NoAddRefReleaseOnCAtsObjPtr : public T
{
	private:
		long AddRef();
		long Release();
};

/////////////////////////////////////////////
// CAtsObjPtr
// 
// 智能指针，自动AddRef与Release
// _NoAddRefReleaseOnCAtsObjPtr
// 智能指针防止调用AddRef与Release
// Ptr>Release(); 应该为Ptr.Release();由这个类来保证不会出现
/////////////////////////////////////////////
template <class T>
class CAtsObjPtr
{
public:
	typedef T _PtrClass;
	CAtsObjPtr()
	{
		p = NULL;
	}
	CAtsObjPtr(T* lp)
	{
		if ((p = lp) != NULL)
			p->AddRef();
	}
	CAtsObjPtr(const CAtsObjPtr<T>& lp)
	{
		if ((p = lp.p) != NULL)
			p->AddRef();
	}
	~CAtsObjPtr()
	{
		if (p)
			p->Release();
	}
	long Release()
	{
		T* pTemp = p;
		if (pTemp)
		{
			p = NULL;
			return pTemp->Release();
		}

		return 0;
	}
	operator T*() const
	{
		return (T*)p;
	}
	T& operator*() const
	{
		ATLASSERT(p!=NULL);
		return *p;
	}
	//The assert on operator& usually indicates a bug.  If this is really
	//what is needed, however, take the address of the p member explicitly.
//	T** operator&()
//	{
//		ATLASSERT(p==NULL);
//		return &p;
//	}
	_NoAddRefReleaseOnCAtsObjPtr<T>* operator->() const
	{
		ATLASSERT(p!=NULL);
		return (_NoAddRefReleaseOnCAtsObjPtr<T>*)p;
	}
	T* operator=(T* lp)
	{
		if (lp != NULL)
			lp->AddRef();
		if (p)
			p->Release();
		p = lp;
		return lp;
	}
	T* operator=(const CAtsObjPtr<T>& lp)
	{
		if (lp != NULL)
			lp.p->AddRef();
		if (p)
			p->Release();
		p = lp;
		return lp;
	}
	bool operator!() const
	{
		return (p == NULL);
	}
	bool operator<(T* pT) const
	{
		return p < pT;
	}
	bool operator==(T* pT) const
	{
		return p == pT;
	}
	// Compare two objects for equivalence
	bool IsEqualObject(T* pOther)
	{
		if ( p == pOther )
			return true;

		return false;
	}
	void Attach(T* p2)
	{
		if (p)
			p->Release();
		p = p2;
	}
	T* Detach()
	{
		T* pt = p;
		p = NULL;
		return pt;
	}
	T* p;
};

#endif // __ATSObjPtr