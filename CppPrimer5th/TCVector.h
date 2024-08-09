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
	typedef T*							iterator;				// 迭代器类型
	typedef const T*					const_iterator;
	typedef Vector_Iterator< Vector_Container< T > >		_iterator;
	typedef const Vector_Iterator< Vector_Container< T > >	_const_iterator;


	// 默认构造，容量为空，直到初次PushBack时分配8个长度
	TCVector() : Vector_Container() {}

	// 用于列表初始化
	TCVector( std::initializer_list< T > list )
		: TCVector()
	{
		_Alloc_Copy( list.begin(), list.end() );
	}

	/**
	*  拷贝构造，与合成默认构造不同，当没有自定义时，编译器【总会合成】拷贝构造
	*
	* @param  allocByCapacity  默认按right的size分配内存，否则按Capacity来分配
	*/ 
	TCVector( const TCVector &right, bool allocByCapacity = false )
		: TCVector()
	{
		_Alloc_Copy( right.m_First, right.m_Current, 
			allocByCapacity ? right.m_End : nullptr );
	}

	// 移动构造，窃取右值的资源，必须保证移后源对象【析构安全】；
	// 【noexcept】在声明和定义处【都要有】，【紧跟】参数列表；
	TCVector( TCVector &&right ) noexcept	MCPP11
		: Vector_Container( std::move( right ) ) {}

	/**
	*  拷贝赋值运算符，一般 = 重构造 + 析构，遵守两个原则：
	*  1、保证【自赋值】可以正常工作；
	*  2、【先分配】新资源并拷贝右值，然后【再销毁】旧资源，确保自赋值，且分配失败可以【回滚】；
	*/
	TCVector& operator =( const TCVector &right )
	{
		_Alloc_Copy( right.m_First, right.m_Current );
		return *this;
	}

	/*
	// 利用【拷贝交换技术】实现operator =，注意参数不是引用，所以实参是一个拷贝的临时变量，
	// 可以不用担心自赋值，赋值完后临时变量自动释放
	TCVector& operator =( TCVector right )
	{
		swap( *this, right );
		return *this;
	}
	*/

	// 移动赋值运算符，自移动是没有必要的
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

	// 用于初始化列表赋值
	TCVector& operator =( std::initializer_list< T > list )
	{
		_Alloc_Copy( list.begin(), list.end() );
		return *this;
	}

	// 析构：在函数体执行完毕后，按类中声明顺序的【逆序】，逐个销毁成员变量
	virtual ~TCVector() { _Destroy(); }


public:
	size_type		Size()		const	{ return m_Current - m_First; }
	size_type		Capacity()	const	{ return m_End - m_First; }

	// 若需实现range for，必须严格命名为"begin"和"end"
	iterator		begin()				{ return m_First; }
	const_iterator	begin()		const	{ return m_First; }
	iterator		end()				{ return m_Current; }
	const_iterator	end()		const	{ return m_Current; }

	// 引用限定符&：只能被左值调用，返回自身副本的排序
	// 如果函数是const的，限定符必须【跟在const后面】
	TCVector Sorted() const &	MCPP11
	{
		TCVector retVal( *this );
		std::sort( retVal.begin(), retVal.end() );
		return retVal;
	}

	// 引用限定符&&：只能被右值调用，直接对自身排序并返回
	// 如果一个成员函数有限定符，那么所有同名、同参数的【重载】都必须有引用限定符
	TCVector Sorted() &&	MCPP11
	{
		std::sort( this->begin(), this->end() );
		return *this;
	}

	// 向末尾添加一个元素，常规版本
	void PushBack( const_reference val )
	{
		if( m_Current == m_End )
			_Alloc_Grow();
		m_Alloc.construct( m_Current++, val );	// 这里将使用T的【拷贝构造】
	}

	// 右引重载版本，添加一个右引对象，将使用T的移动构造
	void PushBack( T &&rrval )	MCPP11
	{
		if( m_Current == m_End )
			_Alloc_Grow();
		m_Alloc.construct( m_Current++, std::move( rrval ) );
	}

	// 向末尾添加一个元素，手动构造
	template< typename... Args >
	void EmplaceBack( Args&&... args )
	{
		if( m_Current == m_End )
			_Alloc_Grow();
		m_Alloc.construct( m_Current++, std::forward< Args >( args )... );
	}

	// 释放末尾的几个元素，默认释放最后一个
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

	// 预分配指定长度的内存，已存在的元素会保留
	void Reserve( size_type newcapacity )
	{
		if( newcapacity > Capacity() )
			_Alloc_Grow( newcapacity );
	}

	// 重整元素的长度，用指定元素填充末尾可能追加的部分，没有指定就使用默认构造填充
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

	// 收缩资源空间
	void ShrinkToFit();

	// 仅清除每个元素，容器资源并不释放
	void Clear()
	{
		if( Size() )
		{
			for( auto i = m_First; i != m_Current; ++i )
				m_Alloc.destroy( i );
			m_Current = m_First;
		}
	}

	// 必须加【friend】修饰，才算是std::swap的【重载】，否则仅仅是一个成员函数
	// std::swap利用类的【移动】构造和移动赋值提速的，如果类没有实现移动，将没有性能提升；
	friend void swap( TCVector &left, TCVector &right )
	{
		using std::swap;	// 下面的调用都不要加std的作用域限定符
		swap( left.m_First, right.m_First );
		swap( left.m_Current, right.m_Current );
		swap( left.m_End, right.m_End );
	}
	
	// 下标运算符必须是成员函数
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
	// 拷贝分配，如果pend不为空，则按照first至pend的长度分配
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

	// 自增长分配，除非指定要分配的内存长度
	void _Alloc_Grow( size_type customcap = 0 )
	{
		// 初次分配8个长度
		size_type newcap = customcap ? customcap : ( Capacity() ? Capacity() * 2 : 8 );
		pointer newfirst = m_Alloc.allocate( newcap );
		pointer newp = newfirst;

		// 这里也可以使用移动迭代器实现快速拷贝，效果是一样的：
		//m_Current = std::uninitialized_copy( make_move_iterator( begin() ), 
		//	make_move_iterator( end() ), newfirst );
		for( auto i = m_First; i != m_Current; ++i )
			m_Alloc.construct( newp++, std::move( *i ) ); MCPP11	// 使用移动构造可以提速

		_Destroy();
		m_First = newfirst;
		m_Current = newp;
		m_End = m_First + newcap;
	}

	// 析构每个元素，并释放容器资源
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
	AllocType m_Alloc;		// 内存分配器
};

// 类外定义要有模参列表
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

// C++11类模板显式实例化之声明
extern template class TCVector< unsigned >;

#endif // !TCVECTOR_H
