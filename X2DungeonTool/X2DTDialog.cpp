#include "dxstdafx.h"
#include ".\x2dtdialog.h"

KX2DTDialog::KX2DTDialog(void)
{
}

KX2DTDialog::~KX2DTDialog(void)
{
}

HRESULT KX2DTDialog::AddEditBox( int ID, LPCWSTR strText, int x, int y, int width, int height, bool bIsDefault, CDXUTEditBox** ppCreated )
{
	HRESULT hr = S_OK;

	CDXUTEditBox *pEditBox = new KX2DTEditBox( this );

	if( ppCreated != NULL )
		*ppCreated = pEditBox;

	if( pEditBox == NULL )
		return E_OUTOFMEMORY;

	hr = AddControl( pEditBox );
	if( FAILED(hr) )
		return hr;

	// Set the ID and position
	pEditBox->SetID( ID ); 
	pEditBox->SetLocation( x, y );
	pEditBox->SetSize( width, height );
	pEditBox->m_bIsDefault = bIsDefault;

	if( strText )
		pEditBox->SetText( strText );

	return S_OK;
}

HRESULT KX2DTDialog::AddListBox( int ID, int x, int y, int width, int height, DWORD dwStyle, CDXUTListBox** ppCreated )
{
	HRESULT hr = S_OK;
	CDXUTListBox *pListBox = new KX2DTListBox( this );

	if( ppCreated != NULL )
		*ppCreated = pListBox;

	if( pListBox == NULL )
		return E_OUTOFMEMORY;

	hr = AddControl( pListBox );
	if( FAILED(hr) )
		return hr;

	// Set the ID and position
	pListBox->SetID( ID );
	pListBox->SetLocation( x, y );
	pListBox->SetSize( width, height );
	pListBox->SetStyle( dwStyle );

	return S_OK;
}

void KX2DTDialog::MoveUIPos( int xMove, int yMove )
{
	if( xMove == 0  &&  yMove == 0 )
		return;

	POINT p;
	GetLocation( p );
	SetLocation( p.x+xMove, p.y+yMove );
}