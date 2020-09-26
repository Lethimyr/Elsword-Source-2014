#pragma once

#include "KTDGStateManager.h"
#include "KTDGEffectStateManager.h"
#include "KTDGMatrix.h"
#include "KTDGCamera.h"
#include "KTDGFrustum.h"
#include "KTDGXRenderer.h"
#include "KTDGObject.h"
#include "KTDGXSkinAnim.h"
#include "KTDGParticleSystem.h"
#include "KTDGLineMap.h"
#include "KTDGGlow.h"
#include "KTDGSkyDome.h"
#include "KTDGXSkinAfterImage.h"
#include "KTDGSlashTrace.h"
#include "KTDGMotionBlur.h"
#include "KTDGPlanShadow.h"
#include "KTDGLensFlare.h"
#include "KTDGFontVertex.h"
#include "KTDGDynamicTexture.h"
#ifdef USE_FREE_TYPE
#include "KTDGFreeTypeFont.h"
#else //USE_FREE_TYPE
#include "KTDGDeviceFont.h"
#endif //USE_FREE_TYPE
#include "KTDGFontManager.h"
#include "KTDGXMeshPlayer.h"
#include "KTDGPicChar.h"
#include "KTDGTargetArrow.h"

//{{ seojt // 2009-8-19, 11:36
class CKTDGUIDialogHandle;
class CKTDGUIDialog;
#ifdef DIALOG_HANDLE_TEST
    // dialog handle을 사용하는 경우, CKTDGUIDialogType을 dialog handle 타입으로 정의한다.
    typedef CKTDGUIDialogHandle     CKTDGUIDialogType;
#else
    // 기존의 dialog* raw 포인터를 CKTDGUIDialogType으로 정의한다.
    typedef CKTDGUIDialog*          CKTDGUIDialogType;
#endif // DIALOG_HANDLE_TEST
//}} seojt // 2009-8-19, 11:36

#include "KTDGUIDialogHandle.h"
#include "KTDGUIControl.h"
#include "KTDGUIStatic.h"
#include "KTDGUIButton.h"
#include "KTDGUICheckBox.h"
#include "KTDGUIUniBuffer.h"
#include "KTDGUIEditBox.h"
#include "KTDGUIIMEEditBox.h"
#include "KTDGUIItem.h"
#include "KTDGUIScrollBar.h"
#include "KTDGUIComboBox.h"
#include "KTDGUIListBox.h"
#include "KTDGUIListControl.h"
#include "KTDGUIRadioButton.h"
#include "KTDGUISlider.h"
#include "KTDGUINavigation.h"

#ifdef NEW_SKILL_TREE_UI
#include "KTDGUIControlList.h"
#include "KTDGUISlot.h"
#endif

#include "KTDGUIDialog.h"
#include "KTDGUIDialogManager.h"
//#include "KTDGGrayScale.h"


class CKTDGManager : public CKTDXStage
{
    private:

//{{ kimhc // 2010.6.17 // enum X2_LAYER에 레이어 추가하는 작업
		static const int       m_ciNumLayerGroups = 32;	// 레이어 6개 추가
//}} kimhc // 2010.6.17 // enum X2_LAYER에 레이어 추가하는 작업

        inline int      MapLayerGroup( int iLayer )
        {
//{{ kimhc // 2010.6.17 // enum X2_LAYER에 레이어 추가하는 작업
			if ( iLayer <= 26 )
				return iLayer;
			if ( iLayer <= 100 )
				return 27;
			if ( iLayer <= 200 )
				return 28;
			if ( iLayer <= 300 )
				return 29;
			if ( iLayer <= 400 )
				return 30;
			return 31;
//}} kimhc // 2010.6.17 // enum X2_LAYER에 레이어 추가하는 작업
            
        }//MapLayerGroup()

    public:

        struct  KObjectRenderHint
        {
            CKTDGObject*      m_pObject;
            RENDER_HINT   m_kRenderHint;
        };//struct  KObjectAndRenderHint

        typedef std::vector<KObjectRenderHint>   KObjectRenderHintVector;


	public:
		CKTDGManager( HWND hWnd, LPDIRECT3DDEVICE9 pd3dDevice );
		~CKTDGManager(void);

		void	BuildRenderStateID();

