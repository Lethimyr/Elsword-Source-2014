
#pragma once

class CX2SlideShot
{
	public:
		CX2SlideShot(void);
		~CX2SlideShot(void);

		void		OnFrameMove( float fElapsedTime );

		void		ScenStart_LUA( char* pScenName, int scenNum );
		void		ScenWait();
		void		ScenEnd();

		void		GoNextScen();
		void		AddText_LUA( bool bNameLeft, int NameID, int TextID, float fFullTime = 0.0f, char* pColor = "#C000000" );
		void		AddText2_LUA( bool bNameLeft, int NameID, int TextID, float fTime = 0.0f, char* pColor = "#C000000" );
		void		CannotInput( float fTime );

		float		GetNowTime(){ return m_fScenTime; }
		bool		EventTimer( float fTime );

		void		SetNowState(CX2State* val) { m_pNowState = val; }
		void		AddSeqMap_LUA( char* ID, CKTDGParticleSystem::CParticleEventSequence* pSeq )
		{
			if( NULL == pSeq )
				return;

			m_SeqMap.insert( std::make_pair(ID, pSeq->GetHandle()) );
		}
		void		CrashSeq_LUA( char* ID, float fTime, float fGap );
		void		ChangeColor_LUA( char* ID, float fTime, D3DXCOLOR d3dColor );
		void		ChangeTex_LUA( char* ID, char* texName );
		void		ChangeSize_LUA( char* ID, float fX, float fY );
		void		DeleteSeq_LUA( char* ID );
		void		SetTextBoxShow( bool bShow ){ m_TextBoxShow = bShow; }

		void 		SetNameTabShow( bool bShow );
		void 		SetSpeechBallonShow( bool bShow );
		//{{ 2010.04.28 / 박교현 / 베스마 비던 개편
		void		SetEnterShow( bool bShow );
		//}}


		bool		IsPresentNow(){ return m_bNowPresent; }

		void		BlackLayOutMove( bool bInning, float fTime );
		void		SetTextSpread();
		bool		IsTextSpreading();
		float		GetElapsedTimeAfterTextSpread() { return m_fElapsedTimeAfterTextSpread; }
		void		PlaySound2D_LUA( char* pFileName );
		void		StopAllSound_LUA();
		void		SetSlideBGM_LUA( char* pFileName );
		void		ResetBGM();
#ifdef  X2OPTIMIZE_SLIDE_SHOT_NPC_SELF_CRASH_BUG_FIX
        void        SetNPC( CX2GUNPC* pNPC );
        void        ResetNPC();
        CX2GUNPC*   GetNPC();
#endif  X2OPTIMIZE_SLIDE_SHOT_NPC_SELF_CRASH_BUG_FIX

#ifdef CHECK_VOICE_IN_SLIDESHOT
		float		GetElapsedTimeAfterVoiceStopped();
#endif //CHECK_VOICE_IN_SLIDESHOT

	private:		
		void		ClearSeq();
		void		AddText( bool bNameLeft, const WCHAR* wszName, const WCHAR* wszMsg, wstring wstrColor = L"#C000000", bool bSpread = true, float fSpreadTime = 0.1f );
		void		ClearText();
		int			LineBreak( int iWidth, wstring& wstrSpeech, const WCHAR* pNextLineString, wstring wstrColor = L"" );
		
		
		bool		m_bNowPresent;
		bool		m_TextBoxShow;
		int			m_NowScenNum;
		string		m_ScenName;
		float		m_fScenTimeBefore;
		float		m_fScenTime;

		CX2State*			m_pNowState;		
		CKTDGUIDialogType		m_pDLGSlideShot;
		float				m_fElapsedTimeAfterTextSpread;
#ifdef CHECK_VOICE_IN_SLIDESHOT
		float				m_fElapsedTimeAfterVoiceStopped;
#endif //CHECK_VOICE_IN_SLIDESHOT

        map<string, CKTDGParticleSystem::CParticleEventSequenceHandle >	m_SeqMap;
		vector<wstring> m_vecSoundList;
		wstring			m_preBGMName;
		bool			m_bSetCursor;
#ifdef  X2OPTIMIZE_SLIDE_SHOT_NPC_SELF_CRASH_BUG_FIX
        CX2GUNPCoPtr    m_coNPC;
#endif  X2OPTIMIZE_SLIDE_SHOT_NPC_SELF_CRASH_BUG_FIX
};
