#pragma once

// dmlee 2008.11.29 
// 하나의 아이템에 2개의 메시를 붙일 수 있게 코드 수정
// 공용아이템의 경우 하나의 메시만 붙일 수 있는 것으로 코드 유지, 나중에 수정 해야함




#define MAX_SPECIAL_ABILITY_NUM     3
#define MAX_ENCHANT_CARD_NUM    10



class CX2Item
{
public:

#include    "X2Item_Preprocessing.inl"

		enum SPECIAL_SKILL_TYPE
		{
#ifdef SPECIAL_USE_ITEM
			SST_WOODEN_SPEAR = 1,
			SST_LIGHT_SPEAR,
			SST_FIRE_SPHERE,
			SST_WATER_SPHERE,
			SST_NATURE_SPHERE,
			SST_WIND_SPHERE,
			SST_LIGHT_SPHERE,
			SST_DARK_SPHERE,

			SST_CHILDRENSDAY_SHPERE_01,	/// 분노의 어린이탄
			SST_CHILDRENSDAY_SHPERE_02,	/// 냉정한 어린이탄
			SST_CHILDRENSDAY_SHPERE_03,	/// 사랑의 어린이탄
			SST_CHILDRENSDAY_SHPERE_04,	/// 행복한 어린이탄

			SST_ANGER_SPHERE,		// 울분의 수정구
#else SPECIAL_USE_ITEM
			SST_THROW_WOODEN_PIECE = 1,
			SST_MAGIC_SPEAR,
			SST_ICE_CRYSTAL,
			SST_FIRE_CRYSTAL,
			SST_MAGIC_BULLET,
			SST_POISON_POCKET,
#endif SPECIAL_USE_ITEM
//#ifdef EVENT_CHINA_THROW_ITEM
			SST_CHINA_EVENT_SPEAR = 1010,
//#endif //EVENT_CHINA_THROW_ITEM
//#ifdef EVENT_ICICLE_THROW_ITEM
			SST_ICICLE_SPEAR = 1011,
//#endif EVENT_ICICLE_THROW_ITEM
#ifdef SERV_RELATIONSHIP_EVENT_INT
			SST_LOVE_LV1,
			SST_LOVE_LV2,
#endif SERV_RELATIONSHIP_EVENT_INT
//#ifdef EVENT_FIRE_CRACKER_THROW_ITEM
			SST_FIRE_CRACKER = 1021,
			SST_GOOD_FIRE_CRACKER = 1022,
//#endif //EVENT_FIRE_CRACKER_THROW_ITEM
		};

        enum SPECIAL_SKILL_MOTION_TYPE
        {
            SSMT_THROW = 1,        //던지는 형식
            SSMT_INSTALL,        //설치하는 형식
            SSMT_DIRECT_USE,    //바로 사용하는 형식( 바람 구슬 )
        };

        enum SUPPLEMENT_ITEM_TYPE
        {
            SIT_PLUS_SUCCESS_RATE = 0,
            SIT_NOT_BROKEN,
        };        


#ifndef X2OPTIMIZE_ITEM_TEMPLET_PREPROCESSING

#ifdef USER_WEAPON_PARTICLE_SCRIPT_TEST
        struct AttachedParticleData
        {
            wstring wstrParticleName;
            wstring wstrBoneName;
        };
#endif USER_WEAPON_PARTICLE_SCRIPT_TEST



#ifdef SUB_EQUIP_TEST
        struct AttachedMeshData
        {
            wstring wstrMeshName;
            wstring wstrBoneName;
#ifdef ATTACH_SKINMESH
            bool    bSkinMesh;
            bool    bJiggle;
#endif
        };
#endif SUB_EQUIP_TEST


        struct SpecialAbility
        {
            SPECIAL_ABILITY_TYPE           m_Type;
            int                            m_CoolTime;
            int                            m_Value1;
            int                            m_Value2;
            int                            m_Value3;
#ifdef SUMMON_MONSTER_CARD_SYSTEM
            wstring                        m_StringValue1;        //Group으로 된 정보를 받아 올 수 있는 인자
#endif SUMMON_MONSTER_CARD_SYSTEM

            SpecialAbility()
            {
                m_Type            = SAT_NONE;
                m_CoolTime        = 0;
                m_Value1        = 0;
                m_Value2        = 0;
                m_Value3        = 0;
#ifdef SUMMON_MONSTER_CARD_SYSTEM
                m_StringValue1    = L"Group 0";
#endif SUMMON_MONSTER_CARD_SYSTEM
            }

            SPECIAL_ABILITY_TYPE        GetType() const { return m_Type; }
#ifdef  SUMMON_MONSTER_CARD_SYSTEM
            const wchar_t*  GetStringValue1() const     { return m_StringValue1.c_str(); }
#endif  SUMMON_MONSTER_CARD_SYSTEM
        };

        struct ItemTemplet : public CX2ItemTemplet_Base
        {
            USE_TYPE                    m_UseType;                    // 사용 방식 장착장비인지 아닌지, 장착장비라면 어떤식으로 장착하는지
            USE_CONDITION                m_UseCondition;                // 사용 조건
            int                            m_UseLevel;                    // 사용 제한 레벨
            CX2Unit::UNIT_TYPE            m_UnitType;                    // 사용 unittype
            CX2Unit::UNIT_CLASS            m_UnitClass;                // 사용 unit 클래스


            int                            m_ItemID;                    //아이템 ID
            std::wstring                m_Name;                        //아이템 이름
            std::wstring                m_Description;                //아이템 설명
            std::wstring                m_DescriptionInShop;        //아이템 구매전 상점에서만 보이는 설명



