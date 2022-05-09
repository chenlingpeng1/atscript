//////////////////////////////////////////////////////////////////////
// author: chen lingpeng (������)
// 2021/5/1
// ver:1.8
// �������ʹ�ã����������������ϵ����(905309882@qq.com)
//////////////////////////////////////////////////////////////////////

#if !defined(ATS_COMMDEF)
#define ATS_COMMDEF

// ��������
#define		ATSVAR_TYPE_UNKNOW		(-1)
#define		ATSVAR_TYPE_NONE		(0)
#define		ATSVAR_TYPE_VARIANT		(1)		// ����
#define		ATSVAR_TYPE_STRING		(2)
#define		ATSVAR_TYPE_INT			(3)
#define		ATSVAR_TYPE_DOUBLE		(4)
#define		ATSVAR_TYPE_BOOL		(5)
#define		ATSVAR_TYPE_SYSSTRING	(6)			// ϵͳ�ڲ�ʹ�õ� String

#define		ATSVAR_TYPE_ATSOBJECT	(50)
#define		ATSVAR_TYPE_VOID		(51)
#define		ATSVAR_TYPE_VARSET		(100)
#define		ATSVAR_TYPE_PARAM		(101)		// ��ʾΪһ������,���ݴ��� string ��, ������','
#define		ATSVAR_TYPE_OP			(102)		// ��ʾΪһ��������
#define		ATSVAR_TYPE_VAR			(103)		// ��ʾ������Ϊһ������,����������string ��
#define		ATSVAR_TYPE_HEX			(104)
#define		ATSVAR_TYPE_BIN			(105)
#define		ATSVAR_TYPE_CHR			(106)
#define		ATSVAR_TYPE_DEC			(107)

#define		ATSVAR_FLAG_OUT			(1)		// ���ݷ���	

#define		ATSOP_TYPE_OTHER		(0)		
#define		ATSOP_TYPE_BINOP		(1)		// ������
#define		ATSOP_TYPE_SINGLEOP		(2)		// �����ݷ���(��: ���� , ! , ~ )
#define		ATSOP_TYPE_SYSFUN		(3)		// ϵͳFun
#define		ATSOP_TYPE_SEG			(4)		// �����,��( '(', ')' , ',' )
#define		ATSOP_TYPE_LOG			(5)		// �߼������( & , | )
#define		ATSOP_TYPE_CHECK		(6)		// �ж������,( AND , OR , == , >= , <= , > , < )
											// Check �ĵȼ�����͵�
#define		ATSOP_TYPE_EXTFUN		(7)		// ��չFun

#define		ATS_STATE_NONE		(0)
#define		ATS_STATE_TRUE		(1)
#define		ATS_STATE_FALSE		(2)
#define		ATS_STATE_ERROR		(3)

#define		ATS_ERRCODE_IF_ERROR		(1)		// IF ��������
#define		ATS_ERRCODE_WHILE_ERROR		(2)		// While ��������
#define		ATS_ERRCODE_SETDATA_ERROR	(3)		// Set ���ݳ���
#define		ATS_ERRCODE_DO_ERROR		(4)		// do ��������
#define		ATS_ERRCODE_SWITCH_ERROR	(5)		// switch ��������
#define		ATS_ERRCODE_FOR_ERROR		(6)		// for ��������
#define		ATS_ERRCODE_EXIT_ERROR		(7)		// exit ��������
#define		ATS_ERRCODE_OBJFUN_ERROR	(8)		// ִ�ж��� Fun ����
#define		ATS_ERRCODE_COMPUTER_ERROR	(9)		// Computer ����
#define		ATS_ERRCODE_SEGMENT_ERROR	(10)	// SEGMETN ����
#define		ATS_ERRCODE_NOSEGMENT		(11)	// ִ�� SEGMETN ����
#define		ATS_ERRCODE_NOTEXISTATSOBJ	(12)	// �����ڵ� AtsObject
#define		ATS_ERRCODE_NOTEXISTVAR		(13)	// �����ڵı���
#define		ATS_ERRCODE_VARTYTPE_ERROR	(14)	// �������ͳ���
#define		ATS_ERRCODE_SENTENCE_ERROR	(15)	// {}����
#define		ATS_ERRCODE_PARAM_ERROR		(16)	// ��������
#define		ATS_ERRCODE_DEFINE_VAR		(17)	// �����������

#endif // !defined(ATS_COMMDEF)
