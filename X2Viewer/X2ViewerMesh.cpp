/**	@file	: X2ViewerMesh.cpp
	@desc	: 메쉬 처리
	@author	: Anomymouse

	@date	JHKang on 2013-09-13,
		- 필드 이후 빌드 성공
	@date	JHKang on 2013-09-17,
		- 파일 정리 및 블록화
*/

#include "dxstdafx.h"
#include ".\x2viewermesh.h"

#pragma region CX2ViewerMesh
CX2ViewerMesh::CX2ViewerMesh(void) :
m_pXMesh( NULL )
{
	CX2ViewerObject::SetObjectStyle( CX2ViewerObject::OS_MESH );
	m_pMatrix	= new CKTDGMatrix( g_pKTDXApp->GetDevice() );
	m_pRenderer	= new CKTDGXRenderer( g_pKTDXApp->GetDevice() );

	//Light 위치표기 메쉬
	m_pXMeshLight		= g_pKTDXApp->GetDeviceManager()->OpenXMesh( L"Sun.X" );
	m_pMatrixLight		= new CKTDGMatrix( g_pKTDXApp->GetDevice() );
	m_pMatrixLight->Scale( 0.5f, 0.5f, 0.5f );

	TEX_STAGE_DATA texStageData;
	m_vecTexStageData.push_back( texStageData );
	m_vecTexStageData.push_back( texStageData );
	m_vecTexStageData.push_back( texStageData );

	Reset();
}
#pragma endregion 생성자

#pragma region ~CX2ViewerMesh
CX2ViewerMesh::~CX2ViewerMesh(void)
{
	SAFE_CLOSE( m_pXMeshLight );
	SAFE_DELETE( m_pMatrixLight );

	SAFE_DELETE( m_pMatrix );
	SAFE_DELETE( m_pRenderer );
}
#pragma endregion 소멸자

#pragma region OnFrameMove
HRESULT CX2ViewerMesh::OnFrameMove( double fTime, float fElapsedTime )
{
	if( m_pXMesh == NULL )
		return S_OK;

	m_pMatrix->Scale( m_vScale );

	//CKTDGXRenderer::RenderParam* pRenderParam = m_pRenderer->GetRenderParam();
	//*pRenderParam = m_RenderParam;        
	//pRenderParam->worldMatrix = m_pMatrix->GetMatrix();

	//Impact Anim setting
	if( m_ImpactData.fAnimTime != 0.0f )
	{
		if( m_ImpactData.fAnimTime <= m_ImpactNowAnimTime )
		{
			m_ImpactNowAnimTime = 0.0f;
			m_RenderParam.fLightFlowImpact = m_ImpactData.fMin;
		}
		else
		{
			m_ImpactNowAnimTime += fElapsedTime;
		}
		m_RenderParam.fLightFlowImpact = m_ImpactData.fMin + ((m_ImpactData.fMax - m_ImpactData.fMin)*(m_ImpactNowAnimTime/m_ImpactData.fAnimTime));
	}
	else
	{
		m_ImpactNowAnimTime = 0.0f;
	}

	//Texture Stage 0~2 Setting
	for( int i = 0; i < (int)m_vecTexStageData.size(); ++i )
	{
		if( m_vecTexStageData[i].fAnimTime != 0.0f )
		{
			if( m_vecTexStageData[i].fAnimTime <= m_vecTexStageData[i].fNowAnimTime )
			{
				m_vecTexStageData[i].fNowAnimTime = 0.0f;
				switch( i )
				{
				case 0: m_RenderParam.texOffsetStage0 = m_vecTexStageData[i].vMin; break;
				case 1: m_RenderParam.texOffsetStage1 = m_vecTexStageData[i].vMin; break;
				case 2: m_RenderParam.texOffsetStage2 = m_vecTexStageData[i].vMin; break;
				}
			}
			else
			{
				m_vecTexStageData[i].fNowAnimTime += fElapsedTime;
			}
			switch( i )
			{
			case 0:
				{
					m_RenderParam.texOffsetStage0.x = m_vecTexStageData[i].vMin.x + ((m_vecTexStageData[i].vMax.x - m_vecTexStageData[i].vMin.x)*(m_vecTexStageData[i].fNowAnimTime/m_vecTexStageData[i].fAnimTime));
					m_RenderParam.texOffsetStage0.y = m_vecTexStageData[i].vMin.y + ((m_vecTexStageData[i].vMax.y - m_vecTexStageData[i].vMin.y)*(m_vecTexStageData[i].fNowAnimTime/m_vecTexStageData[i].fAnimTime));
				}
				break;
			case 1:
				{
					m_RenderParam.texOffsetStage1.x = m_vecTexStageData[i].vMin.x + ((m_vecTexStageData[i].vMax.x - m_vecTexStageData[i].vMin.x)*(m_vecTexStageData[i].fNowAnimTime/m_vecTexStageData[i].fAnimTime));
					m_RenderParam.texOffsetStage1.y = m_vecTexStageData[i].vMin.y + ((m_vecTexStageData[i].vMax.y - m_vecTexStageData[i].vMin.y)*(m_vecTexStageData[i].fNowAnimTime/m_vecTexStageData[i].fAnimTime));
				}
				break;
			case 2:
				{
					m_RenderParam.texOffsetStage2.x = m_vecTexStageData[i].vMin.x + ((m_vecTexStageData[i].vMax.x - m_vecTexStageData[i].vMin.x)*(m_vecTexStageData[i].fNowAnimTime/m_vecTexStageData[i].fAnimTime));
					m_RenderParam.texOffsetStage2.y = m_vecTexStageData[i].vMin.y + ((m_vecTexStageData[i].vMax.y - m_vecTexStageData[i].vMin.y)*(m_vecTexStageData[i].fNowAnimTime/m_vecTexStageData[i].fAnimTime));
				}
				break;
			}
		}
		else
		{
			m_vecTexStageData[i].fNowAnimTime = 0.0f;
		}
	}

	if( m_bIsLight == true )
	{
		m_pMatrixLight->Move( m_RenderParam.lightPos );
	}

	return S_OK;
}
#pragma endregion 매 프레임 갱신

