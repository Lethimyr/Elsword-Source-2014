/** @file   X2Eqip.h
    @date   jintaeks on 2008-10-15, 15:21
            - managed된 리소스를 local memory에 preload하는 기능을 추가하다.

    @date   robobeg on 2008-10-17
            - 렌더링 최적화를 위해 CX2Eqip 을 CKTDGObject 로부터 유도하고, CKTDGManager 가 관리할 수 있도록 수정
*/
#pragma once


//{{ seojt // 2009-1-13, 15:47
class CX2SubEquip;
class CX2Eqip;
#ifdef  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
typedef boost::intrusive_ptr<CX2Eqip>  CX2EqipPtr;
#else   X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
typedef boost::shared_ptr<CX2Eqip>  CX2EqipPtr;
#endif  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
//}} seojt // 2009-1-13, 15:47


//어태치 애니메이션 장비는 3개의 애니메이션을 가진다
//1. Nomal
//2. Action
//3. Die
//무기라면 각성 애니메이션을 가진다
//4. Change

//{{ robobeg : 2008-10-17
class CX2Eqip : public CKTDGObject
//}} robobeg : 2008-10-17
{
	private:
		//CX2Eqip() { ASSERT( !"invalid" ); }
		// @eUnitType: 공용템인 경우에 unit_type에 따라 다른 메시를 로딩한다.
		CX2Eqip( CX2Item* pItem, CKTDGXSkinAnimPtr pUnitXSkinAnim, int enchantLevel, bool bDummyBasic = false, bool bLoad = true, 
			CX2Unit::UNIT_TYPE eUnitType = CX2Unit::UT_NONE );

		virtual ~CX2Eqip(void);

		void	AppendAttachedToDeviceList( CKTDXDeviceDataList& listInOut_ ) const;

	public: 
		//{{ seojt // 2009-1-14, 21:29
		static CX2EqipPtr CreateEquip( CX2Item* pItem, CKTDGXSkinAnimPtr pUnitXSkinAnim
            , int enchantLevel, bool bDummyBasic = false, bool bLoad = true
            , CX2Unit::UNIT_TYPE eUnitType = CX2Unit::UT_NONE )
		{
			CX2EqipPtr  ptrEquip( new CX2Eqip( pItem, pUnitXSkinAnim, enchantLevel, bDummyBasic, bLoad, eUnitType )
#ifndef  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
                                    , CKTDGObject::KTDGObjectDeleter()
#endif  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
                                    );
            
			// 리소스 로딩을 CX2Eqip 생성자에서 하지않고 생성자 호출 이후에 하는 이유는 
			// shared_ptr가 초기화된 이후에 백그라운드 로딩을 요청 해야하기 때문에
			if( ptrEquip != NULL &&
				true == ptrEquip->GetResourceLoadReserved() )
			{
				ptrEquip->EquipResourceLoad();
			}
			
			return ptrEquip;
		}//CreateEquip
		//}} seojt // 2009-1-14, 21:29

		 void    AppendToDeviceList( CKTDXDeviceDataList& listInOut_ ) const;

		void EquipResourceLoad();

		void LoadAttached();
		bool GetResourceLoadReserved() const { return m_bResourceLoadReserved; }




        /// managed된 리소스를 local memory에 preload한다.
        /// - jintaeks on 2008-10-15, 15:21
        void                    PreLoad();

//{{ robobeg : 2008-10-17
		//void					SetShow( bool bShow ) { m_bShow = bShow; }
        virtual void            NotifyShowObjectChanged();
//}} robobeg : 2008-10-17

//{{ robobeg : 2008-10-17
        // OnFrameMove 에서 RenderParam 세팅을 분리하여 OnFrameMove 이전에 실행하도록 수정
        HRESULT                 SetRenderParam( double fTime, float fElapsedTime, CKTDGXRenderer::RenderParam* pRenderParam = NULL );
		void SetRenderParamSpecial( CKTDGXRenderer::RenderParam* pEqipRenderParam );

        virtual HRESULT			OnFrameMove( double fTime, float fElapsedTime );

        virtual RENDER_HINT		OnFrameRender_Prepare();
		virtual void            OnFrameRender_Draw();
//}} robobeg : 2008-10-17

        //{{ seojt // 2008-12-13, 19:01
		//background!!!
        //virtual bool            OnThreadLoad(DWORD dwParam_, DWORD dwParam2_);
        //}} seojt // 2008-12-13, 19:01

		void					ChangeHyperMode();
		void					ChangeNormalMode();

		bool					CanHyperMode()	const		{ return m_bCanHyperMode; }

//{{ robobeg : 2008-10-17
		//virtual HRESULT			OnResetDevice()			{ return S_OK; }
		//virtual HRESULT			OnLostDevice()			{ return S_OK; }
//}} robobeg : 2008-10-17

		//CKTDGXSkinAnim*			GetUnitXSkinAnim()		{ return m_pUnitXSkinAnim; }
		CX2Item*				GetItem();
		CX2Item::USE_TYPE		GetEqipType() const			{ return m_EqipType; }
		
		//{{ dmlee 2008.11.29 mesh index를 인자로 입력받도록 수정했습니다.
		CKTDGXSkinAnim*			GetNormalAnim( int index )			{ return m_pNormalXSkinAnim[index]; }
		CKTDGXSkinAnim*			GetHyperAnim( int index )			{ return m_pHyperXSkinAnim[index]; }
		CKTDXDeviceXSkinMesh*	GetNormalModel( int index )			{ return m_pNormalXSkinModel[index]; }
		CKTDXDeviceXSkinMesh*	GetHyperModel( int index )			{ return m_pHyperXSkinModel[index]; }

		const WCHAR*			GetModelName( int index ) const			{ return m_ModelName[index].c_str(); }
		const WCHAR*			GetModelNameHyper( int index ) const	{ return m_ModelNameHyper[index].c_str(); }
		//}} dmlee 2008.11.29 mesh index를 인자로 입력받도록 수정했습니다.


		CKTDXDeviceXET*			GetNormalTexChangeXET() const { return m_pNormalTexChangeXET; }

		const WCHAR*			GetChangeTexName()	const	{ return m_ChangeTexName.c_str(); }
		const WCHAR*			GetChangeTexNameHyper()	const { return m_ChangeTexNameHyper.c_str(); }
		const WCHAR*			GetAniXETName()	const		{ return m_AniXETName.c_str(); }
		const WCHAR*			GetAniXETNameHyper() const	{ return m_AniXETNameHyper.c_str(); }

		int						GetItemID()	const			{ return m_ItemID; }
		UidType					GetItemUID() const			{ return m_ItemUID; }
		CX2Unit::EQIP_POSITION	GetEqipPos() const			{ return m_EqipPos; }

		int						GetEnchantLevel() const { return m_EnchantLevel; }
		void					ChangeEnchantLevel( int level );

		void					SetOwnerUnit( CX2Unit* pUnit );
		CX2Unit*				GetOwnerUnit() const { return m_pOwnerUnit; }

#ifdef NEW_VILLAGE_UI
		void					SetFixed();
#endif

#ifdef ADD_UPGRADE_WEAPON_PARTICLE		/// 강화 아이템 UV 애니메이션 속도
		float					GetUpgradeWeaponUVAnimatonSpeed();
#endif ADD_UPGRADE_WEAPON_PARTICLE

//{{ robobeg : 2008-10-17

		bool                    IsDummyBasic() const { return m_bDummyBasic; }
		int                     GetEnchantType() const
		{
			// fix!! 여기 나중에 script로 빼던지, switch 문으로 바꾸던지 뭔가 수를 내야 할 듯.
			if ( m_pItemTemplet != NULL )
			{
                DWORD   dwItemID = m_pItemTemplet->GetItemID();
				for( int i=0; i<ARRAY_SIZE( ITEM_ID_ENCHANT_TYPE3_GOLD ); i++ )
				{
					if( ITEM_ID_ENCHANT_TYPE3_GOLD[i] == dwItemID )
						return 3;
				}

				for( int i=0; i<ARRAY_SIZE( ITEM_ID_ENCHANT_TYPE3_BLUE ); i++ )
				{
					if( ITEM_ID_ENCHANT_TYPE3_BLUE[i] == dwItemID )
						return 3;
				}

				for( int i=0; i<ARRAY_SIZE( ITEM_ID_ENCHANT_TYPE3_GREEN ); i++ )
				{
					if( ITEM_ID_ENCHANT_TYPE3_GREEN[i] == dwItemID )
						return 3;
				}		

				for( int i=0; i<ARRAY_SIZE( ITEM_ID_ENCHANT_TYPE3_YELLOW ); i++ )
				{
					if( ITEM_ID_ENCHANT_TYPE3_YELLOW[i] == dwItemID )
						return 3;
				}

				for( int i=0; i<ARRAY_SIZE( ITEM_ID_ENCHANT_TYPE3_RED ); i++ )
				{
					if( ITEM_ID_ENCHANT_TYPE3_RED[i] == dwItemID )
						return 3;
				}

				for( int i=0; i<ARRAY_SIZE( ITEM_ID_ENCHANT_TYPE3_PINK ); i++ )
				{
					if( ITEM_ID_ENCHANT_TYPE3_PINK[i] == dwItemID )
						return 3;
				}



				for( int i=0; i<ARRAY_SIZE( ITEM_ID_ENCHANT_TYPE2 ); i++ )
				{
					if( ITEM_ID_ENCHANT_TYPE2[i] == dwItemID )
						return 2;
				}

				if ( m_EnchantLevel >= ENCHANT_WEAPON_EFFECT_LEVEL_0 )
					return 1;

				for( int i=0; i<ARRAY_SIZE( ITEM_ID_ENCHANT_TYPE1 ); i++ )
				{
					if( ITEM_ID_ENCHANT_TYPE1[i] == dwItemID )
						return 1;
				}
			}

			return 0;
        }//GetEnchantType()

		float GetJiggleBone()
		{
			if ( m_pItemTemplet != NULL )
			{
                DWORD   dwItemID = m_pItemTemplet->GetItemID();
				// 망토
				for(int i=0; i<ARRAY_SIZE( ITEM_ID_JIGGLE_TYPE1 ); ++i)
				{
					if( ITEM_ID_JIGGLE_TYPE1[i] == dwItemID )
						return 1.5f;
				}
			}

			return -1.f;
		}

//}} robobeg : 2008-10-17


#ifdef EQUIP_ATTACHED_PARTICLE_TEST

		void SetShowAttachedParticle( bool bShow );
		void AttachParticle( bool bIsMajorParticle, const wstring& seqName, const wstring& boneName, D3DXVECTOR3 vOffsetPos );



		// fix!! not yet!! 구현이 완료된 상태가 아님
		struct AttachedParticle
		{
			wstring			m_wstrBoneName;
			bool			m_bIsMajorParticle;
			D3DXVECTOR3 	m_vOffsetPos;
			CKTDGParticleSystem::CParticleEventSequenceHandle  m_hSeq;

			CKTDXDeviceXSkinMesh::MultiAnimFrame* m_pAttachFrame;

			AttachedParticle();
			~AttachedParticle();
		};

		struct AttachedParticleGroup
		{
			vector<AttachedParticle*>	m_vecpAttachedParticle;

			AttachedParticleGroup();
			~AttachedParticleGroup();

			void OnFrameMove( double fTime, float fElapsedTime );
			void SetShowObject( bool bShow );
		};
#endif EQUIP_ATTACHED_PARTICLE_TEST

		void					ChangeAnim( const WCHAR* pwszAnimName_, const bool bTransition_ );

private:

		void					InitSkin();
		void					InitAttatchAnim();
		void					InitAttatchNormal();

//{{ robobeg : 2008-10-17
        void                    _UpdateSkin(int iIndex_);
        void                    _UpdateSkin();
//}} robobeg : 2008-10-17

//{{ robobeg : 2008-10-17
		//bool						m_bShow;
//}} robobeg : 2008-10-17

		int							m_EnchantLevel;
		bool						m_bDummyBasic;
		CX2Item*					m_pDummyItem;
		const CX2Item::ItemTemplet*		m_pItemTemplet;
		int							m_ItemID;
		UidType						m_ItemUID;
		CX2Unit::EQIP_POSITION		m_EqipPos;
		
		CX2Unit*					m_pOwnerUnit;
		UidType						m_OwnerUnitUID;		// 백그라운드 로딩중에 unit을 참조하기 위해서 m_pOwnerUnit과 별도록 uid를 저장해두도록 했습니다.


		//{{ seojt // 2009-1-9, 13:52
		//CKTDGXSkinAnim*				m_pUnitXSkinAnim;
		// dmlee, 2009-03-12 백그라운드 로딩시에 dangling pointer 참조 오류 생기는 문제 때문에 smart poiner로 바꿨습니다.
        CKTDGXSkinAnimPtr           m_pUnitXSkinAnim;
		//}} seojt // 2009-1-9, 13:52
		bool						m_bHyperMode;
		bool						m_bCanHyperMode;
		CX2Item::USE_TYPE			m_EqipType;

		float						m_fAttatchNormalNowAniTime;
		CKTDGXRenderer::RenderParam m_AttatchNormalRenderParam;
		CKTDGXRenderer::RenderParam m_EnchantRenderParam;


//{{ robobeg : 2008-10-21
// 현재 SkinModel attachment 상태를 알려준다. 0: attach 없음, 1:Normal, 2:Hyper
        int                         m_iXSkinModelAttachment;
//}} robobeg : 2008-10-21




		//{{ dmlee 2008.11.29 이브 무기 2개 메시 다른거 붙일 수 있게 작업
		D3DXMATRIX*					m_pUnitMatrix[ CX2Item::MAX_MODEL_COUNT_A_ITEM ];						// 부모 unit의 attach된 frame의 combine_matrix
		D3DXMATRIX                  m_mAttachNormalMatrix[ CX2Item::MAX_MODEL_COUNT_A_ITEM ];

        // 내부적으로 관리하는 객체이므로 smart pointer를 사용하지 않는다.
        // - jintaeks on 2009-01-12, 16:19
		CKTDGXSkinAnim*				m_pNormalXSkinAnim[ CX2Item::MAX_MODEL_COUNT_A_ITEM ];
        // 내부적으로 관리하는 객체이므로 smart pointer를 사용하지 않는다.
        // - jintaeks on 2009-01-12, 16:19
		CKTDGXSkinAnim*				m_pHyperXSkinAnim[ CX2Item::MAX_MODEL_COUNT_A_ITEM ];

		CKTDXDeviceXSkinMesh*		m_pNormalXSkinModel[ CX2Item::MAX_MODEL_COUNT_A_ITEM ];
		CKTDXDeviceXSkinMesh*		m_pHyperXSkinModel[ CX2Item::MAX_MODEL_COUNT_A_ITEM ];

		CKTDXDeviceXMesh*			m_pNormalXMesh[ CX2Item::MAX_MODEL_COUNT_A_ITEM ];
		CKTDXDeviceXMesh*			m_pHyperXMesh[ CX2Item::MAX_MODEL_COUNT_A_ITEM ];

		wstring						m_ModelName[ CX2Item::MAX_MODEL_COUNT_A_ITEM ];
		wstring						m_ModelNameHyper[ CX2Item::MAX_MODEL_COUNT_A_ITEM ];		
		//}} dmlee 2008.11.29 이브 무기 2개 메시 다른거 붙일 수 있게 작업
		



		CKTDXDeviceXET*				m_pNormalTexChangeXET;		
		CKTDXDeviceXET*				m_pNormalMultiTexXET;
		CKTDXDeviceXET*				m_pNormalAniXET;
		const CKTDXDeviceXET::AniData*	m_pNormalAniData;

		CKTDXDeviceXET*				m_pHyperTexChangeXET;
		CKTDXDeviceXET*				m_pHyperMultiTexXET;
		CKTDXDeviceXET*				m_pHyperAniXET;
		const CKTDXDeviceXET::AniData*	m_pHyperAniData;

		wstring						m_ChangeTexName;
		wstring						m_AniXETName;
		wstring						m_AniName;
		
		wstring						m_ChangeTexNameHyper;
		wstring						m_AniXETNameHyper;		
		wstring						m_AniNameHyper;

		D3DXVECTOR3					m_Offset;
		float						m_fTexOffset;

		vector<CX2SubEquip*>		m_vecpSubEquip;

#ifdef NEW_VILLAGE_UI
		bool						m_bFixed;
#endif

#ifdef EQUIP_ATTACHED_PARTICLE_TEST
		AttachedParticleGroup		m_AttachedParticleGroup;
#endif EQUIP_ATTACHED_PARTICLE_TEST


		bool						m_bResourceLoadReserved;		// 객체 생성후에 리소스 로딩 할지 말지
		bool						m_bLoadedAttached;				// subequip, attached_particle은 eqip mesh가 모두 로딩된 이후에 로딩해야해서 flag를 하나 만듬
};







