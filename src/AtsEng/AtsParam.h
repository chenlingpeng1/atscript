// AtsParam.h: interface for the CAtsParam class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ATSPARAM_H__2C044C18_1E34_481B_9D5B_50692C437EF7__INCLUDED_)
#define AFX_ATSPARAM_H__2C044C18_1E34_481B_9D5B_50692C437EF7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CAtsEngine;
class CAtsSegIns;

#include "AtsDef.h"
#include "AtsValue.h"
#include "AtsVarItem.h"
#include "AtsString.h"

class ATS_API CAtsParam  
{
public:
	CAtsParam();
	virtual ~CAtsParam();

	CAtsVarItem * GetItem( int nIndex );
	void	DetachList();
	BOOL	IsEmpty();
	BOOL	LoadParamData( CAtsString strParams );
	BOOL		DataCmp( int nIndex1 , int nIndex2 );
	CAtsString	GetItemData( int nIndex , BOOL bFact = true );
	void	SetReturnData( LPCTSTR sData );
	void	SetReturnData( int nData );
	void	SetReturnDataEx( int nData , int nType );
	void	SetReturnDataEx( CAtsString strData , int nType );
	void	SetReturnData_Obj( CAtsObject *pObject );

	void	AddItemToHead( CAtsValue *pValue );
	void	SetOutData( int nIndex,  LPCTSTR sData );
	void	SetOutData( int nIndex,  long nData );
	BOOL	SetParamInfo( CAtsString strParamInfo , CAtsEngine *pEngine = NULL );
	CAtsParam & CAtsParam::operator = ( CAtsParam &Item );

	CAtsSegIns				*m_pParentSegIns;
	CAtsEngine				*m_pEng;
	CPtrList				m_listParam;	//变量对象链表: CAtsVarItem
	CAtsValue				m_vReturn;
};

#endif // !defined(AFX_ATSPARAM_H__2C044C18_1E34_481B_9D5B_50692C437EF7__INCLUDED_)
