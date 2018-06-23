#include "stdafx.h"
#include "Chapter02.h"
#include "TestHelper.h"
#include <vector>
#include <map>

using UI = unsigned int;	MCPP11	// ��������

int _VOutOfFunction;   // ��������ı���������˵���������
					   // ������_��ͷ��_��д��ĸ��ͷ��������û��
extern int VChapter02extern = 2;   // �г�ʼ�����Ƕ��壬�����ں����ڳ�ʼ��


// ��������
void Test_BaseType()
{
	// char��������ص����ͣ�����char�����ܱ�֤���з��ŵģ�
	// ����ʹ�������ȷ����Ҫʹ���У��ޣ����ţ���ñ���
	StartOneTest( "char��������ص�����" );
	char c1 = 127;
	signed char c2 = 127;
	unsigned char c3 = 255;
	cout << ( int )c1 << " " << ( int )c2 << " " << ( int )c3 << endl;


	// long��intһ������±�������ͬ�ģ�
	// ��Ҫʹ����������������������ڼ��㣬�����С��޷��Ż���Ӽ��˳�

	// ����˵��һ���з������͸�������Χ�������ᵼ��δ��������
	// ��ʵ���������ǰ���������
	StartOneTest( "�з���char�������" );
	signed char c4 = 257;
	cout << ( int )c4 << endl;


	StartOneTest( "�С��޷������͹�ͬ�������" );
	UI u1 = 10, u2 = 40;
	int i1 = -42;
	cout << u1 + i1 << endl << u1 * i1 << endl;   // i��תΪ�޷����ټ���
	
	// ��������޷��ż��������ֵ������ᰴ�������
	// unsigned maximum = 4294967295, u1 - u2 = -30, 
	// window will prints: 4294967295 - 29
	cout << u1 - u2 << "( 4294967295 - 29 = " << 4294967295 - 29 << " )" << endl;


	StartOneTest( "ʮ���ơ��˽��ơ�ʮ�����Ƴ�����ʾ" );
	int i2 = 20, i3 = 024, i4 = 0x14;
	cout << i2 << " " << i3 << " " << i4 << endl;
	
	StartOneTest( "�����ַ���Ҫ��ת�����б�ʾ" );
	cout << '\\' << '\t' << '\'' << '\"' << endl;
	

	StartOneTest( "������������ֵ��ǰ��׺����" );
	// ������������ֵ��ǰ��׺����
	const char *cs1 = u8"utf8�ַ���";   // u8����ռ�ֽ�����Ĭ��Ҫ��
	const wchar_t *wcs1 = L"���ַ���";
	cout <<"utf8�ַ�������Ϊ��" << strlen( cs1 ) << endl;
	cout << "���ַ�������Ϊ��" << wcslen( wcs1 ) << endl;

	UI u3 = 245u;
	long l1 = 45342342L;
	cout << u3 << " " << l1 << endl;

	float f1 = 3.123456789101112131415f;
	double d1 = 3.123456789101112131415L;   // �����׺��Ϊf���򾫶Ȼ��Զ���ʧ

	// Ĭ�����������6��������ͷ�ļ�<iomanip>�е�setprecisionҲ��������
	streamsize dft_precision = cout.precision( 15 );
	cout <<  f1 << "\t" << d1 << endl;
	cout.precision( dft_precision );

	StartOneTest( "ȫ�ֱ��������Ͷ������" );
	int VChapter02extern = 5;   // �ֲ��Ḳ��ȫ�֣�����ȫ�ֵĻ���Ҫ��::��ʽ����
	cout << VChapter02extern << " " << ::VChapter02extern << endl;
	

	// �������������غ�����void foo( int i );	void foo( char *p );
	// ���� nullptr ��Ŀ���Ǳ�������ԣ���ȥ��NULL��0�ȼۣ�
	// ����foo( NULL )ʱ�ͻ�������������ڵ���foo( nullptr )����ȷ�ص��õڶ�������
	StartOneTest( nullptr );	MCPP11
}

