#pragma once

#ifdef CPPUNIT_BY_TOOL_TEAM

#include <cppunit/extensions/HelperMacros.h>
#include "X2GageManager.h"


class CX2GameUnitTestCase : public CPPUNIT_NS::TestFixture
{
	CPPUNIT_TEST_SUITE( CX2GameUnitTestCase );
	CPPUNIT_TEST( testUpHPIfAlive );
	CPPUNIT_TEST_SUITE_END();

public:
	CX2GameUnitTestCase(void);
	~CX2GameUnitTestCase(void);

	virtual void setUp() 
	{
		
	};
	virtual void tearDown() 
	{

	};

protected: 
	void testUpHPIfAlive(); 

private: 
	CX2OldGageManager m_GageManager1; 
	CX2OldGageManager m_GageManager2; 

}; 

#endif CPPUNIT_BY_TOOL_TEAM