        CKTDGStateManager::KStateID GetRSICKTDXDeviceTexture()	{ return m_RSICKTDXDeviceTexture;	}
		CKTDGStateManager::KStateID GetRSICKTDGSlashTrace()		{ return m_RSICKTDGSlashTrace;		}
		CKTDGStateManager::KStateID GetRSICKTDGTexturedSlashTrace()		{ return m_RSICKTDGTexturedSlashTrace;		}
		CKTDGStateManager::KStateID GetRSICKTDGLineMap()		{ return m_RSICKTDGLineMap;			}
		CKTDGStateManager::KStateID GetRSICKTDGPlanShadow()		{ return m_RSICKTDGPlanShadow;		}
		CKTDGStateManager::KStateID GetRSICKTDGUIDialog()		{ return m_RSICKTDGUIDialog;		}
        CKTDGStateManager::KStateID GetRSICKTDXDeviceLayer(int inx)	{ return m_RSICKTDXDeviceLayer[inx];		}

		void ClearScreen( int num = 1, D3DXCOLOR color = 0xffffffff ){ SetClearScreenColor( color ); m_ClearScreen = num; }
		int	 GetClearScreen(){ return m_ClearScreen; }
		void SetClearScreenColor( D3DXCOLOR color ){ m_ClearScreenColor = color; }
		D3DXCOLOR GetClearScreenColor(){ return m_ClearScreenColor; }

		void SetClearColor( D3DXCOLOR color ){ m_ClearColor = color; }
		void SetClearColorA( int A ){ m_ClearColor.a = A/255.0f; }
		void SetClearColorR( int R ){ m_ClearColor.r = R/255.0f; }
		void SetClearColorG( int G ){ m_ClearColor.g = G/255.0f; }
		void SetClearColorB( int B ){ m_ClearColor.b = B/255.0f; }
		D3DXCOLOR GetClearColor(){ return m_ClearColor; }

		bool BeginRender();
		void EndRender();

		void ObjectChainSort();
		void ObjectChainNonAlphaRender();
		void ObjectChainAlphaRender();
		void BackUIRender();
		void FrontUIRender();

#ifdef FILDE_ONLY_CHARACTER
		void NonAlphaLayerRender( int iLayer );
		void AlphaLayerRender( int iLayer );
#endif FILDE_ONLY_CHARACTER

		virtual HRESULT OnResetDevice();
		virtual HRESULT OnLostDevice();

		virtual HRESULT OnFrameMove( double fTime, float fElapsedTime );
		virtual HRESULT OnFrameRender();

#ifdef CONVERSION_VS
		HRESULT AddObjectChain( CKTDGObject* object );
#else CONVERSION_VS
		HRESULT AddObjectChain( CKTDGObject* object )
		{ 
			KTDXPROFILE();
			if ( object == NULL )
				return E_FAIL;
            ASSERT( CKTDGObject::CKTDGManagerAccess::_GetDGManagerHandle( *object ) == NULL );
            if ( CKTDGObject::CKTDGManagerAccess::_GetDGManagerHandle( *object ) != NULL )
                return E_FAIL;
            CKTDGObject::CKTDGManagerAccess::_SetDGManagerHandle( *object, m_RenderObjectChain.insert( m_RenderObjectChain.end(), object ) );
            object->NotifyDGManagerChanged( *this );
            return S_OK;
		}
#endif CONVERSION_VS
		
#ifdef CONVERSION_VS
		HRESULT RemoveObjectChain( CKTDGObject* object );
#else CONVERSION_VS
        HRESULT RemoveObjectChain( CKTDGObject* object )
		{
			KTDXPROFILE();

            ASSERT( object != NULL );
            if ( object == NULL )
                return E_FAIL;
            if ( CKTDGObject::CKTDGManagerAccess::_GetDGManagerHandle( *object ) == NULL )
                return E_FAIL;
            m_RenderObjectChain.erase( CKTDGObject::CKTDGManagerAccess::_GetDGManagerHandle( *object ) );
            CKTDGObject::CKTDGManagerAccess::_SetDGManagerHandle( *object, NULL );
            object->NotifyDGManagerChanged( *this );
            return S_OK;
		}
#endif CONVERSION_VS
		bool IsInObjectChain( CKTDGObject* object )
		{
			KTDXPROFILE();

            ASSERT( object != NULL );
            if ( object == NULL )
                return false;
            return object->IsManagedByDGManager();
		}

#ifdef KTDGMANAGER_USE_DISABLE_LAYER
        void SetLayerState( int layer, bool bRender )
		{
			KTDXPROFILE();
			if( bRender == true )
			{
                m_DisableLayer.erase( layer );
			}
			else
			{
                m_DisableLayer.insert( layer );
			}			
		}

