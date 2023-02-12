#include "stdafx.h"

#include <thread>
#include <chrono>

#include "TestBase.h"
#include "Chapter06.h"

#pragma warning( disable:4244 )
#pragma warning( disable:4305 )

using namespace std;

bool Worker1Run = true;


void Test_Base()
{
	StartOneTest( "��̬�ֲ��������Լ�ִ��ֵ��ʼ��" );
	Worker1Run = false;
	CallLocalStaticObject( true );

	StartOneTest( "const���ô��ݲ���" );
	// ���ô��ݿ��Ա��ⲻ��Ҫ���ڴ濽��������const���ÿ��Խ��ܸ��ӹ㷺��ʵ������
	int refi = 100;
	PassingArgsByRef( refi );
	PassingArgsByRef( 3.6 );	// const���ò����������Խ�������ֵ������������ʽת��

	StartOneTest( "��ʼ���б����ֱ����Ϊ����ֵ" );	MCPP11
	// ֻҪ�ͷ�������ƥ�伴�ɣ�����MidPoint������Coordinate::operator TCPoint()

	StartOneTest( "Ĭ��ʵ�γ�ʼ��" );
	// �κηǾֲ��������󣬶�������ΪĬ��ʵ�Σ�����������б䣬������Ĭ��ʵ��Ҳʵʱ�ı�
	PrintSchoolInfo( "����Сѧ" );

	SchoolPosition.Offset( 2, -3 );
	PrintSchoolInfo( "�еڰ���ѧ" );

	StartOneTest( "inline�����ǽ���������Ժ��������Ż�չ����\r\n"
		"����ǿ�ƣ�����������ʵ��������Ժ���inline" );

	StartOneTest( "���������õı���" );
	cout << "�ļ���\t" << __FILE__ << endl;
	cout << "������\t" << __func__ << endl;
	cout << "������\t" << __LINE__ << endl;
	cout << "���ڣ�\t" << __DATE__ << " " << __TIME__ << endl;
}

void Test_ArrayParam()
{
	StartOneTest( "һά������Ϊ�β�" );
	// ������Ϊ�βζ���Ĭ�ϰ�ָ�봦������ʧ��ά����Ϣ���������������ǵȼ۵ģ�
	// �޷�֪����������ĳ��ȣ�ֻ�����⴫�ݳ�����Ϣ���������鱾���β�б�ǣ������ַ�����'\0'��
	auto AryFun1 = []( int *ary ){ ; };
	auto AryFun2 = []( int ary[] ){ ; };
	auto AryFun3 = []( int ary[ 10 ] ){ ; };

	// �����β�����������ã����������ά����Ϣ���Ա�������ʵ������ĳ��ȱ�����β��ϸ�һ��
	int iary4[ 5 ] = { 11, 22, 33, 44, 55 };
	auto AryFun4 = []( int ( &ary )[ 5 ] ){ for( auto &i : ary ) cout << i << endl; };
	cout << "����������Ϊ�β�" << endl;
	AryFun4( iary4 );	// ������Ȳ���5�������ʧ��

	// ����16�µķ�����ʵ�ο��������ⳤ�ȣ�ģ�庯�����Զ��Ƶ�
	int iary5[ 3 ] = { 12, 14, 16 };
	float fary6[ 4 ] = { 1.1, 2.2, 3.3, 4.4 };
	cout << "����ģ�庯���������������͡����ȵ���������" << endl;
	PrintArray1D( iary5 );
	PrintArray1D( fary6 );


	StartOneTest( "��ά�����β�" );	// �Զ�ά����Ϊ�������һλ���飬�����԰쵽
	// ����Ĵ��ݷ�������һ��ά�ȣ��������ᶪʧ��ֻ�����ڶ���ά�ȣ����������贫��һ������
	auto Ary2Fun1 = []( int ( *ary )[ 3 ], size_t rows ){ ; };	//�� int ary[][ 3 ] �ȼ�

	// ��ά��������ã���ʵ�������Ϊ��ά�����һ�����Σ������Ա����С�������Ϣ
	int i2ary[ 2 ][ 3 ] = { { 1, 2, 3 }, { 4, 5, 6 } };
	auto Ary2Fun2 = []( int ( &ary )[ 2 ][ 3 ] )
	{
		for( auto &i : ary )
		{
			for( auto &j : i )
				cout << j << " ";
			cout << endl;
		}
	};
	cout << "��ά����������Ϊ�β�" << endl;
	Ary2Fun2( i2ary );

	// ͬ��������ģ�庯������������������
	cout << "����ģ�庯���������������͡��������Ķ�ά��������" << endl;
	PrintArray2D( i2ary );

	// �Դ����ƣ�����ά�ȵ�����Ҳ��һ����
}

