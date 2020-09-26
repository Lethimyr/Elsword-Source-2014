#include "GSUser.h"
#include ".\usertutorialinfo.h"
#include "TutorialManager.h"
#include "RewardTable.h"
#include "NetError.h"
//{{ 2012. 07. 27	최육사		사제시스템 EXP 보너스 버그 수정
#ifdef SERV_TUTO_EXP_BONUS_BUG_FIX
	#include "GSFSM.h"
#endif SERV_TUTO_EXP_BONUS_BUG_FIX
//}}



KUserTutorialInfo::KUserTutorialInfo(void)
:
m_TutorUnitType( TUT_NONE )
{
}

KUserTutorialInfo::~KUserTutorialInfo(void)
{
}

bool KUserTutorialInfo::IsExistUnit( UidType iUnitUID )
{
	std::vector<KTutorialUnitInfo>::iterator vit;

	for( vit = m_vecTutorInfo.begin(); vit != m_vecTutorInfo.end(); ++vit )
	{
		if( vit->m_iUnitUID == iUnitUID )
			return true;
	}
	
	return false;
}

bool KUserTutorialInfo::IsExistbyTutorData()
{
	if( m_vecTutorInfo.size() > 0 )
		return true;

	return false;
}

int KUserTutorialInfo::GetTutorialUnitNum()
{
	return static_cast<int>(m_vecTutorInfo.size());
}

void KUserTutorialInfo::GetTutorUnitUIDList(std::vector<UidType>& vecUIDList )
{
	std::vector<KTutorialUnitInfo>::iterator vit;

	for( vit = m_vecTutorInfo.begin(); vit != m_vecTutorInfo.end(); ++vit )
	{
		vecUIDList.push_back( vit->m_iUnitUID );
	}
}

bool KUserTutorialInfo::OnSelectUnitAck( KGSUserPtr spUser, std::vector<KTutorialDBUnitInfo>& vecTutorialUnitInfo )
{
	ResetData();

	if( spUser->IsGuestUser() == true )
		return true;

	if( spUser->GetLevel() < KTutorialManager::UNIT_LEVEL_STUDENT_MAX )
	{
		m_TutorUnitType = TUT_STUDENT;
	}
	else if( spUser->GetLevel() >= KTutorialManager::UNIT_LEVEL_TEACHER_MIN )
	{
		m_TutorUnitType = TUT_TEACHER;
	}
	else
	{
		//사제시스템에 속하지 않는 유저도 존재함.
		m_TutorUnitType = TUT_NONE;
		return true;
	}

	std::vector<KTutorialDBUnitInfo>::iterator vit;
	for( vit = vecTutorialUnitInfo.begin(); vit != vecTutorialUnitInfo.end(); ++vit )
	{
		CTime LastTime;
		KTutorialUnitInfo kTutorialUnitInfo;

		if( KncUtil::ConvertStringToCTime( vit->m_wstrLastDate, LastTime ) == false )
		{
			START_LOG( cerr, L"문자열 시간조립 실패.!(;ㅁ; 크리티컬한데..)" )
				<< BUILD_LOG( vit->m_wstrLastDate )
				<< BUILD_LOG( spUser->GetCharName() )
				<< BUILD_LOG( spUser->GetUID() )
				<< END_LOG;

			continue;
		}
		//패킷 교환.
		kTutorialUnitInfo.m_iUnitUID = vit->m_kTutorialUnitInfo.m_iUnitUID;
		kTutorialUnitInfo.m_ucLevel = vit->m_kTutorialUnitInfo.m_ucLevel;
		kTutorialUnitInfo.m_wstrNickName = vit->m_kTutorialUnitInfo.m_wstrNickName;
		kTutorialUnitInfo.m_bIsOnline = vit->m_kTutorialUnitInfo.m_bIsOnline;

		m_vecTutorInfo.push_back( kTutorialUnitInfo );

		//접속일이 7일이상 지났는지 검사한다.
		bool bIsDelete = false;
		if( m_TutorUnitType == TUT_TEACHER )
		{
			CTimeSpan tsTemp( KTutorialManager::STUDENT_DISCONNECT_DAY, 0, 0, 0 );
			CTime tNowDate = CTime::GetCurrentTime();
			if( (LastTime + tsTemp) < tNowDate )
			{
				KDBE_DEL_TUTORIAL_REQ kPacket;

				kPacket.m_iTeacherUID = spUser->GetCharUID();
				kPacket.m_iStudentUID = vit->m_kTutorialUnitInfo.m_iUnitUID;
				kPacket.m_cReason = KTutorialManager::TDR_DISCONNECT_DAY;

				spUser->SendToGameDB( DBE_DEL_TUTORIAL_REQ, kPacket );

				bIsDelete = true;
			}
		}
		
		//접속여부 업데이트 보내기.
		if( bIsDelete == false )
		{
			SendUpdateUnitInfo( spUser, vit->m_kTutorialUnitInfo.m_iUnitUID, true, true );
		}
	}


	return true;
}

