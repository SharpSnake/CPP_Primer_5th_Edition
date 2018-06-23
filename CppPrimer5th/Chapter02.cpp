#include "stdafx.h"
#include "Chapter02.h"
#include "TestHelper.h"
#include <vector>
#include <map>

using UI = unsigned int;	MCPP11	// 别名声明

int _VOutOfFunction;   // 函数体外的变量，书上说函数外变量
					   // 不能以_开头或_大写字母开头，但这里没错
extern int VChapter02extern = 2;   // 有初始化才是定义，不能在函数内初始化


// 基础类型
void Test_BaseType()
{
	// char有三个相关的类型，其中char并不能保证是有符号的，
	// 所以使用中如果确定需要使用有（无）符号，最好标明
	StartOneTest( "char的三个相关的类型" );
	char c1 = 127;
	signed char c2 = 127;
	unsigned char c3 = 255;
	cout << ( int )c1 << " " << ( int )c2 << " " << ( int )c3 << endl;


	// long和int一般情况下表现是相同的，
	// 不要使用容易引起混淆的类型用于计算，比如有、无符号互相加减乘除

	// 书上说给一个有符号类型赋超出范围的数，会导致未定义或崩溃
	// 但实际运行中是按溢出处理的
	StartOneTest( "有符号char溢出测试" );
	signed char c4 = 257;
	cout << ( int )c4 << endl;


	StartOneTest( "有、无符号类型共同参与计算" );
	UI u1 = 10, u2 = 40;
	int i1 = -42;
	cout << u1 + i1 << endl << u1 * i1 << endl;   // i会转为无符号再计算
	
	// 如果两个无符号计算产生负值，结果会按溢出处理
	// unsigned maximum = 4294967295, u1 - u2 = -30, 
	// window will prints: 4294967295 - 29
	cout << u1 - u2 << "( 4294967295 - 29 = " << 4294967295 - 29 << " )" << endl;


	StartOneTest( "十进制、八进制、十六进制常数表示" );
	int i2 = 20, i3 = 024, i4 = 0x14;
	cout << i2 << " " << i3 << " " << i4 << endl;
	
	StartOneTest( "特殊字符需要用转义序列表示" );
	cout << '\\' << '\t' << '\'' << '\"' << endl;
	

	StartOneTest( "内置类型字面值的前后缀声明" );
	// 内置类型字面值的前后缀声明
	const char *cs1 = u8"utf8字符串";   // u8编码占字节数比默认要多
	const wchar_t *wcs1 = L"宽字符串";
	cout <<"utf8字符串长度为：" << strlen( cs1 ) << endl;
	cout << "宽字符串长度为：" << wcslen( wcs1 ) << endl;

	UI u3 = 245u;
	long l1 = 45342342L;
	cout << u3 << " " << l1 << endl;

	float f1 = 3.123456789101112131415f;
	double d1 = 3.123456789101112131415L;   // 如果后缀改为f，则精度会自动损失

	// 默认输出精度是6，或者用头文件<iomanip>中的setprecision也可以设置
	streamsize dft_precision = cout.precision( 15 );
	cout <<  f1 << "\t" << d1 << endl;
	cout.precision( dft_precision );

	StartOneTest( "全局变量声明和定义分离" );
	int VChapter02extern = 5;   // 局部会覆盖全局，想用全局的话需要用::显式访问
	cout << VChapter02extern << " " << ::VChapter02extern << endl;
	

	// 假设有两个重载函数：void foo( int i );	void foo( char *p );
	// 引入 nullptr 的目的是避免二义性，过去的NULL和0等价，
	// 调用foo( NULL )时就会引起混淆，现在调用foo( nullptr )将明确地调用第二个重载
	StartOneTest( nullptr );	MCPP11
}

