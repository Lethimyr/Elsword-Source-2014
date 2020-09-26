#pragma once
#include "X2EffectToolEditor.h"

class CX2EffectToolMain;

class CX2EffectToolMeshEditor : public CX2EffectToolEditor
{
	enum EVENT_LIST_TYPE
	{
		ELT_NONE,
		ELT_ANIM,
		ELT_DIE,
		ELT_GLOBAL,
	};

public:
	int GetIndexByEnum( CKTDGXSkinAnim::XSKIN_ANIM_PLAYTYPE eEnumValue );
	int GetIndexByEnum( CKTDGXMeshPlayer::MESH_TYPE eEnumValue );
	int GetIndexByEnum( CKTDGXMeshPlayer::LIFE_TYPE eEnumValue);
	int GetIndexByEnum( CKTDGXRenderer::CARTOON_TEX_TYPE eEnumValue );
	int GetIndexByEnum( CKTDGSlashTrace::CURVE_TYPE eEnumValue);
	int GetIndexByEnum( CKTDGSlashTrace::SLASH_TRACE_TYPE eEnumValue  );
	int GetIndexByEnum( CKTDGXMeshPlayer::EVENT_TYPE eEnumValue  );
	int GetIndexByEnum( D3DBLEND eEnumValue );
	int GetIndexByEnum( _D3DCULL  eEnumValue  );
	int GetIndexByEnum( CKTDGXRenderer::RENDER_TYPE eEnumValue );
	int GetIndexByEnum( CKTDGMatrix::BILLBOARD_TYPE  eEnumValue  );
	int GetIndexByEnum( EVENT_LIST_TIME_TYPE eEnumValue );
public:
	CX2EffectToolMeshEditor( CX2EffectToolMain* pEffectTool_);
	~CX2EffectToolMeshEditor();

	void CloseDialog();
	void CreateDialogMeshEditor(CKTDGXMeshPlayer::XMeshTemplet* pMeshTemplet_ = NULL);

	void UpdateMeshData( CKTDGXMeshPlayer::XMeshTemplet* pMeshTemplet_ );
	void OnFrameMove_HotKey();

	//////////////////////////////////////////////////////////////////////////
	// Button
	void ON_PA_IDC_EvnetList_Global_ADD();
	void ON_PA_IDC_EvnetList_Global_DEL();
	void ON_PA_IDC_EvnetList_Anim_ADD();
	void ON_PA_IDC_EvnetList_Anim_DEL();
	void ON_PA_IDC_EvnetList_Die_ADD();
	void ON_PA_IDC_EvnetList_Die_DEL();
	
	// UP / DOWN
	void ON_PA_IDC_EvnetList_Global_UP();
	void ON_PA_IDC_EvnetList_Global_DOWN();
	void ON_PA_IDC_EvnetList_Anim_UP();
	void ON_PA_IDC_EvnetList_Anim_DOWN();
	void ON_PA_IDC_EvnetList_Die_UP();
	void ON_PA_IDC_EvnetList_Die_DOWN();

	// List
	void ON_PA_LIST_EventList_Global_SelChnage( int iIndex_ );
	void ON_PA_LIST_EventList_Anim_SelChnage( int iIndex_ );
	void ON_PA_LIST_EventList_Die_SelChnage( int iIndex_ );

	void SaveMeshData();
private:
	//////////////////////////////////////////////////////////////////////////
	// Event List
	CKTDGXMeshPlayer::CXMeshEvent* CreateEventList( CKTDGXMeshPlayer::CXMeshEvent* pEvent_ );
	CKTDGXMeshPlayer::CXMeshEvent* CreateEventList( CKTDGXMeshPlayer::EVENT_TYPE eEventType );
	CKTDGXMeshPlayer::CXMeshEvent* GetEventDataByIndex( int iIndex_ , EVENT_LIST_TYPE eListType_ = ELT_NONE );
	vector<CKTDGXMeshPlayer::CXMeshEvent*>* GetSelectEventList();

	void	SelectEventListByIndex( int iIndex_ );
	void	SetSelectEventListType( CX2EffectToolMeshEditor::EVENT_LIST_TYPE eListType_ ){m_eSelectEventListType =eListType_; }
	int		GetSelectEventListIndex();
	EVENT_LIST_TIME_TYPE GetEventListTimeType ( CKTDGXMeshPlayer::CXMeshEvent* pEventData );

	void UpdateGlobalEventDataByIndex( int iIndex_ );
	void UpdateAnimEventDataByIndex( int iIndex_ );
	void UpdateDieEventDataByIndex( int iIndex_ );
	void UpdateEventData( CKTDGXMeshPlayer::CXMeshEvent* pEventData );
	void UpdateEventList();

	void Move_EventList_Up( EVENT_LIST_TYPE eEventListType_, vector<CKTDGXMeshPlayer::CXMeshEvent*>& vecEventList_ );
	void Move_EventList_Down( EVENT_LIST_TYPE eEventListType_, vector<CKTDGXMeshPlayer::CXMeshEvent*>& vecEventList_ );

	CKTDGXMeshPlayer::CXMeshEvent* GetGlobalEventDataByIndex( int iIndex_ );
	CKTDGXMeshPlayer::CXMeshEvent* GetAnimEventDataByIndex( int iIndex_ );
	CKTDGXMeshPlayer::CXMeshEvent* GetDieEventDataByIndex( int iIndex_ );

	void	SetEventDataByUIValue( OUT CKTDGXMeshPlayer::CXMeshEvent* pEventData_ );

	//////////////////////////////////////////////////////////////////////////
	// Mesh Data
	void SaveMeshData_Column1();
	void SaveMeshData_Column2();
	void SaveMeshData_EventList();
	bool SaveMeshData_OtherName();

private:
	EVENT_LIST_TYPE						m_eSelectEventListType;
	CKTDGXMeshPlayer::XMeshTemplet*		m_pMeshTemplet;
	CX2EffectToolMain*					m_pEffectTool;
};