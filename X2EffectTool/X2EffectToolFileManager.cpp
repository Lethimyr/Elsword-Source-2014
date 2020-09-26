#include "dxstdafx.h"
#include "X2EffectToolFileManager.h"


CX2EffectToolFileManager::CX2EffectToolFileManager(CX2EffectToolMain* pEffectTool_)
:m_iEffectSetCount( 0 )
, m_pEffectToolMain(pEffectTool_)
{
#ifndef PRE_UPDATE_SCRIPT
	if( NULL != m_pEffectToolMain )
	{
		// 이펙트셋 열기
		m_pEffectToolMain->On_IDC_BUTTON_LOAD_EFFECT_SET_WHOLE();

		// 메이저 파티클, 메쉬플레이어 열기.

		m_pEffectToolMain->ON_IDC_BUTTON_LOAD_BASE_PARTICLE_FILE();
		m_pEffectToolMain->ON_IDC_BUTTON_LOAD_MESH_FILE( L"GameMajorXMeshPlayer.txt" );
	}
#endif // PRE_UPDATE_SCRIPT
}


CX2EffectToolFileManager::~CX2EffectToolFileManager(void)
{
}

void CX2EffectToolFileManager::AddDamageEffect( const WCHAR* pwszFileFullPath_, const WCHAR* pwszDamageEffect_ )
{
	locale::global( locale("kor"));

// 	// file path와 name을 분리
// 	WCHAR drive[_MAX_DRIVE];
// 	WCHAR dir[_MAX_DIR];
// 	WCHAR fname[_MAX_FNAME];
// 	WCHAR ext[_MAX_EXT];
// 	_wsplitpath( pwszFileFullPath_, drive, dir, fname, ext);
// 
// 	string strFileName;
// 	ConvertWCHARToChar( strFileName, fname );
// 	//////////////임시
// 	strFileName += ".lua" ;


	std::ofstream ofs;
	ofs.open( pwszFileFullPath_ , std::ios_base::out | std::ios_base::app);

	ofs << endl;

	string strTemp = "";
	ConvertWCHARToChar( strTemp, pwszDamageEffect_ );
	ofs << strTemp << " =\n";

	ofs << "{" << endl;

	ofs << "\tEFFECT_TYPE\t\t\t= DAMAGE_EFFECT_EFFECT_TYPE[\"DEET_BASIC\"]," <<endl;
	ofs << "\tMAIN_EFFECT_NAME\t= \"";
	ofs << strTemp << "\"," << endl;
	ofs << "\tDAMAGE_TIME\t\t\t = 999," << endl;;


	char charBuf[511] = {0,};
	WideCharToMultiByte( CP_UTF8, 0, L"--이펙트툴에 의해 작성된 DamageEffect입니다.\n--추가 작성이 필요합니다.", -1, charBuf, 511, NULL, NULL );
	ofs<< charBuf << endl << endl;

	ofs << "}" << endl;

	ConverFilesAnsiToUTF8ByPullPath( pwszFileFullPath_ );
}
void CX2EffectToolFileManager::SaveEffectSetWhole( const WCHAR* pwszFileFullPath_, const std::vector< CX2EffectSet::EffectSetData* >* vecEffectTemplet_ )
{
	m_setAddDamageEffectList.clear();
	if( NULL != m_pEffectToolMain)
		m_pEffectToolMain->ResetDamageEffect();

	locale::global( locale("kor"));

	std::ofstream ofs;
	OpenFileByFieldPullPath( pwszFileFullPath_, ofs );

	ofs << "-- lua header. UTF-8 ";
	char charBuf[511] = {0,};
	WideCharToMultiByte( CP_UTF8, 0, L"인코딩 인식을 위해 이 줄은 지우지 마세요.", -1, charBuf, 511, NULL, NULL );
	ofs<< charBuf << endl << endl;

	BOOST_FOREACH( CX2EffectSet::EffectSetData* pData, *vecEffectTemplet_ )
	{
		ofs << "g_pEffectSet:AddEffectSetTemplet_LUA" << endl;
		ofs << "{" << "--" ; 
		char charBuf[511] = {0,};
		WideCharToMultiByte( CP_UTF8, 0, m_pEffectToolMain->GetEffectSetDesc( pData->m_wstrEffectSetName.c_str() ).c_str(), -1, charBuf, 511, NULL, NULL );

		ofs << charBuf << endl;


		string strTemp = "";
		ConvertWCHARToChar( strTemp, pData->m_wstrEffectSetName.c_str() );
		ofs << "\tNAME = \"" << strTemp << "\"," << endl;

		ofs << GetStringEventTimerType( pData->m_eEventTimerType );
		
		if( -1 != pData->m_iDeleteShakeCount )
			ofs << "\tDELETE_SHAKE_COUNT = " << pData->m_iDeleteShakeCount << "," << endl;
		
		if( true != pData->m_bAffectedByUnitScale )
			ofs << "\tAFFECTED_BY_UNIT_SCALE = FALSE," << endl;	

		m_iEffectSetCount = 0;
		SaveEffectSet_EffectData(ofs, pData->m_vecpEffectData );

		m_iEffectSetCount = 0;
		SaveEffectSet_CameraShake( ofs, pData->m_vecpCameraShakeData );
		ofs << "}" << endl;
	}

	ofs.close();
	ConverFilesAnsiToUTF8ByPullPath( pwszFileFullPath_ );
}
void CX2EffectToolFileManager::SaveEffectSet_Desc( const WCHAR* pwszFileFullPath_, const std::vector< CX2EffectSet::EffectSetData* >* vecEffectTemplet_ )
{
	m_setAddDamageEffectList.clear();
	if( NULL != m_pEffectToolMain)
		m_pEffectToolMain->ResetDamageEffect();

	locale::global( locale("kor"));

	std::ofstream ofs;
	OpenFileByFieldPullPath( pwszFileFullPath_, ofs );

	ofs << "-- lua header. UTF-8 ";
	char charBuf[511] = {0,};
	WideCharToMultiByte( CP_UTF8, 0, L"인코딩 인식을 위해 이 줄은 지우지 마세요.", -1, charBuf, 511, NULL, NULL );
	ofs<< charBuf << endl << endl;

	// 이펙트셋 버전 관리
	if( NULL != g_pX2Game && g_pX2Game->GetEffectSet() )
	{
		ofs << "g_pEffectSet:SetEffectSetVersion" << endl;
		ofs << "{" << endl;
		string strTemp = "";
		ConvertWCHARToChar( strTemp, g_pX2Game->GetEffectSet()->GetEffectToolVersion().c_str() );
		ofs << "\tVERSION = \"" << strTemp.c_str() << "\"," << endl;
		ofs << "}" << endl;
	}

	// 이펙트셋 
	BOOST_FOREACH( CX2EffectSet::EffectSetData* pData, *vecEffectTemplet_ )
	{
		wstring wstrEffectSetDesc = m_pEffectToolMain->GetEffectSetDesc( pData->m_wstrEffectSetName.c_str() );
		if( L"" == wstrEffectSetDesc )
		{
			continue;
		}

		ofs << "g_pEffectSet:AddEffectSet_Description" << endl;
		ofs << "{" << endl;

		string strTemp = "";
		ConvertWCHARToChar( strTemp, pData->m_wstrEffectSetName.c_str() );

		ofs << "\tNAME = \"" << strTemp.c_str() << "\"," << endl;

		char charBuf[511] = {0,};
		WideCharToMultiByte( CP_UTF8, 0, wstrEffectSetDesc.c_str(), -1, charBuf, 511, NULL, NULL );
		ofs << "\tDESC = \"" << charBuf << "\"," << endl;

		ofs << "}" << endl;
	}

	ofs.close();
	ConverFilesAnsiToUTF8ByPullPath( pwszFileFullPath_ );
}
void CX2EffectToolFileManager::SaveDamageEffect( const WCHAR* wcDamageEffectName_ )
{
	if( NULL == m_pEffectToolMain )
		return;

	if( false == m_pEffectToolMain->IsValidDamageEffectByName( wcDamageEffectName_ ) )
	{
		// 이번 회차에 추가한 데미지 이펙트인지 체크.
		std::set<wstring>::iterator it = m_setAddDamageEffectList.find( wcDamageEffectName_ );
		if( it != m_setAddDamageEffectList.end() )
			return;

		wstring wsFilePathAndName;
		m_pEffectToolMain->GetMajorResourcePath( wsFilePathAndName );
		wsFilePathAndName += L"DamageEffect.lua";

		AddDamageEffect( wsFilePathAndName.c_str(), wcDamageEffectName_ );

		m_setAddDamageEffectList.insert( wcDamageEffectName_ );
	}
}