bool KUserTutorialInfo::OnDelTutorialReq( KGSUserPtr spUser, KEGS_DEL_TUTORIAL_REQ& kInfo )
{
	KEGS_DEL_TUTORIAL_ACK kAck;	

	if( spUser->IsGuestUser() == true )
	{
		kAck.m_iOK = NetError::NET_OK;
		spUser->SendPacket( EGS_DEL_TUTORIAL_ACK, kAck );
		return true;
	}

	KDBE_DEL_TUTORIAL_REQ kDBReq;
	kDBReq.m_cReason = KTutorialManager::TDR_REFUSAL_USER;

	std::vector<KTutorialUnitInfo>::iterator vit;
	for( vit = m_vecTutorInfo.begin(); vit != m_vecTutorInfo.end(); ++vit )
	{
		if( vit->m_iUnitUID == kInfo.m_iUnitUID )
		{
			switch( GetTutorUnitType() )
			{
			case TUT_TEACHER:
				{
					kDBReq.m_iTeacherUID = spUser->GetCharUID();
					kDBReq.m_iStudentUID = kInfo.m_iUnitUID;
				}
				break;
			case TUT_STUDENT:
				{
					kDBReq.m_iTeacherUID = kInfo.m_iUnitUID;
					kDBReq.m_iStudentUID = spUser->GetCharUID();
				}
				break;

			default:
				{
					START_LOG( cerr, L"사제 타입이 이상함.!" )
						<< BUILD_LOG( spUser->GetCharUID() )
						<< BUILD_LOG( spUser->GetCharName() )
						<< BUILD_LOG( kInfo.m_iUnitUID )
						<< END_LOG;

					return false;
				}
			}

			spUser->SendToGameDB( DBE_DEL_TUTORIAL_REQ, kDBReq );

			return true;
		}
	}

	kAck.m_iOK = NetError::ERR_TUTORIAL_11;
	spUser->SendPacket( EGS_DEL_TUTORIAL_ACK, kAck );
	return false;
}

