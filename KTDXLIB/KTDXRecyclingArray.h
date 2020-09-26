#pragma once

#pragma warning( disable : 4311 )
//이 어레이는 순서의 보장을 하지 않는다.
//어레이 엘레먼트는 기본 생성자, 복사 생성자가 있어야만 한다
template <class TArrayElement,int INITIAL_SIZE = 100>
class CKTDXRecyclingArray
{
	public:
		CKTDXRecyclingArray()
		{
			m_Capacity		= INITIAL_SIZE;
			m_Size			= 0;
			m_LastNewIndex	= 0;
			m_bChange		= true;

			m_pArray		= new TArrayElement[m_Capacity];
			m_bAlive		= new bool[m_Capacity];
			m_pOutputArray	= new TArrayElement*[m_Capacity];

			for( int i = 0; i < m_Capacity; i++ )
				m_bAlive[i] = false;
		}

		~CKTDXRecyclingArray(void)
		{
			delete [] ( m_pArray );
			delete [] ( m_bAlive );
			delete [] ( m_pOutputArray );
		}

		TArrayElement*	New( TArrayElement* pElement = NULL )
		{
			KTDXPROFILE();

			m_bChange = true;

			if( m_Capacity - m_Size <= 0 )
				ResetCapacity();

			int findNew = m_LastNewIndex;

			for( int i = 0; i < m_Capacity; i++ ) 
			{
				if( m_bAlive[findNew] == false ) 
					// we've found our free spot!  use it!
					break;
				else 
				{
					findNew++; 
					if( findNew >= m_Capacity ) 
						findNew = 0;
				}
			}

			if( m_bAlive[findNew] )
				return NULL; //실패

			if( pElement == NULL )
				m_pArray[findNew] = m_NewTemplet;
			else
				m_pArray[findNew] = *pElement;
			m_bAlive[findNew] = true;
			m_Size++;
			m_LastNewIndex = findNew;
			
			return &m_pArray[findNew];
		}

		bool Delete( int index )
		{
			KTDXPROFILE();

			m_bChange = true;

			if( index < 0 || index >= m_Capacity || m_bAlive[index] == false ) 
				return false;

			// don't actually delete element at index;
			// just mark it free.
			m_bAlive[index] = false;
			m_Size--;

			return true;	
		}

		bool Delete( TArrayElement *elem )
		{
			KTDXPROFILE();

			if( m_Capacity == 0 ) 
				return false;

			int iElem = reinterpret_cast<int>(elem);
			iElem -= reinterpret_cast<int>(&m_pArray[0]);
			int index = iElem / sizeof(TArrayElement);

			return Delete(index); 
		}

		void Clear()
		{
			KTDXPROFILE();
			if( 0 == m_Size )
				return;
			
			m_bChange = true;
			ZeroMemory( m_bAlive, sizeof(bool) * m_Capacity );
			//for( int i = 0; i < m_Capacity; i++ )
			//{
			//	m_bAlive[i] = false;
			//}
			m_Size = 0;
		}

		bool ResetCapacity( int capacity = 0 )
		{
			KTDXPROFILE();

			m_bChange = true;

			if( capacity < m_Capacity )
			{
				if( capacity != 0 )
				{
					return false;
				}
				else
				{
					capacity = m_Capacity * 2;
				}
			}

			TArrayElement*	pArray	= new TArrayElement[capacity];
			bool*			bAlive	= new bool[capacity];


			for( int i = 0; i < capacity; i++ )
				bAlive[i] = false;

			memcpy( pArray, m_pArray, sizeof(TArrayElement) * m_Capacity );
			memcpy( bAlive, m_bAlive, sizeof(bool) * m_Capacity );

			delete [] ( m_pArray );
			delete [] ( m_bAlive );
			delete [] ( m_pOutputArray );

			m_pOutputArray = new TArrayElement*[capacity];

			m_pArray = pArray;
			m_bAlive = bAlive;
			m_Capacity = capacity;
			
			return true;
		}

		void MergeAlive()
		{
			KTDXPROFILE();

			m_bChange = true;

			bool remain = true;

			for( int i = 0; i < m_Capacity; i++ )
			{
				if( m_bAlive[i] == false )
				{
					for( int j = i; j < m_Capacity; j++ )
					{
						if( m_bAlive[j] == true )
						{
							Swap( i, j );
							remain = true;
							break;
						}
						remain = false;
					}
				}
				if( remain == false )
				{
					break;
				}
			}

			m_LastNewIndex = m_Size - 1;
		}

		TArrayElement* GetElement( int index )
		{
			if( m_bAlive[index] == false )
				return NULL;

			return &m_pArray[index];
		}

		TArrayElement** GetLiveElementList()
		{
			KTDXPROFILE();
			if( m_bChange == true )
			{
				int hit = 0;
				for( int i = 0; i < m_Capacity; i++ )
				{
					if( hit == m_Size )
						break;
					if( m_bAlive[i] == true )
					{
						m_pOutputArray[hit] = &m_pArray[i];
						hit++;
					}
				}
			}
			m_bChange = false;

			return m_pOutputArray;
		}

		int GetCapacity() const { return m_Capacity; }
		int GetSize() const { return m_Size; }

	private:
		bool Swap( int indexA, int indexB )
		{
			KTDXPROFILE();

			if( indexA >= m_Capacity || indexB >= m_Capacity )
				return false;
			
			TArrayElement element = m_pArray[indexA];
			m_pArray[indexA] = m_pArray[indexB];
			m_pArray[indexB] = element;

			bool alive = m_bAlive[indexA];
			m_bAlive[indexA] = m_bAlive[indexB];
			m_bAlive[indexB] = alive;

			return true;
		}

		TArrayElement*	m_pArray;
		bool*			m_bAlive;
		TArrayElement	m_NewTemplet;
		TArrayElement**	m_pOutputArray;

		int				m_Capacity;
		int				m_Size;
		int				m_LastNewIndex;
		bool			m_bChange;
};
