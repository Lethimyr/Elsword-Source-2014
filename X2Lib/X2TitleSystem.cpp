#include "StdAfx.h"

//#ifdef TITLE_SYSTEM

#include ".\x2TitleSystem.h"

CX2TitleSystem::CX2TitleSystem( CKTDXStage* pNowState )
{
    m_pNowState = (CX2State*) pNowState;

    RegisterLuaBind();
    
    m_pDlgTitle = new CKTDGUIDialog( m_pNowState, L"DLG_Title_Window.lua" );
    g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDlgTitle );    
    m_pDlgTitle->SetShowEnable(false, false);
    
    CKTDGUIStatic *pStaticName = (CKTDGUIStatic*)m_pDlgTitle->GetControl(L"title_name");
    if(pStaticName != NULL)
        pStaticName->GetString(0)->msg = L"";
	CKTDGUIStatic *pStaticDesc = (CKTDGUIStatic*)m_pDlgTitle->GetControl(L"title_import");
	if(pStaticDesc != NULL)
		pStaticDesc->GetString(0)->msg = L"";


    m_bShow = false;
    m_bTakeTitle = false;
    m_iGroup = 0;

    if(m_pDlgTitle != NULL)
    {
        CKTDGUICheckBox *pDlgCheck = (CKTDGUICheckBox*)m_pDlgTitle->GetControl(L"g_pCheckBoxtitle_window_Slot1");
        if(pDlgCheck != NULL)
            pDlgCheck->SetChecked(m_bTakeTitle);
    }
}

CX2TitleSystem::~CX2TitleSystem(void)
{
    ClearRecord();

    SAFE_DELETE_DIALOG( m_pDlgTitle );
    m_pNowState = NULL;    
}

void CX2TitleSystem::RegisterLuaBind()
{
    lua_tinker::decl( g_pKTDXApp->GetLuaBinder()->GetLuaState(),  "g_pTitleSystem", this );
}


