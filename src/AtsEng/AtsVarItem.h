// AtVarItem.h: interface for the CAtsVarItem class.
//
//////////////////////////////////////////////////////////////////////
// author: chen lingpeng (������)
// 2021/5/1
// ver:1.8
// �������ʹ�ã����������������ϵ����(905309882@qq.com)
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ATVARITEM_H__C9A509AF_558D_4B13_8D83_3908B87FE349__INCLUDED_)
#define AFX_ATVARITEM_H__C9A509AF_558D_4B13_8D83_3908B87FE349__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "AtsDef.h"
#include "AtsCommDef.h"
#include "AtsValue.h"
#include "AtsObj.h"
#include "AtsObjPtr.h"

class CAtsObject;

////////////////////////////////////////////////////
// ��������
////////////////////////////////////////////////////
class  CAtsVarItem : public CAtsObj  
{
public:
	CAtsVarItem();
	virtual ~CAtsVarItem();

	DWORD						m_nFlag;
	CAtsString					m_strName;
	CAtsValue					m_vData;
	bool						m_bIsConst;
	
	BOOL		IsObject();
	int			GetVarType();
	void		GetValueData( CAtsValue *pValue );
	CAtsString	GetDataToStr();
	void		SetData( CAtsVarItem *pSItem );
	void		SetData( CAtsValue *pData );
	void		SetValueName( LPCTSTR strNam , int nType );
	CAtsObject *GetValueObjData();

	CAtsVarItem & operator = ( CAtsVarItem &Item );

	static int			GetVarType( CAtsString strType );
	static CAtsString	GetVarTypeName( int nType );
};

#endif // !defined(AFX_ATVARITEM_H__C9A509AF_558D_4B13_8D83_3908B87FE349__INCLUDED_)
