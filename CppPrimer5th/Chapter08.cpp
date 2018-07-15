#include "stdafx.h"

#include <fstream>
#include <sstream>
#include <string>	// getline

#include "TestBase.h"
#include "TestHelper.h"
#include "BinaryStream.h"

using namespace std;


void Test_IOBase()
{
	StartOneTest();

	CONSOLE_COLOR_AUTO_RESET;
	SetConsoleTextColor( CmdColor_Yellow );

	cout << "1��IO����֧�ֿ�����ֵ������һ�������ô���" << endl2;

	cout << "2����IO����ǰ��Ҫ��IO�������״̬��飺\r\n"
			"\t��1����ֱ�ӽ�IO������������䣬��ʹ��good()��fail()������ļ�飻\r\n"
			"\t��2����eof()�ж��Ƿ���ĩβ��\r\n"
			"\t��3����rdstate()��setstate��ȡ������״̬��������λ�����жϺ�����״̬��\r\n"
			"\t��4����clear()����״̬��" << endl2;

	cout << "3��fstream������ʱ�Զ���close��" << endl2;
}

void Test_CinCout()
{
	StartOneTest( "Flushing the Output Buffer" );
	// ������ǿ��ˢ��������ģʽ
	cout << "hello" << endl;
	cout << "hello" << flush;	// ֻˢ��β�������κζ���
	cout << "hello" << ends;	// ˢ��ͬʱβ����һ�����ַ����ƺ��ǿո�
	cout << "��ӡ���" << endl;

	StartOneTest( "Output Buffer�Ƿ�����ˢ��" );
	ofstream fstm1( "testunitbuf.txt" );
	fstm1 << unitbuf;	// ��������ˢ��
	fstm1 << "test1\r\n";
	fstm1 << "test2\r\n";

	fstm1 << nounitbuf;
	fstm1 << "test3\r\n";
	fstm1 << "test4\r\n";
	fstm1 << "test finish" << endl;	// �������öϵ�鿴�������Ƿ�����ˢ
	fstm1.close();

	StartOneTest( "��������������" );
	// cinһ����cout�󶨣�cinҪ������ʱcout�᡾�Զ�ˢ��������
	string word;
	ostream *oldstm = cin.tie( nullptr );	// �����
	cout << "input a word";
	cin >> word;
	cout << "cin read a word from cout:\t" << word << endl;
	cin.tie( oldstm );						// �����°�
}

void Test_Fstream()
{
	StartOneTest( "ofstream��Ĭ���ǽضϵ�" );
	ofstream writef( "�ض���׷��.txt" );	// ��ʽofstream::out | ofstream::trunc
	writef << "test1" << endl;
	writef << "test over" << endl;
	writef.close();

	writef.open( "�ض���׷��.txt" );	// Ĭ�ϴ򿪣���ֱ�Ӹ���ԭ���ļ�����
	writef << "test2" << endl;
	writef.close();

	writef.open( "�ض���׷��.txt", ofstream::app );	// ����׷��ģʽ��
	writef << "test3" << endl;
	writef.close();

	ifstream readf( "�ض���׷��.txt" );
	cout << "�ض���׷��.txt" << endl;
	string line;
	while( getline( readf, line ) )
		cout << line << endl;
	readf.close();


	StartOneTest( "�����ƶ�д����" );	// �������л��ͷ����л�
	ofstream binf( "BinaryIOTest.dat", ofstream::binary );
	Coordinate coord{ 120.5, 31.4 };
	double dary[ 2 ] = { 10.5, 1999.9 };
	int ival = -167;

	binf.write( reinterpret_cast< const char* >( &coord ), sizeof( Coordinate ) );
	binf.write( reinterpret_cast< const char* >( &dary ), sizeof( dary ) );
	binf.write( reinterpret_cast< const char* >( &ival ), sizeof( int ) );
	binf.close();

	ifstream binread( "BinaryIOTest.dat", ifstream::binary );
	Coordinate rcoord;
	double rdary[ 2 ];
	int rival;

	binread.read( reinterpret_cast< char* >( &rcoord ), sizeof( Coordinate ) );
	binread.read( reinterpret_cast< char* >( &rdary ), sizeof( rdary ) );
	binread.read( reinterpret_cast< char* >( &rival ), sizeof( int ) );
	binread.close();

	cout << rcoord << endl;
	cout << rdary[ 0 ] << " " << rdary[ 1 ] << endl;
	cout << rival << endl;
}

// ������Ϣ������stringstream
class _HouseInfo
{
public:
	string Name;
	double Area;
	int Altitude;

	friend ostream& operator <<( ostream &os, const _HouseInfo &obj )
	{
		return os << obj.Name << " " << obj.Area << " " << obj.Altitude;
	}

	friend oBinaryStream& operator <<( oBinaryStream &os, const _HouseInfo &obj )
	{
		return os << obj.Name << obj.Area << obj.Altitude;
	}

	friend istream& operator >>( istream &is, _HouseInfo &obj )
	{
		return is >> obj.Name >> obj.Area >> obj.Altitude;
	}

	friend iBinaryStream& operator >>( iBinaryStream &is, _HouseInfo &obj )
	{
		return is >> obj.Name >> obj.Area >> obj.Altitude;
	}
};

void Test_OtherStream()
{
	StartOneTest( "�ַ�����" );
	// �򵥵����Ϊ���������ַ�������ʽ���л��������л�������������ظ�����
	ostringstream ostr;
	_HouseInfo oh1{ "Tom", 200.584, 56 }, oh2{ "Jack", 150.5, -12 };

	ostr << oh1;
	string s1 = ostr.str();
	
	ostr.str( "" );	// ���
	ostr << oh2;
	string s2 = ostr.str();
	cout << "_HouseInfo -> string buffer:" << endl << s1 << endl << s2 << endl2;

	istringstream istr;
	_HouseInfo ih1, ih2;

	istr.str( s1 );
	istr >> ih1;

	// �������Ҫ������λ�����õ���ͷ�����ߵ���clearҲ����
	istr.str( s2 );
	istr.seekg( 0, istringstream::beg );
	istr >> ih2;
	cout << "string buffer -> _HouseInfo:" << endl << ih1 << endl << ih2 << endl;


	StartOneTest( "�������ڴ���" );
	oBinaryStream obin;
	obin << oh1 << oh2;
	cout << "Output binary buffer size:\t" << obin.Size() << endl;

	iBinaryStream ibin;
	ibin.Memory( obin.Memory(), obin.Size() );

	_HouseInfo ih3, ih4;
	ibin >> ih3 >> ih4;
	cout << "binary stream -> _HouseInfo:" << endl << ih3 << endl << ih4 << endl;
}


ChapterBase* Chapter08Init()
{
	ChapterBase *ch08 = new ChapterBase( "�ڰ��� std IO��" );
	ch08->AddSection( 1, "��������", &Test_IOBase );
	ch08->AddSection( 2, "��׼���롢���", &Test_CinCout );
	ch08->AddSection( 3, "�ļ����롢���", &Test_Fstream );
	ch08->AddSection( 4, "�ַ������������ڴ���", &Test_OtherStream );
	return ch08;
}
