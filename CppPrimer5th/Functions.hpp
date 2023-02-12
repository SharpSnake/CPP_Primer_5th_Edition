#ifndef FUNCTIONS_HPP
#define FUNCTIONS_HPP

#include <stdlib.h>     // malloc, free
#include <cstdlib>
#include <ctime>
#include <string>
#include <sstream>
#include <iostream>
#include <utility>
#include <functional>
#include <type_traits>
#include <boost/type_index.hpp>	// this library of boost is head-only


// 打印一个变量的确切类型名称，std的typeid( var ).name()不完整，所以借助boost
#define PrintTypeName( var )\
{\
	cout << "Type of \"" << #var << "\"\t:\t";\
	cout << boost::typeindex::type_id_with_cvr< decltype( var ) >().pretty_name() << endl;\
}

// 安全彻底地释放一个动态对象
#define SafeDelete( p )\
{\
	if( p )\
	{\
		delete p;\
		p = nullptr;\
	}\
}


// 测试引用折叠，保持左右值属性不变
template< typename T >
inline void CollapseKeepReference( T &&var )
{
	std::cout << boost::typeindex::type_id_with_cvr< T >().pretty_name() << " -> " 
		<< boost::typeindex::type_id_with_cvr< decltype( var ) >().pretty_name() << endl;
}

// 打印调试信息的不同重载
inline void DebugInfo( const std::string &info ) {
	std::cout << "DebugInfo( const string& ):\t" << info << endl;
}

template< typename T >
inline void DebugInfo( const T &info ) {
	std::cout << "DebugInfo( const T& ):\t" << info << endl << "\t";
	PrintTypeName( info );
}

template< typename T >
inline void DebugInfoT( const T &info ) {
	std::cout << "DebugInfoT( const T& ):\t" << info << endl << "\t";
	PrintTypeName( info );
}

template< typename T >
inline void DebugInfoT( T *info ) {
	std::cout << "DebugInfoT( T* ):\t" << info << endl << "\t";
	PrintTypeName( info );
}


// 通过模板，可以接受任意维度的数组参数
template< typename T, unsigned N >
inline void PrintArray1D( T ( &ary )[ N ] )
{
	for( auto &i : ary )
		std::cout << i << " ";
	std::cout << std::endl;
}

// 打印长度为5的字符串的特化版本
template<>
inline void PrintArray1D( const char ( &ary )[ 5 ] )	{ std::cout << ary << std::endl; }

template< typename T, unsigned M, unsigned N >
inline void PrintArray2D( T ( &ary )[ M ][ N ] )
{
	for( auto &i : ary )
	{
		for( auto &j : i )
			std::cout << j << " ";
		std::cout << std::endl;
	}
}

// 释c动态内存的函数和函数对象
inline void FreeCMemory( void *cmem )
{
	std::cout << "\tFunction free" << std::endl;
	free( cmem );
}

// 彻底清空一个vector，模参的类型成员需要标注typename
template< typename VecType >
inline typename VecType::size_type FreeVector( VecType &vec )
{
	typename VecType::size_type orisize = vec.end() - vec.begin();

	if( orisize )
	{
		using std::swap;
		swap( vec, VecType() );
		return orisize;
	}
	else
		return typename VecType::size_type();
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


// 计算整数的阶乘
inline constexpr int Factorial( int n )	MCPP11
{
	/*int f = 1;
	for( int i = n; i > 1; --i )
	f *= i;
	return f;*/	// C++14
	return n <= 1 ? 1 : ( n * Factorial( n - 1 ) );
}

// 计算从1到指定整数的累积和
inline constexpr int Cumsum( int n )
{
	return n <= 1 ? 1 : ( n + Cumsum( n - 1 ) );
}

// 模板求阶乘
template< unsigned N >
struct TFactorial
{
	enum{ value = N * TFactorial< N - 1 >::value };
	//static constexpr unsigned value = N * TFactorial< N - 1 >::value;	//this works too
};

// 特化N=1的版本，为计算的终止条件
template<>	struct TFactorial< 1 >	{	enum{ value = 1 };	};


// 对指针指向的整数，调用一个函数得到结果
template< int *ptri, int ( *Fun )( int ) >
inline int Fevali()
{
	return Fun( *ptri );
}

// 执行二元函数对象
template< typename T, typename F = std::plus< T > >
inline T BinaryFeval( const T &left, const T &right, F f = F() )
{
	return f( left, right );
}

// 对两个迭代器执行二元函数，参加运算的是迭代器实际指向的值
// 注意*left的推测类型为引用，decltype( *left + 0 )或remove_reference可以去掉引用
template< typename I, typename F >
inline auto IteratorFeval( I left, I right, F f ) -> 
	typename std::remove_reference< decltype( *left ) >::type
{
	return f( *left, *right );
}


// 生成某个范围内的随机整数，不做b>a的检测
inline int Randi( const int &a, const int &b )
{
	return a + std::rand() % ( b - a + 1 /* this is the range of [ a, b ]*/ );
}

// 计算可变模板参数的个数sizeof Args和sizeof args是一样的
template< typename... Args >
inline std::size_t ArgsSize( Args... args ) { return sizeof...( Args ); }


// 拼接模板递归链的最后一步
template< typename T >
inline void _StringCombine( std::ostringstream &ostr, char separator, const T &t )	{ ostr << t; }

// 通过递归的形式，每次将参数包中的第一个作为下次递归的t，一直到参数包空为止
template< typename T, typename... Args >
inline void _StringCombine( std::ostringstream &ostr, char separator, const T &t, Args... args )
{
	ostr << t << separator;
	_StringCombine( ostr, separator, std::forward< Args >( args )... );	// 注意包扩展的格式
}

// 字符串拼接，separator为分隔符
template< typename... Args >
inline std::string StringCombine( char separator, Args... args )
{
	std::ostringstream ostr;
	_StringCombine( ostr, separator, std::forward< Args >( args )... );	// 注意包扩展的格式
	return ostr.str();
}

// 特化版本，没有任何拼接内容，返回一个空字符串
template<>
inline std::string StringCombine( char separator )	{ return ""; }


// 打印一条消息，标题为title
template< typename T >
inline void PrintMsg( const std::string &title, const T &msg )	{
	std::cout << title << ":\t" << msg << std::endl;
}

// 打印N条错误消息，参数包扩展的方式是对每个参数调用一个普通的函数
template< typename... Args >
inline void ErrorMsg( const Args&... args )
{
	// 这里必须借助逗号运算符来展开参数包，构建了一个临时的全0数组
	//{ ( PrintMsg( arg1 ), 0 ), ( PrintMsg( arg2 ), 0 ), ( PrintMsg( arg3 ), 0 ),  etc... }
	int ary[] = { ( PrintMsg( "ERROR", args ), 0 )... };
}

/////////////////////////////////////////////////////////////////////////////////
inline void FunctionsInit()
{
	// 初始化随机数发生器
	std::srand( ( unsigned )std::time( nullptr ) );
}

// C++11函数模板显式实例化之声明
extern template void PrintArray1D( double ( & )[ 5 ] );

#endif // !FUNCTIONS_HPP