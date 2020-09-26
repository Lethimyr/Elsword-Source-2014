#pragma once

#include "TreeNode.h"

class KPerformanceTree
{
public:
    KPerformanceTree();
    virtual ~KPerformanceTree();

protected:
    PTNIndex GetNextIndex() { return ++m_iCurrentIndex; }

public:

protected:
    PTNIndex m_iCurrentIndex;

    KPerformanceTreeNodePtr m_spRoot;

};