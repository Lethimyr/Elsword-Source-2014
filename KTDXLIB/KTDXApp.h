#pragma once

#ifdef CHANGE_KEY_DEVICE
#include "KSystemTimer.h"
#endif

#define KTDX_BASE_MESSAGE WM_USER + 100

class CKTDXApp

{
	public:
		enum EXIT_CODE
		{
			EC_STANDARD,
		};

		enum KTDX_MESSAGE
		{
			KM_SYSTEM_BASE = KTDX_BASE_MESSAGE + 1,
			KM_UI_CONTROL_EVENT,
			KM_UI_CONTROL_CUSTOM_EVENT,
			KM_UI_CONTROL_CUSTOM_FUNC,
			KM_TCP_RELAY,
			
			KM_RECEIVE_FROM_P2P,
			KM_WEB_BROWSER_KEY_EVENT,				// 넥슨캐시 충전페이지에서 KeyUp event 처리하기 위해서

			KM_GAME_BASE,
		};

		enum KTDX_QUIT_TYPE
		{
			KQT_CLOSE_WINDOW = 0,
			KQT_FIND_HACKING,
			KQT_ESC_GAME,
			KQT_ALREADY_GAME_PLAY,
			KQT_INVALID_ID_OR_PASSWORD,
			KQT_CONNECT_FAILED,
			KQT_NEXON_AUTH_FAILED,
			KQT_CONNECTION_LOST,
			KQT_VERIFY_ACCOUNT,
			KQT_IMEEditBOX_ERROR,
			KQT_DOUBLE_CONNECT,
		};

		struct GameMessage
		{
			HWND	hWnd;
			UINT	uMsg;
            WPARAM	wParam;
			LPARAM	lParam;
		};

	public:
		CKTDXApp( HWND hWnd, HINSTANCE hInstance, IDirect3DDevice9* pd3dDevice );
		~CKTDXApp(void);

#ifndef X2OPTIMIZE_DISABLE_LUA_MULTITHREADING
		//{{ robobeg : 2011-01-19
		void InitializePerThread();
		void FinalizePerThread();
		//}} robobeg : 2011-01-19
#endif  X2OPTIMIZE_DISABLE_LUA_MULTITHREADING

		static void StartUp();
		static void Exit( EXIT_CODE code = EC_STANDARD );

// #ifdef BACKGROUND_LOADING_TEST // 2008-10-17
//         void    OnPrePresent();
//         void    OnPostPresent();
// #endif // BACKGROUND_LOADING_TEST // 2008-10-17
		HRESULT OnFrameMove( double fTime, float fElapsedTime );
		HRESULT OnFrameRender();

		void SlowFrame( float fSlowFactor, float fSlowTime, float fSlowReserveTime = 0.0f ){ m_fSlowFactor = fSlowFactor; m_fSlowTime = fSlowTime; m_fSlowReserveTime = fSlowReserveTime; }
		void SlowFrame_LUA( float fSlowFactor, float fSlowTime ) { SlowFrame( fSlowFactor, fSlowTime ); }
		void SkipFrame(){ m_bSkipFrame = true; }
		void StopFrame( int frame = 1, int delayFrame = 0 ){ m_StopFrame = frame; m_StopFrameDelay = delayFrame; }
#ifdef	X2OPTIMIZE_INFORM_FIRST_FRAME_OF_SIMULATION_LOOP
		bool IsFirstFrameOfSimulationLoop() { return m_bFirstFrame; }
#endif	X2OPTIMIZE_INFORM_FIRST_FRAME_OF_SIMULATION_LOOP

//{{ robobeg : 2008-10-24
//CKTDGStateManager::SetTexture 명령으로 대체
		//bool IsSameBeforeTexture( int stage, LPDIRECT3DTEXTURE9 tex );
		//void SetBeforeTexture( int stage, LPDIRECT3DTEXTURE9 tex );
		//void SetNULLTexture( int stage );
//}} robobeg : 2008-10-24

		HRESULT OnResetDevice( const D3DSURFACE_DESC* pBackBufferSurfaceDesc );
		HRESULT OnLostDevice();

		bool MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
		bool SendGameMessage( UINT uMsg, WPARAM wParam, LPARAM lParam, bool bDirectProcess = true );
		//{{ seojt // 2009-8-19, 16:44
		bool SendGameDlgMessage( UINT uMsg, CKTDGUIDialogType wParam, LPARAM lParam, bool bDirectProcess = true );
		//}} seojt // 2009-8-19, 16:44

