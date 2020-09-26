
#ifndef _SERVERSNMAKER_H_
#define _SERVERSNMAKER_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <windows.h>
#include <string>

//{{ 2011.11.02     김민성    서버에서 SN 발급 및 유효성 검사
//#ifdef SERV_SERIAL_NUMBER_AVAILABILITY_CHECK


/** 
@class : KServerSNMaker
@brief : 게임 클라이언트에 대한 해킹 체크를 위해 서버에서 SN을 발급하고 관리하는 클래스
@date : 2012/09/28
*/
class KServerSNMaker
{
public:
    static KServerSNMaker& GetInstance( void );

    static std::string GetCRCFromString( const std::string& str );
	static bool MakeServerSN( IN __int64& iSN, OUT unsigned char* pcharData );
	static bool CheckServerSN( IN unsigned char* pcharData, OUT __int64& iSN );

	static bool ShuffleData( IN OUT unsigned char* pcharData );
	static bool UnShuffleData( IN OUT unsigned char* pcharData );
	static bool InsertString_ServerSN( IN __int64& iSN, IN OUT unsigned char* pcharData );
	static bool InsertString_CheckSum( IN std::string& strCheckSum, IN OUT unsigned char* pcharData );
	static bool Extract_ServerSN( IN unsigned char* pcharData, OUT std::string& strSN, OUT __int64& iSN );
	static bool Extract_CheckSum( IN unsigned char* pcharData, OUT std::string& strCheckSum );
	static bool Change_CheckSum_8byteTo4byte( IN OUT std::string& strSN );
	static unsigned char Hex2Dec( unsigned char hex);
	static bool IsEmptyServerSN( IN unsigned char* pcharData );

private:
    
    KServerSNMaker( void )
    {
        // This is the official polynomial used by CRC-32 
        // in PKZip, WinZip and Ethernet. 
        ULONG ulPolynomial = 0x04C11DB7;

        // 256 values representing ASCII character codes.
        for (int i = 0; i <= 0xFF; i++)
        {
            Table[i] = Reflect(i, 8) << 24;
            for (int j = 0; j < 8; j++)
                Table[i] = (Table[i] << 1) ^ (Table[i] & (1 << 31) ? ulPolynomial : 0);
            Table[i] = Reflect(Table[i], 32);
        }
    }

public:
   
    inline DWORD CalcCRC( const void* buffer, UINT size ) const
    {
        // check the validity of the data
        if (!buffer || !size)
            return 0;

        // calculate CRC directly
        DWORD CRC = 0xFFFFFFFF;
        Calculate(reinterpret_cast<const BYTE*>( buffer ), size, CRC);
        return CRC ^ 0xFFFFFFFF;
    }

private:
   
    inline void Calculate( const BYTE* buffer, UINT size, ULONG& CRC ) const
    {
        // calculate the CRC
        const BYTE* pbyte = buffer;

        while (size--)
            CRC = (CRC >> 8) ^ Table[(CRC & 0xFF) ^ *pbyte++];
    }

    inline ULONG Reflect( ULONG ref, char ch ) const
    {
        ULONG value = 0;

        // Swap bit 0 for bit 7
        // bit 1 for bit 6, etc.
        for(int i = 1; i < (ch + 1); i++)
        {
            if (ref & 1)
                value |= 1 << (ch - i);
            ref >>= 1;
        }
        return value;
    }

private:
    ULONG Table[256];
};

//#endif SERV_SERIAL_NUMBER_AVAILABILITY_CHECK
//}}

#ifdef CRC
#undef CRC
#endif

#define CRC (KServerSNMaker::GetInstance())

#endif // _SERVERSNMAKER_H_
