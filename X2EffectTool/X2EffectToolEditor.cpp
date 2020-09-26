#include "dxstdafx.h"
#include "X2EffectToolEditor.h"
#include "resource.h"

CX2EffectToolEditor::CX2EffectToolEditor(void):
m_hDialogEditor(NULL)
{
}

CX2EffectToolEditor::~CX2EffectToolEditor(void)
{
}

void CX2EffectToolEditor::SetEventDataValue( const CMinMax<D3DXCOLOR>* ColorValue_ )
{
	char buf[100];	
	sprintf(buf, "%0.3f", ColorValue_->m_Min.r );
	SetWindowTextA(GetDlgItem( m_hDialogEditor, PA_COMBO_COLOR_MinR), buf);

	sprintf(buf, "%0.3f", ColorValue_->m_Min.g );
	SetWindowTextA(GetDlgItem( m_hDialogEditor, PA_COMBO_COLOR_MinG), buf);

	sprintf(buf, "%0.3f", ColorValue_->m_Min.b );
	SetWindowTextA(GetDlgItem( m_hDialogEditor, PA_COMBO_COLOR_MinB ), buf);

	sprintf(buf, "%0.3f", ColorValue_->m_Min.a );
	SetWindowTextA(GetDlgItem( m_hDialogEditor, PA_COMBO_COLOR_MinA), buf);

	sprintf(buf, "%0.3f", ColorValue_->m_Max.r );
	SetWindowTextA(GetDlgItem( m_hDialogEditor, PA_COMBO_COLOR_MaxR ), buf);

	sprintf(buf, "%0.3f", ColorValue_->m_Max.g );
	SetWindowTextA(GetDlgItem( m_hDialogEditor, PA_COMBO_COLOR_MaxG), buf);

	sprintf(buf, "%0.3f", ColorValue_->m_Max.b );
	SetWindowTextA(GetDlgItem( m_hDialogEditor, PA_COMBO_COLOR_MaxB ), buf);

	sprintf(buf, "%0.3f", ColorValue_->m_Max.a );
	SetWindowTextA(GetDlgItem( m_hDialogEditor, PA_COMBO_COLOR_MaxA), buf);
}
void CX2EffectToolEditor::SetEventDataValue( const CMinMax<D3DXVECTOR3>* Vec3Value_ )
{
	char buf[100];	
	sprintf(buf, "%0.3f", Vec3Value_->m_Min.x );
	SetWindowTextA(GetDlgItem( m_hDialogEditor, PA_COMBO_VECTOR3_MinX), buf);

	sprintf(buf, "%0.3f", Vec3Value_->m_Min.y );
	SetWindowTextA(GetDlgItem( m_hDialogEditor, PA_COMBO_VECTOR3_MinY), buf);

	sprintf(buf, "%0.3f", Vec3Value_->m_Min.z );
	SetWindowTextA(GetDlgItem( m_hDialogEditor, PA_COMBO_VECTOR3_MinZ ), buf);

	sprintf(buf, "%0.3f", Vec3Value_->m_Max.x );
	SetWindowTextA(GetDlgItem( m_hDialogEditor, PA_COMBO_VECTOR3_MaxX), buf);

	sprintf(buf, "%0.3f", Vec3Value_->m_Max.y );
	SetWindowTextA(GetDlgItem( m_hDialogEditor, PA_COMBO_VECTOR3_MaxY ), buf);

	sprintf(buf, "%0.3f", Vec3Value_->m_Max.z );
	SetWindowTextA(GetDlgItem( m_hDialogEditor, PA_COMBO_VECTOR3_MaxZ), buf);
}
void CX2EffectToolEditor::SetEventDataValue( const CMinMax<D3DXVECTOR2>* Vec2Value_ )
{
	char buf[100];	
	sprintf(buf, "%0.3f", Vec2Value_->m_Min.x );
	SetWindowTextA(GetDlgItem( m_hDialogEditor, PA_COMBO_VECTOR2_MinX ), buf);

	sprintf(buf, "%0.3f", Vec2Value_->m_Min.y );
	SetWindowTextA(GetDlgItem( m_hDialogEditor, PA_COMBO_VECTOR2_MinY), buf);

	sprintf(buf, "%0.3f", Vec2Value_->m_Max.x );
	SetWindowTextA(GetDlgItem( m_hDialogEditor, PA_COMBO_VECTOR2_MaxX ), buf);

	sprintf(buf, "%0.3f", Vec2Value_->m_Max.y );
	SetWindowTextA(GetDlgItem( m_hDialogEditor, PA_COMBO_VECTOR2_MaxY), buf);
}
void CX2EffectToolEditor::SetEventDataValue( const CMinMax<float>* FloatValue_ )
{
	char buf[100];	
	sprintf(buf, "%0.3f", FloatValue_->m_Min );
	SetWindowTextA(GetDlgItem( m_hDialogEditor, PA_COMBO_FLOAT_Min), buf);

	sprintf(buf, "%0.3f", FloatValue_->m_Max );
	SetWindowTextA(GetDlgItem( m_hDialogEditor, PA_COMBO_FLOAT_Max), buf);
}
void CX2EffectToolEditor::SetEventDataValue( const WCHAR* strValue_ )
{
	SetWindowText(GetDlgItem( m_hDialogEditor, PA_EDIT_STRING), strValue_ );
}