            wstring                        m_AttachFrameName[ MAX_MODEL_COUNT_A_ITEM ];    // 어느 본에 붙는가
            D3DXVECTOR3                    m_FrameOffsetElsword;        //본에서의 오프셋(엘소드)
            D3DXVECTOR3                    m_FrameOffsetLena;            //본에서의 오프셋(레나)
            D3DXVECTOR3                    m_FrameOffsetAisha;            //본에서의 오프셋(아이샤)
            D3DXVECTOR3                    m_FrameOffsetRaven;            //본에서의 오프셋(레이븐)

            D3DXVECTOR3                    m_FrameOffsetEve;            //본에서의 오프셋(이브)

            //{{ kimhc // 2010.11.18 // 2010-12-23 New Character CHUNG
#ifdef    NEW_CHARACTER_CHUNG
            D3DXVECTOR3                    m_FrameOffsetChung;            // 본에서의 오프셋(청)
#endif    NEW_CHARACTER_CHUNG
        //}} kimhc // 2010.11.18 //  2010-12-23 New Character CHUNG

#ifdef ARA_CHARACTER_BASE
            D3DXVECTOR3                    m_FrameOffsetAra;            /// 본에서의 오프셋(아라)
#endif




            std::wstring                m_ModelName[ MAX_MODEL_COUNT_A_ITEM ];    //하이퍼 모드는 _Hyper 를 붙인다
            std::wstring                m_TextureChangeXETName;        //하이퍼 모드는 _Hyper 를 붙인다
            std::wstring                m_AniXETName;                //하이퍼 모드는 _Hyper 를 붙인다
            std::wstring                m_AniName;                    //하이퍼 모드는 _Hyper 를 붙인다
            std::wstring                m_ShopImage;
            std::wstring                m_DropViewer;                //드롭시 사용할 파티클 시스템 이름



            ITEM_TYPE                    m_ItemType;
            ITEM_GRADE                    m_ItemGrade;                //아이템 등급
            bool                        m_bFashion;                    //패션 아이템
            bool                        m_bVested;                    //귀속 아이템(양도불능/판매불능)            
            bool                        m_bCanEnchant;                //강화가능 아이템인가
            bool                        m_bCanUseInventory;            //인벤토리에서 사용가능한 아이템인가 (오른쪽 클릭시 즉시 사용되는 아이템만, 네잎클로버, 

            //{{ 허상형 : [2009/7/29] //    변수 추가
            bool                        m_bNoEquip;                    //착용할수 없는 장비인가(교환용 장비)            
            //}} 허상형 : [2009/7/29] //    변수 추가

            //{{ 2010. 01. 05  최육사    PC방 프리미엄
            bool                        m_bIsPcBang;                //PC방 전용 아이템
            //}}
            //{{ 2011. 10. 14    최육사    배틀필드 아이템 레벨 시스템
#ifdef SERV_BATTLE_FIELD_ITEM_LEVEL
            int                            m_iItemLevel;                // 아이템 레벨 정보
#endif SERV_BATTLE_FIELD_ITEM_LEVEL
            //}}

            //{{ 080721.hoons.
            D3DXVECTOR3                    m_RanderScale[CX2Unit::UT_END];        //랜더링시 캐릭터별 스케일.
            D3DXVECTOR3                    m_RanderRotate[CX2Unit::UT_END];    //랜더링시 캐릭터별 회전.
            //}}

            bool                        m_bCanHyperMode;            //하이퍼 모드 가능 아이템인가            

            PERIOD_TYPE                    m_PeriodType;                //아이템 존재기간
            int                            m_Endurance;                //초기내구도
            CMinMax<int>                m_EnduranceDamage;            //1스테이지플레이당내구도 감소량
            //{{ 2007. 11. 16  최육사  
            float                        m_RepairED;                    //내구도 1 수리당 ED
            int                            m_RepairVP;                    //내구도 1 수리당 VP
            //}}
            int                            m_Quantity;                    //1묶음수량

            SHOP_PRICE_TYPE                m_PriceType;                //가격타입
            int                            m_Price;                    //가격
            int                            m_PricePvPPoint;            //구매대전포인트





#ifdef COMMON_ITEM_TEMPLET_TEST
            map<CX2Unit::UNIT_TYPE, wstring>    m_mapCommonItemModelName;    // UC_ANYONE인 경우에만 사용, 현재는 장착 장비류인 경우에만
            map<CX2Unit::UNIT_TYPE, wstring>    m_mapCommonItemXETName;
#endif COMMON_ITEM_TEMPLET_TEST

#ifdef BUFF_TEMPLET_SYSTEM
            int                            m_Buff_ID;                    //아이템에 설정된 버프 타입
#endif BUFF_TEMPLET_SYSTEM

            CX2Unit::EQIP_POSITION        m_EqipPosition;                //장착 위치
            CX2Stat::Stat                m_Stat;                        //아이템 스탯

            vector<CX2BuffFactorPtr>    m_vecBuffFactorPtr;            /// 발생 시킬 버프/디버프 요소 모음
            std::vector<SpecialAbility>    m_SpecialAbilityList;
            int                            m_CoolTime;
            int                            m_SetID;

            vector<int>                m_vecSocketOption;                    // 왜 short로 되어있을까?

#ifdef USER_WEAPON_PARTICLE_SCRIPT_TEST
            vector< AttachedParticleData >        m_vecAttachedParticleData;   // note!! 일단 무기에만 붙어서 나오도록 구현함.
#endif USER_WEAPON_PARTICLE_SCRIPT_TEST


#ifdef SUB_EQUIP_TEST
            vector< AttachedMeshData >            m_vecAttachedMeshData;   // note!! 일단 무기에만 붙어서 나오도록 구현함.
#endif SUB_EQUIP_TEST



#ifdef ITEM_SLASH_TRACE_COLOR_TEST
            D3DXCOLOR                m_coSlashTrace;
            D3DXCOLOR                m_coSlashTraceHyper;
            D3DXCOLOR                m_coSlashTraceTip;
            D3DXCOLOR                m_coSlashTraceTipHyper;
#endif ITEM_SLASH_TRACE_COLOR_TEST