		void InsertStage( CKTDXStage* stage );
		bool RemoveStage( CKTDXStage* stage, bool bImmediate );
		bool CheckStage( CKTDXStage* stage );
		CKTDXStage* GetStage( const WCHAR* wszStageName );

		//void				TimeReset(){ m_BeforeFrameTime = m_NowFrameTime; }
		float				GetElapsedTime(){ return m_fConstElapsedTime; }
		void				SetFrameRate( float fFrameRate ){ m_fConstElapsedTime = 1.0f/fFrameRate; }

		IDirect3DDevice9*	GetDevice(){ return m_pd3dDevice; }
		HWND				GetHWND(){ return m_hWnd; }
		HINSTANCE			GetHInstance(){ return m_hInstance; }
        //{{ seojt // 2009-1-14, 20:49
        DWORD               GetMainThreadID() const { return m_dwMainThreadID; }
        //}} seojt // 2009-1-14, 20:49
		CKTDXDeviceManager* GetDeviceManager(){ return m_pDeviceManager; }
		CKTDIManager*		GetDIManager(){ return m_pDIManager; }
		CKTDGManager*		GetDGManager(){ return m_pDGManager; }
		CKTDSManager*		GetDSManager(){ return m_pDSManager; }
#ifdef CONVERSION_VS
#else CONVERSION_VS
		CKTDNManager*		GetDNManager(){ return m_pDNManager; }
#endif CONVERSION_VS

//#ifdef  DYNAMIC_VERTEX_BUFFER_OPT
        CKTDGDynamicVBManager*
                            GetDVBManager() { ASSERT( m_pDVBManager != NULL ); return m_pDVBManager; }
//#endif
		
		bool				GetAppDestroy(){ return m_bAppDestroy; }

#ifdef CHANGE_KEY_DEVICE
		DWORD               GetFrameTimestamp() const { return m_dwFrameTimestamp; }
		DWORD               AdjustFrameTimestamp();
#endif

		double				GetTime(){ return m_dTime; }
		float				GetFrameMoveFPS(){ return m_fFrameMoveFPS; }
		int					GetFrameMoveCount(){ return m_FrameMoveCount; }
		int					GetRenderCount(){ return m_RenderCount; }

		void				AddRenderCount() { m_RenderCount++; }

		CKTDXCollision*		GetCollision(){ return m_pCollision; }
		CKTDGUIDialogManager* GetDialogManager_LUA() { return m_pDGManager->GetDialogManager(); }

		void GetWorldTransform( D3DXMATRIX* matrix ){ *matrix = m_WorldMatrix; }
		const D3DXMATRIX& GetWorldTransform() const { return m_WorldMatrix; }

		void GetViewTransform( D3DXMATRIX* matrix ){ *matrix = m_ViewMatrix; }
		const D3DXMATRIX& GetViewTransform() const { return m_ViewMatrix; }

		void GetProjectionTransform( D3DXMATRIX* matrix ){ *matrix = m_ProjectionMatrix; }
		const D3DXMATRIX& GetProjectionTransform() const { return m_ProjectionMatrix; }

		const D3DXMATRIX& GetViewProjectionTransform(){ return m_ViewProjectionMatrix; }
        void ResetWorldTransform()
        {
            D3DXMatrixIdentity( &m_WorldMatrix );
            m_pd3dDevice->SetTransform( D3DTS_WORLD, &m_WorldMatrix ); 
        }
		void SetWorldTransform( D3DXMATRIX* matrix )
		{ 
			m_WorldMatrix = *matrix; 
			m_pd3dDevice->SetTransform( D3DTS_WORLD, matrix ); 
		}
		void SetViewTransform( D3DXMATRIX* matrix )
		{ 
			m_ViewMatrix = *matrix; 
			m_pd3dDevice->SetTransform( D3DTS_VIEW, matrix ); 
			D3DXMatrixMultiply( &m_ViewProjectionMatrix, &m_ViewMatrix, & m_ProjectionMatrix );
			m_ViewMatChangeCount++;
		}
		void SetProjectionTransform( D3DXMATRIX* matrix )
		{ 
			m_ProjectionMatrix = *matrix; 
			m_pd3dDevice->SetTransform( D3DTS_PROJECTION, matrix ); 
			D3DXMatrixMultiply( &m_ViewProjectionMatrix, &m_ViewMatrix, & m_ProjectionMatrix );
		}

