#pragma once

//{{ robobeg : 2008-10-13

class CKTDGObject;
class CKTDGManager;

typedef std::list<CKTDGObject*>             CKTDGObjectList;
typedef CKTDGObjectList::iterator           CKTDGObjectHandle;

//}} robobeg : 2008-10-13


//
//class CThreadSafeNewDelete
//{
//private:
//	boost::shared_ptr<CThreadSafeNewDelete>   this_;
//
//protected:
//	explicit CThreadSafeNewDelete() : this_(this)
//	{
//		//std::cout << "KTDGObject()" << std::endl;
//	}
//
//public:
//	virtual ~CThreadSafeNewDelete()
//	{
//		//std::cout << "~KTDGObject()" << std::endl;
//	}
//
//	boost::weak_ptr<CThreadSafeNewDelete> get_weak_ptr() const
//	{
//		return this_;
//	}
//
//	void reset() { this_.reset(); }
//
//	//virtual void Render()
//	//{
//	//	std::cout << "KTDGObject::Render()" << std::endl;
//	//}
//
////public:
////	static CThreadSafeNewDelete* CreateObject()
////	{
////		return new KTDGObject();
////	}
////
////	static bool DestroyObject( CThreadSafeNewDelete* pKTDGObject_ )
////	{
////		if( pKTDGObject_ == NULL )
////			return false;
////
////		std::cout << "pre reset" << std::endl;
////		pKTDGObject_->reset();
////		std::cout << "post reset" << std::endl;
////		return true;
////	}
//};



/** @class : CKTDGObject
	@brief : ..
	@date  : 2010/11/10
*/
//{{ seojt // 2009-1-14, 21:25
class CKTDGObject
#ifdef  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
    : private boost::noncopyable
#else   X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
    : public boost::enable_shared_from_this<CKTDGObject>
