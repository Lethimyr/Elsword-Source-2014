#include "stdafx.h"
#include ".\kgcbuffermanager.h"

KGCBufferManager::KGCBufferManager()
{
	m_pData			= NULL;
	m_iMemorySize	= 1024 * 1024;		//1MB

	m_pData			= new char[m_iMemorySize];
}

KGCBufferManager::~KGCBufferManager()
{
	Flush();
}

void KGCBufferManager::Flush()
{
	if( m_pData )
	{
		delete []m_pData;
		m_pData = NULL;
	}

	m_iMemorySize = 0;
}

char* KGCBufferManager::GetBuffer(int iSize)
{
	if( m_iMemorySize < iSize )
	{
		if( m_pData != NULL )
		{		
			delete []m_pData;
			m_pData = NULL;
		}

		while( (m_iMemorySize *= 2) < iSize )
		{
		}

		m_pData = new char[m_iMemorySize];
	}

	return m_pData;
}
