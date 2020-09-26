#pragma once

class CLuaCompileUtil
{
public:
	CLuaCompileUtil(void);
	~CLuaCompileUtil(void);

	static void Compile( const WCHAR* szPath, bool bSubDir, bool bSplitPath = true );
};
