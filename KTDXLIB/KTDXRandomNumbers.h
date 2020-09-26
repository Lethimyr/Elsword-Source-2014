#pragma once

class CKTDXRandomNumbers
{
public:
	// 동시에 계산되는 확률의 경우에 서로 다른 random number를 사용해야 하기 때문에 random table을 탐색할 때 임의의 offset을 더해주도록 한다. 
	// 예를 들면 blaze 5%와 크리티컬 5%이면 blaze와 크리티컬은 항상 동시에 터지게 되는데 이를 막기 위해서
	enum SPECIAL_RANDOM_OFFSET
	{

		SRO_ATTACK_CRITICAL = 1,
		SRO_HIT_DODGE,
		SRO_ATTACK_EXTRA_DAMAGE_ENCHANT,
		SRO_ATTACK_EXTRA_DAMAGE_MULTIPLE,	// 중첩 가능한 extra damage 확률 계산에 사용
		SRO_IMMUNE_EXTRA_DAMAGE_SCRIPTED,
		SRO_RESIST_EXTRA_DAMAGE_ENCHANT,
		SRO_SUPER_ARMOR_WHEN_ATTACKED,

		SRO_ADD_MANA_ON_ATTACK,
		SRO_ADD_MANA_WHEN_ATTACKED,
		SRO_STAT_PERCENT_UP,
		SRO_CURE_DEBUFF,
		SRO_DEBUFF_ATTACKER,
		SRO_BONE_SHIELD,
		SRO_ATTACK_SPEED_UP,
		//{{ kimhc // 2010.12.29 // 하멜 마을 추가시 들어가는 셋트 아이템 효과
		SRO_ICE_NOVA,
		//}} kimhc // 2010.12.29 // 하멜 마을 추가시 들어가는 셋트 아이템 효과
#ifdef	SERV_TRAPPING_RANGER_TEST
		SRO_VITALPOINT_PIERCING,
#endif	SERV_TRAPPING_RANGER_TEST
#ifdef ADD_SOCKET_FOR_SECRET_SET
		SRO_POISON_BURST,
		SRO_ABSORB_HP,
#endif
#ifdef HAMEL_SECRET_DUNGEON // 김태환
		SRO_ACTIVE_BUFF,
#endif // HAMEL_SECRET_DUNGEON
	};


public:
	CKTDXRandomNumbers(void);
	~CKTDXRandomNumbers(void);

	static float GetRandomFloat( int iIndex );
	static int GetRandomInt( int iIndex );

public:
	static const int RANDOM_NUMBER_COUNT = 32000;
	static const float MIN_PROBABILITY_RATE;
	static const USHORT s_RandomTable[RANDOM_NUMBER_COUNT];

};