            //{{ kimhc // 2009-08-19 // 아이템 봉인
#ifdef    SEAL_ITEM
            BYTE                    m_ucMaxSealCount;        // 최대로 봉인할 수 있는 횟수
#endif    SEAL_ITEM
            //}} kimhc // 2009-08-19 // 아이템 봉인

            int                    m_iMaxAttribEnchantCount;
#ifdef PVP_SEASON2_SOCKET
            int                    m_iAttributeLevel;    /// 대전 무기 속성 레벨
#endif

            //{{ kimhc // 2009-10-19 // 최대 MP 증가 값 추가
            float                m_fAddMaxMP;
            //}} kimhc // 2009-10-19 // 최대 MP 증가 값 추가

//#ifdef DUNGEON_ITEM
//            SPECIAL_ITEM_TYPE    m_SpecialItemType;
//            wstring                m_wstrDamageEffectName;
//#endif

            //{{ 2010. 03. 22  최육사    기술의 노트
#ifdef SERV_SKILL_NOTE            
            std::wstring        m_DescriptionInSkillNote;
#endif SERV_SKILL_NOTE
            //}}

#ifdef SERV_PVP_NEW_SYSTEM
            CX2PVPEmblem::PVP_RANK    m_BuyPvpRankCondition;
#endif

            CX2UnitManager::NPC_UNIT_ID    m_iSummonNpcID;
#ifdef HIDE_SET_DESCRIPTION
			bool				m_bHideSetDesc;
#endif HIDE_SET_DESCRIPTION

            ItemTemplet()
            {
                m_UseType                = UT_NONE;
                m_UseCondition            = UC_ANYONE; 
                m_UseLevel                = 0;                // 레벨
                m_UnitType                = CX2Unit::UT_NONE;    // 유닛
                m_UnitClass                = CX2Unit::UC_NONE;    // 클래스

                m_ItemID                = -1;                //아이템 ID

                m_ItemType                = IT_NONE;
                m_bFashion                = false;            //패션 아이템
                m_bVested                = false;            //귀속 아이템(양도불능/판매불능)                
                m_bCanEnchant            = false;            //강화가능 아이템인가
                m_bCanUseInventory        = false;            //인벤토리에서 사용가능한 아이템인가
                //{{ 허상형 : [2009/7/29] //    변수 추가
                m_bNoEquip                = false;            //착용할수 없는 장비인가(교환용 장비)            
                //}} 허상형 : [2009/7/29] //    변수 추가
                //{{ 2010. 01. 05  최육사    PC방 프리미엄
                m_bIsPcBang                = false;            //PC방 전용 아이템
                //}}
                //{{ 2011. 10. 14    최육사    배틀필드 아이템 레벨 시스템
#ifdef SERV_BATTLE_FIELD_ITEM_LEVEL
                m_iItemLevel            = 0;                // 아이템 레벨 정보
#endif SERV_BATTLE_FIELD_ITEM_LEVEL
                //}}

                m_ItemGrade                = IG_NONE;

                m_FrameOffsetElsword    = D3DXVECTOR3( 0,0,0 );
                m_FrameOffsetLena        = D3DXVECTOR3( 0,0,0 );
                m_FrameOffsetAisha        = D3DXVECTOR3( 0,0,0 );
                m_FrameOffsetRaven        = D3DXVECTOR3( 0,0,0 );
                m_FrameOffsetEve        = D3DXVECTOR3( 0,0,0 );
                //{{ kimhc // 2010.11.18 // 2010-12-23 New Character CHUNG
#ifdef    NEW_CHARACTER_CHUNG
                m_FrameOffsetChung        = D3DXVECTOR3( 0,0,0 );
#endif    NEW_CHARACTER_CHUNG
                //}} kimhc // 2010.11.18 //  2010-12-23 New Character CHUNG
#ifdef ARA_CHARACTER_BASE
                m_FrameOffsetAra        = D3DXVECTOR3( 0,0,0 );
#endif


                for( int i = CX2Unit::UT_NONE; i < CX2Unit::UT_END; ++i )
                {
                    m_RanderScale[i] = D3DXVECTOR3(1.0f,1.0f,1.0f);
                    m_RanderRotate[i] = D3DXVECTOR3(0.0f,0.0f,0.0f);
                }

                m_bCanHyperMode            = false;            //하이퍼 모드 가능 아이템인가

                m_PeriodType            = PT_INFINITY;        //아이템 존재기간
                m_Endurance                = -1;                //초기내구도
                m_EnduranceDamage        = CMinMax<int>(0,0);        //1스테이지플레이당내구도 감소량
                //{{ 2007. 11. 16  최육사  
                m_RepairED                = 0.0f;                //내구도 1 수리당 ED
                m_RepairVP                = 0;                //내구도 1 수리당 VP
                //}}
                m_Quantity                = 0;                //1묶음수량

                m_PriceType                = SPT_NONE;            //가격타입
                m_Price                    = 0;                //가격
                m_PricePvPPoint            = 0;                //구매대전포인트

                m_EqipPosition            = CX2Unit::EP_NONE;    //장착 위치
                m_CoolTime                = 0;
                m_SetID                    = 0;



#ifdef ITEM_SLASH_TRACE_COLOR_TEST
                m_coSlashTrace                = 0xcc5555ff;
                m_coSlashTraceHyper         = 0xccff3333;
                m_coSlashTraceTip            = 0xff5555ff;
                m_coSlashTraceTipHyper        = 0xffff3333;
#endif ITEM_SLASH_TRACE_COLOR_TEST

                //{{ kimhc // 2009-08-19 // 아이템 봉인
#ifdef    SEAL_ITEM
                m_ucMaxSealCount            = 0;    // 기본 0, 0이면 봉인 할 수 없음                
#endif    SEAL_ITEM
                //}} kimhc // 2009-08-19 // 아이템 봉인

                m_iMaxAttribEnchantCount    = 0;

#ifdef PVP_SEASON2_SOCKET
                m_iAttributeLevel    = 0;
#endif

                //{{ kimhc // 2009-10-19 // 최대 MP 증가 값 추가
                m_fAddMaxMP                    = 0.0f;
                //}} kimhc // 2009-10-19 // 최대 MP 증가 값 추가

//#ifdef DUNGEON_ITEM
//                m_SpecialItemType = SIT_NONE;
//                m_wstrDamageEffectName = L"";
//#endif

                //{{ 2010. 03. 22  최육사    기술의 노트
#ifdef SERV_SKILL_NOTE                
                m_DescriptionInSkillNote = L"";
#endif SERV_SKILL_NOTE
                //}}

#ifdef SERV_PVP_NEW_SYSTEM
                m_BuyPvpRankCondition = CX2PVPEmblem::PVPRANK_NONE;
#endif

                m_iSummonNpcID = CX2UnitManager::NUI_NONE;
#ifdef BUFF_TEMPLET_SYSTEM
                m_Buff_ID = 0;
#endif BUFF_TEMPLET_SYSTEM
#ifdef HIDE_SET_DESCRIPTION
				m_bHideSetDesc = false;
#endif HIDE_SET_DESCRIPTION
            }

