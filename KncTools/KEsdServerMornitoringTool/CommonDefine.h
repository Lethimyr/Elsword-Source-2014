#pragma once

#define ID_SIZE		10
#define PW_SIZE		20
#define TIMER_SEC	100

enum AUTH_LEVEL
{
	NORMAL = 0,
	SUPER,
	ADMIN,
};

const wchar_t* GetAuthString( int iAuthLevel );