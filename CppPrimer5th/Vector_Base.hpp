#ifndef VECTOR_BASE_HPP
#define VECTOR_BASE_HPP

#include <cstddef>
#include <stdexcept>
#include <utility>	// pair


// Vector�����򻯰�Ļ��࣬��������ı߽�ָ��
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

	// �ƶ�����
	Vector_Container( Vector_Container &&right )	noexcept
		: m_First( right.m_First ), m_Current( right.m_Current ), m_End( right.m_End )
	{
		right.m_First = right.m_Current = right.m_End = nullptr;
	}

	const_pointer First() const { return m_First; }
	const_pointer Last() const { return m_Current; }

protected:
	pointer m_First;		// ��һ��Ԫ�ص�ָ��
	pointer m_Current;		// ���һ����ЧԪ�ص���һ��λ��
	pointer m_End;			// �ڴ�ĩβ����һ��λ��
};


// Vector�����ĵ��������Բ����ƣ������滻��TCVector�ڲ�
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
	// ǰ��������Ҫ����Ҫ�ġ�Խ���顿
	Vector_Iterator& operator++()
	{
		if( m_Current >= m_Container->Last() )
			throw std::exception( "TCVector iterator not incrementable" );
		++m_Current;
		return *this;
	}

	// ǰ���Լ�
	Vector_Iterator& operator--()
	{
		if( m_Current <= m_Container->First() )
			throw std::exception( "TCVector iterator not decrementable" );
		--m_Current;
		return *this;
	}

	// ����������ע����ǰ�ð汾���βε����𡿣������Ѿ������ǰ�ð汾����������
	Vector_Iterator operator++( int )
	{
		Vector_Iterator tmp = *this;
		++*this;
		return tmp;
	}

	// �����Լ���ע�ⷵ����������ǰ�ġ�������
	Vector_Iterator operator--( int )
	{
		Vector_Iterator tmp = *this;
		--*this;
		return tmp;
	}


	// ���������������const�ģ��������á���ǿ���ǳ�Ա��Ҳ��ǿ�������ʲô����
	reference operator*() const
	{
		if( m_Current < m_Container->First() || m_Current >= m_Container->Last() )
			throw std::exception( "TCVector iterator not dereferencable" );
		return *m_Current;
	}

	// �������������ͷ�������ǡ���Ա�������뷵�ء���ָ�롿���߶����ˡ�->���Ķ���
	// ��ͷ����������ʳ�Ա���Ļ������岻�ܱ�
	pointer operator->() const
	{
		// ͨ����operator*���ʹ�ã�*������Ҫ�ļ�����ʵ�Ļ�ȡ����ֵ�Ĳ�����
		// �����Ե�д���ǣ�&( **this )
		return &this->operator*();
	}

private:
	pointer			m_Current;		// ��������ǰָ���λ��
	const MyVec		*m_Container;
};

#endif // !VECTOR_BASE_HPP