            int                GetSocketSlotNum() const;
            const wchar_t*     GetNameColor_() const;
            const wchar_t*     GetFullName_() const;    //아이템 등급에 따른 칭호 붙은 이름 : 견고한 견습전사 검

            int                 GetItemID() const                   { return m_ItemID; }

			bool    GetFashion() const                              { return m_bFashion; }
			bool    GetVested() const                               { return m_bVested; }
            bool    GetCanEnchant() const                           { return m_bCanEnchant; }
            bool    GetCanUseInventory() const                      { return m_bCanUseInventory; }
            bool    GetNoEquip() const                              { return m_bNoEquip; }
            bool    GetIsPcBang() const                             { return m_bIsPcBang; }
            bool    GetCanHyperMode() const                         { return m_bCanHyperMode; }
            USE_TYPE GetUseType() const                             { return m_UseType; }
            USE_CONDITION GetUseCondition() const                   { return m_UseCondition; }
            ITEM_TYPE GetItemType() const                           { return m_ItemType; }
            ITEM_GRADE GetItemGrade() const                         { return m_ItemGrade; }
            PERIOD_TYPE GetPeriodType() const                       { return m_PeriodType; }
            SHOP_PRICE_TYPE GetShopPriceType() const                { return m_PriceType; }

#ifdef  SERV_BATTLE_FIELD_ITEM_LEVEL
            int     GetItemLevel() const                            { return m_iItemLevel; }
#endif  SERV_BATTLE_FIELD_ITEM_LEVEL
            int     GetQuantity() const                             { return m_Quantity; }
            float   GetRepairED() const                             { return m_RepairED; }
            int     GetRepairVP() const                             { return m_RepairVP; }
            int     GetPrice() const                                { return m_Price; }
            int     GetPricePvPPoint() const                        { return m_PricePvPPoint; }

            int     GetSetID() const                                { return m_SetID; }
            //{{ kimhc // 2009-10-19 // 최대 MP 증가 값 추가
            float   GetAddMaxMP() const                             { return m_fAddMaxMP; }
            //}} kimhc // 2009-10-19 // 최대 MP 증가 값 추가

            int     GetEndurance() const                            { return m_Endurance; }
            int     GetEnduranceDamageMin() const                   { return m_EnduranceDamage.m_Min; }
            int     GetEnduranceDamageMax() const                   { return m_EnduranceDamage.m_Max; }
            CX2UnitManager::NPC_UNIT_ID GetSummonNpcID() const      { return m_iSummonNpcID; }
#ifdef  SERV_PVP_NEW_SYSTEM
            CX2PVPEmblem::PVP_RANK GetBuyPvpRankCondition() const   { return m_BuyPvpRankCondition; }
#endif  SERV_PVP_NEW_SYSTEM
            int     GetUseLevel() const                             { return m_UseLevel; }
            CX2Unit::UNIT_TYPE GetUnitType() const                  { return m_UnitType; }
            CX2Unit::UNIT_CLASS GetUnitClass() const                { return m_UnitClass; }
#ifdef  BUFF_TEMPLET_SYSTEM
            int     GetBuffID() const                               { return m_Buff_ID; }
#endif  BUFF_TEMPLET_SYSTEM
            CX2Unit::EQIP_POSITION GetEqipPosition() const          { return m_EqipPosition; }
            int     GetCoolTime() const                             { return m_CoolTime; }
#ifdef  SEAL_ITEM
            BYTE    GetMaxSealCount() const                         { return m_ucMaxSealCount; }
#endif  SEAL_ITEM
            int     GetMaxAttribEnchantCount() const                { return m_iMaxAttribEnchantCount; }
#ifdef  PVP_SEASON2_SOCKET
            int     GetAttributeLevel() const                       { return m_iAttributeLevel; }
#endif  PVP_SEASON2_SOCKET

