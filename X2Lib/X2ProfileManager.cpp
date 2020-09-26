#include "StdAfx.h"

#ifdef SERV_LOCAL_RANKING_SYSTEM
#include ".\x2profilemanager.h"


const __time64_t CX2ProfileManager::MINYEAR = 1915;
const __time64_t CX2ProfileManager::MAXYEAR = 2013;
const float CX2ProfileManager::RANKUP_UI_PLAY_TIME = 5.f;

//const int CX2ProfileManager::BUSAN_NUM		= 41;
//const int CX2ProfileManager::DAEGU_NUM		= 57;
//const int CX2ProfileManager::INCHEON_NUM		= 65;
//const int CX2ProfileManager::GWANGJU_NUM		= 75;
//const int CX2ProfileManager::DAEJEON_NUM		= 80;
//const int CX2ProfileManager::ULSAN_NUM		= 85;
//const int CX2ProfileManager::GYEONGGIDO_NUM;
//const int CX2ProfileManager::GANGWONDO_NUM;
//const int CX2ProfileManager::CHUNGBUK_NUM;
//const int CX2ProfileManager::CHUNGNAM_NUM;
//const int CX2ProfileManager::JEONBUK_NUM;
//const int CX2ProfileManager::JEONNAM_NUM;
//const int CX2ProfileManager::GYEONGBUK_NUM;
//const int CX2ProfileManager::GYEONGNAM_NUM;
//const int CX2ProfileManager::JEJU_NUM;

const int CX2ProfileManager::BUSAN_OFFSET		= 41;
const int CX2ProfileManager::DAEGU_OFFSET		= 57;
const int CX2ProfileManager::INCHEON_OFFSET		= 65;
const int CX2ProfileManager::GWANGJU_OFFSET		= 75;
const int CX2ProfileManager::DAEJEON_OFFSET		= 80;
const int CX2ProfileManager::ULSAN_OFFSET		= 85;
const int CX2ProfileManager::GYEONGGIDO_OFFSET	= 90;
const int CX2ProfileManager::GANGWONDO_OFFSET	= 121;
const int CX2ProfileManager::CHUNGBUK_OFFSET	= 139;
const int CX2ProfileManager::CHUNGNAM_OFFSET	= 151;
const int CX2ProfileManager::JEONBUK_OFFSET		= 166;
const int CX2ProfileManager::JEONNAM_OFFSET		= 180;
const int CX2ProfileManager::GYEONGBUK_OFFSET	= 202;
const int CX2ProfileManager::GYEONGNAM_OFFSET	= 225;
const int CX2ProfileManager::JEJU_OFFSET		= 243;

CX2ProfileManager::CX2ProfileManager(void)
{
	m_bShow					= false;
	m_bShowRankup			= false;
	m_bIsProfileSave		= false;
	m_bIsLoadComplete		= false;
	m_bIsOpenSelect			= false;
	m_pDlgProfileInput		= NULL;
	m_pDlgProfile			= NULL;
	m_pDlgRankUp			= NULL;
	m_pDlgCharInfo			= NULL;
	m_pDlgPassword			= NULL;

	m_fRankupFadeTime		= 0.f;
	
	m_Seq1 = NULL;
	m_Seq2 = NULL;
	m_Seq3 = NULL;

	m_wstrPassword = L"";
}

CX2ProfileManager::~CX2ProfileManager(void)
{
	SAFE_DELETE_DIALOG( m_pDlgProfileInput );
	SAFE_DELETE_DIALOG( m_pDlgProfile );
	SAFE_DELETE_DIALOG( m_pDlgRankUp );
	SAFE_DELETE_DIALOG( m_pDlgPassword );

	m_pNowState = NULL;
	
	m_vecArea.clear();
}


/** @function : CreateRankUpUI
	@brief : RankupUI 다이얼로그 생성 함수
	@param : -
	@return : -
*/
void CX2ProfileManager::CreateRankUpUI()
{
	if( NULL != m_pDlgRankUp )
		SAFE_DELETE_DIALOG( m_pDlgRankUp );

	m_pNowState = (CX2State*)g_pMain->GetNowState();

	m_pDlgRankUp = new CKTDGUIDialog( m_pNowState, L"DLG_UI_FriendSystem_RankUp.lua" );
	g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDlgRankUp );
	m_pDlgRankUp->SetShowEnable(false, false);
}


/** @function : CreateProfileUI
	@brief : 프로필 출력 다이얼로그 생성 함수
	@param : -
	@return : -
*/
void CX2ProfileManager::CreateProfileUI()
{
	if( NULL != m_pDlgProfile )
		SAFE_DELETE_DIALOG( m_pDlgProfile );

	m_pNowState = (CX2State*)g_pMain->GetNowState();

	m_pDlgProfile = new CKTDGUIDialog( m_pNowState, L"DLG_UI_FriendSystem_MyProfile.lua" );
	g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDlgProfile );    
	m_pDlgProfile->SetShowEnable(false, false);
}


/** @function : CreateProfileInputUI
	@brief : 프로필 입력 다이얼로그 생성 함수
	@param : -
	@return : -
*/
void CX2ProfileManager::CreateProfileInputUI()
{
	if( NULL != m_pDlgProfileInput )
		SAFE_DELETE_DIALOG( m_pDlgProfileInput );

	m_pNowState = (CX2State*)g_pMain->GetNowState();

	m_pDlgProfileInput = new CKTDGUIDialog( m_pNowState, L"DLG_UI_FriendSystem_ProfileInput.lua" );
	if( NULL == m_pDlgProfileInput )
		return;

	g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDlgProfileInput );    
	m_pDlgProfileInput->SetShowEnable(false, false);


	//지역 리스트 추가
	CKTDGUIComboBox* pComboBoxUpper = static_cast<CKTDGUIComboBox*>(m_pDlgProfileInput->GetControl(L"Location_choice1"));
	if( NULL == pComboBoxUpper )
		return;
	CKTDGUIComboBox* pComboBox = static_cast<CKTDGUIComboBox*>(m_pDlgProfileInput->GetControl(L"Location_choice2"));
	if( NULL == pComboBox )
		return;

	pComboBoxUpper->RemoveAllItems();
	pComboBox->RemoveAllItems();

	pComboBoxUpper->AddItem( m_vecArea[0].UpperArea.c_str(), NULL, false );
	//pComboBox->AddItem( m_vecArea[0].UnderArea.c_str(), NULL, false );
	//BOOST_FOREACH( const Area& AreaData, m_vecArea )
	for(vector<wstring>::size_type i = 1;i < BUSAN_OFFSET+1; ++i)
	{
		if( 0 != StrCmp( m_vecArea[i-1].UpperArea.c_str(), m_vecArea[i].UpperArea.c_str() ) )
			pComboBoxUpper->AddItem( m_vecArea[i].UpperArea.c_str(), NULL, false );
		//if( 0 != StrCmp( m_vecArea[i].UnderArea.c_str(), L"" ) )
		//	pComboBox->AddItem( m_vecArea[i].UnderArea.c_str(), NULL, false );
	}
	pComboBox->SetEnable( false );
	pComboBox->SetShow( false );
	//pComboBox->SetStaticHeight( 100 );
	//pComboBoxUpper->SetStaticHeight( 100 );

	//태어난 년도 추가
	CKTDGUIComboBox* pYearBox = static_cast<CKTDGUIComboBox*>(m_pDlgProfileInput->GetControl(L"Year_choice1"));
	if( NULL == pYearBox )
		return;

	pYearBox->RemoveAllItems();
	int NumYear = MAXYEAR - MINYEAR + 1;
	int year = MAXYEAR;

	WCHAR buff[8] = L"";

	pYearBox->AddItem( GET_STRING( STR_ID_23611 ), (LPVOID)NULL, false );
	for(int i = 0; i < NumYear; ++i)
	{
		StringCchPrintfW( buff, ARRAY_SIZE(buff), L"%d", year );
		pYearBox->AddItem( buff, (LPVOID)NULL, false );
		year--;
	}
}

/** @function : UIServerEventProc
	@brief : 패킷 이벤트 처리
	@param : -
	@return : 해당 버튼 이벤트가 있으면 처리 후 true 반환, 없으면 false 반환
*/
bool CX2ProfileManager::UIServerEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	switch(wParam)
	{
	case EGS_LOCAL_RANKING_USER_INFO_READ_ACK:
		{
			Handler_EGS_LOCAL_RANKING_USER_INFO_READ_ACK( hWnd, uMsg, wParam, lParam );
		}
		return true;

	case EGS_LOCAL_RANKING_USER_INFO_WRITE_ACK:
		{
			Handler_EGS_LOCAL_RANKING_USER_INFO_WRITE_ACK( hWnd, uMsg, wParam, lParam );
		}
		return true;

	case EGS_LOCAL_RANKING_RANK_UP_NOT:
		{
			Handler_EGS_LOCAL_RANKING_RANK_UP_NOT( hWnd, uMsg, wParam, lParam );
		}
		return true;
	}
	return false;
}

bool CX2ProfileManager::Handler_EGS_LOCAL_RANKING_RANK_UP_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_LOCAL_RANKING_RANK_UP_NOT kEvent;
	DeSerialize( pBuff, &kEvent );

	OpenRankup( kEvent );

	return true;
}

