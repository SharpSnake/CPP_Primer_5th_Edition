#include "stdafx.h"

#include <limits>	// numeric_limits
#include <functional>	// function objects, bind, function< T >
#include <algorithm>
#include <map>

#include "TestBase.h"
#include "TestHelper.h"
#include "TCVector.h"

using namespace std;


// �����ɶԶ����������������һ����ʵ�ʵĹ�������һ����ʵ���е���ǰһ����
// ���Լ����ظ����룬����ά��������ǰ��++�ͺ���++��*��->
void Test_OperatorBase()
{
	StartOneTest( "�������������Ա��ǳ�Ա" );
	// �����������Ϊ��Ա�����ǣ���һ��������ʽΪthis���ȶ������ⲿ������һ������
	TCPoint pnt1{ 5.5, 6.2 }, pnt2{ 1.5, 3 };
	cout << "operator+:\t" << ( pnt1 + pnt2 ) << endl;
	cout << "operator-:\t" << ( pnt1 - pnt2 ) << endl;
	cout << "explicitly operator+:\t" << pnt1.operator+( pnt2 ) << endl;

	// ���������Ϊ��Աʱ�������߱����ϸ��Ǵ����ͣ����޷����ڻ�����ͱ��ʽ
	// string s2 = "def" + s1;���string��+����Ϊ��Ա����������ֵ�ַ���ƴ�ӽ��޷�ʵ�֣�
	TSPoint spnt1{ 2.5, 9.0 };
	cout << "operator+ as a member function:\t\t" << ( pnt1 + spnt1 ) << endl;
	cout << "operator- as a nonmember function:\t" << ( spnt1 - pnt2 ) << endl;


	StartOneTest( "������������" );
	TCPoint ipnt;
	cout << "Input a point:";
	if( !( cin >> ipnt ) )			// ����Ҫ��ʲô��
	{
		// �����������л��з�ǰ�Ĵ������룬��λ���ƶ���ĩβ cin.seekg( cin.end )
		cin.clear();
		cin.ignore( numeric_limits< streamsize >::max(), '\n' );

		cout << "Input a point again:";
		cin >> ipnt;
	}
	cout << ipnt << endl;	// �����Ҫ��ʲô��


	StartOneTest( "��ֵ�����ϸ�ֵ���±������" );
	// ǿ��һ�㣬��Ҫ�������������
	pnt1 = pnt2 = { 2, 4 };
	pnt1 += pnt2;
	cout << pnt1 << "\t\t" << pnt2 << endl;

	// �±�operator�����ǳ�Ա��һ�㶨�������汾
	TCVector< TCPoint > vpnt = { { 2.5, 1.0 }, { 3.3, 2.2 }, { 1.5, 9.9 } };
	cout << "TCVector::operator[]:\t" << vpnt[ 1 ] << endl;


	StartOneTest( "������������������������" );
	TCVector< TCPoint >::_iterator iter( vpnt.begin(), &vpnt );
	cout << *( ++iter ) << endl;	// vpnt[ 1 ]
	cout << *iter++ << endl;		// vpnt[ 1 ]
	cout << *iter-- << endl;		// vpnt[ 2 ]
	cout << *( --iter ) << endl;	// vpnt[ 0 ]
	cout << "iterator operator->:\t" << iter->x << " " << iter->y << endl;
	try{
		--iter;						// error : vpnt[ -1 ]
	}
	catch( exception ex ){
		cout << ex.what() << endl;
	}
}

void Test_FunObj()
{
	StartOneTest( "�������������" );
	// �����ǳ�Ա��������()����Ķ���С��������󡿣������ڵ������㷨
	Op_Abs< int > abs;
	cout << abs( -5 ) << endl;

	TCVector< int > ivec = { 3, -7, 9, -55 };
	for_each( ivec.begin(), ivec.end(), abs );
	cout << ivec << endl;


	StartOneTest( "Lambda�ı����Ǻ�������" );
	// �������ὫLambda���ʽ�����һ����()��δ�����࣬������һ����������
	// ���Lambda��Ҫ�����ⲿ��������Щ��������Ϊδ������ĳ�Ա����������Ӧ�Ĺ��캯����
	TCVector< double > dvec = { 1.5, 3.3, 0.8 };
	double ntimes = 2;						MCPP11
	for_each( dvec.begin(), dvec.end(), [ ntimes ]( double &val ) { val *= ntimes; } );
	cout << dvec << endl2;

	// �����Lambda�ᱻ���������ͳ�һ����Op_Times_N< double >���Ƶ���
	for_each( dvec.begin(), dvec.end(), Op_Times_N< double >( ntimes ) );
	cout << dvec << endl;


	StartOneTest( "��׼���ṩ�ĺ�������" );
	// <functional>�ж�����һЩ���õģ����ͣ��������󣺼Ӽ��˳����ȴ�С���߼�����ȵ�
	cout << "minus:\t" << minus< double >()( 5.5, 3 ) << endl;
	cout << "modulus:\t" << modulus< int >()( -10, 3 ) << endl;

	// �����㷨Ҳ���õ���׼��ĺ�������
	TCVector< int > ivec2 = { 3, 7, 22, 11, 9 };
	sort( ivec2.begin(), ivec2.end(), less< int >() );	// ����
	cout << "ascending sort:\t" << endl << ivec2 << endl2;
	sort( ivec2.begin(), ivec2.end(), greater< int >() );	// ����
	cout << "descending sort:\t" << endl << ivec2 << endl;
}

