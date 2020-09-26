#pragma once

class CKTDGUIStatic : public CKTDGUIControl
{
	public:
		CKTDGUIStatic();

		virtual	~CKTDGUIStatic(void);

		virtual	HRESULT	OnFrameMove( double fTime, float fElapsedTime );
		virtual	HRESULT	OnFrameRender();

		bool HandleMouse( UINT uMsg, POINT pt, WPARAM wParam, LPARAM lParam );

		bool CanHaveFocus();

		void AddPicture( CKTDGUIControl::CPictureData* pPicture ){ m_PictureDataList.push_back(pPicture); }
		void AddString( CKTDGUIControl::UIStringData* pString ){ m_StringDataList.push_back(pString); }

		void AddPictureAt( int iIdx, CKTDGUIControl::CPictureData* pPicture );
		void AddStringAt( int iIdx, CKTDGUIControl::UIStringData* pString );
	
		void AddString_LUA();

#if defined(REFORM_ENTRY_POINT) && defined( MOVIE_TEST_BASE ) || defined( MOVIE_TEST ) 
// #ifdef MOVIE_TEST	 	// 13-11-11, 진입 구조 개편, kimjh, MOVIE_TEST 중 사용에 필요한 Define 을 MOVIE_TEST_BASE 로 변경

		void AddMovieData_LUA(CKTDGUIControl::CMovieData* pMovieData_ ) { m_MovieDataList.push_back( pMovieData_ ); }

#endif // defined(REFORM_ENTRY_POINT) && defined( MOVIE_TEST_BASE ) || defined( MOVIE_TEST ) 
// #endif //  MOVIE_TEST	// 13-11-11, 진입 구조 개편, kimjh, MOVIE_TEST 중 사용에 필요한 Define 을 MOVIE_TEST_BASE 로 변경

		bool ContainsPoint(POINT pt);


		CKTDGUIControl::UIStringData* GetString( int index );
		CKTDGUIControl::CPictureData* GetPicture( int index );
        CKTDGUIControl::CPictureData* GetPictureIndex( int index );

#if defined(REFORM_ENTRY_POINT) && defined( MOVIE_TEST_BASE ) || defined( MOVIE_TEST ) 
// #ifdef MOVIE_TEST	 	// 13-11-11, 진입 구조 개편, kimjh, MOVIE_TEST 중 사용에 필요한 Define 을 MOVIE_TEST_BASE 로 변경

		CKTDGUIControl::CMovieData* GetMovie( const int index_ );

#endif // defined(REFORM_ENTRY_POINT) && defined( MOVIE_TEST_BASE ) || defined( MOVIE_TEST ) 
// #endif //  MOVIE_TEST	// 13-11-11, 진입 구조 개편, kimjh, MOVIE_TEST 중 사용에 필요한 Define 을 MOVIE_TEST_BASE 로 변경

		void SetString( int index, const WCHAR* wszMsg );

		int	GetPictureNum() { return (int)m_PictureDataList.size(); }
		int	GetStringNum() { return (int)m_StringDataList.size(); }

//{{ kimhc // 2010.12.18 // 2010-12-23 New Character CHUNG
#ifdef	NEW_CHARACTER_CHUNG
		void RemovePictureByIndex( int iIndex_ );
#endif	NEW_CHARACTER_CHUNG
//}} kimhc // 2010.12.18 //  2010-12-23 New Character CHUNG

		void RemoveAllPicture();
		void RemoveAllString();

		// Maximize / Minimize
		virtual void SetMaximize( bool val );


		void SetRoundBoxPosSize( D3DXVECTOR2 vPos, D3DXVECTOR2 vRectSize, D3DXVECTOR2 vRoundSize, CKTDGUIStatic::CPictureData** apPictureData );


		void SetCustomMsgMouseDblClk( int msg ){ m_CustomMsgMouseDblClk = msg; } // static 더블클릭
		void SetCustomMsgMouseUp(int msg) { m_CustomMsgMouseUp = msg; }

		virtual void	MoveControl( float fx, float fy );
		virtual void	ScaleControl( float fx, float fy ) ;
		
		
#ifdef DLL_BUILD
		virtual void    MoveSubControl( float fx, float fy, wstring subControlName ) override;
		virtual void    ScaleSubControl( float fx, float fy, wstring subControlName ) override;

		virtual void ShowSubView( wstring name, bool bView ) override;

		virtual D3DXVECTOR2 GetPos() override;
		virtual vector<D3DXVECTOR2> GetPosList() override;
		virtual vector<D3DXVECTOR2> GetPosList( wstring name ) override;
		virtual D3DXVECTOR2 GetPos(wstring name) override;
		virtual D3DXVECTOR2 GetSize( wstring name ) override;
		virtual D3DXVECTOR2 GetAddSize( wstring name ) override;

		virtual vector<wstring> GetNameList( wstring name ) override;
		
		virtual vector<D3DXVECTOR2> GetAddScaleList() override;

		virtual float GetRotation( wstring name ) override;
		virtual void Retate( wstring name, float fDegree ) override;
		virtual vector<float> GetRotationList() override;

		virtual bool IsSelectByEditGui( POINT pt ) override;

		virtual vector<wstring> GetTextureKeyList( wstring name ) override;		// 하위 컨트롤에서 Texture의 key정보를 모두 가지고 온다.
		virtual void SetTexture( wstring name, wstring fileName ) override;		// 하위 컨트롤에서 사용되는 texture를 변경한다.
		virtual void SetTextureKey( wstring name, wstring key ) override;		// 하위 컨트롤에 key 값을 변경한다.

		virtual wstring GetAttributeData( wstring name ) override;

		// string
		void SetStringTableID( wstring name, int id );
		void SetString( wstring name, wstring str );
		void SetFontIndex( wstring name, int index );
		void SetFontStyle( wstring name, int style );
		void SetFontSort( wstring name, int flag );
		void SetFontColor( wstring name, D3DXCOLOR color );
		void SetFontOutlineColor( wstring name, D3DXCOLOR color );

	protected:
		CKTDGUIControl::CPictureData * _GetPictureData( wstring name );
		CKTDGUIControl::UIStringData * _GetStringData( wstring name );

#endif
	protected:

		void DrawEdge( bool bDrawOut, int edgeWidth, D3DXCOLOR edgeColor, CKTDGUIControl::UIPointData* pPoint );
		
		vector<CKTDGUIControl::CPictureData*>	m_PictureDataList;
		vector<CKTDGUIControl::UIStringData*>	m_StringDataList;

#if defined(REFORM_ENTRY_POINT) && defined( MOVIE_TEST_BASE ) || defined( MOVIE_TEST ) 
// #ifdef MOVIE_TEST	 	// 13-11-11, 진입 구조 개편, kimjh, MOVIE_TEST 중 사용에 필요한 Define 을 MOVIE_TEST_BASE 로 변경


		vector<CKTDGUIControl::CMovieData*>		m_MovieDataList;

#endif // defined(REFORM_ENTRY_POINT) && defined( MOVIE_TEST_BASE ) || defined( MOVIE_TEST ) 
// #endif //  MOVIE_TEST	// 13-11-11, 진입 구조 개편, kimjh, MOVIE_TEST 중 사용에 필요한 Define 을 MOVIE_TEST_BASE 로 변경

		CKTDGUIControl::VERTEX_UI				m_Vertex[4];

		CKTDXDeviceTexture*						m_pEdgeTexture;

		int										m_CustomMsgMouseDblClk;
		int										m_CustomMsgMouseUp;
};
