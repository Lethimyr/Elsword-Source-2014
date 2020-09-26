#pragma once

class CX2UIUnitManager
{

public:
	CX2UIUnitManager();
	virtual ~CX2UIUnitManager();

	void Initialize();
	CX2UIUnitPtr CreateUIUnit( int iUnitID );
	CX2UIUnit* GetUIUnit( int iUnitID );
	void ClearAllUIUnit();


protected:
	std::map< int, CX2UIUnitPtr > m_mapUIUnit;

};