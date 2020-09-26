/**@file : X2ComboAndSkillChainSystem.h
   @breif : 콤보와 스킬의 연쇄 사용을 위한 시스템 (한가지 버튼으로 사용 한다던지 등)
*/

#pragma once

enum COMBO_AND_SKILL_CHAIN_TYPE
{
	CASCT_NONE					= 0,
	CASCT_COMBO					= 1,
	CASCT_SKILL					= 2,
	CASCT_SECRET_SKILL			= 3,
};

class CX2ComboAndSkillChainInfo;
typedef boost::shared_ptr<CX2ComboAndSkillChainInfo> CX2ComboAndSkillChainInfoPtr;
typedef boost::weak_ptr<CX2ComboAndSkillChainInfo>	CX2ComboAndSkillChainInfoWeakPtr;

class	CX2ComboAndSkillChainInfo
{
public:
	static CX2ComboAndSkillChainInfoPtr CreateComboAndSkillChainInfoTemplet( const int iStateIdChained_, const int iSkillID_, const COMBO_AND_SKILL_CHAIN_TYPE eComboAndSkillChainType_ )
	{
		return CX2ComboAndSkillChainInfoPtr( new CX2ComboAndSkillChainInfo( iStateIdChained_, iSkillID_, eComboAndSkillChainType_) );
	}

	int GetSkillID() const { return m_iSkillID; }
	void SetSkillID(int val) { m_iSkillID = val; }

	COMBO_AND_SKILL_CHAIN_TYPE GetComboAndSkillChainType() const { return m_eComboAndSkillChainType; }
	void SetComboAndSkillChainType(COMBO_AND_SKILL_CHAIN_TYPE val) { m_eComboAndSkillChainType = val; }

	bool GetChaining() const { return m_bChaining; }
	void SetChaining(bool val) { m_bChaining = val; }

	void SetNextComboAndSkillChainInfo( CX2ComboAndSkillChainInfoPtr ptrNextChainInfo_ )
	{
		m_weakPtrNextComboAndSkillChainInfo = ptrNextChainInfo_;
	}
	
	bool IsAlreadyChaining() const
	{
		CX2ComboAndSkillChainInfoPtr ptrNextChainInfo = m_weakPtrNextComboAndSkillChainInfo.lock();

		if ( NULL == ptrNextChainInfo || false == ptrNextChainInfo->GetChaining() )
			return false;
		else
			return true;
	}

	int GetStateIdChained() const { return m_iStateIdChained; }
	void SetStateIdChained(int val) { m_iStateIdChained = val; }

	int GetStateIdMatchedToChain() const { return m_iStateIdMatchedToChain; }
	void SetStateIdMatchedToChain(int val) { m_iStateIdMatchedToChain = val; }

	int GetHyperStateIdMatchedToChain() const { return m_iHyperStateIdMatchedToChain; }
	void SetHyperStateIdMatchedToChain(int val) { m_iHyperStateIdMatchedToChain = val; }

	CX2ComboAndSkillChainInfoPtr GetNotYetChainedInfo()
	{
		CX2ComboAndSkillChainInfoPtr ptrInfo = m_weakPtrNextComboAndSkillChainInfo.lock();
		if ( NULL != ptrInfo && ptrInfo->GetChaining() )
			return ptrInfo->GetNotYetChainedInfo();
		else
			return ptrInfo;	/// NULL 이거나 아직 Chain 되지 않은 정보 
	}

	void ResetChainingFromNext()
	{
		CX2ComboAndSkillChainInfoPtr ptrInfo = m_weakPtrNextComboAndSkillChainInfo.lock();
		if ( NULL != ptrInfo )
		{
			ptrInfo->SetChaining( false );
			ptrInfo->ResetChainingFromNext();
		}		
	}

private:
	int							m_iStateIdChained;				/// 체인 시킬 유저의 스테이트 ID
	int							m_iStateIdMatchedToChain;	/// ComboAndSkillChainInfoTemplet의 스테이트를 직접적으로 사용하는 스테이트
	int							m_iHyperStateIdMatchedToChain;
	int							m_iSkillID;
	/// 가령 스킬의 경우에는 ATTACK 이라는 유저 스테이트가 있어도 스킬슬롯에 장착하면, USI_SPECIAL_ATTACK_1 스테이트 등으로 지정되어 사용됨. \
	/// 이경우 m_ptrComboAndSkillChainInfoTemplet 안에는 ATTACK 이라는 스테이트 정보를 가지고 있고, \
	/// m_iStateIdMatchedToChain 는 USI_SPECIAL_ATTACK_1 라는 정보를 가지고 있도록 함
	COMBO_AND_SKILL_CHAIN_TYPE	m_eComboAndSkillChainType;		/// m_iStateIdChained 가 콤보, 스킬, 오의 인지 구분
	bool						m_bChaining;				/// 체이닝에 사용되었는가? (한줄의 체인에서 이것들이 모두 true인 경우 오의가 발동)

	CX2ComboAndSkillChainInfoWeakPtr m_weakPtrNextComboAndSkillChainInfo;
	