bool CX2ProfileManager::Handler_EGS_LOCAL_RANKING_USER_INFO_READ_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_LOCAL_RANKING_USER_INFO_READ_ACK kEvent;
	DeSerialize( pBuff, &kEvent );

	if( true == g_pMain->DeleteServerPacket( EGS_LOCAL_RANKING_USER_INFO_READ_ACK ) )
	{
		if( true == g_pMain->IsValidPacket( kEvent.m_iOK ) )
		{
		
			if( L"" == kEvent.m_wstrUpdated )
			{
				OpenProfileInput();

				return false;
			}
			m_sProfile.gender			= kEvent.m_kInfo.m_byteGender;
			m_sProfile.birthYear		= kEvent.m_kInfo.m_iBirth;
			m_sProfile.area				= kEvent.m_kInfo.m_iCategory;
			m_sProfile.introduce		= kEvent.m_kInfo.m_wstrProfile;
			m_sProfile.filter			= kEvent.m_kInfo.m_byteFilter;
			const CX2Unit::UnitData* pUnitData = NULL;
			if( NULL != g_pData->GetMyUser() && NULL != g_pData->GetMyUser()->GetSelectUnit())
				pUnitData = &g_pData->GetMyUser()->GetSelectUnit()->GetUnitData();
			if( NULL != pUnitData )
			{
				m_sProfile.nickname		= pUnitData->m_NickName;
				m_sProfile.UnitClass	= pUnitData->m_UnitClass;
				m_sProfile.level		= pUnitData->m_Level;
			}

			if( true == m_bIsOpenSelect )
			{
				m_bIsOpenSelect = false;
				CloseProfile();
				OpenProfileInput();
			}
			else
				OpenProfile( true );

			return true;
		}
	}
	m_bIsOpenSelect = false;

	return false;
}


bool CX2ProfileManager::Handler_EGS_LOCAL_RANKING_USER_INFO_READ_REQ( const wstring& _password )
{
	KEGS_LOCAL_RANKING_USER_INFO_READ_REQ kpacket;
	m_wstrPassword = _password;
	kpacket.m_wstrPassword = m_wstrPassword;
	g_pData->GetServerProtocol()->SendPacket( EGS_LOCAL_RANKING_USER_INFO_READ_REQ, kpacket );
	g_pMain->AddServerPacket( EGS_LOCAL_RANKING_USER_INFO_READ_ACK, 60.f ); 

	return true;
}

bool CX2ProfileManager::Handler_EGS_LOCAL_RANKING_USER_INFO_WRITE_REQ()
{
	KEGS_LOCAL_RANKING_USER_INFO_WRITE_REQ kPacket;
	
	kPacket.m_kInfo.m_byteGender	= m_sProfile.gender;
	kPacket.m_kInfo.m_iBirth		= m_sProfile.birthYear;
	kPacket.m_kInfo.m_iCategory		= m_sProfile.area;
	kPacket.m_kInfo.m_wstrProfile	= m_sProfile.introduce;
	kPacket.m_kInfo.m_byteFilter	= m_sProfile.filter;
	kPacket.m_wstrPassword	= m_wstrPassword;

	g_pData->GetServerProtocol()->SendPacket( EGS_LOCAL_RANKING_USER_INFO_WRITE_REQ, kPacket);
	g_pMain->AddServerPacket( EGS_LOCAL_RANKING_USER_INFO_WRITE_ACK, 60.f ); 

	return true;
}

bool CX2ProfileManager::Handler_EGS_LOCAL_RANKING_USER_INFO_WRITE_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_LOCAL_RANKING_USER_INFO_WRITE_ACK kEvent;
	DeSerialize( pBuff, &kEvent );

	if( true == g_pMain->DeleteServerPacket( EGS_LOCAL_RANKING_USER_INFO_WRITE_ACK ) )
	{
		if( true == g_pMain->IsValidPacket( kEvent.m_iOK ) )
		{
			if( NULL != m_pDlgCharInfo && false == m_bIsProfileSave)
			{
				CKTDGUIStatic* pStatic_Profile = static_cast< CKTDGUIStatic* >( m_pDlgCharInfo->GetControl( L"g_pStatic_Profile" ) );
				CKTDGUIButton* pButton_Profile = static_cast< CKTDGUIButton* >( m_pDlgCharInfo->GetControl( L"Profile" ) );
				if( NULL != pStatic_Profile && NULL != pButton_Profile )
				{
					pStatic_Profile->SetString( 0, GET_STRING( STR_ID_23729 ) );
					pButton_Profile->SetGuideDesc( L"" );
				}

				m_bIsProfileSave = true;
			}
			return true;
		}
	}

	return false;
}

/** @function : UICustomEventProc
	@brief : ui 버튼 이벤트 처리
	@param : -
	@return : 해당 버튼 이벤트가 있으면 처리 후 true 반환, 없으면 false 반환
*/
bool CX2ProfileManager::UICustomEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	switch(wParam)
	{			
	case PMUM_CLOSE:
		{
			//if( false == m_bIsProfileSave )
				CloseProfileInput();
			//else
				CloseProfile();
		}
		return true;

	case PMUM_LOCATION1:
		{
			SetLocationInfo();
		}
		return true;

	case PMUM_SAVE:
		{
			g_pMain->KTDGUIOkAndCancelMsgBox( D3DXVECTOR2(250, 300), GET_STRING( STR_ID_23727 ), PMUM_SAVE_OK, g_pMain->GetNowState() );
		}
		return true;

	case PMUM_CANCEL:
		{
			CloseProfileInput();
		}
		return true;

	case PMUM_PROFILE_CHANGE:
		{
#ifdef SERV_NAVER_CHANNELING
			if ( NULL != g_pData && NULL != g_pData->GetMyUser() )
			{
				switch ( g_pData->GetMyUser()->GetUserData().m_uChannelCode )
				{
				case KNexonAccountInfo::CE_NEXON_ACCOUNT:
					Create_Password();
					break;

				default:
					{
						if( NULL != g_pData->GetProfileManager() )
						{
							m_bIsOpenSelect = true;
							g_pData->GetProfileManager()->Handler_EGS_LOCAL_RANKING_USER_INFO_READ_REQ( L"" );
						}
					} break;
				}
			}					
#else // SERV_NAVER_CHANNELING
			Create_Password();
#endif // SERV_NAVER_CHANNELING
		}
		return true;

	case PMUM_PASSWORD_OK:
		{
			CKTDGUIEditBox* pEditBox = (CKTDGUIEditBox*) m_pDlgPassword->GetControl( L"EditBoxMessage" );
			if(pEditBox != NULL)
			{
				wstring password = pEditBox->GetText();

				if( 0 == StrCmp( L"", password.c_str() ) )
				{
					return true;
				}
				else
				{
					ClosePassword();

					if( NULL != g_pData->GetProfileManager() )
					{
						m_bIsOpenSelect = true;
						g_pData->GetProfileManager()->Handler_EGS_LOCAL_RANKING_USER_INFO_READ_REQ( password );
					}
				}
			}
		}
		return true;

	case PMUM_PASSWORD_CANCLE:
		{
			if(m_pDlgPassword != NULL)
			{
				g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDlgPassword, NULL, false );
				m_pDlgPassword = NULL;
			}
		}
		return true;

	case PMUM_SAVE_OK:
		{
			CKTDGUIControl* pControl = NULL;
			pControl		= reinterpret_cast< CKTDGUIControl* >( lParam );
			g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, pControl->GetDialog(), NULL, false );

			if( true == SaveInformation() )
			{
				Handler_EGS_LOCAL_RANKING_USER_INFO_WRITE_REQ();
				CloseProfileInput();
			}
		}
		return true;
	}

	return false;
}


