#pragma once


class CX2ComboGuide
{
public:
	struct ComboCommand
	{	
		int		m_iIndex;
		int		m_offsetX;
		int		m_offsetY;
		char	m_cCommandKey;		
		wstring m_wstrFullCommand;

		ComboCommand()
		{
			m_iIndex = 0;
			m_offsetX = 0;
			m_offsetY = 0;
			m_cCommandKey = 'N';			
			m_wstrFullCommand = L"N";

		};			
	};

	struct ComboGuide
	{
		wstring m_wstrComboId;
		vector<ComboCommand> m_vecCommand;

		void SetComboId( wstring comboId )
		{
			m_wstrComboId = comboId;
		};

		void AddComboCommand(char cKey, int offsetX, int offsetY, wstring wstrFullCommand)
		{
			ComboCommand comboCommand;

			comboCommand.m_iIndex = 0;
			comboCommand.m_offsetX = offsetX;
			comboCommand.m_offsetY = offsetY;
			comboCommand.m_cCommandKey = cKey;			
			comboCommand.m_wstrFullCommand = wstrFullCommand;

			m_vecCommand.push_back( comboCommand );
		};
	};

	CX2ComboGuide(void);
	virtual ~CX2ComboGuide(void);	

	void InitComboGuide( CX2Unit::UNIT_CLASS eClass );
	
	void CreateComboGuide( CX2ComboGuide::ComboGuide combo );
	void ShowComboCommand( wstring wstrComboId, wstring wstrCommand, bool bShow = true );

	void SetShow(bool val);
	void SetPos(D3DXVECTOR2 &vPos);

private:
	CKTDGUIDialogType	m_DlgComboGuide;
	vector<ComboGuide>	m_vecComboGuide;

	bool				m_bInitedGuide;	// 중복 생성 방지
};


