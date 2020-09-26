#pragma once

#ifdef ADDED_RELATIONSHIP_SYSTEM


namespace		_CONST_RELATIONSHIP_MANAGER_
{
	const float			g_fRefreshRelationDataCoolTime = 5.0f;
}

// 김종훈 / 13-03-28 / 인연 시스템 매니저 클래스 선언

class CX2UIRelationship;

// class CX2UIRelationship

class CX2RelationshipManager 
{
	public :
		
		enum PROPOSE_RETURNED_MESSAGE 
		{
			PRM_OK = 0,
			// 아래로는 전부 거절 메시지
			RPM_TIME_OUT,			/// 시간 종료
			RPM_NOT_THIS_PLACE,		/// 같은 위치가 아닌 경우
			RPM_AUTO_DISAGREE,		/// 던전, 대전 자동 거절
			PRM_DISAGREE,			/// 거절
			PRM_OPTION_DISAGREE,	/// 옵션창에서 커플요청 거부를 설정하였을 때
		};


		struct RelationshipInfo 
		{
			SEnum::RELATIONSHIP_TYPE			m_eRelationStateType;				// 해당 캐릭터의 m_iReleationTargetUserUid 와의 관계
			UidType								m_iRelationTargetUserUid;			// 해당 캐릭터와 관계가 있는 유저의 UID
			CTimeSpan							m_ctsDurationDay;						// 해당 캐릭터와 지나온 나날들			
			wstring								m_wstrRelationTargetUserNickname;	// 해당 캐릭터의 아이디
			wstring								m_wstrNotifyLoveMessage;			// 해당 캐릭터와의 사랑 알림말
			CX2Unit *							m_pOtherUnit;						
			std::map< int, KInventoryItemInfo > m_mapOtherUnitEquippedItem;
			int									m_iPossibleInvitingUserNumber;		// 최대 초대 가능한 유저의 숫자
			int									m_iAddPossibleInvitingUserNumber;	// 추가로 초대 가능한 유저의 숫자
			RelationshipInfo () : m_eRelationStateType ( SEnum::RT_SOLO ), m_iRelationTargetUserUid ( 0 ),
								  m_ctsDurationDay ( 0 ), m_wstrRelationTargetUserNickname ( L"" ),
								  m_wstrNotifyLoveMessage ( L"" ) 
			{			
				m_pOtherUnit = NULL;
			}

			~RelationshipInfo()
			{
				SAFE_DELETE ( m_pOtherUnit );
			}

			void ClearRelationInfo ()
			{
				m_eRelationStateType = SEnum::RT_SOLO;
				m_iRelationTargetUserUid = 0;
				m_ctsDurationDay = 0;
				m_iPossibleInvitingUserNumber = 0;
				m_iAddPossibleInvitingUserNumber = 0;
				m_wstrRelationTargetUserNickname = L"";
				m_wstrNotifyLoveMessage = L"";
			}

			int GetDurationDay() const { return static_cast<int>(m_ctsDurationDay.GetDays()) + 1 ;}
			int GetPossibleInvitingUserMany ( ) { return m_iPossibleInvitingUserNumber + m_iAddPossibleInvitingUserNumber; }
		};		
		
		struct RelationshipCommunityInfo 
		{
			char						m_cPosition;		// 현재 상태
			u_char						m_ucLevel;			// 레벨
			char						m_cUnitClass;		// 클래스
			int							m_iMapID;			// 위치
			__int64						m_tLastLogOutTime;	// 마지막 접속 시간
			bool						m_bIsConnect;		// 현재 접속 여부
			wstring						m_wstrChannelName;	// 채널 이름
			wstring						m_wstrNickName;		// 아이디

			RelationshipCommunityInfo() : m_cPosition (0), m_ucLevel (0),
			m_cUnitClass(0), m_iMapID(0), m_tLastLogOutTime(0), m_bIsConnect(FALSE),
			m_wstrChannelName(L""), m_wstrNickName(L"")
			{
			}
			
			void ClearPartnerInfo ()
			{
				m_cPosition			= 0;
				m_ucLevel			= 0;
				m_cUnitClass		= 0;
				m_iMapID			= 0;
				m_tLastLogOutTime	= 0;
				m_bIsConnect		= FALSE;
				m_wstrChannelName	= L"";
				m_wstrNickName		= L"";
			}
		};


