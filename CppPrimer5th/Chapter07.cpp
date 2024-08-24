#include "Chapter07.h"
#include "TestBase.h"


using namespace std;

// ��ͨ��̬��Ա�ĳ�ʼ���������������ڳ�ʼ����һ����cpp��
JuniorClass::ClassSize JuniorClass::MaxNumberOfStudents = 100;

// constexpr��̬��Ա���Ҳ���ⲿ����һ�£���Ȼ��ǿ�󣬵����Ǹ���ϰ��
constexpr size_t JuniorClass::NumberOfLesson;


void Test_UnderstandingClass()
{
	StartOneTest( "��������" );

	CONSOLE_COLOR_AUTO_RESET;
	SetConsoleTextColor( CmdColor_Yellow );

	cout << "1����Ա������ʵ��ԭ���������ΪC++��û�г�Ա������ֻ�нṹ���ȫ�ֺ�����������\r\n"
		"��ʽ����������һ����Ϊthis�Ĳ��������this���Ƕ����ָ�롣��Ա�������ڶԳ�Ա"
		"�ĵ��ã���ʵ������ʽ��ͨ��this->�����ã�" << endl2;
	
	cout << "2��const��Ա�������������Ƕ�this�����Σ�" << endl2;
	
	cout << "3����ı��벽�裺��1����Ա��������2�������壻\r\n"
		"������Ա�����ͳ�Ա���⣩������˳������ν�Ⱥ󣬺������ڵ��ó�Աʱ����������֪�ˣ�" << endl2;
	
	cout << "4����ʼֵ�б��г�Ա��ʵ��ֱ�ӳ�ʼ����һ�������Ǿ����ڹ��캯����Գ�Ա���и�ֵ��\r\n"
		"�൱�ڳ�Ա��Ĭ�ϳ�ʼ���ٸ�ֵ���������Ͻ�����һ�������������Ƽ�ʹ�ó�ʼֵ�б�" << endl2;
	
	cout << "5���ۺ���4Ҫ�أ�\r\n��1�����г�Ա��public��\r\n��2��û���κι��캯����\r\n"
		"��3��û�����ڳ�ʼֵ��\r\n��4��û�л�����麯����\r\n�����б��ʼ����ע���Ա˳��" << endl2;
}

void Test_Constructor()
{
	StartOneTest( "���ϳɣ�Ĭ�Ϲ��캯��" );
	// �����û���κι��죬������������һ�����ϳ�Ĭ�Ϲ��족��������default�ؼ���
	// ��ʽ����һ��Ĭ�Ϲ��죻��Աִ�С�Ĭ�ϳ�ʼ������û�����ڳ�ʼֵ�ĳ�Ա���ܻ�δ���壻
	Student s1;	// ע�⣺����ʹ��Ĭ�Ϲ��죬�������桾����Ҫ�����š�
	cout << s1 << endl;

	StartOneTest( "��������Եĳ�Ա" );
	// ������û�г�ʼ���ĳ�Ա����ʹ�����ڳ�ʼֵ������ִ��Ĭ�ϳ�ʼ�������ܵ���δ���壻
	Student s2( "С��" );	// �������û�г�ʼ������
	cout << s2 << endl;

	StartOneTest( "���в�������Ĭ��ʵ��=Ĭ�Ϲ���" );
	JuniorClass jc;
	cout << jc << endl;

	StartOneTest( "ί�й��캯��-Delegating Constructor" );
	Student s3( "����", 15 );
	cout << s3 << endl;


	StartOneTest( "���캯������ʽ����ת��" );
	Student s4, s5, s6;
	TCPerson p1( 14, "Сǿ" ), p2( 13, "С��" );

	s4 = static_cast< Student >( string( "��÷÷" ) );
	s5 = p1;
	s6.Swap( p2 );
	cout << s4 << endl2 << s5 << endl2 << s6 << endl;
}

