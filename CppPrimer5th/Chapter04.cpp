#include "stdafx.h"

#include <vector>
#include <bitset>	// 借助这个容器测试位运算
#include <cstring>	// memcpy

#include "TestBase.h"
#include "TestHelper.h"
#include "Inheritance.hpp"


void Test_ExpressionBase()
{
	StartOneTest( "几个基本常识" );
	cout << "表达式：由运算对象（一个或多个）和运算符（0个或多个）组成" << endl;
	cout << "右值可以用左值代替，反之不一定可以" << endl;
	cout << "一般的IO运算符（例如\"<<\"和\">>\"）满足左结合律，即从左向右依次参与运算" << endl;
	cout << "大多数运算，小整型（bool，short，char）会先提升为大整型（int）再参与计算" << endl;

	StartOneTest( "表达式中的求值顺序不可预知" );
	auto _add = []( double val1, double val2 ){
		cout << "function _add called" << endl;
		return val1 + val2;
	};
	auto _multiply = []( double val1, double val2 ){
		cout << "function _multiply called" << endl;
		return val1 * val2;
	};
	double d1 = 3, d2 = 2;
	double d3 = _add( d1, d2 ) + _multiply( d1, d2 );
	// 比如上面表达式中两个函数，我们无法预知先调用哪个，这个和编译器有关系；
	// 如果两个函数内部涉及到共同的外部变量，且它会发生变化，最终可能产生无法预知的错误结果；
	// 所以表达式中包含互相牵连的运算对象时，最好拆分成多个表达式写，确保你的意图不被误解；
	cout << "value of d3: " << d3 << endl;

	// C++中只有四个运算符可以保证运算对象的计算顺序：&&、||、?:、,；
	// 其中&&和||叫做短路求值，都是先计算左边，右边可能根本不计算；


	StartOneTest( "赋值运算符" );
	// 1、满足【右结合律】，即多重赋值时从右向左依次赋值，但必须保证类型是匹配的（或可以隐式转换）
	int a, b, c = 5, *p = &c;
	a = b = ++( *p );
	cout << a << " " << b << endl;

	// 2、运算优先级极低，仅比复合赋值符高，比逻辑运算符还低；
	// 所以在条件语句中嵌入赋值运算，最好【加括号】，否则易引起误解
	int ri;
	while ( ( ri = Randi( 1, 20 ) ) <= 12 )
		cout << ri << "\tstill <= 12" << endl;
	cout << "终于找到一个大于12的随机数:\t" << ri << endl;
}

void Test_BitwiseOperator()
{
	using UL = unsigned long;

	StartOneTest( "移位运算符" );
	// 1、满足【左结合律】；
	// 2、移出边界的会被【舍弃】；
	// 3、参与运算的仅仅是符号左侧对象的【拷贝】，对象本身并没有变化；
	UL u1 = bitset< 8 >( "11111111" ).to_ulong(), u2;
	u2  = u1 >> 3 << 2 << 3;	// 先向右3位，3个1被舍弃；再向左2位，最后再向左3位
	cout << "u1 = " << u1 << ", u2 = " << u2 << endl;
	cout << "bits of u2 is : " << bitset< 10 >( u2 ).to_string() << endl;

	// 移位的复合运算符，直接移自己
	( ( u1 >>= 3 ) <<= 2 ) <<= 3;
	cout << "经过复合移位运算后，u1 = " << u1 << endl;

	// 位运算中，小整型自动提升成大整型
	bool bval = true;
	auto bshift = bval << 3;	// bool的true自动提升为int的1
	cout << "bool bitwise shift : " << bshift << endl;


	StartOneTest( "位运算的实际应用" );
	// 1、交换两个数；引入【01标尺】的概念帮助理解，两个数异或将产生一把标尺，标尺有两个功能：
	// 尺上刻度为1的位置，有异或取反的功能：即 1^1=0; 1^0=1;
	// 尺上刻度为0的位置，有异或保值的功能：即 0^1=1; 0^0=0;
	int a = -99, b = 250;
	a = a ^ b;	// 此时a变成了一把标尺，将a和b不同的地方标为1，相同的地方标为0
	b = a ^ b;	// 标尺与b异或，仅将b中和a不同的位置取反，当然得到了原来的a
	a = a ^ b;	// 同理，标尺与原来的a异或，可以得到原来的b
	cout << "a = " << a << ", b = " << b << endl;

	// 其他还真不太懂，后面学习一下再补，位棋盘？布尔数组？
}

