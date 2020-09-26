//-----------------------------------------------------------------------------
template<class ObjType>
KMemPool<ObjType>::KMemPool()
{
}
//-----------------------------------------------------------------------------
template<class ObjType>
KMemPool<ObjType>::~KMemPool()
{
	DestroyPool();
	SetCreator(NULL);
}
//-----------------------------------------------------------------------------
template<class ObjType>
void KMemPool<ObjType>::SetCreator(KPoolCreator* pkCreator)
{
	if (m_kPoolData.m_pkCreator != NULL)
	{
		m_kPoolData.m_pkCreator->CleanUp();
		m_kPoolData.m_pkCreator = NULL;
	}

	m_kPoolData.m_pkCreator = pkCreator;
}
//-----------------------------------------------------------------------------
template<class ObjType>
void KMemPool<ObjType>::CreatePool(unsigned int uiSize, KPoolCreator* pkCreator)
{
	SetCreator(pkCreator);

	ExtendPool(uiSize);
}
//---------------------------------------------------------------------------
template<class ObjType>
void KMemPool<ObjType>::ExtendPool(unsigned int uiSize)
{
	// Creator 설정, NULL 체크
	if (m_kPoolData.m_pkCreator == NULL)
		m_kPoolData.m_pkCreator = new KDefaultCreator<ObjType>;

	unsigned int iC;
	for (iC = 0; iC < uiSize; ++iC)
	{
		void* pvChunk = m_kPoolData.m_pkCreator->Create();

		m_kPoolData.m_deqFree.push_back(pvChunk);
		m_kPoolData.m_deqAlloc.push_back(pvChunk);
	}
}
//---------------------------------------------------------------------------
template<class ObjType>
void KMemPool<ObjType>::DestroyPool()
{
	unsigned int iC;
	for (iC = 0; iC < m_kPoolData.m_deqAlloc.size(); ++iC)
	{
		void* pvChunk = m_kPoolData.m_deqAlloc[iC];

		m_kPoolData.m_pkCreator->Destroy(pvChunk);
	}

	m_kPoolData.m_deqAlloc.clear();
}
//---------------------------------------------------------------------------
template<class ObjType>
ObjType* KMemPool<ObjType>::Alloc()
{
	if (m_kPoolData.m_deqFree.empty())
	{
		ExtendPool((int) m_kPoolData.m_deqAlloc.size() + 1);
	}

	void* pkRes = *m_kPoolData.m_deqFree.begin();
	m_kPoolData.m_deqFree.pop_front();

	((ObjType*) pkRes)->InitPool();
	return (ObjType*) pkRes;
}
//---------------------------------------------------------------------------
template<class ObjType>
void KMemPool<ObjType>::Free(void* pvChunk)
{
	((ObjType*) pvChunk)->ReleasePool();
	m_kPoolData.m_deqFree.push_back(pvChunk);
}
//---------------------------------------------------------------------------