void CX2EffectToolFileManager::SaveEffectSet_CameraShake( std::ofstream& ofs_, std::vector<CX2EffectSet::CameraShakeData*> pCameraData_ )
{
	BOOST_FOREACH(CX2EffectSet::CameraShakeData* pData, pCameraData_ )
	{
		ofs_ << "\tCAMERA_SHAKE" << m_iEffectSetCount++ << " =" << endl;
		ofs_ << "\t{" <<endl;

		ofs_ << GetStringShakeType( "SHAKE_TYPE", pData->m_eShakeType );

		ofs_ << "\t\t" << "START_TIME = " << pData->m_fStartAnimTime << "," << endl;

		if( -1 == pData->m_iLimitDistanceSq )
		{
			ofs_ << "\t\t" << "LIMIT_DISTANCE = IGNORE_DISTANCE," << endl;
		}
		else if( LIMIT_DISTANCE_SQ != pData->m_iLimitDistanceSq )
			ofs_ << "\t\t" << "LIMIT_DISTANCE = " << sqrt( static_cast<float>(pData->m_iLimitDistanceSq)) << "," << endl;

		if( 0.f != pData->m_fOneDirSpeed )
			ofs_ << "\t\t" << "ONE_DIR_SPEED"	<< " = " << pData->m_fOneDirSpeed << "," << endl;
		
		if( 0.f != pData->m_fOneDirAccel )
			ofs_ << "\t\t" << "ONE_DIR_ACCEL"	<< " = " << pData->m_fOneDirAccel << "," << endl;

		if( 0.f != pData->m_fTwoDirGap )
			ofs_ << "\t\t" << "TWO_DIR_GAP"		<< " = " << pData->m_fTwoDirGap << "," << endl;

		if( 0.f != pData->m_fTwoDirTime )
			ofs_ << "\t\t" << "TWO_DIR_TIME"	<< " = " << pData->m_fTwoDirTime << "," << endl;

		if( 0.05f != pData->m_fTwoDirTimeGap )
			ofs_ << "\t\t" << "TWO_DIR_TIME_GAP"<< " = " << pData->m_fTwoDirTimeGap << "," << endl;

		if( false != pData->m_bFixedLookVec )
			ofs_ << "\t\t" << "FIXED_LOOK_VEC"		<< " = " << "TRUE" << "," << endl;

		if( 0 != pData->m_iEyeShakeCount )
			ofs_ << "\t\t" << "EYE_SHAKE_COUNT"		<< " = " << pData->m_iEyeShakeCount << "," << endl;

		if( 0.f != pData->m_EyeMoveSpeed.m_Min )
			ofs_ << "\t\t" << "EYE_MOVE_SPEED_MIN"	<< " = " << pData->m_EyeMoveSpeed.m_Min << "," << endl;
		
		if( 0.f != pData->m_EyeMoveSpeed.m_Max )
			ofs_ << "\t\t" << "EYE_MOVE_SPEED_MAX"	<< " = " << pData->m_EyeMoveSpeed.m_Max << "," << endl;

		if( 0.f != pData->m_EyeMoveRange.m_Min )
			ofs_ << "\t\t" << "EYE_MOVE_RANGE_MIN"	<< " = " << pData->m_EyeMoveRange.m_Min << "," << endl;

		if( 0.f != pData->m_EyeMoveRange.m_Max )
			ofs_ << "\t\t" << "EYE_MOVE_RANGE_MAX"	<< " = " << pData->m_EyeMoveRange.m_Max << "," << endl;

		if( 0 != pData->m_iLookAtShakeCount )
			ofs_ << "\t\t" << "LOOK_AT_SHAKE_COUNT"		<< " = " << pData->m_iLookAtShakeCount << "," << endl;

		if( 0.f != pData->m_LookAtMoveSpeed.m_Min )
			ofs_ << "\t\t" << "LOOK_AT_MOVE_SPEED_MIN"	<< " = " << pData->m_LookAtMoveSpeed.m_Min << "," << endl;

		if( 0.f != pData->m_LookAtMoveSpeed.m_Max )
			ofs_ << "\t\t" << "LOOK_AT_MOVE_SPEED_MAX"	<< " = " << pData->m_LookAtMoveSpeed.m_Max << "," << endl;

		if( 0.f != pData->m_LookAtMoveRange.m_Min )
			ofs_ << "\t\t" << "LOOK_AT_MOVE_RANGE_MIN"	<< " = " << pData->m_LookAtMoveRange.m_Min << "," << endl;

		if( 0.f != pData->m_LookAtMoveRange.m_Max )
			ofs_ << "\t\t" << "LOOK_AT_MOVE_RANGE_MAX"	<< " = " << pData->m_LookAtMoveRange.m_Max << "," << endl;

		ofs_ << "\t}," <<endl;
	}
}
void CX2EffectToolFileManager::SaveEffectSet_EffectData( std::ofstream& ofs_, std::vector<CX2EffectSet::EffectData*> pEffectData_ )
{
	BOOST_FOREACH(CX2EffectSet::EffectData* pData, pEffectData_ )
	{
		ofs_ << "\tEFFECT" << m_iEffectSetCount++ << " =" << endl;
		ofs_ << "\t{" <<endl;

		ofs_ << GetStringEffectType( "EFFECT_TYPE", pData->m_eEffectType );
		if( CX2EffectSet::ET_DAMAGE_EFFECT == pData->m_eEffectType )
			SaveDamageEffect( pData->m_EffectName.c_str() );

		string strTemp = "";
		ConvertWCHARToChar( strTemp, pData->m_EffectName );
		ofs_ << "\t\t" << "NAME = \"" << strTemp << "\"," << endl;

		if( 0 != pData->m_EffectNameHyper.length() )
		{
			ConvertWCHARToChar( strTemp, pData->m_EffectNameHyper );
			ofs_ << "\t\t" << "HYPER_NAME = \"" << strTemp << "\"," << endl;
		}

		if( 0.f != pData->m_fStartAnimTime )
			ofs_ << "\t\t" << "START_TIME = " << pData->m_fStartAnimTime << "," << endl;

		if( 0 != pData->m_PositionWeaponBoneName.length() )
		{
			ConvertWCHARToChar( strTemp, pData->m_PositionWeaponBoneName );
			ofs_ << "\t\t" << "WEAPON_BONE_NAME"<< " = \"" << strTemp << "\"," << endl;

			if( 0 != pData->m_iWeaponIndex )
				ofs_ << "\t\t" << "WEAPON_BONE_WEAPON_INDEX" << " = " << pData->m_iWeaponIndex << "," << endl;

			if( 0 != pData->m_iModelIndex )
				ofs_ << "\t\t" << "WEAPON_BONE_MODEL_INDEX" << " = " << pData->m_iModelIndex << "," << endl;
		}
		else if( 0 != pData->m_PosRidingPetBoneName.length() )
		{
			ConvertWCHARToChar( strTemp, pData->m_PosRidingPetBoneName );
			ofs_ << "\t\t" << "RIDING_PET_BONE_NAME = \"" << strTemp << "\"," << endl;
		}
		else if( 0 != pData->m_PositionBoneName.length() )
		{
			ConvertWCHARToChar( strTemp, pData->m_PositionBoneName );
			ofs_ << "\t\t" << "BONE_NAME = \"" << strTemp << "\"," << endl;
		}

		// 1000단위 기호 표시되는 문제 때문에 예외처리
		if( 0.f != pData->m_vPositionOffset.x )
		{
			if( true == IsValidRange(pData->m_vPositionOffset.x) )
				ofs_ << "\t\t" << "POSITION_OFFSET_X" << " = " << pData->m_vPositionOffset.x << "," << endl;
			else
				ofs_ << "\t\t" << "POSITION_OFFSET_X" << " = " << GetStringIntByFloat( pData->m_vPositionOffset.x ) << "," << endl;
		}

		if( 0.f != pData->m_vPositionOffset.y )
		{
			if( true == IsValidRange(pData->m_vPositionOffset.y) )
				ofs_ << "\t\t" << "POSITION_OFFSET_Y" << " = " << pData->m_vPositionOffset.y << "," << endl;
			else
				ofs_ << "\t\t" << "POSITION_OFFSET_Y" << " = " << GetStringIntByFloat(pData->m_vPositionOffset.y) << "," << endl;
		}

		if( 0.f != pData->m_vPositionOffset.z )
		{
			if( true == IsValidRange(pData->m_vPositionOffset.z) )
				ofs_ << "\t\t" << "POSITION_OFFSET_Z" << " = " << pData->m_vPositionOffset.z << "," << endl;
			else			
				ofs_ << "\t\t" << "POSITION_OFFSET_Z" << " = " << GetStringIntByFloat(pData->m_vPositionOffset.z) << "," << endl;
		}

		if( 0.f != pData->m_vRotateRel.x )
			ofs_ << "\t\t" << "ROTATE_X" << " = " << pData->m_vRotateRel.x << "," << endl;

		if( 0.f != pData->m_vRotateRel.y )
			ofs_ << "\t\t" << "ROTATE_Y" << " = " << pData->m_vRotateRel.y << "," << endl;

		if( 0.f != pData->m_vRotateRel.z )
			ofs_ << "\t\t" << "ROTATE_Z" << " = " << pData->m_vRotateRel.z << "," << endl;

		if( 0.f != pData->m_vScale.x )
			ofs_ << "\t\t" << "SCALE_X" << " = " << pData->m_vScale.x << "," << endl;
		
		if( 0.f != pData->m_vScale.y )
			ofs_ << "\t\t" << "SCALE_Y" << " = " << pData->m_vScale.y << "," << endl;
		
		if( 0.f != pData->m_vScale.x )
			ofs_ << "\t\t" << "SCALE_Z" << " = " << pData->m_vScale.z << "," << endl;

		if( 0.f != pData->m_vScaleOnLeft.x )
			ofs_ << "\t\t" << "SCALE_X_ON_LEFT" << " = " << pData->m_vScaleOnLeft.x << "," << endl;

		if( 0.f != pData->m_vScaleOnLeft.x )
			ofs_ << "\t\t" << "SCALE_Y_ON_LEFT" << " = " << pData->m_vScaleOnLeft.y << "," << endl;

		if( 0.f != pData->m_vScaleOnLeft.x )
			ofs_ << "\t\t" << "SCALE_Z_ON_LEFT" << " = " << pData->m_vScaleOnLeft.z << "," << endl;
		
		if( false != pData->m_bTrace )
			ofs_ << "\t\t" << "TRACE" << " = " << "TRUE" << "," << endl;

		if( false != pData->m_bTraceMore )
			ofs_ << "\t\t" << "TRACE_MORE" << " = " << "TRUE" << "," << endl;

		if( false != pData->m_bTraceMatrix )
			ofs_ << "\t\t" << "TRACE_MATRIX" << " = " << "TRUE" << "," << endl;

		if( false != pData->m_bReverseY )
			ofs_ << "\t\t" << "REVERSE_Y" << " = " << "TRUE" << "," << endl;

		if( false != pData->m_bLandPosition )
			ofs_ << "\t\t" << "USE_LAND_POSITION" << " = " << "TRUE" << "," << endl;

		if( true != pData->m_bApplyUnitRotation )
		{
			ofs_ << "\t\t" << "APPLY_UNIT_ROTATION" << " = " << "FALSE" << "," << endl;
		}
#ifdef SERV_ARA_CHANGE_CLASS_SECOND 
		// 기본 값 false
		if( false != pData->m_bApplyUnitRotationMirror )
			ofs_ << "\t\t" << "APPLY_UNIT_ROTATION_MIRROR" << " = " << "TRUE" << "," << endl;
#endif // SERV_ARA_CHANGE_CLASS_SECOND

		if( true != pData->m_bApplyInstanceScale )
			ofs_ << "\t\t" << "APPLY_INSTANCE_SCALE" << " = " << "FALSE" << "," << endl;

		if( false != pData->m_bStartWhenTraceTargetDead )
			ofs_ << "\t\t" << "START_WHEN_TRACE_TARGET_DEAD" << " = " << "TRUE" << "," << endl;

		if( false != pData->m_bPassiveEffect )
			ofs_ << "\t\t" << "PASSIVE_EFFECT" << " = " << "TRUE" << "," << endl;

		if( false != pData->m_bUseSubAttackListSet )
			ofs_ << "\t\t" << "USE_SUB_ATTACK_LIST_SET" << " = " << "TRUE" << "," << endl;

#ifdef MODIFY_EFFECT_SET_TRACE_BONE // 오현빈
		if( false != pData->m_bBoneTraceTargetMesh )
			ofs_ << "\t\t" << "BONE_TRACE_TARGET_MESH" << " = " << "TRUE" << "," << endl;
#endif // MODIFY_EFFECT_SET_TRACE_BONE

#ifdef MODIFY_CREATE_EFFECT_SET_LIMIT_DISTANCE // 오현빈
		if( 0.f < pData->m_fLimitDistanceSq )
		{
			float fLimtDistance = sqrt(pData->m_fLimitDistanceSq);
			if( true == IsValidRange(fLimtDistance) )
				ofs_ << "\t\t" << "LIMIT_DISTANCE" << " = " << fLimtDistance << "," << endl;
			else			
				ofs_ << "\t\t" << "LIMIT_DISTANCE" << " = " << GetStringIntByFloat( fLimtDistance ) << "," << endl;
		}
#endif // MODIFY_EFFECT_SET_TRACE_BONE

		ofs_ << "\t}," <<endl;
	}
}
inline std::string CX2EffectToolFileManager::GetStringShakeType( const char* pTemp_, CKTDGCamera::SHAKE_TYPE eShakeType_ )
{
	std::string strTemp = "\t\t";

	strTemp += pTemp_;
	strTemp += " = DAMAGE_EFFECT_CRASH_TYPE[\"";

	switch( eShakeType_ )
	{
	case CKTDGCamera::DECT_INVALID:				strTemp += "DECT_INVALID"; break;				
	case CKTDGCamera::DECT_UP:					strTemp += "DECT_UP"; break;
	case CKTDGCamera::DECT_DOWN:				strTemp += "DECT_DOWN"; break;
	case CKTDGCamera::DECT_UP_DOWN:				strTemp += "DECT_UP_DOWN"; break;
	case CKTDGCamera::DECT_UP_DOWN_NO_RESET:	strTemp += "DECT_UP_DOWN_NO_RESET"; break;
	case CKTDGCamera::DECT_LEFT:				strTemp += "DECT_LEFT"; break;
	case CKTDGCamera::DECT_RIGHT:				strTemp += "DECT_RIGHT"; break;
	case CKTDGCamera::DECT_LEFT_RIGHT:			strTemp += "DECT_LEFT_RIGHT"; break;
	case CKTDGCamera::DECT_LEFT_RIGHT_NO_RESET:	strTemp += "DECT_LEFT_RIGHT_NO_RESET"; break;
	case CKTDGCamera::DECT_RANDOM:				strTemp += "DECT_RANDOM"; break;
	}
	strTemp += "\"],\n";

	return strTemp;
}
inline std::string CX2EffectToolFileManager::GetStringEffectType( const char* pTemp_, CX2EffectSet::EFFECT_TYPE eEffectType_ )
{
	std::string strTemp = "\t\t";

	strTemp += pTemp_;
	strTemp += " = EFFECT_TYPE[\"";

	switch( eEffectType_ )
	{
	case CX2EffectSet::ET_INVALID:					strTemp += "ET_INVALID"; break;
	case CX2EffectSet::ET_DAMAGE_EFFECT:			strTemp += "ET_DAMAGE_EFFECT"; break;
	case CX2EffectSet::ET_MESH_PLAYER_UI_MAJOR:		strTemp += "ET_MESH_PLAYER_UI_MAJOR"; break;
	case CX2EffectSet::ET_MESH_PLAYER_UI_MINOR:		strTemp += "ET_MESH_PLAYER_UI_MINOR"; break;
	case CX2EffectSet::ET_MESH_PLAYER_GAME_MAJOR:	strTemp += "ET_MESH_PLAYER_GAME_MAJOR"; break;
	case CX2EffectSet::ET_MESH_PLAYER_GAME_MINOR:	strTemp += "ET_MESH_PLAYER_GAME_MINOR"; break;
	case CX2EffectSet::ET_PARTICLE_UI_MAJOR:		strTemp += "ET_PARTICLE_UI_MAJOR"; break;
	case CX2EffectSet::ET_PARTICLE_UI_MINOR:		strTemp += "ET_PARTICLE_UI_MINOR"; break;
	case CX2EffectSet::ET_PARTICLE_GAME_MAJOR:		strTemp += "ET_PARTICLE_GAME_MAJOR"; break;
	case CX2EffectSet::ET_PARTICLE_GAME_MINOR:		strTemp += "ET_PARTICLE_GAME_MINOR"; break;
	}
	strTemp += "\"],\n";

	return strTemp;
}

inline std::string CX2EffectToolFileManager::GetStringEventTimerType( CX2EffectSet::EFFECT_SET_TIMER_TYPE eEffectTimerType_ )
{
	std::string strTemp = "\tEVENT_TIMER_TYPE = EFFECT_SET_TIMER_TYPE[\"";

	switch( eEffectTimerType_ )
	{
	case CX2EffectSet::ESTT_ANIM: return ""; break; //EFFECT_SET_TIMER_TYPE[\"ESTT_ANIM\"]";
	case CX2EffectSet::ESTT_CUSTOM: strTemp += "ESTT_CUSTOM";
	}

	return strTemp + "\"],\n";
}
void CX2EffectToolFileManager::SaveEffectSet( const WCHAR* pwszFileFullPath_, 
											vector<CX2EffectToolMain::TimedXMeshPlayer> vecMeshPlayerList_,
											vector<CX2EffectToolMain::TimedParticle> vecMajorParticleList_,
											vector<CX2EffectToolMain::CameraShake> vecCameraShakeList_)
{
	///////////이펙트 셋 저장 메인 함수
	std::ofstream ofs;
	OpenFileByFieldPullPath( pwszFileFullPath_, ofs );
	

	ofs << "g_pEffectSet:AddEffectSetTemplet_LUA" << endl;
	ofs << "{" ;


		
	ofs << endl;

	m_iEffectSetCount = 0;
	SaveEffectSet_MeshPlayer( ofs, vecMeshPlayerList_ );
	SaveEffectSet_MajorParticle( ofs, vecMajorParticleList_ );
	SaveEffectSet_CameraCrash( ofs, vecCameraShakeList_ );

	ofs << "}" << endl;
	ofs.close();

	ConverFilesAnsiToUTF8ByPullPath( pwszFileFullPath_ );
}