void Test_OtherOperator()
{
	StartOneTest( "理解后缀自增、减运算符" );
	int a = 3, b;					// 后缀版的自增、减运算符，返回的是变量改变前的【副本】；
	cout << ( b = a++ ) << endl;	// b = a++ 相当于 int tmp = a; a = a + 1; b = tmp;
	cout << b << endl;				// 多一个临时变量的拷贝，所以除非必要，请用【前缀版】的；


	StartOneTest( "理解逗号运算符" );
	// 从左向右依次运算，逗号右侧的值作为运算的最终结果；
	// 逗号运算符的优先级在C++里是【最低的】，多加括号没坏处；
	int c = ( ++a, ( a + b ) );
	cout << "comma operator result: " << c << endl;


	StartOneTest( "sizeof运算符" );
	// 有两种格式：sizeof( type )、sizeof expr，第二种并不真正计算表达式的值；
	int i1 = 5;
	double dary[] = { 1.5, 2.4, 6.6 };

	size_t memberSize = sizeof TCPoint::x;	MCPP11	// 这个方式是新特性

	// 返回【整个】数组的字节数（哪怕是中间有'\0'侧char[]），不会把数组变成指针
	auto arySize = sizeof dary;

	// sizeof优先级【高于】数学运算符，所以复杂表达式注意【加括号】，否则只算第一个对象
	auto sumSize = sizeof ( ++i1 + dary[ 1 ] );

	// 结构体字节对齐：基本原则是结构体的大小是它里面最大数据类型的整数倍；
	// 其次不同编译器（操作系统）字节的默认对齐（windows好像是按8字节对齐）也不同，
	// 然后变量声明的前后顺序也影响结构体的大小，一般从大到小声明，会比较节省空间；
	// 所以不要靠自己臆测某个结构体的大小，用sizeof更有保障；

//#pragma pack( push )	// VC++里可以这样改变某一作用域里结构体的对齐方式
//#pragma pack( 1 )
	struct _IDPoint
	{
		double X;
		double Y;
		unsigned short ID;
		char Type;
	};
//#pragma pack( pop )

	auto pntSize = sizeof( _IDPoint );

	cout << "size of member variable:\t" << memberSize << endl 
		<< "size of array:\t\t\t" << arySize << endl 
		<< "size of int + double:\t\t" << sumSize << endl 
		<< "size of _IDPoint:\t\t" << pntSize << endl;
}

void Test_ImplicitConversion()
{
	StartOneTest( "隐式类型转换——算术转换" );
	double da = 3.5, db;
	int ia = 3, ix = ia + da;
	cout << "1、整型、浮点混合的算式，整型首先转为对应的浮点型: " << ix << endl;

	// 小整形：bool, char, signed char, unsigned char, short, and unsigned short
	// 首先提升到大整型int（如果数值范围够的话），或着unsigned int；
	// 如果算式里有浮点，转换顺序是：小整形先转为int，然后int再转为浮点；
	bool ba = true, bb = false;
	char ca = 'A';	// 'A'的ASCII码是65
	
	db = ba + ia + bb + da + ca;	// 1 + 3 + 0 + 3.5 + 65
	cout << "2、小整型提升：" << db << endl;

	// 即使算式里并没有大整型，也会自动提升
	short sa = 100;
	auto del1 = sa + ca;
	PrintTypeName( del1 );
	cout << "3、纯小整型的算式结果：" << del1 << endl;


	StartOneTest( "隐式类型转换——数组与指针" );
	// 多数情况下，数组名都自动转换为第一个元素的指针
	int iary[ 10 ] = {};
	PrintTypeName( ( iary + 1 ) );


	StartOneTest( "隐式类型转换——通用指针" );
	// 1、0和nullptr可以转为任意指针类型；
	///2、任意指针可以与void *或const void *互转；
	double dc = 5, *pb = 0;
	TCPoint *ppnt = nullptr;
	void *pv1 = &dc;
	PrintTypeName( pb );
	PrintTypeName( ppnt );
	PrintTypeName( pv1 );


	StartOneTest( "隐式类型转换——转到布尔" );
	// 算术、指针类型都可以自动按布尔处理，非0为真，0为假
	if( dc )
		cout << "dc is nonzero" << endl;
	if( pv1 )
		cout << "pv1 is not null" << endl;


	StartOneTest( "隐式类型转换——转到const" );
	// 任何非const的东西都可以转为const（这句话应该没问题吧？）


	StartOneTest( "隐式类型转换——类类型" );	// 这就要看类自己如何处理了
	TCPoint pnt1 = { 1, 2 };
	Coordinate coord1 = { 120, 31 };	// Coordinate内部实现了向TCPoint的转换方法
	TCPoint sumpnt = pnt1 + coord1;		// coord1首先隐式转换为TCPoint对象，再进行求和
	cout << "TCPoint + Coordinate : " << sumpnt << endl;
}

