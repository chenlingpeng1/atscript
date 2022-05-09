// AtsParam.cpp: implementation of the CAtsParam class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AtsParam.h"
#include "AtsEngine.h"
#include "AtsFile.h"
#include "AtsVarItem.h"
#include "AtsBaseFun.h"
#include "AtsFile.h"
#include "AtsExpMgr.h"
#include "AtsSegIns.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAtsParam::CAtsParam()
{
	m_pEng			= NULL;
	m_pParentSegIns = NULL;
}

CAtsParam::~CAtsParam()
{
	DetachList();
}

CAtsParam & CAtsParam::operator = ( CAtsParam &Item )
{
	POSITION			pos;
	CAtsVarItem			*pItem = NULL;
	CAtsVarItem			*pNewItem = NULL;

	DetachList();

	m_pEng			= Item.m_pEng;
	m_pParentSegIns	= Item.m_pParentSegIns;

	for( pos = Item.m_listParam.GetHeadPosition() ; pos ; )
	{
		pItem = (CAtsVarItem *)Item.m_listParam.GetNext( pos );
		pNewItem = new CAtsVarItem;
		*pNewItem = *pItem;
		m_listParam.AddTail( pNewItem );
	}
	m_vReturn = Item.m_vReturn;
	return *this;
}

void CAtsParam::DetachList( )
{
	POSITION			pos;
	CAtsVarItem			*pItem = NULL;

	for( pos = m_listParam.GetHeadPosition() ; pos ; )
	{
		pItem = (CAtsVarItem *)m_listParam.GetNext( pos );
		pItem->Release();
	}
	m_listParam.RemoveAll();
}

//===================================================================
// װ�����, ͬ��������������������
//
// ��: 3<,>5<,>@a<,>"abc" <,> "@a"
// ��: 2|3<,>2|5<,>2|@a<,>2|"abc"<,> 2|"@a"   ( �� | �ֿ�,ǰ���Ǳ������� )
// ������ַ����������, �����Ϳ����жϳ��������ַ�����
//===================================================================
BOOL CAtsParam::LoadParamData( CAtsString strParams )
{
	int		nLen = 0;

	strParams.TrimLeft(' ');
	strParams.TrimLeft(' ');

	nLen = strParams.GetLength();
	if( nLen == 0 )
		return true;

	while( nLen >= 2 && strParams.GetAt(0) == '(' && strParams.GetAt(nLen-1) == ')' )
	{
		strParams = strParams.Mid(1 , nLen - 2 );
		strParams.TrimLeft(' ');
		strParams.TrimLeft(' ');

		nLen = strParams.GetLength();
		if( nLen == 0 )
			return true;
	}

	CAtsString		strItem;
	while( CAtsFile::GetElementItem(strParams,  strItem,"<,>" ) )
	{
		CAtsVarItem		*pItem		= new CAtsVarItem;
		pItem->m_vData.m_nType		= ATSVAR_TYPE_VARIANT;

		int		nItemLen = strItem.GetLength();
		int		nPos	 = strItem.Find('|');
		if( nPos > 0 )
		{
			pItem->m_vData.m_nType = atoi( strItem.Left( nPos ) );
			pItem->m_vData.SetData( strItem.Mid(nPos+1) );
		}
		else
		{
			nLen = strItem.GetLength();
			if( nLen >= 2 && strItem.GetAt(0) == '\"' && strItem.GetAt(nLen-1) == '\"' )
			{
				strItem = strItem.Mid(1 , nLen - 2 );
				pItem->m_vData.m_nType = ATSVAR_TYPE_STRING;
			}
			else if( nLen > 0 )
			{
				if( CAtsBaseFun::IsValidVarName( strItem ) )
				{
					if( m_pEng && m_pEng->IsVar( strItem , m_pParentSegIns ) )
						pItem->m_vData.m_nType = ATSVAR_TYPE_VAR;
				}
			}
			pItem->m_vData.SetData( strItem );
		}
		m_listParam.AddTail( pItem );
	}
	return true;
}

int CAtsParam::DataCmp( int nIndex1 , int nIndex2 )
{
	int		nDataType = ATSVAR_TYPE_STRING;

	CAtsString strFaceData1 = GetItemData(nIndex1);
	CAtsString strFaceData2 = GetItemData(nIndex2);

	if( nDataType == ATSVAR_TYPE_NONE || nDataType == ATSVAR_TYPE_STRING )
		return strFaceData1.CompareNoCase(strFaceData2);
	else
		return strFaceData1.CompareNoCase(strFaceData2);
}

CAtsVarItem * CAtsParam::GetItem( int nIndex )
{
	POSITION	pos;
	CAtsVarItem	*pItem = NULL;

	pos = m_listParam.FindIndex( nIndex -1 );
	if( !pos )
		return NULL;

	pItem = ( CAtsVarItem * )m_listParam.GetAt( pos );
	return pItem;
}

