#pragma once




class CX2MonsterIndicator
{
public:

#ifdef SERV_BATTLEFIELD_MIDDLE_BOSS
	enum MONSTER_INDICATOR_TYPE			// 몬스터 Indicator Type
	{
		MIT_DEFAULT = 0,				// 기본, 몬스터 얼굴만 있는 것
		MIT_FIELD_MIDDLE_BOSS = 1,		// 중간 보스, MiddleBoss_Monster_Icon.dds
#ifdef SERV_BATTLEFIELD_EVENT_BOSS_INT
		MIT_FIELD_EVENT_BOSS = 2,		// 이벤트 보스, MiddleBoss_Monster_Icon.dds
#endif //SERV_BATTLEFIELD_EVENT_BOSS_INT
	};
	typedef KObserverPtr<CX2GUNPC> CX2GUNPCoPtr;
#endif // SERV_BATTLEFIELD_MIDDLE_BOSS
#ifdef SERV_BATTLEFIELD_MIDDLE_BOSS
	CX2MonsterIndicator( MONSTER_INDICATOR_TYPE eIndicatorType_ = MIT_DEFAULT );
#else SERV_BATTLEFIELD_MIDDLE_BOSS
	CX2MonsterIndicator(void);
#endif SERV_BATTLEFIELD_MIDDLE_BOSS
	~CX2MonsterIndicator(void);

	void OnFrameMove();
#ifdef MODIFY_DUNGEON_STAGING
	void SetShow(bool bVal){m_bShow = bVal;}
#endif //MODIFY_DUNGEON_STAGING

#ifdef SERV_BATTLEFIELD_MIDDLE_BOSS
	CX2MonsterIndicator::MONSTER_INDICATOR_TYPE GetIndicatorType() const { return m_eIndicatorType; }
	void SetIndicatorType (CX2MonsterIndicator::MONSTER_INDICATOR_TYPE val) { m_eIndicatorType = val; }

	CX2GUNPCoPtr GetTargetNPC() const { return m_pTargetNPC; }
	void SetTargetNPC(CX2GUNPC * val) { m_pTargetNPC = val; }

#endif // SERV_BATTLEFIELD_MIDDLE_BOSS

private:
	CKTDGParticleSystem::CParticleEventSequenceHandle 	m_hMonsterIndicator;


	CKTDGParticleSystem::CParticleEventSequenceHandle 	m_hMonsterIndicatorFace;
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
	CKTDGParticleSystem::CParticleHandle				m_hMonsterIndicatorParticle;
	CKTDGParticleSystem::CParticleHandle				m_hMonsterIndicatorFaceParticle;
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
	CKTDGParticleSystem::CParticle*						m_pMonsterIndicatorParticle;
	CKTDGParticleSystem::CParticle*						m_pMonsterIndicatorFaceParticle;
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE

#ifdef MODIFY_DUNGEON_STAGING
	bool												m_bShow;
#endif //MODIFY_DUNGEON_STAGING

#ifdef SERV_BATTLEFIELD_MIDDLE_BOSS
	CX2GUNPCoPtr							m_pTargetNPC;		// 타켓 NPC Point
	MONSTER_INDICATOR_TYPE					m_eIndicatorType;	// Type 열거
#endif // SERV_BATTLEFIELD_MIDDLE_BOSS
};



