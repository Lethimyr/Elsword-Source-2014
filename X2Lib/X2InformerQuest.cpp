#include "StdAfx.h"
#include ".\x2informerquest.h"

CX2InformerQuest::CX2InformerQuest(void)
{
	m_InformerType				= CX2Informer::XIT_QUEST;
#ifdef REFORM_QUEST
	m_iNowInfoChangeQuestID		= -1;
	m_bIsEverySubQuestInQuestComplete		= false;
#endif //REFORM_QUEST
#ifdef SERV_PET_SYSTEM
#ifdef INT_WIDE_BAR
	m_vTalkBoxPos				= D3DXVECTOR2( 885, 702 );
#else // INT_WIDE_BAR
	m_vTalkBoxPos				= D3DXVECTOR2( 810, 702 );
#endif // INT_WIDE_BAR
#else
	m_vTalkBoxPos				= D3DXVECTOR2( 433, 711 );
#endif

	//{{ kimhc // 2010.02.09 // 일정시간마다 퀘스트가 업데이트 되는 시스템 구현
#ifdef	SERV_DAY_QUEST
	SetTimedEventQuest( false );
#endif	SERV_DAY_QUEST
	//}} kimhc // 2010.02.09 // 일정시간마다 퀘스트가 업데이트 되는 시스템 구현
}

CX2InformerQuest::~CX2InformerQuest(void)
{
	UnLoadHouseQuestInfo();
}

void CX2InformerQuest::Reset()
{
	CX2Informer::Reset();
	m_NowClearQuestID.clear();
}

void CX2InformerQuest::LoadHouseQuestInfo( int villageID )
{
	UnLoadHouseQuestInfo();
	CX2LocationManager::VillageTemplet* pVillageTemplet = g_pData->GetLocationManager()->GetVillageMapTemplet( (SEnum::VILLAGE_MAP_ID)villageID );
	if ( pVillageTemplet == NULL )
		return;

	for ( int i = 0; i < (int)pVillageTemplet->m_HouseList.size(); i++ )
	{
		int houseID = pVillageTemplet->m_HouseList[i];
		CX2LocationManager::HouseTemplet* pHouseTemplet = g_pData->GetLocationManager()->GetHouseTemplet( (CX2LocationManager::HOUSE_ID)houseID );
		if ( pHouseTemplet == NULL )
			continue;

		CX2InformerQuest::HouseQuestInfo* pHouseQuestInfo = new CX2InformerQuest::HouseQuestInfo();
		pHouseQuestInfo->m_HouseID = houseID;

		m_vecHouseQuestInfo.push_back( pHouseQuestInfo );
	}
}

void CX2InformerQuest::UnLoadHouseQuestInfo()
{
	for ( int i = 0; i < (int)m_vecHouseQuestInfo.size(); i++ )
	{
		CX2InformerQuest::HouseQuestInfo* pHouseQuestInfo = m_vecHouseQuestInfo[i];
		SAFE_DELETE( pHouseQuestInfo );
	}
	m_vecHouseQuestInfo.clear();
}