		const int GetViewMatChangeCount(){ return m_ViewMatChangeCount; }

		const D3DVIEWPORT9& GetViewport()
		{
			return m_ViewPort;
		}

		void SetViewport( D3DVIEWPORT9* viewport )
		{
			m_ViewPort = *viewport;

			m_ResolutionScale.x = (float)m_ViewPort.Width/1024.0f;
			m_ResolutionScale.y = (float)m_ViewPort.Height/768.0f;
		}
		void SetViewport( int x, int y, int width, int height, float MinZ, float MaxZ )
		{
			m_ViewPort.X		= x;
			m_ViewPort.Y		= y;
			m_ViewPort.Width	= width;
			m_ViewPort.Height	= height;
			m_ViewPort.MinZ		= MinZ;
			m_ViewPort.MaxZ		= MaxZ;

			m_ResolutionScale.x = (float)m_ViewPort.Width/1024.0f;
			m_ResolutionScale.y = (float)m_ViewPort.Height/768.0f;
		}

		D3DXVECTOR2 ConvertByResolution( float fX, float fY )
		{
			D3DXVECTOR2 retval( fX, fY );
			retval.x *= m_ResolutionScale.x;
			retval.y *= m_ResolutionScale.y;
			return retval;
		}
		D3DXVECTOR2 ConvertByResolution( const D3DXVECTOR2& fSrc )
		{
			D3DXVECTOR2 retval = fSrc;
			retval.x *= m_ResolutionScale.x;
			retval.y *= m_ResolutionScale.y;
			return retval;
		}
		const float& GetResolutionScaleX() const { return m_ResolutionScale.x; }
		const float& GetResolutionScaleY() const { return m_ResolutionScale.y; }
		D3DXVECTOR2 MouseConvertByResolution( float fX, float fY )
		{
			D3DXVECTOR2 retval( fX, fY );
			retval.x /= m_ResolutionScale.x;
			retval.y /= m_ResolutionScale.y;
			return retval;
		}
		D3DXVECTOR2 MouseConvertByResolution( D3DXVECTOR2& fSrc )
		{
			D3DXVECTOR2 retval = fSrc;
			retval.x /= m_ResolutionScale.x;
			retval.y /= m_ResolutionScale.y;
			return retval;
		}
		D3DXVECTOR2 GetProj3DPos( const D3DXVECTOR3& pos );
		//{{ seojt // 2008-11-1, 12:43
#ifdef X2TOOL
		D3DXVECTOR3 GetUnProj3DPos( D3DXVECTOR3 pos );
#endif

		KLuabinder*	GetLuaBinder()
        { 
            // 루아 바인더 호출이 보조 쓰레드에서 일어나지 않도록 assertion하다.
            // - jintaeks on 2008-11-01, 12:44
//             ASSERT( ::GetCurrentThreadId() != GetDeviceManager()->GetThreadID() );
//             return &m_LuaBinder; 
			ASSERT( NULL != ms_pLuaBinder );
			return ms_pLuaBinder;
        }
		//}} seojt // 2008-11-1, 12:43
		void		RegisterLuabind( KLuabinder* pKLuabinder );

		void SetEnumToString( wstring wstrType, int enumIndex, wstring wstrEnum );
		const wstring& GetEnumToString( wstring wstrType, int enumIndex );
		void InitEnumToString();

		int	Random_LUA( int randMax ){ return (int)(rand() % randMax); }

		bool			GetIsNowVeryfy() { return m_bNowVeryfy; }

		void			SetFindHacking( bool bCheck, const wstring& findHackingDetailMsg = L"" );
		bool			GetFindHacking() { return m_bFindHacking; }

#ifdef GAMEGUARD_DETECTED_HACK_POPUP
		void			SetGameGuardFindHacking( bool bCheck ) { m_bGameGuardFindHacking = bCheck; }
		bool			GetGameGuardFindHacking() { return m_bGameGuardFindHacking; }
#endif //GAMEGUARD_DETECTED_HACK_POPUP

		const WCHAR*	GetFindHackingDetailMsg() { return m_FindHackingDetailMsg.c_str(); }

		void			GetPhysicRam( DWORD& dwTotal, DWORD& dwFree )
		{
			MEMORYSTATUS memStatus;
			memStatus.dwLength = sizeof( MEMORYSTATUS );
			GlobalMemoryStatus( &memStatus );
			dwTotal = (DWORD)memStatus.dwTotalPhys;
			dwFree = (DWORD)memStatus.dwAvailPhys;
		}
		void			GetVideoRam( DWORD& dwTotal, DWORD& dwFree );

