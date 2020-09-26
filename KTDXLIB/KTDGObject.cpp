#include "StdAfx.h"
#include ".\ktdgobject.h"


// constructor.
CKTDGObject::CKTDGObject( CKTDGObject* pManagerObject_ )
: m_eObjectType( OT_UNKNOWN )
#ifdef  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
, m_uRefCounter( 0 )
#endif  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
, m_vScaleByUnit( 1, 1, 1 )

{

    m_pManagerObject        = pManagerObject_;
    if ( pManagerObject_ == NULL )
    {
        m_pMatrix           = new CKTDGMatrix( g_pKTDXApp->GetDevice() );
        ASSERT( m_pMatrix != NULL );
    }
    else    
    {
        m_pMatrix = NULL;
    }//if


    m_bAlphaObject      = false;
    m_fDistanceToCamera = 0.0f;
    m_Sphere.fRadius    = 0.0f;
    m_Sphere.center     = D3DXVECTOR3( 0,0,0 );
    m_bForceLayer       = false;
    m_Layer             = 0;
//{{ robobeg : 2008-10-13
    //m_RenderStateID       = 0;
//}} robobeg : 2008-10-13

    m_bOverUI           = false;
    m_bInnerUI          = false;
    m_bShowObject       = true;

//{{ robobeg : 2009-01-10
    m_bInstantShow = false;
//}} robobeg : 2009-01-10


    m_bCulled           = false;
    //{{ seojt: 2008.10.23
    //m_dwLastAccessTime  = 0.0;
    //}} seojt

//{{ robobeg : 2008-10-13
#ifdef CONVERSION_VS
	m_hKTDGManagerHandle = g_pKTDXApp->GetDGManager()->GetRenderObjectChainEnd();
#else CONVERSION_VS
    m_hKTDGManagerHandle = NULL;
#endif CONVERSION_VS
//}} robobeg : 2008-10-13
}//CKTDGObject::CKTDGObject()


CKTDGObject::~CKTDGObject(void)
{
    SAFE_DELETE( m_pMatrix );
}//CKTDGObject::~CKTDGObject(void)


//{{ robobeg : 2008-10-17
void CKTDGObject::SetManagerObject( CKTDGObject* pManagerObject_ )
{
    //ASSERT( IsManagedByDGManager() == false );

    m_pManagerObject = pManagerObject_;
    if ( pManagerObject_ == NULL )
    {
        if ( m_pMatrix == NULL )
        {
            m_pMatrix = new CKTDGMatrix( g_pKTDXApp->GetDevice() );
            ASSERT( m_pMatrix != NULL );
        }//if
    }
    else
    {
        SAFE_DELETE( m_pMatrix );
    }//if.. else..
}//CKTDGObject::SetRefMatrix()
//}} robobeg : 2008-10-17

#ifndef  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR

/** @function : CKTDGObject::KTDGObjectDeleter::operator()
	@brief : 1. DGManager에 의해 관리 되는 객체이면 DGManager에서 삭제한다.\n
			 2. MainThread에서 호출되는 경우이면 바로 삭제하고,\n
			 그렇지 않으면 삭제를 요청한다.
	@param : CKTDGObject* pObject_
	@return : void 
*/
//{{ seojt // 2009-1-14, 21:23
void CKTDGObject::KTDGObjectDeleter::operator()( CKTDGObject* pObject_ )
{
    ASSERT( pObject_ != NULL );
    DeleteKTDGObject( pObject_ );
}//CKTDGObject::KTDGObjectDeleter::operator()()
//}} seojt // 2009-1-14, 21:23

#endif  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR


/*static*/ void CKTDGObject::DeleteKTDGObject( CKTDGObject* pObject_ )
{
    //ASSERT( pObject_ != NULL );
    if( pObject_ == NULL )
    {
        return;
    }//if

    if( pObject_->IsManagedByDGManager() )
    {
        if( g_pKTDXApp != NULL && g_pKTDXApp->GetDeviceManager() != NULL )
        {
		    g_pKTDXApp->GetDGManager()->RemoveObjectChain( pObject_ );
        }//if

    }//if

	pObject_->SetManagerObject( NULL );
    delete pObject_;
}//CKTDGObject::DeleteKTDGObject()
#ifdef CONVERSION_VS
bool CKTDGObject::IsManagedByDGManager() const
{
	return m_hKTDGManagerHandle != g_pKTDXApp->GetDGManager()->GetRenderObjectChainEnd();
}

bool CKTDGObject::IsRecursivelyManagedByDGManager() const
{
	return ( m_hKTDGManagerHandle != g_pKTDXApp->GetDGManager()->GetRenderObjectChainEnd() ) ? true :
		( m_pManagerObject != NULL ? m_pManagerObject->IsRecursivelyManagedByDGManager() : false );
}
#endif CONVERSION_VS