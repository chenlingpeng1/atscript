// MathObj.cpp: implementation of the CMathObj class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MathObj.h"
#include <math.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

const double ATS_PI	= 3.14159265358979323846;
const double ATS_e	= 2.7182818284590452354;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMathObj::CMathObj()
{
	modus = 'r';
}

CMathObj::~CMathObj()
{

}

/*****************************************************************************
 * Klasse:		CAtsExpMgr
 * Funktion:	ToRad
 * Parameter:	x			Wert in Radian, Altgrad oder Neugrad
 * Return:		Wert in Radian
 *
 * Wandelt einen Werte vom aktuellen Kreismodus in Radian um.
 ****************************************************************************/
double CMathObj::ToRad(double x)
{
	switch (modus)
	{
		case '?':	return x * ATS_PI / 180;
		case 'g':	return x * ATS_PI / 200;
		default:	return x;
	}
}

/*****************************************************************************
 * Klasse:		CAtsExpMgr
 * Funktion:	ToMod
 * Parameter:	x			Wert in Radian
 * Return:		Wert in Radian, Altgrad oder Neugrad
 *
 * Wendelt einen Wert von Radian in einen Wert im aktuellen Kreismodus um.
 ****************************************************************************/
double CMathObj::ToMod(double x)
{
	switch (modus)
	{
	case '?':	return x * 180 / ATS_PI;
	case 'g':	return x * 200 / ATS_PI;
	default:	return x;
	}
}

//===================================================
// 执行 Install 的对象 Function
//===================================================
BOOL CMathObj::DoFun( LPCTSTR sFunName , CAtsParam *pParam , CAtsSegIns *pSegIns)
{
	CString		strTemp;
	CString		strFunName = sFunName;
	BOOL		bRet = false;

	if( strFunName.CompareNoCase( "pow" ) == 0 )
	{
		CString strData1 = pParam->GetItemData(1);
		CString strData2 = pParam->GetItemData(2);
	
		pParam->SetReturnDataEx( pow( atoi(strData1) , atoi(strData2)) , ATSVAR_TYPE_INT );
		return true;
	}
	else if( strFunName.CompareNoCase( "sin" ) == 0 )
	{
		CString strData1 = pParam->GetItemData(1);
	
		pParam->SetReturnDataEx( sin( atoi(strData1) ) , ATSVAR_TYPE_DOUBLE );
		return true;
	}
	else if( strFunName.CompareNoCase( "cos" ) == 0 )
	{
		CString strData1 = pParam->GetItemData(1);
	
		pParam->SetReturnDataEx( cos( atoi(strData1) ) , ATSVAR_TYPE_DOUBLE );
		return true;
	}
	else if( strFunName.CompareNoCase( "cube" ) == 0 )
	{
		double fData = atof( pParam->GetItemData(1) );
	
		pParam->SetReturnDataEx( fData * fData * fData , ATSVAR_TYPE_DOUBLE );
		return true;
	}
	else if( strFunName.CompareNoCase( "abs" ) == 0 )
	{
		double fData = atof( pParam->GetItemData(1) );
	
		pParam->SetReturnDataEx( fabs(fData) , ATSVAR_TYPE_DOUBLE );
		return true;
	}
	else if( strFunName.CompareNoCase( "tan" ) == 0 )
	{
		double fData = atof( pParam->GetItemData(1) );
	
		pParam->SetReturnDataEx( tan(fData) , ATSVAR_TYPE_DOUBLE );
		return true;
	}
	else if( strFunName.CompareNoCase( "acos" ) == 0 )
	{
		double fData = atof( pParam->GetItemData(1) );
	
		pParam->SetReturnDataEx( acos(fData) , ATSVAR_TYPE_DOUBLE );
		return true;
	}
	else if( strFunName.CompareNoCase( "asin" ) == 0 )
	{
		double fData = atof( pParam->GetItemData(1) );
	
		pParam->SetReturnDataEx( ToMod(asin(fData)) , ATSVAR_TYPE_DOUBLE );
		return true;
	}
	else if( strFunName.CompareNoCase( "atan" ) == 0 )
	{
		double fData = atof( pParam->GetItemData(1) );
	
		pParam->SetReturnDataEx( ToMod(atan(fData)) , ATSVAR_TYPE_DOUBLE );
		return true;
	}
	else if( strFunName.CompareNoCase( "exp" ) == 0 )
	{
		double fData = atof( pParam->GetItemData(1) );
	
		pParam->SetReturnDataEx( exp(fData) , ATSVAR_TYPE_DOUBLE );
		return true;
	}
	else if( strFunName.CompareNoCase( "log" ) == 0 )
	{
		double fData = atof( pParam->GetItemData(1) );
	
		pParam->SetReturnDataEx( log(fData) , ATSVAR_TYPE_DOUBLE );
		return true;
	}
	else if( strFunName.CompareNoCase( "log10" ) == 0 )
	{
		double fData = atof( pParam->GetItemData(1) );
	
		pParam->SetReturnDataEx( log10(fData) , ATSVAR_TYPE_DOUBLE );
		return true;
	}
	else if( strFunName.CompareNoCase( "sqr" ) == 0 )
	{
		double fData = atof( pParam->GetItemData(1) );
	
		pParam->SetReturnDataEx( fData * fData , ATSVAR_TYPE_DOUBLE );
		return true;
	}
	else if( strFunName.CompareNoCase( "sqrt" ) == 0 )
	{
		double fData = atof( pParam->GetItemData(1) );
	
		pParam->SetReturnDataEx( sqrt(fData) , ATSVAR_TYPE_DOUBLE );
		return true;
	}

	return false;
}

