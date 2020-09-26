#include "StdAfx.h"
#include ".\ktdguidialog.h"

CKTDGUIControl* CKTDGUIDialog::s_pControlFocus = NULL;	// The control which has focus

CKTDGUIDialog::CKTDGUIDialog( CKTDXStage* pStage, const WCHAR* wstrScriptFileName /*= NULL*/, float fMoveTime /* = 0.07 */, int iLayer /*= 0 */)
: m_iDialogListChangeCount( 0 )
, m_fTimeForSelfDelete( 0.f )
, m_ID( 0 )
{
#ifdef ACTIVE_KOG_GAME_PERFORMANCE_CHECK
	KOGGamePerformanceCheck::GetInstance()->GetLagCheckMgr()->Pause();
#endif//ACTIVE_KOG_GAME_PERFORMANCE_CHECK

//{{ robobeg : 2009-01-07
    m_pkUIDialogManager = NULL;
//}} robobeg : 2009-01-07

    INIT_VECTOR2( m_Pos, 0.0f, 0.0f );
	m_Color	= D3DXCOLOR(0xffffffff);

	m_iLayer		= iLayer;

	m_pStage		= pStage;
	m_bModal		= true;
	m_bEnable		= true;
	m_bShow			= true;
	m_bFront		= false;
	m_bMaximize		= false;
	m_bCloseOnFocusOut = false;

	m_fAnimateTime = 0.0f;
	INIT_VECTOR2( m_GoalPos, 0.0f, 0.0f );
	m_GoalColor = D3DXCOLOR(0xffffffff);
	m_fChangeTime = 0.0f;
	m_bAutoDelete	= false;
	m_bCheckAutoDeleteEnd = false;

	m_pControlMouseOver = NULL;
	m_pControlMouseOverShowDlg = NULL;

#ifdef PRIORITY_EVERY_UI
	m_pControlMouseOverStatic = NULL;
#endif PRIORITY_EVERY_UI


	m_bEnableMoveByDrag = false;
	m_bMouseDownForDrag = false;

	m_bSuccessInitOpenScript = false;

//{{ robobeg : 2009-01-07
	//m_iDockDialogKeyID = -1;
//}} robobeg : 2009-01-07

	m_ControlsList.reserve( 32 );

	//m_ControlsList.clear();
	m_ControlsMap.clear();
	//m_ControlsMap_MB.clear();

	m_PrevMousePointX = 0;
	m_PrevMousePointY = 0;


	m_vSize = D3DXVECTOR2( 0, 0 );
	m_bEnableDraggingRange = false;
	m_vDraggingRangeCenter = D3DXVECTOR2( 512, 384 );
	m_vDraggingRange = D3DXVECTOR2( 1024, 768 );

	m_CloseCustomUIEventID	= -1;

#ifdef FRONT_GUIDE_DESC
	m_pDialogGuideDesc			= NULL;
#endif //FRONT_GUIDE_DESC
	m_pStaticGuideDesc			= NULL;
	
	m_bOKAndCancelMsgBox = false;

	m_bIsMouseOver = false;
	m_CustomMsgMouseOut = -1; 
	m_CustomMsgMouseOver = -1;
	m_CustomMsgStopDragging = -1;
	m_CustomMsgStartDragging = -1;

	m_CustomMsgFocusOutEventID = -1;

	m_CustomMsgRMouseUp		= -1;

	m_bEnableShortCutEnter = false;
//{{ robobeg : 2009-01-07
	//m_iUniqueIndex = 0;
//}} robobeg : 2009-01-07

    m_bHasSlot = false;
	m_bHasUnit = false;	
#ifdef PARTICLE_RENDER_BY_DIALOG
	m_bHasParticle = false;
#endif //PARTICLE_RENDER_BY_DIALOG

    // 스립립트를 open하기 전에 모든 멤버값들을 초기화해야 한다.
    // - jintaeks on 2009-01-07, 14:35
    m_pUnitViewer = NULL;

	m_bDisableUnderWindow = false;
	m_bIsOverControl = false;

	m_bMouseOverShowDlg = false;

	//다이얼로그 초기화는 여기 밑으로는 하면 안된다.. ㅡ,ㅡ
	m_bSuccessInitOpenScript = false;
	if( NULL != wstrScriptFileName && wcslen( wstrScriptFileName ) > 0 )
	{
		m_bSuccessInitOpenScript = OpenScriptFile( wstrScriptFileName );
#ifdef LOG_LOAD_DLG_SCRIPT
		m_strScriptFileName = wstrScriptFileName;
		m_strMouseOverStaticName = L"";
		m_bShowScriptName = false;
#endif //LOG_LOAD_DLG_SCRIPT
	}

	if ( fMoveTime > 0.0f )
	{
		D3DXCOLOR tempColor = GetColor();
		tempColor.a = 0.0f;
		m_Color = tempColor;

		tempColor.a = 1.0f;
		Move( GetPos(), tempColor, fMoveTime, true, false );
	}//if

#ifdef MOVE_FADE_IN_OUT
	m_bFadeInOut = true;
#endif MOVE_FADE_IN_OUT

    // 아래의 문장 주석처리, OpenScriptFile()의 위로 옮김
    // - jintaeks on 2009-01-07, 14:35
	//m_pUnitViewer = NULL;

#ifdef REFORM_ENTRY_POINT	 	// 13-11-11, 진입 구조 개편, kimjh
	m_wstrCustomButtonMouseOverSndFileName = L"";
	m_wstrCustomButtonMouseUpSndFileName = L"";
#endif // REFORM_ENTRY_POINT	// 13-11-11, 진입 구조 개편, kimjh

#ifdef ACTIVE_KOG_GAME_PERFORMANCE_CHECK
	KOGGamePerformanceCheck::GetInstance()->GetLagCheckMgr()->Resume();
#endif//ACTIVE_KOG_GAME_PERFORMANCE_CHECK

#ifdef SERV_KEY_MAPPING_INT
	m_bOneClickJoyVectorButton = false;	
#endif // SERV_KEY_MAPPING_INT

}

CKTDGUIDialog::~CKTDGUIDialog(void)
{

#ifdef FRONT_GUIDE_DESC
	SAFE_DELETE_DIALOG( m_pDialogGuideDesc );
#endif //FRONT_GUIDE_DESC

//{{ robobeg : 2009-01-07
    if ( GetUIDialogManager() != NULL )
        GetUIDialogManager()->UnmanageDlg( this );
//}} robobeg : 2009-01-07

	if( s_pControlFocus != NULL 
		&& s_pControlFocus->GetDialog() == this )
	{
		ClearFocus();
	}

	if ( m_mapRadioButtonGroupInfo.empty() == false )
	{
		map< int, CKTDGUIRadioButton::RadioButtonGroupInfo* >::iterator it;
		for( it=m_mapRadioButtonGroupInfo.begin(); it!=m_mapRadioButtonGroupInfo.end(); it++ )
		{
			CKTDGUIRadioButton::RadioButtonGroupInfo* pInfo = (CKTDGUIRadioButton::RadioButtonGroupInfo*) it->second;
			SAFE_DELETE( pInfo );
		}

		m_mapRadioButtonGroupInfo.clear();
	}
	

	for( int i = 0; i < (int)m_ControlsList.size(); i++ )
	{
		CKTDGUIControl* pControl = m_ControlsList[i];
		SAFE_DELETE( pControl );
		m_ControlsList[i] = NULL;
	}
	m_ControlsList.clear();
	m_ControlsMap.clear();
	//m_ControlsMap_MB.clear();

}


HRESULT	CKTDGUIDialog::OnFrameMove( double fTime, float fElapsedTime )
{
	KTDXPROFILE();

	if ( m_pStaticGuideDesc != NULL )
		m_pStaticGuideDesc->SetShow( false );

#ifdef FRONT_GUIDE_DESC
	if ( m_pDialogGuideDesc != NULL )
		m_pDialogGuideDesc->SetShow( false );
#endif //FRONT_GUIDE_DESC

	if( m_bEnable == false || m_bShow == false )
		return S_OK;
	

	//{{10.05.25 정협 : 패드로커서 이동 가능
#ifdef SERV_KEY_MAPPING_INT
	if( s_pControlFocus != NULL )
	{		
		unsigned char ucUserInput;		

		if(g_pKTDXApp->GetDIManager()->ListenDeviceInput(ucUserInput, JOYSTICK) == true)
		{
			for(int i = 0; i < 32; i++)	// 임규수 걸리는 부분
			{
				if((unsigned char)DIJOFS_BUTTON(i) == ucUserInput)
				{
					g_pKTDXApp->GetDIManager()->GetJoystic()->SetLock(i, FALSE);
				}				
			}

			if( !m_bOneClickJoyVectorButton )
			{				
				if( g_pKTDXApp->GetDIManager()->GetJoystic()->GetActionState( GAMEACTION_RIGHT ) || g_pKTDXApp->GetDIManager()->GetJoystic()->GetActionState( GAMEACTION_DOWN ) )
				{
					m_bOneClickJoyVectorButton = true;
					map<std::wstring, CKTDGUIControl*>::iterator iter;
					iter = m_ControlsMap.find( s_pControlFocus->GetTabControlName() );
					if( iter != m_ControlsMap.end() )
					{
						CKTDGUIControl* pControl = iter->second;
						RequestFocus( pControl );
						return true;
					}

				}
				else if( g_pKTDXApp->GetDIManager()->GetJoystic()->GetActionState( GAMEACTION_LEFT ) || g_pKTDXApp->GetDIManager()->GetJoystic()->GetActionState( GAMEACTION_UP ) )
				{
					m_bOneClickJoyVectorButton = true;
					map<std::wstring, CKTDGUIControl*>::iterator iter;

					iter = m_ControlsMap.find( s_pControlFocus->GetPrevTabControlName() );
					if( iter != m_ControlsMap.end() )
					{
						CKTDGUIControl* pControl = iter->second;
						RequestFocus( pControl );
						return true;
					}					
				}
			}
		}	
		else
		{
			m_bOneClickJoyVectorButton = false;
		}
	}
#endif // SERV_KEY_MAPPING_INT
	//}}10.05.25 정협 : 패드로커서 이동 가능

	if ( m_fChangeTime > 0.0f )
	{
		if ( m_fAnimateTime >= m_fChangeTime )
		{
			m_fAnimateTime = 0.0f;
			m_fChangeTime = 0.0f;

			m_Pos = m_GoalPos;

			if ( m_bAutoDelete == true )
			{
				m_bCheckAutoDeleteEnd = true;
			}
		}
		else
		{
			float _fRemainTime = m_fChangeTime - m_fAnimateTime;

			D3DXVECTOR2 remainPos		= m_GoalPos - m_Pos;
			m_Pos		+= (remainPos / _fRemainTime) * fElapsedTime;

#ifdef MOVE_FADE_IN_OUT
			if(!m_bFadeInOut)
			{
				m_Color = m_GoalColor;
			}
			else
#endif MOVE_FADE_IN_OUT			
			{
				D3DXCOLOR remainColor = m_GoalColor - m_Color;
				m_Color += (remainColor / _fRemainTime)	* fElapsedTime;
			}

			m_fAnimateTime += fElapsedTime;
		}
	}


	if( GetTimeForSelfDelete() > 0.f )
	{
		if( m_TimerToSelfDelete.elapsed() > GetTimeForSelfDelete() )
		{
			m_bCheckAutoDeleteEnd = true; 
		}
	}






	if ( m_fChangeTime == 0.0f )
	{
		if ( m_queMoveItems.empty() == false )
		{
			DialogMoveItem tempMoveItem; 
			tempMoveItem = m_queMoveItems.front();

			m_GoalPos = tempMoveItem.pos;
			m_GoalColor = tempMoveItem.color;
			m_fChangeTime = tempMoveItem.fChangeTime;
			m_bAutoDelete = tempMoveItem.bIsAutoDelete;

			m_queMoveItems.pop();
		}
	}

	for ( int i = (int)m_ControlsList.size() - 1; i >= 0 ; i-- )
	{
		CKTDGUIControl* pControl = m_ControlsList[i];
		if ( pControl->GetIsAutoDeleteEnd() == true )
		{
			if ( DeleteControl( pControl->GetName() ) == true )
			{
				//--i; // ???
				continue;
			}
		}
#ifdef NEW_SKILL_TREE_UI
		if(pControl->GetOutOfControlList() == false)
			pControl->OnFrameMove( fTime, fElapsedTime );
#else
		pControl->OnFrameMove( fTime, fElapsedTime );
#endif
		
	}

//{{ robobeg : 2009-01-07
	// 도킹 다이얼로그 처리
	//UpdateDockPos();
//}} robobeg : 2009-01-07




	// dialog dragging 범위 제한
	if( m_bEnableMoveByDrag == true && m_bEnableDraggingRange == true )
	{
		D3DXVECTOR2 vRightBottom	= m_vDraggingRangeCenter + m_vDraggingRange * 0.5f - m_vSize * 0.5f;
		D3DXVECTOR2 vLeftTop		= m_vDraggingRangeCenter - m_vDraggingRange * 0.5f - m_vSize * 0.5f;

		if( m_Pos.x < vLeftTop.x )
		{
			m_Pos.x = vLeftTop.x;
		}
		
		if( m_Pos.x > vRightBottom.x )
		{
			m_Pos.x = vRightBottom.x;
		}

		if( m_Pos.y < vLeftTop.y )
		{
			m_Pos.y = vLeftTop.y;
		}
		if( m_Pos.y > vRightBottom.y )
		{
			m_Pos.y = vRightBottom.y;
		}
		SetPos( m_Pos );
	}

	




	return S_OK;
}

