#include "stdafx.h"

#include <memory>
#include <strstream>

#include "TestBase.h"
#include "TestHelper.h"
#include "Inheritance.hpp"

using namespace std;

set< unsigned > TCPerson::IDNumberSet;
boost::format TCPerson::IDFormater( "1401%07d" );


TCMan	lilei( 22, "����", 0.7 );
TCWomen	hanmei( 21, "��÷", "������" );

void Test_OOPBase()
{
	StartOneTest( "��Ҫ����" );
	{
		CONSOLE_COLOR_AUTO_RESET;
		SetConsoleTextColor( CmdColor_Yellow );

		cout << "��̬�󶨣�ʹ�û�������û�ָ����ʳ�Աʱ��ֱ������ʱ�Ű�\r\n"
			"��Ӧ�ĳ�Ա�汾��" << endl2;

		cout << "�����Ӷ������ǿ��Խ����࿴�������Ա��һ���֣��������ܱ�֤\r\n"
			"���ڴ����������洢�ģ����Կ��Խ��������û�ָ��󶨵���������ϡ�" << endl2;

		cout << "��̬���ͣ�д�ڴ����У����������ܿ����ģ����û�ָ�������\r\n"
			"��̬���ͣ�������ʱ����̬�����������󶨵Ķ�������\r\n"
			"��̬�ı��ʣ����û�ָ��ľ�̬�����붯̬���Ͳ�ͬ��" << endl2;

		cout << "��������˵������������classʱĬ����private�̳У�\r\n"
			"������structʱĬ����public�̳�" << endl2;

		cout << "�ϳɺ����������࿴�������һ����Ա���ж�����ϳɺ�����״̬��\r\n"
			"��������ĺϳɿ���������delete�ģ���ô����ĺϳɿ�������Ҳ��ɾ���ġ�\r\n"
			"�ֶ����忽�����ƣ����ȴ�����ࣨ�û����Ӧ�Ŀ������ƣ����ٴ����Լ���"  << endl2;

		cout << "���������е����麯������ʵ�Ǿ�̬�󶨣�����������ã�\r\n"
			"1������ʱ�����໹û���ü��������麯����Ҳû�б����ǣ�\r\n"
			"2������ʱ���������ȱ����٣��麯�����ֻ�ԭ������ʱ�����ӣ�"   << endl2;
	}


	StartOneTest( "�̳й���" );
	// �̳����е�ÿһ����Ը����Լ��ĳ�Ա��ʼ���������ʼ�������ڳ�ʼֵ�б���
	// ���ø���Ĺ��죬�ٳ�ʼ���Լ��ĳ�Ա
	cout << TCMan( 23, "Tom", 5.5 ) << endl;


	StartOneTest( "������ת��" );
	// ����ת�����������û�ָ��ת���࣬��ʽ��
	TCPerson *pPer = &lilei, &rPer = hanmei;

	// ��������ʽ������ת�������������û�ָ��ת���ࣩ��������ȷ��ת����ȫǿת
	TCWomen *women1 = dynamic_cast< TCWomen* >( pPer );
	if( !women1 )	// dynamic_cast�м�⹦�ܣ����Ͳ������Ϊ��
		cout << pPer->Name() << " is NOT a woman!!!" << endl;

	TCWomen &women2 = static_cast< TCWomen& >( rPer );
	cout << women2 << endl;		// static_cast ������⣬�ɳ���Ա��֤ת����ȫ


	StartOneTest( "���౻�и�" );
	// �����ݹ��̣����������û�ָ��ʱ������ֱ���Ի������ʹ��ݣ�
	// �򿽱���������������Ӷ���������Ϣ��ʧ
	TCPerson copyhanmei = hanmei;
	copyhanmei.Speak( cout );


	StartOneTest( "protected����Ȩ�޲��ܴ��ݸ�����ĳ�Ա��friend" );
	// ����ĳ�Ա��friend��Ҫͨ�������������ӷ��ʻ����protected��Ա������ֱ�ӷ���
	TCPerson thompson;
	istrstream( "55 Thompson" ) >> thompson;
	thompson.Speak( cout );

	// ������Ԫ����һ���������Ԫ��ϵ���ܼ̳С����������е���Ԫ�����Է��ʻ���ĳ�Ա��
	// �����Ԫ�Լ̳���ĳ�Աû�з���Ȩ�ޣ�
	// �����ʵ�к����ƣ��Һ����������ѣ��������Һ�С����ʲô��ϵ��������С��Ҳû�й�ϵ


	StartOneTest( "��������˵����" );
	// 1����������˵�����Լ̳��౾���ڲ�������û�С��κ�Լ�������ã�
	// �������������������ġ��û����������ߺͺ����ļ̳���
	ManSoldier soldier001( 25, "�����ӳ�", 3 );	// ManSoldier����private�̳���TCMan

	// TCMan�Լ������ϵĻ����public��Ա��Soldier��ʹ���߶���private��
	//soldier001.Sleep();	// ERROR! TCMan's member is private for user

	// 2����ĳ�δ��룬ֻҪ�����public��Ա�ǿɷ��ʵģ�����ת���Ϳ���
	//TCMan &soldier2base = soldier001;	// ERROR!
	TCMan &soldier2base = soldier001.BaseRef();
	soldier2base.Sleep();	// ��ManSoldier�ڲ���˵��������Ȼ��public�ģ���������ת��

	// 3������ͨ��using�ı䲿�ֻ����Ա�Ŀɷ����ԣ�
	soldier001.m_BeardLength += 3;
	soldier2base.Speak( cout );


	StartOneTest( "�̳��е�������" );
	// ��������Ƕ���������������ڸ����У�Ҳ����˵������ͨ�����������һ����
	// �����ͬ����Ա�����ظ���ĳ�Ա����Ա������ʹ�ββ�ͬ��Ҳ������
	cout << "lilei's Man ID " << lilei.IDNumber( false ) << endl;

	// ���� �� ��ʧ��������::��ʽ���û���İ汾
	cout << "lilei's Base ID " << lilei.IDNumber( true ) << endl;

	// ��using�����������֣����⸲�ǣ���������ĺ����Կ����վ�ʹ��
	cout << "lilei's ID " << lilei.IDNumber() << endl;


	StartOneTest( "C++11�̳й���" );
	// ���๹��ʱΪ���չ˵�����ĳ�ʼ��������Ҫд�ü����ͻ��๹���β���ͬ�����๹�캯��
	// C++11����ʹ��using������Ĺ����������࣬������Բ��ض������ƵĹ���
	ManSoldier soldier002( 100, "����", 5 );		MCPP11
	soldier002.Speak( cout );

	// ��������Ĭ��ʵ��ʱ�����Ĭ��ʵ�β��ᱻ�̳У����ǽ����������ɸ�û��Ĭ��ʵ�εĹ��죬
	// ����һ���ǰ��������βεĹ��죬һ����û������Ǹ��βεİ汾�������Ǹ�Ĭ��ֵ���Զ�����
	ManSoldier soldier003( 30, "��ӥ" );	// �����й��캯������Ĭ����2.5cm��
	soldier003.Speak( cout );
}

