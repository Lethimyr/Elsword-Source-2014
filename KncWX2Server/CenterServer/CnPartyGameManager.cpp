#include "CnPartyGameManager.h"


KCnPartyGameManager::KCnPartyGameManager()
{
	Reset();
}

KCnPartyGameManager::~KCnPartyGameManager()
{
}

void KCnPartyGameManager::Reset()
{
	m_usGameStartEventID = 0;
	m_sGameStartWorldID = 0;
}

void KCnPartyGameManager::SetGameStartInfo( IN const u_short usGameStartEventID, IN const short sGameStartWorldID )
{
    m_usGameStartEventID = usGameStartEventID;
	m_sGameStartWorldID = sGameStartWorldID;
}