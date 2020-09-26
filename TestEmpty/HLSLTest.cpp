#include "dxstdafx.h"
#include ".\hlsltest.h"

float offset = 0;
CHLSLTest::CHLSLTest(void)
{
	HRESULT hr		= 0;
	m_pd3dDevice	= g_pKTDXApp->GetDevice();
	m_pEffect		= NULL;

	KGCMassFileManager::CMassFile::MASSFILE_MEMBERFILEINFO_POINTER Info;
	Info = g_pKTDXApp->GetDeviceManager()->GetMassFileManager()->LoadDataFile( L"TestFX.fx" );
	if( Info == NULL )
		return;

	if(FAILED(hr = D3DXCreateEffect( m_pd3dDevice, Info->pRealData, Info->size,
		NULL, NULL, D3DXFX_NOT_CLONEABLE, NULL, &m_pEffect, NULL )))
	{
		SAFE_RELEASE( m_pEffect );
		return;
	}

	m_pXMesh	= g_pKTDXApp->GetDeviceManager()->OpenXMesh( L"field4.X" );
	m_pMatrix	= new CKTDGMatrix( m_pd3dDevice );
	g_pKTDXApp->GetDGManager()->GetCamera().Point( 0,1300,-3000, 0,500,0 );

	m_pTex = g_pKTDXApp->GetDeviceManager()->OpenTexture( L"Arme_FireballB2.dds" );
}

CHLSLTest::~CHLSLTest(void)
{
	SAFE_RELEASE( m_pEffect );
	SAFE_DELETE( m_pMatrix );
}

HRESULT CHLSLTest::OnFrameMove( double fTime, float fElapsedTime )
{
	g_pKTDXApp->GetDGManager()->GetCamera().UpdateCamera( fElapsedTime );

	offset += fElapsedTime;
	return S_OK;
}

HRESULT CHLSLTest::OnFrameRender()
{
	if( FAILED( m_pEffect->SetTechnique( "REAL_COLOR" ) ) )
		return E_FAIL;

	m_pTex->SetDeviceTexture( 1 );

	D3DXMATRIX mViewProj;
	D3DXMATRIX mWorld = m_pMatrix->GetMatrix();
	D3DXMATRIX mFinal;
	D3DXCOLOR color = 0x55ffff00;
	D3DXVECTOR4 colorIn = (D3DXVECTOR4)color;

	mViewProj = g_pKTDXApp->GetViewProjectionTransform();
	D3DXMatrixMultiply( &mFinal, &mWorld, &mViewProj );

	m_pEffect->SetMatrix( "g_mViewProj",		&mViewProj );
	m_pEffect->SetMatrix( "g_mWorld",			&mWorld );
	m_pEffect->SetMatrix( "g_mWorldViewProj",	&mFinal );
	m_pEffect->SetVector( "g_Color",			&colorIn );
	m_pEffect->SetFloat( "g_Offset",			offset );

	UINT uiPass;
	UINT m_Pass;
	m_pEffect->Begin( &m_Pass, 0 );

	for( uiPass = 0; uiPass < m_Pass; ++ uiPass )
	{
		m_pEffect->BeginPass( uiPass );
		m_pXMesh->Render();
		m_pEffect->EndPass();
	}

	m_pEffect->End();
	m_Pass = 0;

	return S_OK;
}

HRESULT CHLSLTest::OnResetDevice()
{
	return m_pEffect->OnResetDevice(); 
}

HRESULT CHLSLTest::OnLostDevice()
{
	return m_pEffect->OnLostDevice();
}