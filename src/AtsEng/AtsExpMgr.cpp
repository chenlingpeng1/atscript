
#include "AtsExpMgr.h"
#include "AtsExpValue.h"
#include "AtsObjPtr.h"
#include "AtsCommdef.h"
#include "AtsBaseFun.h"
#include "DoubleObj.h"

const double ATS_PI	= 3.14159265358979323846;
const double ATS_e	= 2.7182818284590452354;

// ��һ��ֵ�Ӷ���ȡ��,�������������
// ���ǲ����,��Ҫ�õ�������
#define	VAL_POP_CHECK_NUM_RETSTR( pvItem )	pvItem = ValPop();  \
		if( !pvItem ) {m_bIsError=true;return false;} \
		if( !TranvItemToData( pvItem ) ) {m_bIsError=true;return false;} \
		if( !pvItem->IsNum() ) {m_bIsError=true;return false;} 

#define	VAL_POP_CHECK_NUM( pvItem )	pvItem = ValPop();  \
		if( !pvItem ) {m_bIsError=true;return false;} \
		if( !TranvItemToData( pvItem ) ) {m_bIsError=true;return false;} \
		if( !pvItem->IsNum() ) {m_bIsError=true;return false;} 

#define	VAL_POP_CHECK_NUM_STR( pvItem )	pvItem = ValPop();  \
		if( !pvItem ) {m_bIsError=true;return false;} \
		if( !TranvItemToData( pvItem ) ) {m_bIsError=true;return false;} \
		if( !pvItem->IsNum() && pvItem->m_nType != ATSVAR_TYPE_STRING )\
		{m_bIsError=true;return false;} 

#define	CHECK_PRVITEM_VALUE(strCurWord)	if( m_nPrvItemType != FITEM_TYPE_VALUE) {m_bIsError=true;m_strErrWord=strCurWord;return false;} 
#define	CHECK_PRVITEM_OP()		if( m_nPrvItemType != FITEM_TYPE_OP ) {m_bIsError=true;return false;} 
#define	CHECK_PRVITEM_OP_NONE()	if( (m_nPrvItemType != FITEM_TYPE_OP) && (m_nPrvItemType != FITEM_TYPE_NONE) ) {m_bIsError=true;return false;} 

//CAtsExpMgr eval;

CAtsExpMgr::CAtsExpMgr()
{
	m_nPrvItemType	= FITEM_TYPE_NONE;
	m_bIsError		= false;
	m_pFEExt		= NULL;
	Reset();

	InitSysFun();
}

CAtsExpMgr::~CAtsExpMgr()
{
	m_listSysFunName.clear();
	DetachList();
}

void CAtsExpMgr::DetachList()
{
	POSITION			pos;
	CAtsExpOpItem		*pItem	= NULL;
	CAtsValue			*pValue = NULL;

	for( pos = m_listOP.GetHeadPosition() ; pos ; )
	{
		pItem = (CAtsExpOpItem *)m_listOP.GetNext( pos );
		delete pItem;
	}
	m_listOP.RemoveAll();

	for( pos = m_listValue.GetHeadPosition() ; pos ; )
	{
		pValue = (CAtsValue *)m_listValue.GetNext( pos );
		delete pValue;
	}
	m_listValue.RemoveAll();
}

/*****************************************************************************
 * Klasse:		CAtsExpMgr
 * Funktion:	ToRad
 * Parameter:	x			Wert in Radian, Altgrad oder Neugrad
 * Return:		Wert in Radian
 *
 * Wandelt einen Werte vom aktuellen Kreismodus in Radian um.
 ****************************************************************************/