            const wchar_t*  GetName() const     { return m_Name.c_str(); }
            const wchar_t*  GetFullName() const { return GetName(); }    //아이템 등급에 따른 칭호 붙은 이름 : 견고한 견습전사 검
            const wchar_t*  GetDescription() const  { return m_Description.c_str(); }
            const wchar_t*  GetDescriptionInShop() const    { return m_DescriptionInShop.c_str(); }
            const wchar_t*  GetAttachFrameName( unsigned i ) const  { ASSERT( i < MAX_MODEL_COUNT_A_ITEM ); return m_AttachFrameName[i].c_str(); }
            const wchar_t*  GetModelName( unsigned i ) const        { ASSERT( i < MAX_MODEL_COUNT_A_ITEM ); return m_ModelName[i].c_str(); }
            const wchar_t*  GetTextureChangeXETName() const { return m_TextureChangeXETName.c_str(); }
            const wchar_t*  GetAniXETName() const   { return m_AniXETName.c_str(); }
            const wchar_t*  GetAniName() const      { return m_AniName.c_str(); }
            const wchar_t*  GetShopImage() const    { return m_ShopImage.c_str(); }
            const wchar_t*  GetDropViewer() const   { return m_DropViewer.c_str(); }
            const wchar_t*  GetDescriptionInSkillNote() const   { return m_DescriptionInSkillNote.c_str(); }
            const CX2Stat::Stat&  GetStat() const   { return m_Stat; }
            unsigned        GetNumBuffFactorPtr() const { return m_vecBuffFactorPtr.size(); }
            CX2BuffFactorPtr GetBuffFactorPtr( unsigned i ) const   { ASSERT( i < m_vecBuffFactorPtr.size() ); return m_vecBuffFactorPtr[i]; }
            unsigned        GetNumSpecialAbility() const    { return m_SpecialAbilityList.size(); }
            const SpecialAbility&    GetSpecialAbility( unsigned i ) const { ASSERT( i < m_SpecialAbilityList.size() ); return  m_SpecialAbilityList[i]; }
            unsigned        GetNumSocketOption() const  { return m_vecSocketOption.size(); }
            int             GetSocketOption( unsigned i ) const { ASSERT( i < m_vecSocketOption.size() ); return m_vecSocketOption[i]; }

            unsigned        GetNumAttachedParticleData() const { return m_vecAttachedParticleData.size(); }
            const AttachedParticleData&   GetAttachedParticleData( unsigned i ) const { ASSERT( i < m_vecAttachedParticleData.size() ); return m_vecAttachedParticleData[i]; }
            unsigned        GetNumAttachedMeshData() const  { return m_vecAttachedMeshData.size(); }
            const AttachedMeshData& GetAttachedMeshData( unsigned i ) const { ASSERT( i < m_vecAttachedMeshData.size() ); return m_vecAttachedMeshData[i]; }
            const wchar_t*  GetCommonItemModelName( CX2Unit::UNIT_TYPE eUnitType ) const
            {
                std::map<CX2Unit::UNIT_TYPE, wstring>::const_iterator iter = m_mapCommonItemModelName.find( eUnitType );
                if ( iter != m_mapCommonItemModelName.end() )
                    return iter->second.c_str();
                return L"";
            }
            const wchar_t*  GetCommonItemXETName( CX2Unit::UNIT_TYPE eUnitType ) const
            {
                std::map<CX2Unit::UNIT_TYPE, wstring>::const_iterator iter = m_mapCommonItemXETName.find( eUnitType );
                if ( iter != m_mapCommonItemXETName.end() )
                    return iter->second.c_str();
                return L"";
            }
            //void            GetSlashTraceColors( D3DXCOLOR& dwOut_, D3DXCOLOR& dwOutTip_, D3DXCOLOR& dwOutHyper_, D3DXCOLOR& dwOutHyperTip_ ) const;

            class LessID 
            {
            public:
                bool operator() ( const ItemTemplet*& lhs, const ItemTemplet*& rhs )
                {
                    if( lhs->m_ItemID < rhs->m_ItemID )
                        return true;
                    else
                        return false;
                }
            };

            class LessName
            {
            public:
                bool operator() ( const ItemTemplet*& lhs, const ItemTemplet*& rhs )
                {
                    if( lhs->m_Name.compare( rhs->m_Name ) < 0 )
                        return true;
                    else 
                        return false;
                }
            };

        };

#endif  //X2OPTIMIZE_ITEM_TEMPLET_PREPROCESSING

        struct ItemData : public CX2ItemData_Base
        {

            UidType				m_ItemUID;           //UID
            int					m_ItemID;

            PERIOD_TYPE			m_PeriodType;
            int					m_Endurance;         //내구도
            int					m_Quantity;          //수량

            int					m_EnchantLevel;

            CX2EnchantItem::ItemEnchantedAttribute    m_EnchantedAttribute;

            int					m_Period;            //몇일 짜리 기간제 아이템인가?
            std::wstring		m_wstrExpirationDate;//날짜

            //{{ kimhc // 2009-08-20 // 아이템 봉인
#ifdef    SEAL_ITEM
            bool				m_bIsSealed;         // 봉인된 상태인가?
            BYTE				m_ucTimesToBeSealed; // 봉인된 횟수
#endif    SEAL_ITEM
            //}} kimhc // 2009-08-20 // 아이템 봉인

			vector<int>			m_SocketOption;      
#ifdef SERV_NEW_ITEM_SYSTEM_2013_05
			std::vector< int >	m_vecRandomSocket;	 // 기본 미감정 상태의 랜덤 소켓
			bool				m_bIsEvaluation;		 // 아이템 감정 여부
#endif // SERV_NEW_ITEM_SYSTEM_2013_05

#ifdef ADD_SOCKET_SLOT
			BYTE				m_byAddedSocketSlot;	// 추가 소켓 슬롯 수
#endif // ADD_SOCKET_SLOT

