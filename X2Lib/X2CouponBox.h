#pragma once

#ifndef COUPON_SYSTEM // 이전 UI 제거
#include "X2ItemSlotManager.h"

class CX2CouponBox : public CX2ItemSlotManager
{
public:
	enum COUPON_BOX_CUSTOM_UI_MSG
	{
		CBCUM_ACCEPT0 = 600,
		CBCUM_ACCEPT1,
		CBCUM_ACCEPT2,
		CBCUM_ACCEPT3,
		CBCUM_ACCEPT4,
		CBCUM_ACCEPT5,

		CBCUM_CLOSE_COUPON_BOX,
		CBCUM_PREV_PAGE,
		CBCUM_NEXT_PAGE,

		CBCUM_CLOSE_COUPON_INPUT_BOX,
		CBCUM_USE_COUPON,
	};

public:
	CX2CouponBox( CKTDXStage* pNowState );
	~CX2CouponBox();

	virtual HRESULT OnFrameMove(double fTime, float fElapsedTime);
	virtual void RegisterLuaBind();

	virtual bool MouseDown( D3DXVECTOR2 mousePos );
	virtual bool MouseUp( D3DXVECTOR2 mousePos );	


	bool UICustomEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	bool UIServerEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	bool Handler_EGS_APPLY_COUPON_REQ( const WCHAR* wszCouponNumber );
	bool Handler_EGS_APPLY_COUPON_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
		
	bool Handler_EGS_PURCHASED_CASH_ITEM_LIST_REQ( int iPageIndex, int nItemPerPage );
	bool Handler_EGS_PURCHASED_CASH_ITEM_LIST_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	bool Handler_EGS_GET_PURCHASED_CASH_ITEM_REQ( uLong ulOrderNo, uLong ulProductNo, int iQuantity );
	bool Handler_EGS_GET_PURCHASED_CASH_ITEM_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );


	void SetNowState( CX2State* pState );
	void OpenCouponBoxDLG( bool bOpen = true );	
	void UpdateCouponBoxDLG( int iPageIndex, bool bClearOnly = false );
	
	void OpenCouponNumberInputDLG( bool bOpen = true );
	bool GetCheckOperationCondition();

	
	
private:
	static const int s_nItemPerPage = 6;

	bool			m_bOpen;

	CKTDGUIDialogType m_pDLGBack;
	vector< CKTDGUIDialogType > m_vecpDLGSlot;
	vector< KNXBTOrderInfo > m_vecItemList;


	CKTDGUIDialogType m_pDLGCouponNumberInput;


	int m_iReqPageIndex;
	int m_iNowPageIndex;
	int m_iMaxPageIndex;

};
#endif // COUPON_SYSTEM




