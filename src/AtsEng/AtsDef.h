//////////////////////////////////////////////////////////////////////
// author: chen lingpeng (������)
// 2021/5/1
// ver:1.8
// �������ʹ�ã����������������ϵ����(905309882@qq.com)
//////////////////////////////////////////////////////////////////////
#if !defined(ATS_DEF)
#define ATS_DEF

#include <afx.h>
#include "AtsString.h"
#include <map>
#include <vector>
using namespace std;

#if defined (ATS_HAS_DLL)
	#ifdef ATS_EXPORTS
	#define ATS_API __declspec(dllexport)
	#else
	#define ATS_API __declspec(dllimport)
	#endif
#else
#  define ATS_API
#endif  

#endif // !defined(ATS_DEF)
