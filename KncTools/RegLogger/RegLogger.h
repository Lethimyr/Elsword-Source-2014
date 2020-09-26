#pragma once

#include <string>
#include <windows.h>    // DWORD
#include "CmdLineTool.h"

class KRegLogger : public KCmdLineTool
{
public:
    KRegLogger(void);
    virtual ~KRegLogger(void);

    virtual bool Init();
    virtual bool Run();

protected:
    inline void AddEventSource();
    inline void RemoveEventSource();

public:
    std::string     m_strPath;              // 등록할 실행파일 경로.
    std::string     m_strName;              // 레지스트리에 기록될 등록 명.
    DWORD           m_dwCategoryCount;      // 이벤트 카테고리 갯수.
    bool            m_bIsRegisterAction;    // '/unreg' 일땐 false.
};

DECL_ON_FUNC( reg );
DECL_ON_FUNC( unreg );
DECL_ON_FUNC( name );
DECL_ON_FUNC( cc );
DECL_ON_FUNC( path );