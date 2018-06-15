#include "stdafx.h"
#include "Chapter02.h"
#include "TestHelper.h"
#include <vector>
#include <map>

int _VOutOfFunction;   // ��������ı���������˵���������
					   // ������_��ͷ��_��д��ĸ��ͷ��������û��
extern int VChapter02extern = 2;   // �г�ʼ�����Ƕ��壬�����ں����ڳ�ʼ��

void Chapter02::Run()
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
	unsigned u1 = 10, u2 = 40;
	int i = -42;
	cout << u1 + i << endl << u1 * i << endl;   // i��תΪ�޷����ټ���
	
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

	unsigned int u3 = 245u;
	long l1 = 45342342L;
	cout << u3 << " " << l1 << endl;

	float f1 = 3.123456789101112131415f;
	double d1 = 3.123456789101112131415L;   // �����׺��Ϊf���򾫶Ȼ��Զ���ʧ

	// Ĭ�����������6��������ͷ�ļ�<iomanip>�е�setprecisionҲ��������
	streamsize dft_precision = cout.precision( 15 );
	cout <<  f1 << "\t" << d1 << endl;
	cout.precision( dft_precision );
	

	StartOneTest( "C++11�б��ʼ��" );
	int i5{ 567 }; MCPP11   // C++11�б��ʼ�����ϸ����ͱ���ƥ�䣬���ﲻ����С��
	map< string, TSPoint > map1 = { { "point1", { 1.1, 2.2 } }, 
									{ "point2", { 2.2, 3.3 } } };
	cout << i5 << endl;
	for( auto &i : map1 ) MCPP11
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
	

	StartOneTest( "ȫ�ֱ��������Ͷ������" );
	int VChapter02extern = 5;   // �ֲ��Ḳ��ȫ�֣�����ȫ�ֵĻ���Ҫ��::��ʽ����
	cout << VChapter02extern << " " << ::VChapter02extern << endl;
	

	// �������������غ�����void foo( int i );	void foo( char *p );
	// ���� nullptr ��Ŀ���Ǳ�������ԣ���ȥ��NULL��0�ȼۣ�
	// ����foo( NULL )ʱ�ͻ�������������ڵ���foo( nullptr )����ȷ�ص��õڶ�������
	StartOneTest( nullptr );MCPP11


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


	StartOneTest( "const�ļ���ϸ��" );
	// ��ͬ�ڳ������ã�const���ÿ��԰󶨲�ƥ�����͵ı���(r7)�����ʽ(r8)������ֵ(r9)��
	// ԭ��������������һ�����ɼ�����ʱ���䣩����const������ʵ�󶨵��������ʱ����
	double d2 = 3.14;
	int i8= 10;
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
}
