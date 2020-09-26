#include "StdAfx.h"
#include ".\ktdgpicchar.h"

CKTDGPicChar::CKTDGPicChar( const WCHAR* pPicCharGroup, CKTDGParticleSystem::CParticleEventSequence* pEff )
: m_fElapsedTime( 0.f )
{
	m_StrDataList.reserve( 8 );
	
	m_pParticleSystem = NULL;
	m_hSeqPicChar = INVALID_PARTICLE_SEQUENCE_HANDLE;
	if( NULL != pEff )
	{
		m_hSeqPicChar = pEff->GetHandle();
		m_pParticleSystem = pEff->GetParticleSystem();
	}


	m_fWidth	= -1.0f;
	m_bUseColor	= false;
	m_Color		= 0xffffffff;

	for( int i = 0; i < MAX_PIC_CHAR_GROUP_NUM; i++ )
	{
		m_PicCharTexList[i] = NULL;
	}

	LoadPicCharGroup( pPicCharGroup );
}

CKTDGPicChar::CKTDGPicChar( const WCHAR* pPicCharGroup, CKTDGParticleSystem::CParticleEventSequence* pEff, wstring charList )
: m_fElapsedTime( 0.f )
{
	m_pParticleSystem = NULL;
	m_hSeqPicChar = INVALID_PARTICLE_SEQUENCE_HANDLE;
	if( NULL != pEff )
	{
		m_hSeqPicChar = pEff->GetHandle();
		m_pParticleSystem = pEff->GetParticleSystem();
	}

	m_fWidth	= -1.0f;
	m_bUseColor	= false;
	m_Color		= 0xffffffff;

	for( int i = 0; i < MAX_PIC_CHAR_GROUP_NUM; i++ )
	{
		m_PicCharTexList[i] = NULL;
	}

	LoadPicCharGroup( pPicCharGroup, charList );
}

CKTDGPicChar::CKTDGPicChar( CKTDGPicChar* pPicChar, CKTDGParticleSystem::CParticleEventSequence* pEff )
: m_fElapsedTime( 0.f )
{
	m_pParticleSystem = NULL;
	m_hSeqPicChar = INVALID_PARTICLE_SEQUENCE_HANDLE;
	if( NULL != pEff )
	{
		m_hSeqPicChar = pEff->GetHandle();
		m_pParticleSystem = pEff->GetParticleSystem();
	}

	m_fWidth	= -1.0f;
	m_bUseColor	= false;
	m_Color		= 0xffffffff;

	for( int i = 0; i < MAX_PIC_CHAR_GROUP_NUM; i++ )
	{
		m_PicCharTexList[i] = NULL;
	}

	LoadPicCharGroup( pPicChar );
}

CKTDGPicChar::~CKTDGPicChar(void)
{
	m_StrDataList.clear();

	if( INVALID_PARTICLE_SEQUENCE_HANDLE != m_hSeqPicChar )
	{
		if( NULL != m_pParticleSystem )
		{
			m_pParticleSystem->DestroyInstanceHandle( m_hSeqPicChar );
		}
	}

	//for( int i = 0; i < MAX_PIC_CHAR_GROUP_NUM; i++ )
	//{
		//CKTDXDeviceTexture* pCKTDXDeviceTexture = m_PicCharTexList[i];
		//SAFE_CLOSE( pCKTDXDeviceTexture );
	//}
}

void CKTDGPicChar::OnFrameMove( float fElapsedTime )
{
	m_fElapsedTime = fElapsedTime;

	for( int i = 0; i < (int)m_StrDataList.size(); i++ )
	{
		StrData* pStrData = &m_StrDataList[i];
		if( pStrData->m_wStr.size() == pStrData->m_NowIndex )
		{
			m_StrDataList.erase( m_StrDataList.begin() + i );
			i--;
			continue;
		}

		pStrData->m_fNowRemainTime += fElapsedTime;
		if( pStrData->m_fNowRemainTime >= pStrData->m_fMaxRemainTime )
		{
			DrawText( pStrData->m_wStr.c_str(), pStrData->m_NowIndex, pStrData->m_Pos, pStrData->m_DirVec, pStrData->m_AlineType );
			pStrData->m_fNowRemainTime = 0.0f;
			pStrData->m_NowIndex++;
		}
	}
}