        private:

            void    Init()
            {
                m_ItemUID                = 0;                //UID
                m_ItemID                = 0;

                m_PeriodType            = PT_INFINITY;
                m_Endurance                = -1;                //내구도
                m_Quantity                = 0;                //수량

                m_EnchantLevel            = 0;
                m_EnchantedAttribute.Init();

                //m_EnchantedAttribute.m_aEnchantedType[0]         = 0;
                //m_EnchantedAttribute.m_aEnchantedType[1]         = 0;
                //m_EnchantedAttribute.m_aEnchantedType[2]         = 0;

                m_Period                = 0;
                m_wstrExpirationDate    = L"";

                //{{ kimhc // 2009-08-20 // 아이템 봉인
#ifdef    SEAL_ITEM
                m_bIsSealed                = false;                    // 봉인된 상태인가?
                m_ucTimesToBeSealed        = 0;                        // 봉인된 횟수
#endif    SEAL_ITEM

                m_SocketOption.resize(0);
                //}} kimhc // 2009-08-20 // 아이템 봉인
#ifdef SERV_NEW_ITEM_SYSTEM_2013_05
                m_vecRandomSocket.resize( 0 );
				m_bIsEvaluation	= true;	// 아이템 감정 여부
#endif // SERV_NEW_ITEM_SYSTEM_2013_05
#ifdef ADD_SOCKET_SLOT
				m_byAddedSocketSlot = 0;	// 추가 소켓 슬롯 수
#endif // ADD_SOCKET_SLOT
            }

            void    Init( const KInventoryItemInfo& data )
            {
                m_ItemUID               = data.m_iItemUID;                    //UID
                m_ItemID                = data.m_kItemInfo.m_iItemID;

                m_PeriodType            = (CX2Item::PERIOD_TYPE) data.m_kItemInfo.m_cUsageType;
                m_Endurance             = static_cast<int>(data.m_kItemInfo.m_sEndurance);                //내구도
                m_Quantity              = data.m_kItemInfo.m_iQuantity;                    //수량

                m_EnchantLevel          = (int)data.m_kItemInfo.m_cEnchantLevel;
                m_EnchantedAttribute.m_aEnchantedType[0] = (CX2EnchantItem::ENCHANT_TYPE) data.m_kItemInfo.m_kAttribEnchantInfo.m_cAttribEnchant0;
                m_EnchantedAttribute.m_aEnchantedType[1] = (CX2EnchantItem::ENCHANT_TYPE) data.m_kItemInfo.m_kAttribEnchantInfo.m_cAttribEnchant1;
                m_EnchantedAttribute.m_aEnchantedType[2] = (CX2EnchantItem::ENCHANT_TYPE) data.m_kItemInfo.m_kAttribEnchantInfo.m_cAttribEnchant2;

                m_Period                = (int)data.m_kItemInfo.m_sPeriod;
                m_wstrExpirationDate    = data.m_kItemInfo.m_wstrExpirationDate;

                //{{ kimhc // 2009-08-26 // 아이템 봉인
#ifdef    SEAL_ITEM
                m_bIsSealed                = data.m_kItemInfo.IsSealedItem();    // 봉인된 상태인가?
                m_ucTimesToBeSealed        = data.m_kItemInfo.GetSealCount();    // 봉인된 횟수
#endif    SEAL_ITEM
                //}} kimhc // 2009-08-26 // 아이템 봉인

                m_SocketOption          = data.m_kItemInfo.m_vecItemSocket;
#ifdef SERV_NEW_ITEM_SYSTEM_2013_05
				m_vecRandomSocket		= data.m_kItemInfo.m_vecRandomSocket;
				// m_cItemState가 미감정 상태가 아니라면 감정 된 것으로 설정
				m_bIsEvaluation			= (KItemInfo::IS_NOT_EVALUATED != data.m_kItemInfo.m_cItemState);
#endif // SERV_NEW_ITEM_SYSTEM_2013_05

#ifdef ADD_SOCKET_SLOT
				m_byAddedSocketSlot = data.m_kItemInfo.m_byteExpandedSocketNum;	// 추가 소켓 슬롯 수
#endif // ADD_SOCKET_SLOT
            }

            void    Init( const KItemInfo& data )
            {
                m_ItemUID               = -1;                    //UID
                m_ItemID                = data.m_iItemID;

                m_PeriodType            = (CX2Item::PERIOD_TYPE) data.m_cUsageType;
                m_Endurance             = static_cast<int>(data.m_sEndurance);                //내구도
                m_Quantity              = data.m_iQuantity;                    //수량

                m_EnchantLevel          = (int)data.m_cEnchantLevel;
                m_EnchantedAttribute.m_aEnchantedType[0] = (CX2EnchantItem::ENCHANT_TYPE) data.m_kAttribEnchantInfo.m_cAttribEnchant0;
                m_EnchantedAttribute.m_aEnchantedType[1] = (CX2EnchantItem::ENCHANT_TYPE) data.m_kAttribEnchantInfo.m_cAttribEnchant1;
                m_EnchantedAttribute.m_aEnchantedType[2] = (CX2EnchantItem::ENCHANT_TYPE) data.m_kAttribEnchantInfo.m_cAttribEnchant2;

                m_Period                = (int)data.m_sPeriod;
                m_wstrExpirationDate    = data.m_wstrExpirationDate;


                //{{ kimhc // 2009-08-26 // 아이템 봉인
#ifdef    SEAL_ITEM
                m_bIsSealed                = data.IsSealedItem();            // 봉인된 상태인가?
                m_ucTimesToBeSealed        = data.GetSealCount();            // 봉인된 횟수
#endif    SEAL_ITEM
                //}} kimhc // 2009-08-26 // 아이템 봉인

				m_SocketOption          = data.m_vecItemSocket;
#ifdef SERV_NEW_ITEM_SYSTEM_2013_05
				m_vecRandomSocket		= data.m_vecRandomSocket;
				// m_cItemState가 미감정 상태가 아니라면 감정 된 것으로 설정
				m_bIsEvaluation			= (KItemInfo::IS_NOT_EVALUATED != data.m_cItemState);
#endif // SERV_NEW_ITEM_SYSTEM_2013_05

#ifdef ADD_SOCKET_SLOT
				m_byAddedSocketSlot = data.m_byteExpandedSocketNum;	// 추가 소켓 슬롯 수
#endif // ADD_SOCKET_SLOT
            }

