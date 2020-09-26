// KAccountDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "KEsdRemoteController_Console.h"
#include "KAccountDlg.h"
#include ".\kaccountdlg.h"


// CKAccountDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CKAccountDlg, CDialog)
CKAccountDlg::CKAccountDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CKAccountDlg::IDD, pParent)
{
}

CKAccountDlg::~CKAccountDlg()
{
}

void CKAccountDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_NEW_ID_EDIT, m_newIdEdit);
	DDX_Control(pDX, IDC_NEW_PW_EDIT, m_newPwEdit);
	DDX_Control(pDX, IDC_RE_PW_EDIT, m_rePwEdit);
	DDX_Control(pDX, IDC_ACCOUNT_LIST, m_accList);
	DDX_Control(pDX, IDC_AUTH_COMBO, m_authLevel);
}

BOOL CKAccountDlg::OnInitDialog()
{
    // 리스트 박스에 계정 리스트들 추가
	CListCtrl* pkListEsd = (CListCtrl*) GetDlgItem( IDC_ACCOUNT_LIST );		
	ListView_SetExtendedListViewStyle( pkListEsd->GetSafeHwnd(),
		LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES );

	{
		LV_COLUMN kCol;
		ZeroMemory( &kCol, sizeof(kCol) );
		kCol.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
		kCol.fmt = LVCFMT_LEFT;
		kCol.cchTextMax = 256;
		kCol.cx = 80;
		kCol.pszText = L"ID";

		pkListEsd->InsertColumn(0, &kCol);
	}

	{
		LV_COLUMN kCol;
		ZeroMemory( &kCol, sizeof(kCol) );
		kCol.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
		kCol.fmt = LVCFMT_LEFT;
		kCol.cchTextMax = 256;
		kCol.cx = 90;
		kCol.pszText = L"PASSWORD";

		pkListEsd->InsertColumn(1, &kCol);
	}

	{
		LV_COLUMN kCol;
		ZeroMemory( &kCol, sizeof(kCol) );
		kCol.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
		kCol.fmt = LVCFMT_LEFT;
		kCol.cchTextMax = 256;
		kCol.cx = 110;
		kCol.pszText = L"Description";

		pkListEsd->InsertColumn(2, &kCol);
	}

	{
		LV_COLUMN kCol;
		ZeroMemory( &kCol, sizeof(kCol) );
		kCol.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
		kCol.fmt = LVCFMT_LEFT;
		kCol.cchTextMax = 256;
		kCol.cx = 85;
		kCol.pszText = L"Auth Level";

		pkListEsd->InsertColumn(3, &kCol);
	}
	
	int iIdx = pkListEsd->GetItemCount();

	vector<AccountList> acList;
	//theApp.m_kImp.DoQueryAccountList( acList );

	vector<AccountList>::iterator mit = acList.begin();
	for( ; mit != acList.end(); mit++ )
	{
		{
			LV_ITEM kItem;
			ZeroMemory( &kItem, sizeof(kItem) );
			kItem.mask = LVIF_TEXT; 
			kItem.iItem = iIdx;
			kItem.iSubItem = 0;
			kItem.pszText = const_cast<wchar_t*>(mit->m_id.c_str());
			;

			pkListEsd->InsertItem( &kItem );
		}

		{
			LV_ITEM kItem;
			ZeroMemory( &kItem, sizeof(kItem) );
			kItem.mask = LVIF_TEXT; 
			kItem.iItem = iIdx;
			kItem.iSubItem = 1;
			kItem.pszText = const_cast<wchar_t*>(mit->m_pw.c_str());
			;

			pkListEsd->SetItem( &kItem );
		}

		{
			LV_ITEM kItem;
			ZeroMemory( &kItem, sizeof(kItem) );
			kItem.mask = LVIF_TEXT; 
			kItem.iItem = iIdx;
			kItem.iSubItem = 2;
			kItem.pszText = const_cast<wchar_t*>(mit->m_description.c_str());

			pkListEsd->SetItem( &kItem );
		}

		{
			LV_ITEM kItem;
			ZeroMemory( &kItem, sizeof(kItem) );
			kItem.mask = LVIF_TEXT; 
			kItem.iItem = iIdx;
			kItem.iSubItem = 3;			
			kItem.pszText = const_cast<wchar_t*>( GetAuthString( mit->m_authLevel ) );

			pkListEsd->SetItem( &kItem );
		}
	}

	return CDialog::OnInitDialog();
}

