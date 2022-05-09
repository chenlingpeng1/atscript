// AtsValue.cpp: implementation of the CAtsValue class.
//
//////////////////////////////////////////////////////////////////////

#include "AtsValue.h"
#include "AtsCommdef.h"
#include "AtsObject.h"
#include "StringObj.h"
#include "IntObj.h"
#include "DoubleObj.h"
#include "BoolObj.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAtsValue::CAtsValue()
{
	m_bError	= false;
	m_nType		= ATSVAR_TYPE_NONE;
	m_pObject	= NULL;
}

CAtsValue::~CAtsValue()
{
	Clear();
}

void CAtsValue::Clear()
{
	m_bError	= false;
	m_nType		= ATSVAR_TYPE_NONE;
	m_pObject	= NULL;
}

CAtsString CAtsValue::Repeat(  char cItem , int nCount )
{
	int			nNum;
	CAtsString		strString;

	strString = _T("");
	for( nNum = 0 ; nNum < nCount ; nNum++ )
		strString += cItem;

	return strString;
}

//====================================================
// 优化数字形数据 如：3.44000 —> 3.44
//====================================================
void CAtsValue::OptimizeByNum(CAtsString &strData)
{
	int			nPos;
	CAtsString		strTemp;

	strTemp = strData;
	nPos	= strTemp.Find(".");
	if( nPos > 0 )
	{
		while( TRUE )
		{
			if( strTemp.Right(1) == "0" )
				strTemp = strTemp.Left( strTemp.GetLength() - 1 ); 
			else if( strTemp.Right(1) == '.' )
			{
				strTemp = strTemp.Left( strTemp.GetLength() - 1 ); 
				break;
			}
			else
				break;
		}
	}
	strData = strTemp;
}

CAtsString CAtsValue::PrecisionByNum(CAtsString strData, int nPrecision, BOOL bDelInvalidZero)
{
	double		dTemp;
	CAtsString		strFormat;

	dTemp = atof( strData );
	
	strFormat.Format( "%%.%df",nPrecision);
	
	strData.Format( strFormat,dTemp);

	if( bDelInvalidZero )
		OptimizeByNum( strData );

	return strData;
}

CAtsString CAtsValue::PrecisionByNum( double dData, int nPrecision, BOOL bDelInvalidZero)
{
	CAtsString		strFormat;
	CAtsString		strData;
	
	if( bDelInvalidZero )
		strFormat.Format( "%%.%dg",nPrecision);
	else
		strFormat.Format( "%%.%df",nPrecision);

	strData.Format( strFormat,dData);
	return strData;
}

void CAtsValue::SetData( LPCTSTR strData )
{
	m_strSysData	= strData;
	if( m_nType == ATSVAR_TYPE_NONE )
		m_nType = ATSVAR_TYPE_VARIANT;
	SetDataEx( strData , m_nType );
}

void CAtsValue::SetDataEx( CAtsString strData , int nType )
{
	m_nType			= nType;
	if( nType == ATSVAR_TYPE_INT )
	{
		m_pObject = new CIntObj;
		((CIntObj *)(CAtsObject *)m_pObject)->SetData( atoi(strData) );
	}
	else if( nType == ATSVAR_TYPE_DOUBLE )
	{
		m_pObject = new CDoubleObj;
		((CDoubleObj *)(CAtsObject *)m_pObject)->SetData( atof(strData) );
	}
	else if( nType == ATSVAR_TYPE_STRING )
	{
		m_pObject = new CStringObj;
		((CStringObj *)(CAtsObject *)m_pObject)->SetData( strData );
	}
	else if( nType == ATSVAR_TYPE_BOOL )
	{
		m_pObject = new CBoolObj;
		((CBoolObj *)(CAtsObject *)m_pObject)->SetData( atoi(strData) );
	}
	else
		m_strSysData = strData;
}

void CAtsValue::SetData( int nData , int nType )
{
	if( nType != -1 )
		m_nType = nType;
	if( m_nType == ATSVAR_TYPE_INT )
	{
		if( !m_pObject ) m_pObject = new CIntObj;
		((CIntObj *)(CAtsObject *)m_pObject)->SetData( nData );
	}
	else if( m_nType == ATSVAR_TYPE_DOUBLE )
	{
		if( !m_pObject ) m_pObject = new CDoubleObj;
		((CDoubleObj *)(CAtsObject *)m_pObject)->SetData( (double)nData );
	}
	else if( m_nType == ATSVAR_TYPE_BOOL )
	{
		if( !m_pObject ) m_pObject = new CBoolObj;
		((CBoolObj *)(CAtsObject *)m_pObject)->SetData( nData );
	}
	else if( m_nType == ATSVAR_TYPE_STRING )
	{
		CString strData;
		strData.Format("%d", nData );
		if( !m_pObject ) m_pObject = new CStringObj;
		((CStringObj *)(CAtsObject *)m_pObject)->SetData( strData );
	}
	else
		m_strSysData.Format("%d", nData );
}

