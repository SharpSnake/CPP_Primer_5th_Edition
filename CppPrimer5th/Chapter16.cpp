#include <vector>
#include <type_traits>
#include <utility>	// std::forward
#include <unordered_set>

#include "TestBase.h"
#include "TestHelper.h"
#include "TCVector.h"
#include "Inheritance.hpp"

#pragma warning( disable:4101 )
#pragma warning( disable:4244 )
#pragma warning( disable:26800 )

using namespace std;


int iCha16 = 7;		// 定义一个有静态生存期的整数

// 类模板静态成员的定义及初始化
template< typename T > unsigned Vector2< T >::TotalCount = 0;

// C++11显式实例化之定义
template class TCVector< unsigned >;	MCPP11
template void PrintArray1D( double ( & )[ 5 ] );


void Test_TemplateBase()
{
	StartOneTest( "重要概念" );
	{
		CONSOLE_COLOR_AUTO_RESET;
		SetConsoleTextColor( CmdColor_Yellow );

		// 模板参数列表、模板实参、模板实例

		cout << "类模板内可以直接使用模板名字，不需要提供模板实参。" << endl;
		TCVector< int > vsort = TCVector< int >( { 5, 3, 9, 2 } ).Sorted();
		cout << vsort << endl2;

	}

	StartOneTest( "非类型模板参数" );
	// 1、整型，且必须为常量表达式
	cout << TFactorial< 5 >::value << endl;
	cout << TFactorial< sizeof( int ) >::value << endl;

	// 2、有静态生存期的指针或引用，或者函数指针
	cout << "feval Factorial\t" << Fevali< &iCha16, Factorial >() << endl;
	cout << "feval Cumsum\t" << Fevali< &iCha16, Cumsum >() << endl;


	StartOneTest( "模参的类型成员" );
	// 将模参的类型成员用作类型声明时，必须用关键词typename修饰，因为从语法上看，
	// 不能确定T::typemember是一个静态成员还是类型成员，所以前面需要用typename强调一下
	vector< int > ivec = { 1, 2, 3 };
	vector< int >::size_type orisize = FreeVector( ivec );
	cout << "origin size:\t" << orisize << endl <<  "current capacity:\t" << ivec.capacity() << endl;

	TCVector< TCPoint > vpnt = { { 2.5, 1.0 }, { 3.3, 2.2 }, { 1.5, 9.9 } };
	TCVector< TCPoint >::_iterator iter( vpnt.begin(), &vpnt );
	TCVector< TCPoint >::_iterator::reference vpnt_1 = *( ++iter );
	cout << "second point:\t" << vpnt_1 << endl;

	FreeVector( vpnt );
	cout << "current capacity:\t" << vpnt.Capacity() << endl;


	StartOneTest( "模板形参推断" );
	// 函数模板中，允许的两种类型转换：1、忽略顶层const；2、数组按指针对待；
	// 其余的算术转换和继承中的上行转换都不能【自动】推断；
	double d1 = 3.5;
	const double cd1 = 4.0;
	int i1 = 3;
	BinaryFeval( d1, cd1 );		// OK!
	//BinaryFeval( d1, i1 );	// ERROE! int不能自动转double

	Distance dis1 = 3, dis2 = 5.5;
	DistanceEx disex1 = 4.5;
	BinaryFeval( dis1, dis2 );		// OK!
	//BinaryFeval( dis1, disex1 );	// ERROE!

	// 以上错误的调用，仅仅是不能自动推断，如果【显式】指定模板实参，也是可行的
	BinaryFeval< double >( d1, i1 );
	BinaryFeval< Distance >( dis1, disex1 );


	StartOneTest( "函数模板与函数指针" );
	// 1、用函模初始化一个函数指针时，可以自动进行模板实参推断
	using PFBinary_d = double( * )( const double&, const double&, plus< double > );
	using PFBinary_i = int( * )( const int&, const int&, minus< int > );
	PFBinary_d addfunc_d = BinaryFeval;
	PFBinary_i subfunc_i = BinaryFeval;
	cout << addfunc_d( 1.5, 1.3, plus< double >() ) << endl;
	cout << subfunc_i( 5.5, 1.3, minus< int >() ) << endl;

	// 2、当函数指针本身作为形参时，且这个形参所属的函数有多个重载时，一般需要指明模板实参


	StartOneTest( "函数模板重载规则" );
	// 1、模函与普函相同匹配时，优先选择普函
	DebugInfo( string( "std::string" ) );
	DebugInfo( "char string" );	// 这个匹配模函char数组引用的版本，普函的话涉及到一次string的构造

	// 2、多个可行的模函之中，选特例化最好的
	DebugInfoT( string( "std::string" ) );
	DebugInfoT( "char string" );

	//Exercise 16.49:
	//template < typename T > void f( T );
	//template < typename T > void f( const T* );
	//int *p;
	//f( p );	// 采用f( T )版本，如果p是const int*则采用f( const T* )


	StartOneTest( "函数模板特化" );
	// 以template<>开头，为每个模参提供实参，特化本质上是实例化
	// 定义时常规模板在前，特化版本在后，都放在一个文件里，函模没有偏特化（有重载）
	PrintArray1D( "abcdef" );
	PrintArray1D( "wxyz" );	// Specialization version
}