void CKAccountDlg::UpdateAccountList()
{
	CListCtrl* pkListEsd = (CListCtrl*) GetDlgItem( IDC_ACCOUNT_LIST );
	pkListEsd->DeleteAllItems();
	int iIdx = pkListEsd->GetItemCount();

	CString temp;
	vector<AccountList> acList;
	//theApp.m_kImp.DoQueryAccountList( acList );

	vector<AccountList>::iterator mit = acList.begin();
	for( ; mit != acList.end(); mit++ )
	{
		{
			LV_ITEM kItem;
			ZeroMemory( &kItem, sizeof(kItem) );
			kItem.mask = LVIF_TEXT; 
			kItem.iItem = iIdx;
			kItem.iSubItem = 0;
			kItem.pszText = const_cast<wchar_t*>(mit->m_id.c_str());
			;

			pkListEsd->InsertItem( &kItem );
		}

		{
			LV_ITEM kItem;
			ZeroMemory( &kItem, sizeof(kItem) );
			kItem.mask = LVIF_TEXT; 
			kItem.iItem = iIdx;
			kItem.iSubItem = 1;
			kItem.pszText = const_cast<wchar_t*>(mit->m_pw.c_str());

			pkListEsd->SetItem( &kItem );
		}

		{
			LV_ITEM kItem;
			ZeroMemory( &kItem, sizeof(kItem) );
			kItem.mask = LVIF_TEXT; 
			kItem.iItem = iIdx;
			kItem.iSubItem = 2;
			kItem.pszText = const_cast<wchar_t*>(mit->m_description.c_str());

			pkListEsd->SetItem( &kItem );
		}

		{
			LV_ITEM kItem;
			ZeroMemory( &kItem, sizeof(kItem) );
			kItem.mask = LVIF_TEXT; 
			kItem.iItem = iIdx;
			kItem.iSubItem = 3;
			kItem.pszText = const_cast<wchar_t*>( GetAuthString( mit->m_authLevel ) );

			pkListEsd->SetItem( &kItem );
		}
	}
}


BEGIN_MESSAGE_MAP(CKAccountDlg, CDialog)
ON_BN_CLICKED(IDC_NEW_ACCOUNT, OnBnClickedNewAccount)
ON_BN_CLICKED(IDC_MODIFY, OnBnClickedModify)
ON_BN_CLICKED(IDC_DEL_ACCOUNT, OnBnClickedDelAccount)
ON_NOTIFY(LVN_ITEMACTIVATE, IDC_ACCOUNT_LIST, OnLvnItemActivateAccountList)
END_MESSAGE_MAP()


// CKAccountDlg 메시지 처리기입니다.
void CKAccountDlg::OnBnClickedNewAccount()
{
	CString newID, newPW, rePW, message;
	m_newIdEdit.GetWindowText( newID );
	m_newPwEdit.GetWindowText( newPW );
	m_rePwEdit.GetWindowText( rePW );
	int authIndex = m_authLevel.GetCurSel();

	if( newID.GetLength() > ID_SIZE )
	{
		message.Format( L"ID길이가 %d자 이상 넘어갈 수 없습니다!", ID_SIZE );
		MessageBox( message.GetBuffer(), L"알림", MB_OK );
		return;
	}

	if( newPW.GetLength() > PW_SIZE || rePW.GetLength() > PW_SIZE )
	{
		message.Format( L"암호길이가 %d자 이상 넘어갈 수 없습니다!", PW_SIZE );
		MessageBox( message.GetBuffer(), L"알림", MB_OK );
		return;
	}

	if( newID.IsEmpty() )
	{
		MessageBox( L"ID를 입력하세요!", L"알림", MB_OK );
		return;
	}

	if( newPW.IsEmpty() || rePW.IsEmpty() )
	{
		MessageBox( L"암호를 입력하세요!", L"알림", MB_OK );
		return;
	}

	if( newPW.Compare( rePW.GetBuffer() ) != 0 )
	{
		MessageBox( L"암호를 잘못 입력하였습니다!", L"알림", MB_OK );
		return;
	}

	if( authIndex == -1 )
	{
		MessageBox( L"계정권한을 선택해 주세요!", L"알림", MB_OK );
		return;
	}


	//theApp.m_kImp.DoQueryNewAccount( newID.GetBuffer(), newPW.GetBuffer(), authIndex );
	UpdateAccountList();
}

