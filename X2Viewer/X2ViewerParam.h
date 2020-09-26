#pragma once
/**	@file	: X2ViewerParam.h
	@desc	: 렌더링 파라미터
	@author	: Anomymouse

	@date	JHKang on 2013-09-13,
		- 필드 이후 빌드 성공
	@date	JHKang on 2013-09-17,
		- 파일 정리 및 블록화
*/

#pragma region CX2ViewerParam
/** @class : CX2ViewerParam
	@brief : 렌더링 파라미터 데이터를 가지는 클래스
*/
class CX2ViewerParam
{
public:
	typedef std::map<std::wstring, CKTDGXRenderer::RENDER_TYPE> RenderTypeMap;
	typedef	std::map<std::wstring, CKTDGXRenderer::CARTOON_TEX_TYPE> CartoonTexTypeMap;
	typedef std::map<std::wstring, D3DCULL> D3DCullMap;
	typedef std::map<std::wstring, D3DBLEND> D3DBlendMap;
	//typedef std::map<std::wstring, bool> BooleanMap;

public:
	CX2ViewerParam(void);
	~CX2ViewerParam(void);

	void	GetRenderParam( CKTDGXRenderer::RenderParam* renderParam, IMPACT_DATA* impactData,
							std::vector<TEX_STAGE_DATA>& vecTexStageData );

	void	SetParamDlg( CDXUTDialog* pDlg );
	void	GetParamDlg( CDXUTDialog* pDlg );

	void	Reset();
	void	SetEffect();

private:
	RenderTypeMap		m_mapRenderType;		/// Rendering Type Map 데이터
	CartoonTexTypeMap	m_mapCartoonTexType;	/// Cartoon Texture Type Map 데이터
	D3DCullMap			m_mapD3DCull;			/// D3DCULL Map 데이터
	D3DBlendMap			m_mapD3DBlend;			/// D3DBlend Map 데이터
	//BooleanMap			m_mapTrueFalse;			/// AlphaBlend, ZEnable
	bool	m_bAlpha;		/// AlphaBlend
	bool	m_bZEnable;		/// ZEnable
	bool	m_bZWritable;	/// ZWritable

	CKTDGXRenderer::RenderParam*		m_pRenderParam;		/// Rendering Parameter
	IMPACT_DATA							m_ImpactData;		/// 라이트 플로우 효과에 사용하는 데이터
	std::vector<TEX_STAGE_DATA>			m_vecTexStageData;	/// 텍스처 스테이지 데이터
};
#pragma endregion class