// ������ʼ��
void Test_Initialization()
{
	StartOneTest( "C++11�б��ʼ��" );
	int i5{ 567 };	MCPP11   // C++11�б��ʼ�����ϸ����ͱ���ƥ�䣬���ﲻ����С��
	map< string, TSPoint > map1 = { { "point1", { 1.1, 2.2 } }, 
									{ "point2", { 2.2, 3.3 } } };
	cout << i5 << endl;
	for( auto &i : map1 )	MCPP11
		cout << i.first << ": " << i.second << endl;
	

	StartOneTest( "���������Ĭ�ϳ�ʼ��" );
	int vInFunction;
	// ����������ʹ��vInFunction�����ʧ�ܣ������ڱ�������Ĭ�ϳ�ʼ��
	cout << _VOutOfFunction << endl;

	// ���ṹ��ĳ�Ա��������Ĭ�ϳ�ʼ����Ĭ�϶��ǲ���Ԥ֪��ֵ��
	TSPoint sp1;		// �����ڹ��캯����ָ����ʼֵ���������б��ʼ�������ǿյ�{}��
	TCPoint cp1;		// ��Ա����Ҳ�����Ĭ�ϳ�ʼ������Ȼ����Ծۺ��ࣩ����ʼ������
	QuadraticPoly qp1, qp2{};
	cout << sp1 << endl << cp1 << endl << qp1 << endl << qp2 << endl;
}

// ��������
void Test_CompoundTypes()
{
	StartOneTest( "��������" );
	// ����������������� = basetype + ��������
	// ������ = �������η���*��&�ȣ� + ������
	// ���͵Ľ���ھ�����������
	//
	// r6�Ľ����������������������'&'��˵��r6��������һ�����ã���������'*'��
	// ˵��r6��һ��ָ���������ã��������ǻ�������int��˵��r6��һ������ָ���������ã�
	// r6��Զ��p6�����ã�������ͨ��r6��ӵ��޸�p6ʵ��ָ������֣�
	int i6 = 50, i7 = 100, *p6 = &i6, *&r6 = p6;
	*p6 += 5;
	cout << "p6��ʼָ��i6��ͨ���������޸�i6��ֵ��" << i6 << endl;
	
	r6 = &i7;
	cout << "ͨ��ָ������ã�����޸���p6��ָ������p6ָ�������ֵ�ǣ�" << *p6 << endl;
}

// ���������Ľ׳�
constexpr int _CalFactorial( int n )	MCPP11
{
	/*int f = 1;
	for( int i = n; i >= i; --i )
	f *= i;
	return f;*/	// C++14
	return n <= 1 ? 1 : ( n * _CalFactorial( n - 1 ) );
}

// const���
void Test_Const()
{
	StartOneTest( "const�ļ���ϸ��" );
	// ��ͬ�ڳ������ã�const���ÿ��԰󶨲�ƥ�����͵ı���(r7)�����ʽ(r8)������ֵ(r9)��
	// ԭ��������������һ�����ɼ�����ʱ���䣩����const������ʵ�󶨵��������ʱ����
	double d2 = 3.14;
	int i7 = 100, i8= 10;
	const int &r7 = d2,		/* int tmp_d2 = ( int )d2; */
		&r8 = ( d2 + i8 ),	/* int tmp_exp = ( int )( d2 + i8 ); */
		&r9 = 5.5;			/* int tmp_num = ( int )5.5; */
	cout << r7 << " " << r8 << " " << r9 << endl;

	const int *p7 = &i8;	// ָ������ָ�룺�����޸ı�ָ������ֵ��������������Ƿ���const��
	int *const p9 = &i8;	// ����ָ�룺��ָ����Զָ��ĳһ�����󣬲��ܱ����

	p7 = &i7;		// correct��p7����ָ����i7
	// &p7 = 12;	// error

	*p9 += 5;		// correct��p9��ָ���ܱ䣬����ָ������ֿ��ԸĶ�������i8��ֵ����5
	// p9 = &i7;	// error
	cout << *p7 << " " << i8 << endl;


	StartOneTest( "�¹ؼ���\"constexpr\"" );
	// ��������⣺
	// const������ǿ��������ʱ���Ǳ����ڳ�����
	// constexpr�������ı������ߺ����������ڱ����ھ��ܼ���������ֵ��
	//	����C++14�зſ���constexpr�����ڲ��Ĵ���淶������ʹ��ѭ������ʱ�����ȣ�
	//	��C++11Ҫ��ֻ��һ��return��䣨�������õݹ飩��
	int ary[ _CalFactorial( 4 ) ] = { 1, 2, 3 };	MCPP11
	for( auto i : ary )
		cout << i << " ";
	cout << endl;

	// constexpr������һ���ŵ��ǿ���ʵ���Զ����������ֵ����Щ����ֵ�����ڸ��ٵ�ֻ���ڴ�����
	// ���Լӿ����������ٶȡ����磬���山�����Ϻ������������ֵ��Ȼ����������е㣬ͬ��������ֵ��
	constexpr  Coordinate beijing( 116.398277, 39.909214 ), shanghai( 121.458206, 31.116520 );

	// ��Visual Studio�У�����Ҫ����������������������ϣ����ɿ�������ļ�������
	constexpr  Coordinate midOfTwoCity = MidPoint( beijing, shanghai );
	cout << "������" << beijing << endl << "�Ϻ���" << shanghai << endl 
		<< "�����е㣺" << midOfTwoCity << endl;
}

