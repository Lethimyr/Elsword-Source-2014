#include "StdAfx.h"
#include ".\x2user.h"

CX2User::CX2User( const UserData& kUserData )
    : m_UserData( kUserData )
{
	//m_pUserData		= pUserData;
	m_pSelectUnit	= NULL;

	m_Cash = 0;

	m_UnitList.reserve( 16 );
#ifdef AVATAR_IMAGE_TEST
	m_uiNexonSN = 0;
#endif
#ifdef SERV_EVENT_MONEY
	m_iAngelsFeather = 0;
#endif //SERV_EVENT_MONEY
#ifdef SEND_NEXON_WISE_LOG_BY_URL // 김태환
	m_bFirstCreateCharacter = false;
#endif // SEND_NEXON_WISE_LOG_BY_URL

#ifdef HIDE_SET_DESCRIPTION
	m_bHideSetDesc = true;
#endif HIDE_SET_DESCRIPTION

#ifdef SERV_SHARING_BANK_QUEST_CASH
	m_bSharingBankOpen = false;
#endif // SERV_SHARING_BANK_QUEST_CASH

#ifdef SERV_SUPPORT_SEVERAL_CASH_TYPES
	m_ulGlobalTotalCash = 0;
#endif //SERV_SUPPORT_SEVERAL_CASH_TYPES

#ifdef SERV_EVENT_TEAR_OF_ELWOMAN
	m_iTearOfELWoman = 0;
#endif SERV_EVENT_TEAR_OF_ELWOMAN
}

CX2User::~CX2User(void)
{
	for( int i = 0; i < (int)m_UnitList.size(); i++ )
	{
		CX2Unit* pTempUnit = m_UnitList[i];
		SAFE_DELETE( pTempUnit );
	}
	m_UnitList.clear();

	//SAFE_DELETE( m_pUserData );
}

bool CX2User::AddUnit( CX2Unit* pUnit )
{
	m_UnitList.push_back( pUnit );
	return true;
}

CX2Unit* CX2User::GetUnitByUID( UidType unitUID )
{
	for( int i = 0; i < (int)m_UnitList.size(); i++ )
	{
		CX2Unit* pUnit = m_UnitList[i];
		if( pUnit->GetUID() == unitUID )
			return pUnit;
	}
	return NULL;
}

CX2Unit* CX2User::GetUnitByIndex( int index )
{
	if( index >= (int)m_UnitList.size() )
		return NULL;

	CX2Unit* pUnit = m_UnitList[index];
	return pUnit;
}

CX2Unit* CX2User::GetUnitByNickName( const WCHAR* pNickName )
{
	for( int i = 0; i < (int)m_UnitList.size(); i++ )
	{
		CX2Unit* pUnit = m_UnitList[i];
		if( wcscmp( pUnit->GetNickName(), pNickName ) == 0 )
			return pUnit;
	}
	return NULL;
}

bool CX2User::DeleteUnitByUID( UidType unitUID )
{
	vector< CX2Unit* >::iterator i;

	for( i = m_UnitList.begin(); i != m_UnitList.end(); i++ )
	{
		CX2Unit* pUnit = (*i);
		if( pUnit->GetUID() == unitUID )
		{
			if ( pUnit == m_pSelectUnit )
				m_pSelectUnit = NULL;

			SAFE_DELETE( pUnit );
			m_UnitList.erase(i);
			return true;
		}
	}

	return false;
}

void CX2User::DeleteAllUnit()
{
	vector< CX2Unit* >::iterator i;

	for( i = m_UnitList.begin(); i != m_UnitList.end(); i++ )
	{
		CX2Unit* pUnit = (*i);
		SAFE_DELETE( pUnit );
	}

	m_UnitList.clear();

	m_pSelectUnit = NULL;
}

bool CX2User::IsMyUnitByUnitUID( UidType unitUid )
{
	vector< CX2Unit* >::iterator i;

	for( i = m_UnitList.begin(); i != m_UnitList.end(); i++ )
	{
		CX2Unit* pUnit = (*i);
		if( pUnit->GetUID() == unitUid )
		{
			return true;
		}
	}

	return false;
}

#ifdef SERV_SUPPORT_SEVERAL_CASH_TYPES
void CX2User::SetGlobalCash(KGlobalCashInfo globalCashInfo)
{ 
	m_GlobalCashInfo = globalCashInfo;

	// Get Total Cash //
	m_ulGlobalTotalCash = 0; // init
	for (int i=0; i < KGlobalCashInfo::GCT_TOTAL_NUM; i++)
	{
		m_ulGlobalTotalCash += globalCashInfo.m_ulCash[i];
	}
}

void CX2User::SetGlobalTypeCash(int iCashType, int ulCash)
{
	if (iCashType < 0 || iCashType >= KGlobalCashInfo::GCT_TOTAL_NUM)
		return;

	m_GlobalCashInfo.m_ulCash[iCashType] = ulCash;

	// Get Total Cash //
	m_ulGlobalTotalCash = 0; // init
	for (int i=0; i < KGlobalCashInfo::GCT_TOTAL_NUM; i++)
	{
		m_ulGlobalTotalCash += m_GlobalCashInfo.m_ulCash[i];
	}
}

