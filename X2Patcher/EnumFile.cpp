#include "EnumFile.h"
#include "CRC_32.h"
#include <sstream>
#include <io.h>


extern bool			g_bTestMode;

KEnumFile::KEnumFile()
{
    memset( m_szFileName, 0, _MAX_FNAME );
    //strcpy( m_szCurPath, szCurDir ); // for full path 
    memset( m_szCurPath, 0, _MAX_PATH );
    m_nDepth          = 0;
    
    // xml
    m_pxmlSignElement = 0;
    m_pxmlDirElement  = 0;
    m_pxmlFileElement = 0;
    
    m_pxmlDoc         = NULL;

    m_pDirMap         = NULL;
    m_pFileMap        = NULL;

	WCHAR buff[MAX_PATH] = {0};
	GetCurrentDirectory( MAX_PATH, buff );

	//금태형을 안심시키기 위한 임시 코드임
	m_vecAllFile.clear();
/*
	if ( g_bTestMode == false )
	{
		FindFolderFile( buff, m_vecAllFile );
		SetSubDirectory();
	}
*/
	

} // KEumFile()


KEnumFile::~KEnumFile()
{
    memset(m_szFileName, 0, _MAX_FNAME );
    memset( m_szCurPath, 0, _MAX_PATH );
    m_nDepth    = 0;
    
    DirMap::iterator    dit;
    if( NULL != m_pDirMap )
    {
        for( dit = m_pDirMap->begin(); dit != m_pDirMap->end(); dit++)
            SAFE_DELETE( dit->second );
        m_pDirMap->clear();
        SAFE_DELETE( m_pDirMap );
    } // if
    
    FileMap::iterator   fit;
    if( NULL != m_pFileMap )
    {
        for( fit = m_pFileMap->begin(); fit != m_pFileMap->end(); fit++)
            SAFE_DELETE( fit->second );
        m_pFileMap->clear();
        SAFE_DELETE( m_pFileMap );
    } // if
    
    SAFE_DELETE( m_pxmlDoc );
} // ~KEnumFile()


bool KEnumFile::Signing(TiXmlDocument& xmlDoc_) // by parkch
{
    TiXmlElement*   pxmlSignElement = GetSignElement(xmlDoc_);
    std::string     strFileList     = GetFileListStr(xmlDoc_);

    if (true == strFileList.empty()) return false;

    std::string strSign = CRC_32::GetCRCFromString( strFileList );
    TiXmlNode* node = pxmlSignElement->FirstChild("Item");
    if (NULL == node)
    {
        TiXmlElement item("Item");
        item.SetAttribute("Name", "");
        pxmlSignElement->InsertEndChild(item);
        node = pxmlSignElement->FirstChild("Item");
    }
    node->ToElement()->SetAttribute("Name", strSign.c_str());

    return true;
} // Signing()


bool KEnumFile::Verify(const TiXmlDocument& xmlDoc_) // by parkch
{
    TiXmlElement*   pxmlSignElement = GetSignElement(xmlDoc_);
    std::string     strFileList     = GetFileListStr(xmlDoc_);

    if (true == strFileList.empty()) return false;

    std::string strOrgSign = CRC_32::GetCRCFromString( strFileList );
    TiXmlNode* node = pxmlSignElement->FirstChild("Item");
    if (NULL == node)
    {
        TiXmlElement item("Item");
        item.SetAttribute("Name", "");
        pxmlSignElement->InsertEndChild(item);
        node = pxmlSignElement->FirstChild("Item");
    }
    std::string strCurSign = node->ToElement()->Attribute("Name");

    return (strOrgSign == strCurSign);
} // Verify()


void KEnumFile::AddDataDirectory(const std::string& strDirectory)
{
	m_strDataDirectory.push_back( strDirectory );
}

TiXmlElement* KEnumFile::GetSignElement(const TiXmlDocument& xmlDoc_) // by parkch
{
    TiXmlNode* node = NULL;
    node = xmlDoc_.FirstChild("PatchInfo");
    node = node->FirstChild("Signature");
    if (NULL == node)
    {
        TiXmlElement item("Signature");
        xmlDoc_.ToElement()->InsertEndChild(item);
        node = xmlDoc_.FirstChild("PatchInfo")->FirstChild("Signature");
        ASSERT(node);
    }
    return node->ToElement();
} // GetSignElement()


const std::string KEnumFile::GetFileListStr(const TiXmlDocument& xmlDoc_) // by parkch
{
    std::ostringstream xmlStream;
    xmlStream << xmlDoc_;
    std::string strStream = xmlStream.str();
    std::string::size_type beginPos = strStream.find("<File>");
    std::string::size_type endPos   = strStream.find("</File>");
    if (std::string::npos == beginPos || std::string::npos == endPos)
    {
        strStream.erase();
        return strStream;
    }
    return strStream.substr(beginPos, endPos);
} // GetFileListStr()