/** @function : OpenProfile
	@brief : 프로필 출력 UI 여는 함수
	@param : -
	@return : -
*/
void CX2ProfileManager::OpenProfile( bool _changebutton )
{
	CreateProfileUI();

	if( NULL != m_pDlgProfile )
	{
		CKTDGUIButton* pButton = static_cast<CKTDGUIButton* >( m_pDlgProfile->GetControl( L"button_change" ) );
		if( NULL != pButton )
			pButton->SetShowEnable( _changebutton, _changebutton );

		m_bShow = true;

		g_pKTDXApp->GetDGManager()->GetDialogManager()->ChangeSequence( m_pDlgProfile, false );
		m_pDlgProfile->SetShowEnable( true, true );
		m_pDlgProfile->SetPos( D3DXVECTOR2(350.f, 100.f));

		if( NULL == g_pData )
			return;
		

		//닉네임 출력
		CKTDGUIStatic* pStatic = static_cast<CKTDGUIStatic*>( m_pDlgProfile->GetControl(L"NickName"));
		if( NULL != pStatic )
		{
			pStatic->GetString(0)->msg = m_sProfile.nickname;
		}
		
		//레벨 출력
		pStatic = static_cast<CKTDGUIStatic*>( m_pDlgProfile->GetControl(L"Level"));
		if( NULL != pStatic )
		{
			WCHAR buff[256] = L"";
			StringCchPrintfW( buff, ARRAY_SIZE(buff), L"%d ", m_sProfile.level );
			pStatic->GetString(0)->msg = buff;
		}

		//자기소개글 출력
		pStatic = static_cast<CKTDGUIStatic*>( m_pDlgProfile->GetControl(L"Introduce"));
		if( NULL != pStatic )
		{
			pStatic->GetString(0)->msg = m_sProfile.introduce;
		}

		//나이 출력
		pStatic = static_cast<CKTDGUIStatic*>( m_pDlgProfile->GetControl(L"Age"));
		if( NULL != pStatic )
		{
			WCHAR buff[256] = L"";

			if( 1 > m_sProfile.birthYear )
				pStatic->GetString(0)->msg = buff;
			else
			{
				__time64_t age = MAXYEAR - m_sProfile.birthYear + 1;
				pStatic->GetString(0)->msg = GET_REPLACED_STRING( ( STR_ID_537, "i", age ) );
			}	
		}

		//지역 정보 출력
		pStatic = static_cast<CKTDGUIStatic*>( m_pDlgProfile->GetControl(L"Area"));
		if( NULL != pStatic && SEnum::LRLC_NONE != m_sProfile.area )
		{
			int index = m_sProfile.area + 1;
			pStatic->GetString(0)->msg = m_vecArea[index].UpperArea + L"  " + m_vecArea[index].UnderArea;
		}

		pStatic = static_cast<CKTDGUIStatic*>( m_pDlgProfile->GetControl(L"Profile"));
		if( NULL != pStatic )
		{
			//성별 이미지 출력
			CKTDGUIControl::CPictureData* pPicture = pStatic->GetPicture(9);
			if( NULL != pPicture )
			{
				switch(m_sProfile.gender)
				{
				case PMG_NONE:
					pPicture->SetShow( false );
					break;
				case PMG_MALE:
					pPicture->SetTex( L"DLG_UI_Button20_NEW.tga", L"Bt_Male_Over" );
					pPicture->SetShow( true );
					break;
				case PMG_FEMALE:
					pPicture->SetTex( L"DLG_UI_Button20_NEW.tga", L"Bt_FeMale_Over" );
					pPicture->SetShow( true );
					break;
				}
			}

			//케릭터 초상화 출력
			pPicture = pStatic->GetPicture(7);
			if( NULL != pPicture )
			{
				wstring fileName;
				wstring pieceName;
				
				if( true == CX2Data::GetCharacterImageName( fileName, pieceName,static_cast<CX2Unit::UNIT_CLASS>(m_sProfile.UnitClass), CX2Data::CIT_50by50 ))
				{
					pPicture->SetTex( fileName.c_str(), pieceName.c_str() );
				}
			}
		}
	}
}


/** @function : CloseProfile
	@brief : 프로필 출력 UI 닫는 함수
	@param : -
	@return : -
*/
void CX2ProfileManager::CloseProfile()
{
	if( NULL != m_pDlgProfile )
	{
		m_bShow = false;
		m_pDlgProfile->SetShowEnable(false, false);

		g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDlgProfile, NULL, false );      
		m_pDlgProfile = NULL;
	}
}

