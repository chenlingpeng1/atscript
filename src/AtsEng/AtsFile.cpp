// AtsFile.cpp: implementation of the CAtsFile class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AtsFile.h"
#include "AtsSegmentItem.h"
#include "AtsVarItem.h"
#include "AtsEngine.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAtsFile::CAtsFile()
{
	m_bEndFile		= false;
	m_bUserMemFile	= false;
	m_nCurLine		= 0;
}

CAtsFile::~CAtsFile()
{

}

BOOL CAtsFile::ReadFromFile( CAtsString strFileName , CAtsString &strData  )
{
	CFile		file;
	char		ch[2];
	int			nRead;

	if( !file.Open( strFileName , CFile::modeRead ) )
		return false;

	while( true )
	{
		nRead = file.Read(ch,1);
		ch[1] = '\0';
		if( nRead == 1 )
			strData = strData + ch;
		else
			break;
	}
	file.Close();
	return true;
}

BOOL CAtsFile::ReadLineFromFile( CAtsString &strData , BOOL &bIsEend )
{
	char		ch[2];
	int			nRead;
	
	bIsEend = false;
	strData.Empty();
	while( true )
	{
		if( m_bUserMemFile )
			nRead = m_MemFile.Read(ch,1);
		else
			nRead = m_IOFile.Read(ch,1);

		if( nRead == 1 )
		{
			ch[1] = '\0';
			if( ch[0] != '\r'  && ch[0] != '\n' )
				strData = strData + ch;	
			/*else if(ch == 0)
			{
				bIsEend = true;
				return true;
			}*/
			else
				return true;
		}
		else
		{
			bIsEend = true;
			return true;
		}
	}
//	file.Close();
	return true;
}

void CAtsFile::LoadScript( CAtsString strScript )
{
	m_bUserMemFile	= true;
	m_nCurLine		= 0;

	m_MemFile.Write( strScript.GetBuffer(strScript.GetLength() ) , strScript.GetLength() );
	strScript.ReleaseBuffer();
	m_MemFile.Seek( 0 , MYFILE_POS_BEGIN );
}

BOOL CAtsFile::OpenFile( CAtsString strFileName )
{
	m_nCurLine	= -1;
	if( !m_IOFile.Open( strFileName , CFile::modeRead ) )
		return false;

	m_bEndFile = false;
	return true;
}

void CAtsFile::CloseFile()
{
	if( m_bUserMemFile )
		m_MemFile.Close();
	else
		m_IOFile.Close();
}

BOOL CAtsFile::IsEndFile()
{
	return m_bEndFile;
}

//===========================================
// 是否是一行说明
//===========================================
BOOL CAtsFile::IsRem(CAtsString strLine)
{
	if( strLine.IsEmpty() )
		return false;

	if( strLine.GetLength() >= 2 && strLine.Left(2) == "//" )
		return true;
	return false;
}

//===========================================
// 得到一行有效的数据
// bCancelEmpLine : 忽略空行
//===========================================
BOOL CAtsFile::GetDataLine(CAtsString &strLine , bool bCancelEmpLine )
{
	CAtsString		strSegment;

	while( !IsEndFile() )
	{
		ReadLineFromFile( strLine , m_bEndFile );
		m_nCurLine++;

		strLine.Replace('\t' ,' ');
		strLine.TrimLeft();
		strLine.TrimRight();
		strLine.TrimLeft(' ');
		strLine.TrimRight(' ');

		if( IsRem( strLine ) )
			strLine = _T("");

		if( strLine.IsEmpty() )
		{
			if( !bCancelEmpLine )
				return true;
		}
		else
			return true;
	}
	return false;
}

//====================================================
// 判断是否为一个段的结束
//====================================================
BOOL CAtsFile::IsSegmentEnd( CAtsString strLine , int &nSegmentType )
{
	CAtsString		strFistWord,strOther;

	GetFistWord(strLine , strFistWord , strOther );
	if( strFistWord.CompareNoCase("}") == 0 )
	{
		if( strOther.CompareNoCase("Section") == 0 )
		{
			nSegmentType = SEGMENT_TYPE_SECTION;
			//AfxMessageBox("ss
			return true;
		}
		else if( strOther.CompareNoCase("Function") == 0 )
		{
			nSegmentType = SEGMENT_TYPE_FUNCTION;
			return true;
		}
		return false;
	}
	return false;
}

//====================================================
// 判断是否为一个段
//====================================================
BOOL CAtsFile::IsSegment( CAtsString strLine , CAtsString &strSegmentData , int &nSegmentType )
{
	CAtsString		strFistWord,strOther;

	GetFistWord(strLine , strFistWord , strOther );
	if( strFistWord.CompareNoCase("Section") == 0 )
	{
		nSegmentType = SEGMENT_TYPE_SECTION;
		strSegmentData = strOther;
		return true;
	}
	else if( strFistWord.CompareNoCase("Function") == 0 )
	{
		nSegmentType = SEGMENT_TYPE_FUNCTION;
		strSegmentData = strOther;
		return true;
	}
	return false;
}