            void    Init( const KPostItemInfo& data )
            {
                m_ItemUID               = -1;                    //UID
                m_ItemID                = data.m_iScriptIndex;

                m_PeriodType            = PT_INFINITY;
                m_Endurance                = -1;                //내구도
                m_Quantity              = data.m_iQuantity;                    //수량

                m_EnchantLevel          = (int)data.m_cEnchantLevel;
                m_EnchantedAttribute.m_aEnchantedType[0] = (CX2EnchantItem::ENCHANT_TYPE) data.m_kAttribEnchantInfo.m_cAttribEnchant0;
                m_EnchantedAttribute.m_aEnchantedType[1] = (CX2EnchantItem::ENCHANT_TYPE) data.m_kAttribEnchantInfo.m_cAttribEnchant1;
                m_EnchantedAttribute.m_aEnchantedType[2] = (CX2EnchantItem::ENCHANT_TYPE) data.m_kAttribEnchantInfo.m_cAttribEnchant2;

                m_Period                = 0;
                m_wstrExpirationDate.resize( 0 );

                //{{ kimhc // 2009-08-26 // 아이템 봉인
#ifdef    SEAL_ITEM
                m_bIsSealed                = data.IsSealedItem();            // 봉인된 상태인가?
                m_ucTimesToBeSealed        = data.GetSealCount();            // 봉인된 횟수
#endif    SEAL_ITEM
                //}} kimhc // 2009-08-26 // 아이템 봉인

				m_SocketOption          = data.m_vecItemSocket;
#ifdef SERV_NEW_ITEM_SYSTEM_2013_05
				m_vecRandomSocket		= data.m_vecRandomSocket;
				// m_cItemState가 미감정 상태가 아니라면 감정 된 것으로 설정
				m_bIsEvaluation			= (KItemInfo::IS_NOT_EVALUATED != data.m_cItemState);
#endif // SERV_NEW_ITEM_SYSTEM_2013_05

#ifdef ADD_SOCKET_SLOT
				m_byAddedSocketSlot = data.m_byteExpandedSocketNum;	// 추가 소켓 슬롯 수
#endif // ADD_SOCKET_SLOT
            }

        public:

            ItemData()
            {
                Init();
            }

            ItemData( const KInventoryItemInfo& data )
            {
                Init( data );
            }

            ItemData( const KItemInfo& data )
            {
                Init( data );
            }
//{{ robobeg : 2013-11-04
            bool    Initialize( const KPostItemInfo& data );
//}} robobeg : 2013-11-04

            ItemData& operator=( const KInventoryItemInfo& data )
            {
                m_ItemUID               = data.m_iItemUID;                    //UID
                m_ItemID                = data.m_kItemInfo.m_iItemID;

                m_PeriodType            = static_cast<CX2Item::PERIOD_TYPE>( data.m_kItemInfo.m_cUsageType );
                m_Endurance             = static_cast<int>(data.m_kItemInfo.m_sEndurance);                //내구도
                m_Quantity              = data.m_kItemInfo.m_iQuantity;                    //수량

                m_EnchantLevel          = static_cast<int>( data.m_kItemInfo.m_cEnchantLevel );
				m_SocketOption          = data.m_kItemInfo.m_vecItemSocket;
#ifdef SERV_NEW_ITEM_SYSTEM_2013_05
				m_vecRandomSocket		= data.m_kItemInfo.m_vecRandomSocket;
				// m_cItemState가 미감정 상태가 아니라면 감정 된 것으로 설정
				m_bIsEvaluation			= (KItemInfo::IS_NOT_EVALUATED != data.m_kItemInfo.m_cItemState);
#endif // SERV_NEW_ITEM_SYSTEM_2013_05

                m_Period                = static_cast<int>( data.m_kItemInfo.m_sPeriod );
                m_wstrExpirationDate    = data.m_kItemInfo.m_wstrExpirationDate;

                m_EnchantedAttribute.m_aEnchantedType[0] = static_cast<CX2EnchantItem::ENCHANT_TYPE>( data.m_kItemInfo.m_kAttribEnchantInfo.m_cAttribEnchant0 );
                m_EnchantedAttribute.m_aEnchantedType[1] = static_cast<CX2EnchantItem::ENCHANT_TYPE>( data.m_kItemInfo.m_kAttribEnchantInfo.m_cAttribEnchant1 );
                m_EnchantedAttribute.m_aEnchantedType[2] = static_cast<CX2EnchantItem::ENCHANT_TYPE>( data.m_kItemInfo.m_kAttribEnchantInfo.m_cAttribEnchant2 );

                //{{ kimhc // 2009-08-26 // 아이템 봉인
#ifdef    SEAL_ITEM
                m_bIsSealed                = data.m_kItemInfo.IsSealedItem();    // 봉인된 상태인가?
                m_ucTimesToBeSealed        = data.m_kItemInfo.GetSealCount();    // 봉인된 횟수
#endif    SEAL_ITEM
                //}} kimhc // 2009-08-26 // 아이템 봉인

#ifdef ADD_SOCKET_SLOT
				m_byAddedSocketSlot = data.m_kItemInfo.m_byteExpandedSocketNum;	// 추가 소켓 슬롯 수
#endif // ADD_SOCKET_SLOT 
                return *this;
            }



