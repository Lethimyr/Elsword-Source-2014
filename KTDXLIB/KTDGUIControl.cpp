#include "StdAfx.h"
#include ".\ktdguicontrol.h"

CKTDGUIControl::CKTDGUIControl()
{
	m_pDialog		= NULL;
	m_ControlType	= UCT_NONE;	

	INIT_VECTOR2( m_OffsetPos, 0.0f, 0.0f );
	INIT_VECTOR2( m_Size, 0.0f, 0.0f );
	m_Color	= D3DXCOLOR(0xffffffff);

	m_bHasFocus		= false;
	m_bMouseOver	= false;
	m_bEnable		= true;
	m_bShow			= true;
	m_bSound		= true;
#ifdef DLL_BUILD
	m_bUpdate		= true;
#endif
	m_bMaximize		= false;
#ifdef NEW_SKILL_TREE_UI
	m_bOutofList	= false;
#endif

#ifdef SHOW_DESCRIPTION_WHEN_DISABLE
	m_bShowDescriptionWhenDisable	= false;
#endif SHOW_DESCRIPTION_WHEN_DISABLE
	

	m_kMaximizeOffset.x = 0;
	m_kMaximizeOffset.y = 0;

	m_fAnimateTime			= 0.0f;
	INIT_VECTOR2( m_GoalPos, 0.0f, 0.0f );
	m_GoalColor				= D3DXCOLOR(0xffffffff);
	m_fChangeTime			= 0.0f;
	m_bAutoDelete			= false;
	m_bCheckAutoDeleteEnd	= false;

	m_fElapsedTime	= 0.0f;

	m_vEnableLeftTop		= D3DXVECTOR2( 0, 0 );
	m_vEnableRightBottom	= D3DXVECTOR2( 0, 0 );

	m_GuideDesc = L"";
	m_vGuideDescOffsetPos = D3DXVECTOR2( 0,0 );
	m_vGuideDescAbsolutePos = D3DXVECTOR2( 0,0 );

    m_bGuideDescPosLT = false;

//#ifndef DYNAMIC_VERTEX_BUFFER_OPT
//	m_pVB = NULL;
//	HRESULT hr = g_pKTDXApp->GetDevice()->CreateVertexBuffer( 4 * sizeof(CKTDGUIControl::VERTEX_UI), 
//		D3DUSAGE_WRITEONLY, D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1, 
//		D3DPOOL_MANAGED, &m_pVB, NULL );
//	ASSERT( SUCCEEDED( hr ) );
//#endif
}

CKTDGUIControl::~CKTDGUIControl(void)
{
	m_pDialog		= NULL;
	m_Name.clear();
	//m_Name_MB.clear();

//#ifndef DYNAMIC_VERTEX_BUFFER_OPT
//	SAFE_RELEASE( m_pVB );
//#endif
}

HRESULT	CKTDGUIControl::OnFrameMove( double fTime, float fElapsedTime )
{
	KTDXPROFILE();

	if( m_bShow == false )
		return S_OK;
	
	m_fElapsedTime = fElapsedTime;
	
	Animate();

	return S_OK;
}

HRESULT	CKTDGUIControl::OnFrameRender()
{	
	KTDXPROFILE();

	return S_OK;
}

void CKTDGUIControl::RequestFocus()
{ 
	if ( m_pDialog != NULL ) 
		m_pDialog->RequestFocus(this); 
}

bool CKTDGUIControl::ContainsPoint( POINT pt )
{	
	/*
	D3DXVECTOR2 arPoint[4];
	


	arPoint[0] = m_NowPoint.leftTopPoint + m_OffsetPos;
	arPoint[1] = m_NowPoint.rightTopPoint + m_OffsetPos;
	arPoint[2] = m_NowPoint.leftBottomPoint + m_OffsetPos;
	arPoint[3] = m_NowPoint.rightBottomPoint + m_OffsetPos;

	return Pick2DRect( pt, arPoint[0], arPoint[1], arPoint[2], arPoint[3] ); */

	return Pick2DRect( pt, m_NowPoint.leftTopPoint, m_NowPoint.rightTopPoint, m_NowPoint.leftBottomPoint ,m_NowPoint.rightBottomPoint );
}

bool CKTDGUIControl::SendInternelEvent( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )	// KM_UI_CONTROL_CUSTOM_EVENT, 14035, this
{
	if( m_pDialog == NULL || m_pDialog->GetStage() == NULL )
	{
		return false;
	}

	CKTDXStage* pStage = m_pDialog->GetStage();

	if ( g_pKTDXApp->CheckStage( pStage ) == true )
	{
		pStage->MsgProc( hWnd, uMsg, wParam, lParam );
	}

	return true;
}

void CKTDGUIControl::SetName_LUA( const char* pName )
{
	wstring WName;
	ConvertUtf8ToWCHAR( WName, pName );
	SetName( WName.c_str() );
}

bool CKTDGUIControl::SetName( const WCHAR* pName )
{ 
	m_Name = pName;
	//ConvertWCHARToChar( m_Name_MB, pName );
	if( m_pDialog != NULL )
	{
		return m_pDialog->ReNameControl( this );
	}

	return true;
}

void CKTDGUIControl::SetNameByForce( const WCHAR* pName )
{ 
	m_Name = pName; 
	//ConvertWCHARToChar( m_Name_MB, pName );
}

#ifdef CONTROL_SET_NAME_STR_ID
bool CKTDGUIControl::SetNameStrID_LUA( int iIndex )
{
	m_Name = GET_STRING( iIndex );

	if( m_pDialog != NULL )
	{
		return m_pDialog->ReNameControl( this );
	}

	return true;
}
#endif CONTROL_SET_NAME_STR_ID

void CKTDGUIControl::PlaySound( CKTDXDeviceSound* pSound )
{
	if ( m_bSound == false )
		return;

	if ( pSound != NULL )
		pSound->Play( false, false );
}

void CKTDGUIControl::Move( const D3DXVECTOR2& pos, D3DXCOLOR color, float fChangeTime, bool bDirect, bool bAutoDelete,
						  bool bPingPong, int repeatNum )
{
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

			ControlMoveItem tempMoveItem;
			tempMoveItem.pos.x = m_OffsetPos.x;
			tempMoveItem.pos.y = m_OffsetPos.y;
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
			ControlMoveItem tempMoveItem;
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

				tempMoveItem.pos.x = m_OffsetPos.x;
				tempMoveItem.pos.y = m_OffsetPos.y;
				tempMoveItem.color = m_Color;
				tempMoveItem.fChangeTime = fChangeTime;
				tempMoveItem.bIsAutoDelete = bAutoDelete;

				m_queMoveItems.push( tempMoveItem );
			}
		}
	}	
}

void CKTDGUIControl::Move_LUA()
{



	KLuaManager luaManager( g_pKTDXApp->GetLuaBinder()->GetLuaState() );
#ifndef X2OPTIMIZE_AVOID_LUA_RUNTIME_INTERPRETING
	TableBind( &luaManager, g_pKTDXApp->GetLuaBinder() );
#endif  X2OPTIMIZE_AVOID_LUA_RUNTIME_INTERPRETING

	bool bDirect;
	float fChangeTime;
	bool bAutoDelete;
	int repeatNum;
	bool bPingPong;

	LUA_GET_VALUE( luaManager, "IS_DIRECT", bDirect, false );
	LUA_GET_VALUE( luaManager, "CHANGE_TIME", fChangeTime, 0.0f );

	LUA_GET_VALUE( luaManager, "AUTO_DELETE", bAutoDelete, false );

	LUA_GET_VALUE( luaManager, "IS_PINGPONG", bPingPong, false );
	LUA_GET_VALUE( luaManager, "REPEAT_NUM", repeatNum, 1 );

#ifdef  X2OPTIMIZE_AVOID_LUA_RUNTIME_INTERPRETING
    D3DXVECTOR2    pos;
    D3DXCOLOR       color;
    LUA_GET_USER_DEFINED_TYPE_VALUE( luaManager, "POS", pos, D3DXVECTOR2(0,0) );
    LUA_GET_USER_DEFINED_TYPE_VALUE( luaManager, "COLOR", color, D3DXCOLOR(1,1,1,1) );
#else   X2OPTIMIZE_AVOID_LUA_RUNTIME_INTERPRETING
	D3DXVECTOR2 pos	= lua_tinker::get<D3DXVECTOR2>( luaManager.GetLuaState(),  "POS" );
	D3DXCOLOR color = lua_tinker::get<D3DXCOLOR>( luaManager.GetLuaState(),  "COLOR" );
#endif  X2OPTIMIZE_AVOID_LUA_RUNTIME_INTERPRETING

	Move( pos, color, fChangeTime, bDirect, bAutoDelete, bPingPong, repeatNum );

}

void CKTDGUIControl::MoveStop()
{
	m_fAnimateTime = 0.0f;
	m_fChangeTime = 0.0f;
	while ( m_queMoveItems.empty() == false )
	{
		m_queMoveItems.pop();
	}
}

void CKTDGUIControl::Animate()
{
	KTDXPROFILE();

	if ( m_fChangeTime > 0.0f )
	{
		if ( m_fAnimateTime >= m_fChangeTime )
		{
			m_fAnimateTime = 0.0f;
			m_fChangeTime = 0.0f;

			m_OffsetPos = m_GoalPos;

			if ( m_bAutoDelete == true )
			{
				m_bCheckAutoDeleteEnd = true;
			}
		}
		else
		{
			float _fRemainTime = m_fChangeTime - m_fAnimateTime;

			D3DXVECTOR2 remainPos		= m_GoalPos - m_OffsetPos;
			m_OffsetPos		+= (remainPos / _fRemainTime) * m_fElapsedTime;

			D3DXCOLOR remainColor = m_GoalColor - m_Color;
			m_Color += (remainColor / _fRemainTime)	* m_fElapsedTime;

			m_fAnimateTime += m_fElapsedTime;
		}
	}

	if ( m_fChangeTime == 0.0f )
	{
		if ( m_queMoveItems.empty() == false )
		{
			ControlMoveItem tempMoveItem; 
			tempMoveItem = m_queMoveItems.front();

			m_GoalPos = tempMoveItem.pos;
			m_GoalColor = tempMoveItem.color;
			m_fChangeTime = tempMoveItem.fChangeTime;
			m_bAutoDelete = tempMoveItem.bIsAutoDelete;

			m_queMoveItems.pop();
		}
	}

}


