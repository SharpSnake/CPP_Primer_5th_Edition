#ifndef TESTHELPER_H	// ��Ԥ����������ʵ��ͷ�ļ�������
#define TESTHELPER_H	// ��ĳ��cpp�д�h�ļ����ܱ�ֱ�ӻ��Ӱ������
						// ������Ŀ���Ƿ�ֹ���������ظ�����ı������

#define MCPP11   // ��עC++11������

#include <iostream>
#include <string>

// ͷ�ļ�����ò�Ҫ��using����������ͷ�ļ�������������cppʱ��
// ���ܻ��������벻�������ֳ�ͻ�����������cpp�Լ�����ʹ����Щusing����
using namespace std;

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

// �������ࣺ��
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
		return ostm << "������" << obj.m_name << "�� ���䣺" << obj.m_age;
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