void Test_ArrayReturn()
{
	// �����������������ʱ���Զ���ָ�봦���Ҳ����Զ�����������ֻ�ܷ��������ָ�������
	int matrix[ 3 ][ 3 ] = { { 1, 2, 3 }, 
							{ 4, 5, 6 }, 
							{ 7, 8, 9 } };

	StartOneTest( "���䷵�ط�" );
	ArrayRowi *row0 = GetRow1( matrix, 0 );
	PrintArray1D( *row0 );

	StartOneTest( "�������ط�" );	// �����������ַ���
	ArrayRowi *row1 = GetRow2( matrix, 1 );
	ArrayRowi &row1r = GetRow2r( matrix, 1 );
	PrintArray1D( *row1 );
	PrintArray1D( row1r );

	StartOneTest( "C++11β�����ͷ��ط�" );
	ArrayRowi *row2 = GetRow3( matrix, 2 );
	PrintArray1D( *row2 );
}

void Test_VaryingParam()
{
	StartOneTest( "C++11��initializer_list" );	MCPP11
	// ���β����Ͷ���ͬ������������ʱ������ʹ��initializer_list�����������ص�
	// 1���ڲ����洢�������ĸ�����������п�������˻����Ч��
	// 2��ʹ�÷�ʽ��vector�������ƣ����������������const�ģ�Ҳ�㹻��ȫ��
	// 3��һ��list��ֵ������һ��������list����ͬһ�����ݶ��󸱱������´����
	PrintNames( { "John", "Lucy", "Tom" } );
	PrintNames( { "ZhangSan", "LiSi" } );

	StartOneTest( "��ʽ��ʡ�Է��β�" );
	// ��ò�Ҫ�ӵ�һ���������ʡ�ԣ�һ���ɵ�һ�������ṩ���뷽������������������������֣�
	// 1��������������ʱ������������֪��Ӧ������ͬ�ģ�
	// 2����Ԫ�أ�������������֪��ֹͣ�����ǽ���ֵ�쳣���������int�õ�-1����
	// 3�����������б������ַ���������"%f%d%s"����ʱ�ڲ���������Ϣ���ν�����
	PrintNames( 3, "Hubei", "Shandong", "Jiangxi" );
}

void Test_Overloaded()
{
	StartOneTest( "���������Ͳ�ͬ���������أ���һ������ʾ�������" );

	StartOneTest( "�����β��е�const" );
	// ����������Ͳ��䣬�򶥲�const�����������أ��ײ�const��const���û�ָ�룩��������
	TCPoint pnt{ 3.3, 4.4 };
	const TCPoint cpnt{ 1.1, 2.2 };
	PrintPoint( pnt );
	PrintPoint( cpnt );

	StartOneTest( "������������" );
	// ���ڲ㡿�ĺ����������᡾���أ����Σ���������е�ͬ�����أ�������ĳ�������ռ䡢������
	// ĳһ������ڲ�����һ����������ô���������������������ض���ʧЧ��
	{
		// ��ʱ�����������ж��ٸ����أ�����ֻ��const TCPoint &��һ�����أ�
		// VC++û����������ԭ��gcc�����������ϸ����صģ�
		// ���Գ��ǿ��⣬����������÷���һ��ͳһ������Ҫ������ĳһ����С��������������
		void PrintPoint( const TCPoint & );
		PrintPoint( pnt );
	}

	StartOneTest( "���غ�����ƥ��" );
	// ����ԭ������Խ�ӽ���ƥ���Խ�ã�����ͬ��ʵ�ηֱ��벻ͬ�ĺ�ѡ����ƥ�����ʱ��
	// �ͻ���ֶ��������⣬�������޷����������ĸ����أ����±���ʧ�ܣ�
	// �������������⣬һ����Ҫ����ǿ������ת��������ͨ����׾�ӵ������ɵģ�
	cout << "���ƥ��" << endl;
	Sum( 1, 2 );
	Sum( 0.2F, 0.3F );
	cout << "Sum( 1.3, 3 ) ������" << endl;	//һʵƥ��double�汾��һ��ƥ��int�汾

	// С�����Զ�������Խ��short�汾��ֱ��ƥ��int�汾��
	Sum( 'a', 'b' );

	// ��������ת�����ּ���ߵͣ������������������
	cout << "Multiply( 3.5, 1.6 ) ������" << endl;	// ����˵double��float���ӽ���ƥ��float�汾
}

void Test_FunctionPointer()
{
	StartOneTest( "����������ʶ" );
	cout << "����ָ�븳ֵʱ��������ǰ��'&'��ʡ���������Զ�ȡ������ַ" << endl;	// I prefer &
	cout << "����ָ�����ʱ������Ҫ��ָ������ã�ֱ���õ������������" << endl;

	// ��decltype��ȡ�����Ǻ������ͣ����Ǻ���ָ�룬ʹ��ʱ��Ҫ�ֶ���'*'
	decltype( Subtract ) *func = Subtract;
	PrintTypeName( func );


	StartOneTest( "����ָ����Ϊ�β�" );
	CalMathFunA( 5, 7, Sum );
	CalMathFunB( 6, 9, Multiply );


	StartOneTest( "����ָ����Ϊ����ֵ" );
	// ������ķ�����Щ���ƣ�Ҳ�����ַ�ʽ

	cout << "���غ���ָ�롪������ʽ" << endl;
	PFMath pFSum = GetFunc1( 1 );
	pFSum( 2, 3 );

	cout << "���غ���ָ�롪������ʽ" << endl;
	PFMath pFSub = GetFunc2( 2 );	// ��GetFunc3( 2 )��һ����
	pFSub( 10, 2 );

	cout << "���غ���ָ�롪��C++11β��ʽ" << endl;
	PFMath pFMulti = GetFunc4( 3 );
	pFMulti( 3, 4 );
}

