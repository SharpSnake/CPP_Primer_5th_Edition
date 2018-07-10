#ifndef FUNCTIONS_HPP
#define FUNCTIONS_HPP

#include <cstdlib>
#include <ctime>
#include <iostream>
#include <boost/type_index.hpp>	// this library of boost is head-only


// 打印一个变量的确切类型名称，std的typeid( var ).name()不完整，所以借助boost
#define PrintTypeName( var )\
{\
	cout << "Type of \"" << #var << "\"\t:\t";\
	cout << boost::typeindex::type_id_with_cvr< decltype( var ) >().pretty_name() << endl;\
}

//template< typename T >
//constexpr void constexpr_checker( T&& t ) {}
//
//#define IS_CONSTEXPR( obj ) ( noexcept( constexpr_checker( obj ) ) )


// 计算整数的阶乘
inline constexpr int Factorial( int n )	MCPP11
{
	/*int f = 1;
	for( int i = n; i > 1; --i )
	f *= i;
	return f;*/	// C++14
	return n <= 1 ? 1 : ( n * Factorial( n - 1 ) );
}

// 生成某个范围内的随机整数，不做b>a的检测
inline int Randi( const int &a, const int &b )
{
	return a + std::rand() % ( b - a + 1 /* this is the range of [ a, b ]*/ );
}



/////////////////////////////////////////////////////////////////////////////////
inline void FunctionsInit()
{
	// 初始化随机数发生器
	std::srand( std::time( nullptr ) );
}

#endif // !FUNCTIONS_HPP