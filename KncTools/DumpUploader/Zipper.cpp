#include "StdAfx.h"
#include ".\zipper.h"
#include "zip.h"
#include "IElement.h"
#include <string>



CZipper::CZipper(void)
{
    m_hz = NULL;
}

CZipper::~CZipper(void)
{
}


bool CZipper::CreateZipFile( char* strFileName )
{
    if( strFileName != NULL )
    {
        m_strFileName = strFileName;
        if( m_strFileName.IsEmpty() )
            return false;

        m_hz = CreateZip( m_strFileName.c_str(), 0 );
        if( m_hz == NULL )
            return false;

        return true;
    }//end if

    return false;
}
bool CZipper::AddFile( IElement* pElement )
{

    CElementFileInfo* pFileInfo = (CElementFileInfo*)pElement;
    std::string strDest = pFileInfo->GetFileName();

    
    strDest = strDest.substr( strDest.rfind("\\")+1 );
    ZRESULT zRes = ZipAdd(m_hz, strDest.c_str(), pFileInfo->GetFileName() );
    if( zRes != ZR_OK )
        return false;
    return true;
}

void CZipper::Close()
{
    CloseZip(m_hz);
}

char* CZipper::GetZipFileName()
{
    if( m_strFileName.IsEmpty() )
        return NULL;
    return m_strFileName.c_str();
}