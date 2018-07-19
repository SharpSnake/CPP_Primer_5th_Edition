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


// 头文件中最好不要用using声明，当此头文件被包含到其他cpp时，
// 可能会引发意想不到的名字冲突，所以最好由cpp自己决定使用哪些using声明
using namespace std;


class QuadraticPoly;
class TCPerson;
class Coordinate;


// 编程语言
enum ProgramLan
{
	Lan_CPP			= 0x001,
	Lan_CSharp		= 0x002,
	Lan_Java		= 0x004,
	Lan_Matlab		= 0x008,
	Lan_Python		= 0x010,
	Lan_All			= Lan_CPP | Lan_CSharp | Lan_Java | Lan_Matlab | Lan_Python
};


// 测试用结构体：二维点，是聚合类
struct TSPoint
{
	double x;
	double y;

	friend ostream & operator <<( ostream &ostm, const TSPoint &obj )
	{
		return ostm << "x：" << obj.x << "， y：" << obj.y;

		// 若采用下面的实现，打印会自动换一换，但一般自定义类型不要主动换行好一些
		// return endl( ostm << "x：" << obj.x << "， y：" << obj.y );
	}
};


// 测试用类：二维点，是聚合类（Aggregate Class）
class TCPoint
{
public:
	double x;
	double y;

	// 平移
	void Offset( double xoff, double yoff )
	{
		x += xoff;
		y += yoff;
	}

	// 求和
	TCPoint operator +( const TCPoint &pnt ) const
	{
		return { this->x + pnt.x, this->y + pnt.y };
	}

	friend ostream & operator <<( ostream &ostm, const TCPoint &obj )
	{
		return ostm << "x：" << obj.x << "， y：" << obj.y;
	}
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


// 测试用类：人
class TCPerson
{
protected:
	string m_name = "穆阿迪布";	MCPP11	// 类内初始值
	unsigned int m_age = 4000;

public:
	TCPerson() = default;
	TCPerson( const string &name, unsigned age )
		: m_name( name ), m_age( age ) {}

	virtual ~TCPerson() {}

public:
	string Name() const { return m_name; }
	unsigned Age() const { return m_age; }

	friend istream & operator >>( istream &istm, TCPerson &obj )
	{
		return istm >> obj.m_name >> obj.m_age;
	}

	friend ostream & operator <<( ostream &ostm, const TCPerson &obj )
	{
		return ostm << "姓名：" << obj.m_name << "， 年龄：" << obj.m_age;
	}
};


// 测试用类：男人
class TCMan : public TCPerson
{
protected:
	// 胡须长度，单位cm
	double m_BeardLength = 10;

public:
	TCMan() {}
	~TCMan() {}

	double BeardLength() const { return m_BeardLength; }
};


// 测试用类：女人
class TCWomen : public TCPerson
{
protected:
	// 手提包品牌
	string m_HandbagBrand = "LV";
public:
	TCWomen() {}
	~TCWomen() {}

	string HandbagBrand() const { return m_HandbagBrand; }
};


// 地理坐标点
class Coordinate
{
public:
	Coordinate() = default;

	constexpr Coordinate( double lon, double lat ) 
		: m_Longitude( lon ), m_Latitude( lat ) {}	MCPP11

	Coordinate( const TSPoint &pnt ) = delete;	MCPP11

	Coordinate( const Coordinate &right )
		: m_Longitude( right.m_Longitude ), m_Latitude( right.m_Latitude ) {}

	constexpr double Longitude() const { return m_Longitude; }
	constexpr double Latitude() const { return m_Latitude; }

	/* C++14 constexpr */ void SetLongitude( double lon ) { m_Longitude = lon; }
	/* C++14 constexpr */ void SetLatitude( double lat ) { m_Latitude = lat; }

	Coordinate& operator =( const Coordinate &right )
	{
		m_Longitude = right.m_Longitude;
		m_Latitude = right.m_Latitude;
		return *this;
	}

	explicit operator bool() const
	{
		return m_Longitude != 0 || m_Latitude != 0;
	}

	friend ostream & operator <<( ostream &ostm, const Coordinate &obj )
	{
		return ostm << "( " << obj.m_Longitude << ", " << obj.m_Latitude << " )";
	}

	template< unsigned N >
	friend ostream & operator <<( ostream &ostm, const Coordinate ( &cary )[ N ] )
	{
		for( auto &i : cary )
			ostm << i << endl;
		return ostm;
	}

	// 地理坐标向一般点的转换
	operator TCPoint()
	{
		return { m_Longitude, m_Latitude };
	}

private:
	// 经纬度
	double m_Longitude;
	double m_Latitude;
};

// 计算两点的中点坐标
constexpr Coordinate MidPoint( const Coordinate &coord1, const Coordinate &coord2 )	MCPP11
{
	return { ( coord1.Longitude() + coord2.Longitude() ) / 2,
		( coord1.Latitude() + coord2.Latitude() ) / 2 };
}


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
