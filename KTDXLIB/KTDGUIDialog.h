#pragma once
//{{ seojt // 2009-8-19, 15:42
#include "KTDGUIDialogHandle.h"
//}} seojt // 2009-8-19, 15:42


class   CKTDGUIDialogManager;
class   CKTDGUIDialog;
class   CKTDGUIControl;
typedef std::list<CKTDGUIDialogType>   CKTDGUIDialogList; // moved to end of this file by jintaeks on 2009-08-19, 15:20
typedef std::set<CKTDGUIDialogType>    CKTDGUIDialogSet; // moved to end of this file by jintaeks on 2009-08-19, 15:20
typedef std::map<std::wstring, CKTDGUIDialogType>    CKTDGUIDialogMap; // moved to end of this file by jintaeks on 2009-08-19, 15:20



//class   CKTDGUIDialog : private boost::noncopyable
//{{ seojt // 2009-8-18, 17:53, qff
class CKTDGUIDialog
    : private boost::noncopyable
#ifdef DIALOG_HANDLE_TEST // 2009-8-19 by jintaeks
    , public KJohnObjMan<CKTDGUIDialog>
#endif // DIALOG_HANDLE_TEST // 2009-8-19
//}} seojt // 2009-8-18, 17:53
{
	public:
		struct DialogMoveItem
		{
			D3DXVECTOR2 pos;
			D3DXCOLOR	color;
			float		fChangeTime;
			bool		bIsAutoDelete;

			DialogMoveItem()
			{
				INIT_VECTOR2(pos, 0, 0);
				color = D3DXCOLOR(0xffffffff);
				fChangeTime = 0.0f;
				bIsAutoDelete = false;
			}

			DialogMoveItem& DialogMoveItem::operator=(const DialogMoveItem& rhs) 
			{ 
				if ( this == &rhs ) 
				{ 
					return *this; 
				}
				
				pos = rhs.pos;
				color = rhs.color;
				fChangeTime = rhs.fChangeTime;
				bIsAutoDelete = rhs.bIsAutoDelete;

				return *this; 
			} 
		};


	public:
		CKTDGUIDialog( CKTDXStage* pStage, const WCHAR* wstrScriptFileName = NULL, float fMoveTime = 0.07f, int iLayer = 0 );
		virtual	~CKTDGUIDialog(void);

#ifdef BOOST_SINGLETON_POOL_TEST
        /** UI dialog를 handle로 사용하도록 하는 작업을 진행하면서,
            operator new의 overloading에 문제가 있어서 서진택이 주석처리하다.
            - jintaeks on 2009-08-20, 11:03 */
		//void* operator new(size_t s);
		//void operator delete(void *p);
#endif BOOST_SINGLETON_POOL_TEST

		bool	OpenScriptFile( const WCHAR* pFilename );

		HRESULT	OnFrameMove( double fTime, float fElapsedTime );
		HRESULT	OnFrameRender( bool bForceDraw = false );
		HRESULT	OnResetDevice();
		HRESULT	OnLostDevice();
		bool	MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

		CKTDGUIControl::CPictureData* CreatePicture()
		{ 
			CKTDGUIControl::CPictureData* pPicture = new CKTDGUIControl::CPictureData();
			return pPicture;
		}

#if defined(REFORM_ENTRY_POINT) && defined( MOVIE_TEST_BASE ) || defined( MOVIE_TEST ) 
// #ifdef MOVIE_TEST	 	// 13-11-11, 진입 구조 개편, kimjh, MOVIE_TEST 중 사용에 필요한 Define 을 MOVIE_TEST_BASE 로 변경
		CKTDGUIControl::CMovieData* CreateMovie()
		{ 
			CKTDGUIControl::CMovieData* pMovieData = new CKTDGUIControl::CMovieData();
			return pMovieData;
		}
#endif // defined(REFORM_ENTRY_POINT) && defined( MOVIE_TEST_BASE ) || defined( MOVIE_TEST ) 
// #endif //  MOVIE_TEST	// 13-11-11, 진입 구조 개편, kimjh, MOVIE_TEST 중 사용에 필요한 Define 을 MOVIE_TEST_BASE 로 변경





		CKTDGUIButton*		CreateButton()		{ CKTDGUIButton*		pControl; pControl = new CKTDGUIButton();		return pControl; }
		CKTDGUICheckBox*	CreateCheckBox()	{ CKTDGUICheckBox*		pControl; pControl = new CKTDGUICheckBox();		return pControl; }
		CKTDGUIEditBox*		CreateEditBox()		{ CKTDGUIEditBox*		pControl; pControl = new CKTDGUIEditBox();		return pControl; }
		CKTDGUIIMEEditBox*	CreateIMEEditBox()	{ CKTDGUIIMEEditBox*	pControl; pControl = new CKTDGUIIMEEditBox();	return pControl; }
		CKTDGUIStatic*		CreateStatic()		{ CKTDGUIStatic*		pControl; pControl = new CKTDGUIStatic();		return pControl; }
		CKTDGUIRadioButton*	CreateRadioButton()	{ CKTDGUIRadioButton*	pControl; pControl = new CKTDGUIRadioButton();	return pControl; }
		CKTDGUIScrollBar*	CreateScrollBar()	{ CKTDGUIScrollBar*		pControl; pControl = new CKTDGUIScrollBar();	return pControl; }
		CKTDGUIComboBox*	CreateComboBox()	{ CKTDGUIComboBox*		pControl; pControl = new CKTDGUIComboBox();		return pControl; }
		CKTDGUIListBox*		CreateListBox()		{ CKTDGUIListBox*		pControl; pControl = new CKTDGUIListBox();		return pControl; }
		CKTDGUISlider*		CreateSlider()		{ CKTDGUISlider*		pControl; pControl = new CKTDGUISlider();		return pControl; }
		CKTDGUINavigation*	CreateNavigation()	{ CKTDGUINavigation*	pControl; pControl = new CKTDGUINavigation();	return pControl; }
#ifdef NEW_SKILL_TREE_UI
		CKTDGUIContolList*	CreateControlList() { CKTDGUIContolList*	pControl; pControl = new CKTDGUIContolList();	return pControl; }
		CKTDGUISlot*		CreateSlot()		{ CKTDGUISlot*			pControl; pControl = new CKTDGUISlot();			return pControl; }
#endif

		CKTDGUIItem*		CreateItem()		{ CKTDGUIItem*	pControl; pControl = new CKTDGUIItem(); return pControl; }

		void			AddControl( CKTDGUIControl* pControl );
#ifdef DLL_BUILD
		void			AddControl_Front( CKTDGUIControl* pControl );
#endif
		bool			DeleteControl( const WCHAR* pControlName );

		bool			ChangeSequence( CKTDGUIControl* pControlToChange, bool bFront ); // 가장 앞으로 옮길것이냐 가장 뒤로 옮길것이냐?
		void			ChangeSequenceByName_LUA( const char* pControlName, bool bFront );
		void			ChangeSequence( CKTDGUIControl* pControlToChange, CKTDGUIControl* pPivotControl, bool bFront );
		void			ChangeSequenceByNameAndPivot_LUA( const char* pControlNameToChange, const char* pPivotControlName, bool bFront );

		bool			CheckControl( CKTDGUIControl* pControl );
		bool			CheckControl( const WCHAR* wszControlName );

		//CKTDGUIControl*	GetControl_LUA( const char* pControlName );

		const CKTDGUIControl*   _GetControl( const WCHAR* pControlName ) const;
		CKTDGUIControl*	        GetControl( const WCHAR* pControlName );
		CKTDGUIControl*	        GetControl( const int index );
		//{{ seojt // 2009-8-19, 17:17
		const CKTDGUIControl*   GetControl( const WCHAR* pControlName ) const;
		const CKTDGUIControl*   GetControl( const int index ) const;
		//}} seojt // 2009-8-19, 17:17

		int				GetControlSize() { return (int)m_ControlsList.size(); }

		vector<CKTDGUIControl*>& GetControlList() { return m_ControlsList; }

		bool			ReNameControl( CKTDGUIControl* pControl );

		void					RequestFocus( CKTDGUIControl* pControl );
		static void				ClearFocus();
		static CKTDGUIControl*	GetControlFocus(){ return s_pControlFocus; }

#ifdef SET_SHOW_CONTROLS_WITH_DUMMYINT
		void			SetShowEnableControlsWithDummyInt( IN int iIndex, IN int iValue, IN bool bShow, IN bool bEnable );
#endif SET_SHOW_CONTROLS_WITH_DUMMYINT
		CKTDXStage*	GetStage()						{ return m_pStage; }
		void		SetStage( CKTDXStage* pStage ) { m_pStage = pStage; }

		void			SetName_LUA( const char* pName );
		void			SetName( const WCHAR* pName );	
		
		const WCHAR*	GetName() const         { return m_Name.c_str(); }

		void		SetPos( const D3DXVECTOR2& pos );
		void		SetPos_LUA( float x, float y )	{ SetPos( D3DXVECTOR2(x,y) ); }

		D3DXVECTOR2 GetPos() const              { return m_Pos; }
		D3DXCOLOR	GetColor() const            { return m_Color; }
		void		SetColor( D3DXCOLOR color ) { m_Color = color; }

		void		SetModal( bool bModal )		{ m_bModal = bModal; }
		bool		GetModal() const            { return m_bModal; }

		void		SetEnable( bool bEnable );
		bool		GetEnable() const           { return m_bEnable; }
		void		SetShow( bool bShow );
		bool		GetShow() const             { return m_bShow; }

		void		SetShowEnable( bool bShow, bool bEnable ) { SetShow( bShow ); SetEnable( bEnable ); }

//{{ robobeg : 2009-01-07
		//void		UpdateDockPos();
//}} robobeg : 2009-01-07
		void		UpdateRects(); 

		bool		GetFront()					{ return m_bFront; }
		void		SetFront( bool bFront );

		bool		GetIsAutoDeleteEnd()		{ return m_bCheckAutoDeleteEnd; }

		void		Move( D3DXVECTOR2 pos, D3DXCOLOR color, float fChangeTime, bool bDirect = false, 	
						bool bAutoDelete = false,  bool bPingPong = false, int repeatNum = 1
#ifdef MOVE_FADE_IN_OUT
						, bool bFadeInOut = true 
#endif MOVE_FADE_IN_OUT
						);

		void		Move_LUA();
		void		MoveStop();

		void		SetEnableMoveByDrag_LUA( bool bCheck ) { m_bEnableMoveByDrag = bCheck; }
		bool		GetEnableMoveByDrag() { return m_bEnableMoveByDrag; }

		void		AddDummyPos( D3DXVECTOR3 pos ) { m_DummyPosList.push_back( pos ); }
		__forceinline D3DXVECTOR3 GetDummyPos( int index )
		{
			if ( index < 0 || index >= (int)m_DummyPosList.size() )
				return D3DXVECTOR3( 0, 0, 0 );

			return m_DummyPosList[index]; 
		}

		void		AddDummyString_LUA( int iDummyString );
		wstring		GetDummyString( int index ) { return m_DummyStringList[index]; }

		void		AddDummyInt( int dummy ) { m_DummyIntList.push_back(dummy); }
		__forceinline int GetDummyInt( int index )
		{
			if ( index < 0 || index >= (int)m_DummyIntList.size() )
				return 0;

			return m_DummyIntList[index]; 
		}
		void		ClearDummyInt() { m_DummyIntList.clear(); }

		

		CKTDGUIButton*		GetButton_LUA( const char* pControlName );
		CKTDGUICheckBox*	GetCheckBox_LUA( const char* pControlName );
		CKTDGUIComboBox*	GetComboBox_LUA( const char* pControlName );
		CKTDGUIEditBox*		GetEditBox_LUA( const char* pControlName );
		CKTDGUIIMEEditBox*	GetIMEEdieBox_LUA( const char* pControlName );
		CKTDGUIListBox*		GetListBox_LUA( const char* pControlName );
		CKTDGUIRadioButton*	GetRadioButton_LUA( const char* pControlName );
		CKTDGUIStatic*		GetStatic_LUA( const char* pControlName );
#ifdef NEW_SKILL_TREE_UI
		CKTDGUISlot*		GetSlot_LUA( const char* pControlName );
		CKTDGUISlot*		GetSlotByID(int ID);		// ID는 사용자가 설정해주는 것으로, 겹칠 수 있으므로 주의해야 한다.
#endif

		void			OnMouseLeave();
		bool			GetIsMoving();
		bool			GetIsMouseOver() const { return m_bIsMouseOver; }

		bool			GetIsSuccessInitOpenScript() { return m_bSuccessInitOpenScript; }

		int				GetLayer() { return m_iLayer; }
		void			SetLayer( int iLayer );

		void 			SetID(int id) { m_ID = id; } 
		int				GetID() { return m_ID; }

		CKTDGUIRadioButton::RadioButtonGroupInfo* GetRadioButtonGroupInfo( int iGroupID )
		{
			map< int, CKTDGUIRadioButton::RadioButtonGroupInfo* >::iterator it;
			it = m_mapRadioButtonGroupInfo.find( iGroupID );
			if( it != m_mapRadioButtonGroupInfo.end() )
				return (CKTDGUIRadioButton::RadioButtonGroupInfo*) it->second;
			else
				return NULL;
		}

		void AddRadioButtonGroupInfo( CKTDGUIRadioButton::RadioButtonGroupInfo* pInfo )
		{
			m_mapRadioButtonGroupInfo[pInfo->m_iGroupID] = pInfo;
		}

		void SetMouseDownForDrag( bool bCheck ) { m_bMouseDownForDrag = bCheck; }
		bool GetMouseDownForDrag() const { return m_bMouseDownForDrag; }

//{{ robobeg : 2009-01-07
		// Docking
		//int GetDockDialogKeyID() { return m_iDockDialogKeyID; }
		//void SetDockDialogKeyID( int val ) { m_iDockDialogKeyID = val; }
//}} robobeg : 2009-01-07

//{{ robobeg : 2009-01-07
		//int GetKeyID() { return m_KeyID; }
//}} robobeg : 2009-01-07

		// Minimize / Maximize
		bool GetMaximize() { return m_bMaximize; }
		void SetMaximize( bool val );

		// Close On Focus Out
		bool GetCloseOnFocusOut() { return m_bCloseOnFocusOut; }
		void SetCloseOnFocusOut( bool val ) { m_bCloseOnFocusOut = val; }
		void SetCloseOnFocusOutEvent( int eventID ) { m_CustomMsgFocusOutEventID = eventID; }

		void SetCloseCustomUIEventID( int eventID ) { m_CloseCustomUIEventID = eventID; }
		int GetCloseCustomUIEventID() { return m_CloseCustomUIEventID; }
		void SendCloseCustomUIEventID();

		// dialog dragging 범위를 결정하는데 사용된다
		D3DXVECTOR2 GetSize() const { return m_vSize; }
		void SetSize( float fWidth, float fHeight ) { m_vSize = D3DXVECTOR2( fWidth, fHeight ); }
		
		
		bool GetEnableDraggingRange() const { return m_bEnableDraggingRange; }
		void SetEnableDraggingRange(bool val) { m_bEnableDraggingRange = val; }
		D3DXVECTOR2 GetDraggingRangeCenter() const { return m_vDraggingRangeCenter; }
		void SetDraggingRangeCenter( float x, float y ) { m_vDraggingRangeCenter = D3DXVECTOR2(x, y ); }
		D3DXVECTOR2 GetDraggingRange() const { return m_vDraggingRange; }
		void SetDraggingRange( float fWidth, float fHeight ) { m_vDraggingRange = D3DXVECTOR2(fWidth, fHeight); }

		void ShowGuideDesc( CKTDGUIControl* pControl );
		CKTDGUIControl* GetGuideControl() { return m_pStaticGuideDesc; }

		void SetOKAndCancelMsgBox( bool bCheck ) { m_bOKAndCancelMsgBox = bCheck; }
		bool GetOKAndCancelMsgBox() { return m_bOKAndCancelMsgBox; }

		void ClearRadioButtonGroup( int iButtonGroup );


		void SetCustomMsgMouseOut( int msg ) { m_CustomMsgMouseOut = msg; }
		void SetCustomMsgMouseOver( int msg ){ m_CustomMsgMouseOver = msg; }
		void SetCustomMsgStopDragging(int val) { m_CustomMsgStopDragging = val; }
		void SetCustomMsgStartDragging(int val) { m_CustomMsgStartDragging = val; }
		void SetCustomMsgRMouseUp(int msg) { m_CustomMsgRMouseUp = msg; }
		

//{{ robobeg : 2009-01-07
		//int GetUniqueIndex() const { return m_iUniqueIndex; }
		//void SetUniqueIndex(int val) { m_iUniqueIndex = val; }
//}} robobeg : 2009-01-07

        bool GetHasSlot() { return m_bHasSlot; }
        void SetHasSlot(bool val) { m_bHasSlot = val; }
		bool GetHasUnit() { return m_bHasUnit; }
		void SetHasUnit(CKTDGObject *pUnit);

		void ScaleControls( float fx, float fy );
		void MoveControls( float fx, float fy );

        CKTDGUIDialogManager*   GetUIDialogManager() { return m_pkUIDialogManager; }

        class   CKTDGUIDialogManagerAccess
        {
            friend class    CKTDGUIDialogManager;
            static void SetUIDialogManager( CKTDGUIDialogType kInDialog_, CKTDGUIDialogManager* pkInManager_ )
                {
                    if( kInDialog_ != NULL )
                    {
                        kInDialog_->m_pkUIDialogManager = pkInManager_;
                    }//if
                }
//{{ robobeg : 2009-01-07
            //static void SetKeyID( CKTDGUIDialog& kInDialog_, int val )      { kInDialog_.m_KeyID = val; }
//}} robobeg : 2009-01-07
            static void SetLayer( CKTDGUIDialogType kInDialog_, int iLayer )   
            { 
                if( kInDialog_ != NULL )
                {
                    kInDialog_->m_iLayer = iLayer; 
                }//if
            }
            static void SetFront( CKTDGUIDialogType kInDialog_, bool bInFront_ )
            {
                if( kInDialog_ != NULL )
                {
                    kInDialog_->m_bFront = bInFront_;
                }//if
            }
        };//class   CKTDGUIDialogManagerAccess

		bool GetIsOverControl();
		void SetIsOverControl(bool val);
		bool GetDisableUnderWindow () { return m_bDisableUnderWindow; }
		void SetDisableUnderWindow (bool val) { m_bDisableUnderWindow = val; }		

		void SetMouseOverShowDlg(bool val) { m_bMouseOverShowDlg = val; }


		CKTDGUIDialog* Get() { return this; }

		int GetDialogListChangeCount() const { return m_iDialogListChangeCount; }
		void SetDialogListChangeCount(const int val) { m_iDialogListChangeCount = val; }


		float GetTimeForSelfDelete() const { return m_fTimeForSelfDelete; }
		void SetTimeForSelfDelete( float val) { m_fTimeForSelfDelete = val; }

#ifdef PRIORITY_EVERY_UI
		CKTDGUIControl* GetControlMouseOver() const;
#endif PRIORITY_EVERY_UI

#ifdef PARTICLE_RENDER_BY_DIALOG
		bool GetHasParticle() { return m_bHasParticle; }
		void AddParticle(CKTDGObject *pObj);
		void ClearParticleList();
#endif //PARTICLE_RENDER_BY_DIALOG

#ifdef REFORM_ENTRY_POINT	 	// 13-11-11, 진입 구조 개편, kimjh
		void SetCustomButtonMouseOverSndFile_LUA( const char* pCustomSndFileName );
		void SetCustomButtonMouseUpSndFile_LUA( const char* pCustomSndFileName );
		void SetDownStateToAllButtonType ();
#endif // REFORM_ENTRY_POINT	// 13-11-11, 진입 구조 개편, kimjh


	protected:

		CKTDGUIControl* GetControlAtPoint( POINT pt );
		CKTDGUIControl* GetControlAtPointForStatic( POINT pt );
		CKTDGUIControl*	GetControlAtPointExceptStatic( POINT pt );
		CKTDGUIControl* GetControlAtPointAbsolute( POINT pt );

	
		void			OnMouseMove( POINT pt );
		bool			HandleMouse( UINT uMsg, POINT pt, WPARAM wParam, LPARAM lParam );	//포커스 잡힌 컨트롤부터 처리해주고 처리 안되면 다른 컨트롤들 순서대로 처리해주다 걸리면 return;

		void			CreateGuideDesc();

		bool			SendInternelEvent( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	protected:
	
		wstring						m_Name;
		//string						m_Name_MB;

		D3DXVECTOR2					m_Pos;
		D3DXCOLOR					m_Color;
		float						m_fAnimateTime;

		D3DXVECTOR2					m_GoalPos;
		D3DXCOLOR					m_GoalColor;
		float						m_fChangeTime;

		CKTDXStage*					m_pStage;
		bool						m_bModal;
		bool						m_bEnable;
		bool						m_bShow;
		bool						m_bMaximize;
		bool						m_bCloseOnFocusOut;
		int							m_CloseCustomUIEventID;

//{{ robobeg : 2009-01-07
		//int							m_iDockDialogKeyID;		
//}} robobeg : 2009-01-07
		
		bool						m_bAutoDelete;
		bool						m_bCheckAutoDeleteEnd;

		bool						m_bEnableMoveByDrag;
		bool						m_bMouseDownForDrag;
		CKTDGUIControl*				m_pControlMouseOver;
		CKTDGUIControl*				m_pControlMouseOverShowDlg;
		static CKTDGUIControl*		s_pControlFocus;

#ifdef PRIORITY_EVERY_UI
		CKTDGUIControl*				m_pControlMouseOverStatic;
#endif PRIORITY_EVERY_UI

		vector<CKTDGUIControl*>								m_ControlsList;

		map<std::wstring, CKTDGUIControl*>					m_ControlsMap;
		//map<std::string, CKTDGUIControl*>					m_ControlsMap_MB;

		std::queue< CKTDGUIDialog::DialogMoveItem >	m_queMoveItems;
		
		vector< D3DXVECTOR3 >		m_DummyPosList;
		vector< wstring	>			m_DummyStringList;
		vector< int >				m_DummyIntList;
		
		bool						m_bSuccessInitOpenScript;
		
		int							m_ID;				// 같은 dialog를 여러개 한번에 만들때 구분하기 위한 ID, 보통 그냥 index 

//{{ robobeg : 2009-01-07
		//int							m_KeyID;			// 다이얼로그 유니크한 ID		
//}} robobeg : 2009-01-07

		map< int, CKTDGUIRadioButton::RadioButtonGroupInfo* > m_mapRadioButtonGroupInfo;

		int							m_PrevMousePointX;
		int							m_PrevMousePointY;


		D3DXVECTOR2					m_vSize;

		bool						m_bEnableDraggingRange;
		D3DXVECTOR2					m_vDraggingRangeCenter;
		D3DXVECTOR2					m_vDraggingRange;			// width, height

#ifdef FRONT_GUIDE_DESC
		CKTDGUIDialog*				m_pDialogGuideDesc;
#endif //FRONT_GUIDE_DESC
		CKTDGUIStatic*				m_pStaticGuideDesc;

		bool						m_bOKAndCancelMsgBox;
		bool						m_bIsMouseOver;

		int 						m_CustomMsgMouseOut; 
		int 						m_CustomMsgMouseOver;
		int							m_CustomMsgStopDragging;
		int							m_CustomMsgStartDragging;

		int							m_CustomMsgFocusOutEventID;
		
		
		bool						m_bEnableShortCutEnter;

//{{ robobeg : 2009-01-07
		//int							m_iUniqueIndex;
//}} robobeg : 2009-01-07

        bool                        m_bHasSlot;		

		// m_pUnitViewer 외부에서 관리한다. 내부에서 지우거나 하지 않는다.
		// 외부에서 OnFrameMove를 처리하지만, OnFrameRender는 처리하지 않는다.
		// 내부에서 OnFrameRender 처리한다.
		CKTDGObject*                m_pUnitViewer; 
		bool						m_bHasUnit;
		int							m_CustomMsgRMouseUp;

#ifdef PARTICLE_RENDER_BY_DIALOG
		//다이얼로그가 파티클의 레어이를 관리 하도록 다이얼로그 내부에서 OnFrameRender 처리
		vector<CKTDGObject*>		m_ParticleList;
		bool						m_bHasParticle;
#endif //PARTICLE_RENDER_BY_DIALOG

private:

//{{ robobeg : 2009-01-07
        CKTDGUIDialogManager*       m_pkUIDialogManager;
//}} robobeg : 2009-01-07

		bool						m_bFront;
		int							m_iLayer;


#ifdef DIALOG_CRITICAL_SECTION
//{{ seojt // 2009-1-7, 17:22
        MemberCriticalSection       m_mcs;
//}} seojt // 2009-1-7, 17:22		
#endif DIALOG_CRITICAL_SECTION



		bool						m_bDisableUnderWindow; 
		bool						m_bIsOverControl;
		
		bool						m_bMouseOverShowDlg;	


		int							m_iDialogListChangeCount; // dialog가 dialog list에 추가될때, list가 변경된 회수를 저장. dialog list에 추가된 이후에 dialog list가 변동이 있었는지 확인하기 위해 사용.

#ifdef SERV_KEY_MAPPING_INT
		bool						m_bOneClickJoyVectorButton;	
#endif // SERV_KEY_MAPPING_INT

		CKTDXTimer					m_TimerToSelfDelete;
		float						m_fTimeForSelfDelete;
#ifdef REFORM_ENTRY_POINT	 	// 13-11-11, 진입 구조 개편, kimjh
		wstring		m_wstrCustomButtonMouseOverSndFileName;
		wstring		m_wstrCustomButtonMouseUpSndFileName;
#endif // REFORM_ENTRY_POINT	// 13-11-11, 진입 구조 개편, kimjh

#ifdef LOG_LOAD_DLG_SCRIPT
		wstring		m_strScriptFileName;
		wstring		m_strMouseOverStaticName;
		bool		m_bShowScriptName;

	public:
		bool GetShowScriptName() { return m_bShowScriptName; }
		void SetShowScriptName( bool bValue_ ) { m_bShowScriptName = bValue_; }
		wstring GetScriptFileName() { return m_strScriptFileName; }
		wstring GetMouseOverStaticName() { return m_strMouseOverStaticName; }
#endif //LOG_LOAD_DLG_SCRIPT

#ifdef MOVE_FADE_IN_OUT
		bool						m_bFadeInOut;
#endif MOVE_FADE_IN_OUT

};//class CKTDGUIDialog