            int GetAttribEnchantedCount() const
            {
                int iCount = 0;

                for( int i = 0; i<ARRAY_SIZE( m_EnchantedAttribute.m_aEnchantedType ) ; i++ )
                {
                    if( m_EnchantedAttribute.m_aEnchantedType[i] > 0 )
                    {
                        ++iCount;
                    }
                }

                return iCount;
            }

        };

    public:
        CX2Item( int itemID );
//{{ robobeg : 2013-11-04
        //CX2Item( ItemData* pItemData, CX2Unit* pOwnerUnit );
        CX2Item( const ItemData& kItemData, CX2Unit* pOwnerUnit );
//}} robobeg : 2013-11-04
        ~CX2Item(void);

//{{ robobeg : 2013-11-04
        //ItemData*        GetItemData()            { return m_pItemData; }
        const ItemData&    GetItemData() const      { return m_kItemData; }
        ItemData&          AccessItemData()         { return m_kItemData; }
//}} robobeg : 2013-11-04
        const ItemTemplet*  GetItemTemplet() const    { return m_pItemTemplet; }
		int					GetItemEnchantLevel() const { return m_kItemData.m_EnchantLevel; }

        UidType            GetUID() const            { return m_kItemData.m_ItemUID; }
        void            SetEqip( bool bEqip )    { m_bEqip = bEqip; }
        bool            GetEqip()                { return m_bEqip; }

        CX2Unit*        GetOwnerUnit()            { return m_pOwnerUnit; }

        //float            GetRepairPriceDiscountRate();
        int                GetEDToRepair();
        int                GetVPToRepair();

        int                GetEDToSell();                //아이템 판매가

        int                GetEDToSocketPush();        //박을 때 수수료
        int                GetEDToSocketRemove();

        int                GetSocketOptionNum() const;        //적용된 옵션 갯수 ( 박힌 마법석 갯수 )

#ifdef ITEM_RECOVERY_TEST
        bool            IsDisabled();
#endif


        CX2Stat::Stat   GetStat( bool bIncludeSocketOption = false );
        bool            GetEnchantStat( OUT CX2Stat::Stat& stat_ ) const;
		int             GetIEchantedItemLevel() const;    /// 강화와 관련하여 증가된 아이템 레벨
#ifdef SERV_NEW_ITEM_SYSTEM_2013_05
		CX2Stat::Stat   GetStat( IN CX2Unit::UNIT_TYPE eUnitType_, IN CX2Unit::UNIT_CLASS eUnitClass_ );
		bool            GetEnchantStat( OUT CX2Stat::Stat& stat_, IN CX2Unit::UNIT_TYPE eUnitType_, IN CX2Unit::UNIT_CLASS eUnitClass_ ) const;
		int             GetIEchantedItemLevel(IN CX2Unit::UNIT_TYPE eUnitType_, IN CX2Unit::UNIT_CLASS eUnitClass_) const;    /// 강화와 관련하여 증가된 아이템 레벨

		// 감정 여부 얻기. 기본값은 true
        bool			GetIsEvaluation() const { return m_kItemData.m_bIsEvaluation; }
#endif //SERV_NEW_ITEM_SYSTEM_2013_05

        //+0 불패칸 검
#ifdef SERV_UPGRADE_TRADE_SYSTEM // 김태환
		wstring            GetFullName( IN const bool bAddRandomSocketName_ = false );
#else // SERV_UPGRADE_TRADE_SYSTEM
		wstring            GetFullName();
#endif // SERV_UPGRADE_TRADE_SYSTEM

#ifdef ADD_SOCKET_SLOT
		BYTE			GetbyAddedSocketSlot() const;
#endif // ADD_SOCKET_SLOT 

        //static ItemData* CreateItemData( const KPostItemInfo& kPostItemInfo );

    public:
        static const int        WEAPON_LEVEL_WEIGHT = 4;            /// 아이템 평균 레벨을 구할 때 사용하는 무기 아이템의 레벨 가중치
        static const int        AVERAGE_ITEM_LEVEL_DIVISOR = 8;        /// 아이템 평균 레벨을 구할 때 사용하는 제수

    private:
        static const int        STANDARD_VALUE_PHYSIC_ATTACK = 120;    /// 아이템 1레벨 물공 증가치 (이만큼이 증가해야 1레벨이 오름)
        static const int        STANDARD_VALUE_MAGIC_ATTACK = 120;    /// 아이템 1레벨 마공 증가치    (이만큼이 증가해야 1레벨이 오름)
        static const int        STANDARD_VALUE_PHYSIC_DEFENCE = 30;/// 아이템 1레벨 물방 증가치    (이만큼이 증가해야 1레벨이 오름)
        static const int        STANDARD_VALUE_MAGIC_DEFENCE = 30;    /// 아이템 1레벨 마방 증가치    (이만큼이 증가해야 1레벨이 오름)
        static const int        STANDARD_VALUE_BASE_HP = 3000;        /// 아이템 1레벨 HP 증가치    (이만큼이 증가해야 1레벨이 오름)

        //ItemData*        m_pItemData;
        ItemData        m_kItemData;
        const ItemTemplet*    m_pItemTemplet;
        CX2Unit*        m_pOwnerUnit;

        bool            m_bEqip;
};
