#pragma once
#include "x2baseunitviewerui.h"

class CX2NPCUnitViewerUI : public CX2BaseUnitViewerUI
{
private:
	CX2NPCUnitViewerUI(void);
	virtual ~CX2NPCUnitViewerUI(void);

public: 
	static CX2NPCUnitViewerUI* Create() { return new CX2NPCUnitViewerUI; }
	bool ResetNPC( CX2UnitManager::NPC_UNIT_ID eNPCID );
	virtual HRESULT OnFrameMove( double fTime, float fElapsedTime );
	virtual RENDER_HINT OnFrameRender_Prepare();
	virtual void    OnFrameRender_Draw();
	virtual HRESULT OnResetDevice();
	virtual HRESULT OnLostDevice();

	void ResetRenderParam();
	void PlayAnim( const WCHAR* pName, CKTDGXSkinAnim::XSKIN_ANIM_PLAYTYPE playType = CKTDGXSkinAnim::XAP_LOOP, bool bTransition = false );
	void SetPosition( const D3DXVECTOR3& vPosition );
	void SetScale( const D3DXVECTOR3& vScale );

#ifdef SERV_PET_SYSTEM
	bool ResetPet( int petId, char iLv );
	bool GetForceBillBoard() { return m_bForceBillBoard; }
	float GetScale() { return m_fScale; }
#endif

#ifdef RIDING_SYSTEM
	bool ResetRidingPet( int RidingPetId );
#endif //RIDING_SYSTEM

private:
	CKTDXDeviceXSkinMesh*	m_pXSkinMesh;
	CKTDGXSkinAnimPtr		m_pXSkinAnim;
	D3DXVECTOR3				m_vLightPosition;

#ifdef SERV_PET_SYSTEM
	CKTDGXRenderer::RenderParam			m_RenderParam;
	float								m_fScale;
	bool								m_bForceBillBoard;

	CKTDXDeviceXSkinMesh*				m_pChangeTexXET;
#endif
};





//
//class CX2UnitViewerUI : public CKTDGObject
//{
//	CKTDGXRenderer::RenderParam* GetRenderParam();
//	
//	CKTDGXSkinAnim* GetXSkinAnim(){ return m_pXSkinAnim.get(); }
//	CKTDGXSkinAnimPtr GetXSkinAnimPtr() { return m_pXSkinAnim; }
//
//
//	void SetLightPos( float x, float y, float z )
//	{ 
//		m_LightPos.x = x;
//		m_LightPos.y = y;
//		m_LightPos.z = z;
//	}
//	void SetLightPos( D3DXVECTOR3 lightPos )
//	{ 
//		m_LightPos = lightPos;
//	}
//
//	void SetBillBoardType( CKTDGMatrix::BILLBOARD_TYPE billBoardType ){ m_BillBoardType = billBoardType; }
//
//	void ReplaceEmptyToBasic();
//	void ReplaceEmptyToBasic( CX2Unit::EQIP_POSITION eqipPos, int itemID );
//
//	static void SetShowUnitViewerUI( bool bShow ) { m_sbShow = bShow; }
//
//	void PlayByMotionType( CX2UnitViewerUI::UNIT_VIEWER_UI_MOTION_TYPE motionType );
//
//	static bool GetUnitClicked() { return m_sbUnitClicked; }
//	static void SetUnitClicked( bool bCheck ) { m_sbUnitClicked = bCheck; }
//
//
//#ifdef NEW_VILLAGE_UI
//	void SetPositionOnScr(float x, float y, float z = 0.8f, float scale = 1.f );
//	void SetFixed(bool val);
//	bool GetFixed() { return m_bFixed; }
//#endif
//
//
//	bool    IsFashionItemEquippedAt( CX2Unit::EQIP_POSITION eqipPos )
//	{
//		ASSERT( eqipPos >= CX2Unit::EQIP_POSITION( 0 ) && eqipPos < CX2Unit::EP_END );
//		return m_ViewEquipFashion[ eqipPos ] != 0;
//	}
//
//
//
//
//private:
//
//
//
//
//
//
//
//	////{{ robobeg : 2008-10-17
//	//// 현재의 EQUIP_POSITION 의 부품 착용 여부를 알려주는 함수
//	//bool    IsItemEquippedAt( CX2Unit::EQIP_POSITION eqipPos )
//	//{
//	//	ASSERT( eqipPos >= CX2Unit::EQIP_POSITION( 0 ) && eqipPos < CX2Unit::EP_END );
//	//	return m_ViewEquipNormal[ eqipPos ] != 0 || m_ViewEquipFashion[ eqipPos ] != 0;
//	//}//IsItemEquippedAt()
//	////}} robobeg : 2008-10-17
//
//
//	/// 내부적으로 관리하는 객체이므로 smart pointer를 사용하지 않는다.
//	/// - jintaeks on 2009-01-12, 16:12		
//	// dmlee, 2009-03-12 백그라운드 로딩시에 dangling pointer 참조 오류 생기는 문제 때문에 smart poiner로 바꿨습니다.
//	CKTDXDeviceXSkinMesh*	m_pMotion;
//	CKTDGXSkinAnimPtr		m_pXSkinAnim;
//	D3DXVECTOR3				m_LightPos;
//
//
//	//typedef std::map<UidType,CX2EqipPtr>    ViewEquipMap;
//
//	//ViewEquipMap            m_ViewEquipMap;								// ED템, 패션템 모두 들어있음
//	//UidType                 m_ViewEquipNormal[CX2Unit::EP_END];
//	//UidType                 m_ViewEquipFashion[CX2Unit::EP_END];
//
//	CKTDGMatrix::BILLBOARD_TYPE m_BillBoardType;
//
//	static bool										m_sbShow;
//
//
//	static bool										m_sbUnitClicked;
//
//	
//#ifdef NEW_VILLAGE_UI
//	D3DXVECTOR3 m_vScrPos;
//	bool		m_bFixed;
//#endif
//
//	bool		m_bPickUnit;
//
//
//
//};
//
