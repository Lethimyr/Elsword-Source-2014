#pragma once

#include "X2FieldMapBase.h"

class CX2MiniMapUI;

// 전체 스크린샷의 크기는 512 x 512라고 가정한다.
class CX2FieldMiniMap : public CX2FieldMapBase
{
private:
	CX2FieldMiniMap( CX2MiniMapUI* pMiniMapUI );
	virtual ~CX2FieldMiniMap();

public:
	static CX2FieldMiniMap* CreateFieldMiniMap( CX2MiniMapUI* pMiniMapUI ) { return new CX2FieldMiniMap(pMiniMapUI); }



	virtual void NotifyShowObjectChanged();
	virtual void OnFrameRender_Draw();

	void SetVillageID( int iMapID );


	void ResetWholeMapTexture( int iMapID );
	void CalcWindowTexturePos( const D3DXVECTOR2& vProjectedMyPos );


	void SetStage(CKTDXStage* val) 
	{ 
		m_pStage = val; 


	}

private:
	CKTDXStage*		m_pStage;
	CX2MiniMapUI*	m_pMiniMapUI;

		
};
