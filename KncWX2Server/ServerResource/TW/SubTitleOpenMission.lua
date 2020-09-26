-- lua header. UTF-8 인코딩 인식을 위해 이 줄은 지우지 마세요.

TitleManager:AddSubTitleMissionInfo_LUA
{
    m_iID = 30009,
    m_wstrDescription = "",
    m_eClearType = TITLE_MISSION_CLEAR_TYPE["TMCT_DUNGEON_CLEAR_COUNT"],
    m_bAutomaticDescription = True,
    m_ClearCondition = 
    {
        m_eDungeonID = DUNGEON_ID["DI_ALTERA_CORE_NORMAL"],
        m_cDifficulty = DIFFICULTY_LEVEL["DL_NULL"],
        m_iDungeonClearCount = 1,
    },
}

TitleManager:AddSubTitleMissionInfo_LUA
{
    m_iID = 30010,
    m_wstrDescription = "",
    m_eClearType = TITLE_MISSION_CLEAR_TYPE["TMCT_DUNGEON_CLEAR_COUNT"],
    m_bAutomaticDescription = True,
    m_ClearCondition = 
    {
        m_eDungeonID = DUNGEON_ID["DI_ALTERA_CORE_NORMAL"],
        m_cDifficulty = DIFFICULTY_LEVEL["DL_EXPERT"],
        m_iDungeonClearCount = 1,
    },
}