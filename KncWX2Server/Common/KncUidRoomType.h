#pragma once
/* Room Type

| ff ff ff ff |

| ff 00 00 00 | 채널 id.
| 00 f0 00 00 | 퀘스트/대전.
| 00 0f 00 00 | 난이도/팀전, 서바이벌 등등.
| 00 00 ff 00 | 상위 제한 레벨.
| 00 00 00 ff | 하위 제한 레벨.
*/


//namespace KncRoomType 
//{
//	//Channel set
//	DWORD		GetChannelID( IN DWORD iUID_ );
//	void		SetChannelID( OUT DWORD& iUID_, IN DWORD nChannelID );
//
//	//Room Type set
//	DWORD		GetRoomType( IN DWORD iUID_ );
//	void		SetRoomType( OUT DWORD& iUID_, IN DWORD nRoomType );
//
//	//Game Type set
//	DWORD		GetGameType( IN DWORD iUID_ );
//	void		SetGameType( OUT DWORD& iUID_, IN DWORD nGameType );
//
//	//상위 레벨
//	DWORD		GetMaxLevel( IN DWORD iUID_ );
//	void		SetMaxLevel( OUT DWORD& iUID_, IN DWORD nMaxLevel );
//
//	//하위 레벨
//	DWORD		GetMinLevel( IN DWORD iUID_ );
//	void		SetMinLevel( OUT DWORD& iUID_, IN DWORD nMinLevel );
//}