void Test_ExplicitConversion()
{
	StartOneTest( "简单粗暴的static_cast" );
	// static_cast只作编译时类型检查（dynamic_cast是运行时检查），而运行时哪怕不合理，
	// 结果也不为空，需要程序员保证转换过程是合法的，例如父子类的下行转换；
	TCMan man{};
	TCPerson *person = &man;
	TCWomen *lady = static_cast< TCWomen* >( person );
	if( lady )
		cout << "强制转换为女士结果并不为空" << endl;
	// 此时若调用lady->HandbagBrand()程序直接崩溃

	// 合理的转换示例：
	void *vp = person;
	TCMan *pMan = static_cast< TCMan* >( vp );
	cout << "男人的胡须长度为：" << pMan->BeardLength() << endl;
	

	StartOneTest( "const_cast" );
	cout << "const_cast的作用仅仅是去掉只读，不能改变数据的基本类型" << endl;


	StartOneTest( "更加粗暴的reinterpret_cast" );
	// 我的理解是将原有的变量内存地址，换一种指定数据类型的解释方法；
	// 感觉这个在内存池中有些用，反序列化数据时，对一块内存的解释方式是关键；

	// 假设一个100字节的内存池，pMem是位置游标
	char myMemPool[ 100 ] = {}, *pMem = myMemPool;

	// 首先在池里存一个double和一个int
	double dval = 9.9;
	int ival = -125;
	memcpy( pMem, &dval, sizeof( double ) );
	memcpy( pMem + sizeof( double ), &ival, sizeof( int ) );

	// 现在用reinterpret_cast来解析内存池里的数据
	double *dget = reinterpret_cast< double* >( pMem );
	cout << "double value from memory pool :\t" << *dget << endl;

	int *iget = reinterpret_cast< int* >( pMem + sizeof( double ) );
	cout << "int value from memory pool :\t" << *iget << endl;
}

void Test_ExpressionNew()
{
	StartOneTest( "特殊计算的结果符号" );
	int a = -5, b = 3, c = 7;
	cout << "a / b = " << ( a / b ) <<	MCPP11	// C++11算术标准，商一律【向0取整】
		";\ta % b = " << ( a % b ) << 	MCPP11	// C++11算术标准，余数的符号和【分子】保持一致
		";\tc % a = " << ( c % a ) << endl;

	StartOneTest( "初始化列表（花括号）可以作为右值" );
	TCPoint pnt1{};
	pnt1 = { 1.1, 3.3 };	MCPP11
	cout << pnt1 << endl;

	vector< int > vec( 3, 1 );
	vec = { 1, 2, 3, 4, 5 };
	for( auto &i : vec )
		cout << i << " ";
	cout << endl;
}

ChapterBase* Chapter04Init()
{
	ChapterBase *ch04 = new ChapterBase( "第四章 表达式" );
	ch04->AddSection( 1, "基本概念", &Test_ExpressionBase );
	ch04->AddSection( 2, "位运算", &Test_BitwiseOperator );
	ch04->AddSection( 3, "其他运算符", &Test_OtherOperator );
	ch04->AddSection( 4, "隐式类型转换", &Test_ImplicitConversion );
	ch04->AddSection( 5, "显式类型转换", &Test_ExplicitConversion );
	ch04->AddSection( 6, "新特性", &Test_ExpressionNew );
	return ch04;
}