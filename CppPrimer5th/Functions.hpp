#ifndef FUNCTIONS_HPP
#define FUNCTIONS_HPP

#include <stdlib.h>     // malloc, free
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <boost/type_index.hpp>	// this library of boost is head-only


// ��ӡһ��������ȷ���������ƣ�std��typeid( var ).name()�����������Խ���boost
#define PrintTypeName( var )\
{\
	cout << "Type of \"" << #var << "\"\t:\t";\
	cout << boost::typeindex::type_id_with_cvr< decltype( var ) >().pretty_name() << endl;\
}

// ��ȫ���׵��ͷ�һ����̬����
#define SafeDelete( p )\
{\
	if( p )\
	{\
		delete p;\
		p = nullptr;\
	}\
}


// ��c��̬�ڴ�ĺ����ͺ�������
inline void FreeCMemory( void *cmem )
{
	std::cout << "\tFunction free" << std::endl;
	free( cmem );
}

struct Op_FreeCMemory
{
	void operator()( void *cmem ) const
	{
		std::cout << "\tOp_FreeCMemory free" << std::endl;
		free( cmem );
	}
};

template< typename T >
struct Op_DeleteArray
{
	void operator()( T *ary ) const
	{
		std::cout << "\tOp_DeleteArray delete array" << std::endl;
		delete[] ary;
	}
};



//template< typename T >
//constexpr void constexpr_checker( T&& t ) {}
//
//#define IS_CONSTEXPR( obj ) ( noexcept( constexpr_checker( obj ) ) )


// ���������Ľ׳�
inline constexpr int Factorial( int n )	MCPP11
{
	/*int f = 1;
	for( int i = n; i > 1; --i )
	f *= i;
	return f;*/	// C++14
	return n <= 1 ? 1 : ( n * Factorial( n - 1 ) );
}

// ����ĳ����Χ�ڵ��������������b>a�ļ��
inline int Randi( const int &a, const int &b )
{
	return a + std::rand() % ( b - a + 1 /* this is the range of [ a, b ]*/ );
}



/////////////////////////////////////////////////////////////////////////////////
inline void FunctionsInit()
{
	// ��ʼ�������������
	std::srand( std::time( nullptr ) );
}

#endif // !FUNCTIONS_HPP