ChapterBase* Chapter06Init()
{
	// �����²��ԱȽ����⣬��������������̨�߳����ڲ���
	thread worker1( []( void )
	{
		while ( Worker1Run )	// �Ȳ������̰߳�ȫ��������������Ǽ򵥵Ĳ���
		{
			this_thread::sleep_for( chrono::nanoseconds( 1 ) );	// ���붼�У���
			CallLocalStaticObject( false );
		}
		cout << "Worker1 stoped." << endl;
	} );
	worker1.detach();

	ChapterBase *ch06 = new ChapterBase( "������ ����" );
	ch06->AddSection( 1, "��������", &Test_Base );
	ch06->AddSection( 2, "�����β�", &Test_ArrayParam );
	ch06->AddSection( 3, "������������", &Test_ArrayReturn );
	ch06->AddSection( 4, "�ɱ��β�", &Test_VaryingParam );
	ch06->AddSection( 5, "��������", &Test_Overloaded );
	ch06->AddSection( 6, "����ָ��", &Test_FunctionPointer );
	return ch06;
}

/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
// �����������һ���ֲ���̬���������ڼ�¼���������������̣߳����õĴ����ܺ�
void CallLocalStaticObject( bool needPrint )
{
	static int callingCount;	// �˱�����һֱ���ڣ�Ĭ����0����Ȼ�����ֶ���ʼ��
	if( needPrint )
		cout << "Until now, the CallingCount is : " << callingCount << endl;
	else
		++callingCount;
}

void PassingArgsByRef( const int &ri )
{
	cout << "ref value : " << ri << endl;
}

extern TCPoint SchoolPosition = { 120, 31 };
void PrintSchoolInfo( const std::string &name, size_t number, const TCPoint &pos )
{
	cout << "ѧУ����:\t" << name << endl;
	cout << "ѧ������:\t" << number << endl;
	cout << "ѧУ����:\t" << pos << endl << endl;
}

void PrintNames( initializer_list< string > names ) MCPP11
{
	for( auto &i : names )
		cout << i << endl;
}

// �������������֪�����ַ������Ƚϼ򵥣�������ʾʡ�Է���һ���������
void PrintNames( int argc... )	// include <cstdarg>
{
	va_list list;
	va_start( list, argc );
	for( int i = 0; i < argc; ++i )
		cout << va_arg( list, char* ) << endl;
	va_end( list );
}

int ( *GetRow1 ( int ( *matrix )[ 3 ], int row ) )[ 3 ]
{
	return &matrix[ row ];
}
ArrayRowi* GetRow2( int ( *matrix )[ 3 ], int row )
{
	return &matrix[ row ];
}
ArrayRowi& GetRow2r( int ( *matrix )[ 3 ], int row )
{
	return matrix[ row ];
}
auto GetRow3( int ( *matrix )[ 3 ], int row ) -> int ( * )[ 3 ]
{
	return &matrix[ row ];
}

void PrintPoint( TCPoint &pnt )
{
	PrintTypeName( pnt );
	cout << pnt << endl;
}
void PrintPoint( const TCPoint &pnt )
{
	PrintTypeName( pnt );
	cout << pnt << endl;
}

void Sum( int a, int b )
{
	cout << "The ( int )sum of ab : \t" << ( a + b ) << endl;
}
void Sum( short a, short b )
{
	cout << "The ( short )sum of ab : \t" << ( a + b ) << endl;
}
void Sum( double a, double b )
{
	cout << "The ( double )sum of ab : \t" << ( a + b ) << endl;
}
void Subtract( int a, int b )
{
	cout << "( int ) a - b = \t" << ( a - b ) << endl;
}
void Multiply( int a, int b )
{
	cout << "( int ) a * b = \t" << ( a * b ) << endl;
}
void Multiply( float a, float b )
{
	cout << "( float ) a * b = \t" << ( a * b ) << endl;
}

void CalMathFunA( int a, int b, FMath func )
{
	func( a, b );
}
void CalMathFunB( int a, int b, PFMath func )
{
	func( a, b );
}

void ( *GetFunc1( int flag ) )( int, int )
{
	switch ( flag )
	{
	case 1:
		return &Sum;
	case 2:
		return &Subtract;
	case 3:
		return &Multiply;
	default:
		return nullptr;
	}
}
FMath* GetFunc2( int flag )
{
	return GetFunc1( flag );
}
PFMath GetFunc3( int flag )
{
	return GetFunc1( flag );
}
auto GetFunc4( int flag ) -> void ( * )( int, int )
{
	return GetFunc1( flag );
}