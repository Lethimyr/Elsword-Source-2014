#pragma once

class CX2TalkBoxManagerUI : public CKTDGUIControl
{

	public:

		CX2TalkBoxManagerUI(void);
		virtual ~CX2TalkBoxManagerUI(void);

		HRESULT OnFrameMove( double fTime, float fElapsedTime );
		HRESULT OnFrameRender();

		//void Push(TalkBox* talkBox) { Push( *talkBox ); }
		void Push( CX2TalkBoxManagerImp::TalkBox& talkBox );
		bool Delete(int unitUID, bool bTraceUnit = false, CX2GameUnit::GAME_UNIT_TYPE gameUnitType = CX2GameUnit::GUT_USER );
		void Clear();

		void SetPos( UidType unitUID, D3DXVECTOR2 pos, int boxType, bool bTraceUnit = false, CX2GameUnit::GAME_UNIT_TYPE gameUnitType = CX2GameUnit::GUT_USER );
		void SetPos( int talkBoxID, D3DXVECTOR2 pos, int boxType, bool bTraceUnit = false, CX2GameUnit::GAME_UNIT_TYPE gameUnitType = CX2GameUnit::GUT_USER );

		int GetMaxStrLen();

	protected:

		CX2TalkBoxManagerImp* m_pTalkBoxManagerImp;
};
