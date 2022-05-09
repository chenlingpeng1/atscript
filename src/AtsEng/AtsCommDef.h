//////////////////////////////////////////////////////////////////////
// author: chen lingpeng (陈凌鹏)
// 2021/5/1
// ver:1.8
// 代码免费使用，如果用于商务，请联系作者(905309882@qq.com)
//////////////////////////////////////////////////////////////////////

#if !defined(ATS_COMMDEF)
#define ATS_COMMDEF

// 数据类型
#define		ATSVAR_TYPE_UNKNOW		(-1)
#define		ATSVAR_TYPE_NONE		(0)
#define		ATSVAR_TYPE_VARIANT		(1)		// 变体
#define		ATSVAR_TYPE_STRING		(2)
#define		ATSVAR_TYPE_INT			(3)
#define		ATSVAR_TYPE_DOUBLE		(4)
#define		ATSVAR_TYPE_BOOL		(5)
#define		ATSVAR_TYPE_SYSSTRING	(6)			// 系统内部使用的 String

#define		ATSVAR_TYPE_ATSOBJECT	(50)
#define		ATSVAR_TYPE_VOID		(51)
#define		ATSVAR_TYPE_VARSET		(100)
#define		ATSVAR_TYPE_PARAM		(101)		// 表示为一个参数,数据存在 string 中, 可能有','
#define		ATSVAR_TYPE_OP			(102)		// 表示为一个操作符
#define		ATSVAR_TYPE_VAR			(103)		// 表示其内容为一个变量,变量名存入string 中
#define		ATSVAR_TYPE_HEX			(104)
#define		ATSVAR_TYPE_BIN			(105)
#define		ATSVAR_TYPE_CHR			(106)
#define		ATSVAR_TYPE_DEC			(107)

#define		ATSVAR_FLAG_OUT			(1)		// 数据返回	

#define		ATSOP_TYPE_OTHER		(0)		
#define		ATSOP_TYPE_BINOP		(1)		// 操作符
#define		ATSOP_TYPE_SINGLEOP		(2)		// 单数据符号(如: 负数 , ! , ~ )
#define		ATSOP_TYPE_SYSFUN		(3)		// 系统Fun
#define		ATSOP_TYPE_SEG			(4)		// 段落符,如( '(', ')' , ',' )
#define		ATSOP_TYPE_LOG			(5)		// 逻辑运算符( & , | )
#define		ATSOP_TYPE_CHECK		(6)		// 判断运算符,( AND , OR , == , >= , <= , > , < )
											// Check 的等级是最低的
#define		ATSOP_TYPE_EXTFUN		(7)		// 扩展Fun

#define		ATS_STATE_NONE		(0)
#define		ATS_STATE_TRUE		(1)
#define		ATS_STATE_FALSE		(2)
#define		ATS_STATE_ERROR		(3)

#define		ATS_ERRCODE_IF_ERROR		(1)		// IF 的语句出错
#define		ATS_ERRCODE_WHILE_ERROR		(2)		// While 的语句出错
#define		ATS_ERRCODE_SETDATA_ERROR	(3)		// Set 数据出错
#define		ATS_ERRCODE_DO_ERROR		(4)		// do 的语句出错
#define		ATS_ERRCODE_SWITCH_ERROR	(5)		// switch 的语句出错
#define		ATS_ERRCODE_FOR_ERROR		(6)		// for 的语句出错
#define		ATS_ERRCODE_EXIT_ERROR		(7)		// exit 的语句出错
#define		ATS_ERRCODE_OBJFUN_ERROR	(8)		// 执行对象 Fun 出错
#define		ATS_ERRCODE_COMPUTER_ERROR	(9)		// Computer 出错
#define		ATS_ERRCODE_SEGMENT_ERROR	(10)	// SEGMETN 出错
#define		ATS_ERRCODE_NOSEGMENT		(11)	// 执行 SEGMETN 出错
#define		ATS_ERRCODE_NOTEXISTATSOBJ	(12)	// 不存在的 AtsObject
#define		ATS_ERRCODE_NOTEXISTVAR		(13)	// 不存在的变量
#define		ATS_ERRCODE_VARTYTPE_ERROR	(14)	// 变量类型出错
#define		ATS_ERRCODE_SENTENCE_ERROR	(15)	// {}出错
#define		ATS_ERRCODE_PARAM_ERROR		(16)	// 参数出错
#define		ATS_ERRCODE_DEFINE_VAR		(17)	// 变量定义出错

#endif // !defined(ATS_COMMDEF)