void CKTDGUIControl::SetTabControlName_LUA( const char* pTapControlName )
{
	wstring WName;
	ConvertUtf8ToWCHAR( WName, pTapControlName );
	SetTabControlName( WName.c_str() );	
}

void CKTDGUIControl::SetPrevTabControlName_LUA( const char* pTapControlName )
{
	wstring WName;
	ConvertUtf8ToWCHAR( WName, pTapControlName );
	SetPrevTabControlName( WName.c_str() );	
}

void CKTDGUIControl::ChangePoint( UIPointData* pNowPoint, UIPointData* pEndPoint )
{
	KTDXPROFILE();

	if( pEndPoint->fChangeTime == pNowPoint->fChangeTime )
		return;

	float fRemainTime = pEndPoint->fChangeTime - pNowPoint->fChangeTime;

	if( fRemainTime <= m_fElapsedTime )
	{
		*pNowPoint				= *pEndPoint;
	}
	else
	{
		D3DXCOLOR remainColor		= pEndPoint->color - pNowPoint->color;
		pNowPoint->color			+= (remainColor / fRemainTime)	* m_fElapsedTime;

		D3DXVECTOR2 remainPos		= pEndPoint->leftTopPoint - pNowPoint->leftTopPoint;
		pNowPoint->leftTopPoint		+= (remainPos / fRemainTime) * m_fElapsedTime;
		remainPos					= pEndPoint->rightTopPoint - pNowPoint->rightTopPoint;
		pNowPoint->rightTopPoint	+= (remainPos / fRemainTime) * m_fElapsedTime;
		remainPos					= pEndPoint->leftBottomPoint - pNowPoint->leftBottomPoint;
		pNowPoint->leftBottomPoint	+= (remainPos / fRemainTime) * m_fElapsedTime;
		remainPos					= pEndPoint->rightBottomPoint - pNowPoint->rightBottomPoint;
		pNowPoint->rightBottomPoint	+= (remainPos / fRemainTime) * m_fElapsedTime;

		pNowPoint->fChangeTime		+= m_fElapsedTime;
	}
}

void CKTDGUIControl::UpdateVertex( VERTEX_UI& vertexLT, VERTEX_UI& vertexRT, VERTEX_UI& vertexLB, VERTEX_UI& vertexRB, 
								  const UIPointData& pointData  )
{
	KTDXPROFILE();


    if( m_pDialog == NULL )
    {
        return;
    }

	CKTDGUIDialog* pDialog = m_pDialog->Get();



	D3DXCOLOR tempColor;
	tempColor.a = pointData.color.a * m_pDialog->GetColor().a * m_Color.a;
	tempColor.r = pointData.color.r * m_pDialog->GetColor().r * m_Color.r;
	tempColor.g = pointData.color.g * m_pDialog->GetColor().g * m_Color.g;
	tempColor.b = pointData.color.b * m_pDialog->GetColor().b * m_Color.b;

	

	//0---1
	//2---3
	//Set up the 4 corners of a small square
 	vertexLT.x		= (pointData.leftTopPoint.x + pDialog->GetPos().x + m_OffsetPos.x) * g_pKTDXApp->GetResolutionScaleX();
 	vertexLT.y		= (pointData.leftTopPoint.y + pDialog->GetPos().y + m_OffsetPos.y) * g_pKTDXApp->GetResolutionScaleY();
	vertexLT.z		= 1.0f;   
	vertexLT.rhw	= 1.0f;
	vertexLT.color	= tempColor;


 	vertexRT.x		= (pointData.rightTopPoint.x + pDialog->GetPos().x + m_OffsetPos.x) * g_pKTDXApp->GetResolutionScaleX();     
 	vertexRT.y		= (pointData.rightTopPoint.y + pDialog->GetPos().y + m_OffsetPos.y) * g_pKTDXApp->GetResolutionScaleY();
	vertexRT.z		= 1.0f;   
	vertexRT.rhw	= 1.0f;
	vertexRT.color	= tempColor;


 	vertexLB.x		= (pointData.leftBottomPoint.x + pDialog->GetPos().x + m_OffsetPos.x) * g_pKTDXApp->GetResolutionScaleX();     
 	vertexLB.y		= (pointData.leftBottomPoint.y + pDialog->GetPos().y + m_OffsetPos.y) * g_pKTDXApp->GetResolutionScaleY();
	vertexLB.z		= 1.0f;   
	vertexLB.rhw	= 1.0f;
	vertexLB.color	= tempColor;


 	vertexRB.x		= (pointData.rightBottomPoint.x + pDialog->GetPos().x + m_OffsetPos.x) * g_pKTDXApp->GetResolutionScaleX();     
 	vertexRB.y		= (pointData.rightBottomPoint.y + pDialog->GetPos().y + m_OffsetPos.y) * g_pKTDXApp->GetResolutionScaleY();
	vertexRB.z		= 1.0f;   
	vertexRB.rhw	= 1.0f;
	vertexRB.color	= tempColor;




	vertexLT.x	= (int)vertexLT.x - 0.5f;     
	vertexLT.y	= (int)vertexLT.y - 0.5f;

	vertexRT.x	= (int)vertexRT.x - 0.5f;     
	vertexRT.y	= (int)vertexRT.y - 0.5f;

	vertexLB.x	= (int)vertexLB.x - 0.5f;     
	vertexLB.y	= (int)vertexLB.y - 0.5f;

	vertexRB.x	= (int)vertexRB.x - 0.5f;     
	vertexRB.y	= (int)vertexRB.y - 0.5f;




	if( pointData.pUITextureData != NULL )
	{
		vertexLT.u	= pointData.pUITextureData->uvTexture[VP_LEFT_TOP].x;   
		vertexLT.v	= pointData.pUITextureData->uvTexture[VP_LEFT_TOP].y;

		vertexRT.u	= pointData.pUITextureData->uvTexture[VP_RIGHT_TOP].x;   
		vertexRT.v	= pointData.pUITextureData->uvTexture[VP_RIGHT_TOP].y;

		vertexLB.u	= pointData.pUITextureData->uvTexture[VP_LEFT_BOTTOM].x;   
		vertexLB.v	= pointData.pUITextureData->uvTexture[VP_LEFT_BOTTOM].y;

		vertexRB.u	= pointData.pUITextureData->uvTexture[VP_RIGHT_BOTTOM].x;   
		vertexRB.v	= pointData.pUITextureData->uvTexture[VP_RIGHT_BOTTOM].y;
	}

}



void CKTDGUIControl::UpdateAnimVertex( VERTEX_UI* pVertex, CKTDGUIControl::TexChangeData* pTexChangeData )
{
	if ( pVertex == NULL || pTexChangeData == NULL )
		return;

	D3DXMATRIX matOutPut;
	D3DXVECTOR2 tempTrans = pTexChangeData->m_Pos;
	D3DXVECTOR2 rotationAxisPos = D3DXVECTOR2( 0,0 );
	D3DXVECTOR2 scaleAxisPos = D3DXVECTOR2(0,0);

	rotationAxisPos = GetAxisPos( pVertex, pTexChangeData->m_RotateAxis );
	if ( pTexChangeData->m_RotateAxis == CKTDGUIControl::TA_WORLD )
	{
		rotationAxisPos = pTexChangeData->m_RotateAxisPos;
	}

	scaleAxisPos = GetAxisPos( pVertex, pTexChangeData->m_ScaleAxis );
	if ( pTexChangeData->m_ScaleAxis == CKTDGUIControl::TA_WORLD )
	{
		scaleAxisPos = pTexChangeData->m_ScaleAxisPos;
	}

	D3DXMatrixTransformation2D( &matOutPut, &scaleAxisPos, D3DXToRadian( pTexChangeData->m_fScaleRotationDegree ), &pTexChangeData->m_Scale, &rotationAxisPos, D3DXToRadian( pTexChangeData->m_fRotateDegree ), &tempTrans );

	D3DXVECTOR4 resultPos;
	D3DXVECTOR2 tempPos;

	for ( int i = 0; i < 4; i++ )
	{
		D3DXVECTOR4 resultPos;
		D3DXVECTOR2 tempPos;

		tempPos.x = pVertex[i].x;
		tempPos.y = pVertex[i].y;
		D3DXVec2Transform( &resultPos, &tempPos, &matOutPut );
		pVertex[i].x = resultPos.x;
		pVertex[i].y = resultPos.y;
	}
}

