#pragma once
#include <windows.h>
#include <fstream>
#include <memory>
#include <io.h>
#include <sys/stat.h>

#define KEY_SIZE	sizeof(T)

namespace FileCrypt
{
	// n개의 crypt key를 n바이트씩 교대로 암호화
	template< typename T >
	bool FileEncrypt( IN std::vector<T>& vecEncryptionKey, IN std::string& strFileName );

	template< typename T >
	char* BufferEncrypt( IN std::vector<T>& vecEncryptionKey, IN const char* pDecryptedBuffer, IN int iBufferSize );

	// 복호화
	template< typename T >
	char* FileDecrypt( IN std::vector<T>& vecDecryptionKey, IN const char* pEncryptedBuffer, IN int iBufferSize );
}

template< typename T >
bool FileCrypt::FileEncrypt( IN std::vector<T>& vecEncryptionKey, IN std::string& strFileName )
{
	char pBuf[KEY_SIZE + 1];
	std::string strEncryptedFileName;
	strEncryptedFileName = strFileName;
	strEncryptedFileName += ".encrypt.lua";

	std::ofstream ofs;
	std::ifstream ifs;
	std::string strCryptBuffer;
	ifs.open( strFileName.c_str(), std::ios::binary );
	ofs.open( strEncryptedFileName.c_str(), std::ios::binary );

	if( ifs.is_open() == false || ofs.is_open() == false )	
		return false;

	// 파일 크기 구하기
	struct stat fileSize;
	stat( strFileName.c_str(), &fileSize );

	int iKeyIndex = 0;
	int iKeySize = vecEncryptionKey.size();	
	int iRemainSize = fileSize.st_size;

	while( !ifs.eof() )
	{
		memset( pBuf, 0, KEY_SIZE + 1 );

		// 파일버퍼 읽기
		ifs.read( pBuf, KEY_SIZE );

		T tBufTemp;
		memcpy( &tBufTemp, pBuf, KEY_SIZE );

		// 암호화
		T tEncryptData = tBufTemp ^ vecEncryptionKey[iKeyIndex];

		memcpy( pBuf, &tEncryptData, KEY_SIZE );

		if( iRemainSize < KEY_SIZE )
		{
			strCryptBuffer.append( pBuf, iRemainSize );
			break;
		}
		else
		{
			strCryptBuffer.append( pBuf, KEY_SIZE );
		}

		// 남은 파일 크기 감소
		iRemainSize -= KEY_SIZE;

		// n바이트씩 번갈아 가면서 암호화키 적용
		++iKeyIndex;

		if( iKeyIndex >= iKeySize )
			iKeyIndex = 0;
	}

	ofs.write( strCryptBuffer.c_str(), strCryptBuffer.size() );

	ifs.close();
	ofs.close();

	_chmod( strFileName.c_str(), _S_IREAD | _S_IWRITE ); // 파일 삭제를 위해서

	if( CopyFileA( strEncryptedFileName.c_str(), strFileName.c_str(), false ) == 0 )
		return false;

	if( DeleteFileA( strEncryptedFileName.c_str() ) == 0 )
		return false;

	return true;
}

template< typename T >
char* FileCrypt::BufferEncrypt( IN std::vector<T>& vecEncryptionKey, IN const char* pDecryptedBuffer, IN int iBufferSize )
{
	return FileCrypt::FileDecrypt( vecEncryptionKey, pDecryptedBuffer, iBufferSize );
}

// 복호화
template< typename T >
char* FileCrypt::FileDecrypt( IN std::vector<T>& vecDecryptionKey, IN const char* pEncryptedBuffer, IN int iBufferSize )
{
	char pBuf[KEY_SIZE + 1];

	std::string strCryptBuffer;

	if( pEncryptedBuffer == NULL )
		return NULL;

	int iKeyIndex = 0;
	int iKeySize = vecDecryptionKey.size();
	int iRemainSize = iBufferSize;
	int iCryptedSize = 0;

	while(1)
	{
		memset( pBuf, 0, KEY_SIZE + 1 );

		T tBufTemp;
		memcpy( &tBufTemp, pEncryptedBuffer + iCryptedSize, KEY_SIZE );

		// 복호화
		T tEncryptData = tBufTemp ^ vecDecryptionKey[iKeyIndex];

		memcpy( pBuf, &tEncryptData, KEY_SIZE );

		if( iRemainSize < KEY_SIZE )
		{
			strCryptBuffer.append( pBuf, iRemainSize );
			break;
		}
		else
		{
			strCryptBuffer.append( pBuf, KEY_SIZE );
		}

		// 남은 버퍼 길이 감소
		iRemainSize -= KEY_SIZE;

		// 복호화한 길이 증가
		iCryptedSize += KEY_SIZE;

		// 4바이트씩 번갈아 가면서 복호화키 적용
		++iKeyIndex;

		if( iKeyIndex >= iKeySize )
			iKeyIndex = 0;
	}

	// 힙에 버퍼 생성 후 복호화된 데이터 담기
	char* pDecryptedBuffer = new char[strCryptBuffer.size()];
	memset( pDecryptedBuffer, 0, strCryptBuffer.size() );
	memcpy( pDecryptedBuffer, strCryptBuffer.c_str(), strCryptBuffer.size() );	

	return pDecryptedBuffer;
}