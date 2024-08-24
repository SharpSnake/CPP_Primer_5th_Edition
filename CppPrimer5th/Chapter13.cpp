#include <utility>	// std::move
#include <iterator>	// make_move_iterator
#include <algorithm>	// std::copy

#include "TestBase.h"
#include "TestHelper.h"
#include "TCVector.h"

using namespace std;
using std::swap;

// 生成随机整数的序列
TCVector< int > MakeRandiVec( size_t, int imin = 1, int imax = 100 );

void Test_CopyBase()
{
	StartOneTest();

	CONSOLE_COLOR_AUTO_RESET;
	SetConsoleTextColor( CmdColor_Yellow );

	cout << "1、default修饰仅用于拷贝控制函数，可以在函数声明处，\r\n"
		"也可仅仅在外部定义处修饰（这样的话函数将不是inline）" << endl2;

	cout << "2、delete必须在函数声明处，且它可以修饰任何成员函数" << endl2;

	cout << "3、如果将析构设为delete，那么这个类仍可以new，"
		"但无法释放（error when delete ptr）" << endl2;

	cout << "4、成员的默认拷贝控制（构造、拷贝、赋值、析构）如果是delete的，\r\n"
		"那么类对应的函数也是delete的；" << endl2;

	cout << "5、类含有引用、const成员，则类的合成构造和合成=是delete的；" << endl2;
}

void Test_CopyAssign()
{
	StartOneTest( "拷贝构造：第一个参数为自身类型引用" );
	// 后面可以有其他参数，但必须都有默认实参
	TCVector< int > vec1 = { 1, 2, 3 };
	TCVector< int > copy_vec1 = vec1;	// or copy_vec1( vec1 )
	cout << copy_vec1 << endl;

	StartOneTest( "何时会使用拷贝构造？" );
	// 列表初始化数组时，编译器可能会优化代码，【跳过】拷贝构造，直接使用另一个构造；
	// 但我们也不能删除拷贝构造，无论编译器是否优化（我们不能期望），拷贝构造必须存在；
	Coordinate coords[ 3 ]{ Coordinate( 3, 5 ), Coordinate( 1, 9 ) };
	cout << coords << endl;

	// 其他情况，比如非引用传递参数、函数返回值等等，也会用到拷贝构造;

	StartOneTest( "（合成）拷贝赋值运算符一般要返回自身的引用" );
	// 比如在多重赋值、赋值后当做条件判断等场景中会用到；
	Coordinate coord1, coord2, coord3;
	coord1 = coord2 = Coordinate( 1.5, 3 );
	if( coord3 = coord1 )
		cout << coord3 << endl;
	
	// 拷贝赋值要保证自赋值可以正常工作
	TCVector< string > vec2{ "abc", "def", "ghi" };
	vec2 = vec2;
	cout << "自赋值：" << endl << vec2 << endl;
}

void Test_RvalueMove()
{
	StartOneTest( "右值引用(Rvalue References)" );
	// 右引只能绑定到右值上，它指向的对象承诺即将消失，可以夺取它的资源
	string s1 = "abc", s2 = "123";
	string &&rrs1 = s1 + "def";	MCPP11
	// string &&rs2 = rs1;	// ERROR! 右引【不能】直接绑定到一个变量，即使它是一个右引类型
	string s3( std::move( rrs1 ) ), 
		s4( std::move( s2 ) );	MCPP11	// 使用标准库方法获得一个右引，s2即将变空
	cout << s2 << ", " << s3 << ", " << s4 << endl;

	StartOneTest( "移动不能抛异常" );
	// 必须是不抛异常的，用C++11新关键词noexcept修饰
	// 【noexcept的必要性】：常规的拷贝进行到一半如果有异常，大不了中途回滚，左右值都不受影响，
	// 但如果移动进行到一半出了问题，左右值都已经发生改变，回滚已然不现实；而且移动操作一般
	// 不会涉及到新资源的分配，只是资源所有权的交换，几乎不可能出问题；
	TCVector< int > ivec1 = { 1, 2, 3, 4, 5 };
	TCVector< int > ivec2( std::move( ivec1 ) );
	cout << ivec1 << endl2 << ivec2 << endl;


	StartOneTest( "合成移动操作" );
	// 当类【没有自定义】拷贝操作，且成员都可以移动时，才会合成移动操作，否则移动是delete的
	// 有const或引用成员，移动也是delete的
	struct XX	// XX有默认的移动构造
	{
		int ID;
		string Name;
	};
	struct YY	// YY的移动是delete的，移动构造的尝试会【转为】拷贝构造，赋值亦是
	{
		YY( int id, string lb ) : ID( id ), Label( lb ) {}
		YY( const YY &yy ) : ID( yy.ID ), Label( yy.Label ) {}
		int ID;
		string Label;
	};
	XX x1{ 1, "lilei" }, x2( std::move( x1 ) ), x3{ 5, "hanmeimei" }, x4;
	x4 = std::move( x3 );	// 这个采用合成的移动赋值运算符
	cout << x1.ID << ", " << x1.Name << endl << x2.ID << ", " << x2.Name << endl2;
	cout << x3.ID << ", " << x3.Name << endl << x4.ID << ", " << x4.Name << endl2;

	YY y1 = { 2, "shanghai" }, y2( std::move( y1 ) );
	cout << y1.ID << ", " << y1.Label << endl << y2.ID << ", " << y2.Label << endl2;


	cout << "一旦定义了移动操作，合成拷贝就是delete的" << endl;
	struct ZZ
	{
		ZZ( int id, string nm ) : ID( id ), Name( nm ) {}
		ZZ( ZZ &&zz ) : ID( zz.ID ), Name( std::move( zz.Name ) ) {}
		int ID;
		string Name;
	};
	ZZ z1{ 1, "tom" };
	//ZZ z2( z1 );	// ERROR! 拷贝构造是删除的


	StartOneTest( "【右值移动，左值拷贝】原则" );
	// 函数返回是右值，vec1自动使用移动构造，在C++11前是不可能的，会多一步不必要的拷贝
	TCVector< int > intvec1 = MakeRandiVec( 3 );
	TCVector< int > intvec2 = intvec1;
	cout << intvec1 << endl2 << intvec2 << endl;


	StartOneTest( "移动迭代器" );
	// C++11提供了新方法，生成移动迭代器，相关的方法会将元素移动；
	TCVector< string > svec{ "abc", "def" };
	string sary[ 2 ]{};
	std::copy( make_move_iterator( svec.begin() ),	MCPP11
		make_move_iterator( svec.end() ), sary );
	cout << svec << endl;	// vec中的每个元素被ary偷走了，但vec容器还在
	cout << "sary:" << endl << sary[ 0 ] << endl << sary[ 1 ] << endl;


	StartOneTest( "右引形参的函数重载" );
	// 右引形参版本的重载，右值（比如函数返回、表达式计算值、字面值）与其有更好的匹配
	TCVector< string > svec2;
	svec2.PushBack( sary[ 0 ] );			// 使用常规重载
	svec2.PushBack( sary[ 1 ] + "123" );	// 使用右引重载
	svec2.PushBack( "mmnnbb" );				// 使用右引重载
	cout << svec2 << endl;

	StartOneTest( "引用限定符" );
	TCVector< int > intvec3 = { 5, 8, 1, 4, 2 };
	intvec3 = intvec3.Sorted();	// 使用常规版本
	TCVector< int > intvec4 = MakeRandiVec( 10 ).Sorted();	// 使用右值版本
	cout << intvec3 << endl2 << intvec4 << endl;
}

