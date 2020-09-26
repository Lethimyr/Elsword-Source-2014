#include "StdAfx.h"
#include ".\logger.h"

CLogger* g_Logger = NULL;
CLogger::CLogger(void)
{
}

CLogger::~CLogger(void)
{
}

void CLogger::AddLog( char* strLogString )
{
    std::cout<< strLogString << std::endl;
    m_strLog += strLogString;
}

char* CLogger::GetLogString()
{
    return m_strLog.c_str();
}
