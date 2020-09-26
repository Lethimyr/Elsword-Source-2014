#include ".\usersecuritymanager.h"
//{{ 2011. 09. 06	최육사	머신 ID 기능 업데이트
#ifdef SERV_MACHINE_ID_BLOCK_UPDATE
	#include "Base64/base64.h"
#endif SERV_MACHINE_ID_BLOCK_UPDATE
//}}


//{{ 2011. 08. 17	최육사	머신ID 중복 접속 차단
#ifdef SERV_MACHINE_ID_BLOCK


KUserSecurityManager::KUserSecurityManager(void)
{
	//{{ 2011. 09. 14  김민성	해킹 프로세스 목록 전달 - 게임 로딩 이전 단계
#ifdef SERV_DLL_LIST_CHECK_BEFOR_LOADING
	m_iChannelRandomKey	= 0;
#endif SERV_DLL_LIST_CHECK_BEFOR_LOADING
	//}}
}

KUserSecurityManager::~KUserSecurityManager(void)
{
}

const std::string KUserSecurityManager::GetStringMachineID() const
{
	std::string strResult = KncUtil::toNarrowString( m_wstrMachineID );
	return strResult;
}

void KUserSecurityManager::SetMachineID( IN const std::string& strMachineID )
{
    m_wstrMachineID = KncUtil::toWideString( strMachineID );
}

WORD KUserSecurityManager::GetHashFromMachineID( IN const std::string& strMachineID )
{
	if( strMachineID.size() < 16 )
	{
		return 0;
	}

	WORD wHash = *(WORD*)&strMachineID[14];
	return wHash;
}

WORD KUserSecurityManager::MakeHashFromMachineID( IN const std::string& strMachineID )
{
	if( strMachineID.size() < 16 )
	{
		return 0;
	}

	// 해쉬값 체크!
	BYTE MachineId[16] = {0, };

	for( int i = 0; i < 16; ++i )
	{
		MachineId[i] = strMachineID[i];
	}

	// fills Mac Address, Serial Number.
	WORD wHash = 0;
	for (int i=0; i<6; ++i)
	{
		wHash += ((WORD*)MachineId)[i] * (i+3);
	}

	return wHash;
}

bool KUserSecurityManager::CheckValidMachineID( IN const std::string& strMachineID ) const
{
	// 머신ID가 비어있는지 검사!
	if( strMachineID.empty() == true )
		return false;

	// 머신ID 길이 검사
	if( strMachineID.size() < 16 )
		return false;

	//{{ 2011. 09. 06	최육사	머신 ID 기능 업데이트
#ifdef SERV_MACHINE_ID_BLOCK_UPDATE
	//machine id 유효성 검사 체크 필요
	//위코드 처럼 구현하면 됨.. 단 base64 decode후 검사해야 함
	std::string strDecodingMachineID = base64_decode( strMachineID );

	if( GetHashFromMachineID( strDecodingMachineID ) != MakeHashFromMachineID( strDecodingMachineID ) )
		return false;
#endif SERV_MACHINE_ID_BLOCK_UPDATE
	//}}

    return true;
}

#endif SERV_MACHINE_ID_BLOCK
//}}