void CX2EffectToolFileManager::SaveEffectSet_CameraCrash( std::ofstream& ofs_, vector<CX2EffectToolMain::CameraShake> vecCameraSahkeList_ )
{
	m_iEffectSetCount = 0;
	BOOST_FOREACH( CX2EffectToolMain::CameraShake Camera, vecCameraSahkeList_ )
	{
		ofs_ << "\tCAMERA_SHAKE" << m_iEffectSetCount++ <<endl;
		ofs_ << "\t{" << endl;
		ofs_ << "\t\tSHAKE_TYPE\t= DAMAGE_EFFECT_CRASH_TYPE[\"DECT_UP_DOWN\"]" << "," << endl;

		if( 0.f != Camera.m_fStartTime )
			ofs_ << "\t\tSTART_TIME	= " << Camera.m_fStartTime << "," << endl;

		if( 0.f != Camera.m_fAmplitude )
			ofs_ << "\t\tAMPLITUDE	= " << Camera.m_fAmplitude << "," << endl;

		if( 0.f != Camera.m_fDuration )
			ofs_ << "\t\tDURATION	= " << Camera.m_fDuration << "," << endl;

		ofs_ << "\t\t}, " << endl;
	}
}
void CX2EffectToolFileManager::SaveEffectSet_MajorParticle( std::ofstream& ofs_, vector<CX2EffectToolMain::TimedParticle> vecMajorParticleList_ )
{
	BOOST_FOREACH( CX2EffectToolMain::TimedParticle MajorParticle, vecMajorParticleList_ )
	{
		ofs_ << "\tEFFECT" << m_iEffectSetCount++ <<endl;
		ofs_ << "\t{" << endl;
		ofs_ << "\t\tEFFECT_TYPE\t= EFFECT_TYPE[\"ET_PARTICLE_GAME_MAJOR\"]" << "," << endl;

		string strName = "";
		ConvertWCHARToChar( strName, MajorParticle.m_wstrParticleName.c_str() );
		ofs_ << "\t\tNAME\t\t= \"" << strName << "\"," << endl;

		if( 0.f != MajorParticle.m_fDelayTimeToStart )
			ofs_ << "\t\tSTART_TIME\t= " << MajorParticle.m_fDelayTimeToStart << "," << endl;

		if( 0.f != MajorParticle.m_vRelativePosition.x )
			ofs_ << "\t\tPOSITION_OFFSET_X\t= " << MajorParticle.m_vRelativePosition.x << ", " << endl;

		if( 0.f != MajorParticle.m_vRelativePosition.y )		
			ofs_ << "\t\tPOSITION_OFFSET_Y\t= " << MajorParticle.m_vRelativePosition.y << ", " << endl;

		if( 0.f != MajorParticle.m_vRelativePosition.z )
			ofs_ << "\t\tPOSITION_OFFSET_Z\t= " << MajorParticle.m_vRelativePosition.z << ", " << endl;


		if( true == MajorParticle.m_bTraceUnit )
			ofs_ << "\t\tTRACE\t\t\t= TRUE, " << endl;
		else
			ofs_ << "\t\tTRACE\t\t\t= FALSE, " << endl;

		if( 0 != MajorParticle.m_wstrBoneName.length()  )
		{
			ConvertWCHARToChar( strName, MajorParticle.m_wstrBoneName.c_str() );
			ofs_ << "\t\tBONE_NAME\t= \"" << strName << "\"," << endl;
		}
		ofs_ << "\t}," << endl;
	}
}
void CX2EffectToolFileManager::SaveEffectSet_MeshPlayer( std::ofstream& ofs_, vector<CX2EffectToolMain::TimedXMeshPlayer> vecMeshPlayerList_ )
{
	BOOST_FOREACH( CX2EffectToolMain::TimedXMeshPlayer MeshPlayer, vecMeshPlayerList_ )
	{
		ofs_ << "\tEFFECT" << m_iEffectSetCount++ <<endl;
		ofs_ << "\t{" << endl;
		ofs_ << "\t\tEFFECT_TYPE\t= EFFECT_TYPE[\"ET_MESH_PLAYER_GAME_MAJOR\"]" << "," << endl;
		
		string strName = "";
		ConvertWCHARToChar( strName, MeshPlayer.m_wstrXMeshPlayerName.c_str() );
		ofs_ << "\t\tNAME\t\t= \"" << strName << "\"," << endl;
		
		if( 0.f != MeshPlayer.m_fDelayTimeToStart )
			ofs_ << "\t\tSTART_TIME\t= " << MeshPlayer.m_fDelayTimeToStart << "," << endl;
	
		if( 0.f != MeshPlayer.m_vRelativePosition.x )
			ofs_ << "\t\tPOSITION_OFFSET_X\t= " << MeshPlayer.m_vRelativePosition.x << ", " << endl;
	
		if( 0.f != MeshPlayer.m_vRelativePosition.y )		
			ofs_ << "\t\tPOSITION_OFFSET_Y\t= " << MeshPlayer.m_vRelativePosition.y << ", " << endl;
	
		if( 0.f != MeshPlayer.m_vRelativePosition.z )
			ofs_ << "\t\tPOSITION_OFFSET_Z\t= " << MeshPlayer.m_vRelativePosition.z << ", " << endl;


		if( true == MeshPlayer.m_bTraceUnit )
			ofs_ << "\t\tTRACE\t\t= TRUE, " << endl;
		else
			ofs_ << "\t\tTRACE\t\t= FALSE, " << endl;

		if( 0 != MeshPlayer.m_wstrBoneName.length()  )
		{
			ConvertWCHARToChar( strName, MeshPlayer.m_wstrBoneName.c_str() );
			ofs_ << "\t\tBONE_NAME\t= \"" << strName << "\"," << endl;
		}

		ofs_ << "\t}," << endl;
	}
}

void CX2EffectToolFileManager::SaveMajorMeshPlayer( const WCHAR* pwszFileFullPath_, CKTDGXMeshPlayer* pMeshTemplet_ )
{
	///////////메쉬플레이어 저장 메인 함수
	std::ofstream ofs;
	OpenFileByFieldPullPath( pwszFileFullPath_, ofs );

	ofs << "XMESHPLAYER \"GameMajor\" 1.00" << endl;
	ofs << "{" << endl;


	const vector<CKTDGXMeshPlayer::XMeshTemplet*>& vecTempSeq =
		pMeshTemplet_->GetVecTemplet();

	
	BOOST_FOREACH( CKTDGXMeshPlayer::XMeshTemplet* pParticle, vecTempSeq )
	{
		string strParticleName;
		ConvertWCHARToChar( strParticleName, pParticle->templetName );
		
		ofs << "\tTEMPLET \"" << strParticleName <<"\"" << endl;
		ofs << "\t{" << endl;

		SaveMeshPlayer_BasicData( ofs, pParticle );
		SaveMeshPlayer_EventList( ofs, pParticle );

		ofs << "\t}" << endl;
	}

	ofs << "}" << endl;
	ofs.close();
}

void CX2EffectToolFileManager::SaveMajorParticle( const WCHAR* pwszFileFullPath_, const vector<CKTDGParticleSystem::CParticleEventSequence*> vecParticleTemplet_ )
{
	///////////파티클 저장 메인 함수.
	std::ofstream ofs;
	OpenFileByFieldPullPath( pwszFileFullPath_, ofs );

	ofs << "particlesystem \"GameMajor\" 1.00" << endl;
	ofs << "{" << endl;

	BOOST_FOREACH( CKTDGParticleSystem::CParticleEventSequence* pParticle, vecParticleTemplet_ )
	{
		string strParticleName;
		ConvertWCHARToChar( strParticleName, pParticle->GetName() );

		ofs << "\teventsequence \"" << strParticleName <<"\"" << endl;
		ofs << "\t{" << endl;
		//파티클 본문
		SaveParticle_BasicData( ofs, pParticle );
		SaveParticle_EventList( ofs, pParticle );

		ofs << "\t}" << endl;
	}

	ofs << "}" << endl;
	ofs.close();
}
bool CX2EffectToolFileManager::ConverFilesAnsiToUTF8ByPullPath( const WCHAR* pwszFileFullPath_ )
{
 	// file path와 name을 분리
 	WCHAR drive[_MAX_DRIVE];
 	WCHAR dir[_MAX_DIR];
 	WCHAR fname[_MAX_FNAME];
 	WCHAR ext[_MAX_EXT];
 	_wsplitpath( pwszFileFullPath_, drive, dir, fname, ext);

	string strFilePathAndName;
	ConvertWCHARToChar( strFilePathAndName, pwszFileFullPath_ );

	return ConvertFullPathFileAnsiToUTF8 ( strFilePathAndName, strFilePathAndName );
}

bool CX2EffectToolFileManager::OpenFileByFieldPullPath( const WCHAR* pwszFileFullPath_,OUT std::ofstream& ofs_)
{
	ofs_.open ( pwszFileFullPath_ );
	if( ofs_.is_open() == false )
	{		
		ofs_.close();
		return false;
	}

	return true;
}

void CX2EffectToolFileManager::SaveMeshPlayer_EventList( std::ofstream& ofs_, const CKTDGXMeshPlayer::XMeshTemplet* pMeshTemplet_ )
{	
	ofs_ << endl ;
	SaveMeshPlayer_EventList_Common( ofs_, "Anim ", pMeshTemplet_->animTimeEventList );
	SaveMeshPlayer_EventList_Common( ofs_, "Global ", pMeshTemplet_->globalTimeEventList );
	SaveMeshPlayer_EventList_Common( ofs_, "Die ", pMeshTemplet_->dieEventList );
}
void CX2EffectToolFileManager::SaveMeshPlayer_EventList_Common( std::ofstream& ofs_, const char* strEventType_, vector<CKTDGXMeshPlayer::CXMeshEvent*> vecEventList_)
{
	BOOST_FOREACH( CKTDGXMeshPlayer::CXMeshEvent* pEventList, vecEventList_ )
	{
		ofs_ << "\t\t" << strEventType_;

		switch( pEventList->GetEventType() )
		{
		case CKTDGXMeshPlayer::ET_ANISPEED:
			{
				CKTDGXMeshPlayer::CXMeshEvent_AniSpeed* pEvent = 
					static_cast<CKTDGXMeshPlayer::CXMeshEvent_AniSpeed*>(pEventList);

				ofs_ << GetStringByActualTime(pEvent->IsFade(), &pEvent->GetActualTime() );
				ofs_ << "AniSpeed = " << GetStringByCMinMax( &pEvent->GetAniSpeed() );
			} break;
		case CKTDGXMeshPlayer::ET_ANINAME:
			{
				CKTDGXMeshPlayer::CXMeshEvent_AniName* pEvent = 
					static_cast<CKTDGXMeshPlayer::CXMeshEvent_AniName*>(pEventList);

				ofs_ << GetStringByActualTime(pEvent->IsFade(), &pEvent->GetActualTime() );
				string strTextureName;
				ConvertWCHARToChar( strTextureName, pEvent->GetAniName() );
				ofs_ << "AniName = \"" << strTextureName << "\", ";

				switch( pEvent->GetPlayType() )
				{
				case CKTDGXSkinAnim::XAP_LOOP: ofs_ << "XAP_LOOP"; break;
				case CKTDGXSkinAnim::XAP_ONE: ofs_ << "XAP_ONE"; break;
				case CKTDGXSkinAnim::XAP_ONE_WAIT: ofs_ << "XAP_ONE_WAIT"; break;
				}
			} break;
		case CKTDGXMeshPlayer::ET_COLOR:
			{
				CKTDGXMeshPlayer::CXMeshEvent_Color* pEvent = 
					static_cast<CKTDGXMeshPlayer::CXMeshEvent_Color*>(pEventList);

				ofs_ << GetStringByActualTime(pEvent->IsFade(), &pEvent->GetActualTime() );
				ofs_ << "Color = " << GetStringByCMinMax( &pEvent->GetColor() );
			} break;
		case CKTDGXMeshPlayer::ET_OUTLINECOLOR:
			{
				CKTDGXMeshPlayer::CXMeshEvent_OutLineColor* pEvent = 
					static_cast<CKTDGXMeshPlayer::CXMeshEvent_OutLineColor*>(pEventList);

				ofs_ << GetStringByActualTime(pEvent->IsFade(), &pEvent->GetActualTime() );
				ofs_ << "OutLineColor = " << GetStringByCMinMax( &pEvent->GetOutLineColor() );
			} break;
		case CKTDGXMeshPlayer::ET_SIZE:
			{
				CKTDGXMeshPlayer::CXMeshEvent_Size* pEvent = 
					static_cast<CKTDGXMeshPlayer::CXMeshEvent_Size*>(pEventList);

				ofs_ << GetStringByActualTime(pEvent->IsFade(), &pEvent->GetActualTime() );
				ofs_ << "Size = " << GetStringByCMinMax( &pEvent->GetSize() );
			} break;
		case CKTDGXMeshPlayer::ET_VELOCITY:
			{
				CKTDGXMeshPlayer::CXMeshEvent_Velocity* pEvent = 
					static_cast<CKTDGXMeshPlayer::CXMeshEvent_Velocity*>(pEventList);

				ofs_ << GetStringByActualTime(pEvent->IsFade(), &pEvent->GetActualTime() );
				ofs_ << "Velocity = " << GetStringByCMinMax( &pEvent->GetVelocity());
			} break;
		case CKTDGXMeshPlayer::ET_TEX0UV:
			{
				CKTDGXMeshPlayer::CXMeshEvent_Tex0UV* pEvent = 
					static_cast<CKTDGXMeshPlayer::CXMeshEvent_Tex0UV*>(pEventList);

				ofs_ << GetStringByActualTime(pEvent->IsFade(), &pEvent->GetActualTime() );
				ofs_ << "TEX0UV = " << GetStringByCMinMax( &pEvent->GetTex0UV());
			} break;
		case CKTDGXMeshPlayer::ET_TEX1UV:
			{
				CKTDGXMeshPlayer::CXMeshEvent_Tex1UV* pEvent = 
					static_cast<CKTDGXMeshPlayer::CXMeshEvent_Tex1UV*>(pEventList);

				ofs_ << GetStringByActualTime(pEvent->IsFade(), &pEvent->GetActualTime() );
				ofs_ << "TEX1UV = " << GetStringByCMinMax( &pEvent->GetTex1UV());
			} break;
		case CKTDGXMeshPlayer::ET_TEX2UV:
			{
				CKTDGXMeshPlayer::CXMeshEvent_Tex2UV* pEvent = 
					static_cast<CKTDGXMeshPlayer::CXMeshEvent_Tex2UV*>(pEventList);

				ofs_ << GetStringByActualTime(pEvent->IsFade(), &pEvent->GetActualTime() );
				ofs_ << "TEX2UV = " << GetStringByCMinMax( &pEvent->GetTex2UV());
			} break;
		case CKTDGXMeshPlayer::ET_ROTATE:
			{
				CKTDGXMeshPlayer::CXMeshEvent_Rotate* pEvent = 
					static_cast<CKTDGXMeshPlayer::CXMeshEvent_Rotate*>(pEventList);

				ofs_ << GetStringByActualTime(pEvent->IsFade(), &pEvent->GetActualTime() );
				ofs_ << "Rotate = " << GetStringByCMinMax( &pEvent->GetRotate());
			} break;
		case CKTDGXMeshPlayer::ET_DIRSPEED:
			{
				CKTDGXMeshPlayer::CXMeshEvent_DirSpeed* pEvent = 
					static_cast<CKTDGXMeshPlayer::CXMeshEvent_DirSpeed*>(pEventList);

				ofs_ << GetStringByActualTime(pEvent->IsFade(), &pEvent->GetActualTime() );
				ofs_ << "DirSpeed = " << GetStringByCMinMax( &pEvent->GetDirSpeed());
			} break;
		case CKTDGXMeshPlayer::ET_BLACKHOLETIME:
			{
				CKTDGXMeshPlayer::CXMeshEvent_BlackHoleTime* pEvent = 
					static_cast<CKTDGXMeshPlayer::CXMeshEvent_BlackHoleTime*>(pEventList);

				ofs_ << GetStringByActualTime(pEvent->IsFade(), &pEvent->GetActualTime() );
				ofs_ << "BlackHoleTime = " << GetStringByCMinMax( &pEvent->GetBlackHoleTime());
			} break;
		case CKTDGXMeshPlayer::ET_CRASH:
			{
				CKTDGXMeshPlayer::CXMeshEvent_Crash* pEvent = 
					static_cast<CKTDGXMeshPlayer::CXMeshEvent_Crash*>(pEventList);

				ofs_ << GetStringByActualTime(pEvent->IsFade(), &pEvent->GetActualTime() );
				ofs_ << "Crash = " << GetStringByCMinMax( &pEvent->GetCrash());
			} break;
		case CKTDGXMeshPlayer::ET_RESETCRASH:
			{
				CKTDGXMeshPlayer::CXMeshEvent_ResetCrash* pEvent = 
					static_cast<CKTDGXMeshPlayer::CXMeshEvent_ResetCrash*>(pEventList);

				ofs_ << GetStringByActualTime(pEvent->IsFade(), &pEvent->GetActualTime() );
				ofs_ << "ResetCrash = " << GetStringByCMinMax( &pEvent->GetResetCrash());
			} break;
		case CKTDGXMeshPlayer::ET_LIGHTFLOWIMPACT:
			{
				CKTDGXMeshPlayer::CXMeshEvent_LightFlowImpact* pEvent = 
					static_cast<CKTDGXMeshPlayer::CXMeshEvent_LightFlowImpact*>(pEventList);

				ofs_ << GetStringByActualTime(pEvent->IsFade(), &pEvent->GetActualTime() );
				ofs_ << "LightFlowImpact = " << GetStringByCMinMax( &pEvent->GetLightFlowImpact());
			} break;
		case CKTDGXMeshPlayer::ET_LIGHTFLOWPOINT:
			{
				CKTDGXMeshPlayer::CXMeshEvent_LightFlowPoint* pEvent = 
					static_cast<CKTDGXMeshPlayer::CXMeshEvent_LightFlowPoint*>(pEventList);

				ofs_ << GetStringByActualTime(pEvent->IsFade(), &pEvent->GetActualTime() );
				ofs_ << "LightFlowPoint = " << GetStringByCMinMax( &pEvent->GetLightFlowPoint());
			} break;
		case CKTDGXMeshPlayer::ET_SOUND:
			{
				CKTDGXMeshPlayer::CXMeshEvent_Sound* pEvent = 
					static_cast<CKTDGXMeshPlayer::CXMeshEvent_Sound*>(pEventList);

				ofs_ << GetStringByActualTime(pEvent->IsFade(), &pEvent->GetActualTime() );

				string strTextureName;
				ConvertWCHARToChar( strTextureName, pEvent->GetSoundName() );
				ofs_ << "Sound = \"" << strTextureName << "\"";
			} break;
		case CKTDGXMeshPlayer::ET_SLASHTRACE:
			{
				CKTDGXMeshPlayer::CXMeshEvent_SlashTrace* pEvent = 
					static_cast<CKTDGXMeshPlayer::CXMeshEvent_SlashTrace*>(pEventList);

				ofs_ << GetStringByActualTime(pEvent->IsFade(), &pEvent->GetActualTime() );
				if( true == pEvent->GetSlashTrace() )
					ofs_ << "SlashTrace = true";
				else
					ofs_ << "SlashTrace = false";
			} break;
		case CKTDGXMeshPlayer::ET_RENDERTYPE:
			{
				CKTDGXMeshPlayer::CXMeshEvent_RenderType* pEvent = 
					static_cast<CKTDGXMeshPlayer::CXMeshEvent_RenderType*>(pEventList);

				ofs_ << GetStringByActualTime(pEvent->IsFade(), &pEvent->GetActualTime() );
				ofs_ << "Render_Type = " << GetStringRenderType(pEvent->GetRenderType()) ;
			} break;
		case CKTDGXMeshPlayer::ET_POSITION:
			{
				CKTDGXMeshPlayer::CXMeshEvent_Position* pEvent = 
					static_cast<CKTDGXMeshPlayer::CXMeshEvent_Position*>(pEventList);

				ofs_ << GetStringByActualTime(pEvent->IsFade(), &pEvent->GetActualTime() );
				ofs_ << "Position = " << GetStringByCMinMax( &pEvent->GetPosition() );
			} break;
		} //switch( pEventList->GetEventType() )

		ofs_ << endl;
	} //BOOST_FOREACH
}
void CX2EffectToolFileManager::SaveMeshPlayer_BasicData( std::ofstream& ofs_, const CKTDGXMeshPlayer::XMeshTemplet* pMeshTemplet_ )
{
	if( NULL == pMeshTemplet_ )
		return;

	SaveMeshPlayer_StringProp( ofs_, pMeshTemplet_ );
	SaveMeshPlayer_EnumProp(ofs_, pMeshTemplet_ );
	SaveMeshPlayer_NumericProp( ofs_, pMeshTemplet_ );
	SaveMeshPlayer_VecProp( ofs_, pMeshTemplet_ );
	SaveMeshPlayer_BoolProp( ofs_, pMeshTemplet_ );
}


