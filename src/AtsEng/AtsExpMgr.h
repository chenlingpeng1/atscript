//////////////////////////////////////////////////////////////////////
// author: chen lingpeng (陈凌鹏)
// 2021/5/1
// ver:1.8
// 代码免费使用，如果用于商务，请联系作者(905309882@qq.com)
//////////////////////////////////////////////////////////////////////

#ifndef __ATSJFEVALUATE__
#define __ATSJFEVALUATE__

#include <afxtempl.h>
#include "float.h"
#include "AtsExpValue.h"
#include "AtsExpExt.h"
#include "AtscommDef.h"
#include <map>
#include <vector>
#include <list>
using namespace std;
#include "AtsDef.h"
#include "AtsObject.h"

typedef CList<int, int>								XIntList;

#define		FITEM_TYPE_NONE		(0)
#define		FITEM_TYPE_VALUE	(1)
#define		FITEM_TYPE_OP		(2)

class ATS_API CAtsExpMgr
{
private:
	enum {	ADD = '+', SUB = '-', MUL = '*', DIV = '/',
			MOD = '%', AND = '&', OR  = '|', XOR = '^',
			LT  = '<', GT  = '>', SET = '=', NOT = '!',
			NOTB= '~', DP  = ':', SQR = '?', EXP = 'e',
			DEC = 'd', BIN = 'b', HEX = 'h', CHR = 'c',	// 123, 1100b, 0x12, 'a
			RAD = 'r', DEG = '?', GON = 'g', FIX = 'f',
			CUBE= '?', NOP = '0',
		};
public:
	BOOL			m_bIsError;
	CString			m_strErrWord;
	CAtsExpExt		*m_pFEExt;

// Variable
protected:
	int						m_nPrvItemType;			// 前一个是数据
	int						m_nStrLen;
	CAtsString				m_strData;
	vector<CAtsString>		m_listSysFunName;		// 系统函数名
	//list<CAtsExpOpItem>	m_listOP;				// 符号堆载
	CObList					m_listOP;
	CPtrList				m_listValue;			// 数据堆载
	CPtrList				m_listUserVar;			// 用户定义的变量
	BOOL					GetElementItem(CAtsString &strStuff, CAtsString &strItem,CAtsString strApart);
	
	CAtsString		err;		// Fehlervariable
	double			oldValue;	// Wert der letzten Berechnung (f黵 @)
	int				out;		// Zahlensystem f黵 Ausgabe
	int				modus;		// Modus f黵 Trigonometrische Funktionen
	bool			valFlag;	// Wurde ein Wert auf den Stack gesetzt?
	bool			oldValFlag;	// = valFlag der vorherigen Schleife

public:
	void			InitSysFun();
	BOOL			ExecSysFun( CAtsExpOpItem *pOP );
	BOOL			SetVar(LPCTSTR sName , LPCTSTR strData , int nType );
	BOOL			SetVar(LPCTSTR sName , double nData , int nType );
	CAtsVarItem *	GetVarItem( CAtsString strVarName );
	BOOL			GetVarValue( CAtsString strVarName , CAtsValue *pValue );
	BOOL			NextVCharIs(int nBeginPos , char cItem );
	BOOL			TranvItemToData(CAtsValue *pvItem);
	BOOL			GetItem( int &nPos , CAtsValue &vItem  );
	BOOL			ReadString(int &nPos , CAtsString &strData );
	void			DetachList();
	BOOL			IsSysFunName(CAtsString strName);
	void			CalMastTop();
	CAtsString		ReadNumber(int &nPos );
	CAtsString		Read_FunName_Var(int &nPos );
	void			GetChar(int& nPosition, char& cItem);
	static BOOL		IsNumberVal(CAtsString strValue);
	static void		CutEndChar(CAtsString &strString, int nCount = 1 );
	static void		RemoveAllSubStr(CAtsString &strString, CAtsString strItem );

protected:
//	CAtsString itoa(int i)				{ CAtsString h; h.Format("%d", i); return h; }
	CAtsString utoa(UINT u)				{ CAtsString h; h.Format("%u", u); return h; }
	CAtsString dtoa(double d)			{ CAtsString h; h.Format("%g", d); return h; }
	CAtsString dtoa(double d, int n)	{ CAtsString h; h.Format("%.*f", n, d); return h; }


	double	ToRad(double x);
	double	ToMod(double x);
	CAtsString Reset();
	bool	TestError();


// Inlines
protected:
	bool	CalTop(int nCurOPLevel = -1 );
	void	CalTheSegment(int nCurOPLevel = -1 );
	
	void	OpPush(int oper);
	void	OpPush(int nType , CAtsString strName , int nPos = -1);
	void	OpPush_OldOP(CAtsExpOpItem *pOPItem);

	void	ValPush(CAtsString strData, int nType = ATSVAR_TYPE_STRING );
	void	ValPush(double x);
	void	ValPush(CAtsValue *pValue);
	CAtsValue *	ValPop();
	CAtsValue *	ValTop();

	CAtsExpOpItem *		OpPop(BOOL bAutoDel = false );
	CAtsExpOpItem *		OpTop();

public:
	BOOL IsValidVarChar( char cChar );
	
	CAtsExpMgr();
	virtual ~CAtsExpMgr();

	CAtsString		GetErrorInfo();
	BOOL		Computer( CAtsString strExpress , CAtsValue *pValue );
	BOOL		CalcExpress(LPCTSTR formel );
	BOOL		CheckCondition( CAtsString strExpress );
	BOOL		CmputerParam( CAtsString strExpress , CAtsParam *param );
};



#endif // __ATSJFEVALUATE__