D3DXVECTOR2 CKTDGUIControl::GetAxisPos( VERTEX_UI* pVertex, CKTDGUIControl::TEX_AXIS texAxis )
{
	D3DXVECTOR2 axisPos = D3DXVECTOR2(0,0);

	if ( pVertex == NULL )
		return axisPos;

	switch( texAxis ) 
	{
	case CKTDGUIControl::TA_TOP_LEFT:
		{
			axisPos = D3DXVECTOR2( pVertex[0].x, pVertex[0].y );
		}
		break;
	case CKTDGUIControl::TA_TOP_CENTER:
		{
			axisPos = D3DXVECTOR2( pVertex[0].x + (pVertex[1].x - pVertex[0].x)/2.0f, pVertex[0].y );
		}
		break;
	case CKTDGUIControl::TA_TOP_RIGHT:
		{
			axisPos = D3DXVECTOR2( pVertex[1].x, pVertex[0].y );
		}
		break;
	case CKTDGUIControl::TA_CENTER_LEFT:
		{
			axisPos = D3DXVECTOR2( pVertex[0].x, pVertex[0].y + (pVertex[2].y - pVertex[0].y)/2.0f );
		}
		break;
	case CKTDGUIControl::TA_CENTER_CENTER:
		{
			axisPos = D3DXVECTOR2( pVertex[0].x + (pVertex[1].x - pVertex[0].x)/2.0f, pVertex[0].y + (pVertex[2].y - pVertex[0].y)/2.0f);
		}
		break;
	case CKTDGUIControl::TA_CENTER_RIGHT:
		{
			axisPos = D3DXVECTOR2( pVertex[1].x, pVertex[0].y + (pVertex[2].y - pVertex[0].y)/2.0f);
		}
		break;
	case CKTDGUIControl::TA_BOTTOM_LEFT:
		{
			axisPos = D3DXVECTOR2( pVertex[0].x, pVertex[2].y );
		}
		break;
	case CKTDGUIControl::TA_BOTTOM_CENTER:
		{
			axisPos = D3DXVECTOR2( pVertex[0].x + (pVertex[1].x - pVertex[0].x)/2.0f, pVertex[2].y );
		}
		break;
	case CKTDGUIControl::TA_BOTTOM_RIGHT:
		{
			axisPos = D3DXVECTOR2( pVertex[3].x, pVertex[3].y );
		}
		break;
	}

	return axisPos;
}

//#ifdef DYNAMIC_VERTEX_BUFFER_OPT
HRESULT CKTDGUIControl::RenderVertex( VERTEX_UI* pVertex, bool bRestoreVertexDecl )
{
	CKTDGUIControl* pThis = this;

	ASSERT( pThis != NULL );

	if( NULL == g_pKTDXApp->GetDevice() )
	{
		ErrorLogMsg( KEM_ERROR130, "device null" );
		return E_FAIL; 
	}

	if( NULL == pVertex )
	{
		ErrorLogMsg( KEM_ERROR130, "null" );
		return E_FAIL; 
	}

//#ifndef DYNAMIC_VERTEX_BUFFER_OPT
//	HRESULT hr;
//#endif

	IDirect3DVertexDeclaration9 *pDecl = NULL;
	if ( bRestoreVertexDecl == true )
	{
		g_pKTDXApp->GetDevice()->GetVertexDeclaration( &pDecl );
	}//if

//#ifdef DYNAMIC_VERTEX_BUFFER_OPT
	BOOST_STATIC_ASSERT( D3DFVF_VERTEX_UI == D3DFVF_XYZRHW_DIFFUSE_TEX1 );
	g_pKTDXApp->GetDVBManager()->DrawPrimitive( CKTDGDynamicVBManager::DVB_TYPE_XYZRHW_DIFFUSE_TEX1
		, D3DPT_TRIANGLESTRIP, 2, pVertex );
//#else
//	g_pKTDXApp->GetDevice()->SetFVF( D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1 );
//	g_pKTDXApp->GetDevice()->DrawPrimitiveUP( D3DPT_TRIANGLESTRIP, 2, pVertex, sizeof(VERTEX_UI) );
//
//#endif

	if ( bRestoreVertexDecl == true )
	{
		g_pKTDXApp->GetDevice()->SetVertexDeclaration( pDecl );
		SAFE_RELEASE( pDecl );
	}//if

//#ifdef DYNAMIC_VERTEX_BUFFER_OPT
	return S_OK;
//#else
//	return hr;
//#endif
}//

//#endif

HRESULT CKTDGUIControl::RenderVertex( VERTEX_UI* pVertex, const UIPointData& pointData, int renderCount )
{
	KTDXPROFILE();

	if( NULL == g_pKTDXApp->GetDevice() )
	{
		ErrorLogMsg( KEM_ERROR130, "device null" );
		return E_FAIL; 
	}

	if( NULL == pVertex )
	{
		ErrorLogMsg( KEM_ERROR130, "null" );
		return E_FAIL; 
	}

	if( pointData.pUITextureData == NULL )
	{
		//ErrorLogMsg( KEM_ERROR130, m_Name.c_str() );
		return E_FAIL;
	}

	if( pointData.pUITextureData->pTexture == NULL )
	{
		ErrorLogMsg( KEM_ERROR131, m_Name.c_str() );
		return E_FAIL;
	}

	pointData.pUITextureData->pTexture->SetDeviceTexture();

//#ifdef DYNAMIC_VERTEX_BUFFER_OPT
	BOOST_STATIC_ASSERT( D3DFVF_VERTEX_UI == D3DFVF_XYZRHW_DIFFUSE_TEX1 );
	g_pKTDXApp->GetDVBManager()->DrawPrimitive( CKTDGDynamicVBManager::DVB_TYPE_XYZRHW_DIFFUSE_TEX1
		, D3DPT_TRIANGLESTRIP, 2, pVertex, renderCount );

	return S_OK;
//#else
//	HRESULT hr;
//	hr = g_pKTDXApp->GetDevice()->SetFVF( D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1 );
//
//	if ( hr != S_OK )
//	{
//		WCHAR buff[256] = {0};
//		StringCchPrintfW( buff, ARRAY_SIZE(buff), L"D3DERR_INVALIDCALL %s", m_Name.c_str() );
//		ErrorLogMsg( KEM_ERROR35, buff );
//
//		return hr;
//	}
//
//	g_pKTDXApp->GetDevice()->SetFVF(D3DFVF_VERTEX_UI );
//	for( int i = 0; i < renderCount; i++ )
//	{
//		g_pKTDXApp->GetDevice()->DrawPrimitiveUP( D3DPT_TRIANGLESTRIP, 2, pVertex, sizeof(VERTEX_UI) );
//	}//for
//
//	return hr;
//#endif
}

CKTDGUIControl::UITextureData* CKTDGUIControl::SetTexureData_( const WCHAR* pFileName, const WCHAR* pKeyName )
{
/*	
	KLuaManager luaManager( g_pKTDXApp->GetLuaBinder()->GetLuaState() );
	TableBind( &luaManager, g_pKTDXApp->GetLuaBinder() );

	string texName;
	LUA_GET_VALUE( luaManager, "TEX_NAME", texName, "" );
*/
	UITextureData* pTexData = new UITextureData();
	wstring key = pKeyName;
//	wstring WName;
//	ConvertCharToWCHAR( WName, texName.c_str() );
	pTexData->texName	= pFileName;
	pTexData->keyName	= pKeyName;
	pTexData->pTexture	= g_pKTDXApp->GetDeviceManager()->OpenTexture( pTexData->texName );
	if( pTexData->pTexture == NULL )
	{
		SAFE_DELETE( pTexData );
		return NULL;
	}

	MakeUpperCase(key);
	const CKTDXDeviceTexture::TEXTURE_UV* pTexUV = pTexData->pTexture->GetTexUV( key );
	if( pTexUV != NULL )
	{
		pTexData->uvOrgTexture[CKTDGUIControl::VP_LEFT_TOP]			= pTexUV->leftTop;
		pTexData->uvOrgTexture[CKTDGUIControl::VP_RIGHT_TOP]		= pTexUV->rightTop;
		pTexData->uvOrgTexture[CKTDGUIControl::VP_LEFT_BOTTOM]		= pTexUV->leftBottom;
		pTexData->uvOrgTexture[CKTDGUIControl::VP_RIGHT_BOTTOM]		= pTexUV->rightBottom;

		pTexData->texSize = pTexUV->rectSize;

		pTexData->SetTextureUV();
	}

	return pTexData;
}

