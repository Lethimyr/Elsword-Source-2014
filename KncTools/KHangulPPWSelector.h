#pragma once

//#include <stdio.h>
#include <windows.h>

bool KHanSungDivProcess( WORD kUnicode, char* pcResultChar );
bool KHanSungDivWideChar( char* pcBuff, char* pcResultChar );
bool KHanSungDivUnicode( WCHAR* pcWord, char* pcResultChar );

bool KHanSungPostWordWideChar( const char* pcBuff_ );
bool KHanSungPostWordUnicode( const WCHAR* pcWord_ );