void Test_AutoType()
{
	// ���ڲ��Եı���
	int i1 = 5, i2 = 9, &r1 = i1, *p1 = &i1, *const p2 = p1;
	const int ci1 = i1, &cr1 = ci1, *cp1 = &ci1;

	StartOneTest( "�ؼ���auto" );
	// �����˵��auto�Ƚ�"����"�������top-const���������ͣ���Ҫ�ֶ����������η�
	auto au1 = r1;			PrintTypeName( au1 );// int
	auto &au2 = r1;			PrintTypeName( au2 );// int &
	auto au3 = p1;			PrintTypeName( au3 );// int *
	
	auto au4 = cr1;			PrintTypeName( au4 );// int
	const auto au5 = cr1;	PrintTypeName( au5 );// const int
	auto au6 = cr1;			PrintTypeName( au6 );// int
	auto &au7 = cr1;		PrintTypeName( au7 );// const int &������ci1��������low-const
	auto au8 = p2;			PrintTypeName( au8 );// int *��������p2��top-const
	auto au9 = cp1;			PrintTypeName( au9 );// const int *��ָ��cil��������low-const
	/* auto�ܽ᣺���ϣ������const�����������ͣ���Ҫ�ֶ�������η�����ָ��auto�Զ��Ƶ� */

	StartOneTest( "�ؼ���decltype" );
	// ���auto��decltype��Ӧ�ø��ӹ㷺���Ҹ�"ʵ������"���ᱣ��top-const���������ͣ�
	// decltype�������ȥ������ʽ��ֵ������ñ��ʽ�еĺ������������Ƶ���������ͣ�
	decltype( r1 ) dt1 = i1;MCPP11	PrintTypeName( dt1 );// int &
	decltype( p2 ) dt2 = &i1;		PrintTypeName( dt2 );// int *const
	decltype( cr1 ) dt3 = i2;		PrintTypeName( dt3 );// const int &
	decltype( r1 * 0.5F ) dt4;		PrintTypeName( dt4 );// float
	decltype( MidPoint( { 1, 3 }, { 2, 4 } ) ) dt5{ 0, 0 };	PrintTypeName( dt5 );// Coordinate

	// ������һЩ�����������Ҫ�μǣ�
	// 1��ָ��Ľ����ò�����������������ͣ����Ա����ʼ����
	decltype( *p1 ) dt6 = i2;		PrintTypeName( dt6 );// int &
	
	// 2��������������������������������ͣ����Ա����ʼ����
	decltype( ( i1 ) ) dt7 = i2;	PrintTypeName( dt7 );// int &

	// 3����ֵ���ʽ��������������ͣ����Ա����ʼ����
	decltype( i2 = i1 ) dt8 = i2;	PrintTypeName( dt8 );// int &
}

Chapter02::Chapter02()
{
	m_Title = "�ڶ��� �����ͻ�������";
	m_TestCases[ 1 ] = SectionTest( "��������", &Test_BaseType );
	m_TestCases[ 2 ] = SectionTest( "������ʼ��", &Test_Initialization );
	m_TestCases[ 3 ] = SectionTest( "��������", &Test_CompoundTypes );
	m_TestCases[ 4 ] = SectionTest( "const���", &Test_Const );
	m_TestCases[ 5 ] = SectionTest( "�Զ�����", &Test_AutoType );
}
