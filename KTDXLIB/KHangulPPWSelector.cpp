#include "StdAfx.h"

#include "KHangulPPWSelector.h"

#ifndef	CONVERSION_VS
	#define _countof(array) (sizeof(array)/sizeof(array[0]))
#endif	CONVERSION_VS

bool KHanSungDivProcess( WORD kUnicode, char* pcResultChar, const int iSizeOfResult_ )
{
	int  iChoSung, iJungSung, iJongSung;

	static char acChoSungTbl[] = "ㄱㄲㄴㄷㄸㄹㅁㅂㅃㅅㅆㅇㅈㅉㅊㅋㅌㅍㅎ";
	static char acJungSungTbl[] = "ㅏㅐㅑㅒㅓㅔㅕㅖㅗㅘㅙㅚㅛㅜㅝㅞㅟㅠㅡㅢㅣ";
	static char acJongSungTbl[] = "  ㄱㄲㄳㄴㄵㄶㄷㄹㄺㄻㄼㄽㄾㄿㅀㅁㅂㅄㅅㅆㅇㅈㅊㅋㅌㅍㅎ";

	static int iUnicodeBase = 0xAC00;
	static int iUnicodeLast = 0xD79F;

	// 한글이 범위에서 벗어나면 
	if (kUnicode < iUnicodeBase || kUnicode > iUnicodeLast)
		return false;

	kUnicode = kUnicode - iUnicodeBase;
	iChoSung = kUnicode / (21 * 28);
	kUnicode = kUnicode % (21 * 28);
	iJungSung = kUnicode / 28;
	kUnicode = kUnicode % 28;
	iJongSung = kUnicode;

	if ( pcResultChar != NULL )
	{
		int nX = 0;	

#ifdef	CONVERSION_VS
		strncpy_s( &pcResultChar[nX], iSizeOfResult_ - nX, acChoSungTbl + iChoSung * 2 , 2); nX+=2;
		strncpy_s( &pcResultChar[nX], iSizeOfResult_ - nX, acJungSungTbl + iJungSung * 2, 2 ); nX+=2;
		strncpy_s( &pcResultChar[nX], iSizeOfResult_ - nX, acJongSungTbl + iJongSung * 2, 2 ); 
#else	CONVERSION_VS
		strncpy( &pcResultChar[nX] , acChoSungTbl + iChoSung * 2 , 2); nX+=2;
		strncpy( &pcResultChar[nX] , acJungSungTbl + iJungSung * 2, 2 ); nX+=2;
		strncpy( &pcResultChar[nX] , acJongSungTbl + iJongSung * 2, 2 ); 
#endif	CONVERSION_VS
		
	}

	return true;
}

bool KHanSungDivWideChar( char* pcBuff, char* pcResultChar, const int iSizeOfResult_  )
{
	WCHAR pcUnicode[1] = {0};

	// 해당 글자를 이용해서 유니코드 번호를 얻는다.
	MultiByteToWideChar( CP_ACP, MB_PRECOMPOSED, pcBuff, 2, pcUnicode, 1 );

	return KHanSungDivProcess( (WORD) *pcUnicode, pcResultChar, iSizeOfResult_ );
}

bool KHanSungDivUnicode( WCHAR* pcWord, char* pcResultChar, const int ISizeOfResult )
{
	return KHanSungDivProcess( pcWord[0], pcResultChar, ISizeOfResult );
}

bool KHanSungPostWordWideChar( const char* pcBuff_ )
{
	char* pcBuff = (char*) pcBuff_;
	int iLen = (int) strlen( pcBuff );	

	char szResult[7] = {0};
	if ( KHanSungDivWideChar( &pcBuff[iLen - 2], szResult, _countof(szResult) ) == false )
		return false;

	return !( szResult[4] == ' ' && szResult[5] == ' ' );
}

bool KHanSungPostWordUnicode( const WCHAR* pcWord_ )
{
	WCHAR* pcWord = (WCHAR*) pcWord_;
	int iLen = (int) wcslen( pcWord );	

	char szResult[7] = {0};
	if ( KHanSungDivUnicode( &pcWord[iLen - 1], szResult, _countof(szResult) ) == false )
		return false;

	return !( szResult[4] == ' ' && szResult[5] == ' ' );
}


int KHanSungPostWordUnicodeWrapper( const WCHAR* pcWord_, int eTrueReturn, int eFalseReturn )
{
#ifdef _FOREIGN_
	return STR_ID_EMPTY;
#endif
	return KHanSungPostWordUnicode( pcWord_ ) ? eTrueReturn : eFalseReturn;
}