void CX2ProfileManager::ClosePassword()
{
	if(m_pDlgPassword != NULL)
	{
		g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDlgPassword, NULL, false );
		m_pDlgPassword = NULL;
	}
}
/** @function : OpenProfileInput
	@brief : 프로필 입력 UI 여는 함수
	@param : -
	@return : -
*/
void CX2ProfileManager::OpenProfileInput()
{
	CreateProfileInputUI();

	
	if( NULL != m_pDlgProfileInput )
	{
		if( -1 != m_sProfile.birthYear)
		{
			CKTDGUIIMEEditBox* pIMEIntroduce = static_cast<CKTDGUIIMEEditBox*>( m_pDlgProfileInput->GetControl( L"IMEIntroduce") );
			if( NULL != pIMEIntroduce )
				pIMEIntroduce->SetText( m_sProfile.introduce.c_str() );

			CKTDGUIComboBox* pComboBox = static_cast<CKTDGUIComboBox*>( m_pDlgProfileInput->GetControl( L"Year_choice1" ) );
			if( NULL != pComboBox )
			{
				pComboBox->SetSelectedByIndex( static_cast<UINT>( MAXYEAR - m_sProfile.birthYear + 1));
			}

			CKTDGUIRadioButton* pRadioMale = static_cast<CKTDGUIRadioButton*>( m_pDlgProfileInput->GetControl( L"male" ) );
			CKTDGUIRadioButton* pRadioFemale = static_cast<CKTDGUIRadioButton*>( m_pDlgProfileInput->GetControl( L"female" ) );
			if( NULL != pRadioFemale && NULL != pRadioMale )
			{
				switch(m_sProfile.gender)
				{
				case PMG_MALE:
					pRadioMale->SetChecked( true );
					pRadioFemale->SetChecked( false );
					break;

				case PMG_FEMALE:
					pRadioMale->SetChecked( false );
					pRadioFemale->SetChecked( true );
					break;

				default:
					pRadioMale->SetChecked( true );
					pRadioFemale->SetChecked( false );
					break;
				}
			}

			CKTDGUIComboBox* pComboBoxUpperArea = static_cast<CKTDGUIComboBox*>( m_pDlgProfileInput->GetControl( L"Location_choice1" ) );
			CKTDGUIComboBox* pComboBoxUnderArea = static_cast<CKTDGUIComboBox*>( m_pDlgProfileInput->GetControl( L"Location_choice2" ) );

		
			if( NULL == pComboBoxUpperArea || NULL == pComboBoxUnderArea )
				return;

			pComboBoxUnderArea->RemoveAllItems();
			pComboBoxUnderArea->AddItem( m_vecArea[0].UnderArea.c_str(), NULL, false );

			CKTDGUIComboBox::ComboBoxItem* UpperAreaName = NULL;

			if( m_sProfile.area <= SEnum::LRLC_SEOUL_JUNGNANG )
			{
				pComboBoxUpperArea->SetSelectedByIndex( PMA_SEOUL );
				UpperAreaName = pComboBoxUpperArea->GetSelectedItem();
				if( NULL == UpperAreaName)
					return;
				for(vector<wstring>::size_type i = 1;i < m_vecArea.size(); ++i)
				{
					if( 0 == StrCmp( UpperAreaName->strText, m_vecArea[i].UpperArea.c_str() ) )
						if( 0 != StrCmp( m_vecArea[i].UnderArea.c_str(), L"" ) )
							pComboBoxUnderArea->AddItem( m_vecArea[i].UnderArea.c_str(), NULL, false );
				}

				pComboBoxUnderArea->SetSelectedByIndex( m_sProfile.area + 1 );
			}
			else if( SEnum::LRLC_BUSAN_GANGSEO <= m_sProfile.area && m_sProfile.area <= SEnum::LRLC_BUSAN_HAEUNDAE || m_sProfile.area == SEnum::LRLC_BUSAN )
			{
				pComboBoxUpperArea->SetSelectedByIndex( PMA_BUSAN );
				UpperAreaName = pComboBoxUpperArea->GetSelectedItem();
				if( NULL == UpperAreaName)
					return;
				for(vector<wstring>::size_type i = 1;i < m_vecArea.size(); ++i)
				{
					if( 0 == StrCmp( UpperAreaName->strText, m_vecArea[i].UpperArea.c_str() ) )
						if( 0 != StrCmp( m_vecArea[i].UnderArea.c_str(), L"" ) )
							pComboBoxUnderArea->AddItem( m_vecArea[i].UnderArea.c_str(), NULL, false );
				}

				pComboBoxUnderArea->SetSelectedByIndex( m_sProfile.area - BUSAN_OFFSET + 1);
			}
			else if( SEnum::LRLC_DAEGU_NAM <= m_sProfile.area && m_sProfile.area <= SEnum::LRLC_DAEGU_JUNG || m_sProfile.area == SEnum::LRLC_DAEGU )
			{
				pComboBoxUpperArea->SetSelectedByIndex( PMA_DAEGU );
				UpperAreaName = pComboBoxUpperArea->GetSelectedItem();
				if( NULL == UpperAreaName)
					return;
				for(vector<wstring>::size_type i = 1;i < m_vecArea.size(); ++i)
				{
					if( 0 == StrCmp( UpperAreaName->strText, m_vecArea[i].UpperArea.c_str() ) )
						if( 0 != StrCmp( m_vecArea[i].UnderArea.c_str(), L"" ) )
							pComboBoxUnderArea->AddItem( m_vecArea[i].UnderArea.c_str(), NULL, false );
				}

				pComboBoxUnderArea->SetSelectedByIndex( m_sProfile.area - DAEGU_OFFSET + 1);
			}
			else if( SEnum::LRLC_INCHEON_GANGHWA <= m_sProfile.area && m_sProfile.area <= SEnum::LRLC_INCHEON_JUNG || m_sProfile.area == SEnum::LRLC_INCHEON )
			{
				pComboBoxUpperArea->SetSelectedByIndex( PMA_INCHEON );
				UpperAreaName = pComboBoxUpperArea->GetSelectedItem();
				if( NULL == UpperAreaName)
					return;
				for(vector<wstring>::size_type i = 1;i < m_vecArea.size(); ++i)
				{
					if( 0 == StrCmp( UpperAreaName->strText, m_vecArea[i].UpperArea.c_str() ) )
						if( 0 != StrCmp( m_vecArea[i].UnderArea.c_str(), L"" ) )
							pComboBoxUnderArea->AddItem( m_vecArea[i].UnderArea.c_str(), NULL, false );
				}

				pComboBoxUnderArea->SetSelectedByIndex( m_sProfile.area - INCHEON_OFFSET + 1);
			}
			else if( SEnum::LRLC_GWANGJU_GWANGSAN <= m_sProfile.area && m_sProfile.area <= SEnum::LRLC_GWANGJU_SEO || m_sProfile.area == SEnum::LRLC_GWANGJU )
			{
				pComboBoxUpperArea->SetSelectedByIndex( PMA_GWANGJU );
				UpperAreaName = pComboBoxUpperArea->GetSelectedItem();
				if( NULL == UpperAreaName)
					return;
				for(vector<wstring>::size_type i = 1;i < m_vecArea.size(); ++i)
				{
					if( 0 == StrCmp( UpperAreaName->strText, m_vecArea[i].UpperArea.c_str() ) )
						if( 0 != StrCmp( m_vecArea[i].UnderArea.c_str(), L"" ) )
							pComboBoxUnderArea->AddItem( m_vecArea[i].UnderArea.c_str(), NULL, false );
				}

				pComboBoxUnderArea->SetSelectedByIndex( m_sProfile.area - GWANGJU_OFFSET + 1);
			}
			else if( SEnum::LRLC_DAEJEON_DAEDEOK <= m_sProfile.area && m_sProfile.area <= SEnum::LRLC_DAEJEON_JUNG || m_sProfile.area == SEnum::LRLC_DAEJEON )
			{
				pComboBoxUpperArea->SetSelectedByIndex( PMA_DAEJEON );
				UpperAreaName = pComboBoxUpperArea->GetSelectedItem();
				if( NULL == UpperAreaName)
					return;
				for(vector<wstring>::size_type i = 1;i < m_vecArea.size(); ++i)
				{
					if( 0 == StrCmp( UpperAreaName->strText, m_vecArea[i].UpperArea.c_str() ) )
						if( 0 != StrCmp( m_vecArea[i].UnderArea.c_str(), L"" ) )
							pComboBoxUnderArea->AddItem( m_vecArea[i].UnderArea.c_str(), NULL, false );
				}

				pComboBoxUnderArea->SetSelectedByIndex( m_sProfile.area - DAEJEON_OFFSET + 1);
			}
			else if( SEnum::LRLC_ULSAN_NAM <= m_sProfile.area && m_sProfile.area <= SEnum::LRLC_ULSAN_JUNG || m_sProfile.area == SEnum::LRLC_ULSAN )
			{
				pComboBoxUpperArea->SetSelectedByIndex( PMA_ULSAN );
				UpperAreaName = pComboBoxUpperArea->GetSelectedItem();
				if( NULL == UpperAreaName)
					return;
				for(vector<wstring>::size_type i = 1;i < m_vecArea.size(); ++i)
				{
					if( 0 == StrCmp( UpperAreaName->strText, m_vecArea[i].UpperArea.c_str() ) )
						if( 0 != StrCmp( m_vecArea[i].UnderArea.c_str(), L"" ) )
							pComboBoxUnderArea->AddItem( m_vecArea[i].UnderArea.c_str(), NULL, false );
				}

				pComboBoxUnderArea->SetSelectedByIndex( m_sProfile.area - ULSAN_OFFSET + 1);
			}
			else if( SEnum::LRLC_GYEONGGIDO_GAPYEONG <= m_sProfile.area && m_sProfile.area <= SEnum::LRLC_GYEONGGIDO_HWASEONG || m_sProfile.area == SEnum::LRLC_GYEONGGIDO)
			{
				pComboBoxUpperArea->SetSelectedByIndex( PMA_GYEONGGIDO );
				UpperAreaName = pComboBoxUpperArea->GetSelectedItem();
				if( NULL == UpperAreaName)
					return;
				for(vector<wstring>::size_type i = 1;i < m_vecArea.size(); ++i)
				{
					if( 0 == StrCmp( UpperAreaName->strText, m_vecArea[i].UpperArea.c_str() ) )
						if( 0 != StrCmp( m_vecArea[i].UnderArea.c_str(), L"" ) )
							pComboBoxUnderArea->AddItem( m_vecArea[i].UnderArea.c_str(), NULL, false );
				}

				pComboBoxUnderArea->SetSelectedByIndex( m_sProfile.area - GYEONGGIDO_OFFSET + 1);
			}
			else if( SEnum::LRLC_GANGWONDO_GANGNEUNG <= m_sProfile.area && m_sProfile.area <= SEnum::LRLC_GANGWONDO_HOENGSEONG || m_sProfile.area == SEnum::LRLC_GANGWONDO )
			{
				pComboBoxUpperArea->SetSelectedByIndex( PMA_GANGWONDO );
				UpperAreaName = pComboBoxUpperArea->GetSelectedItem();
				if( NULL == UpperAreaName)
					return;
				for(vector<wstring>::size_type i = 1;i < m_vecArea.size(); ++i)
				{
					if( 0 == StrCmp( UpperAreaName->strText, m_vecArea[i].UpperArea.c_str() ) )
						if( 0 != StrCmp( m_vecArea[i].UnderArea.c_str(), L"" ) )
							pComboBoxUnderArea->AddItem( m_vecArea[i].UnderArea.c_str(), NULL, false );
				}

				pComboBoxUnderArea->SetSelectedByIndex( m_sProfile.area - GANGWONDO_OFFSET + 1);
			}
			else if( SEnum::LRLC_CHUNGBUK_GOESAN <= m_sProfile.area && m_sProfile.area <= SEnum::LRLC_CHUNGBUK_CHUNGJU || m_sProfile.area == SEnum::LRLC_CHUNGBUK )
			{
				pComboBoxUpperArea->SetSelectedByIndex( PMA_CHUNGBUK );
				UpperAreaName = pComboBoxUpperArea->GetSelectedItem();
				if( NULL == UpperAreaName)
					return;
				for(vector<wstring>::size_type i = 1;i < m_vecArea.size(); ++i)
				{
					if( 0 == StrCmp( UpperAreaName->strText, m_vecArea[i].UpperArea.c_str() ) )
						if( 0 != StrCmp( m_vecArea[i].UnderArea.c_str(), L"" ) )
							pComboBoxUnderArea->AddItem( m_vecArea[i].UnderArea.c_str(), NULL, false );
				}

				pComboBoxUnderArea->SetSelectedByIndex( m_sProfile.area - CHUNGBUK_OFFSET + 1);
			}
			else if( SEnum::LRLC_CHUNGNAM_GYERYONG <= m_sProfile.area && m_sProfile.area <= SEnum::LRLC_CHUNGNAM_HONGSEONG || m_sProfile.area == SEnum::LRLC_CHUNGNAM )
			{
				pComboBoxUpperArea->SetSelectedByIndex( PMA_CHUNGNAM );
				UpperAreaName = pComboBoxUpperArea->GetSelectedItem();
				if( NULL == UpperAreaName)
					return;
				for(vector<wstring>::size_type i = 1;i < m_vecArea.size(); ++i)
				{
					if( 0 == StrCmp( UpperAreaName->strText, m_vecArea[i].UpperArea.c_str() ) )
						if( 0 != StrCmp( m_vecArea[i].UnderArea.c_str(), L"" ) )
							pComboBoxUnderArea->AddItem( m_vecArea[i].UnderArea.c_str(), NULL, false );
				}

				pComboBoxUnderArea->SetSelectedByIndex( m_sProfile.area - CHUNGNAM_OFFSET + 1);
			}
			else if( SEnum::LRLC_JEONBUK_GOCHANG <= m_sProfile.area && m_sProfile.area <= SEnum::LRLC_JEONBUK_JINAN || m_sProfile.area == SEnum::LRLC_JEONBUK )
			{
				pComboBoxUpperArea->SetSelectedByIndex( PMA_JEONBUK );
				UpperAreaName = pComboBoxUpperArea->GetSelectedItem();
				if( NULL == UpperAreaName)
					return;
				for(vector<wstring>::size_type i = 1;i < m_vecArea.size(); ++i)
				{
					if( 0 == StrCmp( UpperAreaName->strText, m_vecArea[i].UpperArea.c_str() ) )
						if( 0 != StrCmp( m_vecArea[i].UnderArea.c_str(), L"" ) )
							pComboBoxUnderArea->AddItem( m_vecArea[i].UnderArea.c_str(), NULL, false );
				}

				pComboBoxUnderArea->SetSelectedByIndex( m_sProfile.area - JEONBUK_OFFSET + 1);
			}
			else if( SEnum::LRLC_JEONNAM_GANGJIN <= m_sProfile.area && m_sProfile.area <= SEnum::LRLC_JEONNAM_HWASUN || m_sProfile.area == SEnum::LRLC_JEONNAM )
			{
				pComboBoxUpperArea->SetSelectedByIndex( PMA_JEONNAM );
				UpperAreaName = pComboBoxUpperArea->GetSelectedItem();
				if( NULL == UpperAreaName)
					return;
				for(vector<wstring>::size_type i = 1;i < m_vecArea.size(); ++i)
				{
					if( 0 == StrCmp( UpperAreaName->strText, m_vecArea[i].UpperArea.c_str() ) )
						if( 0 != StrCmp( m_vecArea[i].UnderArea.c_str(), L"" ) )
							pComboBoxUnderArea->AddItem( m_vecArea[i].UnderArea.c_str(), NULL, false );
				}

				pComboBoxUnderArea->SetSelectedByIndex( m_sProfile.area - JEONNAM_OFFSET + 1);
			}
			else if( SEnum::LRLC_GYEONGBUK_GYEONGSAN <= m_sProfile.area && m_sProfile.area <= SEnum::LRLC_GYEONGBUK_POHANG || m_sProfile.area == SEnum::LRLC_GYEONGBUK )
			{
				pComboBoxUpperArea->SetSelectedByIndex( PMA_GYEONGBUK );
				UpperAreaName = pComboBoxUpperArea->GetSelectedItem();
				if( NULL == UpperAreaName)
					return;
				for(vector<wstring>::size_type i = 1;i < m_vecArea.size(); ++i)
				{
					if( 0 == StrCmp( UpperAreaName->strText, m_vecArea[i].UpperArea.c_str() ) )
						if( 0 != StrCmp( m_vecArea[i].UnderArea.c_str(), L"" ) )
							pComboBoxUnderArea->AddItem( m_vecArea[i].UnderArea.c_str(), NULL, false );
				}

				pComboBoxUnderArea->SetSelectedByIndex( m_sProfile.area - GYEONGBUK_OFFSET + 1);
			}
			else if( SEnum::LRLC_GYEONGNAM_GEOJE <= m_sProfile.area && m_sProfile.area <= SEnum::LRLC_GYEONGNAM_HAPCHEON || m_sProfile.area == SEnum::LRLC_GYEONGNAM )
			{
				pComboBoxUpperArea->SetSelectedByIndex( PMA_GYEONGNAM );
				UpperAreaName = pComboBoxUpperArea->GetSelectedItem();
				if( NULL == UpperAreaName)
					return;
				for(vector<wstring>::size_type i = 1;i < m_vecArea.size(); ++i)
				{
					if( 0 == StrCmp( UpperAreaName->strText, m_vecArea[i].UpperArea.c_str() ) )
						if( 0 != StrCmp( m_vecArea[i].UnderArea.c_str(), L"" ) )
							pComboBoxUnderArea->AddItem( m_vecArea[i].UnderArea.c_str(), NULL, false );
				}

				pComboBoxUnderArea->SetSelectedByIndex( m_sProfile.area - GYEONGNAM_OFFSET + 1);
			}
			else if( SEnum::LRLC_JEJU_JEJU <= m_sProfile.area && m_sProfile.area <= SEnum::LRLC_JEJU_SEOGWIPO || m_sProfile.area == SEnum::LRLC_JEJU)
			{
				pComboBoxUpperArea->SetSelectedByIndex( PMA_JEJU );
				UpperAreaName = pComboBoxUpperArea->GetSelectedItem();
				if( NULL == UpperAreaName)
					return;
				for(vector<wstring>::size_type i = 1;i < m_vecArea.size(); ++i)
				{
					if( 0 == StrCmp( UpperAreaName->strText, m_vecArea[i].UpperArea.c_str() ) )
						if( 0 != StrCmp( m_vecArea[i].UnderArea.c_str(), L"" ) )
							pComboBoxUnderArea->AddItem( m_vecArea[i].UnderArea.c_str(), NULL, false );
				}

				pComboBoxUnderArea->SetSelectedByIndex( m_sProfile.area - JEJU_OFFSET + 1);
			}
			else if( m_sProfile.area == SEnum::LRLC_SEJONG )
			{
				pComboBoxUpperArea->SetSelectedByIndex( PMA_SEJONG );
				UpperAreaName = pComboBoxUpperArea->GetSelectedItem();
				if( NULL == UpperAreaName)
					return;
				for(vector<wstring>::size_type i = 1;i < m_vecArea.size(); ++i)
				{
					if( 0 == StrCmp( UpperAreaName->strText, m_vecArea[i].UpperArea.c_str() ) )
						if( 0 != StrCmp( m_vecArea[i].UnderArea.c_str(), L"" ) )
							pComboBoxUnderArea->AddItem( m_vecArea[i].UnderArea.c_str(), NULL, false );
				}

				pComboBoxUnderArea->SetSelectedByIndex( 0 );
			}
			else
				return;


			pComboBoxUnderArea->SetShowEnable( true, true );



			//필터 버튼
			//정보 공개 여부 저장
			CKTDGUICheckBox* pCheckBox = static_cast<CKTDGUICheckBox*>( m_pDlgProfileInput->GetControl( L"secretCheck1" ) );
			if( NULL != pCheckBox )
			{
				if( true == SEnum::CheckFlag( m_sProfile.filter, SEnum::LRF_USER_INFO_PROFILE ) )
					pCheckBox->SetChecked( true );
			}

			pCheckBox = static_cast<CKTDGUICheckBox*>( m_pDlgProfileInput->GetControl( L"secretCheck2" ) );
			if( NULL != pCheckBox )
			{
				if( true == SEnum::CheckFlag( m_sProfile.filter, SEnum::LRF_USER_INFO_BIRTH ) )
					pCheckBox->SetChecked( true );
			}

			pCheckBox = static_cast<CKTDGUICheckBox*>( m_pDlgProfileInput->GetControl( L"secretCheck3" ) );
			if( NULL != pCheckBox )
			{
				if( true == SEnum::CheckFlag( m_sProfile.filter, SEnum::LRF_USER_INFO_CATEGORY ) )
					pCheckBox->SetChecked( true );
			}

			pCheckBox = static_cast<CKTDGUICheckBox*>( m_pDlgProfileInput->GetControl( L"secretCheck4" ) );
			if( NULL != pCheckBox )
			{
				if( true == SEnum::CheckFlag( m_sProfile.filter, SEnum::LRF_USER_INFO_GENDER ) )
					pCheckBox->SetChecked( true );
			}
		}

		m_bShow = true;
		
		g_pKTDXApp->GetDGManager()->GetDialogManager()->ChangeSequence( m_pDlgProfileInput, false );
		m_pDlgProfileInput->SetShowEnable( true, true );
		m_pDlgProfileInput->SetPos( D3DXVECTOR2(350.f, 100.f));
	}	
}

