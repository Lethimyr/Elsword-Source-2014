#include "Stdafx.h"

//{{ kimhc // 2010.3.17 //	광고 노출도
#ifdef	IGA_TEST

#include ".\X2IGA.h"

ImplementSingleton( CX2IGA );

CX2IGA::IGA_STATE CX2IGA::m_eNowState = CX2IGA::IS_NONE;
float CX2IGA::m_fElapsedTime = ELAPSED_TIME_TO_CHECK;

void CX2IGA::Init()
{
	if ( m_eNowState != IS_NONE )
	{
		ASSERT( m_eNowState != IS_NONE );
		return;
	}

	if ( iga_init() < 0 )
	{
		ASSERT( !L"pool init error" );
		return;
	}

	m_eNowState = IS_INIT;

	char szBuffer[MAX_PATH] = { 0, };

	GetCurrentDirectoryA( MAX_PATH, szBuffer );

	StringCchCatA( szBuffer, MAX_PATH, "\\iga_images" );
	
	if ( iga_set_env("ad-worker.nexon.com", "elsword", szBuffer ) < 0 )
	{
		ASSERT( !L"set env error" );
		return;
	}

	StringCchCatA( szBuffer, MAX_PATH, "\\" );

#ifdef WCHAR_DIR
	//{{ 2010/11/01 조효진 코드 수정
	string str_temp = szBuffer;
	wstring wstr_temp = KncUtil::toWideString(str_temp);
	g_pKTDXApp->GetDeviceManager()->GetMassFileManager()->AddDataDirectory( wstr_temp, true );
	//}}
#else WCHAR_DIR
	g_pKTDXApp->GetDeviceManager()->GetMassFileManager()->AddDataDirectory( szBuffer, true );
#endif WCHAR_DIR

	m_eNowState = IS_SET_ENV;

	if (iga_clear_images() < 0)
	{
		ASSERT( !L"clear image error" );
		return;
	}

	m_eNowState = IS_CLEAR_IMG;		
}

void CX2IGA::Clear()
{
	if ( m_eNowState < IS_INIT )
	{
		ASSERT( m_eNowState < IS_INIT );
		return;
	}

	iga_clear();

	m_eNowState = IS_NONE;
}

void CX2IGA::SetUser( wstring& wstrID, int iAge, bool bMan )
{
	if ( m_eNowState >= IS_SET_USER )
		return;

	string strID;
	ConvertWCHARToChar( strID, wstrID );

	char chGender = 'M';

	if ( bMan == false )
		chGender = 'F';

	if ( iga_set_user( const_cast<char*>( strID.c_str() ), iAge, chGender ) < 0 )
	{
		ASSERT( !L"set user error" );
		return;
	}

	m_eNowState = IS_SET_USER;	
}

void CX2IGA::DownLoadTexture()
{
	if ( m_eNowState < IS_SET_USER )
		return;

	int nError = 0;
	if ( ( nError = iga_get_texture( const_cast<char*>( m_strInvid.c_str() ), &m_IgaTexture, 1, 1 ) ) < 0 )
	{
		//ASSERT( !L"get texture error" );
		return;
	}

	m_eNowState = IS_DOWN_LOAD_TEX;
}

