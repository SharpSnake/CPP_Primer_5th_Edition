#include <Windows.h>
#include <locale>
#include <vector>

#include "Lambda_Traits.hpp"
#include "TestBase.h"

using namespace std;


template < typename F >
struct lambda_traits	{};

template < typename F, typename R, typename... Args >
struct lambda_traits< R( F::* )( Args... ) const > 
{
	using pointer = typename std::add_pointer< R ( Args... ) >::type;

	static pointer cify(F&& f) 
	{
		static F fn = std::forward<F>(f);
		return [](Args... args) {
			return fn(std::forward<Args>(args)...);
			};
	}
};

template < typename F >
inline auto cify( F f )	{
	return lambda_traits< decltype( &F::operator() ) >::cify( std::forward< F >( f ) );
}

void TestLambdaTraits()
{
	using IFun = int ( * )( int );

	auto f1 = []( int i ){ return i + i; };
	auto f2 = [&]( int i ){ return i + i; };

	constexpr bool iscon = std::is_convertible_v< decltype( f2 ), int( * )( int )  >;
	constexpr bool iscap = Meteor::Lambda::Is_Captured< decltype( f2 ) >;
	cout << boolalpha << iscap << endl;

	IFun p1 = ( IFun )( f1 );
	//IFun p2 = ( IFun )( f2 );
	cout << p1( 5 ) << endl;
}


vector< wstring > GetSystemLocales()
{
	using pointer = typename std::add_pointer< int* >::type;
	vector< wstring > loc;
	auto enumProc = [&]( LPWSTR pStr, DWORD dwFlags, LPARAM lparam ) -> BOOL	{
		//reinterpret_cast< vector< wstring >* >( lparam )->emplace_back( pStr );
		loc.emplace_back( pStr );
		return TRUE;
		};

	//vector< wstring > loc;
	//EnumSystemLocalesEx( enumProc, LOCALE_ALL, ( LPARAM )&loc, NULL );
	EnumSystemLocalesEx( cify( enumProc ), LOCALE_ALL, NULL, NULL );

	return loc;
}

void TestLocales()
{
	//setlocale( LC_ALL, "en_US.UTF-8" );
	locale::global( locale( "en_US.UTF-8" ) );
	auto s8 = u8"从arg开始才";
	cout << s8 << endl;

	auto allloc = GetSystemLocales();
	for( auto &loc : allloc )
		wcout << loc << endl;
	wcout << "Total " << allloc.size() << " locals found." << endl;
}

void mainX()
{
	TestLocales();
	//ChapterBase::RunMainLoop( 1, 1 );
	system( "pause" );
}