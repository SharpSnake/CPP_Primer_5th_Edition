#ifndef VECTOR_BASE_HPP
#define VECTOR_BASE_HPP

#include <cstddef>
#include <stdexcept>
#include <utility>	// pair


// Vector容器简化版的基类，存放容器的边界指针
template< typename T >
class Vector_Container
{
public:
	typedef T*							pointer;
	typedef const T*					const_pointer;
	typedef T&							reference;
	typedef const T&					const_reference;
	typedef std::size_t					size_type;

	Vector_Container()
		: m_First( nullptr ), m_Current( nullptr ), m_End( nullptr ) {}

	// 移动构造
	Vector_Container( Vector_Container &&right )	noexcept
		: m_First( right.m_First ), m_Current( right.m_Current ), m_End( right.m_End )
	{
		right.m_First = right.m_Current = right.m_End = nullptr;
	}

	const_pointer First() const { return m_First; }
	const_pointer Last() const { return m_Current; }

protected:
	pointer m_First;		// 第一个元素的指针
	pointer m_Current;		// 最后一个有效元素的下一个位置
	pointer m_End;			// 内存末尾的下一个位置
};


// Vector容器的迭代器，仍不完善，后续替换到TCVector内部
template< typename MyVec >
class Vector_Iterator
{
public:
	typedef typename MyVec::pointer				pointer;
	typedef typename MyVec::const_pointer		const_pointer;
	typedef typename MyVec::reference			reference;
	typedef typename MyVec::const_reference		const_reference;
	typedef typename MyVec::size_type			size_type;
	typedef ptrdiff_t							difference_type;


	Vector_Iterator( pointer ptr, const MyVec *cont )
		: m_Current( ptr ), m_Container( cont ) {};

public:
	// 前置自增，要做必要的【越界检查】
	Vector_Iterator& operator++()
	{
		if( m_Current >= m_Container->Last() )
			throw std::exception( "TCVector iterator not incrementable" );
		++m_Current;
		return *this;
	}

	// 前置自减
	Vector_Iterator& operator--()
	{
		if( m_Current <= m_Container->First() )
			throw std::exception( "TCVector iterator not decrementable" );
		--m_Current;
		return *this;
	}

	// 后置自增，注意与前置版本【形参的区别】，利用已经定义的前置版本做自增工作
	Vector_Iterator operator++( int )
	{
		Vector_Iterator tmp = *this;
		++*this;
		return tmp;
	}

	// 后置自减，注意返回类型是增前的【副本】
	Vector_Iterator operator--( int )
	{
		Vector_Iterator tmp = *this;
		--*this;
		return tmp;
	}


	// 访问运算符（都是const的）：解引用。不强求是成员，也不强求具体做什么事情
	reference operator*() const
	{
		if( m_Current < m_Container->First() || m_Current >= m_Container->Last() )
			throw std::exception( "TCVector iterator not dereferencable" );
		return *m_Current;
	}

	// 访问运算符：箭头。必须是【成员】，必须返回【类指针】或者定义了【->】的对象，
	// 箭头运算符【访问成员】的基本含义不能变
	pointer operator->() const
	{
		// 通常与operator*配合使用，*会做必要的检测和真实的获取数据值的操作，
		// 更简略的写法是：&( **this )
		return &this->operator*();
	}

private:
	pointer			m_Current;		// 迭代器当前指向的位置
	const MyVec		*m_Container;
};

#endif // !VECTOR_BASE_HPP