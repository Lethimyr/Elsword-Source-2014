#pragma once

#include <Windows.h>
#include <string>
#include <vector>

class KAutoPath
{
public:
	//{{ 2009. 7. 13  최육사	하드코딩 제거
	static void AddPath( const std::string& strPath ); // 참조 경로 추가 함수
	//}}

public:
	KAutoPath();
	~KAutoPath(void);

	bool	GetPullPath( std::wstring& wstrFileName );
	bool	GetPullPath( std::string& strFileName );

private:
	void	InitDirectory( std::string strCurDir );

private:
	std::vector< std::string >	m_vecDirectory;
};