void CX2TitleSystem::OpenTitle()
{
    if(m_pDlgTitle != NULL)
    {
        m_bShow = true;

		g_pKTDXApp->GetDGManager()->GetDialogManager()->ChangeSequence( m_pDlgTitle, false );

        m_pDlgTitle->SetShowEnable(true, true);

		m_pDlgTitle->SetPos( D3DXVECTOR2(331.f, 0.f) );  // D3DXVECTOR2(331, 78)

        if(m_pDlgTitle != NULL)
        {
            CKTDGUICheckBox *pDlgCheck = (CKTDGUICheckBox*)m_pDlgTitle->GetControl(L"g_pCheckBoxtitle_window_Slot1");
            if(pDlgCheck != NULL)
                pDlgCheck->SetChecked(m_bTakeTitle);
        }

#ifdef SERV_ALTERA_AUTO_OPEN_HARD_CODE
		IF_EVENT_ENABLED( CEI_ALTERA_AUTO_OPEN_HARD_CODE )
		{
			// 2013.03.14 lygan_조성욱 // 코드 비어 있는게 맞습니다.
		}
		ELSE
		{
			if ( m_pDlgTitle != NULL )
			{
				CKTDGUIButton *pButton;
				pButton = (CKTDGUIButton*)m_pDlgTitle->GetControl(L"TitleGroup4");
				if ( pButton != NULL )
					pButton->SetShowEnable(false, false);
			}
		}
#endif //SERV_ALTERA_AUTO_OPEN_HARD_CODE

#ifdef SERV_PEITA_AUTO_OPEN_HARD_CODE
		IF_EVENT_ENABLED( CEI_PEITA_AUTO_OPEN_HARD_CODE )
		{
			// 2013.03.14 lygan_조성욱 // 코드 비어 있는게 맞습니다.
		}
		ELSE
		{
			if ( m_pDlgTitle != NULL )
			{
				CKTDGUIButton *pButton;
				pButton = (CKTDGUIButton*)m_pDlgTitle->GetControl(L"TitleGroup5");
				if ( pButton != NULL )
					pButton->SetShowEnable(false, false);
			}
		}
#endif //SERV_PEITA_AUTO_OPEN_HARD_CODE

#ifdef SERV_VELDER_AUTO_OPEN_HARD_CODE
		IF_EVENT_ENABLED( CEI_VELDER_AUTO_OPEN_HARD_CODE )
		{
			// 2013.03.14 lygan_조성욱 // 코드 비어 있는게 맞습니다.
		}
		ELSE
		{
			if ( m_pDlgTitle != NULL )
			{
				CKTDGUIButton *pButton;
				pButton = (CKTDGUIButton*)m_pDlgTitle->GetControl(L"TitleGroup8");
				if ( pButton != NULL )
					pButton->SetShowEnable(false, false);
			}
		}
#endif //SERV_VELDER_AUTO_OPEN_HARD_CODE

#ifdef SERV_HAMEL_AUTO_OPEN_HARD_CODE
		IF_EVENT_ENABLED( CEI_HAMEL_AUTO_OPEN_HARD_CODE )
		{
			// 2013.03.14 lygan_조성욱 // 코드 비어 있는게 맞습니다.
		}
		ELSE
		{
			if ( m_pDlgTitle != NULL )
			{
				CKTDGUIButton *pButton;
				pButton = (CKTDGUIButton*)m_pDlgTitle->GetControl(L"TitleGroup9");
				if ( pButton != NULL )
					pButton->SetShowEnable(false, false);
			}
		}
#endif //SERV_HAMEL_AUTO_OPEN_HARD_CODE

#ifdef SERV_SANDER_AUTO_OPEN_HARD_CODE
		IF_EVENT_ENABLED( CEI_SANDER_AUTO_OPEN_HARD_CODE )
		{
			// 2013.03.14 lygan_조성욱 // 코드 비어 있는게 맞습니다.
		}
		ELSE
		{
			if ( m_pDlgTitle != NULL )
			{
				CKTDGUIButton *pButton;
				pButton = (CKTDGUIButton*)m_pDlgTitle->GetControl(L"TitleGroup11");
				if ( pButton != NULL )
					pButton->SetShowEnable(false, false);
			}
		}
#endif //SERV_SANDER_AUTO_OPEN_HARD_CODE

        ClearTitle();

        ChangeTitleGroup(m_iGroup);
        //ShowTitleList();
    }
}

void CX2TitleSystem::ClearTitle()
{
	CKTDGUIStatic *pStaticName = (CKTDGUIStatic*)m_pDlgTitle->GetControl(L"title_name");
	pStaticName->GetString(0)->msg = L"";
	CKTDGUIStatic *pStaticDesc = (CKTDGUIStatic*)m_pDlgTitle->GetControl(L"title_import");
	pStaticDesc->GetString(0)->msg = L"";
	CKTDGUIListBox *pListbox = (CKTDGUIListBox*)m_pDlgTitle->GetControl(L"TitleDesc");
	pListbox->RemoveAllItems();
#ifdef TITLE_DESCRIPTION_RENEWAL
	CKTDGUIListBox *pListboxMainTitle = (CKTDGUIListBox*)m_pDlgTitle->GetControl(L"MainTitle");
	pListboxMainTitle->RemoveAllItems();
#endif //TITLE_DESCRIPTION_RENEWAL

	SetPreviewTitle(L"Title_Empty.tga");
}

void CX2TitleSystem::UpdateTitle()
{
    if(m_pDlgTitle != NULL)
    {
        m_bShow = true;
        m_pDlgTitle->SetShowEnable(true, true);
    }
}

void CX2TitleSystem::CloseTitle()
{
    if(m_pDlgTitle != NULL)
    {
        m_bShow = false;
        m_pDlgTitle->SetShowEnable(false, false);        
    }
}

void CX2TitleSystem::AddRecord(TitleRecord aRecord)
{
    m_vecTitleRecord.push_back(aRecord);
}

