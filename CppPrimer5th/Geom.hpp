#ifndef GEOM_HPP
#define GEOM_HPP

#include <iostream>
#include <cmath>


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

// 两个整数的中间值
constexpr int MidPoint( int i1, int i2 )
{
	return ( i1 + i2 ) / 2;
}


#endif // !GEOM_HPP