// MyMemFile.cpp: implementation of the CMyMemFile class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MyMemFile.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMyMemFile::CMyMemFile(UINT nGrowBytes)
{
	ATLASSERT(nGrowBytes <= UINT_MAX);

	m_nGrowBytes	= nGrowBytes;
	m_nPosition		= 0;
	m_nBufferSize	= 0;
	m_nFileSize		= 0;
	m_lpBuffer		= NULL;
	m_bAutoDelete	= TRUE;
}

CMyMemFile::CMyMemFile(BYTE* lpBuffer, UINT nBufferSize, UINT nGrowBytes)
{
	ASSERT(nGrowBytes <= UINT_MAX);

	m_nGrowBytes = nGrowBytes;
	m_nPosition = 0;
	m_nBufferSize = nBufferSize;
	m_nFileSize = nGrowBytes == 0 ? nBufferSize : 0;
	m_lpBuffer = lpBuffer;
	m_bAutoDelete = FALSE;
}

void CMyMemFile::Attach(BYTE* lpBuffer, UINT nBufferSize, UINT nGrowBytes)
{
	ASSERT(m_lpBuffer == NULL);

	m_nGrowBytes	= nGrowBytes;
	m_nPosition		= 0;
	m_nBufferSize	= nBufferSize;
	m_nFileSize		= nGrowBytes == 0 ? nBufferSize : 0;
	m_lpBuffer		= lpBuffer;
	m_bAutoDelete	= FALSE;
}

BYTE* CMyMemFile::Detach()
{
	BYTE* lpBuffer = m_lpBuffer;
	m_lpBuffer = NULL;
	m_nFileSize = 0;
	m_nBufferSize = 0;
	m_nPosition = 0;

	return lpBuffer;
}

CMyMemFile::~CMyMemFile()
{
	// Close should have already been called, but we check anyway
	if (m_lpBuffer)
		Close();
	ASSERT(m_lpBuffer == NULL);

	m_nGrowBytes = 0;
	m_nPosition = 0;
	m_nBufferSize = 0;
	m_nFileSize = 0;
}

BYTE* CMyMemFile::Alloc(DWORD nBytes)
{
	return (BYTE*)malloc((UINT)nBytes);
}

BYTE* CMyMemFile::Realloc(BYTE* lpMem, DWORD nBytes)
{
	return (BYTE*)realloc(lpMem, (UINT)nBytes);
}

#pragma intrinsic(memcpy)
BYTE* CMyMemFile::Memcpy(BYTE* lpMemTarget, const BYTE* lpMemSource,	UINT nBytes)
{
	ASSERT(lpMemTarget != NULL);
	ASSERT(lpMemSource != NULL);

	return (BYTE*)memcpy(lpMemTarget, lpMemSource, nBytes);
}
#pragma function(memcpy)

void CMyMemFile::Free(BYTE* lpMem)
{
	ASSERT(lpMem != NULL);

	free(lpMem);
}

DWORD CMyMemFile::GetPosition() const
{
	return m_nPosition;
}

bool CMyMemFile::GrowFile(DWORD dwNewLen)
{
	if (dwNewLen > m_nBufferSize)
	{
		// grow the buffer
		DWORD dwNewBufferSize = (DWORD)m_nBufferSize;

		// watch out for buffers which cannot be grown!
		ASSERT(m_nGrowBytes != 0);
		if (m_nGrowBytes == 0)
			return false;

		// determine new buffer size
		while (dwNewBufferSize < dwNewLen)
			dwNewBufferSize += m_nGrowBytes;

		// allocate new buffer
		BYTE* lpNew;
		if (m_lpBuffer == NULL)
			lpNew = Alloc(dwNewBufferSize);
		else
			lpNew = Realloc(m_lpBuffer, dwNewBufferSize);

		if (lpNew == NULL)
			return false;

		m_lpBuffer = lpNew;
		m_nBufferSize = dwNewBufferSize;
	}
	return true;
}

DWORD CMyMemFile::GetLength()
{
	return m_nFileSize;
}

void CMyMemFile::SetLength(DWORD dwNewLen)
{
	if (dwNewLen > m_nBufferSize)
		GrowFile(dwNewLen);

	if (dwNewLen < m_nPosition)
		m_nPosition = dwNewLen;

	m_nFileSize = dwNewLen;
}

