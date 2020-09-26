#include "StdAfx.h"
#include ".\ktdxtimer.h"





#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE

void    CKTDXCheckElapsedTime::OnFrameMove( float fElapsedTime )
{
	m_fSumOfElapsedTime += fElapsedTime;
}

#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE

void CKTDXCheckElapsedTime::OnFrameMove()
{
	m_fSumOfElapsedTime += g_pKTDXApp->GetElapsedTime();
}


#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE