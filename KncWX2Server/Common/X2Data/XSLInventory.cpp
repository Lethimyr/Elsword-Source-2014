#include ".\xslinventory.h"


bool CXSLInventory::IsTradePossibleCategory( char cSlotCategory )
{
	if( cSlotCategory < CXSLInventory::ST_EQUIP  ||  cSlotCategory > CXSLInventory::ST_AVARTA )
		return false;

	return true;
}

bool CXSLInventory::IsValidBankMemberShip( MEMBERSHIP_PRIVATE_BANK eGrade )
{
	switch( eGrade )
	{
	case MPB_NORMAL:
	case MPB_SILVER:
	case MPB_GOLD:
	case MPB_EMERALD:
	case MPB_DIAMOND:
	case MPB_PLATINUM:
		return true;
	}

	return false;
}

bool CXSLInventory::IsUserInventoryCategory( char cSlotCategory )
{
    switch( cSlotCategory )
	{
	case ST_EQUIP:
	case ST_ACCESSORY:
	case ST_MATERIAL:
	case ST_SPECIAL:
	case ST_QUEST:
	case ST_QUICK_SLOT:
	case ST_AVARTA:		
	//case ST_PET: -- [육사comment] 펫 인벤토리는 포함시키지 않습니다! 포함시킬경우 많은 예외상황에 생기므로 참고 바랍니다! 기획파트와 반드시 의논!
		return true;
	}

	return false;
}

CXSLInventory::MEMBERSHIP_PRIVATE_BANK CXSLInventory::GetNextBankMemberShip( MEMBERSHIP_PRIVATE_BANK eGrade )
{
	switch( eGrade )
	{
	case MPB_NORMAL:	return MPB_SILVER;
	case MPB_SILVER:	return MPB_GOLD;
	case MPB_GOLD:		return MPB_EMERALD;
	case MPB_EMERALD:	return MPB_DIAMOND;
	case MPB_DIAMOND:	return MPB_PLATINUM;
	case MPB_PLATINUM:	return MPB_NONE;		
	}

	return MPB_NONE;
}