void CX2TitleSystem::UpdateRecord(TitleRecord aRecord)
{
    for(int i=0; i<(int)m_vecTitleRecord.size(); ++i)
    {
        if( aRecord.m_iTitleID == m_vecTitleRecord[i].m_iTitleID )
        {
            m_vecTitleRecord[i] = aRecord;

            ShowTitleList();
            return;
        }
    }    
}

void CX2TitleSystem::ClearRecord()
{
    m_vecTitleRecord.clear();
}

int CX2TitleSystem::GetTitleState(int titleId)
{
    for(int i=0; i<(int)m_vecTitleRecord.size(); ++i)
    {
        if(titleId == m_vecTitleRecord[i].m_iTitleID)
        {
            return m_vecTitleRecord[i].m_nSecret;
        }
    }

    return 2;
}

bool CX2TitleSystem::Cmp(const CX2TitleSystem::TitleRecord& r1, const CX2TitleSystem::TitleRecord& r2)
{
    return r1.m_iSort < r2.m_iSort ? true : false;
}

// note!! 앞으로는 요런식으로 하면 좋을 것 같기도 해요~~ ^^
//struct TitleRecordLessThan : public std::binary_function<CX2TitleSystem::TitleRecord, CX2TitleSystem::TitleRecord, bool>
//{
//	bool operator() ( const CX2TitleSystem::TitleRecord& lhs, const CX2TitleSystem::TitleRecord& rhs ) const
//	{
//		return lhs.m_iSort < rhs.m_iSort;
//	}
//};


void CX2TitleSystem::SortRecord()
{       
    sort(m_vecTitleRecord.begin(), m_vecTitleRecord.end(), CX2TitleSystem::Cmp);
}

void CX2TitleSystem::SetCheckTake()
{
    if(m_pDlgTitle != NULL)
    {
        CKTDGUICheckBox *pDlgCheck = (CKTDGUICheckBox*)m_pDlgTitle->GetControl(L"g_pCheckBoxtitle_window_Slot1");
        if(pDlgCheck != NULL)
        {
            m_bTakeTitle = pDlgCheck->GetChecked();
        }
        ShowTitleList();
    }
}

