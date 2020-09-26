#pragma once

class CX2Stat
{
	public:
		struct Stat
		{
			static const float		MAX_INCREASE_HP_RATE;				
		
			struct ExtraStat
			{
				float	m_fIncreaseHPRate;				// 옵션 수치화 이후 단위 값으로 바뀌었음(단위값이란 1인경우 기준레벨 대비 1% 올리기 위해 필요한 값)
#ifdef PET_AURA_SKILL
				float	m_fIncreaseMPRate;
				float	m_fIncreaseAtkPhysicRate;
				float	m_fIncreaseAtkMagicRate;
				float	m_fIncreaseDefPhysicRate;
				float	m_fIncreaseDefMagicRate;
#endif
			};

			KProtectedType<float>	m_fBaseHP;

			//공격
			KProtectedType<float>	m_fAtkPhysic;
			KProtectedType<float>	m_fAtkMagic;

			//방어
			KProtectedType<float>	m_fDefPhysic;
			KProtectedType<float>	m_fDefMagic;

			ExtraStat		m_ExtraStat;


			Stat()
			{
				Init();
			}
			void Init()
			{
				m_fBaseHP			= 0.0f;
				
				m_fAtkPhysic		= 0.0f;
				m_fAtkMagic			= 0.0f;

				m_fDefPhysic		= 0.0f;
				m_fDefMagic			= 0.0f;

				m_ExtraStat.m_fIncreaseHPRate = 0.f;
#ifdef PET_AURA_SKILL
				m_ExtraStat.m_fIncreaseMPRate = 0.f;
				m_ExtraStat.m_fIncreaseAtkPhysicRate = 0.f;
				m_ExtraStat.m_fIncreaseAtkMagicRate = 0.f;
				m_ExtraStat.m_fIncreaseDefPhysicRate = 0.f;
				m_ExtraStat.m_fIncreaseDefMagicRate = 0.f;
#endif
			}

			void AddStat( const Stat& baseStat, bool bIncludeExtra = false )
			{
				m_fBaseHP			+= baseStat.m_fBaseHP;

				m_fAtkPhysic		+= baseStat.m_fAtkPhysic;
				m_fAtkMagic			+= baseStat.m_fAtkMagic;

				m_fDefPhysic		+= baseStat.m_fDefPhysic;
				m_fDefMagic			+= baseStat.m_fDefMagic;

				if( true == bIncludeExtra )
				{
					m_ExtraStat.m_fIncreaseHPRate		+= baseStat.m_ExtraStat.m_fIncreaseHPRate;
#ifdef PET_AURA_SKILL
					m_ExtraStat.m_fIncreaseMPRate += baseStat.m_ExtraStat.m_fIncreaseMPRate;
					m_ExtraStat.m_fIncreaseAtkPhysicRate += baseStat.m_ExtraStat.m_fIncreaseAtkPhysicRate;
					m_ExtraStat.m_fIncreaseAtkMagicRate += baseStat.m_ExtraStat.m_fIncreaseAtkMagicRate;
					m_ExtraStat.m_fIncreaseDefPhysicRate += baseStat.m_ExtraStat.m_fIncreaseDefPhysicRate;
					m_ExtraStat.m_fIncreaseDefMagicRate += baseStat.m_ExtraStat.m_fIncreaseDefMagicRate;
#endif
				}
				LimitMaximum();
			}

			//{{ kimhc // 2011-07-05 // 옵션데이타 수치화 작업
			void ConvertAndAddStat( const Stat& baseStat_, const int iHpIncrementValue_ )
			{
				m_fBaseHP			+= baseStat_.m_fBaseHP;

				m_fAtkPhysic		+= baseStat_.m_fAtkPhysic;
				m_fAtkMagic			+= baseStat_.m_fAtkMagic;

				m_fDefPhysic		+= baseStat_.m_fDefPhysic;
				m_fDefMagic			+= baseStat_.m_fDefMagic;

				// 옵션 수치화에 의해 계산된 값을 m_fIncreaseHPRate에 곱해준다
				m_ExtraStat.m_fIncreaseHPRate		+= baseStat_.m_ExtraStat.m_fIncreaseHPRate * iHpIncrementValue_;
	#ifdef PET_AURA_SKILL
				m_ExtraStat.m_fIncreaseMPRate += baseStat_.m_ExtraStat.m_fIncreaseMPRate;
				m_ExtraStat.m_fIncreaseAtkPhysicRate += baseStat_.m_ExtraStat.m_fIncreaseAtkPhysicRate;
				m_ExtraStat.m_fIncreaseAtkMagicRate += baseStat_.m_ExtraStat.m_fIncreaseAtkMagicRate;
				m_ExtraStat.m_fIncreaseDefPhysicRate += baseStat_.m_ExtraStat.m_fIncreaseDefPhysicRate;
				m_ExtraStat.m_fIncreaseDefMagicRate += baseStat_.m_ExtraStat.m_fIncreaseDefMagicRate;
	#endif
			}
			//}} kimhc // 2011-07-05 // 옵션데이타 수치화 작업