void CX2InformerQuest::CheckHouseQuestInfo()
{
	for ( int i = 0; i < (int)m_vecHouseQuestInfo.size(); i++ )
	{
		CX2InformerQuest::HouseQuestInfo* pHouseQuestInfo = m_vecHouseQuestInfo[i];
		if ( pHouseQuestInfo == NULL )
			continue;

		CX2LocationManager::HouseTemplet* pHouseTemplet = g_pData->GetLocationManager()->GetHouseTemplet( (CX2LocationManager::HOUSE_ID)pHouseQuestInfo->m_HouseID );
		if ( pHouseTemplet == NULL )
			continue;

		if ( pHouseTemplet->m_NPCList.empty() == false )
		{
			CX2UnitManager::NPC_UNIT_ID npcID = pHouseTemplet->m_NPCList[0];
			vector<int> vecAccecptableQuest;
			vector<int> vecCompleteQuest;

			g_pData->GetQuestManager()->GetAvailableQuest( npcID, vecAccecptableQuest );





			//반복퀘스트 한번 깼으면 표시 안하게끔 수정 ..
			for ( int j = 0; j < (int)vecAccecptableQuest.size(); j++ )
			{
				int tempQuestID = vecAccecptableQuest[j];
				if ( g_pData->GetQuestManager()->GetUnitCompleteQuest( tempQuestID ) == true )
				{
					const CX2QuestManager::QuestTemplet* pQuestTemplet = g_pData->GetQuestManager()->GetQuestTemplet( tempQuestID );
					if ( pQuestTemplet != NULL && 
						( pQuestTemplet->m_eQuestType != CX2QuestManager::QT_EVENT
						//{{ kimhc // 2010.2.1 //	PC방 퀘스트
#ifdef	PC_BANG_QUEST
						|| pQuestTemplet->m_eQuestType == CX2QuestManager::QT_PCBANG
#endif	PC_BANG_QUEST
						//}} kimhc // 2010.2.1 //	PC방 퀘스트
						) )
					{
						vecAccecptableQuest.erase( vecAccecptableQuest.begin() + j );
						j--;
					}
				}
			}

			g_pData->GetQuestManager()->GetCompletableQuest( npcID, vecCompleteQuest );

			
			bool bCheckEventQuest = false;
			bool bCheckNormalQuest = false;


			for ( int i = 0; i < (int)vecAccecptableQuest.size(); i++ )
			{
				int questID = vecAccecptableQuest[i];
				const CX2QuestManager::QuestTemplet* pQuestTemplet = g_pData->GetQuestManager()->GetQuestTemplet( questID );
				if ( pQuestTemplet != NULL && 
					( pQuestTemplet->m_eQuestType == CX2QuestManager::QT_EVENT 
					//{{ kimhc // 2010.2.1 //	PC방 퀘스트
#ifdef	PC_BANG_QUEST
					|| pQuestTemplet->m_eQuestType == CX2QuestManager::QT_PCBANG
#endif	PC_BANG_QUEST
					//}} kimhc // 2010.2.1 //	PC방 퀘스트
					) ) 
				{
					bCheckEventQuest = true;
				}
				else
				{
					bCheckNormalQuest = true;
				}
			}

			if ( vecCompleteQuest.empty() == false )
			{
				if ( pHouseQuestInfo->m_hSeqCompleteQuestInfoBG == INVALID_PARTICLE_SEQUENCE_HANDLE )
				{
					pHouseQuestInfo->m_hSeqCompleteQuestInfoBG = g_pData->GetUIMajorParticle()->CreateSequenceHandle( NULL,  L"CompleteQuestInfoBG", 0,0,0, 1000, 1000, 1, 1 );


					CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pData->GetUIMajorParticle()->GetInstanceSequence( pHouseQuestInfo->m_hSeqCompleteQuestInfoBG );
					if( NULL != pSeq )
					{
						pSeq->SetPosition( D3DXVECTOR3( pHouseTemplet->m_QuestInfoPos.x,pHouseTemplet->m_QuestInfoPos.y, 0 ) );
					}
				}

				if ( pHouseQuestInfo->m_hSeqCompleteQuestInfo == INVALID_PARTICLE_SEQUENCE_HANDLE )
				{
					pHouseQuestInfo->m_hSeqCompleteQuestInfo = g_pData->GetUIMajorParticle()->CreateSequenceHandle( NULL,  L"CompleteQuestInfo", 0,0,0, 1000, 1000, 1, 1 );
					CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pData->GetUIMajorParticle()->GetInstanceSequence( pHouseQuestInfo->m_hSeqCompleteQuestInfo );
					if( NULL != pSeq )
					{
						pSeq->SetPosition( D3DXVECTOR3( pHouseTemplet->m_QuestInfoPos.x,pHouseTemplet->m_QuestInfoPos.y, 0 ) );
					}
				}

				bCheckNormalQuest = true;
			
				if ( pHouseQuestInfo->m_hSeqAccecptableQuestInfoBG != INVALID_PARTICLE_SEQUENCE_HANDLE )
				{
					g_pData->GetUIMajorParticle()->DestroyInstanceHandle( pHouseQuestInfo->m_hSeqAccecptableQuestInfoBG );
				}

				if ( pHouseQuestInfo->m_hSeqAccecptableQuestInfo != INVALID_PARTICLE_SEQUENCE_HANDLE )
				{
					g_pData->GetUIMajorParticle()->DestroyInstanceHandle( pHouseQuestInfo->m_hSeqAccecptableQuestInfo );
				}

				
			}
			else if ( vecAccecptableQuest.empty() == false )
			{
				if ( pHouseQuestInfo->m_hSeqCompleteQuestInfoBG != INVALID_PARTICLE_SEQUENCE_HANDLE )
				{
					g_pData->GetUIMajorParticle()->DestroyInstanceHandle( pHouseQuestInfo->m_hSeqCompleteQuestInfoBG );
				}

				if ( pHouseQuestInfo->m_hSeqCompleteQuestInfo != INVALID_PARTICLE_SEQUENCE_HANDLE )
				{
					g_pData->GetUIMajorParticle()->DestroyInstanceHandle( pHouseQuestInfo->m_hSeqCompleteQuestInfo );
				}

				
				

				if ( bCheckNormalQuest == true )
				{
					if ( pHouseQuestInfo->m_hSeqAccecptableQuestInfoBG == INVALID_PARTICLE_SEQUENCE_HANDLE )
					{
						pHouseQuestInfo->m_hSeqAccecptableQuestInfoBG = g_pData->GetUIMajorParticle()->CreateSequenceHandle( NULL,  L"AcceptableQuestInfoBG", 0,0,0, 1000, 1000, 1, 1  );

						CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pData->GetUIMajorParticle()->GetInstanceSequence( pHouseQuestInfo->m_hSeqAccecptableQuestInfoBG );
						if( NULL != pSeq )
						{
							pSeq->SetPosition( D3DXVECTOR3( pHouseTemplet->m_QuestInfoPos.x,pHouseTemplet->m_QuestInfoPos.y, 0 ) );
						}
					}

					if ( pHouseQuestInfo->m_hSeqAccecptableQuestInfo == INVALID_PARTICLE_SEQUENCE_HANDLE )
					{
						pHouseQuestInfo->m_hSeqAccecptableQuestInfo = g_pData->GetUIMajorParticle()->CreateSequenceHandle( NULL,  L"AcceptableQuestInfo", 0,0,0, 1000, 1000, 1, 1  );
						CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pData->GetUIMajorParticle()->GetInstanceSequence( pHouseQuestInfo->m_hSeqAccecptableQuestInfo );
						if( NULL != pSeq )
						{
							pSeq->SetPosition( D3DXVECTOR3( pHouseTemplet->m_QuestInfoPos.x,pHouseTemplet->m_QuestInfoPos.y, 0 ) );
						}
					}
				}
				else
				{
					if ( pHouseQuestInfo->m_hSeqAccecptableQuestInfoBG != INVALID_PARTICLE_SEQUENCE_HANDLE )
					{
						g_pData->GetUIMajorParticle()->DestroyInstanceHandle( pHouseQuestInfo->m_hSeqAccecptableQuestInfoBG );
					}

					if ( pHouseQuestInfo->m_hSeqAccecptableQuestInfo != INVALID_PARTICLE_SEQUENCE_HANDLE )
					{
						g_pData->GetUIMajorParticle()->DestroyInstanceHandle( pHouseQuestInfo->m_hSeqAccecptableQuestInfo );
					}
				}

			}
			else
			{
				if ( pHouseQuestInfo->m_hSeqAccecptableQuestInfoBG != INVALID_PARTICLE_SEQUENCE_HANDLE )
				{
					g_pData->GetUIMajorParticle()->DestroyInstanceHandle( pHouseQuestInfo->m_hSeqAccecptableQuestInfoBG );
				}

				if ( pHouseQuestInfo->m_hSeqAccecptableQuestInfo != INVALID_PARTICLE_SEQUENCE_HANDLE )
				{
					g_pData->GetUIMajorParticle()->DestroyInstanceHandle( pHouseQuestInfo->m_hSeqAccecptableQuestInfo );
				}

				if ( pHouseQuestInfo->m_hSeqCompleteQuestInfoBG != INVALID_PARTICLE_SEQUENCE_HANDLE )
				{
					g_pData->GetUIMajorParticle()->DestroyInstanceHandle( pHouseQuestInfo->m_hSeqCompleteQuestInfoBG );
				}

				if ( pHouseQuestInfo->m_hSeqCompleteQuestInfo != INVALID_PARTICLE_SEQUENCE_HANDLE )
				{
					g_pData->GetUIMajorParticle()->DestroyInstanceHandle( pHouseQuestInfo->m_hSeqCompleteQuestInfo );
				}

				
			}


			if ( bCheckEventQuest == true )
			{
				if ( pHouseQuestInfo->m_hSeqAccecptableEventQuestInfoBG == INVALID_PARTICLE_SEQUENCE_HANDLE )
				{
					pHouseQuestInfo->m_hSeqAccecptableEventQuestInfoBG = g_pData->GetUIMajorParticle()->CreateSequenceHandle( NULL,  L"AcceptableEventQuestInfoBG", 0,0,0, 1000, 1000, 1, 1  );
					CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pData->GetUIMajorParticle()->GetInstanceSequence( pHouseQuestInfo->m_hSeqAccecptableEventQuestInfoBG );
					if( NULL != pSeq )
					{
						if ( bCheckNormalQuest == false )
							pSeq->SetPosition( D3DXVECTOR3( pHouseTemplet->m_QuestInfoPos.x,pHouseTemplet->m_QuestInfoPos.y, 0 ) );
						else
							pSeq->SetPosition( D3DXVECTOR3( pHouseTemplet->m_EventQuestInfoPos.x,pHouseTemplet->m_EventQuestInfoPos.y, 0 ) );
					}
				}

				if ( pHouseQuestInfo->m_hSeqAccecptableEventQuestInfo == INVALID_PARTICLE_SEQUENCE_HANDLE )
				{
					pHouseQuestInfo->m_hSeqAccecptableEventQuestInfo = g_pData->GetUIMajorParticle()->CreateSequenceHandle( NULL,  L"AcceptableEventQuestInfo", 0,0,0, 1000, 1000, 1, 1  );

					CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pData->GetUIMajorParticle()->GetInstanceSequence( pHouseQuestInfo->m_hSeqAccecptableEventQuestInfo );
					if( NULL != pSeq )
					{
						if ( bCheckNormalQuest == false )
							pSeq->SetPosition( D3DXVECTOR3( pHouseTemplet->m_QuestInfoPos.x,pHouseTemplet->m_QuestInfoPos.y, 0 ) );
						else
							pSeq->SetPosition( D3DXVECTOR3( pHouseTemplet->m_EventQuestInfoPos.x,pHouseTemplet->m_EventQuestInfoPos.y, 0 ) );
					}
				}
			}
			else
			{
				if ( pHouseQuestInfo->m_hSeqAccecptableEventQuestInfoBG != INVALID_PARTICLE_SEQUENCE_HANDLE )
				{
					g_pData->GetUIMajorParticle()->DestroyInstanceHandle( pHouseQuestInfo->m_hSeqAccecptableEventQuestInfoBG );
				}

				if ( pHouseQuestInfo->m_hSeqAccecptableEventQuestInfo != INVALID_PARTICLE_SEQUENCE_HANDLE )
				{
					g_pData->GetUIMajorParticle()->DestroyInstanceHandle( pHouseQuestInfo->m_hSeqAccecptableEventQuestInfo );
				}
			}

		}
	}
}

