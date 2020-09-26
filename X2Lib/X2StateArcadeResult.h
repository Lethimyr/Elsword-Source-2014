#pragma once

class CX2StateArcadeResult : public CX2StateDungeonResult
{
	public:

		enum ARCADE_RESULT_ANIMATE_ORDER
		{
			ARAO_NONE = 0,
			ARAO_RESULT_BG,
			ARAO_MY_RECORD,
			ARAO_RANK,
			ARAO_RANK_CHECK,
			ARAO_END,
		};

	public:
		CX2StateArcadeResult(void);
		virtual ~CX2StateArcadeResult(void);

		virtual HRESULT OnFrameMove( double fTime, float fElapsedTime );

	protected:

		bool Handler_EGS_RESULT_ALL_UNIT_SUCCESS_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

		void PicCharFrameMove( double fTime, float fElapsedTime );

		HRESULT InitArcadeUI();
		void PicCharDrawTextForArcade();
		wstring GetRecordTime( int iPlayTime );
		wstring GetRecordTimeEx( int iPlayTime );

		ARCADE_RESULT_ANIMATE_ORDER m_ArcadeResultAniOrder;

		CKTDGUIDialogType	m_pDLGArcadeBack;

		float			m_fTimeShow;

		CKTDGParticleSystem::CParticleEventSequenceHandle  m_hParticleAracdeRank;

		CKTDGPicChar* m_pPicCharTodayRank;
		CKTDGPicChar* m_pPicCharNowRank;

		CKTDGParticleSystem::CParticleEventSequenceHandle  m_hParticleMyNewRecord;

		CKTDGPicChar* m_pPicCharTodayRankTime;
};
