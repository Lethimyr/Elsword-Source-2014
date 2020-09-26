/**	@file	: X2ViewerMesh.h
	@desc	: 뷰 객체 정의
	@author	: Anomymouse

	@date	JHKang on 2013-09-13,
		- 필드 이후 빌드 성공
	@date	JHKang on 2013-09-17,
		- 파일 정리 및 블록화
*/

#include "dxstdafx.h"
#include ".\x2viewerobject.h"

#pragma region CX2ViewerObject
CX2ViewerObject::CX2ViewerObject(void)
	: m_ObjectStyle( OS_NONE )
{
}
#pragma endregion 생성자

#pragma region ~CX2ViewerObject
CX2ViewerObject::~CX2ViewerObject(void)
{
	m_ObjectStyle = OS_NONE;
}
#pragma endregion 소멸자