void CX2InformerQuest::UnCheckHouseQuestInfo()
{
	for ( int i = 0; i < (int)m_vecHouseQuestInfo.size(); i++ )
	{
		CX2InformerQuest::HouseQuestInfo* pHouseQuestInfo = m_vecHouseQuestInfo[i];
		if ( pHouseQuestInfo == NULL )
			continue;


			
		if ( pHouseQuestInfo->m_hSeqAccecptableQuestInfoBG != INVALID_PARTICLE_SEQUENCE_HANDLE )
		{
			g_pData->GetUIMajorParticle()->DestroyInstanceHandle( pHouseQuestInfo->m_hSeqAccecptableQuestInfoBG );
		}

		if ( pHouseQuestInfo->m_hSeqAccecptableQuestInfo != INVALID_PARTICLE_SEQUENCE_HANDLE )
		{
			g_pData->GetUIMajorParticle()->DestroyInstanceHandle( pHouseQuestInfo->m_hSeqAccecptableQuestInfo );
		}

		if ( pHouseQuestInfo->m_hSeqAccecptableEventQuestInfoBG != INVALID_PARTICLE_SEQUENCE_HANDLE )
		{
			g_pData->GetUIMajorParticle()->DestroyInstanceHandle( pHouseQuestInfo->m_hSeqAccecptableEventQuestInfoBG );
		}

		if ( pHouseQuestInfo->m_hSeqAccecptableEventQuestInfo != INVALID_PARTICLE_SEQUENCE_HANDLE )
		{
			g_pData->GetUIMajorParticle()->DestroyInstanceHandle( pHouseQuestInfo->m_hSeqAccecptableEventQuestInfo );
		}

		if ( pHouseQuestInfo->m_hSeqCompleteQuestInfoBG != INVALID_PARTICLE_SEQUENCE_HANDLE )
		{
			g_pData->GetUIMajorParticle()->DestroyInstanceHandle( pHouseQuestInfo->m_hSeqCompleteQuestInfoBG );
		}

		if ( pHouseQuestInfo->m_hSeqCompleteQuestInfo != INVALID_PARTICLE_SEQUENCE_HANDLE )
		{
			g_pData->GetUIMajorParticle()->DestroyInstanceHandle( pHouseQuestInfo->m_hSeqCompleteQuestInfo );
		}
		
	}
}

