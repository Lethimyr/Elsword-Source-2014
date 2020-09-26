#ifdef	X2OPTIMIZE_ITEM_TEMPLET_PREPROCESSING

#ifdef SERV_ITEM_LUA_TRANS_DEVIDE
struct ItemTransData
{
	std::wstring m_Name;
	std::wstring m_Description;
	std::wstring m_DescriptionInShop;
	std::wstring m_DescriptionInSkillNote;
	bool		 m_bItemTransData;

	ItemTransData()
	{
		m_Name = L"";
		m_Description = L"";
		m_DescriptionInShop = L"";
		m_DescriptionInSkillNote = L"";
		m_bItemTransData = false;
	}
};
#endif //SERV_ITEM_LUA_TRANS_DEVIDE

struct	KProxy
{
	std::set<DWORD>		m_setItemIDs;
    std::set<DWORD>     m_setSetIDs;
	bool	AddItemTemplet_LUA();
    bool    AddSetItemData_LUA();
#ifdef SERV_ITEM_LUA_TRANS_DEVIDE
	std::map<DWORD, ItemTransData>		m_mapItemTrans;
	std::map<DWORD, std::wstring>		m_mapSetItemTrans;
    bool    AddItemTempletTrans_LUA();
    bool    AddSetItemDataTrans_LUA();
#endif //SERV_ITEM_LUA_TRANS_DEVIDE
};//

struct  D3DVECTOR3Compare
{
    bool operator() ( const D3DXVECTOR3& lhs_, const D3DXVECTOR3& rhs_ ) const
    {
        if ( lhs_.x < rhs_.x )
            return true;
        else if ( lhs_.x > rhs_.x )
            return false;
        if ( lhs_.y < rhs_.y )
            return true;
        else if ( lhs_.y > rhs_.y )
            return false;
        return lhs_.z < rhs_.z;
    }
};
typedef std::map<D3DXVECTOR3,DWORD,D3DVECTOR3Compare>   KD3DXVECTOR3OffsetMap;
struct  KProxy2
{
    struct  KSetItemData
    {
        int                                 m_iMaxLevel;
		std::vector<DWORD>	                m_vecSetItemIDList;
        std::map< DWORD, vector< int > >	m_mapNeedPartsNumNOptions;
        
        KSetItemData() : m_iMaxLevel(0) {}
    };//
    typedef std::map<DWORD,KSetItemData>    KSetItemDataMap;
    KFileSerializer             m_kFileSerializer;
    KD3DXVECTOR3OffsetMap       m_mapVectorOffset;
    std::set<DWORD>             m_setSetIDs;
    KSetItemDataMap             m_mapSetItemDataMap;
#ifdef SERV_ITEM_LUA_TRANS_DEVIDE
	std::map<DWORD, ItemTransData>	m_mapItemTrans;
	std::map<DWORD, std::wstring>	m_mapSetItemTrans;
	KProxy2( const std::set<DWORD>& setItemIDs, const std::set<DWORD>& setSetIDs, const std::map<DWORD, ItemTransData>& mapItemTrans, const std::map<DWORD, std::wstring>& mapSetItemTrans );
#else //SERV_ITEM_LUA_TRANS_DEVIDE
    KProxy2( const std::set<DWORD>& setItemIDs, const std::set<DWORD>& setSetIDs );
#endif //SERV_ITEM_LUA_TRANS_DEVIDE
    bool    AddItemTemplet_LUA();
    bool    AddSetItemData_LUA();
    DWORD   AppendD3DXVECTOR3( const D3DXVECTOR3& v, bool bDefaultOne = false );
    void    PostProcess();
};//

typedef CX2Item::KItemFormatSetItemData SetItemData;

unsigned int    GetItemNum() const;
DWORD   GetItemIDFromIndex( unsigned uiIndex ) const;
int     GetIndexFromItemID( int iItemID ) const;
const CX2Item::ItemTemplet*     GetItemTempletFromIndex( unsigned uiIndex ) const;
const CX2Item::ItemTemplet* GetItemTemplet( const int itemID ) const;
static bool	TokenizeByScaleRotate( const std::string& strTokenInfo, D3DXVECTOR3& vOutScale, D3DXVECTOR3& vOutRotate );

unsigned int    GetNumSetIDs() const;
const CX2Item::KItemFormatSetItemData*          GetSetItem( DWORD dwSetID ) const;
int     GetSetItemOptions( DWORD dwSetID, const int iNumOfEquippedItems_, IN vector< int >& vecOptions ) const;

static void RegisterProxyLuabind( lua_State* L );


#endif	X2OPTIMIZE_ITEM_TEMPLET_PREPROCESSING