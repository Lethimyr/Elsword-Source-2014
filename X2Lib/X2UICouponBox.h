#pragma once

#ifdef COUPON_SYSTEM

class CX2UICouponBox
{
public:
	enum COUPON_TAB_TYPE
	{
		CTT_AVAILABLE = 0,		/// 탭 - 사용 가능한 쿠폰
		CTT_HAVE_BEEN_USED,		/// 탭 - 쿠폰 사용 내역
		CTT_REGISTER,			/// 탭 - 쿠폰 등록
	};

	enum COUPON_BOX_COUSTOM_UI_MSG
	{
		CBCUM_COUPON_BOX_CLOSE = 600,			/// 닫기
		
		CBCUM_SELECT_COUPON_TAB_AVAILABLE,		/// 탭 - 사용 가능한 쿠폰
		CBCUM_SELECT_COUPON_TAB_HAVE_BEEN_USED,	/// 탭 - 쿠폰 사용 내역
		CBCUM_SELECT_COUPON_TAB_REGISTER,		/// 탭 - 쿠폰 등록
		
		CBCUM_SELECT_COUPON_LIST_TAB_ALL,
		CBCUM_SELECT_COUPON_LIST_TAB_FREE_CHARGE,
		CBCUM_SELECT_COUPON_LIST_TAB_DISCOUNT,

		CBCUM_PUSH_DOUBLE_LEFT_BUTTON,
		CBCUM_PUSH_SINGLE_LEFT_BUTTON,
		CBCUM_PUSH_DOUBLE_RIGHT_BUTTON,
		CBCUM_PUSH_SINGLE_RIGHT_BUTTON,

		CBCUM_BUTTON_REGISTER_COUPON,			/// 쿠폰 번호 등록 버튼
		CBCUM_BUTTON_USE_COUPON,				/// 쿠폰 사용
	};

	struct CouponInfo
	{
		//SEnum::NEXON_COUPON_BOX_TYPE m_eCouponBoxType;  // 구분
		wstring m_wstrCouponName;	// 쿠폰 명
		int		m_iCouponCardNo;	// 쿠폰 번호

		// 숫자, 날짜 등 서버에 맞추기 위해 문자열로 작업 함.
		wstring m_wstrCouponBoxType;	// 구분
		wstring m_wstrDiscountValue;	// 할인 혜택
		wstring m_wstrObtainDataTime;	// 지급 일
		wstring m_wstrExpireDataTime;	// 만료 일
		wstring m_wstrUsedDateTime;		// 사용 일

		bool	m_bIsInstantlyUse;		// 즉시 사용 가능 여부

		wstring m_CouponTargetItemDesc;	// 적용 가능 아이템 설명 툴팁
		
		CouponInfo():
		m_iCouponCardNo(-1),
		m_bIsInstantlyUse(false)
		{}

		CouponInfo( const KClientCouponBox& sKClientCouponBox_, bool bUsed_, bool bIsInstantlyUse_ )
		{
			m_iCouponCardNo = sKClientCouponBox_.m_iCouponCardNo;// 쿠폰 번호
			m_wstrCouponName = sKClientCouponBox_.m_wstrContractDetailName;// 쿠폰 명
			LimitLengthCouponName( m_wstrCouponName, bUsed_ );
			m_wstrObtainDataTime = sKClientCouponBox_.m_wstrObtainDateTime; // 지급일
			if( 10 < m_wstrObtainDataTime.size() )
			{
				m_wstrObtainDataTime.erase( wstring::size_type(10));
			}

			if( true == bUsed_ )// 쿠폰 사용 내역
			{
				m_wstrUsedDateTime = sKClientCouponBox_.m_wstrUsedDateTime; // 사용 일
				if( 10 < m_wstrUsedDateTime.size() )
				{
					// 날짜 이하 자르기
					m_wstrUsedDateTime.erase( wstring::size_type(10));
				}
			}
			else// 사용 가능 쿠폰
			{
				m_wstrDiscountValue = sKClientCouponBox_.m_wstrDiscountValue;//할인 혜택
				m_wstrExpireDataTime = sKClientCouponBox_.m_wstrExpireDateTime; // 사용 만료 일
				if( 10 < m_wstrExpireDataTime.size() )
				{
					// 날짜 이하 자르기
					m_wstrExpireDataTime.erase( wstring::size_type(10));
				}
			}

			// 무료 / 할인 구분 
			m_bIsInstantlyUse = bIsInstantlyUse_;
			if( true == m_bIsInstantlyUse ) // 즉시 사용 가능할 경우 구분 : 무료
				m_wstrCouponBoxType = GET_STRING( STR_ID_27302 );
			else
				m_wstrCouponBoxType = GET_STRING( STR_ID_27303 );
		}


