#pragma once

class CX2PVPGameTeam : public CX2PVPGame
{
	public:
		CX2PVPGameTeam(void);
		virtual ~CX2PVPGameTeam(void);

		virtual	void		Release();
		virtual HRESULT		OnFrameMove( double fTime, float fElapsedTime );

		bool				DetermineLastKill();
};
