#pragma once

#define MAX_PIC_CHAR_GROUP_NUM 200

class CKTDGPicChar
{
	public:
		enum ALINE_TYPE
		{
			AT_LEFT,
			AT_RIGHT,
			AT_CENTER,
		};

		struct StrData
		{
			wstring		m_wStr;
			ALINE_TYPE	m_AlineType;
			D3DXVECTOR3 m_Pos;
			D3DXVECTOR3 m_DirVec;
			int			m_NowIndex;
			float		m_fNowRemainTime;
			float		m_fMaxRemainTime;

			StrData()
			{
				m_AlineType			= AT_CENTER;
				m_NowIndex			= 0;
				m_Pos				= D3DXVECTOR3(0,0,0);
				m_DirVec			= D3DXVECTOR3(0,0,0);
				m_fNowRemainTime	= 0.0f;
				m_fMaxRemainTime	= 0.0f;
			}

			StrData& operator=( const StrData& data )
			{
				m_wStr				= data.m_wStr;
				m_Pos				= data.m_Pos;
				m_DirVec			= data.m_DirVec;
				m_NowIndex			= data.m_NowIndex;
				m_fNowRemainTime	= data.m_fNowRemainTime;
				m_fMaxRemainTime	= data.m_fMaxRemainTime;

				return *this;
			}
		};
	public:
		CKTDGPicChar( const WCHAR* pPicCharGroup, CKTDGParticleSystem::CParticleEventSequence* pEff );
		CKTDGPicChar( const WCHAR* pPicCharGroup, CKTDGParticleSystem::CParticleEventSequence* pEff, wstring charList );
		CKTDGPicChar( CKTDGPicChar* pPicChar, CKTDGParticleSystem::CParticleEventSequence* pEff );
		~CKTDGPicChar(void);

		void OnFrameMove( float fElapsedTime );

		void SetEffect( CKTDGParticleSystem::CParticleEventSequence* pEff )
		{
			if( NULL == pEff )
				return;

			m_hSeqPicChar = pEff->GetHandle(); 
			m_pParticleSystem = pEff->GetParticleSystem();
		}
		void DrawText( const WCHAR* pStr, D3DXVECTOR3 pos, D3DXVECTOR3 dirVec, ALINE_TYPE alineType, float fGapTime = 0.0f );
		void DrawText( const WCHAR* pStr, int index, D3DXVECTOR3 pos, D3DXVECTOR3 dirVec, ALINE_TYPE alineType );
		void DrawText( const int number, D3DXVECTOR3 pos, D3DXVECTOR3 dirVec, ALINE_TYPE alineType, float fGapTime = 0.0f );

		void LoadPicCharGroup( const WCHAR* pPicCharGroup );
		void LoadPicCharGroup( const WCHAR* pPicCharGroup, wstring charList );
		void LoadPicCharGroup( CKTDGPicChar* pPicChar );
		void Clear();
		void SetWidth( float fWidth ){ m_fWidth = fWidth; }
		void SetColor( D3DXCOLOR color ){ m_bUseColor = true; m_Color = color; }
		void SetColorNo(){ m_bUseColor = false;	}
		CKTDGParticleSystem::CParticleEventSequence* GetSeq();
		
		CKTDXDeviceTexture* GetPicCharList( int index )
		{ 
			if( index < 0 || index >= MAX_PIC_CHAR_GROUP_NUM )
				return NULL;
			return m_PicCharTexList[index];
		}


	private:
		float											m_fElapsedTime;
		CKTDGParticleSystem*							m_pParticleSystem;
		CKTDGParticleSystem::CParticleEventSequenceHandle	m_hSeqPicChar;
		CKTDXDeviceTexture*								m_PicCharTexList[MAX_PIC_CHAR_GROUP_NUM];
		vector<StrData>									m_StrDataList;
		float											m_fWidth;
		bool											m_bUseColor;
		D3DXCOLOR										m_Color;
};
