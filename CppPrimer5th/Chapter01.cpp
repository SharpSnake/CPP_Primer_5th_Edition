#include "stdafx.h"
#include <fstream>
#include "Chapter01.h"
#include "TestHelper.h"


void Chapter01::Run()
{
	int vInput;

	// ����ctrl+z���߲�ƥ������ͼ�����ֹ
	StartOneTest( "ѭ������" );
	while ( cin >> vInput )
		cout << "�¼�����һ�����֣�" << vInput << endl;
	cin.clear();
	cin.sync();


	// ��������������������ÿո񣬶��������ûس�
	StartOneTest( "���������Ӷ���" );
	TCPerson p1, p2;
	cin >> p1 >> p2;
	cout << p1 << endl << p2 << endl;


	// cout����ض���һ���ļ�
	StartOneTest( "cout�ļ��ض���" );
	ofstream fcout( "fcout.txt" );
	streambuf *origin_buf = cout.rdbuf();

	cout << "������Ϣ����ӡ�������" << endl;

	cout.rdbuf( fcout.rdbuf() );
	cout << "������Ϣ����ӡ���ض�����ļ���" << endl;

	cout.rdbuf(origin_buf);
	cout << "������Ϣ�����ٴΡ���ӡ�������" << endl;

	fcout.close();
}