void Test_ClassTemplate()
{
	StartOneTest( "定义成员的格式" );
	// 1、外部定义成员函数
	TCVector< int > fatv = { 1, 2, 3, 4, 5 };
	fatv.PopBack( 2 );
	cout << "fatv's capacity:\t" << fatv.Capacity() << endl;
	fatv.ShrinkToFit();
	cout << "fatv's capacity:\t" << fatv.Capacity() << endl;

	// 2、一般类的模板成员
	Distance dis0( 10 );
	dis0 += 3;	dis0.operator-=< int >( 7.6 );
	cout << "dis0:\t" << ( double )dis0 << endl;

	// 3、类模板的模板成员
	Vector2< float > fv( 10, 10 );
	fv.Move( 3, 3 );	fv.Move< int >( 2.5, -2.5 );
	cout << "fv after move:\t" << fv << endl;

	

	StartOneTest( "类模板成员的实例化" );
	// 1、成员函数只有当程序使用它时才实例化
	TCVector< TSPoint > spntv = { { 3, 5 }, { 6, 1.5 } };
	cout << spntv << endl;
	// TSPoint并没有定义比较运算符，所以不支持排序，但只要不使用Sorted函数，TCVector仍可正常运行
	//spntv = spntv.Sorted();	// ERROR!


	StartOneTest( "友元" );
	// 1、友元可以是其他类模板的【某个实例】，这个友元需要有【前置声明】，
	// 类模板的前置声明，可以很简略，告诉别人有几个模参即可
	Extent2<> ext1( Vector2<>( 1, 1 ), Vector2<>( 3, 5 ) );
	Vector2<> extCenter( ext1 );
	cout << ext1 << endl << extCenter << endl;

	// 2、通用模板友元：意味着友元模板的所有实例，都可以完全访问自己
	// 通用的声明需要有完整的模参列表，但要起一个和自己模参不同的名字
	Extent2<> ext2( Vector2< int >( 2, 1 ), Vector2< int >( 5, 3 ) );
	cout << ext2 << endl;

	// 3、C++11——模参友元，可以直接将模参声明为友元，暂且随便写个例子：
	Vector2< Distance > v1( 3, 0 ), v2( 0, 4 );
	Distance dis( v1, v2 );		MCPP11
	cout << "Distance between v1 and v2:\t" << ( double )dis << endl;


	StartOneTest( "静态成员" );
	// 1、成员变量的定义及初始化，见文件顶端

	// 2、相同模参的模板实例对象共享一个static成员，不同模参的的静态对象互不干扰
	// 另外，和一般成员函数一样，静态成员函数也是外部有使用才实例化；
	Vector2< short > iv1( 1, 2 );
	cout << "double Vector2's TotalCount:\t" << Vector2<>::GetTotalCount() << endl;
	cout << "short Vector2's TotalCount:\t" << Vector2< short >::GetTotalCount() << endl;


	StartOneTest( "类模板特化" );
	// 1、全特化，要求与原模板在同一个namespace里，非排序容器必须自定义哈希器，或者特例化标准库哈希器
	hash< Coordinate > coordhash;
	unordered_set< Coordinate > coords = { { 1, 2 }, { 5, 6 }, { 2, 3 } };
	if( coords.find( Coordinate( 2, 3 ) ) != coords.end() )
		cout << "Coordinate( 2, 3 )已经存在" << endl;
	if( coords.find( Coordinate( 1, 3 ) ) == coords.end() )
		coords.insert( Coordinate( 1, 3 ) );
	for( auto &i : coords )
		cout << "\t" << i << endl;

	// 2、偏特化（部分特化）
	cout << "（1）偏特化多个模参中的一部分" << endl;
	MArray< int, 5 > intary = { 1, 3, 5, 7, 9 };
	cout << intary << endl;
	MArray< string, 3 > strary = { "abc", "i'm hulk!", "you are awesome" };
	cout << strary << endl;

	cout << "（2）偏特化模参的特定形式" << endl;
	// 模仿remove_reference
	int i1 = 55, &ri1 = i1, &&rri1 = std::move( i1 );
	const int &cri1 = i1;
	RemoveRef< decltype( i1 ) >::type var1;					PrintTypeName( var1 );
	RemoveRef< decltype( ri1 ) >::type var2;				PrintTypeName( var2 );
	RemoveRef< decltype( rri1 ) >::type var3;				PrintTypeName( var3 );
	RemoveRef< decltype( cri1 ) >::type var4 = 0;			PrintTypeName( var4 );

	MArray< vector< int >, 2 > vecary = { { 2, 4, 6 }, { 11, 13, 15, 17, 19 } };
	cout << vecary << endl;

	// 3、成员特化，与一般函数模板特化相似，以template<>开头
	Extent2< double > extdbl = { Vector2<>{ 1, 1 }, Vector2<>{ 5, 5 } };
	Extent2< int > extint = { Vector2< int >{ 1, 1 }, Vector2< int >{ 5, 5 } };
	cout << "Extent2< double > width:\t" << extdbl.Width() << endl;
	cout << "Extent2< int > width:\t\t" << extint.Width() << endl;
}


