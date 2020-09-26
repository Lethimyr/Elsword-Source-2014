#pragma once
#include "X2EffectToolEditor.h"
class CX2EffectToolParticleEditor : public CX2EffectToolEditor
{
public:
	enum PARTICLE_DIALOG_TYPE
	{
		PDT_NONE,
		PDT_BASE,
		PDT_ETC,
	};

	int GetIndexByEnum( CKTDGParticleSystem::PARTICLE_TYPE eEnumValue );
	int GetIndexByEnum( CKTDGMatrix::BILLBOARD_TYPE  eEnumValue  );
	int GetIndexByEnum( CKTDGXRenderer::RENDER_TYPE eEnumValue );
	int GetIndexByEnum( D3DBLEND eEnumValue );
	int GetIndexByEnum( _D3DCULL  eEnumValue  );
	int GetIndexByEnum( EVENT_LIST_TIME_TYPE eEnumValue );;
	int GetIndexByEnum( CKTDGParticleSystem::EVENT_TYPE eEnumValue  );
public:
	CX2EffectToolParticleEditor(CX2EffectToolMain* pEffectTool_);
	~CX2EffectToolParticleEditor(void);

	void CloseDialog();
	void CreateParticleEditor_Base( CKTDGParticleSystem::CParticleEventSequence* pParticle_ = NULL, wstring wstParticleFileName_ = L"" );
	void CreateParticleEditor_Etc(CKTDGParticleSystem::CParticleEventSequence* pParticle_ = NULL, wstring wstParticleFileName_ = L"" );

	void UpdateParticleData( CKTDGParticleSystem::CParticleEventSequence* pParticle_, wstring wstParticleFileName_ = L"");
	void OnFrameMove_HotKey();

	void UpdateEventDataByIndex( int iIndex_ );
	void SaveParticleData();

	void ON_PA_IDC_EvnetList_ADD();
	void ON_PA_IDC_EvnetList_DEL();
	void ON_PA_IDC_EvnetList_UP();
	void ON_PA_IDC_EvnetList_DOWN();

	// ETC
	void ON_PA_BUTTON_Final_Seq_ADD();
	void ON_PA_BUTTON_Final_Seq_DEL();
	void ON_PA_BUTTON_Trace_Seq_ADD();
	void ON_PA_BUTTON_Trace_Seq_DEL();

private:
	// 파일로부터 읽어오기.
	void UpdateParticleData_Column1();
	void UpdateParticleData_Column2();	
	// ETC 
	void UpdateParticleData_ETC_Column1();
	void UpdateParticleData_ETC_Column2();

	void UpdateEventList();
	void UpdateEventData( CKTDGParticleSystem::CParticleEvent* pEventData_ );
	EVENT_LIST_TIME_TYPE GetEventListTimeType ( CKTDGParticleSystem::CParticleEvent* pEventData_ );

	// 파일에 쓰기
	bool SaveParticleData_OtherName();

	void SaveParticleData_Column1();
	void SaveParticleData_Column2();
	void SaveParticleData_EventList();
	// ETC 
	void SaveParticleData_ETC_Column1();

	void SetEventDataByUIValue( CKTDGParticleSystem::CParticleEvent* pEventData_, bool bInit_ = false );

	int GetSelectEventListIndex();
	void SelectEventListByIndex( int iIndex_ );
	void EraseEventListByIndex( int iIndex_ );
	void InsertEventListByIndex ( int iIndex_, CKTDGParticleSystem::CParticleEvent* pEventData_);

	CKTDGParticleSystem::CParticleEvent* GetSelectEventData();
	CKTDGParticleSystem::CParticleEvent* CreateEventList( CKTDGParticleSystem::EVENT_TYPE eEventType_ );
	CKTDGParticleSystem::CParticleEvent* CreateEventList( CKTDGParticleSystem::CParticleEvent* pEvent_ );

	// 오현빈 // Set / Get에서의 수치가 다른 방식으로 처리 하기 때문에 ConvertRadianToDegreeByMinMax 함수로 예외처리
	inline const CMinMax<float> ConvertRadianToDegree( const CMinMax<float>* cMinMaxFloat_ ) const { return CMinMax<float>( cMinMaxFloat_->m_Min / D3DX_PI*180.f,  cMinMaxFloat_->m_Max / D3DX_PI*180.f );}
	inline const D3DXVECTOR3	ConvertRadianToDegree( const D3DXVECTOR3* vec_	) const { return ((*vec_) / D3DX_PI*180.f) ;}

	inline const CMinMax<float> ConvertDegreeToRadian( const CMinMax<float>* cMinMaxFloat_ ) const { return CMinMax<float>( cMinMaxFloat_->m_Min * D3DX_PI / 180.f,  cMinMaxFloat_->m_Max * D3DX_PI / 180.f );}

private:
	CKTDGParticleSystem::CParticleEventSequence*	m_pParticleSequence;
	wstring											m_wstrParticleFileName;
	CX2EffectToolMain*								m_pEffectTool;

	PARTICLE_DIALOG_TYPE							m_eDialogType;

	static const UINT								m_uiMAXParticleNum = 400;

};