//=====================================
// �� 1 ��ʼ
// bFact : �Ƿ�Ϊʵ������
//         1 : ���ܾ��Ǳ�����
//         0 : �ͻ��Ǳ�����ֵ
//=====================================
CAtsString CAtsParam::GetItemData( int nIndex , BOOL bFact )
{	
	POSITION	pos;
	CAtsVarItem	*pItem = NULL;

	pos = m_listParam.FindIndex( nIndex -1 );
	if( !pos )
		return _T("");

	pItem = ( CAtsVarItem * )m_listParam.GetAt( pos );
	if( bFact && m_pEng && pItem->GetVarType() == ATSVAR_TYPE_VAR )
		return m_pEng->GetFactData( pItem->GetDataToStr(), m_pParentSegIns );
	else
		return pItem->GetDataToStr();		
}

//==================================
// ���÷��ص�����
//==================================
void CAtsParam::SetReturnData( LPCTSTR sData )
{
	m_vReturn.SetData( sData );
}

void CAtsParam::SetReturnDataEx( CAtsString strData , int nType )
{
	m_vReturn.SetDataEx( strData , nType );
}

void CAtsParam::SetReturnDataEx( int nData , int nType )
{
	m_vReturn.SetData( nData , nType );
}

//==================================
// ���÷��ص�����
//==================================
void CAtsParam::SetReturnData( int nData )
{
	SetReturnDataEx( nData , ATSVAR_TYPE_INT );
}

//==================================
// ���÷��ص�����Ϊһ������
//==================================
void CAtsParam::SetReturnData_Obj( CAtsObject *pObject )
{
	m_vReturn.SetData( (CAtsObject *)pObject );
}

void CAtsParam::AddItemToHead( CAtsValue *pValue )
{
	CAtsVarItem	 *pItem = new CAtsVarItem;
	pItem->m_vData = *pValue;
	
	m_listParam.AddHead( pItem );
}

//==================================
// ����һ������ out ������(����ֵ)
//==================================
void CAtsParam::SetOutData( int nIndex,  long nData )
{
	CAtsString		strData;

	strData.Format("%d", nData );
	SetOutData( nIndex,  strData );
}

//==================================
// ����һ������ out ������(����ֵ)
//==================================
void CAtsParam::SetOutData( int nIndex,  LPCTSTR sData )
{
	POSITION		pos;
	CAtsVarItem		*pItem = NULL;

	pos = m_listParam.FindIndex( nIndex - 1 );
	if( !pos )
		return;

	pItem = (CAtsVarItem *)m_listParam.GetAt( pos );
	if( pItem->m_strName.IsEmpty() )
	{
		// ����������ǿյ�,���ж������ǲ���һ�������,���������Ϊ������
		if( pItem->GetVarType() == ATSVAR_TYPE_VAR )
			pItem->m_strName = pItem->GetDataToStr();
		else
			return;
	}

	pItem = ( CAtsVarItem * )m_listParam.GetAt( pos );
	pItem->m_vData.SetData( sData );
	pItem->m_nFlag |= ATSVAR_FLAG_OUT;
}

//===================================================
// ���ò�����Ϣ
// ��: out String a , int b 
//===================================================
BOOL CAtsParam::SetParamInfo( CAtsString strParamInfo , CAtsEngine *pEngine )
{
	int		nLen = 0;

	strParamInfo.TrimLeft(' ');
	strParamInfo.TrimLeft(' ');

	nLen = strParamInfo.GetLength();
	if( nLen == 0 )
		return true;

	while( nLen >= 2 && strParamInfo.GetAt(0) == '(' && strParamInfo.GetAt(nLen-1) == ')' )
	{
		strParamInfo.Mid(1 , nLen - 2 );
		strParamInfo.TrimLeft(' ');
		strParamInfo.TrimLeft(' ');

		nLen = strParamInfo.GetLength();
		if( nLen == 0 )
			return true;
	}

	CAtsString		strItem;
	int				nItemLen;
	CAtsString		strFistWord,strOther;

	while( CAtsFile::GetElementItem(strParamInfo,  strItem,"," ) )
	{
		CAtsVarItem			*pItem = new CAtsVarItem;
		pItem->m_vData.m_nType 	= ATSVAR_TYPE_VARIANT;

		nItemLen = strItem.GetLength();
		if( nItemLen <= 0 )
			return false;

		CAtsFile::GetFistWord( strItem , strFistWord , strOther );
				
		// ȡ�������������
		if( strFistWord.CompareNoCase("out") == 0 )
		{
			pItem->m_nFlag |= ATSVAR_FLAG_OUT;
			CAtsFile::GetFistWord( strOther , strFistWord , strOther );
		}
		
		pItem->m_vData.m_nType = CAtsVarItem::GetVarType( strFistWord );
		if( pItem->m_vData.m_nType == ATSVAR_TYPE_UNKNOW )
		{
			if( pEngine->GetClassTemplate( strFistWord ) )
				pItem->m_vData.m_nType = ATSVAR_TYPE_ATSOBJECT;
			else
				return false;
		}

		// ���ñ�����
		pItem->m_strName = strOther;

		m_listParam.AddTail( pItem );
	}

	return true;
}

//======================================
// �ж��Ƿ��в���
//======================================
BOOL CAtsParam::IsEmpty()
{
	if( m_listParam.GetCount() <= 0 )
		return true;
	else
		return false;
}
