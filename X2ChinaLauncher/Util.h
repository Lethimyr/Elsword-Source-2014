#pragma once

#include <string>

void ConvertCharToWCHAR(std::wstring& dest, const char* pSrc, int iCodePage = 936 );
void ConvertWCHARToChar(std::string& dest, const std::wstring& src, int iCodePage = 936 );
