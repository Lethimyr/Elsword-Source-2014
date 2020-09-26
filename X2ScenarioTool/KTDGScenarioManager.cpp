#include "dxstdafx.h"
#include ".\ktdgscenariomanager.h"

CKTDGScenarioManager::CKTDGScenarioManager(void)
{
	//_SCENARIO_OBJECT_::CTexture* pTexture = new _SCENARIO_OBJECT_::CTexture();
	//pTexture->Load(L"Cursor_Attrib.tga");
	//m_RenderTexList.push_back(pTexture);
}

CKTDGScenarioManager::~CKTDGScenarioManager(void)
{
	//SAFE_CLOSE(m_pTestTex);
	_SCENARIO_OBJECT_::CTextureList::iterator litr;
	litr = m_RenderTexList.begin();
	while(litr != m_RenderTexList.end())
	{
		SAFE_DELETE((*litr));
		++litr;
	}
	m_RenderTexList.clear();
}

HRESULT CKTDGScenarioManager::OnFrameMove(double fTime, float fElapsedTime)
{
	return S_OK;
}

HRESULT CKTDGScenarioManager::OnFrameRender(bool bFront /* = false  */)
{
	//m_pTestTex->Draw( 100, 100, 100, 100, D3DXCOLOR(1.f, 1.f, 1.f, 0.5f) );

	_SCENARIO_OBJECT_::CTextureList::iterator litr;
	litr = m_RenderTexList.begin();

	while(litr != m_RenderTexList.end())
	{
		(*litr)->Render();
		++litr;
	}

	return S_OK;
}

HRESULT CKTDGScenarioManager::OnFrameRenderTopModal()
{
	return S_OK;
}

HRESULT CKTDGScenarioManager::OnResetDevice()
{
	return S_OK;
}

HRESULT CKTDGScenarioManager::OnLostDevice()
{
	return S_OK;
}

bool CKTDGScenarioManager::MsgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	return false;
}