CKTDGUIControl::UIPointData* CKTDGUIControl::SetPointData_()
{




	KLuaManager luaManager( g_pKTDXApp->GetLuaBinder()->GetLuaState() );
#ifndef X2OPTIMIZE_AVOID_LUA_RUNTIME_INTERPRETING
	TableBind( &luaManager, g_pKTDXApp->GetLuaBinder() );
#endif  X2OPTIMIZE_AVOID_LUA_RUNTIME_INTERPRETING

	UIPointData* pPointData = new UIPointData();

	LUA_GET_VALUE( luaManager, "USE_TEXTURE_SIZE", pPointData->bUseTextureSize, true );
	LUA_GET_VALUE( luaManager, "CHANGE_TIME", pPointData->fChangeTime, 0.0f );

	LUA_GET_VALUE( luaManager, "ADD_SIZE_X", pPointData->addSize.x, 0.0f );
	LUA_GET_VALUE( luaManager, "ADD_SIZE_Y", pPointData->addSize.y, 0.0f );

#ifdef DLL_BUILD
	LUA_GET_VALUE( luaManager, "ROTATION", pPointData->fRotDegree, 0.0f );
#endif

#ifdef RESIZE_TEXTURE
	LUA_GET_VALUE( luaManager, "RESIZE_X", pPointData->kResize.x, 1.0f );
	LUA_GET_VALUE( luaManager, "RESIZE_Y", pPointData->kResize.y, 1.0f );
#endif RESIZE_TEXTURE

	LUA_GET_VALUE( luaManager, "DRAW_COUNT", pPointData->m_iDrawCount, 1 );
	LUA_GET_VALUE_ENUM( luaManager, "SRC_BLEND",  pPointData->m_eSrcBlend,  D3DBLEND, D3DBLEND_SRCALPHA );
	LUA_GET_VALUE_ENUM( luaManager, "DEST_BLEND", pPointData->m_eDestBlend, D3DBLEND, D3DBLEND_INVSRCALPHA );


	bool bRect = true;
	LUA_GET_VALUE( luaManager, "IS_RECT", bRect, true );
#ifdef  X2OPTIMIZE_AVOID_LUA_RUNTIME_INTERPRETING
    LUA_GET_USER_DEFINED_TYPE_VALUE( luaManager, "LEFT_TOP", pPointData->leftTopPoint, D3DXVECTOR2(0,0) );
#else   X2OPTIMIZE_AVOID_LUA_RUNTIME_INTERPRETING
	pPointData->leftTopPoint		= lua_tinker::get<D3DXVECTOR2>( luaManager.GetLuaState(),  "LEFT_TOP" );
#endif  X2OPTIMIZE_AVOID_LUA_RUNTIME_INTERPRETING

	if ( pPointData->bUseTextureSize == false )
	{
		if ( bRect == true )
		{
#ifdef  X2OPTIMIZE_AVOID_LUA_RUNTIME_INTERPRETING
            LUA_GET_USER_DEFINED_TYPE_VALUE( luaManager, "RIGHT_BOTTOM", pPointData->rightBottomPoint, D3DXVECTOR2(0,0) );
#else   X2OPTIMIZE_AVOID_LUA_RUNTIME_INTERPRETING
			pPointData->rightBottomPoint	= lua_tinker::get<D3DXVECTOR2>( luaManager.GetLuaState(),  "RIGHT_BOTTOM" );
#endif  X2OPTIMIZE_AVOID_LUA_RUNTIME_INTERPRETING

			pPointData->rightTopPoint.x = pPointData->rightBottomPoint.x;
			pPointData->rightTopPoint.y = pPointData->leftTopPoint.y;

			pPointData->leftBottomPoint.x = pPointData->leftTopPoint.x;
			pPointData->leftBottomPoint.y = pPointData->rightBottomPoint.y;
		}
		else
		{
#ifdef  X2OPTIMIZE_AVOID_LUA_RUNTIME_INTERPRETING
            LUA_GET_USER_DEFINED_TYPE_VALUE( luaManager, "RIGHT_TOP", pPointData->rightTopPoint, D3DXVECTOR2(0,0) );
            LUA_GET_USER_DEFINED_TYPE_VALUE( luaManager, "LEFT_BOTTOM", pPointData->leftBottomPoint, D3DXVECTOR2(0,0) );
            LUA_GET_USER_DEFINED_TYPE_VALUE( luaManager, "RIGHT_BOTTOM", pPointData->rightBottomPoint, D3DXVECTOR2(0,0) );
#else   X2OPTIMIZE_AVOID_LUA_RUNTIME_INTERPRETING
			pPointData->rightTopPoint		= lua_tinker::get<D3DXVECTOR2>( luaManager.GetLuaState(),  "RIGHT_TOP" );
			pPointData->leftBottomPoint		= lua_tinker::get<D3DXVECTOR2>( luaManager.GetLuaState(),  "LEFT_BOTTOM" );
			pPointData->rightBottomPoint	= lua_tinker::get<D3DXVECTOR2>( luaManager.GetLuaState(),  "RIGHT_BOTTOM" );
#endif  X2OPTIMIZE_AVOID_LUA_RUNTIME_INTERPRETING
		}
	}
	
#ifdef  X2OPTIMIZE_AVOID_LUA_RUNTIME_INTERPRETING
    LUA_GET_USER_DEFINED_TYPE_VALUE( luaManager, "COLOR", pPointData->color, D3DXCOLOR(1,1,1,1) );
#else   X2OPTIMIZE_AVOID_LUA_RUNTIME_INTERPRETING
	pPointData->color				= lua_tinker::get<D3DXCOLOR>( luaManager.GetLuaState(),  "COLOR" );	
#endif  X2OPTIMIZE_AVOID_LUA_RUNTIME_INTERPRETING

	return pPointData;




	
}

CKTDGUIControl::UIStringData* CKTDGUIControl::SetStringData_()
{


	KLuaManager luaManager( g_pKTDXApp->GetLuaBinder()->GetLuaState() );
#ifndef X2OPTIMIZE_AVOID_LUA_RUNTIME_INTERPRETING
	TableBind( &luaManager, g_pKTDXApp->GetLuaBinder() );
#endif  X2OPTIMIZE_AVOID_LUA_RUNTIME_INTERPRETING

	//string msg;	
	wstring wMsg;
	int fontStyle;

    // 다이얼로그 툴용
	//LUA_GET_VALUE( luaManager, "MSG", wMsg, L"" ); 

	int iIndex;
	LUA_GET_VALUE( luaManager, "MSG", iIndex, STR_ID_EMPTY );
	if( STR_ID_EMPTY == iIndex )
	{
		LUA_GET_VALUE( luaManager, "STRING", wMsg, L"" );
	}
	else
	{
		wMsg = GET_STRING( iIndex );		
	}

	LUA_GET_VALUE( luaManager, "FONT_STYLE", fontStyle, CKTDGFontManager::FS_NONE );

	UIStringData* pStringData	= new UIStringData();	
	//LUA_GET_VALUE( luaManager, "IS_UK_FONT", pStringData->bUkFont, false );
	LUA_GET_VALUE( luaManager, "FONT_INDEX", pStringData->fontIndex, 0 );	
	LUA_GET_VALUE( luaManager, "SORT_FLAG", pStringData->sortFlag, DT_LEFT );
	pStringData->msg			= wMsg.c_str();
	pStringData->stringTableID	= iIndex;


	LUA_GET_VALUE( luaManager, "SPREAD", pStringData->bSpread, false );
	if( true == pStringData->bSpread )
	{
		LUA_GET_VALUE( luaManager, "SPREAD_TIME", pStringData->fSpreadTimeGap, 1.f );
	}
	LUA_GET_VALUE( luaManager, "USE_UK_FONT", pStringData->bUseUkFont, true );

	pStringData->fontStyle		= (CKTDGFontManager::FONT_STYLE)fontStyle;
#ifdef  X2OPTIMIZE_AVOID_LUA_RUNTIME_INTERPRETING
    LUA_GET_USER_DEFINED_TYPE_VALUE( luaManager, "POS", pStringData->pos, D3DXVECTOR2(0,0) );
    LUA_GET_USER_DEFINED_TYPE_VALUE( luaManager, "COLOR", pStringData->color, D3DXCOLOR(1,1,1,1) );
    LUA_GET_USER_DEFINED_TYPE_VALUE( luaManager, "OUTLINE_COLOR", pStringData->outlineColor, D3DXCOLOR(1,1,1,1) );
#else   X2OPTIMIZE_AVOID_LUA_RUNTIME_INTERPRETING
	pStringData->pos			= lua_tinker::get<D3DXVECTOR2>( luaManager.GetLuaState(),  "POS" );
	pStringData->color			= lua_tinker::get<D3DXCOLOR>( luaManager.GetLuaState(),  "COLOR" );
	pStringData->outlineColor	= lua_tinker::get<D3DXCOLOR>( luaManager.GetLuaState(),  "OUTLINE_COLOR" );
#endif  X2OPTIMIZE_AVOID_LUA_RUNTIME_INTERPRETING

	return pStringData;

}

