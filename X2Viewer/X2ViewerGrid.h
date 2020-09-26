#pragma once
/**	@file	: X2ViewerGrid.h
	@desc	: 뷰어 내부에 격자 출력
	@author	: Anomymous

	@date	JHKang on 2013-09-13,
		- 필드 이후 빌드 성공
	@date	JHKang on 2013-09-17,
		- 파일 정리 및 블록화
*/

#define D3DFVF_GRID_VERTEX (D3DFVF_XYZ | D3DFVF_DIFFUSE)
#define LINE_NUM	46

#pragma region CX2ViewerGrid
/** @class : CX2ViewerGrid
	@brief : 격자
	@date  : 2013-09-17
*/
class CX2ViewerGrid : public CX2ViewerObject
{
	public:
		struct GRID_VERTEX
		{
			D3DXVECTOR3	pos;	/// 위치
			DWORD		color;	/// 색상
		};

	public:
		CX2ViewerGrid(void);
		virtual ~CX2ViewerGrid(void);

		virtual HRESULT OnFrameMove( double fTime, float fElapsedTime );
		virtual HRESULT OnFrameRender();
		virtual HRESULT OnResetDevice();
		virtual HRESULT OnLostDevice();

		void	SetOnOff( bool bOnOff ){ m_bOnOff = bOnOff; }

	private:
		bool	Init();

	private:
		
		//LPDIRECT3DDEVICE9			m_pd3dDevice;
		LPDIRECT3DVERTEXBUFFER9		m_pGridVB;		/// 정점 버퍼
		CKTDGMatrix*				m_pMatrix;		/// 행렬

		//CKTDGFontManager::CKTDGFont*	m_pFont;
		CKTDGFontManager::CUKFont*	m_pFont;		/// 폰트

		CKTDIDevice*	m_pKTDIMouse;		/// 마우스 장치
		bool			m_bOnOff;			/// On/Off
};
#pragma endregion class