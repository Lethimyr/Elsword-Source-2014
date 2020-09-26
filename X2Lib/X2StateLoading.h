#pragma once


#ifdef FIRST_GAME_LOADING_THREAD_TEST

	typedef void (*PREENDTHREAD_CALLBACK)(DWORD dwUserParam_, void* pFileVerifierObject_);

	class CX2StateLoadingThread : public KJohnThread
	{
	public:
		CX2StateLoadingThread(void);
		virtual ~CX2StateLoadingThread(void);

		/// @note   override KJohnThread::RunThread()
		virtual DWORD RunThread();

		/// end thread 콜백함수를 설정하고, thread를 시작한다.
		/// @param  pPreEndThreadCallback_: [in] RunThread() 가 종료하기 직전에 호출하는 사용자 함수
		/// @param  dwUserParam_: [in] pPreEndThreadCallback_의 첫번째 파라미터로 전달되는 값
		/// @return bool: 
		bool BeginThreadWithPreEndCB(PREENDTHREAD_CALLBACK pPreEndThreadCallback_, DWORD dwUserParam_=0L);

		/// thread에 필요한 event 객체를 생성한다.
		void _InitializeObject();

		/// thread에 사용된 event 객체를 파괴한다.
		void _FinalizeObject();

		/// thread를 종료하기 직전에 호출하는 사용자 정의 콜백 함수를 설정한다.
		/// @param  dwUserParam_: pPreEndThreadCallback_()함수의 첫번째 파라미터로 전달되는 custom 값
		/// @note   pPreEndThreadCallback_의 두번재 파라미터는 이 함수를 호출한 객체의 시작 주소이다.
		///         즉 this가 전달된다.
		void _SetPreEndThreadCallback(PREENDTHREAD_CALLBACK pPreEndThreadCallback_, DWORD dwUserParam_=0L);

		/// thread를 suspend 시킨다.
		void SuspendThread();

		/// thread를 resume시킨다.
		void ResumeThread();

		/// thread가 suspend되어 있는 경우 true를 리턴한다.
		bool IsThreadSuspended() const { return m_bThreadSuspended; }

		bool GetLoadCompleted() const { return m_bLoadCompleted; }

	private:
		HANDLE                  m_hThreadEvent;         ///< thread의 suspend/resume을 위한 event
		bool                    m_bThreadSuspended;
		MemberCriticalSection   m_cs;                   ///< CRITICAL_SECTION의 wrapper
		DWORD                   m_dwThreadUserParam;    ///< 'm_fpPreEndThreadCallback'의 첫번재 파라미터
		PREENDTHREAD_CALLBACK   m_fpPreEndThreadCallback;   ///< thread를 종료하기 직전에 호출하는 사용자 정의 콜백 함수
		bool                    m_bForceExistThread;    ///< thread를 강제로 종료시켜야 하는 경우 이 값을 참으로 만든다.
		/// checksum 검사가 완료하지 않았지만, 사용자가 게임을 빠져나가는 경우등에 사용한다.

		bool					m_bLoadCompleted;
	};

#endif FIRST_GAME_LOADING_THREAD_TEST


class CX2StateLoading : public CX2State
{
	public:
		CX2StateLoading(void);
		virtual ~CX2StateLoading(void);

		virtual HRESULT OnFrameMove( double fTime, float fElapsedTime );
		virtual HRESULT OnFrameRender();


		void SetLoadingGageBar( int iLoadingPercent );
		void SetLoadingString( const WCHAR* msg );
		void DropItemLoading();

	protected:
		bool UIServerEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );



#ifdef NO_MORE_ERROR_TEST
		void CheckOutPossibleError();
#endif NO_MORE_ERROR_TEST

#if defined( SERV_HACKING_TOOL_LIST )
//#ifdef	ADD_SERVER_GROUP
//		bool OpenScriptServerGroupFile();
//#endif
		bool ConnectToChannelServer();
		bool Handler_ECH_VERIFY_ACCOUNT_REQ();
		bool Handler_ECH_VERIFY_ACCOUNT_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
		bool Handler_ECH_DISCONNECT_REQ();
		bool Handler_ECH_DISCONNECT_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
#endif

#ifdef SERV_SERIAL_NUMBER_AVAILABILITY_CHECK
		bool Handler_ECH_NEW_SERVER_SN_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
#endif

		//bool Handler_EGS_STATE_CHANGE_SERVER_SELECT_REQ();
		//bool Handler_EGS_STATE_CHANGE_SERVER_SELECT_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	private:
		CKTDGUIDialogType								m_pDLGLoadingFront;
		float											m_fLogoTime;

		//# KOG & Nexon 합병로고
		CKTDGParticleSystem::CParticleEventSequenceHandle 	m_hCompanyLogo;
		//# 위의 로고 배경
		CKTDGParticleSystem::CParticleEventSequenceHandle 	m_hLogoBack;
#ifndef DELIBERATIONLOGO_SKIP
		//# 심의등급 로고 
		CKTDGParticleSystem::CParticleEventSequenceHandle 	m_hDeliberationLogo;
#endif DELIBERATIONLOGO_SKIP


		//{{AFX
		//CKTDGParticleSystem::CParticleEventSequenceHandle 	m_hNexonLogo;
		//CKTDGParticleSystem::CParticleEventSequenceHandle 	m_hKOGBack;
		//CKTDGParticleSystem::CParticleEventSequenceHandle 	m_hKOGLogo;
		//CKTDGParticleSystem::CParticleEventSequenceHandle 	m_hKOGLogoMini;
		//CKTDGParticleSystem::CParticleEventSequenceHandle 	m_hNexonLogoMini;
		//CKTDGParticleSystem::CParticleEventSequenceHandle 	m_hPPoruSurprise;
		//CKTDGParticleSystem::CParticleEventSequenceHandle 	m_hPPoruLogo;

		//CKTDGXMeshPlayer::CXMeshInstance*				m_pWillium;
		//CKTDGXMeshPlayer::CXMeshInstance*				m_pElSword;
		//CKTDGXMeshPlayer::CXMeshInstance*				m_pLena;
		//CKTDGXMeshPlayer::CXMeshInstance*				m_pAisha;
		//CKTDGXMeshPlayer::CXMeshInstance*				m_pBenders;

		//CKTDXDeviceTexture*								m_pTextureTestMark;
		//}}AFX


#ifdef FIRST_GAME_LOADING_THREAD_TEST
		CX2StateLoadingThread*			m_pStateLoadingThread;
		bool							m_bSentEGS_STATE_CHANGE_SERVER_SELECT_REQ;
#endif FIRST_GAME_LOADING_THREAD_TEST

#ifdef SERV_DLL_LIST_CHECK_BEFOR_LOADING
		bool							m_bFirstVerifyAccount;
#endif

#ifdef ADD_NEW_YEAR_SOUND
		bool							m_bPlayLogoSound;
#endif

};
