#pragma once
#ifdef SERV_EVENT_BINGO
class CX2UIBingo : public CX2ItemSlotManager
{
public:

	enum UI_BINGO_CUSTOM_MSG
	{
		UBCM_BUTTON_EXPAND	= 52000,
		UBCM_BUTTON_MINIMIZE,
		UBCM_OPEN,
		UBCM_CLOSE,
		UBCM_INITIALIZE,
		UBCM_VALUE_MIX,
		UBCM_LOTTERY,
		UBCM_REWARD_POPUP_OK,
	};
public:
	CX2UIBingo( CKTDXStage* pNowStage );
	~CX2UIBingo(void);

	// 멤버 함수
	virtual HRESULT		OnFrameMove( double fTime, float fElapsedTime );
	virtual bool		MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	virtual bool		UICustomEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	virtual bool		UIServerEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	virtual bool		GetCheckOperationCondition(){return true;}//modal 상태에 상관없이 툴팁을 보여주기 위해 설정

	bool Handler_EGS_BINGO_EVENT_INFO_REQ();
	bool Handler_EGS_BINGO_EVENT_INFO_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	bool Handler_EGS_BINGO_EVENT_LOTTERY_REQ();//빙고뽑기
	bool Handler_EGS_BINGO_EVENT_LOTTERY_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	bool Handler_EGS_BINGO_EVENT_MIX_REQ(); //뒤섞기
	bool Handler_EGS_BINGO_EVENT_MIX_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	bool Handler_EGS_BINGO_EVENT_RESET_REQ();//아이템 이용한 초기화
	bool Handler_EGS_BINGO_EVENT_RESET_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	bool Handler_EGS_BINGO_EVENT_INIT_REQ();//모든 빙고 완성 후 초기화
	bool Handler_EGS_BINGO_EVENT_INIT_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );


	//출력 상태 변경
	void SetOpen(bool bVal);										//빙고 UI Show
	void SetShowEventButton(bool bVal, bool bShowEffect = false);	//빙고 오픈 UI SHOW
	void SetExpandEventButton(bool bVal);							//빙고 오픈 UI 확장상태 변경

private:
	//기능 버튼
	//UI 생성/갱신
	bool CreateBingoUI();											//빙고 UI 생성
	void UpdateBingoNumber(int iNumberIndex = -1);					//빙고 숫자판 갱신
	void UpdatePresent( int iPresentIndex = -1);						//빙고 상품 갱신
	void UpdateInfoData(bool bUpdateFunctionButton_ = false);		//빙고관련 수량 정보 갱신
	void UpdateButtonUI(bool bUpdateFunctionButton_ = false);		//빙고 UI 버튼 활성화 여부 갱신

	void NoticePresentGain(bool bIsAllComplete_ = false);			//상품 획득 알림 팝업창
	void NoticeBingoResult(bool bShow_, bool bIsSucces_ = false);	//빙고 성공,실패 여부 알림
	
	void UpdateCompleteLine();										//현재 획득 라인 수 체크

	bool SetBingoNum( byte BingoNum_ );								//1개 숫자 설정(획득여부포함)
	void SetBingoNumList(const vector<byte>& vecBingoNum_);			//전체 빙고판 설정(획득여부포함)

	void SetAcquiredPresentList(const vector<byte>& vecAcuiredPresentIndex_);		//획득 상품 리스트
	void SetPresentList(const std::vector< std::pair<int, bool> >& vecPresentInfo_);//상품 정보 설정(획득 여부 포함)
	void SetPresentList(const std::vector< int >& vecPresentInfo_);					//상품 정보 설정(무조건 미획득)
	
	//텍스쳐 변경
	void SetPresentTex(int iIndex);									//상품 텍스쳐 변경
	void SetNumberTex(int iIndex, bool bShowEffect = false);		//빙고판 숫자 텍스쳐 변경
	void GetNumberTextrueAndKeyName( OUT wstring& NumberTextureName_, OUT wstring& NumberKeyName_, 
		IN int iBingoValue_, IN bool bIsGain_) const;				//빙고판 숫자 획듟아태에 따른 텍스쳐명 얻기

	//빙고 숫자 획득여부 관련
	inline byte	GetBingoNum(int iIndex) const;						//획득 여부를 표현하는 +100 값은 제외
	inline bool	GetIsGainNum(int iIndex) const;						//인덱스로 획득상태로 변경
	inline int GetCountOfCheckedNumber() const;						/// 체크된 숫자의 갯수 얻어오기

	inline byte GetNumExceptGainState(byte ByteNum_) const;			//획득 여부 제외한 실제 숫자 값 얻기

	const int GetBingoIndexByNum(byte byteNum_) const;				//숫자를 통해 빙고판위의 인덱스 얻기

	//UI관련
	CKTDGUIDialogType	m_pDLGEventOpenButton;						//우측하단 오픈 버튼 다이얼로그
	CKTDGUIDialogType	m_pDLGBingo;								//빙고 다이얼로그
	CKTDGUIDialogType	m_pDLGRewardPopup;							//보상 알림 팝업

	CKTDGParticleSystem::CParticleEventSequenceHandle m_hBingoOpenButton; //빙고 오픈 버튼 파티클
	//데이타 관련
	static const int	m_iMaxNumPresent = 12;						//상품 최대 수량
	static const int	m_iMaxNumBingoValue = 25;					//빙고숫자 최대 수량
	
	int					m_iLotteryChange;							//빙고 기회 
	int					m_iLotteryRecord;							//사용한 빙고 기회

	int					m_iCompleteLine;							//완성 빙고 라인 수
	int					m_iUncompleteLine;							//남은 빙고 라인 수

	int					m_iCheckedNumCnt;							//체크된 숫자 개수
	int					m_iResetChance;								//특수기능(초기화) 개수
	int					m_iMixChance;								//특수기능(뒤섞기) 개수

	int					m_iPreAcuireNumber;							//최근에 빙고뽑기로 획득한 숫자


	std::pair<int, bool> m_arrayPresentData[m_iMaxNumPresent];		//<아이템ID, 상품획득여부>상품 정보
	byte				 m_arrayBingoValue[m_iMaxNumBingoValue];	//빙고 숫자						

	bool				m_bShowBingoUI;								//빙고판 UI Show상태
	bool				m_bIsExpandButton;							//이벤트 버튼의 확장 상태

};
#endif //SERV_EVENT_BINGO