bool KUserTutorialInfo::OnDelTutorialAck( KGSUserPtr spUser, KDBE_DEL_TUTORIAL_ACK& kInfo )
{
	KEGS_DEL_TUTORIAL_ACK kAck;	

	if( spUser->IsGuestUser() == true )
	{
		kAck.m_iOK = NetError::NET_OK;
		spUser->SendPacket( EGS_DEL_TUTORIAL_ACK, kAck );
		return true;
	}
	
	UidType iTempUID = 0;

	//나의 사제입장에 따라 검사할 uid를 선정함.
	switch( GetTutorUnitType() )
	{
	case TUT_TEACHER:
		iTempUID = kInfo.m_iStudentUID;
		break;

	case TUT_STUDENT:
		{
			iTempUID = kInfo.m_iTeacherUID;

			//등급처리.
			if( spUser->GetLevel() >= KTutorialManager::UNIT_LEVEL_STUDENT_MAX && spUser->GetLevel() < KTutorialManager::UNIT_LEVEL_TEACHER_MIN )
			{
				if( kInfo.m_cReason == KTutorialManager::TDR_LEVEL_UP_END )
				{
					SetTutorUnitType( TUT_NONE );
				}
			}
		}
		break;

	default:
		{
			START_LOG( cerr, L"사제 타입이 이상함.!" )
				<< BUILD_LOG( kInfo.m_iTeacherUID )
				<< BUILD_LOG( kInfo.m_iStudentUID )
				<< END_LOG;

			kAck.m_iOK = NetError::ERR_TUTORIAL_11;
			spUser->SendPacket( EGS_DEL_TUTORIAL_ACK, kAck );
			return false;
		}
	}

	std::vector<KTutorialUnitInfo>::iterator vit;
	for( vit = m_vecTutorInfo.begin(); vit != m_vecTutorInfo.end(); ++vit )
	{
		if( vit->m_iUnitUID == iTempUID )
		{
			KEGS_DEL_TUTORIAL_NOT kPacket;
			//나에게는 상대방 uid가 삭제대상이고
			kPacket.m_iUnitUID = iTempUID;
			//{{ 2011.09.16 조효진  넷 에러 스트링을 클라이언트쪽에서 가져와서 출력
#ifdef SERV_NETERROR_STR_GET_FROM_CLIENT
			kPacket.m_iReason = SiKTutorialManager()->GetDelReason( static_cast<KTutorialManager::TUTORIAL_DEL_REASON>(kInfo.m_cReason) );
#else SERV_NETERROR_STR_GET_FROM_CLIENT
			kPacket.m_wstrReason = SiKTutorialManager()->GetDelReason( static_cast<KTutorialManager::TUTORIAL_DEL_REASON>(kInfo.m_cReason) );
#endif SERV_NETERROR_STR_GET_FROM_CLIENT
			//}}

			spUser->SendPacket( EGS_DEL_TUTORIAL_NOT, kPacket );

			if( vit->m_bIsOnline == true )
			{
				//상대방에게는 내 uid가 삭제대상이다.
				KELG_DEL_TUTORIAL_NOT kNot;
				kNot.m_iReceiverUnitUID = vit->m_iUnitUID;
				kNot.m_iDeleteUnitUID = spUser->GetCharUID();
				//{{ 2011.09.16 조효진  넷 에러 스트링을 클라이언트쪽에서 가져와서 출력
#ifdef SERV_NETERROR_STR_GET_FROM_CLIENT
				kNot.m_iReason = SiKTutorialManager()->GetDelReason( static_cast<KTutorialManager::TUTORIAL_DEL_REASON>(kInfo.m_cReason) );
#else SERV_NETERROR_STR_GET_FROM_CLIENT
				kNot.m_wstrReason = SiKTutorialManager()->GetDelReason( static_cast<KTutorialManager::TUTORIAL_DEL_REASON>(kInfo.m_cReason) );
#endif SERV_NETERROR_STR_GET_FROM_CLIENT
				//}}
				spUser->SendToLoginServer( ELG_DEL_TUTORIAL_NOT, kNot );
			}

			m_vecTutorInfo.erase( vit );
			
			kAck.m_iOK = NetError::NET_OK;
			spUser->SendPacket( EGS_DEL_TUTORIAL_ACK, kAck );
			return true;
		}
	}

	kAck.m_iOK = NetError::ERR_TUTORIAL_11;
	spUser->SendPacket( EGS_DEL_TUTORIAL_ACK, kAck );
	return false;
}

