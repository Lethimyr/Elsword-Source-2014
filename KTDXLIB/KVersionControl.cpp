#include "StdAfx.h"
#include ".\kversioncontrol.h"

static KVersionControl s_VersionControl;


KVersionControl::KVersionControl(void)
{
	s_CountryCode = CC_KR;
	s_MajorVersion = MajorV_IN_HOUSE;




	//////////////////////////////////////////////////////////////////////////
	AddNode( CN_ROOT, CN_SERVICE_READY, false );



	//////////////////////////////////////////////////////////////////////////
	AddNode( CN_ROOT, CN_IN_HOUSE_TEST_ONLY, false );
	AddNode( CN_IN_HOUSE_TEST_ONLY, CN_EVE_CLASS_CHANGE, true );







	//////////////////////////////////////////////////////////////////////////
	switch( GetCountryCode() )
	{
	case CC_KR:
		{
		} break;
	default:
		{
		} break;
	}




	//////////////////////////////////////////////////////////////////////////
	switch( s_MajorVersion )
	{
	case MajorV_SERVICE:
	case MajorV_SERVICE_OPEN_TEST:
		{
			SetEnabled( CN_SERVICE_READY, true );
			SetEnabled( CN_IN_HOUSE_TEST_ONLY, false );
		} break;

	case MajorV_IN_HOUSE:
		{
			SetEnabled( CN_SERVICE_READY, true );
			SetEnabled( CN_IN_HOUSE_TEST_ONLY, true );
		} break;

	case MajorV_NEW_VILLAGE_SERVICE:
	case MajorV_NEW_VILLAGE_OPEN_TEST:
		{
			SetEnabled( CN_SERVICE_READY, false );
			SetEnabled( CN_IN_HOUSE_TEST_ONLY, false );
		} break;
	}
}


KVersionControl::~KVersionControl(void)
{


}
