#pragma once
/**	@file	: X2ViewerMain.h
	@desc	: 뷰어 메인 코드
	@author	: Anomymouse

	@date	JHKang on 2013-09-13,
		- 필드 이후 빌드 성공
	@date	JHKang on 2013-09-17,
		- 파일 정리 및 블록화
*/

// 빌드시 #define X2VIEWER 해 주세요. ( KTDXLIB )

class CX2ViewerMain : public CKTDXStage
{
	public:
		CX2ViewerMain(void);
		virtual ~CX2ViewerMain(void);

		virtual HRESULT OnFrameMove( double fTime, float fElapsedTime );
		virtual HRESULT OnFrameRender();

		virtual bool MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

		virtual HRESULT OnResetDevice();
		virtual HRESULT OnLostDevice();

		CX2ViewerObject*	GetObject( CX2ViewerObject::OBJECT_STYLE objectStyle );

//		D3DXMATRIX*			GetWeaponMatrix() { return m_pWeaponMatrix; }
//		void				SetWeaponMatrix( D3DXMATRIX* pMatrix ) { m_pWeaponMatrix = pMatrix; }

		int					GetSelectedAnimIndex() { return m_SelectedAnimIndex; }
		void				SetSelectedAnimIndex( int index ) { m_SelectedAnimIndex = index; }

		wstring				GetAnimFileName() { return m_AnimFileName; }
		void				SetAnimFileName(const WCHAR* pName) { m_AnimFileName = pName; }

		wstring				GetAnimDirName() { return m_AnimFileDir; }
		void				SetAnimDirName(const WCHAR* pName) { m_AnimFileDir = pName; }

		// 09.12.23 태완 :(요청에 의한 기능 추가) Resource 읽어오는 폴더를 추가한다. 해당값은 LUA에서 수정하도록 한다. 
		void				ReadAdditionalResourceFolder();
		void				AddAdditionalResourceFolder( string folder, bool bIncludeSub );

		void	RegisterLuabind( KLuabinder* pKLuabinder );

	private:
		std::vector<CX2ViewerObject*>	m_vecObject;
		CX2ViewerSkinMesh*				m_pWeaponXSkinMesh;

//		D3DXMATRIX*						m_pWeaponMatrix;
		int								m_SelectedAnimIndex;
		wstring							m_AnimFileName;
		wstring							m_AnimFileDir;
};