void CAtsValue::SetData( double dData )
{
	if( m_nType == ATSVAR_TYPE_INT )
	{
		if( !m_pObject ) m_pObject = new CIntObj;
		((CIntObj *)(CAtsObject *)m_pObject)->SetData( (int)dData );
	}
	else if( m_nType == ATSVAR_TYPE_DOUBLE )
	{
		if( !m_pObject ) m_pObject = new CDoubleObj;
		((CDoubleObj *)(CAtsObject *)m_pObject)->SetData( dData );
	}
	else if( m_nType == ATSVAR_TYPE_BOOL )
	{
		if( !m_pObject ) m_pObject = new CBoolObj;
		((CBoolObj *)(CAtsObject *)m_pObject)->SetData( (int)dData );
	}
	else if( m_nType == ATSVAR_TYPE_STRING )
	{
		CString strData;
		strData.Format("%f", dData );
		if( !m_pObject ) m_pObject = new CStringObj;
		((CStringObj *)(CAtsObject *)m_pObject)->SetData( strData );
	}
	else
		m_strSysData.Format("%f", dData );
}

void CAtsValue::SetData( CAtsObject *pObject )
{
	m_nType		= ATSVAR_TYPE_ATSOBJECT;
	m_pObject	= pObject;
}

CAtsString CAtsValue::GetStrData()
{
	CString		strData;

	if ( m_nType == ATSVAR_TYPE_STRING )
	{
		if( m_pObject )
			return m_pObject->ToString();
		else
			return m_strSysData;
	}
	else if( m_pObject )
		return m_pObject->ToString();
	else
		return m_strSysData;
}

int CAtsValue::GetnData()
{
	if( m_pObject )
		return m_pObject->ToInt();
	else
		return atoi(m_strSysData);
}

double CAtsValue::GetdData()
{
	if( m_pObject )
		return m_pObject->ToDouble();
	else
		return atof(m_strSysData);
}

//===============================================
// 增加一个数据,生成参数
// 参数的格式
//   Item1Type|Item1Data<,>Item2Type|Item2Data
//===============================================
void CAtsValue::AddParamToFist( CAtsValue *pValue )
{
	CAtsString		strItem;

	strItem.Format("%d|%s" , pValue->m_nType , pValue->GetStrData() );
	if( m_strSysData.IsEmpty() )
		m_strSysData = strItem;
	else
		m_strSysData = strItem + "<,>" + m_strSysData;
}

//===============================================
// 加一个数据
//===============================================
void CAtsValue::AddData( CAtsValue *pValue )
{
	if( m_nType == ATSVAR_TYPE_STRING )
	{
		if( m_pObject )
			((CStringObj *)(CAtsObject *)m_pObject)->AddData( pValue->GetStrData() );
		else
			m_strSysData = m_strSysData + pValue->GetStrData();
	}
	else if( m_nType == ATSVAR_TYPE_INT )
	{
		if( !m_pObject ) m_pObject = new CIntObj;
		((CIntObj *)(CAtsObject *)m_pObject)->AddData( pValue->GetnData() );
	}
	else if( m_nType == ATSVAR_TYPE_DOUBLE )
	{
		if( !m_pObject ) m_pObject = new CDoubleObj;
		((CDoubleObj *)(CAtsObject *)m_pObject)->AddData( pValue->GetdData() );
	}
}

void CAtsValue::AddData( int dData )
{
	if( m_nType == ATSVAR_TYPE_INT )
	{
		if( !m_pObject ) m_pObject = new CIntObj;
		((CIntObj *)(CAtsObject *)m_pObject)->AddData( dData );
	}
	else if( m_nType == ATSVAR_TYPE_DOUBLE )
	{
		if( !m_pObject ) m_pObject = new CDoubleObj;
		((CDoubleObj *)(CAtsObject *)m_pObject)->AddData( (double)dData );
	}
}

