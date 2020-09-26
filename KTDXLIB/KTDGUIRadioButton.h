#pragma once

class CKTDGUIRadioButton : public CKTDGUIControl
{
	public:

		enum RADIOBUTTON_CONTROL_STATE
		{
			RBCS_NORMAL,
			RBCS_MOUSEOVER,
			RBCS_DISABLE,
		};

		enum RADIOBUTTON_EVENT_MSG
		{
			RBEM_RADIOBUTTON_CHECKED	= 0,
			RBEM_RADIOBUTTON_UNCHECKED,
			RBEM_RADIOBUTTON_DOUBLE_CLICKED,
		};

		struct RadioButtonGroupInfo
		{
			int				m_iGroupID;
			int				m_iMaxCheckCount;		// 최대 체크할 수 있는 radio 개수

			vector< CKTDGUIRadioButton* > m_vecpCheckedOrder;

			RadioButtonGroupInfo( int iGroupID )
			{
				m_iGroupID			= iGroupID; 
				m_iMaxCheckCount	= 1;
			}

			bool DeleteRadioButton( CKTDGUIRadioButton* pRadio )
			{
				for( UINT i=0; i<m_vecpCheckedOrder.size(); i++ )
				{
					CKTDGUIRadioButton* pRadioTemp = m_vecpCheckedOrder[i];
					if( pRadio == pRadioTemp )
					{
						m_vecpCheckedOrder.erase( m_vecpCheckedOrder.begin() + i );
						return true;
					}
				}

				return false;
			}

			void SetChecked( CKTDGUIRadioButton* pRadio, bool bChecked )
			{
				// 현재 check 상태가 upate 되는 애들 찾아서 지우고 
				for( UINT i=0; i<m_vecpCheckedOrder.size(); i++ )
				{
					CKTDGUIRadioButton* pRadioTemp = m_vecpCheckedOrder[i];
					if( pRadio == pRadioTemp )
					{
						m_vecpCheckedOrder.erase( m_vecpCheckedOrder.begin() + i );
						i--;
					}
				}

				if( true == bChecked )
				{
					// 최대로 체크할 수 있는 개수를 넘겼으면
					while( (int)m_vecpCheckedOrder.size() >= m_iMaxCheckCount )
					{
						if( m_vecpCheckedOrder.size() == 0 )
							break;

						CKTDGUIRadioButton* pRadioTemp = m_vecpCheckedOrder[0];
						m_vecpCheckedOrder.erase( m_vecpCheckedOrder.begin() );					
						pRadioTemp->SetChecked( false );
					}

					// check 상태가 update 되는 애를 vector에 추가
					m_vecpCheckedOrder.push_back( pRadio );
				}
			}
		};

	public:	
    
		CKTDGUIRadioButton(void);
		virtual ~CKTDGUIRadioButton(void);

		virtual	HRESULT	OnFrameMove( double fTime, float fElapsedTime );
		virtual	HRESULT	OnFrameRender();

		virtual	bool	HandleMouse( UINT uMsg, POINT pt, WPARAM wParam, LPARAM lParam );
		virtual	void	ChangeState( RADIOBUTTON_CONTROL_STATE state );

		void	SetNormalTex_LUA( const char* pFileName, const char* key );
		void	SetOverTex_LUA( const char* pFileName, const char* key );
		void	SetDisableTex_LUA( const char* pFileName, const char* key );
		void	SetCheckedTex_LUA( const char* pFileName, const char* key );

#ifdef DLL_BUILD
		void	SetNarmalTex( wstring fileName, wstring key );
		void	SetOverTex( wstring fileName, wstring key );
		void	SetDisableTex( wstring fileName, wstring key );
		void	SetCheckedTex( wstring fileName, wstring key );
#endif

		void	SetDisabledPoint_LUA();
		void	SetBGPoint_LUA();
		void	SetBGMouseOverPoint_LUA();
		void	SetRBPoint_LUA();
		void	SetCheckPoint_LUA();


		void	SetDisabledPoint( UIPointData* pPointData );
		void	SetBGPoint( UIPointData* pPointData );
		void	SetBGMouseOverPoint( UIPointData* pPointData );
		void	SetRBPoint( UIPointData* pPointData );
		void	SetCheckPoint( UIPointData* pPointData );

		void 	SetCustomMsgMouseOut( int msg ) { m_CustomMsgMouseOut = msg; }
		void 	SetCustomMsgMouseOver( int msg ){ m_CustomMsgMouseOver = msg; }
		void	SetCustomMsgChecked( int msg )	{ m_CustomMsgChecked	= msg; }
		void	SetCustomMsgUnChecked( int msg ){ m_CustomMsgUnChecked	= msg; }
		void	SetCustomMsgDoubleClicked( int msg ) { m_CustomMsgDoubleClicked = msg;}
		void	SetCustomMsgRButtonUp(int val) { m_CustomMsgRButtonUp = val; }



		bool	GetChecked()				{ return m_bChecked; }
		void	SetChecked( bool bChecked, bool bClickedDouble = false, bool bFromInput = false )	{ SetCheckedInternal( bChecked, bClickedDouble, bFromInput ); }
		void	SetChecked_LUA( bool bChecked ) { SetChecked( bChecked ); }
		void	SetCheckedInternal( bool bChecked, bool bClickedDouble = false, bool bFromInput = false );

