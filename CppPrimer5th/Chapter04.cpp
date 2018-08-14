#include "stdafx.h"

#include <vector>
#include <bitset>	// ���������������λ����
#include <cstring>	// memcpy

#include "TestBase.h"
#include "TestHelper.h"
#include "Inheritance.hpp"


void Test_ExpressionBase()
{
	StartOneTest( "����������ʶ" );
	cout << "���ʽ�����������һ�����������������0�����������" << endl;
	cout << "��ֵ��������ֵ���棬��֮��һ������" << endl;
	cout << "һ���IO�����������\"<<\"��\">>\"�����������ɣ��������������β�������" << endl;
	cout << "��������㣬С���ͣ�bool��short��char����������Ϊ�����ͣ�int���ٲ������" << endl;

	StartOneTest( "���ʽ�е���ֵ˳�򲻿�Ԥ֪" );
	auto _add = []( double val1, double val2 ){
		cout << "function _add called" << endl;
		return val1 + val2;
	};
	auto _multiply = []( double val1, double val2 ){
		cout << "function _multiply called" << endl;
		return val1 * val2;
	};
	double d1 = 3, d2 = 2;
	double d3 = _add( d1, d2 ) + _multiply( d1, d2 );
	// ����������ʽ�����������������޷�Ԥ֪�ȵ����ĸ�������ͱ������й�ϵ��
	// ������������ڲ��漰����ͬ���ⲿ�����������ᷢ���仯�����տ��ܲ����޷�Ԥ֪�Ĵ�������
	// ���Ա��ʽ�а�������ǣ�����������ʱ����ò�ֳɶ�����ʽд��ȷ�������ͼ������⣻
	cout << "value of d3: " << d3 << endl;

	// C++��ֻ���ĸ���������Ա�֤�������ļ���˳��&&��||��?:��,��
	// ����&&��||������·��ֵ�������ȼ�����ߣ��ұ߿��ܸ��������㣻


	StartOneTest( "��ֵ�����" );
	// 1�����㡾�ҽ���ɡ��������ظ�ֵʱ�����������θ�ֵ�������뱣֤������ƥ��ģ��������ʽת����
	int a, b, c = 5, *p = &c;
	a = b = ++( *p );
	cout << a << " " << b << endl;

	// 2���������ȼ����ͣ����ȸ��ϸ�ֵ���ߣ����߼���������ͣ�
	// ���������������Ƕ�븳ֵ���㣬��á������š����������������
	int ri;
	while ( ( ri = Randi( 1, 20 ) ) <= 12 )
		cout << ri << "\tstill <= 12" << endl;
	cout << "�����ҵ�һ������12�������:\t" << ri << endl;
}

void Test_BitwiseOperator()
{
	using UL = unsigned long;

	StartOneTest( "��λ�����" );
	// 1�����㡾�����ɡ���
	// 2���Ƴ��߽�Ļᱻ����������
	// 3����������Ľ����Ƿ���������ġ���������������û�б仯��
	UL u1 = bitset< 8 >( "11111111" ).to_ulong(), u2;
	u2  = u1 >> 3 << 2 << 3;	// ������3λ��3��1��������������2λ�����������3λ
	cout << "u1 = " << u1 << ", u2 = " << u2 << endl;
	cout << "bits of u2 is : " << bitset< 10 >( u2 ).to_string() << endl;

	// ��λ�ĸ����������ֱ�����Լ�
	( ( u1 >>= 3 ) <<= 2 ) <<= 3;
	cout << "����������λ�����u1 = " << u1 << endl;

	// λ�����У�С�����Զ������ɴ�����
	bool bval = true;
	auto bshift = bval << 3;	// bool��true�Զ�����Ϊint��1
	cout << "bool bitwise shift : " << bshift << endl;


	StartOneTest( "λ�����ʵ��Ӧ��" );
	// 1�����������������롾01��ߡ��ĸ��������⣬��������򽫲���һ�ѱ�ߣ�������������ܣ�
	// ���Ͽ̶�Ϊ1��λ�ã������ȡ���Ĺ��ܣ��� 1^1=0; 1^0=1;
	// ���Ͽ̶�Ϊ0��λ�ã������ֵ�Ĺ��ܣ��� 0^1=1; 0^0=0;
	int a = -99, b = 250;
	a = a ^ b;	// ��ʱa�����һ�ѱ�ߣ���a��b��ͬ�ĵط���Ϊ1����ͬ�ĵط���Ϊ0
	b = a ^ b;	// �����b��򣬽���b�к�a��ͬ��λ��ȡ������Ȼ�õ���ԭ����a
	a = a ^ b;	// ͬ�������ԭ����a��򣬿��Եõ�ԭ����b
	cout << "a = " << a << ", b = " << b << endl;

	// �������治̫��������ѧϰһ���ٲ���λ���̣��������飿
}