void CX2InformerQuest::SetNowInfo()
{
	if ( g_pData->GetMyUser() == NULL || g_pData->GetMyUser()->GetSelectUnit() == NULL )
		return;

	if ( IsTimedEventQuest() )
		return;

	m_NowClearQuestID.clear();
#ifdef REFORM_QUEST
	m_mapNowClearSubQuestGroup.clear();
#endif //REFORM_QUEST

	/// 진행 중인 퀘스트들을 얻어옴 (완료보고 안한 것들)
	const map<int,CX2QuestManager::QuestInst*>& mapUnitQuest 
		= g_pData->GetQuestManager()->GetUnitQuestMap();

	if ( mapUnitQuest.empty() == true )
		return;

	map<int, CX2QuestManager::QuestInst*>::const_iterator mItr;
	for ( mItr = mapUnitQuest.begin(); mItr != mapUnitQuest.end(); ++mItr )
	{
		const int& iQuestID = mItr->first;
		const CX2QuestManager::QuestInst* pQuestInst = mItr->second;
		const CX2QuestManager::QuestTemplet* pQuestTemplet = g_pData->GetQuestManager()->GetQuestTemplet( iQuestID );

		if ( pQuestInst == NULL || 
			pQuestInst->m_OwnorUnitUID != g_pData->GetMyUser()->GetSelectUnit()->GetUID() ||
			pQuestTemplet == NULL )
			continue;

		/// 퀘스트가 완료 되었으면 (모든 서브퀘스트 완료)
		if ( pQuestInst->IsComplete() == true )
		{
			m_NowClearQuestID.insert( pQuestInst->m_iID );	/// 왜 매프레임 지웠다 썼다를 반복할까?
		}
#ifdef REFORM_QUEST
		else	/// 모든 서브퀘스트가 완료 된게 아니라면
		{
			//완료된 서브퀘스트 그룹 체크
			set<int> setTemp;
			std::map< int, std::vector<int>>::const_iterator mit
				= pQuestTemplet->m_mapSubQuestGroup.begin();
			for( ; mit != pQuestTemplet->m_mapSubQuestGroup.end(); ++mit)
			{
				//서브퀘스트 그룹 순회
				bool bIsSubQuestGroupComplete = true;	/// 서브퀘스트 그룹이 완료 되었는가?
				for( UINT i=0; i<mit->second.size(); i++ )
				{
					const CX2QuestManager::SubQuestInst* pSubQuestInst 
						= pQuestInst->GetSubQuest( mit->second[i] );
					if( NULL != pSubQuestInst && false == pSubQuestInst->IsComplete() )
					{
						bIsSubQuestGroupComplete = false;
					}
				}
				if( true == bIsSubQuestGroupComplete )
				{
					setTemp.insert( mit->first );
				}
			}
			if( false == setTemp.empty() )	/// 서브퀘스트 그룹 중에 완료 된 것이 있다면
			{
				m_mapNowClearSubQuestGroup.insert( std::make_pair( pQuestInst->m_iID, setTemp ) );
			}
		}
#endif //REFORM_QUEST
	}
}

