// =============================================================================
// 
// Some helper functions or structs.
// 						              - WuYu, 2018.06.
// =============================================================================
#ifndef TESTHELPER_H	// ��Ԥ����������ʵ��ͷ�ļ�������
#define TESTHELPER_H	// ��ĳ��cpp�д�h�ļ����ܱ�ֱ�ӻ��Ӱ������
						// ������Ŀ���Ƿ�ֹ���������ظ�����ı������

#define MCPP11   // ��עC++11������

#include <cstdlib>
#include <ctime>
#include <boost/type_index.hpp>	// this library of boost is head-only

#include "ConsoleUtility.h"


// ͷ�ļ�����ò�Ҫ��using����������ͷ�ļ�������������cppʱ��
// ���ܻ��������벻�������ֳ�ͻ�����������cpp�Լ�����ʹ����Щusing����
using namespace std;


// ��ӡһ��������ȷ���������ƣ�std��typeid( var ).name()�����������Խ���boost
#define PrintTypeName( var )\
{\
	cout << "Type of \"" << #var << "\"\t:\t";\
	cout << boost::typeindex::type_id_with_cvr< decltype( var ) >().pretty_name() << endl;\
}


class QuadraticPoly;
class TCPerson;
class Coordinate;


// �����ýṹ�壺��ά��
struct TSPoint
{
	double x;
	double y;

	friend ostream & operator <<( ostream &ostm, const TSPoint &obj )
	{
		return ostm << "x��" << obj.x << "�� y��" << obj.y;

		// �����������ʵ�֣���ӡ���Զ���һ������һ���Զ������Ͳ�Ҫ�������к�һЩ
		// return endl( ostm << "x��" << obj.x << "�� y��" << obj.y );
	}
};

// �������ࣺ��ά��
class TCPoint
{
public:
	double x;
	double y;

	// ƽ��
	void Offset( double xoff, double yoff )
	{
		x += xoff;
		y += yoff;
	}

	// ���
	TCPoint operator +( const TCPoint &pnt ) const
	{
		return { this->x + pnt.x, this->y + pnt.y };
	}

	friend ostream & operator <<( ostream &ostm, const TCPoint &obj )
	{
		return ostm << "x��" << obj.x << "�� y��" << obj.y;
	}
};

// �������ࣺ���ζ���ʽax^2 + bx + c
class QuadraticPoly
{
public:
	double m_Coef[ 3 ];

public:
	// ����ζ���ʽ��ֵ
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


// �������ࣺ��
class TCPerson
{
protected:
	string m_name = "�°��ϲ�";	MCPP11	// ���ڳ�ʼֵ
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
		return ostm << "������" << obj.m_name << "�� ���䣺" << obj.m_age;
	}
};


// �������ࣺ����
class TCMan : public TCPerson
{
protected:
	// ���볤�ȣ���λcm
	double m_BeardLength = 10;

public:
	TCMan() {}
	~TCMan() {}

	double BeardLength() const { return m_BeardLength; }
};


// �������ࣺŮ��
class TCWomen : public TCPerson
{
protected:
	// �����Ʒ��
	string m_HandbagBrand = "LV";
public:
	TCWomen() {}
	~TCWomen() {}

	string HandbagBrand() const { return m_HandbagBrand; }
};


// ���������
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

	// ����������һ����ת��
	operator TCPoint()
	{
		return { m_Longitude, m_Latitude };
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

// ����ĳ����Χ�ڵ��������������b>a�ļ��
inline int Randi( const int &a, const int &b )
{
	return a + std::rand() % ( b - a + 1 /* this is the range of [ a, b ]*/ );
}

// ���Եĳ�ʼ��
inline void TestInit( void )
{
	// ��ʼ�������������
	std::srand( std::time( nullptr ) );

	// ��ʼ������̨
	ConsoleInit();
}

#endif // !TESTHELPER_H
