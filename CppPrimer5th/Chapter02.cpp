#include "stdafx.h"
#include "Chapter02.h"
#include "TestHelper.h"
#include <vector>
#include <map>

int _VOutOfFunction;   // 函数体外的变量，书上说函数外变量
					   // 不能以_开头或_大写字母开头，但这里没错
extern int VChapter02extern = 2;   // 有初始化才是定义，不能在函数内初始化

void Chapter02::Run()
{
	// char有三个相关的类型，其中char并不能保证是有符号的，
	// 所以使用中如果确定需要使用有（无）符号，最好标明
	char c1 = 127;
	signed char c2 = 127;
	unsigned char c3 = 255;
	cout << ( int )c1 << " " << ( int )c2 << " " << ( int )c3 << endl;
	FinishOneTest( "char的三个相关的类型" );


	// long和int一般情况下表现是相同的，
	// 不要使用容易引起混淆的类型用于计算，比如有、无符号互相加减乘除

	// 书上说给一个有符号类型赋超出范围的数，会导致未定义或崩溃
	// 但实际运行中是按溢出处理的
	signed char c4 = 257;
	cout << ( int )c4 << endl;
	FinishOneTest( "有符号char溢出测试" );


	unsigned u1 = 10, u2 = 40;
	int i = -42;
	cout << u1 + i << endl;   // i会转为无符号再计算
	cout << u1 * i << endl;   // 同上
	
	// 如果两个无符号计算产生负值，结果会按溢出处理
	// unsigned maximum = 4294967295, u1 - u2 = -30, 
	// window will prints: 4294967295 - 29
	cout << u1 - u2 << "( 4294967295 - 29 = " << 4294967295 - 29 << " )" << endl;
	FinishOneTest( "有、无符号类型共同参与计算" );


	int i2 = 20, i3 = 024, i4 = 0x14;
	cout << i2 << " " << i3 << " " << i4 << endl;
	FinishOneTest( "十进制、八进制、十六进制常数表示" );

	cout << '\\' << '\t' << '\'' << '\"' << endl;
	FinishOneTest( "特殊字符需要用转义序列表示" );


	// 内置类型字面值的前后缀声明
	const char *cs1 = u8"utf8字符串";   // u8编码占字节数比默认要多
	const wchar_t *wcs1 = L"宽字符串";
	cout <<"utf8字符串长度为：" << strlen( cs1 ) << endl;
	cout << "宽字符串长度为：" << wcslen( wcs1 ) << endl;

	unsigned int u3 = 245u;
	long l1 = 45342342L;
	cout << u3 << " " << l1 << endl;

	float f1 = 3.123456789101112131415f;
	double d1 = 3.123456789101112131415L;   // 如果后缀改为f，则精度会自动损失

	// 默认输出精度是6，或者用头文件<iomanip>中的setprecision也可以设置
	streamsize dft_precision = cout.precision( 15 );
	cout <<  f1 << "\t" << d1 << endl;
	cout.precision( dft_precision );
	FinishOneTest( "内置类型字面值的前后缀声明" );


	int i5{ 567 }; MCPP11   // C++11列表初始化更严格，类型必须匹配，这里不能是小数
	map< string, TSPoint > map1 = { { "point1", { 1.1, 2.2 } }, 
									{ "point2", { 2.2, 3.3 } } };
	cout << i5 << endl;
	for( auto i = map1.begin(); i != map1.end(); ++i ) MCPP11
		cout << i->first << ": " << i->second << endl;
	FinishOneTest( "C++11列表初始化" );

	int vInFunction = 7;
	// 这里若尝试使用vInFunction则编译失败，函数内变量不会默认初始化
	cout << _VOutOfFunction << endl;
	FinishOneTest( "函数体内外变量默认初始化" );


	int VChapter02extern = 5;   // 局部会覆盖全局，想用全局的话需要用::显式访问
	cout << VChapter02extern << " " << ::VChapter02extern << endl;
	FinishOneTest( "全局变量声明和定义分离" );
}