void CX2TitleSystem::ShowTitleList()
{
    if(m_pDlgTitle == NULL)
        return;   
    
    D3DXCOLOR tempColor;
    wstring tempStr, periodDesc;

    CKTDGUIListBox *pListbox = (CKTDGUIListBox*)m_pDlgTitle->GetControl(L"TitleList");
    pListbox->RemoveAllItems();
    bool bShow = true;

	pListbox->AddString( GET_STRING( STR_ID_771 ), D3DXCOLOR(0.f, 0.f, 0.f, 1.f), NULL);

    for(int i=0; i<(int)m_vecTitleRecord.size(); ++i)
    {               
        switch(m_vecTitleRecord[i].m_nSecret)
        {
        case 0:            
            tempStr = m_vecTitleRecord[i].m_wstrTitleName;
            tempColor = D3DXCOLOR(0.f, 0.f, 0.f, 1.f);
            break;
        case 1:
            tempStr = GET_REPLACED_STRING( ( STR_ID_772, "L", m_vecTitleRecord[i].m_wstrTitleName ) );
            tempColor = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.f);
            break;
        case 2:            
            tempStr = GET_STRING( STR_ID_773 );
            tempColor = D3DXCOLOR(1.f, 0.f, 0.f, 1.f);

#ifdef SHOW_GM_TITILE_PREPEQUISITE_INFO
			if ( g_pData != NULL
				&& g_pData->GetMyUser() != NULL)
			{
				if ( g_pData->GetMyUser()->GetAuthLevel() == CX2User::XUAL_DEV )
				{
					tempStr = GET_REPLACED_STRING( ( STR_ID_772, "L", m_vecTitleRecord[i].m_wstrTitleName ) );
					tempColor = D3DXCOLOR(1.f, 0.f, 0.f, 1.f);
				}
			}
#endif //SHOW_GM_TITILE_PREPEQUISITE_INFO

            break;
        }  
        

        bShow = true;
        if(m_vecTitleRecord[i].m_bSecret == true)
        {            
            bShow = false;            
        }
        if(m_bTakeTitle == true && m_vecTitleRecord[i].m_nSecret != 0)
        {
            bShow = false;
        }
        if(m_iGroup > 0 && m_iGroup != m_vecTitleRecord[i].m_eTitleType)
        {
            bShow = false;
        }
		// secret이고 획득하였지만 기간이 다되었다면 보여주지 않는다.
		if(bShow == true && m_vecTitleRecord[i].m_nSecret == 0 && m_vecTitleRecord[i].m_bPeriod == true)
		{
			CTime cTime;
			KncUtil::ConvertStringToCTime( m_vecTitleRecord[i].m_wstrEndDate, cTime );
			__time64_t t0 = g_pData->GetServerCurrentTime();

			double fDiffTime = difftime( KncUtil::TimeToInt( cTime ), (time_t)t0 );

			if(fDiffTime <= 0)
			{
				bShow = false;
			}

		}

        if(g_pData->GetTitleManager()->GetUnitType(m_vecTitleRecord[i].m_iTitleID) == false)
            bShow = false;

        if(g_pData->GetTitleManager()->GetUnitClass(m_vecTitleRecord[i].m_iTitleID) == false)
            bShow = false;

		
        if(bShow == true)
        {
            if(m_vecTitleRecord[i].m_nSecret == 0 && m_vecTitleRecord[i].m_bPeriod == true)
            {                
                periodDesc = L" (";
                
                CTime cTime;
                KncUtil::ConvertStringToCTime( m_vecTitleRecord[i].m_wstrEndDate, cTime );

                wstringstream wstrstm;
                wstrstm.str( L"" );

                //time_t t0 = time(0);
                __time64_t t0 = g_pData->GetServerCurrentTime();

                double fDiffTime = difftime( KncUtil::TimeToInt( cTime ), (time_t)t0 );

                int remainDay =  (int)(fDiffTime / ( 60 * 60 * 24 ) );
                if ( remainDay > 0 )
                    wstrstm << remainDay << GET_STRING( STR_ID_14 ) << L" ";

                int remainHour = ((time_t)fDiffTime % ( 60 * 60 * 24 )) / (60 * 60);
                if ( remainHour < 0 )
                    remainHour = 0;

                wstrstm << remainHour << GET_STRING( STR_ID_310 ) << L" ";

                int remainMinute = ((time_t)fDiffTime % ( 60 * 60 )) / (60 );

                if ( remainDay <= 0 )
                {
                    if ( remainMinute <= 0 )
                        remainMinute = 0;
                    wstrstm << remainMinute << GET_STRING( STR_ID_311 ) << L" ";
                }

                periodDesc += wstrstm.str().c_str();
                periodDesc += L")";
                tempStr += periodDesc;
            }
#ifdef ELLIPSE_GLOBAL
#ifdef TITLE_PERIOD_NO_ELLIPSE_GLOBAL
		if( m_vecTitleRecord[i].m_bPeriod == false)
		{
			bool bEllipse = false;
			tempStr = CWordLineHandler::GetStrByLineBreakInX2MainWithEllipse(tempStr.c_str(), 200, pListbox->GetString()->fontIndex, 1, bEllipse);
		}
#else //TITLE_PERIOD_NO_ELLIPSE_GLOBAL
			bool bEllipse = false;
#if defined ( COUNTRY_WORDWRAP_WIDTH_US )
			tempStr = CWordLineHandler::GetStrByLineBreakInX2MainWithEllipse(tempStr.c_str(), 240, pListbox->GetString()->fontIndex, 1, bEllipse);
#elif defined ( UNIQUENESS_EU_ONLY )
			tempStr = CWordLineHandler::GetStrByLineBreakInX2MainWithEllipse(tempStr.c_str(), 215, pListbox->GetString()->fontIndex, 1, bEllipse);
#else
			tempStr = CWordLineHandler::GetStrByLineBreakInX2MainWithEllipse(tempStr.c_str(), 200, pListbox->GetString()->fontIndex, 1, bEllipse);			
#endif
#endif //TITLE_PERIOD_NO_ELLIPSE_GLOBAL
#endif ELLIPSE_GLOBAL

#ifdef ELLIPSE_CN
			bool bEllipse = false;
			tempStr = CWordLineHandler::CutStringWithEllipse(tempStr.c_str(), 200, pListbox->GetString()->fontIndex, 1, bEllipse);
#endif //ELLIPSE_CN
			pListbox->AddString(tempStr.c_str(), tempColor, &m_vecTitleRecord[i], NULL );
        }
    }

}

