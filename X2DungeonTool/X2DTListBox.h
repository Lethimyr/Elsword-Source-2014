#pragma once

class KX2DTListBox : public CDXUTListBox
{
public:	
	KX2DTListBox( CDXUTDialog *pDialog = NULL );
	virtual ~KX2DTListBox(void);

	void RemoveAllItems( bool bIsRemoveSelectd = true );
	void LastItemSelect();
};
