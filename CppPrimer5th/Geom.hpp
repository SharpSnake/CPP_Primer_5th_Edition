#ifndef GEOM_HPP
#define GEOM_HPP

#include <iostream>
#include <cmath>
#include <string>
#include <vector>
#include <algorithm>
#include <functional>	// hash<>
#include <initializer_list>
#include <boost/functional/hash.hpp>	// hash_combine



// 测试用类：二维点，是聚合类（Aggregate Class）
class TCPoint
{
public:
	double x;
	double y;

	explicit operator bool() const { return x != 0 || y != 0; }

	// 向double的隐式转换，意思是点到原点的距离
	explicit operator double() const
	{
		return sqrt( x * x + y * y );
	}

	// 平移
	void Offset( double xoff, double yoff )
	{
		x += xoff;
		y += yoff;
	}

	// 赋值运算符【必须是成员】
	TCPoint& operator=( const TCPoint &pnt )
	{
		x = pnt.x;
		y = pnt.y;
		return *this;
	}

	// 复合赋值不强求是成员，但一般最好是成员函数
	TCPoint& operator+=( const TCPoint &pnt )
	{
		x += pnt.x;
		y += pnt.y;
		return *this;
	}

	// 求和运算符，定义为成员，要求调用对象必须是一个TCPoint对象
	// 若运算符可以用于混合类型表达式，最好定义为【非成员】
	TCPoint operator+( const TCPoint &pnt ) const
	{
		return { this->x + pnt.x, this->y + pnt.y };
	}

	// 输出应该尽量【减少格式化操作】，比如换行，让用户决定格式细节
	friend std::ostream& operator<<( std::ostream &ostm, const TCPoint &obj )
	{
		return ostm << "x：" << obj.x << "， y：" << obj.y;
	}

	// 要处理输入失败的情况，并适当的标注流的失败状态
	friend std::istream& operator>>( std::istream &istm, TCPoint &obj )
	{
		istm >> obj.x >> obj.y;
		if( !istm )
		{
			obj = TCPoint();
			istm.setstate( istm.failbit );
		}
		return istm;
	}
};

// 求差运算符，非成员，可用于混合型表达式
inline TCPoint operator-( const TCPoint &l, const TCPoint &r )
{
	return { l.x - r.x, l.y - r.y };
}


// 测试用结构体：二维点，是聚合类型
struct TSPoint
{
	double x;
	double y;

	operator TCPoint() { return { x, y }; }

	friend std::ostream & operator <<( std::ostream &ostm, const TSPoint &obj )
	{
		return ostm << "x：" << obj.x << "， y：" << obj.y;
	}
};


// 地理坐标点
class Coordinate
{
	friend struct hash< Coordinate >;

public:
	Coordinate() = default;

	constexpr Coordinate( double lon, double lat ) 
		: m_Longitude( lon ), m_Latitude( lat ) {}	MCPP11

	Coordinate( const TSPoint &pnt ) = delete;	MCPP11

	constexpr Coordinate( const Coordinate &right )
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

	bool operator==( const Coordinate &right ) const
	{
		return right.m_Longitude == m_Longitude && right.m_Latitude == m_Latitude;
	}

	bool operator!=( const Coordinate &right ) const	{ return !( *this == right ); }

	// 地理坐标向一般点的转换，支持隐式转换
	operator TCPoint() const { return { m_Longitude, m_Latitude }; }

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


// 特例化标准库中对Coordinate类型的哈希计算器
namespace std	{
template<>
struct hash< Coordinate >
{
	typedef Coordinate argument_type;
	typedef std::size_t result_type;

	std::size_t operator()( const Coordinate &coord ) const;
};

// 全特化成员外部定义，不需要template<>开头
inline std::size_t hash< Coordinate >::operator()( const Coordinate &coord ) const
{
	// 多个成员的哈希值拼接用boost::hash_combine靠谱一些
	size_t seed = 0;
	boost::hash_combine( seed, coord.m_Longitude );
	boost::hash_combine( seed, coord.m_Latitude );
	return seed;
}
}	// std

// 两个整数的中间值
constexpr int MidPoint( int i1, int i2 )
{
	return ( i1 + i2 ) / 2;
}


// 类模板的前置声明，很简略
template< typename > class Vector2;

// 二维地理范围
template< typename T = double >
class Extent2
{
	friend class TCPoint;		// 非模板友元
	friend class Vector2< T >;	// 一般模板友元，仅当模参相同可以访问

public:
	Extent2() = default;

	template< typename U >
	Extent2( const Vector2< U > &southwest, const Vector2< U > &northeast );