#pragma region GameMajorXMeshPlayer
void CX2EffectToolFileManager::SaveMeshPlayer_EnumProp( std::ofstream& ofs_, const CKTDGXMeshPlayer::XMeshTemplet* pMeshTemplet_ )
{
	// SeqLifeTypeProp( LIFETYPE, DIETYPE )
	// SeqMeshTypeProp( MESHTYPE )
	// SeqRenderTypeProp (RENDERTYPE )
	// SeqCartoonTexTypeProp( CARTOONTEXTYPE )
	// SeqCullModeProp ( CULLMODE )
	// SeqNumericProp( SLASH_TRACE_CURVE_TYPE )
	// SeqAlphaBlendModeProp( SOURCEBLENDMODE, DESTBLENDMODE )
	// SeqBillboardTypeProp( BILLBOARDTYPE )

	if( NULL == pMeshTemplet_ )
		return;

	ofs_ << GetStringLifeType( "LifeType", pMeshTemplet_->lifeType, 2 );
	
	if( CKTDGXMeshPlayer::LT_PLAY_COUNT != pMeshTemplet_->dieType )
		ofs_ << GetStringLifeType( "DieType", pMeshTemplet_->dieType );

	ofs_ << GetStringMeshType( "MeshType", pMeshTemplet_->meshType, 2 );
	ofs_ << GetStringRenderType( "RenderType", pMeshTemplet_->renderType );
	ofs_ << GetStringCartoonTexType( "CartoonTexType", pMeshTemplet_->cartoonTexType, 1);
	ofs_ << GetStringCullMode( "CullMode", pMeshTemplet_->cullMode);
	ofs_ << GetStringSlash_Trace_Curve_Type( "Slash_Trace_Curve_Type", pMeshTemplet_->slashTraceCurveType );
	ofs_ << GetStringBillBoardType( "BillBoardType", pMeshTemplet_->billBoardType, 1);
	ofs_ << GetStringBlend( "SourceBlendMode", static_cast<int>(pMeshTemplet_->srcBlend) );
	ofs_ << GetStringBlend( "DestBlendMode", static_cast<int>(pMeshTemplet_->destBlend) );
}
void CX2EffectToolFileManager::SaveMeshPlayer_StringProp( std::ofstream& ofs_, const CKTDGXMeshPlayer::XMeshTemplet* pMeshTemplet_ )
{
	if( NULL == pMeshTemplet_ )
		return;
	// SeqStringProp
	// MESHNAME, CHANGETEXXET, MULTITEXXET, ANIPACKAGE, SLASH_TRACE_TEXTURE

	ofs_ << GetStringStringProp( "MeshName", pMeshTemplet_->meshName.c_str(), 2 );
	ofs_ << GetStringStringProp( "AniPackage", pMeshTemplet_->aniXETName.c_str(), 2 );
	ofs_ << GetStringStringProp( "ChangeTexXet", pMeshTemplet_->changeTexXETName.c_str(), 2 );
	ofs_ << GetStringStringProp( "MultiTexXet", pMeshTemplet_->multiTexXETName.c_str(), 2 );
	ofs_ << GetStringStringProp( "SLASH_TRACE_TEXTURE", pMeshTemplet_->slashTraceTextureName.c_str(), 1 );
}

void CX2EffectToolFileManager::SaveMeshPlayer_BoolProp( std::ofstream& ofs_, const CKTDGXMeshPlayer::XMeshTemplet* pMeshTemplet_ )
{
	if( NULL == pMeshTemplet_ )
		return;
	// SeqBoolProp
	// USEDIEEVENT, ZENABLE, ZWRITEENABLE, USELAND, SET_LAND_HEIGHT_ON_START
	// FORCELAYER, USESLASHTRACE, CRASHLAND, APPLYUNITSCALE
	if( false != pMeshTemplet_->bUseDieEvent )
		ofs_ << GetStringBoolProp( "UseDieEvent", pMeshTemplet_->bUseDieEvent, 2 ) << endl;

	if( false != pMeshTemplet_->bZEnable )
		ofs_ << GetStringBoolProp( "ZEnable", pMeshTemplet_->bZEnable, 3 ) << endl;

	if( false != pMeshTemplet_->bZWriteEnable )
		ofs_ << GetStringBoolProp( "ZWriteEnable", pMeshTemplet_->bZWriteEnable, 1 ) << endl;

	if( false != pMeshTemplet_->bForceLayer )
		ofs_ << GetStringBoolProp( "ForceLayer", pMeshTemplet_->bForceLayer, 2 ) << endl;

	if( true != pMeshTemplet_->bApplyUnitScale )
		ofs_ << GetStringBoolProp( "ApplyUnitScale", pMeshTemplet_->bApplyUnitScale, 1 ) << endl;

	if( false != pMeshTemplet_->bUseSlashTrace )
		ofs_ << GetStringBoolProp( "UseSlashTrace", pMeshTemplet_->bUseSlashTrace, 1 ) << endl;

	if( false != pMeshTemplet_->bUseLand )
		ofs_ << GetStringBoolProp( "UseLand", pMeshTemplet_->bUseLand, 3 ) << endl;

	if( false != pMeshTemplet_->bCrashLand )
		ofs_ << GetStringBoolProp( "CrashLand", pMeshTemplet_->bCrashLand, 2 ) << endl;

	if( false != pMeshTemplet_->bSetLandHeightOnStart )
		ofs_ << GetStringBoolProp( "SET_LAND_HEIGHT_ON_START", pMeshTemplet_->bSetLandHeightOnStart, 1 ) << endl;

}
void CX2EffectToolFileManager::SaveMeshPlayer_VecProp( std::ofstream& ofs_, const CKTDGXMeshPlayer::XMeshTemplet* pMeshTemplet_ )
{
	if( NULL == pMeshTemplet_ )
		return;

	// SeqVector3Prop
	// LIGHTPOS, GRAVITY, BLACKHOLEPOS, 
	const D3DXVECTOR3 vecInitVector(0.f,0.f,0.f);

	if( vecInitVector != pMeshTemplet_->lightPos.m_Max )
		ofs_ << GetStringVectorProp( "LightPos", &pMeshTemplet_->lightPos, 3 ) << endl;
	
	if( vecInitVector != pMeshTemplet_->gravity.m_Max )
		ofs_ << GetStringVectorProp( "Gravity", &pMeshTemplet_->gravity, 3 ) << endl;
}
void CX2EffectToolFileManager::SaveMeshPlayer_NumericProp( std::ofstream& ofs_, const CKTDGXMeshPlayer::XMeshTemplet* pMeshTemplet_ )
{
	if( NULL == pMeshTemplet_ )
		return;

	// SeqNumericProp
	// PLAYCOUNT, LIFETIME, DIEPLAYCOUNT, DIELIFETIME, OUTLINEWIDE
	// LIGHTFLOWWIDE, DRAWCOUNT, LANDPOS, LAYER, ELASTICCOEFF,
	// ELASTICCOEFF_X, SLASH_TRACE_TYPE, NOT_CHECK_LAND_TIME

	ofs_ << GetStringNumericProp( "Layer", pMeshTemplet_->layer, 3 ) << endl;

	if( 0 != pMeshTemplet_->playCount.m_Max )
		ofs_ << GetStringNumericProp( "PlayCount", &pMeshTemplet_->playCount, 2 ) << endl;

	if( 0.f != pMeshTemplet_->lifeTime.m_Max )
		ofs_ << GetStringNumericProp( "LifeTime", &pMeshTemplet_->lifeTime, 2 ) << endl;

	if( 0 != pMeshTemplet_->diePlayCount.m_Max )
		ofs_ << GetStringNumericProp( "DiePlayCount", &pMeshTemplet_->diePlayCount, 1 ) << endl;

	if( 0.f != pMeshTemplet_->dieLifeTime.m_Max )
		ofs_ << GetStringNumericProp( "DieLifeTime", &pMeshTemplet_->dieLifeTime, 2 ) << endl;

	if( 0.f != pMeshTemplet_->outLineWide.m_Max )
		ofs_ << GetStringNumericProp( "OutLineWide", &pMeshTemplet_->outLineWide, 2 ) << endl;

	if( 0.f != pMeshTemplet_->lightFlowWide.m_Max )
		ofs_ << GetStringNumericProp( "LightFlowWide", &pMeshTemplet_->lightFlowWide, 1 ) << endl;

	if( 1 != pMeshTemplet_->drawCount.m_Max )
		ofs_ << GetStringNumericProp( "DrawCount", &pMeshTemplet_->drawCount, 2 ) << endl;

	if( 0.5f != pMeshTemplet_->elasticCoeff.m_Max ||
		true == pMeshTemplet_->bUseLand )
		ofs_ << GetStringNumericProp( "ElasticCoeff", &pMeshTemplet_->elasticCoeff, 1 ) << endl;

	if( 0.f != pMeshTemplet_->elasticCoeffX.m_Max ||
		true == pMeshTemplet_->bUseLand )
		ofs_ << GetStringNumericProp( "ElasticCoeff_X", &pMeshTemplet_->elasticCoeffX, 1 ) << endl;

	if( static_cast<int>(CKTDGSlashTrace::STT_DEFAULT) != pMeshTemplet_->iSlashTraceType )
		ofs_ << GetStringNumericProp( "SLASH_TRACE_TYPE", pMeshTemplet_->iSlashTraceType, 1 ) << endl;

	if( 0.f != pMeshTemplet_->fNotCheckLandTime )
		ofs_ << GetStringNumericProp( "NOT_CHECK_LAND_TIME", pMeshTemplet_->fNotCheckLandTime, 1 ) << endl;
	//pMeshTemplet_->landPosition;
}

#pragma endregion GameMajorXMeshPlayer


