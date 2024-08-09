// =============================================================================
// 
// A implementation(imitation) of std::vector.
// 
// 						              - WuYu, 2018.06.
// =============================================================================
#ifndef TCVECTOR_H
#define TCVECTOR_H

#include <initializer_list>
#include <memory>		// allocator
#include <utility>		// std::move, std::swap(since C++11)
#include <algorithm>	// std::sort
#include <iostream>

#include "Vector_Base.hpp"


template< typename T, typename AllocType = std::allocator< T > >
class TCVector
	: public Vector_Container< T >
{
	using my_base = Vector_Container< T >;
	using my_base::m_First;
	using my_base::m_Current;
	using my_base::m_End;

public:
	using my_base::pointer;
	using my_base::const_pointer;
	using my_base::const_reference;
	using my_base::size_type;

public:
	typedef T*							iterator;				// ����������
	typedef const T*					const_iterator;
	typedef Vector_Iterator< Vector_Container< T > >		_iterator;
	typedef const Vector_Iterator< Vector_Container< T > >	_const_iterator;


	// Ĭ�Ϲ��죬����Ϊ�գ�ֱ������PushBackʱ����8������
	TCVector() : Vector_Container() {}

	// �����б��ʼ��
	TCVector( std::initializer_list< T > list )
		: TCVector()
	{
		_Alloc_Copy( list.begin(), list.end() );
	}

	/**
	*  �������죬��ϳ�Ĭ�Ϲ��첻ͬ����û���Զ���ʱ�����������ܻ�ϳɡ���������
	*
	* @param  allocByCapacity  Ĭ�ϰ�right��size�����ڴ棬����Capacity������
	*/ 
	TCVector( const TCVector &right, bool allocByCapacity = false )
		: TCVector()
	{
		_Alloc_Copy( right.m_First, right.m_Current, 
			allocByCapacity ? right.m_End : nullptr );
	}

	// �ƶ����죬��ȡ��ֵ����Դ�����뱣֤�ƺ�Դ����������ȫ����
	// ��noexcept���������Ͷ��崦����Ҫ�С����������������б�
	TCVector( TCVector &&right ) noexcept	MCPP11
		: Vector_Container( std::move( right ) ) {}

	/**
	*  ������ֵ�������һ�� = �ع��� + ��������������ԭ��
	*  1����֤���Ը�ֵ����������������
	*  2�����ȷ��䡿����Դ��������ֵ��Ȼ�������١�����Դ��ȷ���Ը�ֵ���ҷ���ʧ�ܿ��ԡ��ع�����
	*/
	TCVector& operator =( const TCVector &right )
	{
		_Alloc_Copy( right.m_First, right.m_Current );
		return *this;
	}

	/*
	// ���á���������������ʵ��operator =��ע������������ã�����ʵ����һ����������ʱ������
	// ���Բ��õ����Ը�ֵ����ֵ�����ʱ�����Զ��ͷ�
	TCVector& operator =( TCVector right )
	{
		swap( *this, right );
		return *this;
	}
	*/

	// �ƶ���ֵ����������ƶ���û�б�Ҫ��
	TCVector& operator =( TCVector &&right ) noexcept	MCPP11
	{
		if( &right != this )
		{
			_Destroy();
			m_First = right.m_First;
			m_Current = right.m_Current;
			m_End = right.m_End;
			right.m_First = right.m_Current = right.m_End = nullptr;
		}
		return *this;
	}

	// ���ڳ�ʼ���б�ֵ
	TCVector& operator =( std::initializer_list< T > list )
	{
		_Alloc_Copy( list.begin(), list.end() );
		return *this;
	}

	// �������ں�����ִ����Ϻ󣬰���������˳��ġ����򡿣�������ٳ�Ա����
	virtual ~TCVector() { _Destroy(); }


public:
	size_type		Size()		const	{ return m_Current - m_First; }
	size_type		Capacity()	const	{ return m_End - m_First; }

	// ����ʵ��range for�������ϸ�����Ϊ"begin"��"end"
	iterator		begin()				{ return m_First; }
	const_iterator	begin()		const	{ return m_First; }
	iterator		end()				{ return m_Current; }
	const_iterator	end()		const	{ return m_Current; }

	// �����޶���&��ֻ�ܱ���ֵ���ã�����������������
	// ���������const�ģ��޶������롾����const���桿
	TCVector Sorted() const &	MCPP11
	{
		TCVector retVal( *this );
		std::sort( retVal.begin(), retVal.end() );
		return retVal;
	}

	// �����޶���&&��ֻ�ܱ���ֵ���ã�ֱ�Ӷ��������򲢷���
	// ���һ����Ա�������޶�������ô����ͬ����ͬ�����ġ����ء��������������޶���
	TCVector Sorted() &&	MCPP11
	{
		std::sort( this->begin(), this->end() );
		return *this;
	}

	// ��ĩβ���һ��Ԫ�أ�����汾
	void PushBack( const_reference val )
	{
		if( m_Current == m_End )
			_Alloc_Grow();
		m_Alloc.construct( m_Current++, val );	// ���ｫʹ��T�ġ��������졿
	}

	// �������ذ汾�����һ���������󣬽�ʹ��T���ƶ�����
	void PushBack( T &&rrval )	MCPP11
	{
		if( m_Current == m_End )
			_Alloc_Grow();
		m_Alloc.construct( m_Current++, std::move( rrval ) );
	}

	// ��ĩβ���һ��Ԫ�أ��ֶ�����
	template< typename... Args >
	void EmplaceBack( Args&&... args )
	{
		if( m_Current == m_End )
			_Alloc_Grow();
		m_Alloc.construct( m_Current++, std::forward< Args >( args )... );
	}

	// �ͷ�ĩβ�ļ���Ԫ�أ�Ĭ���ͷ����һ��
	void PopBack( size_type count = 1 )
	{
		if( count < Size() )
		{
			for( size_type i = 0; i < count; ++i )
				m_Alloc.destroy( --m_Current );
		}
		else
			Clear();
	}

	// Ԥ����ָ�����ȵ��ڴ棬�Ѵ��ڵ�Ԫ�ػᱣ��
	void Reserve( size_type newcapacity )
	{
		if( newcapacity > Capacity() )
			_Alloc_Grow( newcapacity );
	}

	// ����Ԫ�صĳ��ȣ���ָ��Ԫ�����ĩβ����׷�ӵĲ��֣�û��ָ����ʹ��Ĭ�Ϲ������
	void Resize( size_type newsize, const_reference addval = T() )
	{
		if( newsize < Size() )
		{
			PopBack( Size() - newsize );
		}
		else if( newsize > Size() )
		{
			Reserve( newsize );
			m_Current = std::uninitialized_fill_n( m_Current, newsize - Size(), addval );
		}
	}

	// ������Դ�ռ�
	void ShrinkToFit();

	// �����ÿ��Ԫ�أ�������Դ�����ͷ�
	void Clear()
	{
		if( Size() )
		{
			for( auto i = m_First; i != m_Current; ++i )
				m_Alloc.destroy( i );
			m_Current = m_First;
		}
	}

	// ����ӡ�friend�����Σ�������std::swap�ġ����ء������������һ����Ա����
	// std::swap������ġ��ƶ���������ƶ���ֵ���ٵģ������û��ʵ���ƶ�����û������������
	friend void swap( TCVector &left, TCVector &right )
	{
		using std::swap;	// ����ĵ��ö���Ҫ��std���������޶���
		swap( left.m_First, right.m_First );
		swap( left.m_Current, right.m_Current );
		swap( left.m_End, right.m_End );
	}
	
	// �±�����������ǳ�Ա����
	T& operator[]( size_type pos )
	{
		if( pos >= Size() )
			throw std::out_of_range( "TCVector::operator [] : position out of range" );
		return *( m_First + pos );
	}

	const_reference operator[]( size_type pos ) const
	{
		if( pos >= Size() )
			throw std::out_of_range( "TCVector::operator [] : position out of range" );
		return *( m_First + pos );
	}

private:
	// �������䣬���pend��Ϊ�գ�����first��pend�ĳ��ȷ���
	void _Alloc_Copy( const_pointer first, const_pointer last, 
		const_pointer pend = nullptr )
	{
		size_type newcap = pend ? pend - first : last - first;
		pointer newfirst = m_Alloc.allocate( newcap );
		pointer newCurrent = std::uninitialized_copy( first, last, newfirst );

		_Destroy();
		m_First = newfirst;
		m_Current = newCurrent;
		m_End = m_First + newcap;
	}

	// ���������䣬����ָ��Ҫ������ڴ泤��
	void _Alloc_Grow( size_type customcap = 0 )
	{
		// ���η���8������
		size_type newcap = customcap ? customcap : ( Capacity() ? Capacity() * 2 : 8 );
		pointer newfirst = m_Alloc.allocate( newcap );
		pointer newp = newfirst;

		// ����Ҳ����ʹ���ƶ�������ʵ�ֿ��ٿ�����Ч����һ���ģ�
		//m_Current = std::uninitialized_copy( make_move_iterator( begin() ), 
		//	make_move_iterator( end() ), newfirst );
		for( auto i = m_First; i != m_Current; ++i )
			m_Alloc.construct( newp++, std::move( *i ) ); MCPP11	// ʹ���ƶ������������

		_Destroy();
		m_First = newfirst;
		m_Current = newp;
		m_End = m_First + newcap;
	}

	// ����ÿ��Ԫ�أ����ͷ�������Դ
	void _Destroy()
	{
		if( m_First )
		{
			for( auto i = m_First; i != m_Current; ++i )
				m_Alloc.destroy( i );

			m_Alloc.deallocate( m_First, m_End - m_First );
			m_First = m_Current = m_End = nullptr;
		}
	}

private:
	AllocType m_Alloc;		// �ڴ������
};

// ���ⶨ��Ҫ��ģ���б�
template< typename T, typename AllocType >
inline void TCVector< T, AllocType >::ShrinkToFit()
{
	if( this->Capacity() > this->Size() )
	{
		if( Size() == 0 )
			_Destroy();
		else
			_Alloc_Grow( Size() );
	}
}

template< typename T, typename AllocType >
std::ostream& operator <<( std::ostream &o, const TCVector< T, AllocType > &vec )
{
	o << "Elements:" << std::endl;
	for( auto &i : vec )
		o << "\t" << i << std::endl;
	return o << "Size:\t\t" << vec.Size() << std::endl << "Capacity:\t" << vec.Capacity();
}

// C++11��ģ����ʽʵ����֮����
extern template class TCVector< unsigned >;

#endif // !TCVECTOR_H