bool KEnumFile::CreateFileList(const char* szCurDir_, 
                               const char* szFileName_)
{
    if( NULL != szFileName_)
    {
        strcpy( m_szFileName, szFileName_ );
        m_pxmlDoc         = new TiXmlDocument( szFileName_ );
        m_pDirMap         = new DirMap;
        m_pFileMap        = new FileMap;
    }
    else
    {
        return false;
    }

//    const char* szStart = "<?xml version=\"1.0\"  standalone='no' >\n"
//                          "<PatchInfo>"
//                          "<Directory>\n"
//                          "</Directory>"
//                          "<File>\n"
//                          "</File>"
//                          "</PatchInfo>";
    const char* szStart = "<?xml version=\"1.0\"  standalone='no' >\n"
                          "<PatchInfo>"
                          "<Signature>"
                          "</Signature>"
                          "<Directory>\n"
                          "</Directory>"
                          "<File>\n"
                          "</File>"
                          "</PatchInfo>";
    m_pxmlDoc->Parse( szStart );

    if( m_pxmlDoc->Error() )
    {
        printf( "Error in %s: %s\n", m_pxmlDoc->Value(), m_pxmlDoc->ErrorDesc() );
        return false;
    } // if

    TiXmlNode* node = 0;
    node = m_pxmlDoc->FirstChild( "PatchInfo" );
    node = node->FirstChild("Signature");
    assert( node );
    m_pxmlSignElement = node->ToElement();

    node = m_pxmlDoc->FirstChild( "PatchInfo" );
    node = node->FirstChild("Directory");
    assert( node );
    m_pxmlDirElement = node->ToElement();

    node = m_pxmlDoc->FirstChild( "PatchInfo" );
    node = node->FirstChild("File");
    assert( node );
    m_pxmlFileElement = node->ToElement();

    
	// enumerate file

	WCHAR wcharBuf[255] = {0,};
	MultiByteToWideChar( CP_ACP, 0, szCurDir_, -1, wcharBuf, 255 );

	::SetCurrentDirectory( wcharBuf );
    EnumFile();

    // generate xml
    // Signature
    TiXmlElement item( "Item" );
    item.SetAttribute( "Name", "" );
    m_pxmlSignElement->InsertEndChild( item );

    // directory
    DirMap::iterator    dit;
    for( dit = m_pDirMap->begin(); dit != m_pDirMap->end(); dit++ )
    {
        TiXmlElement item( "Item" );
        item.SetAttribute( "Name", dit->second->m_szName );
        m_pxmlDirElement->InsertEndChild( item );
    } // for
    
    // file
    FileMap::iterator   fit;
    for( fit = m_pFileMap->begin(); fit != m_pFileMap->end(); fit++ )
    {
//{{ Modified by ct 2003-04-29 오후 04:54:04
//        char buf[40];
//}} Modified by ct 2003-04-29 오후 04:54:04
        TiXmlElement item( "Item" );
        item.SetAttribute( "Name", fit->second->m_szName );
        item.SetAttribute( "Size", fit->second->m_dwSize );
        item.SetAttribute( "Version", fit->second->m_usVersion );
        item.SetAttribute( "CheckSum", fit->second->m_strCheckSum.c_str() );
//{{ Modified by ct 2003-04-29 오후 04:53:34
/*
        ultoa( fit->second->m_ftLastWriteTime.dwHighDateTime, buf, 10 );
        item.SetAttribute( "LastHigh", buf );
        ultoa( fit->second->m_ftLastWriteTime.dwLowDateTime, buf, 10 );
        item.SetAttribute( "LastLow",  buf );
*/
//}} Modified by ct 2003-04-29 오후 04:53:34

        m_pxmlFileElement->InsertEndChild( item );
    } // for

    m_pxmlDoc->SaveFile();

    // 파괴
    if( NULL != m_pDirMap )
    {
        for( dit = m_pDirMap->begin(); dit != m_pDirMap->end(); dit++)
            SAFE_DELETE( dit->second );
        m_pDirMap->clear();
        SAFE_DELETE( m_pDirMap );
    } // if
    
    if( NULL != m_pFileMap )
    {
        for( fit = m_pFileMap->begin(); fit != m_pFileMap->end(); fit++)
            SAFE_DELETE( fit->second );
        m_pFileMap->clear();
        SAFE_DELETE( m_pFileMap );
    } // if
    
    SAFE_DELETE( m_pxmlDoc );

    return true;
}


