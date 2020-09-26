#pragma once
#include <vector>

namespace Knc
{
	template< typename T >
    class KSingleObject
	{
	public:
		KSingleObject()
		{
		}

		KSingleObject( const T& obj )
		{
			m_vecCon.push_back( obj );
		}

		~KSingleObject()
		{
			Clear();
		}

		void Clear()
		{
			m_vecCon.clear();
		}

		bool Empty() const
		{
			return m_vecCon.empty();
		}

		bool Reg( const T& obj )
		{
			if( Empty() == false )
				return false;

			m_vecCon.push_back( obj );
			return true;
		}

		bool Unreg()
		{
			if( Empty() == true )
				return false;

			Clear();
			return true;
		}

		bool Get( T& obj ) const
		{
			if( Empty() == true )
			{
				return false;
			}

			obj = m_vecCon.front();
			return true;
		}

	private:
		std::vector< T > m_vecCon;
	};

	//////////////////////////////////////////////////////////////////////////
	template< typename T >
	class KSingleObjectTimer
	{
	public:
		KSingleObjectTimer()
		{
			m_fLimitTime = 0.0;
		}

		KSingleObjectTimer( const T& obj, const double fLimitTime )
		{
			m_fLimitTime = fLimitTime;
			m_tTimer.restart();
			m_vecCon.push_back( obj );
		}

		~KSingleObjectTimer()
		{
			Clear();
		}

		void Clear()
		{
			m_fLimitTime = 0.0;
			m_tTimer.restart();
			m_vecCon.clear();
		}

		bool Empty() const
		{
			return m_vecCon.empty();
		}

		bool Reg( const T& obj, const double fLimitTime )
		{
			if( Empty() == false )
				return false;

			m_fLimitTime = fLimitTime;
			m_tTimer.restart();
			m_vecCon.push_back( obj );
			return true;
		}

		bool Unreg()
		{
			if( Empty() == true )
				return false;

			Clear();
			return true;
		}

		bool Get( T& obj ) const
		{
			if( Empty() == true )
			{
				return false;
			}

			obj = m_vecCon.front();
			return true;
		}

		bool IsOverLimitTime() const
		{
			return ( m_tTimer.elapsed() > m_fLimitTime );
		}

		bool IfOverLimitTimeUnregObject()
		{
			if( IsOverLimitTime() == true )
			{
				Unreg();
				return true;
			}

			return false;
		}

	private:
		double				m_fLimitTime;
		boost::timer		m_tTimer;
		std::vector< T >	m_vecCon;
	};
}