			void DecStat( const Stat& baseStat, bool bIncludeExtra = false )
			{
				m_fBaseHP			-= baseStat.m_fBaseHP;

				m_fAtkPhysic		-= baseStat.m_fAtkPhysic;
				m_fAtkMagic			-= baseStat.m_fAtkMagic;

				m_fDefPhysic		-= baseStat.m_fDefPhysic;
				m_fDefMagic			-= baseStat.m_fDefMagic;

				if( true == bIncludeExtra )
				{
					m_ExtraStat.m_fIncreaseHPRate		-= baseStat.m_ExtraStat.m_fIncreaseHPRate;
#ifdef PET_AURA_SKILL
					m_ExtraStat.m_fIncreaseMPRate -= baseStat.m_ExtraStat.m_fIncreaseMPRate;
					m_ExtraStat.m_fIncreaseAtkPhysicRate -= baseStat.m_ExtraStat.m_fIncreaseAtkPhysicRate;
					m_ExtraStat.m_fIncreaseAtkMagicRate -= baseStat.m_ExtraStat.m_fIncreaseAtkMagicRate;
					m_ExtraStat.m_fIncreaseDefPhysicRate -= baseStat.m_ExtraStat.m_fIncreaseDefPhysicRate;
					m_ExtraStat.m_fIncreaseDefMagicRate -= baseStat.m_ExtraStat.m_fIncreaseDefMagicRate;
#endif
				}
				LimitMaximum();
			}

			void MultiplyStat( float fFactor, bool bIncludeExtra = false )
			{
				m_fBaseHP			*= fFactor;

				m_fAtkPhysic		*= fFactor;
				m_fAtkMagic			*= fFactor;

				m_fDefPhysic		*= fFactor;
				m_fDefMagic			*= fFactor;

				if( true == bIncludeExtra )
				{
					m_ExtraStat.m_fIncreaseHPRate		*= fFactor;
#ifdef PET_AURA_SKILL
					m_ExtraStat.m_fIncreaseMPRate *= fFactor;
					m_ExtraStat.m_fIncreaseAtkPhysicRate *= fFactor;
					m_ExtraStat.m_fIncreaseAtkMagicRate *= fFactor;
					m_ExtraStat.m_fIncreaseDefPhysicRate *= fFactor;
					m_ExtraStat.m_fIncreaseDefMagicRate *= fFactor;
#endif
				}
				LimitMaximum();
			}

			void SetKStat( const KStat& kStat )
			{
				m_fBaseHP			= (float)kStat.m_iBaseHP;

				m_fAtkPhysic		= (float)kStat.m_iAtkPhysic;
				m_fAtkMagic			= (float)kStat.m_iAtkMagic;

				m_fDefPhysic		= (float)kStat.m_iDefPhysic;
				m_fDefMagic			= (float)kStat.m_iDefMagic;

				m_ExtraStat.m_fIncreaseHPRate		= 0.f;	// warning!!!
#ifdef PET_AURA_SKILL
				m_ExtraStat.m_fIncreaseMPRate = 0.f;
				m_ExtraStat.m_fIncreaseAtkPhysicRate = 0.f;
				m_ExtraStat.m_fIncreaseAtkMagicRate = 0.f;
				m_ExtraStat.m_fIncreaseDefPhysicRate = 0.f;
				m_ExtraStat.m_fIncreaseDefMagicRate = 0.f;
#endif
				LimitMaximum();
			}

			void SetStat( const Stat& kStat )
			{
				m_fBaseHP			= (float)kStat.m_fBaseHP;

				m_fAtkPhysic		= (float)kStat.m_fAtkPhysic;
				m_fAtkMagic			= (float)kStat.m_fAtkMagic;

				m_fDefPhysic		= (float)kStat.m_fDefPhysic;
				m_fDefMagic			= (float)kStat.m_fDefMagic;

				m_ExtraStat.m_fIncreaseHPRate		= 0.f;	// warning!!!
#ifdef PET_AURA_SKILL
				m_ExtraStat.m_fIncreaseMPRate = 0.f;
				m_ExtraStat.m_fIncreaseAtkPhysicRate = 0.f;
				m_ExtraStat.m_fIncreaseAtkMagicRate = 0.f;
				m_ExtraStat.m_fIncreaseDefPhysicRate = 0.f;
				m_ExtraStat.m_fIncreaseDefMagicRate = 0.f;
#endif
				LimitMaximum();
			}


			void LimitMaximum()
			{
// 				if( m_ExtraStat.m_fIncreaseHPRate > MAX_INCREASE_HP_RATE )
// 					m_ExtraStat.m_fIncreaseHPRate = MAX_INCREASE_HP_RATE;
			}

			bool Verify() const;
		};

