#include "stdafx.h"

#include <limits>	// numeric_limits
#include <functional>	// function objects, bind, function< T >
#include <algorithm>
#include <map>

#include "TestBase.h"
#include "TestHelper.h"
#include "TCVector.h"

using namespace std;


// 有许多成对儿的运算符，让其中一个做实际的工作，另一个在实现中调用前一个，
// 可以减少重复代码，更好维护；比如前置++和后置++，*和->
void Test_OperatorBase()
{
	StartOneTest( "运算符函数—成员与非成员" );
	// 当运算符重载为成员函数是，第一个参数隐式为this，比定义在外部可以少一个参数
	TCPoint pnt1{ 5.5, 6.2 }, pnt2{ 1.5, 3 };
	cout << "operator+:\t" << ( pnt1 + pnt2 ) << endl;
	cout << "operator-:\t" << ( pnt1 - pnt2 ) << endl;
	cout << "explicitly operator+:\t" << pnt1.operator+( pnt2 ) << endl;

	// 运算符定义为成员时，调用者必须严格是此类型，将无法用于混合类型表达式
	// string s2 = "def" + s1;如果string将+定义为成员函数，字面值字符串拼接将无法实现；
	TSPoint spnt1{ 2.5, 9.0 };
	cout << "operator+ as a member function:\t\t" << ( pnt1 + spnt1 ) << endl;
	cout << "operator- as a nonmember function:\t" << ( spnt1 - pnt2 ) << endl;


	StartOneTest( "输入输出运算符" );
	TCPoint ipnt;
	cout << "Input a point:";
	if( !( cin >> ipnt ) )			// 输入要做什么？
	{
		// 忽略输入流中换行符前的错误输入，或将位置移动到末尾 cin.seekg( cin.end )
		cin.clear();
		cin.ignore( numeric_limits< streamsize >::max(), '\n' );

		cout << "Input a point again:";
		cin >> ipnt;
	}
	cout << ipnt << endl;	// 输出不要做什么？


	StartOneTest( "赋值、复合赋值、下标运算符" );
	// 强调一点，都要返回自身的引用
	pnt1 = pnt2 = { 2, 4 };
	pnt1 += pnt2;
	cout << pnt1 << "\t\t" << pnt2 << endl;

	// 下标operator必须是成员，一般定义两个版本
	TCVector< TCPoint > vpnt = { { 2.5, 1.0 }, { 3.3, 2.2 }, { 1.5, 9.9 } };
	cout << "TCVector::operator[]:\t" << vpnt[ 1 ] << endl;


	StartOneTest( "自增、减运算符、访问运算符" );
	TCVector< TCPoint >::_iterator iter( vpnt.begin(), &vpnt );
	cout << *( ++iter ) << endl;	// vpnt[ 1 ]
	cout << *iter++ << endl;		// vpnt[ 1 ]
	cout << *iter-- << endl;		// vpnt[ 2 ]
	cout << *( --iter ) << endl;	// vpnt[ 0 ]
	cout << "iterator operator->:\t" << iter->x << " " << iter->y << endl;
	try{
		--iter;						// error : vpnt[ -1 ]
	}
	catch( exception ex ){
		cout << ex.what() << endl;
	}
}

void Test_FunObj()
{
	StartOneTest( "函数调用运算符" );
	// 必须是成员，定义了()的类的对象叫【函数对象】，可用于迭代器算法
	Op_Abs< int > abs;
	cout << abs( -5 ) << endl;

	TCVector< int > ivec = { 3, -7, 9, -55 };
	for_each( ivec.begin(), ivec.end(), abs );
	cout << ivec << endl;


	StartOneTest( "Lambda的本质是函数对象" );
	// 编译器会将Lambda表达式翻译成一个含()的未命名类，并生成一个函数对象；
	// 如果Lambda需要捕获外部变量，这些变量会作为未命名类的成员，并生成相应的构造函数；
	TCVector< double > dvec = { 1.5, 3.3, 0.8 };
	double ntimes = 2;						MCPP11
	for_each( dvec.begin(), dvec.end(), [ ntimes ]( double &val ) { val *= ntimes; } );
	cout << dvec << endl2;

	// 上面的Lambda会被编译器解释成一个与Op_Times_N< double >类似的类
	for_each( dvec.begin(), dvec.end(), Op_Times_N< double >( ntimes ) );
	cout << dvec << endl;


	StartOneTest( "标准库提供的函数对象" );
	// <functional>中定义了一些常用的（泛型）函数对象：加减乘除、比大小、逻辑运算等等
	cout << "minus:\t" << minus< double >()( 5.5, 3 ) << endl;
	cout << "modulus:\t" << modulus< int >()( -10, 3 ) << endl;

	// 泛型算法也常用到标准库的函数对象
	TCVector< int > ivec2 = { 3, 7, 22, 11, 9 };
	sort( ivec2.begin(), ivec2.end(), less< int >() );	// 升序
	cout << "ascending sort:\t" << endl << ivec2 << endl2;
	sort( ivec2.begin(), ivec2.end(), greater< int >() );	// 降序
	cout << "descending sort:\t" << endl << ivec2 << endl;
}

