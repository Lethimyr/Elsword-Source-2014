#ifdef QUEST_GUIDE
#pragma once
class CX2NPCIndicator
{
public:
	struct GuideTarget
	{
		D3DXVECTOR3 vTargetPos;
		bool bIsNPC;
		bool bIsEpic;

		GuideTarget(): vTargetPos(0,0,0), bIsNPC(false), bIsEpic(false) {}
	};

	struct StartPosInfo
	{
		UINT uiStartPos;
		UINT iMoveMapIndex;
		StartPosInfo(UINT iMoveMapIndex_):
		iMoveMapIndex(iMoveMapIndex_), uiStartPos(0){};
	};

public:
	CX2NPCIndicator(void);
	~CX2NPCIndicator(void);
	void OnFrameMove();
	void ResetGuideTargetInfo();
	void SetShow(bool bVal);
private:	
	void Clear();
	void CreateIndicatorParticle();					
	void SetRotateParticle(int iParticleIndex,const D3DXVECTOR3& vMyUserPos, float fDistance, bool bMove);	//파티클 회전 설정
	void SetShowAllParticle(bool bVal);
	float GetDirVecToDegree( D3DXVECTOR3 dirVec );

	bool GetLinkInfo( OUT map<int, D3DXVECTOR3>& mapVillageInfo_); //현재 위치에서 이동 할 수 있는 마을, 필드에 대한 정보 얻기

	CKTDGParticleSystem::CParticleEventSequenceHandle m_hIndicator[MAX_QUEST_INDICATOR_NUM];			//화살표 파티클
	CKTDGParticleSystem::CParticleEventSequenceHandle m_hIndicatorMark[MAX_QUEST_INDICATOR_NUM];		//Q마크 파티클
	CKTDGParticleSystem::CParticleEventSequenceHandle m_hPortalQuestion;								//포탈에 붙는 퀘스트 완료 파티클	

#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
	CKTDGParticleSystem::CParticleHandle		      m_hParticleIndicator[MAX_QUEST_INDICATOR_NUM];			//화살표 파티클
	CKTDGParticleSystem::CParticleHandle			  m_hParticleIndicatorMark[MAX_QUEST_INDICATOR_NUM];		//Q마크 파티클
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
	CKTDGParticleSystem::CParticle*					  m_pIndicator[MAX_QUEST_INDICATOR_NUM];			//화살표 파티클
	CKTDGParticleSystem::CParticle*					  m_pIndicatorMark[MAX_QUEST_INDICATOR_NUM];		//Q마크 파티클
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE

	CKTDGUIDialogType m_pDLGParticle;					//파티클의 레이어를 관리 하기 위해, 파티클을 멤버로 가지는 다이얼로그 생성
	
	GuideTarget		m_GuideTargetInfo[MAX_QUEST_INDICATOR_NUM];		//타겟 정보 담는 구조체
	int				m_iIndicatorCount;	
	float			m_fCheckDistance;								//유저의 움직임을 판별하기 위한 거리값
	bool			m_bShow; 

};
#endif //QUEST_GUIDE