//{{ robobeg : 2009-01-07
//void CKTDGUIDialog::UpdateDockPos()
//{
//	KTDXPROFILE();
//
//	if ( m_iDockDialogKeyID != -1 )
//	{
//		CKTDGUIDialogType pDialog = 
//			g_pKTDXApp->GetDGManager()->GetDialogManager()->GetDialogByKeyID( m_iDockDialogKeyID );
//		if ( pDialog != NULL && m_bMouseDownForDrag == false && pDialog->GetShow() == true )
//		{
//			SetPos( pDialog->GetPos() );
//		}
//	}
//}
//}} robobeg : 2009-01-07

HRESULT	CKTDGUIDialog::OnFrameRender( bool bForceDraw /* = false */ )
{
	KTDXPROFILE();



	if( m_bShow == false && bForceDraw == false )
		return S_OK;


	if( GetTimeForSelfDelete() > 0.f )
	{
		if( m_TimerToSelfDelete.elapsed() > GetTimeForSelfDelete() )
		{
			return false;
		}
	}


#ifdef DIALOG_SHOW_TOGGLE
	if( g_pKTDXApp->GetDGManager()->GetDialogManager()->GetHideDialog() == true )
	{
		wstring wstrChatDlgName = L"DLG_ChatBox_EditBox";
		if( GetName() == NULL || (GetName() != NULL && wstrChatDlgName.compare( GetName()) ) )
			return true;
	}
#endif

	
    KD3DBEGIN()

	    CKTDGStateManager::PushSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );
	    CKTDGStateManager::PushSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );
	    CKTDGStateManager::PushSamplerState( 0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR );
	    CKTDGStateManager::PushSamplerState( 1, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );
	    CKTDGStateManager::PushSamplerState( 1, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );
	    CKTDGStateManager::PushSamplerState( 1, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR );

	    CKTDGStateManager::PushSamplerState( 0, D3DSAMP_ADDRESSU,  D3DTADDRESS_MIRROR   );
	    CKTDGStateManager::PushSamplerState( 0, D3DSAMP_ADDRESSV,  D3DTADDRESS_MIRROR   );
	    CKTDGStateManager::PushSamplerState( 1, D3DSAMP_ADDRESSU,  D3DTADDRESS_MIRROR   );
	    CKTDGStateManager::PushSamplerState( 1, D3DSAMP_ADDRESSV,  D3DTADDRESS_MIRROR   );

        CKTDGStateManager::PushStates( g_pKTDXApp->GetDGManager()->GetRSICKTDGUIDialog() );

	    for ( int i = 0; i < (int)m_ControlsList.size(); i++ )
	    {
		    CKTDGUIControl* pControl = m_ControlsList[i];
#ifdef NEW_SKILL_TREE_UI
			if( pControl != NULL && pControl->GetOutOfControlList() == false )
				pControl->OnFrameRender();
#else
			pControl->OnFrameRender();
#endif		    
	    }

	    for ( int i = 0; i < (int)m_ControlsList.size(); i++ )
	    {
		    CKTDGUIControl* pControl = m_ControlsList[i];
#ifdef NEW_SKILL_TREE_UI
			if(pControl->GetOutOfControlList() == false)
				pControl->OnFrameRenderEnd();
#else
			pControl->OnFrameRenderEnd();
#endif
	    }		
    KD3DEND()

	KD3DBEGIN()
		if(m_bHasUnit == true && m_pUnitViewer != NULL)
		{	
			// 다이얼로그에 달린 3D Model을 그리는 부분.
			// 강제로 Matrix들을 리셋해서 그려주는 형식이라 유닛뷰어에 달린 이펙트 등등을 모두 이 타이밍에 그려줘야 한다. (DGManager에서 그리지 않게 할 것.)
			D3DXMATRIX matWorld, matView, matProj;

			g_pKTDXApp->GetDevice()->Clear(0, NULL, D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB(0, 0, 0, 0), 1.0f, 0);

			bool bOriginalProjcetionMode = g_pKTDXApp->GetDGManager()->GetPerspective();
			D3DXMATRIX matIden;
			D3DXMatrixIdentity( &matIden );

			g_pKTDXApp->GetDevice()->GetTransform( D3DTS_WORLD, &matWorld );
			g_pKTDXApp->GetDevice()->GetTransform( D3DTS_VIEW, &matView );
			g_pKTDXApp->GetDevice()->GetTransform( D3DTS_PROJECTION, &matProj );


			// World Matrix는 Identify로 리셋하고
 			g_pKTDXApp->SetWorldTransform( &matIden );
 			
			// View Matrix는 0,0,-1300에서 0,0,0을 바라보는 좌표 (PVPRoom, StateServerSelect 등에서 쓰이는 좌표)로 설정
			D3DXMATRIX ViewMatrix;
			D3DXMatrixLookAtLH( &ViewMatrix, &D3DXVECTOR3(0,0,-1300), &D3DXVECTOR3(0,0,0), &D3DXVECTOR3(0,1.f,0) );
			g_pKTDXApp->SetViewTransform( &ViewMatrix );
			
			// Projection 설정
			g_pKTDXApp->GetDGManager()->SetProjection( g_pKTDXApp->GetDGManager()->GetNear(), g_pKTDXApp->GetDGManager()->GetFar(), false );	// Ortho

			CKTDGStateManager::PushStates( m_pUnitViewer->GetRenderStateID() );
			m_pUnitViewer->OnFrameRender();


			// 원래의 Matrix로 돌려준다.

			g_pKTDXApp->GetDGManager()->SetProjection( g_pKTDXApp->GetDGManager()->GetNear(), g_pKTDXApp->GetDGManager()->GetFar(), bOriginalProjcetionMode );
			g_pKTDXApp->SetProjectionTransform( &matProj );
			g_pKTDXApp->SetWorldTransform( &matWorld );
			g_pKTDXApp->SetViewTransform( &matView );
		}
	KD3DEND()

#ifdef PARTICLE_RENDER_BY_DIALOG
	KD3DBEGIN()

	if( true == m_bHasParticle && false == m_ParticleList.empty() )
	{	
		// 다이얼로그에 달린 3D Model을 그리는 부분.
		// 강제로 Matrix들을 리셋해서 그려주는 형식이라 유닛뷰어에 달린 이펙트 등등을 모두 이 타이밍에 그려줘야 한다. (DGManager에서 그리지 않게 할 것.)
		D3DXMATRIX matWorld, matView, matProj;

		g_pKTDXApp->GetDevice()->Clear(0, NULL, D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB(0, 0, 0, 0), 1.0f, 0);

		bool bOriginalProjcetionMode = g_pKTDXApp->GetDGManager()->GetPerspective();
		D3DXMATRIX matIden;
		D3DXMatrixIdentity( &matIden );

		g_pKTDXApp->GetDevice()->GetTransform( D3DTS_WORLD, &matWorld );
		g_pKTDXApp->GetDevice()->GetTransform( D3DTS_VIEW, &matView );
		g_pKTDXApp->GetDevice()->GetTransform( D3DTS_PROJECTION, &matProj );


		// World Matrix는 Identify로 리셋하고
		g_pKTDXApp->SetWorldTransform( &matIden );

		// View Matrix는 0,0,-1300에서 0,0,0을 바라보는 좌표 (PVPRoom, StateServerSelect 등에서 쓰이는 좌표)로 설정
		D3DXMATRIX ViewMatrix;
		D3DXMatrixLookAtLH( &ViewMatrix, &D3DXVECTOR3(0,0,-1300), &D3DXVECTOR3(0,0,0), &D3DXVECTOR3(0,1.f,0) );
		g_pKTDXApp->SetViewTransform( &ViewMatrix );

		// Projection 설정
		g_pKTDXApp->GetDGManager()->SetProjection( g_pKTDXApp->GetDGManager()->GetNear(), g_pKTDXApp->GetDGManager()->GetFar(), false );	// Ortho

		vector<CKTDGObject*>::iterator it = m_ParticleList.begin();
		for( ; it != m_ParticleList.end(); ++it )
		{
			CKTDGStateManager::PushStates( (*it)->GetRenderStateID() );
			(*it)->OnFrameRender();
		}
		// 원래의 Matrix로 돌려준다.

		g_pKTDXApp->GetDGManager()->SetProjection( g_pKTDXApp->GetDGManager()->GetNear(), g_pKTDXApp->GetDGManager()->GetFar(), bOriginalProjcetionMode );
		g_pKTDXApp->SetProjectionTransform( &matProj );
		g_pKTDXApp->SetWorldTransform( &matWorld );
		g_pKTDXApp->SetViewTransform( &matView );
	}
	KD3DEND()
#endif //PARTICLE_RENDER_BY_DIALOG
#ifdef LOG_LOAD_DLG_SCRIPT
	if( true == m_bShowScriptName )		
	{
		if (g_pKTDXApp->GetDIManager()->Getkeyboard()->GetPureKeyState(DIK_LCONTROL) == TRUE )
		{
			/*g_pKTDXApp->GetDGManager()->GetDialogManager()->GetUKFont( 4 )->OutTextXY(  
				412, 300, m_strScriptFileName.c_str(), D3DXCOLOR(1,0.6f,0,1), 	CKTDGFontManager::FS_SHELL, D3DXCOLOR(0,0,0,1), NULL, DT_CENTER );*/

			if( NULL != m_pControlMouseOverStatic )
			{
				m_strMouseOverStaticName = m_pControlMouseOverStatic->GetName();
				/*g_pKTDXApp->GetDGManager()->GetDialogManager()->GetUKFont( 4 )->OutTextXY(  
					412, 370, m_pControlMouseOverStatic->GetName(), D3DXCOLOR(1,0.6f,0,1), 	CKTDGFontManager::FS_SHELL, D3DXCOLOR(0,0,0,1), NULL, DT_CENTER );*/
			}
		}
		if(g_pKTDXApp->GetDIManager()->Getkeyboard()->GetPureKeyState(DIK_LCONTROL) == TRUE &&
			g_pKTDXApp->GetDIManager()->Getkeyboard()->GetPureKeyState(DIK_LSHIFT) == TRUE )
		{
			m_bShowScriptName = false;
			KLOG("MouseOverScript.txt")  << m_strScriptFileName.c_str() << fileout;
			if( NULL != m_pControlMouseOverStatic )
			{
				KLOG("MouseOverScript.txt")  << m_pControlMouseOverStatic->GetName() << fileout;
			}
			ShellExecute(NULL, NULL, L"notepad++.exe", L"MouseOverScript.txt", L"", SW_SHOWNORMAL );
		}
	}