//============================================
// 除一个值
//============================================
void CAtsValue::DivideData( double dData )
{
	if( m_nType == ATSVAR_TYPE_INT )
	{
		if( !m_pObject ) m_pObject = new CIntObj;
		((CIntObj *)(CAtsObject *)m_pObject)->DivideData( dData );
	}
	else if( m_nType == ATSVAR_TYPE_DOUBLE )
	{
		if( !m_pObject ) m_pObject = new CDoubleObj;
		((CDoubleObj *)(CAtsObject *)m_pObject)->DivideData( (double)dData );
	}
}

//============================================
// 乘一个值
//============================================
void CAtsValue::MultiplyData( double dData )
{
	if( m_nType == ATSVAR_TYPE_INT )
	{
		if( !m_pObject ) m_pObject = new CIntObj;
		((CIntObj *)(CAtsObject *)m_pObject)->MultiplyData( dData );
	}
	else if( m_nType == ATSVAR_TYPE_DOUBLE )
	{
		if( !m_pObject ) m_pObject = new CDoubleObj;
		((CDoubleObj *)(CAtsObject *)m_pObject)->MultiplyData( (double)dData );
	}
}

//============================================
// 模一个值
//============================================
void CAtsValue::ModeData( int dData )
{
	if( m_nType == ATSVAR_TYPE_INT )
	{
		if( !m_pObject ) m_pObject = new CIntObj;
		((CIntObj *)(CAtsObject *)m_pObject)->ModeData( dData );
	}
	else if( m_nType == ATSVAR_TYPE_DOUBLE )
	{
		if( !m_pObject ) m_pObject = new CDoubleObj;
		((CDoubleObj *)(CAtsObject *)m_pObject)->ModeData( dData );
	}
}

BOOL CAtsValue::IsNum()
{
	return IsNumType( m_nType );
}

BOOL CAtsValue::IsNumType(int nType)
{
	if( nType == ATSVAR_TYPE_INT    ||
		nType == ATSVAR_TYPE_DOUBLE || 
		nType == ATSVAR_TYPE_BOOL   || 
		nType == ATSVAR_TYPE_VARIANT )
	{
		return true;
	}
	else
		return false;
}

int CAtsValue::GetNumType(CAtsString strData)
{
	if( strData.Find('.') >= 0 )
		return ATSVAR_TYPE_DOUBLE;
	else
		return ATSVAR_TYPE_INT;
}

/*****************************************************************************
 * Klasse:		CAtsExpMgr
 * Funktion:	Dec
 * Parameter:	val			Bin鋜-, Dezimal- oder Hexadezimal-Wert als String
 * Return:		Wert als Double
 * Beispiel:	Dec("1001b")	==> 9
 *				Dec("0x80")		==> 128
 *				Dec("1234")		==> 1234
 *				Dec("12e3")		==> 12000
 *				Dec("12e-3")	==> 0.012
 *
 * Rechnet einen Wert in Stringdarstellung als Bin鋜- (abschlie遝ndes 'b'),
 * Hexadezimal- (f黨rendes '0x') oder Dezimalwert in einen Zahlenwer um.
 ****************************************************************************/
double CAtsValue::Dec(const CAtsString& val)
{
	double ret = 0;
	int i, len = val.GetLength();

	if ((len > 2) && (val.Left(2) == "0x"))
		for (i = 2; i < len; i++)
			ret = 16 * ret + ((val[i] >= 'a') ? val[i] - 'a' + 10 : val[i] - '0');
	else if (val.Right(1) == "b")
		for (i = 0, len--; i < len; i++)
			ret = 2 * ret + ((val[i] == '1') ? 1 : 0);
	else
		ret = atof(val);

	return ret;
}

/*****************************************************************************
 * Klasse:		CAtsExpMgr
 * Funktion:	Bin, Hex
 * Parameter:	val			Zahlenwert
 *				len			Minimale L鋘ge des Bin鋜werts
 * Return:		Wert als Bin鋜- oder Hexadezimalwert in Stringdarstellung
 * Beispiel:	Bin(9)		= "1001"
 *				Bin(9, 8)	= "00001001"
 *				Hex(128)	= "0x80"
 *				Hex(128, 4)	= "0x0080"
 *
 * Berechnet die Bin鋜- bzw. Hexadezimaldarstellung eines Wertes und gibt
 * sie als String zur點k.
 ****************************************************************************/
CAtsString CAtsValue::Bin(int val, int len)
{
	CAtsString		strData;

	ASSERT(len < 50);
	char hilf[50];
	if (len == -1)	return itoa(val, hilf, 2);

	itoa(val, hilf, 2);

	strData = Repeat( '0', len - strlen(hilf) );
	return strData + hilf;
}

