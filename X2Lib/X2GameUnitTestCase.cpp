#include "StdAfx.h"
#include ".\x2gameunittestcase.h"

#ifdef CPPUNIT_BY_TOOL_TEAM

CPPUNIT_TEST_SUITE_REGISTRATION( CX2GameUnitTestCase );

CX2GameUnitTestCase::CX2GameUnitTestCase(void)
{
}

CX2GameUnitTestCase::~CX2GameUnitTestCase(void)
{
}

void CX2GameUnitTestCase::testUpHPIfAlive()
{
	float result = 3.f * 2.f;
	CPPUNIT_ASSERT( result == 6.f );
}


#endif CPPUNIT_BY_TOOL_TEAM