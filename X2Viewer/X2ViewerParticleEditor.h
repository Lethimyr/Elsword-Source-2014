#pragma once

//////////////////////////////////////////////////////////////////////////
// 2009.12 태완
// Particle Editing 기능을 위해 사용하는 Class.
// 
// Particle 속성(이벤트 또는 Emitter 속성)값이 추가되었을 때 추가하는 방법 :
// Emitter의 경우 enum EMITTER_PROPERTIES에 추가, Event의 경우 enum CKTDGParticle::EVENT_TYPE에 추가
// CX2ViewerParticleEditor의 생성자에서 Emitter는 m_aEmitterProperties값, Event는 m_aEventProperties값 설정
// 해당 value type에 대한 Get__Value, Set__Value 함수 내에 해당값에 대한 처리 코드를 추가
//

class CX2ViewerParticleEditor
{
public:

	// 파티클 Property가 같는 value의 타입.
	enum VALUE_TYPE
	{
		VT_INT,					// int			입력 Box 하나
		VT_FLOAT,				// float		입력 Box 하나
		VT_STRING,				// wstring		입력 Box 하나
		VT_XYZ,					// D3DVECTOR3	입력 Box 세개
		VT_XY,					// D3DVECTOR2	입력 Box 두개
		VT_RGBA,				// D3DCOLOR		입력 Box 네개
		VT_BOOL,				// bool			True/False 콤보박스
		VT_COMBO_CUSTOM,		// 기타 특정 enum값 등을 사용하는 경우 입력창을 ComboBox로 사용하고 싶을 때. 
								// 사용방식은 X2ViewerParticleEditor.cpp의 Get/SetCustomComboValue 함수 참조.
	};

	// ParticleEventSequence 자체가 갖는 속성값에 대한 enum.
	enum EMITTER_PROPERTIES
	{
		EP_PARTICLETYPE,
		EP_MESHRENDERTYPE,
		EP_LAYER,
		EP_SRCBLENDMODE,
		EP_DSTBLENDMODE,
		EP_CULLMODE,
		EP_NUMPARTICLES,
		EP_GRAVITY,
		EP_LIFETIME,
		EP_TRIGGERTIME,
		EP_TRIGGERCOUNT,
		EP_EMITRATE,
		EP_EMITRADIUS,
		EP_EMITRADIUSMIN,
		EP_ADDROTATEREL,
		EP_TRACEPARTICLELIST,
		EP_FINALPARTICLELIST,
		EP_BILLBOARDTYPE,
		EP_MESHNAME,
		EP_CHANGETEXNAME,
		EP_MULTITEXXET,
		EP_ANIXET,
		EP_ANINAME,
		EP_LANDPOS,
		EP_USELAND,
		EP_BLACKHOLEPOS,
		EP_LIGHTPOS,
		EP_RESOLUTIONCONVERT,
		EP_TRACE,
		EP_ZENABLE,
		EP_ZWRITEENABLE,
		EP_FORCELAYER,
		EP_DRAWCOUNT,
		EP_SCREW_VALUE,
		EP_SCREW_ROTATESPEED,
		EP_SPHERICAL_EMITRADIUS,
		EP_SPHERICAL_EMITAZIMUTH,
		EP_SPHERICAL_EMITPOLAR,
		EP_SPHERICAL_EMITROTATION,

		EP_END,
	};

	struct PropertyData
	{	
		int m_Type;					// 어떤 값인가? ( EMITTER_PROPERTIES 또는 CKTDGParticle::EVENT_TYPE )
		wstring m_name;				// 해당 속성값의 이름
		VALUE_TYPE m_valuetype;		// 해당 속성값의 Type
		bool m_bAllowMinMax;		// 해당 속성에 random 값 사용이 허용되는지 ( CMinMax인지 )

		void Init( int PropertyType, wstring name, VALUE_TYPE ValueType, bool bAllowMinMax )
		{
			m_Type = PropertyType;
			m_name = name;
			m_valuetype = ValueType;
			m_bAllowMinMax = bAllowMinMax;
		}
	};

	CX2ViewerParticleEditor( /* UI 받아두자 */ );
	virtual ~CX2ViewerParticleEditor(void);

	PropertyData*		GetEmitterProperties( EMITTER_PROPERTIES prop ){ return &(m_aEmitterProperties[prop]); }
	PropertyData*		GetEventProperties( CKTDGParticleSystem::EVENT_TYPE prop ){ return &(m_aEventProperties[prop]); }

	// 스크립트 출력을 위한 function
	bool				ExportParticleScript( CKTDGParticleSystem* pSystem, const WCHAR* pwszFileFullPath );
	wstring				MakeParticleEventSequenceString( CKTDGParticleSystem::CParticleEventSequence* pSeq );
	wstring				MakeParticleEventString( CKTDGParticleSystem::CParticleEvent* pEvent, const float FinalTime );

	// UI 입출력을 위한 Utility function
	template <class T>
		wstring GetMinMaxString( T valmin, T valmax );
	wstring GetMinMaxString( CMinMax<float> valfloat );
	wstring GetMinMaxString( CMinMax<D3DXVECTOR3> valV3 );
	wstring GetMinMaxString( CMinMax<D3DXVECTOR2> valV2 );

