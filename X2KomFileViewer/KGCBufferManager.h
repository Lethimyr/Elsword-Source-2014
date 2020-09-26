#pragma once

class KGCBufferManager  
{
public:
	KGCBufferManager();
	virtual ~KGCBufferManager();

	char*	GetBuffer(int iSize);
	int		GetBufferSize(){ return m_iMemorySize; }
	void	Flush();

private:
	int		m_iMemorySize;
	char*	m_pData;
};