CKTDGUIControl::TexChangeData* CKTDGUIControl::SetTexChangeData_()
{
	wstring FileName; 
	wstring key;
	CKTDGUIControl::TEX_AXIS rotateAxis = CKTDGUIControl::TA_NONE;
	D3DXVECTOR2 rotateAxisPos = D3DXVECTOR2(0,0);
	float fRotateDegree = 0;
	D3DXVECTOR2 pos = D3DXVECTOR2(0,0);
	CKTDGUIControl::TEX_AXIS scaleAxis = CKTDGUIControl::TA_NONE;
	D3DXVECTOR2 scaleAxisPos = D3DXVECTOR2(0,0);
	D3DXVECTOR2	scale = D3DXVECTOR2(1,1);
	float fScaleRotaionDegree = 0.0f;
	int sequenceID = 0;
	float fShowTime = 0.0f;
	bool bEndTexChange = false;
	int sequenceIDToGo = 0;





	KLuaManager luaManager( g_pKTDXApp->GetLuaBinder()->GetLuaState() );
#ifndef X2OPTIMIZE_AVOID_LUA_RUNTIME_INTERPRETING
	TableBind( &luaManager, g_pKTDXApp->GetLuaBinder() );
#endif  X2OPTIMIZE_AVOID_LUA_RUNTIME_INTERPRETING

	LUA_GET_VALUE( luaManager, "FileName", FileName, L"" );
	LUA_GET_VALUE( luaManager, "KeyName", key, L"" );

	LUA_GET_VALUE_ENUM( luaManager, "RotateAxis",	rotateAxis, CKTDGUIControl::TEX_AXIS, CKTDGUIControl::TA_NONE );
	LUA_GET_VALUE( luaManager, "RotateDegree",	fRotateDegree, 0.0f )

	LUA_GET_VALUE( luaManager, "SequenceID", sequenceID, 0 );
	LUA_GET_VALUE( luaManager, "fShowTime", fShowTime, 0.0f );

	LUA_GET_VALUE_ENUM( luaManager, "ScaleAxis",	scaleAxis, CKTDGUIControl::TEX_AXIS, CKTDGUIControl::TA_NONE );
	LUA_GET_VALUE( luaManager, "ScaleRotaionDegree", fScaleRotaionDegree, 0.0f );

	LUA_GET_VALUE( luaManager, "bEndTexChange", bEndTexChange, false );

	LUA_GET_VALUE( luaManager, "SequenceIDToGo", sequenceIDToGo, -1 );

#ifdef  X2OPTIMIZE_AVOID_LUA_RUNTIME_INTERPRETING
    LUA_GET_USER_DEFINED_TYPE_VALUE( luaManager, "POS", pos, D3DXVECTOR2(0,0) );
#else   X2OPTIMIZE_AVOID_LUA_RUNTIME_INTERPRETING
	pos = lua_tinker::get<D3DXVECTOR2>( luaManager.GetLuaState(),  "POS" );
#endif  X2OPTIMIZE_AVOID_LUA_RUNTIME_INTERPRETING


	if ( scaleAxis != CKTDGUIControl::TA_NONE )
    {
#ifdef  X2OPTIMIZE_AVOID_LUA_RUNTIME_INTERPRETING
        LUA_GET_USER_DEFINED_TYPE_VALUE( luaManager, "SCALE", scale, D3DXVECTOR2(1,1) );
#else   X2OPTIMIZE_AVOID_LUA_RUNTIME_INTERPRETING
		scale = lua_tinker::get<D3DXVECTOR2>( luaManager.GetLuaState(),  "SCALE" );
#endif  X2OPTIMIZE_AVOID_LUA_RUNTIME_INTERPRETING
    }

	if ( scaleAxis == CKTDGUIControl::TA_WORLD )
    {
#ifdef  X2OPTIMIZE_AVOID_LUA_RUNTIME_INTERPRETING
        LUA_GET_USER_DEFINED_TYPE_VALUE( luaManager, "SCALE_AXIS_POS", scaleAxisPos, D3DXVECTOR2(0,0) );
#else   X2OPTIMIZE_AVOID_LUA_RUNTIME_INTERPRETING
		scaleAxisPos = lua_tinker::get<D3DXVECTOR2>( luaManager.GetLuaState(),  "SCALE_AXIS_POS" );
#endif  X2OPTIMIZE_AVOID_LUA_RUNTIME_INTERPRETING
    }

	if ( rotateAxis == CKTDGUIControl::TA_WORLD )
    {
#ifdef  X2OPTIMIZE_AVOID_LUA_RUNTIME_INTERPRETING
        LUA_GET_USER_DEFINED_TYPE_VALUE( luaManager, "ROTATE_AXIS_POS", rotateAxisPos, D3DXVECTOR2(0,0) );
#else   X2OPTIMIZE_AVOID_LUA_RUNTIME_INTERPRETING
		rotateAxisPos = lua_tinker::get<D3DXVECTOR2>( luaManager.GetLuaState(),  "ROTATE_AXIS_POS" );
#endif  X2OPTIMIZE_AVOID_LUA_RUNTIME_INTERPRETING
    }
	



	CKTDGUIControl::TexChangeData* pTexChangeData = new CKTDGUIControl::TexChangeData();
	if ( FileName.empty() == false && key.empty() == false )
	{
		pTexChangeData->m_pTexData = SetTexureData_( FileName.c_str(), key.c_str() );
	}

	pTexChangeData->m_SequenceID = sequenceID;
	pTexChangeData->m_fShowTime = fShowTime;
	pTexChangeData->m_RotateAxis = rotateAxis;
	pTexChangeData->m_RotateAxisPos = rotateAxisPos;
	pTexChangeData->m_fRotateDegree = fRotateDegree;
	pTexChangeData->m_Pos = pos;
	pTexChangeData->m_ScaleAxis = scaleAxis;
	pTexChangeData->m_ScaleAxisPos = scaleAxisPos;
	pTexChangeData->m_Scale = scale;
	pTexChangeData->m_fScaleRotationDegree = fScaleRotaionDegree;
	pTexChangeData->m_bEndTexChange = bEndTexChange;
	pTexChangeData->m_SequenceIDToGo = sequenceIDToGo;

	return pTexChangeData;



}

void CKTDGUIControl::ShowGuideDesc()
{
#ifndef DLL_BUILD
	if ( m_GuideDesc.empty() == false && m_pDialog != NULL )
    {
		m_pDialog->ShowGuideDesc( this );
    }//if
#endif
}


void CKTDGUIControl::UpdateTexChangeData( CKTDGUIControl::TexChangeData* pTexChangeData )
{
	if ( pTexChangeData == NULL )
	{
		m_NowTexChangeData.m_SequenceID = 0;
		return;
	}

	if ( pTexChangeData->m_RotateAxis != CKTDGUIControl::TA_NONE )
		m_NowTexChangeData.m_RotateAxis = pTexChangeData->m_RotateAxis;

	if ( pTexChangeData->m_ScaleAxis != CKTDGUIControl::TA_NONE )
		m_NowTexChangeData.m_ScaleAxis = pTexChangeData->m_ScaleAxis;

	m_NowTexChangeData.m_fShowTime += m_fElapsedTime;
	float fRemainTime = pTexChangeData->m_fShowTime - m_NowTexChangeData.m_fShowTime;

	float remainDegree = pTexChangeData->m_fRotateDegree - m_NowTexChangeData.m_fRotateDegree;
	m_NowTexChangeData.m_fRotateDegree += (remainDegree / fRemainTime) * m_fElapsedTime;

	D3DXVECTOR2 remainPos = pTexChangeData->m_Pos - m_NowTexChangeData.m_Pos;
	m_NowTexChangeData.m_Pos += (remainPos / fRemainTime) * m_fElapsedTime;

	D3DXVECTOR2 remainScale = pTexChangeData->m_Scale - m_NowTexChangeData.m_Scale;
	m_NowTexChangeData.m_Scale += (remainScale / fRemainTime) * m_fElapsedTime;

	float remainScaleDegree = pTexChangeData->m_fScaleRotationDegree - m_NowTexChangeData.m_fScaleRotationDegree;
	m_NowTexChangeData.m_fScaleRotationDegree += (remainScaleDegree / fRemainTime) * m_fElapsedTime;



	if ( m_NowTexChangeData.m_fShowTime >= pTexChangeData->m_fShowTime )
	{
		m_NowTexChangeData.m_fShowTime = 0.0f;
		m_NowTexChangeData.m_fRotateDegree = pTexChangeData->m_fRotateDegree;
		m_NowTexChangeData.m_fScaleRotationDegree = pTexChangeData->m_fScaleRotationDegree;
		m_NowTexChangeData.m_Pos = pTexChangeData->m_Pos;
		m_NowTexChangeData.m_Scale = pTexChangeData->m_Scale;

		if ( pTexChangeData->m_bEndTexChange == true )
		{
			m_NowTexChangeData.m_bEndTexChange = true;
		}
		else
		{
			if ( m_NowTexChangeData.m_SequenceIDToGo != -1 )
				m_NowTexChangeData.m_SequenceID = m_NowTexChangeData.m_SequenceIDToGo;
			else
				m_NowTexChangeData.m_SequenceID++;
		}	
	}
}

void CKTDGUIControl::AddTexChangeData( map< int, CKTDGUIControl::TexChangeData* >& mapTexChangeData,
									  CKTDGUIControl::UIPointData* pUIPointData, CKTDGUIControl::TexChangeData* pTexChangeData )
{
	if ( pTexChangeData == NULL )
		return;

	map< int, CKTDGUIControl::TexChangeData* >::iterator mit;
	mit = mapTexChangeData.find( pTexChangeData->m_SequenceID );
	if ( mit == mapTexChangeData.end() )
	{
		mapTexChangeData.insert( std::make_pair( pTexChangeData->m_SequenceID, pTexChangeData ) );
	}
	else
	{
		if ( pUIPointData != NULL && mit->second != NULL &&
			pUIPointData->pUITextureData == mit->second->m_pTexData )
			pUIPointData->pUITextureData = NULL;

		SAFE_DELETE( mit->second );

		mit->second = pTexChangeData;
	}

	if ( pTexChangeData->m_SequenceID == 0 )
	{
		if ( pUIPointData != NULL )
		{
			if ( pTexChangeData != NULL && pTexChangeData->m_pTexData != NULL )
			{
				pUIPointData->pUITextureData = pTexChangeData->m_pTexData;
				pUIPointData->SetAutoPointByTextureSize();
			}
		}
	}
}

bool CKTDGUIControl::GetIsMoving()
{
	return ( m_fAnimateTime != 0.0f || m_fChangeTime != 0.0f || m_queMoveItems.empty() == false );
}

D3DXVECTOR3 CKTDGUIControl::GetDummyPos( int index )
{
	if ( index < 0 || index >= (int)m_DummyPosList.size() )
		return D3DXVECTOR3( 0, 0, 0 );

	return m_DummyPosList[index];
}

int CKTDGUIControl::GetDummyInt( int index )
{
	if ( index < 0 || index >= (int)m_DummyIntList.size() )
		return -1;

	return m_DummyIntList[index];
}

// #ifdef SERV_DUNGEON_FORCED_EXIT_SYSTEM
bool CKTDGUIControl::SetDummyInt( int index, int dummy )
{
	if ( index < 0 || index >= static_cast<int>(m_DummyIntList.size()) )
		return false;

	m_DummyIntList[index] = dummy;
	return true;
}
// #endif //SERV_DUNGEON_FORCED_EXIT_SYSTEM
bool CKTDGUIControl::GetIsGuideDescAbsolutePos() 
{
	if ( m_vGuideDescAbsolutePos != D3DXVECTOR2(0,0) )
	{
		return true;
	}
	else
		return false;
}

std::wstring CKTDGUIControl::GetDummyString( int index )
{
	if ( index < 0 || index >= (int)m_DummyStringList.size() )
		return L"Unknown";

	return m_DummyStringList[index];
}

void CKTDGUIControl::AddDummyString_LUA( const char* pDummyString ) 
{
	wstring dummyString; 
	ConvertUtf8ToWCHAR( dummyString, pDummyString); 
	m_DummyStringList.push_back( dummyString ); 
}

CKTDGUIControl::UITextureData::~UITextureData()
{
	SAFE_CLOSE( pTexture );
}

//////////////////////////////////////////////////////////////////////////
// CKTDGUIControl::CPictureData