#endif //LOG_LOAD_DLG_SCRIPT

	return S_OK;
}

HRESULT	CKTDGUIDialog::OnResetDevice()
{
	for ( int i = 0; i < (int)m_ControlsList.size(); i++ )
	{
		CKTDGUIControl* pControl = m_ControlsList[i];
		if ( pControl != NULL )
			pControl->OnResetDevice();
	}

	UpdateRects();

	return S_OK;
}

HRESULT	CKTDGUIDialog::OnLostDevice()
{
	for ( int i = 0; i < (int)m_ControlsList.size(); i++ )
	{
		CKTDGUIControl* pControl = m_ControlsList[i];
		if ( pControl != NULL )
			pControl->OnLostDevice();
	}

	return S_OK;
}

bool CKTDGUIDialog::MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KTDXPROFILE();

	//m_bIsOverControl = false;



	if( m_bEnable == false || m_bShow == false )
		return false;




	if( GetTimeForSelfDelete() > 0.f )
	{
		if( m_TimerToSelfDelete.elapsed() > GetTimeForSelfDelete() )
		{
			return false;
		}
	}






	if( CKTDGUIIMEEditBox::StaticMsgProc( uMsg, wParam, lParam ) == true )
	{
		return true;
	}

    //{{ seojt // 2009-1-7, 17:23
    ASSERT( ::GetCurrentThreadId() != g_pKTDXApp->GetDeviceManager()->GetThreadID() );
#ifdef DIALOG_CRITICAL_SECTION
    CSLock  lock(m_mcs);
#endif DIALOG_CRITICAL_SECTION
    //}} seojt // 2009-1-7, 17:23

	switch( uMsg )
	{
		// Keyboard messages
		case WM_KEYDOWN:
		case WM_SYSKEYDOWN:
		case WM_KEYUP:
		case WM_SYSKEYUP:
			{
				if( uMsg == WM_KEYUP  && s_pControlFocus != NULL )
				{
					if ( wParam == VK_TAB || wParam == VK_RIGHT || wParam == VK_DOWN )
					{
						map<std::wstring, CKTDGUIControl*>::iterator iter;
						iter = m_ControlsMap.find( s_pControlFocus->GetTabControlName() );
						if( iter != m_ControlsMap.end() )
						{
							CKTDGUIControl* pControl = iter->second;
							RequestFocus( pControl );
							return true;
						}
					}
					else if ( wParam == VK_LEFT || wParam == VK_UP)
					{
						map<std::wstring, CKTDGUIControl*>::iterator iter;

						iter = m_ControlsMap.find( s_pControlFocus->GetPrevTabControlName() );
						if( iter != m_ControlsMap.end() )
						{
							CKTDGUIControl* pControl = iter->second;
							RequestFocus( pControl );
							return true;
						}
					}
				}
				// 이 다이얼로그의 컨트롤이 포커스를 가지고 있다면 키보드 메세지를 처리할 기회를 준다.
				if( s_pControlFocus != NULL
					&& s_pControlFocus->GetDialog() == this
					&& s_pControlFocus->GetEnable() == true
					&& s_pControlFocus->GetShow()	== true )
				{
					if( s_pControlFocus->HandleKeyboard( uMsg, wParam, lParam ) == true )
						return true;
				}
				break;
			}

			// Mouse messages
		case WM_MOUSEMOVE:
		case WM_LBUTTONUP:
		case WM_LBUTTONDOWN:
		case WM_MBUTTONDOWN:
		case WM_MBUTTONUP:
		case WM_RBUTTONDOWN:
		case WM_RBUTTONUP:
		case WM_XBUTTONDOWN:
		case WM_XBUTTONUP:
		case WM_LBUTTONDBLCLK:
		case WM_MBUTTONDBLCLK:
		case WM_RBUTTONDBLCLK:
		case WM_XBUTTONDBLCLK:
		case WM_MOUSEWHEEL:
			{
#ifdef DIALOG_SHOW_TOGGLE
				if( g_pKTDXApp->GetDGManager()->GetDialogManager()->GetHideDialog() == true )
					return true;
#endif

#ifdef LOG_LOAD_DLG_SCRIPT
				m_bShowScriptName = (NULL != m_pControlMouseOverStatic);
#endif //LOG_LOAD_DLG_SCRIPT

				m_bIsOverControl = false;

				// 다이얼로그 위치와 화면 해상도에 따라 마우스 포인터 위치를 재조정한다
				//POINT mousePoint = { short(LOWORD(lParam)), short(HIWORD(lParam)) };
				
#ifdef X2TOOL
				POINT mousePoint = { short(LOWORD(lParam)), short(HIWORD(lParam)) };
#else
				//{{ 허상형 : [2009/7/27] //	마우스 포인터 위치를 절대 좌표에서 클라이언트 좌표로 수정
				POINT mousePoint = { g_pKTDXApp->GetDIManager()->GetMouse()->GetXPos(), g_pKTDXApp->GetDIManager()->GetMouse()->GetYPos() };
				//}} 허상형 : [2009/7/27] //	마우스 포인터 위치를 절대 좌표에서 클라이언트 좌표로 수정
#endif

				D3DXVECTOR2 resolPos;
				resolPos = g_pKTDXApp->ConvertByResolution( m_Pos );

				mousePoint.x -= (LONG)resolPos.x;
				mousePoint.y -= (LONG)resolPos.y;

				if( WM_MOUSEMOVE )
				{
					CKTDGUIControl* pControl = GetControlAtPointAbsolute( mousePoint );

					if( NULL != pControl )
					{
						if( false == m_bIsMouseOver )
						{
							m_bIsMouseOver = true;
							if( -1 != m_CustomMsgMouseOver )
							{
								SendInternelEvent( g_pKTDXApp->GetHWND(), CKTDXApp::KM_UI_CONTROL_CUSTOM_EVENT, m_CustomMsgMouseOver, (LPARAM)NULL );
							}
						}
					}
					else
					{
						if( true == m_bIsMouseOver )
						{
							m_bIsMouseOver = false;
							if( -1 != m_CustomMsgMouseOut )
							{
								SendInternelEvent( g_pKTDXApp->GetHWND(), CKTDXApp::KM_UI_CONTROL_CUSTOM_EVENT, m_CustomMsgMouseOut, (LPARAM)NULL );
							}
						}
					}
				}
				
				//다이얼로그 움직이기..
				if ( GetEnableMoveByDrag() == true && uMsg == WM_MOUSEMOVE /*&& s_pControlFocus != NULL*/)
				{
			
					CKTDGUIControl* pControl = s_pControlFocus;
					if ( pControl == NULL )
					{
						pControl = GetControlAtPointForStatic( mousePoint );
					}
/*
					// dragging 되는 dialog, 즉 static control의 크기가 작은 경우에 대한 예외 처리 
					if( true == m_bMouseDownForDrag && pControl == NULL )
					{
						pControl = GetControl(0);
					}
*/										
					if ( pControl != NULL )
					{

						D3DXVECTOR2 offsetPos;
						offsetPos = g_pKTDXApp->ConvertByResolution( pControl->GetOffsetPos() );

						POINT mousePointFinal = mousePoint;
						mousePointFinal.x -= (LONG)offsetPos.x;
						mousePointFinal.y -= (LONG)offsetPos.y;

						D3DXVECTOR2 mousePos = g_pKTDXApp->MouseConvertByResolution( (float)mousePointFinal.x, (float)mousePointFinal.y );
						mousePointFinal.x = (LONG)mousePos.x;
						mousePointFinal.y = (LONG)mousePos.y;
						
						int deltaX = mousePointFinal.x - m_PrevMousePointX;
						int deltaY = mousePointFinal.y - m_PrevMousePointY;

						if ( m_bMouseDownForDrag == true )
						{
							SetPos( D3DXVECTOR2( GetPos().x + deltaX, GetPos().y + deltaY) );
							return true;
						}

						m_PrevMousePointX = (int)mousePointFinal.x;
						m_PrevMousePointY = (int)mousePointFinal.y;				
					}
				}

				if ( HandleMouse( uMsg, mousePoint, wParam, lParam ) == true )
				{
					return true;
				}
				else
				{
					//마우스가 이 다이얼로그의 어떤 컨트롤 위에도 없으면서 
					// 버튼이 눌려졌다면 현재 컨트롤은 포커스를 잃는다.
					if( uMsg == WM_LBUTTONDOWN && 
						s_pControlFocus != NULL
						&& s_pControlFocus->GetDialog() == this )
					{
						s_pControlFocus->OnFocusOut();
						s_pControlFocus = NULL;
					}

					if( uMsg == WM_LBUTTONDOWN )
					{
						// 포커스를 잃었을때, 감춤 처리
						if ( m_bCloseOnFocusOut == true )
						{
							SetShowEnable( false, false );
							SendCloseCustomUIEventID();
						}

						// 포커스를 잃었을경우 이벤트 연결
						if( m_CustomMsgFocusOutEventID != -1 && m_pStage != NULL )
                        {
							m_pStage->MsgProc( g_pKTDXApp->GetHWND(), CKTDXApp::KM_UI_CONTROL_CUSTOM_EVENT, m_CustomMsgFocusOutEventID, (LPARAM)this );
                        }//if
					}

					m_bMouseDownForDrag = false;
				}

				// 아직도 메세지가 처리되지 않았다면 리턴시킨다.
				switch( uMsg )
				{
				case WM_LBUTTONDOWN:
					{
						CKTDGUIControl* pControl = GetControlAtPointForStatic( mousePoint );
						if ( pControl != NULL 
							&& pControl->GetDialog() == this 
							&& pControl->GetShow() == true )
						{
							//RequestFocus( pControl );
							m_bMouseDownForDrag = true;
							if( -1 != m_CustomMsgStartDragging )
							{
								SendInternelEvent( g_pKTDXApp->GetHWND(), CKTDXApp::KM_UI_CONTROL_CUSTOM_EVENT, (WPARAM)m_CustomMsgStartDragging, (LPARAM)this );
							}
						}

						if(m_bDisableUnderWindow == true && m_bIsOverControl == true)
						{
							return true;
						}
					}
					break;

				case WM_LBUTTONUP:
					{
						m_bMouseDownForDrag = false;
						if( -1 != m_CustomMsgStopDragging )
						{
							SendInternelEvent( g_pKTDXApp->GetHWND(), CKTDXApp::KM_UI_CONTROL_CUSTOM_EVENT, (WPARAM)m_CustomMsgStopDragging, (LPARAM)this );
						}

						if(m_bDisableUnderWindow == true && m_bIsOverControl == true)
						{
							return true;
						}
					}
					break;


				case WM_MOUSEMOVE:
					{
						OnMouseMove( mousePoint );
						
						if(m_bDisableUnderWindow == true && m_bIsOverControl == true)
						{
							return true;
						}
					}
					break;

				case WM_RBUTTONUP:
					{
						if( m_CustomMsgRMouseUp != -1 )
							SendInternelEvent( g_pKTDXApp->GetHWND(), CKTDXApp::KM_UI_CONTROL_CUSTOM_EVENT, m_CustomMsgRMouseUp, (LPARAM)this );
					} break;
				//{{ 09.06.11 태완 : 위에서 명시되지 않은 마우스 메시지들은 m_bDisableUnderWindow의 영향을 받지 않는 문제 수정
				default:	// 위에 명시되지 않은 모든 마우스 메시지.
					{
						if(m_bDisableUnderWindow == true && m_bIsOverControl == true)
						{
							return true;
						}

					} break;
				//}}
				}				
			}
			break;

		default:
			{
				// 포커스 다이얼로그 처리
				if( s_pControlFocus != NULL
					&& s_pControlFocus->GetDialog() == this
					&& s_pControlFocus->GetEnable() == true
					&& s_pControlFocus->GetShow()	== true )
				{
					if( s_pControlFocus->MsgProc( hWnd, uMsg, wParam, lParam ) == true )
						return true;
				}
			}
			break;
	}	


	
	
	return false;
}