bool CX2InformerQuest::IsNowInfoChange()
{
	//{{ kimhc // 2010.02.09 // 일정시간마다 퀘스트가 업데이트 되는 시스템 구현
#ifdef	SERV_DAY_QUEST
	if ( IsTimedEventQuest() )
		return true;
#endif	SERV_DAY_QUEST
	//}} kimhc // 2010.02.09 // 일정시간마다 퀘스트가 업데이트 되는 시스템 구현

	CX2QuestManager* pQuestManager = g_pData->GetQuestManager();

	/// 진행 중인 퀘스트들을 얻어옴 (완료보고 안한 것들)
	const map<int,CX2QuestManager::QuestInst*>& mapUnitQuest = pQuestManager->GetUnitQuestMap();
	if ( mapUnitQuest.empty() )
		return false;

	const UidType uidOfMine = g_pData->GetMyUser()->GetSelectUnit()->GetUID();

	map<int, CX2QuestManager::QuestInst*>::const_iterator mItr;
	for ( mItr = mapUnitQuest.begin(); mItr != mapUnitQuest.end(); ++mItr )
	{
		const int& iQuestID = mItr->first;
		const CX2QuestManager::QuestInst* pQuestInst = mItr->second;
		
		if ( pQuestInst == NULL || 
			pQuestInst->m_OwnorUnitUID != uidOfMine )
			continue;
		
		if ( pQuestInst->IsComplete() == true )	/// 전체 서브 퀘스트가 완료 되었으면
		{
			/// 현재 클리어 목록에 없으면 (이번 체크때 완료 된 것)
			if ( m_NowClearQuestID.find( pQuestInst->m_iID ) == m_NowClearQuestID.end() )
			{
				if( NULL != g_pMain && NULL != g_pMain->GetKeyPad() )
				{
					g_pData->GetPlayGuide()->SetPlayGuide(CX2PlayGuide::PGT_QUEST_COMPLETE, true, g_pMain->GetKeyPad()->GetKeyString( GAMEACTION_QUEST ));
				}
				
#ifdef REFORM_QUEST
				/// 변경됬다고 퀵퀘스트 창에 알려줄 퀘스트 아이디
				m_iNowInfoChangeQuestID = pQuestInst->m_iID;
				m_bIsEverySubQuestInQuestComplete = true;
#endif //REFORM_QUEST
				return true;
			}
		}
		else
		{
			/// 전체 서브퀘스트가 완료되지 않았는데 현재 클리어 목록에 있으면
			if ( m_NowClearQuestID.find( pQuestInst->m_iID ) != m_NowClearQuestID.end() )
			{
				// 삭제함
				m_NowClearQuestID.erase( m_NowClearQuestID.find( pQuestInst->m_iID ) );
			}
			
			const CX2QuestManager::QuestTemplet* pQuestTemplet 
				= pQuestManager->GetQuestTemplet( iQuestID );

#ifdef REFORM_QUEST
			if ( NULL != pQuestTemplet )
			{
				std::map< int, std::vector<int>>::const_iterator mit 
					= pQuestTemplet->m_mapSubQuestGroup.begin();
				set<int> setTemp;


				for( ; mit != pQuestTemplet->m_mapSubQuestGroup.end(); ++mit)	//서브퀘스트 그룹 순회
				{
					bool bIsSubQuestGroupComplete = true;
					for( UINT i=0; i<mit->second.size(); i++ )
					{
						/// 현재 서브퀘스트 그룹 중 완료되지 않은게 있는지 검사
						const CX2QuestManager::SubQuestInst* pSubQuestInst = pQuestInst->GetSubQuest( mit->second[i] );
						if( NULL != pSubQuestInst && false == pSubQuestInst->IsComplete() )
						{
							bIsSubQuestGroupComplete = false;
						}
					}				

					if( true == bIsSubQuestGroupComplete )	/// 현재 그룹에 있는 모든 서브퀘스트가 완료 되었으면
					{
						std::map< int, std::set<int> >::iterator mitClearSubQuest
							= m_mapNowClearSubQuestGroup.find(pQuestInst->m_iID);

						//현재 퀘스트 중 완료된 서브 그룹이 있는지
						if( mitClearSubQuest != m_mapNowClearSubQuestGroup.end() )
						{
							//해당 서브퀘스트 그룹이 기존에 없던 그룹이라면
							if( mitClearSubQuest->second.find( mit->first ) == mitClearSubQuest->second.end() )
							{
								SetTimedEventQuest(false);
								m_iNowInfoChangeQuestID = pQuestInst->m_iID;
								m_bIsEverySubQuestInQuestComplete = false;
								return true;
							}

							/// 있던 그룹이라면 다음 서브퀘스트 그룹 순회
						}
						else	// 없으면 현재 퀘스트의 처음 서브그룹을 완료 한것
						{
							SetTimedEventQuest(false);
							m_iNowInfoChangeQuestID = pQuestInst->m_iID;
							m_bIsEverySubQuestInQuestComplete = false;
							return true;
						}
					}
				}
			}	/// if ( NULL != pQuestTemplet )
#endif //REFORM_QUEST
		}
	}

	return false;
}

