#pragma once
#ifndef _X2STUB_H
#define _X2STUB_H


class   CX2ItemManager;

class   CKTDXApp
{
public:

    CKTDXApp();
    ~CKTDXApp();

    KLuabinder*	GetLuaBinder()  { return m_pLuaBinder; }

private:

    KLuabinder*   m_pLuaBinder;
};

class   CX2Data
{
public:

    CX2Data();
    ~CX2Data();

    CX2ItemManager*     GetItemManager()    { return m_pItemManager; }

private:

    CX2ItemManager*     m_pItemManager;
};

extern  CKTDXApp*   g_pKTDXApp;
extern  CX2Data*    g_pData;



#endif  _X2STUB_H