void Test_VirtualFunctions()
{
	StartOneTest( "�麯���ؼ���virtual" );
	// virtual�������в��Ǳ���ģ�һ�������ڻ����б�ע��virtual��
	// �����ļ̳�����������ͬ�ĺ������Զ�����ģ����Բ����ٱ�ע
	IBehavior &manBehave = TCMan( 25, "xiaoming", 0.3 );
	manBehave.Speak( cout );


	StartOneTest( "�����麯��ƥ��" );
	// �̳�����V�������βκͷ������ͱ���һ�£�
	// �������������������û�ָ�����������
	shared_ptr< TCPerson > spClone1( lilei.Clone() );
	shared_ptr< TCPerson > spClone2( hanmei.Clone() );
	spClone1->Speak( cout );
	spClone2->Speak( cout );


	StartOneTest( "Ĭ��ʵ��" );
	// ���Vfun��Ĭ��ʵ�Σ������ɶ���ľ�̬���;���ʹ���ĸ����Ĭ��ֵ
	// ���ԣ�����������ƣ����������ʹ����ͬ��Ĭ��ֵ
	lilei.Sleep();
	( ( TCPerson& )lilei ).Sleep();
	lilei.TCPerson::Sleep();		// ���߿�����::ָ��vfun�İ汾


	StartOneTest( "���麯�����������Լ����ⲿ���壬�����������ʵʵ���ڵص���" );
	lilei.Speak( cout );
}