        bool IsLayerRenderable( int layer )
        {
            return  m_DisableLayer.find( layer ) == m_DisableLayer.end();
        }//IsLayerRenderable()
#endif // #ifdef KTDGMANAGER_USE_DISABLE_LAYER

#ifdef BUBBLE_BOBBLE_TEST
		float GetOrthoScale() const { return m_fOrthoScale; }
		void SetOrthoScale(float val) { m_fOrthoScale = val; }
		
		void SetProjection( float Near, float Far, bool bPerspective = true, float fOrthoScale = 1.f );
#else
		void SetProjection( float Near, float Far, bool bPerspective = true );
#endif BUBBLE_BOBBLE_TEST
		
#ifdef KEYFRAME_CAMERA
		void SetProjectionFov( float Near, float Far, float fFov );
#endif KEYFRAME_CAMERA

		float GetNear(){ return m_Near; }
		float GetFar(){ return m_Far; }
		bool GetPerspective(){ return m_bPerspective; }
#ifdef MODIFY_FRUSTUM
		void SetPerspectiveValue( float fVal ) { m_fPerspective = fVal; }
		float GetPerspectiveValue() { return m_fPerspective; }
		void SetNear( float fVal ) { m_Near = fVal; }
		void SetFar( float fVal ) { m_Far = fVal; }
#endif

		//CKTDGCamera*				GetCamera(){ return m_pCamera; }
        //CKTDGFrustum*				GetFrustum(){ return m_pFrustum; }
        CKTDGCamera&				GetCamera(){ return m_kCamera; }
		const CKTDGFrustum&			GetFrustum() const { return m_kFrustum; }
		//CKTDGGlow*					GetGlow(){ return m_pGlow; }
		//CKTDGMotionBlur*			GetMotionBlur(){ return m_pMotionBlur; }
		CKTDGXRenderer*				GetXRenderer(){ return m_pXRenderer; }
		CKTDGFontManager*			GetFontManager(){ return m_pFontManager; }
		CKTDGUIDialogManager*		GetDialogManager(){ return m_pDialogManager; }
		//CKTDGGrayScale*				GetGrayScale(){ return m_pGrayScale; }


		bool						GetIsFading() { return m_bFading; }
		void 						StartFadeIn( float fTime = 0.5f );
		void 						StartFadeOut( float fTime = 0.5f );

		UINT						GetRenderObjectChainSize() { return m_RenderObjectChain.size(); }
#ifdef CONVERSION_VS
		CKTDGObjectHandle			GetRenderObjectChainEnd(){ return m_RenderObjectChain.end(); }
#endif CONVERSION_VS

        UINT                        GetAlphaObjectChainSize() { return m_uAlphaObjectNum; }
		UINT						GetNonAlphaObjectChainSize() { return m_uNonAlphaObjectNum; }

		//{{ kimhc // 2010.9.8 // 홈페이지 개편에서 아바타 이미지 표현으로 사용될 캐릭터 이미지 캡쳐 기능 테스트
#ifdef	AVATAR_IMAGE_TEST
		bool ScreenCaptureForAvatar( OUT wstring& wstrFileName, CKTDXDeviceRenderTargetTexture* pRenderTargetTextureAvatar_ );
#endif	AVATAR_IMAGE_TEST
		//}} kimhc // 2010.9.8 // 홈페이지 개편에서 아바타 이미지 표현으로 사용될 캐릭터 이미지 캡쳐 기능 테스트
		bool						ScreenCaptureOld();
		bool						ScreenCapture();
		void						ScreenCapture( const WCHAR* pFileName );
#ifdef FILDE_ONLY_CHARACTER
		bool						ScreenCapture_Png();
#endif FILDE_ONLY_CHARACTER

		bool						IsInFrustum( CKTDGObject* object );
		bool						IsInFrustum( CKTDGObject* object, float& fNeedDistance );

//{{ robobeg : 2008-10-18
        int                         GetCullCount()      { return m_nCull; }
//}} robobeg : 2008-10-18

#ifdef FIELD_CHARACTER_SCREEN_SHOT
		bool ScreenCapture_Png();
		bool ScreenCapture_Tga();

		void SetField_Character_Screen_shot( bool bVal ) { m_bField_Character_Screen_shot = bVal; };
		bool GetField_Character_Screen_shot() const { return m_bField_Character_Screen_shot; };