int CX2TitleSystem::GetSelectedTitle()
{
    CKTDGUIListBox *pListbox = (CKTDGUIListBox*)m_pDlgTitle->GetControl(L"TitleList");
    if(pListbox != NULL)
    {
        CKTDGUIListBox::ListBoxItem *pItem = pListbox->GetSelectedItem();

        if(pItem == NULL)
            return 0;

        TitleRecord *pData = (TitleRecord*)pItem->pData;

        if(pData != NULL)
        {   
            
            ShowTitleName(pData);
            ShowTitleDesc(pData);
            
            return pData->m_iTitleID;
        }        
    }

    return 0;
}

void CX2TitleSystem::ShowTitleGuideDesc()
{
    CKTDGUIListBox *pListbox = (CKTDGUIListBox*)m_pDlgTitle->GetControl(L"TitleList");
    if(pListbox != NULL)
    {
        CKTDGUIListBox::ListBoxItem *pItem = pListbox->GetSelectedItem();

        if(pItem == NULL)
            return;

        TitleRecord *pData = (TitleRecord*)pItem->pData;

        if(pData != NULL)
        {   
            if(pData->m_nSecret == 1)
            {
                //pListbox->SetGuideDesc(GetMissionDesc(pData).c_str());
                //pListbox->SetGuideDescAbsolutePos(D3DXVECTOR2(734, 300));
                //pListbox->SetGuideDescLT(true);
                //m_pDlgTitle->ShowGuideDesc(pListbox);
            }
        }        
    }
}

bool CX2TitleSystem::SetSelectTitle(int val)
{
    CKTDGUIListBox *pListbox = (CKTDGUIListBox*)m_pDlgTitle->GetControl(L"TitleList");
    CKTDGUIListBox::ListBoxItem *pItem = NULL;
    TitleRecord *pData = NULL;

    for(int i=0; i<pListbox->GetSize(); ++i)
    {        
        pItem = pListbox->GetItem(i);
        pData = (TitleRecord*)pItem->pData;
        if(pItem != NULL && pData != NULL)
        {
            if(val == pData->m_iTitleID)
            {
                pListbox->SelectItem(i);
                GetSelectedTitle();
                return true;
            }
        }
    }

    return false;
}

void CX2TitleSystem::ShowTitleName(TitleRecord* pData)
{
    if(pData == NULL)
        return;

    wstring titleName = pData->m_wstrTitleName;

    if(pData->m_nSecret == 2)
        titleName = L"";

    //int nLen = titleName.length();
    //if(nLen > 11)
    //{
    //   titleName.insert(11, L"\n");
    //}

    CKTDGUIStatic *pStaticName = (CKTDGUIStatic*)m_pDlgTitle->GetControl(L"title_name");
#ifdef ELLIPSE_GLOBAL
	bool bEllipse = false;
	titleName = CWordLineHandler::GetStrByLineBreakInX2MainWithEllipse(titleName.c_str(), 215, pStaticName->GetString(0)->fontIndex, 1, bEllipse);
#endif ELLIPSE_GLOBAL
#ifdef GM_SHOW_TITLE_ID
	if ( g_pData->GetMyUser()->GetAuthLevel() >= CX2User::XUAL_OPERATOR )
	{
		WCHAR wszTitleID[16] = {0,};
		StringCchPrintf( wszTitleID, 16, L"%d", pData->m_iTitleID );
		wszTitleID[15] = NULL;

		titleName = titleName + L"\t#CFF6666 TitleID : " + wszTitleID + L"#CX";
	}
#endif GM_SHOW_TITLE_ID
    pStaticName->GetString(0)->msg = titleName.c_str();
}

