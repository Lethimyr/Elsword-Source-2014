#pragma once

class CXSLGUNPCAI
{
public:
	enum TARGET_PRIORITY
	{
		TP_RANDOM,
		TP_ONLY_ONE,
		TP_HIGH_LEVEL_FIRST,
		TP_LOW_LEVEL_FIRST,
		TP_HIGH_HP_FIRST,
		TP_LOW_HP_FIRST,
		TP_NEAR_FIRST,
		TP_FAR_FIRST,
	};

	struct TargetData
	{
		bool				targetNPC;
		TARGET_PRIORITY		targetPriority;
		float 				targetInterval;
		int					targetNearRange;	// 이 범위 안에 들어오면 무조건 타게팅 된다
		int					targetRange;		// 이 범위 안에 들어오면 타게팅 된다
		int					targetLostRange;	// 이 범위 밖으로 벗어나면 현재 타겟을 잃게 된다.
		float				targetSuccessRate;	
		float				attackTargetRate;	// 나를 공격한 유닛을 타겟팅할 확률
		float				preserveLastTargetRate;
	};

	struct ChaseMoveData
	{
		int			moveSplitRange;
		int			destGap;			// 목적지에서 이거리 안에 있으면 도착했다고 판단한다
		int			moveGap;

		float		dirChangeInterval;

		float		walkInterval;
		float		nearWalkRate;
		float		farWalkRate;

		float		jumpInterval;
		float		upJumpRate;
		float		upDownRate;
		float		downJumpRate;
		float		downDownRate;

		float		m_fLineEndDetectRange;
	};

	struct PatrolMoveData
	{
		float		patrolBeginRate;
		int			patrolRange;
		float		patrolCoolingTime;
		bool		bOnlyThisLineGroup;
	};

	struct EscapeMoveData
	{
		int			moveSplitRange;
		int			escapeGap;			// 이 거리 보다 멀면 도망 성공

		float		walkInterval;
		float		nearWalkRate;
		float		farWalkRate;

		float		jumpInterval;
		float		upJumpRate;
		float		upDownRate;
		float		downJumpRate;
		float		downDownRate;

		float		m_fLineEndDetectRange;
	};

	struct EscapeCondition
	{ 
		float		escapeRate;
		int			myHP;
		float		escapeRange;

		EscapeCondition()
		{
			escapeRate		= 0;
			myHP			= 0;
			escapeRange 	= 0.f;			
		}
	};

	struct AIData
	{
		TargetData		targetData;
		ChaseMoveData	chaseMoveData;
		PatrolMoveData	patrolMoveData;
		EscapeMoveData	escapeMoveData;
		EscapeCondition	escapeCondition;
	};

public:
	CXSLGUNPCAI(void);
	~CXSLGUNPCAI(void);

};