// 模板别名必须在全局、命名空间或类中声明。
template< typename T > using MyVec = TCVector< T, std::allocator< T > >;	MCPP11

// 函数模板的别名，目前只知道可以这样用，只能声明一个函数模板指针
template< typename T > using PFArray3 = void ( * )( T ( & )[ 3 ] );			MCPP11
PFArray3< float > PrintAry3f = PrintArray1D< float, 3 >;

void Test_NewTemplate()
{
	StartOneTest( "模板别名" );
	// 1、旧语法，只能为模板的某个实例起别名
	typedef TCVector< int > TIntVec;
	TIntVec tiv = { 1, 2, 3 };

	// 2、新标准利用using可以更灵活的起别名，直接为整个模板起别名
	MyVec< double > mvec = { 1, 2, 5.5 };
	float ary3f[ 3 ] = { 1.5, 2.0, 0.5 };
	PrintAry3f( ary3f );


	StartOneTest( "默认模板实参" );
	// 新标准下，函数模板也可以有默认模实参，之前只能类模板有
	cout << "default is plus:\t" << BinaryFeval( 3, 6 ) << endl;	MCPP11
	cout << "multiplies:\t" << BinaryFeval( 3, 6, multiplies< double >() ) << endl;
	cout << Vector2<>( 1.5, 2.7 ) << endl;	// 类模板至少要有一个尖括号


	StartOneTest( "模板显式实例化" );
	// 若某个模板的实例很常用，那么在许多源文件中都会有这个模板的一个实例，
	// 会增加编译时间，且使程序变得很庞大，新标准允许提前实例化一个模板实例，大家看到声明就能用
	// 1、实例化声明放在TCVector.h末尾，这样每个引用此头文件的地方都会在使用前看到，声明可以有多次
	// 2、实例化定义放在某个源文件中，编译器通过链接，让每个用到此实例的其他地方都能找到这个实例
	TCVector< unsigned > uvec = { 1, 3, 5, 7 };	// 此实例定义在Chapter16.cpp开头
	cout << uvec << endl;	// 和一般的实例化不同的是，类模板显式实例化，会实例化【所有成员】

	double ary5d[ 5 ] = { 1, 1.5, 2, 2.5, 3 };
	PrintArray1D( ary5d );	// Functions.hpp末尾


	StartOneTest( "函模中使用尾置返回类型" );
	// 尾置的话，可以用decltype推测形参解引用后的类型作为返回类型，直接的话不可以
	TCVector< unsigned >::_iterator iter1( uvec.begin(), &uvec );
	TCVector< unsigned >::_iterator iter2( uvec.end() - 1, &uvec );
	cout << "_iterator plus:\t" << IteratorFeval( iter1, iter2, plus< unsigned >() ) << endl;


	StartOneTest( "类型转换及类型支持模板" );	MCPP11
	// 头文件<type_traits>中定义了一系列用于类型转换的模板，IteratorFeval用到了去引用模板
	// 实现原理：模板特化与引用折叠，注意remove_reference不管是 T& 还是 T&& 都变为T
	remove_reference< decltype( *iter1 ) >::type var1;							PrintTypeName( var1 );
	add_lvalue_reference< decltype( var1 ) >::type var2 = var1;					PrintTypeName( var2 );
	add_rvalue_reference< decltype( var1 ) >::type var3 = std::move( var1 );	PrintTypeName( var3 );
	remove_pointer< decltype( &var1 ) >::type var4;								PrintTypeName( var4 );
	add_pointer< decltype( var1 ) >::type var5;									PrintTypeName( var5 );
	make_unsigned< int >::type var6;											PrintTypeName( var6 );
	double dary[ 5 ][ 10 ];
	remove_all_extents< decltype( dary ) >::type var7;							PrintTypeName( var7 );
	add_const< decltype( dary ) >::type var8 = {};								PrintTypeName( var8 );

	// 还有一些有用的获取属性的模板
	cout << extent< decltype( dary ), 1 >::value << endl;	// 这个是测数组某个维度的长度的
	cout << rank< decltype( dary ) >::value << endl;	// 这个是测数组总共有几个维度
	cout << is_class< TCPoint >::value << " " << is_class< ProgramLan >::value << endl;
	cout << "var5 is_pointer:\t" << is_pointer< decltype( var5 ) >::value << endl;
	cout << "TCMan is_abstract:\t" << is_abstract< TCMan >::value << endl;
	cout << "Creature is_abstract:\t" << is_abstract< Creature >::value << endl;


	StartOneTest( "引用折叠" );		MCPP11
	// 形如T &的形参，最终都会折叠为左值引用，且T不能是右值
	// 形如T &&的形参，无论T是什么类型，最终【不会改变】左右值的属性，也会【保留const】属性
	// 当形参是T &&形式时，实参可以是任何类型，所以称T &&为【万能模板形参】
	CollapseKeepReference( var1 );	// 非引用变量当做左值引用看待
	CollapseKeepReference( var2 );
	CollapseKeepReference( std::move( var1 ) );	// 只有T&& &&会折叠为T&&，其余都是T&
	CollapseKeepReference( 5 );
	CollapseKeepReference( var8 );	// const -> const

	// std::move的实现原理就是用了万能形参，然后先对T去引用后，再static_cast为 &&
	// std::move说白了就是强转！
	string s1 = "abcde", s2 = static_cast< string&& >( s1 );	MCPP11
	cout << "s1: " << s1 /* 此时s1已经被s2掠夺空了 */ << " s2: " << s2 << endl2;

	// C++11完美转发：std::forward，当使用上面的完美形参传递一个变量后，
	// 模板内或许还要继续调用其他函数，这时往往不希望改变变量的左右值属性，就用forward保驾护航
	string s3 = "12345", s4 = "fghij";
	TCVector< string > svec;
	svec.EmplaceBack( s3 );		MCPP11
	svec.EmplaceBack( std::move( s4 ) );
	svec.EmplaceBack( 10, 'c' );
	svec.EmplaceBack( "I'm Iron Man!", 8 );
	cout << "s3: " << s3 << " s4: " << s4 << endl;
	cout << svec << endl;
	// 智能指针的make_shared和vector的emplace_back都需要用到完美转发，其原理也是引用折叠


	StartOneTest( "可变参数模板" );		MCPP11
	// 1、模板参数包和函数参数包的sizeof...计算结果是一样的
	cout << "ArgsSize:\t" << ArgsSize() << endl;		MCPP11
	cout << "ArgsSize:\t" << ArgsSize( 4, 4.6, "one string" ) << endl;

	// 2、简单的可变参数函模，一般用递归的方式逐一取出参数包中的首元素
	string strcom = StringCombine( '-', 8.999, 23, "i'm groot", 44U, 's' );		MCPP11
	cout << "StringCombine:\t" << strcom << endl;
	cout << "StringCombine:\t" << StringCombine( ' ' ) << endl;

	// 3、模参包扩展后形成了函数的形参：Args... args
	// 函参包的扩展，指的是对每一个形参分别应用相同的【模式】，比如调用一个单形参函数或一个可变形参函数
	ErrorMsg( "It's wrong", "It's totally wrong", "I'm sorry" );		MCPP11
	// 上面的StringCombine是解包后继续调用可变参数函数，还有TCVector的EmplaceBack也是一个例子
}


ChapterBase* Chapter16Init()
{
	ChapterBase *ch16 = new ChapterBase( "第十六章 模板、泛型" );
	ch16->AddSection( 1, "基础概念", Test_TemplateBase );
	ch16->AddSection( 2, "类模板", Test_ClassTemplate );
	ch16->AddSection( 3, "模板新特性", Test_NewTemplate );
	return ch16;
}
