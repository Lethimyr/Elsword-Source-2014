#include "StdAfx.h"
#include ".\x2checksummanager.h"

CX2CheckSumManager::CX2CheckSumManager(void)
{
#ifdef _SERVICE_
//	OpenScript( L"CheckSumFileList.lua" );
//	CheckSumFileProcess();
#endif
}

CX2CheckSumManager::~CX2CheckSumManager(void)
{
}


void CX2CheckSumManager::Handler_EGS_CHECK_SUM_REQ( bool bCheckFile /* = false */ )
{
	map< wstring, wstring > mapCheckSum;

	if ( bCheckFile == true )
	{
		mapCheckSum = m_mapCheckSum;
	}

	DWORD protectCheckSum = CRC_32::GetInstance().CalcCRC( ProtectTable::g_kProtectTable, ProtectTable::g_iTableSize );

	KEGS_CHECK_SUM_REQ kPacket;

	kPacket.m_mapCheckSum = mapCheckSum;
	kPacket.m_ProtectTableCSVal = protectCheckSum;

	g_pData->GetServerProtocol()->SendPacket( EGS_CHECK_SUM_REQ, kPacket );
	g_pMain->AddServerPacket( EGS_CHECK_SUM_ACK );
}

bool CX2CheckSumManager::OpenScript( const WCHAR* pFileName )
{
//{{ robobeg : 2008-10-28
	//KLuaManager kLuaManager;
    KLuaManager kLuaManager( g_pKTDXApp->GetLuaBinder()->GetLuaState(), 0, true );
//}} robobeg : 2008-10-28
	g_pKTDXApp->LoadAndDoMemory( &kLuaManager, pFileName );

	if( kLuaManager.BeginTable( "CheckSumFileList" ) == true )
	{
		int tableIndex = 1;
		wstring fileName = L"";
		while( kLuaManager.GetValue( tableIndex, fileName ) == true )
		{
			MakeUpperCase( fileName );
			m_vecCheckSumFileName.push_back( fileName );
			tableIndex++;
		}

		kLuaManager.EndTable();
	}
	else
	{
		MessageBox( NULL, GET_STRING( STR_ID_61 ), GET_STRING( STR_ID_62 ), MB_OK );
		return false;
	}

	return true;
}


void CX2CheckSumManager::CheckSumFileProcess()
{
	m_mapCheckSum.clear();

	for ( int i = 0; i < (int)m_vecCheckSumFileName.size(); i++ )
	{
		wstring fileName = m_vecCheckSumFileName[i];
		WCHAR buff[MAX_PATH];
		memset( buff, 0, 256*sizeof(WCHAR) );
		memcpy( buff, fileName.c_str(), fileName.size()*sizeof(WCHAR) );
		g_pKTDXApp->GetDeviceManager()->GetMassFileManager()->GetPullPath( buff );

		string strFileName;
		ConvertWCHARToChar( strFileName, buff );

		string strCheckSum = CRC_32::GetCRCFromFile( strFileName.c_str() );
		wstring checkSum;
		ConvertCharToWCHAR( checkSum, strCheckSum.c_str() );

		MakeUpperCase( fileName );
		m_mapCheckSum.insert( make_pair( fileName, checkSum ) );
	}	
}