std::string CX2EffectToolFileManager::GetStringByEventData( CKTDGXMeshPlayer::CXMeshEvent* pEventData_ )
{
	std::string strEventData = "";
	switch( pEventData_->GetEventType() )
	{
	case CKTDGXMeshPlayer::ET_ANISPEED:
		{
			CKTDGXMeshPlayer::CXMeshEvent_AniSpeed* pEvent = 
				static_cast<CKTDGXMeshPlayer::CXMeshEvent_AniSpeed*>(pEventData_);

			strEventData += GetStringByActualTime(pEvent->IsFade(), &pEvent->GetActualTime() );
			strEventData += "     AniSpeed = " ;
			strEventData += GetStringByCMinMax( &pEvent->GetAniSpeed() );
		} break;
	case CKTDGXMeshPlayer::ET_ANINAME:
		{
			CKTDGXMeshPlayer::CXMeshEvent_AniName* pEvent = 
				static_cast<CKTDGXMeshPlayer::CXMeshEvent_AniName*>(pEventData_);

			string strTextureName;
			ConvertWCHARToChar( strTextureName, pEvent->GetAniName() );

			strEventData += GetStringByActualTime(pEvent->IsFade(), &pEvent->GetActualTime() );
			strEventData += "     AniName = \"" ;
			strEventData += strTextureName;
			strEventData += "\", ";

			switch( pEvent->GetPlayType() )
			{
			case CKTDGXSkinAnim::XAP_LOOP: strEventData += "XAP_LOOP"; break;
			case CKTDGXSkinAnim::XAP_ONE: strEventData += "XAP_ONE"; break;
			case CKTDGXSkinAnim::XAP_ONE_WAIT: strEventData += "XAP_ONE_WAIT"; break;
			}
		} break;
	case CKTDGXMeshPlayer::ET_COLOR:
		{
			CKTDGXMeshPlayer::CXMeshEvent_Color* pEvent = 
				static_cast<CKTDGXMeshPlayer::CXMeshEvent_Color*>(pEventData_);

			strEventData += GetStringByActualTime(pEvent->IsFade(), &pEvent->GetActualTime() );
			strEventData += "     Color = " ;
			strEventData += GetStringByCMinMax( &pEvent->GetColor() );
		} break;
	case CKTDGXMeshPlayer::ET_OUTLINECOLOR:
		{
			CKTDGXMeshPlayer::CXMeshEvent_OutLineColor* pEvent = 
				static_cast<CKTDGXMeshPlayer::CXMeshEvent_OutLineColor*>(pEventData_);

			strEventData += GetStringByActualTime(pEvent->IsFade(), &pEvent->GetActualTime() );
			strEventData += "     OutLineColor = " ;
			strEventData += GetStringByCMinMax( &pEvent->GetOutLineColor() );
		} break;
	case CKTDGXMeshPlayer::ET_SIZE:
		{
			CKTDGXMeshPlayer::CXMeshEvent_Size* pEvent = 
				static_cast<CKTDGXMeshPlayer::CXMeshEvent_Size*>(pEventData_);

			strEventData += GetStringByActualTime(pEvent->IsFade(), &pEvent->GetActualTime() );
			strEventData += "     Size = " ;
			strEventData += GetStringByCMinMax( &pEvent->GetSize() );
		} break;
	case CKTDGXMeshPlayer::ET_VELOCITY:
		{
			CKTDGXMeshPlayer::CXMeshEvent_Velocity* pEvent = 
				static_cast<CKTDGXMeshPlayer::CXMeshEvent_Velocity*>(pEventData_);

			strEventData += GetStringByActualTime(pEvent->IsFade(), &pEvent->GetActualTime() );
			strEventData += "     Velocity = " ;
			strEventData += GetStringByCMinMax( &pEvent->GetVelocity() );
		} break;
	case CKTDGXMeshPlayer::ET_TEX0UV:
		{
			CKTDGXMeshPlayer::CXMeshEvent_Tex0UV* pEvent = 
				static_cast<CKTDGXMeshPlayer::CXMeshEvent_Tex0UV*>(pEventData_);

			strEventData += GetStringByActualTime(pEvent->IsFade(), &pEvent->GetActualTime() );
			strEventData += "     TEX0UV = " ;
			strEventData += GetStringByCMinMax( &pEvent->GetTex0UV() );
		} break;
	case CKTDGXMeshPlayer::ET_TEX1UV:
		{
			CKTDGXMeshPlayer::CXMeshEvent_Tex1UV* pEvent = 
				static_cast<CKTDGXMeshPlayer::CXMeshEvent_Tex1UV*>(pEventData_);

			strEventData += GetStringByActualTime(pEvent->IsFade(), &pEvent->GetActualTime() );
			strEventData += "     TEX1UV = " ;
			strEventData += GetStringByCMinMax( &pEvent->GetTex1UV() );
		} break;
	case CKTDGXMeshPlayer::ET_TEX2UV:
		{
			CKTDGXMeshPlayer::CXMeshEvent_Tex2UV* pEvent = 
				static_cast<CKTDGXMeshPlayer::CXMeshEvent_Tex2UV*>(pEventData_);

			strEventData += GetStringByActualTime(pEvent->IsFade(), &pEvent->GetActualTime() );
			strEventData += "     TEX2UV = " ;
			strEventData += GetStringByCMinMax( &pEvent->GetTex2UV() );
		} break;
	case CKTDGXMeshPlayer::ET_ROTATE:
		{
			CKTDGXMeshPlayer::CXMeshEvent_Rotate* pEvent = 
				static_cast<CKTDGXMeshPlayer::CXMeshEvent_Rotate*>(pEventData_);

			strEventData += GetStringByActualTime(pEvent->IsFade(), &pEvent->GetActualTime() );
			strEventData += "     Rotate = " ;
			strEventData += GetStringByCMinMax( &pEvent->GetRotate() );
		} break;
	case CKTDGXMeshPlayer::ET_DIRSPEED:
		{
			CKTDGXMeshPlayer::CXMeshEvent_DirSpeed* pEvent = 
				static_cast<CKTDGXMeshPlayer::CXMeshEvent_DirSpeed*>(pEventData_);

			strEventData += GetStringByActualTime(pEvent->IsFade(), &pEvent->GetActualTime() );
			strEventData += "     DirSpeed = " ;
			strEventData += GetStringByCMinMax( &pEvent->GetDirSpeed() );
		} break;
	case CKTDGXMeshPlayer::ET_BLACKHOLETIME:
		{
			CKTDGXMeshPlayer::CXMeshEvent_BlackHoleTime* pEvent = 
				static_cast<CKTDGXMeshPlayer::CXMeshEvent_BlackHoleTime*>(pEventData_);

			strEventData += GetStringByActualTime(pEvent->IsFade(), &pEvent->GetActualTime() );
			strEventData += "     BlackHoleTime = " ;
			strEventData += GetStringByCMinMax( &pEvent->GetBlackHoleTime() );
		} break;
	case CKTDGXMeshPlayer::ET_CRASH:
		{
			CKTDGXMeshPlayer::CXMeshEvent_Crash* pEvent = 
				static_cast<CKTDGXMeshPlayer::CXMeshEvent_Crash*>(pEventData_);

			strEventData += GetStringByActualTime(pEvent->IsFade(), &pEvent->GetActualTime() );
			strEventData += "     Crash = " ;
			strEventData += GetStringByCMinMax( &pEvent->GetCrash() );
		} break;
	case CKTDGXMeshPlayer::ET_RESETCRASH:
		{
			CKTDGXMeshPlayer::CXMeshEvent_ResetCrash* pEvent = 
				static_cast<CKTDGXMeshPlayer::CXMeshEvent_ResetCrash*>(pEventData_);

			strEventData += GetStringByActualTime(pEvent->IsFade(), &pEvent->GetActualTime() );
			strEventData += "     ResetCrash = " ;
			strEventData += GetStringByCMinMax( &pEvent->GetResetCrash() );
		} break;
	case CKTDGXMeshPlayer::ET_LIGHTFLOWIMPACT:
		{
			CKTDGXMeshPlayer::CXMeshEvent_LightFlowImpact* pEvent = 
				static_cast<CKTDGXMeshPlayer::CXMeshEvent_LightFlowImpact*>(pEventData_);

			strEventData += GetStringByActualTime(pEvent->IsFade(), &pEvent->GetActualTime() );
			strEventData += "     LightFlowImpact = " ;
			strEventData += GetStringByCMinMax( &pEvent->GetLightFlowImpact() );
		} break;
	case CKTDGXMeshPlayer::ET_LIGHTFLOWPOINT:
		{
			CKTDGXMeshPlayer::CXMeshEvent_LightFlowPoint* pEvent = 
				static_cast<CKTDGXMeshPlayer::CXMeshEvent_LightFlowPoint*>(pEventData_);

			strEventData += GetStringByActualTime(pEvent->IsFade(), &pEvent->GetActualTime() );
			strEventData += "     LightFlowPoint = " ;
			strEventData += GetStringByCMinMax( &pEvent->GetLightFlowPoint() );
		} break;
	case CKTDGXMeshPlayer::ET_SOUND:
		{
			CKTDGXMeshPlayer::CXMeshEvent_Sound* pEvent = 
				static_cast<CKTDGXMeshPlayer::CXMeshEvent_Sound*>(pEventData_);

			strEventData += GetStringByActualTime(pEvent->IsFade(), &pEvent->GetActualTime() );
			strEventData += "     Sound = \"" ;

			string strTextureName;
			ConvertWCHARToChar( strTextureName, pEvent->GetSoundName() );
			strEventData += strTextureName;
			strEventData += "\"";
		} break;
	case CKTDGXMeshPlayer::ET_SLASHTRACE:
		{
			CKTDGXMeshPlayer::CXMeshEvent_SlashTrace* pEvent = 
				static_cast<CKTDGXMeshPlayer::CXMeshEvent_SlashTrace*>(pEventData_);

			strEventData += GetStringByActualTime(pEvent->IsFade(), &pEvent->GetActualTime() );
			strEventData += "     SlashTrace = " ;

			if( true == pEvent->GetSlashTrace() )
				strEventData += "True";
			else
				strEventData += "False";
		} break;
	case CKTDGXMeshPlayer::ET_RENDERTYPE:
		{
			CKTDGXMeshPlayer::CXMeshEvent_RenderType* pEvent = 
				static_cast<CKTDGXMeshPlayer::CXMeshEvent_RenderType*>(pEventData_);


			strEventData += GetStringByActualTime(pEvent->IsFade(), &pEvent->GetActualTime() );
			strEventData += "     Render_Type = " ;
			strEventData += GetStringRenderType( pEvent->GetRenderType() );
		} break;
	case CKTDGXMeshPlayer::ET_POSITION:
		{
			CKTDGXMeshPlayer::CXMeshEvent_Position* pEvent = 
				static_cast<CKTDGXMeshPlayer::CXMeshEvent_Position*>(pEventData_);

			strEventData += GetStringByActualTime(pEvent->IsFade(), &pEvent->GetActualTime() );
			strEventData += "     Position = " ;
			strEventData += GetStringByCMinMax( &pEvent->GetPosition() );
		} break;
	} //switch( pEventData_->GetEventType() )
	return strEventData;
}
std::string CX2EffectToolFileManager::GetStringByEventData( CKTDGParticleSystem::CParticleEvent* pEventData_ )
{
	std::string strEventData = "";
	switch( pEventData_->GetEventType() )
	{
	case CKTDGParticleSystem::ET_COLOR:
		{
			CKTDGParticleSystem::CParticleEvent_Color* pEvent = 
				static_cast<CKTDGParticleSystem::CParticleEvent_Color*>(pEventData_);

			strEventData += GetStringByActualTime(pEvent->IsFade(), &pEvent->GetActualTime() );
			strEventData += "     Color = " ;
			strEventData += GetStringByCMinMax( &pEvent->GetColor() );
		} break; //case CKTDGParticleSystem::ET_COLOR:

	case CKTDGParticleSystem::ET_SIZE:
		{
			CKTDGParticleSystem::CParticleEvent_Size* pEvent = 
				static_cast<CKTDGParticleSystem::CParticleEvent_Size*>(pEventData_);

			strEventData += GetStringByActualTime(pEvent->IsFade(), &pEvent->GetActualTime() );
			strEventData += "     Size = " + GetStringByCMinMax( &pEvent->GetSize() );
		} break; //case CKTDGParticleSystem::ET_SIZE:

	case CKTDGParticleSystem::ET_SCALE:
		{
			CKTDGParticleSystem::CParticleEvent_Scale* pEvent = 
				static_cast<CKTDGParticleSystem::CParticleEvent_Scale*>(pEventData_);

			strEventData += GetStringByActualTime(pEvent->IsFade(), &pEvent->GetActualTime() );
			strEventData +="     Scale = " + GetStringByCMinMax( &pEvent->GetScale() );
		} break;

	case CKTDGParticleSystem::ET_VELOCITY:
		{
			CKTDGParticleSystem::CParticleEvent_Velocity* pEvent = 
				static_cast<CKTDGParticleSystem::CParticleEvent_Velocity*>(pEventData_);

			strEventData += GetStringByActualTime(pEvent->IsFade(), &pEvent->GetActualTime() );
			strEventData +="     Velocity = " + GetStringByCMinMax( &pEvent->GetVelocity() );
		} break;

	case CKTDGParticleSystem::ET_POSITION:
		{
			CKTDGParticleSystem::CParticleEvent_Position* pEvent = 
				static_cast<CKTDGParticleSystem::CParticleEvent_Position*>(pEventData_);

			strEventData += GetStringByActualTime(pEvent->IsFade(), &pEvent->GetActualTime() );
			strEventData +="     Position = " + GetStringByCMinMax( &pEvent->GetPosition() );
		} break;
	case CKTDGParticleSystem::ET_TEX0UV:
		{
			CKTDGParticleSystem::CParticleEvent_Tex0UV* pEvent = 
				static_cast<CKTDGParticleSystem::CParticleEvent_Tex0UV*>(pEventData_);

			strEventData += GetStringByActualTime(pEvent->IsFade(), &pEvent->GetActualTime() );
			strEventData +="     TEX0UV = " + GetStringByCMinMax( &pEvent->GetTexUV() );
		} break;
	case CKTDGParticleSystem::ET_TEX1UV:
		{
			CKTDGParticleSystem::CParticleEvent_Tex1UV* pEvent = 
				static_cast<CKTDGParticleSystem::CParticleEvent_Tex1UV*>(pEventData_);

			strEventData += GetStringByActualTime(pEvent->IsFade(), &pEvent->GetActualTime() );
			strEventData +="     TEX1UV = " + GetStringByCMinMax( &pEvent->GetTexUV() );
		} break;
	case CKTDGParticleSystem::ET_TEX2UV:
		{
			CKTDGParticleSystem::CParticleEvent_Tex2UV* pEvent = 
				static_cast<CKTDGParticleSystem::CParticleEvent_Tex2UV*>(pEventData_);

			strEventData += GetStringByActualTime(pEvent->IsFade(), &pEvent->GetActualTime() );
			strEventData +="     TEX2UV = " + GetStringByCMinMax( &pEvent->GetTexUV() );
		} break;
	case CKTDGParticleSystem::ET_ROTATE:
		{
			CKTDGParticleSystem::CParticleEvent_Rotate* pEvent = 
				static_cast<CKTDGParticleSystem::CParticleEvent_Rotate*>(pEventData_);

			strEventData += GetStringByActualTime(pEvent->IsFade(), &pEvent->GetActualTime() );
			strEventData +="     Rotate = " + GetStringByCMinMax( &pEvent->GetRotate() );
		} break; //case CKTDGParticleSystem::ET_SIZE:
	case CKTDGParticleSystem::ET_DIRSPEED:
		{
			CKTDGParticleSystem::CParticleEvent_DirSpeed* pEvent = 
				static_cast<CKTDGParticleSystem::CParticleEvent_DirSpeed*>(pEventData_);

			strEventData += GetStringByActualTime(pEvent->IsFade(), &pEvent->GetActualTime() );
			strEventData +="     DIRSPEED = " + GetStringByCMinMax( &pEvent->GetValue() );
		} break;
	case CKTDGParticleSystem::ET_BLACKHOLE:
		{
			CKTDGParticleSystem::CParticleEvent_BlackHole* pEvent = 
				static_cast<CKTDGParticleSystem::CParticleEvent_BlackHole*>(pEventData_);

			strEventData += GetStringByActualTime(pEvent->IsFade(), &pEvent->GetActualTime() );
			strEventData +="     BlackHoleTime = " + GetStringByCMinMax( &pEvent->GetEventTimer() );
		} break;
	case CKTDGParticleSystem::ET_CRASH:
		{
			CKTDGParticleSystem::CParticleEvent_Crash* pEvent = 
				static_cast<CKTDGParticleSystem::CParticleEvent_Crash*>(pEventData_);

			strEventData += GetStringByActualTime(pEvent->IsFade(), &pEvent->GetActualTime() );
			strEventData +="     Crash = " + GetStringByCMinMax( &pEvent->GetCrash() );
		} break;
	case CKTDGParticleSystem::ET_RESETCRASH:
		{
			CKTDGParticleSystem::CParticleEvent_ResetCrash* pEvent = 
				static_cast<CKTDGParticleSystem::CParticleEvent_ResetCrash*>(pEventData_);

			strEventData += GetStringByActualTime(pEvent->IsFade(), &pEvent->GetActualTime() );
			strEventData +="     ResetCrash = " + GetStringByCMinMax( &pEvent->GetResetCrash() );
		} break;
	case CKTDGParticleSystem::ET_TEXTURE:
		{
			CKTDGParticleSystem::CParticleEvent_Texture* pEvent = 
				static_cast<CKTDGParticleSystem::CParticleEvent_Texture*>(pEventData_);

			strEventData += GetStringByActualTime(pEvent->IsFade(), &pEvent->GetActualTime() );

			string strTextureName;
			ConvertWCHARToChar( strTextureName, pEvent->GetTextureName() );
			strEventData +="     Texture = \"" + strTextureName + "\"";
		} break;
	case CKTDGParticleSystem::ET_EVENTTIMER:
		{
			CKTDGParticleSystem::CParticleEvent_EventTimer* pEvent = 
				static_cast<CKTDGParticleSystem::CParticleEvent_EventTimer*>(pEventData_);

			strEventData += GetStringByActualTime(pEvent->IsFade(), &pEvent->GetActualTime() );
			strEventData +="     EventTimer = " + GetStringByCMinMax( &pEvent->GetEventTimer() );
		} break;
	case CKTDGParticleSystem::ET_STRETCH:
		{
			CKTDGParticleSystem::CParticleEvent_Stretch* pEvent = 
				static_cast<CKTDGParticleSystem::CParticleEvent_Stretch*>(pEventData_);

			strEventData += GetStringByActualTime(pEvent->IsFade(), &pEvent->GetActualTime() );
			strEventData +="     Stretch = " + GetStringByCMinMax( &pEvent->GetValue() );
		} break;
	} //switch( pEventList->GetEventType() )

	return strEventData;
}
void CX2EffectToolFileManager::SaveParticle_EventList( std::ofstream& ofs_, CKTDGParticleSystem::CParticleEventSequence* pParticleSequnce_ )
{	
	ofs_ << endl ;
	BOOST_FOREACH( CKTDGParticleSystem::CParticleEvent* pEventList, pParticleSequnce_->m_EventList )
	{
		ofs_ << "\t\t" ;

		switch( pEventList->GetEventType() )
		{
		case CKTDGParticleSystem::ET_COLOR:
			{
				CKTDGParticleSystem::CParticleEvent_Color* pEvent = 
					static_cast<CKTDGParticleSystem::CParticleEvent_Color*>(pEventList);
				
				ofs_ << GetStringByActualTime(pEvent->IsFade(), &pEvent->GetActualTime() );
				ofs_ << "Color = " << GetStringByCMinMax( &pEvent->GetColor() );
			} break; //case CKTDGParticleSystem::ET_COLOR:

		case CKTDGParticleSystem::ET_SIZE:
			{
				CKTDGParticleSystem::CParticleEvent_Size* pEvent = 
					static_cast<CKTDGParticleSystem::CParticleEvent_Size*>(pEventList);

				ofs_ << GetStringByActualTime(pEvent->IsFade(), &pEvent->GetActualTime() );
				ofs_ << "Size = " << GetStringByCMinMax( &pEvent->GetSize() );
			} break; //case CKTDGParticleSystem::ET_SIZE:
		
		case CKTDGParticleSystem::ET_SCALE:
			{
				CKTDGParticleSystem::CParticleEvent_Scale* pEvent = 
					static_cast<CKTDGParticleSystem::CParticleEvent_Scale*>(pEventList);

				ofs_ << GetStringByActualTime(pEvent->IsFade(), &pEvent->GetActualTime() );
				ofs_ << "Scale = " << GetStringByCMinMax( &pEvent->GetScale() );
			} break;
				
		case CKTDGParticleSystem::ET_VELOCITY:
			{
				CKTDGParticleSystem::CParticleEvent_Velocity* pEvent = 
					static_cast<CKTDGParticleSystem::CParticleEvent_Velocity*>(pEventList);

				ofs_ << GetStringByActualTime(pEvent->IsFade(), &pEvent->GetActualTime() );
				ofs_ << "Velocity = " << GetStringByCMinMax( &pEvent->GetVelocity() );
			} break;

		case CKTDGParticleSystem::ET_POSITION:
			{
				CKTDGParticleSystem::CParticleEvent_Position* pEvent = 
					static_cast<CKTDGParticleSystem::CParticleEvent_Position*>(pEventList);

				ofs_ << GetStringByActualTime(pEvent->IsFade(), &pEvent->GetActualTime() );
				ofs_ << "Position = " << GetStringByCMinMax( &pEvent->GetPosition() );
			} break;
		case CKTDGParticleSystem::ET_TEX0UV:
			{
				CKTDGParticleSystem::CParticleEvent_Tex0UV* pEvent = 
					static_cast<CKTDGParticleSystem::CParticleEvent_Tex0UV*>(pEventList);

				ofs_ << GetStringByActualTime(pEvent->IsFade(), &pEvent->GetActualTime() );
				ofs_ << "TEX0UV = " << GetStringByCMinMax( &pEvent->GetTexUV() );
			} break;
		case CKTDGParticleSystem::ET_TEX1UV:
			{
				CKTDGParticleSystem::CParticleEvent_Tex1UV* pEvent = 
					static_cast<CKTDGParticleSystem::CParticleEvent_Tex1UV*>(pEventList);

				ofs_ << GetStringByActualTime(pEvent->IsFade(), &pEvent->GetActualTime() );
				ofs_ << "TEX1UV = " << GetStringByCMinMax( &pEvent->GetTexUV() );
			} break;
		case CKTDGParticleSystem::ET_TEX2UV:
			{
				CKTDGParticleSystem::CParticleEvent_Tex2UV* pEvent = 
					static_cast<CKTDGParticleSystem::CParticleEvent_Tex2UV*>(pEventList);

				ofs_ << GetStringByActualTime(pEvent->IsFade(), &pEvent->GetActualTime() );
				ofs_ << "TEX2UV = " << GetStringByCMinMax( &pEvent->GetTexUV() );
			} break;
		case CKTDGParticleSystem::ET_ROTATE:
			{
				CKTDGParticleSystem::CParticleEvent_Rotate* pEvent = 
					static_cast<CKTDGParticleSystem::CParticleEvent_Rotate*>(pEventList);

				ofs_ << GetStringByActualTime(pEvent->IsFade(), &pEvent->GetActualTime() );
				ofs_ << "Rotate = " << GetStringByCMinMax( &pEvent->GetRotate() );
			} break; //case CKTDGParticleSystem::ET_SIZE:
		case CKTDGParticleSystem::ET_DIRSPEED:
			{
				CKTDGParticleSystem::CParticleEvent_DirSpeed* pEvent = 
					static_cast<CKTDGParticleSystem::CParticleEvent_DirSpeed*>(pEventList);

				ofs_ << GetStringByActualTime(pEvent->IsFade(), &pEvent->GetActualTime() );
				ofs_ << "DIRSPEED = " << GetStringByCMinMax( &pEvent->GetValue() );
			} break;
		case CKTDGParticleSystem::ET_BLACKHOLE:
			{
				CKTDGParticleSystem::CParticleEvent_BlackHole* pEvent = 
					static_cast<CKTDGParticleSystem::CParticleEvent_BlackHole*>(pEventList);

				ofs_ << GetStringByActualTime(pEvent->IsFade(), &pEvent->GetActualTime() );
				ofs_ << "BlackHoleTime = " << GetStringByCMinMax( &pEvent->GetEventTimer() );
			} break;
		case CKTDGParticleSystem::ET_CRASH:
			{
				CKTDGParticleSystem::CParticleEvent_Crash* pEvent = 
					static_cast<CKTDGParticleSystem::CParticleEvent_Crash*>(pEventList);

				ofs_ << GetStringByActualTime(pEvent->IsFade(), &pEvent->GetActualTime() );
				ofs_ << "Crash = " << GetStringByCMinMax( &pEvent->GetCrash() );
			} break;
		case CKTDGParticleSystem::ET_RESETCRASH:
			{
				CKTDGParticleSystem::CParticleEvent_ResetCrash* pEvent = 
					static_cast<CKTDGParticleSystem::CParticleEvent_ResetCrash*>(pEventList);

				ofs_ << GetStringByActualTime(pEvent->IsFade(), &pEvent->GetActualTime() );
				ofs_ << "ResetCrash = " << GetStringByCMinMax( &pEvent->GetResetCrash() );
			} break;
		case CKTDGParticleSystem::ET_TEXTURE:
			{
				CKTDGParticleSystem::CParticleEvent_Texture* pEvent = 
					static_cast<CKTDGParticleSystem::CParticleEvent_Texture*>(pEventList);

				ofs_ << GetStringByActualTime(pEvent->IsFade(), &pEvent->GetActualTime() );

				string strTextureName;
				ConvertWCHARToChar( strTextureName, pEvent->GetTextureName() );
				ofs_ << "Texture = \"" << strTextureName << "\"";
			} break;
		case CKTDGParticleSystem::ET_EVENTTIMER:
			{
				CKTDGParticleSystem::CParticleEvent_EventTimer* pEvent = 
					static_cast<CKTDGParticleSystem::CParticleEvent_EventTimer*>(pEventList);

				ofs_ << GetStringByActualTime(pEvent->IsFade(), &pEvent->GetActualTime() );
				ofs_ << "EventTimer = " << GetStringByCMinMax( &pEvent->GetEventTimer() );
			} break;
		case CKTDGParticleSystem::ET_STRETCH:
			{
				CKTDGParticleSystem::CParticleEvent_Stretch* pEvent = 
					static_cast<CKTDGParticleSystem::CParticleEvent_Stretch*>(pEventList);

				ofs_ << GetStringByActualTime(pEvent->IsFade(), &pEvent->GetActualTime() );
				ofs_ << "Stretch = " << GetStringByCMinMax( &pEvent->GetValue() );
			} break;
		} //switch( pEventList->GetEventType() )

		ofs_ << endl;
	} //BOOST_FOREACH
}
void CX2EffectToolFileManager::SaveParticle_BasicData( std::ofstream& ofs_, CKTDGParticleSystem::CParticleEventSequence* pParticleSequnce_)
{
	ofs_ << GetStringParticleType( pParticleSequnce_ ) ;
	ofs_ << GetStringRenderType( "MeshRenderType", pParticleSequnce_->GetMeshRenderType() );


	SaveParticle_NumericProp( ofs_, pParticleSequnce_ );
	SaveParticle_VectorProp( ofs_, pParticleSequnce_ );
	SaveParticle_EnumProp( ofs_, pParticleSequnce_ );
	SaveParticle_BoolProp( ofs_, pParticleSequnce_ );
	SaveParticle_StringProp( ofs_, pParticleSequnce_ );
}

