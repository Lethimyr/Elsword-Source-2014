#pragma once

template< class T >
class KCacheData
{
public:
	KCacheData()
		:
	m_tVerifyMin(0)
	{}
public:
    void SetValue( T tValue )
    {
        m_tInit     = tValue;
        m_tAlive    = tValue;
    }
	void SetVerifyMin( T tValue )
	{
		m_tVerifyMin = tValue;
	}

	void SetAliveValue( T tValue )
	{
		m_tAlive	= tValue;
	}

    T GetChangeValue()	const { return m_tAlive - m_tInit; }
    bool IsChanged()    { return (m_tAlive != m_tInit); }

    operator T(void) const { return m_tAlive; }

	void operator+=( const T tValue )       { m_tAlive += tValue; if( m_tAlive < m_tVerifyMin ){ m_tAlive = m_tVerifyMin; } }
    void operator-=( const T tValue )       { m_tAlive -= tValue; if( m_tAlive < m_tVerifyMin ){ m_tAlive = m_tVerifyMin; } }

    // db 작업이 실패한 경우, m_tInit 수치를 다시 이전으로 되돌릴 인터페이스가 필요하다.
    T GetInitValue()                        { return m_tInit; }
    void AdjustInitValue( const T tValue )  { m_tInit = tValue; }

private:
    T   m_tInit;
    T   m_tAlive;

	T	m_tVerifyMin;
};


template< class T, class MT >
class KCacheDataMaxLimit
{
public:
	void SetValue( T tValue )
	{
		m_tInit     = tValue;
		m_tAlive    = tValue;
	}

	void SetMaxValue( T tMaxValue )
	{
		m_tMaxValue = tMaxValue;
	}

	T GetMaxValue() const		{ return m_tMaxValue; }
	T GetChangeValue() const	{ return m_tAlive - m_tInit; }
	bool IsChanged() const		{ return (m_tAlive != m_tInit); }

	operator T(void) const { return m_tAlive; }

	void operator+=( const T tValue )
	{
		MT tValueCheck = static_cast<MT>(m_tAlive) + static_cast<MT>(tValue);

		m_tAlive += tValue;
		if( tValueCheck < 0 ) { m_tAlive = 0; }
		if( tValueCheck > static_cast<MT>(m_tMaxValue) ) { m_tAlive = m_tMaxValue; }
	}

	void operator-=( const T tValue )
	{
		MT tValueCheck = static_cast<MT>(m_tAlive) - static_cast<MT>(tValue);

		m_tAlive -= tValue;
		if( tValueCheck < 0 ) { m_tAlive = 0; }
		if( tValueCheck > static_cast<MT>(m_tMaxValue) ) { m_tAlive = m_tMaxValue; }
	}

	// db 작업이 실패한 경우, m_tInit 수치를 다시 이전으로 되돌릴 인터페이스가 필요하다.
	T GetInitValue()                       { return m_tInit; }
	void AdjustInitValue( const T tValue ) { m_tInit = tValue; }

private:
	T   m_tInit;
	T   m_tAlive;
	T	m_tMaxValue;
};