void CKTDGPicChar::DrawText( const WCHAR* pStr, D3DXVECTOR3 pos, D3DXVECTOR3 dirVec, ALINE_TYPE alineType, float fGapTime )
{
	if( NULL == m_pParticleSystem )
		return;


	string str;
	ConvertWCHARToChar( str, pStr );
	dirVec.y = 0.0f;


	if( fGapTime == 0.0f )
	{
		CKTDGParticleSystem::CParticleEventSequence* pSeq = m_pParticleSystem->GetInstanceSequence( m_hSeqPicChar );
		if( pSeq != NULL )
		{
			for( int i = 0; i < (int)str.size(); i++ )
			{
				const char ch = str.at(i);
				if( ch < MAX_PIC_CHAR_GROUP_NUM )
				{
                    float fWidth = m_fWidth;
					CKTDGParticleSystem::CParticle* pParticle = pSeq->CreateNewParticle( pos );
                    if ( pParticle != NULL )
                    {
					    if( fWidth == -1.0f )
						    fWidth = pParticle->GetSize().x;
					    pParticle->SetTextureID( ch );
					    if( m_bUseColor == true )
						    pParticle->SetColor( m_Color );
                    }
					if( i == 0 )
					{
						switch( alineType )
						{
						case AT_LEFT:
							break;

						case AT_RIGHT:
							pos -= dirVec * (fWidth * str.size());
							break;

						case AT_CENTER:
							pos -= dirVec * (fWidth * str.size() / 2);
							break;
						}			
                        if ( pParticle != NULL )
						    pParticle->SetPos( pos );
					}
					pos += dirVec * fWidth;
				}			
			}
		}
	}
	else
	{
		const char ch = str.at(0);
		if( ch < MAX_PIC_CHAR_GROUP_NUM )
		{
			CKTDGParticleSystem::CParticleEventSequence* pSeq = m_pParticleSystem->GetInstanceSequence( m_hSeqPicChar );
			if( pSeq != NULL )
			{
                float fWidth = m_fWidth;
				CKTDGParticleSystem::CParticle* pParticle = pSeq->CreateNewParticle( pos );
                if ( pParticle != NULL )
                {
				    if( fWidth == -1.0f )
					    fWidth = pParticle->GetSize().x;
				    pParticle->SetTextureID( ch );
				    if( m_bUseColor == true )
					    pParticle->SetColor( m_Color );
                }
				switch( alineType )
				{
				case AT_LEFT:
					break;
				case AT_RIGHT:
					pos -= dirVec * (fWidth * str.size());
					break;
				case AT_CENTER:
					pos -= dirVec * (fWidth * str.size() / 2);
					break;
				}
                if ( pParticle != NULL )
                {
				    pParticle->SetPos( pos );
                }
			}
		}

		StrData strData;
		strData.m_wStr				= pStr;
		strData.m_AlineType			= alineType;
		strData.m_Pos				= pos;
		strData.m_DirVec			= dirVec;
		strData.m_NowIndex			= 1;
		strData.m_fNowRemainTime	= 0.0f;
		strData.m_fMaxRemainTime	= fGapTime;

		m_StrDataList.push_back( strData );
	}
}

void CKTDGPicChar::DrawText( const WCHAR* pStr, int index, D3DXVECTOR3 pos, D3DXVECTOR3 dirVec, ALINE_TYPE alineType )
{

	if( NULL == m_pParticleSystem )
		return;

	string str;
	ConvertWCHARToChar( str, pStr );


	const char ch = str.at(index);	
	if( ch < MAX_PIC_CHAR_GROUP_NUM )
	{
		CKTDGParticleSystem::CParticleEventSequence* pSeq = m_pParticleSystem->GetInstanceSequence( m_hSeqPicChar );
		if( pSeq != NULL )
		{
			CKTDGParticleSystem::CParticle* pParticle = pSeq->CreateNewParticle( pos );
			float fWidth = m_fWidth;
			if( fWidth == -1.0f )
				fWidth = pParticle->GetSize().x;
			pParticle->SetTextureID( ch );
			if( m_bUseColor == true )
				pParticle->SetColor( m_Color );

			pos += dirVec * (fWidth * (index));
			pParticle->SetPos( pos );
		}
	}	
}

void CKTDGPicChar::DrawText( const int number, D3DXVECTOR3 pos, D3DXVECTOR3 dirVec, ALINE_TYPE alineType, float fGapTime )
{
	WCHAR buf[100] = {0,};

#ifdef	CONVERSION_VS
	_itow_s( number, buf, _countof(buf), 10 );
#else	CONVERSION_VS
	_itow( number, buf, 10 );
#endif	CONVERSION_VS
	
	DrawText( buf, pos, dirVec, alineType, fGapTime );
}

