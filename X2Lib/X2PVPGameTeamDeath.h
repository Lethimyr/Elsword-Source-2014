#pragma once

class CX2PVPGameTeamDeath : public CX2PVPGame
{
	public:
		CX2PVPGameTeamDeath(void);
		virtual ~CX2PVPGameTeamDeath(void);

		virtual	void		Release();
		virtual	HRESULT		OnFrameMove( double fTime, float fElapsedTime );
		virtual HRESULT		OnFrameRender();

		virtual	void		AddUserUnit( CX2Room::SlotData* pSlotData, int slotIndex );
		virtual bool		DetermineLastKill();

		virtual void		UserUnitDieNot(	KEGS_USER_UNIT_DIE_NOT& kEGS_USER_UNIT_DIE_NOT );

	protected:
		void				UpdateScore();

#ifdef MODIFY_PVP_SCORE_UI
		void				SetScoreTexture( int iMyTeamKill_, int iOtherTeamKill_, bool bIsRedTeam_ = true );
#endif //MODIFY_PVP_SCORE_UI

		float									m_fReBirthTime;		/// 사용안함, 표시용으로는 GUUser의 ReBirthTime을 사용함
		CKTDGFontManager::CUKFont*				m_pFontForReBirth;	/// 부활 시간 출력용
		CKTDGFontManager::CUKFont*				m_pFontForWinKill;	/// 목표킬 출력용


		CKTDGPicChar*							m_pPicCharForRedAndBlueKillNum;	/// 레드팀과 블루 팀의 킬수를 나타내기 위한 것

		int										m_MaxKillNum;		/// 목표 킬수
		float									m_fRemainRebirthTime;	/// 사용하는 곳 없음
		int										m_BeforeRedTeamKill;	/// 현재의 킬수와 비교하여 달라진 경우 m_pPicCharForRedAndBlueKillNum를 업데이트 시켜주기 위한 용도
		int										m_BeforeBlueTeamKill;	/// 현재의 킬수와 비교하여 달라진 경우 m_pPicCharForRedAndBlueKillNum를 업데이트 시켜주기 위한 용도
};
