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


// ���������۵�����������ֵ���Բ���
template< typename T >
inline void CollapseKeepReference( T &&var )
{
	std::cout << boost::typeindex::type_id_with_cvr< T >().pretty_name() << " -> " 
		<< boost::typeindex::type_id_with_cvr< decltype( var ) >().pretty_name() << endl;
}

// ��ӡ������Ϣ�Ĳ�ͬ����
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


// ͨ��ģ�壬���Խ�������ά�ȵ��������
template< typename T, unsigned N >
inline void PrintArray1D( T ( &ary )[ N ] )
{
	for( auto &i : ary )
		std::cout << i << " ";
	std::cout << std::endl;
}

// ��ӡ����Ϊ5���ַ������ػ��汾
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

// ��c��̬�ڴ�ĺ����ͺ�������
inline void FreeCMemory( void *cmem )
{
	std::cout << "\tFunction free" << std::endl;
	free( cmem );
}

// �������һ��vector��ģ�ε����ͳ�Ա��Ҫ��עtypename
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


// ���������Ľ׳�
inline constexpr int Factorial( int n )	MCPP11
{
	/*int f = 1;
	for( int i = n; i > 1; --i )
	f *= i;
	return f;*/	// C++14
	return n <= 1 ? 1 : ( n * Factorial( n - 1 ) );
}

// �����1��ָ���������ۻ���
inline constexpr int Cumsum( int n )
{
	return n <= 1 ? 1 : ( n + Cumsum( n - 1 ) );
}

// ģ����׳�
template< unsigned N >
struct TFactorial
{
	enum{ value = N * TFactorial< N - 1 >::value };
	//static constexpr unsigned value = N * TFactorial< N - 1 >::value;	//this works too
};

// �ػ�N=1�İ汾��Ϊ�������ֹ����
template<>	struct TFactorial< 1 >	{	enum{ value = 1 };	};


// ��ָ��ָ�������������һ�������õ����
template< int *ptri, int ( *Fun )( int ) >
inline int Fevali()
{
	return Fun( *ptri );
}

// ִ�ж�Ԫ��������
template< typename T, typename F = std::plus< T > >
inline T BinaryFeval( const T &left, const T &right, F f = F() )
{
	return f( left, right );
}

// ������������ִ�ж�Ԫ�������μ�������ǵ�����ʵ��ָ���ֵ
// ע��*left���Ʋ�����Ϊ���ã�decltype( *left + 0 )��remove_reference����ȥ������
template< typename I, typename F >
inline auto IteratorFeval( I left, I right, F f ) -> 
	typename std::remove_reference< decltype( *left ) >::type
{
	return f( *left, *right );
}


// ����ĳ����Χ�ڵ��������������b>a�ļ��
inline int Randi( const int &a, const int &b )
{
	return a + std::rand() % ( b - a + 1 /* this is the range of [ a, b ]*/ );
}

// ����ɱ�ģ������ĸ���sizeof Args��sizeof args��һ����
template< typename... Args >
inline std::size_t ArgsSize( Args... args ) { return sizeof...( Args ); }


// ƴ��ģ��ݹ��������һ��
template< typename T >
inline void _StringCombine( std::ostringstream &ostr, char separator, const T &t )	{ ostr << t; }

// ͨ���ݹ����ʽ��ÿ�ν��������еĵ�һ����Ϊ�´εݹ��t��һֱ����������Ϊֹ
template< typename T, typename... Args >
inline void _StringCombine( std::ostringstream &ostr, char separator, const T &t, Args... args )
{
	ostr << t << separator;
	_StringCombine( ostr, separator, std::forward< Args >( args )... );	// ע�����չ�ĸ�ʽ
}

// �ַ���ƴ�ӣ�separatorΪ�ָ���
template< typename... Args >
inline std::string StringCombine( char separator, Args... args )
{
	std::ostringstream ostr;
	_StringCombine( ostr, separator, std::forward< Args >( args )... );	// ע�����չ�ĸ�ʽ
	return ostr.str();
}

// �ػ��汾��û���κ�ƴ�����ݣ�����һ�����ַ���
template<>
inline std::string StringCombine( char separator )	{ return ""; }


// ��ӡһ����Ϣ������Ϊtitle
template< typename T >
inline void PrintMsg( const std::string &title, const T &msg )	{
	std::cout << title << ":\t" << msg << std::endl;
}

// ��ӡN��������Ϣ����������չ�ķ�ʽ�Ƕ�ÿ����������һ����ͨ�ĺ���
template< typename... Args >
inline void ErrorMsg( const Args&... args )
{
	// ���������������������չ����������������һ����ʱ��ȫ0����
	//{ ( PrintMsg( arg1 ), 0 ), ( PrintMsg( arg2 ), 0 ), ( PrintMsg( arg3 ), 0 ),  etc... }
	int ary[] = { ( PrintMsg( "ERROR", args ), 0 )... };
}

/////////////////////////////////////////////////////////////////////////////////
inline void FunctionsInit()
{
	// ��ʼ�������������
	std::srand( ( unsigned )std::time( nullptr ) );
}

// C++11����ģ����ʽʵ����֮����
extern template void PrintArray1D( double ( & )[ 5 ] );

#endif // !FUNCTIONS_HPP