/** @function : CloseProfileInput
	@brief : 프로필 입력 UI 닫는 함수
	@param : -
	@return : -
*/
void CX2ProfileManager::CloseProfileInput()
{
	if( NULL != m_pDlgProfileInput )
	{
		m_bShow = false;
		m_pDlgProfileInput->SetShowEnable(false, false);        

		g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDlgProfileInput, NULL, false );      
		m_pDlgProfileInput = NULL;
	}
}

/** @function : OpenRankup
	@brief : 실시간 피드백 UI 출력하는 함수
	@param : -
	@return : -
*/
void CX2ProfileManager::OpenRankup( KEGS_LOCAL_RANKING_RANK_UP_NOT& kEvent )
{
	const CX2Unit::UnitData* pUnitData = NULL;
	if( NULL != g_pData->GetMyUser() && NULL != g_pData->GetMyUser()->GetSelectUnit())
		pUnitData = &g_pData->GetMyUser()->GetSelectUnit()->GetUnitData();
	if( NULL == pUnitData )
		return;
	
	CreateRankUpUI();

	if( NULL != m_pDlgRankUp )
	{
		m_bShowRankup = true;

		g_pKTDXApp->GetDGManager()->GetDialogManager()->ChangeSequence( m_pDlgRankUp, false );
		m_pDlgRankUp->SetShowEnable( true, true );
		m_pDlgRankUp->Move( m_pDlgRankUp->GetPos(), D3DXCOLOR( 1.f, 1.f, 1.f, 0.f ), 0.5f, true );
		m_fRankupFadeTime = RANKUP_UI_PLAY_TIME;


		//자신 닉네임 출력
		CKTDGUIStatic* pStatic = static_cast<CKTDGUIStatic*>( m_pDlgRankUp->GetControl(L"id1"));
		if( NULL != pStatic )
		{
			pStatic->GetString(0)->msg = pUnitData->m_NickName;
		}
		//상대방 닉네임 출력
		if( 0 != StrCmp( pUnitData->m_NickName.c_str(), kEvent.m_kNextUnitInfo.m_wstrNickName.c_str() ) )
		{
			pStatic = static_cast<CKTDGUIStatic*>( m_pDlgRankUp->GetControl(L"id2"));
			if( NULL != pStatic )
			{
				pStatic->GetString(0)->msg = kEvent.m_kNextUnitInfo.m_wstrNickName;
			}
		}

		//순위 출력
		pStatic = static_cast<CKTDGUIStatic*>( m_pDlgRankUp->GetControl(L"Rankup"));
		if( NULL != pStatic )
		{
			SetRankTexture( pStatic, kEvent.m_iMyRank );
		}

		pStatic = static_cast<CKTDGUIStatic*>( m_pDlgRankUp->GetControl(L"Info"));
		if( NULL != pStatic )
		{
			//자신 초상화
			CKTDGUIControl::CPictureData* pPicture = pStatic->GetPicture(0);
			if( NULL != pPicture )
			{
				wstring fileName;
				wstring pieceName;

				if( true == CX2Data::GetCharacterImageName( fileName, pieceName, pUnitData->m_UnitClass, CX2Data::CIT_50by50 ))
				{
					pPicture->SetTex( fileName.c_str(), pieceName.c_str() );
				}
			}

			//순위가 바뀐 상대방 초상화
			pPicture = pStatic->GetPicture(3);
			if( NULL != pPicture )
			{
				if( CX2Unit::UC_NONE == static_cast<CX2Unit::UNIT_CLASS>( kEvent.m_kNextUnitInfo.m_cUnitClass ) )
					pPicture->SetShow( false );
				else if( 0 == StrCmp( pUnitData->m_NickName.c_str(), kEvent.m_kNextUnitInfo.m_wstrNickName.c_str() ) )
					pPicture->SetShow( false );
				else
				{
					wstring fileName;
					wstring pieceName;

					if( true == CX2Data::GetCharacterImageName( fileName, pieceName, static_cast<CX2Unit::UNIT_CLASS>( kEvent.m_kNextUnitInfo.m_cUnitClass ), CX2Data::CIT_50by50 ))
					{
						pPicture->SetTex( fileName.c_str(), pieceName.c_str() );
						pPicture->SetShow( true );
					}
				}
			}

			//자신 던전&필드 또는 대전 텍스처
			pPicture = pStatic->GetPicture(1);
			if( NULL != pPicture )
			{
				SetAreaCategoryTexture( pPicture, kEvent.m_cMainTabIndex, 0 );
			}

			//자신 지역 또는 길드 텍스처
			pPicture = pStatic->GetPicture(2);
			if( NULL != pPicture )
			{
				SetAreaCategoryTexture( pPicture, kEvent.m_cSubTabIndex, 1 );
			}

			//상대방 던전&필드 또는 대전 텍스처
			pPicture = pStatic->GetPicture(4);
			if( NULL != pPicture )
			{
				SetAreaCategoryTexture( pPicture, kEvent.m_cMainTabIndex, 0 );
			}


			//상대방 지역 또는 길드 텍스처
			pPicture = pStatic->GetPicture(5);
			if( NULL != pPicture )
			{
				SetAreaCategoryTexture( pPicture, kEvent.m_cSubTabIndex, 1 );
			}


			//근성도 텍스처 세팅
			CKTDGUIStatic* pStaticTexture = static_cast<CKTDGUIStatic*>( m_pDlgRankUp->GetControl( L"subtitle" ) );
			int point1,point2;
			switch( kEvent.m_cMainTabIndex )
			{
			case SEnum::LRMTC_AP:
				point1 = kEvent.m_kMyUnitInfo.m_iPoint[SEnum::LRMTC_AP];
				point2 = kEvent.m_kNextUnitInfo.m_iPoint[SEnum::LRMTC_AP];
				if( NULL != pStaticTexture )
					if( NULL != pStaticTexture->GetPicture( 0 ) && NULL != pStaticTexture->GetPicture( 1 ) )
					{
						pStaticTexture->GetPicture( 0 )->SetTex( L"DLG_UI_Title03.TGA", L"Bg_Stitle_AP" );
						pStaticTexture->GetPicture( 1 )->SetTex( L"DLG_UI_Title03.TGA", L"Bg_Stitle_AP" );
					}
				break;

			case SEnum::LRMTC_SPIRIT:
				point1 = kEvent.m_kMyUnitInfo.m_iPoint[SEnum::LRMTC_SPIRIT];
				point2 = kEvent.m_kNextUnitInfo.m_iPoint[SEnum::LRMTC_SPIRIT];
				if( NULL != pStaticTexture )
					if( NULL != pStaticTexture->GetPicture( 0 ) && NULL != pStaticTexture->GetPicture( 1 ) )
					{
						pStaticTexture->GetPicture( 0 )->SetTex( L"DLG_UI_Title03.TGA", L"STitle_SP" );
						pStaticTexture->GetPicture( 1 )->SetTex( L"DLG_UI_Title03.TGA", L"STitle_SP" );
					}
				break;
			}
			
			SetStrongGage( true, point1 );
			if( 0 != StrCmp( pUnitData->m_NickName.c_str(), kEvent.m_kNextUnitInfo.m_wstrNickName.c_str() ) )
				SetStrongGage( false, point2 );

		}
	}
}