void Test_OtherOperator()
{
	StartOneTest( "����׺�������������" );
	int a = 3, b;					// ��׺���������������������ص��Ǳ����ı�ǰ�ġ���������
	cout << ( b = a++ ) << endl;	// b = a++ �൱�� int tmp = a; a = a + 1; b = tmp;
	cout << b << endl;				// ��һ����ʱ�����Ŀ��������Գ��Ǳ�Ҫ�����á�ǰ׺�桿�ģ�


	StartOneTest( "��ⶺ�������" );
	// ���������������㣬�����Ҳ��ֵ��Ϊ��������ս����
	// ��������������ȼ���C++���ǡ���͵ġ����������û������
	int c = ( ++a, ( a + b ) );
	cout << "comma operator result: " << c << endl;


	StartOneTest( "sizeof�����" );
	// �����ָ�ʽ��sizeof( type )��sizeof expr���ڶ��ֲ�������������ʽ��ֵ��
	int i1 = 5;
	double dary[] = { 1.5, 2.4, 6.6 };

	size_t memberSize = sizeof TCPoint::x;	MCPP11	// �����ʽ��������

	// ���ء�������������ֽ������������м���'\0'��char[]���������������ָ��
	auto arySize = sizeof dary;

	// sizeof���ȼ������ڡ���ѧ����������Ը��ӱ��ʽע�⡾�����š�������ֻ���һ������
	auto sumSize = sizeof ( ++i1 + dary[ 1 ] );

	// �ṹ���ֽڶ��룺����ԭ���ǽṹ��Ĵ�С������������������͵���������
	// ��β�ͬ������������ϵͳ���ֽڵ�Ĭ�϶��루windows�����ǰ�8�ֽڶ��룩Ҳ��ͬ��
	// Ȼ�����������ǰ��˳��ҲӰ��ṹ��Ĵ�С��һ��Ӵ�С��������ȽϽ�ʡ�ռ䣻
	// ���Բ�Ҫ���Լ��ܲ�ĳ���ṹ��Ĵ�С����sizeof���б��ϣ�

//#pragma pack( push )	// VC++����������ı�ĳһ��������ṹ��Ķ��뷽ʽ
//#pragma pack( 1 )
	struct _IDPoint
	{
		double X;
		double Y;
		unsigned short ID;
		char Type;
	};
//#pragma pack( pop )

	auto pntSize = sizeof( _IDPoint );

	cout << "size of member variable:\t" << memberSize << endl 
		<< "size of array:\t\t\t" << arySize << endl 
		<< "size of int + double:\t\t" << sumSize << endl 
		<< "size of _IDPoint:\t\t" << pntSize << endl;
}

void Test_ImplicitConversion()
{
	StartOneTest( "��ʽ����ת����������ת��" );
	double da = 3.5, db;
	int ia = 3, ix = ia + da;
	cout << "1�����͡������ϵ���ʽ����������תΪ��Ӧ�ĸ�����: " << ix << endl;

	// С���Σ�bool, char, signed char, unsigned char, short, and unsigned short
	// ����������������int�������ֵ��Χ���Ļ���������unsigned int��
	// �����ʽ���и��㣬ת��˳���ǣ�С������תΪint��Ȼ��int��תΪ���㣻
	bool ba = true, bb = false;
	char ca = 'A';	// 'A'��ASCII����65
	
	db = ba + ia + bb + da + ca;	// 1 + 3 + 0 + 3.5 + 65
	cout << "2��С����������" << db << endl;

	// ��ʹ��ʽ�ﲢû�д����ͣ�Ҳ���Զ�����
	short sa = 100;
	auto del1 = sa + ca;
	PrintTypeName( del1 );
	cout << "3����С���͵���ʽ�����" << del1 << endl;


	StartOneTest( "��ʽ����ת������������ָ��" );
	// ��������£����������Զ�ת��Ϊ��һ��Ԫ�ص�ָ��
	int iary[ 10 ] = {};
	PrintTypeName( ( iary + 1 ) );


	StartOneTest( "��ʽ����ת������ͨ��ָ��" );
	// 1��0��nullptr����תΪ����ָ�����ͣ�
	///2������ָ�������void *��const void *��ת��
	double dc = 5, *pb = 0;
	TCPoint *ppnt = nullptr;
	void *pv1 = &dc;
	PrintTypeName( pb );
	PrintTypeName( ppnt );
	PrintTypeName( pv1 );


	StartOneTest( "��ʽ����ת������ת������" );
	// ������ָ�����Ͷ������Զ�������������0Ϊ�棬0Ϊ��
	if( dc )
		cout << "dc is nonzero" << endl;
	if( pv1 )
		cout << "pv1 is not null" << endl;


	StartOneTest( "��ʽ����ת������ת��const" );
	// �κη�const�Ķ���������תΪconst����仰Ӧ��û����ɣ���


	StartOneTest( "��ʽ����ת������������" );	// ���Ҫ�����Լ���δ�����
	TCPoint pnt1 = { 1, 2 };
	Coordinate coord1 = { 120, 31 };	// Coordinate�ڲ�ʵ������TCPoint��ת������
	TCPoint sumpnt = pnt1 + coord1;		// coord1������ʽת��ΪTCPoint�����ٽ������
	cout << "TCPoint + Coordinate : " << sumpnt << endl;
}

