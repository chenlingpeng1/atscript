
#include "AtsExpMgr.h"
#include "AtsExpValue.h"
#include "AtsObjPtr.h"
#include "AtsCommdef.h"
#include "AtsBaseFun.h"
#include "DoubleObj.h"

const double ATS_PI	= 3.14159265358979323846;
const double ATS_e	= 2.7182818284590452354;

// 把一个值从堆中取出,并检测数据类型
// 如是查变量,还要得到其内容
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
 * gel鰏cht und der Ursprungszustand der Klasse wieder hergestellt.
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
// 判断是否出错了
//=================================================
bool CAtsExpMgr::TestError()
{
	if( m_bIsError )
		return false;

	return true;
}

//========================================================
// 计算表达试
//========================================================
BOOL CAtsExpMgr::Computer( CAtsString strExpress , CAtsValue *pValue )
{
	CAtsString		strData;
	CAtsValue	*pValueItem = NULL;
	
	if( !CalcExpress( strExpress ) )
		return false;

	// 从堆中取数据
	if( m_listValue.GetCount() <= 0 )
		return false;
	else
		pValueItem = ValTop();

	// 如果是变量,则转为数据
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
// 判断条件语句是否成立
//========================================================
BOOL CAtsExpMgr::CheckCondition( CAtsString strExpress )
{
	CAtsString		strData;
	CAtsValue	*pValue = NULL;
	
	if( strExpress.IsEmpty() )
		return false;

	CalcExpress( strExpress );

	// 从堆中取数据
	if( m_listValue.GetCount() <= 0 )
		return false;
	else
		pValue = ValTop();

	// 如果是变量,则转为数据
	if( pValue->m_nType == ATSVAR_TYPE_VAR )
	{
		if( !TranvItemToData( pValue ) ) 
		{
			m_bIsError=true;
			ASSERT( false );
			return false;
		}
	}

	// 如果数据不为 0 , 都认为条件成立
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
// 进行表达试计算
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
			// 是一个变量
			/*if( m_pFEExt )
			{
				CAtsValue		vDataItem;
				m_pFEExt->GetVarValue( vItem.m_strData , &vDataItem );
				if( vDataItem.IsNum() )
					ValPush( vDataItem.m_dData );
				else
					ValPush( vDataItem.m_strData );
			}*/
			// 显示直接放到堆中,只有在计算时才把它转为数据
			// 其目的是为了在做为 Function 的参数时可能要还回数据,要让 function 知道
			// 变量名
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
			
					// 结束括号判断上一个是不是 fun , 如果是,则一定要加入一个 value 
					// 做为 function 的 Param, 内容是空的
					if( OpTop() && OpTop()->m_nType == ATSOP_TYPE_SYSFUN ||
						OpTop() && OpTop()->m_nType == ATSOP_TYPE_EXTFUN )
					{
						// 如果前一个值就是符号,则表括号中内容为空
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
						// 合并 Fun 参数, 合并所有的 ','
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
						// 如是还不是 "(" , 则出错
						m_bIsError = true;
						ASSERT( false );
						return false;
					}
				}

				// 结束括号判断上一个是不是 fun 
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

				// 结束括号后要执行马上可以执行的
				CalMastTop();
			}
			else if( vItem.GetStrData() == "(" )
			{
				CHECK_PRVITEM_OP_NONE();
				OpPush( ATSOP_TYPE_SEG , vItem.GetStrData() );
			}
			else if( vItem.GetStrData() == ",")
			{
				CHECK_PRVITEM_VALUE(vItem.GetStrData() );	// 前一个Item 一定要是value	

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
				CHECK_PRVITEM_VALUE(strItemData);	// 前一个Item 一定要是value	

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
					OpPush( ATSOP_TYPE_BINOP , strItemData );		// 减号
				else
					OpPush(ATSOP_TYPE_SINGLEOP , "-" );				// 负号
			}
			else if( strItemData == "&" ||		// lOG 运算符
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
			else if( strItemData == "!" ||		// 单数据运算符
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
					// 认为是扩展函数
					OpPush( ATSOP_TYPE_EXTFUN , strItemData );
					// 如果是函数, 下一个有效符号必须是 '('
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

	// 计算到底, 到 '(' , ',' 停止
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
	// 是字 A – Z, a – z, or 0 – 9
	if( isalnum(cChar) )
		return true;
	
	//是汉字
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
// 取出符号堆的最上一个
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
// 得到符号堆的最上一个
//=========================================
CAtsExpOpItem * CAtsExpMgr::OpTop()					
{ 
	if( m_listOP.GetCount() <= 0 )
		return NULL;

	return (CAtsExpOpItem *)m_listOP.GetHead();
}

//=======================================
// 向堆中加入一个符号
//=======================================
void CAtsExpMgr::OpPush(int nType , CAtsString strName , int nPos )
{
	CAtsExpOpItem *pItem  = new CAtsExpOpItem;
	pItem->m_nType		= nType;
	pItem->m_strName	= strName;
	pItem->m_nPos		= nPos;

	// 如果加进来的不是马上运行的符号,而前一个是双数操作符
	// 则先运算前一个
	if( pItem->m_nType == ATSOP_TYPE_BINOP && !pItem->IsMastCal() )
	{
		if( OpTop() && OpTop()->m_nType == ATSOP_TYPE_BINOP )
			CalTop();
	}

	m_nPrvItemType = FITEM_TYPE_OP;
	m_listOP.AddHead( pItem );
}

//=======================================
// 向堆中放回原来的 OP 加入一个符号
// 所以不修改 m_nPrvItemType
//=======================================
void CAtsExpMgr::OpPush_OldOP(CAtsExpOpItem *pOPItem)
{
	m_listOP.AddHead( pOPItem );
}

//=======================================
// 向堆中加入一个符号
//=======================================
void CAtsExpMgr::OpPush(int oper)		
{ 
	CAtsExpOpItem *pItem  = new CAtsExpOpItem;
	pItem->m_nType   = ATSOP_TYPE_BINOP;
	pItem->m_strName = (char)oper;

	// 如果加进来的不是马上运行的符号,而前一个是双数操作符
	// 则先运算前一个
	if( pItem->m_nType == ATSOP_TYPE_BINOP && !pItem->IsMastCal() )
	{
		if( OpTop() && OpTop()->m_nType == ATSOP_TYPE_BINOP )
			CalTop();
	}

	m_nPrvItemType = FITEM_TYPE_OP;
	m_listOP.AddHead( pItem );					
}

//=======================================
// 向堆中加入一个值
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
// 向堆中加入一个值
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
// 向堆中加入一个值
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
// 取出数据堆的最上一个
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
// 得到数据堆的最上一个
//=========================================
CAtsValue * CAtsExpMgr::ValTop()					
{ 
	if( m_listValue.GetCount() <= 0 )
		return NULL;

	return (CAtsValue *)m_listValue.GetHead();
}

//=====================================
// 执行马上可以执行的运算, 1 级运算符
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
// 一直计算到 '(' , ',' 为至
//=====================================
void CAtsExpMgr::CalTheSegment(int nCurOPLevel)		
{ 
	while( CalTop( nCurOPLevel ) )
		;
}

//================================================
// 对是了上一个进行计算
// 到 '(' , "," 时会停下,因为下面没例出 '(' , ','
//================================================
bool CAtsExpMgr::CalTop(int nCurOPLevel)		
{ 
	CAtsObjPtr<CAtsValue>		a, b;	// 用 CAtsObjPtr 的好处就是会自动放开指针
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
		//放回去
		OpPush_OldOP(pOP);
		return false;
	}

	if( pOP->m_nType == ATSOP_TYPE_SINGLEOP )
	{
		// 单数据运算符
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
		// 双数据运算符
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
		// 逻辑运算符
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
		// 判断运算符
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
			// 数据比较要用 "==" , "=" 目前还不确定
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
		// 执行系统函数		
		bCal = ExecSysFun( pOP );
	}
	else if( pOP->m_nType == ATSOP_TYPE_EXTFUN )
	{
		// 执行扩展的函数
		// 函数的后面必须有 (...)
		CAtsParam		param;

		// 函数必然会有一个 Value 做为其 Param 与其对应
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
		// 设置某一个参数的变量名
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
		// 如果还不能,则放回去
		OpPush_OldOP(pOP);
		return  false;
	}

	return true;
}

//=============================================
// 定义系统函数
//=============================================
void CAtsExpMgr::InitSysFun()
{
	m_listSysFunName.push_back( "int" );
//	m_listSysFunName.push_back( "log" );
//	m_listSysFunName.push_back( "pow" );
}

//=============================================
// 执行系统函数
//=============================================
BOOL CAtsExpMgr::ExecSysFun( CAtsExpOpItem *pOP )
{
	BOOL						bCal = false;
	CAtsObjPtr<CAtsValue>		a, b;	// 用 CAtsObjPtr 的好处就是会自动放开指针

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
// 读取一个数字
//?? 在读数字时，会在最后多一个字符，如　1,  3)
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
// 取出一个字符,跳过一个空格
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
// 是否为系统函数
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
// 读取一个 Item
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
				// 可能是 !=
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
				// 可能是  -= , *= , /=
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
				// 可能是  &=
				strTemp = cFist;
				strTemp = strTemp + "=";
				nPos++; 
				vItem.SetDataEx( strTemp , ATSVAR_TYPE_OP );	
			}
			else if( nPos < m_nStrLen && m_strData.GetAt( nPos ) == '&' )
			{
				// 可能是  &&
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
				// 可能是  |=
				strTemp = cFist;
				strTemp = strTemp + "=";
				nPos++; 
				vItem.SetDataEx( strTemp , ATSVAR_TYPE_OP );	
			}
			else if( nPos < m_nStrLen && m_strData.GetAt( nPos ) == '|' )
			{
				// 可能是  ||
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
				// 可能是 += , ++
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
				// 可能是 <= , <>
				strTemp = cFist;
				strTemp = strTemp + m_strData.GetAt( nPos );
				nPos++; 
				vItem.SetDataEx( strTemp , ATSVAR_TYPE_OP );	
			}
			else
				vItem.SetDataEx( cFist , ATSVAR_TYPE_OP );
			break;
		case '\"':
			// 读取一个字符串
			if( !ReadString( nPos , strTemp ) )
			{
				m_bIsError = true;
				return false;
			}
			vItem.SetDataEx( strTemp , ATSVAR_TYPE_STRING );	
			break;
		case '@':
			// 读取一个变量
			strTemp = Read_FunName_Var( nPos );
			vItem.SetDataEx( strTemp , ATSVAR_TYPE_VAR );	
			break;
		default: 
			// 读取一个 FunName 或 变量名
			strTemp = Read_FunName_Var( nPos );
			if( nPos < m_nStrLen &&  m_strData.GetAt( nPos ) == '.' )
			{
				// 可能是 对象的 fun, 如: MyObj.funname
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
// 读取一个变量或函数名
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

		// 如果不是字有效的变量字符
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
// 读取一个字符串, 不含引号
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
			//path: 如果是 '\' 则是转义字符
			// 读 \ 后的一位数据
			if( (nPos + 1) < m_nStrLen )
			{
				char x = m_strData.GetAt(nPos + 1);
				nPos++;

				if( x == 'x' )
				{
					//path: \xAA
					// \x 后面要读取两位数据
					if( ( nPos + 2 ) < m_nStrLen )
					{
						c = CAtsBaseFun::GetEscapeChar_x(m_strData.GetAt(nPos+1),m_strData.GetAt(nPos+2));
						nPos += 2;
					}
					else
					{
						// 转义字符后没数据了,则出错
						ASSERT( false );
						return false;
					}
				}
				else
					c = CAtsBaseFun::GetEscapeChar(x);
			}
			else
			{
				// 转义字符后没数据了,则出错
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
// 设置 vItem 为数据内容
//================================================
BOOL CAtsExpMgr::TranvItemToData(CAtsValue *pvItem)
{
	if( pvItem->m_nType == ATSVAR_TYPE_VAR )
	{
		// 如果是表达式中的变量
		if( GetVarValue( pvItem->GetStrData() , pvItem ) )
			return true;

		// 是一个变量
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
// 根据变量名得到变量的值
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
// 根据变量名得到变量的值
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
// 判断下一个有效字符是不是 citem
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

		// 如果不是字有效的变量字符
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
// 生成一个 Functoin 的Param
// 计算出来的内容不转数为数据,因为要作为function 的参数
//========================================================
BOOL CAtsExpMgr::CmputerParam( CAtsString strExpress , CAtsParam *param )
{
	CAtsString		strData;
	CAtsValue	*pValue = NULL;
	
	if( strExpress.IsEmpty() )
		return true;

	if( !CalcExpress( strExpress ) )
		return false;

	// 从堆中取数据
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
