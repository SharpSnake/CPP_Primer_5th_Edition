#include "Chapter07.h"
#include "TestBase.h"


using namespace std;

// 普通静态成员的初始方法，不能在类内初始化，一般在cpp里
JuniorClass::ClassSize JuniorClass::MaxNumberOfStudents = 100;

// constexpr静态成员最好也在外部定义一下，虽然不强求，但这是个好习惯
constexpr size_t JuniorClass::NumberOfLesson;


void Test_UnderstandingClass()
{
	StartOneTest( "深度理解类" );

	CONSOLE_COLOR_AUTO_RESET;
	SetConsoleTextColor( CmdColor_Yellow );

	cout << "1、成员函数的实现原理：可以理解为C++并没有成员函数，只有结构体和全局函数。编译器\r\n"
		"隐式地向函数传递一个名为this的参数，这个this就是对象的指针。成员函数体内对成员"
		"的调用，其实都是隐式的通过this->来调用；" << endl2;
	
	cout << "2、const成员函数，本质上是对this的修饰；" << endl2;
	
	cout << "3、类的编译步骤：（1）成员声明；（2）函数体；\r\n"
		"这样成员（类型成员除外）的声明顺序无所谓先后，函数体内调用成员时编译器都已知了；" << endl2;
	
	cout << "4、初始值列表中成员用实参直接初始化，一个误区是经常在构造函数体对成员进行赋值，\r\n"
		"相当于成员先默认初始化再赋值，从性能上讲多了一步拷贝，所以推荐使用初始值列表；" << endl2;
	
	cout << "5、聚合类4要素：\r\n（1）所有成员是public；\r\n（2）没有任何构造函数；\r\n"
		"（3）没有类内初始值；\r\n（4）没有基类和虚函数；\r\n可用列表初始化，注意成员顺序" << endl2;
}

void Test_Constructor()
{
	StartOneTest( "（合成）默认构造函数" );
	// 如果类没有任何构造，编译器会生成一个“合成默认构造”；或者用default关键字
	// 显式声明一个默认构造；成员执行【默认初始化】，没有类内初始值的成员可能会未定义；
	Student s1;	// 注意：若想使用默认构造，变量后面【不需要加括号】
	cout << s1 << endl;

	StartOneTest( "被构造忽略的成员" );
	// 构造中没有初始化的成员，将使用类内初始值，否则执行默认初始化，可能导致未定义；
	Student s2( "小明" );	// 这个构造没有初始化年龄
	cout << s2 << endl;

	StartOneTest( "所有参数都有默认实参=默认构造" );
	JuniorClass jc;
	cout << jc << endl;

	StartOneTest( "委托构造函数-Delegating Constructor" );
	Student s3( "李磊", 15 );
	cout << s3 << endl;


	StartOneTest( "构造函数与隐式类型转换" );
	Student s4, s5, s6;
	TCPerson p1( 14, "小强" ), p2( 13, "小亮" );

	s4 = static_cast< Student >( string( "韩梅梅" ) );
	s5 = p1;
	s6.Swap( p2 );
	cout << s4 << endl2 << s5 << endl2 << s6 << endl;
}

void Test_MemFunction()
{
	StartOneTest( "inline成员函数" );
	// 类内或类外定义的成员函数都可以声明inline，前提是函数体和类在一个文件中（一般都在头文件中）
	// inline函数调用没有性能消耗，推荐将一些内容重复的代码抽象成【小的inline函数】，易于维护
	JuniorClass t1( "三年二班" );
	t1.Add( { "小明", 12, 10 } ).Add( { "小芳", 13, 7 } );
	cout << t1 << endl;
	cout << endl << t1.Get( 1 ) << endl;

	StartOneTest( "同名const修饰算重载" );
	Student &s1 = t1.Get( 0, true );
	const Student &s2 = t1.Get( 1, true );	// 老师不是const的，所以仍然调用Get的非const重载
	
	const JuniorClass &rt1 = t1;
	const Student &rs2 = rt1.Get( 1, true );
}

// 字面值类型也可以是全局的
constexpr Placemark HuangHeLou( 119, 32, 90, "黄鹤楼" );

void Test_Features()
{
	StartOneTest( "类型成员" );
	JuniorClass t1( "三年二班", 50 );
	JuniorClass::ClassSize numStudents = t1.NumberOfStudents();
	cout << "三年二班学生人数：" <<  numStudents << endl;


	StartOneTest( "可变数据成员" );
	// 关键字mutable修饰的成员变量，即使在const成员函数中也可更改
	// mutable的另外一个用处是，Lambda表达式中使【引用访问的外部变量】可更改；
	const JuniorClass t2( "一年三班", 60 );
	t2.Get( 3, true ); t2.Get( 9, true ); t2.Get( 45, true );
	cout << "张老师班学生被访问次数：" << t2.VisitTime() << endl;


	StartOneTest( "友元函数" );
	Student s1( "小王", 12, 5 );
	cout << s1 << endl;	// 标准输出是Student的友元函数

	JuniorClass j1, j2;
	j1.Add( s1 ).Add( { "小明", 12, 10 } );
	j2.Add( { "小芳", 13, 7 } ).Add( { "小张", 11, 5 } );
	cout << "友元类访问私有成员:\t" << j1.StudentName( 1 ) << endl;
	
	School sh1;		// 学校不能越过班级，直接访问学生的信息
	sh1.Add( j1 ).Add( j2 );
	cout << "友元没有传递性:\t\t" << sh1.StudentName( 1, 0 ) << endl;

	unsigned oldID = sh1.ModifyStudentID( 1, 1, 19 );
	cout << "友元成员函数示例：" << endl << "OldID: " << oldID << endl
		<< "NewID:" << sh1.Get( 1 ).StudentID( 1 ) << endl;


	StartOneTest( "静态成员" );
	cout << j2 << endl2;

	j2.Resize();	// 静态成员作为默认实参
	cout << j2 << endl;

	StartOneTest( "字面值类型" );
	constexpr Placemark TianAnMen;
	constexpr Placemark WangFuJing( 120, 35, 250, "王府井" );
	cout << TianAnMen << endl2 << WangFuJing << endl2 << HuangHeLou << endl;
}

void Chapter07Init()
{
	auto ch07 = ChapterBase::AddChapter( 7, "第七章 类基础" );
	ch07->AddSection( 1, "深度理解类", &Test_UnderstandingClass );
	ch07->AddSection( 2, "构造函数", &Test_Constructor );
	ch07->AddSection( 3, "成员函数", &Test_MemFunction );
	ch07->AddSection( 4, "类的一些重要特性", &Test_Features );
}
static int _Init = ( Chapter07Init(), 0 );

/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
ostream& operator <<( ostream &o, const Student &obj )
{
	o << "Name:\t" << obj.m_Name << endl
		<< "Age:\t" << obj.m_Age << endl
		<< "ID:\t" << obj.m_ID;
	return o;
}