#endif  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
//}} seojt // 2009-1-14, 21:25
{
    //{{ seojt // 2009-1-9, 10:39
    //template<class T> friend void boost::checked_delete(T * x);
    //}} seojt // 2009-1-9, 10:39

	public: 
		enum OBJECT_TYPE 
		{
			OT_UNKNOWN,
			OT_CX2Eqip,
			OT_CKTDGXSkinAnim,
			OT_CX2GUUser,
			OT_CX2GUNPC,
			OT_EFFECT_MADE_BY_GAME_UNIT,
		};


	protected:
        // 생성과 파괴를 명시적으로 하지 않고, 팩토리를 거쳐서 안전하게 하는 것을 보장하기
        // 위해 생성자와 파괴자는 protected로 접근권한을 가져야 한다.
        // - jintaeks on 2009-01-09, 10:46
//{{ robobeg : 2008-10-17
        // 몸체에 부착된 장비 등과 같이 manager object가 있는 경우, NULL 이어도 된다.
		CKTDGObject( CKTDGObject* pManagerObject_ = NULL );
//}} robobeg : 2008-10-17
		virtual ~CKTDGObject(void);

#ifndef X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
        //{{ seojt // 2009-1-14, 21:24
        class KTDGObjectDeleter;
        friend class KTDGObjectDeleter;

		/** @class : KTDGObjectDeleter
			@brief : CKTDGObject를 상속하고 있는 객체는 팩토리함수를 거쳐서 생성되어야 하며\n
					shared_ptr로 생성되어 리턴된다. 이때 shared_ptr에 deallocator를 지정할 수 있는데\n
					KTDGObjectDeleter가 deallocator에 해당함\n
			@date  : 2010/11/10
		*/
        class KTDGObjectDeleter
        {
        public:
            void operator()( CKTDGObject* pObject_ );
        };//class KTDGObjectDeleter
        //}} seojt // 2009-1-14, 21:24
#endif  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR

    public:
        //{{ seojt // 2009-1-14, 22:00
        static void DeleteKTDGObject( CKTDGObject* pObject_ );
        //}} seojt // 2009-1-14, 22:00

		virtual HRESULT OnFrameMove( double fTime, float fElapsedTime ){ return S_OK; }
//{{ robobeg : 2008-10-28
		//virtual HRESULT OnFrameRender(){ return S_OK; }
        virtual RENDER_HINT OnFrameRender_Prepare() { return RENDER_HINT_DEFAULT; }
        virtual void            OnFrameRender_Draw() {}
		virtual CKTDGXRenderer::RenderParam* GetRenderParam() { return NULL; }
        void    OnFrameRender() 
        { 
            if ( GetShowObject() && OnFrameRender_Prepare().m_eHint != RENDER_HINT::NORENDER )
            {
                SetInstantShow( true );
                OnFrameRender_Draw();
            }
            else
            {
                SetInstantShow( false );
            }//if.. else..
        }//OnFrameRender()
//}} robobeg : 2008-10-28

        virtual HRESULT OnResetDevice(){ return S_OK; }
        virtual HRESULT OnLostDevice(){ return S_OK; }

// #ifdef BACKGROUND_LOADING_TEST // 2008-10-20
//         /// 리소스 로딩 쓰레드가 객체가 필요로 하는 리소스를 로드할 때 호출하는
//         /// 함수이다. - jintaeks on 2008-10-20, 10:03
//         virtual bool    OnThreadLoad(DWORD,DWORD) { return true;}
// #endif // BACKGROUND_LOADING_TEST // 2008-10-20


        CKTDGMatrix&	GetMatrix()
		{
			return ( m_pManagerObject != NULL ) ? m_pManagerObject->GetMatrix() : *m_pMatrix;
		}
        void            SetManagerObject( CKTDGObject* pManagerObject_ );
        CKTDGObject*    GetManagerObject() { return m_pManagerObject; }

		void			SetAlphaObject( bool bAlpha ){ m_bAlphaObject = bAlpha; }
		bool			IsAlphaObject() const { return m_bAlphaObject; }

        void            SetDistanceToCamera( float distanceToCamera ){ m_fDistanceToCamera = distanceToCamera; }
        float           GetDistanceToCamera() const { return m_fDistanceToCamera; }

        void SetBoundingSphere( float radius, D3DXVECTOR3 center )
        {
            SetBoundingRadius(radius);
            SetCenter( center );
        }

//{{ robobeg : 2008-10-17
		void	SetBoundingRadius( float radius ){ m_Sphere.fRadius = radius; }
        void	SetCenter( D3DXVECTOR3 center ){ m_Sphere.center = center; }
		
        float	GetBoundingRadius() const { return ( m_pManagerObject != NULL ) ? m_pManagerObject->GetBoundingRadius() : m_Sphere.fRadius; }	
        const D3DXVECTOR3& GetCenter() const { return ( m_pManagerObject != NULL ) ? m_pManagerObject->GetCenter() : m_Sphere.center; }
//}} robobeg : 2008-10-17
#ifdef  X2OPTIMIZE_CULLING_WORLDOBJECTMESH_SUBSET
        __forceinline float   GetMaxScale() const
        {
            float   fScale = 0.f;
		    D3DXVECTOR3 vScale = const_cast<CKTDGObject*>(this)->GetMatrix().GetScale();
			if( vScale.x > vScale.y )
            {
				if( vScale.x > vScale.z ) 					//X가 제일 큼
					fScale = vScale.x;
				else                                        //Z가 제일 큼
					fScale = vScale.z;
			}
			else
			{
				if( vScale.y > vScale.z )               //Y가 제일 큼
					fScale = vScale.y;
				else                                    //Z가 제일 큼
					fScale = vScale.z;
			}
            return  fScale;
        }
        __forceinline float   GetScaledBoundingRadius() const
        {
            float   fBoundingRadius = GetBoundingRadius();
            if ( fBoundingRadius <= 0.f )
                return 0.f;
            return  fBoundingRadius * GetMaxScale();
        }
#endif  X2OPTIMIZE_CULLING_WORLDOBJECTMESH_SUBSET

        __forceinline void GetTransformCenter( D3DXVECTOR3* pCenter )
        {
//{{ robobeg : 2008-10-17
		    D3DXVECTOR4 out;
		    D3DXVec3Transform( &out, &m_Sphere.center, &GetMatrix().GetMatrix() );
//}} robobeg : 2008-10-17
            pCenter->x = out.x;
            pCenter->y = out.y;
            pCenter->z = out.z;
        }

        void            SetForceLayer( bool bForceLayer ){ m_bForceLayer = bForceLayer; }
//{{ robobeg : 2008-10-17
        bool			GetForceLayer() const { return ( m_pManagerObject != NULL ) ? m_pManagerObject->GetForceLayer() : m_bForceLayer; }
//}} robobeg : 2008-10-17

        void            SetLayer( int layer ){ m_Layer = layer; }
//{{ robobeg : 2008-10-17
        int             GetLayer() const { return ( m_pManagerObject != NULL ) ? m_pManagerObject->GetLayer() : m_Layer; }
//}} robobeg : 2008-10-17

//{{ robobeg : 2008-10-13
        //void          SetRenderStateID( int renderStateID ){ m_RenderStateID = renderStateID; }
        //int               GetRenderStateID(){ return m_RenderStateID; }



#ifdef GIANT_UNIT_GIANT_EFFECT_TEST

		void SetScaleByUnit(const D3DXVECTOR3& val) 
		{ 
			ASSERT( 0.f != val.x ); 
			ASSERT( 0.f != val.y ); 
			ASSERT( 0.f != val.z ); 

			m_vScaleByUnit = val; 
		}

#ifdef VERIFY_STAT_BY_BUFF
		bool VerifyScaleByUnit() { return m_vScaleByUnit.Verify(); }

		const PROTECT_VECTOR3& GetScaleByUnit() const 
		{ 
			ASSERT( 0.f != m_vScaleByUnit.x ); 
			ASSERT( 0.f != m_vScaleByUnit.y ); 
			ASSERT( 0.f != m_vScaleByUnit.z ); 

			return m_vScaleByUnit; 
		}

		D3DXVECTOR3 GetVec3ScaleByUnit() const 
		{ 
			ASSERT( 0.f != m_vScaleByUnit.x ); 
			ASSERT( 0.f != m_vScaleByUnit.y ); 
			ASSERT( 0.f != m_vScaleByUnit.z ); 

			return m_vScaleByUnit.GetVector3(); 
		}
		
		void SetScaleByUnit(const PROTECT_VECTOR3& val) 
		{ 
			ASSERT( 0.f != val.x ); 
			ASSERT( 0.f != val.y ); 
			ASSERT( 0.f != val.z ); 

			m_vScaleByUnit = val; 
		}
#else	// VERIFY_STAT_BY_BUFF
		D3DXVECTOR3 GetScaleByUnit() const 
		{ 
			ASSERT( 0.f != m_vScaleByUnit.x ); 
			ASSERT( 0.f != m_vScaleByUnit.y ); 
			ASSERT( 0.f != m_vScaleByUnit.z ); 

			return m_vScaleByUnit; 
		}
#endif // VERIFY_STAT_BY_BUFF

#endif GIANT_UNIT_GIANT_EFFECT_TEST


//{{ robobeg : 2008-10-17
        void    SetRenderStateID( const CKTDGStateManager::KStateID& renderStateID ){ m_RenderStateID = renderStateID; }
        const CKTDGStateManager::KStateID& GetRenderStateID() const
        {
            return ( m_pManagerObject != NULL ) ? m_pManagerObject->GetRenderStateID() : m_RenderStateID;
        }
//}} robobeg : 2008-10-17


//}} robobeg : 2008-10-13

        void            SetShowObject( bool bShow, bool bForce = false )
        {
			if( true == bForce )
			{
				m_bShowObject = bShow;
				NotifyShowObjectChanged();
			}
			else
			{
				bool bOldShow = GetShowObject();
				m_bShowObject = bShow;
				bool bNewShow = GetShowObject();
				if ( bOldShow != bNewShow )
					NotifyShowObjectChanged();
			}
        }//SetShowObject()

        virtual void    NotifyShowObjectChanged()   {}

        bool            GetShowObject() const { return ( m_pManagerObject != NULL ) ? m_pManagerObject->GetShowObject() && m_bShowObject : m_bShowObject; }
		bool			GetShowObjectPrivate() const { return m_bShowObject; } // dmlee 2009.3.22 Equip 백그라운드로딩에서 참조하기위해 사용함, 다른곳에는 사용하면 안됨. GetShowObject() 함수를 사용하세요
		

//{{ robobeg : 2009-01-10
        bool            IsInstantShow() const { return m_bInstantShow; }
        void            SetInstantShow( bool bInShow_ )   { m_bInstantShow = bInShow_; }
//}} robobeg : 2009-01-10

        void            SetOverUI( bool bOverUI ){ m_bOverUI = bOverUI; }
//{{ robobeg : 2008-10-17
        bool            GetOverUI(){ return ( m_pManagerObject != NULL ) ? m_pManagerObject->GetOverUI() : m_bOverUI; }
//}} robobeg : 2008-10-17
        void            SetInnerUI( bool bInnerUI ){ m_bInnerUI = bInnerUI; }
//{{ robobeg : 2008-10-17
        bool            GetInnerUI(){ return ( m_pManagerObject != NULL ) ? m_pManagerObject->GetInnerUI() : m_bInnerUI; }
//}} robobeg : 2008-10-17

//{{ robobeg :: 2008-10-17
		void			SetScale( const D3DXVECTOR3& vScale )
		{
            if ( m_pManagerObject == NULL )
            {
                ASSERT( m_pMatrix != NULL );
#ifdef BALANCE_PATCH_20120329
				if ( NULL == m_pMatrix )
					return;
#endif
			    m_pMatrix->Scale( vScale );
            }//if
		}
//}} robobeg :: 2008-10-17

		void			SetIsCulled( bool bCulled ) { m_bCulled = bCulled; }  
        bool            IsCulled() const { return ( m_pManagerObject != NULL ) ? m_pManagerObject->IsCulled() : m_bCulled; }
        //{{ seojt: 2008.10.23
        //double          GetLastAccessTime() const { return m_dwLastAccessTime; }
        //void            SetLastAccessTime(double dwTime_) { m_dwLastAccessTime = dwTime_; }
        //}} seojt

//{{ robobeg : 2008-10-17
#ifdef CONVERSION_VS
		bool IsManagedByDGManager() const;
#else CONVERSION_VS
        bool            IsManagedByDGManager() const { return m_hKTDGManagerHandle != NULL; }
#endif CONVERSION_VS
        
#ifdef CONVERSION_VS
		bool            IsRecursivelyManagedByDGManager() const;
#else CONVERSION_VS
		bool            IsRecursivelyManagedByDGManager() const
		{
            return ( m_hKTDGManagerHandle != NULL ) ? true :
            ( m_pManagerObject != NULL ? m_pManagerObject->IsRecursivelyManagedByDGManager() : false );
		}
#endif CONVERSION_VS
        
        virtual void    NotifyDGManagerChanged( CKTDGManager& refInManager_ ) {}

		CKTDGObject::OBJECT_TYPE GetObjectType() const { return m_eObjectType; }		
		void SetObjectType( const OBJECT_TYPE eObjectType_ ) { m_eObjectType = eObjectType_; }

#ifndef X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR        
        boost::weak_ptr<CKTDGObject> get_weak_ptr()
        {
            return shared_from_this();
        }//get_weak_ptr()

        //{{ seojt // 2009-1-12, 14:57
        template<class TARGET_TYPE>
        boost::shared_ptr<TARGET_TYPE> get_shared_ptr()
        {
            return boost::static_pointer_cast<TARGET_TYPE>( shared_from_this() );
        }//get_shared_ptr()
        //}} seojt // 2009-1-12, 14:57
#endif  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR

    public:
        class   CKTDGManagerAccess
        {
            friend class    CKTDGManager;
            static CKTDGObjectHandle   _GetDGManagerHandle( const CKTDGObject& kInObject_ ) { return kInObject_.m_hKTDGManagerHandle; }
            static void                _SetDGManagerHandle( CKTDGObject& kInObject_, CKTDGObjectHandle hInHandle_ ) 
                { kInObject_.m_hKTDGManagerHandle = hInHandle_; }
        };//class   CKTDGManagerAccess


#ifdef  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
        void            AddRef()    { ++m_uRefCounter; }
        void            Release()   { if ( (--m_uRefCounter) == 0 ) DeleteKTDGObject( this ); }
#endif  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR

	protected:
		OBJECT_TYPE		m_eObjectType;		// ktdgobject를 상속받은 클래스를 구분하기 위해서


    private:

#ifdef  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
        unsigned        m_uRefCounter;
#endif  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR

//{{ robobeg : 2008-10-17
        CKTDGObject*    m_pManagerObject;
        CKTDGMatrix*    m_pMatrix;
//}} robobeg : 2008-10-17

        bool            m_bAlphaObject;
        float           m_fDistanceToCamera;
        KTDGSphere      m_Sphere;
        bool            m_bForceLayer;
        int             m_Layer;
//{{ robobeg : 2008-10-13
        CKTDGStateManager::KStateID     m_RenderStateID;
//}} robobeg : 2008-10-13
        bool            m_bOverUI;
        bool            m_bInnerUI;

        bool            m_bShowObject;
//{{ robobeg : 2009-01-10
        bool            m_bInstantShow;
//}} robobeg : 2009-01-10

        bool            m_bCulled;				// 디버깅 용도로 화면 출력 때에만 사용함 
        //{{ seojt: 2008.10.23
        //double          m_dwLastAccessTime;
        //}} seojt

//{{ robobeg : 2008-10-13

        CKTDGObjectHandle   m_hKTDGManagerHandle;
//}} robobeg : 2008-10-13



#ifdef VERIFY_STAT_BY_BUFF
		PROTECT_VECTOR3 m_vScaleByUnit;		// 게임중에 캐릭터가 아이템을 먹고, 거인 혹은 소인이 되었을 때 이펙트의 크기도 줄여주기 위해서 처음 추가
#else	// VERIFY_STAT_BY_BUFF
		D3DXVECTOR3		m_vScaleByUnit;		// 게임중에 캐릭터가 아이템을 먹고, 거인 혹은 소인이 되었을 때 이펙트의 크기도 줄여주기 위해서 처음 추가
#endif // VERIFY_STAT_BY_BUFF

#ifdef X2OPTIMIZE_CULLING_WORLDOBJECTMESH_SUBSET
	public:
		__forceinline virtual bool IsInFrustum( const D3DXVECTOR3& kEye, const CKTDGFrustum& kFrustum )
		{
			if( GetShowObject() == false )
				return false;

			D3DXVECTOR3 center;
			float fScale = 0.f;


			GetTransformCenter( &center );
			SetDistanceToCamera( GetDistance( kEye, center ) );

			if ( GetBoundingRadius() <= 0 )
				return true;

			if( kFrustum.CheckSphere( center, GetScaledBoundingRadius() ) == false )
			{
				//...... -_-a I want to use this code but............ T^T
				SetIsCulled( true );
				return false;
			}
			else
			{
				SetIsCulled( false );
			}
			return true;
		}
#endif//X2OPTIMIZE_CULLING_WORLDOBJECTMESH_SUBSET

};

IMPLEMENT_INTRUSIVE_PTR( CKTDGObject );