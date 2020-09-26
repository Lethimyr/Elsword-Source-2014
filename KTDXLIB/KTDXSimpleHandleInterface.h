#pragma once

template<typename T>
class CKTDXSimpleHandleInterface
{
public: 
	typedef UINT Handle; 
	static const Handle INVALID_HANDLE		= 0;
	static const Handle VALID_HANDLE_START	= 2;
	static const Handle VALID_HANDLE_END	= UINT_MAX-2;

public:
	~CKTDXSimpleHandleInterface(void) {};
	CKTDXSimpleHandleInterface(void)
	{
		m_iNextValidHandle = VALID_HANDLE_START;
	}
	
	Handle GetNextValidHandle()
	{
		m_iNextValidHandle++;

		if( m_iNextValidHandle > VALID_HANDLE_END )
		{
			m_iNextValidHandle = VALID_HANDLE_START;
		}

		return m_iNextValidHandle;
	}

	T* GetObjectByHandle( const Handle hHandle )
	{
		std::map< Handle, T* >::iterator it = m_mapHandleToObject.find( hHandle );
		if( it != m_mapHandleToObject.end() )
		{
			return it->second;
		}

		return NULL;
	}

	bool AssignHandle( const Handle hHandle, T* pObject )
	{
		if( INVALID_HANDLE == hHandle )
			return false;

		if( NULL == pObject )
			return false; 

		m_mapHandleToObject[ hHandle ] = pObject;
		return true;
	}

	void ReleaseHandle( const Handle hHandle )
	{
		m_mapHandleToObject.erase( hHandle );
	}

	
private:
	std::map< Handle, T* > m_mapHandleToObject;
	Handle m_iNextValidHandle;
};