//====================================================
// 判断是否为一个变量
// 格式为：　Var String strVarName
//====================================================
BOOL CAtsFile::IsVar( CAtsString strLine , CAtsString &strVarName , CString &strVarType )
{
	CAtsString		strFistWord,strOther;
	CAtsString		strData;

	if( strLine.Right(1) == ";" )
		strLine.TrimRight(";");

//	nVarType = ATSVAR_TYPE_NONE;
	GetFistWord(strLine , strFistWord , strOther );
	if( strFistWord.CompareNoCase("Var") == 0 )
	{
		GetElementItem(strOther, strData," " );
		strVarType = strData;

		GetElementItem(strOther, strData," " );
		strVarName = strData;
		
		return true;
	}

	return false;
}

//======================================================
// 得到最后得一个 Word
//======================================================
void CAtsFile::GetLastWord( CAtsString strLine, CAtsString &strLastWord, CAtsString &strOtherData )
{
	CAtsString		strData;
	int				nPos;

	nPos  = strLine.ReverseFind(' ');	
	if( nPos < 0 )
	{
		strOtherData = "";
		strLastWord = "";
		return;
	}

	strLastWord  = strLine.Mid(nPos+1);
	strOtherData = strLine.Left(nPos);

	strLastWord.TrimLeft(' ');
	strLastWord.TrimRight(' ');

	strOtherData.TrimLeft(' ');
	strOtherData.TrimRight(' ');
}

//========================================================
// 可以分开三种符号: ' ' , '(' , '='
// 如: if ... then
//     @a=3
//     Myfun()
//========================================================
void CAtsFile::GetFistWord( CAtsString strLine, CAtsString &strFistWord, CAtsString &strOtherData )
{
	int				nPos = -1;
	char			cItem;
	int				nNum,nCount;

	strLine.TrimLeft(' ');
	nCount = strLine.GetLength();
	for( nNum = 0 ; nNum < nCount ; nNum++ )
	{
		cItem = strLine.GetAt(nNum);
		if( cItem == ' ' || cItem == '(' || cItem == '=' || 
			cItem == '+' || cItem == '-' || cItem == '*' ||
		    cItem == '/' || cItem == '%')
		{
			nPos = nNum;
			break;
		}
	}
	if( nPos < 0 )
	{
		strOtherData	= _T("");
		strFistWord		= strLine;
		return;
	}

	strFistWord  = strLine.Left(nPos);
	strOtherData = strLine.Mid(nPos);

	strFistWord.TrimLeft(' ');
	strFistWord.TrimRight(' ');

	strOtherData.TrimLeft(' ');
	strOtherData.TrimRight(' ');
}

BOOL CAtsFile::GetElementItem(CAtsString &strStuff, CAtsString &strItem,CAtsString strApart)
{
	if( strStuff.GetLength() == 0 )
		return FALSE;

	int pos = strStuff.Find(strApart);
	if( pos >= 0 )
	{
		strItem = strStuff.Left(pos);
		strStuff = strStuff.Right( strStuff.GetLength() - pos - strApart.GetLength() );
		strItem.TrimLeft(' ');
		strItem.TrimRight(' ');
		strStuff.TrimLeft(' ');
		strStuff.TrimRight(' ');
	}
	else
	{
		strItem		= strStuff;
		strStuff	= "";
		strItem.TrimLeft(' ');
		strItem.TrimRight(' ');
	}
	return TRUE;
}

//====================================================
// 得到一个段
//====================================================
CAtsSegmentItem * CAtsFile::GetSegment( CAtsString strLine )
{
	CAtsString		strFistWord,strOther;
	int				nSegmentType;
	CAtsSegmentItem	*pItem = NULL;

	GetFistWord(strLine , strFistWord , strOther );
	if( strFistWord.CompareNoCase("Section") == 0 )
	{
		pItem = new CAtsSegmentItem;
		pItem->m_nSegmentType  = SEGMENT_TYPE_SECTION;
		pItem->m_strName       = strOther;
		return pItem;
	}
	// 例
	// function void funname( out @a as AtsString, @b )
	// {
	// }
	else if( strFistWord.CompareNoCase("Function") == 0 )
	{
		CAtsString		strParam;

		pItem = new CAtsSegmentItem;
		
		nSegmentType = SEGMENT_TYPE_FUNCTION;
		// 得到function的类型
		GetFistWord( strOther , strFistWord , strOther );
		pItem->SetReturnDataType( CAtsVarItem::GetVarType( strFistWord ) );

		// 得到function的名称
		GetFistWord( strOther , strFistWord , strOther );
		pItem->m_strName = strFistWord;
		if( strOther.GetAt(0) == '(' )
		{
			int		nPos = strOther.Find(')');
			if( nPos <= 0 )
			{
				delete pItem;
				return NULL;
			}
			strParam = strOther.Mid(1,nPos - 1 );
			strOther = strOther.Mid( nPos + 1 );
			strParam.TrimLeft();
			strParam.TrimRight();
			strOther.TrimLeft();
			strOther.TrimRight();
		
			// 设置 Param
			if( !strParam.IsEmpty() )
			{
				if( !pItem->m_Param.SetParamInfo( strParam , m_pEngine ) )
				{
					delete pItem;
					return NULL;
				}
			}
		}

		return pItem;
	}
	
	return NULL;
}

//=======================================================
// 是否为预编译指令
//=======================================================
BOOL CAtsFile::IsPreCompiled( CAtsString strLine , CAtsString &strCode )
{
	if( strLine.Left(1) != "#" )
		return false;

	GetElementItem(strLine, strCode,' ');
	return true;
}