bool KUserTutorialInfo::OnDelTutorialNot( KGSUserPtr spUser, KEGS_DEL_TUTORIAL_NOT& kInfo )
{
	if( spUser->IsGuestUser() == true )
		return true;

	//{{ 2012. 07. 27	최육사		사제시스템 EXP 보너스 버그 수정
#ifdef SERV_TUTO_EXP_BONUS_BUG_FIX
	if( spUser->GetStateID() == KGSFSM::S_ROOM  &&  spUser->GetRoomUID() != 0 )
	{
		switch( CXSLRoom::GetRoomType( spUser->GetRoomUID() ) )
		{
		case CXSLRoom::RT_PVP:
		case CXSLRoom::RT_DUNGEON:
			{
                KERM_UPDATE_TUTORIAL_INFO_NOT kPacketToRm;
				spUser->GetTutorUnitUIDList( kPacketToRm.m_vecStudentUnitUID );
				spUser->SendToCnRoom( ERM_UPDATE_TUTORIAL_INFO_NOT, kPacketToRm );
			}
			break;
		}
	}
#endif SERV_TUTO_EXP_BONUS_BUG_FIX
	//}}

	std::vector<KTutorialUnitInfo>::iterator vit;
	for( vit = m_vecTutorInfo.begin(); vit != m_vecTutorInfo.end(); ++vit )
	{
		if( vit->m_iUnitUID == kInfo.m_iUnitUID )
		{
			spUser->SendPacket( EGS_DEL_TUTORIAL_NOT, kInfo );

			m_vecTutorInfo.erase( vit );

			return true;
		}
	}

	return false;
}

bool KUserTutorialInfo::OnTutorialUpdateUnitInfoNot( KGSUserPtr spUser, KEGS_TUTORIAL_UPDATE_UNIT_INFO_NOT& kInfo )
{
	if( spUser->IsGuestUser() == true )
		return true;

	std::vector<KTutorialUnitInfo>::iterator vit;
	for( vit = m_vecTutorInfo.begin(); vit != m_vecTutorInfo.end(); ++vit )
	{
		if( kInfo.m_kTutorialUnitInfo.m_iUnitUID == vit->m_iUnitUID )
		{
			*vit = kInfo.m_kTutorialUnitInfo;

			spUser->SendPacket( EGS_TUTORIAL_UPDATE_UNIT_INFO_NOT, kInfo );

			//업데이트가 왔는데 나의 정보를 다시 업데이트 요청할때 보내준다.
			if( kInfo.m_bIsReply == true )
			{
				SendUpdateUnitInfo( spUser, kInfo.m_kTutorialUnitInfo.m_iUnitUID );
			}

			return true;
		}
	}

	//처음 사제관계를 맺는 제자가 업데이트를 받을경우.
	if( m_vecTutorInfo.empty() == true )
	{
		m_vecTutorInfo.push_back( kInfo.m_kTutorialUnitInfo );

		spUser->SendPacket( EGS_TUTORIAL_UPDATE_UNIT_INFO_NOT, kInfo );

		return true;
	}

	return false;
}

void KUserTutorialInfo::SendUpdateUnitInfo( KGSUserPtr spUser, UidType iReceiverUID, bool bIsOnline /* = true */, bool bIsReply /* = false  */)
{
	if( spUser->IsGuestUser() == true )
		return;

	KEGS_TUTORIAL_UPDATE_UNIT_INFO_NOT kNot;
	kNot.m_bIsReply		= bIsReply;
	kNot.m_iReceiverUID	= iReceiverUID;

	kNot.m_kTutorialUnitInfo.m_bIsOnline	= bIsOnline;
	kNot.m_kTutorialUnitInfo.m_iUnitUID		= spUser->GetCharUID();
	kNot.m_kTutorialUnitInfo.m_ucLevel		= spUser->GetLevel();
	kNot.m_kTutorialUnitInfo.m_wstrNickName	= spUser->GetCharName();

	spUser->SendToLoginServer( ELG_TUTORIAL_UPDATE_UNIT_INFO_NOT, kNot );
}

