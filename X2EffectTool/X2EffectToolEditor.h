#pragma once

#define MAX_EVENT_LIST_TIME_TYPE 4
#define MAX_PARTICLE_TYPE 4
#define MAX_EVENT_TYPE	17
#define MAX_BILL_BOARD_TYPE	5
#define MAX_RENDER_TYPE 11
#define MAX_BLEND_MODE 15
#define MAX_CULL_MODE 4

#define MAX_PLAY_TYPE 3
#define MAX_MESH_TYPE 2
#define MAX_MESH_EVENT_TYPE	22
#define MAX_LIFE_TYPE 3
#define MAX_CURVE_TYPE 3
#define MAX_CARTOON_TEX_TYPE 10
#define MAX_SLASH_TRACE_TYPE 5

typedef struct
{
	WCHAR strName[100];
	int  iIndex;
} NameAndIndex;

enum EVENT_LIST_TIME_TYPE
{
	ELTT_NONE = -1,
	ELTT_INITITAL,
	ELTT_AT,
	ELTT_FADE_SO_FROM,
	ELTT_FADE_SO_FINAL,
};
class CX2EffectToolEditor
{
public:
	CX2EffectToolEditor(void);
	~CX2EffectToolEditor(void);

	void SetEventDataValue( const CMinMax<D3DXCOLOR>* ColorValue_ );
	void SetEventDataValue( const CMinMax<D3DXVECTOR3>* Vec3Value_ );
	void SetEventDataValue( const CMinMax<D3DXVECTOR2>* Vec2Value_ );
	void SetEventDataValue( const CMinMax<float>* FloatValue_ );
	void SetEventDataValue( const WCHAR* strValue_ );


	void SetValueByEditControl( OUT CMinMax<D3DXVECTOR3>* cVec3Value_, vector<int> vecEditControlID_ );
	void SetValueByEditControl( OUT CMinMax<float>* cFloatValue_, vector<int> vecEditControlID_ );
	void SetValueByEditControl( OUT CMinMax<int>* cIntValue_, vector<int> vecEditControlID_ );

	void SetEditControlByValue( const CMinMax<int>* cMinMaxValue_, vector<int> vecEditControlID_ );
	void SetEditControlByValue( const CMinMax<float>* cMinMaxValue_, vector<int> vecEditControlID_ );
	void SetEditControlByValue( const CMinMax<D3DXVECTOR3>* cMinMaxValue_, vector<int> vecEditControlID_ );


	// 이벤트 타입에 맞게 다른 값으로 설정
	void InitEventDataValue();
	void InitEventDataValue( CKTDGParticleSystem::EVENT_TYPE eEventType_ ){};

	bool		GetChecked( int iControlID );
	wstring		GetString( int iControlID );
	int			GetIntValue( int iControlID );
	float		GetFloatValue( int iControlID );
	int			GetComboValue( int iControlID );

	void		SetFloatValue( int iControlID_, float fValue_ ) ;
	void		SetIntValue( int iControlID_, int iValue_ ) ;
	void		SetCheckValue( int iControlID_, bool bVal_ ) ;

	HWND GethParticleEditor() { return m_hDialogEditor ;}
	HWND	m_hDialogEditor;
private:
};