//==========================================
// 复制数据, Position 不发生变化
//==========================================
UINT CMyMemFile::CopyMemData(void* lpBuf , int nPos )
{
	if( m_nFileSize <= 0 )
		return 0;

	Memcpy((BYTE*)lpBuf, (BYTE*)m_lpBuffer + nPos , m_nFileSize - nPos );

	return m_nFileSize - nPos;
}

UINT CMyMemFile::Read(void* lpBuf, UINT nCount)
{
	if (nCount == 0)
		return 0;

	ASSERT(lpBuf != NULL);

	if (m_nPosition > m_nFileSize)
		return 0;

	UINT nRead;
	if (m_nPosition + nCount > m_nFileSize)
		nRead = (UINT)(m_nFileSize - m_nPosition);
	else
		nRead = nCount;

	if( nRead <= 0 )
		return nRead;

	Memcpy((BYTE*)lpBuf, (BYTE*)m_lpBuffer + m_nPosition, nRead);
	m_nPosition += nRead;

	return nRead;
}

void CMyMemFile::Write(const void* lpBuf, UINT nCount)
{
	if (nCount == 0)
		return;

	ASSERT(lpBuf != NULL);

	if (m_nPosition + nCount > m_nBufferSize)
		GrowFile(m_nPosition + nCount);

	ASSERT(m_nPosition + nCount <= m_nBufferSize);

	Memcpy((BYTE*)m_lpBuffer + m_nPosition, (BYTE*)lpBuf, nCount);

	m_nPosition += nCount;

	if (m_nPosition > m_nFileSize)
		m_nFileSize = m_nPosition;
}

LONG CMyMemFile::Seek(LONG lOff, UINT nFrom)
{
	ASSERT(nFrom == MYFILE_POS_BEGIN || nFrom == MYFILE_POS_END || nFrom == MYFILE_POS_CURRENT );

	LONG lNewPos = m_nPosition;

	if (nFrom == MYFILE_POS_BEGIN)
		lNewPos = lOff;
	else if (nFrom == MYFILE_POS_CURRENT)
		lNewPos += lOff;
	else if (nFrom == MYFILE_POS_END)
		lNewPos = m_nFileSize + lOff;
	else
		return -1;

	if (lNewPos < 0)
		return -1;

	m_nPosition = lNewPos;

	return m_nPosition;
}

void CMyMemFile::Flush()
{
}

void CMyMemFile::Close()
{
	ASSERT(m_nFileSize <= m_nBufferSize);

	m_nGrowBytes	= 0;
	m_nPosition		= 0;
	m_nBufferSize	= 0;
	m_nFileSize		= 0;
	if (m_lpBuffer && m_bAutoDelete)
		Free(m_lpBuffer);
	m_lpBuffer = NULL;
}

void CMyMemFile::Abort()
{
	Close();
}

// only CMyMemFile supports "direct buffering" interaction with CArchive
UINT CMyMemFile::GetBufferPtr(UINT nCommand, UINT nCount,
	void** ppBufStart, void**ppBufMax)
{
	ASSERT(nCommand == bufferCheck || nCommand == bufferCommit ||
		nCommand == bufferRead || nCommand == bufferWrite);

	if (nCommand == bufferCheck)
		return 1;   // just a check for direct buffer support

	if (nCommand == bufferCommit)
	{
		// commit buffer
		ASSERT(ppBufStart == NULL);
		ASSERT(ppBufMax == NULL);
		m_nPosition += nCount;
		if (m_nPosition > m_nFileSize)
			m_nFileSize = m_nPosition;
		return 0;
	}

	ASSERT(nCommand == bufferWrite || nCommand == bufferRead);
	ASSERT(ppBufStart != NULL);
	ASSERT(ppBufMax != NULL);

	// when storing, grow file as necessary to satisfy buffer request
	if (nCommand == bufferWrite && m_nPosition + nCount > m_nBufferSize)
		GrowFile(m_nPosition + nCount);

	// store buffer max and min
	*ppBufStart = m_lpBuffer + m_nPosition;

	// end of buffer depends on whether you are reading or writing
	if (nCommand == bufferWrite)
		*ppBufMax = m_lpBuffer + min(m_nBufferSize, m_nPosition + nCount);
	else
	{
		if (nCount == (UINT)-1)
			nCount = m_nBufferSize - m_nPosition;
		*ppBufMax = m_lpBuffer + min(m_nFileSize, m_nPosition + nCount);
		m_nPosition += LPBYTE(*ppBufMax) - LPBYTE(*ppBufStart);
	}

	// return number of bytes in returned buffer space (may be <= nCount)
	return LPBYTE(*ppBufMax) - LPBYTE(*ppBufStart);
}