/** @function : CloseRankup
	@brief : 실시간 피드백 UI 닫는 함수
	@param : -
	@return : -
*/
void CX2ProfileManager::CloseRankup()
{
	switch( g_pMain->GetNowStateID() )
	{
	case CX2Main::XS_VILLAGE_MAP:
	case CX2Main::XS_BATTLE_FIELD:
	case CX2Main::XS_DUNGEON_RESULT:
	case CX2Main::XS_PVP_RESULT:
		{
			if( 0.f < m_fRankupFadeTime )
				return;
		}
		break;
	}
	

	if( NULL != m_pDlgRankUp )
	{
		m_bShowRankup = false;

		m_pDlgRankUp->SetShowEnable( false, false );
		g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDlgRankUp, NULL, false );
		m_pDlgRankUp = NULL;
		m_fRankupFadeTime = 0.f;

		ClearRankUpPaticle();
	}
}

void CX2ProfileManager::SetAreaCategoryTexture( CKTDGUIControl::CPictureData* _picture, char _category, bool _updown )
{
	const wstring CATEGORY_TEXTURE = L"DLG_UI_Common_Texture_Bg_01.tga";

	const wstring CATEGORY_KEY[] =
	{
		L"BG_RankUp_Duel", L"BG_RankUp_DungeonField", L"BG_RankUp_Guild", L"BG_RankUp_Local"
	};

	if( 0 == _updown )
	{
		switch( _category )
		{
		case SEnum::LRMTC_AP:
			{
				_picture->SetTex( CATEGORY_TEXTURE.c_str(), CATEGORY_KEY[0].c_str() );
				_picture->SetShow( true );
			}
			break;

		case SEnum::LRMTC_SPIRIT:
			{
				_picture->SetTex( CATEGORY_TEXTURE.c_str(), CATEGORY_KEY[1].c_str() );
				_picture->SetShow( true );
			}
			break;

		default:
			{
				_picture->SetShow( false );
			}
			break;
		}
	}
	else if( 1 == _updown )
	{
		switch( _category )
		{
		case SEnum::LRSTC_GUILD:
			{
				_picture->SetTex( CATEGORY_TEXTURE.c_str(), CATEGORY_KEY[2].c_str() );
				_picture->SetShow( true );
			}
			break;

		case SEnum::LRSTC_LOCATION:
			{
				_picture->SetTex( CATEGORY_TEXTURE.c_str(), CATEGORY_KEY[3].c_str() );
				_picture->SetShow( true );
			}
			break;

		default:
			{
				_picture->SetShow( false );
			}
			break;
		}
	}
}

