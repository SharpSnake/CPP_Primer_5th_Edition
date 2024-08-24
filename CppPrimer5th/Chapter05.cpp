#include <stdexcept>

#include "TestBase.h"
#include "TestHelper.h"
#include "TCVector.h"

using namespace std;

void Test_Ifelse()
{
	StartOneTest( "悬垂else问题" );
	// else与离它最近的未匹配if配对，加花括号避免意图误解
	int ia = 3;
	if( ia % 2 )
		if( ia >= 5 )
			cout << "ia是大于等于5的奇数" << endl;
	else	// 与if( ia >= 5 )配对
		cout << "ia = " << ia << endl;
}

void Test_Switch()
{
	StartOneTest( "case标签必须是【整型常量表达式】" );
	auto _testFactorial = []( const int &f )	// 判断f是几的阶乘
	{
		switch( f )
		{
		case 2:		// 字面值整数当然可以，也可以是char类型（一个单引号字符）
			cout << "Factorial of 2" << endl;
			break;
		case Factorial( 3 ):					// 标注了constexpr，所以可以
			cout << "Factorial of 3" << endl;
			break;
		case Factorial( 3 ) * 4:				// 稍微复杂的常量表达式也可以
			cout << "Factorial of 4" << endl;
			break;
		default:	// 不管其他情况是否有代码，加一个default是一个好习惯
			cout << "Factorial of number greater than 4" << endl;
			break;
		}
	};

	int a = 1, b = 2, c = 3, d = 4;
	_testFactorial( a * b );
	_testFactorial( a * b * c * d );
	_testFactorial( Factorial( 6 ) );
	

	StartOneTest( "case标签的贯穿原则" );
	// 某个case标签匹配成功后，将从此case往后顺序执行所有case分支，直到break或者switch结束；
	// 所以，除非是有意设计为批量匹配，否则记得加break；
	auto _testScore = []( const int &s )
	{
		switch( s )
		{
		case 1:
			cout << "1分需要留级" << endl;	// 这里应该有一个break，否则程序将继续向下贯穿
		case 2: case 3:		// 贯穿的case可以不换行
			cout << "2~3分需要补考" << endl;
			break;
		case 4: case 5:
			cout << "4~5分优秀" << endl;	// 这里虽然是整个switch的结束，但加break仍是个好习惯
		}
	};
	_testScore( 1 );
	_testScore( 4 );


	StartOneTest( "case标签中的变量" );
	// 在一个标签定义并初始化一个变量，不能在另一个标签使用;
	// 当case的代码有多行时，用花括号包起来是比较好的习惯，省很多麻烦；
	auto _CalFactorial = []( const int &a )	// 计算f的阶乘
	{
		switch( a )
		{
		case 3:
			int f;	// 这里如果f有任何初始化行为，在case 4中就无法使用了，引发编译错误
			f = Factorial( 3 );
			cout << "Factorial of 3 is " << f << endl;
			break;
		case 4:
			f = Factorial( 4 );
			cout << "Factorial of 4 is " << f << endl;
			break;
		}
	};
	_CalFactorial( 3 );
	_CalFactorial( 4 );
}

void Test_Iterative()
{
	StartOneTest( "range for的原理" );
	// range for是靠获取容器的头尾迭代器，然后从头至尾将迭代器【解引用】返回给调用端的；
	// 一个容器类，必须包含【begin】和【end】这两个名字的函数，才能用range for依次访问元素；
	TCVector< int > myVec;
	for( int i = 0; i < 5; ++i )
		myVec.PushBack( Randi( 1, 100 ) );

	// 用range for打印元素，与右边注释的代码是等价的
	for( auto &i : myVec )	// for( auto i = myVec.begin(); i != myVec.end(); ++i )
		cout << i << endl;	//	cout << *i << endl;

	StartOneTest( "do-while中变量使用细节" );
	int ri;
	do
	{
		// 1、do中声明的变量生命周期仅在do语块内，while不能使用，所以ri需要在外面定义；
		ri = Randi( 1, 10 );
		cout << ri << endl;
	}
	while( ri > 5 );	// 别忘了';'

	int ri2;
	do
		cout << "继续尝试获取0-1随机数" << endl;
	while( ri2 = Randi( 0, 1 ) );	// 2、while中不能定义变量，需要使用的话在外面定义；
}