void CKTDGUIDialog::AddControl( CKTDGUIControl* pControl )
{
	KTDXPROFILE();


	if( NULL == pControl )
		return;

    //{{ seojt // 2009-1-7, 17:23
    ASSERT( ::GetCurrentThreadId() != g_pKTDXApp->GetDeviceManager()->GetThreadID() );
#ifdef DIALOG_CRITICAL_SECTION
	CSLock  lock(m_mcs);
#endif DIALOG_CRITICAL_SECTION

    //}} seojt // 2009-1-7, 17:23

	pControl->SetDialog( this );

	map<std::wstring, CKTDGUIControl*>::iterator iter;

	ASSERT( NULL != pControl->GetName() );
	if( NULL == pControl->GetName() )
	{
		pControl->SetNameByForce( L"unnamed_control_" );
	}
	
	iter = m_ControlsMap.find( pControl->GetName() );
	if( iter != m_ControlsMap.end() )
	{
		WCHAR buff[256] = {0};
		int i = 100;
		while( iter != m_ControlsMap.end() )
		{
			StringCchPrintfW( buff, ARRAY_SIZE(buff), L"%s%d", pControl->GetName(), i );
			iter = m_ControlsMap.find( buff );
			i++;
		}
		pControl->SetNameByForce( (const WCHAR*) buff );
	}
#ifdef REFORM_ENTRY_POINT	 	// 13-11-11, 진입 구조 개편, kimjh
	if ( m_wstrCustomButtonMouseOverSndFileName != L"" )
	{
		pControl->SetCustomMouseOverSound( m_wstrCustomButtonMouseOverSndFileName );
	}
	if ( m_wstrCustomButtonMouseUpSndFileName != L"" )
	{
		pControl->SetCustomMouseUpSound( m_wstrCustomButtonMouseUpSndFileName );
	}
#endif // REFORM_ENTRY_POINT	// 13-11-11, 진입 구조 개편, kimjh

	m_ControlsMap.insert( std::make_pair( pControl->GetName(), pControl ) );
	//m_ControlsMap_MB.insert( std::make_pair( pControl->GetName_MB(), pControl ) );
	m_ControlsList.push_back( pControl );
}//CKTDGUIDialog::AddControl()

#ifdef DLL_BUILD
void CKTDGUIDialog::AddControl_Front( CKTDGUIControl* pControl )
{
	if( NULL == pControl )
		return;

	AddControl( pControl );

	// 앞에 추가하기 위해 제거
	vector<CKTDGUIControl * >::iterator itor = find( m_ControlsList.begin(), m_ControlsList.end(), pControl );
	if( itor != m_ControlsList.end() )
		m_ControlsList.erase( itor );

	// 리스트에 앞에 추가한다/ 컨트롤을 제일 뒤에 보이게 하기 위함
	m_ControlsList.insert( m_ControlsList.begin(), pControl );
}
#endif

bool CKTDGUIDialog::DeleteControl( const WCHAR* pControlName )
{
	KTDXPROFILE();

	if( NULL == pControlName )
		return false;

    //{{ seojt // 2009-1-7, 17:23
    ASSERT( ::GetCurrentThreadId() != g_pKTDXApp->GetDeviceManager()->GetThreadID() );
#ifdef DIALOG_CRITICAL_SECTION
	CSLock  lock(m_mcs);
#endif DIALOG_CRITICAL_SECTION

    //}} seojt // 2009-1-7, 17:23

	map<std::wstring, CKTDGUIControl*>::iterator iter = m_ControlsMap.find( pControlName );

	if( iter == m_ControlsMap.end() )
		return false;

	CKTDGUIControl* pControl = iter->second;
	m_ControlsMap.erase( iter );

	//string cName = "";
	//ConvertWCHARToChar( cName, pControlName );
	//map<std::string, CKTDGUIControl*>::iterator iter2 = m_ControlsMap_MB.find( cName );
	//if( iter2 == m_ControlsMap_MB.end() )
	//	return false;

	//m_ControlsMap_MB.erase( iter2 );

	for( int i = 0; i < (int)m_ControlsList.size(); i++ )
	{
		if( pControl == m_ControlsList[i] )
		{
			if( s_pControlFocus == pControl )
			{
				ClearFocus();
			}
			SAFE_DELETE( pControl );
			m_ControlsList.erase( m_ControlsList.begin() + i );
			
			return true;
		}
	}
	return false;
}

bool CKTDGUIDialog::ChangeSequence( CKTDGUIControl* pControlToChange, bool bFront ) // 가장 앞으로 옮길것이냐 가장 뒤로 옮길것이냐
{
	KTDXPROFILE();

	if ( pControlToChange == NULL )
		return false;

	for ( int index = 0; index < (int)m_ControlsList.size(); index++ )
	{
		CKTDGUIControl* pControl = m_ControlsList[index];
		if ( pControl == pControlToChange )
		{
			m_ControlsList.erase( m_ControlsList.begin() + index );
			if ( bFront == true )
			{
				m_ControlsList.push_back( pControl );
			}
			else
			{
				m_ControlsList.insert( m_ControlsList.begin(), pControl );
			}
			return true;
		}
		
	}

	return false;
}



void CKTDGUIDialog::ChangeSequenceByName_LUA( const char* pControlName, bool bFront )
{
	KTDXPROFILE();

    wstring wstrControlName;
    ConvertUtf8ToWCHAR( wstrControlName, pControlName );

	CKTDGUIControl* pControlToChange = GetControl( wstrControlName.c_str() );
	ChangeSequence( pControlToChange, bFront );
}

void CKTDGUIDialog::ChangeSequence( CKTDGUIControl* pControlToChange, CKTDGUIControl* pPivotControl, bool bFront )
{
	KTDXPROFILE();

	if ( CheckControl( pControlToChange ) == false || CheckControl( pPivotControl ) == false )
		return;

	for ( int index = 0; index < (int)m_ControlsList.size(); index++ )
	{
		CKTDGUIControl* pControl = m_ControlsList[index];
		if ( pControl == pControlToChange )
		{
			m_ControlsList.erase( m_ControlsList.begin() + index );

			vector<CKTDGUIControl*>::iterator it;
			for( it=m_ControlsList.begin(); it!=m_ControlsList.end(); it++ )
			{
				CKTDGUIControl* pTempControl= (CKTDGUIControl*)*it;

				if( pTempControl == pPivotControl )
				{
					if ( bFront == false )
					{
						m_ControlsList.insert( it, pControlToChange );
					}
					else
					{
						it++;
						m_ControlsList.insert( it, pControlToChange );
					}
					return;
				}
			}
		}

	}
}

void CKTDGUIDialog::ChangeSequenceByNameAndPivot_LUA( const char* pControlNameToChange, const char* pPivotControlName, bool bFront )
{
	KTDXPROFILE();

    wstring wstrControlNameToChange;
    ConvertUtf8ToWCHAR( wstrControlNameToChange, pControlNameToChange );
    wstring wstrPivotControlName;
    ConvertUtf8ToWCHAR( wstrPivotControlName, pPivotControlName );

	CKTDGUIControl* pControlToChange = GetControl( wstrControlNameToChange.c_str() );
	CKTDGUIControl* pPivotControl = GetControl( wstrPivotControlName.c_str() );
	ChangeSequence( pControlToChange, pPivotControl, bFront );
}


bool CKTDGUIDialog::CheckControl( CKTDGUIControl* pControlToFind )
{
	KTDXPROFILE();

	if ( pControlToFind == NULL )
		return false;

	for ( int index = 0; index < (int)m_ControlsList.size(); index++ )
	{
		CKTDGUIControl* pControl = m_ControlsList[index];
		if ( pControl == pControlToFind )
		{
			return true;
		}
	}

	return false;
}

bool CKTDGUIDialog::CheckControl( const WCHAR* wszControlName )
{
	KTDXPROFILE();

	if ( wszControlName == NULL )
		return false;


    //{{ seojt // 2009-1-7, 17:23
    ASSERT( ::GetCurrentThreadId() != g_pKTDXApp->GetDeviceManager()->GetThreadID() );
#ifdef DIALOG_CRITICAL_SECTION
	CSLock  lock(m_mcs);
#endif DIALOG_CRITICAL_SECTION

    //}} seojt // 2009-1-7, 17:23

	CKTDGUIControl* pControl = NULL;
	map<std::wstring, CKTDGUIControl*>::iterator iter;

	if ( m_ControlsMap.empty() == true )
		return false;

	iter = m_ControlsMap.find( wszControlName );
	if( iter != m_ControlsMap.end() )
	{
		return true;
	}

	return false;
}

//CKTDGUIControl*	CKTDGUIDialog::GetControl_LUA( const char* pControlName )
//{
//	KTDXPROFILE();
//
//
//	if ( pControlName == NULL )
//		return NULL;
//
//	CKTDGUIControl* pControl = NULL;
//	map<std::string, CKTDGUIControl*>::iterator iter;
//
//	//{{ seojt // 2009-1-7, 17:23
//	ASSERT( ::GetCurrentThreadId() != g_pKTDXApp->GetDeviceManager()->GetThreadID() );
//	//CSLock  lock(m_mcs);
//	//}} seojt // 2009-1-7, 17:23
//
//
//	//{{ dmlee 2009.3.22 critical section 범위를 변경했습니다.
//	{
//#ifdef DIALOG_CRITICAL_SECTION
//		CSLock  lock(m_mcs);
//#endif DIALOG_CRITICAL_SECTION
//
//
//		if ( m_ControlsMap_MB.empty() == true )
//			return pControl;
//
//		iter = m_ControlsMap_MB.find( pControlName );
//		if( iter != m_ControlsMap_MB.end() )
//		{
//			pControl = iter->second;
//		}
//	}
//	//}} dmlee 2009.3.22 critical section 범위를 변경했습니다.
//
//
//	if ( pControl == NULL )
//	{
//		string fileName = pControlName;
//		fileName += " <- 이 컨트롤 찾기 실패! ";
//		string dialogName;
//		ConvertWCHARToChar( dialogName, m_Name.c_str() );
//		fileName += "-부모 다이얼로그 이름 : ";
//		fileName += dialogName;
//		ErrorLogMsg( KEM_ERROR36, fileName.c_str() );
//	}
//
//
//	return pControl;
//}	

const CKTDGUIControl* CKTDGUIDialog::_GetControl( const WCHAR* pControlName ) const
{
	KTDXPROFILE();


	if ( pControlName == NULL )
		return NULL;

	CKTDGUIControl* pControl = NULL;
	map<std::wstring, CKTDGUIControl*>::const_iterator iter;


	//{{ seojt // 2009-1-7, 17:23
	ASSERT( ::GetCurrentThreadId() != g_pKTDXApp->GetDeviceManager()->GetThreadID() );
	//CSLock  lock(m_mcs);
	//}} seojt // 2009-1-7, 17:23

	
	//{{ dmlee 2009.3.22 critical section 범위를 변경했습니다.
	{
#ifdef DIALOG_CRITICAL_SECTION
		CSLock  lock(m_mcs);
#endif DIALOG_CRITICAL_SECTION


		if ( m_ControlsMap.empty() == true )
			return pControl;

		iter = m_ControlsMap.find( pControlName );
		if( iter != m_ControlsMap.end() )
		{
			pControl = iter->second;
		}
	}
	//}} dmlee 2009.3.22 critical section 범위를 변경했습니다.

	if ( pControl == NULL )
	{
		string fileName;
		ConvertWCHARToChar( fileName, pControlName );
		fileName += " <- 이 컨트롤 찾기 실패! ";
		string dialogName;
		ConvertWCHARToChar( dialogName, m_Name.c_str() );
		fileName += "-부모 다이얼로그 이름 : ";
		fileName += dialogName;
		ErrorLogMsg( KEM_ERROR36, fileName.c_str() );
	}

	return pControl;
}


