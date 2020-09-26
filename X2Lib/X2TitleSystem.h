#pragma once


class CX2TitleSystem
{   
public:
    struct TitleRecord
    {
        int         m_iTitleID;
        int         m_eTitleType; // Áö¿ª
        int         m_iSort;
        wstring     m_wstrTitleName;
        wstring     m_wstrTitleDesc;        

        int         m_nSecret; // ½Àµæ/½Àµæ°¡´É/???
        bool        m_bSecret; // ¼û±è¿©ºÎ

        // ¸¸·á±â°£
        bool        m_bPeriod;
        __int64     m_periodTime;
        wstring     m_wstrEndDate;

		TitleRecord() 
			: m_iTitleID( 0 ), m_eTitleType( 0 ), m_iSort( 0 ),
			m_wstrTitleName(), m_wstrTitleDesc(), m_nSecret( 0 ), m_bSecret( false ),
			m_bPeriod( false ), m_periodTime( 0 ), m_wstrEndDate()
		{}
    };

public:
    CX2TitleSystem( CKTDXStage* pNowState );
    virtual ~CX2TitleSystem();

    void RegisterLuaBind();

    static bool Cmp(const CX2TitleSystem::TitleRecord& r1, const CX2TitleSystem::TitleRecord& r2);
    bool GetShow() { return m_bShow; }
    void OpenTitle();
	void ClearTitle();
    void UpdateTitle();
    void CloseTitle();
    
    void AddRecord(TitleRecord aRecord);
    void ClearRecord();
    void UpdateRecord(TitleRecord aRecord);    
    void SortRecord();

    void ShowTitleList();
    void ShowTitleGuideDesc();

    int GetTitleState(int titleId);
    int GetSelectedTitle();
    bool SetSelectTitle(int val);

    wstring GetMissionDesc(TitleRecord* pData);
    void ShowTitleName(TitleRecord* pData);
    void ShowTitleDesc(TitleRecord* pData);

    void SetCheckTake();
    void ChangeTitleGroup(int iGroup);

    void CheckPeriod();
	void SetPreviewTitle(wstring titleName);

protected:
    
    CX2State*				m_pNowState;

    CKTDGUIDialogType       m_pDlgTitle;
    
private:
    bool                    m_bShow;
    bool                    m_bTakeTitle;
    int                     m_iGroup;
    std::vector<TitleRecord>            m_vecTitleRecord;    
};
