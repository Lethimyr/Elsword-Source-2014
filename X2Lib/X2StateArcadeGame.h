#pragma once

class CX2StateArcadeGame : public CX2StateDungeonGame
{
public:
	CX2StateArcadeGame(void);
	virtual ~CX2StateArcadeGame(void);


protected:

	bool StateChangeLocalMapReq();


};