double CAtsExpMgr::ToRad(double x)
{
	switch (modus)
	{
		case DEG:	return x * ATS_PI / 180;
		case GON:	return x * ATS_PI / 200;
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
double CAtsExpMgr::ToMod(double x)
{
	switch (modus)
	{
	case DEG:	return x * 180 / ATS_PI;
	case GON:	return x * 200 / ATS_PI;
	default:	return x;
	}
}

/*****************************************************************************
 * Klasse:		CAtsExpMgr
 * Funktion:	Reset
 * Parameter:	-
 * Return:		"Reset"
 *
 * Mit dieser Funktion werden alle gespeicherten Variablen und Funktionen
 * gel�scht und der Ursprungszustand der Klasse wieder hergestellt.
 ****************************************************************************/
CAtsString CAtsExpMgr::Reset()
{
//	var.RemoveAll();
//	usrfn.RemoveAll();

	DetachList();

	out			= DEC;
	modus		= RAD;
	m_bIsError	= false;
	
	return "Reset";
}

//=================================================
// �ж��Ƿ������
//=================================================
bool CAtsExpMgr::TestError()
{
	if( m_bIsError )
		return false;

	return true;
}

//========================================================
// ��������
//========================================================
BOOL CAtsExpMgr::Computer( CAtsString strExpress , CAtsValue *pValue )
{
	CAtsString		strData;
	CAtsValue	*pValueItem = NULL;
	
	if( !CalcExpress( strExpress ) )
		return false;

	// �Ӷ���ȡ����
	if( m_listValue.GetCount() <= 0 )
		return false;
	else
		pValueItem = ValTop();

	// ����Ǳ���,��תΪ����
	if( pValueItem->m_nType == ATSVAR_TYPE_VAR )
	{
		if( !TranvItemToData( pValueItem ) ) 
		{
			m_bIsError=true;
			ASSERT( false );
			return false;
		}
	}

	*pValue = *pValueItem;
	return true;
}

//========================================================
// �ж���������Ƿ����
//========================================================
BOOL CAtsExpMgr::CheckCondition( CAtsString strExpress )
{
	CAtsString		strData;
	CAtsValue	*pValue = NULL;
	
	if( strExpress.IsEmpty() )
		return false;

	CalcExpress( strExpress );

	// �Ӷ���ȡ����
	if( m_listValue.GetCount() <= 0 )
		return false;
	else
		pValue = ValTop();

	// ����Ǳ���,��תΪ����
	if( pValue->m_nType == ATSVAR_TYPE_VAR )
	{
		if( !TranvItemToData( pValue ) ) 
		{
			m_bIsError=true;
			ASSERT( false );
			return false;
		}
	}

	// ������ݲ�Ϊ 0 , ����Ϊ��������
	if( atoi( pValue->GetStrData() ) )
		return true;

	return false;
}

void CAtsExpMgr::CutEndChar(CAtsString &strString, int nCount)
{
	strString = strString.Left( strString.GetLength() - nCount );
}

BOOL CAtsExpMgr::IsNumberVal(CAtsString strValue)
{
	int			nNum,nCount;
	BOOL		bHaveDot;
	UINT		nChar;

	if( strValue.GetLength() <= 0 )
		return false;

	nCount = strValue.GetLength();
	if( nCount < 0 )
		return false;

	bHaveDot = false;
	for( nNum = 0 ; nNum < nCount ; nNum++ )
	{
		nChar = strValue.GetAt( nNum );
		if( nChar < '0' || nChar > '9' )
		{
			if( nChar == '-' && nNum == 0 )
				continue;

			if( nChar != '.' )
				return false;
			else if( bHaveDot )
				return false;
			else
				bHaveDot = true;
		}
	}
	if( strValue == "0" )
		return true;

	if( strValue.GetAt( 0 ) == '0' && !bHaveDot )
		return false;

	if( strValue.GetAt( nCount -1 ) == '.' )
		return false;

	return true;
}

//========================================================================
// ���б���Լ���
//========================================================================
BOOL CAtsExpMgr::CalcExpress(LPCTSTR formel )
{
	int				nPrvPos = 0;
	int				nCurPos = 0;
	int				j = 0;

	m_strData = formel;
	if( m_strData.IsEmpty() )
		return true;

	const int	len = m_strData.GetLength();
	m_nStrLen = len;

	oldValFlag = false;

	CAtsValue		vItem;
	CString			strItemData;
	
	while( GetItem(nCurPos,vItem) )
	{
		strItemData = vItem.GetStrData();
		if (!TestError()) 
			return false;

		valFlag = false;

		if( vItem.IsNum() )
			ValPush( &vItem );
		else if( vItem.m_nType == ATSVAR_TYPE_VAR )
		{
			// ��һ������
			/*if( m_pFEExt )
			{
				CAtsValue		vDataItem;
				m_pFEExt->GetVarValue( vItem.m_strData , &vDataItem );
				if( vDataItem.IsNum() )
					ValPush( vDataItem.m_dData );
				else
					ValPush( vDataItem.m_strData );
			}*/
			// ��ʾֱ�ӷŵ�����,ֻ���ڼ���ʱ�Ű���תΪ����
			// ��Ŀ����Ϊ������Ϊ Function �Ĳ���ʱ����Ҫ��������,Ҫ�� function ֪��
			// ������
			ValPush( strItemData , ATSVAR_TYPE_VAR );
		}
		else if( vItem.m_nType == ATSVAR_TYPE_STRING )
			ValPush( strItemData , ATSVAR_TYPE_STRING );
		else if( vItem.m_nType == ATSVAR_TYPE_OP )
		{
			if( strItemData == ")" )
			{
				CAtsExpOpItem *pTopOP = OpTop();
				if( pTopOP && pTopOP->m_strName == "(" )
				{
					OpPop( true );
			
					// ���������ж���һ���ǲ��� fun , �����,��һ��Ҫ����һ�� value 
					// ��Ϊ function �� Param, �����ǿյ�
					if( OpTop() && OpTop()->m_nType == ATSOP_TYPE_SYSFUN ||
						OpTop() && OpTop()->m_nType == ATSOP_TYPE_EXTFUN )
					{
						// ���ǰһ��ֵ���Ƿ���,�������������Ϊ��
						if( m_nPrvItemType == FITEM_TYPE_OP )
						{
							CAtsValue		vValue;
							vValue.m_nType = ATSVAR_TYPE_PARAM;
							ValPush( &vValue );
						}
					}
				}
				else
				{
					CalTheSegment( CAtsBaseFun::GetOPLevel(ATSOP_TYPE_SEG) );
					pTopOP = OpTop();
					if( pTopOP && pTopOP->m_strName == "," )
					{
						// �ϲ� Fun ����, �ϲ����е� ','
						CAtsValue		vValue;
						CAtsValue		*pTempVal = NULL;

						pTempVal = ValPop();
						if( !pTempVal )
						{
							m_bIsError = true;
							ASSERT( false );
							return false;
						}

						vValue.m_nType = ATSVAR_TYPE_PARAM;
						vValue.AddParamToFist( pTempVal );
						delete pTempVal;
						pTempVal = NULL;
						do
						{
							pTempVal = ValPop();
							if( !pTempVal )
							{
								m_bIsError = true;
								ASSERT( false );
								return false;
							}
							vValue.AddParamToFist( pTempVal );
							OpPop(true);
							delete pTempVal;
						}
						while( OpTop() && OpTop()->m_strName == "," );

						ValPush( &vValue );
					}
					if( OpTop() && OpTop()->m_strName == "(" )
						OpPop(true);
					else
					{
						// ���ǻ����� "(" , �����
						m_bIsError = true;
						ASSERT( false );
						return false;
					}
				}

				// ���������ж���һ���ǲ��� fun 
				if( OpTop() && OpTop()->m_nType == ATSOP_TYPE_SYSFUN ||
					OpTop() && OpTop()->m_nType == ATSOP_TYPE_EXTFUN )
				{
					if( !CalTop() )
					{
						m_bIsError = true;
						ASSERT( false );
						return false;
					}
				}

				// �������ź�Ҫִ�����Ͽ���ִ�е�
				CalMastTop();
			}
			else if( vItem.GetStrData() == "(" )
			{
				CHECK_PRVITEM_OP_NONE();
				OpPush( ATSOP_TYPE_SEG , vItem.GetStrData() );
			}
			else if( vItem.GetStrData() == ",")
			{
				CHECK_PRVITEM_VALUE(vItem.GetStrData() );	// ǰһ��Item һ��Ҫ��value	

				CalTheSegment( CAtsBaseFun::GetOPLevel(ATSOP_TYPE_SEG , vItem.GetStrData() ) );
				OpPush( ATSOP_TYPE_SEG , vItem.GetStrData() );
			}
			else if( strItemData.CompareNoCase("AND") == 0	||
					 strItemData.CompareNoCase("OR") == 0	||
					 strItemData.CompareNoCase("&&") == 0	||
					 strItemData.CompareNoCase("||") == 0	||
					 strItemData == "=="	||
					 strItemData == ">="	||
					 strItemData == ">"		||
					 strItemData == "<="	||
					 strItemData == "<"		||
					 strItemData == "!="	||
					 strItemData == "<>"	||
					 strItemData == "="	)
			{
				CHECK_PRVITEM_VALUE(strItemData);	// ǰһ��Item һ��Ҫ��value	

				CalTheSegment( CAtsBaseFun::GetOPLevel(ATSOP_TYPE_CHECK , strItemData) );
				OpPush( ATSOP_TYPE_CHECK , strItemData);
			}
			else if( strItemData == "+" ||
					 strItemData == "*" ||
					 strItemData == "/" ||
					 strItemData == "%" ||
					 strItemData == "^" ||
					 strItemData == "+=" ||
					 strItemData == "-=" ||
					 strItemData == "*=" ||
					 strItemData == "%=" ||
					 strItemData == "/=" )
				OpPush( ATSOP_TYPE_BINOP , strItemData);
			else if( strItemData == "-" )
			{
				if( m_nPrvItemType == FITEM_TYPE_VALUE )
					OpPush( ATSOP_TYPE_BINOP , strItemData );		// ����
				else
					OpPush(ATSOP_TYPE_SINGLEOP , "-" );				// ����
			}
			else if( strItemData == "&" ||		// lOG �����
					 strItemData == "|" )
				OpPush( ATSOP_TYPE_LOG , strItemData );
			else if( strItemData == "AND"  ||
					 strItemData == "OR"   ||
					 strItemData == "&&"  ||
					 strItemData == "||"   ||
					 strItemData == "=="   ||
					 strItemData == ">="   ||
					 strItemData == ">"    ||
					 strItemData == "<"    ||
					 strItemData == "!="   ||
					 strItemData == "<>"   ||
					 strItemData == "<="   )
				OpPush( ATSOP_TYPE_CHECK , strItemData);
			else if( strItemData == "!" ||		// �����������
					 strItemData == "~"  )
				OpPush( ATSOP_TYPE_SINGLEOP , strItemData);
			else if( strItemData == "++" )
			{
				CAtsObjPtr<CAtsValue>		a;
				VAL_POP_CHECK_NUM_RETSTR( a );
				ValPush( a->GetdData() + 1 );
			}
			else
			{
				// 1. System Funktion
				if( IsSysFunName( strItemData ) )   
				{
					OpPush( ATSOP_TYPE_SYSFUN , strItemData );
					if( !NextVCharIs( nCurPos  , '('))
					{
						ASSERT( false );
						m_bIsError = true;
						return false;
					}
				}
				else
				{
					// ��Ϊ����չ����
					OpPush( ATSOP_TYPE_EXTFUN , strItemData );
					// ����Ǻ���, ��һ����Ч���ű����� '('
					if( !NextVCharIs( nCurPos , '('))
					{
						ASSERT( false );
						m_strErrWord = strItemData;
						m_bIsError = true;
						return false;
					}
				}
	/*??
				//if (fkt.Lookup(strLowerName, j))		{  OpPush(j); break; }
				// 2. Benutzerfunktion
				if (usrfn.Lookup(name, hilf))			{ TxtPush(name); OpPush(STOP); OpPush(USRFN); break; }
				// 3. Zuweisung
				if ((i+1 < len) && (str[i+1] == '='))	{ TxtPush(name); break; }
				// 4. Arithmetische Zuweisung (+=, ..)
				if ((i+2 < len) && (str[i+2] == '='))	{ TxtPush(name); break; }
				// 5. Externe Variable
				if (extVar && extVar->Lookup(name, a))	{ ValPush(a); break; }
				// 6. Interne Variable
				if (var.Lookup(name, a))				{ ValPush(a); break; }
				// 7. Fehler
	*/			
			}
		}
		nPrvPos = nCurPos;
	}	
	if( m_bIsError )
		return false;

	// ���㵽��, �� '(' , ',' ֹͣ
	CalTheSegment(CAtsBaseFun::GetOPLevel(ATSOP_TYPE_SEG));

	if( m_bIsError )
		return false;

	return true;
}

CAtsString CAtsExpMgr::GetErrorInfo()
{
	return err;
}

BOOL CAtsExpMgr::IsValidVarChar(char cChar)
{
	// ���� A �C Z, a �C z, or 0 �C 9
	if( isalnum(cChar) )
		return true;
	
	//�Ǻ���
	if( cChar & 0x80 ) 
		return true;
		
	if( cChar == ']' || cChar == '[' )
		return true;

	if( cChar == '@' )
		return true;

	if( cChar == '_' )
		return true;

	return false;
}

void CAtsExpMgr::RemoveAllSubStr(CAtsString &strString, CAtsString strItem ) 
{
	return;
	int i = strString.Find( strItem );
	while (i >= 0)
	{
		strString	=  strString.Left(i) + strString.Mid( i + strItem.GetLength() );
		i			= strString.Find( strItem );
	}
}

//=========================================
// ȡ�����Ŷѵ�����һ��
//=========================================
CAtsExpOpItem * CAtsExpMgr::OpPop(BOOL bAutoDel)					
{ 
	if( m_listOP.GetCount() <= 0 )
		return NULL;

	CAtsExpOpItem	*pItem = (CAtsExpOpItem *)m_listOP.GetHead();
	m_listOP.RemoveHead();

	if( bAutoDel )
	{
		delete pItem;
		return NULL;
	}
	else
		return pItem;
}

//=========================================
// �õ����Ŷѵ�����һ��
//=========================================
CAtsExpOpItem * CAtsExpMgr::OpTop()					
{ 
	if( m_listOP.GetCount() <= 0 )
		return NULL;

	return (CAtsExpOpItem *)m_listOP.GetHead();
}

//=======================================
// ����м���һ������
//=======================================
void CAtsExpMgr::OpPush(int nType , CAtsString strName , int nPos )
{
	CAtsExpOpItem *pItem  = new CAtsExpOpItem;
	pItem->m_nType		= nType;
	pItem->m_strName	= strName;
	pItem->m_nPos		= nPos;

	// ����ӽ����Ĳ����������еķ���,��ǰһ����˫��������
	// ��������ǰһ��
	if( pItem->m_nType == ATSOP_TYPE_BINOP && !pItem->IsMastCal() )
	{
		if( OpTop() && OpTop()->m_nType == ATSOP_TYPE_BINOP )
			CalTop();
	}

	m_nPrvItemType = FITEM_TYPE_OP;
	m_listOP.AddHead( pItem );
}

//=======================================
// ����зŻ�ԭ���� OP ����һ������
// ���Բ��޸� m_nPrvItemType
//=======================================
void CAtsExpMgr::OpPush_OldOP(CAtsExpOpItem *pOPItem)
{
	m_listOP.AddHead( pOPItem );
}

//=======================================
// ����м���һ������
//=======================================
void CAtsExpMgr::OpPush(int oper)		
{ 
	CAtsExpOpItem *pItem  = new CAtsExpOpItem;
	pItem->m_nType   = ATSOP_TYPE_BINOP;
	pItem->m_strName = (char)oper;

	// ����ӽ����Ĳ����������еķ���,��ǰһ����˫��������
	// ��������ǰһ��
	if( pItem->m_nType == ATSOP_TYPE_BINOP && !pItem->IsMastCal() )
	{
		if( OpTop() && OpTop()->m_nType == ATSOP_TYPE_BINOP )
			CalTop();
	}

	m_nPrvItemType = FITEM_TYPE_OP;
	m_listOP.AddHead( pItem );					
}

//=======================================
// ����м���һ��ֵ
//=======================================
void CAtsExpMgr::ValPush(double x)		
{ 
	CAtsValue		*pItem = new CAtsValue;

	pItem->m_nType		= ATSVAR_TYPE_DOUBLE;
	pItem->m_pObject	= new CDoubleObj;
	((CDoubleObj *)(CAtsObject *)pItem->m_pObject)->SetData( x ); 

	m_nPrvItemType = FITEM_TYPE_VALUE;
	m_listValue.AddHead( pItem );
	CalMastTop();	
}

//=======================================
// ����м���һ��ֵ
//=======================================
void CAtsExpMgr::ValPush(CAtsString strData, int nType )		
{ 
	CAtsValue		*pItem = new CAtsValue;

	pItem->m_nType		= nType;
	pItem->m_strSysData	= strData;

	m_nPrvItemType = FITEM_TYPE_VALUE;
	m_listValue.AddHead( pItem );
	CalMastTop();	
}

//=======================================
// ����м���һ��ֵ
//=======================================
void CAtsExpMgr::ValPush(CAtsValue *pValue)		
{ 
	CAtsValue		*pItem = new CAtsValue;

	*pItem = *pValue;
	m_nPrvItemType = FITEM_TYPE_VALUE;
	m_listValue.AddHead( pItem );
	CalMastTop();	
}

//=========================================
// ȡ�����ݶѵ�����һ��
//=========================================
CAtsValue * CAtsExpMgr::ValPop()					
{ 
	if( m_listValue.GetCount() <= 0 )
		return NULL;

	CAtsValue	*pItem = (CAtsValue *)m_listValue.GetHead();
	m_listValue.RemoveHead();
	return pItem;
}

//=========================================
// �õ����ݶѵ�����һ��
//=========================================
CAtsValue * CAtsExpMgr::ValTop()					
{ 
	if( m_listValue.GetCount() <= 0 )
		return NULL;

	return (CAtsValue *)m_listValue.GetHead();
}

//=====================================
// ִ�����Ͽ���ִ�е�����, 1 �������
// * , / , fun
//=====================================
void CAtsExpMgr::CalMastTop()		
{
	if( OpTop() && OpTop()->IsMastCal() )
	{
		if( CalTop(1) )
			CalMastTop();
	}
}

//=====================================
// һֱ���㵽 '(' , ',' Ϊ��
//=====================================
void CAtsExpMgr::CalTheSegment(int nCurOPLevel)		
{ 
	while( CalTop( nCurOPLevel ) )
		;
}

//================================================
// ��������һ�����м���
// �� '(' , "," ʱ��ͣ��,��Ϊ����û���� '(' , ','
//================================================
bool CAtsExpMgr::CalTop(int nCurOPLevel)		
{ 
	CAtsObjPtr<CAtsValue>		a, b;	// �� CAtsObjPtr �ĺô����ǻ��Զ��ſ�ָ��
	CAtsExpOpItem				*pOP	= NULL;
	char						c;
	BOOL						bCal = false;

	if( m_bIsError )
		return 0;

	pOP = OpPop();
	if( !pOP )
		return false;

	if( nCurOPLevel > 0 && pOP->GetOPLevel() > nCurOPLevel )
	{
		//�Ż�ȥ
		OpPush_OldOP(pOP);
		return false;
	}

	if( pOP->m_nType == ATSOP_TYPE_SINGLEOP )
	{
		// �����������
		c = pOP->m_strName.GetAt(0);
		switch( c )
		{
			case '-': a = ValPop();		ValPush( -(a->GetdData()) );		bCal=true;	break;
			case '!': a = ValPop();		ValPush(a->GetdData() == 0);		bCal=true;	break;	
			case '~': a = ValPop();		ValPush(~(int) a->GetdData());		bCal=true;	break;			
		}
	}
	else if( pOP->m_nType == ATSOP_TYPE_BINOP )
	{
		// ˫���������
		if( pOP->m_strName == "+" )
		{	
			VAL_POP_CHECK_NUM_STR( a );
			VAL_POP_CHECK_NUM_STR( b );
			b->AddData( a ); ValPush( b ); 
			bCal=true;
		}
		if( pOP->m_strName == "-" )
		{
			VAL_POP_CHECK_NUM( a );
			VAL_POP_CHECK_NUM( b );
			ValPush( b->GetdData() - a->GetdData() );				
			bCal=true;
		}
		else if( pOP->m_strName == "*" )
		{
			VAL_POP_CHECK_NUM( a );
			VAL_POP_CHECK_NUM( b );
			ValPush( b->GetdData()*a->GetdData());			
			bCal=true;
		}
		else if( pOP->m_strName == "/" )
		{
			VAL_POP_CHECK_NUM( a );
			VAL_POP_CHECK_NUM( b );		
			ValPush( b->GetdData()/a->GetdData());			
			bCal=true;
		}
		else if( pOP->m_strName == "%" )
		{
			VAL_POP_CHECK_NUM( a );
			VAL_POP_CHECK_NUM( b );
			ValPush((int) b->GetdData() % (int) a->GetdData());			
			bCal=true;
		}
		else if( pOP->m_strName == "^" )
		{
			VAL_POP_CHECK_NUM( a );
			VAL_POP_CHECK_NUM( b );		
			ValPush((int) b->GetdData() ^ (int) a->GetdData());	
			bCal=true;
		}
	}
	else if( pOP->m_nType == ATSOP_TYPE_LOG )
	{
		// �߼������
		c = pOP->m_strName.GetAt(0);
		if( pOP->m_strName == "&" )
		{
			a = ValPop();	b = ValPop();		ValPush((int) b->GetdData() & (int) a->GetdData() );		
			bCal=true;
		}
		else if( pOP->m_strName == "|" )
		{
			a = ValPop();	b = ValPop();	
			ValPush((int) b->GetdData() | (int) a->GetdData());	
			bCal=true;
		}
	}
	else if( pOP->m_nType == ATSOP_TYPE_CHECK )
	{
		// �ж������
		if( pOP->m_strName.CompareNoCase( "AND" ) == 0  ||
			pOP->m_strName.CompareNoCase( "&&" ) == 0  )
		{
			VAL_POP_CHECK_NUM( a );
			VAL_POP_CHECK_NUM( b );
			if( a->GetdData() != 0 && b->GetdData() != 0 )
				ValPush(1);	
			else
				ValPush((double)0);	
			bCal=true;
		}
		else if( pOP->m_strName.CompareNoCase( "OR" ) == 0 ||
			     pOP->m_strName.CompareNoCase( "||" ) == 0  )
		{
			VAL_POP_CHECK_NUM( a );
			VAL_POP_CHECK_NUM( b );
			if( a->GetdData() != 0 || b->GetdData() != 0 )
				ValPush(1);	
			else
				ValPush((double)0);	
			bCal=true;
		}
		else if( pOP->m_strName == "==" ) //||pOP->m_strName == "=")
		{
			// ���ݱȽ�Ҫ�� "==" , "=" Ŀǰ����ȷ��
			VAL_POP_CHECK_NUM_STR( a );
			VAL_POP_CHECK_NUM_STR( b );
			if( b->CmpData( a ) )
				ValPush( 1 );
			else
				ValPush((double)0);
			bCal=true;
		}
		else if( pOP->m_strName == ">=" )
		{
			VAL_POP_CHECK_NUM( a );
			VAL_POP_CHECK_NUM( b );
			ValPush(b->GetdData() >= a->GetdData());				
			bCal=true;
		}
		else if( pOP->m_strName == ">" )
		{
			VAL_POP_CHECK_NUM( a );
			VAL_POP_CHECK_NUM( b );		
			ValPush(b->GetdData() > a->GetdData());				
			bCal=true;	
		}
		else if( pOP->m_strName == "<=" )
		{
			VAL_POP_CHECK_NUM( a );
			VAL_POP_CHECK_NUM( b );
			ValPush(b->GetdData() <= a->GetdData() );
			bCal=true;
		}
		else if( pOP->m_strName == "<" )
		{
			VAL_POP_CHECK_NUM( a );
			VAL_POP_CHECK_NUM( b );
			ValPush(b->GetdData() < a->GetdData() );
			bCal=true;
		}
		else if( pOP->m_strName == "!=" || pOP->m_strName == "<>" )
		{
			VAL_POP_CHECK_NUM_STR( a );
			VAL_POP_CHECK_NUM_STR( b );
			if( b->CmpData( a ) )
				ValPush((double)0);
			else
				ValPush( 1 );
			bCal=true;
		}
	}
	else if( pOP->m_nType == ATSOP_TYPE_SYSFUN )
	{
		// ִ��ϵͳ����		
		bCal = ExecSysFun( pOP );
	}
	else if( pOP->m_nType == ATSOP_TYPE_EXTFUN )
	{
		// ִ����չ�ĺ���
		// �����ĺ�������� (...)
		CAtsParam		param;

		// ������Ȼ����һ�� Value ��Ϊ�� Param �����Ӧ
		a = ValPop();
		if( !a )
			return false;
	
		param.m_pEng			= m_pFEExt->m_pEgnine; 
		param.m_pParentSegIns	= m_pFEExt->m_pSegIns;
		if( !param.LoadParamData( (LPCTSTR)a->GetStrData() ) )
		{
			m_strErrWord = a->GetStrData();
			return false;
		}
		// ����ĳһ�������ı�����
		if( m_pFEExt->RunFun( pOP->m_strName , &param ) ) 
			ValPush( &param.m_vReturn );
		else
		{
			m_strErrWord = pOP->m_strName;
			return false;
		}
		bCal=true;
	}

	if( bCal )
	{
		delete pOP;
		return true;
	}
	else
	{
		// ���������,��Ż�ȥ
		OpPush_OldOP(pOP);
		return  false;
	}

	return true;
}

//=============================================
// ����ϵͳ����
//=============================================
void CAtsExpMgr::InitSysFun()
{
	m_listSysFunName.push_back( "int" );
//	m_listSysFunName.push_back( "log" );
//	m_listSysFunName.push_back( "pow" );
}

//=============================================
// ִ��ϵͳ����
//=============================================
BOOL CAtsExpMgr::ExecSysFun( CAtsExpOpItem *pOP )
{
	BOOL						bCal = false;
	CAtsObjPtr<CAtsValue>		a, b;	// �� CAtsObjPtr �ĺô����ǻ��Զ��ſ�ָ��

	if( pOP->m_strName.CompareNoCase( "int" ) == 0 )
	{
		VAL_POP_CHECK_NUM( a );
		ValPush((int)a->GetdData());	bCal=true;
	}
/*
	else if( pOP->m_strName.CompareNoCase( "pow" ) == 0 )
	{
		a = ValPop();
		CAtsString		strItem,strItem2;
		CAtsParam	Param;
		CAtsVarItem *pItem1;
		CAtsVarItem *pItem2;
		Param.LoadParamData( (LPCTSTR)a->GetStrData() );

		pItem1 = Param.GetItem( 1 );
		if( !TranvItemToData( &pItem1->m_vData ) ) 
			return false;
		pItem2 = Param.GetItem( 2 );
		if( !TranvItemToData( &pItem2->m_vData ) ) 
			return false;

		ValPush(pow( pItem1->m_vData.GetdData() , pItem2->m_vData.GetdData() ));	
		bCal=true;
	}
	else if( pOP->m_strName.CompareNoCase( "log" ) == 0 )
	{
		VAL_POP_CHECK_NUM( a );
		ValPush(log(a->GetdData()));	
		bCal=true;
	}
*/

	return bCal;
}

BOOL CAtsExpMgr::GetElementItem(CAtsString &strStuff, CAtsString &strItem,CAtsString strApart)
{
	if( strStuff.GetLength() == 0 )
		return FALSE;

	int pos = strStuff.Find(strApart);
	if( pos >= 0 )
	{
		strItem = strStuff.Left(pos);
		strStuff = strStuff.Right( strStuff.GetLength() - pos - strApart.GetLength() );
	}
	else
	{
		strItem		= strStuff;
		strStuff	= "";
	}
	return TRUE;
}

//================================================
// ��ȡһ������
//?? �ڶ�����ʱ����������һ���ַ����硡1,  3)
//================================================
CAtsString CAtsExpMgr::ReadNumber(int &nPos )
{
	CAtsString hilf;
	char	c;

	hilf = "";
	hilf += m_strData.GetAt(nPos);
	for (nPos++ ; nPos < m_nStrLen ; nPos++)
	{
		c = m_strData.GetAt(nPos);					// Aktuelles Zeichen
		hilf += c;
		if (isxdigit(c))				continue;	// Hex, Bin oder Dec
		if (c == '.')					continue;	// Nachkommastellen

		switch (tolower(m_strData.GetAt(nPos-1)))
		{
		case 'e':	if (c == '+')		continue;	// Positiver Exponent
					if (c == '-')		continue;	// Negativer Exponent
					break;
		case '0':	if (c == 'x')		continue;	// Hexadezimalzahl
					break;
		}

		break;		// Zahl abgeschlossen
	}

	//nPos--;
	//return CAtsValue::Dec(hilf);	
	return hilf;
}

//===================================================
// ȡ��һ���ַ�,����һ���ո�
//===================================================
void CAtsExpMgr::GetChar(int& nPosition, char& cItem)
{
	do
	{
		nPosition ++;	// evtl. nach strCharacter
		if (nPosition <= m_strData.GetLength())
			cItem = m_strData.GetAt( nPosition - 1 );			
		else 
			cItem = '\0';
 	}
	while (cItem == ' '); 
}

//================================================
// �Ƿ�Ϊϵͳ����
//================================================
BOOL CAtsExpMgr::IsSysFunName(CAtsString strName)
{
	int		nNum,nCount;

	nCount = m_listSysFunName.size();
	for( nNum = 0 ; nNum < nCount ; nNum++ )
	{
		if( m_listSysFunName[nNum].CompareNoCase( strName ) == 0 )
			return true;
	}
	return false;
}

//========================================================
// ��ȡһ�� Item
//========================================================
BOOL CAtsExpMgr::GetItem( int &nPos , CAtsValue &vItem )
{
	if( nPos >= m_nStrLen )
		return false;

	char			cFist;
	CAtsString		strTemp;

	vItem.Clear();
	do
	{
		cFist = m_strData.GetAt( nPos );
		if( cFist == ' ' )
			nPos++;
	}while( nPos < m_nStrLen && cFist == ' ' );
	if( cFist == ' ' )
		return false;

	switch( cFist )
	{
		case '0': case '1': case '2': case '3': case '4':
		case '5': case '6': case '7': case '8': case '9':
			strTemp = ReadNumber( nPos );
			vItem.SetDataEx( strTemp , CAtsValue::GetNumType( strTemp ) );
			return true;
			break;
		case ')':
		case '(':
		case ',':
		case '^':
		case '~':
		case '!':
			nPos++;
			if( nPos < m_nStrLen && m_strData.GetAt( nPos ) == '=' )
			{
				// ������ !=
				strTemp = cFist;
				strTemp = strTemp + m_strData.GetAt( nPos );
				nPos++; 
				vItem.SetDataEx( strTemp , ATSVAR_TYPE_OP );	
			}
			else
				vItem.SetDataEx( cFist , ATSVAR_TYPE_OP );
			break;

		case '=':
		case '-':	
		case '*':
		case '%':
		case '/':
		case '>':
			nPos++;
			if( nPos < m_nStrLen && m_strData.GetAt( nPos ) == '=' )
			{
				// ������  -= , *= , /=
				strTemp = cFist;
				strTemp = strTemp + "=";
				nPos++; 
				vItem.SetDataEx( strTemp , ATSVAR_TYPE_OP );	
			}
			else
				vItem.SetDataEx( cFist , ATSVAR_TYPE_OP );
			break;
		case '&':
			nPos++;
			if( nPos < m_nStrLen && m_strData.GetAt( nPos ) == '=' )
			{
				// ������  &=
				strTemp = cFist;
				strTemp = strTemp + "=";
				nPos++; 
				vItem.SetDataEx( strTemp , ATSVAR_TYPE_OP );	
			}
			else if( nPos < m_nStrLen && m_strData.GetAt( nPos ) == '&' )
			{
				// ������  &&
				strTemp = cFist;
				strTemp = strTemp + "&";
				nPos++; 
				vItem.SetDataEx( strTemp , ATSVAR_TYPE_OP );	
			}
			else
				vItem.SetDataEx( cFist , ATSVAR_TYPE_OP );
			break;
		case '|':
			nPos++;
			if( nPos < m_nStrLen && m_strData.GetAt( nPos ) == '=' )
			{
				// ������  |=
				strTemp = cFist;
				strTemp = strTemp + "=";
				nPos++; 
				vItem.SetDataEx( strTemp , ATSVAR_TYPE_OP );	
			}
			else if( nPos < m_nStrLen && m_strData.GetAt( nPos ) == '|' )
			{
				// ������  ||
				strTemp = cFist;
				strTemp = strTemp + "|";
				nPos++; 
				vItem.SetDataEx( strTemp , ATSVAR_TYPE_OP );	
			}
			else
				vItem.SetDataEx( cFist , ATSVAR_TYPE_OP );
			break;
		case '+':
			nPos++;
			if( nPos < m_nStrLen && ( m_strData.GetAt( nPos ) == '=' || m_strData.GetAt( nPos ) == '+' ) )
			{
				// ������ += , ++
				strTemp = cFist;
				strTemp = strTemp + m_strData.GetAt( nPos );
				nPos++; 
				vItem.SetDataEx( strTemp , ATSVAR_TYPE_OP );	
			}
			else
				vItem.SetDataEx( cFist , ATSVAR_TYPE_OP );
			break;
		case '<':
			nPos++;
			if( nPos < m_nStrLen && 
				( m_strData.GetAt( nPos ) == '=' || m_strData.GetAt( nPos ) == '>' ) )
			{
				// ������ <= , <>
				strTemp = cFist;
				strTemp = strTemp + m_strData.GetAt( nPos );
				nPos++; 
				vItem.SetDataEx( strTemp , ATSVAR_TYPE_OP );	
			}
			else
				vItem.SetDataEx( cFist , ATSVAR_TYPE_OP );
			break;
		case '\"':
			// ��ȡһ���ַ���
			if( !ReadString( nPos , strTemp ) )
			{
				m_bIsError = true;
				return false;
			}
			vItem.SetDataEx( strTemp , ATSVAR_TYPE_STRING );	
			break;
		case '@':
			// ��ȡһ������
			strTemp = Read_FunName_Var( nPos );
			vItem.SetDataEx( strTemp , ATSVAR_TYPE_VAR );	
			break;
		default: 
			// ��ȡһ�� FunName �� ������
			strTemp = Read_FunName_Var( nPos );
			if( nPos < m_nStrLen &&  m_strData.GetAt( nPos ) == '.' )
			{
				// ������ ����� fun, ��: MyObj.funname
				CAtsString		strSub;
				nPos++;
				
				strSub = Read_FunName_Var( nPos );	
				strTemp = strTemp + "." + strSub;
				if( strSub.GetLength() <= 0 )
					return false;
				if( strSub.GetAt(0) == '@' )
					vItem.SetDataEx( strTemp , ATSVAR_TYPE_VAR );
			}
			
			if( GetVarItem( strTemp ) )
				vItem.SetDataEx( strTemp , ATSVAR_TYPE_VAR );
			else if( m_pFEExt && m_pFEExt->IsVar( strTemp ) )
				vItem.SetDataEx( strTemp , ATSVAR_TYPE_VAR );
			else if( m_pFEExt && m_pFEExt->IsFunction( strTemp ) )
				vItem.SetDataEx( strTemp , ATSVAR_TYPE_OP );
			else
				vItem.SetDataEx( strTemp , ATSVAR_TYPE_OP );	

			break;
	}

	return true;
}

//================================================
// ��ȡһ������������
//================================================
CAtsString CAtsExpMgr::Read_FunName_Var(int &nPos )
{
	CAtsString		strName;
	char		c;

	while( true )
	{
		if( nPos >= m_nStrLen )
			break;

		c = m_strData.GetAt(nPos);

		// �����������Ч�ı����ַ�
		if( IsValidVarChar(c) )
		{
			nPos++;
			strName += c;
		}
		else
			break;
	}
	return strName;
}



//================================================
// ��ȡһ���ַ���, ��������
//================================================
BOOL CAtsExpMgr::ReadString(int &nPos , CAtsString &strData)
{
	char		c;
	BOOL		bIsChar = false;

	c = m_strData.GetAt(nPos);
	if( c != '\"' )	
	{
		m_bIsError = false;
		return false;
	}
	nPos++;

	while( true )
	{
		if( nPos >= m_nStrLen )
			break;

		c = m_strData.GetAt(nPos);

		if( c == '\"' )
		{
			nPos++;
			return true;
		}
		else if( c == '\\' )
		{
			//path: ����� '\' ����ת���ַ�
			// �� \ ���һλ����
			if( (nPos + 1) < m_nStrLen )
			{
				char x = m_strData.GetAt(nPos + 1);
				nPos++;

				if( x == 'x' )
				{
					//path: \xAA
					// \x ����Ҫ��ȡ��λ����
					if( ( nPos + 2 ) < m_nStrLen )
					{
						c = CAtsBaseFun::GetEscapeChar_x(m_strData.GetAt(nPos+1),m_strData.GetAt(nPos+2));
						nPos += 2;
					}
					else
					{
						// ת���ַ���û������,�����
						ASSERT( false );
						return false;
					}
				}
				else
					c = CAtsBaseFun::GetEscapeChar(x);
			}
			else
			{
				// ת���ַ���û������,�����
				ASSERT( false );
				return false;
			}
		}

		strData = strData + c;
		nPos++;
	}
	return false;
}

//================================================
// ���� vItem Ϊ��������
//================================================
BOOL CAtsExpMgr::TranvItemToData(CAtsValue *pvItem)
{
	if( pvItem->m_nType == ATSVAR_TYPE_VAR )
	{
		// ����Ǳ��ʽ�еı���
		if( GetVarValue( pvItem->GetStrData() , pvItem ) )
			return true;

		// ��һ������
		if( m_pFEExt )
		{
			m_pFEExt->GetVarValue( pvItem->GetStrData() , pvItem );
		}
		else
			return false;
	}
	return true;
}

//============================================
// ���ݱ������õ�������ֵ
//============================================
BOOL CAtsExpMgr::GetVarValue( CAtsString strVarName , CAtsValue *pValue )
{
	CAtsVarItem		*pVarItem = NULL;

	pVarItem = GetVarItem( strVarName );
	if( !pVarItem )
		return false;

	pVarItem->GetValueData( pValue );
	return true;
}

//============================================
// ���ݱ������õ�������ֵ
//============================================
CAtsVarItem * CAtsExpMgr::GetVarItem( CAtsString strVarName )
{
	CAtsVarItem		*pVarItem = NULL;
	POSITION		pos;

	for( pos = m_listUserVar.GetHeadPosition() ; pos ; )
	{
		pVarItem = (CAtsVarItem *)m_listUserVar.GetNext(pos); 
		if( pVarItem->m_strName.CompareNoCase( strVarName ) == 0 )
			return pVarItem;
	}

	return NULL;
}

//================================================
// �ж���һ����Ч�ַ��ǲ��� citem
//================================================
BOOL CAtsExpMgr::NextVCharIs(int nBeginPos , char cItem )
{
	int			nPos = nBeginPos;
	char		c;

	while( true )
	{
		if( nPos >= m_nStrLen )
			break;

		c = m_strData.GetAt(nPos);

		// �����������Ч�ı����ַ�
		if( c == ' ' )
			nPos++;
		else if( cItem == c )
			return true;
		else
			return false;
	}
	return false;
}

//========================================================
// ����һ�� Functoin ��Param
// ������������ݲ�ת��Ϊ����,��ΪҪ��Ϊfunction �Ĳ���
//========================================================
BOOL CAtsExpMgr::CmputerParam( CAtsString strExpress , CAtsParam *param )
{
	CAtsString		strData;
	CAtsValue	*pValue = NULL;
	
	if( strExpress.IsEmpty() )
		return true;

	if( !CalcExpress( strExpress ) )
		return false;

	// �Ӷ���ȡ����
	pValue = ValPop();
	if( pValue && pValue->m_nType == ATSVAR_TYPE_PARAM )
	{
		param->LoadParamData( (LPCTSTR)pValue->GetStrData() );
		delete pValue;
		pValue = NULL;
	}
	else
	{
		while( pValue )
		{
			param->AddItemToHead( pValue );
			delete pValue;
			pValue = NULL;

			pValue = ValPop();
		}
	}
	return true;
}

BOOL CAtsExpMgr::SetVar(LPCTSTR sName , LPCTSTR strData , int nType )
{
	CAtsVarItem		*pVarItem = NULL;
	CAtsString		strName = sName;
 
	pVarItem = GetVarItem( sName );
	if( pVarItem )
	{
		pVarItem->m_vData.SetData( strData );
		return true;
	}

	pVarItem = new CAtsVarItem;
	pVarItem->m_strName = strName;
	pVarItem->m_vData.m_nType = nType;
	pVarItem->m_vData.SetData( strData );
	pVarItem->AddRef();
	m_listUserVar.AddTail( pVarItem );

	return true;
}

BOOL CAtsExpMgr::SetVar(LPCTSTR sName , double nData , int nType )
{
	CAtsVarItem		*pVarItem = NULL;
	CAtsString		strName = sName;
 
	pVarItem = GetVarItem( sName );
	if( pVarItem )
	{
		pVarItem->m_vData.SetData( nData );
		return true;
	}

	pVarItem = new CAtsVarItem;
	pVarItem->m_strName = strName;
	pVarItem->m_vData.m_nType = nType;
	pVarItem->m_vData.SetData( nData );
	pVarItem->AddRef();
	m_listUserVar.AddTail( pVarItem );
	
	return true;
}
