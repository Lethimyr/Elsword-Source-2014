#pragma once

#include <vector>
#include <boost/timer.hpp>


//{{ 2012. 03. 29	최육사	Inventory Lock 기능
#ifdef SERV_INVENTORY_LOCK


class KInventoryLocker
{
public:
	enum LOCK_TYPE
	{
		LT_INFINITE = 0,
		LT_TIME_OUT,

		LT_END,
	};

private:
	bool			m_bLocked;
	boost::timer	m_tTimer;
	double			m_fLockTime;
	LOCK_TYPE		m_eLockType;

public:
	KInventoryLocker()
	{
		Clear();
	}

	void Clear()
	{
		m_bLocked = false;
		m_fLockTime = 0.0;
		m_eLockType = LT_INFINITE;
	}

	bool IsLocked()
	{
		// 잠겨 있지 않다면!
		if( !m_bLocked )
		{
			return false;
		}

		// 타임아웃 타입이면 타이머 체크!
		if( m_eLockType == LT_TIME_OUT )
		{
			// 시간이 오버했다면 lock을 풀자!
			if( m_tTimer.elapsed() > m_fLockTime )
			{
				m_bLocked = false;
				return false;
			}
		}

		// 그것도 아니면 Lock!
		return true;
	}

	bool Lock( IN const LOCK_TYPE eLockType, IN const double fLockTime = 99999.0 )
	{
		if( IsLocked() )
		{
			return false;
		}

		// 초기화
		Clear();
		
		// 잠금 처리!
		m_bLocked = true;
		m_eLockType = eLockType;
		if( eLockType == LT_TIME_OUT )
		{
			m_fLockTime = fLockTime;
			m_tTimer.restart();
		}
		return true;
	}

	void Unlock()
	{
		Clear();
	}
};


#endif SERV_INVENTORY_LOCK
//}}


