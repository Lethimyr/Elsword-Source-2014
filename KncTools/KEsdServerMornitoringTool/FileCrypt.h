#pragma once
#define IN
#define OUT
#include <vector>

class CFileCrypt
{
public:
	CFileCrypt(void);
	~CFileCrypt(void);

	// n개의 crypt key를 특정 바이트씩 교대로 4바이트 단위의 암호화
	static bool FileEncrypt( IN std::vector<int>& vecEncryptionKey, IN std::string& strFileName );
	static char* BufferEncrypt( IN std::vector<int>& vecEncryptionKey, IN const char* pDecryptedBuffer, IN int iBufferSize );

	// 복호화
	static char* FileDecrypt( IN std::vector<int>& vecEncryptionKey, IN std::string& strFileName );
	static char* BufferDecrypt( IN std::vector<int>& vecEncryptionKey, IN const char* pDecryptedBuffer, IN int iBufferSize );
};