CKTDGUIControl::CPictureData::CPictureData()
{
	pTexture				= NULL; 
	pPoint					= NULL; 
	bShow					= true;

	bMaximizePos			= false;
	bMaximizeSize			= false;

	pos						= D3DXVECTOR2(0, 0);
	size					= D3DXVECTOR2(0, 0);
	originalPos				= D3DXVECTOR2( 0, 0 );
	originalSize			= D3DXVECTOR2(0, 0);

	m_bTwinkle				= false;
	m_fOriginalAlpha		= 0.f;
	m_fRemainTwinkleTime	= 0.f;
	m_bLoop					= false;

#ifdef NEW_SKILL_TREE
	m_bFlicker				= false;
	m_fFlickerTime			= 0.f;
	m_fFlickerMaxAlpha		= 0.f;
	m_fFlickerMinAlpha		= 0.f;
	m_fRemainFlickerTime	= 0.f;
	m_bFlickerGainAlpha		= false;
	m_bFlickerLoop			= false;
#endif

	m_EdgeColor				= D3DXCOLOR( 1, 1, 1, 1 );
	m_EdgeWidth				= 0;
	m_bDrawEdgeOut			= true; //바깥쪽에 그릴것인가 안쪽에 그릴것인가?

    m_nIndex                = 0;

#ifdef BUFF_ICON_UI
	m_bFadeIn				= false;
	m_bFadeOut				= false;
	m_fFadeTime				= 0.f;
	m_fRemainFadeTime		= 0.f;
#endif //BUFF_ICON_UI

	m_fTwinkleTime			= 0.f;
	m_EdgeEnlarge			= 0;
}

CKTDGUIControl::CPictureData::~CPictureData()
{
	SAFE_DELETE( pTexture );
	SAFE_DELETE( pPoint );
}

void CKTDGUIControl::CPictureData::OnFrameMove( double fTime, float fElapsedTime )
{
	KTDXPROFILE();


	KTDXPROFILE_BEGIN( "twinkle" );
	if( true == m_bTwinkle )
	{
		if( m_fRemainTwinkleTime > 0.f )
		{
			m_fRemainTwinkleTime -= fElapsedTime;
			if( NULL != pPoint )
			{
				const float MAGIC_ON_TIME = 0.5f;
				const float MAGIC_OFF_TIME = 0.2f;
				const float MAGIC_TOTAL_TIME = MAGIC_ON_TIME + MAGIC_OFF_TIME;


				int iTemp = (int)( m_fRemainTwinkleTime/MAGIC_TOTAL_TIME );
				float fTemp = m_fRemainTwinkleTime - (float)iTemp;
				if( fTemp < MAGIC_OFF_TIME )
				{
					pPoint->color.a = 0.f;
				}
				else
				{
					pPoint->color.a = m_fOriginalAlpha;
				}
			}
		}
		else
		{
			if ( m_bLoop == true )
			{
				m_fRemainTwinkleTime = m_fTwinkleTime;
			}
			else
			{
				m_bTwinkle = false;
			}
			
			if( NULL != pPoint )
			{
				pPoint->color.a = m_fOriginalAlpha;
			}
		}
	}
	KTDXPROFILE_END(); // "twinkle"



	KTDXPROFILE_BEGIN( "flicker" );
	if( true == m_bFlicker )
	{
		if( m_bFlickerGainAlpha == true )
		{	
			m_fRemainFlickerTime += fElapsedTime;
			if(m_fRemainFlickerTime > m_fFlickerTime)
				m_bFlickerGainAlpha = false;
		}
		else
		{
			m_fRemainFlickerTime -= fElapsedTime;
			if(m_fRemainFlickerTime < 0)
			{
				if(m_bFlickerLoop == true)
				{
					m_bFlickerGainAlpha = true;
				}
				else
				{
					m_bFlicker = false;
				}
			}				
		}
		if(NULL != pPoint )
		{
			pPoint->color.a = (m_fFlickerMaxAlpha - m_fFlickerMinAlpha) * m_fRemainFlickerTime/m_fFlickerTime + m_fFlickerMinAlpha;
		}
	}
	KTDXPROFILE_END(); // "flicker"



	// vibrate
	KTDXPROFILE_BEGIN( "vibrate" );
	if( m_VibrateData.m_fElapsedTime < m_VibrateData.m_fMaxTime )
	{
		m_VibrateData.m_fElapsedTime += fElapsedTime;

		if( true == m_VibrateData.m_bGoingAway )
		{
			pos += m_VibrateData.m_vDirSpeed * fElapsedTime;
			SetPos( pos );

			D3DXVECTOR2 vDistance = pos - m_VibrateData.m_vOriginalPos;
			float fDistance = D3DXVec2Length( &vDistance );
			if( fDistance > m_VibrateData.m_fDistance )
			{
				m_VibrateData.m_bGoingAway = false;
			}
		}
		else
		{
			pos -= m_VibrateData.m_vDirSpeed * fElapsedTime;
			SetPos( pos );

			D3DXVECTOR2 vDistance = pos - m_VibrateData.m_vOriginalPos;
			if( D3DXVec2Dot( &vDistance, &m_VibrateData.m_vDirSpeed ) <= 0.f )
			{
				m_VibrateData.m_bGoingAway = true;
			}
		}
	}
	else 
	{
		if( m_VibrateData.m_fMaxTime != 0.f )
		{
			m_VibrateData.m_fMaxTime = 0.f;
			pos = m_VibrateData.m_vOriginalPos;
		}
	}
	KTDXPROFILE_END(); // "vibrate"

#ifdef BUFF_ICON_UI
	KTDXPROFILE_BEGIN( "FadeInOut" );
	if( true == m_bFadeIn )
	{
		if( m_fFadeTime >= m_fRemainFadeTime )
		{
			m_fRemainFadeTime += fElapsedTime;
			if(NULL != pPoint )
				pPoint->color.a = ( m_fRemainFadeTime / m_fFadeTime );
		}
		else
		{
			if ( NULL != pPoint )
				pPoint->color.a = 1.f;
			m_bFadeIn = false;
		}
	}

	if( true == m_bFadeOut )
	{
		if( m_fFadeTime >= m_fRemainFadeTime )
		{
			m_fRemainFadeTime += fElapsedTime;
			if(NULL != pPoint )
				pPoint->color.a = 1.f - ( m_fRemainFadeTime / m_fFadeTime );
		}
		else
		{
			if ( NULL != pPoint )
				pPoint->color.a = 0.f;
			m_bFadeOut = false;
		}
	}
	KTDXPROFILE_END(); // "FadeInOut"

#endif //BUFF_ICON_UI
}

void CKTDGUIControl::CPictureData::SetTex_LUA2( const char* pFileName )
{
	KTDXPROFILE();

	wstring wstrFileName;
	ConvertUtf8ToWCHAR( wstrFileName, pFileName );
	SetTex( wstrFileName.c_str() );
}

void CKTDGUIControl::CPictureData::SetTex_LUA( const char* pFileName, const char* key )
{
	KTDXPROFILE();

	SAFE_DELETE( pTexture );
	wstring wstrFileName;
	ConvertUtf8ToWCHAR( wstrFileName, pFileName );

	wstring wstrKeyName;
	ConvertUtf8ToWCHAR( wstrKeyName, key );

	SetTex( wstrFileName.c_str(), wstrKeyName.c_str() );
/*
	pTexture = CKTDGUIControl::SetTexureData_( wstrFileName.c_str(), wstrKeyName.c_str() );
	if( pPoint != NULL )
		pPoint->pUITextureData = pTexture;
*/
}

bool CKTDGUIControl::CPictureData::SetTex( const WCHAR* pFileName, const WCHAR* key )
{
	KTDXPROFILE();

	if( NULL == pFileName )
		return false;

//#ifdef EFFICIENT_SET_TEX_FOR_UI
//	if( NULL != pTexture && NULL != key )
//	{
//		if( 0 == pTexture->texName.compare( pFileName) &&
//			0 == pTexture->keyName.compare( key ) )
//		{
//			return true;
//		}
//	}
//#endif EFFICIENT_SET_TEX_FOR_UI

	CKTDGUIControl::UITextureData* pNewTextureData = CKTDGUIControl::SetTexureData_( pFileName, key );
	if( pPoint != NULL )
	{
		pPoint->pUITextureData = pNewTextureData;
	}

	SAFE_DELETE( pTexture );
	pTexture = pNewTextureData;

	if ( pTexture == NULL )
		return false;

	return true;
}

void CKTDGUIControl::CPictureData::SetSizeAsTextureSize()
{
	if( NULL != pPoint )
	{
		pPoint->SetAutoPointByTextureSize();

		size.x = pPoint->rightTopPoint.x - pPoint->leftTopPoint.x;
		size.y = pPoint->rightBottomPoint.y - pPoint->leftTopPoint.y;
	}
}

bool CKTDGUIControl::CPictureData::SetTex( const WCHAR* pFileName )
{
	KTDXPROFILE();


	if( NULL == pFileName )
		return false;

//#ifdef EFFICIENT_SET_TEX_FOR_UI
//	if( NULL != pTexture  )
//	{
//		if( 0 == pTexture->texName.compare( pFileName) )
//		{
//			return true;
//		}
//	}
//#endif EFFICIENT_SET_TEX_FOR_UI


	//wstring wstrFileName = pFileName;

	UITextureData* pTexData = new UITextureData();
	pTexData->texName	= pFileName;
	pTexData->pTexture	= g_pKTDXApp->GetDeviceManager()->OpenTexture( pTexData->texName );
	if( pTexData->pTexture == NULL )
	{
		SAFE_DELETE( pTexData );
		return false;
	}

	float fWidth	= (float)pTexData->pTexture->GetWidth();
	float fHeight	= (float)pTexData->pTexture->GetHeight();

	pTexData->uvOrgTexture[CKTDGUIControl::VP_LEFT_TOP]			= D3DXVECTOR2(0,0);
	pTexData->uvOrgTexture[CKTDGUIControl::VP_RIGHT_TOP]		= D3DXVECTOR2(fWidth,0);
	pTexData->uvOrgTexture[CKTDGUIControl::VP_LEFT_BOTTOM]		= D3DXVECTOR2(0,fHeight);
	pTexData->uvOrgTexture[CKTDGUIControl::VP_RIGHT_BOTTOM]		= D3DXVECTOR2(fWidth, fHeight);

	pTexData->texSize = D3DXVECTOR2( fWidth, fHeight );

	//pTexture = CKTDGUIControl::SetTexureData_( wstrFileName.c_str(), wstrKeyName.c_str() );
	pTexData->SetTextureUV();


	SAFE_DELETE( pTexture );
	pTexture = pTexData;
	if( pPoint != NULL )
		pPoint->pUITextureData = pTexture;

	return true;
}