void KEnumFile::EnumFile()
{
    HANDLE          hFile;
    WIN32_FIND_DATA	wfd;
    
    hFile = FindFirstFile( L"*.*", &wfd );

    while( FindNextFile(hFile, &wfd) )
	{
		// parent directory인 경우 skip
        if( wcscmp(wfd.cFileName, L"..") == 0 ) 
            continue;

		// directory인 경우
        if( wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
		{
            SetCurrentDirectory( wfd.cFileName );
			m_nDepth++;

			char charBuf[255] = {0,};
			WideCharToMultiByte( CP_ACP, 0, wfd.cFileName, -1, charBuf, 255, NULL, NULL );
			strcat( m_szCurPath, charBuf );
			strcat( m_szCurPath, "\\" );
            
            // Add item to Directory element
            KDirInfo*   pkDir = new KDirInfo;
            strcpy( pkDir->m_szName, m_szCurPath );
            (*m_pDirMap)[pkDir->m_szName] = pkDir;

			EnumFile();
		}
		else
		{
			int nTempLen;
            char szTempPath[_MAX_PATH];

			nTempLen = sprintf( szTempPath, "%s%s", m_szCurPath, wfd.cFileName );
			szTempPath[nTempLen++] = 0;
            
            // Add item to File element
            KFileInfo*  pkFile  = new KFileInfo;
            strcpy( pkFile->m_szName, szTempPath );
            pkFile->m_dwSize    = wfd.nFileSizeLow;
            pkFile->m_usVersion = 1;
            ::CharUpper( wfd.cFileName );

			char charBuf[255] = {0,};
			WideCharToMultiByte( CP_ACP, 0, wfd.cFileName, -1, charBuf, 255, NULL, NULL );

            std::string strFileName( charBuf );
            if ( ( strFileName.find( ".EXE" ) != std::string::npos ) || 
                 ( strFileName.find( ".DLL" ) != std::string::npos ) ||
                 ( strFileName.find( ".FRM" ) != std::string::npos ) ||
                 ( strFileName.find( ".STG" ) != std::string::npos ) ||
                 ( strFileName.find( ".LUA" ) != std::string::npos ) ||
                 ( strFileName.find( ".KOM" ) != std::string::npos ) ||
                 ( strFileName.find( ".KSF" ) != std::string::npos ) )
            {
                pkFile->m_strCheckSum = CRC_32::GetCRCFromFile( charBuf );
            }
            else
            {
                pkFile->m_strCheckSum.erase();
            }
//{{ Modified by ct 2003-04-29 오후 04:54:24
//            pkFile->m_ftLastWriteTime = wfd.ftLastWriteTime;
//}} Modified by ct 2003-04-29 오후 04:54:24
            (*m_pFileMap)[pkFile->m_szName] = pkFile;

        } // else
    } // while

	if( m_nDepth ) 
	{
		if( m_nDepth == 1 ) 
            m_szCurPath[0] = 0;
		else 
		{
			*strrchr( m_szCurPath, '\\' ) = '\0';
			*(strrchr( m_szCurPath, '\\' ) + 1) = '\0';
        } // else

		SetCurrentDirectory( L".." );
		m_nDepth--;
    } // if

    if( hFile != INVALID_HANDLE_VALUE )
        FindClose( hFile );
} // EnumFile()


bool KEnumFile::CreateFileList(const char* szCurDir_, 
                               TiXmlDocument& xmldoc_)
{
    m_pDirMap         = new DirMap;
    m_pFileMap        = new FileMap;
    
    const char* szStart = "<?xml version=\"1.0\"  standalone='no' >"
                          "<PatchInfo>"
                          "<Signature>"
                          "</Signature>"
                          "<Directory>"
                          "</Directory>"
                          "<File>"
                          "</File>"
                          "</PatchInfo>";
    
    xmldoc_.Parse( szStart );
    if( xmldoc_.Error() )
    {
        printf( "Error in %s: %s\n", xmldoc_.Value(), xmldoc_.ErrorDesc() );
        return false;
    } // if
    
    //xmldoc_.Print( stdout );
    
    TiXmlNode* node = 0;
    node = xmldoc_.FirstChild( "PatchInfo" );
    node = node->FirstChild("Signature");
    assert( node );
    m_pxmlSignElement = node->ToElement();
    node = xmldoc_.FirstChild( "PatchInfo" );
    node = node->FirstChild("Directory");
    assert( node );
    m_pxmlDirElement = node->ToElement();
    node = xmldoc_.FirstChild( "PatchInfo" );
    node = node->FirstChild("File");
    assert( node );
    m_pxmlFileElement = node->ToElement();
    
    // enumerate file

	WCHAR wcharBuf[255] = {0,};
	MultiByteToWideChar( CP_ACP, 0, m_szCurWorkingDir, -1, wcharBuf, 255 );


    GetCurrentDirectory( _MAX_PATH, wcharBuf );

	MultiByteToWideChar( CP_ACP, 0, szCurDir_, -1, wcharBuf, 255 );

    SetCurrentDirectory( wcharBuf );
    EnumFile();

	MultiByteToWideChar( CP_ACP, 0, m_szCurWorkingDir, -1, wcharBuf, 255 );

    SetCurrentDirectory( wcharBuf );
    
    // generate xml
    // Signature
    TiXmlElement item( "Item" );
    item.SetAttribute( "Name", "" );
    m_pxmlSignElement->InsertEndChild( item );
    
    // directory
    DirMap::iterator    dit;
    for( dit = m_pDirMap->begin(); dit != m_pDirMap->end(); dit++ )
    {
        TiXmlElement item( "Item" );
        item.SetAttribute( "Name", dit->second->m_szName );
        m_pxmlDirElement->InsertEndChild( item );
    } // for
    
    // file
    FileMap::iterator   fit;
    for( fit = m_pFileMap->begin(); fit != m_pFileMap->end(); fit++ )
    {
//{{ Modified by ct 2003-04-29 오후 04:56:05
/*
        char buf[40];
*/
//}} Modified by ct 2003-04-29 오후 04:56:05
        TiXmlElement item( "Item" );
        item.SetAttribute( "Name", fit->second->m_szName );
        item.SetAttribute( "Size", fit->second->m_dwSize );
        item.SetAttribute( "Version", fit->second->m_usVersion );
        item.SetAttribute( "CheckSum", fit->second->m_strCheckSum.c_str() );
//{{ Modified by ct 2003-04-29 오후 04:55:35
/*
        ultoa( fit->second->m_ftLastWriteTime.dwHighDateTime, buf, 10 );
        item.SetAttribute( "LastHigh", buf );
        ultoa( fit->second->m_ftLastWriteTime.dwLowDateTime, buf, 10 );
        item.SetAttribute( "LastLow",  buf );
*/
//}} Modified by ct 2003-04-29 오후 04:55:35

        m_pxmlFileElement->InsertEndChild( item );
    } // for
    
    //xmldoc_.SaveFile();
    //xmldoc_.Print( stdout );
    
    // 파괴
    if( NULL != m_pDirMap )
    {
        for( dit = m_pDirMap->begin(); dit != m_pDirMap->end(); dit++)
            SAFE_DELETE( dit->second );
        m_pDirMap->clear();
        SAFE_DELETE( m_pDirMap );
    } // if
    
    if( NULL != m_pFileMap )
    {
        for( fit = m_pFileMap->begin(); fit != m_pFileMap->end(); fit++)
            SAFE_DELETE( fit->second );
        m_pFileMap->clear();
        SAFE_DELETE( m_pFileMap );
    } // if
    
    return true;
}

bool KEnumFile::CompareFileVersion(const TiXmlDocument& xmlOldDoc_, 
                                   const TiXmlDocument& xmlDoc_, 
                                   TiXmlDocument& xmlNewDoc_)
{
    // downloadfilelist
    const char* szInitDoc = "<?xml version=\"1.0\"  standalone='no' >\n"
                            "<PatchInfo>"
                            "<Signature>"
                            "</Signature>"
                            "<Directory>\n"
                            "</Directory>"
                            "<File>\n"
                            "</File>"
                            "</PatchInfo>";
    xmlNewDoc_.Parse(szInitDoc);
    
    TiXmlNode*      node = 0;
    TiXmlNode*      nodeOld = 0;
    TiXmlNode*      nodeNew = 0;

    TiXmlElement*   dirElement = 0;
    TiXmlElement*   dirElementOld = 0;
    TiXmlElement*   dirElementNew = 0;

    TiXmlElement*   fileElement = 0;
    TiXmlElement*   fileElementOld = 0;
    TiXmlElement*   fileElementNew = 0;

    node = xmlDoc_.FirstChild( "PatchInfo" );
    dirElement = node->FirstChild("Directory")->ToElement();
    assert( dirElement );
    
    nodeOld = xmlOldDoc_.FirstChild( "PatchInfo" );
    dirElementOld = nodeOld->FirstChild("Directory")->ToElement();
    assert( dirElementOld );
    
    node = xmlNewDoc_.FirstChild( "PatchInfo" );
    dirElementNew = node->FirstChild("Directory")->ToElement();
    assert( dirElementNew );
    
    // Directory
    node = dirElement->FirstChild("Item");
    while( node != 0 )
    {
        //dirElementNew->InsertEndChild(*(node->Clone()));
        dirElementNew->InsertEndChild(*node);
        node = node->NextSibling("Item");
    } // while

    // File
    node = xmlDoc_.FirstChild( "PatchInfo" );
    fileElement = node->FirstChild("File")->ToElement();
    assert( fileElement );
    
    nodeOld = xmlOldDoc_.FirstChild( "PatchInfo" );
    fileElementOld = nodeOld->FirstChild("File")->ToElement();
    assert( fileElementOld );

    node = xmlNewDoc_.FirstChild( "PatchInfo" );
    fileElementNew = node->FirstChild("File")->ToElement();
    assert( fileElementNew );

    int nSize = 0,
        nSizeOld = 0,
        nVersion = 0,
        nVersionOld = 0;
    
    node = fileElement->FirstChild("Item");
    nodeOld = fileElementOld->FirstChild("Item");
    while( node != 0 || nodeOld != 0 )
    {
        if( nodeOld != 0 && node == 0 )
        {
            // Old 파일 삭제될 경우
            nodeOld = nodeOld->NextSibling("Item");
            continue;
        } // if
        
        if( nodeOld == 0 && node != 0 )
        {
            // new list에 추가 되어야 할 경우            
            //fileElementNew->InsertEndChild(*(node->Clone()));
            fileElementNew->InsertEndChild(*node);
            node = node->NextSibling("Item");
            continue;
        } // if

        // name이 같은 경우
        if( (strcmp(nodeOld->ToElement()->Attribute("Name"),
             node->ToElement()->Attribute("Name")) == 0) )
        {
            node->ToElement()->Attribute("Size", &nSize);
            node->ToElement()->Attribute("Version", &nVersion);
            nodeOld->ToElement()->Attribute("Size", &nSizeOld);
            nodeOld->ToElement()->Attribute("Version", &nVersionOld);
        
//{{ Modified by ct 2003-04-29 오후 04:57:32
/*
            FILETIME ft;
            FILETIME ftOld;
            ft.dwHighDateTime   = atol( node->ToElement()->Attribute("LastHigh") );
            ft.dwLowDateTime    = atol( node->ToElement()->Attribute("LastLow") );
            ftOld.dwHighDateTime= atol( nodeOld->ToElement()->Attribute("LastHigh") );
            ftOld.dwLowDateTime = atol( nodeOld->ToElement()->Attribute("LastLow") );
*/
//}} Modified by ct 2003-04-29 오후 04:57:32

            
//{{ Modified by ct 2003-04-29 오후 04:57:57
/*
            if( (nSize != nSizeOld) ||
                (nVersion != nVersionOld) ||
                (CompareFileTime(&ft, &ftOld) != 0) ) // 시간이 다른 경우
            {
*/
//}} Modified by ct 2003-04-29 오후 04:57:57
            if( (nSize != nSizeOld) && 
                (nVersion != nVersionOld) ) // 시간이 다른 경우
            {
                // file의 version이 변경된 경우
                node->ToElement()->SetAttribute("Version", atoi(nodeOld->ToElement()->Attribute("Version")) + 1);
            }
            else
            {
                node->ToElement()->SetAttribute("Version", atoi(nodeOld->ToElement()->Attribute("Version")));
            }
            //fileElementNew->InsertEndChild(*(node->Clone()));
            fileElementNew->InsertEndChild(*node);
            node = node->NextSibling("Item");
            nodeOld = nodeOld->NextSibling("Item");
            continue;
        } 
        else
        {
            // a < b
            if( (strcmp(nodeOld->ToElement()->Attribute("Name"),
                node->ToElement()->Attribute("Name")) < 0) )
            {
                // Old이 삭제되는 경우
                nodeOld = nodeOld->NextSibling("Item");
            }
            else
            {
                //fileElementNew->InsertEndChild(*(node->Clone()));
                fileElementNew->InsertEndChild(*node);
                node = node->NextSibling("Item");
            } // else
            continue;
        } // else
    } // while

    return true;
} // CompareFileVersion()


bool KEnumFile::CreateUpdateFileList(const char* szLocalFileName_, 
                                     const char* szServerFileName_, 
                                     TiXmlDocument& xmlUpdateDoc_)
{
    char szPath[_MAX_PATH];
    char szLocalFileFullPath[_MAX_PATH];

	WCHAR wcharBuf[255] = {0,};

	GetCurrentDirectory( _MAX_PATH, wcharBuf );
	WideCharToMultiByte( CP_ACP, 0, wcharBuf, -1, szPath, 255, NULL, NULL );

    strcat( szPath, "\\" );

    // local filelist
    strcpy( szLocalFileFullPath, szPath );
    strcat( szLocalFileFullPath, szLocalFileName_ );



    TiXmlDocument docLocal_;

    //TiXmlDocument docLocal(szLocalFileFullPath);
    TiXmlDocument docLocal(szLocalFileFullPath);

    if( !docLocal_.LoadFile(szLocalFileFullPath) )
    {
//        const char* szInitDoc = "<?xml version=\"1.0\"  standalone='no' >\n"
//                                "<PatchInfo>"
//                                "<Signature>"
//                                "</Signature>"
//                                "<Directory>\n"
//                                "</Directory>"
//                                "<File>\n"
//                                "</File>"
//                                "</PatchInfo>";
//        docLocal.Parse(szInitDoc);
        CreateFileList(szPath, docLocal);
        KEnumFile::Signing(docLocal);
        docLocal.SaveFile();

    }
    else
    {
        docLocal.LoadFile();

        if (false == Verify(docLocal))
        {
            docLocal.Clear();
            CreateFileList(szPath, docLocal);
            KEnumFile::Signing(docLocal);
            docLocal.SaveFile();
        }
    }


    // server filelist
    TiXmlDocument docServer( szServerFileName_ );
    if( !docServer.LoadFile() )
    {
        printf( "Could not load file. Error='%s'. Exiting.\n", docServer.ErrorDesc() );
        return false;
    }
    //docServer.Print( stdout );

    // downloadfilelist
    const char* szInitDoc = "<?xml version=\"1.0\"  standalone='no' >\n"
                            "<PatchInfo>"
//{{ Appended by parkch: 2003-11-18 오전 9:23:19
                            "<PatchVersion>"
                            "</PatchVersion>"
//}} Appended by parkch: 2003-11-18 오전 9:23:19
                            "<Signature>"
                            "</Signature>"
                            "<Directory>\n"
                            "</Directory>"
                            "<File>\n"
                            "</File>"
                            "</PatchInfo>";


    //TiXmlDocument docDownload( "downloadfilelist.xml" );
    xmlUpdateDoc_.Parse(szInitDoc);

	ClearNotNeedFiles( docServer );
    
    TiXmlNode*      node = 0;
    TiXmlNode*      nodeLocal = 0;
    TiXmlNode*      nodeDownload = 0;

    TiXmlElement*   dirElement = 0;
    TiXmlElement*   dirElementLocal = 0;
    TiXmlElement*   dirElementDownload = 0;

    TiXmlElement*   fileElement = 0;
    TiXmlElement*   fileElementLocal = 0;
    TiXmlElement*   fileElementDownload = 0;

    node = docServer.FirstChild( "PatchInfo" );
    dirElement = node->FirstChild("Directory")->ToElement();
    assert( dirElement );
    
    nodeLocal = docLocal.FirstChild( "PatchInfo" );
    dirElementLocal = nodeLocal->FirstChild("Directory")->ToElement();
    assert( dirElementLocal );
    
    node = xmlUpdateDoc_.FirstChild( "PatchInfo" );
    dirElementDownload = node->FirstChild("Directory")->ToElement();
    assert( dirElementDownload );
    
    node = dirElement->FirstChild("Item");
    nodeLocal = dirElementLocal->FirstChild("Item");
    while( node != 0 || nodeLocal != 0 )
    {
        if( nodeLocal != 0 && node == 0 )
        {
            // local 파일 삭제될 경우
            nodeLocal = nodeLocal->NextSibling("Item");
            continue;
        } // if
        
        if( nodeLocal == 0 && node != 0 )
        {
            // server의 list에 추가 되어야 할 경우            
            //dirElementDownload->InsertEndChild(*(node->Clone()));
            dirElementDownload->InsertEndChild(*node);
            node = node->NextSibling("Item");
            continue;
        } // if
        
        // name이 같은 경우
        if( (strcmp(nodeLocal->ToElement()->Attribute("Name"),
             node->ToElement()->Attribute("Name")) == 0) )
        {
            node = node->NextSibling("Item");
            nodeLocal = nodeLocal->NextSibling("Item");
            continue;
        } 
        else
        {
            // a < b
            if( (strcmp(nodeLocal->ToElement()->Attribute("Name"),
                node->ToElement()->Attribute("Name")) < 0) )
            {
                // local이 삭제되는 경우
                nodeLocal = nodeLocal->NextSibling("Item");
            }
            else
            {
                //dirElementDownload->InsertEndChild(*(node->Clone()));
                dirElementDownload->InsertEndChild(*node);
                node = node->NextSibling("Item");
            } // else
            continue;
        } // else
    } // while


    node = docServer.FirstChild( "PatchInfo" );
    fileElement = node->FirstChild("File")->ToElement();
    assert( fileElement );
    
    nodeLocal = docLocal.FirstChild( "PatchInfo" );
    fileElementLocal = nodeLocal->FirstChild("File")->ToElement();
    assert( fileElementLocal );

    node = xmlUpdateDoc_.FirstChild( "PatchInfo" );
    fileElementDownload = node->FirstChild("File")->ToElement();
    assert( fileElementDownload );

    int nSize = 0,
        nSizeLocal = 0,
        nVersion = 0,
        nVersionLocal = 0;
    std::string strCheckSum;
    std::string strCheckSumLocal;
    std::string strCheckSumActual;
    char szActualFilePath[_MAX_PATH];

    node = fileElement->FirstChild("Item");
    nodeLocal = fileElementLocal->FirstChild("Item");
    while( node != 0 || nodeLocal != 0 )
    {
        if( nodeLocal != 0 && node == 0 )
        {
            // local 파일 삭제될 경우
            nodeLocal = nodeLocal->NextSibling("Item");
            continue;
        } // if
        
        if( nodeLocal == 0 && node != 0 )
        {
            // server의 list에 추가 되어야 할 경우            
            // fileElementDownload->InsertEndChild(*(node->Clone()));
            fileElementDownload->InsertEndChild(*node);
            node = node->NextSibling("Item");
            continue;
        } // if

        // name이 같은 경우
        if( (strcmp(nodeLocal->ToElement()->Attribute("Name"),
             node->ToElement()->Attribute("Name")) == 0) )
        {
            node->ToElement()->Attribute("Size", &nSize);
            node->ToElement()->Attribute("Version", &nVersion);
            strCheckSum = node->ToElement()->Attribute("CheckSum");
            nodeLocal->ToElement()->Attribute("Size", &nSizeLocal);
            nodeLocal->ToElement()->Attribute("Version", &nVersionLocal);
            strCheckSumLocal = nodeLocal->ToElement()->Attribute("CheckSum");

//{{ Appended by parkch: 2003-07-09 오후 5:24:01
            // 실제 파일의 경로를 구한다.
            ZeroMemory( szActualFilePath, _MAX_PATH );
            strcpy( szActualFilePath, szPath );
            strcat( szActualFilePath, node->ToElement()->Attribute("Name") );

            // 업데이트 리스트에 넣을 파일들을 선택한다.
            if( -1 == _access( szActualFilePath, 0 ) ) // 0. 파일 존재 여부 체크
            {
                // 파일이 존재하지 않으면 업데이트 리스트에 추가.
                fileElementDownload->InsertEndChild(*node);
            }
            else if( nVersion != nVersionLocal ) // 1. 버전 체크
            {
                // 버전이 다르면 업데이트 리스트에 추가.
                fileElementDownload->InsertEndChild(*node);
            }
            else if( nSize != nSizeLocal ) // 2. 파일 크기 체크
            {
                // 파일 크기가 다르면 업데이트 리스트에 추가.
                fileElementDownload->InsertEndChild(*node);
            }
            else if( !strCheckSum.empty() ) // 체크섬 체크가 필요한 파일의 경우
            {
				//{{ dmlee 2008.05.14 실제 파일에 대한 checksum 검사는 게임실행중 별도의 thread에서
				if ( strCheckSum != strCheckSumLocal ) // 3. 새 리스트의 체크섬 체크
				{
					// 새 리스트의 체크섬이 다르면 업데이트 리스트에 추가.
					fileElementDownload->InsertEndChild(*node);
				}
				//}} dmlee 2008.05.14 실제 파일에 대한 checksum 검사는 게임실행중 별도의 thread에서

//
////{{ seojt // 2008-1-7, 12:23
//                // original
//                //
//                // 중요 파일의 경우, 해킹 방지를 위해 체크섬으로 유효한지 판단한다.
//                strCheckSumActual = CRC_32::GetCRCFromFile(szActualFilePath);
//
//                if ( strCheckSum != strCheckSumLocal ) // 3. 새 리스트의 체크섬 체크
//                {
//                    // 새 리스트의 체크섬이 다르면 업데이트 리스트에 추가.
//                    fileElementDownload->InsertEndChild(*node);
//                }
//                else if( false == strCheckSumActual.empty() &&
//                         strCheckSum != strCheckSumActual ) // 4. 실제 파일의 체크섬 체크
//                {
//                    // 실제 파일의 체크섬이 다르면 업데이트 리스트에 추가.
//                    fileElementDownload->InsertEndChild(*node);
//                }
//
//                // modified - jintaeks on 2008-01-07, 12:23
//                //
//                //if ( strCheckSum != strCheckSumLocal ) // 3. 새 리스트의 체크섬 체크
//                //{
//                //    // 새 리스트의 체크섬이 다르면 업데이트 리스트에 추가.
//                //    fileElementDownload->InsertEndChild(*node);
//                //}
//                //else
//                //{
//                //    // 중요 파일의 경우, 해킹 방지를 위해 체크섬으로 유효한지 판단한다.
//                //    strCheckSumActual = CRC_32::GetCRCFromFile(szActualFilePath);
//                //    if ( false == strCheckSumActual.empty() &&
//                //            strCheckSum != strCheckSumActual ) // 4. 실제 파일의 체크섬 체크
//                //    {
//                //        // 실제 파일의 체크섬이 다르면 업데이트 리스트에 추가.
//                //        fileElementDownload->InsertEndChild(*node);
//                //    }
//                //}//if.. else..
////}} seojt // 2008-1-7, 12:23

            }
//}} Appended by parkch: 2003-07-09 오후 5:24:01

            node = node->NextSibling("Item");
            nodeLocal = nodeLocal->NextSibling("Item");
            continue;
        } 
        else
        {
            // a < b
            if( (strcmp(nodeLocal->ToElement()->Attribute("Name"),
                node->ToElement()->Attribute("Name")) < 0) )
            {
                // local이 삭제되는 경우
                nodeLocal = nodeLocal->NextSibling("Item");
            }
            else
            {
                // fileElementDownload->InsertEndChild(*(node->Clone()));
                fileElementDownload->InsertEndChild(*node);
                node = node->NextSibling("Item");
            } // else
            continue;
        } // else
    } // while

    KEnumFile::Signing(docServer);
    docServer.SaveFile();

    return true;
} //CreateUpdateFileList()



bool KEnumFile::IsDiffFiles(const char* szSrcName, const char* szDstName)
{
    FILE* fpSrc = NULL;
    FILE* fpDst = NULL;

	WCHAR wcharBuf[255] = {0,};
	MultiByteToWideChar( CP_ACP, 0, szSrcName, -1, wcharBuf, 255 );
	WCHAR wcharBuf2[255] = {0,};
	MultiByteToWideChar( CP_ACP, 0, szDstName, -1, wcharBuf2, 255 );


    fpSrc = ::_tfopen(wcharBuf, L"rb");
    fpDst = ::_tfopen(wcharBuf2, L"rb");

    // 1. error case check
    // std::cout << "step 1: check error case" << endl;
    if (fpSrc == NULL || fpDst == NULL)
    {
        ::fprintf(stderr, "Warnning: %s or %s not exist\n", szSrcName, szDstName);
        if (fpSrc != NULL) { ::fclose(fpSrc); fpSrc = NULL; }
        if (fpDst != NULL) { ::fclose(fpDst); fpDst = NULL; }
        return true;
    }

    // 2. size compare
    //cout << "step 2: size compare" << endl;
    fpos_t posSrc = NULL;
    fpos_t posDst = NULL;
    ::fseek(fpSrc, 0, SEEK_END);
    ::fgetpos(fpSrc, &posSrc);
    ::fseek(fpSrc, 0, SEEK_SET);
    ::fseek(fpDst, 0, SEEK_END);
    ::fgetpos(fpDst, &posDst);
    ::fseek(fpDst, 0, SEEK_SET);
    if (posSrc != posDst)
    {
        if (fpSrc != NULL) { ::fclose(fpSrc); fpSrc = NULL; }
        if (fpDst != NULL) { ::fclose(fpDst); fpDst = NULL; }
        return true;
    }

    // 3. read the file contents
    // cout << "step 3: read the contents" << endl;
    char* bufSrc = NULL;
    char* bufDst = NULL;
    bufSrc = new char[(size_t)posSrc];
    bufDst = new char[(size_t)posDst];
    if (bufSrc == NULL || bufDst == NULL)
    {
        fprintf(stderr, "ERROR: cannot allocate memory\n");
        if (bufSrc != NULL) { delete bufSrc; bufSrc = NULL; }
        if (bufDst != NULL) { delete bufDst; bufDst = NULL; }
        if (fpSrc != NULL) { ::fclose(fpSrc); fpSrc = NULL; }
        if (fpDst != NULL) { ::fclose(fpDst); fpDst = NULL; }
        return true;
    }
    ::fread(bufSrc, sizeof(char), (size_t)posSrc, fpSrc);
    ::fread(bufDst, sizeof(char), (size_t)posDst, fpDst);
    if (fpSrc != NULL) { ::fclose(fpSrc); fpSrc = NULL; }
    if (fpDst != NULL) { ::fclose(fpDst); fpDst = NULL; }

    // 4. byte-by-byte compare
    // cout << "step 4: byte compare" << endl;
    char* ptrSrc = bufSrc;
    char* ptrDst = bufDst;
    while (ptrSrc < bufSrc + posSrc)
    {
        if (*ptrSrc++ != *ptrDst++)
        {
            if (bufSrc != NULL) { delete bufSrc; bufSrc = NULL; }
            if (bufDst != NULL) { delete bufDst; bufDst = NULL; }
            return true;
        }
    }
    if (bufSrc != NULL) { delete bufSrc; bufSrc = NULL; }
    if (bufDst != NULL) { delete bufDst; bufDst = NULL; }
    return false;
} // IsDiffFiles()


bool KEnumFile::CompareFileVersion(const char* szOldPath_, const TiXmlDocument& xmlOldDoc_, 
                                   const char* szNewPath_, const TiXmlDocument& xmlDoc_, 
                                   TiXmlDocument& xmlNewDoc_)
{
    // downloadfilelist
    const char* szInitDoc = "<?xml version=\"1.0\"  standalone='no' >\n"
                            "<PatchInfo>"
                            "<Signature>"
                            "</Signature>"
                            "<Directory>\n"
                            "</Directory>"
                            "<File>\n"
                            "</File>"
                            "</PatchInfo>";
    xmlNewDoc_.Parse(szInitDoc);
    
    TiXmlNode*      node = 0;
    TiXmlNode*      nodeOld = 0;
    TiXmlNode*      nodeNew = 0;

    TiXmlElement*   dirElement = 0;
    TiXmlElement*   dirElementOld = 0;
    TiXmlElement*   dirElementNew = 0;

    TiXmlElement*   fileElement = 0;
    TiXmlElement*   fileElementOld = 0;
    TiXmlElement*   fileElementNew = 0;

    node = xmlDoc_.FirstChild( "PatchInfo" );
    node = node->FirstChild("Signature");
    assert( node );
    m_pxmlSignElement = node->ToElement();

    node = xmlDoc_.FirstChild( "PatchInfo" );
    dirElement = node->FirstChild("Directory")->ToElement();
    assert( dirElement );
    
    nodeOld = xmlOldDoc_.FirstChild( "PatchInfo" );
    dirElementOld = nodeOld->FirstChild("Directory")->ToElement();
    assert( dirElementOld );
    
    node = xmlNewDoc_.FirstChild( "PatchInfo" );
    dirElementNew = node->FirstChild("Directory")->ToElement();
    assert( dirElementNew );

    // Signature
    TiXmlElement item( "Item" );
    item.SetAttribute( "Name", "" );
    m_pxmlSignElement->InsertEndChild( item );
    
    // Directory
    node = dirElement->FirstChild("Item");
    while( node != 0 )
    {
        dirElementNew->InsertEndChild(*node);
        node = node->NextSibling("Item");
    } // while

    // File
    node = xmlDoc_.FirstChild( "PatchInfo" );
    fileElement = node->FirstChild("File")->ToElement();
    assert( fileElement );
    
    nodeOld = xmlOldDoc_.FirstChild( "PatchInfo" );
    fileElementOld = nodeOld->FirstChild("File")->ToElement();
    assert( fileElementOld );

    node = xmlNewDoc_.FirstChild( "PatchInfo" );
    fileElementNew = node->FirstChild("File")->ToElement();
    assert( fileElementNew );

    int nSize = 0,
        nSizeOld = 0,
        nVersion = 0,
        nVersionOld = 0;
    
    node = fileElement->FirstChild("Item");
    nodeOld = fileElementOld->FirstChild("Item");
    SYSTEMTIME st;
    GetSystemTime(&st);
    while( node != 0 || nodeOld != 0 )
    {
        if( nodeOld != 0 && node == 0 )
        {
            // Old 파일 삭제될 경우
            nodeOld = nodeOld->NextSibling("Item");
            continue;
        } // if
        
        if( nodeOld == 0 && node != 0 )
        {
            // new list에 추가 되어야 할 경우            
            fileElementNew->InsertEndChild(*node);
            node = node->NextSibling("Item");
            continue;
        } // if

        // name이 같은 경우
        if( (strcmp(nodeOld->ToElement()->Attribute("Name"),
             node->ToElement()->Attribute("Name")) == 0) )
        {
            node->ToElement()->Attribute("Size", &nSize);
            node->ToElement()->Attribute("Version", &nVersion);
            nodeOld->ToElement()->Attribute("Size", &nSizeOld);
            nodeOld->ToElement()->Attribute("Version", &nVersionOld);
    
            std::string strOldFileName;
            std::string strNewFileName;
            strOldFileName = szOldPath_;
            strOldFileName.append(nodeOld->ToElement()->Attribute("Name"));
            strNewFileName = szNewPath_;
            strNewFileName.append(node->ToElement()->Attribute("Name"));

            if( IsDiffFiles(strOldFileName.c_str(),
                            strNewFileName.c_str()) ) // 파일을 비교
            {
                // file의 version이 변경된 경우
                node->ToElement()->SetAttribute("Version", atoi(nodeOld->ToElement()->Attribute("Version")) + 1);
                fileElementNew->InsertEndChild(*node);
            }
            else
            {
                node->ToElement()->SetAttribute("Version", atoi(nodeOld->ToElement()->Attribute("Version")));
                fileElementNew->InsertEndChild(*node);
            }
            node = node->NextSibling("Item");
            nodeOld = nodeOld->NextSibling("Item");

            // file의 시간 setting
            HANDLE          hFile;

			WCHAR wcharBuf[255] = {0,};
			MultiByteToWideChar( CP_ACP, 0, strNewFileName.c_str(), -1, wcharBuf, 255 );

            hFile = CreateFile( wcharBuf,
                                GENERIC_WRITE,
                                FILE_SHARE_READ,
                                NULL,
                                OPEN_EXISTING,
                                FILE_ATTRIBUTE_NORMAL,
                                NULL);
 
            if( hFile == INVALID_HANDLE_VALUE ) 
            { 
                continue;
            }  // if
            FILETIME ft;
            BOOL f;
            SystemTimeToFileTime(&st, &ft);
            f = SetFileTime( hFile, &ft, &ft, &ft );
            CloseHandle(hFile); 
            continue;
        } 
        else
        {
            // a < b
            if( (strcmp(nodeOld->ToElement()->Attribute("Name"),
                node->ToElement()->Attribute("Name")) < 0) )
            {
                // Old이 삭제되는 경우
                nodeOld = nodeOld->NextSibling("Item");
            }
            else
            {
                fileElementNew->InsertEndChild(*node);
                node = node->NextSibling("Item");
            } // else
            continue;
        } // else
    } // while

    return true;
} // CompareFileVersion()



bool KEnumFile::SetSubDirectory()
{
	char	szSearchDir[MAX_PATH] = "";
	char	szCurrentDir[MAX_PATH] = "";

	GetCurrentDirectoryA( MAX_PATH, szSearchDir );

	strcpy( szCurrentDir, szSearchDir );
	strcat( szCurrentDir, "\\" );

	AddDataDirectory( szCurrentDir );

	return GetSubDirectory( szSearchDir );	
}

bool KEnumFile::GetSubDirectory( char* szSearchDir)
{

	HANDLE				hSearch;
	WIN32_FIND_DATAA	fd;
	char				szSearchPath[256];

	strcpy(szSearchPath, szSearchDir);
	strcat(szSearchPath, "\\*.*");

	hSearch = FindFirstFileA(szSearchPath, &fd);

	if(hSearch == INVALID_HANDLE_VALUE)
		return false;

	do
	{
		if(strcmp(fd.cFileName, ".") && strcmp(fd.cFileName, ".."))
		{
			if(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				char	szNewSearchDir[256];
				string	strDir;

				strcpy(szNewSearchDir, szSearchDir);
				strcat(szNewSearchDir, "\\");
				strcat(szNewSearchDir, fd.cFileName);

				strDir = szNewSearchDir;
				strDir += "\\";
				AddDataDirectory( strDir);

				GetSubDirectory( szNewSearchDir);
			}
		}

	}while(FindNextFileA(hSearch, &fd));

	FindClose(hSearch);

	return true;
}

void KEnumFile::FindFolderFile( WCHAR* wstrFloder, std::vector<std::wstring>& vecFloder )
{

	HANDLE				hSearch;
	WIN32_FIND_DATA		fd;
	WCHAR				wstrSearchPath[256];


	wcscpy( wstrSearchPath, wstrFloder);
	wcscat( wstrSearchPath, L"\\*.*");

	hSearch = FindFirstFile( wstrSearchPath, &fd );

	if(hSearch == INVALID_HANDLE_VALUE)
		return;

	do
	{
		if( wcscmp(fd.cFileName, L".") && wcscmp(fd.cFileName, L"..") )
		{
			if( fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
			{
				WCHAR	wstrNewSearchDir[256];

				wcscpy( wstrNewSearchDir, wstrFloder );
				wcscat( wstrNewSearchDir, L"\\");
				wcscat( wstrNewSearchDir, fd.cFileName );

				FindFolderFile( wstrNewSearchDir, vecFloder );
			}
			else
			{
				WCHAR			wstrNewSearchFile[256];
				std::wstring	wstrFile;
				std::wstring	wstrFileName;

				wcscpy( wstrNewSearchFile, wstrFloder );
				wcscat( wstrNewSearchFile, L"\\");
				wcscat( wstrNewSearchFile, fd.cFileName );

				wstrFile = wstrNewSearchFile;
				wstrFileName = fd.cFileName;

				if ( wcscmp( fd.cFileName, L"X2Patcher.exe" ) != 0 &&
					wcscmp( fd.cFileName, L"directx_aug2006_redist.exe" ) != 0 &&
					wcscmp( fd.cFileName, L"dotnetfx_1_1.exe" ) != 0 &&
					wcscmp( fd.cFileName, L"msvcp71.dll" ) != 0 &&
					wcscmp( fd.cFileName, L"msvcr71.dll" ) != 0 &&
					wcscmp( fd.cFileName, L"unicows.dll" ) != 0 &&
					wcscmp( fd.cFileName, L"GameOption.lua" ) != 0 &&
					wcscmp( fd.cFileName, L"log.htm") != 0 &&
					wcscmp( fd.cFileName, L"Result.txt") != 0 &&
					wcscmp( fd.cFileName, L"ErrorLog.txt") != 0 )
				{
					//안쓰는 kom파일 없애는거(지금은 이름 바꾸는거) 넣은 부분임
					int strIndex = wstrFileName.rfind( L"." );

					if ( strIndex != -1 )
					{
						wstring extName = wstrFileName.substr( strIndex + 1 );


						for( std::wstring::iterator i = extName.begin(); i != extName.end(); i++ ) 
							*i = (WCHAR)towupper(*i);

						if ( extName == L"KOM" || extName == L"KOM_" || extName == L"DLL" || extName == L"MP3" )
						{
							vecFloder.push_back( wstrFileName );
						}
					}
				}
			}
		}

	}while( FindNextFile( hSearch, &fd ) );

	FindClose(hSearch);
}

void KEnumFile::ClearNotNeedFiles( TiXmlDocument& _xmlDocument )
{
	TiXmlNode*      node = 0;
	TiXmlElement*   dirElement = 0;
	TiXmlElement*   fileElement = 0;

	node = _xmlDocument.FirstChild( "PatchInfo" );
	dirElement = node->FirstChild("File")->ToElement();
	assert( dirElement );

	for ( int i = 0; i < (int)m_vecAllFile.size(); i++ )
	{
		wstring curFileName = m_vecAllFile[i];
		node = dirElement->FirstChild("Item");
		bool IsNeedFile = false;

		while( node != 0  )
		{
			char charBuf[255] = {0,};
			WideCharToMultiByte( CP_ACP, 0, curFileName.c_str(), -1, charBuf, 255, NULL, NULL );

			// name이 같은 경우

			basic_string <char>::size_type indexString;
			string serverFileName = node->ToElement()->Attribute("Name");
			indexString = serverFileName.rfind( "\\" );

			string realServerFileName;
			realServerFileName.assign( serverFileName.begin() + indexString + 1, serverFileName.end() );


			if( (strcmp( charBuf,
				realServerFileName.c_str()) == 0) )
			{
				IsNeedFile = true;
				break;
			}
			else
			{
				node = node->NextSibling("Item");
				continue;
			}
		} // while
		
		if ( IsNeedFile == false )
		{
			WCHAR buff[256];
			wsprintf( buff, curFileName.c_str() );
			wstring fullFileName = GetPullPath( buff );
			wstring fullFileReName = fullFileName + L"_";
/*
			string strFullFileName;
			char strBuff[256];
			memset( strBuff, 0, 256 );
			WideCharToMultiByte( CP_ACP, 0, fullFileName.c_str(), -1, strBuff, fullFileName.size(), NULL, NULL );
			strFullFileName = strBuff;
			

			string strFullFileReName;
			memset( strBuff, 0, 256 );
			WideCharToMultiByte( CP_ACP, 0, fullFileReName.c_str(), -1, strBuff, fullFileReName.size(), NULL, NULL );
			strFullFileReName = strBuff;
*/
			//DeleteFile( fullFileName );
			/*
			while ( rename( strFullFileName.c_str(), strFullFileReName.c_str() ) == EACCES )
			{
				strFullFileReName += "_";
			}
			*/

			//CopyFile( fullFileName.c_str(), fullFileReName.c_str(), false );
			DeleteFile( fullFileName.c_str() );
		}
	}
}


char* KEnumFile::GetPullPath( char* cFileName )
{
	FILE* file;
	std::string strName;

	for( int i = 0; i < (int)m_strDataDirectory.size(); ++i )
	{
		strName = m_strDataDirectory[i] + cFileName;

		file = fopen( strName.c_str(), "rb" );
		if(file != NULL)
		{
			strcpy( cFileName, strName.c_str());
			break;
		}
	}

	if( file == NULL)
	{
		return NULL;
	}

	fclose( file );
	return cFileName;
}

WCHAR* KEnumFile::GetPullPath( WCHAR* wcFileName )
{
	char strFileName[255] = {0,};
	std::string strName;

	WideCharToMultiByte( CP_ACP, 0, wcFileName, -1, strFileName, 255, NULL, NULL );

	FILE* file;

	for( int i = 0; i < (int)m_strDataDirectory.size(); ++i )
	{
		strName = m_strDataDirectory[i] + strFileName;

		file = fopen( strName.c_str(), "rb" );
		if(file != NULL)
		{
			MultiByteToWideChar( CP_ACP, 0, strName.c_str(), -1, wcFileName, 255);
			break;
		}
	}

	if( file == NULL)
	{
		return NULL;
	}

	fclose( file );
	return wcFileName;
}