void CX2InformerQuest::SetShow( bool bShow )
{
	CX2Informer::SetShow( bShow );
	if ( bShow == true )
	{
#ifdef REFORM_QUEST
		//초보자 가이드(화살표)에 방해되는 요소 제거
		if ( CX2PlayGuide::TQI_CHASE_THIEF != m_iNowInfoChangeQuestID &&
			m_bIsEverySubQuestInQuestComplete )
#endif //REFORM_QUEST
		{
			CX2TalkBoxManagerImp::TalkBox talkBox;

			//캐릭터 상태에 따라 메뉴 버튼의 위치가 달라 지기 때문에 위치 수정
			talkBox.m_vPos				= m_vTalkBoxPos+m_vTalkBoxOffsetPos;
			talkBox.m_GameUnitType		= CX2GameUnit::GUT_USER;
			talkBox.m_fRemainTime		= 4.0f;


			talkBox.m_bTraceUnit		= false;
			talkBox.m_TalkBoxType		= CX2TalkBoxManagerImp::TBT_FROM_DOWN_LEFT;
			talkBox.m_coTextColor		= D3DXCOLOR(0,0,0,1);

			//{{ kimhc // 2010.02.09 // 일정시간마다 퀘스트가 업데이트 되는 시스템 구현
#ifdef	SERV_DAY_QUEST
			if ( IsTimedEventQuest() == true )
			{
				talkBox.m_wstrTalkContent	= GET_STRING( STR_ID_4914 );
				SetTimedEventQuest( false );
			}
			else
#endif	SERV_DAY_QUEST
				//}} kimhc // 2010.02.09 // 일정시간마다 퀘스트가 업데이트 되는 시스템 구현
			{
				talkBox.m_wstrTalkContent	= GET_STRING( STR_ID_231 );
			}		
			m_pTalkBoxManager->Push( talkBox );
		}

		g_pKTDXApp->GetDeviceManager()->PlaySound( L"MessageAlarm.ogg", false, false );
#ifdef REFORM_QUEST
		if( NULL != g_pData && NULL != g_pData->GetUIManager() && NULL != g_pData->GetUIManager()->GetUIQuestNew() )
		{
			//g_pData->GetUIManager()->GetUIQuestNew()->UpdateQuickQuestDLG();
			g_pData->GetUIManager()->GetUIQuestNew()->ResetQuestUI();
			g_pData->GetUIManager()->GetUIQuestNew()->NotificationByQuickQuestInfoChange(m_iNowInfoChangeQuestID, m_bIsEverySubQuestInQuestComplete);
		}
#endif //REFORM_QUEST
	}
	else
	{
		m_pTalkBoxManager->Clear();
		
		//{{ kimhc // 2010.02.09 // 일정시간마다 퀘스트가 업데이트 되는 시스템 구현
#ifdef	SERV_DAY_QUEST
		SetTimedEventQuest( false );
#endif	SERV_DAY_QUEST
		//}} kimhc // 2010.02.09 // 일정시간마다 퀘스트가 업데이트 되는 시스템 구현
	}
}




