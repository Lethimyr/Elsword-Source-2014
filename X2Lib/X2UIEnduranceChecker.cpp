// 2009.01.29 김태완
// 내구도 표시 UI.

#include "StdAfx.h"



#include ".\X2UIEnduranceChecker.h"

CX2UIEnduranceChecker::CX2UIEnduranceChecker() :
m_bShow(false),
m_pDLGUIEnduranceChecker(NULL)
{
	// 루아 읽어오고
	m_pDLGUIEnduranceChecker = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_UI_Equip_Durability.lua" );
	m_pDLGUIEnduranceChecker->SetShowEnable(false, false);
	g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGUIEnduranceChecker );	

}

CX2UIEnduranceChecker::~CX2UIEnduranceChecker(void)
{
	SAFE_DELETE_DIALOG( m_pDLGUIEnduranceChecker );	
}

void CX2UIEnduranceChecker::SetLayer( X2_DIALOG_LAYER layer )
{
	if(m_pDLGUIEnduranceChecker != NULL) 
	{
		g_pKTDXApp->GetDGManager()->GetDialogManager()->ChangeLayer(m_pDLGUIEnduranceChecker, layer);
		g_pKTDXApp->GetDGManager()->GetDialogManager()->ChangeSequence( m_pDLGUIEnduranceChecker, false );
	}
}

void CX2UIEnduranceChecker::SetShow(bool val)
{

	if(val && Update())	// 켜질 때 처리해야 할 부분
	{
		m_pDLGUIEnduranceChecker->SetShowEnable(true, true);
		m_bShow = true;
	}
	else	// 꺼질 때 처리해야 할 부분
	{
		m_pDLGUIEnduranceChecker->SetShowEnable(false, false);
		m_bShow = false;
	}
}

bool CX2UIEnduranceChecker::Update()
{
	if ( g_pData == NULL )
		return false;
	if( g_pData != NULL &&  g_pData->GetMyUser() == NULL )
		return false;	
	if( g_pData != NULL &&  g_pData->GetMyUser() != NULL && g_pData->GetMyUser()->GetSelectUnit() == NULL )
		return false;

	const CX2Unit::UnitData* pMyUnitData = &g_pData->GetMyUser()->GetSelectUnit()->GetUnitData();
	const CX2Inventory& kInventory = g_pData->GetMyUser()->GetSelectUnit()->GetInventory();

	bool bDamagedEquipExist = false;
	D3DXVECTOR2 posOffset(0.0f, 0.0f);
	{
		// 일단 싹 꺼주고..
		CKTDGUIStatic* pStatic;
		pStatic = (CKTDGUIStatic*) m_pDLGUIEnduranceChecker->GetControl( L"Equip_Coat_Durability" );
		if(pStatic != NULL)
		{
			pStatic->GetPicture(0)->SetShow(false);
			pStatic->GetPicture(1)->SetShow(false);
		}
		pStatic = (CKTDGUIStatic*) m_pDLGUIEnduranceChecker->GetControl( L"Equip_Pant_Durability" );
		if(pStatic != NULL)	
		{
			pStatic->GetPicture(0)->SetShow(false);
			pStatic->GetPicture(1)->SetShow(false);
		}
        pStatic = (CKTDGUIStatic*) m_pDLGUIEnduranceChecker->GetControl( L"Equip_Glover_Durability" );
		if(pStatic != NULL)
		{
			pStatic->GetPicture(0)->SetShow(false);
			pStatic->GetPicture(1)->SetShow(false);
		}
		pStatic = (CKTDGUIStatic*) m_pDLGUIEnduranceChecker->GetControl( L"Equip_Shoes_Durability" );
		if(pStatic != NULL)
		{
			pStatic->GetPicture(0)->SetShow(false);
			pStatic->GetPicture(1)->SetShow(false);
		}
		pStatic = (CKTDGUIStatic*) m_pDLGUIEnduranceChecker->GetControl( L"Equip_Weapon_Durability" );
		if(pStatic != NULL)
		{
			pStatic->GetPicture(0)->SetShow(false);
			pStatic->GetPicture(1)->SetShow(false);
		}

		for ( int i = 0; i < (int)pMyUnitData->m_NowEqipItemUIDList.size(); i++ )
		{
			UidType uidType = pMyUnitData->m_NowEqipItemUIDList[i];
			CX2Item* pItem = kInventory.GetItem( uidType );
			if ( pItem == NULL )
				continue;

			// 아이템 받아와서 'ㅅ'
			const CX2Item::ItemTemplet* pItemTemplet = pItem->GetItemTemplet();
			if(pItemTemplet == NULL) 
				continue;

			if ( pItemTemplet->GetPeriodType() == CX2Item::PT_ENDURANCE )
			{
				int Endurance = pItem->GetItemData().m_Endurance;
				int PicTurnOn = 0;
				// 장비템이고..
				if( Endurance <= 0 )
				{
					// 깨진상황
					bDamagedEquipExist = true;
					PicTurnOn = 1;
				}
				else if( Endurance <= pItemTemplet->GetEndurance() * 0.2 )
				{
					// 노란색
					bDamagedEquipExist = true;
					PicTurnOn = 0;
				}
				else
				{
					// 멀쩡
					continue;
				}
				
				CKTDGUIStatic* pStatic = NULL;

				// 해당 스태틱 받아와서
				switch(pItemTemplet->GetEqipPosition())
				{
				case CX2Unit::EP_DEFENCE_BODY:
					{
						pStatic = (CKTDGUIStatic*) m_pDLGUIEnduranceChecker->GetControl( L"Equip_Coat_Durability" );
					}break;
				case CX2Unit::EP_DEFENCE_LEG:
					{
						pStatic = (CKTDGUIStatic*) m_pDLGUIEnduranceChecker->GetControl( L"Equip_Pant_Durability" );
					}break;
				case CX2Unit::EP_DEFENCE_HAND:
					{
                        pStatic = (CKTDGUIStatic*) m_pDLGUIEnduranceChecker->GetControl( L"Equip_Glover_Durability" );
					}break;
				case CX2Unit::EP_DEFENCE_FOOT:
					{
						pStatic = (CKTDGUIStatic*) m_pDLGUIEnduranceChecker->GetControl( L"Equip_Shoes_Durability" );
					}break;
				case CX2Unit::EP_WEAPON_HAND:
					{
						pStatic = (CKTDGUIStatic*) m_pDLGUIEnduranceChecker->GetControl( L"Equip_Weapon_Durability" );
					}break;
				default:
					break;
				}

				if(pStatic != NULL)
				{
					const int MAGIC_OFFSET = 2;
					pStatic->GetPicture(PicTurnOn)->SetShow(true);
					pStatic->GetPicture(PicTurnOn)->SetPos( posOffset );
					posOffset.x = posOffset.x + (pStatic->GetPicture(PicTurnOn)->GetSize().x + MAGIC_OFFSET);
					//pStatic->SetShow(true);
				}				
			}
		}
	}
	
	return bDamagedEquipExist;

}


