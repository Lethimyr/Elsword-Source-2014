#ifdef RIDING_SYSTEM
#pragma once

class CX2RidingPetSkillSlot
{
public:
	CX2RidingPetSkillSlot();
	~CX2RidingPetSkillSlot();

	bool		GetShowRidingSkillSlot(){ return m_pDlgSkillSlot->GetShow(); }
	void		SetShowRidingSkillSlot( bool _Val );
	void		UpdateRideOffSlotGuide();
	void		UpdateSkillSlotString();
#ifdef ADJUST_RIDINGPET_SKILLSLOT
	void		SetSkillSlotTexture();
#endif //ADJUST_RIDINGPET_SKILLSLOT
	void		UpdateSkillSlotTexture();
	void		UpdateStamina( IN const float fStamina_, IN const float fMaxStamina_ );
	void		SetBlindSkillSlot( UINT _slotNum, bool _val );
	void		SetSkillSlotCoolTime( IN UINT islotNum_, IN float fCoolTime_ );
	float		GetSkillCoolTime( IN UINT islotNum_ ) { return m_fCoolTime[islotNum_]; }
	void		OnFrameMove( double fTime, float fElapsedTime );
	void		SetSkillUIType( bool bIsTypeA );

private:
	CKTDGUIDialogType	m_pDlgSkillSlot;
	float				m_fCoolTime[3];

};

#endif //RIDING_SYSTEM