CX2InformerQuest::HouseQuestInfo::HouseQuestInfo()
{
	m_HouseID = -1;

	m_hSeqAccecptableQuestInfoBG		= INVALID_PARTICLE_SEQUENCE_HANDLE;
	m_hSeqAccecptableQuestInfo			= INVALID_PARTICLE_SEQUENCE_HANDLE;
	m_hSeqCompleteQuestInfoBG			= INVALID_PARTICLE_SEQUENCE_HANDLE;
	m_hSeqCompleteQuestInfo				= INVALID_PARTICLE_SEQUENCE_HANDLE;

	m_hSeqAccecptableEventQuestInfoBG	= INVALID_PARTICLE_SEQUENCE_HANDLE;
	m_hSeqAccecptableEventQuestInfo		= INVALID_PARTICLE_SEQUENCE_HANDLE;
}

CX2InformerQuest::HouseQuestInfo::~HouseQuestInfo()
{
	if ( m_hSeqAccecptableQuestInfoBG != INVALID_PARTICLE_SEQUENCE_HANDLE )
		g_pData->GetUIMajorParticle()->DestroyInstanceHandle( m_hSeqAccecptableQuestInfoBG );

	if ( m_hSeqAccecptableQuestInfo != INVALID_PARTICLE_SEQUENCE_HANDLE )
		g_pData->GetUIMajorParticle()->DestroyInstanceHandle( m_hSeqAccecptableQuestInfo );

	if ( m_hSeqCompleteQuestInfoBG != INVALID_PARTICLE_SEQUENCE_HANDLE )
		g_pData->GetUIMajorParticle()->DestroyInstanceHandle( m_hSeqCompleteQuestInfoBG );

	if ( m_hSeqCompleteQuestInfo != INVALID_PARTICLE_SEQUENCE_HANDLE )
		g_pData->GetUIMajorParticle()->DestroyInstanceHandle( m_hSeqCompleteQuestInfo );

	if ( m_hSeqAccecptableEventQuestInfoBG != INVALID_PARTICLE_SEQUENCE_HANDLE )
		g_pData->GetUIMajorParticle()->DestroyInstanceHandle( m_hSeqAccecptableEventQuestInfoBG );

	if ( m_hSeqAccecptableEventQuestInfo != INVALID_PARTICLE_SEQUENCE_HANDLE )
		g_pData->GetUIMajorParticle()->DestroyInstanceHandle( m_hSeqAccecptableEventQuestInfo );
}

HRESULT CX2InformerQuest::OnFrameMove( double fTime, float fElapsedTime )
{
#ifndef REFORM_QUEST
	if ( g_pMain->GetNowStateID() != CX2Main::XS_DUNGEON_GAME )
#endif //REFORM_QUEST
		return CX2Informer::OnFrameMove( fTime, fElapsedTime );

	return S_OK;
}