void CX2EffectToolEditor::SetValueByEditControl( OUT CMinMax<D3DXVECTOR3>* cVec3Value_, vector<int> vecEditControlID_ )
{
	// min x, y, z
	// max x, y, z 순서로 입력받아야 함.

	if( vecEditControlID_.size() != 6 )
		return;

	cVec3Value_->m_Min.x = GetFloatValue( vecEditControlID_[0] );
	cVec3Value_->m_Min.y = GetFloatValue( vecEditControlID_[1] );
	cVec3Value_->m_Min.z = GetFloatValue( vecEditControlID_[2] );

	cVec3Value_->m_Max.x = max( cVec3Value_->m_Min.x, GetFloatValue( vecEditControlID_[3] ) );
	cVec3Value_->m_Max.y = max( cVec3Value_->m_Min.y, GetFloatValue( vecEditControlID_[4] ) );
	cVec3Value_->m_Max.z = max( cVec3Value_->m_Min.z, GetFloatValue( vecEditControlID_[5] ) );
}
void CX2EffectToolEditor::SetValueByEditControl( OUT CMinMax<float>* cFloatValue_, vector<int> vecEditControlID_ )
{
	// min, max 순서로 입력받아야 함.
	if( vecEditControlID_.size() != 2 )
		return;

	cFloatValue_->m_Min = GetFloatValue( vecEditControlID_[0] );
	cFloatValue_->m_Max = max(cFloatValue_->m_Min, GetFloatValue( vecEditControlID_[1] ) );
}
void CX2EffectToolEditor::SetValueByEditControl( OUT CMinMax<int>* cIntValue_, vector<int> vecEditControlID_ )
{
	// min, max 순서로 입력받아야 함.
	if( vecEditControlID_.size() != 2 )
		return;

	cIntValue_->m_Min = GetIntValue( vecEditControlID_[0] );
	cIntValue_->m_Max = max( cIntValue_->m_Min, GetIntValue( vecEditControlID_[1] ) );
}
void CX2EffectToolEditor::SetEditControlByValue( const CMinMax<int>* cMinMaxValue_, vector<int> vecEditControlID_ )
{
	// vecEditControlID_ 
	// 0 : Min
	// 1 : Max
	if( 2 != vecEditControlID_.size() )
		return;

	char buf[100];

	sprintf(buf, "%d", cMinMaxValue_->m_Min );
	SetWindowTextA(GetDlgItem( m_hDialogEditor, vecEditControlID_[0]), buf);

	sprintf(buf, "%d", cMinMaxValue_->m_Max );
	SetWindowTextA(GetDlgItem( m_hDialogEditor, vecEditControlID_[1]), buf);
}
void CX2EffectToolEditor::SetEditControlByValue( const CMinMax<float>* cMinMaxValue_, vector<int> vecEditControlID_ )
{
	// vecEditControlID_ 
	// 0 : Min
	// 1 : Max
	if( 2 != vecEditControlID_.size() )
		return;

	char buf[100];

	sprintf(buf, "%0.3f", cMinMaxValue_->m_Min );
	SetWindowTextA(GetDlgItem( m_hDialogEditor, vecEditControlID_[0]), buf);

	sprintf(buf, "%0.3f", cMinMaxValue_->m_Max );
	SetWindowTextA(GetDlgItem( m_hDialogEditor, vecEditControlID_[1]), buf);
}
void CX2EffectToolEditor::SetEditControlByValue( const CMinMax<D3DXVECTOR3>* cMinMaxValue_, vector<int> vecEditControlID_ )
{
	// vecEditControlID_ 
	// 0~2 : Min.x , y, z
	// 3~5 : Max.x , y, z
	if( 6 != vecEditControlID_.size() )
		return;

	char buf[100];

	sprintf(buf, "%0.3f", cMinMaxValue_->m_Min.x );
	SetWindowTextA(GetDlgItem( m_hDialogEditor, vecEditControlID_[0]), buf);

	sprintf(buf, "%0.3f", cMinMaxValue_->m_Min.y );
	SetWindowTextA(GetDlgItem( m_hDialogEditor, vecEditControlID_[1]), buf);

	sprintf(buf, "%0.3f", cMinMaxValue_->m_Min.z );
	SetWindowTextA(GetDlgItem( m_hDialogEditor, vecEditControlID_[2]), buf);

	sprintf(buf, "%0.3f", cMinMaxValue_->m_Max.x );
	SetWindowTextA(GetDlgItem( m_hDialogEditor, vecEditControlID_[3]), buf);

	sprintf(buf, "%0.3f", cMinMaxValue_->m_Max.y );
	SetWindowTextA(GetDlgItem( m_hDialogEditor, vecEditControlID_[4]), buf);

	sprintf(buf, "%0.3f", cMinMaxValue_->m_Max.z );
	SetWindowTextA(GetDlgItem( m_hDialogEditor, vecEditControlID_[5]), buf);
}