bool CX2ProfileManager::SaveInformation()
{
	CKTDGUIComboBox* pComboBoxUpper = static_cast<CKTDGUIComboBox*>(m_pDlgProfileInput->GetControl(L"Location_choice1"));
	CKTDGUIComboBox* pComboBox = static_cast<CKTDGUIComboBox*>(m_pDlgProfileInput->GetControl(L"Location_choice2"));

	//닉네임, 클래스, 레벨 저장
	const CX2Unit::UnitData* pUnitData = NULL;
	if( NULL != g_pData->GetMyUser() && NULL != g_pData->GetMyUser()->GetSelectUnit())
		pUnitData = &g_pData->GetMyUser()->GetSelectUnit()->GetUnitData();
	if( NULL != pUnitData )
	{
		m_sProfile.nickname		= pUnitData->m_NickName;
		m_sProfile.UnitClass	= pUnitData->m_UnitClass;
		m_sProfile.level		= pUnitData->m_Level;
	}
	
	
	//지역 저장
	if( NULL != pComboBoxUpper && NULL != pComboBox )
	{
		int UpperArea = pComboBoxUpper->GetSelectedItemIndex();
		if( SEnum::LRLC_NONE == ( UpperArea - 1 ) )
		{
			g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250, 300), GET_STRING( STR_ID_23793 ), NULL );
			return false;
		}
		else
		{
			int Area = pComboBox->GetSelectedItemIndex() - 1;
			if( SEnum::LRLC_NONE == Area && PMA_SEJONG != UpperArea ) //구단위까지 선택을 안했을경우, 세종시 예외 하위단계가 없음.
			{
				g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250, 300), GET_STRING( STR_ID_23612 ), NULL );
				return false;
			}

			int Offset = -1;

			switch( UpperArea )
			{
			case PMA_SEOUL:
				{
					Offset = 0;
				}
				break;

			case PMA_BUSAN:
				{
					Offset = BUSAN_OFFSET;
				}
				break;

			case PMA_DAEGU:
				{
					Offset = DAEGU_OFFSET;
				}
				break;

			case PMA_INCHEON:
				{
					Offset = INCHEON_OFFSET;
				}
				break;

			case PMA_GWANGJU:
				{
					Offset = GWANGJU_OFFSET;
				}
				break;

			case PMA_DAEJEON:
				{
					Offset = DAEJEON_OFFSET;
				}
				break;

			case PMA_ULSAN:
				{
					Offset = ULSAN_OFFSET;
				}
				break;

			case PMA_SEJONG:
				{
					Offset = SEnum::LRLC_SEJONG + 1;
				}
				break;

			case PMA_GYEONGGIDO:
				{
					Offset = GYEONGGIDO_OFFSET;
				}
				break;

			case PMA_GANGWONDO:
				{
					Offset = GANGWONDO_OFFSET;
				}
				break;

			case PMA_CHUNGBUK:
				{
					Offset = CHUNGBUK_OFFSET;
				}
				break;

			case PMA_CHUNGNAM:
				{
					Offset = CHUNGNAM_OFFSET;
				}
				break;

			case PMA_JEONBUK:
				{
					Offset = JEONBUK_OFFSET;
				}
				break;

			case PMA_JEONNAM:
				{
					Offset = JEONNAM_OFFSET;
				}
				break;

			case PMA_GYEONGBUK:
				{
					Offset = GYEONGBUK_OFFSET;
				}
				break;

			case PMA_GYEONGNAM:
				{
					Offset = GYEONGNAM_OFFSET;
				}
				break;

			case PMA_JEJU:
				{
					Offset = JEJU_OFFSET;
				}
				break;

			default:
				{
					return false;
				}
				break;
			}
			m_sProfile.area = Area + Offset;
		}
	}

	//자기 소개 저장
	CKTDGUIIMEEditBox* pIMEEditbox = static_cast<CKTDGUIIMEEditBox*>(m_pDlgProfileInput->GetControl(L"IMEIntroduce"));
	if( NULL != pIMEEditbox )
	{
		if( g_pMain->GetStringFilter()->CheckIsValidString( CX2StringFilter::FT_CHAT, pIMEEditbox->GetText() ) == false )
		{	
			g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250, 300), GET_STRING( STR_ID_23799 ), NULL );
			return false;
		}
		m_sProfile.introduce = pIMEEditbox->GetText();
	}

	//태어난 해 저장
	CKTDGUIComboBox* pComboBoxYear = static_cast<CKTDGUIComboBox*>(m_pDlgProfileInput->GetControl(L"Year_choice1"));
	if( NULL != pComboBoxYear )
	{
		int yearPlus = pComboBoxYear->GetSelectedItemIndex();

		if( 0 == yearPlus)
		{
			g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250, 300), GET_STRING( STR_ID_23792 ), NULL );
			return false;
		}
		else
			m_sProfile.birthYear = MAXYEAR - yearPlus + 1;
	}

	//성별 저장
	CKTDGUIRadioButton* pRadioGenderM = static_cast<CKTDGUIRadioButton*>(m_pDlgProfileInput->GetControl(L"male"));
	CKTDGUIRadioButton* pRadioGenderF = static_cast<CKTDGUIRadioButton*>(m_pDlgProfileInput->GetControl(L"female"));
	if( NULL != pRadioGenderM && NULL != pRadioGenderF )
	{
		if( true == pRadioGenderM->GetChecked() )
			m_sProfile.gender = PMG_MALE;
		else if( true == pRadioGenderF->GetChecked() )
			m_sProfile.gender = PMG_FEMALE;
		else
		{
			g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250, 300), GET_STRING( STR_ID_23794 ), NULL );
			return false;
		}
	}

	//정보 공개 여부 저장
	m_sProfile.filter = 0;
	CKTDGUICheckBox* pCheckBox = static_cast<CKTDGUICheckBox*>( m_pDlgProfileInput->GetControl( L"secretCheck1" ) );
	if( NULL != pCheckBox )
	{
		if( true == pCheckBox->GetChecked() )
			SEnum::AddFlag( m_sProfile.filter, SEnum::LRF_USER_INFO_PROFILE );
	}

	pCheckBox = static_cast<CKTDGUICheckBox*>( m_pDlgProfileInput->GetControl( L"secretCheck2" ) );
	if( NULL != pCheckBox )
	{
		if( true == pCheckBox->GetChecked() )
			SEnum::AddFlag( m_sProfile.filter, SEnum::LRF_USER_INFO_BIRTH );
	}

	pCheckBox = static_cast<CKTDGUICheckBox*>( m_pDlgProfileInput->GetControl( L"secretCheck3" ) );
	if( NULL != pCheckBox )
	{
		if( true == pCheckBox->GetChecked() )
			SEnum::AddFlag( m_sProfile.filter, SEnum::LRF_USER_INFO_CATEGORY );
	}

	pCheckBox = static_cast<CKTDGUICheckBox*>( m_pDlgProfileInput->GetControl( L"secretCheck4" ) );
	if( NULL != pCheckBox )
	{
		if( true == pCheckBox->GetChecked() )
			SEnum::AddFlag( m_sProfile.filter, SEnum::LRF_USER_INFO_GENDER );
	}

	return true;
}

void CX2ProfileManager::ShowRankUpFade( float _elapsedtime )
{
	if( NULL != m_pDlgRankUp )
	{
		if( RANKUP_UI_PLAY_TIME == m_fRankupFadeTime)
		{
			if( NULL != g_pData->GetUIMajorParticle() )
			{
				ClearRankUpPaticle();

				m_Seq1 = g_pData->GetUIMajorParticle()->CreateSequence( NULL,  L"UI_Arrow_Rank_Up_DTX5_P03", 512, 70, 0);//, 1000, 1000, 1, 1, 1 );
				if( NULL != m_Seq1 )
					m_Seq1->SetOverUI( true );

				m_Seq2 = g_pData->GetUIMajorParticle()->CreateSequence( NULL,  L"UI_Arrow_Rank_Up_DTX5_P01", 512, 70, 0);//, 1000, 1000, 1, 1, 1 );
				if( NULL != m_Seq2 )
					m_Seq2->SetOverUI( true );

				m_Seq3 = g_pData->GetUIMajorParticle()->CreateSequence( NULL,  L"UI_Arrow_Rank_Up_DTX5_P02", 512, 70, 0);//, 1000, 1000, 1, 1, 1 );
				if( NULL != m_Seq3)
					m_Seq3->SetOverUI( true );
			}
		}

		if( 4.5f < m_fRankupFadeTime )		//Fade In
			m_pDlgRankUp->Move( m_pDlgRankUp->GetPos(), D3DXCOLOR( 1.f, 1.f, 1.f, 1.f ), 0.5f, true );
		else if( 0.5f > m_fRankupFadeTime )	//Fade Out
			m_pDlgRankUp->Move( m_pDlgRankUp->GetPos(), D3DXCOLOR( 1.f, 1.f, 1.f, 0.f ), 0.5f, true );

		m_fRankupFadeTime -= _elapsedtime;
	}
}

void CX2ProfileManager::SetStrongGage( bool _me, int _value )
{
	CKTDGUIStatic* pStaticRankupAll = static_cast<CKTDGUIStatic*>( m_pDlgRankUp->GetControl(L"Point"));
	if( NULL == pStaticRankupAll )
		return;

	if( _value < 0 )
		_value = 0;
	if( _value > 999999 ) //최대 한계 수치 6자리까지
		_value = 999999;

	bool Is_Upper_Zero = false;
	int index = ( true == _me )? 0 : 6;

	const wstring TEXTURE_KEY[] = {	L"Will_0",
		L"Will_1",
		L"Will_2",
		L"Will_3",
		L"Will_4",
		L"Will_5",
		L"Will_6",
		L"Will_7",
		L"Will_8",
		L"Will_9" };

	UINT iStrong100000	= _value / 100000;	_value = _value % 100000;
	UINT iStrong10000	= _value / 10000;	_value = _value % 10000;
	UINT iStrong1000	= _value / 1000;	_value = _value % 1000;
	UINT iStrong100		= _value / 100;		_value = _value % 100;
	UINT iStrong10		= _value / 10;		
	UINT iStrong1		= _value % 10;	

	//십만의 자리
	CKTDGUIControl::CPictureData* pPicture = pStaticRankupAll->GetPicture(index);
	if( NULL != pPicture )
	{
		if( 0 == iStrong100000 )
		{
			Is_Upper_Zero = true;
			pPicture->SetShow( false );
		}
		else
		{
			pPicture->SetTex(L"DLG_UI_Common_Texture_Bg_01.tga", TEXTURE_KEY[iStrong100000].c_str());
			pPicture->SetShow( true );
		}
	}

	// 만의 자리
	index++;
	pPicture = pStaticRankupAll->GetPicture(index);
	if( NULL != pPicture )
	{
		if( 0 == iStrong10000 && true == Is_Upper_Zero )
		{
			pPicture->SetShow( false );
		}
		else
		{
			Is_Upper_Zero = false;
			pPicture->SetTex(L"DLG_UI_Common_Texture_Bg_01.tga", TEXTURE_KEY[iStrong10000].c_str());
			pPicture->SetShow( true );
		}
	}

	// 천의 자리
	index++;
	pPicture = pStaticRankupAll->GetPicture(index);
	if( NULL != pPicture )
	{
		if( 0 == iStrong1000 && true == Is_Upper_Zero )
		{
			pPicture->SetShow( false );
		}
		else
		{
			Is_Upper_Zero = false;
			pPicture->SetTex(L"DLG_UI_Common_Texture_Bg_01.tga", TEXTURE_KEY[iStrong1000].c_str());
			pPicture->SetShow( true );
		}
	}

	// 백의 자리
	index++;
	pPicture = pStaticRankupAll->GetPicture(index);
	if( NULL != pPicture )
	{
		if( 0 == iStrong100 && true == Is_Upper_Zero )
		{
			pPicture->SetShow( false );
		}
		else
		{
			Is_Upper_Zero = false;
			pPicture->SetTex(L"DLG_UI_Common_Texture_Bg_01.tga", TEXTURE_KEY[iStrong100].c_str());
			pPicture->SetShow( true );
		}
	}

	// 십의 자리
	index++;
	pPicture = pStaticRankupAll->GetPicture(index);
	if( NULL != pPicture )
	{
		if( 0 == iStrong10 && true == Is_Upper_Zero )
		{
			pPicture->SetShow( false );
		}
		else
		{
			Is_Upper_Zero = false;
			pPicture->SetTex(L"DLG_UI_Common_Texture_Bg_01.tga", TEXTURE_KEY[iStrong10].c_str());
			pPicture->SetShow( true );
		}
	}

	// 일의 자리
	index++;
	pPicture = pStaticRankupAll->GetPicture(index);
	if( NULL != pPicture )
	{
		pPicture->SetTex(L"DLG_UI_Common_Texture_Bg_01.tga", TEXTURE_KEY[iStrong1].c_str());
		pPicture->SetShow( true );
	}
}