		bool			GetIsFullScreen() { DXUTDeviceSettings deviceSettings = DXUTGetDeviceSettings(); 
											return !deviceSettings.pp.Windowed; }        

		void			Make2DPosToRay( int posx, int posy, float fDepth, D3DXVECTOR3& startPos, D3DXVECTOR3& endPos )
		{
			D3DVIEWPORT9 vp;
			D3DXMATRIX matProj, matView, m;
			POINT ptCursor;
			m_pd3dDevice->GetViewport(&vp);
			m_pd3dDevice->GetTransform( D3DTS_PROJECTION, &matProj );
			GetCursorPos( &ptCursor );
			ScreenToClient( m_hWnd, &ptCursor );

			D3DXVECTOR3 v;
			v.x = ((  (((ptCursor.x-vp.X)*2.0f/vp.Width ) - 1.0f)) - matProj._31 ) / matProj._11;
			v.y = ((- (((ptCursor.y-vp.Y)*2.0f/vp.Height) - 1.0f)) - matProj._32 ) / matProj._22;
			v.z =  1.0f;

			m_pd3dDevice->GetTransform( D3DTS_VIEW, &matView );
			D3DXMatrixInverse( &m, NULL, &matView );
			D3DXVECTOR3 vPickRayDir;
			vPickRayDir.x  = v.x*m._11 + v.y*m._21 + v.z*m._31;
			vPickRayDir.y  = v.x*m._12 + v.y*m._22 + v.z*m._32;
			vPickRayDir.z  = v.x*m._13 + v.y*m._23 + v.z*m._33;
			startPos.x = m._41;
			startPos.y = m._42;
			startPos.z = m._43;
			
			endPos = startPos + vPickRayDir * fDepth;
		}

		void			NoticeQuitType( KTDX_QUIT_TYPE quitType );
		void			PlaySound2D_LUA( char* pFileName );
		void			PlaySound_LUA( char* pFileName );
		void			StopSound_LUA( char* pFileName );
		bool			GetCannotStretchRect(){ return m_bCannotStretchRect; }
		bool			FindProcess( wstring wstrExeFile );

		D3DXVECTOR3		GetVectorByRotate( const D3DXVECTOR3& pos, const D3DXVECTOR3& vRotateDegree )
		{
			D3DXMATRIX mat;
			D3DXMatrixIdentity( &mat );
			D3DXMatrixRotationYawPitchRoll( &mat, D3DXToRadian(vRotateDegree.y), D3DXToRadian(vRotateDegree.x), D3DXToRadian(vRotateDegree.z) );
			D3DXVECTOR4 resultPos;
			D3DXVec3Transform( &resultPos, &pos, &mat );

			return D3DXVECTOR3(resultPos);
		}

		void OnWebBrowserKeyEvents( LPMSG pMsg );


#ifdef CHECKSUM_THREAD_TEST
		KFileVerifier* GetFileVerifier() { return m_pFileVerifier; }
#endif CHECKSUM_THREAD_TEST
		

		D3DXVECTOR3	GetVec3( float fPosX, float fPosY, float fPosZ ) { return D3DXVECTOR3(fPosX,fPosY,fPosZ ); }
		
//{{ robobeg : 2008-10-28
// 초기 리소스 로딩시 메인 쓰레드에서 동작하는 루아 가비지 콜렉터를 일시 중단시키기 위해 사용
        void    EnterLuaThread();
        void    ExitLuaThread();
//}} robobeg : 2008-10-28


		//HRESULT LoadKLuaManagerTemplate();

		static void LuaPerFrameGarbageCollection( int iNumSteps = PERFRAME_LUA_GARBAGE_COLLECTION_STEP );

//{{ robobeg : 2014-01-07
        // CKTDXDeviceManager 에서 옮겨옴

        // 아래 template 코드는 KLuaManager, KLuaManagerProxy, KLuabinder 등
        // bool DoMemoryNotEncrypt( const char* pRealFile, long nSize ) API 를 제공하는 매니저는 무엇이든 적용 가능합니다.
        template<typename LUADOMEMORYNOTENCRYPT>
        HRESULT LoadAndDoMemory_ErrorCode( LUADOMEMORYNOTENCRYPT* pLuaManager, const WCHAR* pFileName, bool bEncryption = true );
        template<typename LUADOMEMORYNOTENCRYPT>
        bool LoadAndDoMemory( LUADOMEMORYNOTENCRYPT* pManager, const WCHAR* pFileName, bool bEncryption = true );

