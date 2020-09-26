#pragma once

//note : 한국 nProtect 모듈

#define CHECK_KNP_TIME	5000

extern CNPGameLib*	pNpgl;


bool	CheckNPHackUser();

#ifdef ALWAYS_INTERNAL_NPGE_PACKING
bool	KNP_Init( HINSTANCE hInst , string TempArg);
#else //ALWAYS_INTERNAL_NPGE_PACKING
bool	KNP_Init( HINSTANCE hInst );
#endif ALWAYS_INTERNAL_NPGE_PACKING

void	KNP_Close();

void	KNP_SetHwnd( HWND hWnd );

bool	KNP_GetFindGameMon();

DWORD	KNP_Send( LPCWSTR wstrMSG );

void	KNP_AUTH2( GG_AUTH_DATA* pAuthData );

#ifdef SERV_NPROTECT_CS_AUTH_30
void	KNP_AUTH3( BYTE *pbyteCSAuth3, UINT32 uReceivedSize );
#endif SERV_NPROTECT_CS_AUTH_30

BOOL CALLBACK NPGameMonCallback( DWORD dwMsg, DWORD dwArg );