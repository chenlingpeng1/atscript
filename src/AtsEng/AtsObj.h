// AMObj.h: interface for the CAtsObj class.
//
//////////////////////////////////////////////////////////////////////
// author: chen lingpeng (������)
// 2021/5/1
// ver:1.8
// �������ʹ�ã����������������ϵ����(905309882@qq.com)
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AMOBJ_H__A6D5AF01_203D_4E6A_AF6A_FD0BF7F44A26__INCLUDED_)
#define AFX_AMOBJ_H__A6D5AF01_203D_4E6A_AF6A_FD0BF7F44A26__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//////////////////////////////////////////////////////
// CAtsObj
// ������Ҫ���ü�������Ļ��࣬����ʵ�����ü���
//////////////////////////////////////////////////////
class  CAtsObj
{
public:
	long Release();
	long AddRef();

	CAtsObj();
	virtual ~CAtsObj();

private:
	long m_nRef;
};

#endif // !defined(AFX_AMOBJ_H__A6D5AF01_203D_4E6A_AF6A_FD0BF7F44A26__INCLUDED_)