	CX2ComboAndSkillChainInfo()
		: m_iStateIdChained( 0 ), m_iSkillID( 0 ), m_iStateIdMatchedToChain( 0 ), m_iHyperStateIdMatchedToChain( 0 ),
		m_eComboAndSkillChainType( CASCT_NONE ), m_bChaining( false ), m_weakPtrNextComboAndSkillChainInfo()
	{}

	CX2ComboAndSkillChainInfo( const int iStateIdChained_, const int iSkillID_, const COMBO_AND_SKILL_CHAIN_TYPE eComboAndSkillChainType_ )
		: m_iStateIdChained( iStateIdChained_ ), m_iSkillID( iSkillID_ ), m_iStateIdMatchedToChain( 0 ), m_iHyperStateIdMatchedToChain( 0 ),
		m_eComboAndSkillChainType( eComboAndSkillChainType_ ), m_bChaining( false ), m_weakPtrNextComboAndSkillChainInfo()
	{}
};

class CX2ComboAndSkillChainInfoList;
#ifdef  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
typedef  boost::intrusive_ptr<CX2ComboAndSkillChainInfoList> CX2ComboAndSkillChainInfoListPtr;
#else   X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
typedef  boost::shared_ptr<CX2ComboAndSkillChainInfoList> CX2ComboAndSkillChainInfoListPtr;
#endif  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR

class CX2ComboAndSkillChainInfoList
#ifdef  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
    : private boost::noncopyable
#endif  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
{
public:
	static CX2ComboAndSkillChainInfoListPtr CreateComboAndSkillChainInfoTemplet()
	{
		return CX2ComboAndSkillChainInfoListPtr( new CX2ComboAndSkillChainInfoList );
	}

	void	ConnectNextChainInfo( CX2ComboAndSkillChainInfoPtr ptrComboAndskillChainInfo_ )
	{
		if ( !m_vecComboAndSkillChainInfoPtr.empty() )
		{
			CX2ComboAndSkillChainInfoPtr ptrPreChainSkill = m_vecComboAndSkillChainInfoPtr.back();
			if ( NULL != ptrPreChainSkill )
				ptrPreChainSkill->SetNextComboAndSkillChainInfo( ptrComboAndskillChainInfo_ );
		}
	}

	void	PushBack( CX2ComboAndSkillChainInfoPtr ptrComboAndskillChainInfo_ )
	{	
		m_vecComboAndSkillChainInfoPtr.push_back( ptrComboAndskillChainInfo_ );
	}
	
	const vector<CX2ComboAndSkillChainInfoPtr>& GetListComboAndSkillChainInfoPtr() const { return m_vecComboAndSkillChainInfoPtr; }

	void ParsingSecretSkillStateId( KLuaManager& luaManager_ )
	{
		LUA_GET_VALUE( luaManager_, "SECRET_SKILL_STATE_ID", m_iSecretSkillStateId, 0 );
	}
	int GetSecretSkillStateId() const { return m_iSecretSkillStateId; }
	void SetSecretSkillStateId(int val) { m_iSecretSkillStateId = val; }

	void ParsingTriggerStateIdToSecretSkill( KLuaManager& luaManager_ )
	{
		LUA_GET_VALUE( luaManager_, "TRIGGER_STATE_ID_TO_SECRET_SKILL", m_iTriggerStateIdToSecretSkill, 0 );
	}
	int GetTriggerStateIdToSecretSkill() const { return m_iTriggerStateIdToSecretSkill; }
	void SetTriggerStateIdToSecretSkill(int val) { m_iTriggerStateIdToSecretSkill = val; }

	void ParsingTriggerTimeToSecretSkill( KLuaManager& luaManager_ )
	{
		LUA_GET_VALUE( luaManager_, "TRIGGER_TIME_TO_SECRET_SKILL", m_fTriggerTimeToSecretSkill, 100.0f );
	}
	float GetTriggerTimeToSecretSkill() const { return m_fTriggerTimeToSecretSkill; }
	void SetTriggerTimeToSecretSkill(float val) { m_fTriggerTimeToSecretSkill = val; }

	bool CanTriggerToSecretSkill() const
	{
		BOOST_FOREACH( CX2ComboAndSkillChainInfoPtr ptrComboAndSkillChainInfo, m_vecComboAndSkillChainInfoPtr )
		{
			if ( NULL == ptrComboAndSkillChainInfo || false == ptrComboAndSkillChainInfo->GetChaining() )
				return false;				
		}

		return true;
	}

	bool HasThisChainInfo( CX2ComboAndSkillChainInfoPtr ptrChainInfo_ )
	{
		BOOST_FOREACH( CX2ComboAndSkillChainInfoPtr ptrComboAndSkillChainInfo, m_vecComboAndSkillChainInfoPtr )
		{
			if ( NULL != ptrComboAndSkillChainInfo 
				&& ptrComboAndSkillChainInfo->GetStateIdChained() == ptrChainInfo_->GetStateIdChained() )
				return true;
		}

		return false;
	}

	void ResetChaining()
	{
		BOOST_FOREACH( CX2ComboAndSkillChainInfoPtr ptrComboAndSkillChainInfo, m_vecComboAndSkillChainInfoPtr )
		{
			if ( NULL != ptrComboAndSkillChainInfo )
				ptrComboAndSkillChainInfo->SetChaining( false );
		}
	}

#ifdef  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
    void    AddRef()    {   ++m_uRefCount; }
    void    Release()   { if ( (--m_uRefCount) == 0 )   delete this; }
#endif  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR

private:
	CX2ComboAndSkillChainInfoList() : m_iSecretSkillStateId( 0 ), m_iTriggerStateIdToSecretSkill( 0 ), m_fTriggerTimeToSecretSkill( 100.0f )
#ifdef  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
        , m_uRefCount(0)
#endif  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
	{}

#ifdef  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
    unsigned                                        m_uRefCount;
#endif  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR

	vector<CX2ComboAndSkillChainInfoPtr>		m_vecComboAndSkillChainInfoPtr;
	int											m_iSecretSkillStateId;
	int											m_iTriggerStateIdToSecretSkill;
	float										m_fTriggerTimeToSecretSkill;
};