//{{ seojt // 2009-8-19, 17:47
CKTDGUIControl*	CKTDGUIDialog::GetControl( const WCHAR* pControlName )
{
    return const_cast<CKTDGUIControl*>( _GetControl( pControlName ) );
}//CKTDGUIDialog::GetControl()


const CKTDGUIControl* CKTDGUIDialog::GetControl( const WCHAR* pControlName ) const
{
    return _GetControl( pControlName );
}//CKTDGUIDialog::GetControl()
//}} seojt // 2009-8-19, 17:47


CKTDGUIControl*	CKTDGUIDialog::GetControl( int index )
{
	KTDXPROFILE();

	CKTDGUIControl* pControl = NULL;
	if ( index >= 0 && index < (int)m_ControlsList.size() )
	{
		pControl = m_ControlsList[index];
		return pControl;
	}

	return NULL;
}


//{{ seojt // 2009-8-19, 17:18
const CKTDGUIControl* CKTDGUIDialog::GetControl( int index ) const
{
	KTDXPROFILE();

	const CKTDGUIControl* pControl = NULL;
	if ( index >= 0 && index < (int)m_ControlsList.size() )
	{
		pControl = m_ControlsList[index];
		return pControl;
	}

	return NULL;
}
//}} seojt // 2009-8-19, 17:18


bool CKTDGUIDialog::ReNameControl( CKTDGUIControl* pControl )
{
	KTDXPROFILE();

	if( NULL == pControl )
		return false;

    //{{ seojt // 2009-1-7, 17:23
    ASSERT( ::GetCurrentThreadId() != g_pKTDXApp->GetDeviceManager()->GetThreadID() );
#ifdef DIALOG_CRITICAL_SECTION
	CSLock  lock(m_mcs);
#endif DIALOG_CRITICAL_SECTION

    //}} seojt // 2009-1-7, 17:23

	map<std::wstring, CKTDGUIControl*>::iterator iter;

	WCHAR buff[256] = {0};
	StringCchCopyW( buff, ARRAY_SIZE(buff), pControl->GetName() );
	iter = m_ControlsMap.find( pControl->GetName() );

	int i = 100;

	if( iter != m_ControlsMap.end() )
	{
		while( iter != m_ControlsMap.end() )
		{
			StringCchPrintfW( buff, ARRAY_SIZE(buff), L"%s%d", pControl->GetName(), i );
			iter = m_ControlsMap.find( buff );
			i++;
		}
		
		pControl->SetName( buff );
		return false;
	}

	CKTDGUIControl* pTemp = NULL;
	for( iter = m_ControlsMap.begin(); iter != m_ControlsMap.end(); iter++ )
	{
		pTemp = iter->second;
        // TODO : 이런 경우에는 assert를 띄워야 합니다.
		if( pControl == pTemp )
		{
			m_ControlsMap.erase( iter );
			break;
		}
	}
	pTemp = NULL;
	
	//map<std::string, CKTDGUIControl*>::iterator iter2;

	//for( iter2 = m_ControlsMap_MB.begin(); iter2 != m_ControlsMap_MB.end(); iter2++ )
	//{
	//	pTemp = iter2->second;
	//	if( pControl == pTemp )
	//	{
	//		m_ControlsMap_MB.erase( iter2 );
	//		break;
	//	}
	//}

	wstring tempName = buff;
	pControl->SetNameByForce( tempName.c_str() );

	m_ControlsMap.insert( std::make_pair(pControl->GetName(),pControl) );
	//m_ControlsMap_MB.insert( std::make_pair(pControl->GetName_MB(),pControl) );

	return true;
}

void CKTDGUIDialog::RequestFocus( CKTDGUIControl* pControl )
{
	KTDXPROFILE();

	if( s_pControlFocus == pControl )
		return;

	if( pControl->CanHaveFocus() == false )
		return;

	if( s_pControlFocus != NULL )
		s_pControlFocus->OnFocusOut();

	pControl->OnFocusIn();
	s_pControlFocus = pControl;
}

void CKTDGUIDialog::ClearFocus()
{
	KTDXPROFILE();

	if( s_pControlFocus != NULL )
	{
		s_pControlFocus->OnFocusOut();
		s_pControlFocus = NULL;
	}
}

void CKTDGUIDialog::OnMouseMove( POINT pt )
{
	KTDXPROFILE();

#ifdef PRIORITY_EVERY_UI
	m_pControlMouseOverStatic = GetControlAtPointForStatic( pt );
#endif PRIORITY_EVERY_UI

	CKTDGUIControl* pControl = GetControlAtPointExceptStatic( pt );

	
	if(m_bMouseOverShowDlg == true)
	{
		CKTDGUIControl *pOverControl = GetControlAtPointAbsolute( pt );

		if(m_pControlMouseOverShowDlg != pOverControl)
		{
			if(m_pControlMouseOverShowDlg != NULL)
				m_pControlMouseOverShowDlg->OnMouseLeave();

			m_pControlMouseOverShowDlg = pOverControl;
			if(pOverControl != NULL)
			{
				pOverControl->OnMouseEnter();
			}
		}
	}

	// 만약 이전과 똑같은 컨트롤에 마우스가 있다면 그냥 리턴
	if( pControl == m_pControlMouseOver )
		return;

	// Handle mouse leaving the old control
	if( m_pControlMouseOver != NULL )
		m_pControlMouseOver->OnMouseLeave();

	// Handle mouse entering the new control
	m_pControlMouseOver = pControl;
	if( pControl != NULL )
		m_pControlMouseOver->OnMouseEnter();	
}

bool CKTDGUIDialog::HandleMouse( UINT uMsg, POINT pt, WPARAM wParam, LPARAM lParam )
{
	KTDXPROFILE();	

	
	// 이 다이얼로그의 컨트롤이 포커스를 가지고 있다면 마우스 메세지를 처리할 기회를 준다.
	// 콤보박스만 처리해준다.. 특수한 형태의 컨트롤 이기 때문에..
	if( s_pControlFocus != NULL
		&& s_pControlFocus->GetDialog() == this
		&& s_pControlFocus->GetEnable() == true
		&& s_pControlFocus->GetShow()	== true
		&& ( s_pControlFocus->GetType() == CKTDGUIControl::UCT_COMBOBOX ||
			 s_pControlFocus->GetType() == CKTDGUIControl::UCT_IME_EDITBOX )	// dmlee, 2009-01-12: editbox에서 drag로 텍스트 선택중에 editbox영역을 벗어나는 경우에 releasecapture해주기 위해서 여기에 추가
		)
	{
		D3DXVECTOR2 offsetPos;
		offsetPos = g_pKTDXApp->ConvertByResolution( s_pControlFocus->GetOffsetPos() );

		POINT mousePointFinal = pt;
		mousePointFinal.x -= (LONG)offsetPos.x;
		mousePointFinal.y -= (LONG)offsetPos.y;

		D3DXVECTOR2 mousePos = g_pKTDXApp->MouseConvertByResolution( (float)mousePointFinal.x, (float)mousePointFinal.y );
		mousePointFinal.x = (LONG)mousePos.x;
		mousePointFinal.y = (LONG)mousePos.y;

		// 자식 컨트롤의 메세지 핸들링
		if( s_pControlFocus->HandleMouse( uMsg, mousePointFinal, wParam, lParam ) == true )
		{
			return true;
		}
	}


	if( m_ControlsList.size() == 0 )
		return NULL;

	// 나머지 우선순위대로 업데이트
	for ( int i = (int)m_ControlsList.size()-1; i >= 0 ; i-- )
	{
		CKTDGUIControl* pControl = m_ControlsList[i];
		if( pControl == NULL )
			continue;

		D3DXVECTOR2 offsetPos;
		offsetPos = g_pKTDXApp->ConvertByResolution( pControl->GetOffsetPos() );

		POINT mousePointFinal = pt;
		mousePointFinal.x -= (LONG)offsetPos.x;
		mousePointFinal.y -= (LONG)offsetPos.y;

		D3DXVECTOR2 mousePos = g_pKTDXApp->MouseConvertByResolution( (float)mousePointFinal.x, (float)mousePointFinal.y );
		mousePointFinal.x = (LONG)mousePos.x;
		mousePointFinal.y = (LONG)mousePos.y;

		if( pControl->ContainsPoint( mousePointFinal ) == true && pControl->GetShow() == true )
		{
			m_bIsOverControl = true;
		}
		

		if( pControl->ContainsPoint( mousePointFinal ) == true
			&& pControl->GetEnable() == true
			&& pControl->GetShow() == true 
#ifdef NEW_SKILL_TREE_UI
			&& pControl->GetOutOfControlList() == false 
#endif
			)
		{
			OnMouseMove( pt );

			// 자식 컨트롤의 메세지 핸들링
			if ( pControl->HandleMouse( uMsg, mousePointFinal, wParam, lParam ) == true )
			{
				return true;
			}						
		}		
	}

	return false;
}

void CKTDGUIDialog::CreateGuideDesc()
{
	KTDXPROFILE();

#ifdef FRONT_GUIDE_DESC
	if ( m_pDialogGuideDesc != NULL )
		return;

	m_pDialogGuideDesc = new CKTDGUIDialog( m_pStage, L"DLG_GuideDesc.lua", 0.07f, 400 );
	m_pDialogGuideDesc->SetPos( GetPos() );
	g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDialogGuideDesc );

	m_pStaticGuideDesc = m_pDialogGuideDesc->GetStatic_LUA( "GuideDesc" );
#else //FRONT_GUIDE_DESC
	if ( m_pStaticGuideDesc != NULL )
		return;

	m_pStaticGuideDesc = CreateStatic();
	AddControl( m_pStaticGuideDesc );
	m_pStaticGuideDesc->SetName( L"GuideDesc" );
	CKTDGUIControl::CPictureData* pPicture = new CKTDGUIControl::CPictureData();
	m_pStaticGuideDesc->AddPicture( pPicture );
	pPicture->SetEdge( false, 2, D3DXCOLOR(1,1,1,1) );
	pPicture->SetTex( L"UIEdge.tga" );
#endif //FRONT_GUIDE_DESC

	CKTDGUIControl::UIStringData* pString = new CKTDGUIControl::UIStringData();
	m_pStaticGuideDesc->AddString( pString );
	pString->fontIndex = 2;
	pString->color = D3DXCOLOR( 1,1,1,1 );
	pString->outlineColor = D3DXCOLOR( 1,1,1,1 );
	pString->msg = L"";
	pString->fontStyle = CKTDGFontManager::FS_NONE;
	pString->sortFlag = DT_LEFT;
	pString->pos = D3DXVECTOR2( 5, 5 );

	m_pStaticGuideDesc->SetShow( false );
}

CKTDGUIButton* CKTDGUIDialog::GetButton_LUA( const char* pControlName )
{
	KTDXPROFILE();

    wstring wstrControlName;
    ConvertUtf8ToWCHAR( wstrControlName, pControlName );

	CKTDGUIButton* pControl = (CKTDGUIButton*)GetControl( wstrControlName.c_str() );

	if ( pControl == NULL )
		MessageBoxA( g_pKTDXApp->GetHWND(), pControlName, "GetButton_LUA 실패", MB_OK );

	return pControl;
}

CKTDGUICheckBox* CKTDGUIDialog::GetCheckBox_LUA( const char* pControlName )
{
	KTDXPROFILE();

    wstring wstrControlName;
    ConvertUtf8ToWCHAR( wstrControlName, pControlName );

	CKTDGUICheckBox* pControl = (CKTDGUICheckBox*)GetControl( wstrControlName.c_str() );

	if ( pControl == NULL )
		MessageBoxA( g_pKTDXApp->GetHWND(), pControlName, "GetCheckBox_LUA 실패", MB_OK );

	return pControl;
}

CKTDGUIComboBox* CKTDGUIDialog::GetComboBox_LUA( const char* pControlName )
{
	KTDXPROFILE();

    wstring wstrControlName;
    ConvertUtf8ToWCHAR( wstrControlName, pControlName );

	CKTDGUIComboBox* pControl = (CKTDGUIComboBox*)GetControl( wstrControlName.c_str() );

	if ( pControl == NULL )
		MessageBoxA( g_pKTDXApp->GetHWND(), pControlName, "GetComboBox_LUA 실패", MB_OK );

	return pControl;
}