void Test_NewFunction()
{
	StartOneTest( "�󶨺�������" );
	using namespace std::placeholders;		MCPP11

	// bind�����Ľ����ʵҲ�Ǻ������󣬿��԰󶨣���Ա��������lambda����������ȵ�
	auto bfunc = bind( Randi, _1, 100 );
	TCPoint pnt = { 5, 5 };
	auto bmemfunc = bind( &TCPoint::Offset, &pnt, _1, _2 );
	auto blambda = bind( []( double d1, double d2 ){ return d1 / d2; }, 
		_2, _1 );	// bind���Ըı��������������˳��͸���
	auto bfunobj = bind( plus< double >(), _1, _2 );

	cout << "bind Randi:\t" << bfunc( 50 ) << endl;
	bmemfunc( 1.5, 2 );
	cout << "bind member function:\t" << pnt << endl;
	cout << "bind lambda:\t" << blambda( 3, 10 ) << endl;	// 10 / 3
	cout << "bind func obj:\t" << bfunobj( 10, 5 ) << endl;

	// ǰ�����г�2������Ҳ��������ʵ�֣���������ڶ�������д��Ϊ2���㷨ֻ���ṩ��һ������
	TCVector< double > dvec = { 1.5, 3.3, 0.8 };
	transform( dvec.begin(), dvec.end(), dvec.begin(), 
		bind( multiplies< double >(), _1, 2 ) );	MCPP11
	cout << "multiplies:\t" << endl << dvec << endl2;

	// Exercise 14.43���ж�һ�������������Ƿ��ܱ�һ�����е�ĳһ��������������Ϊ0
	auto initsCont = { 3, 4, 5 };
	int givenint = 16;
	auto divisibleBy = find_if_not( initsCont.begin(), initsCont.end(), 
		bind( modulus< int >(), givenint, _1 ) );	MCPP11
	if( divisibleBy != initsCont.end() )
		cout << givenint << " is divisible by " << *divisibleBy << endl;
	else
		cout << givenint << " is not divisible by any int of container" << endl;


	StartOneTest( "C++11 function<>" );
	// function<T>���԰�����ͬ������ʽ�ĺ���ָ�롢lambda����������
	using BinaryIntFun = function< int( int, int ) >;	MCPP11

	// �򵥵ļ�����
	map< string, BinaryIntFun > calculator = { { "rand", Randi }, { "+", plus< int >() }, 
	{ "*", []( int i1, int i2 ){ return i1 * i2; } } };
	cout << "+:\t" << calculator[ "+" ]( 3, 6 ) << endl;
	cout << "*:\t" << calculator[ "*" ]( 5, 4 ) << endl;
	cout << "rand:\t" << calculator[ "rand" ]( 1, 10 ) << endl;

	// ����ֱ�Ӵ�һ�����غ�����function�����Զ�ƥ��������ͺ��ʵ���һ���������ú���ָ�����
	int ( *pMid )( int, int ) = &MidPoint;
	calculator.insert( { "mid", pMid } );
	cout << "mid:\t" << calculator[ "mid" ]( 3, 7 ) << endl;
}

void Test_ClassConver()
{
	StartOneTest( "����ת�������" );
	// �����ǳ�Ա��û���βκͷ������ͣ�һ����const�ģ�Ĭ������ʽת��
	// һ��A->B��ת��Ҫ��һ��һ�ġ�ӳ���ϵ��������Ҫ���׶���ת�������
	Coordinate coord( 120, 30 );
	TCPoint pnt = { 5.5, 9.9 };
	TCPoint plusPnt = pnt + coord;	// Coordinate�ж�������TCPointת�����������������ʽת��
	cout << plusPnt << endl;


	StartOneTest( "ǿ����ʽת��" );
	// �������"���캯������ʽ����ת��"�ᵽ��һ����ʹ�á�explicit���ؼ���ʹת������ʽ��
	// �����󱻵�������ʱ��������bool�����ԡ����ԡ�explicit��boolת��һ�㶼��explicit��
	// ��������ʽת�������ڡ���ϱ��ʽ���п��ܲ�����ֵĽ��
	if( coord )
		cout << coord << " ����ԭ��" << endl;

	TCPoint pnt2 = { 3, 4 };	// ��ʽ��ת��Ϊdouble������㵽ԭ��ľ���
	cout << ( double )pnt2 << " or " << static_cast< double >( pnt2 ) << endl;


	StartOneTest( "��ʽת���������" );
	CONSOLE_COLOR_AUTO_RESET
	SetConsoleTextColor( CmdColor_Yellow );

	cout << "��Ҫ��������䶨����ͬ������ת����"
		"������Coordinate���Ѿ�����������TCPoint��ת����,\r\n"
		"�Ͳ�Ҫ��TCPoint�ж���һ������Coordinate�βεĹ��캯����\r\n"
		"��������غ����л��ж�����" << endl2;
	
	cout << "Ҳ��Ҫ��һ�����ж���һ�����ϵ���������ת���������\r\n"
		"������غ������û�����������Ҳ���������" << endl;
}

ChapterBase* Chapter14Init()
{
	ChapterBase *ch14 = new ChapterBase( "��ʮ���� �����������ת��" );
	ch14->AddSection( 1, "���������", &Test_OperatorBase );
	ch14->AddSection( 2, "��������", &Test_FunObj );
	ch14->AddSection( 3, "C++11 function", &Test_NewFunction );
	ch14->AddSection( 4, "����ת��", &Test_ClassConver );
	return ch14;
}