		// 결혼 아이템에 관한 정보를 담고 있다.
		// 해당 아이템 ID 가 청첩장 일 경우 검사한다.
		// ItemSlotManager 의 Item UID 와 WeddingItemInfo 구조체 맵에서
		// UID 로 검색하여 WeddingItemInfo 데이터를 가져온다.
		// WeddingItemInfo 에는 신랑 아이디, 신부 아이디, 주례사 ID, 월드 ID, 일시, 편지 글귀 가 담겨 있다. 
		struct WeddingCoupleNickName 
		{
			wstring m_wstrWeddingMaleNickname;	// 신랑 아이디
			wstring m_wstrWeddingFemaleNickname;	// 신부 아이디

			WeddingCoupleNickName()
			{
				m_wstrWeddingMaleNickname = L"";
				m_wstrWeddingFemaleNickname = L"";
			};

			WeddingCoupleNickName ( wstring wstrWeddingMaleNickname, wstring wstrWeddingFemaleNickname )
			{
				m_wstrWeddingMaleNickname = wstrWeddingMaleNickname;
				m_wstrWeddingFemaleNickname = wstrWeddingFemaleNickname;	
			};
		};

		struct WeddingItemInfo
		{
			WeddingCoupleNickName m_CoupleNickName;		// 신랑 신부 ID
			int					  m_cOfficiantID;		// 주례사 ID
			int					  m_iWeddingHallUID;			// 월드 ID
			wstring					  m_wstrLetterMessage;  // 편지 글귀
			char				  m_cWeddingHallType;	// 웨딩 홀 타입
			wstring				  m_wstrWeddingDate;	// 결혼 하는 날
			UidType					  m_iMaleID;			// 신랑 ID
			UidType					  m_iFemaleID;			// 신부 ID

			wstring					GetWeddingFemaleNickname() const { return m_CoupleNickName.m_wstrWeddingFemaleNickname; }
			void					SetWeddingFemaleNickname(wstring val) { m_CoupleNickName.m_wstrWeddingFemaleNickname = val; }
			wstring					GetWeddingMaleNickname() const { return m_CoupleNickName.m_wstrWeddingMaleNickname; }
			void					SetWeddingMaleNickname(wstring val) { m_CoupleNickName.m_wstrWeddingMaleNickname = val; }
			WeddingCoupleNickName	GetWeddingCoupleNickName () const { return m_CoupleNickName; }
			

			void Clear()
			{
				m_CoupleNickName.m_wstrWeddingFemaleNickname	= L"";
				m_CoupleNickName.m_wstrWeddingMaleNickname		= L"";
				m_cOfficiantID									= 0;	
				m_iWeddingHallUID								= 0;
				m_wstrLetterMessage								= L"";
				m_wstrWeddingDate								= L"";
				m_cWeddingHallType								= 0;
				m_iMaleID										= 0;
				m_iFemaleID										= 0;
			}
		};

		CX2RelationshipManager();
		~CX2RelationshipManager();

		HRESULT			OnFrameMove( double fTime, float fElapsedTime );
		bool			UICustomEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
		bool			UIServerEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