CKTDGUIEditBox*	 CKTDGUIDialog::GetEditBox_LUA( const char* pControlName )
{
	KTDXPROFILE();

    wstring wstrControlName;
    ConvertUtf8ToWCHAR( wstrControlName, pControlName );

	CKTDGUIEditBox* pControl = (CKTDGUIEditBox*)GetControl( wstrControlName.c_str() );

	if ( pControl == NULL )
		MessageBoxA( g_pKTDXApp->GetHWND(), pControlName, "GetEditBox_LUA 실패", MB_OK );

	return pControl;
}

CKTDGUIIMEEditBox* CKTDGUIDialog::GetIMEEdieBox_LUA( const char* pControlName )
{
	KTDXPROFILE();

    wstring wstrControlName;
    ConvertUtf8ToWCHAR( wstrControlName, pControlName );

	CKTDGUIIMEEditBox* pControl = (CKTDGUIIMEEditBox*)GetControl( wstrControlName.c_str() );

	if ( pControl == NULL )
		MessageBoxA( g_pKTDXApp->GetHWND(), pControlName, "GetIMEEdieBox_LUA 실패", MB_OK );

	return pControl;

}

CKTDGUIListBox*	CKTDGUIDialog::GetListBox_LUA( const char* pControlName )
{
	KTDXPROFILE();

    wstring wstrControlName;
    ConvertUtf8ToWCHAR( wstrControlName, pControlName );

	CKTDGUIListBox* pControl = (CKTDGUIListBox*)GetControl( wstrControlName.c_str() );

	if ( pControl == NULL )
		MessageBoxA( g_pKTDXApp->GetHWND(), pControlName, "GetListBox_LUA 실패", MB_OK );

	return pControl;
}

CKTDGUIRadioButton*	CKTDGUIDialog::GetRadioButton_LUA( const char* pControlName )
{
	KTDXPROFILE();

    wstring wstrControlName;
    ConvertUtf8ToWCHAR( wstrControlName, pControlName );

	CKTDGUIRadioButton* pControl = (CKTDGUIRadioButton*)GetControl( wstrControlName.c_str() );

	if ( pControl == NULL )
		MessageBoxA( g_pKTDXApp->GetHWND(), pControlName, "GetRadioButton_LUA 실패", MB_OK );

	return pControl;
}

CKTDGUIStatic*	CKTDGUIDialog::GetStatic_LUA( const char* pControlName )
{
	KTDXPROFILE();

    wstring wstrControlName;
    ConvertUtf8ToWCHAR( wstrControlName, pControlName );

	CKTDGUIStatic* pControl = (CKTDGUIStatic*)GetControl( wstrControlName.c_str() );

	if ( pControl == NULL )
		MessageBoxA( g_pKTDXApp->GetHWND(), pControlName, "GetStatic_LUA 실패", MB_OK );

	return pControl;
}

#ifdef NEW_SKILL_TREE_UI
CKTDGUISlot*	CKTDGUIDialog::GetSlot_LUA( const char* pControlName )
{
	KTDXPROFILE();

    wstring wstrControlName;
    ConvertUtf8ToWCHAR( wstrControlName, pControlName );

	CKTDGUISlot* pControl = (CKTDGUISlot*)GetControl( wstrControlName.c_str() );

	if ( pControl == NULL )
		MessageBoxA( g_pKTDXApp->GetHWND(), pControlName, "GetSlot_LUA 실패", MB_OK );

	return pControl;
}


CKTDGUISlot*		CKTDGUIDialog::GetSlotByID(int ID)
{
	KTDXPROFILE();

	if ( ID == -1 )
		return NULL;

	CKTDGUISlot* pSlot = NULL;
	
	//{{ seojt // 2009-1-7, 17:23
	ASSERT( ::GetCurrentThreadId() != g_pKTDXApp->GetDeviceManager()->GetThreadID() );
	//CSLock  lock(m_mcs);
	//}} seojt // 2009-1-7, 17:23

	//{{ dmlee 2009.3.22 critical section 범위를 변경했습니다.
	{
#ifdef DIALOG_CRITICAL_SECTION
		CSLock  lock(m_mcs);
#endif DIALOG_CRITICAL_SECTION

		if ( m_ControlsList.empty() == true )
			return pSlot;

		for(vector<CKTDGUIControl*>::iterator iter = m_ControlsList.begin(); iter<m_ControlsList.end(); ++iter)
		{
			CKTDGUIControl* pControl = *iter;
			if( pControl->GetType() == CKTDGUIControl::UCT_SLOT )
			{
				pSlot = (CKTDGUISlot*) pControl;
				if(pSlot->GetID() == ID)
				{
					return pSlot;
				}
			}
		}
	}
	//}} dmlee 2009.3.22 critical section 범위를 변경했습니다.


	// 못 찾았다

// 		string fileName;
// 		ConvertWCHARToChar( fileName, pControlName );
// 		fileName += " <- 이 컨트롤 찾기 실패! ";
// 		string dialogName;
// 		ConvertWCHARToChar( dialogName, m_Name.c_str() );
// 		fileName += "-부모 다이얼로그 이름 : ";
// 		fileName += dialogName;
// 		ErrorLogMsg( KEM_ERROR36, fileName.c_str() );

	return NULL;
	

}

#endif NEW_SKILL_TREE_UI

void CKTDGUIDialog::OnMouseLeave()
{
	KTDXPROFILE();

	if ( m_pControlMouseOver == NULL )
		return;

	// Handle mouse leaving the old control
	if( m_pControlMouseOver != NULL )
		m_pControlMouseOver->OnMouseLeave();

	m_pControlMouseOver = NULL;
}


CKTDGUIControl* CKTDGUIDialog::GetControlAtPoint( POINT pt )
{
	KTDXPROFILE();

	if( m_ControlsList.size() == 0 )
		return NULL;
	for ( int i = (int)m_ControlsList.size()-1; i >= 0 ; i-- )
	{
		CKTDGUIControl* pControl = m_ControlsList[i];
		if( pControl == NULL )
			continue;

		D3DXVECTOR2 offsetPos;
		offsetPos = g_pKTDXApp->ConvertByResolution( pControl->GetOffsetPos() );

		POINT mousePointFinal = pt;
		mousePointFinal.x -= (LONG)offsetPos.x;
		mousePointFinal.y -= (LONG)offsetPos.y;

		D3DXVECTOR2 mousePos = g_pKTDXApp->MouseConvertByResolution( (float)mousePointFinal.x, (float)mousePointFinal.y );
		mousePointFinal.x = (LONG)mousePos.x;
		mousePointFinal.y = (LONG)mousePos.y;

		if( pControl->ContainsPoint( mousePointFinal ) == true
			&& pControl->GetEnable() == true
			&& pControl->GetShow() == true
#ifdef NEW_SKILL_TREE_UI
			&& pControl->GetOutOfControlList() == false 
#endif
			)
		{
			return pControl;
		}
	}

	return NULL;
}

CKTDGUIControl* CKTDGUIDialog::GetControlAtPointForStatic( POINT pt )
{
	KTDXPROFILE();

	if( m_ControlsList.size() == 0 )
		return NULL;
	for ( int i = (int)m_ControlsList.size()-1; i >= 0 ; i-- )
	{
		CKTDGUIControl* pControl = m_ControlsList[i];
		if( pControl == NULL )
			continue;

		D3DXVECTOR2 offsetPos;
		offsetPos = g_pKTDXApp->ConvertByResolution( pControl->GetOffsetPos() );

		POINT mousePointFinal = pt;
		mousePointFinal.x -= (LONG)offsetPos.x;
		mousePointFinal.y -= (LONG)offsetPos.y;

		D3DXVECTOR2 mousePos = g_pKTDXApp->MouseConvertByResolution( (float)mousePointFinal.x, (float)mousePointFinal.y );
		mousePointFinal.x = (LONG)mousePos.x;
		mousePointFinal.y = (LONG)mousePos.y;

		if( pControl->ContainsPoint( mousePointFinal ) == true
			//&& pControl->GetEnable() == true
			&& pControl->GetShow() == true
#ifdef NEW_SKILL_TREE_UI
			&& pControl->GetOutOfControlList() == false
#endif
			)
		{
			return pControl;
		}
	}

	return NULL;
}

CKTDGUIControl*	CKTDGUIDialog::GetControlAtPointExceptStatic( POINT pt )
{
	KTDXPROFILE();

	if( m_ControlsList.size() == 0 )
		return NULL;
	for ( int i = (int)m_ControlsList.size()-1; i >= 0 ; i-- )
	{
		CKTDGUIControl* pControl = m_ControlsList[i];
		if( pControl == NULL )
			continue;

		if ( pControl->GetType() == CKTDGUIControl::UCT_STATIC )
			continue;

		D3DXVECTOR2 offsetPos;
		offsetPos = g_pKTDXApp->ConvertByResolution( pControl->GetOffsetPos() );

		POINT mousePointFinal = pt;
		mousePointFinal.x -= (LONG)offsetPos.x;
		mousePointFinal.y -= (LONG)offsetPos.y;

		D3DXVECTOR2 mousePos = g_pKTDXApp->MouseConvertByResolution( (float)mousePointFinal.x, (float)mousePointFinal.y );
		mousePointFinal.x = (LONG)mousePos.x;
		mousePointFinal.y = (LONG)mousePos.y;

		if( pControl->ContainsPoint( mousePointFinal ) == true
#ifdef SHOW_DESCRIPTION_WHEN_DISABLE
#else SHOW_DESCRIPTION_WHEN_DISABLE
			&& pControl->GetEnable() == true
#endif SHOW_DESCRIPTION_WHEN_DISABLE
			&& pControl->GetShow() == true 
#ifdef NEW_SKILL_TREE_UI
			&& pControl->GetOutOfControlList() == false 
#endif
			)
		{
			return pControl;
		}
	}

	return NULL;
}



CKTDGUIControl* CKTDGUIDialog::GetControlAtPointAbsolute( POINT pt )
{
	KTDXPROFILE();

	if( m_ControlsList.size() == 0 )
		return NULL;

	for ( int i = (int)m_ControlsList.size()-1; i >= 0 ; i-- )
	{
		CKTDGUIControl* pControl = m_ControlsList[i];
		if( pControl == NULL )
			continue;

		D3DXVECTOR2 offsetPos;
		offsetPos = g_pKTDXApp->ConvertByResolution( pControl->GetOffsetPos() );

		POINT mousePointFinal = pt;
		mousePointFinal.x -= (LONG)offsetPos.x;
		mousePointFinal.y -= (LONG)offsetPos.y;

		D3DXVECTOR2 mousePos = g_pKTDXApp->MouseConvertByResolution( (float)mousePointFinal.x, (float)mousePointFinal.y );
		mousePointFinal.x = (LONG)mousePos.x;
		mousePointFinal.y = (LONG)mousePos.y;

		if( pControl->ContainsPoint( mousePointFinal ) == true )
		{
			return pControl;
		}
	}

	return NULL;
}


void CKTDGUIDialog::SetName_LUA( const char* pName )
{
	KTDXPROFILE();

	//m_Name_MB = pName;
	wstring WName;
	ConvertUtf8ToWCHAR( WName, pName );
	SetName( WName.c_str() );
}

void CKTDGUIDialog::SetName( const WCHAR* pName )
{
	KTDXPROFILE();

	m_Name = pName; 
	//ConvertWCHARToChar( m_Name_MB, pName );
}

void CKTDGUIDialog::SetEnable( bool bEnable )
{
	KTDXPROFILE();
	m_bEnable = bEnable;
}