void Test_CopyOther()
{
	StartOneTest( "交换操作" );
	// 一般交换会有几步不必要的赋值和拷贝，通过自定义和移动运算可以提速；
	// 如果一个类实现了移动构造和移动赋值，那么理论上std的swap和自己重载的swap是一样快的
	TCVector< string > vec1{ "abc", "def", "ghi" };
	TCVector< string > vec2{ "123", "789" };
	swap( vec1, vec2 );
	cout << vec1 << endl2 << vec2 << endl;

	// 交换操作还有一个有趣的应用，是利用【拷贝交换技术】实现operator =


	StartOneTest( "TCVector的其他接口" );
	TCVector< Coordinate > vCoord{ { 1.0, 3 }, { 2.5, 3.3 } };
	vCoord.Reserve( 20 );
	vCoord.Resize( 6 );
	vCoord.PopBack( 2 );
	cout << vCoord << endl;


	StartOneTest( "allocator的真相及原理" );
	// 一般new将内存分配和类的构造初始化合并在一起，即使某些对象永远用不着也会构造，造成了浪费；
	// 而delete也将类的析构和内存的释放合并在一起，即使那块内存你还想继续用；
	Coordinate *dynCoords = new Coordinate[ 3 ];
	delete[] dynCoords;

	// allocator将上面的4个步骤分开来，更加灵活，也更加高效：
	cout << "1、allocate continuous memory block" << endl;
	void *memCoords = ::operator new( sizeof( Coordinate ) * 3 );

	cout << "2、construct an object if needed:\t";
	Coordinate *pCoords = static_cast< Coordinate* >( memCoords );
	::new( ( void* )( pCoords + 0 ) ) Coordinate( 1.5, 2.2 );	// 使用拷贝构造或移动构造
	cout << pCoords[ 0 ] << endl;

	cout << "3、destroy an object if no more used" << endl;
	( pCoords + 0 )->~Coordinate();

	cout << "4、deallocate the whole memory block" << endl;
	::operator delete( memCoords );

	cout << "uninitialized_copy相当于在一块未经开垦的内存上重复第2步；" << endl;
}

void Chapter13Init()
{
	auto ch13 = ChapterBase::AddChapter( 13, "第十三章 拷贝控制" );
	ch13->AddSection( 1, "重要概念", &Test_CopyBase );
	ch13->AddSection( 2, "拷贝和赋值", &Test_CopyAssign );
	ch13->AddSection( 3, "右值引用与移动", &Test_RvalueMove );
	ch13->AddSection( 4, "其他重要操作", &Test_CopyOther );
}
static int _Init = ( Chapter13Init(), 0 );

TCVector< int > MakeRandiVec( size_t count, int imin, int imax )
{
	TCVector< int > temVec;
	temVec.Reserve( count );
	for( size_t i = 0; i < count; ++i )
		temVec.PushBack( Randi( imin, imax ) );

	return temVec;
}