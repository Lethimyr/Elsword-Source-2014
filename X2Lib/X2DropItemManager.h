#pragma once

class CX2DropItemManager : public CKTDXStage
{
	public:
		struct DropItemData
		{
			KProtectedType<int>		m_ItemID;
			KProtectedType<UidType>	m_ItemUID;
			D3DXVECTOR3				m_Pos;
			KProtectedType<bool>	m_bGet;
			KProtectedType<bool>	m_bGetTry;
			bool					m_bLeft;
			float					m_fNowSpeed;
			float					m_fScale;
			float					m_fCoolTime;
			float					m_fRemainTime;
			
			const CX2Item::ItemTemplet*	m_pItemTemplet;
			CKTDGParticleSystem::CParticleEventSequenceHandle 	m_hSeq;
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
            CKTDGParticleSystem::CParticleHandle				m_hParticle;
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
			CKTDGParticleSystem::CParticle*					m_pParticle;
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE

#ifdef SERV_ENCHANT_ITEM_DROP_EVENT ///드랍되는 아이템에 강화 수치 추가
			char					m_cEnchantLevel;
#endif SERV_ENCHANT_ITEM_DROP_EVENT

			DropItemData()
			{
				m_ItemID		= -1;
				m_ItemUID		= -1;
				m_Pos			= D3DXVECTOR3(0.0f,0.0f,0.0f);
				m_bGet			= false;
				m_bGetTry		= false;
				m_bLeft			= true;
				m_fNowSpeed		= 0.0f;
				m_fScale		= 1.0f;
				m_fCoolTime		= 1.0f;
				m_fRemainTime	= 20.0f;

				m_pItemTemplet	= NULL;
				m_hSeq			= INVALID_PARTICLE_SEQUENCE_HANDLE;		
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
                m_hParticle		= INVALID_PARTICLE_HANDLE;
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
				m_pParticle		= NULL;
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE

#ifdef SERV_ENCHANT_ITEM_DROP_EVENT ///드랍되는 아이템에 강화 수치 추가
				m_cEnchantLevel	= 0;
#endif SERV_ENCHANT_ITEM_DROP_EVENT
			}
			~DropItemData();

			void Verify();
		};

	public:
		CX2DropItemManager(void);
		virtual ~CX2DropItemManager(void);


		virtual HRESULT OnFrameMove( double fTime, float fElapsedTime );
		void			RenderName();

		void			AddDropItem_LUA( int iItemID, D3DXVECTOR3 pos, float fSpeed, float fRemainTime );

#ifdef SERV_ENCHANT_ITEM_DROP_EVENT		/// 드랍 아이템에 강화 레벨 추가
		void			AddDropItem( int iItemID, UidType ItemUID, const D3DXVECTOR3 &pos, bool bLeft, float fSpeed, float fRemainTime = 20.f, char cEnchantLevel = 0 );
#else
		void			AddDropItem( int iItemID, UidType ItemUID, D3DXVECTOR3 pos, bool bLeft, float fSpeed, float fRemainTime = 20.f );
#endif SERV_ENCHANT_ITEM_DROP_EVENT
		
		UidType			TryGetDropItem( D3DXVECTOR3 pos );
		bool			GetDropItem( IN const UidType itemUID_, IN const UidType unitUID_, 
									OUT int& iItemID_, OUT const CX2Item::ItemTemplet*& pItemTemplet_ );
		int				GetDropItemID( UidType itemUID );

		void			DeleteAllItem();
		int				CountDropItemByID( int iItemID );

		bool			GetDropItemPosition( int iIndex, D3DXVECTOR3& vPos );
#ifdef  X2OPTIMIZE_HANDLE_VALIDITY_CHECK
        const D3DXVECTOR3*  GetDropItemPosition_LUA( int iIndex );
#endif  X2OPTIMIZE_HANDLE_VALIDITY_CHECK

#ifdef DUNGEON_ITEM		
		int				GetDropItemCount() { return m_DropItemDataList.size(); }
		bool			GetDropItemPositionByUid( int itemUID, D3DXVECTOR3& vPos );
		bool			ChangeDropItemPositionByUid(int itemUID, D3DXVECTOR3 vPos );
		void			AddDropItem( int iItemID, UidType ItemUID, D3DXVECTOR3 pos, float fCoolTime = 0.f, float fRemainTime = 20.f );
#endif

#ifdef PET_DROP_ITEM_PICKUP
		void			SetSummonItemPickupPet(bool bVal){ m_bIsSummonItemPickupPet = bVal; } 
		bool			GetIsSummonItemPickupPet() const { return m_bIsSummonItemPickupPet ; }
#endif //PET_DROP_ITEM_PICKUP
	private:
		float						m_fElapsedTime;
		vector<DropItemData*>		m_DropItemDataList;

		CKTDXDeviceSound*			m_apSoundGetItem[6];

#ifdef FIX_TRYGETDROPITEM
		float						m_fGetDist;
		static const float			ms_MaximumDistanceSquareToPickUp;
#endif
		
#ifdef PET_DROP_ITEM_PICKUP
		bool						m_bIsSummonItemPickupPet; // 아이템 먹는 펫 소환 여부
#endif //PET_DROP_ITEM_PICKUP
};