void Test_MemFunction()
{
	StartOneTest( "inline��Ա����" );
	// ���ڻ����ⶨ��ĳ�Ա��������������inline��ǰ���Ǻ����������һ���ļ��У�һ�㶼��ͷ�ļ��У�
	// inline��������û���������ģ��Ƽ���һЩ�����ظ��Ĵ������ɡ�С��inline������������ά��
	JuniorClass t1( "�������" );
	t1.Add( { "С��", 12, 10 } ).Add( { "С��", 13, 7 } );
	cout << t1 << endl;
	cout << endl << t1.Get( 1 ) << endl;

	StartOneTest( "ͬ��const����������" );
	Student &s1 = t1.Get( 0, true );
	const Student &s2 = t1.Get( 1, true );	// ��ʦ����const�ģ�������Ȼ����Get�ķ�const����
	
	const JuniorClass &rt1 = t1;
	const Student &rs2 = rt1.Get( 1, true );
}

// ����ֵ����Ҳ������ȫ�ֵ�
constexpr Placemark HuangHeLou( 119, 32, 90, "�ƺ�¥" );

void Test_Features()
{
	StartOneTest( "���ͳ�Ա" );
	JuniorClass t1( "�������", 50 );
	JuniorClass::ClassSize numStudents = t1.NumberOfStudents();
	cout << "�������ѧ��������" <<  numStudents << endl;


	StartOneTest( "�ɱ����ݳ�Ա" );
	// �ؼ���mutable���εĳ�Ա��������ʹ��const��Ա������Ҳ�ɸ���
	// mutable������һ���ô��ǣ�Lambda���ʽ��ʹ�����÷��ʵ��ⲿ�������ɸ��ģ�
	const JuniorClass t2( "һ������", 60 );
	t2.Get( 3, true ); t2.Get( 9, true ); t2.Get( 45, true );
	cout << "����ʦ��ѧ�������ʴ�����" << t2.VisitTime() << endl;


	StartOneTest( "��Ԫ����" );
	Student s1( "С��", 12, 5 );
	cout << s1 << endl;	// ��׼�����Student����Ԫ����

	JuniorClass j1, j2;
	j1.Add( s1 ).Add( { "С��", 12, 10 } );
	j2.Add( { "С��", 13, 7 } ).Add( { "С��", 11, 5 } );
	cout << "��Ԫ�����˽�г�Ա:\t" << j1.StudentName( 1 ) << endl;
	
	School sh1;		// ѧУ����Խ���༶��ֱ�ӷ���ѧ������Ϣ
	sh1.Add( j1 ).Add( j2 );
	cout << "��Ԫû�д�����:\t\t" << sh1.StudentName( 1, 0 ) << endl;

	unsigned oldID = sh1.ModifyStudentID( 1, 1, 19 );
	cout << "��Ԫ��Ա����ʾ����" << endl << "OldID: " << oldID << endl
		<< "NewID:" << sh1.Get( 1 ).StudentID( 1 ) << endl;


	StartOneTest( "��̬��Ա" );
	cout << j2 << endl2;

	j2.Resize();	// ��̬��Ա��ΪĬ��ʵ��
	cout << j2 << endl;

	StartOneTest( "����ֵ����" );
	constexpr Placemark TianAnMen;
	constexpr Placemark WangFuJing( 120, 35, 250, "������" );
	cout << TianAnMen << endl2 << WangFuJing << endl2 << HuangHeLou << endl;
}

void Chapter07Init()
{
	auto ch07 = ChapterBase::AddChapter( 7, "������ �����" );
	ch07->AddSection( 1, "��������", &Test_UnderstandingClass );
	ch07->AddSection( 2, "���캯��", &Test_Constructor );
	ch07->AddSection( 3, "��Ա����", &Test_MemFunction );
	ch07->AddSection( 4, "���һЩ��Ҫ����", &Test_Features );
}
static int _Init = ( Chapter07Init(), 0 );

/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
ostream& operator <<( ostream &o, const Student &obj )
{
	o << "Name:\t" << obj.m_Name << endl
		<< "Age:\t" << obj.m_Age << endl
		<< "ID:\t" << obj.m_ID;
	return o;
}