        template<typename LUADOMEMORYNOTENCRYPT>
        HRESULT LoadAndDoMemory_LocalFile_ErrorCode( LUADOMEMORYNOTENCRYPT* pLuaManager, const WCHAR* pFileName );
        template<typename LUADOMEMORYNOTENCRYPT>
        bool LoadAndDoMemory_LocalFile( LUADOMEMORYNOTENCRYPT* pManager, const WCHAR* pFileName );


        __forceinline bool       LoadLuaTinker( const WCHAR* pFileName, bool bEncryption = true )
        {
            return LoadAndDoMemory( GetLuaBinder(), pFileName, bEncryption );
        }

        __forceinline bool       LoadLuaTinker_LocalFile( const WCHAR* pFileName )
        {
            return LoadAndDoMemory_LocalFile( GetLuaBinder(), pFileName );
        }


//}} robobeg : 2014-01-07

	private:

#ifdef CHANGE_KEY_DEVICE
		void		FrameProcess( IN bool bStop_ );
#else
		void    FrameProcess();
#endif

//{{ robobeg : 2008-10-28
// KLuaManager를 생성할 때 기본 global environment 로 제공될 lua 파일 등을 설정하는 함수.
// 이곳에서 로딩된 라이브러리, global variables 들은 이후 새로 로딩하지 않아도 new env = true 옵션을 주고 생성한
// KLuaManager 의 lua state에서는 기본적으로 제공된다.


//}} robobeg : 2008-10-28
    private:		

#ifdef  X2OPTIMIZE_DISABLE_LUA_MULTITHREADING
        static KLuabinder*   ms_pLuaBinder;
#else   X2OPTIMIZE_DISABLE_LUA_MULTITHREADING
		__declspec(thread) static KLuabinder*   ms_pLuaBinder;
#endif  X2OPTIMIZE_DISABLE_LUA_MULTITHREADING

		IDirect3DDevice9*		m_pd3dDevice;
#ifdef CONVERSION_VS
#else CONVERSION_VS
		LPDIRECTDRAW7			m_lpDD;
#endif CONVERSION_VS
		HWND					m_hWnd;
		HINSTANCE				m_hInstance;
        //{{ seojt // 2009-1-14, 20:49
        /// main thread의 ID를 가진다. GetCurrentThreadID()를 호출하여
        /// 함수가 main thread에서 호출되는지 어떤지를 검사하기 위해서 사용한다.
        /// - jintaeks on 2009-01-14, 20:49
        DWORD                   m_dwMainThreadID;
        //}} seojt // 2009-1-14, 20:49
		bool					m_bAppDestroy;

#ifdef CHANGE_KEY_DEVICE
		DWORD					m_dwFrameTimestamp;
		float                   m_fFrameTimestampFraction;
		DWORD					m_dwCurrentFrameMoveTimestamp;
		bool                    m_bInFrameMoveStepLoop;
#else
		float					m_BeforeFrameTime;
		float					m_NowFrameTime;
		float					m_RemainTime;

#endif

		float					m_fConstElapsedTime;
		double					m_dTime;

		bool					m_bSkipFrame;
		int						m_StopFrame;
		int						m_StopFrameDelay;		
		float					m_fSlowFactor;
		float					m_fSlowTime;
		float					m_fSlowReserveTime;

		int						m_FrameMoveCount;
		int						m_RenderCount;
		float					m_fFrameMoveFPS;
		float					m_fElapsedTimeAdd;
		int						m_FrameMoveCountAdd;
		

//{{ robobeg : 2008-10-24
		//LPDIRECT3DTEXTURE9		m_pBeforeTexture0;
		//LPDIRECT3DTEXTURE9		m_pBeforeTexture1;
		//LPDIRECT3DTEXTURE9		m_pBeforeTexture2;
//}} robobeg : 2008-10-24

		D3DXMATRIX				m_WorldMatrix;
		D3DXMATRIX				m_ViewMatrix;
		D3DXMATRIX				m_ProjectionMatrix;
		D3DXMATRIX				m_ViewProjectionMatrix;
		D3DVIEWPORT9			m_ViewPort;
		int						m_ViewMatChangeCount;
		D3DXVECTOR2				m_ResolutionScale;
		//KLuabinder				m_LuaBinder;