	T West() const { return m_West; }
	T East() const { return m_East; }
	T South() const { return m_South; }
	T North() const { return m_North; }
	T Width() const { return m_East - m_West; }

	friend std::ostream& operator<<( std::ostream &ostm, const Extent2 &obj ) {
		return ostm << "West：" << obj.m_West << "， East：" << obj.m_East 
			<< "， South：" << obj.m_South << "， North：" << obj.m_North;
	}

private:
	T m_West;
	T m_East;
	T m_South;
	T m_North;
};

// 针对int的特化版本，范围宽度加1的补偿值
template<>
int Extent2< int >::Width() const { return m_East - m_West + 1; }


// 二维向量，与Extent2为一对一好友
template< typename T = double >
class Vector2
{
	// 【通用模板友元】，意味着Extent2的所有实例，都可以完全访问Vector2
	template< typename U > friend class Extent2;
	
	// C++11——模参友元，T可能是class、struct、或函数，所以T前什么都不用交代
	friend T;

public:
	Vector2() = default;
	Vector2( T x, T y ) : X( x ), Y( y ) { ++TotalCount; }
	Vector2( const Extent2< T > &ext )	// 用范围的中心初始化
		: X( ( ext.m_West + ext.m_East ) / 2 ), 
		Y( ( ext.m_South + ext.m_North ) / 2 ) { ++TotalCount; }

	T GetX() const { return X; }
	T GetY() const { return Y; }

	// 向量平移
	template< typename S >
	void Move( S, S );

	friend std::ostream & operator <<( std::ostream &ostm, const Vector2 &obj ) {
		return ostm << "x：" << obj.X << "， y：" << obj.Y;
	}

	static unsigned GetTotalCount() { return TotalCount; } ;
	
private:
	T X;
	T Y;

	static unsigned TotalCount;		// 记录程序中创建对象的总数
};

template< typename T >
template< typename S >
void Vector2< T >::Move( S xdiff, S ydiff )
{
	X += xdiff;
	Y += ydiff;
}


// 距离，可以当做double使用，它是Vector2< Distance >的友元
struct Distance
{
	double Value;
	
	Distance() = default;
	Distance( double val ) : Value( val ) {}
	operator double() const { return Value; }

	// 用两个向量的差，初始化距离类
	Distance( const Vector2< Distance > &v1, const Vector2< Distance > &v2 )
		: Value( sqrt( pow( v2.X - v1.X, 2 ) + pow( v2.Y - v1.Y, 2 ) ) ) {}

	template< typename T >
	void operator +=( const T &right ) { Value += right; }

	template< typename T >
	void operator -=( const T & );
};

template< typename T >
void Distance::operator -=( const T &right ) { Value -= right; }

// just for testing
struct DistanceEx : Distance
{
	using Distance::Distance;
};


template< typename T >
template< typename U >
Extent2< T >::Extent2( const Vector2< U > &southwest, const Vector2< U > &northeast )
{
	m_West = southwest.X;
	m_East = northeast.X;
	m_South = southwest.Y;
	m_North = northeast.Y;
}


// 简单的数组封装
template< typename T, unsigned N >
struct MArray
{
	MArray( std::initializer_list< T > list )	{
		std::copy( list.begin(), list.end(), m_ary );
	}

	friend std::ostream & operator <<( std::ostream &ostm, const MArray &obj )
	{
		for( auto &i : obj.m_ary )
			ostm << i << "\t";
		return ostm;
	}

	T m_ary[ N ];
};

// string类型数组的偏特化版本
template< unsigned N >
struct MArray< std::string, N >
{
	MArray( std::initializer_list< std::string > list )	{
		std::copy( list.begin(), list.end(), m_ary );
	}

	friend std::ostream & operator <<( std::ostream &ostm, const MArray &obj )
	{
		for( auto &i : obj.m_ary )
			ostm << i << std::endl;	// string的特化版本是每个元素都换行
		return ostm;
	}

	std::string m_ary[ N ];
};

// 数组元素必须是一个vector的偏特化版本，相当于二维数组
template< typename T, unsigned N >
struct MArray< std::vector< T >, N >
{
	MArray( std::initializer_list< std::vector< T > > list )	{
		std::copy( list.begin(), list.end(), m_ary );
	}

	friend std::ostream & operator <<( std::ostream &ostm, const MArray &obj )
	{
		for( auto &i : obj.m_ary )
		{
			for( auto &j : i )	// 这个版本，每一行vector中的元素间用空格间隔
				ostm << j << "\t";
			ostm << std::endl;	// 打印完一个vector换行，打印下一个vector
		}
		return ostm;
	}

	std::vector< T > m_ary[ N ];
};

#endif // !GEOM_HPP