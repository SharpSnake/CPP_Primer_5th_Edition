// =============================================================================
// 
// Some helper functions or structs.
// 						              - WuYu, 2018.06.
// =============================================================================
#ifndef TESTHELPER_H	// 用预处理器变量实现头文件保护符
#define TESTHELPER_H	// 在某个cpp中此h文件可能被直接或间接包含多次
						// 这样做目的是防止出现类型重复定义的编译错误

#define MCPP11   // 标注C++11新特性


#include "ConsoleUtility.h"
#include "Functions.hpp"
#include "Geom.hpp"


// 头文件中最好不要用using声明，当此头文件被包含到其他cpp时，
// 可能会引发意想不到的名字冲突，所以最好由cpp自己决定使用哪些using声明
using namespace std;


// C++11枚举：编程语言
enum class ProgramLan : unsigned		MCPP11
{
	Lan_CPP			= 0x001,
	Lan_CSharp		= 0x002,
	Lan_Java		= 0x004,
	Lan_Matlab		= 0x008,
	Lan_Python		= Lan_Matlab << 1,
	Lan_All			= Lan_CPP | Lan_CSharp | Lan_Java | Lan_Matlab | Lan_Python
};


// 测试用类：二次多项式ax^2 + bx + c
class QuadraticPoly
{
public:
	double m_Coef[ 3 ];

public:
	// 求二次多项式的值
	double Polyval( const double &x ) const
	{
		return m_Coef[ 0 ] * x * x + m_Coef[ 1 ] * x + m_Coef[ 2 ];
	}

	friend ostream & operator <<( ostream &ostm, const QuadraticPoly &obj )
	{
		return ostm << obj.m_Coef[ 0 ] << " * x^2 + " << obj.m_Coef[ 1 ] 
			<< " * x + " << obj.m_Coef[ 2 ];
	}
};


// 字面值类型示例：地标
class Placemark		MCPP11
{
public:
	using ushort = unsigned short;

public:
	// 当default为constexpr时，【每个】成员必须有类内初始值
	constexpr Placemark() = default;

	Placemark( double x, double y, ushort h )
		: m_Coord( x, y ), m_Height( h ) {}

	// 至少有一个constexpr构造，且函数体必须为【空】，且【每个】成员都要初始化，少一个都不行
	constexpr Placemark( double x, double y, ushort h, const char *title )
		: m_Coord( x, y ), m_Height( h ), m_Title( title ) {}

	// 【不能】自定义析构函数

public:
	friend ostream& operator <<( ostream &o, const Placemark &p )
	{
		o   << "坐标：" << p.m_Coord << endl
			<< "海拔：" << p.m_Height << endl
			<< "名称：" << p.m_Title;
		return o;
	}
private:
	// 成员类型必须是内置类型，或者是其他自定义字面值类型；
	// 类内初始值必须是【常量表达式】，或者使用自己的【constexpr构造函数】
	Coordinate m_Coord{ 121, 36 };	// 类类型类内初始值只能用【列表初始化】
	ushort m_Height = Factorial( 5 );
	const char *m_Title = "天安门";
};


// 函数对象：求绝对值
template< typename T >
struct Op_Abs
{
	T operator()( const T &val ) const
	{
		return val > 0 ? val : -val;
	}

	void operator()( T &val ) const
	{
		if( val < 0 )
			val = -val;
	}
};


// 函数对象：与常数相乘
template< typename T >
struct Op_Times_N
{
	Op_Times_N( T n ) : m_N( n ) {}
	
	T operator()( const T &val ) const { return val * m_N; }
	void operator()( T &val ) const { val *= m_N; }

private:
	T m_N;
};

// 模仿标注库去引用的模板，利用偏特化实现
template< typename T >
struct RemoveRef	{
	typedef T type;
};

template< typename T >
struct RemoveRef< T& >	{	// 针对左值特化
	typedef T type;
};

template< typename T >
struct RemoveRef< T&& >	{	// 针对右值特化
	typedef T type;
};


/////////////////////////////////////////////////////////////////////////////////
inline void StartOneTest( const char *msg = nullptr MCPP11 )
{
	CONSOLE_COLOR_AUTO_RESET;

	SetConsoleTextColor( CmdColor_Red );
	cout << endl << "*********************************"
		"*********************************" << endl;

	SetConsoleTextColor( CmdColor_Green );
	if ( msg )
		cout << msg << endl;
}

// 测试的初始化
inline void HelperInit( void )
{
	FunctionsInit();
	ConsoleInit();
}

#endif // !TESTHELPER_H
