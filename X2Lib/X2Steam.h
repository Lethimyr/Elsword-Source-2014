#pragma once

#ifdef SERV_STEAM

class CX2Steam
{
public:
	static bool InitSteam();

	static void RunSteamCallback();

	static bool IsSteamUser();

	static bool IsValidSteamUser( const std::wstring wstrPW_ );

	static bool GetSteamUserIDAndPass( OUT std::wstring& wstrID_, OUT std::wstring& wstrPw_ );

	static std::wstring GetSteamUserID();

	static void OpenSteamOverlayWebPage( IN std::wstring& wstrURL_ );

	static void EndSteam();

protected:
};
#endif //SERV_STEAM