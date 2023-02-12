#include "stdafx.h"

#include <thread>
#include <chrono>

#include "TestBase.h"
#include "Chapter06.h"

#pragma warning( disable:4244 )
#pragma warning( disable:4305 )

using namespace std;

bool Worker1Run = true;


void Test_Base()
{
	StartOneTest( "静态局部变量会自己执行值初始化" );
	Worker1Run = false;
	CallLocalStaticObject( true );

	StartOneTest( "const引用传递参数" );
	// 引用传递可以避免不必要的内存拷贝，另外const引用可以接受更加广泛的实参类型
	int refi = 100;
	PassingArgsByRef( refi );
	PassingArgsByRef( 3.6 );	// const引用参数不仅可以接受字面值，甚至可以隐式转换

	StartOneTest( "初始化列表可以直接作为返回值" );	MCPP11
	// 只要和返回类型匹配即可，例如MidPoint函数和Coordinate::operator TCPoint()

	StartOneTest( "默认实参初始化" );
	// 任何非局部变量对象，都可以作为默认实参，且这个对象有变，函数的默认实参也实时改变
	PrintSchoolInfo( "光明小学" );

	SchoolPosition.Offset( 2, -3 );
	PrintSchoolInfo( "市第八中学" );

	StartOneTest( "inline仅仅是建议编译器对函数进行优化展开，\r\n"
		"不是强制，编译器根据实际情况可以忽略inline" );

	StartOneTest( "几个调试用的变量" );
	cout << "文件：\t" << __FILE__ << endl;
	cout << "函数：\t" << __func__ << endl;
	cout << "行数：\t" << __LINE__ << endl;
	cout << "日期：\t" << __DATE__ << " " << __TIME__ << endl;
}

void Test_ArrayParam()
{
	StartOneTest( "一维数组作为形参" );
	// 数组作为形参都会默认按指针处理，即丢失了维度信息；下面三个函数是等价的，
	// 无法知道传入数组的长度，只有另外传递长度信息，或者数组本身结尾有标记，比如字符串的'\0'；
	auto AryFun1 = []( int *ary ){ ; };
	auto AryFun2 = []( int ary[] ){ ; };
	auto AryFun3 = []( int ary[ 10 ] ){ ; };

	// 除非形参是数组的引用，这样数组的维度信息得以保留，但实参数组的长度必须和形参严格一致
	int iary4[ 5 ] = { 11, 22, 33, 44, 55 };
	auto AryFun4 = []( int ( &ary )[ 5 ] ){ for( auto &i : ary ) cout << i << endl; };
	cout << "数组引用作为形参" << endl;
	AryFun4( iary4 );	// 如果长度不是5，则编译失败

	// 利用16章的方法，实参可以是任意长度，模板函数会自动推导
	int iary5[ 3 ] = { 12, 14, 16 };
	float fary6[ 4 ] = { 1.1, 2.2, 3.3, 4.4 };
	cout << "利用模板函数，接受任意类型、长度的数组引用" << endl;
	PrintArray1D( iary5 );
	PrintArray1D( fary6 );


	StartOneTest( "多维数组形参" );	// 以二维数组为例，类比一位数组，都可以办到
	// 经典的传递方法，第一个维度（行数）会丢失，只保留第二个维度（列数），需传递一个行数
	auto Ary2Fun1 = []( int ( *ary )[ 3 ], size_t rows ){ ; };	//与 int ary[][ 3 ] 等价

	// 二维数组的引用（其实可以理解为三维数组的一个波段），可以保留行、列数信息
	int i2ary[ 2 ][ 3 ] = { { 1, 2, 3 }, { 4, 5, 6 } };
	auto Ary2Fun2 = []( int ( &ary )[ 2 ][ 3 ] )
	{
		for( auto &i : ary )
		{
			for( auto &j : i )
				cout << j << " ";
			cout << endl;
		}
	};
	cout << "二维数组引用作为形参" << endl;
	Ary2Fun2( i2ary );

	// 同样，利用模板函数，行列数可以任意
	cout << "利用模板函数，接受任意类型、行列数的二维数组引用" << endl;
	PrintArray2D( i2ary );

	// 以此类推，更高维度的数组也是一样的
}

void Test_ArrayReturn()
{
	// 数组的特性是在运行时会自动按指针处理，且不会自动拷贝，所以只能返回数组的指针或引用
	int matrix[ 3 ][ 3 ] = { { 1, 2, 3 }, 
							{ 4, 5, 6 }, 
							{ 7, 8, 9 } };

	StartOneTest( "经典返回法" );
	ArrayRowi *row0 = GetRow1( matrix, 0 );
	PrintArray1D( *row0 );

	StartOneTest( "别名返回法" );	// 我倾向于这种方法
	ArrayRowi *row1 = GetRow2( matrix, 1 );
	ArrayRowi &row1r = GetRow2r( matrix, 1 );
	PrintArray1D( *row1 );
	PrintArray1D( row1r );

	StartOneTest( "C++11尾置类型返回法" );
	ArrayRowi *row2 = GetRow3( matrix, 2 );
	PrintArray1D( *row2 );
}