// 变量初始化
void Test_Initialization()
{
	StartOneTest( "C++11列表初始化" );
	int i5{ 567 };	MCPP11   // C++11列表初始化更严格，类型必须匹配，这里不能是小数
	map< string, TSPoint > map1 = { { "point1", { 1.1, 2.2 } }, 
									{ "point2", { 2.2, 3.3 } } };
	cout << i5 << endl;
	for( auto &i : map1 )	MCPP11
		cout << i.first << ": " << i.second << endl;
	

	StartOneTest( "各种情况的默认初始化" );
	int vInFunction;
	// 这里若尝试使用vInFunction则编译失败，函数内变量不会默认初始化
	cout << _VOutOfFunction << endl;

	// 类或结构体的成员变量不会默认初始化，默认都是不可预知的值，
	TSPoint sp1;		// 除非在构造函数中指定初始值（或者用列表初始化哪怕是空的{}，
	TCPoint cp1;		// 成员变量也会进行默认初始化，当然仅针对聚合类）来初始化对象。
	QuadraticPoly qp1, qp2{};
	cout << sp1 << endl << cp1 << endl << qp1 << endl << qp2 << endl;
}

// 复合类型
void Test_CompoundTypes()
{
	StartOneTest( "复合类型" );
	// 复合类型声明的组成 = basetype + 声明符；
	// 声明符 = 类型修饰符（*、&等） + 变量名
	// 类型的解读口诀：从右向左；
	//
	// r6的解读：最右离变量名最近的是'&'，说明r6根本上是一个引用，再往左是'*'，
	// 说明r6是一个指针对象的引用，再往左是基础类型int，说明r6是一个整型指针对象的引用；
	// r6永远是p6的引用，但可以通过r6间接地修改p6实际指向的数字；
	int i6 = 50, i7 = 100, *p6 = &i6, *&r6 = p6;
	*p6 += 5;
	cout << "p6开始指向i6，通过解引用修改i6的值：" << i6 << endl;
	
	r6 = &i7;
	cout << "通过指针的引用，间接修改了p6的指向，现在p6指向的整数值是：" << *p6 << endl;
}

// 计算整数的阶乘
constexpr int _CalFactorial( int n )	MCPP11
{
	/*int f = 1;
	for( int i = n; i >= i; --i )
	f *= i;
	return f;*/	// C++14
	return n <= 1 ? 1 : ( n * _CalFactorial( n - 1 ) );
}

// const相关
void Test_Const()
{
	StartOneTest( "const的几个细节" );
	// 不同于常规引用，const引用可以绑定不匹配类型的变量(r7)、表达式(r8)、字面值(r9)；
	// 原理：编译器构造了一个不可见的临时（变）量，const引用其实绑定的是这个临时量；
	double d2 = 3.14;
	int i7 = 100, i8= 10;
	const int &r7 = d2,		/* int tmp_d2 = ( int )d2; */
		&r8 = ( d2 + i8 ),	/* int tmp_exp = ( int )( d2 + i8 ); */
		&r9 = 5.5;			/* int tmp_num = ( int )5.5; */
	cout << r7 << " " << r8 << " " << r9 << endl;

	const int *p7 = &i8;	// 指向常量的指针：不能修改被指向对象的值，无论这个对象是否是const；
	int *const p9 = &i8;	// 常量指针：本指针永远指向某一个对象，不能变更；

	p7 = &i7;		// correct：p7重新指向了i7
	// &p7 = 12;	// error

	*p9 += 5;		// correct：p9的指向不能变，但它指向的数字可以改动，于是i8的值加了5
	// p9 = &i7;	// error
	cout << *p7 << " " << i8 << endl;


	StartOneTest( "新关键词\"constexpr\"" );
	// 基本的理解：
	// const—并不强调是运行时还是编译期常量；
	// constexpr—标明的变量或者函数，必须在编译期就能计算出具体的值；
	//	且在C++14中放宽了constexpr函数内部的代码规范，可以使用循环、临时变量等，
	//	而C++11要求只有一条return语句（但可以用递归）；
	int ary[ _CalFactorial( 4 ) ] = { 1, 2, 3 };	MCPP11
	for( auto i : ary )
		cout << i << " ";
	cout << endl;

	// constexpr的另外一个优点是可以实现自定义类的字面值，这些字面值储存在高速的只读内存区域；
	// 可以加快程序的运行速度。例如，定义北京和上海的坐标的字面值，然后计算两地中点，同样是字面值；
	constexpr  Coordinate beijing( 116.398277, 39.909214 ), shanghai( 121.458206, 31.116520 );

	// 在Visual Studio中，不需要启动程序，鼠标放在这个变量上，即可看到坐标的计算结果！
	constexpr  Coordinate midOfTwoCity = MidPoint( beijing, shanghai );
	cout << "北京：" << beijing << endl << "上海：" << shanghai << endl 
		<< "两地中点：" << midOfTwoCity << endl;
}