#pragma region OnFrameRender
HRESULT CX2ViewerMesh::OnFrameRender()
{
	if( m_pXMesh == NULL )
		return S_OK;

	//-- Wireframe Mode
	if( m_bWireframeMode )
		CKTDGStateManager::SetRenderState( D3DRS_FILLMODE, D3DFILL_WIREFRAME );
	else
		CKTDGStateManager::SetRenderState( D3DRS_FILLMODE, D3DFILL_SOLID );

	//m_pRenderer->BeginRender();    

	m_pMatrix->UpdateWorldMatrix();
	m_pRenderer->OnFrameRender( m_RenderParam, m_pMatrix->GetMatrix(), *m_pXMesh );

	//m_pRenderer->EndRender();

	//Light Rendering
	if( m_bIsLight == true )
	{
		CKTDGStateManager::SetRenderState( D3DRS_FILLMODE, D3DFILL_SOLID );
		m_pMatrixLight->UpdateWorldMatrix( CKTDGMatrix::BT_ALL );

		CKTDGStateManager::PushRenderState( D3DRS_ALPHABLENDENABLE,	true );
		m_pXMeshLight->Render();
		CKTDGStateManager::PopRenderState( D3DRS_ALPHABLENDENABLE );
	}

	return S_OK;
}
#pragma endregion 매 프레임 렌더링

#pragma region OnResetDevice
HRESULT CX2ViewerMesh::OnResetDevice()
{
	m_pRenderer->OnResetDevice();

	if( m_pXMesh == NULL )
		return S_OK;

	return S_OK;
}
#pragma endregion 장치 재설정

#pragma region OnLostDevice
HRESULT CX2ViewerMesh::OnLostDevice()
{
	if( m_pXMesh == NULL )
		return S_OK;

	return S_OK;
}
#pragma endregion 장치 소실

#pragma region Reset
void CX2ViewerMesh::Reset()
{
	m_bWireframeMode	= false;
	m_bIsLight			= true;
	m_bIsBounding		= false;

	INIT_VECTOR3( m_vScale,	1.0f, 1.0f, 1.0f );
	INIT_VECTOR3( m_RenderParam.lightPos, 300, 300, -500 );

	SAFE_CLOSE( m_pXMesh );
}
#pragma endregion 속성 초기화

#pragma region SetMesh
bool CX2ViewerMesh::SetMesh( const WCHAR* fileName, const WCHAR* dir )
{
	Reset();

	m_pXMesh = g_pKTDXApp->GetDeviceManager()->OpenXMesh( fileName );

	if( m_pXMesh == NULL )
	{
		std::string strDir;
		ConvertWCHARToChar( strDir, dir );
		g_pKTDXApp->GetDeviceManager()->GetMassFileManager()->AddDataDirectory( strDir );

		m_pXMesh = g_pKTDXApp->GetDeviceManager()->OpenXMesh( fileName );

		if( m_pXMesh == NULL )
		{
			WARNINGMSG( L"Mesh 파일 열기 실패~ 이런." );
			return false;
		}
	}

	return true;
}
#pragma endregion 메쉬 설정