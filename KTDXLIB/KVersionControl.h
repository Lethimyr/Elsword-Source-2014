
// dmlee, 2009-01-31, 국가별, 패치별 버전을 관리하기 위해 만듬 - GC팀에서 관리하는 방법 참조해서 수정, 보완 필요. 테스트 필요.


#pragma once

#include <map>


class KVersionControl
{
public:
	enum COUNTRY_CODE
	{
		CC_KR,
		CC_END,
	};

	enum MAJOR_VERSION
	{
		MajorV_SERVICE,
		MajorV_SERVICE_OPEN_TEST,
		MajorV_IN_HOUSE,
		MajorV_NEW_VILLAGE_SERVICE,
		MajorV_NEW_VILLAGE_OPEN_TEST,
		MajorV_END,
	};


	enum COMPONENT_NAME
	{
		CN_ROOT,

		CN_IN_HOUSE_TEST_ONLY,
		CN_SERVICE_READY,

		CN_EVE_CLASS_CHANGE,
		CN_END,
	};

	struct ComponentNode
	{
		COMPONENT_NAME	m_eName;
		bool			m_bEnabled;
		std::map< COMPONENT_NAME, ComponentNode > m_mapChildComponent;

		ComponentNode()
		{
			m_eName = CN_ROOT;
			m_bEnabled = true;
		}
	};


public:
	KVersionControl(void);
	~KVersionControl(void);

	static COUNTRY_CODE GetCountryCode() { return s_CountryCode; }
	static MAJOR_VERSION GetMajorVersion() { return s_MajorVersion; }

	static bool IsEnabled( COMPONENT_NAME eComponentName )
	{
		return IsEnabled( &s_RootComponentNode, eComponentName );
	}



protected:
	static bool SetEnabled( COMPONENT_NAME eComponentName, bool bEnable )
	{
		ComponentNode* pNode = GetComponentNode( eComponentName );
		ASSERT( pNode != NULL );
		if( NULL == pNode )
			return false;

		pNode->m_bEnabled = bEnable;
		return true;
	}


	static bool IsEnabled( ComponentNode* pRootNode, COMPONENT_NAME eComponentName )
	{
		if( pRootNode == NULL )
			return false;

		if( eComponentName == pRootNode->m_eName )
		{
			return pRootNode->m_bEnabled;
		}
		else if( false == pRootNode->m_mapChildComponent.empty() )
		{
			std::map< COMPONENT_NAME, ComponentNode >::iterator it;
			for( it=pRootNode->m_mapChildComponent.begin(); it!=pRootNode->m_mapChildComponent.end(); it++ )
			{
				ComponentNode* pChildNode = &it->second;
				if( true == IsEnabled( pChildNode, eComponentName ) )
					return true;
			}

			return false;
		}
	
		return false;
	}

	static ComponentNode* GetComponentNode( COMPONENT_NAME eComponentName )
	{
		return GetComponentNode( &s_RootComponentNode, eComponentName );
	}


	static ComponentNode* GetComponentNode( ComponentNode* pRootNode, COMPONENT_NAME eComponentName )
	{
		if( pRootNode == NULL )
			return NULL;

		if( eComponentName == pRootNode->m_eName )
		{
			return pRootNode;
		}
		else if( false == pRootNode->m_mapChildComponent.empty() )
		{
			std::map< COMPONENT_NAME, ComponentNode >::iterator it;
			for( it=pRootNode->m_mapChildComponent.begin(); it!=pRootNode->m_mapChildComponent.end(); it++ )
			{
				ComponentNode* pChildNode = &it->second;
				pChildNode = GetComponentNode( pChildNode, eComponentName );
				if( pChildNode != NULL )
					return pChildNode;
			}

			return NULL;
		}

		return NULL;
	}


	static bool AddNode( COMPONENT_NAME eParentComponentName, COMPONENT_NAME eComponentName, bool bEnabled )
	{
		ComponentNode* pParentNode = GetComponentNode( eParentComponentName );
		ASSERT( NULL != pParentNode );
		if( NULL == pParentNode )
			return false;

		ComponentNode* pNode = GetComponentNode( eComponentName );
		ASSERT( NULL == pNode );
		if( NULL != pNode )
			return false;

		ComponentNode newNode;
		newNode.m_eName = eComponentName;
		newNode.m_bEnabled = bEnabled;

		pParentNode->m_mapChildComponent[ eComponentName ] = newNode;
		return true;
	}

private:
	static COUNTRY_CODE s_CountryCode;
	static MAJOR_VERSION s_MajorVersion;

	static ComponentNode s_RootComponentNode;
};
