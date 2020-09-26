#pragma once
#ifdef SERV_NEW_ITEM_SYSTEM_2013_05

namespace _CONST_
{
	const UINT MAX_ITEM_TEMPLET_STAT_COUNT = 100;
}

class CX2ItemStatCalculator
{
public:

	// 스크립트 파싱 할 때 사용하는 enum
	enum TABLE_DATA_TYPE
	{
		TDT_TYPE = 1,
		TDT_ATK_PHYSIC,
		TDT_DEF_PHYSIC,
		TDT_ATK_MAGIC,
		TDT_DEF_MAGIC,
		TDT_HP,
	};

	// ItemStat을 구할 때 가변적인 요소들 키값으로 설정
	struct ItemStatKey
	{
		int									m_iItemID;		// 아이템ID
		CX2Unit::UNIT_CLASS					m_eUnitClass;		// 클래스
		CX2SocketItem::KItemStatRelLVData	m_sStatRelLVData;	// 스탯별 추가 레벨

		ItemStatKey():
		m_iItemID(0),
		m_eUnitClass(CX2Unit::UC_NONE)
		{}

		ItemStatKey( int iItemdID_, CX2Unit::UNIT_CLASS eUnitClass_, const CX2SocketItem::KItemStatRelLVData& sStatRelLVData_ ):
		m_iItemID(iItemdID_),
		m_eUnitClass( eUnitClass_ ),
		m_sStatRelLVData( sStatRelLVData_ )
		{}

		// Map 컨테이너 Find를 위한 연산자 오버로딩
		bool operator < ( const ItemStatKey& rhs ) const 
		{
			if( m_iItemID < rhs.m_iItemID )
				return true;
			else if( m_iItemID > rhs.m_iItemID )
				return false;

			if( m_eUnitClass < rhs.m_eUnitClass )
				return true;
			else if( m_eUnitClass > rhs.m_eUnitClass )
				return false;

			if( m_sStatRelLVData < rhs.m_sStatRelLVData )
				return true;

			return false;
		}
	};

public:
	CX2ItemStatCalculator();
	~CX2ItemStatCalculator();

	void ResetItemStatCalculator();

	void CalculateItemStat( OUT CX2Item::KItemFormatStatData& sRealStatData_, IN const CX2Item::ItemTemplet* pItemTemplet_,
							IN CX2Unit::UNIT_TYPE eUnitType_, IN CX2Unit::UNIT_CLASS eUnitClass_)
	{
		CalculateItemStat( sRealStatData_, pItemTemplet_, CX2SocketItem::ms_KDummyItemStatRelLVData, eUnitType_, eUnitClass_ );
	}

	void CalculateItemStat( OUT CX2Item::KItemFormatStatData& sRealStatData_, IN const CX2Item::ItemTemplet* pItemTemplet_,
							IN const CX2SocketItem::KItemStatRelLVData& sStatRelLVData_ = CX2SocketItem::ms_KDummyItemStatRelLVData,
							IN CX2Unit::UNIT_TYPE eUnitType_ = CX2Unit::UT_NONE, IN CX2Unit::UNIT_CLASS eUnitClass_ = CX2Unit::UC_NONE );
	void SetUnitData( CX2Unit::UNIT_TYPE eUnitType_, CX2Unit::UNIT_CLASS eUnitClass_ );

	bool IsRandomSocketOptionItem( const CX2Item::ItemTemplet* pItemTemplet ) const;

	// 사내 테스트를 위해 보조 디스크립션 추가
	void GetItemDescDev( OUT wstring& wstrItemDesc_, 
						IN const CX2Item::ItemTemplet* pItemTemplet ) const;

	void GetSocketOptionStatRelLV( OUT CX2SocketItem::KItemStatRelLVData& kItemStatRelLVData_,
									IN const CX2Item::ItemData& kItemData_, IN const CX2Item::ItemTemplet* pItemTemplet_) const;

private:	
	void OpenScriptFile();
	bool InvalideDataCheck() const;
	void GetStatDataTable( IN CX2Unit::UNIT_TYPE eUnitType_, IN CX2Unit::UNIT_CLASS eUnitClass_,IN CX2Unit::EQIP_POSITION eEqipPosition_, 
							OUT CX2Item::KItemFormatStatData& sCharStat_, OUT CX2Item::KItemFormatStatData& sClassStat, OUT CX2Item::KItemFormatStatData& sEqipStat ) const;
	void CalculateRealStatData( OUT CX2Item::KItemFormatStatData& sRealStatData_,
								IN const CX2Item::KItemFormatStatData& sClassStatTable_, 
								IN const CX2Item::KItemFormatStatData& sCharStatTable_, 
								IN const CX2Item::KItemFormatStatData& sEqipStat_, 
								IN int iItemLevel_,
								IN const CX2SocketItem::KItemStatRelLVData& sStatRelLVData_) const;

	map< CX2Unit::UNIT_TYPE, CX2Item::KItemFormatStatData >			m_mapCharStatDataTable;  // 캐릭터별 스탯 테이블
	map< CX2Unit::UNIT_CLASS, CX2Item::KItemFormatStatData >		m_mapClassStatDataTable; // 전직별 스탯 테이블(2차 전직이 아니라면 캐릭터별 능력치 사용)
	map< CX2Unit::EQIP_POSITION, CX2Item::KItemFormatStatData >		m_mapEqipStatDataTable;  // 장비 장착 부위별 스탯 테이블
	
	map< ItemStatKey, CX2Item::KItemFormatStatData >				m_mapItemTemplmetStat;   // 한 번 계산된 아이템에 대한 정보 기록(연산 횟수 줄이기 위한 변수)

	CX2Unit::UNIT_TYPE				m_eUnitType;
	CX2Unit::UNIT_CLASS				m_eUnitClass;
};

#endif //SERV_NEW_ITEM_SYSTEM_2013_05