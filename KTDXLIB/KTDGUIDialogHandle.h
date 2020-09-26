/** @file   KTDGUIDialogHandle.h
    @desc   interface of KTDGUIDialogHandle
    @author jintaeks@kogstudios.com
    @since  2009-8-18, 17:58
*/

#pragma once
#ifndef _KTDGUIDIALOGHANDLE_H
#define _KTDGUIDIALOGHANDLE_H


class CKTDGUIDialog;


/** @class  CKTDGUIDialogHandle
@date   jintaeks on 2009-08-18, 17:17
*/
class CKTDGUIDialogHandle
{
public:
    /// constructor.
    CKTDGUIDialogHandle()
    {
    }

#ifdef DIALOG_HANDLE_TEST // 2009-8-19 by jintaeks
    /// constructor to port raw pointer interface.
    CKTDGUIDialogHandle(CKTDGUIDialog* pControl_)
    {
        operator=( pControl_ );
    }

    explicit CKTDGUIDialogHandle(const KHandle& handle_)
    {
        m_handle = handle_;
    }

    CKTDGUIDialogHandle& operator=(CKTDGUIDialog* pControl_);

    CKTDGUIDialog* operator->();
    const CKTDGUIDialog* operator->() const;

    //operator CKTDGUIDialog*();


	bool operator < (const CKTDGUIDialogHandle& rhs ) const
	{
		if( GetHandle() < rhs.GetHandle() )
			return true;

		return false;
	}

    bool            IsValid() const { return m_handle.IsValid(); }
    const KHandle&  GetHandle() const { return m_handle; }
    void            Invalidate() { m_handle.Invalidate(); }
#endif // DIALOG_HANDLE_TEST // 2009-8-19

private:
    KHandle m_handle;
};//struct CKTDGUIDialogHandle


bool operator!=( const CKTDGUIDialog* pLeftControl_, const CKTDGUIDialogHandle rightControl_ );
bool operator!=( const CKTDGUIDialogHandle leftControl_, const CKTDGUIDialog* pRightControl_ );
bool operator!=( const CKTDGUIDialogHandle leftControl_, const CKTDGUIDialogHandle rightControl_ );
bool operator==( const CKTDGUIDialog* pLeftControl_, const CKTDGUIDialogHandle rightControl_ );
bool operator==( const CKTDGUIDialogHandle leftControl_, const CKTDGUIDialog* pRightControl_ );
bool operator==( const CKTDGUIDialogHandle leftControl_, const CKTDGUIDialogHandle rightControl_ );

#endif // _KTDGUIDIALOGHANDLE_H
