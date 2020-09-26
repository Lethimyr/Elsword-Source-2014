#pragma once

class CXSLSquareGame
{
public:
	enum SQUARE_TYPE
	{
		ST_NONE = 0,
		ST_AREA,
		ST_AGE,
		ST_FREE,
		ST_TRADE,
		ST_END,
	};

public:
	CXSLSquareGame();
	~CXSLSquareGame();
};