unsigned long CX2User::GetGlobalCash(int iCashType)
{
	if (iCashType < 0 || iCashType >= KGlobalCashInfo::GCT_TOTAL_NUM)
		return 0;

	return m_GlobalCashInfo.m_ulCash[iCashType];
}
unsigned long CX2User::GetGlobalCashForRescurrection()
{
	int iCashType = KGlobalCashInfo::GCT_PUBLISHER_CASH;

#if defined (SERV_COUNTRY_CN) || defined(CLIENT_COUNTRY_PH)
	iCashType = KGlobalCashInfo::GCT_KOG_ELSWORD_CASH;
#elif defined(SERV_COUNTRY_TWHK)
	iCashType = KGlobalCashInfo::GCT_PUBLISHER_ELSWORD_CASH;
#endif //SERV_COUTNRY_CN

	if (iCashType < 0 || iCashType >= KGlobalCashInfo::GCT_TOTAL_NUM)
		return 0;

	return m_GlobalCashInfo.m_ulCash[iCashType];
}

unsigned long CX2User::GetGlobalMainCash()
{
	int iCashType = KGlobalCashInfo::GCT_PUBLISHER_CASH;

#if defined(SERV_COUNTRY_CN) || defined(CLIENT_COUNTRY_PH)
	iCashType = KGlobalCashInfo::GCT_KOG_ELSWORD_CASH;
#elif defined(SERV_COUNTRY_TWHK)
	iCashType = KGlobalCashInfo::GCT_PUBLISHER_ELSWORD_CASH;
#endif //SERV_COUTNRY_CN

	if (iCashType < 0 || iCashType >= KGlobalCashInfo::GCT_TOTAL_NUM)
		return 0;

	return m_GlobalCashInfo.m_ulCash[iCashType];
}
#endif //SERV_SUPPORT_SEVERAL_CASH_TYPES

#ifdef LIST_SORT_AT_CHARACTER_SELECT
bool CX2User::SortFirstByUID(UidType aUnitUID)
{
	auto FirstByUidFunc = [aUnitUID]( const CX2Unit* pUnit_ ) {
		return pUnit_->GetUID() == aUnitUID;
	};

	vector<CX2Unit*>::iterator it = find_if(m_UnitList.begin(), m_UnitList.end(), FirstByUidFunc );
	if( m_UnitList.end() == it ) 
		return false;
	else
	{
		CX2Unit* pSelectUnit = *it;
		m_UnitList.erase(it);
		m_UnitList.insert(m_UnitList.begin(), pSelectUnit);
		return true;
	}
}
#endif // LIST_SORT_AT_CHARACTER_SELECT

#ifdef FIX_SKILL_SLOT_COOLTIME_IN_VILLAGE //2013.08.09
void CX2User::ChangeEquippedSkillState( const int iSkillSlotId_, const int iSkillId_ )
{
	if ( NULL == g_pData->GetMyUser()->GetSelectUnit() )
		return;

	CX2Unit::UnitData* pUnitData = &g_pData->GetMyUser()->GetSelectUnit()->AccessUnitData();

	// iSkillSlotId_가 B슬롯인지 아닌지..
	bool bSlotB = false;
	// B슬롯이면 B슬롯의 시작 슬롯을 현재 인자로 넘겨진 아이디에서 빼서 (0~3) 사이의 슬롯 Index를 구함
	int iSlotIndex = 0;

	CX2UserSkillTree::GetSlotIndexAndSlotB( IN iSkillSlotId_, OUT iSlotIndex, OUT bSlotB );
	const CX2SkillTree::SKILL_ID eSkillId = static_cast<CX2SkillTree::SKILL_ID>( iSkillId_ );
	const int iSkillLevel = pUnitData->m_UserSkillTree.UpdateEquippedSkillLevelFromAcqureidMap( eSkillId, iSlotIndex, bSlotB );

	const CX2SkillTree::SkillTemplet* pSkillTemplet = g_pData->GetSkillTree()->GetSkillTemplet( eSkillId );
	if ( NULL != pSkillTemplet )
	{
		pUnitData->m_UserSkillTree.SetSkillCoolTimeLeft( eSkillId, pSkillTemplet->GetSkillCoolTimeValue( iSkillLevel ) );
	}
}
#endif //FIX_SKILL_SLOT_COOLTIME_IN_VILLAGE

#ifdef SERV_UNIT_WAIT_DELETE
int CX2User::GetIndexByUID( UidType unitUID )
{
	for( int i = 0; i < (int)m_UnitList.size(); i++ )
	{
		CX2Unit* pUnit = m_UnitList[i];
		if( pUnit->GetUID() == unitUID )
			return i;
	}
	return -1;
}
#endif SERV_UNIT_WAIT_DELETE