void CX2ProfileManager::CloseAll()
{
	CloseProfileInput();
	CloseProfile();
	CloseRankup();
	ClosePassword();
}

void CX2ProfileManager::ClearRankUpPaticle()
{
	if( NULL != m_Seq1 )
	{
		m_Seq1->SetAutoDie();
		m_Seq1->ClearAllParticle();
		m_Seq1 = NULL;
	}

	if( NULL != m_Seq2 )
	{
		m_Seq2->SetAutoDie();
		m_Seq2->ClearAllParticle();
		m_Seq2 = NULL;
	}

	if( NULL != m_Seq3 )
	{
		m_Seq3->SetAutoDie();
		m_Seq3->ClearAllParticle();
		m_Seq3 = NULL;
	}
}

void CX2ProfileManager::SetProfile( const Profile* _profile )
{
	m_sProfile.area			= _profile->area;
	m_sProfile.birthYear	= _profile->birthYear;
	m_sProfile.gender		= _profile->gender;
	m_sProfile.introduce	= _profile->introduce;
	m_sProfile.level		= _profile->level;
	m_sProfile.nickname		= _profile->nickname;
	m_sProfile.UnitClass	= _profile->UnitClass;
}

void CX2ProfileManager::SetRankTexture( CKTDGUIStatic* _static, int _rank )
{
	if( NULL == _static )
		return;

	if( _rank < 1)
		_rank = 1;
	else if( _rank > 1000 )
		_rank = 1000;

	const wstring TEXTURE_KEY[] = {	L"Bg_RankUp_Ranking_0",
		L"Bg_RankUp_Ranking_1",
		L"Bg_RankUp_Ranking_2",
		L"Bg_RankUp_Ranking_3",
		L"Bg_RankUp_Ranking_4",
		L"Bg_RankUp_Ranking_5",
		L"Bg_RankUp_Ranking_6",
		L"Bg_RankUp_Ranking_7",
		L"Bg_RankUp_Ranking_8",
		L"Bg_RankUp_Ranking_9" };

	UINT iRank1000		= _rank / 1000;		_rank = _rank % 1000;
	UINT iRank100		= _rank / 100;		_rank = _rank % 100;
	UINT iRank10		= _rank / 10;		
	UINT iRank1			= _rank % 10;	

	bool Is_Upper_Zero = false;

	//천의 자리
	CKTDGUIControl::CPictureData* pPicture = _static->GetPicture(5);
	if( NULL != pPicture )
	{
		if( 0 == iRank1000 )
		{
			Is_Upper_Zero = true;
			pPicture->SetShow( false );
		}
		else
		{
			pPicture->SetTex( L"DLG_UI_Common_Texture_Bg_01.tga", TEXTURE_KEY[iRank1000].c_str() );
			pPicture->SetShow( true );
		}
	}

	//백의 자리
	pPicture = _static->GetPicture(4);
	if( NULL != pPicture )
	{
		if( 0 == iRank100 && true == Is_Upper_Zero)
		{
			pPicture->SetShow( false );
		}
		else
		{
			Is_Upper_Zero = false;
			pPicture->SetTex( L"DLG_UI_Common_Texture_Bg_01.tga", TEXTURE_KEY[iRank100].c_str() );
			pPicture->SetShow( true );
		}
	}

	//십의 자리
	pPicture = _static->GetPicture(3);
	if( NULL != pPicture )
	{
		if( 0 == iRank10 && true == Is_Upper_Zero )
		{
			pPicture->SetShow( false );
		}
		else
		{
			Is_Upper_Zero = false;
			pPicture->SetTex( L"DLG_UI_Common_Texture_Bg_01.tga", TEXTURE_KEY[iRank10].c_str() );
			pPicture->SetShow( true );
		}
	}

	//일의 자리
	pPicture = _static->GetPicture(2);
	if( NULL != pPicture )
	{
		pPicture->SetTex( L"DLG_UI_Common_Texture_Bg_01.tga", TEXTURE_KEY[iRank1].c_str() );
		pPicture->SetShow( true );
	}
}

void CX2ProfileManager::Create_Password()
{
	if(m_pDlgPassword != NULL)
	{
		g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDlgPassword, NULL, false );
		m_pDlgPassword = NULL;
	}

	m_pDlgPassword = g_pMain->KTDGUIOkAndCancelEditBox_Hide( D3DXVECTOR2( 300, 300), GET_STRING( STR_ID_12537 ), PMUM_PASSWORD_OK, g_pMain->GetNowState(),16,false, PMUM_PASSWORD_CANCLE );
	if( NULL == m_pDlgPassword )
		return;

	CKTDGUIStatic* pStatic = static_cast<CKTDGUIStatic*>(m_pDlgPassword->GetControl( L"OkAndCancelEditBox" ));
	if( NULL != pStatic )
	{
		CKTDGUIControl::CPictureData* pPicture = pStatic->GetPicture( 4 );
		if( NULL != pPicture )
		{
			pPicture->SetSizeX( 250.f );
		}
		pPicture = pStatic->GetPicture( 5 );
		if( NULL != pPicture )
		{
			pPicture->SetPos( D3DXVECTOR2( 280,50) );
		}
		pPicture = pStatic->GetPicture( 0 );
		if( NULL != pPicture )
		{
			pPicture->SetSizeX( 310.0f );
		}
		pPicture = pStatic->GetPicture( 1 );
		if( NULL != pPicture )
		{
			pPicture->SetSizeX( 310.0f );
		}
		pPicture = pStatic->GetPicture( 2 );
		if( NULL != pPicture )
		{
			pPicture->SetSizeX( 310.0f );
		}
		CKTDGUIButton* pButton = static_cast<CKTDGUIButton*>(m_pDlgPassword->GetControl( L"Button_Check" ) );
		if( NULL != pButton )
		{
			pButton->SetOffsetPos(D3DXVECTOR2(1,6));
		}
		pButton = static_cast<CKTDGUIButton*>(m_pDlgPassword->GetControl( L"Cancel" ) );
		if( NULL != pButton )
		{
			pButton->SetOffsetPos(D3DXVECTOR2(4,6));
		}
	}
}

void CX2ProfileManager::SetLocationInfo()
{
	CKTDGUIComboBox* pComboBoxUpper = static_cast<CKTDGUIComboBox*>(m_pDlgProfileInput->GetControl(L"Location_choice1"));
	CKTDGUIComboBox* pComboBox = static_cast<CKTDGUIComboBox*>(m_pDlgProfileInput->GetControl(L"Location_choice2"));
	if( NULL == pComboBox || NULL == pComboBoxUpper )
		return;

	pComboBox->RemoveAllItems();
	pComboBox->AddItem( m_vecArea[0].UnderArea.c_str(), NULL, false );
	//int SelectUpperArea = pComboBoxUpper->GetSelectedItemIndex();
	CKTDGUIComboBox::ComboBoxItem* UpperAreaName = pComboBoxUpper->GetSelectedItem();

	for(vector<wstring>::size_type i = 1;i < m_vecArea.size(); ++i)
	{
		if( 0 == StrCmp( UpperAreaName->strText, m_vecArea[i].UpperArea.c_str() ) )
			if( 0 != StrCmp( m_vecArea[i].UnderArea.c_str(), L"" ) )
				pComboBox->AddItem( m_vecArea[i].UnderArea.c_str(), NULL, false );
	}

	pComboBox->SetEnable( true );
	pComboBox->SetShow( true );

}
#endif //SERV_LOCAL_RANKING_SYSTEM