void Test_NewInheritance()
{
	StartOneTest( "��ֹ�̳к��麯����д" );		MCPP11
	// TCWomen��ע��final�������������ٱ��̳У���TCMan���滹������һ��ManSoldier����
	ManSoldier soldier001( 22, "����", 0.5 );
	soldier001.Speak( cout );

	// ��һ���麯����ע��final��������麯�������ٱ�������д���ǣ�


	StartOneTest( "override������д�麯��" );		MCPP11
	// �����ຯ�������עoverride�����߱���������Ҫ��д������麯�����кܺõļ�⹦��
	// ��ϰ15.12����Ա��������ͬʱ��עoverride��final

	// TCPerson��Species������עoverride��д�����ұ�ע��final��
	// �������඼һ���������࣬����Speciesû��Ҫ����������д
	TCPerson per;
	cout << per.Species() << endl;

}

void Test_VirtualFunctionTable()
{
	StartOneTest( "�ܰ���ȴû�����õ��麯�������ˣ�" );
	// �麯������ʵ����һ�麯��ָ�룬����һ�������ڴ���ʼ��
	// ������ĵ�ַ����һ����ά���飬���̳��˼��������麯�����࣬���м����麯����
	// �ֱ��Ƕ�ά����ĵ�0�С���1��......

	// ���ڳ�Ա������ʽ��this������������ԣ����ﹹ��һ���򵥵ļ̳��������漰��Ա����
	struct IBase{
		virtual ~IBase() {}
		virtual void Talk() = 0;
	};
	struct Base1{
		virtual ~Base1()	{}
		virtual void Fun1()	{ cout << "Base1::Fun1" << endl; }
		virtual void Fun2()	{ cout << "Base1::Fun2" << endl; }
	};
	struct Derived1 : public Base1, public IBase{
		virtual ~Derived1()		{}
		void Fun1() override	{ cout << "Derived1::Fun1" << endl; }
		void Talk() override	{ cout << "Derived1::Talk" << endl; };

		// ��һ�ű����Base1��������麯��
		virtual void DerFun()	{ cout << "Derived1::DerFun" << endl; };
	};
	struct Derived2 : Derived1{
		virtual ~Derived2()		{}
		void Fun2() override	{ cout << "Derived2::Fun2" << endl; }
		void DerFun() override	{ cout << "Derived2::DerFun" << endl; };
	};
	Derived1 &derObj = Derived2();

	// ��derObj�ĵ�ַ��int**������������תΪ�����ĺ���ָ��
	typedef void ( *PV_Func )();
	int **pVfunMap = ( int** )( &derObj );

	// ������һ�㶼��ÿ�ű�ĵ�һ�����������Ա����������1��ʼ
	// Base1::vftable
	PV_Func der_Fun1 = ( PV_Func )pVfunMap[ 0 ][ 1 ];
	PV_Func der_Fun2 = ( PV_Func )pVfunMap[ 0 ][ 2 ];

	// Derived1::vftable����һ�ű�Ҳ�����Լ������vftable�����Լ����麯�����ڻ���ĺ���
	PV_Func der_DerFun = ( PV_Func )pVfunMap[ 0 ][ 3 ];

	// IBase::vftable
	PV_Func der_Talk = ( PV_Func )pVfunMap[ 1 ][ 1 ];

	der_Fun1();
	der_Fun2();
	der_DerFun();
	der_Talk();
}

ChapterBase* Chapter15Init()
{
	ChapterBase *ch15 = new ChapterBase( "��ʮ���� ���������" );
	ch15->AddSection( 1, "OOP��������", Test_OOPBase );
	ch15->AddSection( 2, "�麯��", Test_VirtualFunctions );
	ch15->AddSection( 3, "�̳е�������", Test_NewInheritance );
	ch15->AddSection( 4, "���⻰���麯����", Test_VirtualFunctionTable );
	return ch15;
}

string TCPerson::_GenIDNumber()
{
	unsigned newid = 0;
	if( IDNumberSet.size() )
		newid = *--IDNumberSet.end();
	IDNumberSet.insert( ++newid );

	return ( IDFormater % newid ).str();	// this return will be moved
}
