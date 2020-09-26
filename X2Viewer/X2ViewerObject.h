#pragma once
/**	@file	: X2ViewerMesh.h
	@desc	: 뷰 객체 정의
	@author	: Anomymouse

	@date	JHKang on 2013-09-13,
		- 필드 이후 빌드 성공
	@date	JHKang on 2013-09-17,
		- 파일 정리 및 블록화
*/

#pragma region CX2ViewerObject
/** @class : CX2ViewerObject
	@brief : 뷰 오브젝트
	@date  : 2013-09-17
*/
class CX2ViewerObject
{
	public:
		enum OBJECT_STYLE
		{
			OS_NONE		= 0,	/// 없음
			OS_GRID,			/// 격자
			OS_CAMERA,			/// 카메라
			OS_SKIN_MESH,		/// 스킨 메쉬
			OS_MESH,			/// 메쉬
			OS_UI,				/// UI
			OS_WORLD_MESH,		/// 월드 메쉬

#ifndef UPDATE_X2VIEWER_2013 //JHKang
			OS_PARTICLE,
			OS_PARTICLE_EDITOR,
#endif //UPDATE_X2VIEWER_2013
		};

	public:
		CX2ViewerObject(void);
		virtual ~CX2ViewerObject(void);

		virtual HRESULT OnFrameMove( double fTime, float fElapsedTime ){return S_OK;}
		virtual HRESULT OnFrameRender(){return S_OK;}

		virtual bool MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam ){return false;}

		virtual HRESULT OnResetDevice(){return S_OK;}
		virtual HRESULT OnLostDevice(){return S_OK;}

		OBJECT_STYLE	GetObjectStyle()							{ return m_ObjectStyle; }
		void			SetObjectStyle( OBJECT_STYLE objectStyle )	{ m_ObjectStyle = objectStyle; }              
        
	private:
		OBJECT_STYLE	m_ObjectStyle;	/// 오브젝트 스타일
};
#pragma endregion class
