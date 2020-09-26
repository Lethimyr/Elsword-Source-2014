#pragma once

class CSystemInfo
{
	public:
								CSystemInfo ();
		virtual				   ~CSystemInfo ();

		void					Init();
		const unsigned char*	GetSupportId();
		const unsigned char*	GetMachineId();
		DWORD					GetGameRoomClient();

	protected:
		unsigned char			SupportId[16];
		unsigned char			MachineId[16];
};
