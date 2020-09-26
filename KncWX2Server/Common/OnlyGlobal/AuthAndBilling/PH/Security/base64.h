#ifndef _D_BASE64_H__
#define _D_BASE64_H__

#pragma warning(disable:4996)

#include "../Common/OnlyGlobal/AuthAndBilling/PH/Security/targetver.h"

#include <stdio.h>
#include <tchar.h>

#include <windows.h>
#include <string>
#include <vector>

typedef unsigned int uint32;
typedef unsigned short uint16;
typedef unsigned char uint8;
typedef std::vector<uint8> dataBuffer_t;
typedef std::string string_t;

void base64_encode(const unsigned char * bytes_to_encode, unsigned int len, string_t & ret);
void base64_decode(const string_t & encoded_string, dataBuffer_t & ret);

#endif
