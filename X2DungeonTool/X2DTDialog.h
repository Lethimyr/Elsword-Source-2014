#pragma once

class KX2DTDialog : public CDXUTDialog
{
public:
	KX2DTDialog(void);
	virtual ~KX2DTDialog(void);

	HRESULT AddEditBox( int ID, LPCWSTR strText, int x, int y, int width, int height, bool bIsDefault=false, CDXUTEditBox** ppCreated=NULL );
	HRESULT AddListBox( int ID, int x, int y, int width, int height, DWORD dwStyle=0, CDXUTListBox** ppCreated=NULL );

	KX2DTListBox*     GetListBox( int ID ) { return (KX2DTListBox*) GetControl( ID, DXUT_CONTROL_LISTBOX ); }
	void	MoveUIPos( int xMove, int yMove );
};