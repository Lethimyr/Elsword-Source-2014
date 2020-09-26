#pragma once

#include <ToString.h>
#include <KNCSingleton.h>

class KChannelChangeManager
{
	DeclareSingleton( KChannelChangeManager );
	DeclDump;
	DeclToStringW;

public:
	KChannelChangeManager(void);
	~KChannelChangeManager(void);	

private:
};

DefSingletonInline( KChannelChangeManager );