bool CX2EffectToolEditor::GetChecked( int iControlID )
{
	return (0 == SendDlgItemMessage(m_hDialogEditor, iControlID, BM_GETCHECK, 0, 0)) ? false : true;
}
wstring CX2EffectToolEditor::GetString( int iControlID )
{
	char buf[100] = {0, };
	wstring wsValue;
	GetWindowTextA(GetDlgItem(m_hDialogEditor, iControlID ), buf, 100);
	ConvertCharToWCHAR( wsValue, buf);
	return wsValue;
}
int CX2EffectToolEditor::GetIntValue( int iControlID )
{
	char buf[100] = {0, };
	GetWindowTextA(GetDlgItem(m_hDialogEditor, iControlID), buf, 100);
	return atoi(buf);
}
float CX2EffectToolEditor::GetFloatValue( int iControlID )
{
	char buf[100] = {0, };
	GetWindowTextA(GetDlgItem(m_hDialogEditor, iControlID), buf, 100);
	return static_cast<float>(atof(buf));
}
int CX2EffectToolEditor::GetComboValue( int iControlID )
{
	// 선택되어진 인덱스에 해당하는 값의 Data값을 반환함.
	int index = SendDlgItemMessage(m_hDialogEditor, iControlID, CB_GETCURSEL, 0, 0);
	if( -1 == index )
		return -1;

	return SendDlgItemMessage(m_hDialogEditor, iControlID, CB_GETITEMDATA, index, 0);
}
void CX2EffectToolEditor::SetFloatValue( int iControlID_, float fValue_ )
{
	char buf[100];
	sprintf(buf, "%0.3f", fValue_ );
	SetWindowTextA(GetDlgItem( m_hDialogEditor, iControlID_), buf);
}
void CX2EffectToolEditor::SetIntValue( int iControlID_, int iValue_ )
{
	char buf[100];
	sprintf(buf, "%d", iValue_ );
	SetWindowTextA(GetDlgItem( m_hDialogEditor, iControlID_), buf);
}
void CX2EffectToolEditor::SetCheckValue( int iControlID_, bool bVal_ )
{
	SendDlgItemMessage(m_hDialogEditor, iControlID_, BM_SETCHECK, (WPARAM)bVal_, 0);	
}

