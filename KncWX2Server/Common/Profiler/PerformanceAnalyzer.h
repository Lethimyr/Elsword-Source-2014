#pragma once

#include <map>
#include "Tree.h"

class KPerformanceAnalyzer
{
public:
    KPerformanceAnalyzer();
    virtual ~KPerformanceAnalyzer();

protected:

public:

protected:
    std::map< std::string, PTNIndex > m_mapNameTable;
};