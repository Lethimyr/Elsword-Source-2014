#pragma once

class CX2WordBook
{
public:
	CX2WordBook(void);
	virtual ~CX2WordBook(void);


private:
	void OpenScript( const std::wstring& fileName );
	void AddWord( const std::wstring& word );
	void AddMeaning( const std::wstring& meaning );

};