		bool			Send_EGS_GET_GUILD_USER_LIST_REQ ();
		bool			Handler_EGS_GET_GUILD_USER_LIST_ACK ( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
		bool			Send_EGS_COUPLE_PROPOSE_REQ ( wstring wstrNickName_ );
		bool			Handler_EGS_CHANGE_LOVE_WORD_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
		bool			Send_EGS_CHANGE_LOVE_WORD_REQ ( UidType ItemUID_, wstring wstrMessage );
		bool			Handler_EGS_COUPLE_PROPOSE_ACK ( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
		bool			Handler_EGS_COUPLE_PROPOSE_NOT ( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
		bool			Send_EGS_RELATIONSHIP_INFO_REQ ();
		bool			Send_EGS_CHECK_NICK_NAME_REQ ( wstring wstrNickname_ );
		bool			Handler_EGS_CHECK_NICK_NAME_ACK  ( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
		bool			Handler_EGS_CHANGE_LOVE_WORD_ACK ( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
		bool			Handler_EGS_JOIN_WEDDING_HALL_REQ ( UidType iItemUID_ );
		bool			Handler_EGS_JOIN_WEDDING_HALL_ACK ( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
		bool			Handler_EGS_CHECK_BREAK_UP_ACK ( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
		bool			Handler_EGS_BREAK_UP_ACK ( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
		bool			Handler_EGS_BREAK_UP_NOT ( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

		bool			Send_EGS_COUPLE_PROPOSE_AGREE_NOT ( PROPOSE_RETURNED_MESSAGE eAgreeCouple );
		bool			Handler_EGS_COUPLE_PROPOSE_RESULT_NOT ( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
		bool			Send_EGS_WEDDING_PROPOSE_REQ ( KEGS_WEDDING_PROPOSE_REQ & kEvent );
		bool			Handler_EGS_WEDDING_PROPOSE_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
		bool			Send_EGS_WEDDING_ITEM_INFO_REQ ( std::vector< UidType > & m_vecRelationItemUID_);
		bool			Handler_EGS_WEDDING_ITEM_INFO_ACK ( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
		bool			Handler_EGS_RELATIONSHIP_EFFECT_NOT ( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
		bool			Send_EGS_RELATIONSHIP_EFFECT_NOT  ( int Idx_ );

		RelationshipInfo *						GetMyRelationshipInfo ( ) const { return m_pMyRelationshipInfo; }
		CX2UIRelationship *						GetUIRelationship () const { return m_pUIRelationship; }
		SEnum::RELATIONSHIP_TYPE				GetMyRelationInfoType () const { return ( NULL != m_pMyRelationshipInfo ? m_pMyRelationshipInfo->m_eRelationStateType : SEnum::RT_SOLO ); }
		WeddingCoupleNickName					GetWeddingInviteLetterCoupleNickNameByItemUID ( UidType ItemUID );
		WeddingItemInfo *						GetMyWeddingItemInfo () const { return m_pMyWeddingItemInfo; }


		float			GetRefreshDataCoolTime() const { return m_fRefreshDataCoolTime; }
		void			SetRefreshDataCoolTime(float val) { m_fRefreshDataCoolTime = val; }
		bool			GetIsPossibleRefreshData() const { return m_bIsPossibleRefreshData; }
		void			SetIsPossibleRefreshData(bool val) { m_bIsPossibleRefreshData = val; }


		void			AddWeddingInviteLetterNickNameMap ( UidType ItemUID_, wstring wstrMaleName_, wstring wstrFemaleName_ ) 
		{
			m_mapMyWeddingInviteLetterCoupleNickNameInfo.insert( std::make_pair ( ItemUID_, WeddingCoupleNickName ( wstrMaleName_, wstrFemaleName_ ) ) );
		}
		
		WeddingItemInfo * GetWeddingInviteLetterInfoMapByItemUID ( UidType ItemUID_ )
		{
			map<UidType, WeddingItemInfo>::iterator it = m_mapMyWeddingInviteLetterItemInfo.begin();

			for ( ; it != m_mapMyWeddingInviteLetterItemInfo.end(); ++it )
			{
				if ( it->first == ItemUID_ )
					return &(it->second);		
			}
			return NULL;
		}

		void			AddWeddingInviteLetterInfoMap ( UidType ItemUID_, WeddingItemInfo WeddingItemInfo_ )
		{
			m_mapMyWeddingInviteLetterItemInfo.insert( std::make_pair ( ItemUID_, WeddingItemInfo_ ) );
		}

		void			ClearRelationCommunityInfo ()    
		{
			m_vecRelationCommunityInfo.clear();
		}

		vector<RelationshipCommunityInfo> & GetRelationshipCommunityInfo ()
		{
			return m_vecRelationCommunityInfo;
		}	
		
		CX2GameUnitoPtr GetRelationPartnerInGame() const { return m_optrGameUnitRelationPartner; }
		void SetRelationPartnerInGame(CX2GameUnitoPtr val) { m_optrGameUnitRelationPartner = val; }
		void ResetRelationPartnerInGame () { m_optrGameUnitRelationPartner.Reset(); }
		void FindRelationPartnerInGame ();

		CX2SquareUnitPtr GetRelationPartnerInVillage() const { return m_pSquareUnitRelationPartner; }
		void SetRelationPartnerInVillage(CX2SquareUnitPtr val) {  m_pSquareUnitRelationPartner = val; }
		void ResetRelationPartnerInVillage () { m_pSquareUnitRelationPartner.reset(); }
		bool FindAndSetRelationPartnerInVillage ();
		bool FindAndSetRelationPartnerInGame ();
		bool IsEqipWeddingClothes() const;

		bool CheckWeddingQualification() const;// 웨딩 가능 여부 체크
private:

private :
		bool					m_bIsPossibleRefreshData;		// 리플래시 가능? ( 5초 쿨타임 설정 ) 	
		float					m_fRefreshDataCoolTime;		// 0 이면 리플래시 가능, 
		RelationshipInfo *		m_pMyRelationshipInfo;		// 내 관계 정보
		CX2UIRelationship *		m_pUIRelationship;			// 인연 UI 
		vector<RelationshipCommunityInfo>  m_vecRelationCommunityInfo;	 // 인연 정보, 커뮤니티 창, 인연 탭 용
		map<UidType, WeddingCoupleNickName> m_mapMyWeddingInviteLetterCoupleNickNameInfo; // 청첩장 오버롤 했을 때 띄울 이름 정보를 보관하는 곳
		map<UidType, WeddingItemInfo> m_mapMyWeddingInviteLetterItemInfo;			// 청첩장 정보
		WeddingItemInfo *		m_pMyWeddingItemInfo;						
		CX2GameUnitoPtr	m_optrGameUnitRelationPartner;	// 나의 인연
		CX2SquareUnitPtr m_pSquareUnitRelationPartner;
		
#ifdef ADJUST_THIRD_PERSON_BUG
		bool	m_bPropose;	//현재 내가 커플 신청을 한 상태임을 체크하기 위한 변수.
#endif //ADJUST_THIRD_PERSON_BUG
#ifdef RELATIONSHIP_SHOW_GUILD_MEMBER_FIX
		bool	m_bInviteGuildMember;
#endif //RELATIONSHIP_SHOW_GUILD_MEMBER_FIX
};


class CX2RelationshipEffectManager
{

	public :
	
	enum RELATION_EFFECT_SHOW_TYPE 
	{
		REST_SHOW_ALL = 0,
		REST_HIDE_ALL,
		REST_SHOW_PARTY_ONLY,
		REST_HIDE_NOT_PARTY_ONLY,
	};

	struct RelationEffectInfo
	{	
		enum RELATION_EFFECT_PLAY_TYPE 
		{
			RET_NONE = 0,
			RET_ATTACH_EFFECT = 1,
			RET_CLOSE_EFFECT = 2,
			RET_NOTIFY_EFFECT = 3,
			RET_COUPLE_MATCHING_EFFECT = 4,
		};

		enum RELATION_EFFECT_PLAY_OWNER
		{
			REO_MINE_MALE = 0,
			REO_MINE_FEMALE,
			REO_OTHER,
		};

	
		RelationEffectInfo::RelationEffectInfo() :
				m_iIndex(-1),
				m_fPopNotifyEffectTime(0),
				m_fRelationEffectPlayingTime(0),
				m_wstrAttachEffectName (L""),
				m_wstrCloseEffectName(L""),
				m_wstrNotifyEffectName(L""),
				m_wstrUserInputMessage(L""),
				m_fRelationCloseEffectDistance(0),
				m_fRelationEffectCooltime (0),
				m_bRealtionEffectPopNotifyCheck(false),
				m_bIsErase (false),
				m_iUIDOwnerAttachEffect ( 0 ),
				m_vCloseEffectPosition ( D3DXVECTOR3 ( 0, 0, 0 ) )
			{
// 후에 인연 이펙트 추가 되면 여기 수정해야함!
				m_iIndex = 1;			
			}

			// 파티클은 UI Minor Particle 
			// 메쉬는   UI Minor Mesh Player

			int		m_iIndex;					// 인덱스 번호를 지정하여 Effect 들을 불러오게 됨			
			bool	m_bIsErase;
			float	m_fRelationEffectCooltime;
			bool	m_bRealtionEffectPopNotifyCheck;
			D3DXVECTOR3		m_vCloseEffectPosition; // Close Effect 가 뿌려진 위치
			float	m_fPopNotifyEffectTime;		//  Notify Effect 가 언제 등장할 것인가
			float	m_fRelationEffectPlayingTime; //  Effect 가 Play 되고 있는 시간,
			// 쿨타임 및 Notify Effect 의 Playtime 에 영향을 끼침
			UidType m_iUIDOwnerAttachEffect;
	
			wstring m_wstrAttachEffectName;		// 플레이어에게 붙는 하트 같은 Particle 이펙트 
			wstring m_wstrCloseEffectName;		// 관계된 플레이어에게 근접할 시 등장하는 Effect Name
			wstring m_wstrNotifyEffectName;		// Close Effect 가 등장한 이후 m_fRelationEffectPlayingTime 에
			// 따라 등장하게되는 Notify Effect 의 이름

			wstring m_wstrUserInputMessage;		// 유저가 입력한 인연 메시지
			
			float	m_fRelationCloseEffectDistance; // Close Effect 가 등장할 거리 조건
			
			float		GetRelationEffectCooltime() const { return m_fRelationEffectCooltime; }
			void		SetRelationEffectCooltime(float val) { m_fRelationEffectCooltime = val; }

			bool		FindAndSetMyRelationPartner ( UidType Uid_ );
	
			bool		GetRealtionEffectPopNotifyCheck() const { return m_bRealtionEffectPopNotifyCheck; }
			void		SetRealtionEffectPopNotifyCheck(bool val) { m_bRealtionEffectPopNotifyCheck = val; }

			int			GetIndex() const { return m_iIndex; }
			void		SetIndex(int val) { m_iIndex = val; }

			float		GetRelationEffectPlayingTime() const { return m_fRelationEffectPlayingTime; }
			void		SetRelationEffectPlayingTime(float val) { m_fRelationEffectPlayingTime = val; }
		
			float		GetPopNotifyEffectTime() const { return m_fPopNotifyEffectTime; }
			void		SetPopNotifyEffectTime(float val) { m_fPopNotifyEffectTime = val; }

			float		GetRelationCloseEffectDistance() const { return m_fRelationCloseEffectDistance; }
			void		SetRelationCloseEffectDistance(float val) { m_fRelationCloseEffectDistance = val; }

			wstring		GetUserInputMessage() const { return m_wstrUserInputMessage; }
			void		SetUserInputMessage(wstring val) { m_wstrUserInputMessage = val; }

			wstring		GetAttachEffectName() const { return m_wstrAttachEffectName; }
			void		SetAttachEffectName(wstring val) { m_wstrAttachEffectName = val; }

			wstring		GetCloseEffectName() const { return m_wstrCloseEffectName; }
			void		SetCloseEffectName(wstring val) { m_wstrCloseEffectName = val; }

			wstring		GetNotifyEffectName() const { return m_wstrNotifyEffectName; }
			void		SetNotifyEffectName(wstring val) { m_wstrNotifyEffectName = val; }

			void		SetCloseEffectPosition ( D3DXVECTOR3 vPos )  { m_vCloseEffectPosition = vPos; }
			D3DXVECTOR3	GetCloseEffectPosition () const { return m_vCloseEffectPosition; }

			UidType		GetUIDOwnerAttachEffect() const { return m_iUIDOwnerAttachEffect; }
			void		SetUIDOwnerAttachEffect(UidType val) { m_iUIDOwnerAttachEffect = val; }

			void		PrepareDrawRelationshipEffect	( RELATION_EFFECT_PLAY_TYPE ePlayingEffectType_, UidType OwnerUID_ = 0 );  // 통합
			void		DrawRelationshipEffect	( wstring wstrEffectName_, UidType Uid, CX2RelationshipEffectManager::RelationEffectInfo::RELATION_EFFECT_PLAY_TYPE eEffectType_, D3DXVECTOR3 vEffectPos );
			void        PopTalkBoxEffect			( UidType iUnitUID, wstring& wstrMsg, wstring wstrTexName, D3DXVECTOR3 vEffectPos, D3DXCOLOR color = 0xff000000, D3DXCOLOR outcolor = (DWORD)0x00000000, D3DXCOLOR backColor = (DWORD)0xffffffff );
			void		DrawRelationshipAttachEffect	();	// 1 스탭, 다른 이펙트에 영향을 받지 않음
			void		DrawRelationshipCloseEffect		();		// 2 스탭, 관계된 플레이어에게 근접 시, 등장
			void		DrawRelationshipNotifyEffect	();	// 3 스탭, Close Effect 가 등장하고 
			
//			void		PlayRelationshipEffect( wstring wstrEffectName_, UidType Uid, CX2RelationshipEffectManager::RelationEffectInfo::RELATION_EFFECT_PLAY_TYPE eEffectType_ );

			// m_fPopNotifyEffectTime 에 따라 등장하는 
			// 알림 이펙트
			
	};
	vector<RelationEffectInfo> m_vecRelationEffectInfoIndex;

	/* 이거 My Effect User Info 로 묶자	 */
	RelationEffectInfo * m_pMyRealtionEffectInfo;
	vector<RelationEffectInfo> m_vecRelationEffectInfo;
	map<UidType, CX2EffectSet::Handle> m_mapRelationshipAttachEffectHandleMap;
	float m_fMyRelationAttachEffectCooltime;
	float m_fMyRelationCloseEffectCooltime;
	int	  m_iMyRelationEffectIndex;
	bool  m_bMyRealtionEffectPopNotifyCheck;

	CX2RelationshipEffectManager::CX2RelationshipEffectManager ():
		m_pMyRealtionEffectInfo ( NULL ),
		m_fMyRelationAttachEffectCooltime ( 0 ),
		m_fMyRelationCloseEffectCooltime ( 0 ),
		m_iMyRelationEffectIndex ( 0 ),
		m_bMyRealtionEffectPopNotifyCheck ( true )
	{	  	

	}
				
	bool		IsPartnerClose ();	
	bool		GetMyRealtionEffectPopNotifyCheck() const { return m_bMyRealtionEffectPopNotifyCheck; }
	void		SetMyRealtionEffectPopNotifyCheck(bool val) { m_bMyRealtionEffectPopNotifyCheck = val; }

	int			GetMyRelationEffectIndex() const { return m_iMyRelationEffectIndex; }
	void		SetMyRelationEffectIndex(int val)	
	{	
		m_iMyRelationEffectIndex = val; 
	}

	CX2EffectSet::Handle GetRelationshipAttachEffectHandleByUID ( UidType UnitUID_ )
	{
		map<UidType, CX2EffectSet::Handle>::iterator it = m_mapRelationshipAttachEffectHandleMap.begin();
		it = m_mapRelationshipAttachEffectHandleMap.find ( UnitUID_ );
		if ( it != m_mapRelationshipAttachEffectHandleMap.end() )
		{
			return it->second;
		}
		return INVALID_EFFECTSET_HANDLE;
	}

	void		SetShowRelationshipAttachEffect ( RELATION_EFFECT_SHOW_TYPE eREST_ );
	
	void		DeleteRelationshipAttachEffectHandleByUID ( UidType UnitUID_ );

	void		SetMyRelationEffect(int idx)	
	{
		SAFE_DELETE ( m_pMyRealtionEffectInfo )
		
		m_iMyRelationEffectIndex = idx; 
		m_pMyRealtionEffectInfo = new RelationEffectInfo;
		(* m_pMyRealtionEffectInfo) = m_vecRelationEffectInfoIndex[idx];
	}
	float		GetMyRelationCloseEffectCooltime() const { return m_fMyRelationCloseEffectCooltime; }
	void		SetMyRelationCloseEffectCooltime(float val) { m_fMyRelationCloseEffectCooltime = val; }

	float		GetMyRelationAttachEffectCooltime() const { return m_fMyRelationAttachEffectCooltime; }
	void		SetMyRelationAttachEffectCooltime(float val) { m_fMyRelationAttachEffectCooltime = val; }
	
	void		SetRelationEffectInfoIndex ( );
	
	bool		CalculateCloseEffectPosition ( UidType Uid1_, UidType Uid2, D3DXVECTOR3 & vPos );
	
	void		ClearRelationEffectInfoIdex ()
	{				
		m_vecRelationEffectInfoIndex.clear();
	}

	void ClearRelationshipAttachEffectMap ()
	{
		m_mapRelationshipAttachEffectHandleMap.clear();
	}
	
	const RelationEffectInfo * GetRelationEffectInfoIndex ( int idx_ )
	{
		if ( idx_ > (int) m_vecRelationEffectInfoIndex.size() )
		{
			ASSERT ( !"EffectInfoIndex Size Over" );
			return NULL;
		}
		else
		{
			return &m_vecRelationEffectInfoIndex[idx_];
		}
		return NULL;
	}

	RelationEffectInfo * GetMyRelationEffectInfo () { return m_pMyRealtionEffectInfo; }
	void				 SetMyRelationEffectInfo ( int idx_ )
	{
		if ( idx_ > (int) m_vecRelationEffectInfoIndex.size() )
		{
			ASSERT ( !"EffectInfoIndex Size Over" );
		}
		else
		{
			m_pMyRealtionEffectInfo = &m_vecRelationEffectInfoIndex[idx_];
		}
	}
	bool			GetMyRelationCloseEffectPosition ( D3DXVECTOR3 & vec );
	

};


#endif // ADDED_RELATIONSHIP_SYSTEM
