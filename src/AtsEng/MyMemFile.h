// MyMemFile.h: interface for the CMyMemFile class.
//
//////////////////////////////////////////////////////////////////////
// author: chen lingpeng (陈凌鹏)
// 2021/5/1
// ver:1.8
// 代码免费使用，如果用于商务，请联系作者(905309882@qq.com)
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MYMEMFILE_H__C2B14E22_1FEE_4308_8A56_D368F05F3B45__INCLUDED_)
#define AFX_MYMEMFILE_H__C2B14E22_1FEE_4308_8A56_D368F05F3B45__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define	MYFILE_POS_BEGIN		(1)
#define	MYFILE_POS_END			(2)
#define	MYFILE_POS_CURRENT		(3)

class CMyMemFile 
{
public:
	CMyMemFile(UINT nGrowBytes = 1024);
	CMyMemFile(BYTE* lpBuffer, UINT nBufferSize, UINT nGrowBytes = 0);

	virtual ~CMyMemFile();

// Operations
	void Attach(BYTE* lpBuffer, UINT nBufferSize, UINT nGrowBytes = 0);
	BYTE* Detach();

// Advanced Overridables
protected:
	virtual BYTE* Alloc(DWORD nBytes);
	virtual BYTE* Realloc(BYTE* lpMem, DWORD nBytes);
	virtual BYTE* Memcpy(BYTE* lpMemTarget, const BYTE* lpMemSource, UINT nBytes);
	virtual void Free(BYTE* lpMem);
	virtual bool GrowFile(DWORD dwNewLen);

public:
	DWORD m_nPosition;

// Implementation
protected:
	UINT m_nGrowBytes;

	DWORD m_nBufferSize;
	DWORD m_nFileSize;
	BYTE* m_lpBuffer;
	BOOL m_bAutoDelete;

public:
	virtual	UINT	CopyMemData(void* lpBuf , int nPos = 0 );
	virtual DWORD	GetPosition() const;
	virtual LONG	Seek(LONG lOff, UINT nFrom);
	virtual void	SetLength(DWORD dwNewLen);
	virtual DWORD	GetLength();
	virtual UINT	Read(void* lpBuf, UINT nCount);
	virtual void	Write(const void* lpBuf, UINT nCount);
	virtual void	Abort();
	virtual void	Flush();
	virtual void	Close();
	virtual UINT	GetBufferPtr(UINT nCommand, UINT nCount = 0,
		void** ppBufStart = NULL, void** ppBufMax = NULL);

	enum BufferCommand { bufferRead, bufferWrite, bufferCommit, bufferCheck };
};

#endif // !defined(AFX_MYMEMFILE_H__C2B14E22_1FEE_4308_8A56_D368F05F3B45__INCLUDED_)
