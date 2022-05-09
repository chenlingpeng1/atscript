// AtsValue.h: interface for the CAtsValue class.
//
//////////////////////////////////////////////////////////////////////
// author: chen lingpeng (陈凌鹏)
// 2021/5/1
// ver:1.8
// 代码免费使用，如果用于商务，请联系作者(905309882@qq.com)
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ATSVALUE_H__BA27B697_5F9B_41D1_9B4F_4742741D696C__INCLUDED_)
#define AFX_ATSVALUE_H__BA27B697_5F9B_41D1_9B4F_4742741D696C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "afx.h"
#include "AtsObj.h"
#include "AtsObjPtr.h"
#include "AtsString.h"

class CAtsObject;
class CAtsEngine;

/////////////////////////////////////////////////
// 值对象
/////////////////////////////////////////////////
class CAtsValue : public CAtsObj  
{
public:
	CAtsValue();
	virtual ~CAtsValue();

	int						m_nType;
	CAtsString				m_strSysData;		// 存放符号等
	bool					m_bError;
	CAtsObjPtr<CAtsObject>	m_pObject;

	static BOOL				IsSystemType( int nType );
	static double			Dec(const CAtsString& val);
	static CAtsString		Repeat(  char cItem , int nCount );
	CAtsString				Hex(int val, int len = -1);
	CAtsString				Bin(int val, int len = -1);
	static BOOL				IsNumType(int nType);
	static int				GetNumType(CAtsString strData);
	BOOL					IsNum();
	BOOL					CmpData( CAtsValue *pValue );
	void					AddParamToFist( CAtsValue *pValue );
	void					AddData( int dData );
	void					MultiplyData( double dData );
	void					DivideData( double dData );
	void					ModeData( int nData );
	void					AddData( CAtsValue *pValue);
	CAtsString				GetStrData();
	double					GetdData();
	int						GetnData();
	void					SetData( LPCTSTR strData  );
	void					SetData( double dData );
	void					SetData( int nData, int nType = -1 );
	void					SetDataEx( CAtsString strData , int nType );
	void					SetData( CAtsObject *pObject );
	void					SetData( CAtsValue *pData );
	void					Clear();

	CAtsValue& operator = ( CAtsValue &Item );

	static void OptimizeByNum(CAtsString &strData);
	static CAtsString PrecisionByNum(CAtsString strData, int nPrecision, BOOL bDelInvalidZero);
	static CAtsString PrecisionByNum( double dData, int nPrecision, BOOL bDelInvalidZero);
};

#endif // !defined(AFX_ATSVALUE_H__BA27B697_5F9B_41D1_9B4F_4742741D696C__INCLUDED_)
