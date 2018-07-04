#include "stdafx.h"

#include <string>
#include <cctype>
#include <cstring>
#include <vector>
#include <iterator>
#include <algorithm>

#include "TestBase.h"
#include "TestHelper.h"

using namespace std;


// 标准库字符串自己变大写
void _StringToUpper( string &str )
{
	std::transform( str.begin(), str.end(), str.begin(), 
		[]( unsigned char c ){ return toupper( c ); } );	MCPP11	// Lambda表达式
}

// 打印一维数组
template< typename T, size_t N >
void _PrintArray( const T ( &ary )[ N ] )	// 这里必须是数组的引用，还没想明白
{
	for( auto &i : ary )	MCPP11	// 静态的数组也可以用range for
		cout << i << " ";
	cout << endl;
}

// 打印二维数组
template< typename T, size_t M, size_t N >
void _PrintArray( const T ( &ary )[ M ][ N ] )
{
	for( auto &i : ary )
	{
		for( auto &j : i )
			cout << j << " ";
		cout << endl;
	}
	cout << endl;
}


void Test_StdString()
{
	StartOneTest( "string IO" );
	string words;

	// 输入一个单词，遇到空格、换行、tab就停止，终止条件是键入 ctrl + z
	while ( cin >> words )
	{
		CONSOLE_COLOR_AUTO_RESET;
		SetConsoleTextColor( CmdColor_Magenta );
		cout << "\tInput word: " << words << endl;
	}
	cin.clear();

	// 输入一行，终止条件同样是键入 ctrl + z
	while ( getline( cin, words ) )
	{
		CONSOLE_COLOR_AUTO_RESET;
		SetConsoleTextColor( CmdColor_Magenta );
		cout << "\tInput words: " << words << endl;
	}
	cin.clear();

	StartOneTest( "string size" );
	// size_t（定义在<cstddef>中）与机器无关，好处是增强代码的可移植性
	string str1 = "1234567";
	size_t len_str1 = str1.size();
	cout << "\"" << str1 << "\"'s length is: " << len_str1 << endl;

	// std的size属性一般都是无符号整形，尽量避免size与有符号数字计算，产生意外的结果；
	// -3会隐式转为unsigned，按溢出原则将会是一个非常大的数字，所以下面判断几乎永远是true；
	cout << "len_str1 < -3 ?: " << ( len_str1 < -3 ) << endl;


	StartOneTest( "string and character methods" );
	string str2 = "abc", str3 = "ghi";
	string str4 = str2 + "def" + str3 + "xyz";	// 加法原则：'+'两边至少有一个是std::string类型
	cout << "adding strings: " << str4 << endl;

	// <cctype>里面定义了从C继承过来的C++版本（头文件都没有.h后缀）的字符处理函数
	// 仅能处理0~255范围的char，中文字符没法处理
	// 下面将每个单词首字母大写，数字乘以二，标点全换成'#'，空白字符全换成'-'
	string str5 = "functions, described in Table	3.3 (overleaf)!!?";
	cout << str5 << endl;

	// 用到一个单词开头的标识符，遇到标点或者空白，预示着后面的字母是单词开头
	bool wordHead = true;

	for ( auto &i : str5 )	MCPP11	// std::string range for
	{
		if ( wordHead && isalpha( i ) )	// 如果i是一个单词的首字母，且小写
		{
			if( islower( i ) )	// 仅当是小写才转换
				i = toupper( i );
			wordHead = false;
		}
		else
		{
			if( isdigit( i ) )	// 如果i是一个数字
			{
				int d = stoi( string( 1, i ) );	// 这里用到数字转换函数定义在<string>中
				d *= 2;
				i = to_string( d )[ 0 ];
				wordHead = false;
			}
			else if( ispunct( i ) )	// 如果i是标点
			{
				i = '#';
				wordHead = true;
			}
			else if( isspace( i ) )	// 如果i是空白字符
			{
				i = '-';
				wordHead = true;
			}
		}
	}
	cout << str5 << endl;


	StartOneTest( "标准库string和C字符串的衔接" );
	char cstr[ 4 ] = "abc";		// C字符串末尾有一个'\0'，分配长度必须+1
	string str6 = cstr;			// std string不需要考虑末尾问题

	str6 += "def";	// 与旧字符串的拼接
	char cstr2[ 7 ] = { '\0' };
	strcpy( cstr2, str6.c_str() );	// <cstring>中的旧函数，以及std string查看旧字符串
	cout << "copy std string to c string: " << endl << cstr2 
		<< "——length is " << strlen( cstr2 ) /*这个长度计算没有加末尾的\0*/ << endl;
}

// std是模板库，注意模板≠类型，不要混淆，模板实例化后才是类
// 比如：std::vector仅仅是模板，std::vector< int >才是一个具体的类
void Test_StdVector()
{
	StartOneTest( "下标运算符与迭代器" );
	vector< string > vStr = { "123", "abc", "xxx" };

	// std容器的下标运算符和迭代器的解引用，结果基本都是（元素）引用类型；
	// 如果你声明临时变量类型时没有加&，就会发生一个变量拷贝，影响性能；
	// 除非你有意需要拷贝容器的元素进行操作，其他情况最好声明为（const）引用类型；
	decltype( vStr[ 0 ] ) item0 = vStr[ 0 ];	// string &
	item0 += "456";

	for( auto &i : vStr )	// auto会忽略引用，需要手动加&
		_StringToUpper( i );

	for( auto i = vStr.begin(); i != vStr.end(); ++i )	// 普通迭代器
	{
		// 【注意】：用到迭代器的循环过程，不要涉及容器本身的增删操作
		decltype( *i ) item = *i;	// string &
		item = "Item from iterator: " + item;
	}
	
	for( auto i = vStr.cbegin(); i != vStr.cend(); ++i )	MCPP11	// const迭代器，元素为只读
	{
		decltype( *i ) item = *i;	// const string &
		cout << item << endl;
	}
}