void CKTDGUIDialog::SetShow( bool bShow )
{
	KTDXPROFILE();
	if( bShow != m_bShow )
	{
		m_bShow = bShow;

		//////////////////////////////////
		if( bShow == true)
		{
			POINT mousePoint = { g_pKTDXApp->GetDIManager()->GetMouse()->GetXPos(), g_pKTDXApp->GetDIManager()->GetMouse()->GetYPos() };
			D3DXVECTOR2 resolPos;
			resolPos = g_pKTDXApp->ConvertByResolution( m_Pos );
			mousePoint.x -= (LONG)resolPos.x;
			mousePoint.y -= (LONG)resolPos.y;
			OnMouseMove( mousePoint );
		}  
		/////////////////////////////////
#ifdef FRONT_GUIDE_DESC
		else if ( m_pDialogGuideDesc != NULL )
			m_pDialogGuideDesc->SetShow( false );
#endif //FRONT_GUIDE_DESC

		if( false == GetModal() ) // modal dialog 이면
		{
			if( NULL != GetUIDialogManager() )
			{
				GetUIDialogManager()->CountModalDialog();
			}
		}
	}

#ifdef NEW_SKILL_TREE_UI
	if(CKTDGUISlot::s_pDragSlot != NULL)
	{
		if(CKTDGUISlot::s_pDragSlot->GetDialog() == this && bShow == false )
		{
			CKTDGUISlot::ReturnDraggingItem();
		}
	}
#endif

//{{ robobeg : 2009-01-07
	// 도킹 다이얼로그 처리
	//UpdateDockPos();
//}} robobeg : 2009-01-07
}

void CKTDGUIDialog::SetPos( const D3DXVECTOR2& pos )		
{ 
	KTDXPROFILE();

	m_Pos.x = pos.x; 
	m_Pos.y = pos.y; 

	if (  m_fAnimateTime < m_fChangeTime )
	{
		m_GoalPos.x = pos.x;
		m_GoalPos.y = pos.y;
	}
}

void CKTDGUIDialog::Move( D3DXVECTOR2 pos, D3DXCOLOR color, float fChangeTime, bool bDirect, 
						 bool bAutoDelete, bool bPingPong, int repeatNum
#ifdef MOVE_FADE_IN_OUT
						 , bool bFadeInOut
#endif MOVE_FADE_IN_OUT
						 )
{
	KTDXPROFILE();
/*
	if ( bDirect == true )
	{
		while ( m_queMoveItems.empty() != true )
		{
			m_queMoveItems.pop();
		}

		//m_queMoveItems.clear();

		m_fAnimateTime = 0.0f;

		m_GoalPos.x = pos.x;
		m_GoalPos.y = pos.y;

		m_GoalColor = color;
		m_fChangeTime = fChangeTime;
		m_bAutoDelete = bAutoDelete;
	}
	else
	{
		//큐에 집어넣어서 사용한다.

		DialogMoveItem tempMoveItem;
		tempMoveItem.pos.x = pos.x;
		tempMoveItem.pos.y = pos.y;
		tempMoveItem.color = color;
		tempMoveItem.fChangeTime = fChangeTime;
		tempMoveItem.bIsAutoDelete = bAutoDelete;

		m_queMoveItems.push( tempMoveItem );
	}	
	*/
#ifdef MOVE_FADE_IN_OUT
	m_bFadeInOut = bFadeInOut;
#endif MOVE_FADE_IN_OUT

	if ( bDirect == true )
	{
		//다이렉트는 반복횟수를 무시한다 무조건 한번

		while ( m_queMoveItems.empty() != true )
		{
			m_queMoveItems.pop();
		}

		m_fAnimateTime = 0.0f;

		if ( bPingPong == false )
		{	
			m_GoalPos.x = pos.x;
			m_GoalPos.y = pos.y;

			m_GoalColor = color;
			m_fChangeTime = fChangeTime;
			m_bAutoDelete = bAutoDelete;
		}
		else
		{
			m_GoalPos.x = pos.x;
			m_GoalPos.y = pos.y;

			m_GoalColor = color;
			m_fChangeTime = fChangeTime;
			m_bAutoDelete = false;

			DialogMoveItem tempMoveItem;
			tempMoveItem.pos.x = m_Pos.x;
			tempMoveItem.pos.y = m_Pos.y;
			tempMoveItem.color = m_Color;
			tempMoveItem.fChangeTime = fChangeTime;
			tempMoveItem.bIsAutoDelete = bAutoDelete;

			m_queMoveItems.push( tempMoveItem );
		}



	}
	else
	{
		//큐에 집어넣어서 사용한다.
		for ( int i = 0; i < repeatNum; i++ )
		{
			DialogMoveItem tempMoveItem;
			if ( bPingPong == false )
			{

				tempMoveItem.pos.x = pos.x;
				tempMoveItem.pos.y = pos.y;
				tempMoveItem.color = color;
				tempMoveItem.fChangeTime = fChangeTime;
				tempMoveItem.bIsAutoDelete = bAutoDelete;

				m_queMoveItems.push( tempMoveItem );
			}
			else
			{
				tempMoveItem.pos.x = pos.x;
				tempMoveItem.pos.y = pos.y;
				tempMoveItem.color = color;
				tempMoveItem.fChangeTime = fChangeTime;
				tempMoveItem.bIsAutoDelete = false;

				m_queMoveItems.push( tempMoveItem );

				tempMoveItem.pos.x = m_Pos.x;
				tempMoveItem.pos.y = m_Pos.y;
				tempMoveItem.color = m_Color;
				tempMoveItem.fChangeTime = fChangeTime;
				tempMoveItem.bIsAutoDelete = bAutoDelete;

				m_queMoveItems.push( tempMoveItem );
			}
		}
	}	
}

void CKTDGUIDialog::Move_LUA()
{
	KTDXPROFILE();
	
	bool bDirect;
	float fChangeTime;
	bool bAutoDelete;



	KLuaManager luaManager( g_pKTDXApp->GetLuaBinder()->GetLuaState() );
#ifndef X2OPTIMIZE_AVOID_LUA_RUNTIME_INTERPRETING
	TableBind( &luaManager, g_pKTDXApp->GetLuaBinder() );
#endif  X2OPTIMIZE_AVOID_LUA_RUNTIME_INTERPRETING


	LUA_GET_VALUE( luaManager, "IS_DIRECT", bDirect, false );
	LUA_GET_VALUE( luaManager, "CHANGE_TIME", fChangeTime, 0.0f );

	LUA_GET_VALUE( luaManager, "AUTO_DELETE", bAutoDelete, false );

#ifdef  X2OPTIMIZE_AVOID_LUA_RUNTIME_INTERPRETING
    D3DXVECTOR2 pos;
    D3DXCOLOR color;
    LUA_GET_USER_DEFINED_TYPE_VALUE( luaManager, "POS", pos, D3DXVECTOR2(0,0) );
    LUA_GET_USER_DEFINED_TYPE_VALUE( luaManager, "COLOR", color, D3DXCOLOR(1,1,1,1) );
#else   X2OPTIMIZE_AVOID_LUA_RUNTIME_INTERPRETING
	D3DXVECTOR2 pos	= lua_tinker::get<D3DXVECTOR2>( luaManager.GetLuaState(),  "POS" );
	D3DXCOLOR color = lua_tinker::get<D3DXCOLOR>( luaManager.GetLuaState(),  "COLOR" );
#endif  X2OPTIMIZE_AVOID_LUA_RUNTIME_INTERPRETING
    

	if ( bDirect == true )
	{
		while ( m_queMoveItems.empty() != true )
		{
			m_queMoveItems.pop();
		}

		m_fAnimateTime = 0.0f;

		m_GoalPos.x = pos.x;
		m_GoalPos.y = pos.y;

		m_GoalColor = color;
		m_fChangeTime = fChangeTime;
		m_bAutoDelete = bAutoDelete;
	}
	else

	{
		//큐에 집어넣어서 사용한다.

		DialogMoveItem tempMoveItem;
		tempMoveItem.pos.x = pos.x;
		tempMoveItem.pos.y = pos.y;
		tempMoveItem.color = color;
		tempMoveItem.fChangeTime = fChangeTime;
		tempMoveItem.bIsAutoDelete = bAutoDelete;

		m_queMoveItems.push( tempMoveItem );
	}	
}

void CKTDGUIDialog::MoveStop()
{
	KTDXPROFILE();

	m_fAnimateTime = 0.0f;
	m_fChangeTime = 0.0f;
	while ( m_queMoveItems.empty() == false )
	{
		m_queMoveItems.pop();
	}
}

void CKTDGUIDialog::AddDummyString_LUA( int iDummyString ) 
{
	KTDXPROFILE();

	wstring dummyString; 
	//ConvertUtf8ToWCHAR( dummyString, pDummyString); 
	dummyString = GET_STRING(iDummyString);
	m_DummyStringList.push_back( dummyString ); 
}



bool CKTDGUIDialog::OpenScriptFile( const WCHAR* pFilename )
{
	KTDXPROFILE();

	if ( pFilename == NULL )
		return false;

#ifdef LOG_LOAD_DLG_SCRIPT
	static wstring wstrPreFilename = L"";
	if( 0 != wstrPreFilename.compare( pFilename ) )
	{
		KLOG("OpenScriptFile.log")  << pFilename << fileout;
		wstrPreFilename = pFilename;
	}
#endif
	
	//lua_getfield( g_pKTDXApp->GetLuaBinder()->GetLuaState(), LUA_GLOBALSINDEX, "g_pStage" );
	//int iRefStage = lua_ref( g_pKTDXApp->GetLuaBinder()->GetLuaState(), true );
	//lua_pushnil( g_pKTDXApp->GetLuaBinder()->GetLuaState() );
	//lua_setfield( g_pKTDXApp->GetLuaBinder()->GetLuaState(), LUA_GLOBALSINDEX, "g_pStage" );


	//바인더에 등록
	lua_tinker::decl( g_pKTDXApp->GetLuaBinder()->GetLuaState(),  "g_pUIDialog",	this );
	//lua_tinker::decl( g_pKTDXApp->GetLuaBinder()->GetLuaState(),  "g_pStage",		m_pStage );			// UI dialog 관련해서 이것을 사용하는 부분이 없어서 comment out



	bool bResult = g_pKTDXApp->LoadLuaTinker( pFilename );



	//lua_getref( g_pKTDXApp->GetLuaBinder()->GetLuaState(), iRefStage );
	//lua_setfield( g_pKTDXApp->GetLuaBinder()->GetLuaState(), LUA_GLOBALSINDEX, "g_pStage" );
	//lua_unref( g_pKTDXApp->GetLuaBinder()->GetLuaState(), iRefStage );



	return bResult;
}

bool CKTDGUIDialog::GetIsMoving()
{
	KTDXPROFILE();

	return ( m_fAnimateTime != 0.0f || m_fChangeTime != 0.0f || m_queMoveItems.empty() == false );
}



void CKTDGUIDialog::SetMaximize( bool val )
{
	KTDXPROFILE();

	if ( m_bMaximize == val )
		return;

	m_bMaximize = val;

	for ( unsigned int iC = 0; iC < m_ControlsList.size(); ++iC )
	{
		CKTDGUIControl* pControl = m_ControlsList[iC];
		pControl->SetMaximize( val );
	}
}

void CKTDGUIDialog::SendCloseCustomUIEventID()
{
	KTDXPROFILE();

	if ( m_pStage != NULL )
		m_pStage->MsgProc( g_pKTDXApp->GetHWND(), CKTDXApp::KM_UI_CONTROL_CUSTOM_EVENT, m_CloseCustomUIEventID, (LPARAM)this );
}


