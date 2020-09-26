#pragma once

class CX2InformerSkill : public CX2Informer
{
	public:

		CX2InformerSkill(void);
		virtual ~CX2InformerSkill(void);

		HRESULT OnFrameMove( double fTime, float fElapsedTime );
		void Reset();

	protected:

		void SetNowInfo();
		bool IsNowInfoChange();
		void SetShow( bool bShow );

		int	m_NowIsPossibleLearnSkillNum;
		bool m_bCheckFirstConnect;
		float	m_fTimeCheckIsNowInfoChange;
};
