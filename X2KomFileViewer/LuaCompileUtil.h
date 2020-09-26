#pragma once

class CLuaCompileUtil
{
public:
	CLuaCompileUtil(void);
	~CLuaCompileUtil(void);

	//static bool Compile( IN const std::wstring& szPath, IN const std::wstring& wstrLuacCommand, IN bool bSubDir, OUT std::map< std::wstring, MemberFileInfo >& mapLuaFiles, IN bool bCopyFile = false );
	static bool Compile( const std::wstring& szPath, const std::wstring& wstrLuacCommand, const std::wstring& wstrAnsiToUTF8Command, bool bSubDir, std::map< std::wstring, MemberFileInfo >& mapLuaFiles, bool bCopyFile = false );
    //static bool Compile( IN const std::wstring& wstrFromFilePath, IN const std::wstring& wstrToFilePath, IN const std::wstring& wstrLuacCommand );
    static bool Compile( const std::wstring& wstrFromFilePath, const std::wstring& wstrToFilePath, const std::wstring& wstrLuacCommand, const std::wstring& wstrAnsiToUTF8Command );

	static bool TextFileCopy( IN const std::wstring& szPath, IN bool bSubDir, OUT std::map< std::wstring, MemberFileInfo >& mapTextFiles );

private:
	std::wstring m_strLuaTempPath;
};
