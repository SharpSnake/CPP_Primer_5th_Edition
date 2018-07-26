#ifndef GEOM_HPP
#define GEOM_HPP

#include <iostream>
#include <cmath>


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

// �����������м�ֵ
constexpr int MidPoint( int i1, int i2 )
{
	return ( i1 + i2 ) / 2;
}


#endif // !GEOM_HPP