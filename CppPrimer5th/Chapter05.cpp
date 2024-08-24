#include <stdexcept>

#include "TestBase.h"
#include "TestHelper.h"
#include "TCVector.h"

using namespace std;

void Test_Ifelse()
{
	StartOneTest( "����else����" );
	// else�����������δƥ��if��ԣ��ӻ����ű�����ͼ���
	int ia = 3;
	if( ia % 2 )
		if( ia >= 5 )
			cout << "ia�Ǵ��ڵ���5������" << endl;
	else	// ��if( ia >= 5 )���
		cout << "ia = " << ia << endl;
}

void Test_Switch()
{
	StartOneTest( "case��ǩ�����ǡ����ͳ������ʽ��" );
	auto _testFactorial = []( const int &f )	// �ж�f�Ǽ��Ľ׳�
	{
		switch( f )
		{
		case 2:		// ����ֵ������Ȼ���ԣ�Ҳ������char���ͣ�һ���������ַ���
			cout << "Factorial of 2" << endl;
			break;
		case Factorial( 3 ):					// ��ע��constexpr�����Կ���
			cout << "Factorial of 3" << endl;
			break;
		case Factorial( 3 ) * 4:				// ��΢���ӵĳ������ʽҲ����
			cout << "Factorial of 4" << endl;
			break;
		default:	// ������������Ƿ��д��룬��һ��default��һ����ϰ��
			cout << "Factorial of number greater than 4" << endl;
			break;
		}
	};

	int a = 1, b = 2, c = 3, d = 4;
	_testFactorial( a * b );
	_testFactorial( a * b * c * d );
	_testFactorial( Factorial( 6 ) );
	

	StartOneTest( "case��ǩ�Ĺᴩԭ��" );
	// ĳ��case��ǩƥ��ɹ��󣬽��Ӵ�case����˳��ִ������case��֧��ֱ��break����switch������
	// ���ԣ��������������Ϊ����ƥ�䣬����ǵü�break��
	auto _testScore = []( const int &s )
	{
		switch( s )
		{
		case 1:
			cout << "1����Ҫ����" << endl;	// ����Ӧ����һ��break��������򽫼������¹ᴩ
		case 2: case 3:		// �ᴩ��case���Բ�����
			cout << "2~3����Ҫ����" << endl;
			break;
		case 4: case 5:
			cout << "4~5������" << endl;	// ������Ȼ������switch�Ľ���������break���Ǹ���ϰ��
		}
	};
	_testScore( 1 );
	_testScore( 4 );


	StartOneTest( "case��ǩ�еı���" );
	// ��һ����ǩ���岢��ʼ��һ����������������һ����ǩʹ��;
	// ��case�Ĵ����ж���ʱ���û����Ű������ǱȽϺõ�ϰ�ߣ�ʡ�ܶ��鷳��
	auto _CalFactorial = []( const int &a )	// ����f�Ľ׳�
	{
		switch( a )
		{
		case 3:
			int f;	// �������f���κγ�ʼ����Ϊ����case 4�о��޷�ʹ���ˣ������������
			f = Factorial( 3 );
			cout << "Factorial of 3 is " << f << endl;
			break;
		case 4:
			f = Factorial( 4 );
			cout << "Factorial of 4 is " << f << endl;
			break;
		}
	};
	_CalFactorial( 3 );
	_CalFactorial( 4 );
}

void Test_Iterative()
{
	StartOneTest( "range for��ԭ��" );
	// range for�ǿ���ȡ������ͷβ��������Ȼ���ͷ��β���������������á����ظ����ö˵ģ�
	// һ�������࣬���������begin���͡�end�����������ֵĺ�����������range for���η���Ԫ�أ�
	TCVector< int > myVec;
	for( int i = 0; i < 5; ++i )
		myVec.PushBack( Randi( 1, 100 ) );

	// ��range for��ӡԪ�أ����ұ�ע�͵Ĵ����ǵȼ۵�
	for( auto &i : myVec )	// for( auto i = myVec.begin(); i != myVec.end(); ++i )
		cout << i << endl;	//	cout << *i << endl;

	StartOneTest( "do-while�б���ʹ��ϸ��" );
	int ri;
	do
	{
		// 1��do�������ı����������ڽ���do����ڣ�while����ʹ�ã�����ri��Ҫ�����涨�壻
		ri = Randi( 1, 10 );
		cout << ri << endl;
	}
	while( ri > 5 );	// ������';'

	int ri2;
	do
		cout << "�������Ի�ȡ0-1�����" << endl;
	while( ri2 = Randi( 0, 1 ) );	// 2��while�в��ܶ����������Ҫʹ�õĻ������涨�壻
}

