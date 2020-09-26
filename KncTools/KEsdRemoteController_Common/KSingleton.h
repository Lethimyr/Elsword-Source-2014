#pragma once

#define USE_SINASSERT

#include "KSuperAssert.h"

#include <cassert>
#include <string>
#include <vector>
#include <algorithm>

//---------------------------------------------------------------------------
class KSingletonObject
{

public:
	std::string m_strSingletonName;
	bool m_bReleased;

public:
	KSingletonObject(std::string strSingletonName)
	: m_strSingletonName(strSingletonName), m_bReleased(false) {}

	virtual ~KSingletonObject()
	{
		m_strSingletonName.clear();
	}

};
//---------------------------------------------------------------------------
class KSingletonBase
{

protected:
	typedef std::vector<KSingletonObject*> KSingletonObjectContainer;
	static KSingletonObjectContainer m_vecSingletonObject;

public:
	static void ShowAllInstance()
	{
#ifdef USE_SINASSERT
        KDbgStm << "[Show Allocated Instance]" << trace;
#endif // USE_SINASSERT

		if (!m_vecSingletonObject.empty())
		{
			KSingletonObjectContainer::iterator iO;
			for (iO = m_vecSingletonObject.begin(); iO != m_vecSingletonObject.end(); ++iO)
			{
				KSingletonObject* pkSingletonObject = *iO;
#ifdef USE_SINASSERT
				KDbgStm << "\n" << pkSingletonObject->m_strSingletonName.c_str() << trace;
#endif // USE_SINASSERT
			}
		}
	}

	static void DeleteAllInstance()
	{
		KSingletonObjectContainer::iterator iO;
		for (iO = m_vecSingletonObject.begin(); iO != m_vecSingletonObject.end(); ++iO)
		{
			KSingletonObject* pkSingletonObject = *iO;
#ifdef USE_SINASSERT
			KDbgStm << pkSingletonObject->m_strSingletonName.c_str();
			KDbgStm << "\n모든 Singleton 객체를 제거하지 않았습니다.";
			KAssert(pkSingletonObject->m_bReleased == true);
#endif // USE_SINASSERT

			delete pkSingletonObject;
		}

		m_vecSingletonObject.clear();
	}

};
//---------------------------------------------------------------------------
template <class T>
class KSingleton : public KSingletonBase
{
protected:
	// Singleton Object
	static T* m_pkSingletonChunk;	

public:
	// Singleton Pattern ( Can make object Only one in this program )
	// Set Object Instance
	static void SetInstance(T* i_pSingletonObject)
	{
		m_pkSingletonChunk = i_pSingletonObject;
	}

	// Create Instance
	static void CreateInstance()
	{
#ifdef USE_SINASSERT
		KDbgStm << "이미 생성된 객체를 생성 시도하였습니다.";
		KAssert(m_pkSingletonChunk == NULL);
#endif // USE_SINASSERT

        std::string strTypeName = typeid(m_pkSingletonChunk).name();
#ifdef USE_SINASSERT
		KDbgStm << "[Create Instance] " << strTypeName.c_str() << trace;
#endif // USE_SINASSERT

		m_pkSingletonChunk = new T;

		KSingletonObjectContainer::iterator iO;
		for (iO = m_vecSingletonObject.begin(); iO != m_vecSingletonObject.end(); ++iO)
		{
			KSingletonObject* pkSingletonObject = *iO;

			if (pkSingletonObject->m_strSingletonName == strTypeName)
			{
#ifdef USE_SINASSERT
				KAssert(!"한번 지운 객체를 재 생성 시도하였습니다.");
#endif // USE_SINASSERT
			}
		}

		if (iO == m_vecSingletonObject.end())
		{
			m_vecSingletonObject.push_back(new KSingletonObject(strTypeName));
		}
	}

	// Singleton Pattern ( Can make object Only one in this program )
	// Get Object Instance
	static T* GetInstance()
	{
		if (!m_pkSingletonChunk) CreateInstance();
		
		return m_pkSingletonChunk;
	}

	// Delete Instance
	static void DeleteInstance()
	{
		if (m_pkSingletonChunk == NULL)
			return;

		//SuperAssert( m_pkSingletonChunk != NULL ).msg( "이미 제거된 객체를 제거 시도하였습니다." ).fatal();

        std::string strTypeName = typeid(m_pkSingletonChunk).name();

		KSingletonObjectContainer::iterator iO;
		for (iO = m_vecSingletonObject.begin(); iO != m_vecSingletonObject.end(); )
		{
			KSingletonObject* pkSingletonObject = *iO;

			if (pkSingletonObject->m_strSingletonName == strTypeName)
			{
				pkSingletonObject->m_bReleased = true;

				delete pkSingletonObject;
				iO = m_vecSingletonObject.erase(iO);
			}
			else
			{
				++iO;
			}
		}

		delete m_pkSingletonChunk;
		m_pkSingletonChunk = NULL;
	}

    static bool IsAllocated() { return m_pkSingletonChunk ? true : false; }

protected:
	// Protect Call Creation by User
	KSingleton() {}

};
//---------------------------------------------------------------------------

template <class T>
T* KSingleton<T>::m_pkSingletonChunk = 0;

#define KDeclareSingleton(mClassName)			\
    inline mClassName* _##mClassName##()		\
	{ return mClassName::GetInstance(); }

#define KDeclareGlobalVal(mClassName)			\
	extern mClassName g_##mClassName;			\
	inline mClassName* _##mClassName##()		\
	{ return &g_##mClassName; }

#define KImplementGlobalVal(mClassName)			\
	mClassName g_##mClassName;

/*

	* 사용 방법

	1. pubic으로 KSingleton클래스를 상속 받습니다.
	2. Header의 하단에 KDeclareSingleton 매크로를 이용하여 접근 인라인 함수를 정의합니다.

	Ex)
		class KObjectMgr : public KSingleton< KObjectMgr >
		{
		};

		KDeclareSingleton( KObjectMgr );

*/


//-----------------------------------------------------------------------------
#define DeclareSingleton( classname ) \
	public: \
	static classname* GetInstance(); \
	static void ReleaseInstance(); \
	protected: \
	static classname*   ms_selfInstance

#define DefSingletonInline(ClassName)   \
	inline ClassName* Si##ClassName##() { return ClassName::GetInstance(); }

#define ImplementSingleton( classname ) \
	classname* classname::ms_selfInstance = NULL; \
	classname* classname::GetInstance() \
{ \
	if( ms_selfInstance == NULL ) \
	ms_selfInstance = new classname; \
	return ms_selfInstance; \
} \
	void classname::ReleaseInstance() \
{ \
	if( ms_selfInstance != NULL ) \
{ \
	delete ms_selfInstance; \
	ms_selfInstance = NULL; \
} \
}
