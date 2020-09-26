#pragma once

class KX2DTWorldManager : public CX2WorldManager
{
public:
	KX2DTWorldManager(void);
	virtual ~KX2DTWorldManager(void);

	bool OpenScriptFile( const WCHAR* pFileName );

	KX2DTWorld* CreateWorld( CX2World::WORLD_ID worldID );
	void GetWorldNameList( std::map< CX2World::WORLD_ID, std::wstring >& mapWorldNameList );
};