		CKTDXCollision*			m_pCollision;

		vector<CKTDXStage*>		m_vecStageChain;
		queue<GameMessage>		m_GameMsgQ;

		CKTDXDeviceManager*		m_pDeviceManager;
		CKTDIManager*			m_pDIManager;
		CKTDGManager*			m_pDGManager;
		CKTDSManager*			m_pDSManager;
#ifdef CONVERSION_VS
#else CONVERSION_VS
		CKTDNManager*			m_pDNManager;
#endif CONVERSION_VS
//#ifdef  DYNAMIC_VERTEX_BUFFER_OPT
		CKTDGDynamicVBManager*  m_pDVBManager;
//#endif

#ifdef CHECKSUM_THREAD_TEST
		KFileVerifier*          m_pFileVerifier;		// filechecksum 검사 쓰레드 - seojt // 2008-1-5, 17:50
#endif CHECKSUM_THREAD_TEST
		

		bool					m_bNowVeryfy;
		float					m_fVeryfyTime;
		bool					m_bFindHacking;
#ifdef GAMEGUARD_DETECTED_HACK_POPUP
		bool					m_bGameGuardFindHacking;
#endif //GAMEGUARD_DETECTED_HACK_POPUP
		wstring					m_FindHackingDetailMsg;

		bool					m_bCannotStretchRect;

		std::map< std::pair<wstring, int>, wstring >	m_mapEnumToString;
		
//{{ robobeg : 2008-10-28
// 초기 리소스 로딩시 메인 쓰레드에서 동작하는 루아 가비지 콜렉터를 일시 중단시키기 위해 사용
        int     m_iLuaThreadCounter;
//}} robobeg : 2008-10-28

#ifdef	X2OPTIMIZE_INFORM_FIRST_FRAME_OF_SIMULATION_LOOP
		bool	m_bFirstFrame;
#endif	X2OPTIMIZE_INFORM_FIRST_FRAME_OF_SIMULATION_LOOP

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
public:
	bool IsFinalFrameOfSimulationLoop() { return m_bFinalFrame; }

private:
	bool m_bFinalFrame;
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE

#ifdef X2OPTIMIZE_ONE_SIMUL_ONE_RENDER_TEST
public:
	void SetEnableOneSimulOneRenderTest( bool bEnable ) { m_bOneSimulOneRender = bEnable; }

private:
	bool m_bOneSimulOneRender;
#endif//X2OPTIMIZE_ONE_SIMUL_ONE_RENDER_TEST
};

extern CRITICAL_SECTION g_csGameMessage;
extern bool KTDXSendGameMessage( UINT uMsg, WPARAM wParam, LPARAM lParam, bool bDirectProcess = true );