CAtsString CAtsValue::Hex(int val, int len)
{
	CAtsString		strData;

	ASSERT(len < 50);
	char hilf[50];
	if (len == -1)	return CAtsString("0x") + itoa(val, hilf, 16);

	itoa(val, hilf, 16);
	
	strData = Repeat( '0', len - strlen(hilf) );
	return "0x" + strData + hilf;
}

//===============================================
// 比较数据
//===============================================
BOOL CAtsValue::CmpData( CAtsValue *pValue )
{
	if( m_nType == ATSVAR_TYPE_INT )
	{
		if( GetnData() == pValue->GetnData() ) return true;
	}
	else if( m_nType == ATSVAR_TYPE_DOUBLE )
	{
		if( GetdData() == pValue->GetdData() ) return true;
	}
	else if( m_nType == ATSVAR_TYPE_STRING )
	{
		if( GetStrData() == pValue->GetStrData() ) return true;
	}
	else if( m_nType == ATSVAR_TYPE_BOOL )
	{
		if( GetnData() == pValue->GetnData() ) return true;
	}

	return false;
}

//===============================================
// 是不是是系统类型
// String int double bool
// 系统类型是进行值传递的
//===============================================
BOOL CAtsValue::IsSystemType( int nType )
{
	if( nType == ATSVAR_TYPE_STRING ||
		nType == ATSVAR_TYPE_INT	||
		nType == ATSVAR_TYPE_BOOL	||
		nType == ATSVAR_TYPE_DOUBLE )
	{
		return true;
	}
	else
		return false;
}

CAtsValue & CAtsValue::operator = ( CAtsValue &Item )
{
	m_nType			= Item.m_nType;
	m_strSysData	= Item.m_strSysData;
	m_bError		= Item.m_bError;

	if( m_nType == ATSVAR_TYPE_STRING )
	{
		if( !m_pObject ) m_pObject = new CStringObj;
		((CStringObj *)(CAtsObject *)m_pObject)->SetData((LPCTSTR)Item.GetStrData() );
	}
	else if( m_nType == ATSVAR_TYPE_INT )
	{
		if( !m_pObject ) m_pObject = new CIntObj;
		((CIntObj *)(CAtsObject *)m_pObject)->SetData( Item.GetnData() );
	}
	else if( m_nType == ATSVAR_TYPE_BOOL )
	{
		if( !m_pObject ) m_pObject = new CBoolObj;
		((CBoolObj *)(CAtsObject *)m_pObject)->SetData( Item.GetnData() );
	}
	else if( m_nType == ATSVAR_TYPE_DOUBLE )
	{
		if( !m_pObject ) m_pObject = new CDoubleObj;
		((CDoubleObj *)(CAtsObject *)m_pObject)->SetData( Item.GetdData() );
	}
	else if( IsSystemType( m_nType ) )
	{
		//if( m_pObject )
		//	m_pObject->CopyData( Item.m_pObject );
	}
	else
		m_pObject = Item.m_pObject;

	return *this;
}

void CAtsValue::SetData( CAtsValue *pData )
{
	m_strSysData	= pData->m_strSysData;
	m_bError		= pData->m_bError;
	if( m_nType == ATSVAR_TYPE_NONE )
		m_nType		= pData->m_nType;
	
	if( m_nType == ATSVAR_TYPE_STRING )
	{
		if( !m_pObject ) m_pObject = new CStringObj;
		((CStringObj *)(CAtsObject *)m_pObject)->SetData((LPCTSTR)pData->GetStrData() );
	}
	else if( m_nType == ATSVAR_TYPE_INT )
	{
		if( !m_pObject ) m_pObject = new CIntObj;
		((CIntObj *)(CAtsObject *)m_pObject)->SetData( pData->GetnData() );
	}
	else if( m_nType == ATSVAR_TYPE_BOOL )
	{
		if( !m_pObject ) m_pObject = new CBoolObj;
		((CBoolObj *)(CAtsObject *)m_pObject)->SetData( pData->GetnData() );
	}
	else if( m_nType == ATSVAR_TYPE_DOUBLE )
	{
		if( !m_pObject ) m_pObject = new CDoubleObj;
		((CDoubleObj *)(CAtsObject *)m_pObject)->SetData( pData->GetdData() );
	}
	else if( IsSystemType( m_nType ) )
	{
		//if( m_pObject )
		//	m_pObject->CopyData( Item.m_pObject );
	}
	else
		m_pObject = pData->m_pObject;
}