void Test_Jump()
{
	StartOneTest( "goto往下跳" );
	// 1、不能跳过一个定义并初始化的变量，并在后面继续使用它；
	// 这个和case标签中的变量定义相似；
	goto CodeEnd;
	int ri;	// 这里只能声明，不能有初始化行为
	ri = Randi( 1, 10 );

CodeEnd:	// 在eclipse里测试确实不能跳；但VC++ 2015竟然可以跳！！！
	ri = Randi( 50, 100 );
	cout << ri << endl;


	StartOneTest( "goto往回跳" );
	// 2、回跳时，变量相当于重新创建一次，利用回跳可以实现循环；
CodeBegin:
	int ri2 = Randi( 1, 10 );
	if( ri2 < 7 )
	{
		cout << ri2 << " < 7, continue." << endl;
		goto CodeBegin;
	}
	else
		cout << ri2 << " > 7, end." << endl;
}

void Test_Exception()
{
	StartOneTest( "exception safe" );
	// 当异常发生时，执行必要的清理（或者说回退）工作，成为异常安全；
	TCVector< int > ivec;
	try
	{
		ivec = { 1, 2, 3, 4, 5 };
		cout << ivec[ 7 ] << endl;
		ivec.Clear();		// 正常的逻辑，打印完要做清理，但这之前很可能因为异常而提前终止
	}
	catch ( const out_of_range &ex )
	{
		cout << ex.what() << endl;
		if( ivec.Size() )	// 所以捕获异常后，首先检查数据状态，视情况做必要的清理
			ivec.Clear();
	}


	StartOneTest( "throw and catch" );
	// 1、常见的std异常，一般就是定义在头文件<stdexcept>中的
	try
	{
		size_t buflen = 1024 * 1024 * 1024;
		double *buf = new double[ buflen ];
	}
	catch( const bad_alloc &ex )	// 尝试申请8G的内存，会引发bad_alloc
	{
		cout << ex.what() << endl;
	}

	try
	{
		string str = "abc";
		cout << str.substr( 4, 10 ) << endl;
	}
	catch( const out_of_range &ex )	// 获取超出范围的字符串，会引发out_of_range
	{
		cout << ex.what() << endl;
	}

	// 2、其他异常捕获：std抛异常一般是基于std::exception类，自定义异常也应该继承这个类；
	try
	{
		// 甚至可以抛一个数字、字符、字符串等等
		throw string( "随便抛个字符串异常" );
	}
	catch( const std::exception &ex )	// 不确定具体异常类，就捕获基类
	{
		cout << ex.what() << endl;
	}
	catch( const string &errmsg )	// 如果前面抛的是字面字符串，这里应该是char *
	{
		cout << errmsg << endl;
	}
	catch( ... )
	{
		// 省略符可以捕获所有的C++异常，但系统级异常无法捕获，比如数组越界等等；
		// try-catch仅仅捕获当前软件中C++代码throw出的异常，并不负责系统底层的错误；
		// 编码中要首先适度地做各种状态检测，再编写业务逻辑；
		// 然而这是很难的，且需要取舍的，当意外发生时，要做好回滚，以及随时打印日志；
		cout << "unknown error occurred" << endl;
	}
}

void Chapter05Init()
{
	auto ch05 = ChapterBase::AddChapter( 5, "第五章 语句" );
	ch05->AddSection( 1, "条件语句", &Test_Ifelse );
	ch05->AddSection( 2, "switch语句", &Test_Switch );
	ch05->AddSection( 3, "迭代语句", &Test_Iterative );
	ch05->AddSection( 4, "跳转语句", &Test_Jump );
	ch05->AddSection( 5, "异常", &Test_Exception );
}
static int _Init = ( Chapter05Init(), 0 );