		void SetField_NPC_Screen_shot( bool bVal ) { m_bField_NPC_Screen_shot = bVal; };
		bool GetField_NPC_Screen_shot() const { return m_bField_NPC_Screen_shot; };

		void SetField_MainCha_Screen_shot( bool bVal ) { m_bField_MainCha_Screen_shot = bVal; };
		bool GetField_MainCha_Screen_shot() const { return m_bField_MainCha_Screen_shot; };

		void SetField_BG_Screen_shot( bool bVal ) { m_bField_BG_Screen_shot = bVal; };
		bool GetField_BG_Screen_shot() const { return m_bField_BG_Screen_shot; };
#endif

    private:

        void                        _RenderObjectRenderHintVector( const KObjectRenderHintVector& vecInObject_, bool bInLayerExclusive_ = false );

	public:
		const static int				LIMIT_DISTANCE = 800;
		const static int				LIMIT_DISTANCE_SQ = LIMIT_DISTANCE * LIMIT_DISTANCE;

	private:
		HWND						m_hWnd;

		CKTDGStateManager::KStateID m_RSICKTDXDeviceTexture;
		CKTDGStateManager::KStateID m_RSICKTDGSlashTrace;
		CKTDGStateManager::KStateID m_RSICKTDGTexturedSlashTrace;
		CKTDGStateManager::KStateID m_RSICKTDGLineMap;
		CKTDGStateManager::KStateID m_RSICKTDGPlanShadow;
		CKTDGStateManager::KStateID m_RSICKTDGUIDialog;
        CKTDGStateManager::KStateID m_RSICKTDXDeviceLayer[6];
		
		int							m_ClearScreen;
		D3DXCOLOR					m_ClearScreenColor;
		CKTDXTimer					m_ClearScreenTimer;

		D3DXCOLOR					m_ClearColor;

		D3DXMATRIX					m_matProjection;
		float						m_Near;
		float						m_Far;
		bool						m_bPerspective;
#ifdef MODIFY_FRUSTUM
		float						m_fPerspective;
#endif

        std::set<int>               m_DisableLayer;

        CKTDGObjectList             m_RenderObjectChain;
		KObjectRenderHintVector		m_InnerUIChain;
		KObjectRenderHintVector		m_OverUIChain;
#ifdef OVER_UI_ALPHA_RENDER_FIX
		KObjectRenderHintVector		m_OverUIAlphaChain;
#endif OVER_UI_ALPHA_RENDER_FIX

		//CKTDGCamera*				m_pCamera;
        //CKTDGFrustum*				m_pFrustum;
        CKTDGCamera                 m_kCamera;
		CKTDGFrustum				m_kFrustum;
		//CKTDGGlow*					m_pGlow;
		//CKTDGMotionBlur*			m_pMotionBlur;
		CKTDGXRenderer*				m_pXRenderer;
		CKTDGFontManager*			m_pFontManager;
		CKTDGUIDialogManager*		m_pDialogManager;
		//CKTDGGrayScale*				m_pGrayScale;

		bool						m_bFading;
		bool						m_bFadeOut;
		float						m_fTimeFadeMax;
		float						m_fTimeFade;
		float						m_fFadeAlpha;
		CKTDXDeviceTexture*			m_pTextureFadeInOut;

		float						m_fMaxTimeFadeIn;		// fadein 되는 시간
		float						m_fMaxTimeFadeOut;		// fadeout 되는 시간

		float						m_fCaptureDelay;

#ifdef BUBBLE_BOBBLE_TEST
		float						m_fOrthoScale;
#endif BUBBLE_BOBBLE_TEST

//{{ robobeg : 2008-10-18
        int                         m_nCull;
        UINT                        m_uAlphaObjectNum;
        UINT                        m_uNonAlphaObjectNum;

#ifdef FIELD_CHARACTER_SCREEN_SHOT
		bool			m_bField_Character_Screen_shot;
		bool			m_bField_NPC_Screen_shot;
		bool			m_bField_MainCha_Screen_shot;
		bool			m_bField_BG_Screen_shot;
#endif

        class LayerGroup
        {
        public:
            LayerGroup()
            {                
                m_layerObject.reserve(500);
            }
            KObjectRenderHintVector m_layerObject;
        };

        // 우선은 레이어수를 강제로 설정한다.
        LayerGroup m_AlphaGroup[m_ciNumLayerGroups];
        LayerGroup m_NonAlphaGroup[m_ciNumLayerGroups];
//}}robobeg : 2008-10-18

};