IMPLEMENT_INTRUSIVE_PTR( CX2ComboAndSkillChainInfoList );

class CX2ComboAndSkillChainSystem;
#ifdef  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
typedef boost::intrusive_ptr<CX2ComboAndSkillChainSystem> CX2ComboAndSkillChainSystemPtr;
#else   X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
typedef boost::shared_ptr<CX2ComboAndSkillChainSystem> CX2ComboAndSkillChainSystemPtr;
#endif  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR

class CX2ComboAndSkillChainSystem
#ifdef  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
    : private boost::noncopyable
#endif  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
{
public:
	typedef std::vector<CX2ComboAndSkillChainInfoListPtr> VecComboAndSkillChainListPtr;
	
	CX2ComboAndSkillChainSystem()
#ifdef  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
        : m_uRefCount(0)
#endif  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR    
    {}

	bool	CreateComboAndSkillChain( KLuaManager& luaManager_ );
	void	InitSkillStateChained( CX2GUUser* pCX2GUUser_ );
	CX2ComboAndSkillChainInfoPtr	GetSkillChainInfoPtrBySkillID( const int iSkillID_ ) const;
	CX2ComboAndSkillChainInfoPtr	GetSkillChainInfoPtrByStateID( const int iStateID_ ) const;

	void	ResetStateIdMatchedToChain()
	{
		BOOST_FOREACH( CX2ComboAndSkillChainInfoPtr ptrComboAndSkillChainInfo, m_vecComboAndSkillChainInfoPtrInSystem )
		{
			if ( NULL != ptrComboAndSkillChainInfo )
			{
				ptrComboAndSkillChainInfo->SetStateIdMatchedToChain( 0 );
				ptrComboAndSkillChainInfo->SetHyperStateIdMatchedToChain( 0 );
			}
		}
	}

	void	ResetChaining()
	{
		BOOST_FOREACH( CX2ComboAndSkillChainInfoPtr ptrComboAndSkillChainInfo, m_vecComboAndSkillChainInfoPtrInSystem )
		{
			if ( NULL != ptrComboAndSkillChainInfo )
				ptrComboAndSkillChainInfo->SetChaining( false );
		}
	}

	void	ResetOtherListChaining( CX2ComboAndSkillChainInfoPtr ptrNowChainInfo_ )
	{
		BOOST_FOREACH( CX2ComboAndSkillChainInfoListPtr ptrComboAndSkillChainInfoList, m_vecComboAndSkillChainListPtr )
		{
			if ( NULL != ptrComboAndSkillChainInfoList 
				&& false == ptrComboAndSkillChainInfoList->HasThisChainInfo( ptrNowChainInfo_ ) )
				ptrComboAndSkillChainInfoList->ResetChaining();
		}
	}

	CX2ComboAndSkillChainInfoPtr GetPtrNowUseChainInfo() const { return m_ptrNowUseChainInfo; }
	void SetPtrNowUseChainInfo(CX2ComboAndSkillChainInfoPtr val) { m_ptrNowUseChainInfo = val; }
	void ResetPtrNowUseChainInfo() { m_ptrNowUseChainInfo.reset(); }

	int GetTriggerSecretSkillState( const CX2GUUser* pCX2GUUser_ );

#ifdef  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
    void    AddRef()    {   ++m_uRefCount; }
    void    Release()   { if ( (--m_uRefCount) == 0 )   delete this; }
#endif  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR

private:

#ifdef  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
    unsigned                                        m_uRefCount;
#endif  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR

	VecComboAndSkillChainListPtr								m_vecComboAndSkillChainListPtr;
	vector<CX2ComboAndSkillChainInfoPtr>						m_vecComboAndSkillChainInfoPtrInSystem;
	CX2ComboAndSkillChainInfoPtr								m_ptrNowUseChainInfo;
};

IMPLEMENT_INTRUSIVE_PTR( CX2ComboAndSkillChainSystem );