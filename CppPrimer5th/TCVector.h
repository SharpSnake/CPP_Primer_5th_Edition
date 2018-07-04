// =============================================================================
// 
// A simple imitation of std::vector.
// 
// 						              - WuYu, 2018.06.
// =============================================================================
#ifndef TCVECTOR_H
#define TCVECTOR_H

#include <cstddef>
#include <cstring>
#include <stdexcept>		// 标准库定义的一些异常类，基类都是exception
#include <initializer_list>


template< typename T >
class TCVector
{
public:
	typedef T* iterator;				// 迭代器类型
	typedef const T* const_iterator;

public:
	TCVector()
	{
		m_Pos = 0;
		m_Capacity = 8;
		m_Data = new T[ m_Capacity ];
	}

	TCVector( const TCVector &right )
	{
		m_Pos = right.m_Pos;
		m_Capacity = right.m_Capacity;
		m_Data = new T[ m_Capacity ];
		std::memcpy( m_Data, right.m_Data, sizeof( T ) * m_Pos );
	}

	~TCVector()
	{
		_Destroy();
		m_Pos = 0;
		m_Capacity = 0;
	}

public:
	void PushBack( const T &val )
	{
		if( m_Pos == m_Capacity )
			_ReAlloc();
		m_Data[ m_Pos++ ] = val;
	}

	std::size_t Size() const { return m_Pos; }

	void Clear()	// only set data value and position to zero, not destroying.
	{
		std::memset( m_Data, 0, sizeof( T ) * m_Pos );
		m_Pos = 0;
	}
	
	T& operator []( std::size_t pos )
	{
		if( pos >= m_Pos )
			throw std::out_of_range( "TCVector::operator [] : position out of range" );
		return m_Data[ pos ];
	}

	TCVector& operator =( std::initializer_list< T > list )
	{
		_Destroy();

		m_Pos = 0;
		m_Capacity = list.size();
		m_Data = new T[ m_Capacity ];
		for( auto i = list.begin(); i != list.end(); ++i )
			m_Data[ m_Pos++ ] = *i;

		return ( *this );
	}

// std interface
public:
	iterator begin()	// 若需实现range for，必须严格命名为'begin'和'end'
	{
		return m_Data;
	}

	iterator end()
	{
		return m_Data + m_Pos;
	}

private:
	void _ReAlloc()	// double the capacity
	{
		m_Capacity *= 2;
		T *tmpData = new T[ m_Capacity ];
		std::memcpy( tmpData, m_Data, sizeof( T ) * m_Pos );
		_Destroy();
		m_Data = tmpData;
	}

	void _Destroy()
	{
		if( m_Data )
		{
			delete[] m_Data;
			m_Data = nullptr;
		}
	}

private:
	std::size_t m_Pos;			// current position = current length of data
	std::size_t m_Capacity;
	T *m_Data;
};

#endif // !TCVECTOR_H