void CKTDGUIControl::CPictureData::SetTex( CKTDGUIControl::UITextureData* pNewTexture )
{
	KTDXPROFILE();

	SAFE_DELETE( pTexture );
	pTexture = pNewTexture;
	if ( pPoint != NULL )
		pPoint->pUITextureData = pTexture;
}

void CKTDGUIControl::CPictureData::SetPoint_LUA()
{
	KTDXPROFILE();

	SAFE_DELETE( pPoint );
	pPoint = CKTDGUIControl::SetPointData_();
	pPoint->pUITextureData = pTexture;
	pPoint->SetAutoPointByTextureSize();
	pos.x = pPoint->leftTopPoint.x;
	pos.y = pPoint->leftTopPoint.y;
	size.x = pPoint->rightTopPoint.x - pPoint->leftTopPoint.x;
	size.y = pPoint->rightBottomPoint.y - pPoint->leftTopPoint.y;
	originalPos = pos;
	originalSize = size;

#ifdef DLL_BUILD
	if( pPoint->fRotDegree != 0 )
		pPoint->Rotate( pPoint->fRotDegree );
#endif
}

void CKTDGUIControl::CPictureData::SetPoint( CKTDGUIControl::UIPointData* pNewPoint )
{
	KTDXPROFILE();

	SAFE_DELETE( pPoint );
	pPoint = pNewPoint;
	pPoint->pUITextureData = pTexture;
	pPoint->SetAutoPointByTextureSize();
	pos.x = pPoint->leftTopPoint.x;
	pos.y = pPoint->leftTopPoint.y;
	size.x = pNewPoint->rightTopPoint.x - pPoint->leftTopPoint.x;
	size.y = pPoint->rightBottomPoint.y - pPoint->leftTopPoint.y;
	originalPos = pos;
	originalSize = size;

#ifdef DLL_BUILD
	if( pPoint->fRotDegree != 0 )
		pPoint->Rotate( pPoint->fRotDegree );
#endif
}

void CKTDGUIControl::CPictureData::SetPoint()
{
	KTDXPROFILE();

#ifndef DLL_BUILD
	if ( pPoint == NULL && pTexture != NULL )
	{
		pPoint = new CKTDGUIControl::UIPointData();
		pPoint->pUITextureData = pTexture;
		pPoint->SetAutoPointByTextureSize();
		pos.x = pPoint->leftTopPoint.x;
		pos.y = pPoint->leftTopPoint.y;
		originalPos = pos;
		size = pTexture->texSize;
		originalSize = size;
	}
#else
	SetPoint( new CKTDGUIControl::UIPointData() );
#endif

}

void CKTDGUIControl::CPictureData::SetSize( D3DXVECTOR2 _size )
{
	KTDXPROFILE();

	size = _size;

	if ( pPoint == NULL && pTexture != NULL )
	{
		pPoint = new CKTDGUIControl::UIPointData();
		pPoint->pUITextureData = pTexture;
		pPoint->SetAutoPointByTextureSize();
		pos.x = pPoint->leftTopPoint.x;
		pos.y = pPoint->leftTopPoint.y;
		originalPos = pos;
		originalSize = size;
	}

	if( pPoint != NULL )
	{
		pPoint->rightTopPoint.x = pPoint->leftTopPoint.x + size.x;
		pPoint->rightTopPoint.y = pPoint->leftTopPoint.y;

		pPoint->leftBottomPoint.x = pPoint->leftTopPoint.x;
		pPoint->leftBottomPoint.y = pPoint->leftTopPoint.y + size.y;

		pPoint->rightBottomPoint.x = pPoint->leftTopPoint.x + size.x;
		pPoint->rightBottomPoint.y = pPoint->leftTopPoint.y + size.y;
	}
	else
	{
		ErrorLog( KEM_ERROR345 );
	}
}

void CKTDGUIControl::CPictureData::SetSizeX( float x )
{
	KTDXPROFILE();

	size.x = x;

	if( pPoint != NULL )
	{
		pPoint->rightTopPoint.x = pPoint->leftTopPoint.x + size.x;
		pPoint->rightTopPoint.y = pPoint->leftTopPoint.y;

		pPoint->leftBottomPoint.x = pPoint->leftTopPoint.x;
		pPoint->leftBottomPoint.y = pPoint->leftTopPoint.y + size.y;

		pPoint->rightBottomPoint.x = pPoint->leftTopPoint.x + size.x;
		pPoint->rightBottomPoint.y = pPoint->leftTopPoint.y + size.y;
	}
	else
	{
		ErrorLog( KEM_ERROR346 );
	}
}

void CKTDGUIControl::CPictureData::SetSizeY( float y )
{
	KTDXPROFILE();

	size.y = y;

	if( pPoint != NULL )
	{
		pPoint->rightTopPoint.x = pPoint->leftTopPoint.x + size.x;
		pPoint->rightTopPoint.y = pPoint->leftTopPoint.y;

		pPoint->leftBottomPoint.x = pPoint->leftTopPoint.x;
		pPoint->leftBottomPoint.y = pPoint->leftTopPoint.y + size.y;

		pPoint->rightBottomPoint.x = pPoint->leftTopPoint.x + size.x;
		pPoint->rightBottomPoint.y = pPoint->leftTopPoint.y + size.y;
	}
}

void CKTDGUIControl::CPictureData::SetSizeY( float y, bool isInverse  )
{
	KTDXPROFILE();

	size.y = y;

	if( pPoint != NULL )
	{
		if ( true == isInverse )
		{
			pPoint->rightBottomPoint.x = pPoint->leftBottomPoint.x + size.x;
			pPoint->rightBottomPoint.y = pPoint->leftBottomPoint.y;

			pPoint->leftTopPoint.x = pPoint->leftBottomPoint.x;
			pPoint->leftTopPoint.y = pPoint->leftBottomPoint.y - size.y;

			pPoint->rightTopPoint.x = pPoint->leftBottomPoint.x + size.x;
			pPoint->rightTopPoint.y = pPoint->leftBottomPoint.y - size.y;
		}
		else
		{
			pPoint->rightTopPoint.x = pPoint->leftTopPoint.x + size.x;
			pPoint->rightTopPoint.y = pPoint->leftTopPoint.y;

			pPoint->leftBottomPoint.x = pPoint->leftTopPoint.x;
			pPoint->leftBottomPoint.y = pPoint->leftTopPoint.y + size.y;

			pPoint->rightBottomPoint.x = pPoint->leftBottomPoint.x;
			pPoint->rightBottomPoint.y = pPoint->leftTopPoint.y + size.y;
		}
	}
	else
	{
		ErrorLog( KEM_ERROR346 );
	}
}

void CKTDGUIControl::CPictureData::SetPos( D3DXVECTOR2 _pos )
{
	KTDXPROFILE();

	pos = _pos;

	if( pPoint != NULL )
	{
		pPoint->leftTopPoint.x = pos.x;
		pPoint->leftTopPoint.y = pos.y;

		pPoint->rightTopPoint.x = pos.x + size.x;
		pPoint->rightTopPoint.y = pos.y;

		pPoint->leftBottomPoint.x = pos.x;
		pPoint->leftBottomPoint.y = pos.y + size.y;

		pPoint->rightBottomPoint.x = pos.x + size.x;
		pPoint->rightBottomPoint.y = pos.y + size.y;
	}
	else
	{
		ErrorLog( KEM_ERROR347 );
	}
}

void CKTDGUIControl::CPictureData::SetMaximizeType( bool bPos, bool bSize )
{
	KTDXPROFILE();

	bMaximizePos = bPos;
	bMaximizeSize = bSize;
}

#ifdef BUFF_ICON_UI
void CKTDGUIControl::CPictureData::SetFadeIn( float fTime )
{
	if( NULL == pPoint )
		return;

	if ( IsSamef( fTime ) )
		return;

	m_bFadeIn = true;
	m_bFadeOut = false;
	m_fFadeTime = fTime;
	m_fRemainFadeTime = 0.f;
	pPoint->color.a = 0.f;
}
void CKTDGUIControl::CPictureData::SetFadeOut( float fTime )
{
	if( NULL == pPoint )
		return;

	if( IsSamef( fTime ) )
		return;

	m_bFadeIn = false;
	m_bFadeOut = true;
	m_fFadeTime = fTime;
	m_fRemainFadeTime = 0.f;
	pPoint->color.a = 1.f;
}
#endif //BUFF_ICON_UI

void CKTDGUIControl::SetGuideDesc_LUA( int iStringIndex )
{
    m_GuideDesc = GET_STRING( iStringIndex );
}

void CKTDGUIControl::SetGuideDescByString_LUA( const char* pGuideDesc )
{
    ConvertUtf8ToWCHAR( m_GuideDesc, pGuideDesc );
}
//////////////////////////////////////////////////////////////////////////
#if defined(REFORM_ENTRY_POINT) && defined( MOVIE_TEST_BASE ) || defined( MOVIE_TEST ) 
// #ifdef MOVIE_TEST	 	// 13-11-11, 진입 구조 개편, kimjh, MOVIE_TEST 중 사용에 필요한 Define 을 MOVIE_TEST_BASE 로 변경


