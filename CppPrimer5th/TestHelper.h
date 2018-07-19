// =============================================================================
// 
// Some helper functions or structs.
// 						              - WuYu, 2018.06.
// =============================================================================
#ifndef TESTHELPER_H	// ��Ԥ����������ʵ��ͷ�ļ�������
#define TESTHELPER_H	// ��ĳ��cpp�д�h�ļ����ܱ�ֱ�ӻ��Ӱ������
						// ������Ŀ���Ƿ�ֹ���������ظ�����ı������

#define MCPP11   // ��עC++11������


#include "ConsoleUtility.h"
#include "Functions.hpp"


// ͷ�ļ�����ò�Ҫ��using����������ͷ�ļ�������������cppʱ��
// ���ܻ��������벻�������ֳ�ͻ�����������cpp�Լ�����ʹ����Щusing����
using namespace std;


class QuadraticPoly;
class TCPerson;
class Coordinate;


// �������
enum ProgramLan
{
	Lan_CPP			= 0x001,
	Lan_CSharp		= 0x002,
	Lan_Java		= 0x004,
	Lan_Matlab		= 0x008,
	Lan_Python		= 0x010,
	Lan_All			= Lan_CPP | Lan_CSharp | Lan_Java | Lan_Matlab | Lan_Python
};


// �����ýṹ�壺��ά�㣬�Ǿۺ���
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


// �������ࣺ��ά�㣬�Ǿۺ��ࣨAggregate Class��
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


// ����ֵ����ʾ�����ر�
class Placemark		MCPP11
{
public:
	using ushort = unsigned short;

public:
	// ��defaultΪconstexprʱ����ÿ������Ա���������ڳ�ʼֵ
	constexpr Placemark() = default;

	Placemark( double x, double y, ushort h )
		: m_Coord( x, y ), m_Height( h ) {}

	// ������һ��constexpr���죬�Һ��������Ϊ���ա����ҡ�ÿ������Ա��Ҫ��ʼ������һ��������
	constexpr Placemark( double x, double y, ushort h, const char *title )
		: m_Coord( x, y ), m_Height( h ), m_Title( title ) {}

	// �����ܡ��Զ�����������

public:
	friend ostream& operator <<( ostream &o, const Placemark &p )
	{
		o   << "���꣺" << p.m_Coord << endl
			<< "���Σ�" << p.m_Height << endl
			<< "���ƣ�" << p.m_Title;
		return o;
	}
private:
	// ��Ա���ͱ������������ͣ������������Զ�������ֵ���ͣ�
	// ���ڳ�ʼֵ�����ǡ��������ʽ��������ʹ���Լ��ġ�constexpr���캯����
	Coordinate m_Coord{ 121, 36 };	// ���������ڳ�ʼֵֻ���á��б��ʼ����
	ushort m_Height = Factorial( 5 );
	const char *m_Title = "�찲��";
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

// ���Եĳ�ʼ��
inline void HelperInit( void )
{
	FunctionsInit();
	ConsoleInit();
}

#endif // !TESTHELPER_H