void Test_VaryingParam()
{
	StartOneTest( "C++11的initializer_list" );	MCPP11
	// 当形参类型都相同，但数量不定时，可以使用initializer_list，它有以下特点
	// 1、内部仅存储输入对象的副本，不会进行拷贝，因此还算高效；
	// 2、使用方式与vector基本相似，但访问输入对象都是const的，也足够安全；
	// 3、一个list赋值给另外一个，两个list共享同一份数据对象副本，不会拷贝；
	PrintNames( { "John", "Lucy", "Tom" } );
	PrintNames( { "ZhangSan", "LiSi" } );

	StartOneTest( "老式的省略符形参" );
	// 最好不要从第一个参数起就省略，一般由第一个参数提供解码方法，解码参数可以是以下三种：
	// 1、参数个数，此时参数类型是已知且应该是相同的；
	// 2、首元素，参数类型是已知且停止条件是解析值异常（比如针对int得到-1）；
	// 3、参数类型列表描述字符串，例如"%f%d%s"，此时内部按描述信息依次解析；
	PrintNames( 3, "Hubei", "Shandong", "Jiangxi" );
}

void Test_Overloaded()
{
	StartOneTest( "仅返回类型不同，不叫重载，后一个会提示编译错误" );

	StartOneTest( "重载形参中的const" );
	// 如果基础类型不变，则顶层const声明不算重载，底层const（const引用或指针）才算重载
	TCPoint pnt{ 3.3, 4.4 };
	const TCPoint cpnt{ 1.1, 2.2 };
	PrintPoint( pnt );
	PrintPoint( cpnt );

	StartOneTest( "重载与作用域" );
	// 【内层】的函数声明，会【隐藏（屏蔽）】外层所有的同名重载，比如在某个命名空间、函数、
	// 某一个语块内部声明一个函数，那么这个作用域外面的其他重载都会失效；
	{
		// 此时，无论外面有多少个重载，这里只认const TCPoint &这一个重载；
		// VC++没有遵守这条原则，gcc经过测试是严格遵守的；
		// 所以除非刻意，函数声明最好放在一起统一管理，不要单独在某一个【小作用域】内声明；
		void PrintPoint( const TCPoint & );
		PrintPoint( pnt );
	}

	StartOneTest( "重载函数的匹配" );
	// 基本原则：类型越接近，匹配度越好；当不同的实参分别与不同的候选函数匹配更好时，
	// 就会出现二义性问题，编译器无法决定调用哪个重载，导致编译失败，
	// 遇到这样的问题，一般需要进行强制类型转换，但这通常是拙劣的设计造成的；
	cout << "最佳匹配" << endl;
	Sum( 1, 2 );
	Sum( 0.2F, 0.3F );
	cout << "Sum( 1.3, 3 ) 二义性" << endl;	//一实匹配double版本，一个匹配int版本

	// 小整型自动提升，越过short版本，直接匹配int版本，
	Sum( 'a', 'b' );

	// 算术类型转换不分级别高低，容易引起二义性问题
	cout << "Multiply( 3.5, 1.6 ) 二义性" << endl;	// 不能说double与float更接近就匹配float版本
}

void Test_FunctionPointer()
{
	StartOneTest( "几个基础常识" );
	cout << "函数指针赋值时，函数名前的'&'可省，编译器自动取函数地址" << endl;	// I prefer &
	cout << "函数指针调用时，不需要对指针解引用，直接用调用运算符即可" << endl;

	// 用decltype获取到的是函数类型，不是函数指针，使用时需要手动加'*'
	decltype( Subtract ) *func = Subtract;
	PrintTypeName( func );


	StartOneTest( "函数指针作为形参" );
	CalMathFunA( 5, 7, Sum );
	CalMathFunB( 6, 9, Multiply );


	StartOneTest( "函数指针作为返回值" );
	// 和数组的返回有些类似，也有三种方式

	cout << "返回函数指针——经典式" << endl;
	PFMath pFSum = GetFunc1( 1 );
	pFSum( 2, 3 );

	cout << "返回函数指针——别名式" << endl;
	PFMath pFSub = GetFunc2( 2 );	// 或GetFunc3( 2 )是一样的
	pFSub( 10, 2 );

	cout << "返回函数指针——C++11尾置式" << endl;
	PFMath pFMulti = GetFunc4( 3 );
	pFMulti( 3, 4 );
}

