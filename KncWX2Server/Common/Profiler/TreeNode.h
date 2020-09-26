#pragma once

#include <map>
#include "KncSmartPtr.h"

#include "PerformanceElement.h"

#define KPerformanceTreeNodePtr KPerformanceTreeNode*
#undef KPerformanceTreeNodePtr
SmartPointer( KPerformanceTreeNode );

typedef int PTNIndex;

class KPerformanceTreeNode
{
public:
    KPerformanceTreeNode();
    KPerformanceTreeNode( const PTNIndex& iIndex );
    virtual ~KPerformanceTreeNode();

    PTNIndex GetIndex() { return m_iIndex; }
    KPerformanceTreeNodePtr GetParent() { return m_spParent; }
    KPerformanceTreeNodePtr GetPreviousSibling() { return m_spPreviousSibling; }
    KPerformanceTreeNodePtr GetNextSibling() { return m_spNextSibling; }
    KPerformanceTreeNodePtr GetChild( const PTNIndex& iIndex );

    bool BeginCheck();
    bool EndCheck();

protected:

public:

protected:
    PTNIndex m_iIndex;
    std::wstring m_wstrName;

    KPerformanceTreeNodePtr m_spParent;
    KPerformanceTreeNodePtr m_spPreviousSibling;
    KPerformanceTreeNodePtr m_spNextSibling;
    std::map< PTNIndex, KPerformanceTreeNodePtr > m_mapChild;

    KPerformanceElement m_kPerformanceElement;
};