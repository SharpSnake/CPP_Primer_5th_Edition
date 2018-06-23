// =============================================================================
// 
// Some helper functions or structs.
// 						              - WuYu, 2018.06.
// =============================================================================
#ifndef TESTHELPER_H	// ��Ԥ����������ʵ��ͷ�ļ�������
#define TESTHELPER_H	// ��ĳ��cpp�д�h�ļ����ܱ�ֱ�ӻ��Ӱ������
						// ������Ŀ���Ƿ�ֹ���������ظ�����ı������

#define MCPP11   // ��עC++11������

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

	// ��ƽ��
	void Offset( double xoff, double yoff )
	{
		x += xoff;
		y += yoff;
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
private:
	string m_name = "�°��ϲ�";	MCPP11	// ���ڳ�ʼֵ
	unsigned int m_age = 4000;

public:
	TCPerson() {}
	~TCPerson() {}

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

#endif // !TESTHELPER_H