void Test_Jump()
{
	StartOneTest( "goto������" );
	// 1����������һ�����岢��ʼ���ı��������ں������ʹ������
	// �����case��ǩ�еı����������ƣ�
	goto CodeEnd;
	int ri;	// ����ֻ�������������г�ʼ����Ϊ
	ri = Randi( 1, 10 );

CodeEnd:	// ��eclipse�����ȷʵ����������VC++ 2015��Ȼ������������
	ri = Randi( 50, 100 );
	cout << ri << endl;


	StartOneTest( "goto������" );
	// 2������ʱ�������൱�����´���һ�Σ����û�������ʵ��ѭ����
CodeBegin:
	int ri2 = Randi( 1, 10 );
	if( ri2 < 7 )
	{
		cout << ri2 << " < 7, continue." << endl;
		goto CodeBegin;
	}
	else
		cout << ri2 << " > 7, end." << endl;
}

void Test_Exception()
{
	StartOneTest( "exception safe" );
	// ���쳣����ʱ��ִ�б�Ҫ����������˵���ˣ���������Ϊ�쳣��ȫ��
	TCVector< int > ivec;
	try
	{
		ivec = { 1, 2, 3, 4, 5 };
		cout << ivec[ 7 ] << endl;
		ivec.Clear();		// �������߼�����ӡ��Ҫ����������֮ǰ�ܿ�����Ϊ�쳣����ǰ��ֹ
	}
	catch ( const out_of_range &ex )
	{
		cout << ex.what() << endl;
		if( ivec.Size() )	// ���Բ����쳣�����ȼ������״̬�����������Ҫ������
			ivec.Clear();
	}


	StartOneTest( "throw and catch" );
	// 1��������std�쳣��һ����Ƕ�����ͷ�ļ�<stdexcept>�е�
	try
	{
		size_t buflen = 1024 * 1024 * 1024;
		double *buf = new double[ buflen ];
	}
	catch( const bad_alloc &ex )	// ��������8G���ڴ棬������bad_alloc
	{
		cout << ex.what() << endl;
	}

	try
	{
		string str = "abc";
		cout << str.substr( 4, 10 ) << endl;
	}
	catch( const out_of_range &ex )	// ��ȡ������Χ���ַ�����������out_of_range
	{
		cout << ex.what() << endl;
	}

	// 2�������쳣����std���쳣һ���ǻ���std::exception�࣬�Զ����쳣ҲӦ�ü̳�����ࣻ
	try
	{
		// ����������һ�����֡��ַ����ַ����ȵ�
		throw string( "����׸��ַ����쳣" );
	}
	catch( const std::exception &ex )	// ��ȷ�������쳣�࣬�Ͳ������
	{
		cout << ex.what() << endl;
	}
	catch( const string &errmsg )	// ���ǰ���׵��������ַ���������Ӧ����char *
	{
		cout << errmsg << endl;
	}
	catch( ... )
	{
		// ʡ�Է����Բ������е�C++�쳣����ϵͳ���쳣�޷����񣬱�������Խ��ȵȣ�
		// try-catch��������ǰ�����C++����throw�����쳣����������ϵͳ�ײ�Ĵ���
		// ������Ҫ�����ʶȵ�������״̬��⣬�ٱ�дҵ���߼���
		// Ȼ�����Ǻ��ѵģ�����Ҫȡ��ģ������ⷢ��ʱ��Ҫ���ûع����Լ���ʱ��ӡ��־��
		cout << "unknown error occurred" << endl;
	}
}

void Chapter05Init()
{
	auto ch05 = ChapterBase::AddChapter( 5, "������ ���" );
	ch05->AddSection( 1, "�������", &Test_Ifelse );
	ch05->AddSection( 2, "switch���", &Test_Switch );
	ch05->AddSection( 3, "�������", &Test_Iterative );
	ch05->AddSection( 4, "��ת���", &Test_Jump );
	ch05->AddSection( 5, "�쳣", &Test_Exception );
}
static int _Init = ( Chapter05Init(), 0 );