void Test_ExplicitConversion()
{
	StartOneTest( "�򵥴ֱ���static_cast" );
	// static_castֻ������ʱ���ͼ�飨dynamic_cast������ʱ��飩��������ʱ���²�����
	// ���Ҳ��Ϊ�գ���Ҫ����Ա��֤ת�������ǺϷ��ģ����縸���������ת����
	TCMan man{};
	TCPerson *person = &man;
	TCWomen *lady = static_cast< TCWomen* >( person );
	if( lady )
		cout << "ǿ��ת��ΪŮʿ�������Ϊ��" << endl;
	// ��ʱ������lady->HandbagBrand()����ֱ�ӱ���

	// �����ת��ʾ����
	void *vp = person;
	TCMan *pMan = static_cast< TCMan* >( vp );
	cout << "���˵ĺ��볤��Ϊ��" << pMan->BeardLength() << endl;
	

	StartOneTest( "const_cast" );
	cout << "const_cast�����ý�����ȥ��ֻ�������ܸı����ݵĻ�������" << endl;


	StartOneTest( "���Ӵֱ���reinterpret_cast" );
	// �ҵ�����ǽ�ԭ�еı����ڴ��ַ����һ��ָ���������͵Ľ��ͷ�����
	// �о�������ڴ������Щ�ã������л�����ʱ����һ���ڴ�Ľ��ͷ�ʽ�ǹؼ���

	// ����һ��100�ֽڵ��ڴ�أ�pMem��λ���α�
	char myMemPool[ 100 ] = {}, *pMem = myMemPool;

	// �����ڳ����һ��double��һ��int
	double dval = 9.9;
	int ival = -125;
	memcpy( pMem, &dval, sizeof( double ) );
	memcpy( pMem + sizeof( double ), &ival, sizeof( int ) );

	// ������reinterpret_cast�������ڴ���������
	double *dget = reinterpret_cast< double* >( pMem );
	cout << "double value from memory pool :\t" << *dget << endl;

	int *iget = reinterpret_cast< int* >( pMem + sizeof( double ) );
	cout << "int value from memory pool :\t" << *iget << endl;
}

void Test_ExpressionNew()
{
	StartOneTest( "�������Ľ������" );
	int a = -5, b = 3, c = 7;
	cout << "a / b = " << ( a / b ) <<	MCPP11	// C++11������׼����һ�ɡ���0ȡ����
		";\ta % b = " << ( a % b ) << 	MCPP11	// C++11������׼�������ķ��ź͡����ӡ�����һ��
		";\tc % a = " << ( c % a ) << endl;

	StartOneTest( "��ʼ���б������ţ�������Ϊ��ֵ" );
	TCPoint pnt1{};
	pnt1 = { 1.1, 3.3 };	MCPP11
	cout << pnt1 << endl;

	vector< int > vec( 3, 1 );
	vec = { 1, 2, 3, 4, 5 };
	for( auto &i : vec )
		cout << i << " ";
	cout << endl;
}

ChapterBase* Chapter04Init()
{
	ChapterBase *ch04 = new ChapterBase( "������ ���ʽ" );
	ch04->AddSection( 1, "��������", &Test_ExpressionBase );
	ch04->AddSection( 2, "λ����", &Test_BitwiseOperator );
	ch04->AddSection( 3, "���������", &Test_OtherOperator );
	ch04->AddSection( 4, "��ʽ����ת��", &Test_ImplicitConversion );
	ch04->AddSection( 5, "��ʽ����ת��", &Test_ExplicitConversion );
	ch04->AddSection( 6, "������", &Test_ExpressionNew );
	return ch04;
}