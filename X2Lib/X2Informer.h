#pragma once

class CX2Informer
{

	public:

		enum X2_INFORMER_TYPE
		{
			XIT_SKILL = 0,
			XIT_QUEST,
			XIT_INVEN,
			XIT_CASH_SHOP,
			XIT_GUILD_EXP,
		};

		enum X2_UID_USED_IN_INFORMER
		{
			XUUII_ENOUGH_INVEN = 1,
			XUUII_NEW_ITEM,
			XUUII_GUILD_EXP,
			XUUII_NEW_QUICK_SLOT_ITEM,
//#ifdef CASHSHOP_CLASS_CHANGE_NOTIFICATION
			XUUII_CLASS_CHANGE,
			XUUII_PRESENT,
//#endif CASHSHOP_CLASS_CHANGE_NOTIFICATION
		};

	public:

		CX2Informer(void);
		virtual ~CX2Informer(void);

		X2_INFORMER_TYPE GetType() { return m_InformerType; }

		virtual bool MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam ); //토크박스 찍었을 시 범위 확인 후 무효화

		virtual HRESULT OnFrameMove( double fTime, float fElapsedTime );

		virtual void Reset(); // 캐릭터 선택하고 처음 빌리지 마을 들어갈 때 체크하는 함수
								// 현재 스킬 정보라던지 현재 완료한 퀘스트 정보 저장
								
																		//스테이트 체인지 할 때 마다 체크해둘 리스트
		virtual void CheckInfo( bool bStateChange );		//바뀐 부분이 있다면 현재 정보 업데이트
																		//그 이전 스테이트에서 알림창이 활성화 되있었다면 무효화
									
		virtual void InvalidInfo();
		virtual void ClearAllInfo() 
		{
			if ( m_pTalkBoxManager != NULL )
				m_pTalkBoxManager->Clear();
		}
		void SetTalkBoxOffsetPos(D3DXVECTOR2 vPos){m_vTalkBoxOffsetPos=vPos;}

		bool GetNeedCheckInfo() const { return m_bNeedCheckInfo; }
		void SetNeedCheckInfo(bool val) { m_bNeedCheckInfo = val; }

	protected:	

		virtual void SetNowInfo() = 0;
		virtual bool IsNowInfoChange() = 0;
		virtual void SetShow( bool bShow );

		X2_INFORMER_TYPE	m_InformerType;

		CX2TalkBoxManager*	m_pTalkBoxManager;
		D3DXVECTOR2			m_vTalkBoxPos;

		bool		m_bCheckInfo;	//한번 알렸는지 안알렸는지

		bool		m_bMouseDown;

#ifdef EVENT_SCENE
		bool		m_bSetShowAfterEventSceneEnd;
#endif EVENT_SCENE

		CKTDXDeviceSound*	m_pSoundAlarm;
		//캐릭터 상태에 따라 메뉴 버튼의 위치가 달라 지기 때문에 위치 변수 추가
		D3DXVECTOR2			m_vTalkBoxOffsetPos;

		bool m_bNeedCheckInfo;		/// 체크할 필요가 있는지 여부
};
