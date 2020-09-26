/** @file   KTDGUIDialogHandle.cpp
    @desc   implementation of KTDGUIDialogHandle
    @author jintaeks@kogstudios.com
    @since  2009-8-18, 17:58
*/

#include "StdAfx.h"
#include "KTDGUIDialogHandle.h"
#include "KTDGUIDialog.h"


static CKTDGUIDialog   fs_uiDialogNullObject( NULL ); // null object for safe 'CKTDGUIDialogHandle::operator->()'


#ifdef DIALOG_HANDLE_TEST // 2009-8-19 by jintaeks
CKTDGUIDialogHandle& CKTDGUIDialogHandle::operator=(CKTDGUIDialog* pControl_)
{
    if( pControl_ != NULL )
    {
        m_handle = pControl_->GetUID();
    }
    else
    {
        m_handle.Invalidate();
    }//if.. else..
    return *this;
}//CKTDGUIDialogHandle::operator=()


CKTDGUIDialog* CKTDGUIDialogHandle::operator->()
{
	KTDXPROFILE();

    CKTDGUIDialog* pControl = CKTDGUIDialog::GetObjectByUID( m_handle );
    //ASSERT( pControl != NULL );
    /** dialog에 대한 포인터가 null인 경우, null object를 리턴해서 프로그램이 죽지 않도록
        한다. operator->()는 항상 valid 한 포인터를 리턴하므로 null 포인터 참조로 인한 crash는
        발생하지 않는다.
        - jintaeks on 2009-08-21, 11:25 */
    return pControl != NULL ? pControl : &fs_uiDialogNullObject;
}//CKTDGUIDialogHandle::operator->()


const CKTDGUIDialog* CKTDGUIDialogHandle::operator->() const
{
    const CKTDGUIDialog* pControl = CKTDGUIDialog::GetObjectByUID( m_handle );
    ASSERT( pControl != NULL );
    /** dialog에 대한 포인터가 null인 경우, null object를 리턴해서 프로그램이 죽지 않도록
        한다. operator->()는 항상 valid 한 포인터를 리턴하므로 null 포인터 참조로 인한 crash는
        발생하지 않는다.
        - jintaeks on 2009-08-21, 11:25 */
    return pControl != NULL ? pControl : &fs_uiDialogNullObject;
}//CKTDGUIDialogHandle::operator->()


//CKTDGUIDialogHandle::operator CKTDGUIDialog*()
//{
//    return CKTDGUIDialog::GetObjectByUID( m_handle );
//}//CKTDGUIDialogHandle::operator CKTDGUIDialog*()


///-----------------------------------------------------------------------------
// global helper functions
//
bool operator!=( const CKTDGUIDialog* pLeftControl_, const CKTDGUIDialogHandle rightControl_ )
{
    const CKTDGUIDialog* pRightControl = NULL;
    if( rightControl_.IsValid() && CKTDGUIDialog::IsValidUID(rightControl_.GetHandle()) )
    {
        pRightControl = CKTDGUIDialog::GetObjectByUID( rightControl_.GetHandle() );
        return pLeftControl_ != pRightControl;
    }//if

    return pLeftControl_ != pRightControl;
}//operator!=()


bool operator!=( const CKTDGUIDialogHandle leftControl_, const CKTDGUIDialog* pRightControl_ )
{
    const CKTDGUIDialog* pLeftControl = NULL;
    if( leftControl_.IsValid() && CKTDGUIDialog::IsValidUID(leftControl_.GetHandle()) )
    {
        pLeftControl = CKTDGUIDialog::GetObjectByUID( leftControl_.GetHandle() );
        return pLeftControl != pRightControl_;
    }//if

    return pLeftControl != pRightControl_;
}//operator!=()


bool operator!=( const CKTDGUIDialogHandle leftControl_, const CKTDGUIDialogHandle rightControl_ )
{
    return leftControl_.GetHandle() != rightControl_.GetHandle();
}//operator!=()


bool operator==( const CKTDGUIDialog* pLeftControl_, const CKTDGUIDialogHandle rightControl_ )
{
    const CKTDGUIDialog* pRightControl = NULL;
    if( rightControl_.IsValid() && CKTDGUIDialog::IsValidUID(rightControl_.GetHandle()) )
    {
        pRightControl = CKTDGUIDialog::GetObjectByUID( rightControl_.GetHandle() );
        return pLeftControl_ == pRightControl;
    }//if

    return pLeftControl_ == pRightControl;
}//operator==()


bool operator==( const CKTDGUIDialogHandle leftControl_, const CKTDGUIDialog* pRightControl_ )
{
    const CKTDGUIDialog* pLeftControl = NULL;
    if( leftControl_.IsValid() && CKTDGUIDialog::IsValidUID(leftControl_.GetHandle()) )
    {
        pLeftControl = CKTDGUIDialog::GetObjectByUID( leftControl_.GetHandle() );
        return pLeftControl == pRightControl_;
    }//if

    return pLeftControl == pRightControl_;
}//operator==()


bool operator==( const CKTDGUIDialogHandle leftControl_, const CKTDGUIDialogHandle rightControl_ )
{
    return leftControl_.GetHandle() == rightControl_.GetHandle();
}//operator==()
#endif // DIALOG_HANDLE_TEST // 2009-8-19