void KUserTutorialInfo::OnDeleteUnitAck( KGSUserPtr spUser )
{
	if( spUser->IsGuestUser() == true )
		return;

	std::vector<KTutorialUnitInfo>::iterator vit;
	for( vit = m_vecTutorInfo.begin(); vit != m_vecTutorInfo.end(); ++vit )
	{
		KEGS_DEL_TUTORIAL_NOT kPacket;
		kPacket.m_iUnitUID = vit->m_iUnitUID;
		//{{ 2011.09.16 조효진  넷 에러 스트링을 클라이언트쪽에서 가져와서 출력
#ifdef SERV_NETERROR_STR_GET_FROM_CLIENT
		kPacket.m_iReason = SiKTutorialManager()->GetDelReason( KTutorialManager::TDR_DELETE_UNIT );
#else SERV_NETERROR_STR_GET_FROM_CLIENT
		kPacket.m_wstrReason = SiKTutorialManager()->GetDelReason( KTutorialManager::TDR_DELETE_UNIT );
#endif SERV_NETERROR_STR_GET_FROM_CLIENT
		//}}

		spUser->SendPacket( EGS_DEL_TUTORIAL_NOT, kPacket );

		if( vit->m_bIsOnline == true )
		{
			KELG_DEL_TUTORIAL_NOT kNot;
			kNot.m_iReceiverUnitUID = vit->m_iUnitUID;
			kNot.m_iDeleteUnitUID = m_iDeleteUnitUID;
			//{{ 2011.09.16 조효진  넷 에러 스트링을 클라이언트쪽에서 가져와서 출력
#ifdef SERV_NETERROR_STR_GET_FROM_CLIENT
			kNot.m_iReason = SiKTutorialManager()->GetDelReason( KTutorialManager::TDR_DELETE_UNIT );
#else SERV_NETERROR_STR_GET_FROM_CLIENT
			kNot.m_wstrReason = SiKTutorialManager()->GetDelReason( KTutorialManager::TDR_DELETE_UNIT );
#endif SERV_NETERROR_STR_GET_FROM_CLIENT
			//}}
			spUser->SendToLoginServer( ELG_DEL_TUTORIAL_NOT, kNot );
		}
	}

	ResetData();
}

void KUserTutorialInfo::DisconnectUpdateUnitInfo( KGSUserPtr spUser )
{
	if( spUser->IsGuestUser() == true )
		return;

	//접속종료시 나의 사제관계 유저들에게 접속종료를 알린다. 
	std::vector<KTutorialUnitInfo>::iterator vit;
	for( vit = m_vecTutorInfo.begin(); vit != m_vecTutorInfo.end(); ++vit )
	{
		SendUpdateUnitInfo( spUser, vit->m_iUnitUID, false );
	}
}

void KUserTutorialInfo::OnInsertTutorialAck( KGSUserPtr spUser, KTutorialUnitInfo& kInfo )
{
	if( spUser->IsGuestUser() == true )
		return;

	m_vecTutorInfo.push_back( kInfo );

	//상대방에게 업데이트 정보 날려주기.
	SendUpdateUnitInfo( spUser, kInfo.m_iUnitUID );
}

