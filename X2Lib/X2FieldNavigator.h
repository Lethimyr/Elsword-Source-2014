#ifdef FIELD_NAVIGATOR
#pragma once
class CX2FieldNavigator
{
public:
	enum MAP_TYPE
	{
		MT_INVALID	= 0,
		MT_VILLAGE,
		MT_BATTLE_FIELD,
	};
	struct FieldMapLinkInfo
	{
		//UINT		uiMapID;		// 필드, 배틀필드를 모두 담는 ID
		UINT		uiMoveMapID;	// 포탈을 통해 이동 가능한 맵ID
		UINT		uiPortalPosID;	// 포탈위치ID(필드, 배틀필드냐에 따라 다른 값을 가짐)
		MAP_TYPE	eMapType;		// 맵타입(필드, 배틀필드)

		FieldMapLinkInfo(UINT uiMoveMapID_, UINT uiPortalPosID_, MAP_TYPE eMapType_ = MT_BATTLE_FIELD):
		uiMoveMapID(uiMoveMapID_), uiPortalPosID(uiPortalPosID_), eMapType(eMapType_){}
	};
public:
	CX2FieldNavigator(void);
	~CX2FieldNavigator(void);

	void	InsertFieldMapLinkInfo(FieldMapLinkInfo MapLinkInfo);
	void	InsertFieldMapLinkInfo(UINT uiMapID);
	void	ClearVecFieldMapLinkInfo(){m_vecFieldMapLinkInfo.clear();}
private:

	map<UINT, vector<FieldMapLinkInfo>>		m_mapFieldMapLinkInfo;
	vector<FieldMapLinkInfo>				m_vecFieldMapLinkInfo;
	bool									m_bAbleInsertMapLinkInfo;
	
};
#endif //FIELD_NAVIGATOR
