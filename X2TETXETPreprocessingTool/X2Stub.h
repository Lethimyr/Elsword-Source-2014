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

extern  CKTDXApp*   g_pKTDXApp;




#endif  _X2STUB_H