void KUserTutorialInfo::ProcCharLevelUP( u_char ucBeforeCharLv, KGSUserPtr spUser )
{
	if( spUser->IsGuestUser() == true )
		return;

	//제자일경우만 처리를 시작함.
	if( GetTutorUnitType() == TUT_STUDENT )
	{
		if( GetTutorialUnitNum() > KTutorialManager::TEACHER_NUM )
		{
			START_LOG( cerr, L"스승이 너무 많다.?" )
				<< BUILD_LOG( spUser->GetCharUID() )
				<< BUILD_LOG( spUser->GetCharName() )
				<< BUILD_LOG( GetTutorialUnitNum() )
				<< END_LOG;
		}

		//일반 우체국보상.
		if( ( ucBeforeCharLv + 1 ) < KTutorialManager::UNIT_LEVEL_STUDENT_MAX )
		{
			//{{ 2009. 2. 20  최육사	한번에 2레벨 이상 업했을 경우 보상처리 수정
			for( u_char ucLv = ucBeforeCharLv + 1; ucLv <= spUser->GetLevel(); ++ucLv )
			{
				int iRewardID = 0;
				switch( ucLv )
				{
				case 2:
				case 3:
					iRewardID = 9;
					break;

				case 4:
				case 5:
				case 6:
					iRewardID = 10;
					break;

				case 7:
				case 8:
				case 9:
					iRewardID = 11;
					break;

				case 10:
					// [참고] 8레벨에서 한번에 10렙까지 올라가는경우
					break;

				default:					
					START_LOG( cerr, L"레벨이 이상하다?" )
						<< BUILD_LOG( spUser->GetCharUID() )
						<< BUILD_LOG( static_cast<int>(ucLv) )
						<< END_LOG;
					break;
				}

				if( iRewardID != 0 )
				{
					std::vector<KTutorialUnitInfo>::iterator vit;
					for( vit = m_vecTutorInfo.begin(); vit != m_vecTutorInfo.end(); ++vit )
					{
						//{{ 2008. 4. 8  최육사  레벨업 보상
						KDBE_INSERT_REWARD_TO_POST_REQ kPacketToDB;
						kPacketToDB.m_iFromUnitUID = spUser->GetCharUID();
						kPacketToDB.m_iToUnitUID   = vit->m_iUnitUID;
						kPacketToDB.m_iRewardType  = KPostItemInfo::LT_TUTOL;
						kPacketToDB.m_iRewardID	   = iRewardID; // 스승의 은혜 큐브						

						CStringW cwstrMessage;
						cwstrMessage.Format( L"%s;%d", spUser->GetCharName().c_str(), ucLv );
						kPacketToDB.m_wstrMessage = static_cast<LPCTSTR>(cwstrMessage);

						spUser->SendToGameDB( DBE_INSERT_REWARD_TO_POST_REQ, kPacketToDB );
						//}}

						//우선 업데이트 정보를 스승에게 알려주고.
						SendUpdateUnitInfo( spUser, vit->m_iUnitUID );
					}
				}
			}
			//}}
		}

		//졸업이면 보상처리후 디비에서도 지워준다.
		if( spUser->GetLevel() >= KTutorialManager::UNIT_LEVEL_STUDENT_MAX  &&  ucBeforeCharLv < KTutorialManager::UNIT_LEVEL_STUDENT_MAX )
		{
			std::vector<KTutorialUnitInfo>::iterator vit;
			for( vit = m_vecTutorInfo.begin(); vit != m_vecTutorInfo.end(); ++vit )
			{
				//{{ 2008. 4. 8  최육사  제자 졸업 보상
				KDBE_INSERT_REWARD_TO_POST_REQ kPacketToDB;
				kPacketToDB.m_iFromUnitUID = spUser->GetCharUID();
				kPacketToDB.m_iToUnitUID   = vit->m_iUnitUID;
				kPacketToDB.m_iRewardType  = KPostItemInfo::LT_TUTOL;
				kPacketToDB.m_iRewardID	   = 6; // 부활석 5개				

				CStringW cwstrMessage;
				cwstrMessage.Format( L"%s;%d", spUser->GetCharName().c_str(), spUser->GetLevel() );
				kPacketToDB.m_wstrMessage = static_cast<LPCTSTR>(cwstrMessage);

				spUser->SendToGameDB( DBE_INSERT_REWARD_TO_POST_REQ, kPacketToDB );
				//}}

				//사제관계를 끈어준다.
				KDBE_DEL_TUTORIAL_REQ kPacket;

				if( GetTutorUnitType() == TUT_STUDENT )
				{
					kPacket.m_iTeacherUID = vit->m_iUnitUID;
					kPacket.m_iStudentUID = spUser->GetCharUID();
				}
				else
				{
					kPacket.m_iTeacherUID = spUser->GetCharUID();
					kPacket.m_iStudentUID = vit->m_iUnitUID;

					START_LOG( cerr, L"+ㅁ+ 난 제자란 말이지..!" )
						<< BUILD_LOG( spUser->GetCharUID() )
						<< BUILD_LOG( spUser->GetCharName() )
						<< BUILD_LOG( vit->m_iUnitUID )
						<< END_LOG;
				}

				kPacket.m_cReason = KTutorialManager::TDR_LEVEL_UP_END;
				spUser->SendToGameDB( DBE_DEL_TUTORIAL_REQ, kPacket );
			}

			//유저 상태를 교체시켜준다. 졸업레벨 이니깐..
			if( IsExistbyTutorData() == false )
			{
				SetTutorUnitType( TUT_NONE );
			}
		}
	}
}