template<typename LUADOMEMORYNOTENCRYPT>
__forceinline HRESULT CKTDXApp::LoadAndDoMemory_ErrorCode( LUADOMEMORYNOTENCRYPT* pLuaManager, const WCHAR* pFileName, bool bEncryption )
{

	//루아 매니져 로딩
    if ( pLuaManager == NULL || pFileName == NULL || pFileName[0] == NULL )
    {
		ErrorLogMsg( KEM_ERROR72, "" );
        return E_INVALIDARG;
    }//if
//	CSLock locker( m_DeviceLock );

    if ( GetDeviceManager() == NULL || GetDeviceManager()->GetMassFileManager() == NULL )
    {
		ErrorLogMsg( KEM_ERROR72, "" );
        return E_INVALIDARG;
    }

	KGCMassFileManager::CMassFile::MASSFILE_MEMBERFILEINFO_POINTER Info;
//#ifdef X2OPTIMIZE_MASS_FILE_FIRST_BUGFIX
	Info = GetDeviceManager()->GetMassFileManager()->LoadDataFile( pFileName, true
#ifdef  X2OPTIMIZE_ENCRYPT_AFTER_COMPRESS
        , bEncryption
#endif  X2OPTIMIZE_ENCRYPT_AFTER_COMPRESS
        );
//#else//X2OPTIMIZE_MASS_FILE_FIRST_BUGFIX
//	Info = m_MassFileManager.LoadDataFile( pFileName );
//#endif//X2OPTIMIZE_MASS_FILE_FIRST_BUGFIX

#ifdef MASSFILE_MAPPING_FUNCTION
	wstring wstrFileNameExtension = pFileName;
	int ifirstIndex = 0;
	wstring wstrFileExtension= L"";
	
	wstrFileExtension = wstrFileNameExtension.substr(wstrFileNameExtension.find_last_of(L".")+1);

	if ( wstrFileExtension == L"lua")
	{
		wstrFileNameExtension = Info->strFileName;
	}

#endif//MASSFILE_MAPPING_FUNCTION



	if( Info == NULL || Info->pRealData == NULL || Info->size <= 0 )
	{
#ifdef MASSFILE_MAPPING_FUNCTION
		ErrorLogMsg( KEM_ERROR72, wstrFileNameExtension.c_str() );
#else //MASSFILE_MAPPING_FUNCTION
		ErrorLogMsg( KEM_ERROR72, pFileName );
#endif//MASSFILE_MAPPING_FUNCTION
		
		return HRESULT_FROM_WIN32( ERROR_FILE_NOT_FOUND );
	}

	if( true == bEncryption 
#if !defined(MASS_FILE_FIRST) || !defined(_SERVICE_)
#ifdef  X2OPTIMIZE_INDICATE_LOCAL_MASSFILE
        && ( Info->dwFlag & MFI_LOCALFILE ) == 0
#endif  X2OPTIMIZE_INDICATE_LOCAL_MASSFILE
#endif
        )
	{
        int iEncryptSize = Info->size;
#ifdef  X2OPTIMIZE_ENCRYPT_AFTER_COMPRESS
        if ( Info->dwFlag & MFI_COMPRESSEDDATA )
            iEncryptSize = Info->compressedSize;
#endif  X2OPTIMIZE_ENCRYPT_AFTER_COMPRESS

#ifdef _ENCRIPT_SCRIPT_
#ifdef  X2OPTIMIZE_ENFORCE_IMPORTANT_DATA_ENCRYPTION
        char* pDecryptedBuffer = (char*) Info->pRealData;
#ifdef MASSFILE_MAPPING_FUNCTION
		XORCRCDecrypt( pDecryptedBuffer, Info->pRealData, iEncryptSize, wstrFileNameExtension.c_str() );
#else //MASSFILE_MAPPING_FUNCTION
		XORCRCDecrypt( pDecryptedBuffer, Info->pRealData, iEncryptSize, pFileName );
#endif//MASSFILE_MAPPING_FUNCTION
        
#else   X2OPTIMIZE_ENFORCE_IMPORTANT_DATA_ENCRYPTION
	    char* pDecryptedBuffer = XORDecrypt( Info->pRealData, iEncryptSize );
#endif  X2OPTIMIZE_ENFORCE_IMPORTANT_DATA_ENCRYPTION
#else   _ENCRIPT_SCRIPT_
        const char* pDecryptedBuffer = Info->pRealData;
#endif  _ENCRIPT_SCRIPT_

	    bool bRetVal = false;
#ifdef  X2OPTIMIZE_ENCRYPT_AFTER_COMPRESS
        if ( Info->dwFlag & MFI_COMPRESSEDDATA )
        {
            KGCMassFileBufferPtr spRealBuffer = KGCMassFileManager::GetMassFileBufMan().GetBuffer( Info->size );
            char* pRealBuffer = (char*) spRealBuffer->GetBuffer();
		    unsigned long size = Info->size;
#ifdef  X2OPTIMIZE_ZLIB_UNCOMPRESS_ERROR_CHECK
		    if ( Z_OK == uncompress((BYTE*)pRealBuffer,&size,(BYTE*)pDecryptedBuffer,Info->compressedSize) )
            {
                ASSERT( size == Info->size );
                bRetVal = pLuaManager->DoMemoryNotEncrypt( pRealBuffer, Info->size );
            }
            else
            {
                bRetVal = false;
            }
#else   X2OPTIMIZE_ZLIB_UNCOMPRESS_ERROR_CHECK
		    uncompress((BYTE*)pRealBuffer,&size,(BYTE*)pDecryptedBuffer,Info->compressedSize);
            ASSERT( size == Info->size );
            bRetVal = pLuaManager->DoMemoryNotEncrypt( pRealBuffer, Info->size );
#endif  X2OPTIMIZE_ZLIB_UNCOMPRESS_ERROR_CHECK
        }
        else
#endif  X2OPTIMIZE_ENCRYPT_AFTER_COMPRESS     
        {  
            bRetVal = pLuaManager->DoMemoryNotEncrypt( pDecryptedBuffer, Info->size );
        }
#ifdef _ENCRIPT_SCRIPT_
#ifndef X2OPTIMIZE_ENFORCE_IMPORTANT_DATA_ENCRYPTION
	    SAFE_DELETE_ARRAY( pDecryptedBuffer );	 // XORDecrypt() 함수에서 할당된 메모리 해제
#endif  X2OPTIMIZE_ENFORCE_IMPORTANT_DATA_ENCRYPTION
#endif _ENCRIPT_SCRIPT_

		if( bRetVal == false )
		{
#ifdef MASSFILE_MAPPING_FUNCTION
			ErrorLogMsg( KEM_ERROR73, wstrFileNameExtension.c_str() );
#else //MASSFILE_MAPPING_FUNCTION
			ErrorLogMsg( KEM_ERROR73, pFileName );
#endif//MASSFILE_MAPPING_FUNCTION
			
		    return HRESULT_FROM_WIN32( ERROR_INVALID_DATA );
		}
	}
	else
	{
		if( pLuaManager->DoMemoryNotEncrypt( Info->pRealData, Info->size ) == false )
		{
#ifdef MASSFILE_MAPPING_FUNCTION
			ErrorLogMsg( KEM_ERROR73, wstrFileNameExtension.c_str() );
#else //MASSFILE_MAPPING_FUNCTION
			ErrorLogMsg( KEM_ERROR73, pFileName );
#endif//MASSFILE_MAPPING_FUNCTION
			
		    return HRESULT_FROM_WIN32( ERROR_INVALID_DATA );
		}
    }

	return S_OK;
}

