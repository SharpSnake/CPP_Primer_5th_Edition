#ifndef TESTHELPER_H	// 用预处理器变量实现头文件保护符
#define TESTHELPER_H	// 在某个cpp中此h文件可能被直接或间接包含多次
						// 这样做目的是防止出现类型重复定义的编译错误

#define MCPP11   // 标注C++11新特性

#include <iostream>
#include <string>

// 头文件中最好不要用using声明，当此头文件被包含到其他cpp时，
// 可能会引发意想不到的名字冲突，所以最好由cpp自己决定使用哪些using声明
using namespace std;

// 测试用结构体：二维点
struct TSPoint
{
	double x;
	double y;

	friend ostream & operator <<( ostream &ostm, const TSPoint &obj )
	{
		return ostm << "x：" << obj.x << "， y：" << obj.y;

		// 若采用下面的实现，打印会自动换一换，但一般自定义类型不要主动换行好一些
		// return endl( ostm << "x：" << obj.x << "， y：" << obj.y );
	}
};

// 测试用类：人
class TCPerson
{
private:
	MCPP11
	string m_name = "";
	unsigned int m_age = 0;

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
		return ostm << "姓名：" << obj.m_name << "， 年龄：" << obj.m_age;
	}
};


inline void FinishOneTest( const char *msg = nullptr MCPP11 )
{
	if ( msg )
		cout << msg << endl;
	cout << "*********************************"
		"*********************************" << endl << endl;
}


#endif // !TESTHELPER_H