void Test_AutoType()
{
	// 用于测试的变量
	int i1 = 5, i2 = 9, &r1 = i1, *p1 = &i1, *const p2 = p1;
	const int ci1 = i1, &cr1 = ci1, *cp1 = &ci1;

	StartOneTest( "关键字auto" );
	// 相对来说，auto比较"懒惰"，会忽略top-const和引用类型，需要手动添加相关修饰符
	auto au1 = r1;			PrintTypeName( au1 );// int
	auto &au2 = r1;			PrintTypeName( au2 );// int &
	auto au3 = p1;			PrintTypeName( au3 );// int *
	
	auto au4 = cr1;			PrintTypeName( au4 );// int
	const auto au5 = cr1;	PrintTypeName( au5 );// const int
	auto au6 = cr1;			PrintTypeName( au6 );// int
	auto &au7 = cr1;		PrintTypeName( au7 );// const int &，引用ci1，保留了low-const
	auto au8 = p2;			PrintTypeName( au8 );// int *，忽略了p2的top-const
	auto au9 = cp1;			PrintTypeName( au9 );// const int *，指向cil，保留了low-const
	/* auto总结：如果希望声明const或者引用类型，需要手动添加修饰符，别指望auto自动推导 */

	StartOneTest( "关键字decltype" );
	// 相对auto，decltype的应用更加广泛，且更"实事求是"，会保留top-const和引用类型；
	// decltype不会真的去计算表达式的值，或调用表达式中的函数，仅仅是推导结果的类型；
	decltype( r1 ) dt1 = i1;MCPP11	PrintTypeName( dt1 );// int &
	decltype( p2 ) dt2 = &i1;		PrintTypeName( dt2 );// int *const
	decltype( cr1 ) dt3 = i2;		PrintTypeName( dt3 );// const int &
	decltype( r1 * 0.5F ) dt4;		PrintTypeName( dt4 );// float
	decltype( MidPoint( { 1, 3 }, { 2, 4 } ) ) dt5{ 0, 0 };	PrintTypeName( dt5 );// Coordinate

	// 以下是一些特殊情况，需要牢记：
	// 1、指针的解引用操作，结果是引用类型（所以必须初始化）
	decltype( *p1 ) dt6 = i2;		PrintTypeName( dt6 );// int &
	
	// 2、变量用括号括起来，结果是引用类型（所以必须初始化）
	decltype( ( i1 ) ) dt7 = i2;	PrintTypeName( dt7 );// int &

	// 3、赋值表达式，结果是引用类型（所以必须初始化）
	decltype( i2 = i1 ) dt8 = i2;	PrintTypeName( dt8 );// int &
}

Chapter02::Chapter02()
{
	m_Title = "第二章 变量和基本类型";
	m_TestCases[ 1 ] = SectionTest( "基础类型", &Test_BaseType );
	m_TestCases[ 2 ] = SectionTest( "变量初始化", &Test_Initialization );
	m_TestCases[ 3 ] = SectionTest( "复合类型", &Test_CompoundTypes );
	m_TestCases[ 4 ] = SectionTest( "const相关", &Test_Const );
	m_TestCases[ 5 ] = SectionTest( "自动类型", &Test_AutoType );
}