CKTDGUIControl::CMovieData::CMovieData()
	: m_pBvt( NULL ), /*m_pInfo( NULL ),*/ /*m_pPointData( NULL ),*/
	m_pRenderTargetTexture( NULL ),
	/*m_pReader( NULL ),*/
	m_wstrMovieFileName( L"" ),
	m_vSize(0.0f, 0.0f), m_vOriginPos(0.0f, 0.0f), m_vPos(0.0f, 0.0f), m_bShow( false )
#ifdef PLAY_PROMOTION_MOVIE //JHKang
	, m_bLoop( false )
#endif //PLAY_PROMOTION_MOVIE
{
// 	if ( !m_Bvl.IsCreated() )
// 		m_Bvl.Create( BANDIVIDEO_RELEASE_DLL_FILE_NAME, NULL, NULL );
}

CKTDGUIControl::CMovieData::~CMovieData()
{
	SAFE_CLOSE( m_pRenderTargetTexture );

	if ( m_Bvl.IsCreated() )
		m_Bvl.Destroy();

	if ( m_pBvt )
		m_pBvt->Close();

	//SAFE_DELETE( m_pReader );
	SAFE_DELETE( m_pBvt );
}

void CKTDGUIControl::CMovieData::SetMovieFileName_LUA( const char* strMoiveFileName_ )
{
	wstring wstrFileName;
	ConvertUtf8ToWCHAR( wstrFileName, strMoiveFileName_ );
	SetMovieFileName( wstrFileName.c_str() );	
	
}


void CKTDGUIControl::CMovieData::SetMovieFileName( const WCHAR* wstrMovieFileName_ )
{
	if ( !m_Bvl.IsCreated() )
	{
// 		if ( NULL == m_pReader )
// 			m_pReader = new CBandiVideoFileReader();
		m_Bvl.Create( BANDIVIDEO_RELEASE_DLL_FILE_NAME, NULL, NULL );
	}
#ifdef VERIFY_BANDI_VIDEO_LIBRARY
	m_Bvl.Verify("KOG_ELSWORD_20131203", "1b9b533c");
#endif // VERIFY_BANDI_VIDEO_LIBRARY

	WCHAR buff[MAX_PATH] = {0, };
	GetCurrentDirectory( MAX_PATH, buff );	
	m_wstrMovieFileName = buff;
#if defined(REFORM_ENTRY_POINT) && defined( MOVIE_TEST_BASE ) 
	m_wstrMovieFileName += L"\\movie\\";
#else  // defined(REFORM_ENTRY_POINT) && defined( MOVIE_TEST_BASE ) 
	m_wstrMovieFileName += L"\\music\\";
#endif // defined(REFORM_ENTRY_POINT) && defined( MOVIE_TEST_BASE ) 
	m_wstrMovieFileName += wstrMovieFileName_;

// 	KGCMassFileManager::CMassFile::MASSFILE_MEMBERFILEINFO_POINTER Info
// 		= g_pKTDXApp->GetDeviceManager()->GetMassFileManager()->LoadDataFile( wstrMovieFileName_ );

	m_Bvl.Close();
	if(SUCCEEDED(m_Bvl.Open( m_wstrMovieFileName.c_str(), FALSE)))
	//if ( SUCCEEDED( m_pReader->Read( reinterpret_cast<BYTE*>( Info->pRealData ), Info->size ) ) )
	{
#ifndef X2OPTIMIZE_BANDI_DYNAMICVB
		BV_DEVICE_DX9 bvd = { DXUTGetD3DObject(), g_pKTDXApp->GetDevice(), DXUTGetHWND() };
#endif  X2OPTIMIZE_BANDI_DYNAMICVB

		m_Bvl.GetVideoInfo(m_Info);

		if ( NULL == m_pBvt )
        {
#ifdef  X2OPTIMIZE_BANDI_DYNAMICVB
            m_pBvt = new CBandiVideoTexture_DX9();
#else   X2OPTIMIZE_BANDI_DYNAMICVB
			m_pBvt = new CBandiVideoTexture_DX9(&bvd);
#endif  X2OPTIMIZE_BANDI_DYNAMICVB
        }
		else
			m_pBvt->Close();

		m_pBvt->Open(m_Info.width, m_Info.height);
		m_Bvl.Play();
	}
}

void CKTDGUIControl::CMovieData::SetRect_LUA( const float fX_, const float fY_, const float fWidth_, const float fHeight_ )
{
	m_vOriginPos.x	= fX_;
	m_vOriginPos.y	= fY_;
	m_vSize.x	= fWidth_;
	m_vSize.y	= fHeight_;

	m_pRenderTargetTexture
		= g_pKTDXApp->GetDeviceManager()->OpenRenderTargetTexture( L"MoveData", 
		static_cast<int>( fWidth_ ), static_cast<int>( fHeight_ ), D3DFMT_A8R8G8B8 );
}

// void CKTDGUIControl::CMovieData::SetMoivePoint_LUA()
// {
// 	SAFE_DELETE( m_pPointData );
// 	m_pPointData = SetPointData_();
// 	m_pPointData->SetAutoPointByTextureSize();
// 
// 	m_pRenderTargetTexture
// 		= g_pKTDXApp->GetDeviceManager()->OpenRenderTargetTexture( L"MoveData", m_pPointData->leftBottomPoint, iRenderTargetHeight, D3DFMT_A8R8G8B8 }
// }


void CKTDGUIControl::CMovieData::OnFrameMove()
{
	BVL_STATUS status;
	m_Bvl.GetStatus(status);

#ifdef PLAY_PROMOTION_MOVIE //JHKang
	if ( status == BVL_STATUS_PLAYEND )
	{
		m_Bvl.Seek(0, BVL_SEEK_TIME);
		
		if ( m_bLoop )
			m_Bvl.Play();
		else
			m_Bvl.Stop();
	}
#else //PLAY_PROMOTION_MOVIE
	if ( status == BVL_STATUS_PLAYEND )
	{
		m_Bvl.Seek(0, BVL_SEEK_TIME);
		m_Bvl.Play();
	}
#endif //PLAY_PROMOTION_MOVIE

	if(m_Bvl.IsNextFrame())
	{
		if ( NULL != m_pBvt )
		{
			INT pitch;
			BYTE* buf = m_pBvt->Lock(pitch);
			if(buf)
			{
				// Get frame
				BVL_FRAME frame;
				frame.frame_buf = buf;
				frame.frame_buf_size = m_Info.height*pitch;
				frame.pitch = pitch;
				frame.width = m_Info.width;
				frame.height = m_Info.height;
				frame.pixel_format = m_pBvt->GetFormat();

				m_Bvl.GetFrame(frame, TRUE);

				m_pBvt->Unlock();
			}
		}
	}
}

void CKTDGUIControl::CMovieData::OnFrameRender()
{
	m_pRenderTargetTexture->BeginRender( true );
	m_pRenderTargetTexture->Clear( D3DCOLOR_RGBA( 0, 0, 0, 0 ) );
	
	// Draw frame
	if ( NULL != m_pBvt )
	{

#ifdef REFORM_ENTRY_POINT		// 13-11-11, kimjh 진입 구조 개편
		// 해상도 변경 시 ( 기준 1024 by 768 ) 
		// 원본이 되는 사이즈는 변경되지 않아야 하기 때문에
		// 해당 부분 수정
		m_pBvt->Draw(0, 0, static_cast<int>(m_vSize.x), static_cast<int>( m_vSize.y ) );
#else	// REFORM_ENTRY_POINT	// 13-11-11, kimjh 진입 구조 개편
		m_pBvt->Draw(0, 0, 
			static_cast<DWORD>( m_vSize.x  * g_pKTDXApp->GetResolutionScaleX() ), 
			static_cast<DWORD>( m_vSize.y * g_pKTDXApp->GetResolutionScaleY() ) );
#endif	// REFORM_ENTRY_POINT	// 13-11-11, kimjh 진입 구조 개편
	}

	m_pRenderTargetTexture->EndRender();    

#ifdef REFORM_ENTRY_POINT	 	// 13-11-11, 진입 구조 개편, kimjh
	// 해당도 변경 시 ( 기준 1024 by 768 ) 
	// 시작점은 현재 스크린의 실 좌표에 영향을 받고 크기는 기준 값에 영향을 받기 때문에
	// 해당 부분 수정
	m_pRenderTargetTexture->Draw( static_cast<int> (m_vPos.x / g_pKTDXApp->GetResolutionScaleX() ),	
		static_cast<int>(m_vPos.y / g_pKTDXApp->GetResolutionScaleY() ), 
		static_cast<DWORD>( m_vSize.x ), 
		static_cast<DWORD>( m_vSize.y ) );
#else  // REFORM_ENTRY_POINT	// 13-11-11, 진입 구조 개편, kimjh
	m_pRenderTargetTexture->Draw( static_cast<int>(m_vPos.x),	static_cast<int>(m_vPos.y), 
		static_cast<DWORD>( m_vSize.x  * g_pKTDXApp->GetResolutionScaleX() ), 
		static_cast<DWORD>( m_vSize.y * g_pKTDXApp->GetResolutionScaleY() ) );
#endif // REFORM_ENTRY_POINT	// 13-11-11, 진입 구조 개편, kimjh
}


#endif // defined(REFORM_ENTRY_POINT) && defined( MOVIE_TEST_BASE ) || defined( MOVIE_TEST ) 
// #endif //  MOVIE_TEST	// 13-11-11, 진입 구조 개편, kimjh, MOVIE_TEST 중 사용에 필요한 Define 을 MOVIE_TEST_BASE 로 변경

