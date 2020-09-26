#include "StdAfx.h"

#ifdef UPGRADE_SKILL_SYSTEM_2013

#include ".\x2uiskilltreeNew.h"

const int	MAGIC_SKILL_DESC_WIDTH					= 370;		/// 스킬 툴팁 스트링 한줄당 최대 가로 길이
const int	MAX_ONE_CLASS_SKILL_ROW_NUM				= 5;		/// 하나의 전직당 스킬 트리 최대 행 개수
const int	SKILL_USE_IMAGE_WIDTH_SIZE				= 70;		/// 스킬 시전 샘플 이미지 가로 크기
const float SKILL_MAIN_DESC_OFFSET_Y				= 65.f;		/// 메인 툴팁의 오프셋 세로 위치

//////////////////////////////////////////////////////////////////////////
/// CX2SkillTreeSlot : 스킬 트리에 나오는 슬롯
//////////////////////////////////////////////////////////////////////////
CX2SkillTreeSlotData::CX2SkillTreeSlotData()
{
	m_eSlotDataType = SDT_SKILL_TREE;
	m_eSkillID = CX2SkillTree::SI_NONE;
}
/*virtual*/ CX2SkillTreeSlotData::~CX2SkillTreeSlotData()
{


}
/*virtual*/ void CX2SkillTreeSlotData::UpdateUI( CKTDGUISlot* pSlot )
{   
	if ( NULL == g_pData ||
		 NULL == g_pData->GetMyUser() ||
		 NULL == g_pData->GetMyUser()->GetSelectUnit() ||
		 NULL == g_pData->GetSkillTree() ||
		 NULL == g_pData->GetUIManager() ||
		 NULL == g_pData->GetUIManager()->GetUISkillTree() )
	{
		return;
	}

	CX2SkillTree*		pSkillTree			= g_pData->GetSkillTree();

	CX2UserSkillTree&	refUserSkillTree	= g_pData->GetMyUser()->GetSelectUnit()->AccessUnitData().m_UserSkillTree;


	/// 현재 스킬창에서 스킬 레벨을 올리고 있는가
	const bool			bLearnMode			= g_pData->GetUIManager()->GetUISkillTree()->GetNowLearnSkill();

	/// 현재 스킬창에서 스킬 레벨을 초기화 하고 있는가
	const bool			bInitSkill			= g_pData->GetUIManager()->GetUISkillTree()->GetNowInitSkill();


	/// 현재 슬롯 아이콘의 레벨 설정 --------------------------------------------------------------------------------------------------------------------------------
	int iSkillLevel = g_pData->GetUIManager()->GetUISkillTree()->GetSkillLevelInSkillWimdow( m_eSkillID );

	/// 스킬 템플릿 객체들 ------------------------------------------------------------------------------------------------------------------------------------------
		/// 스킬 템플릿 ( 특정 레벨의 정보만 있는 스킬 템플릿 )
	const CX2SkillTree::SkillTemplet*		pSkillTemplet		= g_pData->GetSkillTree()->GetSkillTemplet( m_eSkillID );

		/// 스킬 트리 템플릿
	int										iUnitClass			= static_cast<int>( g_pData->GetMyUser()->GetSelectUnit()->GetClass() );
	const CX2SkillTree::SkillTreeTemplet*	pSkillTreeTemplet	= g_pData->GetSkillTree()->GetSkillTreeTemplet( iUnitClass, m_eSkillID );

		/// 스킬 트리 다이얼로그
	CKTDGUIDialogType						pDLGUISkillTree		= g_pData->GetUIManager()->GetUISkillTree()->GetDLGUISkillTree();

	if( NULL == pSkillTree || NULL == pSkillTemplet || NULL == pSkillTreeTemplet || NULL == pDLGUISkillTree )
		return;

	/// 스킬 아이콘 관련 컨트롤 저장 객체 ---------------------------------------------------------------------------------------------------------------------------
		/// 스킬 아이콘 관련 컨트롤들을 저장하는 객체
	const map<CX2SkillTree::SKILL_ID, CX2UISkillTreeNew::SkillSlotUI>& refmapSkillSlotUISet = g_pData->GetUIManager()->GetUISkillTree()->GetMapSkillSlotUISet();

		/// 스킬 아이디에 대한 스킬 아이콘 컨트롤 객체 검색
	map<CX2SkillTree::SKILL_ID, CX2UISkillTreeNew::SkillSlotUI>::const_iterator it = refmapSkillSlotUISet.find(m_eSkillID);

	if(it == refmapSkillSlotUISet.end())
	{
		return;
	}

	/// 스킬 아이콘 컨트롤 객체 설정
	const CX2UISkillTreeNew::SkillSlotUI& refSkillSlotUI = it->second;

	/// 남은 스킬 포인트 반환 ---------------------------------------------------------------------------------------------------------------------------------------
		/// 남은 스킬 포인트 ( 스킬 습득 및 강화 상태이면 스킬 포인트 사용량을 저장중인 변수를 반환 )
	const int iNoUseSP		= true == bLearnMode ? g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_iSPoint - pSkillTree->GetUsedSPoint() 
												 : g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_iSPoint;
		/// 남은 캐시 스킬 포인트 ( 스킬 습득 및 강화 상태이면 캐시 스킬 포인트 사용량을 저장중인 변수를 반환 )
	const int iNoUseCP		= true == bLearnMode ? g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_iCSPoint - pSkillTree->GetUsedCSPoint()
												 : g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_iCSPoint;


	/// 현재 유저 레벨 ----------------------------------------------------------------------------------------------------------------------------------------------
	const int iUnitLevel	= g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_Level;	/// 현재 유저 레벨

	/// 각종 컨트롤 유효 검사 ---------------------------------------------------------------------------------------------------------------------------------------
	CKTDGUIStatic*	pStaticSkillLock	= refSkillSlotUI.m_pStaticSkillLock;			/// 잠김 스킬
	CKTDGUIStatic*	pStaticRequireSP	= refSkillSlotUI.m_pStaticStringRequireSP;		/// 스킬 요구 SP
	CKTDGUIButton*	pButtonSkillLearn	= refSkillSlotUI.m_pButtonSkillLearn;			/// 스킬 습득 버튼
	CKTDGUIButton*	pButtonSkillDesc	= refSkillSlotUI.m_pButtonSkillDesc;			/// 스킬 툴팁용 버튼
	CKTDGUIButton*	pButtonSkillInit	= refSkillSlotUI.m_pButtonSkillInit;			/// 스킬 초기화 버튼
	CKTDGUIStatic*	pPictureSkillMaster	= refSkillSlotUI.m_pStaticSkillMaster;			/// 마스터 스킬 처리 UI
	CKTDGUIStatic*	pStaticSkillNoLearn = refSkillSlotUI.m_pStaticSkillNoLearn;			/// 스킬 습득 불가 처리 UI
	CKTDGUIStatic*	pStaticSkillBG		= refSkillSlotUI.m_pStaticSkillBG;				/// 스킬 슬롯 배경

	if ( NULL == pStaticSkillLock ||
		 NULL == pStaticRequireSP ||
		 NULL == pButtonSkillLearn ||
		 NULL == pButtonSkillDesc ||
		 NULL == pButtonSkillInit ||
		 NULL == pPictureSkillMaster || 
		 NULL == pStaticSkillNoLearn ||
		 NULL == pStaticSkillBG )
		return;

#ifdef SKILL_PAGE_SYSTEM //JHKang
	USHORT usUsingPage = refUserSkillTree.GetUsingPage();
	const USHORT usSelectPage = refUserSkillTree.GetSelectPage();
	bool bUsingPage = true;

	if ( usUsingPage != usSelectPage - 1 )
		bUsingPage = false;

	if ( usSelectPage > 0 )
		usUsingPage = usSelectPage - 1;
#endif //SKILL_PAGE_SYSTEM

#ifdef ADDED_RELATIONSHIP_SYSTEM
	// 인연 스킬에 대한 예외 처리
	if ( pSkillTemplet->m_eType == CX2SkillTree::ST_RELATIONSHIP_SKILL )
	{
		int iRelationshipLevel = 0; // 현재 인연 단계, 0 = 솔로, 1 = 커플, 2 = 결혼

		/// 초기화 및 스킬 레벨 표기 -----------------------------------------------------------------------------------
		if ( NULL != g_pData->GetRelationshipManager() &&
			NULL != g_pData->GetRelationshipManager()->GetMyRelationshipInfo() )
		{
			switch ( g_pData->GetRelationshipManager()->GetMyRelationshipInfo()->m_eRelationStateType )
			{
			case SEnum::RT_SOLO :
				{
					iRelationshipLevel = 0;
					// 장착중인 인연 스킬 장착 해제
					if( NULL != g_pData && NULL != g_pData->GetSkillTree() )
					{
						g_pData->GetSkillTree()->UnequipRelationSkill();
					}
				}
				break;
			case SEnum::RT_COUPLE :
				iRelationshipLevel = 1;
				break;

			case SEnum::RT_MARRIED :
				iRelationshipLevel = 2;
				break;
			}

		}

		// 텍스처 설정 -------------------------------------------------------------------------------------------------
		pSlot->SetItemTex( pSkillTemplet->m_wstrIconName.c_str(), pSkillTemplet->m_wstrIconPieceName.c_str() );

		/// 결혼 스킬 설정 ---------------------------------------------------------------------------------------------
#ifdef SKILL_PAGE_SYSTEM //JHKang
		refUserSkillTree.SetSkillLevelAndCSP( m_eSkillID, iRelationshipLevel, 0, usUsingPage );
#else //SKILL_PAGE_SYSTEM
		refUserSkillTree.SetSkillLevelAndCSP( m_eSkillID, iRelationshipLevel, 0 );
#endif //SKILL_PAGE_SYSTEM

		// if(true == refUserSkillTree.DoIHaveThisSkill(m_eSkillID)) // 이 스킬을 갖고 있다면..
		// 모든 캐릭터는 인연 스킬을 갖고 있어야함
		{
			// 커플 이상 단계일 때는 "Lv. 1" Static 과 Drag 를 막는다.
			// RButtonUp 으로 스킬 넣는 부분은 해당 메시지에서 다시 코딩
			// 그냥 상태에서는 Drag 불가
#ifdef SKILL_PAGE_SYSTEM //JHKang
			if( iRelationshipLevel > 0 && bUsingPage == true )
#else //SKILL_PAGE_SYSTEM
			if( iRelationshipLevel > 0 )
#endif //SKILL_PAGE_SYSTEM
				pSlot->SetDragable( true );
			else
				pSlot->SetDragable( false );
		}

		/// 스킬 아이콘 업데이트 ---------------------------------------------------------------------------------------
		/// 획득 가능, 은 무조건 안보여줌	
		/// 사용 불가, 는 상황에 맞춰 보여줌
		ShowSlotPicture( false, pSlot, STSAPT_CANLEARN ); 

#ifdef SKILL_PAGE_SYSTEM //JHKang
		if ( iRelationshipLevel > 0 && bUsingPage == true )
#else //SKILL_PAGE_SYSTEM
		if ( iRelationshipLevel > 0 )
#endif //SKILL_PAGE_SYSTEM
		{
			ShowSlotPicture( false, pSlot, STSAPT_DISABLE );
		}
		else
		{
			ShowSlotPicture( true, pSlot, STSAPT_DISABLE );
		}

		//if( -1 != refUserSkillTree.GetSkillEquippedSlotIndex( m_eSkillID, false ) || 
		//	-1 != refUserSkillTree.GetSkillEquippedSlotIndex( m_eSkillID, true ) )
		//{
		//	// 스킬을 장착 중이면
		//	ShowSlotPicture( true, pSlot, STSAPT_EQUIPPED );
		//}
		//else
		//{
		//	ShowSlotPicture( false, pSlot, STSAPT_EQUIPPED );
		//}


		/// 요구 레벨 및 마스터 레벨 업데이트 --------------------------------------------------------------------------
		if ( NULL != pStaticRequireSP->GetString( 0 ) && 
			 NULL != pPictureSkillMaster->GetPicture(0) )
		{
			if ( 2 > iRelationshipLevel )	/// 결혼 이전이면, 관련 정보 표시
			{
				pStaticRequireSP->SetShow( true );
				pPictureSkillMaster->GetPicture(0)->SetShow( false );

				/// 요구 SP 대신 필요한 연인 상태 표시 -------------------------------------------------------------------------
				wstring wstrRelationState = L"";

				switch ( iRelationshipLevel ) 
				{
				case 0:
					wstrRelationState = GET_STRING ( STR_ID_24461 );	/// 커플
					break;
				default:
					wstrRelationState = GET_STRING ( STR_ID_24462 );	/// 결혼
					break;
				}

				WCHAR buf[256] = {0,};

				/// 무조건 습득 조건 미달 표시
				StringCchPrintf( buf, 255, L"#CFF8000%s#CX", wstrRelationState.c_str() );
				pStaticRequireSP->GetString( 0 )->msg = buf;
			}
			else		/// 결혼 이상이면, 마스터 스킬 처리
			{
				pStaticRequireSP->SetShow( false );
				pPictureSkillMaster->GetPicture(0)->SetShow( true );	/// 마스터 레벨 설정
			}
			
		}

		/// 버튼 업데이트 ----------------------------------------------------------------------------------------------
		pButtonSkillLearn->SetEnableClick( false );		/// 스킬 포인트로 배울 수 있는 스킬이 아니므로, 무조건 비활성
		pButtonSkillLearn->SetEnable( false );
		pButtonSkillLearn->SetShow( 2 > iRelationshipLevel ? true : false );		/// 결혼 중이라면, 버튼 숨김

		/// 자물쇠 업데이트 --------------------------------------------------------------------------------------------
		if ( iRelationshipLevel > 0 )
			pStaticSkillLock->SetShow( false );

		else
			pStaticSkillLock->SetShow( true );
	}

	else
#endif // ADDED_RELATIONSHIP_SYSTEM
	{
		/// 스킬 조건 검사 ---------------------------------------------------------------------------------------------
		bool bIsLockSkill			= false;	/// 봉인스킬 잠김 여부
		bool bMasterLevel			= false;	/// 마스터 스킬 레벨 여부
		bool bCanLearnLevel			= false;	/// 스킬을 배울 수 있는 레벨 여부
		bool bHaveLearnSP			= false;	/// 스킬을 배울 수 있는 SP 소지 여부
		bool bLearnPrecedingSkill	= true;		/// 선행 스킬 습득 여부 ( 선행 스킬이 없다면, true )

		const int iMaxLearnLevel = pSkillTreeTemplet->m_iMaxSkillLevel;	/// 최대로 찍을 수 있는 스킬 레벨

			/// 봉인스킬이며, 풀리지 않았을 때
		if( true == pSkillTemplet->m_bBornSealed && false == refUserSkillTree.IsSkillUnsealed( m_eSkillID ) )
		{
			bIsLockSkill = true;		/// 봉인 스킬 잠김

			/// 봉인 스킬 아이콘은 무조건 상위로 설			pDLGUISkillTree->ChangeSequence( refSkillSlotUI.m_pStaticSkillLock, true );
		}

		refSkillSlotUI.m_pStaticSkillLock->SetShow( bIsLockSkill );

			/// 마스터 레벨의 스킬인지 검사
		if ( iMaxLearnLevel <= iSkillLevel )		/// 스킬을 더이상 찍을 수 없다면, 마스터 레벨 UI 처리
			bMasterLevel = true;

			/// 다음 레벨의 스킬을 습득할 수 있는지 검사
		if ( pSkillTemplet->GetSkillRequireLevelValue( iSkillLevel + 1 ) <= iUnitLevel )
			bCanLearnLevel = true;

			/// 선행 스킬 습득 여부 검사
		if ( 0 < pSkillTreeTemplet->m_iPrecedingSkill )
		{
			/// 선행 스킬의 레벨
			int iPrecedingSkillLevel = g_pData->GetUIManager()->GetUISkillTree()->GetSkillLevelInSkillWimdow
				( static_cast<CX2SkillTree::SKILL_ID>( pSkillTreeTemplet->m_iPrecedingSkill ) );

			if ( 0 >= iPrecedingSkillLevel )
				bLearnPrecedingSkill = false;	/// 선행 스킬이 0 이하이면, 미습득 처리
		}


		/// 슬롯 업데이트 ----------------------------------------------------------------------------------------------
		pSlot->SetItemTex( pSkillTemplet->m_wstrIconName.c_str(), pSkillTemplet->m_wstrIconPieceName.c_str() );

		if(true == refUserSkillTree.DoIHaveThisSkill(m_eSkillID))
		{
			switch(pSkillTemplet->m_eType)
			{
			case CX2SkillTree::ST_ACTIVE:
			case CX2SkillTree::ST_BUFF:
			case CX2SkillTree::ST_SPECIAL_ACTIVE:
#ifdef FINALITY_SKILL_SYSTEM //JHKang
			case CX2SkillTree::ST_HYPER_ACTIVE_SKILL:
#endif //FINALITY_SKILL_SYSTEM
				{
					/// 블라인드 처리된 스킬이거나 스킬 레벨을 올리는 중이라면, 드래그 할 수 없다.
				#ifdef SKILL_PAGE_SYSTEM //JHKang
					if ( true == refSkillSlotUI.m_bSelectSkillBlind || true == bLearnMode || true == bIsLockSkill || bUsingPage == false )
				#else //SKILL_PAGE_SYSTEM
					if ( true == refSkillSlotUI.m_bSelectSkillBlind || true == bLearnMode || true == bIsLockSkill )
				#endif //SKILL_PAGE_SYSTEM
						pSlot->SetDragable(false);
					else
						pSlot->SetDragable(true);
				} break;
			default:
				{
					pSlot->SetDragable(false);
				} break;
			}
		}
		else
		{
			ShowSlotPicture( false, pSlot, STSAPT_LEVEL );
			pSlot->SetDragable(false);
		}

			/// 스킬 레벨  표시
		if ( 0 < iSkillLevel )
			ShowSlotPicture( true, pSlot, STSAPT_LEVEL, iSkillLevel );

#ifdef SKILL_PAGE_SYSTEM //JHKang
		if ( 0 == iSkillLevel && bUsingPage == false )
			ShowSlotPicture( false, pSlot, STSAPT_LEVEL, iSkillLevel );
#endif //SKILL_PAGE_SYSTEM


			
		/// 슬롯 상태 업데이트 -----------------------------------------------------------------------------------------
#ifdef SKILL_PAGE_SYSTEM //JHKang
		if( false == refUserSkillTree.DoIHaveThisSkill(m_eSkillID) || true == bIsLockSkill || bUsingPage == false )	// 배우지 않은 스킬이거나 잠긴 스킬은 아이콘 어둡게 처리
#else //SKILL_PAGE_SYSTEM
		if( false == refUserSkillTree.DoIHaveThisSkill(m_eSkillID) || true == bIsLockSkill )		/// 배우지 않은 스킬이거나 잠긴 스킬은 아이콘 어둡게 처리
#endif //SKILL_PAGE_SYSTEM
			ShowSlotPicture( true, pSlot, STSAPT_DISABLE );
		else
			ShowSlotPicture( false, pSlot, STSAPT_DISABLE );

		// 스킬 장착중 표시
		/*if( -1 != refUserSkillTree.GetSkillEquippedSlotIndex( m_eSkillID, false ) || 
			-1 != refUserSkillTree.GetSkillEquippedSlotIndex( m_eSkillID, true ) )
			ShowSlotPicture( true, pSlot, STSAPT_EQUIPPED );
		else
			ShowSlotPicture( false, pSlot, STSAPT_EQUIPPED );*/

		/// 스킬 습득 가능 여부 안내 텍스처 표시
#ifdef SKILL_PAGE_SYSTEM //JHKang
		if ( 0 >= iSkillLevel && false == refSkillSlotUI.m_bSkillLineBlind && bUsingPage == true )	// 배울 수 있는 스킬일 때
#else //SKILL_PAGE_SYSTEM
		if ( 0 >= iSkillLevel && false == refSkillSlotUI.m_bSkillLineBlind )			/// 배울 수 있는 스킬일 때
#endif //SKILL_PAGE_SYSTEM
		{
			const int	iIndexInTier			= pSkillTreeTemplet->m_iIndexInTier;	/// 스킬 라인 가로 인덱스
			bool		bIsSelectAnotherSkill	= false;								/// 반대편 스킬 선택 가능 여부

			/// 2지 선다 스킬일 때, 반대편 스킬 아이디가 존재하거나 선택 가능한 스킬인지 검사
			if ( CX2UISkillTreeNew::STST_SINGLE_SKILL != iIndexInTier )
			{
				const CX2SkillTree::SKILL_ID eAnotherSkillID = g_pData->GetUIManager()->GetUISkillTree()->GetAnotherSelectSkillID( m_eSkillID );

				/// 반대편 스킬 아이디가 선택 가능한 스킬인지 검사
				if ( CX2SkillTree::SI_NONE != eAnotherSkillID )
				{
					const CX2SkillTree::SkillTemplet* pSkillTemplet	= g_pData->GetSkillTree()->GetSkillTemplet( eAnotherSkillID );

					/// 봉인 스킬이 아니거나, 해제된 봉인스킬이면, 선택 가능한 반대편 스킬로 설정
					if ( NULL != pSkillTemplet && 
							false == pSkillTemplet->m_bBornSealed || 
							( true == pSkillTemplet->m_bBornSealed && true == refUserSkillTree.IsSkillUnsealed( eAnotherSkillID ) ) )
					{
						bIsSelectAnotherSkill = true;
					}
				}
			}


			if ( CX2UISkillTreeNew::STST_SINGLE_SKILL != iIndexInTier &&	/// 2지선다 스킬이며,
			 	 false == refSkillSlotUI.m_bSelectSkillBlind &&				/// 선택을 받지 못한 스킬이 아니며,
				 false == bIsLockSkill &&									/// 잠긴 스킬이 아니며,
				 true  == bIsSelectAnotherSkill )							/// 반대편 스킬이 선택 가능한 스킬일 때
			{
				if ( CX2UISkillTreeNew::STST_SELECT_SKILL_LEFT == iIndexInTier )	/// 왼쪽 스킬이면, 선택 가능 표시
				{
					ShowSlotPicture( true, pSlot, STSAPT_SELECT );

					pDLGUISkillTree->ChangeSequence( pSlot, true );		/// 2지선다 선택 가능 최상위로 설정
				}
				else	/// 오른쪽 스킬이면, 선택 가능 해제 ( 왼쪽에서 출력할 것이다. )
					ShowSlotPicture( false, pSlot, STSAPT_SELECT );

				ShowSlotPicture( false, pSlot, STSAPT_CANLEARN );		/// 획득 가능 해제
			}
			else		/// 하나의 스킬만 습득 가능 상태
			{
				ShowSlotPicture( false, pSlot, STSAPT_SELECT );			/// 2지선다 선택 가능 해제

				if ( false == bIsLockSkill )
					ShowSlotPicture( true, pSlot, STSAPT_CANLEARN );	/// 획득 가능 설정
			}
		}
		else			/// 배울 수 없는 스킬일 때
		{
			ShowSlotPicture( false, pSlot, STSAPT_SELECT );				/// 2지선다 선택 가능 해제
			ShowSlotPicture( false, pSlot, STSAPT_CANLEARN );			/// 획득 가능 해제
		}



		/// 정보 표시용 스트링 설정 ------------------------------------------------------------------------------------
		if ( NULL != pStaticRequireSP->GetString( 0 ) )
		{
			
			/// 마스터 레벨이 아닐 때 값 설정
			if ( false == bMasterLevel && false == bInitSkill )
			{
				/// 요구 SP 업데이트 -------------------------------------------------------------------------------------------
				int iOutputREquireSPValue = 0 == iSkillLevel ? pSkillTemplet->m_iRequireLearnSkillPoint : 
																pSkillTemplet->m_iRequireUpgradeSkillPoint;		/// 스킬 강화시 요구되는 SP 양

				wstring wstrOutputMsg = L"";	/// 출력시킬 스트링

				/// 스킬을 배우는데 요구되는 포인트를 갖고있을 때, 파란 색( 기본 )으로 표기
				if ( iOutputREquireSPValue <= iNoUseSP + iNoUseCP )
				{
					wstrOutputMsg	+= GET_REPLACED_STRING( ( STR_ID_25073, "i", iOutputREquireSPValue ) );
					bHaveLearnSP	=  true;
				}
				/// 스킬을 배우는데 요구되는 포인트를 갖고있지 않을 때, 붉은 색으로 표기
				else
				{
					wstrOutputMsg	=  L"#CFF8000";
					wstrOutputMsg	+= GET_REPLACED_STRING( ( STR_ID_25073, "i", iOutputREquireSPValue ) );
					wstrOutputMsg	+=  L"#CX";
				}

				/// 출력
				pStaticRequireSP->GetString( 0 )->msg = wstrOutputMsg;

				pStaticRequireSP->SetShow( true );
			}
			else
			{
				pStaticRequireSP->SetShow( false );
			}
		}
			


		/// 마스터 스킬 UI 설정 ----------------------------------------------------------------------------------------
		if ( NULL != pPictureSkillMaster->GetPicture(0) )
		{
			if ( true == bInitSkill )
				pPictureSkillMaster->GetPicture(0)->SetShow( false );		 /// 스킬 초기화 모드라면, 표시 해제
			else
				pPictureSkillMaster->GetPicture(0)->SetShow( bMasterLevel ); /// 마스터 레벨이면 활성
		}

			/// 습득 버튼은 초기화 상태이면 무조건 표시, 그 외엔 마스터 레벨이 아닐 때만 표시
		pButtonSkillLearn->SetShow( true == bInitSkill ? false : !bMasterLevel );

			/// 초기화 버튼은 초기화시 무조건 표시
		pButtonSkillInit->SetShow( bInitSkill );

			/// 초기화 버튼 동작시, SP 표시 백판 숨김
		if ( NULL != pStaticSkillBG->GetPicture( 1 ) )
			pStaticSkillBG->GetPicture( 1 )->SetShow( !bInitSkill );

			/// 초기화 상태이면, 무조건 마스터 레벨 이미지 미표시, 그 외엔 마스터 레벨일 때만 표시
		if ( NULL != pPictureSkillMaster->GetPicture( 0 ) )
			pPictureSkillMaster->GetPicture( 0 )->SetShow( true == bInitSkill ? false : bMasterLevel );



		/// 버튼 처리 --------------------------------------------------------------------------------------------------
		if ( true == bInitSkill )		/// 스킬 초기화 상태일 때
		{
			pButtonSkillDesc->SetEnable( false );	/// 툴팁용 버튼 해제

			const bool bIsDefaultSkill = g_pData->GetSkillTree()->isDefaultSkill( m_eSkillID );		/// 기본 스킬 여부

			/// 메시지 및 텍스처 설정
#ifdef SKILL_PAGE_SYSTEM //JHKang
			if ( ( ( false == bIsDefaultSkill && 0 < iSkillLevel ) || ( true == bIsDefaultSkill && 1 < iSkillLevel ) ) && bUsingPage == true )	// 기본 스킬이 아니면서 0레벨 보다 크거나, 기본 스킬이면서 1레벨 보다 클 때
#else //SKILL_PAGE_SYSTEM
			if ( ( false == bIsDefaultSkill && 0 < iSkillLevel ) || 
				( true == bIsDefaultSkill && 1 < iSkillLevel ) )		/// 기본 스킬이 아니면서 0레벨 보다 크거나, 기본 스킬이면서 1레벨 보다 클 때
#endif //SKILL_PAGE_SYSTEM
			{
				pButtonSkillInit->SetEnableClick( true );
				pButtonSkillInit->SetEnable( true );
			}
			else						/// 배우지 않았으면, 버튼 무조건 비활성
			{
				pButtonSkillInit->SetEnableClick( false );
				pButtonSkillInit->SetEnable( false );
			}
		}
		else
		{
			if ( false == bMasterLevel )
			{
				/// 배울 수 있는 조건일 때, 스킬 습득 버튼 활성
				if ( true  == bCanLearnLevel &&							/// 배울 수 있는 레벨
					 true  == bHaveLearnSP &&							/// 충분한 SP 소지
					 false == bIsLockSkill &&							/// 비봉인 스킬
					 true  == bLearnPrecedingSkill &&					/// 선행 스킬 습득 여부
					 false == refSkillSlotUI.m_bSelectSkillBlind &&		/// 2지선다중 선택받은 스킬
					 false == refSkillSlotUI.m_bSkillLineBlind			/// 스킬 라인이 열려있는 스킬
				#ifdef SKILL_PAGE_SYSTEM //JHKang
					 && true == bUsingPage
				#endif //SKILL_PAGE_SYSTEM
					  )
				{
					pButtonSkillLearn->SetEnable( true );
					pButtonSkillLearn->SetEnableClick( true );
					pButtonSkillDesc->SetEnable( false );	/// 툴팁용 버튼 해제

					if ( 0 >= iSkillLevel )		/// 습득
						pButtonSkillLearn->SetCustomMsgMouseUp( CX2UISkillTreeNew::STUCM_BUTTON_SKILL_LEVEL_PLUS );
					else						/// 강화
						pButtonSkillLearn->SetCustomMsgMouseUp( CX2UISkillTreeNew::STUCM_BUTTON_SKILL_LEVEL_UPGRADE );
				}
				else	/// 레벨이 부족하거나 SP가 모자랄 때, 스킬 습득 버튼 비활성
				{
					pButtonSkillLearn->SetEnable( false );
					pButtonSkillLearn->SetEnableClick( false );

					/// 스킬 라인 설정으로 인하여 가려져 있지 않으면, 툴팁 표시
					if ( false == refSkillSlotUI.m_bSkillLineBlind )
					{
						pButtonSkillDesc->SetEnable( true );	/// 툴팁용 버튼 활성

						/// 비활성된 조건에 따른 툴팁 설정
						wstring wstrNotLearnSkillDesc = L"";

						if ( false == bCanLearnLevel )	/// 레벨 부족
							wstrNotLearnSkillDesc += GET_REPLACED_STRING( ( STR_ID_25111, "i", pSkillTemplet->GetSkillRequireLevelValue( iSkillLevel + 1 ) ) );

						if ( false == bHaveLearnSP )	/// 스킬 포인트가 부족
						{
							if ( wstrNotLearnSkillDesc != L"" )		/// 빈칸이 아니면, 한줄 띄어쓰기
								wstrNotLearnSkillDesc += L"\n";

							wstrNotLearnSkillDesc += GET_STRING( STR_ID_25107 );
						}

						pButtonSkillDesc->SetGuideDesc( wstrNotLearnSkillDesc.c_str() );
					}
					else		/// 스킬 라인 설정으로 인하여 가려졌으면, 툴팁 미표기
						pButtonSkillDesc->SetEnable( false );	/// 툴팁용 버튼 비활성
				}
			}
			else	/// 마스터 레벨 상태이면, 클릭 해제
			{
				pButtonSkillLearn->SetEnableClick( false );
			}
		}



		/// 2지선다 스킬 선택 불능 UI 설정 -----------------------------------------------------------------------------
		if ( NULL != pStaticSkillNoLearn->GetPicture( 0 ) )
		{
			pStaticSkillNoLearn->GetPicture( 0 )->SetShow( refSkillSlotUI.m_bSelectSkillBlind );

#ifdef SKILL_PAGE_SYSTEM //JHKang
			if ( true == refSkillSlotUI.m_bSelectSkillBlind )
			{
				pDLGUISkillTree->ChangeSequence( pStaticSkillNoLearn, true );
				CKTDGUIStatic* pStaticBlind = reinterpret_cast<CKTDGUIStatic*>( pDLGUISkillTree->GetControl( L"g_pStaticBlind" ) );
				
				if ( pStaticBlind != NULL )
					pDLGUISkillTree->ChangeSequence( pStaticBlind, true );
			}
#else //SKILL_PAGE_SYSTEM
			if ( true == refSkillSlotUI.m_bSelectSkillBlind )
				pDLGUISkillTree->ChangeSequence( pStaticSkillNoLearn, true );
#endif //SKILL_PAGE_SYSTEM					
		}
	}
}

void CX2SkillTreeSlotData::ShowSlotPicture(bool bShow, CKTDGUISlot* pSlot, SKILL_TREE_SLOT_ADD_PICTURE_TYPE eType, int val /*= -1*/ )
{
	if(pSlot == NULL)
		return;

	CKTDGUIControl::CPictureData* pPicture = pSlot->GetPicture((int)eType);
	if(pPicture == NULL )
	{
		if(bShow)
		{
			pPicture = new CKTDGUIControl::CPictureData();
			CKTDGUIControl::UIPointData* pPoint = new CKTDGUIControl::UIPointData(pSlot->GetNowPoint());
			pPicture->SetPoint( pPoint );

			switch(eType)
			{
			case STSAPT_DISABLE:
				{
					pPicture->SetTex( L"HQ_ImpossibleEqip.tga" );			
					pPicture->pPoint->color = D3DXCOLOR( 1,1,1,0.6f );
				} break;
			case STSAPT_SEALED:
				{
					pPicture->SetTex( L"DLG_UI_Common_Texture02.TGA", L"lock" );
					pPicture->pPoint->color = D3DXCOLOR( 1,1,1,1 );
				} break;
			case STSAPT_LEVEL:
				{
					// 레벨 텍스쳐. 리소스 나오면...
					wstring wstrLevelTextureName = g_pData->GetSkillTree()->GetSkillLevelTextureName( val );
					wstring wstrLevelTextureKey = g_pData->GetSkillTree()->GetSkillLevelTextureKey( val );
					if( false == pPicture->SetTex( wstrLevelTextureName.c_str(), wstrLevelTextureKey.c_str() ) )
					{
						pPicture->SetTex( L"NoAlphaImage.dds" );
					}
					pPicture->pPoint->color = D3DXCOLOR( 1,1,1,1 );

					/// 레벨 텍스처 위치 조정
					D3DXVECTOR2 vecPos					= pPicture->GetPos();
					pPicture->SetPos( D3DXVECTOR2( vecPos.x, vecPos.y + 33.f ) );

					D3DXVECTOR2 vecPicturePos			= pPicture->pPoint->leftTopPoint;
					pPicture->pPoint->leftBottomPoint	= D3DXVECTOR2( vecPicturePos.x, vecPicturePos.y + 14.f );
					pPicture->pPoint->rightTopPoint		= D3DXVECTOR2( vecPicturePos.x + 25.f, vecPicturePos.y );
					pPicture->pPoint->rightBottomPoint	= D3DXVECTOR2( vecPicturePos.x + 25.f, vecPicturePos.y + 14.f );
				} break;
			case STSAPT_EQUIPPED:
				{
					pPicture->SetTex( L"DLG_Common_New_Texture26.tga", L"Skill_Mount" );
					pPicture->SetFlicker( 2.0f, 1.4f, 0.2f );

				} break;
			case STSAPT_CANLEARN:
				{
					pPicture->SetTex( L"DLG_Common_New_Texture26.tga", L"acquire_Skill" );
					pPicture->SetFlicker( 2.0f, 1.4f, 0.2f );

				} break;
			case STSAPT_SELECT:
				{
					pPicture->SetTex( L"DLG_UI_Common_Texture_NewSkill_01.TGA", L"AbleChoice" );
					pPicture->SetFlicker( 2.0f, 1.4f, 0.2f );

					D3DXVECTOR2 vecPos = pPicture->GetPos();
#if defined(CLIENT_COUNTRY_ID)
					pPicture->SetPos( D3DXVECTOR2( vecPos.x + 44.0f, vecPos.y ) );
#elif defined(CLIENT_COUNTRY_TH) 
					pPicture->SetPos( D3DXVECTOR2( vecPos.x + 18.0f, vecPos.y ) );
#else
					pPicture->SetPos( D3DXVECTOR2( vecPos.x + 20.f, vecPos.y ) );
#endif 
					pPicture->SetSizeX( 90.f );
				} break;
			default:
				{

				} break;
			}

			pSlot->AddPicture( (int)eType, pPicture );
			pPicture->SetShow(bShow);
		}		
	}
	else
	{
		if(bShow)
		{
			switch(eType)
			{
			case STSAPT_LEVEL:
				{
					wstring wstrLevelTextureName = g_pData->GetSkillTree()->GetSkillLevelTextureName( val );
					wstring wstrLevelTextureKey = g_pData->GetSkillTree()->GetSkillLevelTextureKey( val );
					if( false == pPicture->SetTex( wstrLevelTextureName.c_str(), wstrLevelTextureKey.c_str() ) )
					{
						pPicture->SetTex( L"NoAlphaImage.dds" );
					}
					pPicture->pPoint->color = D3DXCOLOR( 1,1,1,1 );

				} break;
			default:
				break;
			}

		}

		pPicture->SetShow(bShow);
	}

}

//////////////////////////////////////////////////////////////////////////
/// CX2EquippedSkillSlot : 내가 장비중인 스킬 슬롯
//////////////////////////////////////////////////////////////////////////
CX2EquippedSkillSlotData::CX2EquippedSkillSlotData()
{
	m_eSlotDataType = SDT_SKILL_EQUIP;
	m_bDataUpdated	= true;
	m_eSkillID		= CX2SkillTree::SI_NONE;
	m_iIndex = -1;
	m_bSlotb = false;
	m_bDataUpdated = true;
}
CX2EquippedSkillSlotData::CX2EquippedSkillSlotData(int index, bool bSlotb )
{
	m_eSlotDataType = SDT_SKILL_EQUIP;
	m_bDataUpdated	= true;
	m_eSkillID		= CX2SkillTree::SI_NONE;
	m_iIndex		= index;
	m_bSlotb		= bSlotb;
	m_bDataUpdated = true;

}
/*virtual*/ CX2EquippedSkillSlotData::~CX2EquippedSkillSlotData()
{

}
/*virtual*/ void CX2EquippedSkillSlotData::UpdateUI( CKTDGUISlot* pSlot )
{
	// 1. 지금 익힐 수 있는 스킬 여부 : 버튼의 활성화/비활성화. bool 값에 따라서
	// 2. 1레벨이라도 익힌 스킬 여부 : 스킬 슬롯의 드래그 활성화 / 비활성화(패시브면 드래그 안 되게), 텍스쳐 변경. 스트링 설정 (레벨/최대레벨)


	const CX2UserSkillTree& refUserSkillTree = g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_UserSkillTree;


	if(m_bSlotb == true && false == refUserSkillTree.GetEnabledSkillSlotB() )
	{
		if(m_eSkillID != CX2SkillTree::SI_NONE)
		{
			m_eSkillID = CX2SkillTree::SI_NONE;
			pSlot->SetItemTex( L"DLG_Common_New_Texture03.TGA", L"invisible" );
			pSlot->SetDragable(false);
			ShowSlotPicture(false, pSlot, SESAPT_LEVEL );
		}
		if(true == pSlot->GetEnable())
		{			
			ShowSlotPicture(true, pSlot, SESAPT_DISABLE );
			pSlot->SetEnable(false);
		}		
		return;
	}
	else
	{
		ShowSlotPicture(false, pSlot, SESAPT_DISABLE );
		pSlot->SetEnable(true);
	}	


	const CX2UserSkillTree::SkillSlotData* pSkillData = refUserSkillTree.GetSkillSlot( m_iIndex, m_bSlotb);
	m_eSkillID = pSkillData->m_eID;

	const CX2SkillTree::SkillTemplet* pSkillTemplet = g_pData->GetSkillTree()->GetSkillTemplet( m_eSkillID );
	if( pSkillTemplet != NULL)
	{
		pSlot->SetItemTex( pSkillTemplet->m_wstrIconName.c_str(), pSkillTemplet->m_wstrIconPieceName.c_str() );		
		pSlot->SetDragable(true);

		int iSkillLevel = refUserSkillTree.GetSkillLevel( m_eSkillID, true );

		if(iSkillLevel <= 0 && m_eSkillID != CX2SkillTree::SI_NONE)
		{
			ASSERT( !"Skill Level 0 Equipped!" );

			/// 스킬 레벨이 0인 스킬을 장착중이라면, 뻬버리자
			CX2EquippedSkillSlotData* pSlotData = (CX2EquippedSkillSlotData*)pSlot->GetSlotData();

			g_pData->GetSkillTree()->Handler_EGS_CHANGE_SKILL_SLOT_REQ( pSlotData->GetSlotID(), CX2SkillTree::SI_NONE );

			return;
		}

		if(pSkillTemplet->m_bShowSkillLevel)
		{
			int ilevelUpNum = 0;

			/// 인연 스킬이 아닐때만 추가 레벨 증가 효과 적용
			if ( pSkillTemplet->m_eType != CX2SkillTree::ST_RELATIONSHIP_SKILL )
				ilevelUpNum = refUserSkillTree.GetIncreaseSkillLevel( m_eSkillID );

			ShowSlotPicture(true, pSlot, SESAPT_LEVEL, min(iSkillLevel + ilevelUpNum, MAX_LIMITED_SKILL_LEVEL), ( ilevelUpNum != 0 ));
		}
		else
			ShowSlotPicture(false, pSlot, SESAPT_LEVEL);

#ifdef FIX_SPECTRO_EL_CRYSTAL_ACTIVE_BUG	/// 분광결정 아이콘 갱신 후, CKTDGUISlot 의 OnFrameMove 타면서 아이콘 초기화 되버리는 문제 수정
		if( NULL != g_pX2Game && CX2SkillTree::SI_A_EEL_SPECTRO_EL_CRYSTAL == pSkillTemplet->m_eID )	/// 분광결정 스킬일 때, 아이콘 갱신시켜주자
		{
			g_pX2Game->UpdateEveElectraSkillSlotIcon();
		}
#endif FIX_SPECTRO_EL_CRYSTAL_ACTIVE_BUG
	}
	else
	{
		if ( CX2SkillTree::SI_NONE != m_eSkillID )		/// 스킬 템플릿에 없으며, 스킬 아이디가 0도 아닐 때
		{
			/// 이상한 스킬을 장착하고 있어요!!
			ASSERT( !L"Equipped Unknown SKill!!!" );

			/// 스킬 템플릿에 없는 아이디의 스킬을 장착중이라면, 뻬버리자
			CX2EquippedSkillSlotData* pSlotData = (CX2EquippedSkillSlotData*)pSlot->GetSlotData();

			g_pData->GetSkillTree()->Handler_EGS_CHANGE_SKILL_SLOT_REQ( pSlotData->GetSlotID(), CX2SkillTree::SI_NONE );

			return;
		}
		else
		{
			pSlot->SetItemTex( L"DLG_Common_New_Texture03.TGA", L"invisible" );
			ShowSlotPicture(false, pSlot, SESAPT_LEVEL);
			pSlot->SetDragable(false);
		}
	}
}

void CX2EquippedSkillSlotData::ShowSlotPicture(bool bShow, CKTDGUISlot* pSlot, SKILL_EQUIPPED_SLOT_ADD_PICTURE_TYPE eType, int val /*= -1*/ , bool bIsSkillLevelUpByItem/* = false*/)
{
	if(pSlot == NULL)
		return;

	CKTDGUIControl::CPictureData* pPicture = pSlot->GetPicture((int)eType);

	if(pPicture == NULL )
	{
		if(bShow)
		{
			pPicture = new CKTDGUIControl::CPictureData();
			CKTDGUIControl::UIPointData* pPoint = new CKTDGUIControl::UIPointData(pSlot->GetNowPoint());
			pPicture->SetPoint( pPoint );

			switch(eType)
			{
			case SESAPT_DISABLE:
				{
					pPicture->SetTex( L"HQ_ImpossibleEqip.tga" );			
					pPicture->pPoint->color = D3DXCOLOR( 1,1,1,0.3f );
				} break;

			case SESAPT_LEVEL:
				{
					wstring wstrLevelTextureName = g_pData->GetSkillTree()->GetSkillLevelTextureName( val );
					wstring wstrLevelTextureKey = g_pData->GetSkillTree()->GetSkillLevelTextureKey( val );
					if( false == pPicture->SetTex( wstrLevelTextureName.c_str(), wstrLevelTextureKey.c_str() ) )
					{
						pPicture->SetTex( L"NoAlphaImage.dds" );
					}
					pPicture->pPoint->color = D3DXCOLOR( 1,1,1,1 );

					/// 레벨 텍스처 위치 조정
					D3DXVECTOR2 vecPos					= pPicture->GetPos();
					pPicture->SetPos( D3DXVECTOR2( vecPos.x, vecPos.y + 30.f ) );

					D3DXVECTOR2 vecPicturePos			= pPicture->pPoint->leftTopPoint;
					pPicture->pPoint->leftBottomPoint	= D3DXVECTOR2( vecPicturePos.x, vecPicturePos.y + 14.f );
					pPicture->pPoint->rightTopPoint		= D3DXVECTOR2( vecPicturePos.x + 25.f, vecPicturePos.y );
					pPicture->pPoint->rightBottomPoint	= D3DXVECTOR2( vecPicturePos.x + 25.f, vecPicturePos.y + 14.f );

					if( true == bIsSkillLevelUpByItem )
					{
						pPicture->SetColor( D3DXCOLOR(0.2f, 1.0f, 0.2f, 1.f));
					}
					else
					{
						pPicture->SetColor( D3DXCOLOR(1, 1, 1, 1));
					}

				} break;
			default:
				{

				} break;
			}

			pSlot->AddPicture( (int)eType, pPicture );
			pPicture->SetShow(bShow);
		}
	}
	else
	{
		if(bShow)
		{
			switch(eType)
			{
			case SESAPT_LEVEL:
				{
					wstring wstrLevelTextureName = g_pData->GetSkillTree()->GetSkillLevelTextureName( val );
					wstring wstrLevelTextureKey = g_pData->GetSkillTree()->GetSkillLevelTextureKey( val );
					if( false == pPicture->SetTex( wstrLevelTextureName.c_str(), wstrLevelTextureKey.c_str() ) )
					{
						pPicture->SetTex( L"NoAlphaImage.dds" );
					}
					pPicture->pPoint->color = D3DXCOLOR( 1,1,1,1 );


					if( true == bIsSkillLevelUpByItem )
					{
						pPicture->SetColor( D3DXCOLOR(0.2f, 1.0f, 0.2f, 1.f));
					}
					else
					{
						pPicture->SetColor( D3DXCOLOR(1, 1, 1, 1));
					}
				} break;
			default:
				break;
			}

		}
		pPicture->SetShow(bShow);
	}

}

int CX2EquippedSkillSlotData::GetSlotID()		// 0~8로 계산되는 ID를 돌려준다
{
	if(m_bSlotb)
	{
		return m_iIndex + EQUIPPED_SKILL_SLOT_COUNT;
	}
	else
	{
		return m_iIndex;
	}
}


//////////////////////////////////////////////////////////////////////////

CX2UISkillTreeNew::CX2UISkillTreeNew( CKTDXStage* pNowStage ) :
m_bShow(false),
m_bSkillRemoveMode(false),
m_bClassInfoVaild(false),
//m_MovedPosition(0,0),
//m_DLGPosition(0,0),
m_DLGSize(0,0),
m_pDLGUISkillTree(NULL),
m_pDLGSkillToolTip(NULL),
m_pDLGSkillUnsealInform(NULL),
m_pDLGSkillUnsealed( NULL ),
m_pUserSkillTree(NULL),
m_SumDelta(0),
m_iPrevSP(-1),
m_fNaviMoveRange(0.f),
m_pTalkBoxManager(NULL)
#ifdef OPEN_SKILL_GUIDE_FIRST_TIME
, m_pDLGFirstGuide(NULL)
#endif


#ifdef REFORM_UI_SKILLSLOT
, m_bIsExpandSkillSlot( false )
, m_pDLGGameSkillSlot (NULL)
, m_bShowEquipSkillGuide (false)
#endif //REFORM_UI_SKILLSLOT
, m_bSkillSlotSwap( false )
#ifdef SKILL_SLOT_UI_TYPE_B 
, m_bIsSkillUITypeA( false )
#endif //SKILL_SLOT_UI_TYPE_B
, m_iMaxSkillRowNum( 0 )
, m_bNowLearnSkill( false )
, m_bNowInitSkill(false)
, m_bReplacedEsCounter( false )
#ifdef SKILL_PAGE_SYSTEM //JHKang
, m_iConfirmED( 0 )
#endif //SKILL_PAGE_SYSTEM
{
	m_pTalkBoxManager = CX2TalkBoxManager::CreateTalkBoxManger();
	g_pKTDXApp->GetDGManager()->AddObjectChain( m_pTalkBoxManager );
	m_pTalkBoxManager->SetOverUI( true );
	m_bUsingCSP = false;	// 사용하는 곳이 없는듯 한데.....
#ifdef REFORM_UI_SKILLSLOT
	m_pDLGGameSkillSlot	= new CKTDGUIDialog( pNowStage, L"DLG_GAME_SKILL_SLOT_NEW.lua" );
	g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGGameSkillSlot );
	m_pDLGGameSkillSlot->SetShow(false);
	UpdateSkillSlotString();
#endif //REFORM_UI_SKILLSLOT
	m_mapSkillPosition.clear();
}


CX2UISkillTreeNew::~CX2UISkillTreeNew(void)
{
	SAFE_DELETE_DIALOG( m_pDLGUISkillTree );	
	SAFE_DELETE_DIALOG( m_pDLGSkillToolTip );
	SAFE_DELETE_DIALOG( m_pDLGSkillUnsealInform );

	SAFE_DELETE_DIALOG( m_pDLGSkillUnsealed );

#ifdef OPEN_SKILL_GUIDE_FIRST_TIME
	SAFE_DELETE_DIALOG( m_pDLGFirstGuide );
#endif
#ifdef ELSWORD_WAY_OF_SWORD
	SAFE_DELETE_DIALOG( m_pDLGElswordWSSDesc );
#endif ELSWORD_WAY_OF_SWORD

#ifdef REFORM_UI_SKILLSLOT
	SAFE_DELETE_DIALOG( m_pDLGGameSkillSlot );
#endif //REFORM_UI_SKILLSLOT

	SAFE_DELETE_KTDGOBJECT( m_pTalkBoxManager );

#ifdef SKILL_PAGE_SYSTEM //JHKang
	SAFE_DELETE_DIALOG( m_pDLGBuyConfirm );
#endif //SKILL_PAGE_SYSTEM


	m_mapSkillSlotUISet.clear();

}

void CX2UISkillTreeNew::SetLayer( X2_DIALOG_LAYER layer )
{
	if(m_pDLGUISkillTree != NULL) 
	{
		g_pKTDXApp->GetDGManager()->GetDialogManager()->ChangeLayer(m_pDLGUISkillTree, layer);
		g_pKTDXApp->GetDGManager()->GetDialogManager()->ChangeSequence( m_pDLGUISkillTree, false );
	}
}

void CX2UISkillTreeNew::SetPosition( D3DXVECTOR2 vec )
{
	// 슬롯 다이얼로그
	m_pDLGUISkillTree->SetPos(vec);
	//m_MovedPosition = vec;
	
}

HRESULT CX2UISkillTreeNew::OnFrameMove( double fTime, float fElapsedTime )
{	
	return S_OK;
}

bool CX2UISkillTreeNew::MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	// 앞에 모달 다이얼로그가 있으면 메시지를 처리하지 않는다
	if( true == g_pKTDXApp->GetDGManager()->GetDialogManager()->CheckFrontModalDlg( m_pDLGUISkillTree ) )
		return false;

	bool bFlag = false;
	switch( uMsg )
	{
		// 마우스 메시지들에 대해
	case WM_MOUSEMOVE:
	case WM_LBUTTONUP:
	case WM_MBUTTONUP:
	case WM_RBUTTONUP:
	case WM_XBUTTONDOWN:
	case WM_XBUTTONUP:
	case WM_LBUTTONDBLCLK:
	case WM_MBUTTONDBLCLK:
	case WM_RBUTTONDBLCLK:
		//case WM_MOUSEWHEEL:
		{
#ifdef REFORM_UI_SKILLSLOT
			//스킬 드래그시에 알림 보여주도록 설정
			for(map<CX2SkillTree::SKILL_ID, SkillSlotUI>::iterator it =	m_mapSkillSlotUISet.begin(); it != m_mapSkillSlotUISet.end(); ++it)
			{
				SkillSlotUI& refSkillSlotUI = it->second;
				if ( NULL != refSkillSlotUI.m_pSlot && NULL != refSkillSlotUI.m_pSlot->GetSlotData() )
				{
					if( refSkillSlotUI.m_pSlot->GetState() == CKTDGUISlot::SCS_ONDRAG && uMsg != WM_LBUTTONUP) //드래그 중
					{
						ShowSkillDesc(false);
						refSkillSlotUI.m_pSlot->SetGuideDescLT(false);
						m_bShowEquipSkillGuide = true;
						break;
					}	 
					m_bShowEquipSkillGuide = false;
				}
			}
#endif //REFORM_UI_SKILLSLOT
			if ( false == m_pDLGUISkillTree->GetIsMouseOver())
			{
				return false;
			}
			bFlag = true;
		}
		break;
	case WM_LBUTTONDOWN:
	case WM_MBUTTONDOWN:
	case WM_RBUTTONDOWN:
		{
			if ( false == m_pDLGUISkillTree->GetIsMouseOver())
			{
				return false;
			}
			g_pData->GetUIManager()->SetUILayerIndex(CX2UIManager::UI_MENU_SKILL, true);
			bFlag = true;		
		}
		break;
	default:
		break;
	}

	switch( uMsg )
	{
	case WM_MOUSEWHEEL:
		{ 
			if( m_pDLGUISkillTree->GetIsMouseOver() ) 
				OnMouseWheel(hWnd, uMsg, wParam,lParam);
		}break;
	}

	return bFlag;
}

bool CX2UISkillTreeNew::OnMouseWheel(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	CKTDGUIContolList* pControlList = (CKTDGUIContolList*) m_pDLGUISkillTree->GetControl( L"SkillTreeControlList" );
	if( pControlList == NULL )
		return false;
	CKTDGUIScrollBar* pScroolBarY = pControlList->GetYScroolBar();
	if(pScroolBarY == NULL)
		return false;

	short zDelta = (short)HIWORD(wParam);
	m_SumDelta += zDelta;

#if 0
	if(abs(m_SumDelta) < WHEEL_DELTA)
		return false;

	pScroolBarY->Scroll(-m_SumDelta / WHEEL_DELTA);
	m_SumDelta %= WHEEL_DELTA;
#else
	while (abs(m_SumDelta) >= WHEEL_DELTA)
	{
		if(m_SumDelta>0)
		{
			pScroolBarY->Scroll(-1);
			m_SumDelta -= WHEEL_DELTA;
		}
		else
		{	
			pScroolBarY->Scroll(1);
			m_SumDelta += WHEEL_DELTA;
		}	

	}
#endif

	UpdateAtMoveScrollBar();
	return true;
}

bool CX2UISkillTreeNew::UICustomEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	switch(wParam)
	{
	case STUCM_UNSEAL_SKILL_CANCLE:
		{
			if( m_pDLGSkillUnsealInform != NULL )
			{
				g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDLGSkillUnsealInform, NULL, false );
				m_pDLGSkillUnsealInform = NULL;
			}			
		} break;

	case STUCM_UNSEAL_SKILL_BUY:
		{
			//** 스킬 구매 버튼을 눌렀을 때 여기에 작업해 주세요.

		} break;

	case STUCM_CSP_GUIDE_MOUSEIN:
		{	
			CKTDGUIButton* pControl = (CKTDGUIButton*)lParam;
			D3DXVECTOR2 pos = pControl->GetPos() + pControl->GetDialog()->GetPos();
			pos.x += pControl->GetWidth() / 2.f;
			pos.y += pControl->GetHeight();
						
			if( NULL != g_pData->GetSkillTree() &&
				NULL != g_pData->GetMyUser() &&
				NULL != g_pData->GetMyUser()->GetSelectUnit() )
			{
				const CX2UserSkillTree& refUserSkillTree = g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_UserSkillTree;
				int			iSPoint			= g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_iSPoint;	/// 현재 스킬 포인트
				int			iCSPoint		= g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_iCSPoint;	/// 현재 캐시 스킬 포인트
				const int	iUnitLevel		= g_pData->GetSelectUnitLevel();	/// 유닛 레벨
				const int	iInitSkillPoint	= g_pData->GetSkillTree()->GetCalcInitSkillPoint( iUnitLevel );		/// 유닛이 특정 레벨에 가질 수 있는 스킬 포인트량
				int			iUsedSPoint		= 0;	/// 사용한 스킬 포인트
				int			iUsedCSPoint	= 0;	/// 사용한 캐시 스킬 포인트

				refUserSkillTree.CalcUsedSPointAndCSPoint( iUsedSPoint,iUsedCSPoint );	/// 사용한 스킬 포인트 연산
				const int iMaxCSPoint = iSPoint + iCSPoint + iUsedSPoint + iUsedCSPoint - iInitSkillPoint;	/// 그노시스를 통해 올라간 총 SP 연산

				WCHAR wszText[32] = L"";
				StringCchPrintfW( wszText, ARRAY_SIZE(wszText), L"(%dSP)\n", iMaxCSPoint );
				
				wstring wstr = L"";
				wstr += GET_STRING(STR_ID_2689);
				wstr += wszText;
				wstr += GetExpirationDateDesc( g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_wstrCSPointEndDate, g_pData->GetServerCurrentTime() );

				ShowUIDesc(true, wstr, pos, CX2TalkBoxManagerImp::TBT_FROM_UP_LEFT, D3DXCOLOR(1,1,1,1), D3DXCOLOR(0.97f, 0.23f, 0.06f, 1) );			
			}
			
			return true;
		} break;

	case STUCM_CSP_GUIDE_MOUSEOUT:
		{
			ShowUIDesc(false);
			return true;
		} break;

#ifndef REFORM_UI_SKILLSLOT
	case STUCM_SKILLTREESLOTB_GUIDE_MOUSEIN:
		{
			CKTDGUIButton* pControl = (CKTDGUIButton*)lParam;
			D3DXVECTOR2 pos = pControl->GetPos() + pControl->GetDialog()->GetPos();
			pos.x += pControl->GetWidth() / 2.f;
			
			if(g_pData->GetMyUser() != NULL &&
				g_pData->GetMyUser()->GetSelectUnit() != NULL )
			{
				const CX2UserSkillTree& refUserSkillTree = g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_UserSkillTree;
				wstring wstr = L"";
				wstr += GET_STRING(STR_ID_2690);
				wstr += GetExpirationDateDesc( refUserSkillTree.GetSkillSlotBEndDateString(), g_pData->GetServerCurrentTime() );
				ShowUIDesc(true, wstr, pos, CX2TalkBoxManagerImp::TBT_FROM_DOWN_LEFT, D3DXCOLOR(1,1,1,1), D3DXCOLOR(0.97f, 0.23f, 0.06f, 1) );			
			}
						 
			return true;
		} break;

	case STUCM_SKILLTREESLOTB_GUIDE_MOUSEOUT:
		{
			ShowUIDesc(false);
			return true;
		} break;
#endif //REFORM_UI_SKILLSLOT

#ifdef GUIDE_SKILL_SLOTB
	case STUCM_SKILLTREESLOTB_BUY_GUIDE_MOUSEIN:
		{
			CKTDGUIButton* pControl = (CKTDGUIButton*)lParam;
			D3DXVECTOR2 pos = pControl->GetPos() + pControl->GetDialog()->GetPos();
			pos.x += pControl->GetWidth() / 2.f; 
			ShowUIDesc( true, GET_STRING(STR_ID_16097), pos, CX2TalkBoxManagerImp::TBT_FROM_DOWN_LEFT ); 

			return true;
		} break;

	case STUCM_SKILLTREESLOTB_BUY_GUIDE_MOUSEOUT:
		{
			ShowUIDesc(false);
			return true;
		} break; 
#endif GUIDE_SKILL_SLOTB

#ifdef GUIDE_SKILL_LEVEL_MINUS
	case STUCM_BUTTON_SKILL_LEVEL_MINUS_GUIDE_MOUSEOVER:
		{
			CKTDGUIButton* pControl = (CKTDGUIButton*)lParam;
			D3DXVECTOR2 pos = pControl->GetPos() + pControl->GetDialog()->GetPos();
			pos.x += pControl->GetWidth() / 2;
			pos.y += pControl->GetHeight() / 2.f;
			ShowUIDesc( true, GET_STRING(STR_ID_16099), pos );
			return true;
		} break;

	case STUCM_BUTTON_SKILL_LEVEL_MINUS_GUIDE_MOUSEOUT:
		{
			ShowUIDesc(false);
			return true;
		} break; 
#endif GUIDE_SKILL_LEVEL_MINUS

#ifdef OPEN_SKILL_GUIDE_FIRST_TIME
	case STUCM_FIRST_GUIDE_CLOSE:
		{
			if(m_pDLGFirstGuide != NULL)
			{
				g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDLGFirstGuide, NULL, false );
			}
			m_pDLGFirstGuide = NULL;
			return true;
		} break;
#endif OPEN_SKILL_GUIDE_FIRST_TIME

#ifdef ELSWORD_WAY_OF_SWORD
	case STUCM_INGAME_ELSWORD_WSS_MOUSE_OVER:
		{
			if( CX2Unit::UT_ELSWORD == g_pData->GetMyUser()->GetSelectUnit()->GetType()
#ifdef NEW_CHARACTER_EL
				|| CX2Unit::UT_ELESIS == g_pData->GetMyUser()->GetSelectUnit()->GetType()
#endif //NEW_CHARACTER_EL
				)
			{
				CKTDGUIButton* pControl = (CKTDGUIButton*)lParam;
				D3DXVECTOR2 vControlPos = pControl->GetPos();
				DrawElswordWSSDesc( vControlPos );
			}
			return true;
		} break;
	case STUCM_INGAME_ELSWORD_WSS_MOUSE_OUT:
		{
			if( m_pDLGElswordWSSDesc != NULL )
				m_pDLGElswordWSSDesc->SetShow(false);
			return true;
		} break;
#endif ELSWORD_WAY_OF_SWORD

	case STUCM_SKILL_RESET_MODE_CHANGE_MOUSEOVER:
		{
			CKTDGUIButton* pControl = (CKTDGUIButton*)lParam;
			D3DXVECTOR2 pos = pControl->GetPos() + pControl->GetDialog()->GetPos();
			pos.x += pControl->GetWidth() / 2.f;
			ShowUIDesc( true, GET_STRING(STR_ID_2667), pos, CX2TalkBoxManagerImp::TBT_FROM_DOWN_LEFT);

			return true;
		} break;

	case STUCM_SKILL_RESET_MODE_CHANGE_MOUSEOUT:
		{
			ShowUIDesc( false );
			return true;
		} break;

	case STUCM_BUTTON_SKILL_LEVEL_PLUS_MOUSEOVER:
		{
			CKTDGUIButton* pControl = (CKTDGUIButton*)lParam;
			D3DXVECTOR2 pos = pControl->GetPos() + pControl->GetDialog()->GetPos();
			pos.x += pControl->GetWidth() / 2;
			pos.y += pControl->GetHeight() / 2.f;
			ShowUIDesc( true, GET_STRING(STR_ID_2663), pos );

			return true;
		} break;

	case STUCM_BUTTON_SKILL_LEVEL_PLUS_MOUSEOUT:
		{
			ShowUIDesc( false );
			return true;
		} break;

	case STUCM_BUTTON_SKILL_LEVEL_MINUS_MOUSEOVER:
		{
			CKTDGUIButton* pControl = (CKTDGUIButton*)lParam;
			D3DXVECTOR2 pos = pControl->GetPos() + pControl->GetDialog()->GetPos();
			pos.x += pControl->GetWidth() / 2;
			pos.y += pControl->GetHeight() / 2.f;
			ShowUIDesc( true, GET_STRING(STR_ID_2664), pos );

				return true;
		} break;

	case STUCM_BUTTON_SKILL_LEVEL_MINUS_MOUSEOUT:
		{
			ShowUIDesc( false );
			return true;
		} break;

	case STUCM_BUTTON_SKILL_MASTER_MOUSEOVER:
		{
			CKTDGUIButton* pControl = (CKTDGUIButton*)lParam;
			D3DXVECTOR2 pos = pControl->GetPos() + pControl->GetDialog()->GetPos();
			pos.x += pControl->GetWidth() / 2;
			pos.y += pControl->GetHeight() / 2.f;
			ShowUIDesc( true, GET_STRING(STR_ID_2665), pos );

				return true;
		} break;

	case STUCM_BUTTON_SKILL_MASTER_MOUSEOUT:
		{
			ShowUIDesc( false );
			return true;
		} break;

	case STUCM_CONTROLLIST_SCROLL:
		{
			UpdateAtMoveScrollBar();	/// 스크롤 바가 움직일 때 변경되어야 할 정보를 처리하는 함수

			UpdateBlind();			/// 스킬 상황에 따라 처리해야 하는 스킬 슬롯 설정
		
			return true;
		} break;
	case STUCM_SKILLTREESLOT_MOUSEIN:
		{
			CKTDGUISlot* pSlot = (CKTDGUISlot*) lParam;
			CX2SkillTreeSlotData* pSlotData = (CX2SkillTreeSlotData*) pSlot->GetSlotData();
			if(pSlotData == NULL || pSlotData->m_eSkillID == CX2SkillTree::SI_NONE )
			{
				ShowSkillDesc(false);
				return true;
			}

			D3DXVECTOR2 pos = pSlot->GetPos() + pSlot->GetOffsetPos() + pSlot->GetDialog()->GetPos();
			D3DXVECTOR2 size = pSlot->GetSize();

			ShowSkillDesc(true, pSlotData->m_eSkillID, pos, size, false );		
			return true;

		} break;

	case STUCM_EQUIPSLOT_MOUSEIN:
		{
			CKTDGUISlot* pSlot = (CKTDGUISlot*) lParam;
			CX2SkillTreeSlotData* pSlotData = (CX2SkillTreeSlotData*) pSlot->GetSlotData();
			if(pSlotData == NULL || pSlotData->m_eSkillID == CX2SkillTree::SI_NONE )
			{
				D3DXVECTOR2 pos = pSlot->GetPos() + pSlot->GetDialog()->GetPos();
				pos.x = pos.x + pSlot->GetSize().x * 0.5f;
				ShowUIDesc( true, GET_STRING(STR_ID_2666), pos, CX2TalkBoxManagerImp::TBT_FROM_DOWN_LEFT );

				ShowSkillDesc(false);
				return true;
			}

			D3DXVECTOR2 pos = pSlot->GetPos() + pSlot->GetOffsetPos() + pSlot->GetDialog()->GetPos();
			D3DXVECTOR2 size = pSlot->GetSize();
			ShowSkillDesc(true, pSlotData->m_eSkillID, pos, size, true );			
			return true;

		} break;

	case STUCM_SKILLTREESLOT_MOUSEOUT:
	case STUCM_EQUIPSLOT_MOUSEOUT:
		{			
			ShowUIDesc( false );
			ShowSkillDesc(false);
			return true;

		} break;

	case STUCM_SKILLTREESLOT_RMOUSEUP:
		{
		//{{ kimhc // 2010.3.26 // 무한 스킬 버그 수정
		#ifdef	MODIFY_INFINITE_SKILL_BUG
			if ( g_pMain->GetNowState() != NULL)
			{
				CX2State* pX2State = static_cast<CX2State*>( g_pMain->GetNowState() );
				if ( pX2State->CheckIsReady() == true )
				{
					pX2State->ShowOkAndCancelMsgForUnReadyBySkill();
					return true;
				}
			}
		#endif	MODIFY_INFINITE_SKILL_BUG
			//}} kimhc // 2010.3.26 // 무한 스킬 버그 수정			

			return ProcessSkillTreeSlotRMouseUp( lParam );

		} break;

	case STUCM_BUTTON_EXIT:
		{
			if ( NULL != g_pData && NULL != g_pData->GetSkillTree() )
			{
				/// 스킬 초기화 상태가 아니고, 레벨을 올리고자 하는 스킬이 있을 때 선택 팝업 생성
				if ( false == m_bNowInitSkill && false == g_pData->GetSkillTree()->GetMapSkillInfo().empty() )
				{
					g_pMain->KTDGUIOkAndCancelMsgBox( D3DXVECTOR2(-999,-999), GET_STRING( STR_ID_25109 ), 
						STUCM_EXIT_WITH_SKILL_LEVEL_PLUS, g_pMain->GetNowState(), STUCM_EXIT );

					m_bNowLearnSkill = false;
				}
				/// 없다면, 바로 닫기
				else
				{
					m_bNowInitSkill = false;
					m_bNowLearnSkill = false;
					SetShow(false);
				}
			}

			return true;
		} break;

	case STUCM_EXIT:
		{
			CKTDGUIControl* pControl = (CKTDGUIControl*)lParam;

			if ( NULL != pControl )
				g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, pControl->GetDialog(), NULL, false );

			 SetShow(false);

			 return true;
		} break;

	case STUCM_OPEN_COMBO_TREE:
		{
			//g_pData->GetUIManager()->ToggleUI( CX2UIManager::UI_MENU_SKILL );
			SetShow(false);
			g_pData->GetUIManager()->ToggleUI( CX2UIManager::UI_MENU_COMBO_TREE );
			return true;
		} break;

	case STUCM_EQUIPSLOT_DROPPED:
		{
			if ( NULL != g_pX2Game )
			{
				if ( g_pX2Game->GetGameType() == CX2Game::GT_PVP ||
					g_pX2Game->CheckAndWarningBusyStateNow() )
					return true;
			}

			//{{ kimhc // 2010.3.26 // 무한 스킬 버그 수정
#ifdef	MODIFY_INFINITE_SKILL_BUG
			if ( g_pMain->GetNowState() != NULL)
			{
				CX2State* pX2State = static_cast<CX2State*>( g_pMain->GetNowState() );
				if ( pX2State->CheckIsReady() == true )
				{
					pX2State->ShowOkAndCancelMsgForUnReadyBySkill();
					return true;
				}
#ifdef REFORM_UI_SKILLSLOT
				UpdateSkillEquipedSlotUI();
#endif //REFORM_UI_SKILLSLOT
			}
#endif	MODIFY_INFINITE_SKILL_BUG
			//}} kimhc // 2010.3.26 // 무한 스킬 버그 수정

			return ProcessEquipSlotDropped( lParam );

		} break;

	case STUCM_BUTTON_SKILL_LEVEL_PLUS:					/// 스킬 습득 버튼을 눌렀을 때
		{
			CKTDGUIButton* pControl = (CKTDGUIButton*)lParam;
			CX2SkillTree::SKILL_ID eSkillID = static_cast<CX2SkillTree::SKILL_ID>( pControl->GetDummyInt(0) );

			if ( NULL != g_pData && NULL != g_pData->GetSkillTree() )
			{
				const CX2SkillTree::SkillTemplet* pSkillTemplet = g_pData->GetSkillTree()->GetSkillTemplet( eSkillID );	/// 스킬 템플릿

				if ( NULL != pSkillTemplet )
				{
					wstring wstrSkillName	=  L"#CFF0000";
					wstrSkillName			+= pSkillTemplet->m_wstrName;
					wstrSkillName			+= L"#CX";

					/// 메시지 박스 스트링
					const wstring wstrMsgString	= GET_REPLACED_STRING( ( STR_ID_25106, "L", wstrSkillName ) );

					CKTDGUIDialogType m_pDLGMsgBox = g_pMain->KTDGUIOkAndCancelMsgBox( D3DXVECTOR2(-999,-999), wstrMsgString.c_str(), 
																		STUCM_CHANGE_SKILL_LEVEL_PLUS, g_pMain->GetNowState() );

					m_pDLGMsgBox->AddDummyInt( static_cast<int>( eSkillID ) );
				}
			}

			return true;
		} break;

	case STUCM_CHANGE_SKILL_LEVEL_PLUS:					/// 스킬 습득 처리
		{
			CKTDGUIControl* pControl = (CKTDGUIControl*)lParam;

			if ( NULL != pControl && NULL != pControl->GetDialog() )
			{
				g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, pControl->GetDialog(), NULL, false );

				if ( false == m_bNowLearnSkill )
				{
					g_pData->GetSkillTree()->SetUsedSPoint( 0 );
					g_pData->GetSkillTree()->SetUsedCSPoint( 0 );
				}

				m_bNowLearnSkill = true;
				CX2SkillTree::SKILL_ID eCheckSkillID = static_cast<CX2SkillTree::SKILL_ID>( pControl->GetDialog()->GetDummyInt( 0 ) );

				if ( CX2SkillTree::SI_NONE != eCheckSkillID )
					SetLearnSkillInfo( eCheckSkillID );
			}

			return true;
		} break;

	case STUCM_BUTTON_SKILL_LEVEL_UPGRADE:				/// 스킬 강화 버튼을 눌렀을 때
		{
			CKTDGUIButton* pControl = (CKTDGUIButton*)lParam;
			CX2SkillTree::SKILL_ID eSkillID = static_cast<CX2SkillTree::SKILL_ID>( pControl->GetDummyInt(0) );

			if ( false == m_bNowLearnSkill )
			{
				g_pData->GetSkillTree()->SetUsedSPoint( 0 );
				g_pData->GetSkillTree()->SetUsedCSPoint( 0 );

				m_bNowLearnSkill = true;
			}

			if ( CX2SkillTree::SI_NONE != eSkillID )
				SetLearnSkillInfo( eSkillID );
		} break;

	case STUCM_BUTTON_SKILL_LEVEL_PLUS_RESULT:			/// 결정하기 버튼 눌렀을 때
		{

			if ( NULL != g_pData && NULL != g_pData->GetSkillTree() )
			{
				g_pData->GetSkillTree()->Handler_EGS_GET_SKILL_REQ();

				g_pData->GetSkillTree()->SetUsedSPoint( 0 );
				g_pData->GetSkillTree()->SetUsedCSPoint( 0 );
				g_pData->GetSkillTree()->ClearMapSkillInfo();
			}

			m_bNowLearnSkill = false;

			return true;
		}  break;

	case STUCM_BUTTON_SKILL_LEVEL_PLUS_CANCEL:			/// 취소 버튼을 눌렀을 때
		{
			if ( NULL != g_pData && NULL != g_pData->GetSkillTree() )
			{
				g_pData->GetSkillTree()->SetUsedSPoint( 0 );
				g_pData->GetSkillTree()->SetUsedCSPoint( 0 );
				g_pData->GetSkillTree()->ClearMapSkillInfo();
			}

			m_bNowLearnSkill = false;

			UpdateUI( false, true, true );

			return true;
		} break;

	case STUCM_EXIT_WITH_SKILL_LEVEL_PLUS:				/// 닫으면서 스킬 레벨 증가 처리
		{
			CKTDGUIControl* pControl = (CKTDGUIControl*)lParam;

			if ( NULL != pControl )
				g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, pControl->GetDialog(), NULL, false );

			if ( NULL != g_pData && NULL != g_pData->GetSkillTree() )
			{
				g_pData->GetSkillTree()->Handler_EGS_GET_SKILL_REQ();

				g_pData->GetSkillTree()->SetUsedSPoint( 0 );
				g_pData->GetSkillTree()->SetUsedCSPoint( 0 );
				g_pData->GetSkillTree()->ClearMapSkillInfo();
			}
			m_bNowLearnSkill = false;

			SetShow(false);
		} break;

	
	case STUCM_EQUIPSLOT_RMOUSEUP:
		{
	//{{ kimhc // 2010.3.26 // 무한 스킬 버그 수정
	#ifdef	MODIFY_INFINITE_SKILL_BUG
			if ( g_pMain->GetNowState() != NULL)
			{
				CX2State* pX2State = static_cast<CX2State*>( g_pMain->GetNowState() );
				if ( pX2State->CheckIsReady() == true )
				{
					pX2State->ShowOkAndCancelMsgForUnReadyBySkill();
					return true;
				}
			}
	#endif	MODIFY_INFINITE_SKILL_BUG
			//}} kimhc // 2010.3.26 // 무한 스킬 버그 수정

			// 바쁜 상태 체크
			if ( NULL != g_pX2Game && g_pX2Game->CheckAndWarningBusyStateNow() )
				return true;

#ifdef REFORM_UI_SKILLSLOT // 스킬창 닫혀있을 때 만 장착된 스킬 제거 할 수 있도록 수정
			if ( false == m_bShow ) 
			{
				return true;
			}
#endif //REFORM_UI_SKILLSLOT

			return ProcessEquipSlotRMouseUp( lParam );
		} break;

	case STUCM_SKILL_RESET_MODE_CHANGE:
		{
			m_bSkillRemoveMode = !m_bSkillRemoveMode;
			UpdateUI(false, false, true);
 
			return true;
		} break;

	case STUCM_BUTTON_ENABLE_INIT_SKILL:		/// 스킬 트리 내 초기화 버튼
		{
			m_bNowLearnSkill	= false;					/// 스킬 레벨 조정중이었다면, 캔슬
			g_pData->GetSkillTree()->ClearMapSkillInfo();	/// 임시로 찍어놓은 스킬 정보 초기화

			if ( false == m_bNowInitSkill )	/// 초기화 버튼 활성 시킬 때, 활성 조건 검사
			{
				if ( NULL != g_pData && NULL != g_pData->GetMyUser()  )
				{
					CX2Unit* pMyUnit = g_pData->GetMyUser()->GetSelectUnit();

					if( NULL != pMyUnit )
					{
						/// 초기화 아이템을 소지하고 있는가
						if( NULL != pMyUnit->GetInventory().GetItemByTID( RESET_A_SKILL_ITEM_ID ) ||
							NULL != pMyUnit->GetInventory().GetItemByTID( RESET_A_SKILL_EVENT_ITEM_ID ) ||
#ifdef SERV_UNLIMITED_SKILL_RESET_ITEM
							NULL != pMyUnit->GetInventory().GetItemByTID( UNLIMITED_SKILL_RESET_ITEM ) ||
#endif //SERV_UNLIMITED_SKILL_RESET_ITEM
#ifdef SERV_EVENT_RESET_A_SKILL_ITEM
							NULL != pMyUnit->GetInventory().GetItemByTID( RESET_A_SKILL_ITEM ) ||
#endif SERV_EVENT_RESET_A_SKILL_ITEM
#ifdef SERV_QUEST_SKILL_RESET
							NULL != pMyUnit->GetInventory().GetItemByTID( RESET_A_SKILL_QUEST_ITEM_ID ) ||
#endif SERV_QUEST_SKILL_RESET
#ifdef SERV_2ND_CLASS_SKILL_RESET
							NULL != pMyUnit->GetInventory().GetItemByTID( RESET_A_SKILL_2ND_CLASS_CHANGE_EVENT_ITEM_ID ) ||
#endif SERV_2ND_CLASS_SKILL_RESET
#ifdef SERV_EVENT_RURIEL_RESET_SKILL_ITEM
							NULL != pMyUnit->GetInventory().GetItemByTID( RURIEL_RESET_A_SKILL_ITEM ) ||
							NULL != pMyUnit->GetInventory().GetItemByTID( RURIEL_RESET_A_SKILL_EVENT_ITEM ) ||
#endif SERV_EVENT_RURIEL_RESET_SKILL_ITEM
#ifdef SERV_EVENT_CASH_SKILL_POINT_ITEM_JP
							NULL != pMyUnit->GetInventory().GetItemByTID( RESET_A_SKILL_EVENT_ITEM_ID_BY_JP ) ||
							NULL != pMyUnit->GetInventory().GetItemByTID( RESET_A_SKILL_EVENT_ITEM_ID_BY_JP2 ) ||
#endif //SERV_EVENT_CASH_SKILL_POINT_ITEM_JP
							NULL != pMyUnit->GetInventory().GetItemByTID( RESET_A_SKILL_ITEM_NOT_TRADE_ID ) ||
							NULL != pMyUnit->GetInventory().GetItemByTID( RESET_A_SKILL_EVENT_ITEM2 ) )
						{
							m_bNowInitSkill = true;		/// 소지 했으니, 초기화 활성
						}
						else
						{
							/// 미 소지 중 이라면, 초기화 아이템 획득 안내창 표시
							g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_25175 ), g_pMain->GetNowState() );
						}
					}
				}
			}
			else	/// 초기화 버튼 비활성 할 때는 조건 검사 필요없이 비활성 시키자
			{
				m_bNowInitSkill = false;
			}

			UpdateUI( false, true, true );

			return true;
		} break;

	case STUCM_BUTTON_INIT_SKILL:		/// 스킬 슬롯 초기화 버튼
		{
			CKTDGUIButton* pControl = (CKTDGUIButton*)lParam;
			CX2SkillTree::SKILL_ID eSkillID = static_cast<CX2SkillTree::SKILL_ID>( pControl->GetDummyInt(0) );

			if ( NULL != g_pData && 
				 NULL != g_pData->GetSkillTree() &&
				 NULL != g_pData->GetMyUser() &&
				 NULL != g_pData->GetMyUser()->GetSelectUnit() )
			{
				/// 유닛 클레스 타입
				CX2Unit::UNIT_CLASS						eUnitClass			= static_cast<CX2Unit::UNIT_CLASS>( g_pData->GetMyUser()->GetSelectUnit()->GetClass() );
				/// 스킬 트리 템플릿
				const CX2SkillTree::SkillTreeTemplet*	pSkillTreeTemplet	= g_pData->GetSkillTree()->GetSkillTreeTemplet( eUnitClass, eSkillID );
				/// 스킬 템플릿
				const CX2SkillTree::SkillTemplet*		pSkillTemplet		= g_pData->GetSkillTree()->GetSkillTemplet( eSkillID );	/// 스킬 템플릿
				/// 유저 스킬 정보
				const CX2UserSkillTree&						refUserSkillTree	= g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_UserSkillTree;

				if ( NULL != pSkillTreeTemplet && NULL != pSkillTemplet )
				{
					const int				iSkillLevel				= refUserSkillTree.GetSkillLevel( eSkillID );						/// 스킬 레벨
					const int				iUnitLevel				= g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_Level;	/// 유닛 레벨
					CX2SkillTree::SKILL_ID	eAnotherSkillID			= GetAnotherSelectSkillID( eSkillID );	/// 반대편 스킬 아이디
					bool					bEnoughLevel			= false;								/// 요구 습득 레벨 만족 여부
					
					/// 현재 스킬을 선행 스킬로 한 스킬이 찍혀 있는지 검사
					const bool bHaveFollowingSkill = 0 < refUserSkillTree.GetSkillLevel( static_cast<CX2SkillTree::SKILL_ID>( pSkillTreeTemplet->m_iFollowingSkill ) ) ? true : false ;

					/// 기본 스킬 이거나, 현재 스킬을 선행 스킬로 한 스킬이 찍혀 있다면
					if ( true == g_pData->GetSkillTree()->isDefaultSkill( eSkillID ) || true == bHaveFollowingSkill )
					{
						/// 스킬 레벨 1로 초기화 시킨다는 팝업 안내
						CKTDGUIDialogType m_pDLGMsgBox = g_pMain->KTDGUIOkAndCancelMsgBox( D3DXVECTOR2(-999,-999), 
							GET_REPLACED_STRING( ( STR_ID_25295, "L", pSkillTemplet->m_wstrName ) ), STUCM_INIT_SKILL_ONE_POINT, g_pMain->GetNowState() );

						if ( NULL != m_pDLGMsgBox )
							m_pDLGMsgBox->AddDummyInt( static_cast<int>( eSkillID ) );
					}
					else
					{
						/// 스킬 레벨 0으로 초기화 시킨다는 팝업 안내
						CKTDGUIDialogType m_pDLGMsgBox = g_pMain->KTDGUIOkAndCancelMsgBox( D3DXVECTOR2(-999,-999), 
							GET_REPLACED_STRING( ( STR_ID_25174, "L", pSkillTemplet->m_wstrName ) ), STUCM_INIT_SKILL, g_pMain->GetNowState() );

						if ( NULL != m_pDLGMsgBox )
							m_pDLGMsgBox->AddDummyInt( static_cast<int>( eSkillID ) );
					}
				}
			}

			return true;
		} break;

	case STUCM_INIT_SKILL:				/// 스킬 슬롯 초기화 처리 ( 0으로 )
		{
			CKTDGUIControl* pControl = (CKTDGUIControl*)lParam;

			if ( NULL != pControl && NULL != pControl->GetDialog() )
			{
				CX2SkillTree::SKILL_ID eCheckSkillID = static_cast<CX2SkillTree::SKILL_ID>( pControl->GetDialog()->GetDummyInt( 0 ) );

				g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, pControl->GetDialog(), NULL, false );

				if ( NULL != g_pData && NULL != g_pData->GetSkillTree() && CX2SkillTree::SI_NONE != eCheckSkillID  )
				{
					m_bNowInitSkill = false;
					g_pData->GetSkillTree()->Handler_EGS_RESET_SKILL_REQ( eCheckSkillID );
				}
			}

			return true;
		} break;

	case STUCM_INIT_SKILL_ONE_POINT:	/// 스킬 슬롯 초기화 처리 ( 1로 )
		{
			CKTDGUIControl* pControl = (CKTDGUIControl*)lParam;

			if ( NULL != pControl && NULL != pControl->GetDialog() )
			{
				CX2SkillTree::SKILL_ID eCheckSkillID = static_cast<CX2SkillTree::SKILL_ID>( pControl->GetDialog()->GetDummyInt( 0 ) );

				g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, pControl->GetDialog(), NULL, false );

				if ( NULL != g_pData && NULL != g_pData->GetSkillTree() && CX2SkillTree::SI_NONE != eCheckSkillID  )
				{
					m_bNowInitSkill = false;
					g_pData->GetSkillTree()->Handler_EGS_RESET_SKILL_REQ( eCheckSkillID );
				}
			}

			return true;
		} break;

#ifdef SKILL_PAGE_SYSTEM //JHKang
	case STUCM_SKILL_PAGE1:
		{
			if ( g_pData == NULL || g_pData->GetMyUser() == NULL || g_pData->GetMyUser()->GetSelectUnit() == NULL )
				return false;
			
			CX2Unit::UnitData* pUnitData = &g_pData->GetMyUser()->GetSelectUnit()->AccessUnitData();

			pUnitData->m_UserSkillTree.SetSelectPage( 1 );
			pUnitData->m_iSPoint = pUnitData->m_UserSkillTree.GetSkillPoint( 1 );
			pUnitData->m_iCSPoint = pUnitData->m_UserSkillTree.GetCashSkillPoint( 1 );
			UpdateUI( false, true, true, true );
			
			return true;
		} break;

	case STUCM_SKILL_PAGE2:
		{
			if ( g_pData == NULL || g_pData->GetMyUser() == NULL || g_pData->GetMyUser()->GetSelectUnit() == NULL )
				return false;

			CX2Unit::UnitData* pUnitData = &g_pData->GetMyUser()->GetSelectUnit()->AccessUnitData();

			pUnitData->m_UserSkillTree.SetSelectPage( 2 );
			pUnitData->m_iSPoint = pUnitData->m_UserSkillTree.GetSkillPoint( 2 );
			pUnitData->m_iCSPoint = pUnitData->m_UserSkillTree.GetCashSkillPoint( 2 );
			UpdateUI( false, true, true, true );

			return true;
		} break;

	case STUCM_SKILL_PAGE3:
		{
			if ( g_pData == NULL || g_pData->GetMyUser() == NULL || g_pData->GetMyUser()->GetSelectUnit() == NULL )
				return false;

			CX2Unit::UnitData* pUnitData = &g_pData->GetMyUser()->GetSelectUnit()->AccessUnitData();

			pUnitData->m_UserSkillTree.SetSelectPage( 3 );
			pUnitData->m_iSPoint = pUnitData->m_UserSkillTree.GetSkillPoint( 3 );
			pUnitData->m_iCSPoint = pUnitData->m_UserSkillTree.GetCashSkillPoint( 3 );
			UpdateUI( false, true, true, true );

			return true;
		} break;

	case STUCM_ADD_SKILL_PAGE_ED:
		{
			if ( g_pMain->GetNowStateID() != CX2Main::XS_VILLAGE_MAP )
			{
				g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( -999, -999 ), GET_STRING( STR_ID_29410 ), g_pMain->GetNowState() );
				return false;
			}

			if( true == g_pMain->IsWaitingServerPacket( EGS_GET_NEXT_SKILL_PAGE_ED_ACK ) )
				return false;

			g_pData->GetServerProtocol()->SendID( EGS_GET_NEXT_SKILL_PAGE_ED_REQ );
			g_pMain->AddServerPacket( EGS_GET_NEXT_SKILL_PAGE_ED_ACK );

			return true;
		} break;

	case STUCM_ADD_SKILL_PAGE_CASH:
		{
			if ( g_pData && g_pData->GetMyUser() && g_pData->GetMyUser()->GetSelectUnit() )
			{
				CX2StateField*	pStateField	=	NULL;
				pStateField		=  static_cast< CX2StateField* >( g_pMain->GetNowState() );

				if ( pStateField != NULL )
				{
					if ( g_pTFieldGame != NULL )
					{
						g_pTFieldGame->CloseFieldName( 0.0f );
					}

					g_pData->GetCashShop()->SetMenuTypeCallingCashShop( CX2UIManager::UI_MENU_PRIVATE_BANK );
					pStateField->ToggleCashShop();
				}

				return true;
			}

			return false;
		} break;

	case STUCM_DECIDE_SKILL_PAGE:
		{
			if( true == g_pMain->IsWaitingServerPacket( EGS_DECIDE_TO_USE_THIS_SKILL_PAGE_ACK ) )
				return false;

			if ( g_pData && g_pData->GetMyUser() && g_pData->GetMyUser()->GetSelectUnit() &&
				g_pData->GetMyUser()->GetSelectUnit()->AccessUnitData().m_UserSkillTree.GetUsingPage() ==
				g_pData->GetMyUser()->GetSelectUnit()->AccessUnitData().m_UserSkillTree.GetSelectPage() - 1 )
				return false;

			if ( g_pMain->GetNowStateID() != CX2Main::XS_VILLAGE_MAP )
			{
				g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( -999, -999 ), GET_STRING( STR_ID_29368 ), g_pMain->GetNowState() );
				return false;
			}

			if ( g_pData->GetSkillTree() != NULL )
			{
				/// 스킬 초기화 상태가 아니고, 레벨을 올리고자 하는 스킬이 있을 때 선택 팝업 생성
				if ( false == m_bNowInitSkill && false == g_pData->GetSkillTree()->GetMapSkillInfo().empty() )
				{
					int iPage = g_pData->GetMyUser()->GetSelectUnit()->AccessUnitData().m_UserSkillTree.GetUsingPage() + 1;

					g_pMain->KTDGUIOkAndCancelMsgBox( D3DXVECTOR2(-999,-999), GET_REPLACED_STRING( ( STR_ID_29387, "i", iPage ) ), 
						STUCM_DECIDE_WITH_SKILL_LEVEL_PLUS, g_pMain->GetNowState(), STUCM_CANCEL_DECIDE_WITH_SKILL_LEVEL_PLUS );

					m_bNowLearnSkill = false;
				}
				/// 없다면, 바로 닫기
				else
				{
					return g_pData->GetSkillTree()->Handler_EGS_DECIDE_TO_USE_THIS_SKILL_PAGE_REQ( 
						g_pData->GetMyUser()->GetSelectUnit()->AccessUnitData().m_UserSkillTree.GetSelectPage() );
				}
			}

			return false;

		} break;

	case STUCM_BUY_CONFIRM:
		{
			if ( m_pDLGBuyConfirm != NULL )
				g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDLGBuyConfirm, NULL, false );

			m_pDLGBuyConfirm = NULL;

			if ( g_pData->GetMyUser()->GetSelectUnit()->AccessUnitData().m_ED >= m_iConfirmED )
				return g_pData->GetSkillTree()->Handler_EGS_EXPAND_SKILL_PAGE_REQ();
			else
				g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( -999, -999 ), GET_STRING( STR_ID_28961 ), g_pMain->GetNowState() );
		} break;

	case STUCM_BUY_CANCEL:
		{
			if ( m_pDLGBuyConfirm != NULL )
				g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDLGBuyConfirm, NULL, false );

			m_pDLGBuyConfirm = NULL;

			return true;
		} break;

	case STUCM_DECIDE_WITH_SKILL_LEVEL_PLUS:
		{
			CKTDGUIControl* pControl = (CKTDGUIControl*)lParam;

			if ( NULL != pControl )
				g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, pControl->GetDialog(), NULL, false );

			if ( NULL != g_pData && NULL != g_pData->GetSkillTree() )
			{
				g_pData->GetSkillTree()->Handler_EGS_GET_SKILL_REQ();

				g_pData->GetSkillTree()->SetUsedSPoint( 0 );
				g_pData->GetSkillTree()->SetUsedCSPoint( 0 );
				g_pData->GetSkillTree()->ClearMapSkillInfo();
			}
			m_bNowLearnSkill = false;

			return g_pData->GetSkillTree()->Handler_EGS_DECIDE_TO_USE_THIS_SKILL_PAGE_REQ( 
				g_pData->GetMyUser()->GetSelectUnit()->AccessUnitData().m_UserSkillTree.GetSelectPage() );
		} break;

	case STUCM_CANCEL_DECIDE_WITH_SKILL_LEVEL_PLUS:
		{
			CKTDGUIControl* pControl = (CKTDGUIControl*)lParam;

			if ( NULL != pControl )
				g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, pControl->GetDialog(), NULL, false );

			if ( NULL != g_pData && NULL != g_pData->GetSkillTree() )
			{
				g_pData->GetSkillTree()->SetUsedSPoint( 0 );
				g_pData->GetSkillTree()->SetUsedCSPoint( 0 );
				g_pData->GetSkillTree()->ClearMapSkillInfo();
			}
			m_bNowLearnSkill = false;

			return g_pData->GetSkillTree()->Handler_EGS_DECIDE_TO_USE_THIS_SKILL_PAGE_REQ( 
				g_pData->GetMyUser()->GetSelectUnit()->AccessUnitData().m_UserSkillTree.GetSelectPage() );
		} break;
#endif //SKILL_PAGE_SYSTEM

	default:
		break;
	}
	return false;
}

void CX2UISkillTreeNew::SetShow(bool val)
{
	m_bShow = val;

	if(val)	// 켜질 때 처리해야 할 부분
	{
#ifdef FIX_INTRUSION_SKILL_BUG
		//게임 시작시 스킬창 열 수 없도록 수정
		if( CX2Main::XS_PVP_ROOM == g_pMain->GetNowStateID())
		{
			CX2StatePVPRoom* pState = (CX2StatePVPRoom*) g_pMain->GetNowState();
			if( NULL != pState )
			{
				if( true == pState->GetDisableSkillUIOpen() )
				{
					m_bShow = false;
					return;
				}
			}
		}
#endif //FIX_INTRUSION_SKILL_BUG
#ifdef OPEN_SKILL_GUIDE_FIRST_TIME
		if( g_pData != NULL && g_pData->GetSelectUnitLevel() <= 10 )
		{
			const string strFileName = "readme.txt";
			FILE* file = NULL;
			file = fopen( strFileName.c_str(), "r" );		
			if( NULL == file )
			{
				file = fopen( strFileName.c_str(), "w" );
				fclose(file);

				if( m_pDLGFirstGuide == NULL )
				{
					m_pDLGFirstGuide = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_Help_Skill.lua" );
					g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGFirstGuide );
					m_pDLGFirstGuide->SetDisableUnderWindow(true);					
				}		
				m_pDLGFirstGuide->SetShow(true);
			}
			else
			{
				fclose(file);
			}		
		}
#endif OPEN_SKILL_GUIDE_FIRST_TIME

		m_bSkillRemoveMode = false;
#ifdef SKILL_PAGE_SYSTEM //JHKang
		UpdateUI( true, true, true, true );
#else //SKILL_PAGE_SYSTEM
		UpdateUI(true, true, true);
#endif //SKILL_PAGE_SYSTEM

		g_pData->GetUIManager()->UIOpened( CX2UIManager::UI_MENU_SKILL );

#ifdef REFORM_UI_SKILLSLOT
		if( CX2Main::XS_PVP_ROOM == g_pMain->GetNowStateID() )
			g_pData->GetUIManager()->SetShowSkillSlot(true);
		
#endif //REFORM_UI_SKILLSLOT

#ifdef SKILL_SLOT_UI_TYPE_B
		SetSkillUIType(true);
#endif //SKILL_SLOT_UI_TYPE_B
	}
	else	// 꺼질 때 처리해야 할 부분
	{
#ifdef OPEN_SKILL_GUIDE_FIRST_TIME
		if(m_pDLGFirstGuide != NULL)
			m_pDLGFirstGuide->SetShow(false);
#endif
		ShowSkillDesc(false);
		ShowUIDesc(false);
		SAFE_DELETE_DIALOG(m_pDLGSkillUnsealInform);

#ifdef REFORM_UI_SKILLSLOT
		m_bShowEquipSkillGuide = false;
#endif //REFORM_UI_SKILLSLOT		
		g_pData->GetUIManager()->UIClosed(CX2UIManager::UI_MENU_SKILL);

#ifdef REFORM_UI_SKILLSLOT
		if( CX2Main::XS_PVP_ROOM == g_pMain->GetNowStateID() )
			g_pData->GetUIManager()->SetShowSkillSlot(false);
#endif //REFORM_UI_SKILLSLOT

#ifdef SKILL_SLOT_UI_TYPE_B //옵션으로 저장 하게되면 옵션쪽에서 가져오도록 수정
		if( NULL != g_pMain )
		{
			SetSkillUIType(g_pMain->GetGameOption().GetIsSkillUITypeA());
		}
#endif //SKILL_SLOT_UI_TYPE_B

		m_bNowLearnSkill	= false;	/// 스킬 습득 상태 해제
		m_bNowInitSkill		= false;	/// 스킬 초기화 상태 해제

		if (	NULL != g_pData &&
			NULL != g_pData->GetSkillTree() )
		{
			g_pData->GetSkillTree()->ClearMapSkillInfo();
			g_pData->GetSkillTree()->SetUsedSPoint( 0 );
			g_pData->GetSkillTree()->SetUsedCSPoint( 0 );
		}
#ifdef SKILL_PAGE_SYSTEM //JHKang
		if ( g_pData != NULL && g_pData->GetMyUser() != NULL && g_pData->GetMyUser()->GetSelectUnit() != NULL )
		{
			CX2UserSkillTree& refUserSkillTree = g_pData->GetMyUser()->GetSelectUnit()->AccessUnitData().m_UserSkillTree;
			refUserSkillTree.SetSelectPage( refUserSkillTree.GetUsingPage() + 1 );
		}
#endif //SKILL_PAGE_SYSTEM
	}
	if( NULL != m_pDLGUISkillTree )
		m_pDLGUISkillTree->SetShowEnable(m_bShow, m_bShow);
}

// 캐릭터가 선택됐을 때 / 전직했을 때만 해 준다.
void CX2UISkillTreeNew::InitSkillTreeUI()
{
	SAFE_DELETE_DIALOG(m_pDLGUISkillTree);
	
	m_bSkillSlotSwap = false;

	m_mapSkillSlotUISet.clear();
	m_pDLGUISkillTree = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_UI_Skill_Tree_New.lua" );
	g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGUISkillTree );
	m_pDLGUISkillTree->SetDisableUnderWindow(true);
	m_pDLGUISkillTree->SetShow(false);


	D3DXVECTOR3 tmp;
	tmp = m_pDLGUISkillTree->GetDummyPos(1);
 	m_DLGSize.x = tmp.x;
 	m_DLGSize.y = tmp.y;

	

	if( g_pData->GetMyUser() != NULL &&
		g_pData->GetMyUser()->GetSelectUnit() != NULL && 
		g_pData->GetMyUser()->GetSelectUnit()->GetUnitTemplet() != NULL )
	{
		m_pUserSkillTree = &(g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_UserSkillTree);

		const CX2Unit::UnitTemplet* pUnitTemplet = g_pData->GetMyUser()->GetSelectUnit()->GetUnitTemplet();

		CreateSkillTree( pUnitTemplet->m_UnitClass );
		InitEquippedSkillSlotData();

	#ifdef SKILL_PAGE_SYSTEM //JHKang
		UpdateSkillPageUI();
	#endif //SKILL_PAGE_SYSTEM
	}	
}

void CX2UISkillTreeNew::CreateSkillTree( CX2Unit::UNIT_CLASS eUnitClass )
{

	if( NULL == m_pDLGUISkillTree ||
		NULL == g_pData ||
		NULL == g_pData->GetSkillTree() ||
		NULL == g_pData->GetMyUser() ||
		NULL == g_pData->GetMyUser()->GetSelectUnit()  )
	{
		return;
	}

	CKTDGUIContolList*	pControlList	= static_cast<CKTDGUIContolList*>( m_pDLGUISkillTree->GetControl( L"SkillTreeControlList" ) );	/// 스크롤 객체
	if ( NULL == pControlList )
		return;

	// Unit Class를 이용해서 해당 Map을 어떻게든 받아온다. 왔다고 치자.
	const CX2SkillTree::SkillTreeTempletMap& refSkillTreeTempletMap = g_pData->GetSkillTree()->GetSkillTreeTempletMap( (int)eUnitClass );

	for( CX2SkillTree::SkillTreeTempletMap::const_iterator mapit = refSkillTreeTempletMap.begin(); mapit != refSkillTreeTempletMap.end(); ++mapit )
	{
		CX2SkillTree::SKILL_ID eSkillID = mapit->first;
		const CX2SkillTree::SkillTreeTemplet& refSkillTreeTemplet = mapit->second;


		/// 현재 남은 SP 반환
		const CX2Unit::UnitData* pUnitData = &g_pData->GetMyUser()->GetSelectUnit()->GetUnitData();

		int iNoUseSP = 0;
		int iNoUseCP = 0;

		if ( NULL != pUnitData )
		{
			iNoUseSP = pUnitData->m_iSPoint;		/// 남은 스킬 포인트
			iNoUseCP = pUnitData->m_iCSPoint;	/// 남은 캐시 스킬 포인트 ( 그노시스의 축복 )
		}

		/// 현재 스킬 슬롯의 레벨 반환
		const CX2UserSkillTree&	refUserSkillTree	= pUnitData->m_UserSkillTree;
		const int			iSkillLevel			= refUserSkillTree.GetSkillLevel( eSkillID );

		const CX2SkillTree::SkillTemplet* pSkillTemplet = g_pData->GetSkillTree()->GetSkillTemplet( eSkillID );
		if(pSkillTemplet == NULL)
		{
#ifndef _SERVICE_
			WCHAR buf[256] = {0,};
			//wsprintf(buf, L"Skill ID : %d\nExist in SkillTreeTemplet but NULL SkillTemplet", (int)eSkillID);
			StringCchPrintf(buf, 256, L"Skill ID : %d\nExist in SkillTreeTemplet but NULL SkillTemplet", (int)eSkillID);
			MessageBox( g_pKTDXApp->GetHWND(), buf, L"Error", MB_OK );
#endif
			continue;
		}

		int row = refSkillTreeTemplet.m_iTier;
		int col = refSkillTreeTemplet.m_iIndexInTier;
		
		/// 행과 열에 따른 스킬 아이디 저장
		std::map<SkillPositionKey, CX2SkillTree::SKILL_ID>::iterator it = m_mapSkillPosition.find( SkillPositionKey( col, row ) );
		if( it == m_mapSkillPosition.end() )
		{
			m_mapSkillPosition.insert( make_pair( SkillPositionKey( col, row ), eSkillID ) );
		}

		SkillSlotUI UISet;

		CreateSlotUIPreset( UISet, static_cast<int>( eSkillID ), col );		/// 각 슬롯별 UI 설정

		/// 스크롤 리스트에 슬롯 관련 UI 컨트롤 추가
		pControlList->InsertItem( UISet.m_pSlot,					row, col );
		pControlList->InsertItem( UISet.m_pStaticBG,				row, col );
		pControlList->InsertItem( UISet.m_pStaticSkillBG,			row, col );
		pControlList->InsertItem( UISet.m_pStaticSkillLock,			row, col );
		pControlList->InsertItem( UISet.m_pButtonSkillInit,			row, col );
		pControlList->InsertItem( UISet.m_pButtonSkillDesc,			row, col );
		pControlList->InsertItem( UISet.m_pButtonSkillLearn,		row, col );
		pControlList->InsertItem( UISet.m_pStaticSkillMaster,		row, col );
		pControlList->InsertItem( UISet.m_pStaticSkillNoLearn,		row, col );
		pControlList->InsertItem( UISet.m_pStaticStringRequireSP,	row, col );

		/// 관리용 Map에 추가
		m_mapSkillSlotUISet.insert(std::make_pair(eSkillID, UISet));

		/// 이제 Data를 만들어 넣을 차례				
		CX2SkillTreeSlotData* pSkillTreeSlotData = new CX2SkillTreeSlotData;
		pSkillTreeSlotData->m_eSkillID = eSkillID;

		UISet.m_pSlot->SetID( static_cast<int>( eSkillID ) );
		UISet.m_pSlot->SetSlotData( pSkillTreeSlotData );

		pSkillTreeSlotData->m_eSkillID = eSkillID;
		pSkillTreeSlotData->m_bDataUpdated = true;

		/// 스킬 트리 초상화 등록을 위한 최대 스킬 배열 높이 연산
		if ( row > m_iMaxSkillRowNum )
			m_iMaxSkillRowNum = row;
	}

	SetBlindSelectSkill();						/// 2지선다 스킬 중 선택되지 않은 스킬에 대한 블라인드 설정

	CreateSkillTreePortrait( pControlList );	/// 스킬 트리 초상화 설정

	SetSelectSkillSlotPosition();				/// 2지선다 스킬 위치 조정

	SetDivisionLine( pControlList );			/// 스킬 구분선 설정

	/// 스킬 메인 창을 가장 하위 레이어로 설정
	CKTDGUIStatic* pStaticSkill = static_cast<CKTDGUIStatic*>( m_pDLGUISkillTree->GetControl( L"g_pStaticSkill" ) );

	if ( NULL != pStaticSkill )
		m_pDLGUISkillTree->ChangeSequence( pStaticSkill, false );

	/// 스크롤 리스트 가장 위로 설정
	pControlList->SetIndex(0, 0);
}

CX2EquippedSkillSlotData* CX2UISkillTreeNew::GetEquippedSlotData( int index, bool bSlotB /*= false*/ )
{
	CX2EquippedSkillSlotData* pSlotData = NULL;

#ifdef REFORM_UI_SKILLSLOT
	if(false == bSlotB)	// 슬롯 A
	{
		switch(index)
		{
		case 0:
			{
				CKTDGUISlot* pSlot = (CKTDGUISlot*) m_pDLGGameSkillSlot->GetControl( L"SlotA1" );
				if(pSlot != NULL)
					pSlotData = (CX2EquippedSkillSlotData*) pSlot->GetSlotData();
			} break;
		case 1:
			{
				CKTDGUISlot* pSlot = (CKTDGUISlot*) m_pDLGGameSkillSlot->GetControl( L"SlotA2" );
				if(pSlot != NULL)
					pSlotData = (CX2EquippedSkillSlotData*) pSlot->GetSlotData();
			} break;
		case 2:
			{
				CKTDGUISlot* pSlot = (CKTDGUISlot*) m_pDLGGameSkillSlot->GetControl( L"SlotA3" );
				if(pSlot != NULL)
					pSlotData = (CX2EquippedSkillSlotData*) pSlot->GetSlotData();
			} break;
		case 3:
			{
				CKTDGUISlot* pSlot = (CKTDGUISlot*) m_pDLGGameSkillSlot->GetControl( L"SlotA4" );
				if(pSlot != NULL)
					pSlotData = (CX2EquippedSkillSlotData*) pSlot->GetSlotData();
			} break;
		default:
			break;
		}
	}
	else	// 슬롯 B
	{
		switch(index)
		{
		case 0:
			{
				CKTDGUISlot* pSlot = (CKTDGUISlot*) m_pDLGGameSkillSlot->GetControl( L"SlotB1" );
				if(pSlot != NULL)
					pSlotData = (CX2EquippedSkillSlotData*) pSlot->GetSlotData();
			} break;
		case 1:
			{
				CKTDGUISlot* pSlot = (CKTDGUISlot*) m_pDLGGameSkillSlot->GetControl( L"SlotB2" );
				if(pSlot != NULL)
					pSlotData = (CX2EquippedSkillSlotData*) pSlot->GetSlotData();
			} break;
		case 2:
			{
				CKTDGUISlot* pSlot = (CKTDGUISlot*) m_pDLGGameSkillSlot->GetControl( L"SlotB3" );
				if(pSlot != NULL)
					pSlotData = (CX2EquippedSkillSlotData*) pSlot->GetSlotData();
			} break;
		case 3:
			{
				CKTDGUISlot* pSlot = (CKTDGUISlot*) m_pDLGGameSkillSlot->GetControl( L"SlotB4" );
				if(pSlot != NULL)
					pSlotData = (CX2EquippedSkillSlotData*) pSlot->GetSlotData();
			} break;
		default:
			break;
		}
	}
#else
	if(false == bSlotB)	// 슬롯 A
	{
		switch(index)
		{
		case 0:
			{
				CKTDGUISlot* pSlot = (CKTDGUISlot*) m_pDLGUISkillTree->GetControl( L"SlotA1" );
				if(pSlot != NULL)
					pSlotData = (CX2EquippedSkillSlotData*) pSlot->GetSlotData();
			} break;
		case 1:
			{
				CKTDGUISlot* pSlot = (CKTDGUISlot*) m_pDLGUISkillTree->GetControl( L"SlotA2" );
				if(pSlot != NULL)
					pSlotData = (CX2EquippedSkillSlotData*) pSlot->GetSlotData();
			} break;
		case 2:
			{
				CKTDGUISlot* pSlot = (CKTDGUISlot*) m_pDLGUISkillTree->GetControl( L"SlotA3" );
				if(pSlot != NULL)
					pSlotData = (CX2EquippedSkillSlotData*) pSlot->GetSlotData();
			} break;
		case 3:
			{
				CKTDGUISlot* pSlot = (CKTDGUISlot*) m_pDLGUISkillTree->GetControl( L"SlotA4" );
				if(pSlot != NULL)
					pSlotData = (CX2EquippedSkillSlotData*) pSlot->GetSlotData();
			} break;
		default:
			break;
		}
	}
	else	// 슬롯 B
	{
		switch(index)
		{
		case 0:
			{
				CKTDGUISlot* pSlot = (CKTDGUISlot*) m_pDLGUISkillTree->GetControl( L"SlotB1" );
				if(pSlot != NULL)
					pSlotData = (CX2EquippedSkillSlotData*) pSlot->GetSlotData();
			} break;
		case 1:
			{
				CKTDGUISlot* pSlot = (CKTDGUISlot*) m_pDLGUISkillTree->GetControl( L"SlotB2" );
				if(pSlot != NULL)
					pSlotData = (CX2EquippedSkillSlotData*) pSlot->GetSlotData();
			} break;
		case 2:
			{
				CKTDGUISlot* pSlot = (CKTDGUISlot*) m_pDLGUISkillTree->GetControl( L"SlotB3" );
				if(pSlot != NULL)
					pSlotData = (CX2EquippedSkillSlotData*) pSlot->GetSlotData();
			} break;
		case 3:
			{
				CKTDGUISlot* pSlot = (CKTDGUISlot*) m_pDLGUISkillTree->GetControl( L"SlotB4" );
				if(pSlot != NULL)
					pSlotData = (CX2EquippedSkillSlotData*) pSlot->GetSlotData();
			} break;
		default:
			break;
		}
	}
#endif //REFORM_UI_SKILLSLOT

	return pSlotData;
}
#ifdef REFORM_UI_SKILLSLOT
CKTDGUISlot* CX2UISkillTreeNew::GetEquippedSlot( int index , bool bSlotB /* = false */ )
{
	if (m_pDLGUISkillTree == NULL)
		return NULL;

	CKTDGUISlot* pSlot = NULL ;

	//인덱스값이 4이상으로 오면 무조건 B슬롯으로 판단
	if( index >= 4 )
	{
		index -= 4;
		bSlotB = true;
	}
		
	if(false == bSlotB)	// 슬롯 A
	{
		switch(index)
		{
		case 0:
			{
				pSlot = (CKTDGUISlot*) m_pDLGGameSkillSlot->GetControl( L"SlotA1" );
			} break;
		case 1:
			{
				pSlot = (CKTDGUISlot*) m_pDLGGameSkillSlot->GetControl( L"SlotA2" );
			} break;
		case 2:
			{
				pSlot = (CKTDGUISlot*) m_pDLGGameSkillSlot->GetControl( L"SlotA3" );
			} break;
		case 3:
			{
				pSlot = (CKTDGUISlot*) m_pDLGGameSkillSlot->GetControl( L"SlotA4" );
			} break;
		default:
			break;
		}
	}
	else	// 슬롯 B
	{
		switch(index)
		{
		case 0:
			{
				pSlot = (CKTDGUISlot*) m_pDLGGameSkillSlot->GetControl( L"SlotB1" );
			} break;
		case 1:
			{
				pSlot = (CKTDGUISlot*) m_pDLGGameSkillSlot->GetControl( L"SlotB2" );
			} break;
		case 2:
			{
				pSlot = (CKTDGUISlot*) m_pDLGGameSkillSlot->GetControl( L"SlotB3" );
			} break;
		case 3:
			{
				pSlot = (CKTDGUISlot*) m_pDLGGameSkillSlot->GetControl( L"SlotB4" );
			} break;
		default:
			break;
		}
	}

	return pSlot;
}
#endif //REFORM_UI_SKILLSLOT
void CX2UISkillTreeNew::InitEquippedSkillSlotData()
{

	if(m_pDLGUISkillTree == NULL)
		return;

	for(int i=0; i<EQUIPPED_SKILL_SLOT_COUNT*2; ++i)
	{
		bool bSlotB = (i > 3) ? true : false;
		int iSlotIndex = (i > 3) ? i-4 : i;
		CX2EquippedSkillSlotData* pData = GetEquippedSlotData(iSlotIndex, bSlotB);
		SAFE_DELETE(pData);
	}
#ifdef REFORM_UI_SKILLSLOT
	CKTDGUISlot* pSlot = NULL;
	pSlot = (CKTDGUISlot*) m_pDLGGameSkillSlot->GetControl( L"SlotA1" );
	if(pSlot != NULL)
		pSlot->SetSlotData(new CX2EquippedSkillSlotData(0, false));

	pSlot = (CKTDGUISlot*) m_pDLGGameSkillSlot->GetControl( L"SlotA2" );
	if(pSlot != NULL)
		pSlot->SetSlotData(new CX2EquippedSkillSlotData(1, false));

	pSlot = (CKTDGUISlot*) m_pDLGGameSkillSlot->GetControl( L"SlotA3" );
	if(pSlot != NULL)
		pSlot->SetSlotData(new CX2EquippedSkillSlotData(2, false));

	pSlot = (CKTDGUISlot*) m_pDLGGameSkillSlot->GetControl( L"SlotA4" );
	if(pSlot != NULL)
		pSlot->SetSlotData(new CX2EquippedSkillSlotData(3, false));

	pSlot = (CKTDGUISlot*) m_pDLGGameSkillSlot->GetControl( L"SlotB1" );
	if(pSlot != NULL)
		pSlot->SetSlotData(new CX2EquippedSkillSlotData(0, true));

	pSlot = (CKTDGUISlot*) m_pDLGGameSkillSlot->GetControl( L"SlotB2" );
	if(pSlot != NULL)
		pSlot->SetSlotData(new CX2EquippedSkillSlotData(1, true));

	pSlot = (CKTDGUISlot*) m_pDLGGameSkillSlot->GetControl( L"SlotB3" );
	if(pSlot != NULL)
		pSlot->SetSlotData(new CX2EquippedSkillSlotData(2, true));

	pSlot = (CKTDGUISlot*) m_pDLGGameSkillSlot->GetControl( L"SlotB4" );
	if(pSlot != NULL)
		pSlot->SetSlotData(new CX2EquippedSkillSlotData(3, true));
#else
	CKTDGUISlot* pSlot = NULL;
	pSlot = (CKTDGUISlot*) m_pDLGUISkillTree->GetControl( L"SlotA1" );
	if(pSlot != NULL)
		pSlot->SetSlotData(new CX2EquippedSkillSlotData(0, false));

	pSlot = (CKTDGUISlot*) m_pDLGUISkillTree->GetControl( L"SlotA2" );
	if(pSlot != NULL)
		pSlot->SetSlotData(new CX2EquippedSkillSlotData(1, false));

	pSlot = (CKTDGUISlot*) m_pDLGUISkillTree->GetControl( L"SlotA3" );
	if(pSlot != NULL)
		pSlot->SetSlotData(new CX2EquippedSkillSlotData(2, false));

	pSlot = (CKTDGUISlot*) m_pDLGUISkillTree->GetControl( L"SlotA4" );
	if(pSlot != NULL)
		pSlot->SetSlotData(new CX2EquippedSkillSlotData(3, false));

	pSlot = (CKTDGUISlot*) m_pDLGUISkillTree->GetControl( L"SlotB1" );
	if(pSlot != NULL)
		pSlot->SetSlotData(new CX2EquippedSkillSlotData(0, true));

	pSlot = (CKTDGUISlot*) m_pDLGUISkillTree->GetControl( L"SlotB2" );
	if(pSlot != NULL)
		pSlot->SetSlotData(new CX2EquippedSkillSlotData(1, true));

	pSlot = (CKTDGUISlot*) m_pDLGUISkillTree->GetControl( L"SlotB3" );
	if(pSlot != NULL)
		pSlot->SetSlotData(new CX2EquippedSkillSlotData(2, true));

	pSlot = (CKTDGUISlot*) m_pDLGUISkillTree->GetControl( L"SlotB4" );
	if(pSlot != NULL)
		pSlot->SetSlotData(new CX2EquippedSkillSlotData(3, true));
#endif //REFORM_UI_SKILLSLOT
}

void CX2UISkillTreeNew::UpdateSkillEquipedSlotUI()
{
	if(m_pUserSkillTree != NULL)
	{
		for(int i=0; i<EQUIPPED_SKILL_SLOT_COUNT*2; ++i)
		{			
			bool bSlotB = (i > 3) ? true : false;
			int iSlotIndex = (i > 3) ? i-4 : i;
			CX2EquippedSkillSlotData* pSlotData = GetEquippedSlotData(iSlotIndex, bSlotB);
			if(pSlotData != NULL)
			{
				pSlotData->m_bDataUpdated = true;				
			}
		}

#ifdef REFORM_UI_SKILLSLOT
		if(m_pDLGGameSkillSlot != NULL)
		{
			switch(m_pUserSkillTree->GetSkillSlotBExpirationState())
			{
			case CX2UserSkillTree::SSBES_NOT_EXPIRED: //슬롯 확장 상태
			case CX2UserSkillTree::SSBES_PERMANENT: 
				{
					m_bIsExpandSkillSlot = true;
					CKTDGUIButton* pButtonBuyGuide = (CKTDGUIButton*) m_pDLGGameSkillSlot->GetControl( L"Button_Buy_Guide_Slot" );
					if( NULL != pButtonBuyGuide)
						pButtonBuyGuide->SetShowEnable(false, false);

					CKTDGUIStatic* pStaticLock = (CKTDGUIStatic*) m_pDLGGameSkillSlot->GetControl( L"LOCK" );
					if( NULL != pStaticLock)
						pStaticLock->SetShow(false);

					CKTDGUIStatic* pStaticSkillHotKey = (CKTDGUIStatic*) m_pDLGGameSkillSlot->GetControl( L"SkillHotKeyEx" );
					if( NULL != pStaticSkillHotKey)
						pStaticSkillHotKey->SetColor(D3DXCOLOR(1.f,1.f,1.f,1.f));
				}
				break;
			case CX2UserSkillTree::SSBES_EXPIRED: //슬롯 미확장 상태
			default:
				{
					m_bIsExpandSkillSlot =false;
					CKTDGUIButton* pButtonBuyGuide = (CKTDGUIButton*) m_pDLGGameSkillSlot->GetControl( L"Button_Buy_Guide_Slot" );
					if( NULL != pButtonBuyGuide)
						pButtonBuyGuide->SetShowEnable(true, true);

					CKTDGUIStatic* pStaticLock = (CKTDGUIStatic*) m_pDLGGameSkillSlot->GetControl( L"LOCK" );
					if( NULL != pStaticLock)
						pStaticLock->SetShow(true);

					CKTDGUIStatic* pStaticSkillHotKey = (CKTDGUIStatic*) m_pDLGGameSkillSlot->GetControl( L"SkillHotKeyEx" );
					if( NULL != pStaticSkillHotKey)
						pStaticSkillHotKey->SetColor(D3DXCOLOR(1.f,1.f,1.f,0.5f));
				}
				break;
			}
#else
		if(m_pDLGUISkillTree != NULL)
		{
			CKTDGUIButton* pButton = (CKTDGUIButton*) m_pDLGUISkillTree->GetControl( L"Button_Notice_Slot" );
			if(pButton != NULL)
			{
				switch(m_pUserSkillTree->GetSkillSlotBExpirationState())
				{
				case CX2UserSkillTree::SSBES_NOT_EXPIRED: //슬롯 확장 상태
					{
						pButton->SetShowEnable(true, true);
					} break;
				case CX2UserSkillTree::SSBES_PERMANENT: //슬롯 미확장 상태
				case CX2UserSkillTree::SSBES_EXPIRED:
				default:
					{
						pButton->SetShowEnable(false, false);
					} break;
				}
			}
#endif //REFORM_UI_SKILLSLOT
#ifdef GUIDE_SKILL_SLOTB 
			pButton = (CKTDGUIButton*) m_pDLGUISkillTree->GetControl( L"Button_Buy_Guide_Slot" );
			if(pButton != NULL)
			{
				switch(m_pUserSkillTree->GetSkillSlotBExpirationState())
				{
				case CX2UserSkillTree::SSBES_EXPIRED:
					{
						pButton->SetShowEnable(true, true);
					} break;
				case CX2UserSkillTree::SSBES_PERMANENT:
				case CX2UserSkillTree::SSBES_NOT_EXPIRED:
				default:
					{
						pButton->SetShowEnable(false, false);
					} break;
				}
			}
#endif GUIDE_SKILL_SLOTB
		}

#ifdef ELSWORD_WAY_OF_SWORD
		if( NULL == g_pX2Game && NULL != m_pDLGGameSkillSlot )
		{
			CKTDGUIStatic* pStatic_ElSkillVigor = (CKTDGUIStatic*) m_pDLGGameSkillSlot->GetControl( L"Static_EL_SKILL_B_B" );
			CKTDGUIStatic* pStatic_ElSkillDest = (CKTDGUIStatic*) m_pDLGGameSkillSlot->GetControl( L"Static_EL_SKILL_R_B" );
			if( NULL != pStatic_ElSkillVigor && NULL != pStatic_ElSkillDest)
			{
				pStatic_ElSkillVigor->SetShow(false);
				pStatic_ElSkillDest->SetShow(false);
			}

			pStatic_ElSkillVigor = (CKTDGUIStatic*) m_pDLGGameSkillSlot->GetControl( L"Static_EL_SKILL_B" );
			pStatic_ElSkillDest = (CKTDGUIStatic*) m_pDLGGameSkillSlot->GetControl( L"Static_EL_SKILL_R" );
			if( NULL != pStatic_ElSkillVigor && NULL != pStatic_ElSkillDest)
			{
				pStatic_ElSkillVigor->SetShow(false);
				pStatic_ElSkillDest->SetShow(false);
			}
		}
#endif //ELSWORD_WAY_OF_SWORD

	}
}

#ifdef SKILL_PAGE_SYSTEM //JHKang
void CX2UISkillTreeNew::UpdateUI( bool bEquipped, bool bSP /*= true*/, bool bSkillTree /*= false*/, bool bSkillPage_ /*= false*/ )
#else //SKILL_PAGE_SYSTEM
void CX2UISkillTreeNew::UpdateUI( bool bEquipped, bool bSP /*= true*/, bool bSkillTree /*= false*/ )
#endif //SKILL_PAGE_SYSTEM
{
	if(bEquipped)
	{
		UpdateSkillEquipedSlotUI();
	}

	if(bSP)
	{
		UpdateSPInfo();
	}

	if(bSkillTree)
	{
		UpdateSkillTree();
	}

#ifdef SKILL_PAGE_SYSTEM //JHKang
	if ( bSkillPage_ )
		UpdateSkillPageUI();
#endif //SKILL_PAGE_SYSTEM

}

void CX2UISkillTreeNew::UpdateSkillTree()
{
	// ControlList의 Map을 차례로 Iteration해 가면서 (이쪽이 빠르다!)
	// 슬롯이랑 버튼이 존재하면 해당 슬롯의 데이터를 읽어와서
	// 그냥 데이타 바뀌었다라고 눌러주면 되네? -_-
	// 1. 지금 익힐 수 있는 스킬 여부 : 버튼의 활성화/비활성화. bool 값에 따라서
	// 2. 1레벨이라도 익힌 스킬 여부 : 스킬 슬롯의 드래그 활성화 / 비활성화(패시브면 드래그 안 되게), 텍스쳐 변경. 스트링 설정 (레벨/최대레벨)

	SetBlindSelectSkill();		/// 2지선다 스킬 중 선택되지 않은 스킬에 대한 블라인드 설정

	/// 모든 스킬 슬롯 갱신
	for(map<CX2SkillTree::SKILL_ID, SkillSlotUI>::iterator it =	m_mapSkillSlotUISet.begin(); it != m_mapSkillSlotUISet.end(); ++it)
	{
		SkillSlotUI& refSkillSlotUI = it->second;
		if ( refSkillSlotUI.m_pSlot != NULL )
		{
			if(refSkillSlotUI.m_pSlot->GetSlotData() != NULL)
			{
				refSkillSlotUI.m_pSlot->GetSlotData()->m_bDataUpdated = true;		
			}		 
		}
	}

	/// 현재 스킬창에서 스킬 레벨을 올리고 있는 상태일 때만 버튼 활성
	CKTDGUIButton* pButtonDecide = static_cast<CKTDGUIButton*>( m_pDLGUISkillTree->GetControl( L"Decide" ) );
	CKTDGUIButton* pButtonCancel = static_cast<CKTDGUIButton*>( m_pDLGUISkillTree->GetControl( L"Cancel" ) );

	if( NULL != pButtonDecide && NULL != pButtonCancel )
	{
		pButtonDecide->SetEnable( m_bNowLearnSkill );
		pButtonCancel->SetEnable( m_bNowLearnSkill );
	}

#ifdef SKILL_PAGE_SYSTEM //JHKang
	CKTDGUIButton* pButtonInitializeSkill = static_cast<CKTDGUIButton*>( m_pDLGUISkillTree->GetControl( L"InitializeSkill" ) );
	if ( pButtonInitializeSkill != NULL )
	{
		CX2UserSkillTree& refUserSkillTree = g_pData->GetMyUser()->GetSelectUnit()->AccessUnitData().m_UserSkillTree;
		
		if ( refUserSkillTree.IsUsingPage() )
			pButtonInitializeSkill->SetEnable( true );
		else
			pButtonInitializeSkill->SetEnable( false );
	}
#endif //SKILL_PAGE_SYSTEM

	UpdateBlind();
}

void CX2UISkillTreeNew::UpdateSkillTree( CX2SkillTree::SKILL_ID eSkillID )
{
	map<CX2SkillTree::SKILL_ID, SkillSlotUI>::iterator it =	m_mapSkillSlotUISet.find(eSkillID);
	if( it != m_mapSkillSlotUISet.end() )
	{
		SkillSlotUI& refSkillSlotUI = it->second;
		if ( refSkillSlotUI.m_pSlot != NULL )
		{
			refSkillSlotUI.m_pSlot->GetSlotData()->m_bDataUpdated = true;			
		}
		const CX2SkillTree::SkillTreeTemplet* pSkillTreeTemplet = g_pData->GetSkillTree()->GetSkillTreeTemplet((int)g_pData->GetMyUser()->GetSelectUnit()->GetClass(), eSkillID);
		if(pSkillTreeTemplet != NULL)
		{
#ifdef UPGRADE_SKILL_SYSTEM_2013 // 김태환 - 스킬 시스템 변경
			CX2SkillTree::SKILL_ID eFollowingSkillID = static_cast<CX2SkillTree::SKILL_ID>( pSkillTreeTemplet->m_iFollowingSkill );
			map<CX2SkillTree::SKILL_ID, SkillSlotUI>::iterator itfol =	m_mapSkillSlotUISet.find(eFollowingSkillID);
			if( itfol != m_mapSkillSlotUISet.end())
			{
				SkillSlotUI& refFollowingSkillSlotUI = itfol->second;
				if ( refFollowingSkillSlotUI.m_pSlot != NULL )
				{
					refFollowingSkillSlotUI.m_pSlot->GetSlotData()->m_bDataUpdated = true;			
				}

			}
#else // UPGRADE_SKILL_SYSTEM_2013
			for(vector<int>::const_iterator it = pSkillTreeTemplet->m_vecFollowingSkill.begin(); it != pSkillTreeTemplet->m_vecFollowingSkill.end(); ++it)
			{
				CX2SkillTree::SKILL_ID eFollowingSkillID = (CX2SkillTree::SKILL_ID) *it;
				map<CX2SkillTree::SKILL_ID, SkillSlotUI>::iterator itfol =	m_mapSkillSlotUISet.find(eFollowingSkillID);
				if( itfol != m_mapSkillSlotUISet.end())
				{
					SkillSlotUI& refFollowingSkillSlotUI = itfol->second;
					if ( refFollowingSkillSlotUI.m_pSlot != NULL )
					{
						refFollowingSkillSlotUI.m_pSlot->GetSlotData()->m_bDataUpdated = true;			
					}

				}

			}
#endif // UPGRADE_SKILL_SYSTEM_2013
		}

	}
	UpdateBlind();

}

void CX2UISkillTreeNew::UpdateBlind()
{
	CKTDGUIContolList*	pControlList = static_cast<CKTDGUIContolList*>( m_pDLGUISkillTree->GetControl( L"SkillTreeControlList" ) );
	CKTDGUIStatic*		pStaticBlind = (CKTDGUIStatic*) m_pDLGUISkillTree->GetControl( L"g_pStaticBlind" );
#ifdef SKILL_PAGE_SYSTEM //JHKang
	bool bUsingPage = false;
#endif //SKILL_PAGE_SYSTEM
	
	if( NULL != pStaticBlind &&
		NULL != pControlList && 
		NULL != g_pData &&
		NULL != g_pData->GetMyUser() &&
		NULL != g_pData->GetMyUser()->GetSelectUnit() )
	{

#ifdef SKILL_PAGE_SYSTEM //JHKang
		USHORT usUsingPage = g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_UserSkillTree.GetUsingPage();
		USHORT usSelectPage = g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_UserSkillTree.GetSelectPage();

		if ( usUsingPage == usSelectPage - 1 )
			bUsingPage = true;

		if ( usSelectPage > 0 )
			usUsingPage = usSelectPage - 1;
#endif //SKILL_PAGE_SYSTEM

#if defined( _IN_HOUSE_ ) || defined( _IN_HOUSE_SERVICE_READY_QA_ ) || defined( _OPEN_TEST_ ) || defined( _OPEN_TEST_2_ )
		/// 개발자 기능 - 스킬 모두 오픈해야 할 때, 블라인드 처리 해제

		int iLearnSkillNum	= 0;		/// 현재 배운 스킬 수
		int	iiLimitSkillnum	= 0;		/// 블라인드 해제 조건 스킬 수

		/// 개발자 등급에선 블라인드 처리 해제
		if( true == g_pMain->IsMyAuthLevelHigherThan( CX2User::XUAL_DEV ) )
		{
			const int iClassLevel = g_pData->GetMyUser()->GetSelectUnit()->GetClassLevel();	/// 전직 등급

			switch ( iClassLevel )
			{
			case 0:		/// 기본 전직일 때 스킬 10개 찍으면, 모두 오픈
				iiLimitSkillnum = 10;
				break;
			case 1:		/// 1차 전직일 때 스킬 20개 찍으면, 모두 오픈
				iiLimitSkillnum = 20;
				break;
			default:	/// 2차 전직일 때 스킬 30개 찍으면, 모두 오픈
				iiLimitSkillnum = 30;
			}

			/// 현재 배운 스킬수 반환
		#ifdef SKILL_PAGE_SYSTEM //JHKang
			iLearnSkillNum = static_cast<int>( g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_UserSkillTree
				.GetMapSkillAcquired( usUsingPage ).size() );
		#else //SKILL_PAGE_SYSTEM
			iLearnSkillNum = static_cast<int>( g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_UserSkillTree.GetMapSkillAcquired().size() );
		#endif //SKILL_PAGE_SYSTEM
		}
#endif // _SERVICE_
		int							iTopTier				= pControlList->GetIndexY();											/// 현재 스킬창의 스크롤 바 위치 인덱스
		int							iUnitLevel				= g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_Level;	/// 유닛 레벨
		CX2Unit::UNIT_CLASS_TYPE	eUnitClassType			= static_cast<CX2Unit::UNIT_CLASS_TYPE>( g_pData->GetMyUser()->GetSelectUnit()->GetClassLevel() );	/// 유닛 클레스 타입
		const int					iNotLearnTierIndex		= GetNotLearnTierIndex();											/// 배우지 못한 최상위 스킬 라인 반환
		const int					iEnoughLevelTierIndex	= GetNotEnoughLevelTierIndex( iUnitLevel );							/// 배울 수 없는 레벨의 최상위 스킬 라인 반환


		for( int i = 0; i < pControlList->GetViewSizeY(); ++i )
		{
			if ( NULL == pStaticBlind->GetPicture( i ) ||
				 NULL == pStaticBlind->GetPicture( 5+ i ) ||
				 NULL == pStaticBlind->GetString( i ) )
				continue;

			const float fOffSet = static_cast<float>( 92.f * i );

			/// 느낌표 텍스처 위치 조정
			const float fPictureXPos = pStaticBlind->GetPicture( 5 + i )->GetPos().x;
			pStaticBlind->GetPicture( 5 + i )->SetPos( D3DXVECTOR2( fPictureXPos, 120.f + fOffSet ) );

			/// 안내 스트링 위치 조정
			pStaticBlind->GetString( i )->pos.y = 121.f + fOffSet;


			const int	iNowSkillLineLevel	= iTopTier + i;		/// 해당 스킬 라인 레벨
			bool		bButtonBlind		= false;			/// 해당 스킬 라인의 스킬 습득 버튼 숨김 여부

			bool		bEnoughLevel		= false;			/// 요구 습득 레벨 만족 여부
			bool		bLearnBeforeSkill	= false;			/// 이전 스킬 습득 여부

			/// 현재 스킬이 기본 스킬이거나, 현재 유닛 레벨이 해당 스킬 라인 오픈 요구 레벨을 만족할 때
			if ( 0 == GetRequireSkillLineLevel( iNowSkillLineLevel ) || 
				 iUnitLevel >= GetRequireSkillLineLevel( iNowSkillLineLevel ) )
			{
				bEnoughLevel = true;
			}

			/// 레벨도 부족하면, 레벨 부족 표시
			if ( false == bEnoughLevel )
			{
				/// 느낌표 텍스처 활성
				pStaticBlind->GetPicture( 5 + i )->SetShow( true );

				pStaticBlind->GetString( i )->msg = GET_REPLACED_STRING( ( STR_ID_25172, "i", GetRequireSkillLineLevel( iNowSkillLineLevel ) ) );
				pStaticBlind->GetString( i )->fontIndex = XUF_DODUM_11_NORMAL;

				bButtonBlind = true;		/// 버튼 블라인드
			}

			/// 블라인드 조건이 아니라면, 블라인드 해제
			if ( false == bButtonBlind )
			{
				pStaticBlind->GetPicture( 5 + i )->SetShow( false );

				pStaticBlind->GetString( i )->msg = L"";
			}

			/// 블라인드 여부에 따라서 블라인드 텍스처 표시 설정
#ifdef SKILL_PAGE_SYSTEM //JHKang
			if ( bUsingPage == true )
				pStaticBlind->GetPicture(i)->SetShow( bButtonBlind );
			else
			{
				pStaticBlind->GetPicture(i)->SetShow( true );
				pStaticBlind->GetPicture( 5 )->SetShow( true );
				pStaticBlind->GetString( 0 )->fontIndex = XUF_DODUM_13_SEMIBOLD;
				pStaticBlind->GetString( 0 )->msg = GET_STRING( STR_ID_29929 );
			}
#else //SKILL_PAGE_SYSTEM
			pStaticBlind->GetPicture(i)->SetShow( bButtonBlind );
#endif //SKILL_PAGE_SYSTEM

#if defined( _IN_HOUSE_ ) || defined( _IN_HOUSE_SERVICE_READY_QA_ ) || defined( _OPEN_TEST_ ) || defined( _OPEN_TEST_2_ )
			/// 개발자 기능 - 스킬 모두 오픈해야 할 때, 블라인드 처리 해제
			if( true == g_pMain->IsMyAuthLevelHigherThan( CX2User::XUAL_DEV ) )
			{
#ifdef SKILL_PAGE_SYSTEM //JHKang
				if ( iiLimitSkillnum <= iLearnSkillNum && bUsingPage == true )
#else //SKILL_PAGE_SYSTEM
				if ( iiLimitSkillnum <= iLearnSkillNum )
#endif //SKILL_PAGE_SYSTEM
				{
					pStaticBlind->GetPicture(i)->SetShow( false );	/// 블라인드 비활성
					pStaticBlind->GetPicture( 5 + i )->SetShow( false );
					pStaticBlind->GetString( i )->msg = L"";

					bButtonBlind = false;
				}
			}
#endif // _SERVICE_

			/// 해당 라인의 스킬을 검사하여, 버튼 활성 여부 설정
			for ( int index = 0; index < 3; ++index )
			{
				/// 위치에 해당하는 스킬 아이디 반환
				std::map<SkillPositionKey, CX2SkillTree::SKILL_ID>::iterator it = m_mapSkillPosition.find( SkillPositionKey( index, iNowSkillLineLevel ) );

				if ( it != m_mapSkillPosition.end() )
				{
					/// 스킬 아이디에 해당하는 UI Set 반환
					map<CX2SkillTree::SKILL_ID, CX2UISkillTreeNew::SkillSlotUI>::iterator itUISet = m_mapSkillSlotUISet.find( it->second );

					if ( itUISet != m_mapSkillSlotUISet.end() )		/// 버튼 활성 여부 설정
						itUISet->second.SetSkillLineBlind( bButtonBlind );
				}
			}
		}

		m_pDLGUISkillTree->ChangeSequence( pStaticBlind, true );
	}
}

void CX2UISkillTreeNew::UpdateSPInfo()
{
	/// 스킬 포인트 정보 스트링 설정
	CKTDGUIStatic* pNowSPString = static_cast<CKTDGUIStatic*>( m_pDLGUISkillTree->GetControl( L"g_pStaticSPBox" ) );

	if( NULL != pNowSPString && 
		NULL != pNowSPString->GetString(0) &&
		NULL != g_pData &&
		NULL != g_pData->GetMyUser() &&
		NULL != g_pData->GetMyUser()->GetSelectUnit() )
	{
		const CX2Unit::UnitData*	pUnitData	= &g_pData->GetMyUser()->GetSelectUnit()->GetUnitData();
		CKTDGUIButton*		pButton		= (CKTDGUIButton*) m_pDLGUISkillTree->GetControl( L"Button_Notice_Notice_SP" );

		if ( NULL != pUnitData && NULL != pButton )
		{
			const int iHaveSPAndCSP	= pUnitData->m_iSPoint + pUnitData->m_iCSPoint;		/// 남은 스킬 포인트 + 남은 캐시 스킬 포인트

			WCHAR	buf[255]	= {0,};
			/// 총 스킬 포인트 색상 연산

			/// 캐시 스킬 포인트( 그노시스의 축복 )가 적용중인지 검사
			CTime cTime;
			KncUtil::ConvertStringToCTime( g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_wstrCSPointEndDate, cTime );
			CTime tCurrentTime = g_pData->GetServerCurrentTime();

			bool bUsingCSP = tCurrentTime >= cTime ? false : true;

			wstring wstrUsedSkillString = L"";

			if (true == bUsingCSP )
			{
				StringCchPrintf(buf, 256, L"#CEC008C%d#CX", iHaveSPAndCSP );	/// 캐시 스킬 포인트가 적용 중 이라면, 붉은색으로 표기
				pButton->SetShowEnable(true, true);								/// 그노시스 사용중 팝업 표시
			}
			else
			{
				StringCchPrintf(buf, 256, L"%d", iHaveSPAndCSP );				/// 캐시 스킬 포인트가 적용즁이지 않으면, 기본 색으로 표기 ( 노랑색 )
				pButton->SetShowEnable(false, false);							/// 그노시스 사용중 팝업 해제
			}

			wstrUsedSkillString = buf;


#ifdef SKILL_PAGE_SYSTEM //JHKang
			if ( true == m_bNowLearnSkill && pUnitData->m_UserSkillTree.IsUsingPage() )		/// 스킬 레벨 올리는 중 이라면
#else //SKILL_PAGE_SYSTEM
			if ( true == m_bNowLearnSkill )		/// 스킬 레벨 올리는 중 이라면
#endif //SKILL_PAGE_SYSTEM
			{
				if ( NULL != g_pData->GetSkillTree() )
				{
					const int iUsedSPoint	= g_pData->GetSkillTree()->GetUsedSPoint();			/// 사용한 스킬 포인트
					const int iUsedCSPoint	= g_pData->GetSkillTree()->GetUsedCSPoint();		/// 사용한 캐시 스킬 포인트
					const int iUsedSPAndCSP	= iHaveSPAndCSP - ( iUsedSPoint + iUsedCSPoint );	/// 남은 스킬 포인트

					StringCchPrintf( buf, 256, L"#CFF0000%d#CX / %s", iUsedSPAndCSP, wstrUsedSkillString.c_str() );		/// 남은 포인트 ( 붉은색 ) / 총 스킬 포인트

					wstrUsedSkillString = buf;
				}
			}

			pNowSPString->GetString(0)->msg = wstrUsedSkillString;
		}
	}
}

void CX2UISkillTreeNew::CreateSlotUIPreset(SkillSlotUI& UISet, int eSkillID, int iCol )
{
	if( m_pDLGUISkillTree == NULL )
		return;

	/// 슬롯 설정 --------------------------------------------------------------------------------------------------------------------
 	m_pDLGUISkillTree->OpenScriptFile( L"DLG_UI_Skill_Icon_New.lua" );

	UISet.m_pSlot = static_cast<CKTDGUISlot*>( m_pDLGUISkillTree->GetControl( L"g_pSlot_SkillTree" ) );		/// 슬롯 설정

	SKILL_TREE_SLOT_TYPE eSkillTreeSlotType = static_cast<SKILL_TREE_SLOT_TYPE>( iCol );

	WCHAR buf[255] = {0,};
	
	if ( NULL != UISet.m_pSlot )
	{
		StringCchPrintf( buf, 255, L"g_pSlot_SkillTree_%d", eSkillID );
		UISet.m_pSlot->SetName(buf);

		if ( STST_SELECT_SKILL_LEFT == iCol )
		{
			CKTDGUIControl::UIPointData* pPoint		= new CKTDGUIControl::UIPointData();
			pPoint->fChangeTime						= 0.0f;
			pPoint->addSize							= D3DXVECTOR2( 46.f, 46.f );
			pPoint->color							= D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f );
			pPoint->leftTopPoint					= D3DXVECTOR2( 119.f, 16.f );

			CKTDGUIControl::UIPointData* pOverPoint = new CKTDGUIControl::UIPointData();
			pOverPoint->fChangeTime					= 0.0f;
			pOverPoint->color						= D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f );
			pOverPoint->leftTopPoint				= D3DXVECTOR2( 112.f, 9.f );

			UISet.m_pSlot->SetPoint( pPoint );
			UISet.m_pSlot->SetMouseOverPoint( pOverPoint );
		}
		if ( STST_SELECT_SKILL_RIGHT == iCol )		/// 2지선다 우측 스킬이면, 위치 이동
		{
			CKTDGUIControl::UIPointData* pPoint		= new CKTDGUIControl::UIPointData();
			pPoint->fChangeTime						= 0.0f;
			pPoint->addSize							= D3DXVECTOR2( 46.f, 46.f );
			pPoint->color							= D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f );
			pPoint->leftTopPoint					= D3DXVECTOR2( 89.f, 16.f );

			CKTDGUIControl::UIPointData* pOverPoint = new CKTDGUIControl::UIPointData();
			pOverPoint->fChangeTime					= 0.0f;
			pOverPoint->color						= D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f );
			pOverPoint->leftTopPoint				= D3DXVECTOR2( 82.f, 9.f );

			UISet.m_pSlot->SetPoint( pPoint );
			UISet.m_pSlot->SetMouseOverPoint( pOverPoint );
		}
	}

	/// 배경 백판 --------------------------------------------------------------------------------------------------------------------
	UISet.m_pStaticBG = static_cast<CKTDGUIStatic*>( m_pDLGUISkillTree->GetControl( L"g_pStatic_BackGround" ) );

	if( UISet.m_pStaticBG != NULL && 
		UISet.m_pStaticBG->GetPicture( 0 ) != NULL && 
		UISet.m_pStaticBG->GetPicture( 1 ) != NULL &&
		UISet.m_pStaticBG->GetPicture( 2 ) != NULL)
	{
		StringCchPrintf( buf, 256, L"g_pStatic_BackGround_%d", eSkillID );
		UISet.m_pStaticBG->SetName( buf );

		if ( STST_SELECT_SKILL_LEFT == iCol )			/// 2지선다 좌측 스킬
		{
			UISet.m_pStaticBG->GetPicture( 0 )->SetShow( false );
			UISet.m_pStaticBG->GetPicture( 1 )->SetPos( D3DXVECTOR2( 97.f, 8.f ) );
			UISet.m_pStaticBG->GetPicture( 2 )->SetShow( true );
		}
		else if ( STST_SELECT_SKILL_RIGHT == iCol )		/// 2지선다 우측 스킬
		{
			UISet.m_pStaticBG->GetPicture( 0 )->SetShow( false );
			UISet.m_pStaticBG->GetPicture( 1 )->SetShow( false );
			UISet.m_pStaticBG->GetPicture( 2 )->SetShow( false );
		}
		else
		{
			UISet.m_pStaticBG->GetPicture( 1 )->SetShow( false );
			UISet.m_pStaticBG->GetPicture( 2 )->SetShow( false );
		}

		/// 배경 백판은 맨뒤로 가세요
		m_pDLGUISkillTree->ChangeSequence( UISet.m_pStaticBG, false );
	}

	/// 스킬 백판 설정 ---------------------------------------------------------------------------------------------------------------
	UISet.m_pStaticSkillBG	= static_cast<CKTDGUIStatic*>( m_pDLGUISkillTree->GetControl( L"g_pStatic_SkillBackGround" ) );

	if ( NULL != UISet.m_pStaticSkillBG && NULL != UISet.m_pStaticSkillBG->GetPicture(0) && NULL != UISet.m_pStaticSkillBG->GetPicture(1) )
	{
		StringCchPrintf( buf, 255, L"g_pStaticSkillBackGround_%d", eSkillID );
		UISet.m_pStaticSkillBG->SetName( buf );

		const CX2SkillTree::SkillTemplet* pSkillTemplet = g_pData->GetSkillTree()->GetSkillTemplet( static_cast<CX2SkillTree::SKILL_ID>( eSkillID ) );
		if(pSkillTemplet != NULL)
		{
			wstring wstrSkillBGKeyName = L"";	/// 스킬 백판 텍스처 키값

			switch(pSkillTemplet->m_eType)		/// 스킬 타입에 따른 텍스쳐 변경
			{
			case CX2SkillTree::ST_ACTIVE:
		#ifdef ADDED_RELATIONSHIP_SYSTEM
			case CX2SkillTree::ST_RELATIONSHIP_SKILL:
		#endif // ADDED_RELATIONSHIP_SYSTEM
				{
					wstrSkillBGKeyName = L"Skill_BG_Active";
				} break;
			case CX2SkillTree::ST_SPECIAL_ACTIVE:
#ifdef FINALITY_SKILL_SYSTEM //JHKang
			case CX2SkillTree::ST_HYPER_ACTIVE_SKILL:
#endif //FINALITY_SKILL_SYSTEM
				{
					wstrSkillBGKeyName = L"Skill_BG_Active_Special";
				} break;
			case CX2SkillTree::ST_BUFF:
				{
					wstrSkillBGKeyName = L"Skill_BG_Active_Buff";
				} break;
			default:
				{
					wstrSkillBGKeyName = L"Skill_BG_Passive";
				} break;
			}

			UISet.m_pStaticSkillBG->GetPicture( 0 )->SetTex( L"DLG_UI_Common_Texture10.TGA", wstrSkillBGKeyName.c_str() );	/// 변경된 텍스쳐 설정
		}

		if ( STST_SELECT_SKILL_LEFT == iCol )			/// 2지선다 좌측 스킬이면, 위치 이동
			UISet.m_pStaticSkillBG->GetPicture( 0 )->SetPos( D3DXVECTOR2( 116.f, 13.f ) );
		else if ( STST_SELECT_SKILL_RIGHT == iCol )		/// 2지선다 우측 스킬이면, 위치 이동
			UISet.m_pStaticSkillBG->GetPicture( 0 )->SetPos( D3DXVECTOR2( 86.f, 13.f ) );

		/// 요구 SP 백판 위치 지정
		D3DXVECTOR2 vecSkillBGPos = UISet.m_pStaticSkillBG->GetPicture( 0 )->GetPos();
		UISet.m_pStaticSkillBG->GetPicture( 1 )->SetPos( D3DXVECTOR2( vecSkillBGPos.x + 21.f, vecSkillBGPos.y + 52.f ) );
	}

	/// 스킬 습득 버튼 설정 ----------------------------------------------------------------------------------------------------------
	UISet.m_pButtonSkillLearn =	static_cast<CKTDGUIButton*>( m_pDLGUISkillTree->GetControl( L"g_pButton_SkillLearn" ) );

	if( UISet.m_pButtonSkillLearn != NULL )
	{
		StringCchPrintf( buf, 256, L"ButtonSkillLearn_%d", eSkillID );
		UISet.m_pButtonSkillLearn->SetName( buf );
		UISet.m_pButtonSkillLearn->AddDummyInt( static_cast<int>( eSkillID ) );			/// 버튼에 스킬 아이디 설정


		if ( STST_SELECT_SKILL_LEFT == iCol )		/// 2지선다 좌측 스킬이면, 위치 이동
		{
			CKTDGUIControl::UIPointData* pNormalPoint = new CKTDGUIControl::UIPointData();
			pNormalPoint->fChangeTime	= 0.0f;
			pNormalPoint->leftTopPoint	= D3DXVECTOR2( 118.f, 65.f );
			pNormalPoint->color			= D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f );

			CKTDGUIControl::UIPointData* pOverPoint = new CKTDGUIControl::UIPointData();
			pOverPoint->fChangeTime		= 0.0f;
			pOverPoint->leftTopPoint	= D3DXVECTOR2( 116.f, 63.f );
			pOverPoint->color			= D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f );

			CKTDGUIControl::UIPointData* pDownPoint = new CKTDGUIControl::UIPointData();
			pDownPoint->fChangeTime		= 0.0f;
			pDownPoint->addSize			= D3DXVECTOR2( -2.f, -2.f );
			pDownPoint->leftTopPoint	= D3DXVECTOR2( 117.f, 64.f );
			pDownPoint->color			= D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f );

			CKTDGUIControl::UIPointData* pDisablePoint = new CKTDGUIControl::UIPointData();
			pDisablePoint->fChangeTime	= 0.0f;
			pDisablePoint->leftTopPoint	= D3DXVECTOR2( 118.f, 65.f );
			pDisablePoint->color		= D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f );

			UISet.m_pButtonSkillLearn->SetNormalPoint( pNormalPoint );
			UISet.m_pButtonSkillLearn->SetOverPoint( pOverPoint );
			UISet.m_pButtonSkillLearn->SetDownPoint( pDownPoint );
			UISet.m_pButtonSkillLearn->SetDisablePoint( pDisablePoint );
			UISet.m_pButtonSkillLearn->SetGuideDescAbsolutePos( pNormalPoint->leftTopPoint );
		}
		else if ( STST_SELECT_SKILL_RIGHT == iCol )		/// 2지선다 우측 스킬이면, 위치 이동
		{
			CKTDGUIControl::UIPointData* pNormalPoint = new CKTDGUIControl::UIPointData();
			pNormalPoint->fChangeTime	= 0.0f;
			pNormalPoint->leftTopPoint	= D3DXVECTOR2( 89.f, 65.f );
			pNormalPoint->color			= D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f );

			CKTDGUIControl::UIPointData* pOverPoint = new CKTDGUIControl::UIPointData();
			pOverPoint->fChangeTime		= 0.0f;
			pOverPoint->leftTopPoint	= D3DXVECTOR2( 87.f, 63.f );
			pOverPoint->color			= D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f );

			CKTDGUIControl::UIPointData* pDownPoint = new CKTDGUIControl::UIPointData();
			pDownPoint->fChangeTime		= 0.0f;
			pDownPoint->addSize			= D3DXVECTOR2( -2.f, -2.f );
			pDownPoint->leftTopPoint	= D3DXVECTOR2( 88.f, 64.f );
			pDownPoint->color			= D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f );

			CKTDGUIControl::UIPointData* pDisablePoint = new CKTDGUIControl::UIPointData();
			pDisablePoint->fChangeTime	= 0.0f;
			pDisablePoint->leftTopPoint	= D3DXVECTOR2( 89.f, 65.f );
			pDisablePoint->color		= D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f );

			UISet.m_pButtonSkillLearn->SetNormalPoint( pNormalPoint );
			UISet.m_pButtonSkillLearn->SetOverPoint( pOverPoint );
			UISet.m_pButtonSkillLearn->SetDownPoint( pDownPoint );
			UISet.m_pButtonSkillLearn->SetDisablePoint( pDisablePoint );
			UISet.m_pButtonSkillLearn->SetGuideDescAbsolutePos( pNormalPoint->leftTopPoint );
		}
	}

	/// 스킬 툴팁용 버튼 설정 --------------------------------------------------------------------------------------------------------
	UISet.m_pButtonSkillDesc =	static_cast<CKTDGUIButton*>( m_pDLGUISkillTree->GetControl( L"g_pButton_SkillDesc" ) );

	if( UISet.m_pButtonSkillDesc != NULL )
	{
		StringCchPrintf( buf, 256, L"ButtonSkillDesc_%d", eSkillID );
		UISet.m_pButtonSkillDesc->SetName( buf );


		if ( STST_SELECT_SKILL_LEFT == iCol )		/// 2지선다 좌측 스킬이면, 위치 이동
		{
			CKTDGUIControl::UIPointData* pNormalPoint = new CKTDGUIControl::UIPointData();
			pNormalPoint->fChangeTime	= 0.0f;
			pNormalPoint->leftTopPoint	= D3DXVECTOR2( 118.f, 65.f );
			pNormalPoint->color			= D3DXCOLOR( 1.0f, 1.0f, 1.0f, 0.0f );

			CKTDGUIControl::UIPointData* pOverPoint = new CKTDGUIControl::UIPointData();
			pOverPoint->fChangeTime		= 0.0f;
			pOverPoint->leftTopPoint	= D3DXVECTOR2( 116.f, 63.f );
			pOverPoint->color			= D3DXCOLOR( 1.0f, 1.0f, 1.0f, 0.0f );

			CKTDGUIControl::UIPointData* pDownPoint = new CKTDGUIControl::UIPointData();
			pDownPoint->fChangeTime		= 0.0f;
			pDownPoint->addSize			= D3DXVECTOR2( -2.f, -2.f );
			pDownPoint->leftTopPoint	= D3DXVECTOR2( 117.f, 64.f );
			pDownPoint->color			= D3DXCOLOR( 1.0f, 1.0f, 1.0f, 0.0f );

			CKTDGUIControl::UIPointData* pDisablePoint = new CKTDGUIControl::UIPointData();
			pDisablePoint->fChangeTime	= 0.0f;
			pDisablePoint->leftTopPoint	= D3DXVECTOR2( 118.f, 65.f );
			pDisablePoint->color		= D3DXCOLOR( 1.0f, 1.0f, 1.0f, 0.0f );

			UISet.m_pButtonSkillDesc->SetNormalPoint( pNormalPoint );
			UISet.m_pButtonSkillDesc->SetOverPoint( pOverPoint );
			UISet.m_pButtonSkillDesc->SetDownPoint( pDownPoint );
			UISet.m_pButtonSkillDesc->SetDisablePoint( pDisablePoint );
			UISet.m_pButtonSkillDesc->SetGuideDescAbsolutePos( pNormalPoint->leftTopPoint );
		}
		else if ( STST_SELECT_SKILL_RIGHT == iCol )		/// 2지선다 우측 스킬이면, 위치 이동
		{
			CKTDGUIControl::UIPointData* pNormalPoint = new CKTDGUIControl::UIPointData();
			pNormalPoint->fChangeTime	= 0.0f;
			pNormalPoint->leftTopPoint	= D3DXVECTOR2( 89.f, 65.f );
			pNormalPoint->color			= D3DXCOLOR( 1.0f, 1.0f, 1.0f, 0.0f );

			CKTDGUIControl::UIPointData* pOverPoint = new CKTDGUIControl::UIPointData();
			pOverPoint->fChangeTime		= 0.0f;
			pOverPoint->leftTopPoint	= D3DXVECTOR2( 87.f, 63.f );
			pOverPoint->color			= D3DXCOLOR( 1.0f, 1.0f, 1.0f, 0.0f );

			CKTDGUIControl::UIPointData* pDownPoint = new CKTDGUIControl::UIPointData();
			pDownPoint->fChangeTime		= 0.0f;
			pDownPoint->addSize			= D3DXVECTOR2( -2.f, -2.f );
			pDownPoint->leftTopPoint	= D3DXVECTOR2( 88.f, 64.f );
			pDownPoint->color			= D3DXCOLOR( 1.0f, 1.0f, 1.0f, 0.0f );

			CKTDGUIControl::UIPointData* pDisablePoint = new CKTDGUIControl::UIPointData();
			pDisablePoint->fChangeTime	= 0.0f;
			pDisablePoint->leftTopPoint	= D3DXVECTOR2( 89.f, 65.f );
			pDisablePoint->color		= D3DXCOLOR( 1.0f, 1.0f, 1.0f, 0.0f );

			UISet.m_pButtonSkillDesc->SetNormalPoint( pNormalPoint );
			UISet.m_pButtonSkillDesc->SetOverPoint( pOverPoint );
			UISet.m_pButtonSkillDesc->SetDownPoint( pDownPoint );
			UISet.m_pButtonSkillDesc->SetDisablePoint( pDisablePoint );
			UISet.m_pButtonSkillDesc->SetGuideDescAbsolutePos( pNormalPoint->leftTopPoint );
		}
		else
		{
			CKTDGUIControl::UIPointData* pNormalPoint = UISet.m_pButtonSkillDesc->GetNormalPoint();

			if ( NULL != pNormalPoint )
			{
				UISet.m_pButtonSkillDesc->SetGuideDescAbsolutePos( pNormalPoint->leftTopPoint );
			}
		}

		UISet.m_pButtonSkillDesc->SetGuideDesc(L"");
	}

	/// 스킬 초기화 버튼 설정 --------------------------------------------------------------------------------------------------------
	UISet.m_pButtonSkillInit =	static_cast<CKTDGUIButton*>( m_pDLGUISkillTree->GetControl( L"g_pButton_SkillInit" ) );

	if( UISet.m_pButtonSkillInit != NULL )
	{
		StringCchPrintf( buf, 256, L"ButtonSkillInit_%d", eSkillID );
		UISet.m_pButtonSkillInit->SetName( buf );
		UISet.m_pButtonSkillInit->AddDummyInt( static_cast<int>( eSkillID ) );			/// 버튼에 스킬 아이디 설정


		if ( STST_SELECT_SKILL_LEFT == iCol )		/// 2지선다 좌측 스킬이면, 위치 이동
		{
			CKTDGUIControl::UIPointData* pNormalPoint = new CKTDGUIControl::UIPointData();
			pNormalPoint->fChangeTime	= 0.0f;
			pNormalPoint->leftTopPoint	= D3DXVECTOR2( 119.f, 65.f );
			pNormalPoint->color			= D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f );

			CKTDGUIControl::UIPointData* pOverPoint = new CKTDGUIControl::UIPointData();
			pOverPoint->fChangeTime		= 0.0f;
			pOverPoint->leftTopPoint	= D3DXVECTOR2( 119.f, 65.f );
			pOverPoint->color			= D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f );

			CKTDGUIControl::UIPointData* pDownPoint = new CKTDGUIControl::UIPointData();
			pDownPoint->fChangeTime		= 0.0f;
			pDownPoint->addSize			= D3DXVECTOR2( -2.f, -2.f );
			pDownPoint->leftTopPoint	= D3DXVECTOR2( 120.f, 66.f );
			pDownPoint->color			= D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f );

			CKTDGUIControl::UIPointData* pDisablePoint = new CKTDGUIControl::UIPointData();
			pDisablePoint->fChangeTime	= 0.0f;
			pDisablePoint->leftTopPoint	= D3DXVECTOR2( 119.f, 65.f );
			pDisablePoint->color		= D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f );

			UISet.m_pButtonSkillInit->SetNormalPoint( pNormalPoint );
			UISet.m_pButtonSkillInit->SetOverPoint( pOverPoint );
			UISet.m_pButtonSkillInit->SetDownPoint( pDownPoint );
			UISet.m_pButtonSkillInit->SetDisablePoint( pDisablePoint );
			UISet.m_pButtonSkillInit->SetGuideDescAbsolutePos( pNormalPoint->leftTopPoint );
		}
		else if ( STST_SELECT_SKILL_RIGHT == iCol )		/// 2지선다 우측 스킬이면, 위치 이동
		{
			CKTDGUIControl::UIPointData* pNormalPoint = new CKTDGUIControl::UIPointData();
			pNormalPoint->fChangeTime	= 0.0f;
			pNormalPoint->leftTopPoint	= D3DXVECTOR2( 89.f, 65.f );
			pNormalPoint->color			= D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f );

			CKTDGUIControl::UIPointData* pOverPoint = new CKTDGUIControl::UIPointData();
			pOverPoint->fChangeTime		= 0.0f;
			pOverPoint->leftTopPoint	= D3DXVECTOR2( 89.f, 65.f );
			pOverPoint->color			= D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f );

			CKTDGUIControl::UIPointData* pDownPoint = new CKTDGUIControl::UIPointData();
			pDownPoint->fChangeTime		= 0.0f;
			pDownPoint->addSize			= D3DXVECTOR2( -2.f, -2.f );
			pDownPoint->leftTopPoint	= D3DXVECTOR2( 90.f, 66.f );
			pDownPoint->color			= D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f );

			CKTDGUIControl::UIPointData* pDisablePoint = new CKTDGUIControl::UIPointData();
			pDisablePoint->fChangeTime	= 0.0f;
			pDisablePoint->leftTopPoint	= D3DXVECTOR2( 89.f, 65.f );
			pDisablePoint->color		= D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f );

			UISet.m_pButtonSkillInit->SetNormalPoint( pNormalPoint );
			UISet.m_pButtonSkillInit->SetOverPoint( pOverPoint );
			UISet.m_pButtonSkillInit->SetDownPoint( pDownPoint );
			UISet.m_pButtonSkillInit->SetDisablePoint( pDisablePoint );
			UISet.m_pButtonSkillInit->SetGuideDescAbsolutePos( pNormalPoint->leftTopPoint );
		}

		UISet.m_pButtonSkillInit->SetShow( false );
	}

	/// 정보 스트링 설정 -------------------------------------------------------------------------------------------------------------
	UISet.m_pStaticStringRequireSP = static_cast<CKTDGUIStatic*>( m_pDLGUISkillTree->GetControl( L"g_pStatic_StringInfo" ) );

	if( UISet.m_pStaticStringRequireSP != NULL )
	{
		StringCchPrintf( buf, 256, L"g_pStaticStringInfo_%d", eSkillID );
		UISet.m_pStaticStringRequireSP->SetName( buf );

		if ( STST_SELECT_SKILL_LEFT == iCol )			/// 2지선다 좌측 스킬이면, 위치 이동
			UISet.m_pStaticStringRequireSP->GetString( 0 )->pos = D3DXVECTOR2( 151.f, 66.f );
		else if ( STST_SELECT_SKILL_RIGHT == iCol )		/// 2지선다 우측 스킬이면, 위치 이동
			UISet.m_pStaticStringRequireSP->GetString( 0 )->pos = D3DXVECTOR2( 121.f, 66.f );
	}

	/// 마스터 스킬 이미지 설정 ------------------------------------------------------------------------------------------------------
	UISet.m_pStaticSkillMaster = static_cast<CKTDGUIStatic*>( m_pDLGUISkillTree->GetControl( L"g_pStatic_SkillMaster" ) );

	if( UISet.m_pStaticSkillMaster != NULL )
	{
		StringCchPrintf( buf, 256, L"g_pStaticSkillMaster_%d", eSkillID );
		UISet.m_pStaticSkillMaster->SetName( buf );

		if ( STST_SELECT_SKILL_LEFT == iCol )			/// 2지선다 좌측 스킬이면, 위치 이동
			UISet.m_pStaticSkillMaster->GetPicture(0)->SetPos( D3DXVECTOR2( 119.f, 65.f ) );
		else if ( STST_SELECT_SKILL_RIGHT == iCol )		/// 2지선다 우측 스킬이면, 위치 이동
			UISet.m_pStaticSkillMaster->GetPicture(0)->SetPos( D3DXVECTOR2( 89.f, 65.f ) );

		UISet.m_pStaticSkillMaster->GetPicture( 0 )->SetShow( false );
	}

	/// 스킬 습득 불가 설정 ----------------------------------------------------------------------------------------------------------
	UISet.m_pStaticSkillNoLearn = static_cast<CKTDGUIStatic*>( m_pDLGUISkillTree->GetControl( L"g_pStatic_SkillNoLearn" ) );

	if( UISet.m_pStaticSkillNoLearn != NULL )
	{
		StringCchPrintf( buf, 256, L"g_pStatic_SkillNoLearn_%d", eSkillID );
		UISet.m_pStaticSkillNoLearn->SetName( buf );
		UISet.m_pStaticSkillNoLearn->GetPicture(0)->SetShow( false );

		if ( STST_SELECT_SKILL_LEFT == iCol )			/// 2지선다 좌측 스킬이면, 위치 이동
			UISet.m_pStaticSkillNoLearn->GetPicture(0)->SetPos( D3DXVECTOR2( 114.f, 12.f ) );
		else if ( STST_SELECT_SKILL_RIGHT == iCol )		/// 2지선다 우측 스킬이면, 위치 이동
			UISet.m_pStaticSkillNoLearn->GetPicture(0)->SetPos( D3DXVECTOR2( 84.f, 12.f ) );
	}

	/// 자물쇠 설정 ----------------------------------------------------------------------------------------------------------
	UISet.m_pStaticSkillLock = static_cast<CKTDGUIStatic*>( m_pDLGUISkillTree->GetControl( L"g_pStaticSkill_Lock" ) );

	if( UISet.m_pStaticSkillLock != NULL )
	{
		StringCchPrintf( buf, 256, L"g_pStaticSkill_Lock_%d", eSkillID );
		UISet.m_pStaticSkillLock->SetName( buf );
		UISet.m_pStaticSkillLock->SetShow( false );

		if ( STST_SELECT_SKILL_LEFT == iCol )			/// 2지선다 좌측 스킬이면, 위치 이동
			UISet.m_pStaticSkillLock->GetPicture(0)->SetPos( D3DXVECTOR2( 126.f, 19.f ) );
		else if ( STST_SELECT_SKILL_RIGHT == iCol )		/// 2지선다 우측 스킬이면, 위치 이동
			UISet.m_pStaticSkillLock->GetPicture(0)->SetPos( D3DXVECTOR2( 97.f, 19.f ) );

		/// 자물쇠 최상위에 배치
		m_pDLGUISkillTree->ChangeSequence( UISet.m_pStaticSkillLock, true );
	}

	return;
}

// CX2UISkillTreeNew::SkillSlotUI& CX2UISkillTreeNew::GetSkillSlotUI( CX2SkillTree::SKILL_ID eSkillID )
// {
// 	map<CX2SkillTree::SKILL_ID, SkillSlotUI>::iterator it = m_mapSkillSlotUISet.find(eSkillID);
// 	if(it != m_mapSkillSlotUISet.end())
// 	{
// 		return it->second;
// 	}
// 
// 	ASSERT( !"Wrong Skill ID" );
// 	return m_mapSkillSlotUISet.begin()->second;
// 
// }

// CX2UISkillTreeNew::ArrowUI& CX2UISkillTreeNew::GetArrowUI( CX2SkillTree::SKILL_ID eSkillID )
// {
// 	map<CX2SkillTree::SKILL_ID, ArrowUI>::iterator it = m_mapArrowUISet.find(eSkillID);
// 	if(it != m_mapArrowUISet.end())
// 	{
// 		return it->second;
// 	}
// 
// 	ASSERT( !"Wrong Skill ID" );
// 	return m_mapArrowUISet.begin()->second;
// }


// 잠긴 스킬에 우클릭을 하면 나타나는 다이얼로그를 열기 위한 함수.
// 아직 스킬 봉인해제 자체가 미구현이므로 기반만 닦아 둔다.
// 다이얼로그 닫는 경우까지만 만들어 뒀습니다. 구매 버튼을 눌렀을 때는 아무것도 안 하게 되어 있으니 다음 작업자 분께서는 
// UICustomMsgProc에서 case STUCM_UNSEAL_SKILL_BUY 항목에 그 부분을 작업해 주세요.
void CX2UISkillTreeNew::OpenSkillUnsealInformDLG( const CX2SkillTree::SkillTemplet* pSkillTemplet, D3DXVECTOR2 SlotCenterPos )
{	
	SAFE_DELETE_DIALOG( m_pDLGSkillUnsealInform );
		
	m_pDLGSkillUnsealInform = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_Skill_Purchase_Window.lua" );
	g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGSkillUnsealInform );
	m_pDLGSkillUnsealInform->SetDisableUnderWindow(true);

	// 다이얼로그가 열려야 할 위치(슬롯 중심에서부터의 상대값) : 스크립트에 넣어 둠. DLG_Skill_Purchase_Window.lua 최상단 참조.
	D3DXVECTOR2 OffsetPos((float)m_pDLGSkillUnsealInform->GetDummyInt(0), (float)m_pDLGSkillUnsealInform->GetDummyInt(1));	
	m_pDLGSkillUnsealInform->SetPos( SlotCenterPos + OffsetPos );

	CKTDGUIStatic* pStaticRequireItem = (CKTDGUIStatic*) m_pDLGSkillUnsealInform->GetControl( L"StaticRequireItemName" );
	CKTDGUIStatic* pStaticRequireCash = (CKTDGUIStatic*) m_pDLGSkillUnsealInform->GetControl( L"StaticPrice" );
	

//#if 0		//** 스킬 봉인해제 기능이 들어갈 때 여기에 추가작업 해 주세요. 
////{{AFX
//	if( NULL != pStaticRequireCash->GetString(0) )
//	{
//		//** 여기에 해당 스킬의 봉인해제서 가격을 써 주세요
//		int iRequireCash = pSkillTemplet->가격;
//		WCHAR buf[256] = {0,};
//		//wsprintf( buf, L"(%d%s)", iRequireCash, GET_STRING(STR_ID_34) );
//		StringCchPrintf( buf, 256, L"(%d%s)", iRequireCash, GET_STRING(STR_ID_34) );
//		pStaticRequireCash->GetString(0)->msg = buf;
//	}
//	else
//	{
//		SAFE_DELETE_DIALOG(m_pDLGSkillUnsealInform);
//		ASSERT( "Script Error." );
//		return;
//	}
//
//	if( NULL != pStaticRequireItem->GetString(0) )
//	{
//
//		//** 여기에 해당 스킬의 봉인해제서 이름을 써 주세요
//		WCHAR buf[256] = {0,};
//		wsprintf( buf, L"%s", pSkillTemplet->필요한아이템이름 );
//		StringCchPrintf( buf, 256, L"%s", pSkillTemplet->필요한아이템이름 );
//		pStaticRequireCash->GetString(0)->msg = buf;
//	}
//	else
//	{
//		SAFE_DELETE_DIALOG(m_pDLGSkillUnsealInform);
//		ASSERT( "Script Error." );
//		return;
//	}
//
//	m_pDLGSkillUnsealInform->SetShowEnable(true, true);
////}}AFX
//#endif

		
}

// SlotManager의 ItemDesc 쪽 코드를 긁어왔음
void CX2UISkillTreeNew::ShowSkillDesc( bool bShow, CX2SkillTree::SKILL_ID eSkillID /*= CX2SkillTree::SI_NONE*/, D3DXVECTOR2 pos /*= D3DXVECTOR2(0,0)*/, 
								    D3DXVECTOR2 size /*= D3DXVECTOR2(0,0)*/, bool bEquipped /*= false*/ )
{
	if(m_pDLGSkillToolTip == NULL )
	{
		/// 툴팁 다이얼로그 생성
		
#ifdef MOVIE_TEST
		m_pDLGSkillToolTip = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_UI_Skill_Desc_Video.lua", 0.07f, XDL_POP_UP );
#else	// MOVIE_TEST
		m_pDLGSkillToolTip = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_UI_Skill_Desc.lua", 0.07f, XDL_POP_UP );
#endif // MOVIE_TEST


		if ( NULL != m_pDLGSkillToolTip )
			g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGSkillToolTip );
		else
			return;
	}

	// 여기부터..

	if(bShow)
	{	
		CKTDGUIStatic* pItemDesc = static_cast<CKTDGUIStatic*>( m_pDLGSkillToolTip->GetControl(L"SkillDesc") );

		if ( NULL == g_pData ||
			 NULL == g_pData->GetSkillTree() ||
			 NULL == g_pData->GetTitleManager() ||
			 NULL == g_pData->GetMyUser() ||
			 NULL == g_pData->GetMyUser()->GetSelectUnit() ||
			 NULL == g_pKTDXApp ||
			 NULL == g_pKTDXApp->GetDGManager() ||
			 NULL == g_pKTDXApp->GetDGManager()->GetDialogManager() ||
			 NULL == pItemDesc )
		{
			m_pDLGSkillToolTip->SetShow(false);
			return;
		}

		CKTDGUIControl::UIStringData* pTitleStringData		= pItemDesc->GetString( 0 );	/// 타이틀 툴팁 스트링 데이터 ( 스킬 이름 및 레벨 )
		CKTDGUIControl::UIStringData* pMainStringData		= pItemDesc->GetString( 1 );	/// 메인 스킬 설명 툴팁 스트링 데이터 ( 스킬 설명 및 효과 )
		CKTDGUIControl::UIStringData* pSubStringData		= pItemDesc->GetString( 2 );	/// 서브 스킬 툴팁 스트링 데이터 ( 스킬 타입, 소모 MP, 재사용 시간 )

		CKTDGUIControl::CPictureData* pMainPictureData		= pItemDesc->GetPicture( 0 );	/// 메인 스킬 설명 툴팁 배경 이미지 데이터
		CKTDGUIControl::CPictureData* pIconPictureData		= pItemDesc->GetPicture( 1 );	/// 스킬 아이콘 이미지 데이터
		
#ifdef MOVIE_TEST
		CKTDGUIControl::CMovieData* pUseSkillMovieData				= pItemDesc->GetMovie( 0 );		/// 스킬 시전 영상
#else	// MOVIE_TEST
		CKTDGUIControl::CPictureData* pUseSkillPictureData	= pItemDesc->GetPicture( 2 );	/// 스킬 시전 샘플 이미지 데이터
#endif // MOVIE_TEST


		if ( NULL == pTitleStringData ||
			 NULL == pMainStringData ||
			 NULL == pSubStringData ||
			 NULL == pMainPictureData ||
			 NULL == pIconPictureData ||
#ifdef MOVIE_TEST
			 NULL == pUseSkillMovieData
#else	// MOVIE_TEST
			NULL == pUseSkillPictureData
#endif // MOVIE_TEST

			 )
		{
			m_pDLGSkillToolTip->SetShow(false);
			return;
		}

		
		/// 스킬 레벨 연산 -------------------------------------------------------------------------------------
		const CX2UserSkillTree& refUserSkillTree = g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_UserSkillTree;
		
		int iSkillLevel = refUserSkillTree.GetSkillLevel( eSkillID , true );		/// 스킬 레벨 ( 실제로 설정되어 있는 레벨 )

		/// 만약 스킬창에서 스킬 레벨을 올리고 있으며, 장비된 스킬의 툴팁이 아닐 때
#ifdef SKILL_PAGE_SYSTEM //JHKang
		if ( true == m_bNowLearnSkill && false == bEquipped && refUserSkillTree.IsUsingPage() )
#else //SKILL_PAGE_SYSTEM
		if ( true == m_bNowLearnSkill && false == bEquipped )
#endif //SKILL_PAGE_SYSTEM
		{
			const std::map<int, KGetSkillInfo> mapSkillInfo = g_pData->GetSkillTree()->GetMapSkillInfo();

			std::map<int, KGetSkillInfo>::const_iterator it = mapSkillInfo.find( static_cast<int>( eSkillID ) );

			if ( it != mapSkillInfo.end() )
				iSkillLevel = it->second.m_iSkillLevel;
		}

		/// 스킬 템플릿 반환 -----------------------------------------------------------------------------------
#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
		m_bReplacedEsCounter = false;
		if( bEquipped == true && eSkillID == CX2SkillTree::SI_A_ES_COUNTER )
		{
			int iSkillLevel = refUserSkillTree.GetSkillLevel( CX2SkillTree::SI_P_EIS_COMPACT_COUNTER );
			if( iSkillLevel > 0 )
			{
				eSkillID = CX2SkillTree::SI_P_EIS_COMPACT_COUNTER;
				m_bReplacedEsCounter = true;
			}
		}

		if ( true == m_bReplacedEsCounter )
			iSkillLevel = refUserSkillTree.GetSkillLevel( CX2SkillTree::SI_P_EIS_COMPACT_COUNTER );
#endif //UPGRADE_SKILL_SYSTEM_2013

		const CX2SkillTree::SkillTemplet* pSkillTemplet = g_pData->GetSkillTree()->GetSkillTemplet( eSkillID );

		if ( NULL == pSkillTemplet )	/// 스킬 템플릿이 없으면, 툴팁을 숨겨 버리자
		{
			m_pDLGSkillToolTip->SetShow(false);
			return;
		}
		
		/// 스킬 레벨 증가 버프 및 소켓으로 증가된 레벨 --------------------------------------------------------
		/// 효과로 인해 증가된 스킬 수치
		int iIncreasekillLevel = 0;

		/// 패시브 스킬이 아닐때만 추가 스킬 레벨 연산
		switch( pSkillTemplet->m_eType )
		{
		case CX2SkillTree::ST_BUFF:		
		case CX2SkillTree::ST_ACTIVE:
		case CX2SkillTree::ST_SPECIAL_ACTIVE:
#ifdef FINALITY_SKILL_SYSTEM		// 해외팀 오류 수정
		case CX2SkillTree::ST_HYPER_ACTIVE_SKILL:
#endif FINALITY_SKILL_SYSTEM
			{
				iIncreasekillLevel = g_pData->GetMyUser()->GetSelectUnit()->GetIncreaseSkillLevelByBuff() + 
									 g_pData->GetMyUser()->GetSelectUnit()->GetIncreaseSkillLevelBySocket( eSkillID );
			} break;
		}

		/// 증가치까지 합해서 한계치를 넘는다면, 한계치에 맞게 조정
		if ( iSkillLevel + iIncreasekillLevel > MAX_LIMITED_SKILL_LEVEL )
			iIncreasekillLevel = MAX_LIMITED_SKILL_LEVEL - iSkillLevel;

		
		/// 스킬 트리 템플릿 반환 ------------------------------------------------------------------------------
		int										iUnitClass			= static_cast<int>( g_pData->GetMyUser()->GetSelectUnit()->GetClass() );
		const CX2SkillTree::SkillTreeTemplet*	pSkillTreeTemplet	= g_pData->GetSkillTree()->GetSkillTreeTemplet( iUnitClass, pSkillTemplet->m_eID );


		if ( NULL == pSkillTreeTemplet )	/// 스킬 트리 템플릿이 없으면, 툴팁을 숨겨 버리자
		{
			m_pDLGSkillToolTip->SetShow(false);
			return;
		}


		/// 스킬 아이콘 설정 -----------------------------------------------------------------------------------
		pIconPictureData->SetTex( pSkillTemplet->m_wstrIconName.c_str(), pSkillTemplet->m_wstrIconPieceName.c_str() );
		//pIconPictureData->SetSize( D3DXVECTOR2( 30.f, 30.f ) );


		/// 스킬 시전 샘플 이미지 설정 -------------------------------------------------------------------------
		
		
#ifdef MOVIE_TEST
		if ( !pSkillTemplet->m_wstrImageName.empty() )
		{
			pUseSkillMovieData->SetShow( true );
			pUseSkillMovieData->SetMovieFileName( pSkillTemplet->m_wstrImageName.c_str() );
		}
		else
			pUseSkillMovieData->SetShow( false );
#else	// MOVIE_TEST
		if ( L"" != pSkillTemplet->m_wstrImageName && L"" != pSkillTemplet->m_wstrImagePieceName )
		{
			pUseSkillPictureData->SetShow( true );		/// 스킬 이미지가 설정되어 있을 때만 표시
			pUseSkillPictureData->SetTex( pSkillTemplet->m_wstrImageName.c_str(), pSkillTemplet->m_wstrImagePieceName.c_str() );
		}
		else
		{
			pUseSkillPictureData->SetShow( false );
		}
#endif // MOVIE_TEST


		
		/// 타이틀 툴팁 설정 -----------------------------------------------------------------------------------
		wstring SkillSkillNameAndLevelDesc = L""; 

		GetSkillTitleDesc( SkillSkillNameAndLevelDesc, pSkillTemplet, pSkillTreeTemplet, iSkillLevel, iIncreasekillLevel );		/// 스킬 이름 및 레벨 스트링 생성

		if( SkillSkillNameAndLevelDesc.empty() == true )		/// 스트링이 없으면, 툴팁을 숨겨 버리자
		{
			m_pDLGSkillToolTip->SetShow(false);
			return;
		}

		/// 스트링 설정
		pTitleStringData->msg	= SkillSkillNameAndLevelDesc;

		/// 위치 설정 ( 스킬 아이콘 옆으로 조정 )
		pTitleStringData->pos.x	= pMainStringData->pos.x + 45.f;


		/// 서브 툴팁 생성 -------------------------------------------------------------------------------------
		wstring SkillSubDesc = L"";

		const int iSubDeskSkillLevel = 0 < iSkillLevel ? iSkillLevel + iIncreasekillLevel : 1;

		GetSkillSubDesc( pSkillTemplet, iSubDeskSkillLevel, SkillSubDesc );		/// 서브 툴팁 스트링 생성

		if( SkillSubDesc.empty() == true )			/// 스트링이 없으면, 툴팁을 숨겨 버리자
		{
			m_pDLGSkillToolTip->SetShow(false);
			return;
		}

		/// 스트링 설정
		pSubStringData->msg		= SkillSubDesc;

		
		wstring wstrMainSkillDesc = L"";
		/// 선행 스킬 툴팁 설정 --------------------------------------------------------------------------------
		if ( 0 < pSkillTreeTemplet->m_iPrecedingSkill )		/// 선행 스킬이 있을 때, 선행 스킬 
			GetPrecedingSkillDesc( wstrMainSkillDesc, static_cast<CX2SkillTree::SKILL_ID>( pSkillTreeTemplet->m_iPrecedingSkill ) );


		/// 봉인 스킬 아이템 구입 안내 툴팁 설정 ---------------------------------------------------------------
		if( true == pSkillTemplet->m_bBornSealed && false == refUserSkillTree.IsSkillUnsealed( eSkillID ) )
			GetSkillUnSealedDesc( wstrMainSkillDesc, eSkillID );

		
		/// 툴팁 본문 설정 -------------------------------------------------------------------------------------
		wstrMainSkillDesc += pSkillTemplet->m_wstrMainDesc;					/// 공통되는 스킬 설명 스트링 추가 ( 공통 설명과 스킬 효과 사이에 선을 넣기 위해, 본문 분리 )

		wstrMainSkillDesc += L"\n";

		/// 줄 정리
		wstrMainSkillDesc = g_pMain->GetStrByLineBreakColor
			( wstrMainSkillDesc.c_str(), static_cast<int>( MAGIC_SKILL_DESC_WIDTH ), SLOT_MANAGER_FONT_INDEX );

		
		/// 툴팁 크기 설정 -------------------------------------------------------------------------------------
		CKTDGFontManager::CUKFont* pItemDescFont = g_pKTDXApp->GetDGManager()->GetDialogManager()->GetUKFont( SLOT_MANAGER_FONT_INDEX );	/// XUF_DODUM_13_SEMIBOLD

		/// 메인 툴팁 스트링 높이
		int iMainDescHeight = static_cast<int>( pItemDescFont->GetHeight( wstrMainSkillDesc.c_str() ) );

		/// 글자간 간격 처리
		int iLineNum = 0;
		int iOffSetSize = wstrMainSkillDesc.find( L"\n", 0 );
		while( iOffSetSize != -1 )
		{
			iOffSetSize++;
			iLineNum += 2;
			iOffSetSize = wstrMainSkillDesc.find( L"\n", iOffSetSize );
		}
		iMainDescHeight += iLineNum;

		/// 메인 툴팁 Y 위치
		int iMainDescYPos = static_cast<int>( pTitleStringData->pos.y + SKILL_MAIN_DESC_OFFSET_Y );

		/// 툴팁의 가로 사이즈 강제 설정
		int			itemDescWidth		= static_cast<int>( MAGIC_SKILL_DESC_WIDTH ) + ITEM_DESC_ADD_SIZE_X;
		/// 툴팁의 세로 사이즈 연산
		int			itemDescHeight		= static_cast<int>( iMainDescHeight / g_pKTDXApp->GetResolutionScaleY() );


		/// 툴팁 내 분할선 추가 ( 공통 설명과 스킬 효과 사이 )
		CKTDGUIStatic* pDevideLine = static_cast<CKTDGUIStatic*>( m_pDLGSkillToolTip->GetControl(L"g_pStatic_HorizonLine") );
		if ( NULL != pDevideLine && 
			 NULL != pDevideLine->GetPicture( 0 ) && 
			 NULL != pDevideLine->GetPicture( 0 )->pPoint && 
			 NULL != pDevideLine->GetPicture( 1 ) &&
			 NULL != pDevideLine->GetPicture( 1 )->pPoint )
		{
			pDevideLine->GetPicture( 0 )->SetSizeX( itemDescWidth - 20.f );
			pDevideLine->GetPicture( 1 )->SetSizeX( itemDescWidth - 20.f );
			pDevideLine->GetPicture( 1 )->SetPos( D3DXVECTOR2( 10.f, static_cast<float>( iMainDescYPos + itemDescHeight ) ) );
		}


		/// 스킬 효과 툴팁 반환 ( 공통 설명까지의 길이만 구한 후 해당 위치에 선 삽입, 이후 나머지 스트링 연결하여 툴팁 완성 )
		GetSkillEffectiveDesc( wstrMainSkillDesc, iSkillLevel, iIncreasekillLevel, pSkillTemplet );

		/// 줄 정리
		wstrMainSkillDesc = g_pMain->GetStrByLineBreakColor
			( wstrMainSkillDesc.c_str(), static_cast<int>( MAGIC_SKILL_DESC_WIDTH ), SLOT_MANAGER_FONT_INDEX );

		pMainStringData->msg = wstrMainSkillDesc;		/// 툴팁 메인 본문 스트링 설정


		/// 메인 툴팁 스트링 높이 ( 공통 설명 + 스킬 효과 )
		iMainDescHeight = static_cast<int>( pItemDescFont->GetHeight( pMainStringData->msg.c_str() ) + SKILL_MAIN_DESC_OFFSET_Y );

		/// 글자간 간격 처리
		iLineNum = 0;
		iOffSetSize = wstrMainSkillDesc.find( L"\n", 0 );
		while( iOffSetSize != -1 )
		{
			iOffSetSize++;
			iLineNum += 2;
			iOffSetSize = wstrMainSkillDesc.find( L"\n", iOffSetSize );
		}
		iMainDescHeight += iLineNum;


		/// 툴팁 높이 재 연산 ( 공통 설명 + 스킬 효과 )
		itemDescHeight	= static_cast<int>( iMainDescHeight / g_pKTDXApp->GetResolutionScaleY() ) + ITEM_DESC_ADD_SIZE_Y;

		pTitleStringData->fontIndex	= SLOT_MANAGER_FONT_INDEX;	/// 타이틀 툴팁 폰트 크기 변경
		pMainStringData->fontIndex	= SLOT_MANAGER_FONT_INDEX;	/// 메인 툴팁 폰트 크기 변경
		pSubStringData->fontIndex	= SLOT_MANAGER_FONT_INDEX;	/// 서브 툴팁 폰트 크기 변경


		/// 스킬 툴팁 생성 위치 지정 ---------------------------------------------------------------------------
		int itemDescPosX = 0;
		int itemDescPosY = 0;

		int iDescWidthWithPicture = 
			
#ifdef MOVIE_TEST
		( true == pUseSkillMovieData->GetShow() ?
			static_cast<int>( itemDescWidth + ( 10.f * g_pKTDXApp->GetResolutionScaleX() ) + pUseSkillMovieData->GetSize().x ) : itemDescWidth );
#else	// MOVIE_TEST
		( true == pUseSkillPictureData->GetShow() ?
			static_cast<int>( itemDescWidth + ( 10.f * g_pKTDXApp->GetResolutionScaleX() ) + pUseSkillPictureData->GetSize().x ) : itemDescWidth );
#endif // MOVIE_TEST


		if ( pos.x + ( size.x / 2 ) - ( iDescWidthWithPicture / 2 ) >= 0 ) /// 왼쪽에 공간이 충분한 경우
		{
			if ( pos.x+ ( size.x / 2 ) - ( iDescWidthWithPicture / 2 ) + iDescWidthWithPicture > 1024 )
			{
				itemDescPosX = static_cast<int>( 1024 - iDescWidthWithPicture );
			}
			else
			{
				itemDescPosX = static_cast<int>( pos.x + ( size.x / 2 ) - ( iDescWidthWithPicture / 2 ) );
			}
		}
		else
		{
			itemDescPosX = 0;
		}

		/// 가리키고 있는 스킬 슬롯 위쪽으로 툴팁을 표시할 수 있는경우
		if ( static_cast<int>( pos.y ) - itemDescHeight >= 0 )
		{
			/// 가리키고 있는 스킬 슬롯 위치 + 툴팁 사이즈
			itemDescPosY =  static_cast<int>( pos.y ) - itemDescHeight;
		}
		else	/// 가리키고 있는 스킬 슬롯 아래에 표시
		{
			itemDescPosY = static_cast<int>( pos.y + size.y );		/// 스킬 슬롯의 Bottom 위치

			int iBottomOverSize = 768 - (itemDescPosY + itemDescHeight );	/// 화면을 넘어가는 크기를 계산하여, 스킬 슬롯의 Bottom 위치에 증가 시켜 준다.

			if ( iBottomOverSize < 0 )
				itemDescPosY += iBottomOverSize;
		}

		// 설명 위부분이 화면 밖으로 나가지 않도록 제한
		if( itemDescPosY < 0 )
			itemDescPosY = 0;
		
		/// 툴팁 위치 설정
		m_pDLGSkillToolTip->SetPos( D3DXVECTOR2( static_cast<float>( itemDescPosX ), static_cast<float>( itemDescPosY ) ) );
		
		/// 툴팁 배경 설정
		pMainPictureData->SetSize( D3DXVECTOR2( static_cast<float>( itemDescWidth ), static_cast<float>( itemDescHeight ) ) );
		pMainPictureData->pPoint->color = D3DXCOLOR( 1,1,1,0.7f );

		bool bCheckEquipping = false;


		/// 메인 설명 툴팁의 y좌표를 타이틀 툴팁의 위치에 맞춰 설정 
		/// ( 타이틀 툴팁이 가장 상단에 있으므로, 기준으로 설정하여 조정 + 타이틀 상단 스트링 높이 + 타이틀 스트링과 메인 설명 타이틀 사이의 간격 )
		pMainStringData->pos.y = static_cast<float>( iMainDescYPos );

		/// 타이틀 스트링 위치 설정 ( 스킬 아이콘 옆으로 조정 )
		pTitleStringData->pos.x	= pMainStringData->pos.x + 45.f;

		/// 서브 툴팁 위치를 메인 툴팁 가로 길이에 맞춰 설정 ( 툴팁 크기의 가장 우측에서 '텍스트 오른쪽 정렬'로 설정 )
		pSubStringData->pos.x	= static_cast<float>( pMainStringData->pos.x + MAGIC_SKILL_DESC_WIDTH - 15.f );
		
		/// 스킬 시전 샘플 이미지 위치 조정 ( 툴팁 오른쪽 끝 + Offset )
#ifdef MOVIE_TEST
		pUseSkillMovieData->SetOriginPos( D3DXVECTOR2( static_cast<float>(itemDescWidth ) + ( 10.f * g_pKTDXApp->GetResolutionScaleX() ), 0.f ) );
#else	// MOVIE_TEST
		pUseSkillPictureData->SetPos( D3DXVECTOR2( static_cast<float>(itemDescWidth ) + ( 10.f * g_pKTDXApp->GetResolutionScaleX() ), 0.f ) );
#endif // MOVIE_TEST



		m_pDLGSkillToolTip->SetShow( true );
	}
	else
	{
		m_pDLGSkillToolTip->SetShow( false );
	}
	
	
}

/** @function	: GetSkillDesc
	@brief		: 스킬 효과를 표시하는 메인 툴팁 생성
	@param		: 스트링 참조자, 스킬 레벨, 추가 스킬 레벨, 스킬 템플릿
*/
void CX2UISkillTreeNew::GetSkillEffectiveDesc( OUT wstring& wstrDesc, IN int iSkillLevel, int iIncreasekillLevel, IN const CX2SkillTree::SkillTemplet* pSkillTemplet ) const
{
	if ( NULL == pSkillTemplet ||
		 NULL == g_pData ||
		 NULL == g_pData->GetSkillTree() )
		return;

	wstrDesc += L"\n";

	if ( 0 < iSkillLevel )		/// 스킬 레벨이 0보다 클 때만, 현재 레벨 스킬 효과 출력
	{
		wstrDesc += L"\n";

		wstrDesc += GET_STRING( STR_ID_25071 );	/// <현재 레벨>

		wstrDesc += L"\n";

		wstrDesc += pSkillTemplet->GetSkillEffectiveDescValue( iSkillLevel + iIncreasekillLevel );			/// 현재 레벨에 대한 스킬 효과 툴팁 설정
	}


	if ( MAX_LIMITED_SKILL_LEVEL > iSkillLevel )		/// 현재 스킬 레벨이 최대 스킬 레벨 한계보다 낮을 때, 다음 레벨 표시
	{
		if ( NULL != g_pData &&
			 NULL != g_pData->GetMyUser() && 
			 NULL != g_pData->GetMyUser()->GetSelectUnit() )
		{
			const int iUnitLevel = g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_Level;	/// 유닛의 레벨
			const int iNextLevel = 0 == iSkillLevel ? 1 : iSkillLevel + iIncreasekillLevel + 1;		/// 레벨이 0이면, 1레벨 표시


#ifdef HIDE_DO_NOT_EXIST_SKILL_EFFECTIVE_DESC // 김태환
			/// 다음 레벨의 툴팁이 있을때만 출력 한다.
			const wstring wstrNextEffectiveDesc =  pSkillTemplet->GetSkillEffectiveDescValue( iNextLevel );

			if ( 0 != wstrNextEffectiveDesc.compare( L"" ) )
#endif //HIDE_DO_NOT_EXIST_SKILL_EFFECTIVE_DESC
			{
				/// 요구 레벨 표시 스트링 설정
				wstring wstrRequireLevel = L"";

				/// 요구 레벨 반환 ( 다음 스킬 레벨을 올리는데 필요한 레벨이므로, 추가되는 레벨은 베제한다. )
				const int iRequireLevel = pSkillTemplet->GetSkillRequireLevelValue( iSkillLevel + 1 );

				if ( iUnitLevel >= iRequireLevel )
					wstrRequireLevel = GET_REPLACED_STRING( ( STR_ID_25173, "i", iRequireLevel ) );		/// 캐릭터 레벨 @1 이상
				else	/// 현재 유닛 레벨이 부족하면, 빨간색 처리
				{
					wstrRequireLevel =  L"#CFF0000";
					wstrRequireLevel += GET_REPLACED_STRING( ( STR_ID_25173, "i", iRequireLevel ) );	/// 캐릭터 레벨 @1 이상
					wstrRequireLevel += L"#C878787";
				}

				if ( 0 < iSkillLevel )
					wstrDesc += L"\n";

				wstrDesc += L"\n#C878787";			/// 스트링 회색 처리 ( R:135, G: 135, B: 135 )

#ifdef BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
				// 해당 스킬이 찍을 수 있는 최고 레벨 이라면 ( 기본 20 )
				// 다음 레벨에 대해 보여주지 않는다.
				int										iUnitClass			= static_cast<int>( g_pData->GetMyUser()->GetSelectUnit()->GetClass() );		/// 유닛 클레스
				const CX2SkillTree::SkillTreeTemplet*	pSkillTreeTemplet	= g_pData->GetSkillTree()->GetSkillTreeTemplet( iUnitClass, pSkillTemplet->m_eID );	/// 스킬 트리 템플릿

				if ( NULL != pSkillTreeTemplet && pSkillTreeTemplet->m_iMaxSkillLevel <= iSkillLevel )
				{

					wstrDesc += GET_STRING ( STR_ID_25375 );
				}

				else
#endif // BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
				{
					wstrDesc += GET_REPLACED_STRING( ( STR_ID_25072, "L", wstrRequireLevel ) );				/// <다음 레벨 : @1>
				}

				wstrDesc += L"\n";

				/// 다음 레벨에 대한 스킬 효과 툴팁 설정 ( 추가되는 레벨 효과가 있다면, 해당 레벨을 포함하여 반환 )
				wstrDesc += pSkillTemplet->GetSkillEffectiveDescValue( iNextLevel );

				wstrDesc += L"#CX";

				const int iRequireLearnSkillPoint	= pSkillTemplet->m_iRequireLearnSkillPoint;		/// 습득시 필요한 SP량
				const int iRequireUpgradeSkillPoint = pSkillTemplet->m_iRequireUpgradeSkillPoint;	/// 강화시 필요한 SP량

				/// 실제 습득중 레벨 반환
				const int iLearnLevel = g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_UserSkillTree.GetSkillLevel( pSkillTemplet->m_eID );

				/// 만약 습득과 강화시 필요한 SP량이 다르고, 아직 배우지 못한 스킬일 때, 안내 문구 출력
				if ( iRequireLearnSkillPoint != iRequireUpgradeSkillPoint && 0 >= iLearnLevel )
				{
					wstrDesc += L"\n\n";

					wstrDesc += GET_REPLACED_STRING( ( STR_ID_25398, "ii", iRequireLearnSkillPoint, iRequireUpgradeSkillPoint ) );
				}
			}
		}
	}

#if defined( _IN_HOUSE_ ) || defined( _IN_HOUSE_SERVICE_READY_QA_ ) || defined( _OPEN_TEST_ ) || defined( _OPEN_TEST_2_ )
	/// 개발자 기능 - 스킬 아이디 출력
	if( true == g_pMain->IsMyAuthLevelHigherThan( CX2User::XUAL_DEV ) )
	{
		WCHAR buf[256] = {0,};
		StringCchPrintf( buf, 255, L"\n\n#C00FF00Skill ID : %d#CX", static_cast<int>( pSkillTemplet->m_eID ) );
		
		wstrDesc += buf;
	}
#endif // _SERVICE_
}

void CX2UISkillTreeNew::ShowUIDesc( bool bShow, wstring wstr /*= L""*/, D3DXVECTOR2 pos /*= D3DXVECTOR2(0,0)*/, CX2TalkBoxManagerImp::TALK_BOX_TYPE TBT /*= CX2TalkBoxManagerImp::TBT_FROM_UP_RIGHT*/, D3DXCOLOR coTextColor /*= D3DXCOLOR(0,0,0,1)*/, D3DXCOLOR coBackColor /*= D3DXCOLOR(1, 0.96f, 0.6f, 1 )*/)
{
	m_pTalkBoxManager->Clear();

	if(bShow)
	{
		CX2TalkBoxManagerImp::TalkBox talkBox;

		talkBox.m_vPos				= pos;
		talkBox.m_GameUnitType		= CX2GameUnit::GUT_USER;
		talkBox.m_fRemainTime		= 2.0f;

		talkBox.m_wstrTalkContent	= wstr.c_str();
		talkBox.m_bTraceUnit		= false;
		talkBox.m_TalkBoxType		= TBT;
		talkBox.m_coTextColor		= coTextColor;
		talkBox.m_BackTexColor		= coBackColor;
	
		m_pTalkBoxManager->Push( talkBox );

	}
	
}


// kimhc // KTDXCommonFunc로 옮김 // 2009-06-16
//wstring CX2UISkillTreeNew::GetExpirationDateDesc( const wstring& wstrExpirationDate )
//{
//	wstring tempDesc = L"";
//
//	
//	tempDesc = GET_STRING( STR_ID_309 );
//	//tempDesc = L"만료 시간 : ";
//
//	CTime cTime;
//	KncUtil::ConvertStringToCTime( wstrExpirationDate, cTime );
//
//	wstringstream wstrstm;
//
//	//wstrstm << cTime.GetYear() << L"년 " << cTime.GetMonth() << L"월 " << cTime.GetDay() << L"일 " << cTime.GetHour() << L"시 " << cTime.GetMinute() << L"분"; 
//	//tempDesc += wstrstm.str().c_str();
//	//tempDesc += L"\n";
//
//	wstrstm.str( L"" );
//	wstrstm << L"\n";
//
//	//time_t t0 = time(0);
//	__time64_t t0 = g_pData->GetServerCurrentTime();
//
//	double fDiffTime = difftime( KncUtil::TimeToInt( cTime ), (time_t)t0 );
//
//	int remainDay =  (int)(fDiffTime / ( 60 * 60 * 24 ) );
//	if ( remainDay > 0 )
//		wstrstm << remainDay << GET_STRING( STR_ID_14 ) << L" ";
//
//	int remainHour = ((time_t)fDiffTime % ( 60 * 60 * 24 )) / (60 * 60);
//	if ( remainHour < 0 )
//		remainHour = 0;
//
//	wstrstm << remainHour << GET_STRING( STR_ID_310 ) << L" ";
//
//	int remainMinute = ((time_t)fDiffTime % ( 60 * 60 )) / (60 );
//
//	if ( remainDay <= 0 )
//	{
//		if ( remainMinute <= 0 )
//			remainMinute = 0;
//		wstrstm << remainMinute << GET_STRING( STR_ID_311 ) << L" ";
//	}
//
//	tempDesc += wstrstm.str().c_str();
//
//
//	return tempDesc;
//
//}







void CX2UISkillTreeNew::OpenDLGSkillUnsealed( bool bOpen )
{
	if( false == bOpen )
	{
		if( m_pDLGSkillUnsealed != NULL )
		{
			m_pDLGSkillUnsealed->SetShowEnable( bOpen, bOpen );
		}
	}
	else
	{
		if( m_pDLGSkillUnsealed == NULL )
		{
			m_pDLGSkillUnsealed = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_Skill_Acquisition.lua" );
			g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGSkillUnsealed );
		}

		m_pDLGSkillUnsealed->SetShowEnable( bOpen, bOpen );
	}
}



void CX2UISkillTreeNew::UpdateDLGSkillUnsealed( CX2SkillTree::SKILL_ID eSkillID )
{
	if( m_pDLGSkillUnsealed == NULL )
		return;


	const CX2SkillTree::SkillTemplet* pSkillTemplet = g_pData->GetSkillTree()->GetSkillTemplet( eSkillID );
	if( NULL == pSkillTemplet )
		return;
	

	CKTDGUIStatic* pStaticSkillSlot = (CKTDGUIStatic*) m_pDLGSkillUnsealed->GetControl( L"g_pStaticSkill_Acquisition_Slot" );
	if( NULL != pStaticSkillSlot &&
		NULL != pStaticSkillSlot->GetPicture(1) )
	{
		std::string textureName = "";
		std::string texturePieceName = "";

		ConvertWCHARToChar( textureName, pSkillTemplet->m_wstrIconName.c_str() );
		ConvertWCHARToChar( texturePieceName, pSkillTemplet->m_wstrIconPieceName.c_str() );

		pStaticSkillSlot->GetPicture(1)->SetTex_LUA( textureName.c_str(), texturePieceName.c_str() );
	}


	CKTDGUIStatic* pStaticSkillName = (CKTDGUIStatic*) m_pDLGSkillUnsealed->GetControl( L"Static_Skill_Name" );
	if( NULL != pStaticSkillName )
	{
		pStaticSkillName->GetString(0)->msg = pSkillTemplet->m_wstrName;
	}

	/// 스킬 트리 업데이트
	g_pData->GetUIManager()->GetUISkillTree()->UpdateUI( false, false, true );
}

//{{ kimhc // 2010.3.26 // 무한 스킬 버그 수정
#ifdef MODIFY_INFINITE_SKILL_BUG
bool CX2UISkillTreeNew::ProcessSkillTreeSlotRMouseUp( LPARAM lParam )
{
	CX2State* pNowState = static_cast<CX2State*>( g_pMain->GetNowState() );
	if ( NULL != pNowState && false == pNowState->GetEnableShortCutKey() )	/// 단축키를 사용 못하는 때이면
		return true;	/// 드래그앤 드랍을 못하게 막음

	CKTDGUISlot* pSlot = (CKTDGUISlot*) lParam;
	CX2SkillTreeSlotData* pSlotData = (CX2SkillTreeSlotData*)pSlot->GetSlotData();
	if( pSlotData != NULL)
	{
		/// 만약 적용할 스킬 슬롯의 레벨이 0이면, 스킬 슬롯 설정 진행 중지
		if ( NULL != g_pData && 
			 NULL != g_pData->GetMyUser() &&
			 NULL != g_pData->GetMyUser()->GetSelectUnit() )
		{
			const int iSkillLevel = g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_UserSkillTree.GetSkillLevel( pSlotData->m_eSkillID );

			if ( 0 >= iSkillLevel )
			{
				g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_25347 ), g_pMain->GetNowState() );

				return false;
			}
		}

		const CX2SkillTree::SkillTemplet* pSkillTemplet = g_pData->GetSkillTree()->GetSkillTemplet( pSlotData->m_eSkillID );
		if(pSkillTemplet != NULL)
		{		
			if( true == pSkillTemplet->m_bBornSealed && false == m_pUserSkillTree->IsSkillUnsealed(pSlotData->m_eSkillID) )
			{
				D3DXVECTOR2 CenterPos = pSlot->GetPos() + pSlot->GetDialog()->GetPos() + pSlot->GetSize() * 0.5f;
				OpenSkillUnsealInformDLG( pSkillTemplet, CenterPos );
			}
			else if( m_pUserSkillTree->DoIHaveThisSkill( pSlotData->m_eSkillID ) )
			{

				switch(pSkillTemplet->m_eType)
				{
				case CX2SkillTree::ST_ACTIVE:
				case CX2SkillTree::ST_BUFF:
				case CX2SkillTree::ST_SPECIAL_ACTIVE:
#ifdef ADDED_RELATIONSHIP_SYSTEM
				case CX2SkillTree::ST_RELATIONSHIP_SKILL:
#endif // ADDED_RELATIONSHIP_SYSTEM
#ifdef FINALITY_SKILL_SYSTEM //JHKang
				case CX2SkillTree::ST_HYPER_ACTIVE_SKILL:
#endif //FINALITY_SKILL_SYSTEM
					{
						int iAlreadyEquippedSlotID = m_pUserSkillTree->GetSkillEquippedSlotIndex(pSlotData->m_eSkillID, false );
						int iAlreadyEquippedSlotBID = m_pUserSkillTree->GetSkillEquippedSlotIndex(pSlotData->m_eSkillID, true );
						// 같은 스킬이 장착되어 있지 않으면
						if( -1 == iAlreadyEquippedSlotID && -1 == iAlreadyEquippedSlotBID )					
						{
							/// A 슬롯이 비어있는지 확인
							iAlreadyEquippedSlotID = m_pUserSkillTree->GetSkillEquippedSlotIndex(CX2SkillTree::SI_NONE, false );
							if( -1 != iAlreadyEquippedSlotID )
							{
								g_pKTDXApp->GetDeviceManager()->PlaySound( L"Skill_Equip.ogg", false, false );
								g_pData->GetSkillTree()->Handler_EGS_CHANGE_SKILL_SLOT_REQ( iAlreadyEquippedSlotID, pSlotData->m_eSkillID );
								return true;
							}
							else
							{
								// B  빈 슬롯을 찾아서 장착한다
								if( true == m_bIsExpandSkillSlot )
								{
									iAlreadyEquippedSlotID = m_pUserSkillTree->GetSkillEquippedSlotIndex(CX2SkillTree::SI_NONE, true );
									if( -1 != iAlreadyEquippedSlotID )
									{
										g_pKTDXApp->GetDeviceManager()->PlaySound( L"Skill_Equip.ogg", false, false );
										g_pData->GetSkillTree()->Handler_EGS_CHANGE_SKILL_SLOT_REQ( iAlreadyEquippedSlotID+EQUIPPED_SKILL_SLOT_COUNT, pSlotData->m_eSkillID );
										return true;
									}
								}
							}
						}
					} break;
				default:
					break;
				}
			}
		}
	}
	return true;
}

bool CX2UISkillTreeNew::ProcessEquipSlotDropped( LPARAM lParam )
{
	CKTDGUISlot* pSlot = (CKTDGUISlot*) lParam;
	CX2EquippedSkillSlotData* pSlotData = (CX2EquippedSkillSlotData*)pSlot->GetSlotData();

	CX2State* pNowState = static_cast<CX2State*>( g_pMain->GetNowState() );
	if ( NULL != pNowState && false == pNowState->GetEnableShortCutKey() )	/// 단축키를 사용 못하는 때이면
		return false;	/// 드래그앤 드랍을 못하게 막음

	CKTDGUISlot* pDroppedSlot = pSlot->GetDroppedSlot();
	CKTDGUISlotData* pDroppedSlotData = NULL;
	if( pDroppedSlot != NULL )
	{
		pDroppedSlotData = pDroppedSlot->GetSlotData();
		
		if ( NULL != pDroppedSlotData )
		{
			switch(pDroppedSlotData->m_eSlotDataType)
			{
			case CKTDGUISlotData::SDT_SKILL_TREE:
				{
					g_pKTDXApp->GetDeviceManager()->PlaySound( L"Skill_Equip.ogg", false, false );
					CX2SkillTreeSlotData* pDropData = (CX2SkillTreeSlotData*) pDroppedSlotData;
#ifndef SERV_SKILL_SLOT_CHANGE_PACKET_INTEGRATE
					int iAlreadyEquippedSlotID = m_pUserSkillTree->GetSkillEquippedSlotIndex(pDropData->m_eSkillID, false );
					int iAlreadyEquippedSlotBID = m_pUserSkillTree->GetSkillEquippedSlotIndex(pDropData->m_eSkillID, true );

					// 같은 스킬이 스킬슬롯에 장착 되어 있으면 해제
					if( -1 != iAlreadyEquippedSlotID )					
					{
						g_pData->GetSkillTree()->Handler_EGS_CHANGE_SKILL_SLOT_REQ( iAlreadyEquippedSlotID, CX2SkillTree::SI_NONE );
					}
					if( -1 != iAlreadyEquippedSlotBID)
					{
						g_pData->GetSkillTree()->Handler_EGS_CHANGE_SKILL_SLOT_REQ( iAlreadyEquippedSlotBID+EQUIPPED_SKILL_SLOT_COUNT, CX2SkillTree::SI_NONE );
					}
#endif // SERV_SKILL_SLOT_CHANGE_PACKET_INTEGRATE

					if ( NULL != pSlotData )
						g_pData->GetSkillTree()->Handler_EGS_CHANGE_SKILL_SLOT_REQ( pSlotData->GetSlotID(), pDropData->m_eSkillID );

					return true;
				} break;
			case CKTDGUISlotData::SDT_SKILL_EQUIP:
				{
					g_pKTDXApp->GetDeviceManager()->PlaySound( L"Skill_Equip.ogg", false, false );
#ifdef SERV_SKILL_SLOT_CHANGE_PACKET_INTEGRATE
					CX2EquippedSkillSlotData* pDropData = (CX2EquippedSkillSlotData*) pDroppedSlotData;
					if ( NULL != pSlotData  )
						g_pData->GetSkillTree()->Handler_EGS_CHANGE_SKILL_SLOT_REQ( pSlotData->GetSlotID(), pDropData->m_eSkillID );
#else SERV_SKILL_SLOT_CHANGE_PACKET_INTEGRATE
					// Skill Swap
					CX2EquippedSkillSlotData* pDropData = (CX2EquippedSkillSlotData*) pDroppedSlotData;
					CX2SkillTree::SKILL_ID SkillIDA = pSlotData->m_eSkillID;
					CX2SkillTree::SKILL_ID SkillIDB = pDropData->m_eSkillID;
					g_pData->GetSkillTree()->Handler_EGS_CHANGE_SKILL_SLOT_REQ( pSlotData->GetSlotID(), SkillIDB );
					g_pData->GetSkillTree()->Handler_EGS_CHANGE_SKILL_SLOT_REQ( pDropData->GetSlotID(), SkillIDA );
#endif // SERV_SKILL_SLOT_CHANGE_PACKET_INTEGRATE

					return true;
				} break;
			default:
				break;
			}
		}
	} 
	return false;
}

bool CX2UISkillTreeNew::ProcessEquipSlotRMouseUp( LPARAM lParam )
{
	CX2State* pNowState = static_cast<CX2State*>( g_pMain->GetNowState() );
	if ( NULL != pNowState && false == pNowState->GetEnableShortCutKey() )	/// 단축키를 사용 못하는 때이면
		return true;	/// 드래그앤 드랍을 못하게 막음

	CKTDGUISlot* pSlot = (CKTDGUISlot*) lParam;
	CX2EquippedSkillSlotData* pSlotData = (CX2EquippedSkillSlotData*)pSlot->GetSlotData();
	if(pSlotData->m_eSkillID != CX2SkillTree::SI_NONE)
	{
		g_pKTDXApp->GetDeviceManager()->PlaySound( L"Skill_UnEquip.ogg", false, false );
	}			

	g_pData->GetSkillTree()->Handler_EGS_CHANGE_SKILL_SLOT_REQ( pSlotData->GetSlotID(), CX2SkillTree::SI_NONE );
	return true;
}

// bool CX2UISkillTreeNew::CheckIsReady()
// {
// 	bool bIsReady = false;
// 
// 	switch ( g_pMain->GetNowStateID() )
// 	{
// 	case CX2Main::XS_PVP_ROOM:
// 		{
// 			CX2PVPRoom* pPvpRoom = g_pData->GetPVPRoom();
// 
// 			if ( pPvpRoom != NULL &&
// 				pPvpRoom->GetMySlot() != NULL && 
// 				pPvpRoom->GetMySlot()->m_bReady == true )
// 				bIsReady = true;
// 		} break;
// 
// 	case CX2Main::XS_VILLAGE_MAP:
// 		{
// 			CX2PartyManager::PartyMemberData* pPartyMemberData = 
// 				g_pData->GetPartyManager()->GetMyPartyMemberData();
// 
// 			if ( pPartyMemberData != NULL &&
// 				pPartyMemberData->m_bGameReady == true )
// 				bIsReady = true;
// 		} break;
// 
// 	default:
// 		break;
// 	}
// 
// 	return bIsReady;
// }
// 
// void CX2UISkillTreeNew::ShowOkAndCancelMsgForUnReady( LPARAM lParam, SKILL_TREE_UI_CUSTOM_MSG eSTUCM )
// {
// 	m_pDLGUnReady = g_pMain->KTDGUIOkAndCancelMsgBox( D3DXVECTOR2( -999, -999 ), GET_STRING( STR_ID_4958 ),
// 		STUCM_UNREADY_OK, g_pMain->GetNowState(), STUCM_UNREADY_CANCEL );
// 	
// 	m_pDLGUnReady->ClearDummyInt();
// 	m_pDLGUnReady->AddDummyInt( static_cast<int>(lParam) );	// 첫번째 더미에 이전 커스텀 MSG의 lParam 값( 여기서는 슬롯 )
// 	m_pDLGUnReady->AddDummyInt( static_cast<int>(eSTUCM) );	// 두번째 더미 int에 이전에 실행했던 커스텀 MSG가 무엇인가
// }
// 
// void CX2UISkillTreeNew::ProcessChangeEquipSlotAfterUnReady()
// {
// 	if ( m_pDLGUnReady == NULL )
// 		return;
// 
// 	LPARAM lParam = m_pDLGUnReady->GetDummyInt( 0 );
// 
// 	switch ( m_pDLGUnReady->GetDummyInt( 1 ) )
// 	{
// 	case STUCM_SKILLTREESLOT_RMOUSEUP:
// 		ProcessSkillTreeSlotRMouseUp( lParam );
// 		break;
// 
// 	case STUCM_EQUIPSLOT_DROPPED:
// 		ProcessEquipSlotDropped( lParam );
// 		break;
// 
// 	case STUCM_EQUIPSLOT_RMOUSEUP:
// 		ProcessEquipSlotRMouseUp( lParam );
// 		break;
// 
// 	default:
// 		break;
// 
// 	}
// 
// 	g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDLGUnReady, NULL, false );
// }
#endif MODIFY_INFINITE_SKILL_BUG
//}} kimhc // 2010.3.26 // 무한 스킬 버그 수정

#ifdef ELSWORD_WAY_OF_SWORD
void CX2UISkillTreeNew::DrawElswordWSSDesc( D3DXVECTOR2 vDrawPos_ )
{
	if( CX2Unit::UT_ELSWORD != g_pData->GetMyUser()->GetSelectUnit()->GetType() )
	{
		return;
	}

	if( m_pDLGElswordWSSDesc == NULL)
	{
		m_pDLGElswordWSSDesc = new CKTDGUIDialog( g_pMain->GetNowState(), L"", 0.07f, XDL_POP_UP );
		m_pDLGElswordWSSDesc->SetFront(true);
		m_pDLGElswordWSSDesc->SetCloseOnFocusOut(true);

		g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGElswordWSSDesc );
		m_pDLGElswordWSSDesc->SetFront( true );
		m_pDLGElswordWSSDesc->SetModal( true );

		CKTDGUIStatic* pStatic = new CKTDGUIStatic();
		pStatic->SetName( L"ItemDesc" );
		m_pDLGElswordWSSDesc->AddControl( pStatic );
		CKTDGUIControl::CPictureData* pPicture = new CKTDGUIControl::CPictureData();
		pStatic->AddPicture( pPicture );
		pPicture->SetEdge( false, 2, D3DXCOLOR(1,1,1,1) );
		pPicture->SetTex( L"FadeInOut.dds" );

		CKTDGUIControl::UIStringData* pString = new CKTDGUIControl::UIStringData();
		pStatic->AddString( pString );
		pString->fontIndex = SLOT_MANAGER_FONT_INDEX;
		pString->color = D3DXCOLOR( 1,1,1,1 );
		pString->outlineColor = D3DXCOLOR( 1,1,1,1 );
		pString->msg = L"";
		pString->fontStyle = CKTDGFontManager::FS_NONE;
		pString->sortFlag = DT_LEFT;
		pString->pos = D3DXVECTOR2( 20, 20 );
	}

	int itemDescWidth = 300;
	int itemDescHeight = 200;

	int itemDescPosX = (int)vDrawPos_.x;
	int itemDescPosY = (int)vDrawPos_.y;
	
	if( g_pX2Game != NULL && g_pX2Game->GetMyUnit() != NULL )
	{
		wstringstream wstrm;
		
		switch( g_pX2Game->GetMyUnit()->GetWayOfSwordState() )
		{
			case -2: // CX2GUElsword_SwordMan::WSS_DESTRUCTION
				{
					wstrm << L"#CFFFFFF" << L"\n" << GET_STRING(STR_ID_14553) << L"\n" <<  L"#CX" <<
						L"\n" << L"#CFF4444" << L"\n" << GET_STRING(STR_ID_14554) << L"#CX" << L"\n\n";
					m_pDLGElswordWSSDesc->SetShow( true );
				} break;
			case -1: // CX2GUElsword_SwordMan::WSS_TOWARD_DESTRUCTION
				{
					wstrm << L"#CFFFFFF" << L"\n" << GET_STRING(STR_ID_14553) << L"\n" <<  L"#CX" <<
						L"\n" << L"#CFF4444" << L"\n" << GET_STRING(STR_ID_14554) << L"#CX" << L"\n\n";
					m_pDLGElswordWSSDesc->SetShow( true );
				} break;
			case 0: // CX2GUElsword_SwordMan::WSS_CENTER
				{
					wstrm << L"#CFFFFFF" << L"\n" << GET_STRING(STR_ID_14555) << L"\n" <<  
						L"\n" << L"#C999999" << L"\n" << GET_STRING(STR_ID_14556) << L"#CX" << L"\n\n";
					m_pDLGElswordWSSDesc->SetShow( true );
				} break;
			case 1: // CX2GUElsword_SwordMan::WSS_TOWARD_VIGOR
				{
					wstrm << L"#CFFFFFF" << L"\n" << GET_STRING(STR_ID_14557) <<  L"#CX" << 
						L"\n" << L"#C4488FF" << L"\n" << GET_STRING(STR_ID_14558) << L"#CX" << L"\n\n";
					m_pDLGElswordWSSDesc->SetShow( true );
				} break;
			case 2: // CX2GUElsword_SwordMan::WSS_VIGOR
				{
					wstrm << L"#CFFFFFF" << L"\n" << GET_STRING(STR_ID_14557) <<  L"#CX" << 
						L"\n" << L"#C4488FF" << L"\n" << GET_STRING(STR_ID_14558) << L"#CX" << L"\n\n";
					m_pDLGElswordWSSDesc->SetShow( true );
				} break;
			default:
				m_pDLGElswordWSSDesc->SetShow( false );
		}
		wstring wstrDesc = wstrm.str();
		CKTDGUIStatic* pItemDesc = (CKTDGUIStatic*)m_pDLGElswordWSSDesc->GetControl(L"ItemDesc");

		const CKTDGFontManager::CUKFont* pFont = g_pKTDXApp->GetDGManager()->GetDialogManager()->GetUKFont( pItemDesc->GetString(0)->fontIndex );

		const int MAGIC_ELSWORD_DESC_WIDTH = 330;
		wstrDesc = g_pMain->GetStrByLineBreakColor(wstrDesc.c_str(), (int)((float)MAGIC_ELSWORD_DESC_WIDTH*g_pKTDXApp->GetResolutionScaleX()), SLOT_MANAGER_FONT_INDEX);

		CKTDGFontManager::CUKFont* pItemDescFont = g_pKTDXApp->GetDGManager()->GetDialogManager()->GetUKFont( SLOT_MANAGER_FONT_INDEX );
		int itemDescWidth = (int)( pItemDescFont->GetWidth( wstrDesc.c_str() ) / g_pKTDXApp->GetResolutionScaleX() ) + ITEM_DESC_ADD_SIZE_X;
		int itemDescHeight = (int)( pItemDescFont->GetHeight( wstrDesc.c_str() ) / g_pKTDXApp->GetResolutionScaleY() ) + ITEM_DESC_ADD_SIZE_Y;


		m_pDLGElswordWSSDesc->SetPos( D3DXVECTOR2( (float)itemDescPosX, (float)itemDescPosY ) );
		m_pDLGElswordWSSDesc->GetStatic_LUA( "ItemDesc" )->GetPicture(0)->SetSize( D3DXVECTOR2( (float)itemDescWidth, (float)itemDescHeight) );
		m_pDLGElswordWSSDesc->GetStatic_LUA( "ItemDesc" )->GetPicture(0)->pPoint->color = D3DXCOLOR( 1,1,1,0.7f );		
		m_pDLGElswordWSSDesc->GetStatic_LUA( "ItemDesc" )->GetString(0)->msg = wstrDesc.c_str();
	}
	
	return;
}


#endif ELSWORD_WAY_OF_SWORD

//x2game이 생성되어 있지 않을 때 사용하는 쿨타임갱신 함수.
void CX2UISkillTreeNew::UpdateSkillCoolTime( float fElapsedTime )
{
	if( g_pData != NULL &&
		g_pData->GetMyUser() != NULL &&
		g_pData->GetMyUser()->GetSelectUnit() != NULL )
	{
		CX2UserSkillTree& refUserSkillTree = g_pData->GetMyUser()->GetSelectUnit()->AccessUnitData().m_UserSkillTree;
		
		if ( m_pDLGGameSkillSlot != NULL )
		{
			CKTDGUIStatic* pStaticCoolTime = (CKTDGUIStatic*)m_pDLGGameSkillSlot->GetControl( L"Static_Skill_Cool_Time" );
			CKTDGUIStatic* pStatic_Skill_Cool_Time_Fade = (CKTDGUIStatic*)m_pDLGGameSkillSlot->GetControl( L"Static_Skill_Cool_Time_Fade" );

			CKTDGUIStatic* pStaticSlot_A = (CKTDGUIStatic*) m_pDLGGameSkillSlot->GetControl( L"Static_SKILL_SLOT_A" );
			CKTDGUIStatic* pStaticSlot_B = (CKTDGUIStatic*) m_pDLGGameSkillSlot->GetControl( L"Static_SKILL_SLOT_B" );
			for ( int i = 0; i < EQUIPPED_SKILL_SLOT_COUNT; i++ )
			{
				pStaticSlot_A->GetPicture(i)->SetColor( D3DXCOLOR(1.f,1.f,1.f,0.3f));
				pStaticSlot_B->GetPicture(i)->SetColor( D3DXCOLOR(1.f,1.f,1.f,0.3f));
			}
			if ( pStaticCoolTime != NULL && pStatic_Skill_Cool_Time_Fade != NULL )
			{  //B슬롯도 같이 하기 위해서 EQUIPPED_SKILL_SLOT_COUNT*2
				for ( int i = 0; i < EQUIPPED_SKILL_SLOT_COUNT*2; i++ )
				{				
					bool bSlotB = (i > 3) ? true : false;
					int iSlotIndex = (i > 3) ? i-4 : i;

					//슬롯 B가 활성화 되지 않으면 슬롯 A만 확인하도록 break;
					if( false == g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_UserSkillTree.GetEnabledSkillSlotB() 
						&& bSlotB == true)
					{
						break;
					}

					if( true == bSlotB )
					{
						pStatic_Skill_Cool_Time_Fade = (CKTDGUIStatic*)m_pDLGGameSkillSlot->GetControl( L"Static_Skill_Cool_Time_Fade_B" );
						if( NULL == pStatic_Skill_Cool_Time_Fade )
							break;

						pStaticCoolTime = (CKTDGUIStatic*)m_pDLGGameSkillSlot->GetControl( L"Static_Skill_Cool_Time_B" );
						if( NULL == pStaticCoolTime )
							break;
					}


					const CX2UserSkillTree::SkillSlotData* pSlotData = refUserSkillTree.GetSkillSlot(iSlotIndex, bSlotB);
					refUserSkillTree.SetSkillCoolTimeLeft(CX2SkillTree::SI_NONE,0.f);
					if(pSlotData != NULL)
					{			
						if( NULL != pStaticSlot_A && NULL != pStaticSlot_B )
						{
							if( CX2SkillTree::SI_NONE != pSlotData->m_eID )
							{// 0~3 : B슬롯, 4~8 : A슬롯				
								if( true == bSlotB )
								{
									pStaticSlot_B->GetPicture(iSlotIndex)->SetColor( D3DXCOLOR(1.f,1.f,1.f,1.f));
								}
								else
								{
									pStaticSlot_A->GetPicture(iSlotIndex)->SetColor( D3DXCOLOR(1.f,1.f,1.f,1.f));
								}
							}
						}

						if(pSlotData->m_fCoolTimeLeft > 0.0f)
						{
							if (pStatic_Skill_Cool_Time_Fade->GetPicture(iSlotIndex) != NULL )
								pStatic_Skill_Cool_Time_Fade->GetPicture(iSlotIndex)->bShow = true;

							wstringstream wstrstm;
							wstrstm << (int)( ceil(pSlotData->m_fCoolTimeLeft) );
							pStaticCoolTime->SetString( iSlotIndex, wstrstm.str().c_str() );
						}
						else
						{
							if ( pStatic_Skill_Cool_Time_Fade->GetPicture(iSlotIndex) != NULL )
							{
								pStatic_Skill_Cool_Time_Fade->GetPicture(iSlotIndex)->bShow = false;
							}

							pStaticCoolTime->SetString( iSlotIndex, L"" );
						}						
					}			
				}
			}
		}
	}
}

/** @function : SwapSkillSlot
	@brief  : 현재 스킬슬롯 타입에 따라 스왑 함수 호출
*/
void CX2UISkillTreeNew::SwapSkillSlot(bool bVal, bool bForce/* = false*/)
{

#ifdef SKILL_SLOT_UI_TYPE_B
	if( true == m_bIsSkillUITypeA )
	{
		SwapSkillSlotTypeA( bVal, bForce );
	}
	else
	{
		SwapSkillSlotTypeB( bVal, bForce );
	}
#else
	if( m_bSkillSlotSwap == bVal && false == bForce )
		return;

	m_bSkillSlotSwap = bVal;
	if( true == m_bSkillSlotSwap ) 
	{
		D3DXVECTOR2 vOffsetPos(0,0);
		for( int i = 0 ; i < EQUIPPED_SKILL_SLOT_COUNT * 2; ++i )
		{
			vOffsetPos.y = ( i >= 4 ) ? 51.f : -51.f;
			if( NULL != GetEquippedSlot(i) )
			{
				GetEquippedSlot(i)->SetOffsetPos( vOffsetPos );
			}
		}	
		vOffsetPos.y = 51.f;
		SetOffsetPosSkillSLotStatic(vOffsetPos);
	}
	else
	{
		for( int i = 0 ; i < EQUIPPED_SKILL_SLOT_COUNT * 2; ++i )
		{
			if( NULL != GetEquippedSlot(i) )
			{
				GetEquippedSlot(i)->SetOffsetPos(D3DXVECTOR2(0,0));
			}
		}

		SetOffsetPosSkillSLotStatic(D3DXVECTOR2(0,0));
	}
#endif //SKILL_SLOT_UI_TYPE_B
}

/** @function : SetOffsetPosSkillSLotStatic
	@brief : 스킬슬롯에 사용된 스태틱들 오프셋 지정	
*/
void CX2UISkillTreeNew::SetOffsetPosSkillSLotStatic(const D3DXVECTOR2& vPos_, const D3DXVECTOR2& vAddPos_/* = D3DXVECTOR2(0,0)*/)
{
	if( NULL == m_pDLGGameSkillSlot )
		return;

	SetOffsetStatic( L"Static_Skill_Cool_Time_Fade", -vPos_ -vAddPos_);
	SetOffsetStatic( L"Static_Skill_Cool_Time_Fade_B", vPos_ );
	SetOffsetStatic( L"Static_Skill_Cool_Time", -vPos_ -vAddPos_);
	SetOffsetStatic( L"Static_Skill_Cool_Time_B", vPos_ );
	SetOffsetStatic( L"Static_SKILL_SLOT_A", -vPos_ -vAddPos_ );
	SetOffsetStatic( L"Static_SKILL_SLOT_B", vPos_ );
	SetOffsetStatic( L"Static_EL_SKILL_B", -vPos_ -vAddPos_ );
	SetOffsetStatic( L"Static_EL_SKILL_B_B", vPos_ );
	SetOffsetStatic( L"Static_EL_SKILL_R", -vPos_ -vAddPos_ );
	SetOffsetStatic( L"Static_EL_SKILL_R_B", vPos_ );
}

/** @function : SetOffsetStatic
	@brief : 컨트롤 이름으로 얻은 스태틱 객체의 오프셋값 지정
			SetOffsetPosSkillSLotStatic에서 사용되는 함수( 코드중복 제거 )
	@param : 컨트롤이름 (strControl_), 오프셋값(vPos)
*/
void CX2UISkillTreeNew::SetOffsetStatic( const WCHAR* strControl_, const D3DXVECTOR2& vPos_)
{
	CKTDGUIStatic* pStatic = static_cast<CKTDGUIStatic*>(m_pDLGGameSkillSlot->GetControl(strControl_));
	if( NULL != pStatic )
	{
		pStatic->SetOffsetPos(vPos_);
	}
}

void CX2UISkillTreeNew::UpdateSkillSlotString()
{
	if ( NULL == m_pDLGGameSkillSlot || NULL == g_pMain || NULL == g_pMain->GetKeyPad() )
		return;

	if ( NULL != g_pMain->GetKeyPad() )
	{
		CKTDGUIStatic* pStaticSkillHotKey = (CKTDGUIStatic*) m_pDLGGameSkillSlot->GetControl( L"SkillHotKeyEx" );

		if( NULL != pStaticSkillHotKey &&
			NULL != pStaticSkillHotKey->GetString(0) &&
			NULL != pStaticSkillHotKey->GetString(1) &&
			NULL != pStaticSkillHotKey->GetString(2) &&
			NULL != pStaticSkillHotKey->GetString(3)  )
		{
	#ifdef SKILL_SLOT_UI_TYPE_B
			if( false == m_bIsSkillUITypeA )
			{
				pStaticSkillHotKey->GetString(0)->msg = L"";
				pStaticSkillHotKey->GetString(1)->msg = L"";
				pStaticSkillHotKey->GetString(2)->msg = L"";
				pStaticSkillHotKey->GetString(3)->msg = L"";
			}
			else
	#endif //SKILL_SLOT_UI_TYPE_B
			{
				pStaticSkillHotKey->GetString(0)->msg = g_pMain->GetKeyPad()->GetKeyString( GAMEACTION_SKILL_EX1 );
				pStaticSkillHotKey->GetString(1)->msg = g_pMain->GetKeyPad()->GetKeyString( GAMEACTION_SKILL_EX2 );
				pStaticSkillHotKey->GetString(2)->msg = g_pMain->GetKeyPad()->GetKeyString( GAMEACTION_SKILL_EX3 );
				pStaticSkillHotKey->GetString(3)->msg = g_pMain->GetKeyPad()->GetKeyString( GAMEACTION_SKILL_EX4 );
			}
		}

		pStaticSkillHotKey = (CKTDGUIStatic*) m_pDLGGameSkillSlot->GetControl( L"SkillHotKey" );

		if( NULL != pStaticSkillHotKey &&
			NULL != pStaticSkillHotKey->GetString(0) &&
			NULL != pStaticSkillHotKey->GetString(1) &&
			NULL != pStaticSkillHotKey->GetString(2) &&
			NULL != pStaticSkillHotKey->GetString(3)  )
		{
			pStaticSkillHotKey->GetString(0)->msg = g_pMain->GetKeyPad()->GetKeyString( GAMEACTION_SKILL1 );
			pStaticSkillHotKey->GetString(1)->msg = g_pMain->GetKeyPad()->GetKeyString( GAMEACTION_SKILL2 );
			pStaticSkillHotKey->GetString(2)->msg = g_pMain->GetKeyPad()->GetKeyString( GAMEACTION_SKILL3 );
			pStaticSkillHotKey->GetString(3)->msg = g_pMain->GetKeyPad()->GetKeyString( GAMEACTION_SKILL4 );
		}
	}
}

#ifdef SKILL_SLOT_UI_TYPE_B
void CX2UISkillTreeNew::SetSkillUIType( bool bIsTypeA_ )
{
	if( NULL == m_pDLGGameSkillSlot)
		return;

	m_bIsSkillUITypeA = bIsTypeA_;


	D3DXVECTOR2 vSlotPos(0,0);
	if( false == m_bIsSkillUITypeA )
	{
		vSlotPos.x = -297.f;
#ifdef INT_WIDE_BAR
		vSlotPos.y = -599.f;
#else // INT_WIDE_BAR
		vSlotPos.y = -619.f;
#endif // INT_WIDE_BAR
	}
	//슬롯 B 활성화 여부
	bool bEnableSlotB = false;
	if( NULL != g_pData && NULL != g_pData->GetMyUser() && NULL != g_pData->GetMyUser()->GetSelectUnit() )
	{
		if( true == g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_UserSkillTree.GetEnabledSkillSlotB() )
			bEnableSlotB = true;
	}

	//현재 슬롯이 A슬롯인지 B슬롯인지 알려주는 UI
	CKTDGUIStatic* pStatic = static_cast<CKTDGUIStatic*>(m_pDLGGameSkillSlot->GetControl( L"g_pStatic_Slot_Guide" ));
	if( NULL != pStatic )
	{
		if( false == m_bIsSkillUITypeA && //스킬슬롯 B타입이고
			true == bEnableSlotB ) //슬롯 B가 활성화 되어 있다면
			pStatic->SetShow(true); 
		else
			pStatic->SetShow(false);
	}

	
	

 	m_pDLGGameSkillSlot->SetPos( vSlotPos );
	
	m_pDLGGameSkillSlot->GetShow( );

	SetOffsetPosEtcSkillSlotUI();// B슬롯 잠금/구매가이드 위치 변경
	UpdateSkillSlotString();// B슬롯 단축키 스트링 변경
	SwapSkillSlot( m_bSkillSlotSwap, true );
}

void CX2UISkillTreeNew::SetOffsetPosEtcSkillSlotUI()
{
	if( NULL == m_pDLGGameSkillSlot )
		return;

	D3DXVECTOR2 vOffsetPos(0,0);
	if( false == m_bIsSkillUITypeA )
		vOffsetPos.y = -200.f;

	CKTDGUIStatic* pStaticLock = static_cast<CKTDGUIStatic*>(m_pDLGGameSkillSlot->GetControl(L"LOCK"));
	if( NULL != pStaticLock )
		pStaticLock->SetOffsetPos(vOffsetPos);

	CKTDGUIButton* pButtonBuyGuide = static_cast<CKTDGUIButton*>(m_pDLGGameSkillSlot->GetControl(L"Button_Buy_Guide_Slot"));
		pButtonBuyGuide->SetOffsetPos(vOffsetPos);

}

/** @function : SwapSkillSlotTypeA
	@brief  : 상단/하단 슬롯 서로 위치 변경
*/
void CX2UISkillTreeNew::SwapSkillSlotTypeA(bool bVal, bool bForce/* = false*/)
{
	if( m_bSkillSlotSwap == bVal && false == bForce )
		return;

	m_bSkillSlotSwap = bVal;
	if( true == m_bSkillSlotSwap ) 
	{
		D3DXVECTOR2 vOffsetPos(0,0);
		for( int i = 0 ; i < EQUIPPED_SKILL_SLOT_COUNT * 2; ++i )
		{
			vOffsetPos.y = ( i >= 4 ) ? 51.f : -51.f;
			if( NULL != GetEquippedSlot(i) )
			{
				GetEquippedSlot(i)->SetOffsetPos( vOffsetPos );
			}
		}	
		vOffsetPos.y = 51.f;
		SetOffsetPosSkillSLotStatic(vOffsetPos);
	}
	else
	{
		for( int i = 0 ; i < EQUIPPED_SKILL_SLOT_COUNT * 2; ++i )
		{
			if( NULL != GetEquippedSlot(i) )
			{
				GetEquippedSlot(i)->SetOffsetPos(D3DXVECTOR2(0,0));
			}
		}

		SetOffsetPosSkillSLotStatic(D3DXVECTOR2(0,0));
	}
}


/** @function : SwapSkillSlotTypeB
	@brief  : 상단 슬롯은 화면에서 안보이도록 위로 보내면서 위치 변경
*/
void CX2UISkillTreeNew::SwapSkillSlotTypeB(bool bVal_, bool bForce_/* = false */)
{
	if( m_bSkillSlotSwap == bVal_ && false == bForce_)
		return;

	m_bSkillSlotSwap = bVal_;

	D3DXVECTOR2 vOffsetPos(0,0);	// 슬롯 교체를 위한 기본 오프셋 값
	D3DXVECTOR2 vOffsetAddPos(0,0); // 상단 슬롯을 위로 더 보내기 위한 추가 오프셋 값

	if( true == m_bSkillSlotSwap ) //슬롯 교체 중(B슬롯이 하단에 내려온 상태)
	{
		for( int i = 0 ; i < EQUIPPED_SKILL_SLOT_COUNT * 2; ++i )//4~7 상단슬롯
		{
			vOffsetPos.y = ( i >= 4 ) ? 51.f : -200.f;
			if( NULL != GetEquippedSlot(i) )
				GetEquippedSlot(i)->SetOffsetPos( vOffsetPos );
		}	
		vOffsetPos.y = 51.f;
		vOffsetAddPos.y = 200.f;

	}
	else //기본 상태( A슬롯이 하단에 내려온 상태)
	{
		for( int i = 0 ; i < EQUIPPED_SKILL_SLOT_COUNT * 2; ++i ) //4~7 상단슬롯
		{
			if( NULL != GetEquippedSlot(i) )
				vOffsetPos.y = ( i >= 4 ) ? -150.f : 0.f;
				GetEquippedSlot(i)->SetOffsetPos( vOffsetPos );
		}

		vOffsetPos.y = -200.f;
		vOffsetAddPos.y = 200.f;
	}
	SetOffsetPosSkillSLotStatic(vOffsetPos, vOffsetAddPos);
}
#endif //SKILL_SLOT_UI_TYPE_B

/** @function	: CreateSkillTreePortrait
	@brief		: 스킬 트리 가장 좌측에 붙는 초상화 관련 이미지 설정 함수
	@param		: 컨트롤 리스트 포인터, 최대 스킬 배열 높이
*/
void CX2UISkillTreeNew::CreateSkillTreePortrait( CKTDGUIContolList* pControlList )
{
	if( NULL == m_pDLGUISkillTree || 
		NULL == pControlList ||
		NULL == g_pData->GetMyUser() ||
		NULL == g_pData->GetMyUser()->GetSelectUnit()  ) 
	{
		return;
	}

	CX2Unit* pUnit = g_pData->GetMyUser()->GetSelectUnit();

	if ( NULL == pUnit )
		return;

	WCHAR buf[255] = {0,};

	/// 초상화 UI 개수 ( 하나의 전직당 5줄, 최종 줄을 계산하여 전직별 초상화 UI 표시 수량 설정  )
	int iPortraitUINum = pUnit->GetClassLevel() + 1;

	/// 초상화 설정
	for ( int i = 0; i < iPortraitUINum; ++i )
	{
		m_pDLGUISkillTree->OpenScriptFile( L"DLG_UI_Skill_Tree_Portrait.lua" );

		/// 초상화 박스 ------------------------------------------------------------------------------------------------------------------
		CKTDGUIStatic* pStaticPortraitBox = static_cast<CKTDGUIStatic*>( m_pDLGUISkillTree->GetControl( L"g_pStatic_PortraitBox" ) );

		if ( NULL != pStaticPortraitBox )
		{
			wstring wstrTectureKey = L"Bg_Portrait_Aisha";	/// 초상화 배경 텍스처 키값

			/// 유닛 타입에 따른 초상화 배경 설정
			switch ( pUnit->GetType() )
			{
			case CX2Unit::UT_ELSWORD:
#ifdef NEW_CHARACTER_EL // 디파인 제거 예정
			case CX2Unit::UT_ELESIS:
#endif //NEW_CHARACTER_EL
				{
					wstrTectureKey = L"Bg_Portrait_Elsword";
				}
				break;
			case CX2Unit::UT_ARME:
				{
					wstrTectureKey = L"Bg_Portrait_Aisha";
				}
				break;
			case CX2Unit::UT_LIRE:
				{
					wstrTectureKey = L"Bg_Portrait_Lena";
				}
				break;
			case CX2Unit::UT_RAVEN:
				{
					wstrTectureKey = L"Bg_Portrait_Raven";
				}
				break;
			case CX2Unit::UT_EVE:
				{
					wstrTectureKey = L"Bg_Portrait_Eve";
				}
				break;
			case CX2Unit::UT_CHUNG:
				{
					wstrTectureKey = L"Bg_Portrait_Chung";
				}
				break;
			case CX2Unit::UT_ARA:
				{
					wstrTectureKey = L"Bg_Portrait_Ara";
				}
				break;
#ifdef SERV_9TH_NEW_CHARACTER // 김태환 ( 캐릭터 추가용 )
			case CX2Unit::UT_ADD:
				{
					wstrTectureKey = L"Bg_Portrait_Add";
				}
				break;
#endif //SERV_9TH_NEW_CHARACTER
			}

			pStaticPortraitBox->GetPicture( 0 )->SetTex( L"DLG_UI_Common_Texture_NewSkill_01.tga", wstrTectureKey.c_str() );

			StringCchPrintf( buf, 255, L"pStaticPortraitBox_%d", i );
			pStaticPortraitBox->SetName(buf);

			pControlList->InsertItem( pStaticPortraitBox, ( i * MAX_ONE_CLASS_SKILL_ROW_NUM ), 0 );							/// 한 전직 5개의줄중 최상단

			SetUnitClassImageAndName( pStaticPortraitBox, pUnit->GetClass(), static_cast<CX2Unit::UNIT_CLASS_TYPE>( i ) );	/// 초상화 및 전직명 스트링 설정
		}

		/// 초상화 라인 하단 시작 --------------------------------------------------------------------------------------------------------
		CKTDGUIStatic* pStaticPortraitBottomStart = static_cast<CKTDGUIStatic*>( m_pDLGUISkillTree->GetControl( L"g_pStatic_PortraitBottomStart" ) );

		if ( NULL != pStaticPortraitBottomStart )
		{
			StringCchPrintf( buf, 255, L"g_pStatic_PortraitBottomStart_%d", i );
			pStaticPortraitBottomStart->SetName(buf);

			pControlList->InsertItem( pStaticPortraitBottomStart, ( i * MAX_ONE_CLASS_SKILL_ROW_NUM ) + 1, 0 );		/// 한 전직 5개의줄중 중간 상단
		}

		/// 초상화 라인	하단 중간 ----------------------------------------------------------------------------------------------------------
		CKTDGUIStatic* pStaticPortraitBottomMiddle = static_cast<CKTDGUIStatic*>( m_pDLGUISkillTree->GetControl( L"g_pStatic_PortraitBottomMiddle" ) );

		if ( NULL != pStaticPortraitBottomMiddle )
		{
			StringCchPrintf( buf, 255, L"g_pStatic_PortraitBottomMiddle_%d", i );
			pStaticPortraitBottomMiddle->SetName(buf);

			pControlList->InsertItem( pStaticPortraitBottomMiddle, ( i * MAX_ONE_CLASS_SKILL_ROW_NUM ) + 2, 0 );	/// 한 전직 5개의줄중 중간 하단
		}

		/// 초상화 라인	하단 끝 ----------------------------------------------------------------------------------------------------------
		CKTDGUIStatic* pStaticPortraitBottomEnd = static_cast<CKTDGUIStatic*>( m_pDLGUISkillTree->GetControl( L"g_pStatic_PortraitBottomEnd" ) );

		if ( NULL != pStaticPortraitBottomEnd )
		{
			StringCchPrintf( buf, 255, L"g_pStatic_PortraitBottomEnd_%d", i );
			pStaticPortraitBottomEnd->SetName(buf);

			pControlList->InsertItem( pStaticPortraitBottomEnd, ( i * MAX_ONE_CLASS_SKILL_ROW_NUM ) + 3, 0 );		/// 한 전직 5개의줄중 최하단
		}
	}

	return;
}

/** @function	: SetSelectSkillSlotPosition
	@brief		: 2지선다 스킬 중, 1개만 있을 때 중앙으로 위치 이동시키는 함수
	@param		: 최대 스킬 배열 높이
*/
void CX2UISkillTreeNew::SetSelectSkillSlotPosition()
{
	for ( int i = 0; i <= m_iMaxSkillRowNum; ++i )
	{
		/// 2지 선다 스킬 중, 우측 스킬이 존재하는지 검사
		std::map<SkillPositionKey, CX2SkillTree::SKILL_ID>::iterator itSecond = m_mapSkillPosition.find( SkillPositionKey( 1, i ) );

		if( itSecond != m_mapSkillPosition.end() )	/// 우측 스킬이 존재한다면, 좌측 스킬 위치 이동 시킬 필요 없다.
			continue;

		/// 2지 선다 좌측 스킬 아이디 반환
		std::map<SkillPositionKey, CX2SkillTree::SKILL_ID>::iterator itFirst = m_mapSkillPosition.find( SkillPositionKey( 0, i ) );

		if ( itFirst != m_mapSkillPosition.end() )
		{
			CX2SkillTree::SKILL_ID eSkillID = itFirst->second;

			WCHAR buf[256] = {0,};

			/// 슬롯 위치 변경
			StringCchPrintf( buf, 255, L"g_pSlot_SkillTree_%d", eSkillID );

			CKTDGUISlot* pSlotSkillTree = static_cast<CKTDGUISlot*>( m_pDLGUISkillTree->GetControl( buf ) );

			if ( NULL != pSlotSkillTree )
			{
				CKTDGUIControl::UIPointData* pPoint		= new CKTDGUIControl::UIPointData();
				pPoint->fChangeTime						= 0.0f;
				pPoint->addSize							= D3DXVECTOR2( 46.f, 46.f );
				pPoint->color							= D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f );
				pPoint->leftTopPoint					= D3DXVECTOR2( 162.f, 16.f );

				CKTDGUIControl::UIPointData* pOverPoint = new CKTDGUIControl::UIPointData();
				pOverPoint->fChangeTime					= 0.0f;
				pOverPoint->color						= D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f );
				pOverPoint->leftTopPoint				= D3DXVECTOR2( 156.f, 9.f );

				pSlotSkillTree->SetPoint( pPoint );
				pSlotSkillTree->SetMouseOverPoint( pOverPoint );
			}

			/// 배경 백판 가운데 라인 설정
			StringCchPrintf( buf, 256, L"g_pStatic_BackGround_%d", eSkillID );
			
			CKTDGUIStatic* pStaticBackGround = static_cast<CKTDGUIStatic*>( m_pDLGUISkillTree->GetControl( buf ) );

			if ( NULL != pStaticBackGround && NULL != pStaticBackGround->GetPicture( 2 ) )
			{
				pStaticBackGround->GetPicture( 2 )->SetShow( false );	/// 2지선다 스킬인데 한개만 있다면, 가운데 구분선 해제
			}


			/// 스킬 백판 위치 변경 
			StringCchPrintf( buf, 255, L"g_pStaticSkillBackGround_%d", eSkillID );

			CKTDGUIStatic* pStaticSkillBackGround = static_cast<CKTDGUIStatic*>( m_pDLGUISkillTree->GetControl( buf ) );

			if ( NULL != pStaticSkillBackGround && 
				 NULL != pStaticSkillBackGround->GetPicture( 0 ) && 
				 NULL != pStaticSkillBackGround->GetPicture( 1 ) )
			{
				pStaticSkillBackGround->GetPicture( 0 )->SetPos( D3DXVECTOR2( 159.f, 13.f ) );

				/// 요구 SP 백판 위치 지정
				D3DXVECTOR2 vecSkillBGPos = pStaticSkillBackGround->GetPicture( 0 )->GetPos();
				pStaticSkillBackGround->GetPicture( 1 )->SetPos( D3DXVECTOR2( vecSkillBGPos.x + 21.f, vecSkillBGPos.y + 52.f ) );
			}


			/// 스킬 습득 버튼 위치 변경 
			StringCchPrintf( buf, 255, L"ButtonSkillLearn_%d", eSkillID );

			CKTDGUIButton* pButtonSkillLearn = static_cast<CKTDGUIButton*>( m_pDLGUISkillTree->GetControl( buf ) );

			if ( NULL != pButtonSkillLearn )
			{
				CKTDGUIControl::UIPointData* pNormalPoint = new CKTDGUIControl::UIPointData();
				pNormalPoint->fChangeTime	= 0.0f;
				pNormalPoint->leftTopPoint	= D3DXVECTOR2( 161.f, 65.f );
				pNormalPoint->color			= D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f );

				CKTDGUIControl::UIPointData* pOverPoint = new CKTDGUIControl::UIPointData();
				pOverPoint->fChangeTime		= 0.0f;
				pOverPoint->leftTopPoint	= D3DXVECTOR2( 159.f, 63.f );
				pOverPoint->color			= D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f );

				CKTDGUIControl::UIPointData* pDownPoint = new CKTDGUIControl::UIPointData();
				pDownPoint->fChangeTime		= 0.0f;
				pDownPoint->addSize			= D3DXVECTOR2( -2.f, -2.f );
				pDownPoint->leftTopPoint	= D3DXVECTOR2( 160.f, 64.f );
				pDownPoint->color			= D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f );

				CKTDGUIControl::UIPointData* pDisablePoint = new CKTDGUIControl::UIPointData();
				pDisablePoint->fChangeTime	= 0.0f;
				pDisablePoint->leftTopPoint	= D3DXVECTOR2( 161.f, 65.f );
				pDisablePoint->color		= D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f );

				pButtonSkillLearn->SetNormalPoint( pNormalPoint );
				pButtonSkillLearn->SetOverPoint( pOverPoint );
				pButtonSkillLearn->SetDownPoint( pDownPoint );
				pButtonSkillLearn->SetDisablePoint( pDisablePoint );
			}



			/// 스킬 툴팁용 버튼 위치 변경 
			StringCchPrintf( buf, 255, L"ButtonSkillDesc_%d", eSkillID );

			CKTDGUIButton* pButtonSkillDesc = static_cast<CKTDGUIButton*>( m_pDLGUISkillTree->GetControl( buf ) );

			if ( NULL != pButtonSkillDesc )
			{
				CKTDGUIControl::UIPointData* pNormalPoint = new CKTDGUIControl::UIPointData();
				pNormalPoint->fChangeTime	= 0.0f;
				pNormalPoint->leftTopPoint	= D3DXVECTOR2( 161.f, 65.f );
				pNormalPoint->color			= D3DXCOLOR( 1.0f, 1.0f, 1.0f, 0.0f );

				CKTDGUIControl::UIPointData* pOverPoint = new CKTDGUIControl::UIPointData();
				pOverPoint->fChangeTime		= 0.0f;
				pOverPoint->leftTopPoint	= D3DXVECTOR2( 159.f, 63.f );
				pOverPoint->color			= D3DXCOLOR( 1.0f, 1.0f, 1.0f, 0.0f );

				CKTDGUIControl::UIPointData* pDownPoint = new CKTDGUIControl::UIPointData();
				pDownPoint->fChangeTime		= 0.0f;
				pDownPoint->addSize			= D3DXVECTOR2( -2.f, -2.f );
				pDownPoint->leftTopPoint	= D3DXVECTOR2( 160.f, 64.f );
				pDownPoint->color			= D3DXCOLOR( 1.0f, 1.0f, 1.0f, 0.0f );

				CKTDGUIControl::UIPointData* pDisablePoint = new CKTDGUIControl::UIPointData();
				pDisablePoint->fChangeTime	= 0.0f;
				pDisablePoint->leftTopPoint	= D3DXVECTOR2( 161.f, 65.f );
				pDisablePoint->color		= D3DXCOLOR( 1.0f, 1.0f, 1.0f, 0.0f );

				pButtonSkillDesc->SetNormalPoint( pNormalPoint );
				pButtonSkillDesc->SetOverPoint( pOverPoint );
				pButtonSkillDesc->SetDownPoint( pDownPoint );
				pButtonSkillDesc->SetDisablePoint( pDisablePoint );
				pButtonSkillDesc->SetGuideDescAbsolutePos( pNormalPoint->leftTopPoint );
			}



			/// 스킬 초기화 버튼 위치 변경 
			StringCchPrintf( buf, 255, L"ButtonSkillInit_%d", eSkillID );

			CKTDGUIButton* pButtonSkillInit = static_cast<CKTDGUIButton*>( m_pDLGUISkillTree->GetControl( buf ) );

			if ( NULL != pButtonSkillInit )
			{
				CKTDGUIControl::UIPointData* pNormalPoint = new CKTDGUIControl::UIPointData();
				pNormalPoint->fChangeTime	= 0.0f;
				pNormalPoint->leftTopPoint	= D3DXVECTOR2( 162.f, 65.f );
				pNormalPoint->color			= D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f );

				CKTDGUIControl::UIPointData* pOverPoint = new CKTDGUIControl::UIPointData();
				pOverPoint->fChangeTime		= 0.0f;
				pOverPoint->leftTopPoint	= D3DXVECTOR2( 162.f, 65.f );
				pOverPoint->color			= D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f );

				CKTDGUIControl::UIPointData* pDownPoint = new CKTDGUIControl::UIPointData();
				pDownPoint->fChangeTime		= 0.0f;
				pDownPoint->addSize			= D3DXVECTOR2( -2.f, -2.f );
				pDownPoint->leftTopPoint	= D3DXVECTOR2( 163.f, 66.f );
				pDownPoint->color			= D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f );

				CKTDGUIControl::UIPointData* pDisablePoint = new CKTDGUIControl::UIPointData();
				pDisablePoint->fChangeTime	= 0.0f;
				pDisablePoint->leftTopPoint	= D3DXVECTOR2( 162.f, 65.f );
				pDisablePoint->color		= D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f );

				pButtonSkillInit->SetNormalPoint( pNormalPoint );
				pButtonSkillInit->SetOverPoint( pOverPoint );
				pButtonSkillInit->SetDownPoint( pDownPoint );
				pButtonSkillInit->SetDisablePoint( pDisablePoint );
			}



			/// 정보 스트링 위치 변경 
			StringCchPrintf( buf, 255, L"g_pStaticStringInfo_%d", eSkillID );

			CKTDGUIStatic* pStaticStringInfo = static_cast<CKTDGUIStatic*>( m_pDLGUISkillTree->GetControl( buf ) );

			if ( NULL != pStaticStringInfo && NULL != pStaticStringInfo->GetString( 0 ) )
			{
				pStaticStringInfo->GetString( 0 )->pos = D3DXVECTOR2( 194.f, 66.f );
			}


			/// 마스터 스킬 이미지 위치 변경 
			StringCchPrintf( buf, 255, L"g_pStaticSkillMaster_%d", eSkillID );

			CKTDGUIStatic* pStaticSkillMaster = static_cast<CKTDGUIStatic*>( m_pDLGUISkillTree->GetControl( buf ) );

			if ( NULL != pStaticSkillMaster && NULL != pStaticSkillMaster->GetPicture( 0 ) )
			{
				pStaticSkillMaster->GetPicture( 0 )->SetPos( D3DXVECTOR2( 162.f, 65.f ) );
			}


			/// 자물쇠 이미지 위치 변경 
			StringCchPrintf( buf, 255, L"g_pStaticSkill_Lock_%d", eSkillID );

			CKTDGUIStatic* pStaticSkill_Lock = static_cast<CKTDGUIStatic*>( m_pDLGUISkillTree->GetControl( buf ) );

			if ( NULL != pStaticSkill_Lock && NULL != pStaticSkill_Lock->GetPicture( 0 ) )
			{
				pStaticSkill_Lock->GetPicture( 0 )->SetPos( D3DXVECTOR2( 170.f, 19.f ) );
			}
		}
	}
}

/** @function	: SetDivisionLine
	@brief		: 전직별 스킬 구분선 설정 함수
	@param		: 컨트롤 리스트 포인터
*/
void CX2UISkillTreeNew::SetDivisionLine( CKTDGUIContolList* pControlList )
{
	if( NULL == pControlList ||
		NULL == g_pData ||
		NULL == g_pData->GetMyUser() ||
		NULL == g_pData->GetMyUser()->GetSelectUnit() )
	{
		return;
	}

	CX2Unit* pUnit = g_pData->GetMyUser()->GetSelectUnit();

	if ( NULL != pUnit )
	{
		WCHAR buf[255] = {0,};

		/// 초상화 UI 개수 ( 하나의 전직당 5줄, 최종 줄을 계산하여 전직별 초상화 UI 표시 수량 설정  )
		CX2Unit::UNIT_CLASS_TYPE eUnitClassLevel = static_cast<CX2Unit::UNIT_CLASS_TYPE>( pUnit->GetClassLevel() );


		/// 스킬 구분선( 가로 ) 설정 ---------------------------------------------------------------------------------------------------------
		CKTDGUIStatic* pStatic_HorizonLine1 = static_cast<CKTDGUIStatic*>( m_pDLGUISkillTree->GetControl( L"g_pStatic_HorizonLine1" ) );
		CKTDGUIStatic* pStatic_HorizonLine2 = static_cast<CKTDGUIStatic*>( m_pDLGUISkillTree->GetControl( L"g_pStatic_HorizonLine2" ) );

		if ( NULL != pStatic_HorizonLine1 && NULL != pStatic_HorizonLine2 )
		{
			switch ( eUnitClassLevel )
			{
			case CX2Unit::UCT_FIRST_CLASS:	/// 노전직 -> 1차 전직
				{
					pStatic_HorizonLine1->SetShow(true);
					pStatic_HorizonLine2->SetShow(false);

					pControlList->InsertItem( pStatic_HorizonLine1, MAX_ONE_CLASS_SKILL_ROW_NUM - 1 , 0 );			/// 노전직 스킬 마지막 줄에 구분선 추가
				} break;
			case CX2Unit::UCT_SECOND_CLASS: /// 노전직 -> 1차 전직 -> 2차 전직
				{
					pStatic_HorizonLine1->SetShow(true);
					pStatic_HorizonLine2->SetShow(true);

					pControlList->InsertItem( pStatic_HorizonLine1, MAX_ONE_CLASS_SKILL_ROW_NUM - 1 , 0 );			/// 노전직 스킬 마지막 줄에 구분선 추가
					pControlList->InsertItem( pStatic_HorizonLine2, ( MAX_ONE_CLASS_SKILL_ROW_NUM * 2 ) - 1, 0 );	/// 1차 전직 스킬 마지막 줄에 구분선 추가
				} break;
			default: /// 노전직
				{
					pStatic_HorizonLine1->SetShow(false);	/// 구분선 없음
					pStatic_HorizonLine2->SetShow(false);
				} break;
			}
		}
	}
}

/** @function	: UpdateAtMoveScrollBar
	@brief		: 스크롤 바가 움직일 때 변경되어야 할 정보를 처리하는 함수
*/
void CX2UISkillTreeNew::UpdateAtMoveScrollBar()
{
	if (	NULL == m_pDLGUISkillTree ||
			NULL == g_pData ||
			NULL == g_pData->GetMyUser() ||
			NULL == g_pData->GetMyUser()->GetSelectUnit() )
		{
			return;
		}

	/// 전직별 스킬 구분선이 스킬 창 아래에 있을 때, 예외적으로 숨겨주기
	CKTDGUIContolList*	pControlList			= static_cast<CKTDGUIContolList*>( m_pDLGUISkillTree->GetControl( L"SkillTreeControlList" ) );
	CKTDGUIStatic*		pStatic_HorizonLine1	= static_cast<CKTDGUIStatic*>( m_pDLGUISkillTree->GetControl( L"g_pStatic_HorizonLine1" ) );
	CKTDGUIStatic*		pStatic_HorizonLine2	= static_cast<CKTDGUIStatic*>( m_pDLGUISkillTree->GetControl( L"g_pStatic_HorizonLine2" ) );
	CX2Unit*			pUnit					= g_pData->GetMyUser()->GetSelectUnit();

	if ( NULL != pUnit && NULL != pControlList && NULL != pStatic_HorizonLine1 && NULL != pStatic_HorizonLine2 )
	{
		CX2Unit::UNIT_CLASS_TYPE	eUnitClassType	= static_cast<CX2Unit::UNIT_CLASS_TYPE >( pUnit->GetClassLevel() );	/// 현제 전직 등급
		int							iTopTier		= pControlList->GetIndexY();	/// 현재 스크롤 인덱스

		{
			if ( CX2Unit::UCT_FIRST_CLASS <= eUnitClassType )	/// 1차 전직일 때 ( 기본 전직 -> 1차 전직 스킬 구분선이 존재할 때 )
			{
				if ( 0 == iTopTier )		/// 현재 첫번째 스킬 구분선이 스킬창 가장 밑에 배치되어 있을 때, 숨김
					pStatic_HorizonLine1->SetShow( false );
				else
					pStatic_HorizonLine1->SetShow( true );
			}

			if ( CX2Unit::UCT_SECOND_CLASS <= eUnitClassType )	/// 2차 전직일 때 ( 1차 전직 -> 2차 전직 스킬 구분선이 존재할 때 )
			{
				if ( 5 == iTopTier )		/// 현재 두번째 스킬 구분선이 스킬창 가장 밑에 배치되어 있을 때, 숨김
					pStatic_HorizonLine2->SetShow( false );
				else
					pStatic_HorizonLine2->SetShow( true );
			}
		}
	}
}
/** @function	: SetUnitClassImageAndName
	@brief		: 스킬 트리 초상화의 전직 이미지 및 전직명 설정 함수
	@Param		: 변경할 초상화 컨트롤, 현재 유닛 클레스
*/
void CX2UISkillTreeNew::SetUnitClassImageAndName(	CKTDGUIStatic* pStatic,
													CX2Unit::UNIT_CLASS eUnitClass /*= CX2Unit::UC_ARME_VOID_PRINCESS*/,
													CX2Unit::UNIT_CLASS_TYPE UnitClassType /*= CX2Unit::UCT_BASIC_CLASS*/ )
{
	if ( NULL == pStatic )
		return;

	wstring wstrFileName	= L"";
	wstring wstrPieceName	= L"";

	switch ( eUnitClass )
	{
		/// 엘소드
	case CX2Unit::UC_ELSWORD_SWORDMAN:			/// 기본 전직
		{
			if ( CX2Unit::UCT_BASIC_CLASS ==  UnitClassType )	/// 기본 전직 정보 설정
			{
				pStatic->GetString( 0 )->msg = GET_STRING( STR_ID_25010 );
				eUnitClass = CX2Unit::UC_ELSWORD_SWORDMAN;
			}
		} break;
	case CX2Unit::UC_ELSWORD_KNIGHT:			/// 1차 전직
	case CX2Unit::UC_ELSWORD_LORD_KNIGHT:		/// 2차 전직
		{
			switch ( UnitClassType )					/// 원하는 전직 등급에 따른 정보 설정
			{
			case CX2Unit::UCT_BASIC_CLASS:	/// 기본 전직 정보 설정
				{
					pStatic->GetString( 0 )->msg = GET_STRING( STR_ID_25010 );
					eUnitClass = CX2Unit::UC_ELSWORD_SWORDMAN;
				} break;
			case CX2Unit::UCT_FIRST_CLASS:	/// 1차 전직 정보 설정
				{
					pStatic->GetString( 0 )->msg = GET_STRING( STR_ID_25011 );
					eUnitClass = CX2Unit::UC_ELSWORD_KNIGHT;
				} break;
			case CX2Unit::UCT_SECOND_CLASS:	/// 2치 전직 정보 설정
				{
					pStatic->GetString( 0 )->msg = GET_STRING( STR_ID_25012 );
					eUnitClass = CX2Unit::UC_ELSWORD_LORD_KNIGHT;
				} break;
			}
		} break;
	case CX2Unit::UC_ELSWORD_MAGIC_KNIGHT:		/// 1차 전직
	case CX2Unit::UC_ELSWORD_RUNE_SLAYER:		/// 2차 전직
		{
			switch ( UnitClassType )					/// 원하는 전직 등급에 따른 정보 설정
			{
			case CX2Unit::UCT_BASIC_CLASS:	/// 기본 전직 정보 설정
				{
					pStatic->GetString( 0 )->msg = GET_STRING( STR_ID_25010 );
					eUnitClass = CX2Unit::UC_ELSWORD_SWORDMAN;
				} break;
			case CX2Unit::UCT_FIRST_CLASS:	/// 1차 전직 정보 설정
				{
					pStatic->GetString( 0 )->msg = GET_STRING( STR_ID_25013 );
					eUnitClass = CX2Unit::UC_ELSWORD_MAGIC_KNIGHT;
				} break;
			case CX2Unit::UCT_SECOND_CLASS:	/// 2치 전직 정보 설정
				{
					pStatic->GetString( 0 )->msg = GET_STRING( STR_ID_25014 );
					eUnitClass = CX2Unit::UC_ELSWORD_RUNE_SLAYER;
				} break;
			}
		} break;
	case CX2Unit::UC_ELSWORD_SHEATH_KNIGHT:		/// 1차 전직
	case CX2Unit::UC_ELSWORD_INFINITY_SWORD:	/// 2차 전직
		{
			switch ( UnitClassType )					/// 원하는 전직 등급에 따른 정보 설정
			{
			case CX2Unit::UCT_BASIC_CLASS:	/// 기본 전직 정보 설정
				{
					pStatic->GetString( 0 )->msg = GET_STRING( STR_ID_25010 );
					eUnitClass = CX2Unit::UC_ELSWORD_SWORDMAN;
				} break;
			case CX2Unit::UCT_FIRST_CLASS:	/// 1차 전직 정보 설정
				{
					pStatic->GetString( 0 )->msg = GET_STRING( STR_ID_25015 );
					eUnitClass = CX2Unit::UC_ELSWORD_SHEATH_KNIGHT;
				} break;
			case CX2Unit::UCT_SECOND_CLASS:	/// 2치 전직 정보 설정
				{
					pStatic->GetString( 0 )->msg = GET_STRING( STR_ID_25016 );
					eUnitClass = CX2Unit::UC_ELSWORD_INFINITY_SWORD;
				} break;
			}
		} break;

		/// 아이샤
	case CX2Unit::UC_ARME_VIOLET_MAGE:			/// 기본 전직
		{
			if ( CX2Unit::UCT_BASIC_CLASS ==  UnitClassType )	/// 기본 전직 정보 설정
			{
				pStatic->GetString( 0 )->msg = GET_STRING( STR_ID_25017 );
				eUnitClass = CX2Unit::UC_ARME_VIOLET_MAGE;
			}
		} break;
	case CX2Unit::UC_ARME_HIGH_MAGICIAN:		/// 1차 전직
	case CX2Unit::UC_ARME_ELEMENTAL_MASTER:		/// 2차 전직
		{
			switch ( UnitClassType )					/// 원하는 전직 등급에 따른 정보 설정
			{
			case CX2Unit::UCT_BASIC_CLASS:	/// 기본 전직 정보 설정
				{
					pStatic->GetString( 0 )->msg = GET_STRING( STR_ID_25017 );
					eUnitClass = CX2Unit::UC_ARME_VIOLET_MAGE;
				} break;
			case CX2Unit::UCT_FIRST_CLASS:	/// 1차 전직 정보 설정
				{
					pStatic->GetString( 0 )->msg = GET_STRING( STR_ID_25018 );
					eUnitClass = CX2Unit::UC_ARME_HIGH_MAGICIAN;
				} break;
			case CX2Unit::UCT_SECOND_CLASS:	/// 2치 전직 정보 설정
				{
					pStatic->GetString( 0 )->msg = GET_STRING( STR_ID_25019 );
					eUnitClass = CX2Unit::UC_ARME_ELEMENTAL_MASTER;
				} break;
			}
		} break;
	case CX2Unit::UC_ARME_DARK_MAGICIAN:		/// 1차 전직
	case CX2Unit::UC_ARME_VOID_PRINCESS:		/// 2차 전직
		{
			switch ( UnitClassType )					/// 원하는 전직 등급에 따른 정보 설정
			{
			case CX2Unit::UCT_BASIC_CLASS:	/// 기본 전직 정보 설정
				{
					pStatic->GetString( 0 )->msg = GET_STRING( STR_ID_25017 );
					eUnitClass = CX2Unit::UC_ARME_VIOLET_MAGE;
				} break;
			case CX2Unit::UCT_FIRST_CLASS:	/// 1차 전직 정보 설정
				{
					pStatic->GetString( 0 )->msg = GET_STRING( STR_ID_25020 );
					eUnitClass = CX2Unit::UC_ARME_DARK_MAGICIAN;
				} break;
			case CX2Unit::UCT_SECOND_CLASS:	/// 2치 전직 정보 설정
				{
					pStatic->GetString( 0 )->msg = GET_STRING( STR_ID_25021 );
					eUnitClass = CX2Unit::UC_ARME_VOID_PRINCESS;
				} break;
			}
		} break;
	case CX2Unit::UC_ARME_BATTLE_MAGICIAN:		/// 1차 전직
	case CX2Unit::UC_ARME_DIMENSION_WITCH:		/// 2차 전직
		{
			switch ( UnitClassType )					/// 원하는 전직 등급에 따른 정보 설정
			{
			case CX2Unit::UCT_BASIC_CLASS:	/// 기본 전직 정보 설정
				{
					pStatic->GetString( 0 )->msg = GET_STRING( STR_ID_25017 );
					eUnitClass = CX2Unit::UC_ARME_VIOLET_MAGE;
				} break;
			case CX2Unit::UCT_FIRST_CLASS:	/// 1차 전직 정보 설정
				{
					pStatic->GetString( 0 )->msg = GET_STRING( STR_ID_25022 );
					eUnitClass = CX2Unit::UC_ARME_BATTLE_MAGICIAN;
				} break;
			case CX2Unit::UCT_SECOND_CLASS:	/// 2치 전직 정보 설정
				{
					pStatic->GetString( 0 )->msg = GET_STRING( STR_ID_25023 );
					eUnitClass = CX2Unit::UC_ARME_DIMENSION_WITCH;
				} break;
			}
		} break;

		/// 레나
	case CX2Unit::UC_LIRE_ELVEN_RANGER:			/// 기본 전직
		{
			if ( CX2Unit::UCT_BASIC_CLASS ==  UnitClassType )	/// 기본 전직 정보 설정
			{
				pStatic->GetString( 0 )->msg = GET_STRING( STR_ID_25024 );
				eUnitClass = CX2Unit::UC_LIRE_ELVEN_RANGER;
			}
		} break;
	case CX2Unit::UC_LIRE_COMBAT_RANGER:		/// 1차 전직
	case CX2Unit::UC_LIRE_WIND_SNEAKER:			/// 2차 전직
		{
			switch ( UnitClassType )					/// 원하는 전직 등급에 따른 정보 설정
			{
			case CX2Unit::UCT_BASIC_CLASS:	/// 기본 전직 정보 설정
				{
					pStatic->GetString( 0 )->msg = GET_STRING( STR_ID_25024 );
					eUnitClass = CX2Unit::UC_LIRE_ELVEN_RANGER;
				} break;
			case CX2Unit::UCT_FIRST_CLASS:	/// 1차 전직 정보 설정
				{
					pStatic->GetString( 0 )->msg = GET_STRING( STR_ID_25025 );
					eUnitClass = CX2Unit::UC_LIRE_COMBAT_RANGER;
				} break;
			case CX2Unit::UCT_SECOND_CLASS:	/// 2치 전직 정보 설정
				{
					pStatic->GetString( 0 )->msg = GET_STRING( STR_ID_25026 );
					eUnitClass = CX2Unit::UC_LIRE_WIND_SNEAKER;
				} break;
			}
		} break;
	case CX2Unit::UC_LIRE_SNIPING_RANGER:		/// 1차 전직
	case CX2Unit::UC_LIRE_GRAND_ARCHER:			/// 2차 전직
		{
			switch ( UnitClassType )					/// 원하는 전직 등급에 따른 정보 설정
			{
			case CX2Unit::UCT_BASIC_CLASS:	/// 기본 전직 정보 설정
				{
					pStatic->GetString( 0 )->msg = GET_STRING( STR_ID_25024 );
					eUnitClass = CX2Unit::UC_LIRE_ELVEN_RANGER;
				} break;
			case CX2Unit::UCT_FIRST_CLASS:	/// 1차 전직 정보 설정
				{
					pStatic->GetString( 0 )->msg = GET_STRING( STR_ID_25027 );
					eUnitClass = CX2Unit::UC_LIRE_SNIPING_RANGER;
				} break;
			case CX2Unit::UCT_SECOND_CLASS:	/// 2치 전직 정보 설정
				{
					pStatic->GetString( 0 )->msg = GET_STRING( STR_ID_25028 );
					eUnitClass = CX2Unit::UC_LIRE_GRAND_ARCHER;
				} break;
			}
		} break;
	case CX2Unit::UC_LIRE_TRAPPING_RANGER:		/// 1차 전직
	case CX2Unit::UC_LIRE_NIGHT_WATCHER:		/// 2차 전직
		{
			switch ( UnitClassType )					/// 원하는 전직 등급에 따른 정보 설정
			{
			case CX2Unit::UCT_BASIC_CLASS:	/// 기본 전직 정보 설정
				{
					pStatic->GetString( 0 )->msg = GET_STRING( STR_ID_25024 );
					eUnitClass = CX2Unit::UC_LIRE_ELVEN_RANGER;
				} break;
			case CX2Unit::UCT_FIRST_CLASS:	/// 1차 전직 정보 설정
				{
					pStatic->GetString( 0 )->msg = GET_STRING( STR_ID_25029 );
					eUnitClass = CX2Unit::UC_LIRE_TRAPPING_RANGER;
				} break;
			case CX2Unit::UCT_SECOND_CLASS:	/// 2치 전직 정보 설정
				{
					pStatic->GetString( 0 )->msg = GET_STRING( STR_ID_25030 );
					eUnitClass = CX2Unit::UC_LIRE_NIGHT_WATCHER;
				} break;
			}
		} break;

		/// 레이븐
	case CX2Unit::UC_RAVEN_FIGHTER:				/// 기본 전직
		{
			if ( CX2Unit::UCT_BASIC_CLASS ==  UnitClassType )	/// 기본 전직 정보 설정
			{
				pStatic->GetString( 0 )->msg = GET_STRING( STR_ID_25031 );
				eUnitClass = CX2Unit::UC_RAVEN_FIGHTER;
			}
		} break;
	case CX2Unit::UC_RAVEN_SOUL_TAKER:			/// 1차 전직
	case CX2Unit::UC_RAVEN_BLADE_MASTER:		/// 2차 전직
		{
			switch ( UnitClassType )					/// 원하는 전직 등급에 따른 정보 설정
			{
			case CX2Unit::UCT_BASIC_CLASS:	/// 기본 전직 정보 설정
				{
					pStatic->GetString( 0 )->msg = GET_STRING( STR_ID_25031 );
					eUnitClass = CX2Unit::UC_RAVEN_FIGHTER;
				} break;
			case CX2Unit::UCT_FIRST_CLASS:	/// 1차 전직 정보 설정
				{
					pStatic->GetString( 0 )->msg = GET_STRING( STR_ID_25032 );
					eUnitClass = CX2Unit::UC_RAVEN_SOUL_TAKER;
				} break;
			case CX2Unit::UCT_SECOND_CLASS:	/// 2치 전직 정보 설정
				{
					pStatic->GetString( 0 )->msg = GET_STRING( STR_ID_25033 );
					eUnitClass = CX2Unit::UC_RAVEN_BLADE_MASTER;
				} break;
			}
		} break;
	case CX2Unit::UC_RAVEN_OVER_TAKER:			/// 1차 전직
	case CX2Unit::UC_RAVEN_RECKLESS_FIST:		/// 2차 전직
		{
			switch ( UnitClassType )					/// 원하는 전직 등급에 따른 정보 설정
			{
			case CX2Unit::UCT_BASIC_CLASS:	/// 기본 전직 정보 설정
				{
					pStatic->GetString( 0 )->msg = GET_STRING( STR_ID_25031 );
					eUnitClass = CX2Unit::UC_RAVEN_FIGHTER;
				} break;
			case CX2Unit::UCT_FIRST_CLASS:	/// 1차 전직 정보 설정
				{
					pStatic->GetString( 0 )->msg = GET_STRING( STR_ID_25034 );
					eUnitClass = CX2Unit::UC_RAVEN_OVER_TAKER;
				} break;
			case CX2Unit::UCT_SECOND_CLASS:	/// 2치 전직 정보 설정
				{
					pStatic->GetString( 0 )->msg = GET_STRING( STR_ID_25035 );
					eUnitClass = CX2Unit::UC_RAVEN_RECKLESS_FIST;
				} break;
			}
		} break;
	case CX2Unit::UC_RAVEN_WEAPON_TAKER:		/// 1차 전직
	case CX2Unit::UC_RAVEN_VETERAN_COMMANDER:	/// 2차 전직
		{
			switch ( UnitClassType )					/// 원하는 전직 등급에 따른 정보 설정
			{
			case CX2Unit::UCT_BASIC_CLASS:	/// 기본 전직 정보 설정
				{
					pStatic->GetString( 0 )->msg = GET_STRING( STR_ID_25031 );
					eUnitClass = CX2Unit::UC_RAVEN_FIGHTER;
				} break;
			case CX2Unit::UCT_FIRST_CLASS:	/// 1차 전직 정보 설정
				{
					pStatic->GetString( 0 )->msg = GET_STRING( STR_ID_25036 );
					eUnitClass = CX2Unit::UC_RAVEN_WEAPON_TAKER;
				} break;
			case CX2Unit::UCT_SECOND_CLASS:	/// 2치 전직 정보 설정
				{
					pStatic->GetString( 0 )->msg = GET_STRING( STR_ID_25037 );
					eUnitClass = CX2Unit::UC_RAVEN_VETERAN_COMMANDER;
				} break;
			}
		} break;

		/// 이브
	case CX2Unit::UC_EVE_NASOD:					/// 기본 전직
		{
			if ( CX2Unit::UCT_BASIC_CLASS == UnitClassType )	/// 기본 전직 정보 설정
			{
				pStatic->GetString( 0 )->msg = GET_STRING( STR_ID_25038 );
				eUnitClass = CX2Unit::UC_EVE_NASOD;
			}
		} break;
	case CX2Unit::UC_EVE_EXOTIC_GEAR:			/// 1차 전직
	case CX2Unit::UC_EVE_CODE_NEMESIS:			/// 2차 전직
		{
			switch ( UnitClassType )					/// 원하는 전직 등급에 따른 정보 설정
			{
			case CX2Unit::UCT_BASIC_CLASS:	/// 기본 전직 정보 설정
				{
					pStatic->GetString( 0 )->msg = GET_STRING( STR_ID_25038 );
					eUnitClass = CX2Unit::UC_EVE_NASOD;
				} break;
			case CX2Unit::UCT_FIRST_CLASS:	/// 1차 전직 정보 설정
				{
					pStatic->GetString( 0 )->msg = GET_STRING( STR_ID_25039 );
					eUnitClass = CX2Unit::UC_EVE_EXOTIC_GEAR;
				} break;
			case CX2Unit::UCT_SECOND_CLASS:	/// 2치 전직 정보 설정
				{
					pStatic->GetString( 0 )->msg = GET_STRING( STR_ID_25040 );
					eUnitClass = CX2Unit::UC_EVE_CODE_NEMESIS;
				} break;
			}
		} break;
	case CX2Unit::UC_EVE_ARCHITECTURE:			/// 1차 전직
	case CX2Unit::UC_EVE_CODE_EMPRESS:			/// 2차 전직
		{
			switch ( UnitClassType )					/// 원하는 전직 등급에 따른 정보 설정
			{
			case CX2Unit::UCT_BASIC_CLASS:	/// 기본 전직 정보 설정
				{
					pStatic->GetString( 0 )->msg = GET_STRING( STR_ID_25038 );
					eUnitClass = CX2Unit::UC_EVE_NASOD;
				} break;
			case CX2Unit::UCT_FIRST_CLASS:	/// 1차 전직 정보 설정
				{
					pStatic->GetString( 0 )->msg = GET_STRING( STR_ID_25041 );
					eUnitClass = CX2Unit::UC_EVE_ARCHITECTURE;
				} break;
			case CX2Unit::UCT_SECOND_CLASS:	/// 2치 전직 정보 설정
				{
					pStatic->GetString( 0 )->msg = GET_STRING( STR_ID_25042 );
					eUnitClass = CX2Unit::UC_EVE_CODE_EMPRESS;
				} break;
			}
		} break;
	case CX2Unit::UC_EVE_ELECTRA:				/// 1차 전직
	case CX2Unit::UC_EVE_BATTLE_SERAPH:			/// 2차 전직
		{
			switch ( UnitClassType )					/// 원하는 전직 등급에 따른 정보 설정
			{
			case CX2Unit::UCT_BASIC_CLASS:	/// 기본 전직 정보 설정
				{
					pStatic->GetString( 0 )->msg = GET_STRING( STR_ID_25038 );
					eUnitClass = CX2Unit::UC_EVE_NASOD;
				} break;
			case CX2Unit::UCT_FIRST_CLASS:	/// 1차 전직 정보 설정
				{
					pStatic->GetString( 0 )->msg = GET_STRING( STR_ID_25043 );
					eUnitClass = CX2Unit::UC_EVE_ELECTRA;
				} break;
			case CX2Unit::UCT_SECOND_CLASS:	/// 2치 전직 정보 설정
				{
					pStatic->GetString( 0 )->msg = GET_STRING( STR_ID_25044 );
					eUnitClass = CX2Unit::UC_EVE_BATTLE_SERAPH;
				} break;
			}
		} break;

		/// 청
	case CX2Unit::UC_CHUNG_IRON_CANNON:			/// 기본 전직
		{
			if ( CX2Unit::UCT_BASIC_CLASS ==  UnitClassType )	/// 기본 전직 정보 설정
			{
				pStatic->GetString( 0 )->msg = GET_STRING( STR_ID_25045 );
				eUnitClass = CX2Unit::UC_CHUNG_IRON_CANNON;
			}
		} break;
	case CX2Unit::UC_CHUNG_FURY_GUARDIAN:		/// 1차 전직
	case CX2Unit::UC_CHUNG_IRON_PALADIN:		/// 2차 전직
		{
			switch ( UnitClassType )					/// 원하는 전직 등급에 따른 정보 설정
			{
			case CX2Unit::UCT_BASIC_CLASS:	/// 기본 전직 정보 설정
				{
					pStatic->GetString( 0 )->msg = GET_STRING( STR_ID_25045 );
					eUnitClass = CX2Unit::UC_CHUNG_IRON_CANNON;
				} break;
			case CX2Unit::UCT_FIRST_CLASS:	/// 1차 전직 정보 설정
				{
					pStatic->GetString( 0 )->msg = GET_STRING( STR_ID_25046 );
					eUnitClass = CX2Unit::UC_CHUNG_FURY_GUARDIAN;
				} break;
			case CX2Unit::UCT_SECOND_CLASS:	/// 2치 전직 정보 설정
				{
					pStatic->GetString( 0 )->msg = GET_STRING( STR_ID_25047 );
					eUnitClass = CX2Unit::UC_CHUNG_IRON_PALADIN;
				} break;
			}
		} break;
	case CX2Unit::UC_CHUNG_SHOOTING_GUARDIAN:	/// 1차 전직
	case CX2Unit::UC_CHUNG_DEADLY_CHASER:		/// 2차 전직
		{
			switch ( UnitClassType )					/// 원하는 전직 등급에 따른 정보 설정
			{
			case CX2Unit::UCT_BASIC_CLASS:	/// 기본 전직 정보 설정
				{
					pStatic->GetString( 0 )->msg = GET_STRING( STR_ID_25045 );
					eUnitClass = CX2Unit::UC_CHUNG_IRON_CANNON;
				} break;
			case CX2Unit::UCT_FIRST_CLASS:	/// 1차 전직 정보 설정
				{
					pStatic->GetString( 0 )->msg = GET_STRING( STR_ID_25048 );
					eUnitClass = CX2Unit::UC_CHUNG_SHOOTING_GUARDIAN;
				} break;
			case CX2Unit::UCT_SECOND_CLASS:	/// 2치 전직 정보 설정
				{
					pStatic->GetString( 0 )->msg = GET_STRING( STR_ID_25049 );
					eUnitClass = CX2Unit::UC_CHUNG_DEADLY_CHASER;
				} break;
			}
		} break;
	case CX2Unit::UC_CHUNG_SHELLING_GUARDIAN:	/// 1차 전직
	case CX2Unit::UC_CHUNG_TACTICAL_TROOPER:	/// 2차 전직
		{
			switch ( UnitClassType )					/// 원하는 전직 등급에 따른 정보 설정
			{
			case CX2Unit::UCT_BASIC_CLASS:	/// 기본 전직 정보 설정
				{
					pStatic->GetString( 0 )->msg = GET_STRING( STR_ID_25045 );
					eUnitClass = CX2Unit::UC_CHUNG_IRON_CANNON;
				} break;
			case CX2Unit::UCT_FIRST_CLASS:	/// 1차 전직 정보 설정
				{
					pStatic->GetString( 0 )->msg = GET_STRING( STR_ID_25050 );
					eUnitClass = CX2Unit::UC_CHUNG_SHELLING_GUARDIAN;
				} break;
			case CX2Unit::UCT_SECOND_CLASS:	/// 2치 전직 정보 설정
				{
					pStatic->GetString( 0 )->msg = GET_STRING( STR_ID_25051 );
					eUnitClass = CX2Unit::UC_CHUNG_TACTICAL_TROOPER;
				} break;
			}
		} break;
		/// 아라
	case CX2Unit::UC_ARA_MARTIAL_ARTIST:		/// 기본 전직
		{
			if ( CX2Unit::UCT_BASIC_CLASS ==  UnitClassType )	/// 기본 전직 정보 설정
			{
				pStatic->GetString( 0 )->msg = GET_STRING( STR_ID_25052 );
				eUnitClass = CX2Unit::UC_ARA_MARTIAL_ARTIST;
			}
		} break;
	case CX2Unit::UC_ARA_LITTLE_HSIEN:			/// 1차 전직
	case CX2Unit::UC_ARA_SAKRA_DEVANAM:			/// 2차 전직
		{
			switch ( UnitClassType )					/// 원하는 전직 등급에 따른 정보 설정
			{
			case CX2Unit::UCT_BASIC_CLASS:	/// 기본 전직 정보 설정
				{
					pStatic->GetString( 0 )->msg = GET_STRING( STR_ID_25052 );
					eUnitClass = CX2Unit::UC_ARA_MARTIAL_ARTIST;
				} break;
			case CX2Unit::UCT_FIRST_CLASS:	/// 1차 전직 정보 설정
				{
					pStatic->GetString( 0 )->msg = GET_STRING( STR_ID_25053 );
					eUnitClass = CX2Unit::UC_ARA_LITTLE_HSIEN;
				} break;
			case CX2Unit::UCT_SECOND_CLASS:	/// 2치 전직 정보 설정
				{
					pStatic->GetString( 0 )->msg = GET_STRING( STR_ID_25054 );
					eUnitClass = CX2Unit::UC_ARA_SAKRA_DEVANAM;
				} break;
			}
		} break;
	case CX2Unit::UC_ELESIS_KNIGHT:
		{
			if ( CX2Unit::UCT_BASIC_CLASS ==  UnitClassType )	/// 기본 전직 정보 설정
			{
				pStatic->GetString( 0 )->msg = GET_STRING( STR_ID_25873 );
				eUnitClass = CX2Unit::UC_ELESIS_KNIGHT;
			}
		} break;
	case CX2Unit::UC_ELESIS_SABER_KNIGHT:
#ifdef SERV_ELESIS_SECOND_CLASS_CHANGE	// 김종훈, 엘리시스 1-2 그랜드 마스터
	case CX2Unit::UC_ELESIS_GRAND_MASTER :
#endif // SERV_ELESIS_SECOND_CLASS_CHANGE	// 김종훈, 엘리시스 1-2 그랜드 마스터

		{
			switch ( UnitClassType )					/// 원하는 전직 등급에 따른 정보 설정
			{
			case CX2Unit::UCT_BASIC_CLASS:	/// 기본 전직 정보 설정
				{
					pStatic->GetString( 0 )->msg = GET_STRING( STR_ID_25873 );
					eUnitClass = CX2Unit::UC_ELESIS_KNIGHT;
				} break;
			case CX2Unit::UCT_FIRST_CLASS:	/// 1차 전직 정보 설정
				{
					pStatic->GetString( 0 )->msg = GET_STRING( STR_ID_26114 );
					eUnitClass = CX2Unit::UC_ELESIS_SABER_KNIGHT;
				} break;

#ifdef SERV_ELESIS_SECOND_CLASS_CHANGE	// 김종훈, 엘리시스 1-2 그랜드 마스터
			case CX2Unit::UCT_SECOND_CLASS:	/// 1차 전직 정보 설정
				{
					pStatic->GetString( 0 )->msg = GET_STRING( STR_ID_28263 );
					eUnitClass = CX2Unit::UC_ELESIS_GRAND_MASTER;
				} break;
#endif // SERV_ELESIS_SECOND_CLASS_CHANGE	// 김종훈, 엘리시스 1-2 그랜드 마스터
			}
		} break;
	case CX2Unit::UC_ELESIS_PYRO_KNIGHT:
#ifdef SERV_ELESIS_SECOND_CLASS_CHANGE	// 김종훈, 엘리시스 2-2 블레이징 하트
	case CX2Unit::UC_ELESIS_BLAZING_HEART :
#endif // SERV_ELESIS_SECOND_CLASS_CHANGE	// 김종훈, 엘리시스 2-2 블레이징 하트
		{
			switch ( UnitClassType )					/// 원하는 전직 등급에 따른 정보 설정
			{
			case CX2Unit::UCT_BASIC_CLASS:	/// 기본 전직 정보 설정
				{
					pStatic->GetString( 0 )->msg = GET_STRING( STR_ID_25873 );
					eUnitClass = CX2Unit::UC_ELESIS_KNIGHT;
				} break;
			case CX2Unit::UCT_FIRST_CLASS:	/// 1차 전직 정보 설정
				{
					pStatic->GetString( 0 )->msg = GET_STRING( STR_ID_26115 );
					eUnitClass = CX2Unit::UC_ELESIS_PYRO_KNIGHT;
				} break;
				
#ifdef SERV_ELESIS_SECOND_CLASS_CHANGE	// 김종훈, 엘리시스 2-2 블레이징 하트
			case CX2Unit::UCT_SECOND_CLASS:	/// 1차 전직 정보 설정
				{
					pStatic->GetString( 0 )->msg = GET_STRING( STR_ID_28264 );
					eUnitClass = CX2Unit::UC_ELESIS_BLAZING_HEART;
				} break;
#endif // SERV_ELESIS_SECOND_CLASS_CHANGE	// 김종훈, 엘리시스 2-2 블레이징 하트

			}
		} break;
#ifdef SERV_ARA_CHANGE_CLASS_SECOND // 김태환
	case CX2Unit::UC_ARA_LITTLE_DEVIL:			/// 1차 전직
	case CX2Unit::UC_ARA_YAMA_RAJA:					/// 2차 전직
		{
			switch ( UnitClassType )					/// 원하는 전직 등급에 따른 정보 설정
			{
			case CX2Unit::UCT_BASIC_CLASS:	/// 기본 전직 정보 설정
				{
					pStatic->GetString( 0 )->msg = GET_STRING( STR_ID_25052 );
					eUnitClass = CX2Unit::UC_ARA_MARTIAL_ARTIST;
				} break;
			case CX2Unit::UCT_FIRST_CLASS:	/// 1차 전직 정보 설정
				{
					pStatic->GetString( 0 )->msg = GET_STRING( STR_ID_26771 );
					eUnitClass = CX2Unit::UC_ARA_LITTLE_DEVIL;
				} break;
			case CX2Unit::UCT_SECOND_CLASS:	/// 2치 전직 정보 설정
				{
					pStatic->GetString( 0 )->msg = GET_STRING( STR_ID_26772 );
					eUnitClass = CX2Unit::UC_ARA_YAMA_RAJA;
				} break;
			}
		} break;
#endif // SERV_ARA_CHANGE_CLASS_SECOND

#ifdef SERV_9TH_NEW_CHARACTER // 김태환 ( 캐릭터 추가용 )
	case CX2Unit::UC_ADD_NASOD_RULER:
		{
			if ( CX2Unit::UCT_BASIC_CLASS ==  UnitClassType )	/// 기본 전직 정보 설정
			{
				pStatic->GetString( 0 )->msg = GET_STRING( STR_ID_29422 );
				eUnitClass = CX2Unit::UC_ADD_NASOD_RULER;
			}
		}

	case CX2Unit::UC_ADD_PSYCHIC_TRACER:
#ifdef SERV_ADD_LUNATIC_PSYKER // 김태환
	case CX2Unit::UC_ADD_LUNATIC_PSYKER:
#endif //SERV_ADD_LUNATIC_PSYKER
		{
			if ( CX2Unit::UCT_BASIC_CLASS ==  UnitClassType )	/// 기본 전직 정보 설정
			{
				pStatic->GetString( 0 )->msg = GET_STRING( STR_ID_29422 );
				eUnitClass = CX2Unit::UC_ADD_NASOD_RULER;
			}
			if ( CX2Unit::UCT_FIRST_CLASS ==  UnitClassType )	/// 1차 전직 정보 설정
			{
				pStatic->GetString( 0 )->msg = GET_STRING( STR_ID_29887 );
				eUnitClass = CX2Unit::UC_ADD_PSYCHIC_TRACER;
			}
	#ifdef SERV_ADD_LUNATIC_PSYKER // 김태환
			if ( CX2Unit::UCT_SECOND_CLASS ==  UnitClassType )	/// 2차 전직 정보 설정
			{
				pStatic->GetString( 0 )->msg = GET_STRING( STR_ID_30044 );
				eUnitClass = CX2Unit::UC_ADD_LUNATIC_PSYKER;
			}
	#endif //SERV_ADD_LUNATIC_PSYKER
		} break;

#endif //SERV_9TH_NEW_CHARACTER
		/// 전직 혹은 케릭터 추가시, 해당 case문 추가해야 합니다!
		/// 구조 : 
		/// 기본 전직						- 기본 전직만 추가
		/// 1차 혹은 2차 전직 ( 루트 1 )	- 기본 전직 ~ 2차 전직까지 추가 ( 해당 전직에서 이전 전직의 정보를 받아올 수 있도록 )
		/// 1차 혹은 2차 전직 ( 루트 2 )	- 기본 전직 ~ 2차 전직까지 추가
		/// 1차 혹은 2차 전직 ( 루트 3 )	- 기본 전직 ~ 2차 전직까지 추가
	}

	/// 초상화 이미지 키 값 설정
	if( false == CX2Data::GetCharacterImageName( wstrFileName, wstrPieceName, eUnitClass, CX2Data::CIT_50by50 ) )
	{
		/// Unit Class 인자값을 잘못 설정하였다!
		ASSERT( !"Skill Tree Portrait Unit Class Is Wrong!!!" );

		/// 아이샤 보이드 프린세스를 디폴트로 설정 ( 별 의미는 없다. )
		wstrFileName	= L"DLG_UI_Common_Texture21_NEW.tga";
		wstrPieceName	= L"V_PRINCESS_SMALL";
	}

	pStatic->GetPicture( 1 )->SetTex( wstrFileName.c_str(),	wstrPieceName.c_str() );	/// 초상화 설정

	/// Unit Class Type 예외 처리
	if ( L"None" == pStatic->GetString( 0 )->msg )
	{
		/// Unit Class Type 인자값을 잘못 설정하였다!
		ASSERT( !"Skill Tree Portrait Unit Class Type Is Wrong!!!" );

		pStatic->GetString( 0 )->msg = GET_STRING( STR_ID_25021 );
	}

	
}

/** @function	: GetSkillTitleDesc
	@brief		: 스킬 이름, 스킬 레벨을 표시하는 상위 툴팁 생성
	@param		: 스킬 아이디
	@return		: 스킬 템플릿, 스트링 참조자
*/
void CX2UISkillTreeNew::GetSkillTitleDesc( OUT wstring& wstrTitleDesc, IN const CX2SkillTree::SkillTemplet* pSkillTemplet, 
										   IN const CX2SkillTree::SkillTreeTemplet* pSkillTreeTemplet, IN int iSkillLevel, IN int iIncreasekillLevel ) const
{
	if( NULL == pSkillTemplet ||
		NULL == pSkillTreeTemplet ||
		NULL == g_pData ||
		NULL == g_pData->GetMyUser() ||
		NULL == g_pData->GetMyUser()->GetSelectUnit() ||
		NULL == g_pData->GetSocketItem() ||
		NULL == g_pData->GetSkillTree() )
		return;

#ifdef CLIENT_GLOBAL_LINEBREAK
	wstrTitleDesc = CWordLineHandler::GetStrByLineBreakInX2Main( pSkillTemplet->m_wstrName.c_str(), 150, XUF_DODUM_11_NORMAL );
#else //CLIENT_GLOBAL_LINEBREAK
	wstrTitleDesc = pSkillTemplet->m_wstrName;		/// 스킬명
#endif //CLIENT_GLOBAL_LINEBREAK

	wstrTitleDesc += L"\n";

	/// 스킬 레벨
	wstring wstrLevelTitle = L"";

	/// 만약 마스터 레벨일 때, 레벨 뒤에 (마스터) 추가
	if ( pSkillTreeTemplet->m_iMaxSkillLevel <= iSkillLevel )
		wstrLevelTitle = GET_STRING(STR_ID_25113);

	WCHAR buf[256] = {0,};

	/// 현재 스킬 레벨이 0이 아니고, 추가 스킬 레벨이 있으면
	if ( 0 < iSkillLevel && 0 < iIncreasekillLevel )
		StringCchPrintf( buf, 256, L"#C0DBE26%s.%d (+%d)#CX #CFF0000%s#CX\n", GET_STRING(STR_ID_489), iSkillLevel, iIncreasekillLevel, wstrLevelTitle.c_str() );
	else
		StringCchPrintf( buf, 256, L"%s.%d #CFF0000%s#CX\n", GET_STRING(STR_ID_489), iSkillLevel, wstrLevelTitle.c_str() );

	wstrTitleDesc += buf;
}

/** @function	: GetSkillSubDesc
	@brief		: 스킬 타입, 소모 MP, 재사용 시간을 표시하는 서브 툴팁 생성
	@param		: 스킬 템플릿, 총 스킬 레벨, 스트링 참조자
*/
void  CX2UISkillTreeNew::GetSkillSubDesc( IN const CX2SkillTree::SkillTemplet* pSkillTemplet, IN const int iTotalSkillLevel, OUT wstring& wstrSubDesc ) const
{
	
#ifdef SERV_9TH_NEW_CHARACTER // 김태환
	if( NULL == pSkillTemplet ||
		NULL == g_pData ||
		NULL == g_pData->GetMyUser() ||
		NULL == g_pData->GetMyUser()->GetSelectUnit() )
		return;
#else //SERV_9TH_NEW_CHARACTER
	if( NULL == pSkillTemplet )
		return;
#endif //SERV_9TH_NEW_CHARACTER
		

	bool bIsPassive = false;	/// 패시브 스킬 여부 ( 패시브 스킬은 재사용 시간 0일때, 미표기 )

	/// 스킬 타입
	WCHAR buf[256] = {0,};

	switch(pSkillTemplet->m_eType)
	{	
	case CX2SkillTree::ST_SPECIAL_ACTIVE:
		{
			StringCchPrintf( buf, 256, L"[%s]", GET_STRING(STR_ID_2672) );		/// 스페셜 액티브

		} break;
#ifdef FINALITY_SKILL_SYSTEM //JHKang
	case CX2SkillTree::ST_HYPER_ACTIVE_SKILL:
		{
			StringCchPrintf( buf, 256, L"[%s]", GET_STRING( STR_ID_26134 ) );		/// 하이퍼 액티브

		} break;
#endif //FINALITY_SKILL_SYSTEM
	case CX2SkillTree::ST_ACTIVE:
#ifdef ADDED_RELATIONSHIP_SYSTEM
	case CX2SkillTree::ST_RELATIONSHIP_SKILL:
#endif // ADDED_RELATIONSHIP_SYSTEM
		{
			StringCchPrintf( buf, 256, L"[%s]", GET_STRING(STR_ID_2673) );		/// 액티브

		} break;
	case CX2SkillTree::ST_BUFF:
		{
			StringCchPrintf( buf, 256, L"[%s]", GET_STRING(STR_ID_2674) );		/// 버프

		} break;
	default:
		{
			if ( true == m_bReplacedEsCounter )
			{
				StringCchPrintf( buf, 256, L"[%s]", GET_STRING(STR_ID_2673) );		/// 액티브
			}
			else
			{
				StringCchPrintf( buf, 256, L"[%s]", GET_STRING(STR_ID_2675) );		/// 패시브
				bIsPassive = true;		/// 패시브 스킬임
			}
		} break;
	}
	wstrSubDesc = buf;	/// 스킬 타입 설정

	/// 소모 MP
	const int iMPConsumption = static_cast<int>( pSkillTemplet->GetSkillMPConsumptionValue( iTotalSkillLevel ) );	/// 소모 MP 값

	/// 패시브 스킬이고 소모 MP가 있거나, 패시브 스킬이 아닐 때 소모 MP 표기
	if ( ( true == bIsPassive && 0 < iMPConsumption ) || false == bIsPassive )
	{
#ifdef SERV_9TH_NEW_CHARACTER // 김태환
		/// 애드일 때만 DP에 대한 소모량 표시
		if ( CX2Unit::UT_ADD == g_pData->GetMyUser()->GetSelectUnit()->GetType() )
		{
			const int iConsumeMP = static_cast<int>( pSkillTemplet->GetSkillMPConsumptionValue( iTotalSkillLevel ) );
			const int iConsumeDP = static_cast<int>( pSkillTemplet->m_usFPConsumtion / DISPLAY_DP_RATE );

			if ( 0 < iConsumeMP )		/// MP 소모량 표시
			{
				StringCchPrintf( buf, 256, L"\n%s%d", GET_STRING(STR_ID_2648), iConsumeMP );

				wstrSubDesc += buf;

				if ( 0 < iConsumeDP )	/// + DP 소모량 표시
				{
					StringCchPrintf( buf, 256, L" / %s%d", GET_STRING(STR_ID_29866), iConsumeDP );

					wstrSubDesc += buf;
				}
			}
			else if ( 0 < iConsumeDP )						/// DP 소모량 표시
			{
				StringCchPrintf( buf, 256, L"\n%s%d", GET_STRING(STR_ID_29866), iConsumeDP );

				wstrSubDesc += buf;
			}
		}
		else
		{
			StringCchPrintf( buf, 256, L"\n%s%d", GET_STRING(STR_ID_2648), static_cast<int>( pSkillTemplet->GetSkillMPConsumptionValue( iTotalSkillLevel ) ) );

			wstrSubDesc += buf;
		}
#else //SERV_9TH_NEW_CHARACTER
		StringCchPrintf( buf, 256, L"\n%s%d", GET_STRING(STR_ID_2648), static_cast<int>( pSkillTemplet->GetSkillMPConsumptionValue( iTotalSkillLevel ) ) );
	

		wstrSubDesc += buf;
#endif //SERV_9TH_NEW_CHARACTER
	}

	/// 재사용 시간
	const int iSkillCoolTime = static_cast<int>( pSkillTemplet->GetSkillCoolTimeValue( iTotalSkillLevel ) );		/// 재사용 시간 값

	/// 패시브 스킬이고 재사용 시간이 있거나, 패시브 스킬이 아닐 때 재사용 시간 표기
	if ( ( true == bIsPassive && 0 < iSkillCoolTime ) || false == bIsPassive )
	{
		wstrSubDesc += L"\n";
		wstrSubDesc += GET_REPLACED_STRING( ( STR_ID_25070, "i", static_cast<int>( pSkillTemplet->GetSkillCoolTimeValue( iTotalSkillLevel ) ) ) );	/// 재사용 시간 @1
	}
}

/** @function	: SetLearnSkillInfo
	@brief		: 스킬창에서 스킬 습득하거나 강화할 때, 변경값 설정하는 함수
	@param		: 스킬 아이디
*/
void CX2UISkillTreeNew::SetLearnSkillInfo( CX2SkillTree::SKILL_ID eSkillID )
{
	if (	NULL == g_pData ||
			NULL == g_pData->GetSkillTree() )
	{
		return;
	}

	g_pData->GetSkillTree()->FixOrInsertMapSkillInfoValue( eSkillID );

	/// 스킬 슬롯 업데이트 처리
	UpdateUI( false, true, true );
}

/** @function	: SetBlindSelectSkill
	@brief		: 2지선다 스킬 중 선택되지 않은 스킬에 대한 블라인드 설정
*/
void CX2UISkillTreeNew::SetBlindSelectSkill()
{
	if (	NULL == m_pDLGUISkillTree ||
			NULL == g_pData ||
			NULL == g_pData->GetMyUser() ||
			NULL == g_pData->GetMyUser()->GetSelectUnit() )
	{
		return;
	}
	
	for ( int iTier = 0; iTier < m_iMaxSkillRowNum + 1; ++iTier )
	{
		/// 2지선다 우측에 있는 스킬 아이디 검사
		std::map<CX2UISkillTreeNew::SkillPositionKey, CX2SkillTree::SKILL_ID>::iterator itRight
			= m_mapSkillPosition.find( CX2UISkillTreeNew::SkillPositionKey( 1, iTier ) );

		if ( itRight != m_mapSkillPosition.end() )	/// 2지선다 우측에 스킬이 존재할 때
		{
			const CX2UserSkillTree&	refUserSkillTree = g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_UserSkillTree;	/// 습득한 스킬 객체

#ifdef SKILL_PAGE_SYSTEM //JHKang
			int iSkillLevelRight = refUserSkillTree.GetSkillLevel( itRight->second, false, refUserSkillTree.GetSelectPage() );	// 2지선다 우측의 스킬 레벨 ( 실제로 설정되어 있는 레벨 )

			if ( true == m_bNowLearnSkill /*&& refUserSkillTree.IsUsingPage()*/ )		/// 만약 스킬창에서 스킬 레벨을 올리고 있으며, 해당 스킬의 레벨을 올린적이 있다면 스킬창 내에서 올라간 레벨을 설정
#else //SKILL_PAGE_SYSTEM
			int						iSkillLevelRight = refUserSkillTree.GetSkillLevel( itRight->second );		/// 2지선다 우측의 스킬 레벨 ( 실제로 설정되어 있는 레벨 )

			if ( true == m_bNowLearnSkill )		/// 만약 스킬창에서 스킬 레벨을 올리고 있으며, 해당 스킬의 레벨을 올린적이 있다면 스킬창 내에서 올라간 레벨을 설정
#endif //SKILL_PAGE_SYSTEM

			{
				const std::map<int, KGetSkillInfo> mapSkillInfo = g_pData->GetSkillTree()->GetMapSkillInfo();

				std::map<int, KGetSkillInfo>::const_iterator it = mapSkillInfo.find( static_cast<int>( itRight->second ) );

				if ( it != mapSkillInfo.end() )
					iSkillLevelRight = it->second.m_iSkillLevel;
			}

			std::map<CX2UISkillTreeNew::SkillPositionKey, CX2SkillTree::SKILL_ID>::iterator itLeft		/// 2지선다 좌측의 스킬 아이디 검사
				= m_mapSkillPosition.find( CX2UISkillTreeNew::SkillPositionKey( 0, iTier ) );

			/// 2지선다 좌, 우측 슬롯 UI 객체 검색
			std::map<CX2SkillTree::SKILL_ID, SkillSlotUI>::iterator itLeftUISet		= m_mapSkillSlotUISet.find( itLeft->second );
			std::map<CX2SkillTree::SKILL_ID, SkillSlotUI>::iterator itRightUISet	= m_mapSkillSlotUISet.find( itRight->second );

			if ( itLeft == m_mapSkillPosition.end() || itLeftUISet == m_mapSkillSlotUISet.end() || itRightUISet == m_mapSkillSlotUISet.end() )
			{
				ASSERT( !"Skill Tree Slot Information Is Error!!!" );	/// 스킬 슬롯 내 정보가 뭔가 이상하다!!!
				return;
			}

#if defined( _IN_HOUSE_ ) || defined( _IN_HOUSE_SERVICE_READY_QA_ ) || defined( _OPEN_TEST_ ) || defined( _OPEN_TEST_2_ )
			/// 개발자 기능 - 양쪽 모두 스킬이 찍혀있다면, 블라인드 해제
			if( true == g_pMain->IsMyAuthLevelHigherThan( CX2User::XUAL_DEV ) )
			{
#ifdef SKILL_PAGE_SYSTEM //JHKang
				int iSkillLevelLeft = refUserSkillTree.GetSkillLevel( itLeft->second, false, refUserSkillTree.GetSelectPage() );		// 2지선다 좌측 스킬 레벨 ( 실제로 설정되어 있는 레벨 )
#else //SKILL_PAGE_SYSTEM
				int iSkillLevelLeft = refUserSkillTree.GetSkillLevel( itLeft->second );		/// 2지선다 좌측 스킬 레벨 ( 실제로 설정되어 있는 레벨 )
#endif //SKILL_PAGE_SYSTEM

				if ( 0 < iSkillLevelLeft && 0 < iSkillLevelRight )
				{
					itLeftUISet->second.m_bSelectSkillBlind		= false;	/// 2지선다 좌측 스킬 블라인드 해제
					itRightUISet->second.m_bSelectSkillBlind	= false;	/// 2지선다 우측 스킬 블라인드 해제

					continue;
				}
			}
#endif // _SERVICE_

			if ( 0 < iSkillLevelRight )		/// 2지선다 우측의 스킬이 투자되어 있다면,
			{
				itLeftUISet->second.m_bSelectSkillBlind		= true;		/// 2지선다 좌측 스킬 블라인드 처리
				itRightUISet->second.m_bSelectSkillBlind	= false;	/// 2지선다 우측 스킬 블라인드 해제
			}
			else	/// 2지선다 우측의 스킬이 투자되어 있지 않다면,
			{
#ifdef SKILL_PAGE_SYSTEM //JHKang
				int iSkillLevelLeft = refUserSkillTree.GetSkillLevel( itLeft->second, false, refUserSkillTree.GetSelectPage() );	/// 2지선다 좌측 스킬 레벨 ( 실제로 설정되어 있는 레벨 )

				if ( true == m_bNowLearnSkill && refUserSkillTree.IsUsingPage() )		/// 만약 스킬창에서 스킬 레벨을 올리고 있으며, 해당 스킬의 레벨을 올린적이 있다면 스킬창 내에서 올라간 레벨을 설정
#else //SKILL_PAGE_SYSTEM
				int iSkillLevelLeft = refUserSkillTree.GetSkillLevel( itLeft->second );		/// 2지선다 좌측 스킬 레벨 ( 실제로 설정되어 있는 레벨 )

				if ( true == m_bNowLearnSkill )		/// 만약 스킬창에서 스킬 레벨을 올리고 있으며, 해당 스킬의 레벨을 올린적이 있다면 스킬창 내에서 올라간 레벨을 설정
#endif //SKILL_PAGE_SYSTEM
				{
					const std::map<int, KGetSkillInfo> mapSkillInfo = g_pData->GetSkillTree()->GetMapSkillInfo();

					std::map<int, KGetSkillInfo>::const_iterator it = mapSkillInfo.find( static_cast<int>( itLeft->second ) );

					if ( it != mapSkillInfo.end() )
						iSkillLevelLeft = it->second.m_iSkillLevel;
				}

				if( 0 < iSkillLevelLeft )		/// 2지선다 좌측의 스킬이 투자되어 있다면,
				{
					itLeftUISet->second.m_bSelectSkillBlind		= false;	/// 2지선다 좌측 스킬 블라인드 해제
					itRightUISet->second.m_bSelectSkillBlind	= true;		/// 2지선다 우측 스킬 블라인드 처리
				}
				else							/// 2지선다 스킬 모두 투자된 적 없다면,
				{
					itLeftUISet->second.m_bSelectSkillBlind		= false;	/// 2지선다 좌측 스킬 블라인드 해제
					itRightUISet->second.m_bSelectSkillBlind	= false;	/// 2지선다 우측 스킬 블라인드 해제
				}
			}
		}
	}
}

/** @function	: GetAnotherSelectSkillID
	@brief		: 2지선다 스킬 중 반대편에 있는 스킬 아이디를 반환
	#param		: 검사할 스킬 아이디
	@return		: 반대편에 있는 스킬 아이디
*/
const CX2SkillTree::SKILL_ID CX2UISkillTreeNew::GetAnotherSelectSkillID( CX2SkillTree::SKILL_ID eCheckSkillID ) const
{
	if (	NULL == g_pData ||
			NULL == g_pData->GetSkillTree() ||
			NULL == g_pData->GetMyUser() ||
			NULL == g_pData->GetMyUser()->GetSelectUnit() )
	{
		return CX2SkillTree::SI_NONE;
	}

	int										iUnitClass			= static_cast<int>( g_pData->GetMyUser()->GetSelectUnit()->GetClass() );		/// 유닛 클레스
	const CX2SkillTree::SkillTreeTemplet*	pSkillTreeTemplet	= g_pData->GetSkillTree()->GetSkillTreeTemplet( iUnitClass, eCheckSkillID );	/// 스킬 트리 템플릿

	if ( NULL != pSkillTreeTemplet )
	{
		int iIndexInTier	= pSkillTreeTemplet->m_iIndexInTier;	/// 가로 인덱스

		if ( STST_SINGLE_SKILL == iIndexInTier )	/// 2지선다 스킬이 아니라면, 중지
			return CX2SkillTree::SI_NONE;

		int iTier			= pSkillTreeTemplet->m_iTier;			/// 세로 인덱스

		/// 반대편의 스킬 아이디 검사
		std::map<CX2UISkillTreeNew::SkillPositionKey, CX2SkillTree::SKILL_ID>::const_iterator it
									= m_mapSkillPosition.find( CX2UISkillTreeNew::SkillPositionKey( 1 == iIndexInTier ? 0 : 1, iTier ) );

		/// 반대편의 스킬 아이디가 있다면, 해당 스킬 아이디 반환
		if ( it != m_mapSkillPosition.end() )
			return it->second;
	}

	return CX2SkillTree::SI_NONE;
}

/** @function	: GetSkillLevelInSkillWimdow
	@brief		: 스킬창 내 스킬 아이디 반환 함수 ( 실제 적용중인 스킬이라면 적용중 레벨, 적용중은 아니지만 스킬창에서 임시로 찍어본 스킬이라면 찍은 레벨 )
	#param		: 검사할 스킬 아이디
	@return		: 스킬 레벨
*/
const int CX2UISkillTreeNew::GetSkillLevelInSkillWimdow( const CX2SkillTree::SKILL_ID eSkillID ) const
{
	if (	NULL == g_pData ||
			NULL == g_pData->GetMyUser() ||
			NULL == g_pData->GetMyUser()->GetSelectUnit() )
	{
		return 0;
	}

	const CX2UserSkillTree&	refUserSkillTree = g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_UserSkillTree;	/// 습득한 스킬 객체

#ifdef SKILL_PAGE_SYSTEM //JHKang
	int iSkillLevel = refUserSkillTree.GetSkillLevel( eSkillID, true, refUserSkillTree.GetSelectPage() );		/// 스킬 레벨 ( 실제로 설정되어 있는 레벨 )

	if ( true == m_bNowLearnSkill && refUserSkillTree.IsUsingPage() )		/// 만약 스킬창에서 스킬 레벨을 올리고 있으며, 해당 스킬의 레벨을 올린적이 있다면 스킬창 내에서 올라간 레벨을 설정
#else //SKILL_PAGE_SYSTEM
	int iSkillLevel = refUserSkillTree.GetSkillLevel( eSkillID, true );		/// 스킬 레벨 ( 실제로 설정되어 있는 레벨 )

	if ( true == m_bNowLearnSkill )		/// 만약 스킬창에서 스킬 레벨을 올리고 있으며, 해당 스킬의 레벨을 올린적이 있다면 스킬창 내에서 올라간 레벨을 설정
#endif //SKILL_PAGE_SYSTEM
	{
		const std::map<int, KGetSkillInfo> mapSkillInfo = g_pData->GetSkillTree()->GetMapSkillInfo();

		std::map<int, KGetSkillInfo>::const_iterator it = mapSkillInfo.find( static_cast<int>( eSkillID ) );

		if ( it != mapSkillInfo.end() )
			iSkillLevel = it->second.m_iSkillLevel;
	}

	return iSkillLevel;
}

/** @function	: GetPrecedingSkillDesc
	@brief		: 선행 스킬에 대한 툴팁 설정
	#param		: 툴팁 스트링, 선행 스킬 아이디
*/
void CX2UISkillTreeNew::GetPrecedingSkillDesc( OUT wstring& wstrDesc, CX2SkillTree::SKILL_ID ePrecedingSkill ) const
{
	if ( NULL == g_pData ||
		 NULL == g_pData->GetSkillTree() )
	{
		return;
	}

	/// 선행 스킬의 레벨
	const int iPrecedingSkillLevel = GetSkillLevelInSkillWimdow( ePrecedingSkill );

	if ( 0 >= iPrecedingSkillLevel )		/// 선행 스킬을 배우지 않았을 때만, 선행 스킬 안내 팝업 설정
	{
		const CX2SkillTree::SkillTemplet* pPrecedingSkillTemplet = g_pData->GetSkillTree()->GetSkillTemplet( ePrecedingSkill );

		if ( NULL != pPrecedingSkillTemplet )
		{
			WCHAR buf[256] = {0,};
			StringCchPrintf( buf, 256, L"\n#CFF0000%s#CX\n\n", GET_REPLACED_STRING( ( STR_ID_25114, "L", pPrecedingSkillTemplet->m_wstrName ) ) );	/// 선행스킬 : @1 Lv.1 필요
			wstrDesc += buf;
		}
	}
}

/** @function	: SetSkillSlotButton
	@brief		: 스킬 트리의 스킬 슬롯 버튼 설정 함수
	#param		: 버튼 객체, 버튼 타입
*/
void CX2UISkillTreeNew::SetSkillSlotButton( CKTDGUIButton* pSlotButton, SKILL_TREE_SLOT_BUTTON_TYPE eButtonType )
{
	if ( NULL == pSlotButton )
		return;

	switch ( eButtonType )
	{
	case STSBT_LEARN_SKILL:
		{

		} break;
	case STSBT_UPGRADE_SKILL:
		{

		} break;
	case STSBT_CAN_NOT_LEARN_SKILL:
		{

		} break;
	case STSBT_INIT_SKILL:
		{

		} break;
	default:
		{
			ASSERT( !"Button Type is Error" );
		} break;
	}
}

/** @function	: IsLearnBeforeSelectSkill
	@brief		: 이전 단계의 2지선다 스킬을 배웠는지 여부를 반환
	@param		: 현재 스킬 라인의 Tier 인덱스
	@return		: 이전 단계 2지선다 스킬 습득 여부
*/
bool CX2UISkillTreeNew::IsLearnBeforeSelectSkill( const int iNowTierIndex )
{
	/// 검사하려는 Tier 인덱스가 기본 스킬이라면, 무조건 습득 처리
	if ( 0 == GetRequireSkillLineLevel( iNowTierIndex ) )
		return true;

	if ( NULL == g_pData ||
		 NULL == g_pData->GetMyUser() ||
		 NULL == g_pData->GetMyUser()->GetSelectUnit() )
		 return true;

	const CX2Unit::UNIT_CLASS				eUnitClass			= g_pData->GetMyUser()->GetSelectUnit()->GetClass();

	/// 만약 가장 첫번째 줄( 기본 전직의 기본 스킬 )이거나 두번째 줄( 기본 스킬을 선행으로 둔 스킬 ) 이라면, 무조건 스킬 습득 처리
	if ( 0 == iNowTierIndex || 1 == iNowTierIndex )
		return true;

	int BeforeSkillLineIndex = iNowTierIndex - 1;	/// 이전 스킬 라인의 세로 인덱스

	/// 이전 라인의 스킬이 기본 스킬이라면, 기본 스킬 이전의 인덱스를 검사 ( 2단계 이전 스킬 라인 )
	if ( 0 == GetRequireSkillLineLevel( BeforeSkillLineIndex ) )
		--BeforeSkillLineIndex;

	/// 이전 단계의 스킬 아이디 검색
	const SkillPositionKey skillPositionKey	= SkillPositionKey( 0, BeforeSkillLineIndex );

	std::map<SkillPositionKey, CX2SkillTree::SKILL_ID>::const_iterator mit = m_mapSkillPosition.find( skillPositionKey );

	if ( m_mapSkillPosition.end() != mit )	/// 이전 단계의 스킬 아이디가 있을 때
	{
		CX2SkillTree::SKILL_ID eSkillID = mit->second;

		/// 해당 스킬의 레벨이 1보다 크면, 습득 된것으로 처리
		if ( 0 < GetSkillLevelInSkillWimdow( eSkillID ) )
			return true;
		/// 아니면, 2지선다 반대편 아이디 검사
		else
		{
			const CX2SkillTree::SKILL_ID eAnotherSkillID = GetAnotherSelectSkillID( mit->second );

			/// 반대편 스킬 아이디가 있으며, 해당 스킬의 레벨이 1보다 크면 습득 된것으로 처리
			if ( CX2SkillTree::SI_NONE != eAnotherSkillID && 0 < GetSkillLevelInSkillWimdow( eAnotherSkillID ) )
				return true;
		}
	}

	return false;	/// 이전 라인의 스킬을 배우지 않았다.
}

/** @function	: GetNotLearnTierIndex
	@brief		: 배우지 못한 최상위 스킬 라인 반환
	@return		: 스킬 라인 Tier 인덱스
*/
const int CX2UISkillTreeNew::GetNotLearnTierIndex()
{
	std::map<SkillPositionKey, CX2SkillTree::SKILL_ID>::iterator mit = m_mapSkillPosition.find( SkillPositionKey( 0, 0 ) );
	int iTierIndex = 0;

	/// 맨 위에서부터 아래로 차례대로 검사하여, 못배운 스킬 라인을 찾아내면 해당 Tier 인덱스를 반환시키자
	while ( m_mapSkillPosition.end() != mit )
	{
		const CX2SkillTree::SKILL_ID eLeftSkillID = mit->second;	/// 2지선다 왼쪽 스킬 아이디

		if ( 0 != GetRequireSkillLineLevel( iTierIndex ) && 0 >= GetSkillLevelInSkillWimdow( eLeftSkillID ) )	/// 기본 스킬이 아니고, 스킬을 배우지 않았다면
		{
			const CX2SkillTree::SKILL_ID eRightSkillID = GetAnotherSelectSkillID( eLeftSkillID );				/// 2지선다 오른쪽 스킬 아이디 반환

			if ( CX2SkillTree::SI_NONE == eRightSkillID || 0 >= GetSkillLevelInSkillWimdow( eRightSkillID ) )	/// 2지 선다 오른쪽 스킬이 없거나, 배우지 않았다
				break;		/// 해당 스킬 라인을 않배웠으니, Tier 인덱스 반환 하자
		}

		++iTierIndex;

		mit = m_mapSkillPosition.find( SkillPositionKey( 0, iTierIndex ) );
	}

	return iTierIndex;
}

/** @function	: GetEnoughLevelSelectSkillTierIndex
	@brief		: 배울 수 없는 레벨의 최상위 스킬 라인 반환
	@return		: 스킬 라인 Tier 인덱스
*/
const int CX2UISkillTreeNew::GetNotEnoughLevelTierIndex( const int iLevel )
{
	int iTierIndex = 0;

	if ( NULL != m_pDLGUISkillTree )
	{
		CKTDGUIContolList*	pControlList = static_cast<CKTDGUIContolList*>( m_pDLGUISkillTree->GetControl( L"SkillTreeControlList" ) );

		if ( NULL != pControlList )
		{
			/// 맨 위에서부터 아래로 차례대로 검사하여, 배울 수 없는 레벨의 스킬 라인을 찾아내면 해당 Tier 인덱스를 반환시키자
			while ( pControlList->GetMaxIndexY() >= iTierIndex )
			{
				int iRequireSkillLevel = GetRequireSkillLineLevel( iTierIndex );

				if ( 0 != iRequireSkillLevel && iRequireSkillLevel > iLevel )	/// 기본 스킬이 아니고, 스킬을 배우지 않았다면
					break;		/// 해당 스킬 라인을 않배웠으니, Tier 인덱스 반환 하자

				++iTierIndex;
			}
		}
	}

	return iTierIndex;
}

/** @function	: GetSkillUnSealedDesc
	@brief		: 봉인 스킬 아이템 구입 안내 툴팁 생성
	@param		: 스트링 참조자, 스킬 아이디
*/
void CX2UISkillTreeNew::GetSkillUnSealedDesc( OUT wstring& wstrDesc, IN const CX2SkillTree::SKILL_ID eSkillID ) const
{
	if ( NULL == g_pData ||
		 NULL == g_pData->GetSkillTree() ||
		 NULL == g_pData->GetItemManager() )
		 return;

	/// 봉인 스킬 템플릿 반환
	const std::map<int, CX2SkillTree::SealSkillItemInfo>& mapSealSkillItemTemplet = g_pData->GetSkillTree()->GetSealSkillItemTemplet();

	int iUnSealedItemID = 0;		/// 봉인 스킬 아이템 아이디

	
	/// 해당 스킬 아이디를 지닌 봉인 스킬 아이템을 검사
	std::map<int, CX2SkillTree::SealSkillItemInfo>::const_iterator mit = mapSealSkillItemTemplet.begin();

	for ( mit; mit != mapSealSkillItemTemplet.end(); ++mit )
	{
		std::vector<CX2SkillTree::SKILL_ID> vecValue = mit->second.m_vecSkillID;

		BOOST_FOREACH( CX2SkillTree::SKILL_ID eValue, vecValue )
		{
			/// 해당 봉인 스킬 아이템을 찾았다면, 아이템 아이디 반환
			if ( eSkillID == eValue )
				iUnSealedItemID = mit->first;
		}
	}

	/// 아이템 아이디를 찾았다면, 툴팁에 아이템 구매 안내 표시
	if ( iUnSealedItemID != 0 )
	{
		const CX2Item::ItemTemplet * pItemTemplet = g_pData->GetItemManager()->GetItemTemplet( iUnSealedItemID );
		if ( NULL != pItemTemplet )
		{			
			wstring wstrItemName = pItemTemplet->GetName();

			wstrDesc += L"#CFF0000";
			wstrDesc += GET_REPLACED_STRING( ( STR_ID_25304, "L", wstrItemName ) );;
			wstrDesc += L"#CX\n\n";
		}	
	}

}
#endif // UPGRADE_SKILL_SYSTEM_2013

#ifdef SKILL_PAGE_SYSTEM //JHKang
#pragma region InitSkillPageUI
void CX2UISkillTreeNew::UpdateSkillPageUI()
{
	CX2UserSkillTree& refUserSkillTree = g_pData->GetMyUser()->GetSelectUnit()->AccessUnitData().m_UserSkillTree;

	const USHORT usUsingPage = refUserSkillTree.GetUsingPage();
	const USHORT usOpenedPage = refUserSkillTree.GetOpenedPage();
	const USHORT usSelectPage = refUserSkillTree.GetSelectPage();
	D3DXVECTOR2 vPos( 0.f, 0.f );
		
	CKTDGUIRadioButton* pRadioButton_Page1 = reinterpret_cast<CKTDGUIRadioButton*>( m_pDLGUISkillTree->GetControl( L"Skill_Page1" ) );
	CKTDGUIRadioButton* pRadioButton_Page2 = reinterpret_cast<CKTDGUIRadioButton*>( m_pDLGUISkillTree->GetControl( L"Skill_Page2" ) );
	CKTDGUIRadioButton* pRadioButton_Page3 = reinterpret_cast<CKTDGUIRadioButton*>( m_pDLGUISkillTree->GetControl( L"Skill_Page3" ) );
	CKTDGUIButton* pButton_Add_Page_ED = reinterpret_cast<CKTDGUIButton*>( m_pDLGUISkillTree->GetControl( L"Add_Skill_Page_ED" ) );
	CKTDGUIButton* pButton_Add_Page_Cash = reinterpret_cast<CKTDGUIButton*>( m_pDLGUISkillTree->GetControl( L"Add_Skill_Page_Cash" ) );
	CKTDGUIButton* pButton_Decide_Page = reinterpret_cast<CKTDGUIButton*>( m_pDLGUISkillTree->GetControl( L"Decide_Skill_Page" ) );

	if ( pRadioButton_Page1 == NULL || pRadioButton_Page2 == NULL || pRadioButton_Page3 == NULL || pButton_Add_Page_ED == NULL 
		|| pButton_Decide_Page == NULL )
		return;

	if ( refUserSkillTree.IsUsingPage() )
		pButton_Decide_Page->SetShowEnable( true, false );
	else
		pButton_Decide_Page->SetShowEnable( true, true );

	switch ( usSelectPage )
	{
	case 1:
		{
			pRadioButton_Page1->SetChecked( true );
		} break;

	case 2:
		{
			pRadioButton_Page2->SetChecked( true );
		} break;

	case 3:
		{
			pRadioButton_Page3->SetChecked( true );
		} break;
	default:
		break;
	}

	bool bPage1, bPage2, bPage3, bAdd;
	bPage1 = bPage2 = bPage3 = bAdd = false;

	switch ( usOpenedPage )
	{
	case 1:
		{
			bPage1 = bAdd = true;
			bPage2 = bPage3 = false;
			
			vPos = pRadioButton_Page2->GetPos();
		} break;

	case 2:
		{
			bPage1 = bPage2 = bAdd = true;
			bPage3 = false;

			vPos = pRadioButton_Page3->GetPos();
		} break;

	case 3:
		{
			bPage1 = bPage2 = bPage3 = true;
			bAdd = false;
		} break;

	default:
		{
			bPage1 = bPage2 = bPage3 = bAdd = false;
		} break;
	}

	pRadioButton_Page1->SetShowEnable( bPage1, bPage1 );
	pRadioButton_Page2->SetShowEnable( bPage2, bPage2 );
	pRadioButton_Page3->SetShowEnable( bPage3, bPage3 );

	if ( g_pMain->GetNowStateID() == CX2Main::XS_VILLAGE_MAP )
	{
		pButton_Add_Page_ED->SetOffsetPos( vPos );
		pButton_Add_Page_ED->SetShowEnable( bAdd, bAdd );
		pButton_Add_Page_ED->SetGuideDesc( GET_STRING( STR_ID_29366 ) );
		pButton_Add_Page_ED->SetGuideDescOffsetPos( D3DXVECTOR2( 100, 0 ) );
		vPos.x += 27.f;
		pButton_Add_Page_Cash->SetOffsetPos( vPos );
		pButton_Add_Page_Cash->SetShowEnable( bAdd, bAdd );
		pButton_Add_Page_Cash->SetGuideDesc( GET_STRING( STR_ID_29367 ) );
		pButton_Add_Page_Cash->SetGuideDescOffsetPos( D3DXVECTOR2( 100, 0 ) );
	}
	else
	{
		pButton_Add_Page_ED->SetShowEnable( false, false );
		pButton_Add_Page_Cash->SetShowEnable( false, false );
	}
}
#pragma endregion 스킬 페이지 UI 초기화

#pragma region BuySkillPageDialog
void CX2UISkillTreeNew::BuySkillPageDialog( const int iED_ )
{
	if ( g_pData && g_pData->GetMyUser() && g_pData->GetMyUser()->GetSelectUnit() )
	{
		wstringstream	wstrstm;

		m_iConfirmED = iED_;

		wstrstm << GET_REPLACED_STRING( ( STR_ID_29037, "LL", g_pMain->GetEDString( m_iConfirmED  ),
			g_pMain->GetEDString( g_pData->GetMyUser()->GetSelectUnit()->AccessUnitData().m_ED ) ) );

		m_pDLGBuyConfirm = g_pMain->KTDGUIOkAndCancelMsgBox( D3DXVECTOR2(-999,-999), wstrstm.str().c_str(), STUCM_BUY_CONFIRM,
			g_pMain->GetNowState(), STUCM_BUY_CANCEL, L"", D3DXVECTOR2( 0, 40 ) );
	}
}
#pragma endregion 구입 요청 대화상자 출력
#endif //SKILL_PAGE_SYSTEM