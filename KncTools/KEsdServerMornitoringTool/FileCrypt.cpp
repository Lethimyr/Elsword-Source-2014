#include "StdAfx.h"
#include ".\filecrypt.h"
#include <windows.h>
#include <fstream>
#include <memory>
#include <io.h>
#include <sys/stat.h>

CFileCrypt::CFileCrypt(void)
{
}

CFileCrypt::~CFileCrypt(void)
{
}

bool CFileCrypt::FileEncrypt( std::vector<int>& vecEncryptionKey, std::string& strFileName )
{
	char pBuf[5];
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
	int iKeySize = static_cast<int>(vecEncryptionKey.size());
	int iRemainSize = fileSize.st_size;

	while( !ifs.eof() )
	{
		memset( pBuf, 0, 5 );

		// 파일버퍼 읽기
		ifs.read( pBuf, 4 );

		int iBufTemp;
		memcpy( &iBufTemp, pBuf, 4 );

		// 암호화
		int iEncryptData = iBufTemp ^ vecEncryptionKey[iKeyIndex];

		memcpy( pBuf, &iEncryptData, 4 );

		if( iRemainSize < 4 )
		{
			strCryptBuffer.append( pBuf, iRemainSize );
			break;
		}
		else
		{
			strCryptBuffer.append( pBuf, 4 );
		}

		// 남은 파일 크기 감소
		iRemainSize -= 4;

		// 4바이트씩 번갈아 가면서 암호화키 적용
		++iKeyIndex;

		if( iKeyIndex >= iKeySize )
			iKeyIndex = 0;
	}

	ofs.write( strCryptBuffer.c_str(), static_cast<int>(strCryptBuffer.size()) );

	ifs.close();
	ofs.close();

	_chmod( strFileName.c_str(), _S_IREAD | _S_IWRITE ); // 파일 삭제를 위해서

	if( CopyFileA( strEncryptedFileName.c_str(), strFileName.c_str(), false ) == 0 )
		return false;

	if( DeleteFileA( strEncryptedFileName.c_str() ) == 0 )
		return false;

	return true;	
}

char* CFileCrypt::FileDecrypt( IN std::vector<int>& vecEncryptionKey, IN std::string& strFileName )
{
	char pBuf[5];	

	std::ofstream ofs;
	std::ifstream ifs;
	std::string strCryptBuffer;
	ifs.open( strFileName.c_str(), std::ios::binary );

	if( ifs.is_open() == false )
		return NULL;

	// 파일 크기 구하기
	struct stat fileSize;
	stat( strFileName.c_str(), &fileSize );

	int iKeyIndex = 0;
	int iKeySize = static_cast<int>(vecEncryptionKey.size());
	int iRemainSize = fileSize.st_size;

	while( !ifs.eof() )
	{
		memset( pBuf, 0, 5 );

		// 파일버퍼 읽기
		ifs.read( pBuf, 4 );

		int iBufTemp;
		memcpy( &iBufTemp, pBuf, 4 );

		// 암호화
		int iEncryptData = iBufTemp ^ vecEncryptionKey[iKeyIndex];

		memcpy( pBuf, &iEncryptData, 4 );

		if( iRemainSize < 4 )
		{
			strCryptBuffer.append( pBuf, iRemainSize );
			break;
		}
		else
		{
			strCryptBuffer.append( pBuf, 4 );
		}

		// 남은 파일 크기 감소
		iRemainSize -= 4;

		// 4바이트씩 번갈아 가면서 암호화키 적용
		++iKeyIndex;

		if( iKeyIndex >= iKeySize )
			iKeyIndex = 0;
	}

	ifs.close();	

	// 힙에 버퍼 생성 후 복호화된 데이터 담기
	char* pDecryptedBuffer = new char[strCryptBuffer.size()+1];
	memset( pDecryptedBuffer, 0, strCryptBuffer.size()+1 );
	memcpy( pDecryptedBuffer, strCryptBuffer.c_str(), strCryptBuffer.size() );

	return pDecryptedBuffer;
}

char* CFileCrypt::BufferEncrypt( std::vector<int>& vecEncryptionKey, const char* pDecryptedBuffer, int iBufferSize )
{
	return CFileCrypt::BufferDecrypt( vecEncryptionKey, pDecryptedBuffer, iBufferSize );
}

char* CFileCrypt::BufferDecrypt( std::vector<int>& vecDecryptionKey, const char* pEncryptedBuffer, int iBufferSize )
{
	char pBuf[5];

	std::string strCryptBuffer;

	if( pEncryptedBuffer == NULL )
		return NULL;

	int iKeyIndex = 0;
	int iKeySize = static_cast<int>(vecDecryptionKey.size());
	int iRemainSize = iBufferSize;
	int iCryptedSize = 0;

	while(1)
	{
		memset( pBuf, 0, 5 );

		int iBufTemp;
		memcpy( &iBufTemp, pEncryptedBuffer + iCryptedSize, 4 );

		// 복호화
		int iEncryptData = iBufTemp ^ vecDecryptionKey[iKeyIndex];

		memcpy( pBuf, &iEncryptData, 4 );

		if( iRemainSize < 4 )
		{
			strCryptBuffer.append( pBuf, iRemainSize );
			break;
		}
		else
		{
			strCryptBuffer.append( pBuf, 4 );
		}		

		// 남은 버퍼 길이 감소
		iRemainSize -= 4;

		// 복호화한 길이 증가
		iCryptedSize += 4;

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