	// Particle Event의 시간 설정을 스크립트로 출력할때를 위함
	wstring GetEventTimeString( CKTDGParticleSystem::CParticleEvent* pEvent, float Finaltime );

	CKTDGParticleSystem::CParticleEvent* EventFactory( CKTDGParticleSystem::EVENT_TYPE eventtype );

	CMinMax<float> ParseCMinMaxFloat( wstring str );
	CMinMax<D3DXVECTOR2> ParseCMinMaxD3DVECTOR2( wstring strX, wstring strY );
	CMinMax<D3DXVECTOR3> ParseCMinMaxD3DVECTOR3( wstring strX, wstring strY, wstring strZ );
	
	//////////////////////////////////////////////////////////////////////////
	// 각 Value Type에 따른 Get/Set Method ( Emitter 값 )
	
	CMinMax<D3DXVECTOR3> GetXYZValue( CKTDGParticleSystem::CParticleEventSequence* pSeq, EMITTER_PROPERTIES prop );
	CMinMax<D3DXVECTOR2> GetXYValue( CKTDGParticleSystem::CParticleEventSequence* pSeq, EMITTER_PROPERTIES prop );
	CMinMax<float> GetFloatValue( CKTDGParticleSystem::CParticleEventSequence* pSeq, EMITTER_PROPERTIES prop );
	int	GetIntValue( CKTDGParticleSystem::CParticleEventSequence* pSeq, EMITTER_PROPERTIES prop );
	wstring GetStringValue( CKTDGParticleSystem::CParticleEventSequence* pSeq, EMITTER_PROPERTIES prop );
	bool GetBoolValue( CKTDGParticleSystem::CParticleEventSequence* pSeq, EMITTER_PROPERTIES prop );
	void GetCustomComboBoxValue( CKTDGParticleSystem::CParticleEventSequence* pSeq, EMITTER_PROPERTIES prop, CDXUTComboBox* pCombo );

	void SetXYZValue( CKTDGParticleSystem::CParticleEventSequence* pSeq, EMITTER_PROPERTIES prop, CMinMax<D3DXVECTOR3> val );
	void SetXYValue( CKTDGParticleSystem::CParticleEventSequence* pSeq, EMITTER_PROPERTIES prop, CMinMax<D3DXVECTOR2> val );
	void SetFloatValue( CKTDGParticleSystem::CParticleEventSequence* pSeq, EMITTER_PROPERTIES prop, CMinMax<float> val );
	void SetIntValue( CKTDGParticleSystem::CParticleEventSequence* pSeq, EMITTER_PROPERTIES prop, int val );
	void SetStringValue( CKTDGParticleSystem::CParticleEventSequence* pSeq, EMITTER_PROPERTIES prop, wstring val );
	void SetBoolValue( CKTDGParticleSystem::CParticleEventSequence* pSeq, EMITTER_PROPERTIES prop, bool val );
	void SetCustomComboBoxValue( CKTDGParticleSystem::CParticleEventSequence* pSeq, EMITTER_PROPERTIES prop, DXUTComboBoxItem* pItem );

	//////////////////////////////////////////////////////////////////////////
	// 각 Value Type에 따른 Get/Set Method ( Event 값 )
	CMinMax<D3DXVECTOR3> GetXYZValue( CKTDGParticleSystem::CParticleEvent* pEvent, CKTDGParticleSystem::EVENT_TYPE prop );
	CMinMax<D3DXVECTOR2> GetXYValue( CKTDGParticleSystem::CParticleEvent* pEvent, CKTDGParticleSystem::EVENT_TYPE prop );
	CMinMax<float> GetFloatValue( CKTDGParticleSystem::CParticleEvent* pEvent, CKTDGParticleSystem::EVENT_TYPE prop );
	wstring GetStringValue( CKTDGParticleSystem::CParticleEvent* pEvent, CKTDGParticleSystem::EVENT_TYPE prop );
	CMinMax<D3DXCOLOR> GetRGBAValue( CKTDGParticleSystem::CParticleEvent* pEvent, CKTDGParticleSystem::EVENT_TYPE prop );

	void SetXYZValue( CKTDGParticleSystem::CParticleEvent* pEvent, CKTDGParticleSystem::EVENT_TYPE prop, CMinMax<D3DXVECTOR3> val );
	void SetXYValue( CKTDGParticleSystem::CParticleEvent* pEvent, CKTDGParticleSystem::EVENT_TYPE prop, CMinMax<D3DXVECTOR2> val );
	void SetFloatValue( CKTDGParticleSystem::CParticleEvent* pEvent, CKTDGParticleSystem::EVENT_TYPE prop, CMinMax<float> val );
	void SetStringValue( CKTDGParticleSystem::CParticleEvent* pEvent, CKTDGParticleSystem::EVENT_TYPE prop, wstring val );
	void SetRGBAValue( CKTDGParticleSystem::CParticleEvent* pEvent, CKTDGParticleSystem::EVENT_TYPE prop, CMinMax<D3DXCOLOR> val );

private:

	PropertyData	m_aEmitterProperties[EP_END];
	PropertyData	m_aEventProperties[CKTDGParticleSystem::ET_END];


};