void CX2TitleSystem::ShowTitleDesc(TitleRecord* pData)
{
	CKTDGUIStatic *pStaticDesc = (CKTDGUIStatic*)m_pDlgTitle->GetControl(L"title_import");	
    CKTDGUIListBox *pListbox = (CKTDGUIListBox*)m_pDlgTitle->GetControl(L"TitleDesc");
    pListbox->RemoveAllItems();
#ifdef TITLE_DESCRIPTION_RENEWAL
	CKTDGUIListBox *pListboxMainTitle = (CKTDGUIListBox*)m_pDlgTitle->GetControl(L"MainTitle");
	pListboxMainTitle->RemoveAllItems();
#endif //TITLE_DESCRIPTION_RENEWAL

    if(pData == NULL)    
        return;    

#ifdef SHOW_TITLE_ID
	std::wstring wstrTitleID;
	if ( g_pData != NULL //&& g_pData->GetMyUser() != NULL )
		&& g_pData->GetMyUser() != NULL)
	{
		if ( g_pData->GetMyUser()->GetAuthLevel() == CX2User::XUAL_DEV )
		{
			wstrTitleID = (boost::wformat(L"ID = %1%\n") % pData->m_iTitleID).str();
		}
	}
#endif SHOW_TITLE_ID
		
	wstring errMsg;

#ifdef SHOW_GM_TITILE_PREPEQUISITE_INFO
	if ( g_pData != NULL //&& g_pData->GetMyUser() != NULL )
		&& g_pData->GetMyUser() != NULL)
	{
		if ( g_pData->GetMyUser()->GetAuthLevel() != CX2User::XUAL_DEV )
		{

#endif //SHOW_GM_TITILE_PREPEQUISITE_INFO
			if(pData->m_nSecret == 2)
			{
#ifdef SHOW_TITLE_ID
				errMsg = wstrTitleID;
				errMsg += GET_STRING( STR_ID_776 );
#else
				errMsg = GET_STRING( STR_ID_776 );
#endif SHOW_TITLE_ID
#ifdef CLIENT_GLOBAL_LINEBREAK
#ifdef COUNTRY_WORDWRAP_WIDTH_US
				errMsg = CWordLineHandler::GetStrByLineBreakInX2Main( errMsg.c_str(), 220, XUF_DODUM_13_SEMIBOLD );
#else //COUNTRY_WORDWRAP_WIDTH_US
				errMsg = CWordLineHandler::GetStrByLineBreakInX2Main( errMsg.c_str(), 235, XUF_DODUM_13_SEMIBOLD );
#endif //COUNTRY_WORDWRAP_WIDTH_US
#endif //CLIENT_GLOBAL_LINEBREAK
				pStaticDesc->GetString(0)->msg = errMsg.c_str();
				return;
			}
#ifdef SHOW_GM_TITILE_PREPEQUISITE_INFO
		}
	}
#endif //SHOW_GM_TITILE_PREPEQUISITE_INFO

    wstring str1 = pData->m_wstrTitleDesc;
#ifdef CLIENT_GLOBAL_LINEBREAK
#if defined ( COUNTRY_WORDWRAP_WIDTH_US )
	str1 = CWordLineHandler::GetStrByLineBreakInX2Main( str1.c_str(), 235, XUF_DODUM_13_SEMIBOLD );
#elif defined ( UNIQUENESS_EU_ONLY )
	str1 = CWordLineHandler::GetStrByLineBreakInX2Main( str1.c_str(), 225, XUF_DODUM_13_SEMIBOLD );
#else	
	str1 = CWordLineHandler::GetStrByLineBreakInX2Main( str1.c_str(), 170, XUF_DODUM_13_SEMIBOLD );
#endif
#else //CLIENT_GLOBAL_LINEBREAK
#ifdef ALWAYS_TEMP_LINEBREAK
	wstring TempStr;
	 TempStr = g_pMain->GetStrByLienBreak( str1.c_str(), 185, XUF_DODUM_13_SEMIBOLD );
#else //ALWAYS_TEMP_LINEBREAK
	g_pMain->GetStrByLienBreak( str1.c_str(), 235, XUF_DODUM_13_SEMIBOLD );
#endif ALWAYS_TEMP_LINEBREAK
#endif //CLIENT_GLOBAL_LINEBREAK
#ifdef SHOW_TITLE_ID
#ifdef ALWAYS_TEMP_LINEBREAK
	pStaticDesc->GetString(0)->msg = wstrTitleID + TempStr.c_str();
#else //ALWAYS_TEMP_LINEBREAK
#ifdef TITLE_DESCRIPTION_RENEWAL
	wstring tempTitleDec = wstrTitleID + str1;

	wstring::size_type lastPosCond = tempTitleDec.find_first_not_of(L"\n", 0);
	wstring::size_type posCond     = tempTitleDec.find_first_of(L"\n", lastPosCond);

	while (wstring::npos != posCond || wstring::npos != lastPosCond)
	{        
		wstring tempStr = tempTitleDec.substr(lastPosCond, posCond - lastPosCond);
		pListboxMainTitle->AddItem(tempStr.c_str(),NULL);

		lastPosCond = tempTitleDec.find_first_not_of(L"\n", posCond);
		posCond = tempTitleDec.find_first_of(L"\n", lastPosCond);
	}  
	pStaticDesc->GetString(0)->msg = L"";
	
#else //TITLE_DESCRIPTION_RENEWAL
	pStaticDesc->GetString(0)->msg = wstrTitleID + str1.c_str();
#endif //TITLE_DESCRIPTION_RENEWAL
#endif ALWAYS_TEMP_LINEBREAK
#else
	pStaticDesc->GetString(0)->msg = str1.c_str();
#endif SHOW_TITLE_ID

	// 칭호 획득조건
	// 획득 가능 칭호만 획득조건을 보여준다.
	if(pData->m_nSecret == 1
#ifdef SHOW_GM_TITILE_PREPEQUISITE_INFO
		|| pData->m_nSecret == 2
#endif //SHOW_GM_TITILE_PREPEQUISITE_INFO
		
		)
	{
#ifdef SHOW_GM_TITILE_PREPEQUISITE_INFO
		if ( g_pData != NULL //&& g_pData->GetMyUser() != NULL )
			&& g_pData->GetMyUser() != NULL)
		{
			if ( g_pData->GetMyUser()->GetAuthLevel() != CX2User::XUAL_DEV && pData->m_nSecret == 2 )
			{
				// 2013.01.31 lygan_조성욱 // 위에서 어느정도 필터가 되지만 혹시나 싶어서 조건 추가
				return;
			}
		}
#endif //SHOW_GM_TITILE_PREPEQUISITE_INFO

		wstring strCondition = GET_REPLACED_STRING( ( STR_ID_777, "L", GetMissionDesc( pData ) ) );
#ifdef CLIENT_GLOBAL_LINEBREAK
#ifdef COUNTRY_WORDWRAP_WIDTH_US
		strCondition = CWordLineHandler::GetStrByLineBreakInX2Main( strCondition.c_str(), 300, XUF_DODUM_13_SEMIBOLD );
#else //COUNTRY_WORDWRAP_WIDTH_US
		strCondition = CWordLineHandler::GetStrByLineBreakInX2Main( strCondition.c_str(), 260, XUF_DODUM_13_SEMIBOLD );
#endif //COUNTRY_WORDWRAP_WIDTH_US
#else //CLIENT_GLOBAL_LINEBREAK
		g_pMain->GetStrByLienBreak( str1.c_str(), 260, XUF_DODUM_13_SEMIBOLD );
#endif //CLIENT_GLOBAL_LINEBREAK
		wstring::size_type lastPosCond = strCondition.find_first_not_of(L"\n", 0);
		wstring::size_type posCond     = strCondition.find_first_of(L"\n", lastPosCond);

		while (wstring::npos != posCond || wstring::npos != lastPosCond)
		{        
			wstring tempStr = strCondition.substr(lastPosCond, posCond - lastPosCond);
			pListbox->AddItem(tempStr.c_str(), NULL);

			lastPosCond = strCondition.find_first_not_of(L"\n", posCond);
			posCond = strCondition.find_first_of(L"\n", lastPosCond);
		}    
	}		

	// 칭호 옵션
    wstring str2 = g_pData->GetTitleManager()->GetTitleAbilityDesc(pData->m_iTitleID);
#ifdef CLIENT_GLOBAL_LINEBREAK
#ifdef COUNTRY_WORDWRAP_WIDTH_US
	str2 = CWordLineHandler::GetStrByLineBreakInX2Main( str2.c_str(), 300, XUF_DODUM_13_SEMIBOLD );
#else //COUNTRY_WORDWRAP_WIDTH_US
	str2 = CWordLineHandler::GetStrByLineBreakInX2Main( str2.c_str(), 260, XUF_DODUM_13_SEMIBOLD );
#endif //COUNTRY_WORDWRAP_WIDTH_US
#endif //CLIENT_GLOBAL_LINEBREAK
    wstring::size_type lastPos = str2.find_first_not_of(L"\n", 0);
    wstring::size_type pos     = str2.find_first_of(L"\n", lastPos);

    while (wstring::npos != pos || wstring::npos != lastPos)
    {        
        wstring tempStr = str2.substr(lastPos, pos - lastPos);
        pListbox->AddItem(tempStr.c_str(), NULL);

        lastPos = str2.find_first_not_of(L"\n", pos);
        pos = str2.find_first_of(L"\n", lastPos);
    }    

    pListbox->AddItem(L"\n ", NULL);

    
}

