#include "stdafx.h"
#include <fstream>
#include "Chapter01.h"
#include "TestHelper.h"

void Test_CinAndCout()
{
	int vInput;

	// ����ctrl+z���߲�ƥ������ͼ�����ֹ
	StartOneTest( "ѭ������" );
	while ( cin >> vInput )
		cout << "�¼�����һ�����֣�" << vInput << endl;
	cin.clear();
	cin.sync();


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

Chapter01::Chapter01()
{
	m_Title = "��һ�� ���";
	m_TestCases[ 1 ] = SectionTest( "��׼�������", &Test_CinAndCout );
	m_TestCases[ 2 ] = SectionTest( "�ļ��ض���", &Test_FileRedirection );
}
