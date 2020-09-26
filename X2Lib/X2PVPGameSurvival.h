#pragma once

class CX2PVPGameSurvival : public CX2PVPGame
{
	public:
		CX2PVPGameSurvival(void);
		virtual ~CX2PVPGameSurvival(void);

		virtual	void		Release();
		virtual	HRESULT		OnFrameMove( double fTime, float fElapsedTime );
		virtual HRESULT		OnFrameRender();

		virtual	void		AddUserUnit( CX2Room::SlotData* pSlotData, int slotIndex );
		virtual bool		DetermineLastKill();

		virtual void		UserUnitDieNot(	KEGS_USER_UNIT_DIE_NOT& kEGS_USER_UNIT_DIE_NOT );

	protected:

		float									m_fReBirthTime;		/// 사용안함, 표시용으로는 GUUser의 ReBirthTime을 사용함
		CKTDGFontManager::CUKFont*				m_pFontForReBirth;	/// 부활 시간 출력 용
		CKTDGFontManager::CUKFont*				m_pFontForWinKill;	/// 목표 킬 수 출력 용

		int										m_MaxKillNum;		/// 목표 킬 수
		float									m_fRemainRebirthTime;	/// 사용 하는 곳 없음
};