void CKAccountDlg::OnBnClickedModify()
{
	CString newID, newPW, rePW, message;
	m_newIdEdit.GetWindowText( newID );
	m_newPwEdit.GetWindowText( newPW );
	m_rePwEdit.GetWindowText( rePW );
	int authIndex = m_authLevel.GetCurSel();

	if( newID.GetLength() > ID_SIZE )
	{
		message.Format( L"ID길이가 %d자 이상 넘어갈 수 없습니다!", ID_SIZE );
		MessageBox( message.GetBuffer(), L"알림", MB_OK );
		return;
	}

	if( newPW.GetLength() > PW_SIZE || rePW.GetLength() > PW_SIZE )
	{
		message.Format( L"암호길이가 %d자 이상 넘어갈 수 없습니다!", PW_SIZE );
		MessageBox( message.GetBuffer(), L"알림", MB_OK );
		return;
	}

	if( newID.IsEmpty() )
	{
		MessageBox( L"ID를 입력하세요!", L"알림", MB_OK );
		return;
	}

	if( newPW.IsEmpty() || rePW.IsEmpty() )
	{
		MessageBox( L"암호를 입력하세요!", L"알림", MB_OK );
		return;
	}

	if( newPW.Compare( rePW.GetBuffer() ) != 0 )
	{
		MessageBox( L"암호를 잘못 입력하였습니다!", L"알림", MB_OK );
		return;
	}

	if( authIndex == -1 )
	{
		MessageBox( L"계정권한을 선택해 주세요!", L"알림", MB_OK );
		return;
	}

	//theApp.m_kImp.DoQueryModifyAccount( newID.GetBuffer(), newPW.GetBuffer(), authIndex );
	UpdateAccountList();
}

void CKAccountDlg::OnBnClickedDelAccount()
{
	CString newID, message;
	m_newIdEdit.GetWindowText( newID );

	if( newID.IsEmpty() )
	{
		MessageBox( L"삭제할 계정을 선택해 주세요!", L"알림", MB_OK );
		return;
	}

	message.Format( L"[%s]계정을 정말 삭제하시겠습니까?", newID.GetBuffer() );
	if( MessageBox( message.GetBuffer(), L"알림", MB_YESNO ) == IDNO )
		return;

	//theApp.m_kImp.DoQueryDelAccount( newID.GetBuffer() );
	UpdateAccountList();
}

void CKAccountDlg::OnLvnItemActivateAccountList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMIA = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.	
	CListCtrl* pkListEsd = (CListCtrl*) GetDlgItem( IDC_ACCOUNT_LIST );	
	CString id, pw;
	id = pkListEsd->GetItemText( pkListEsd->GetSelectionMark(), 0 );
	pw = pkListEsd->GetItemText( pkListEsd->GetSelectionMark(), 1 );
	int authLevel = theApp.m_kImp.GetAccountInfo( (pkListEsd->GetItemCount()-1) - pkListEsd->GetSelectionMark() ).m_authLevel;
	m_newIdEdit.SetWindowText( id.GetBuffer() );
	m_newPwEdit.SetWindowText( pw.GetBuffer() );
	m_rePwEdit.SetWindowText( L"" );
	m_authLevel.SetCurSel( authLevel );
	*pResult = 0;
}