wstring CX2TitleSystem::GetMissionDesc(TitleRecord* pData)
{
    if(g_pData != NULL)
        return g_pData->GetTitleManager()->GetMIssionDesc(pData->m_iTitleID);

    return L"";
}

void CX2TitleSystem::ChangeTitleGroup(int iGroup)
{

    if(m_pDlgTitle == NULL)
        return;

    CKTDGUIButton *pButton[CX2TitleManager::TT_END]; // 8: 그룹수
	WCHAR groupBuf[50] = {0,};
    for(int i=0; i<CX2TitleManager::TT_END; ++i)
    {
       // wsprintf(groupBuf, L"TitleGroup%d", i);
		StringCchPrintf(groupBuf, 50, L"TitleGroup%d", i);
        pButton[i] = (CKTDGUIButton*)m_pDlgTitle->GetControl(groupBuf);
        if(pButton[i] != NULL)
            pButton[i]->SetDownStateAtNormal(false);
    }

    if(pButton[iGroup] != NULL)
        pButton[iGroup]->SetDownStateAtNormal(true);

    m_iGroup = iGroup;
    ShowTitleList();
}

void CX2TitleSystem::CheckPeriod()
{    
    for(int i=0; i<(int)m_vecTitleRecord.size(); ++i)
    {
        if(m_vecTitleRecord[i].m_bPeriod == true)
        {
            // 만료일을 검사한다.
            if(m_vecTitleRecord[i].m_periodTime <= g_pData->GetServerCurrentTime())
            {
                m_vecTitleRecord[i].m_nSecret = 1;
                //m_vecTitleRecord[i].m_bPeriod = true;
            }
        }
    }
}

void CX2TitleSystem::SetPreviewTitle(wstring titleName)
{
	CKTDGUIStatic *pStaticTitle = (CKTDGUIStatic*)m_pDlgTitle->GetControl(L"g_pStatictitle_window");
	if(pStaticTitle != NULL)
	{
		CKTDGUIControl::CPictureData* pTitlePicture = pStaticTitle->GetPictureIndex(1);

		if(pTitlePicture != NULL)
		{
			pTitlePicture->SetTex(titleName.c_str());
		}
	}	
}
//#endif