#pragma region Paritcle - SeqNumericProp
void CX2EffectToolFileManager::SaveParticle_NumericProp( std::ofstream& ofs_, CKTDGParticleSystem::CParticleEventSequence* pParticleSequnce_ )
{
	if( NULL == pParticleSequnce_ )
		return;

	///  EMITRATE, 	LIFETIME, NUMPARTICLES, TRIGGERCOUNT, TRIGGERTIME, DRAWCOUNT, LANDPOS, LAYER, SCREWVALUE
	///	 SCREWROTATESPEED, SPHERICAL_EMITRADIUS, SPHERICAL_EMITAZIMUTH, SPHERICAL_EMITPOLAR

	if( 0.f != pParticleSequnce_->GetEmitRate().m_Max )
		ofs_ << GetStringNumericProp( "Emitrate", &pParticleSequnce_->GetEmitRate(), 3 ) << endl;

	if( 0.f != pParticleSequnce_->GetLifeTime().m_Max )
		ofs_ << GetStringNumericProp( "Lifetime", &pParticleSequnce_->GetLifeTime(), 3 ) << endl;

	if( 0 != pParticleSequnce_->GetMaxParticleNum() )
		ofs_ << GetStringNumericProp( "NumParticles", pParticleSequnce_->GetMaxParticleNum(), 2 ) << endl;

	if( -1 != pParticleSequnce_->GetTriggerCount() )
		ofs_ << GetStringNumericProp( "TriggerCount", &pParticleSequnce_->GetTriggerCount_Origin(), 2 ) << endl;

	if( -1.f != pParticleSequnce_->GetTriggerTime() )
		ofs_ << GetStringNumericProp( "TriggerTime", &pParticleSequnce_->GetTriggerTime_Origin(), 3 ) << endl;

	if( 1 != pParticleSequnce_->GetDrawCount() )
		ofs_ << GetStringNumericProp( "DrawCount", &pParticleSequnce_->GetDrawCount_Origin(), 3 ) << endl;

	if( 0.f != pParticleSequnce_->GetLandPosition() )
		ofs_ << GetStringNumericProp( "LandPos", &pParticleSequnce_->GetLandPosition_Origin(), 4 ) << endl;

	if( -1 != pParticleSequnce_->GetScrewValue() )
		ofs_ << GetStringNumericProp( "ScrewValue", pParticleSequnce_->GetScrewValue(), 3 ) << endl;

	if( 10 != pParticleSequnce_->GetScrewRotateSpeed() )
		ofs_ << GetStringNumericProp( "ScrewRotateSpeed", pParticleSequnce_->GetScrewRotateSpeed(), 1 ) << endl;

	ofs_ << GetStringNumericProp( "Layer", pParticleSequnce_->GetLayer(), 4 ) << endl;	

	if( 0.f != pParticleSequnce_->GetSphericalEmitRadius().m_Max )
	{
		ofs_ << GetStringNumericProp( "Spherical_Emitradius", &pParticleSequnce_->GetSphericalEmitRadius(), 1 ) << endl;

		// 오현빈 // Set / Get에서의 수치가 다른 방식으로 처리 하기 때문에 
		// ConvertRadianToDegreeByMinMax 함수로 예외처리
		ofs_ << GetStringNumericProp( "Spherical_Emitazimuth", &ConvertRadianToDegree(&pParticleSequnce_->GetSphericalEmitAzimuth()), 1 ) << endl;
		ofs_ << GetStringNumericProp( "Spherical_Emitpolar", &ConvertRadianToDegree(&pParticleSequnce_->GetSphericalEmitPolar()), 2 ) << endl;
	}
}
// lifetime			= 1.5 형태의 스트링 반환
inline std::string CX2EffectToolFileManager::GetStringNumericProp( const char* pTemp_, float fValue_, int iEndTabCnt_ )
{	
	int iTempValue_= static_cast<int>(fValue_);

	char charTemp[128] = {0,};
	if( static_cast<float>(iTempValue_) == fValue_ )
		sprintf( charTemp, "= %d", iTempValue_ ); 
	else
		sprintf( charTemp, "= %.3f", fValue_ ); 

	return GetStringTabAndKeword( pTemp_, iEndTabCnt_ ) + charTemp;
}
// numparticles		= 1 형태의 스트링 반환
inline std::string CX2EffectToolFileManager::GetStringNumericProp( const char* pTemp_, int iValue_, int iEndTabCnt_ )
{	
	char charTemp[128] = {0,};
	sprintf( charTemp, "= %d", iValue_ ); 
	return GetStringTabAndKeword( pTemp_, iEndTabCnt_ ) + charTemp;
	//str(boost::format("= %d") % iValue_);
}
// Spherical_Emitradius	= random(40, 50) 형태의 스트링 반환
inline std::string CX2EffectToolFileManager::GetStringNumericProp( const char* pTemp_, const CMinMax<float>* cMinMaxFloat_, int iEndTabCnt_ )
{	
	return GetStringTabAndKeword( pTemp_, iEndTabCnt_ ) + "= " + GetStringByCMinMax(cMinMaxFloat_);
}
inline std::string CX2EffectToolFileManager::GetStringNumericProp( const char* pTemp_, const CMinMax<int>* cMinMaxInt_, int iEndTabCnt_ )
{		
	return GetStringNumericProp( pTemp_, &CMinMax<float>( static_cast<float>(cMinMaxInt_->m_Max), static_cast<float>(cMinMaxInt_->m_Min) ), iEndTabCnt_);
}
#pragma endregion Paritcle - SeqNumericProp

