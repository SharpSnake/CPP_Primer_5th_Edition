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



// �������ࣺ��ά�㣬�Ǿۺ��ࣨAggregate Class��
class TCPoint
{
public:
	double x;
	double y;

	explicit operator bool() const { return x != 0 || y != 0; }

	// ��double����ʽת������˼�ǵ㵽ԭ��ľ���
	explicit operator double() const
	{
		return sqrt( x * x + y * y );
	}

	// ƽ��
	void Offset( double xoff, double yoff )
	{
		x += xoff;
		y += yoff;
	}

	// ��ֵ������������ǳ�Ա��
	TCPoint& operator=( const TCPoint &pnt )
	{
		x = pnt.x;
		y = pnt.y;
		return *this;
	}

	// ���ϸ�ֵ��ǿ���ǳ�Ա����һ������ǳ�Ա����
	TCPoint& operator+=( const TCPoint &pnt )
	{
		x += pnt.x;
		y += pnt.y;
		return *this;
	}

	// ��������������Ϊ��Ա��Ҫ����ö��������һ��TCPoint����
	// ��������������ڻ�����ͱ��ʽ����ö���Ϊ���ǳ�Ա��
	TCPoint operator+( const TCPoint &pnt ) const
	{
		return { this->x + pnt.x, this->y + pnt.y };
	}

	// ���Ӧ�þ��������ٸ�ʽ�������������绻�У����û�������ʽϸ��
	friend std::ostream& operator<<( std::ostream &ostm, const TCPoint &obj )
	{
		return ostm << "x��" << obj.x << "�� y��" << obj.y;
	}

	// Ҫ��������ʧ�ܵ���������ʵ��ı�ע����ʧ��״̬
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

// �����������ǳ�Ա�������ڻ���ͱ��ʽ
inline TCPoint operator-( const TCPoint &l, const TCPoint &r )
{
	return { l.x - r.x, l.y - r.y };
}


// �����ýṹ�壺��ά�㣬�Ǿۺ�����
struct TSPoint
{
	double x;
	double y;

	operator TCPoint() { return { x, y }; }

	friend std::ostream & operator <<( std::ostream &ostm, const TSPoint &obj )
	{
		return ostm << "x��" << obj.x << "�� y��" << obj.y;
	}
};


// ���������
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

	// ����������һ����ת����֧����ʽת��
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
	// ��γ��
	double m_Longitude;
	double m_Latitude;
};

// ����������е�����
constexpr Coordinate MidPoint( const Coordinate &coord1, const Coordinate &coord2 )	MCPP11
{
	return { ( coord1.Longitude() + coord2.Longitude() ) / 2,
		( coord1.Latitude() + coord2.Latitude() ) / 2 };
}


// ��������׼���ж�Coordinate���͵Ĺ�ϣ������
namespace std	{
template<>
struct hash< Coordinate >
{
	typedef Coordinate argument_type;
	typedef std::size_t result_type;

	std::size_t operator()( const Coordinate &coord ) const;
};

// ȫ�ػ���Ա�ⲿ���壬����Ҫtemplate<>��ͷ
inline std::size_t hash< Coordinate >::operator()( const Coordinate &coord ) const
{
	// �����Ա�Ĺ�ϣֵƴ����boost::hash_combine����һЩ
	size_t seed = 0;
	boost::hash_combine( seed, coord.m_Longitude );
	boost::hash_combine( seed, coord.m_Latitude );
	return seed;
}
}	// std

// �����������м�ֵ
constexpr int MidPoint( int i1, int i2 )
{
	return ( i1 + i2 ) / 2;
}


// ��ģ���ǰ���������ܼ���
template< typename > class Vector2;

// ��ά����Χ
template< typename T = double >
class Extent2
{
	friend class TCPoint;		// ��ģ����Ԫ
	friend class Vector2< T >;	// һ��ģ����Ԫ������ģ����ͬ���Է���

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
		return ostm << "West��" << obj.m_West << "�� East��" << obj.m_East 
			<< "�� South��" << obj.m_South << "�� North��" << obj.m_North;
	}

private:
	T m_West;
	T m_East;
	T m_South;
	T m_North;
};

// ���int���ػ��汾����Χ��ȼ�1�Ĳ���ֵ
template<>
int Extent2< int >::Width() const { return m_East - m_West + 1; }


// ��ά��������Extent2Ϊһ��һ����
template< typename T = double >
class Vector2
{
	// ��ͨ��ģ����Ԫ������ζ��Extent2������ʵ������������ȫ����Vector2
	template< typename U > friend class Extent2;
	
	// C++11����ģ����Ԫ��T������class��struct������������Tǰʲô�����ý���
	friend T;

public:
	Vector2() = default;
	Vector2( T x, T y ) : X( x ), Y( y ) { ++TotalCount; }
	Vector2( const Extent2< T > &ext )	// �÷�Χ�����ĳ�ʼ��
		: X( ( ext.m_West + ext.m_East ) / 2 ), 
		Y( ( ext.m_South + ext.m_North ) / 2 ) { ++TotalCount; }

	T GetX() const { return X; }
	T GetY() const { return Y; }

	// ����ƽ��
	template< typename S >
	void Move( S, S );

	friend std::ostream & operator <<( std::ostream &ostm, const Vector2 &obj ) {
		return ostm << "x��" << obj.X << "�� y��" << obj.Y;
	}

	static unsigned GetTotalCount() { return TotalCount; } ;
	
private:
	T X;
	T Y;

	static unsigned TotalCount;		// ��¼�����д������������
};

template< typename T >
template< typename S >
void Vector2< T >::Move( S xdiff, S ydiff )
{
	X += xdiff;
	Y += ydiff;
}


// ���룬���Ե���doubleʹ�ã�����Vector2< Distance >����Ԫ
struct Distance
{
	double Value;
	
	Distance() = default;
	Distance( double val ) : Value( val ) {}
	operator double() const { return Value; }

	// �����������Ĳ��ʼ��������
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


// �򵥵������װ
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

// string���������ƫ�ػ��汾
template< unsigned N >
struct MArray< std::string, N >
{
	MArray( std::initializer_list< std::string > list )	{
		std::copy( list.begin(), list.end(), m_ary );
	}

	friend std::ostream & operator <<( std::ostream &ostm, const MArray &obj )
	{
		for( auto &i : obj.m_ary )
			ostm << i << std::endl;	// string���ػ��汾��ÿ��Ԫ�ض�����
		return ostm;
	}

	std::string m_ary[ N ];
};

// ����Ԫ�ر�����һ��vector��ƫ�ػ��汾���൱�ڶ�ά����
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
			for( auto &j : i )	// ����汾��ÿһ��vector�е�Ԫ�ؼ��ÿո���
				ostm << j << "\t";
			ostm << std::endl;	// ��ӡ��һ��vector���У���ӡ��һ��vector
		}
		return ostm;
	}

	std::vector< T > m_ary[ N ];
};

#endif // !GEOM_HPP