void CX2IGA::CreateIgaInfoInVillage( int iVillageID )
{
	m_vecIgaInfo.resize(0);

	switch ( iVillageID )
	{
	case SEnum::VMI_RUBEN:
		InitAdCoord( D3DXVECTOR3( 5869.224f, 574.083f, 27.408f ),
			D3DXVECTOR3( 5869.224f, 359.757f, 27.408f ),
			D3DXVECTOR3( 6223.341f, 574.159f, -205.532f ),
			D3DXVECTOR3( 6223.341f, 359.757f, -205.532f ) );
		m_strInvid = "Luben_Town@512x256";

		break;

	case SEnum::VMI_ELDER:
		InitAdCoord( D3DXVECTOR3( -1257.407f, 1570.104f, 1164.614f ),
			D3DXVECTOR3( -1257.407f, 1316.739f, 1164.614f ),
			D3DXVECTOR3( -715.684f, 1570.104f, 1167.177f ),
			D3DXVECTOR3( -715.684f, 1316.739f, 1167.177f ) );
		m_strInvid = "Elder_Town@512x256";

		break;

	case SEnum::VMI_BESMA:
		InitAdCoord( D3DXVECTOR3( 2306.145f, 6304.484f, -2705.624f ),
			D3DXVECTOR3( 2306.145f, 5991.688f, -2705.624f ),
			D3DXVECTOR3( 3167.752f, 6304.484f, -2705.624f ),
			D3DXVECTOR3( 3167.752f, 5991.688f, -2705.624f ) );
		m_strInvid = "Besma_Town@512x256";

		break;

	case SEnum::VMI_ALTERA:
		InitAdCoord( D3DXVECTOR3( 229.186f, 1916.526f, 2549.209f ),
			D3DXVECTOR3( 229.187f, 1395.251f, 2549.209f ),
			D3DXVECTOR3( 1316.055f, 1916.526f, 2549.209f ),
			D3DXVECTOR3( 1316.055f, 1395.251f, 2549.209f ) );
		m_strInvid = "Altera_Town@512x256";

		break;

	case SEnum::VMI_VELDER:
		InitAdCoord( D3DXVECTOR3( -141.022f, 762.158f, 375.303f ),
			D3DXVECTOR3( -141.022f, 528.29f, 375.303f ),
			D3DXVECTOR3( 346.598f, 762.158f, 375.303f ),
			D3DXVECTOR3( 346.598f, 528.29f, 375.303f ) );
		m_strInvid = "Belder_Capital@512x256";

		break;

	case SEnum::VMI_DUNGEON_GATE_RUBEN:
		InitAdCoord( D3DXVECTOR3( -2818.553f, 474.83f, 255.554f ),
			D3DXVECTOR3( -2818.553f, 193.991f, 255.554f ),
			D3DXVECTOR3( -2053.655f, 474.83f, 371.124f ),
			D3DXVECTOR3( -2053.655f, 193.991f, 371.124f ) );
		m_strInvid = "Luben_outside@512x256";
		break;

	case SEnum::VMI_DUNGEON_GATE_ELDER:
		InitAdCoord( D3DXVECTOR3( 1141.388f, 1707.954f, 331.734f ),
			D3DXVECTOR3( 1141.388f, 1447.29f, 331.734f ),
			D3DXVECTOR3( 1859.395f, 1707.954f, 331.734f ),
			D3DXVECTOR3( 1859.395f, 1447.29f, 331.734f ) );
		m_strInvid = "Elder_outside@512x256";
		break;

	case SEnum::VMI_DUNGEON_GATE_BESMA:
		InitAdCoord( D3DXVECTOR3( 410.299f, 1545.25f, 346.972f ),
			D3DXVECTOR3( 410.299f, 1232.454f, 346.972f ),
			D3DXVECTOR3( 1271.906f, 1545.25f, 346.972f ),
			D3DXVECTOR3( 1271.906f, 1232.454f, 346.972f ) );
		m_strInvid = "Besma_MT@512x256";
		break;

	case SEnum::VMI_DUNGEON_GATE_ALTERA:
		InitAdCoord( D3DXVECTOR3( 43.578f, 1463.127f, 439.44f ),
			D3DXVECTOR3( 43.578f, 1202.463f, 439.44f ),
			D3DXVECTOR3( 761.584f, 1463.127f, 439.44f ),
			D3DXVECTOR3( 761.584f, 1202.463f, 439.44f ) );
		m_strInvid = "Altera_outside@512x256";
		break;

	case SEnum::VMI_PEITA:
		InitAdCoord( D3DXVECTOR3( 14793.707f, 1425.825f, 197.458f ),
			D3DXVECTOR3( 14793.707f, 1165.161f, 197.458f ),
			D3DXVECTOR3( 15511.713f, 1425.825f, 197.458f ),
			D3DXVECTOR3( 15511.713f, 1165.161f, 197.458f ) );
		m_strInvid = "Peita@512x256";
		break;

	case SEnum::VMI_DUNGEON_GATE_VELDER:
		InitAdCoord( D3DXVECTOR3( -3969.28f, 462.899f, -238.78f ),
			D3DXVECTOR3( -3969.28f, 202.225f, -238.78f ),
			D3DXVECTOR3( -3251.273f, 462.899f, -238.78f ),
			D3DXVECTOR3( -3251.273f, 202.225f, -238.78f ) );
		m_strInvid = "Belder_Outside@512x256";
		break;

	case SEnum::VMI_DUNGEON_LOUNGE_RUBEN:
	case SEnum::VMI_DUNGEON_LOUNGE_ELDER:
	case SEnum::VMI_DUNGEON_LOUNGE_BESMA:
	case SEnum::VMI_DUNGEON_LOUNGE_ALTERA:
	case SEnum::VMI_DUNGEON_LOUNGE_PEITA:
	case SEnum::VMI_DUNGEON_LOUNGE_VELDER:
		m_strInvid = "";
		return;						// 이경우에는 리턴을 시켜줌
	}

	DownLoadTexture();
}