		struct AddOnTime
		{
			//공격
			KProtectedType<float>	m_fAtkPhysic;
			KProtectedType<float>	m_fAtkMagic;

			

			//방어
			KProtectedType<float>	m_fDefPhysic;
			KProtectedType<float>	m_fDefMagic;

		

			AddOnTime()
			{
				Init();
			}
			void Init()
			{
				m_fAtkPhysic		= 0.0f;
				m_fAtkMagic			= 0.0f;
				
				m_fDefPhysic		= 0.0f;
				m_fDefMagic			= 0.0f;
				
			}
		};

		// 현재는 내 정보 UI에서 표시해주기 위한 용도로만 사용
		struct EnchantStat	// 실제로는 속성 stat 이므로 ElementalStat정도로 고쳐야함
		{
			float m_fDefBlaze;
			float m_fDefWater;
			float m_fDefNature;
			float m_fDefWind;
			float m_fDefLight;
			float m_fDefDark;

			EnchantStat()
			{
				Init();
			}

			//{{ kimhc // 2011.4.24 // 월드버프 2단계(드래곤의 숨결)
#ifdef	CHUNG_FIRST_CLASS_CHANGE
			EnchantStat( const float fDef )
			{
				m_fDefBlaze		= fDef;
				m_fDefWater		= fDef;
				m_fDefNature	= fDef;
				m_fDefWind		= fDef;
				m_fDefLight		= fDef;
				m_fDefDark		= fDef;
			}
			void operator+=( const float fRhs )
			{
				m_fDefBlaze		+= fRhs;
				m_fDefWater		+= fRhs;
				m_fDefNature	+= fRhs;
				m_fDefWind		+= fRhs;
				m_fDefLight		+= fRhs;
				m_fDefDark		+= fRhs;
			}
			void operator+=( const EnchantStat* pRhsEnchantStat )
			{
				m_fDefBlaze		+= pRhsEnchantStat->m_fDefBlaze;
				m_fDefWater		+= pRhsEnchantStat->m_fDefWater;
				m_fDefNature	+= pRhsEnchantStat->m_fDefNature;
				m_fDefWind		+= pRhsEnchantStat->m_fDefWind;
				m_fDefLight		+= pRhsEnchantStat->m_fDefLight;
				m_fDefDark		+= pRhsEnchantStat->m_fDefDark;
			}
#endif	CHUNG_FIRST_CLASS_CHANGE
			//}} kimhc // 2011.4.24 // 월드버프 2단계(드래곤의 숨결)

			void Init()
			{
				m_fDefBlaze = 0.f;
				m_fDefWater = 0.f;
				m_fDefNature = 0.f;
				m_fDefWind = 0.f;
				m_fDefLight = 0.f;
				m_fDefDark = 0.f;
			}

//{{ kimhc // 2011.1.14 // 청 1차 전직
#ifdef	CHUNG_FIRST_CLASS_CHANGE
			void operator+=( const EnchantStat& rhsEnchantStat )
			{
				m_fDefBlaze		+= rhsEnchantStat.m_fDefBlaze;
				m_fDefWater		+= rhsEnchantStat.m_fDefWater;
				m_fDefNature	+= rhsEnchantStat.m_fDefNature;
				m_fDefWind		+= rhsEnchantStat.m_fDefWind;
				m_fDefLight		+= rhsEnchantStat.m_fDefLight;
				m_fDefDark		+= rhsEnchantStat.m_fDefDark;
			}
#endif	CHUNG_FIRST_CLASS_CHANGE
//}} kimhc // 2011.1.14 // 청 1차 전직
		};
		
	public:
		CX2Stat(void);
		~CX2Stat(void);

		void	InitStat(){ m_Stat.Init(); }
//{{ robobeg : 2013-11-04
		//Stat*	GetStat(){ return &m_Stat; }
        const Stat&	GetStat() const { return m_Stat; }
        Stat&	AccessStat() { return m_Stat; }
//}} robobeg : 2013-11-04

		void	AddStat( const Stat& baseStat, bool bIncludeExtra = false )
		{ 
			m_Stat.AddStat( baseStat, bIncludeExtra ); 
		}

//{{ robobeg : 2013-11-04
		//Stat*		GetAddOnStat(){ return &m_AddOnStat; }
		//AddOnTime*	GetAddOnTime(){ return &m_AddOnTime; }
		const Stat&		GetAddOnStat() const { return m_AddOnStat; }
        Stat&		AccessAddOnStat(){ return m_AddOnStat; }
		const AddOnTime&	GetAddOnTime() const { return m_AddOnTime; }
        AddOnTime&	AccessAddOnTime(){ return m_AddOnTime; }
//}} robobeg : 2013-11-04

		bool		Verify() const { return m_Stat.Verify(); }

	private:
		Stat		m_Stat;
		Stat		m_AddOnStat;
		AddOnTime	m_AddOnTime;
};