void Test_ArrayDeclaration()
{
	StartOneTest( "理解复杂数组类型" );
	// 类型的解读口诀：由内向外，从右向左；
	// 数组如果不初始化，也不会发生什么错误，只是元素的值都是不可预知的
	int iary[ 10 ] = { 1, 2, 3 };

	// 包含10个int指针的数组，和下面加括号的pary是完全不同的两个概念
	int *ptrs[ 10 ];
	for( auto i = 0; i < 10; ++i )
		ptrs[ i ] = &iary[ i ];
	*ptrs[ 3 ] = 4;

	// 从括号内看，pary是一个指针；从右看，它指向含有10个元素的数组；从左看，元素是int型的；
	int ( *pary )[ 10 ] = &iary;
	( *pary )[ 4 ] = 5;

	// 同理，rary是一个数组的引用，它绑定到含有10个元素的int数组；
	// rptrs也是一个数组的引用，不同的是，它绑定的数组元素类型是int*；
	int ( &rary )[ 10 ] = iary, *( &rptrs )[ 10 ] = ptrs;
	rary[ 8 ] = 9;
	*( rptrs[ 9 ] ) = 10;

	_PrintArray( iary );
}

void Test_ArrayNew()
{
	StartOneTest( "数组自动类型" );
	TCPoint pntary[ 3 ] = {};

	// auto会将数组解析成对应类型的指针，这也是C++里默认的隐式转换
	auto ptr = pntary;
	ptr->x = 2.2;
	ptr->y = 4.4;
	cout << *ptr << endl;

	// decltype解析仍然是数组类型
	decltype( pntary ) pntary2 = { { 1.5, 3.3 }, { 2.2, 5.5 } };
	_PrintArray( pntary2 );

	StartOneTest( "C++11数组\"迭代器\"" );	// 对数组来说，其实就是指针
	// 定义在头文件<iterator>中，对应的只读迭代器函数是cbegin和cend
	for( auto i = begin( pntary2 ); i != end( pntary2 ); ++i )	MCPP11
		i->Offset( 2, 3 );
	cout << "After offset, points are: " << endl;
	_PrintArray( pntary2 );

	// 指针间距离（即两个指针做减法）是一个带符号整形，同样定义在<cstddef>中
	ptrdiff_t arylen = end( pntary2 ) - begin( pntary2 );
	cout << "通过指针差得到数组的长度为：" << arylen << endl;

	StartOneTest( "数组的其他几个细节" );

	// 1、用数组初始化vector，下面两条是等价的
	vector< TCPoint > vPnt( pntary2, pntary2 + 3 );
	//vector< TCPoint > vPnt( begin( pntary2 ), end( pntary2 ) );

	cout << "initialize vector by array:" << endl;
	for( auto &i : vPnt )
		cout << i << endl;

	// 2、多维数组，其实C++中应该叫做数组的数组
	// mary是3行4列的二维数组，三维数组：a[ 3 ][ 5 ][ 5 ]，表示有3个波段，每个波段是5X5的二维数组
	// 初始化用嵌套的花括号，省略的元素将自动用默认值初始化；
	int mary[ 3 ][ 4 ] = { { 1, 2, 3, 4 }, { 5, 6, 7 }, { 9, 10 } };

	// 低维数据的获取：引用或者指针
	int ( &row )[ 4 ] = mary[ 1 ];		// 这个表示mary中的第二行的引用
	int ( *prow )[ 4 ] = &mary[ 1 ];	// 或者第二行的指针，等价于mary + 1

	// 多维数组的range for，每一维最好都用引用，一者是因为auto会自动将数组解释为指针，
	// 导致编译失败，二者是避免不必要的内存拷贝；
	cout << "Multidimensional Arrays:" << endl;
	for( auto &i : mary )
	{
		for( auto &j : i )
			cout << j << " ";
		cout << endl;
	}

	// 3、数组类型的别名声明
	using Vec4d = double[ 4 ];	MCPP11
	Vec4d v1 = { 1.1, 3.3, 4.4, 5.5 };
	cout << "Alias simplify for array with \"using\":" << endl;
	_PrintArray( v1 );

	typedef double Vec3d[ 3 ];
	Vec3d v2 = { 5.5, 9.9 };
	cout << "Alias simplify for array with \"typedef\":" << endl;
	_PrintArray( v2 );

	typedef double Vec2X2d[ 2 ][ 2 ];
	using Vec3X3d = double[ 3 ][ 3 ];
	Vec2X2d v3 = { { 1, 2 }, { 3, 4 } };
	Vec3X3d v4 = { { 1, 2, 3 }, { 4, 5, 6 }, { 7, 8, 9 } };
	cout << "Alias simplify for two-dimensional array:" << endl;
	_PrintArray( v3 );
	_PrintArray( v4 );
}

ChapterBase* Chapter03Init()
{
	ChapterBase *ch03 = new ChapterBase( "第三章 字符串、向量和数组" );
	ch03->AddSection( 1, "std::string", &Test_StdString );
	ch03->AddSection( 2, "std::vector", &Test_StdVector );
	ch03->AddSection( 3, "数组的类型声明", &Test_ArrayDeclaration );
	ch03->AddSection( 4, "数组的新特性", &Test_ArrayNew );
	return ch03;
}