void CX2EffectToolEditor::InitEventDataValue()
{
	// D3DXCOLOR
	SetWindowTextA(GetDlgItem( m_hDialogEditor, PA_COMBO_COLOR_MinR), "");
	SetWindowTextA(GetDlgItem( m_hDialogEditor, PA_COMBO_COLOR_MinG), "");
	SetWindowTextA(GetDlgItem( m_hDialogEditor, PA_COMBO_COLOR_MinB ), "");
	SetWindowTextA(GetDlgItem( m_hDialogEditor, PA_COMBO_COLOR_MinA), "");
	SetWindowTextA(GetDlgItem( m_hDialogEditor, PA_COMBO_COLOR_MaxR ), "");
	SetWindowTextA(GetDlgItem( m_hDialogEditor, PA_COMBO_COLOR_MaxG), "");
	SetWindowTextA(GetDlgItem( m_hDialogEditor, PA_COMBO_COLOR_MaxB ), "");
	SetWindowTextA(GetDlgItem( m_hDialogEditor, PA_COMBO_COLOR_MaxA), "");

	// String
	SetWindowTextA(GetDlgItem( m_hDialogEditor, PA_EDIT_STRING), "");
	SetWindowTextA(GetDlgItem( m_hDialogEditor, PA_COMBO_EVENT_RENDER_TYPE), "");
	SetWindowTextA(GetDlgItem( m_hDialogEditor, PA_COMBO_PlayType), "");
	
	// bool
	SendDlgItemMessage(m_hDialogEditor, PA_IDC_EVENT_USE_SLASH_TRACE, BM_SETCHECK, (WPARAM)false, 0);

	// D3DXVECTOR3
	SetWindowTextA(GetDlgItem( m_hDialogEditor, PA_COMBO_VECTOR3_MinX), "");
	SetWindowTextA(GetDlgItem( m_hDialogEditor, PA_COMBO_VECTOR3_MinY), "");
	SetWindowTextA(GetDlgItem( m_hDialogEditor, PA_COMBO_VECTOR3_MinZ), "");
	SetWindowTextA(GetDlgItem( m_hDialogEditor, PA_COMBO_VECTOR3_MaxX), "");
	SetWindowTextA(GetDlgItem( m_hDialogEditor, PA_COMBO_VECTOR3_MaxY), "");
	SetWindowTextA(GetDlgItem( m_hDialogEditor, PA_COMBO_VECTOR3_MaxZ), "");

	// D3DXVECTOR2
	SetWindowTextA(GetDlgItem( m_hDialogEditor, PA_COMBO_VECTOR2_MinX), "");
	SetWindowTextA(GetDlgItem( m_hDialogEditor, PA_COMBO_VECTOR2_MinY), "");
	SetWindowTextA(GetDlgItem( m_hDialogEditor, PA_COMBO_VECTOR2_MaxX), "");
	SetWindowTextA(GetDlgItem( m_hDialogEditor, PA_COMBO_VECTOR2_MaxY), "");

	// FLOAT
	SetWindowTextA(GetDlgItem( m_hDialogEditor, PA_COMBO_FLOAT_Min), "");
	SetWindowTextA(GetDlgItem( m_hDialogEditor, PA_COMBO_FLOAT_Max), "");
}