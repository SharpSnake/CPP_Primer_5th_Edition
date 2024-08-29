#include <fstream>

#include "TestBase.h"
#include "TestHelper.h"
#include "Inheritance.hpp"

void Test_CinAndCout()
{
	int vInput;

	// ����ctrl+z���߲�ƥ������ͼ�����ֹ
	StartOneTest( "ѭ������" );
	while ( cin >> vInput )
		cout << "�¼�����һ�����֣�" << vInput << endl;
	cin.clear();
	cin.ignore( numeric_limits< streamsize >::max(), '\n' );

	// ��������������������ÿո񣬶��������ûس�
	StartOneTest( "����������Person����" );
	TCPerson p1, p2;
	cin >> p1 >> p2;
	cout << p1 << endl << p2 << endl;
}

void Test_FileRedirection()
{
	// cout����ض���һ���ļ�
	StartOneTest( "cout�ļ��ض���" );
	ofstream fcout( "fcout.txt" );
	streambuf *origin_buf = cout.rdbuf();

	cout << "������Ϣ����ӡ�������" << endl;

	cout.rdbuf( fcout.rdbuf() );
	cout << "������Ϣ����ӡ���ض�����ļ���" << endl;

	cout.rdbuf( origin_buf );
	cout << "������Ϣ�����ٴΡ���ӡ�������" << endl;

	fcout.close();
}

void Chapter01Init()
{
	auto ch01 = ChapterBase::AddChapter( 1, "��һ�� ���" );
	ch01->AddSection( 1, "��׼�������", &Test_CinAndCout );
	ch01->AddSection( 2, "�ļ��ض���", &Test_FileRedirection );
}
static int _Init = ( Chapter01Init(), 0 ); // ���ö���������Ĺ���
