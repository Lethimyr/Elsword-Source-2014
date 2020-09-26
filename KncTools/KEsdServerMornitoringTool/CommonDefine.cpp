#include "stdafx.h"
#include "CommonDefine.h"

const wchar_t* GetAuthString( int iAuthLevel )
{
	switch( iAuthLevel )
	{
	case NORMAL:
		return L"운영자";
	case SUPER:
		return L"서버관리자";
	case ADMIN:
		return L"최종관리자";
	default:
		return L"";
	}
}