/*------------------------------------
 *
 * X2SimpleEncryption.h
 * 
 * 2013. 12. Im Cheolsu.
 *
 * Desc : 외부에서 데이터값을 검색할 수 없게 데이터를 변경(암호화)
 *------------------------------------
 */

#pragma once



class CX2SimpleEncryption
{
public:
	CX2SimpleEncryption();
	virtual ~CX2SimpleEncryption();

	vector<char> EncrypteData( const char * data, size_t length );
	vector<char> DencrypteData( const char * data, size_t length );

	static string GetString( const vector<char> & data );

private:
	bool _IsEqualData( const char * data1, const char * data2, size_t length );
};