ChapterBase* Chapter06Init()
{
	// 第六章测试比较特殊，程序启动后开启后台线程用于测试
	thread worker1( []( void )
	{
		while ( Worker1Run )	// 先不考虑线程安全和锁，这里仅仅是简单的测试
		{
			this_thread::sleep_for( chrono::nanoseconds( 1 ) );	// 纳秒都行！？
			CallLocalStaticObject( false );
		}
		cout << "Worker1 stoped." << endl;
	} );
	worker1.detach();

	ChapterBase *ch06 = new ChapterBase( "第六章 函数" );
	ch06->AddSection( 1, "基础概念", &Test_Base );
	ch06->AddSection( 2, "数组形参", &Test_ArrayParam );
	ch06->AddSection( 3, "返回数组类型", &Test_ArrayReturn );
	ch06->AddSection( 4, "可变形参", &Test_VaryingParam );
	ch06->AddSection( 5, "函数重载", &Test_Overloaded );
	ch06->AddSection( 6, "函数指针", &Test_FunctionPointer );
	return ch06;
}

/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
// 这个函数包含一个局部静态变量，用于记录这个函数被（多个线程）调用的次数总和
void CallLocalStaticObject( bool needPrint )
{
	static int callingCount;	// 此变量会一直存在，默认是0，当然可以手动初始化
	if( needPrint )
		cout << "Until now, the CallingCount is : " << callingCount << endl;
	else
		++callingCount;
}

void PassingArgsByRef( const int &ri )
{
	cout << "ref value : " << ri << endl;
}

extern TCPoint SchoolPosition = { 120, 31 };
void PrintSchoolInfo( const std::string &name, size_t number, const TCPoint &pos )
{
	cout << "学校名称:\t" << name << endl;
	cout << "学生人数:\t" << number << endl;
	cout << "学校坐标:\t" << pos << endl << endl;
}

void PrintNames( initializer_list< string > names ) MCPP11
{
	for( auto &i : names )
		cout << i << endl;
}

// 这个函数我们已知都是字符串，比较简单，仅仅演示省略符的一般解析流程
void PrintNames( int argc... )	// include <cstdarg>
{
	va_list list;
	va_start( list, argc );
	for( int i = 0; i < argc; ++i )
		cout << va_arg( list, char* ) << endl;
	va_end( list );
}

int ( *GetRow1 ( int ( *matrix )[ 3 ], int row ) )[ 3 ]
{
	return &matrix[ row ];
}
ArrayRowi* GetRow2( int ( *matrix )[ 3 ], int row )
{
	return &matrix[ row ];
}
ArrayRowi& GetRow2r( int ( *matrix )[ 3 ], int row )
{
	return matrix[ row ];
}
auto GetRow3( int ( *matrix )[ 3 ], int row ) -> int ( * )[ 3 ]
{
	return &matrix[ row ];
}

void PrintPoint( TCPoint &pnt )
{
	PrintTypeName( pnt );
	cout << pnt << endl;
}
void PrintPoint( const TCPoint &pnt )
{
	PrintTypeName( pnt );
	cout << pnt << endl;
}

void Sum( int a, int b )
{
	cout << "The ( int )sum of ab : \t" << ( a + b ) << endl;
}
void Sum( short a, short b )
{
	cout << "The ( short )sum of ab : \t" << ( a + b ) << endl;
}
void Sum( double a, double b )
{
	cout << "The ( double )sum of ab : \t" << ( a + b ) << endl;
}
void Subtract( int a, int b )
{
	cout << "( int ) a - b = \t" << ( a - b ) << endl;
}
void Multiply( int a, int b )
{
	cout << "( int ) a * b = \t" << ( a * b ) << endl;
}
void Multiply( float a, float b )
{
	cout << "( float ) a * b = \t" << ( a * b ) << endl;
}

void CalMathFunA( int a, int b, FMath func )
{
	func( a, b );
}
void CalMathFunB( int a, int b, PFMath func )
{
	func( a, b );
}

void ( *GetFunc1( int flag ) )( int, int )
{
	switch ( flag )
	{
	case 1:
		return &Sum;
	case 2:
		return &Subtract;
	case 3:
		return &Multiply;
	default:
		return nullptr;
	}
}
FMath* GetFunc2( int flag )
{
	return GetFunc1( flag );
}
PFMath GetFunc3( int flag )
{
	return GetFunc1( flag );
}
auto GetFunc4( int flag ) -> void ( * )( int, int )
{
	return GetFunc1( flag );
}