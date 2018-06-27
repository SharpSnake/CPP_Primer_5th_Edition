// =============================================================================
// 
// Some helper functions or structs.
// 						              - WuYu, 2018.06.
// =============================================================================
#ifndef TESTHELPER_H	// 用预处理器变量实现头文件保护符
#define TESTHELPER_H	// 在某个cpp中此h文件可能被直接或间接包含多次
						// 这样做目的是防止出现类型重复定义的编译错误

#define MCPP11   // 标注C++11新特性

#include <cstdlib>
#include <ctime>
#include <boost/type_index.hpp>	// this library of boost is head-only

#include "ConsoleUtility.h"


// 头文件中最好不要用using声明，当此头文件被包含到其他cpp时，
// 可能会引发意想不到的名字冲突，所以最好由cpp自己决定使用哪些using声明
using namespace std;


// 打印一个变量的确切类型名称，std的typeid( var ).name()不完整，所以借助boost
#define PrintTypeName( var )\
{\
	cout << "Type of \"" << #var << "\"\t:\t";\
	cout << boost::typeindex::type_id_with_cvr< decltype( var ) >().pretty_name() << endl;\
}


class QuadraticPoly;
class TCPerson;
class Coordinate;


// 测试用结构体：二维点
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

// 测试用类：二维点
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
	TCPerson() {}
	virtual ~TCPerson() {}

public:
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
	constexpr Coordinate( double lon, double lat ) 
		: m_Longitude( lon ), m_Latitude( lat ) {}	MCPP11

	constexpr double Longitude() const { return m_Longitude; }
	constexpr double Latitude() const { return m_Latitude; }

	/* C++14 constexpr */ void SetLongitude( double lon ) { m_Longitude = lon; }
	/* C++14 constexpr */ void SetLatitude( double lat ) { m_Latitude = lat; }

	friend ostream & operator <<( ostream &ostm, const Coordinate &obj )
	{
		return ostm << "( " << obj.m_Longitude << ", " << obj.m_Latitude << " )";
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

// 生成某个范围内的随机整数，不做b>a的检测
inline int Randi( const int &a, const int &b )
{
	return a + std::rand() % ( b - a + 1 /* this is the range of [ a, b ]*/ );
}

// 测试的初始化
inline void TestInit( void )
{
	// 初始化随机数发生器
	std::srand( std::time( nullptr ) );

	// 初始化控制台
	ConsoleInit();
}

#endif // !TESTHELPER_H
