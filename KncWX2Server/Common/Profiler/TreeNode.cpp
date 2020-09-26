#include "TreeNode.h"

KPerformanceTreeNode::KPerformanceTreeNode()
: m_iIndex( 0 )
{
}

KPerformanceTreeNode::KPerformanceTreeNode( const PTNIndex& iIndex )
: m_iIndex( iIndex )
{
}

KPerformanceTreeNode::~KPerformanceTreeNode()
{
}

KPerformanceTreeNodePtr KPerformanceTreeNode::GetChild( const PTNIndex& iIndex )
{
    std::map< PTNIndex, KPerformanceTreeNodePtr >::const_iterator mit = m_mapChild.find( iIndex );

    if( mit != m_mapChild.end() )
    {
        return mit->second;
    }
    else
    {
        return KPerformanceTreeNodePtr();
    }
}

bool KPerformanceTreeNode::BeginCheck()
{
    return m_kPerformanceElement.BeginCheck();
}

bool KPerformanceTreeNode::EndCheck()
{
    return m_kPerformanceElement.EndCheck();
}
