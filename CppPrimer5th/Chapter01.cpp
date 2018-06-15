#include "stdafx.h"
#include <fstream>
#include "Chapter01.h"
#include "TestHelper.h"


void Chapter01::Run()
{
	int vInput;

	// 键入ctrl+z或者不匹配的类型即可终止
	StartOneTest( "循环键入" );
	while ( cin >> vInput )
		cout << "新键入了一个数字：" << vInput << endl;
	cin.clear();
	cin.sync();


	// 单个对象姓名和年龄间用空格，多个对象间用回车
	StartOneTest( "键入多个复杂对象" );
	TCPerson p1, p2;
	cin >> p1 >> p2;
	cout << p1 << endl << p2 << endl;


	// cout输出重定向到一个文件
	StartOneTest( "cout文件重定向" );
	ofstream fcout( "fcout.txt" );
	streambuf *origin_buf = cout.rdbuf();

	cout << "这条消息将打印在命令窗口" << endl;

	cout.rdbuf( fcout.rdbuf() );
	cout << "这条消息将打印在重定向的文件中" << endl;

	cout.rdbuf(origin_buf);
	cout << "这条消息将【再次】打印在命令窗口" << endl;

	fcout.close();
}