		CKTDGUIRadioButton*	GetCheckedControl();

		void	SetGroupMaxCheckCount( int iMaxCheckCount );
		void	SetGroupID_LUA( int iGroupID );
		int		GetGroupID() { return m_GroupID; }

		void SetString( CKTDGUIControl::UIStringData* pString ){ m_pStringData = pString; }

		void SetString_LUA()
		{
			CKTDGUIControl::UIStringData* pString = SetStringData_();
			SetString( pString );
		}

		void	SetShowOffBGByCheck( bool bCheck ) { m_bShowOffBGByCheck = bCheck; }
		void	SetFixOverByCheck( bool bCheck ) { m_bFixOverByCheck = bCheck; }

		

		void	SetEnableDoubleClick( bool bEnable ) { m_bEnableDoubleClick = bEnable; }
		void	SetEnableCheckToggle( bool bEnable ) { m_bEnableCheckToggle = bEnable; }

		D3DXVECTOR2 GetPos();

		// Maximize / Minimize
		virtual void SetMaximize( bool val );

		void SetCheckedEdge( bool bDrawOut, int _width, D3DXCOLOR _color );

		D3DXVECTOR2		GetGuideDescPos();

		virtual void	MoveControl( float fx, float fy );
		virtual void	ScaleControl( float fx, float fy );

#ifdef DLL_BUILD
		virtual void    MoveSubControl( float fx, float fy, wstring subControlName ) override;
		virtual void	SetEditGUI( bool bEdit ) override;
		virtual void	ShowSubView( wstring name, bool bView ) override;

		virtual vector<D3DXVECTOR2> GetPosList() override;
		virtual D3DXVECTOR2 GetPos(wstring name) override;			

		virtual bool IsSelectByEditGui( POINT pt ) override { return ContainsPoint(pt); }
		void DrawEditEdge( CKTDGUIControl::UIPointData*	m_pEditEdgePoint );		// UITool에서 편집용으로 사용된다.

		// texture
		CKTDGUIControl::UIPointData * _GetPointData( wstring name );
		virtual wstring GetTextureName( wstring name ) override;
		virtual RECT GetTextureUV( wstring name ) override;
		virtual wstring GetTextureKey( wstring name ) override;
		virtual vector<wstring> GetTextureKeyList( wstring name ) override;

		virtual void SetTexture( wstring name, wstring fileName ) override;
		virtual void SetTextureKey( wstring name, wstring key ) override;
#endif

#ifdef REFORM_ENTRY_POINT	 	// 13-11-11, 진입 구조 개편, kimjh
		virtual	void	SetCustomMouseOverSound ( wstring wstrSoundFileName );
		virtual	void	SetCustomMouseUpSound  ( wstring wstrSoundFileName ); 
#endif // REFORM_ENTRY_POINT	// 13-11-11, 진입 구조 개편, kimjh

	protected:

		void							DrawCheckedEdge( bool bDrawOut );
#ifdef DLL_BUILD		
		bool							m_bEditEdge;
#endif
		bool							m_bChecked;
		bool							m_bPressed;
		bool							m_bRButtonDown;
		bool							m_bEnableDoubleClick;
		bool							m_bEnableCheckToggle;
		RADIOBUTTON_CONTROL_STATE		m_RadioButtonState;


		CKTDGUIControl::UITextureData*	m_pNormalTex;
		CKTDGUIControl::UITextureData*	m_pMouseOverTex;
		CKTDGUIControl::UITextureData*	m_pDisableTex;
		CKTDGUIControl::UITextureData*	m_pCheckedTex;


		CKTDGUIControl::UIPointData*	m_pBGPoint;				// 백그라운드 텍스쳐 포인트
		CKTDGUIControl::UIPointData*	m_pBGMouseOverPoint;	// 백그라운드 텍스쳐 포인트
		CKTDGUIControl::UIPointData*	m_pCheckedPoint;		// 체크 텍스쳐 포인트
		CKTDGUIControl::UIPointData*	m_pRadioButtonPoint;	// 전체 라디오 버튼을 체크 언체크하는 포인트
		CKTDGUIControl::UIPointData*	m_pDisabledPoint;		// 비활성화 상태일 때



		CKTDGUIControl::VERTEX_UI		m_Vertex[4];

		int								m_CustomMsgMouseOut;
		int								m_CustomMsgMouseOver;
		int								m_CustomMsgChecked;
		int								m_CustomMsgUnChecked;
		int								m_CustomMsgDoubleClicked;
		int								m_CustomMsgRButtonUp;
		

		CKTDGUIControl::UIStringData*	m_pStringData;
		int								m_GroupID;

		CKTDGUIControl::UIPointData		m_MyNowPoint;
		CKTDGUIControl::UIPointData		m_MyEndPoint;


		bool							m_bShowOffBGByCheck;
		bool							m_bFixOverByCheck;

		CKTDXDeviceSound*				m_pSndCheck;
		CKTDXDeviceSound*				m_pSndMouseOver;


		D3DXCOLOR						m_CheckedEdgeColor;
		int								m_CheckedEdgeWidth;
		CKTDXDeviceTexture*				m_pCheckedEdgeTexture;
		bool							m_bDrawCheckedEdgeOut; //바깥쪽에 그릴것인가 안쪽에 그릴것인가?
};
