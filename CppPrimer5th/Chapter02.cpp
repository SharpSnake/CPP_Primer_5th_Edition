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
	char c1 = 127;
	signed char c2 = 127;
	unsigned char c3 = 255;
	cout << ( int )c1 << " " << ( int )c2 << " " << ( int )c3 << endl;
	FinishOneTest( "char��������ص�����" );


	// long��intһ������±�������ͬ�ģ�
	// ��Ҫʹ����������������������ڼ��㣬�����С��޷��Ż���Ӽ��˳�

	// ����˵��һ���з������͸�������Χ�������ᵼ��δ��������
	// ��ʵ���������ǰ���������
	signed char c4 = 257;
	cout << ( int )c4 << endl;
	FinishOneTest( "�з���char�������" );


	unsigned u1 = 10, u2 = 40;
	int i = -42;
	cout << u1 + i << endl;   // i��תΪ�޷����ټ���
	cout << u1 * i << endl;   // ͬ��
	
	// ��������޷��ż��������ֵ������ᰴ�������
	// unsigned maximum = 4294967295, u1 - u2 = -30, 
	// window will prints: 4294967295 - 29
	cout << u1 - u2 << "( 4294967295 - 29 = " << 4294967295 - 29 << " )" << endl;
	FinishOneTest( "�С��޷������͹�ͬ�������" );


	int i2 = 20, i3 = 024, i4 = 0x14;
	cout << i2 << " " << i3 << " " << i4 << endl;
	FinishOneTest( "ʮ���ơ��˽��ơ�ʮ�����Ƴ�����ʾ" );

	cout << '\\' << '\t' << '\'' << '\"' << endl;
	FinishOneTest( "�����ַ���Ҫ��ת�����б�ʾ" );


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
	FinishOneTest( "������������ֵ��ǰ��׺����" );


	int i5{ 567 }; MCPP11   // C++11�б��ʼ�����ϸ����ͱ���ƥ�䣬���ﲻ����С��
	map< string, TSPoint > map1 = { { "point1", { 1.1, 2.2 } }, 
									{ "point2", { 2.2, 3.3 } } };
	cout << i5 << endl;
	for( auto i = map1.begin(); i != map1.end(); ++i ) MCPP11
		cout << i->first << ": " << i->second << endl;
	FinishOneTest( "C++11�б��ʼ��" );

	int vInFunction = 7;
	// ����������ʹ��vInFunction�����ʧ�ܣ������ڱ�������Ĭ�ϳ�ʼ��
	cout << _VOutOfFunction << endl;
	FinishOneTest( "�������������Ĭ�ϳ�ʼ��" );


	int VChapter02extern = 5;   // �ֲ��Ḳ��ȫ�֣�����ȫ�ֵĻ���Ҫ��::��ʽ����
	cout << VChapter02extern << " " << ::VChapter02extern << endl;
	FinishOneTest( "ȫ�ֱ��������Ͷ������" );
}