#pragma region Paritcle - SeqVectorProp
void CX2EffectToolFileManager::SaveParticle_VectorProp( std::ofstream& ofs_, CKTDGParticleSystem::CParticleEventSequence* pParticleSequnce_ )
{
	if( NULL == pParticleSequnce_ )
		return;

	/// EMITRADIUS, EMITRADIUSMIN, GRAVITY, BLACKHOLEPOS, LIGHTPOS, ADDROTATEREL, 
	///	SPHERICAL_EMITROTATION

	const D3DXVECTOR3 vecInitVector(0.f,0.f,0.f);

	if( vecInitVector != pParticleSequnce_->GetEmitRadius().m_Max )		
		ofs_ << GetStringVectorProp( "Emitradius", &pParticleSequnce_->GetEmitRadius() , 3 ) << endl;

	if( vecInitVector != pParticleSequnce_->GetEmitRadiusMin().m_Max )
		ofs_ << GetStringVectorProp( "EmitradiusMin", &pParticleSequnce_->GetEmitRadiusMin() , 2 ) << endl;	

	if( vecInitVector != pParticleSequnce_->GetGravity().m_Max )
		ofs_ << GetStringVectorProp( "Gravity", &pParticleSequnce_->GetGravity() , 4 ) << endl;

	if( vecInitVector != pParticleSequnce_->GetBlackHolePosition_Origin().m_Max )
		ofs_ << GetStringVectorProp( "BlackHolePos", &pParticleSequnce_->GetBlackHolePosition_Origin() , 2 ) << endl;

	if( vecInitVector != pParticleSequnce_->GetLightPos_Origin().m_Max )
		ofs_ << GetStringVectorProp( "LightPos", &pParticleSequnce_->GetLightPos_Origin() , 3 ) << endl;	

	if( vecInitVector != pParticleSequnce_->GetAddRotateRel_Origin().m_Max )
		ofs_ << GetStringVectorProp( "AddRotateRel", &pParticleSequnce_->GetAddRotateRel_Origin() , 2 ) << endl;

	// 오현빈 // Set / Get에서의 수치가 다른 방식으로 처리 하기 때문에 
	if( vecInitVector != pParticleSequnce_->GetvSphericalEmitRotation_Origin().m_Max )
		ofs_ << GetStringVectorProp( "Spherical_EmitRotation", &pParticleSequnce_->GetvSphericalEmitRotation_Origin() , 1 ) << endl;
}
// emitradius			= XYZ(random(-300,500),400,0) 형태의 스트링 반환
inline std::string CX2EffectToolFileManager::GetStringVectorProp( const char* pTemp_, const CMinMax<D3DXVECTOR3>* cMinMaxVec_, int iEndTabCnt_ )
{	
	return GetStringTabAndKeword( pTemp_, iEndTabCnt_) + "= " + GetStringByCMinMax(cMinMaxVec_);
}
// blackholepos 		= xyz( 200,800,0 ) 형태의 스트링 반환
inline std::string CX2EffectToolFileManager::GetStringVectorProp( const char* pTemp_, const D3DXVECTOR3* vecValue_, int iEndTabCnt_ )
{	
	return GetStringTabAndKeword( pTemp_, iEndTabCnt_) + "= " + GetStringByVec(vecValue_);
}
#pragma endregion SeqVectorProp

#pragma region Particle - Etc. 
void CX2EffectToolFileManager::SaveParticle_EnumProp( std::ofstream& ofs_, CKTDGParticleSystem::CParticleEventSequence* pParticleSequnce_ )
{
	if( NULL == pParticleSequnce_ )
		return;

	/// SeqAlphaBlendModeProp
	/// SeqCullModeProp 
	/// SeqParticleTypeProp
	/// SeqMeshRenderTypeProp
	/// SeqBillboardTypeProp

	ofs_ << GetStringCullMode( "CullMode", static_cast<D3DCULL>(pParticleSequnce_->GetCullMode()) ) ;
	ofs_ << GetStringBillBoardType( "BillBoardType", pParticleSequnce_->GetBillBoardType() );	
	ofs_ << GetStringBlend( "SourceBlendMode", pParticleSequnce_->GetSrcBlendMode() );
	ofs_ << GetStringBlend( "DestBlendMode", pParticleSequnce_->GetDestBlendMode() ) ;
}

#pragma endregion Particle - Etc.

#pragma region Particle - SeqBoolProp
void CX2EffectToolFileManager::SaveParticle_BoolProp( std::ofstream& ofs_, CKTDGParticleSystem::CParticleEventSequence* pParticleSequnce_ )
{
	if( NULL == pParticleSequnce_ )
		return;

	/// USELAND, TRACE, ZENABLE, ZWRITEENABLE, RESOLUTIONCONVERT, FORCELAYER, APPLYUNITSCALE,

	if( false != pParticleSequnce_->GetUseLand() )
		ofs_ << GetStringBoolProp( "UseLand", pParticleSequnce_->GetUseLand(), 4 ) << endl;

	if( false != pParticleSequnce_->GetTrace() )
		ofs_ << GetStringBoolProp( "Trace", pParticleSequnce_->GetTrace(), 4 ) << endl;

	if( true != pParticleSequnce_->GetZEnable() )
		ofs_ << GetStringBoolProp( "ZEnable", pParticleSequnce_->GetZEnable(), 4 ) << endl;

	if( true != pParticleSequnce_->GetZWriteEnable() )
		ofs_ << GetStringBoolProp( "ZWriteEnable", pParticleSequnce_->GetZWriteEnable(), 2 ) << endl;

	if( false != pParticleSequnce_->GetResolutionConvert() )
		ofs_ << GetStringBoolProp( "ResolutionConvert", pParticleSequnce_->GetResolutionConvert(), 1 ) << endl;

	if( false != pParticleSequnce_->GetForceLayer() )
		ofs_ << GetStringBoolProp( "ForceLayer", pParticleSequnce_->GetForceLayer(), 3 ) << endl;

	if( true != pParticleSequnce_->GetApplyUnitScale() )
		ofs_ << GetStringBoolProp( "ApplyUnitScale", pParticleSequnce_->GetApplyUnitScale(), 2 ) << endl;
}
inline std::string CX2EffectToolFileManager::GetStringBoolProp( const char* pTemp_, bool bBool_, int iEndTabCnt_ )
{
	if( true == bBool_ )
		return GetStringTabAndKeword( pTemp_, iEndTabCnt_ ) + "= TRUE";
	else
		return GetStringTabAndKeword( pTemp_, iEndTabCnt_ ) + "= FALSE";
}

#pragma endregion Particle - SeqBoolProp

#pragma region Particle - SeqStringProp
void CX2EffectToolFileManager::SaveParticle_StringProp( std::ofstream& ofs_, CKTDGParticleSystem::CParticleEventSequence* pParticleSequnce_ )
{
	if( NULL == pParticleSequnce_ )
		return;

	/// MESH, SKINMESH, CHANGETEXXET, MULTITEXXET, ANIXET, ANINAME, TRACEPARTICLELIST, FINALPARTICLELIST
	ofs_ << GetStringStringProp( "Mesh", pParticleSequnce_->GetMeshName(), 4 );
	ofs_ << GetStringStringProp( "ChangeTexXet", pParticleSequnce_->GetChangeTexXETName(), 3 );
	ofs_ << GetStringStringProp( "MultiTexXet", pParticleSequnce_->GetMultiTexXETName(), 3 );
	ofs_ << GetStringStringProp( "AniXet", pParticleSequnce_->GetAniXETName(), 4 );
	ofs_ << GetStringStringProp( "AniName", pParticleSequnce_->GetAniName(), 4 );

	BOOST_FOREACH( wstring SeqName, pParticleSequnce_->GetTraceSeqNameList() ) 
	{
		ofs_ << GetStringStringProp( "TraceParticleList", SeqName.c_str() , 2 );
	}
	BOOST_FOREACH( wstring SeqName, pParticleSequnce_->GetFinalSeqNameList() ) 
	{
		ofs_ << GetStringStringProp( "FinalParticleList", SeqName.c_str() , 2 );
	}
}

inline std::string CX2EffectToolFileManager::GetStringStringProp( const char* pStrType_, const WCHAR* pStrValue_, int iEndTabCnt_ ) const
{
	if( wcslen(pStrValue_) == 0)
		return "";

	std::string strValue = "";
	ConvertWCHARToChar( strValue, pStrValue_ );

	std::string strTemp = GetStringTabAndKeword( pStrType_, iEndTabCnt_ );
	strTemp += "= \"";
	strTemp += strValue;
	strTemp += "\"\n";
	
	return strTemp;
}

#pragma endregion Particle - SeqStringProp
/// 기본 띄어쓰기(tab)과 키워드(ex:Layer)를 조합하는 함수
/// 코드 중복 제거하기 위해 추가.
inline std::string CX2EffectToolFileManager::GetStringTabAndKeword( const char* pTemp_, int iEndTabCnt_ ) 
{
	std::string strTabAndKeword = "\t\t";
	strTabAndKeword += pTemp_ ;
	for( int i = 0; i < iEndTabCnt_; ++i)
		strTabAndKeword += "\t";
	return strTabAndKeword;
}