void Test_NewFunction()
{
	StartOneTest( "绑定函数对象" );
	using namespace std::placeholders;		MCPP11

	// bind产生的结果其实也是函数对象，可以绑定（成员）函数、lambda、函数对象等等
	auto bfunc = bind( Randi, _1, 100 );
	TCPoint pnt = { 5, 5 };
	auto bmemfunc = bind( &TCPoint::Offset, &pnt, _1, _2 );
	auto blambda = bind( []( double d1, double d2 ){ return d1 / d2; }, 
		_2, _1 );	// bind可以改变最终输入参数的顺序和个数
	auto bfunobj = bind( plus< double >(), _1, _2 );

	cout << "bind Randi:\t" << bfunc( 50 ) << endl;
	bmemfunc( 1.5, 2 );
	cout << "bind member function:\t" << pnt << endl;
	cout << "bind lambda:\t" << blambda( 3, 10 ) << endl;	// 10 / 3
	cout << "bind func obj:\t" << bfunobj( 10, 5 ) << endl;

	// 前面数列乘2的例子也可以这样实现，函数对象第二个参数写死为2，算法只需提供第一个参数
	TCVector< double > dvec = { 1.5, 3.3, 0.8 };
	transform( dvec.begin(), dvec.end(), dvec.begin(), 
		bind( multiplies< double >(), _1, 2 ) );	MCPP11
	cout << "multiplies:\t" << endl << dvec << endl2;

	// Exercise 14.43：判断一个给定的整数是否能被一列数中的某一个除尽，即余数为0
	auto initsCont = { 3, 4, 5 };
	int givenint = 16;
	auto divisibleBy = find_if_not( initsCont.begin(), initsCont.end(), 
		bind( modulus< int >(), givenint, _1 ) );	MCPP11
	if( divisibleBy != initsCont.end() )
		cout << givenint << " is divisible by " << *divisibleBy << endl;
	else
		cout << givenint << " is not divisible by any int of container" << endl;


	StartOneTest( "C++11 function<>" );
	// function<T>可以包容相同调用形式的函数指针、lambda、函数对象
	using BinaryIntFun = function< int( int, int ) >;	MCPP11

	// 简单的计算器
	map< string, BinaryIntFun > calculator = { { "rand", Randi }, { "+", plus< int >() }, 
	{ "*", []( int i1, int i2 ){ return i1 * i2; } } };
	cout << "+:\t" << calculator[ "+" ]( 3, 6 ) << endl;
	cout << "*:\t" << calculator[ "*" ]( 5, 4 ) << endl;
	cout << "rand:\t" << calculator[ "rand" ]( 1, 10 ) << endl;

	// 不能直接存一个重载函数，function不能自动匹配参数类型合适的那一个，可以用函数指针代替
	int ( *pMid )( int, int ) = &MidPoint;
	calculator.insert( { "mid", pMid } );
	cout << "mid:\t" << calculator[ "mid" ]( 3, 7 ) << endl;
}

void Test_ClassConver()
{
	StartOneTest( "类型转换运算符" );
	// 必须是成员，没有形参和返回类型，一般是const的，默认是隐式转换
	// 一般A->B的转换要有一对一的【映射关系】，否则不要轻易定义转换运算符
	Coordinate coord( 120, 30 );
	TCPoint pnt = { 5.5, 9.9 };
	TCPoint plusPnt = pnt + coord;	// Coordinate中定义了向TCPoint转换的运算符，所以隐式转了
	cout << plusPnt << endl;


	StartOneTest( "强制显式转换" );
	// 与第七章"构造函数与隐式类型转换"提到的一样，使用【explicit】关键词使转换是显式的
	// 当对象被当做条件时（即当做bool）可以【忽略】explicit，bool转换一般都是explicit的
	// 【慎用隐式转换】，在【混合表达式】中可能产生奇怪的结果
	if( coord )
		cout << coord << " 不在原点" << endl;

	TCPoint pnt2 = { 3, 4 };	// 显式的转换为double，即求点到原点的距离
	cout << ( double )pnt2 << " or " << static_cast< double >( pnt2 ) << endl;


	StartOneTest( "隐式转换与二义性" );
	CONSOLE_COLOR_AUTO_RESET
	SetConsoleTextColor( CmdColor_Yellow );

	cout << "不要在两个类间定义相同的类型转换，"
		"比如在Coordinate中已经定义了它向TCPoint的转换符,\r\n"
		"就不要在TCPoint中定义一个接受Coordinate形参的构造函数，\r\n"
		"否则在相关函数中会有二义性" << endl2;
	
	cout << "也不要在一个类中定义一个以上的算术类型转换运算符，\r\n"
		"否则相关函数调用或算术运算中也会产生困扰" << endl;
}

ChapterBase* Chapter14Init()
{
	ChapterBase *ch14 = new ChapterBase( "第十四章 运算符与类型转换" );
	ch14->AddSection( 1, "常用运算符", &Test_OperatorBase );
	ch14->AddSection( 2, "函数对象", &Test_FunObj );
	ch14->AddSection( 3, "C++11 function", &Test_NewFunction );
	ch14->AddSection( 4, "类型转换", &Test_ClassConver );
	return ch14;
}
