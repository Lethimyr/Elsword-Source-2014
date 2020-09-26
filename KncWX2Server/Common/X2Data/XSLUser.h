#pragma once

#include "XSLStat.h"

class CXSLUser
{
	public:
		struct UserData
		{
			int					userUID;			//유저의 유니크 아이디
			std::wstring		userID;				//아이디
			std::wstring		userNickName;		//닉네임
			std::wstring		userName;			//실명

			int			maxUnitCount;		//내가 만들 수 있는 캐릭터 수(가지고 있는것 포함)
			int			cash;				//소유캐쉬

			UserData()
			{
				userUID			= 0;
				maxUnitCount	= 0;
				cash			= 0;
			}

			UserData( const UserData& data ){ *this = data; }

			UserData& operator=( const UserData& data )
			{
				userUID			= data.userUID;				//유저의 유니크 아이디
				userID			= data.userID;				//아이디
				userNickName	= data.userNickName;		//닉네임
				userName		= data.userName;

				maxUnitCount	= data.maxUnitCount;		//내가 만들 수 있는 캐릭터 수(가지고 있는것 포함)
				cash			= data.cash;

				return *this;
			}

			//UserData& operator=( const KDB_EVENT_X2_SERVER_READ_USER_DATA_ACK& data )
			//{
			//	userUID			= data.userInfor.userUID;			//유저의 유니크 아이디
			//	userID			= data.userInfor.userID;			//아이디
			//	userNickName	= data.userInfor.userNickName;		//닉네임
			//	userName		= data.userName;					//실명

			//	maxUnitCount	= data.userInfor.maxUnitCount;		//내가 만들 수 있는 캐릭터 수(가지고 있는것 포함)
			//	cash			= data.userInfor.cash;

			//	return *this;
			//}
		};

	public:
		CXSLUser(void);
		~CXSLUser(void);
};
