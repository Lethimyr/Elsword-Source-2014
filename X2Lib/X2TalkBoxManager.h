#pragma once


class CX2TalkBoxManager : public CKTDGObject
{
	public:

		enum TALK_BOX_MANAGER_TYPE
		{
			TBMT_DEFAULT = 0,
			TBMT_PERSONAL_SHOP,
			TBMT_PARTY_INFO,
		};

	private:
		CX2TalkBoxManager( CX2TalkBoxManager::TALK_BOX_MANAGER_TYPE talkBoxManagerType = CX2TalkBoxManager::TBMT_DEFAULT );
		virtual ~CX2TalkBoxManager(void);

	public:
		static CX2TalkBoxManager* CreateTalkBoxManger( CX2TalkBoxManager::TALK_BOX_MANAGER_TYPE talkBoxManagerType = CX2TalkBoxManager::TBMT_DEFAULT ) 
		{
			return new CX2TalkBoxManager( talkBoxManagerType );
		}
	


		bool MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

		HRESULT OnFrameMove( double fTime, float fElapsedTime );
//{{ robobeg : 2008-10-28
		//HRESULT OnFrameRender();
        virtual void    OnFrameRender_Draw();
//}} robobeg : 2008-10-28

		//void Push(TalkBox* talkBox) { Push( *talkBox ); }
		void Push( CX2TalkBoxManagerImp::TalkBox& talkBox );
		bool Delete(UidType unitUID, bool bTraceUnit = false, CX2GameUnit::GAME_UNIT_TYPE gameUnitType = CX2GameUnit::GUT_USER );
		bool DeleteByIndex( int index ) 
		{ 
			if ( m_pTalkBoxManagerImp != NULL ) 
				return m_pTalkBoxManagerImp->DeleteByIndex( index );

			return false;
		}

		int GetTalkBoxNum()
		{
			if ( m_pTalkBoxManagerImp != NULL )
				return m_pTalkBoxManagerImp->GetTalkBoxNum();

			return -1;
		}

		bool CheckTalkBox( UidType unitUID )
		{
			if ( m_pTalkBoxManagerImp != NULL )
				return m_pTalkBoxManagerImp->CheckTalkbox( unitUID );

			return false;
		}

		void Clear();

		void SetPos( UidType unitUID, D3DXVECTOR2 pos, int boxType, bool bTraceUnit = false, CX2GameUnit::GAME_UNIT_TYPE gameUnitType = CX2GameUnit::GUT_USER );
		void SetPos( int talkBoxID, D3DXVECTOR2 pos, int boxType, bool bTraceUnit = false, CX2GameUnit::GAME_UNIT_TYPE gameUnitType = CX2GameUnit::GUT_USER );

		bool CheckMousePointInTalkbox( D3DXVECTOR2 mousePos, UidType& uidType )
		{
			if ( m_pTalkBoxManagerImp != NULL )
				return m_pTalkBoxManagerImp->CheckMousePointInTalkbox( mousePos, uidType );

			return false;
		}

		CX2TalkBoxManagerImp::TalkBox* GetTalkBoxByUID( UidType unitUID )
		{
			if ( m_pTalkBoxManagerImp != NULL )
				return m_pTalkBoxManagerImp->GetTalkBoxByUID( unitUID );

			return NULL;
		}

		void OverTalkbox(bool val)
		{
			if ( m_pTalkBoxManagerImp != NULL )
				m_pTalkBoxManagerImp->SeTOverTalkbox( val );
		}

		int GetMaxStrLen();

		bool GetShowTalk() { return m_bShowTalk; }
		void SetShowTalk(bool val) { m_bShowTalk = val; }

		void SetOffset(D3DXVECTOR2 vPos);

		CKTDGFontManager::CUKFont* GetUKFont() { return m_pTalkBoxManagerImp->GetUKFont(); }

	protected:

		CX2TalkBoxManagerImp* m_pTalkBoxManagerImp;

		TALK_BOX_MANAGER_TYPE m_TalkBoxManagerType;

		bool m_bShowTalk;		
};
