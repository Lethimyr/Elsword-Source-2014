#pragma once

class KX2DTEditBox : public CDXUTEditBox
{
public:
	KX2DTEditBox( CDXUTDialog *pDialog = NULL );
	virtual ~KX2DTEditBox(void);

	virtual bool MsgProc( UINT uMsg, WPARAM wParam, LPARAM lParam );

	void SetIsOnlyNumber( bool val ) { m_bIsOnlyNumber = val; }
	void SetMinusSign( bool val ) { m_bMinusSign = val; }

private:
	bool		m_bIsOnlyNumber;
	bool		m_bMinusSign;
};