void CKTDGPicChar::LoadPicCharGroup( const WCHAR* pPicCharGroup )
{
	if( NULL == m_pParticleSystem )
		return;

	if( m_hSeqPicChar != INVALID_PARTICLE_SEQUENCE_HANDLE )
	{
		CKTDGParticleSystem::CParticleEventSequence* pSeq = m_pParticleSystem->GetInstanceSequence( m_hSeqPicChar );
		if( pSeq != NULL )
		{
			map<int,CKTDXDeviceTexture*>::iterator i;
			for( i = pSeq->m_TextureMap.begin(); i != pSeq->m_TextureMap.end(); i++ )
			{
				SAFE_CLOSE( i->second );
			}
			pSeq->m_TextureMap.clear();


			for( int i = 32; i < 125; i++ )
			{
				WCHAR wFileName[200] = {0,};
				StringCchPrintf( wFileName, 200 , L"%s%c.dds", pPicCharGroup, i );
				//wsprintf( wFileName, L"%s%c.dds", pPicCharGroup, i );

#ifdef DELETE_ERROR_LOG
				// 아스키코드로 순회하면서, 파일명으로는 생성 할 수 없는 파일도 같이 접근 시도하고 있음.
				// 로그 남기지 않도록 예외처리
				CKTDXDeviceTexture* pTexture = g_pKTDXApp->GetDeviceManager()->OpenTexture( wFileName, D3DFMT_UNKNOWN, false, false );
#else
				CKTDXDeviceTexture* pTexture = g_pKTDXApp->GetDeviceManager()->OpenTexture( wFileName );
#endif // DELETE_ERROR_LOG
				if( pTexture != NULL && 
					m_hSeqPicChar != INVALID_PARTICLE_SEQUENCE_HANDLE )
					pSeq->m_TextureMap.insert( std::make_pair(i,pTexture) );

				m_PicCharTexList[i] = pTexture;
			}
		}
	}
}

void CKTDGPicChar::LoadPicCharGroup( const WCHAR* pPicCharGroup, wstring charList )
{

	if( NULL == m_pParticleSystem )
		return;

	if( m_hSeqPicChar != INVALID_PARTICLE_SEQUENCE_HANDLE )
	{
		CKTDGParticleSystem::CParticleEventSequence* pSeq = m_pParticleSystem->GetInstanceSequence( m_hSeqPicChar );
		if( pSeq != NULL )
		{
			map<int,CKTDXDeviceTexture*>::iterator i;
			for( i = pSeq->m_TextureMap.begin(); i != pSeq->m_TextureMap.end(); i++ )
			{
				SAFE_CLOSE( i->second );
			}
			pSeq->m_TextureMap.clear();


			for( int i = 0; i < (int)charList.size(); i++ )
			{
				wstringstream wFileName;
				wFileName << pPicCharGroup << charList.at(i) << L".dds";
				char index = (char)charList.at(i);

				CKTDXDeviceTexture* pTexture = g_pKTDXApp->GetDeviceManager()->OpenTexture( wFileName.str().c_str() );
				if( pTexture != NULL &&
					m_hSeqPicChar != INVALID_PARTICLE_SEQUENCE_HANDLE )
					pSeq->m_TextureMap.insert( std::make_pair(index,pTexture) );

				m_PicCharTexList[index] = pTexture;
			}
		}

	}
}

void CKTDGPicChar::LoadPicCharGroup( CKTDGPicChar* pPicChar )
{

	if( NULL == m_pParticleSystem )
		return;


	if( m_hSeqPicChar != INVALID_PARTICLE_SEQUENCE_HANDLE )
	{
		CKTDGParticleSystem::CParticleEventSequence* pSeq = m_pParticleSystem->GetInstanceSequence( m_hSeqPicChar );
		if( pSeq != NULL )
		{
			map<int,CKTDXDeviceTexture*>::iterator i;
			for( i = pSeq->m_TextureMap.begin(); i != pSeq->m_TextureMap.end(); i++ )
			{
				SAFE_CLOSE( i->second );
			}
			pSeq->m_TextureMap.clear();


			if( pPicChar->GetSeq() != NULL )
			{
				pSeq->m_TextureMap = pPicChar->GetSeq()->m_TextureMap;
				for( int i = 32; i < 125; i++ )
				{
					m_PicCharTexList[i] = pPicChar->GetPicCharList( i );
					if( m_PicCharTexList[i] != NULL )
					{
						m_PicCharTexList[i]->RefDevice();
					}
				}
			}
		}
	}
}

void CKTDGPicChar::Clear()
{
	if( NULL == m_pParticleSystem )
		return;


	CKTDGParticleSystem::CParticleEventSequence* pSeq = m_pParticleSystem->GetInstanceSequence( m_hSeqPicChar );
	if( pSeq != NULL )
	{
		pSeq->ClearAllParticle();
	}

	
	m_StrDataList.resize( 0 );
}

CKTDGParticleSystem::CParticleEventSequence* CKTDGPicChar::GetSeq()
{
	if( NULL == m_pParticleSystem )
		return NULL;
    
	if( m_hSeqPicChar == INVALID_PARTICLE_SEQUENCE_HANDLE )
		return NULL;
	else
		return m_pParticleSystem->GetInstanceSequence( m_hSeqPicChar );
}
