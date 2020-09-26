#pragma once
#ifndef _X2ITEMMANAGER_H
#define _X2ITEMMANAGER_H



class   CX2ItemManager
{
public:

    CX2ItemManager();

#include    "X2ItemManager_Preprocessing.inl"


    bool	OpenItemScriptFile( const wchar_t* pKimFile, const std::vector<std::wstring>& vecScriptFile );

private:

    std::vector<BYTE>   m_vecItemTempletInfo;

};//class   CX2ItemManager

#endif  _X2ITEMMANAGER_H