void CKTDGUIDialog::ShowGuideDesc( CKTDGUIControl* pControl )
{
    //{{ seojt // 2009-1-7, 17:23
    ASSERT( ::GetCurrentThreadId() != g_pKTDXApp->GetDeviceManager()->GetThreadID() );
#ifdef DIALOG_CRITICAL_SECTION
	CSLock  lock(m_mcs);
#endif DIALOG_CRITICAL_SECTION

    //}} seojt // 2009-1-7, 17:23

	KTDXPROFILE();

	CreateGuideDesc();
	//ChangeSequence( m_pStaticGuideDesc, pControl, true ); 
	ChangeSequence( m_pStaticGuideDesc, true );			// 090422 태완 : 툴팁이 다이얼로그의 다른 컨트롤에 가려지던 문제 수정
	m_pStaticGuideDesc->SetShow( true );
#ifdef FRONT_GUIDE_DESC
	m_pDialogGuideDesc->SetShow( true );
	m_pDialogGuideDesc->SetPos( GetPos() );
#endif //FRONT_GUIDE_DESC

	wstring guideDesc = pControl->GetGuideDesc();
	CKTDGFontManager::CUKFont* pItemDescFont = g_pKTDXApp->GetDGManager()->GetDialogManager()->GetUKFont( 2 );
#ifdef FIX_TOOLTIP
	const int iWidth = static_cast<int>( DXUTGetBackBufferSurfaceDesc()->Width / g_pKTDXApp->GetResolutionScaleX() );
	int itemDescWidth = (int)( pItemDescFont->GetWidth( guideDesc.c_str() ) / g_pKTDXApp->GetResolutionScaleX() ) + 12;
#else
	int itemDescWidth = (int)( pItemDescFont->GetWidth( guideDesc.c_str() ) / g_pKTDXApp->GetResolutionScaleX() ) + 12;
#endif FIX_TOOLTIP
	int itemDescHeight = (int)( pItemDescFont->GetHeight( guideDesc.c_str() ) / g_pKTDXApp->GetResolutionScaleY() ) + 12;


	int offsetSize = 0;
	int lineNum = 0;
	offsetSize = guideDesc.find( L"\n", offsetSize );
	while( offsetSize != -1 )
	{
		offsetSize++;
		lineNum++;
		offsetSize = guideDesc.find( L"\n", offsetSize );
	}
	itemDescHeight += lineNum;

	int itemDescPosX = 0;
	int itemDescPosY = 0;

	D3DXVECTOR2 offsetPos;

    if(pControl->GetGuideDescLT() == true)
    {
        offsetPos.x = pControl->GetGuideDescPos().x;
        offsetPos.y = pControl->GetGuideDescPos().y;
    }
    else
    {
#ifdef FIX_TOOLTIP
		offsetPos.x = pControl->GetGuideDescPos().x + GetPos().x - (itemDescWidth/2.0f);
#else 
        offsetPos.x = pControl->GetGuideDescPos().x - (itemDescWidth/2.0f);
#endif FIX_TOOLTIP
        offsetPos.y = pControl->GetGuideDescPos().y - (itemDescHeight );
    }
	

	if ( offsetPos.x >= 0 ) //왼쪽에 공간이 충분한 경우
	{
#ifdef FIX_TOOLTIP
		if ( offsetPos.x + itemDescWidth >= iWidth )
			itemDescPosX = static_cast<int>( iWidth - itemDescWidth );
#else
		if ( offsetPos.x + itemDescWidth > 1024 )
			itemDescPosX = 1024 - itemDescWidth;
#endif FIX_TOOLTIP
		else
			itemDescPosX = (int)offsetPos.x;
	}
	else
	{
		itemDescPosX = 0;
	}

	if ( offsetPos.y >= 0 ) 
	{
		itemDescPosY = (int)offsetPos.y;
	}
	else
	{
		itemDescPosY = 4;
	}

	if ( pControl->GetIsGuideDescAbsolutePos() == true )
	{
		m_pStaticGuideDesc->SetOffsetPos( D3DXVECTOR2( (float)itemDescPosX + pControl->GetOffsetPos().x - GetPos().x, 
			(float)itemDescPosY + pControl->GetOffsetPos().y - GetPos().y ) );

	}
	else
	{
#ifdef FIX_TOOLTIP
		m_pStaticGuideDesc->SetOffsetPos( D3DXVECTOR2( (float)itemDescPosX - GetPos().x, (float)itemDescPosY + pControl->GetOffsetPos().y ) );
#else
		m_pStaticGuideDesc->SetOffsetPos( D3DXVECTOR2( (float)itemDescPosX + pControl->GetOffsetPos().x, (float)itemDescPosY + pControl->GetOffsetPos().y ) );
#endif FIX_TOOLTIP
		
	}
	m_pStaticGuideDesc->GetPicture(0)->SetSize( D3DXVECTOR2( (float)itemDescWidth, (float)itemDescHeight) );
	m_pStaticGuideDesc->GetPicture(0)->pPoint->color = D3DXCOLOR( 0,0,0,0.7f );
	
	m_pStaticGuideDesc->GetString(0)->msg = guideDesc.c_str();
}
void CKTDGUIDialog::ClearRadioButtonGroup( int iButtonGroup )
{
	KTDXPROFILE();

	// Find all radio buttons with the given group number
	for( UINT i=0; i<m_ControlsList.size(); i++ )
	{
		CKTDGUIControl* pControl = m_ControlsList[i];
		if( pControl == NULL )
			continue;

		if( pControl->GetType() == CKTDGUIControl::UCT_RADIOBUTTON )
		{
			CKTDGUIRadioButton* pRadioButton = (CKTDGUIRadioButton*) pControl;
			
			if( pRadioButton->GetGroupID() == iButtonGroup )
			{
				pRadioButton->SetChecked( false );
			}
		}
	}
}

bool CKTDGUIDialog::SendInternelEvent( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	if( GetStage() == NULL )
	{
		return false;
	}

	CKTDXStage* pStage = GetStage();

	if ( g_pKTDXApp->CheckStage( pStage ) == true )
	{
		pStage->MsgProc( hWnd, uMsg, wParam, lParam );
	}

	return true;
}

void CKTDGUIDialog::UpdateRects()
{
	for( int i = 0; i < (int)m_ControlsList.size(); i++ )
	{
		CKTDGUIControl* pControl = m_ControlsList[i];
		if( NULL != pControl )
		{
			switch( pControl->GetType() )
			{
			case CKTDGUIControl::UCT_LISTBOX:
				{
					CKTDGUIListBox* pListBox = (CKTDGUIListBox*) pControl;
					pListBox->UpdateRects();
				} break;

			case CKTDGUIControl::UCT_COMBOBOX:
				{
					CKTDGUIComboBox* pComboBox = (CKTDGUIComboBox*) pControl;
					pComboBox->UpdateRects();
				} break;
#ifdef NEW_SKILL_TREE_UI
			case CKTDGUIControl::UCT_CONTROLLIST:
				{
					CKTDGUIContolList* pControlList = (CKTDGUIContolList*) pControl;
					pControlList->UpdateRects();

				} break;
#endif
			}
		}
	}
}



void CKTDGUIDialog::MoveControls( float fx, float fy )
{
	for( int i = 0; i < (int)m_ControlsList.size(); i++ )
	{
		CKTDGUIControl* pControl = m_ControlsList[i];
		if( NULL == pControl )
			continue;

		pControl->MoveControl( fx, fy );
	}
}


void CKTDGUIDialog::ScaleControls( float fx, float fy )
{
	for( int i = 0; i < (int)m_ControlsList.size(); i++ )
	{
		CKTDGUIControl* pControl = m_ControlsList[i];
		if( NULL == pControl )
			continue;

		pControl->ScaleControl( fx, fy );
	}
}
void CKTDGUIDialog::SetHasUnit(CKTDGObject *pUnit)
{
	if(pUnit == NULL)
	{
		m_bHasUnit = false;
		m_pUnitViewer = NULL;
		return;
	}

	m_pUnitViewer = pUnit;
	m_bHasUnit = true;
}

#ifdef BOOST_SINGLETON_POOL_TEST
    /** UI dialog를 handle로 사용하도록 하는 작업을 진행하면서,
        operator new의 overloading에 문제가 있어서 서진택이 주석처리하다.
        - jintaeks on 2009-08-20, 11:03 */
	//typedef boost::singleton_pool<boost::pool_allocator_tag, sizeof(CKTDGUIDialog)> CKTDGUIDialogPool;

	//void* CKTDGUIDialog::operator new(size_t s)
	//{
	//	return CKTDGUIDialogPool::malloc();
	//}

	//void CKTDGUIDialog::operator delete(void *p)
	//{
	//	CKTDGUIDialogPool::free(p);
	//}
#endif BOOST_SINGLETON_POOL_TEST



void    CKTDGUIDialog::SetLayer( int iLayer )
{ 
    if ( iLayer == m_iLayer )
        return;

    if ( GetUIDialogManager() != NULL )
	{
        GetUIDialogManager()->ChangeLayer( this, iLayer );
	}
    else
    {
        m_iLayer = iLayer;
    }//if
}

void    CKTDGUIDialog::SetFront( bool bFront )
{ 
    if ( m_bFront == bFront )
        return;

    if ( GetUIDialogManager() != NULL )
        GetUIDialogManager()->ChangeFront( this, bFront );
    else
        m_bFront = bFront;
}

bool CKTDGUIDialog::GetIsOverControl()
{ 
	if(m_bDisableUnderWindow == false)
		return true;

	if(m_bShow == false)
		return false;

	return m_bIsOverControl; 
}

void CKTDGUIDialog::SetIsOverControl(bool val)
{
	if(m_bDisableUnderWindow == false)
		return;
	m_bIsOverControl = val;
}



#ifdef PRIORITY_EVERY_UI
CKTDGUIControl* CKTDGUIDialog::GetControlMouseOver() const
{
	if( GetEnable() == false )
		return NULL;

	if( GetShow() == false )
		return NULL;

	float fAlpha = GetColor().a;
	
	if( GetColor().a < 0.01f )
		return NULL;

	if( m_pControlMouseOverStatic == NULL )
		return NULL;

	if( m_pControlMouseOverStatic->GetName() == std::wstring( L"INVISIBLE" ) )
		return NULL;

	return m_pControlMouseOverStatic;
}
#endif PRIORITY_EVERY_UI

#ifdef PARTICLE_RENDER_BY_DIALOG
void CKTDGUIDialog::AddParticle(CKTDGObject *pObj)
{
	if(pObj == NULL)
	{
		return;
	}
	m_ParticleList.push_back(pObj);
	m_bHasParticle = true;
}
void CKTDGUIDialog::ClearParticleList()
{
	m_ParticleList.clear();
	m_bHasParticle = false;
}
#endif //PARTICLE_RENDER_BY_DIALOG

#ifdef REFORM_ENTRY_POINT	 	// 13-11-11, 진입 구조 개편, kimjh
void CKTDGUIDialog::SetCustomButtonMouseOverSndFile_LUA( const char* pCustomSndFileName )
{
	ConvertUtf8ToWCHAR( m_wstrCustomButtonMouseOverSndFileName, pCustomSndFileName );
}
void CKTDGUIDialog::SetCustomButtonMouseUpSndFile_LUA( const char* pCustomSndFileName )
{
	ConvertUtf8ToWCHAR( m_wstrCustomButtonMouseUpSndFileName, pCustomSndFileName );
}

void CKTDGUIDialog::SetDownStateToAllButtonType()
{
	for ( int iSize = 0; iSize < GetControlSize(); ++iSize )
	{
		CKTDGUIControl* pControl = GetControl( iSize );
		if ( NULL != pControl )
		{
			if ( pControl->GetType () == CKTDGUIControl::UCT_BUTTON ) 
			{
				static_cast <CKTDGUIButton *> ( pControl )->SetDownStateAtNormal ( false );
			}
		}
	}
}
#endif // REFORM_ENTRY_POINT	// 13-11-11, 진입 구조 개편, kimjh

#ifdef SET_SHOW_CONTROLS_WITH_DUMMYINT
void CKTDGUIDialog::SetShowEnableControlsWithDummyInt( IN int iIndex, IN int iValue, IN bool bShow, IN bool bEnable )
{
	vector< CKTDGUIControl* >::iterator vitControlsList;

	for( vitControlsList = m_ControlsList.begin(); vitControlsList != m_ControlsList.end(); ++vitControlsList )
	{
		CKTDGUIControl* pControl = *vitControlsList;
		if( pControl->GetDummyInt( iIndex ) == iValue )
		{
			pControl->SetShowEnable( bShow, bEnable );
		}
	}
}
#endif SET_SHOW_CONTROLS_WITH_DUMMYINT