void KUserTutorialInfo::ResetData()
{
	m_vecTutorInfo.clear();
	m_TutorUnitType = TUT_NONE;
	m_iDeleteUnitUID = 0;
}

#ifdef SERV_JUMPING_CHARACTER// 작업날짜: 2013-07-12	// 박세훈
void KUserTutorialInfo::ProcJumpingCharLevelUP( IN const u_char ucBeforeCharLv, IN const KGSUserPtr spUser )
{
	if( spUser->IsGuestUser() == true )
		return;

	//제자일경우만 처리를 시작함.
	if( GetTutorUnitType() == TUT_STUDENT )
	{
		if( GetTutorialUnitNum() > KTutorialManager::TEACHER_NUM )
		{
			START_LOG( cerr, L"스승이 너무 많다.?" )
				<< BUILD_LOG( spUser->GetCharUID() )
				<< BUILD_LOG( spUser->GetCharName() )
				<< BUILD_LOG( GetTutorialUnitNum() )
				<< END_LOG;
		}

		//일반 우체국보상.
		if( ( ucBeforeCharLv + 1 ) < KTutorialManager::UNIT_LEVEL_STUDENT_MAX )
		{
			//{{ 2009. 2. 20  최육사	한번에 2레벨 이상 업했을 경우 보상처리 수정
			for( u_char ucLv = ucBeforeCharLv + 1; ucLv <= spUser->GetLevel(); ++ucLv )
			{
				int iRewardID = 0;
				switch( ucLv )
				{
				case 2:
				case 3:
					iRewardID = 9;
					break;

				case 4:
				case 5:
				case 6:
					iRewardID = 10;
					break;

				case 7:
				case 8:
				case 9:
					iRewardID = 11;
					break;

				case 10:
					// [참고] 8레벨에서 한번에 10렙까지 올라가는경우
					break;

				default:
					break;
				}

				if( iRewardID != 0 )
				{
					std::vector<KTutorialUnitInfo>::iterator vit;
					for( vit = m_vecTutorInfo.begin(); vit != m_vecTutorInfo.end(); ++vit )
					{
						//우선 업데이트 정보를 스승에게 알려주고.
						SendUpdateUnitInfo( spUser, vit->m_iUnitUID );
					}
				}
			}
			//}}
		}

		//졸업이면 보상처리후 디비에서도 지워준다.
		if( spUser->GetLevel() >= KTutorialManager::UNIT_LEVEL_STUDENT_MAX  &&  ucBeforeCharLv < KTutorialManager::UNIT_LEVEL_STUDENT_MAX )
		{
			std::vector<KTutorialUnitInfo>::iterator vit;
			for( vit = m_vecTutorInfo.begin(); vit != m_vecTutorInfo.end(); ++vit )
			{
				//사제관계를 끈어준다.
				KDBE_DEL_TUTORIAL_REQ kPacket;

				if( GetTutorUnitType() == TUT_STUDENT )
				{
					kPacket.m_iTeacherUID = vit->m_iUnitUID;
					kPacket.m_iStudentUID = spUser->GetCharUID();
				}
				else
				{
					kPacket.m_iTeacherUID = spUser->GetCharUID();
					kPacket.m_iStudentUID = vit->m_iUnitUID;

					START_LOG( cerr, L"+ㅁ+ 난 제자란 말이지..!" )
						<< BUILD_LOG( spUser->GetCharUID() )
						<< BUILD_LOG( spUser->GetCharName() )
						<< BUILD_LOG( vit->m_iUnitUID )
						<< END_LOG;
				}

				kPacket.m_cReason = KTutorialManager::TDR_LEVEL_UP_END;
				spUser->SendToGameDB( DBE_DEL_TUTORIAL_REQ, kPacket );
			}

			//유저 상태를 교체시켜준다. 졸업레벨 이니깐..
			if( IsExistbyTutorData() == false )
			{
				SetTutorUnitType( TUT_NONE );
			}
		}
	}
}
#endif // SERV_JUMPING_CHARACTER
