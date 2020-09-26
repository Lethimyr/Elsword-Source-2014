#pragma once

#include <windows.h>

typedef LONGLONG TickType;

class KPerformanceElement
{
public:
    KPerformanceElement();
    virtual ~KPerformanceElement();

    bool IsChecking() { return m_bIsChecking; }
    bool BeginCheck();
    bool EndCheck();
    double GetAvg();
    double GetMin();
    double GetMax();

protected:
    static TickType GetTickFrequency();
    TickType GetTickCount();

public:

protected:
    static TickType m_sttTickFrequency;
    TickType m_ttStart;
    bool m_bIsChecking;
    unsigned long m_ulCount;

    TickType m_ttTotal;
    TickType m_ttMin;
    TickType m_ttMax;
};