void CX2IGA::CreateIgaInfoInMarket()
{
	m_vecIgaInfo.resize(0);

	InitAdCoord( D3DXVECTOR3( 953.964f, 1575.825f, 332.672f ),
		D3DXVECTOR3( 953.964f, 1315.161f, 332.672f ),
		D3DXVECTOR3( 1671.97f, 1575.825f, 332.672f ),
		D3DXVECTOR3( 1671.97f, 1315.161f, 332.672f ) );

	InitAdCoord( D3DXVECTOR3( 3040.477f, 1575.825f, 332.672f ),
		D3DXVECTOR3( 3040.477f, 1315.161f, 332.672f ),
		D3DXVECTOR3( 3758.484f, 1575.825f, 332.672f ),
		D3DXVECTOR3( 3758.484f, 1315.161f, 332.672f ) );
	
	m_strInvid = "Market@512x256";

	DownLoadTexture();

}

void CX2IGA::Start()
{
	if ( m_eNowState != IS_DOWN_LOAD_TEX )
		return;

	iga_start();

	m_eNowState = IS_START;
}

void CX2IGA::End()
{
	if ( m_eNowState == IS_START )
	{
		iga_end();
		m_eNowState = IS_END;
	}
}

void CX2IGA::ProcessTracking( CX2Camera* pCamera, float fElapsedTime )
{
	if ( m_eNowState != IS_START )
		return;
	
	m_fElapsedTime += fElapsedTime;
	if ( m_fElapsedTime >= ELAPSED_TIME_TO_CHECK )
	{
		int				iSize		= m_vecIgaInfo.size();
		const CKTDGFrustum&	kFrustum	= g_pKTDXApp->GetDGManager()->GetFrustum();

		for ( int i = 0; i < iSize; i++ )
		{
			for ( int index = 0; index < 4; index++ )
			{
				if ( kFrustum.CheckSphere( m_vecIgaInfo[i].m_vPoint[index], 0 ) == true )
				{
					Track( m_vecIgaInfo[i], i, pCamera );
					break;
				}
			}
		}

		m_fElapsedTime = 0.0f;
	}
}

void CX2IGA::Track( IGA_INFO& IgaInfo, int iIndex, CX2Camera* pCamera )
{
	if ( iIndex < 0 || iIndex >= m_IgaTexture.count )
	{
		ASSERT( !L"iIndex is fault!" );
		return;
	}

	D3DXVECTOR3 vCamera = pCamera->GetCamera()->GetLookAt() - pCamera->GetCamera()->GetEye();
	D3DXVec3Normalize( &vCamera, &vCamera );

	D3DXMATRIX mtViewProj( g_pKTDXApp->GetViewProjectionTransform() );
	D3DXVECTOR3 vResult;
	IGA_RECT rcRect;

	for ( int i = 0; i < 4; i++ )
	{
		D3DXVec3TransformCoord( &vResult, &IgaInfo.m_vPoint[i], &mtViewProj );

		rcRect.p[i].x = (vResult.x + 1) / 2; // 투영좌표는 -1~1 사이 이지만 track 함수로 보내야하는 값은 0 ~ 1 사이임
		rcRect.p[i].y = (vResult.y + 1) / 2; // 투영좌표는 -1~1 사이 이지만 track 함수로 보내야하는 값은 0 ~ 1 사이임
	}

	iga_track( const_cast<char*>( m_strInvid.c_str() ), m_IgaTexture.texture[iIndex].filename, 
		D3DXVec3Dot( &IgaInfo.m_vNormal, &vCamera ), rcRect );
}

void CX2IGA::InitAdCoord( D3DXVECTOR3& vLT, D3DXVECTOR3& vLB, D3DXVECTOR3& vRT, D3DXVECTOR3& vRB )
{
	IGA_INFO IgaInfo;
	IgaInfo.SetPoint( vLB, vLT, vRT, vRB );

	D3DXVECTOR3 U = vLT - vLB;
	D3DXVECTOR3 V = vRB - vLB;

	D3DXVec3Cross( &IgaInfo.m_vNormal, &U, &V );
	D3DXVec3Normalize( &IgaInfo.m_vNormal, &IgaInfo.m_vNormal );

	m_vecIgaInfo.push_back( IgaInfo );
}
#endif	IGA_TEST
//}} kimhc // 2010.3.17 //	광고 노출도