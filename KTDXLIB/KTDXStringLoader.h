#pragma once

class CKTDXStringLoader
{
	public:
		CKTDXStringLoader( void );
		~CKTDXStringLoader( void );

	public:
		bool Load( const WCHAR* pFileName );
		const std::wstring& GetString( int ID );
		std::wstring GetReplacedString(int ID, char* szParamList, ...);

	private:
		std::vector<std::wstring> m_vecString;
};