#pragma region CMinMax To String
std::string CX2EffectToolFileManager::GetStringByActualTime( bool bFade_, const CMinMax<float>* cMinMaxFloat_)
{
	char charTemp[64] = {0,};
	if( true == bFade_)
	{
		//fade so final
		if( 0.f == cMinMaxFloat_->m_Min && false == IsRangeValue(cMinMaxFloat_) )
			sprintf( charTemp, "Fade so final\t\t\t\t" );
		else // fade so from
			sprintf( charTemp, "Fade so From %.3f to %.3f\t", cMinMaxFloat_->m_Min, cMinMaxFloat_->m_Max );
	}
	else
	{
		// Initial 
		if( 0.f == cMinMaxFloat_->m_Min && false == IsRangeValue(cMinMaxFloat_))
			sprintf( charTemp, "Initial\t\t\t\t\t\t" );
		else //at
			sprintf( charTemp, "at %.3f\t\t\t\t\t\t", cMinMaxFloat_->m_Max );
	}
	return std::string(charTemp);
}
std::string	CX2EffectToolFileManager::GetStringByCMinMax( const CMinMax<D3DXCOLOR>* cMinMaxColor_ ) 
{
	char charTemp[128] = {0,};
	sprintf( charTemp, "RGBA(%s, %s, %s, %s)", GetStringByCMinMax(cMinMaxColor_->m_Min.r, cMinMaxColor_->m_Max.r).c_str() 
		, GetStringByCMinMax(cMinMaxColor_->m_Min.g, cMinMaxColor_->m_Max.g).c_str()
		, GetStringByCMinMax(cMinMaxColor_->m_Min.b, cMinMaxColor_->m_Max.b).c_str() 
		, GetStringByCMinMax(cMinMaxColor_->m_Min.a, cMinMaxColor_->m_Max.a).c_str());
	return std::string(charTemp);
}
std::string	CX2EffectToolFileManager::GetStringByCMinMax( const CMinMax<D3DXVECTOR2>* cMinMaxVec_ ) 
{
	char charTemp[128] = {0,};
	sprintf( charTemp, "XY(%s, %s)", GetStringByCMinMax(cMinMaxVec_->m_Min.x, cMinMaxVec_->m_Max.x).c_str() 
		, GetStringByCMinMax(cMinMaxVec_->m_Min.y, cMinMaxVec_->m_Max.y).c_str());
	return std::string(charTemp);
}
std::string	CX2EffectToolFileManager::GetStringByCMinMax( const CMinMax<D3DXVECTOR3>* cMinMaxVec_ ) 
{
	char charTemp[128] = {0,};
	sprintf( charTemp, "XYZ(%s, %s, %s)", GetStringByCMinMax(cMinMaxVec_->m_Min.x, cMinMaxVec_->m_Max.x).c_str() 
		, GetStringByCMinMax(cMinMaxVec_->m_Min.y, cMinMaxVec_->m_Max.y).c_str()
		, GetStringByCMinMax(cMinMaxVec_->m_Min.z, cMinMaxVec_->m_Max.z).c_str() );
	return std::string(charTemp);
}
// MinMax Float 값을 스트링으로 변환.
std::string	CX2EffectToolFileManager::GetStringByCMinMax( const CMinMax<float>* cMinMaxFloat_ ) 
{	
	return GetStringByCMinMax( cMinMaxFloat_->m_Min, cMinMaxFloat_->m_Max );
}
// MinMax Float 값을 스트링으로 변환.
std::string	CX2EffectToolFileManager::GetStringByCMinMax( float fMinValue_, float fMaxValue_ ) 
{
	char charTemp[32] = {0,};
	if( true == IsRangeValue(fMinValue_, fMaxValue_) )
	{
		//{{소수점 자릿수가 0인지 체크 하기 위한 구문.
		int iTempMaxValue = static_cast<int>(fMaxValue_);
		int iTempMinValue = static_cast<int>(fMinValue_);

		if( static_cast<float>(iTempMaxValue) == fMaxValue_ )
		{
			if ( static_cast<float>(iTempMinValue) == fMinValue_ )
				sprintf( charTemp, "random(%d, %d)", iTempMinValue, iTempMaxValue );
			else
				sprintf( charTemp, "random(%.3f, %d)", fMinValue_, iTempMaxValue );
		}
		else if ( static_cast<float>(iTempMinValue) == fMinValue_ )
			sprintf( charTemp, "random(%d, %.3f)", iTempMinValue, fMaxValue_ );
		else
			sprintf( charTemp, "random(%.3f, %.3f)", fMinValue_, fMaxValue_ );
	}
	else
	{
		//{{소수점 자릿수가 0인지 체크 하기 위한 구문.
		int iTempMaxValue = static_cast<int>(fMaxValue_);

		if( static_cast<float>(iTempMaxValue) == fMaxValue_ )
			sprintf( charTemp, "%d", iTempMaxValue );
		else
			sprintf( charTemp, "%.3f", fMaxValue_);
	}

	return string(charTemp);
}
std::string	CX2EffectToolFileManager::GetStringIntByFloat( float fFalue_) 
{
	char charTemp[128] = {0,};
	sprintf( charTemp, "%d", static_cast<int>(fFalue_));
	return std::string(charTemp);
}
std::string	CX2EffectToolFileManager::GetStringByVec( const D3DXVECTOR3* vecValue_ )
{
	char charTemp[128] = {0,};
	sprintf( charTemp, "XYZ(%s, %s, %s)", GetStringByCMinMax(vecValue_->x, vecValue_->x).c_str() 
		, GetStringByCMinMax(vecValue_->y, vecValue_->y).c_str()
		, GetStringByCMinMax(vecValue_->z, vecValue_->z).c_str() );
	return std::string(charTemp);
}
#pragma endregion CMinMax To String

std::string CX2EffectToolFileManager::GetStringMeshType(  const char* pTemp_, CKTDGXMeshPlayer::MESH_TYPE eMeshType_, int iEndTabCnt_ /*= 3 */ )
{	
	std::string strTemp ="";
	switch( eMeshType_ )
	{
	case CKTDGXMeshPlayer::X_MESH:			strTemp += "X_MESH";break;
	case CKTDGXMeshPlayer::X_SKIN_MESH:		strTemp += "X_SKIN_MESH";	break;
	}
	strTemp += "\n";
	return GetStringTabAndKeword( pTemp_, iEndTabCnt_) + "= " + strTemp;
}
std::string CX2EffectToolFileManager::GetStringRenderType(  const char* pTemp_, CKTDGXRenderer::RENDER_TYPE eRenderType_ )
{	
	if( CKTDGXRenderer::RT_REAL_COLOR == eRenderType_ )
		return "";

	return GetStringTabAndKeword( pTemp_, 2) + "= " + GetStringRenderType(eRenderType_) + "\n";
}
std::string CX2EffectToolFileManager::GetStringRenderType( CKTDGXRenderer::RENDER_TYPE eRenderType_ )
{
	std::string strTemp ="";
	switch( eRenderType_ )
	{
	case CKTDGXRenderer::RT_REAL_COLOR:				strTemp += "RT_REAL_COLOR";			break;
	case CKTDGXRenderer::RT_CARTOON_BLACK_EDGE:		strTemp += "RT_CARTOON_BLACK_EDGE";	break;
	case CKTDGXRenderer::RT_CARTOON_COLOR_EDGE:		strTemp += "RT_CARTOON_COLOR_EDGE";	break;
	case CKTDGXRenderer::RT_CARTOON:				strTemp += "RT_CARTOON";			break;
	case CKTDGXRenderer::RT_ADD_CARTOON_BLACK_EDGE:	strTemp += "RT_ADD_CARTOON_BLACK_EDGE";	break;
	case CKTDGXRenderer::RT_ADD_CARTOON_COLOR_EDGE:	strTemp += "RT_ADD_CARTOON_COLOR_EDGE";	break;
	case CKTDGXRenderer::RT_ADD_CARTOON:			strTemp += "RT_ADD_CARTOON";		break;
	case CKTDGXRenderer::RT_BLACK_EDGE:				strTemp += "RT_BLACK_EDGE";			break;
	case CKTDGXRenderer::RT_COLOR_EDGE:				strTemp += "RT_COLOR_EDGE";			break;
	case CKTDGXRenderer::RT_LIGHT_FLOW_WORLD_X:		strTemp += "RT_LIGHT_FLOW_WORLD_X";	break;
	case CKTDGXRenderer::RT_LIGHT_FLOW_WORLD_Y:		strTemp += "RT_LIGHT_FLOW_WORLD_Y";	break;
	case CKTDGXRenderer::RT_LIGHT_FLOW_WORLD_Z:		strTemp += "RT_LIGHT_FLOW_WORLD_Z";	break;
	case CKTDGXRenderer::RT_LIGHT_FLOW_WORLD_POINT:	strTemp += "RT_LIGHT_FLOW_WORLD_POINT";	break;
	case CKTDGXRenderer::RT_LIGHT_FLOW_LOCAL_X:		strTemp += "RT_LIGHT_FLOW_LOCAL_X";	break;
	case CKTDGXRenderer::RT_LIGHT_FLOW_LOCAL_Y:		strTemp += "RT_LIGHT_FLOW_LOCAL_Y";	break;
	case CKTDGXRenderer::RT_LIGHT_FLOW_LOCAL_Z:		strTemp += "RT_LIGHT_FLOW_LOCAL_Z";	break;
	case CKTDGXRenderer::RT_LIGHT_FLOW_LOCAL_POINT:	strTemp += "RT_LIGHT_FLOW_LOCAL_POINT";	break;
	case CKTDGXRenderer::RT_CARTOON_FADE:			strTemp += "RT_CARTOON_FADE";		break;
	case CKTDGXRenderer::RT_CARTOON_FADE_NONTEX:	strTemp += "RT_CARTOON_FADE_NONTEX";	break;
	};
	return strTemp;
}
std::string CX2EffectToolFileManager::GetStringCartoonTexType(  const char* pTemp_, CKTDGXRenderer::CARTOON_TEX_TYPE eCartoonType_, int iEndTabCnt_ /*= 3 */ )
{	
	std::string strTemp ="";
	switch( eCartoonType_ )
	{
	case CKTDGXRenderer::CTT_NORMAL:			return strTemp; break;
	case CKTDGXRenderer::CTT_RED:				strTemp += "CTT_RED"; break;
	case CKTDGXRenderer::CTT_GREEN:				strTemp += "CTT_GREEN"; break;
	case CKTDGXRenderer::CTT_VIOLET:			strTemp += "CTT_VIOLET"; break;
	case CKTDGXRenderer::CTT_NORMAL_COUNTER:	strTemp += "CTT_NORMAL_COUNTER"; break;
	case CKTDGXRenderer::CTT_DAMAGE:			strTemp += "CTT_DAMAGE"; break;
	case CKTDGXRenderer::CTT_INVINCIBLE:		strTemp += "CTT_INVINCIBLE"; break;
	case CKTDGXRenderer::CTT_PETRIFIED:			strTemp += "CTT_PETRIFIED"; break;
	case CKTDGXRenderer::CTT_SKYBLUE:			strTemp += "CTT_SKYBLUE"; break;
	case CKTDGXRenderer::CTT_SCARLET:			strTemp += "CTT_SCARLET"; break;
	case CKTDGXRenderer::CTT_END:				strTemp += "CTT_END"; break;
	}
	strTemp += "\n";
	return GetStringTabAndKeword( pTemp_, iEndTabCnt_ ) + "= " + strTemp;
}
std::string CX2EffectToolFileManager::GetStringCullMode(  const char* pTemp_, D3DCULL eCullMode_ )
{	
	std::string strTemp ="";
	switch( eCullMode_ )
	{
	case D3DCULL_NONE:			return strTemp; break;
	case D3DCULL_CW:			strTemp += "D3DCULL_CW\n"; break;
	case D3DCULL_CCW:			strTemp += "D3DCULL_CCW\n"; break;
	case D3DCULL_FORCE_DWORD:	strTemp += "D3DCULL_FORCE_DWORD\n"; break;
	}
	return GetStringTabAndKeword( pTemp_, 3) + "= " + strTemp;
}
std::string CX2EffectToolFileManager::GetStringSlash_Trace_Curve_Type(  const char* pTemp_, CKTDGSlashTrace::CURVE_TYPE eSlashTraceCurveType_ )
{	
	std::string strTemp ="";
	switch( eSlashTraceCurveType_ )
	{
	case CKTDGSlashTrace::CT_SMOOTH_CURVE:		return strTemp; break;
	case CKTDGSlashTrace::CT_TOUGH_CURVE:		strTemp += "1";	break;
	case CKTDGSlashTrace::CT_STRAIGHT_LINE:		strTemp += "2";	break;
	}
	strTemp += "\n";
	return GetStringTabAndKeword( pTemp_, 1) + "= " + strTemp;
}
std::string CX2EffectToolFileManager::GetStringBillBoardType(  const char* pTemp_, CKTDGMatrix::BILLBOARD_TYPE eBillBoardType_ , int iEndTabCnt_ /*= 2 */)
{	
	std::string strTemp ="";
	switch( eBillBoardType_ )
	{
	case CKTDGMatrix::BT_NONE: strTemp += "BT_NONE"; break;
	case CKTDGMatrix::BT_ALL: strTemp += "BT_ALL";	break;
	case CKTDGMatrix::BT_X:	 strTemp += "BT_X";	break;
	case CKTDGMatrix::BT_Y:	 strTemp += "BT_Y";			break;
	case CKTDGMatrix::BT_Z:	 strTemp += "BT_Z";			break;
	}
	strTemp += "\n";
	return GetStringTabAndKeword( pTemp_, iEndTabCnt_) + "= " + strTemp;
}
std::string CX2EffectToolFileManager::GetStringLifeType(  const char* pTemp_, CKTDGXMeshPlayer::LIFE_TYPE eLifeType_, int iEndTabCnt_/* = 3*/)
{	
	std::string strTemp ="";
	switch( eLifeType_ )
	{
	case CKTDGXMeshPlayer::LT_PLAY_COUNT:strTemp += "LT_PLAY_COUNT";break;
	case CKTDGXMeshPlayer::LT_TIME:		strTemp += "LT_TIME";	break;
	case CKTDGXMeshPlayer::LT_MANUAL: 	strTemp += "LT_MANUAL";	break;
	}
	strTemp += "\n";
	return GetStringTabAndKeword( pTemp_, iEndTabCnt_) + "= " + strTemp;
}
std::string CX2EffectToolFileManager::GetStringBlend(  const char* pTemp_, int iBlendMode_ )
{
	std::string strTemp = "\t\t";	
	strTemp += pTemp_;
	strTemp += "\t= ";

	switch( iBlendMode_ )
	{
	case D3DBLEND_ZERO:				strTemp += "D3DBLEND_ZERO";	break;
	case D3DBLEND_ONE:				strTemp += "D3DBLEND_ONE";	break;
	case D3DBLEND_SRCCOLOR:			strTemp += "D3DBLEND_SRCCOLOR";		break;
	case D3DBLEND_INVSRCCOLOR:		strTemp += "D3DBLEND_INVSRCCOLOR";	break;	
	case D3DBLEND_SRCALPHA:			strTemp += "D3DBLEND_SRCALPHA";	break;	
	case D3DBLEND_INVSRCALPHA:		strTemp += "D3DBLEND_INVSRCALPHA";	break;
	case D3DBLEND_DESTALPHA:		strTemp += "D3DBLEND_DESTALPHA";	break;
	case D3DBLEND_INVDESTALPHA:		strTemp += "D3DBLEND_INVDESTALPHA";		break;
	case D3DBLEND_DESTCOLOR:		strTemp += "D3DBLEND_DESTCOLOR";	break;	
	case D3DBLEND_INVDESTCOLOR:		strTemp += "D3DBLEND_INVDESTCOLOR";	break;	
	case D3DBLEND_SRCALPHASAT:		strTemp += "D3DBLEND_SRCALPHASAT";	break;	
	case D3DBLEND_BOTHSRCALPHA:		strTemp += "D3DBLEND_BOTHSRCALPHA";	break;	
	case D3DBLEND_BOTHINVSRCALPHA:	strTemp += "D3DBLEND_BOTHINVSRCALPHA";	break;	
	case D3DBLEND_BLENDFACTOR:		strTemp += "D3DBLEND_BLENDFACTOR";	break;	
	case D3DBLEND_INVBLENDFACTOR:	strTemp += "D3DBLEND_INVBLENDFACTOR";	break;	
	}
	strTemp += "\n";
	return strTemp;
}
std::string CX2EffectToolFileManager::GetStringParticleType( CKTDGParticleSystem::CParticleEventSequence* pParticleSequnce_ )
{
	std::string strTemp = "\t\tparticleType\t\t= ";

	switch( pParticleSequnce_->GetParticleType())
	{
	case CKTDGParticleSystem::PT_3D_PLANE:
		strTemp += "PT_3D_PLANE";
		break;
	case CKTDGParticleSystem::PT_2D_PLANE:
		strTemp += "PT_2D_PLANE";
		break;
	case CKTDGParticleSystem::PT_MESH:
		strTemp += "PT_MESH";
		break;
	case CKTDGParticleSystem::PT_SKINMESH:
		strTemp += "PT_SKINMESH";
		break;	
	}
	strTemp += "\n";
	return strTemp;
}