		void LimitLengthCouponName( OUT wstring& wstrCouponName_, bool bUsed_ );

	};

public:
	CX2UICouponBox();
	~CX2UICouponBox();

	void SetOpen( const bool bShow_ );
	HRESULT OnFrameMove(double fTime, float fElapsedTime) {}
	bool MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam ) {}

	bool UICustomEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	bool UIServerEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	void SetNowState( CX2State* pState ) {}

	CX2UICouponBox::COUPON_TAB_TYPE GetCouponTabType() const { return m_eCouponTabType; }
	bool SetCouponTabType(CX2UICouponBox::COUPON_TAB_TYPE val);

	SEnum::NEXON_COUPON_BOX_TYPE GetCouponBoxType() const { return m_eCouponBoxType; }
	bool SetCouponBoxType(SEnum::NEXON_COUPON_BOX_TYPE val);

	// 쿠폰 목록 갱신
	void Handler_COUPON_LIST_REQ();
	bool Handler_COUPON_LIST_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	// 쿠폰 등록
	void Handler_REGISTER_COUPON_REQ( const WCHAR* wszCouponNumber );
	bool Handler_REGISTER_COUPON_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	// 쿠폰 사용
	void Handler_USE_COUPON_REQ( const UINT uiColumn );
	bool Handler_USE_COUPON_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

protected:
	void Initialize();
	void RegisterLuaBind();

	void UpdateUI();
	void SetShowStaticForm( const bool bShowAbleToUse_, const bool bShowHaveBeenUsed_, 
		const bool bShowRegister_, const bool bShowFrameAvalable_, const bool bShowFrame_HaveBeenUsed_, const bool bPageNum_ );

	void SetShowButtonForm( const bool bShowArrow_, const bool bShowRegister_, const bool bPageNum_, const bool bDesc_ );
	void SetShowRadioButtonForm( const bool bShowListType_ );
	void SetEnableImeEditBox( const bool bEnable_ );
	void SetShowAndCouponNum(  const bool bShow_, const UINT uiCouponNum_ = 0 ); // 쿠폰 수량

	void RegisterCoupon();// 쿠폰 번호 등록 버튼
	void SelectPageButton( UINT uiPageNum_, UINT uiMaxPageNum_ );// 페이지 선택
	void UpdateCouponInfo(); // 쿠폰 목록 갱신	
	void SetCouponInfo(const CouponInfo& sCouponInfo_, const UINT uiColumn_ );

	void SetNowPageIndex( USHORT uiPageIndex_ );
private:
	CKTDGUIDialogType			m_pDlgCouponBox;
	COUPON_TAB_TYPE				m_eCouponTabType; // 현재 선택 된 UI 탭 타입

	// 쿠폰 박스 타입 : 전체/아이템쿠폰/할인 쿠폰 분류 
	SEnum::NEXON_COUPON_BOX_TYPE m_eCouponBoxType;	// 쿠폰 박스 타입

	USHORT						m_uiNowPageIndex;	// 시작 : 0
	USHORT						m_uiMaxPage;		// 시작 : 0

	const static BYTE			byDoublePageDistance;
	const static UINT			uiCouponNameAbailableLimit;
	const static UINT			uiCouponNameHasBeenUsedLimit;

	vector<CouponInfo>			m_vecCouponInfoList;  // 쿠폰 목록 
	
};
#endif // COUPON_SYSTEM