template<typename LUADOMEMORYNOTENCRYPT>
__forceinline bool CKTDXApp::LoadAndDoMemory( LUADOMEMORYNOTENCRYPT* pLuaManager, const WCHAR* pFileName, bool bEncryption )
{
    HRESULT hr = LoadAndDoMemory_ErrorCode( pLuaManager, pFileName, bEncryption );
    return  SUCCEEDED( hr );
}

template<typename LUADOMEMORYNOTENCRYPT>
__forceinline HRESULT CKTDXApp::LoadAndDoMemory_LocalFile_ErrorCode( LUADOMEMORYNOTENCRYPT* pLuaManager, const WCHAR* pFileName )
{

	//루아 매니져 로딩
    if ( pLuaManager == NULL || pFileName == NULL || pFileName[0] == NULL )
    {
		ErrorLogMsg( KEM_ERROR72, "" );
        return E_INVALIDARG;
    }//if
//	CSLock locker( m_DeviceLock );

    if ( GetDeviceManager() == NULL || GetDeviceManager()->GetMassFileManager() == NULL )
    {
		ErrorLogMsg( KEM_ERROR72, "" );
        return E_INVALIDARG;
    }

	KGCMassFileManager::CMassFile::MASSFILE_MEMBERFILEINFO_POINTER Info;
//#ifdef X2OPTIMIZE_MASS_FILE_FIRST_BUGFIX
	Info = GetDeviceManager()->GetMassFileManager()->LoadDataFile_LocalFile( pFileName );
//#else//X2OPTIMIZE_MASS_FILE_FIRST_BUGFIX
//	Info = m_MassFileManager.LoadDataFile( pFileName );
//#endif//X2OPTIMIZE_MASS_FILE_FIRST_BUGFIX
	if( Info == NULL || Info->pRealData == NULL || Info->size <= 0 )
	{
		ErrorLogMsg( KEM_ERROR72, pFileName );
		return HRESULT_FROM_WIN32( ERROR_FILE_NOT_FOUND );
	}

	if( pLuaManager->DoMemoryNotEncrypt( Info->pRealData, Info->size ) == false )
	{
		ErrorLogMsg( KEM_ERROR73, pFileName );
		return HRESULT_FROM_WIN32( ERROR_INVALID_DATA );
	}

	return S_OK;
}

template<typename LUADOMEMORYNOTENCRYPT>
__forceinline bool CKTDXApp::LoadAndDoMemory_LocalFile( LUADOMEMORYNOTENCRYPT* pLuaManager, const WCHAR* pFileName )
{
    HRESULT hr = LoadAndDoMemory_LocalFile_ErrorCode( pLuaManager, pFileName );
    return  SUCCEEDED( hr );
}