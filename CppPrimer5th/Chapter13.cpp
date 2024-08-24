#include <utility>	// std::move
#include <iterator>	// make_move_iterator
#include <algorithm>	// std::copy

#include "TestBase.h"
#include "TestHelper.h"
#include "TCVector.h"

using namespace std;
using std::swap;

// �����������������
TCVector< int > MakeRandiVec( size_t, int imin = 1, int imax = 100 );

void Test_CopyBase()
{
	StartOneTest();

	CONSOLE_COLOR_AUTO_RESET;
	SetConsoleTextColor( CmdColor_Yellow );

	cout << "1��default���ν����ڿ������ƺ����������ں�����������\r\n"
		"Ҳ�ɽ������ⲿ���崦���Σ������Ļ�����������inline��" << endl2;

	cout << "2��delete�����ں������������������������κγ�Ա����" << endl2;

	cout << "3�������������Ϊdelete����ô������Կ���new��"
		"���޷��ͷţ�error when delete ptr��" << endl2;

	cout << "4����Ա��Ĭ�Ͽ������ƣ����졢��������ֵ�������������delete�ģ�\r\n"
		"��ô���Ӧ�ĺ���Ҳ��delete�ģ�" << endl2;

	cout << "5���ຬ�����á�const��Ա������ĺϳɹ���ͺϳ�=��delete�ģ�" << endl2;
}

void Test_CopyAssign()
{
	StartOneTest( "�������죺��һ������Ϊ������������" );
	// ������������������������붼��Ĭ��ʵ��
	TCVector< int > vec1 = { 1, 2, 3 };
	TCVector< int > copy_vec1 = vec1;	// or copy_vec1( vec1 )
	cout << copy_vec1 << endl;

	StartOneTest( "��ʱ��ʹ�ÿ������죿" );
	// �б��ʼ������ʱ�����������ܻ��Ż����룬���������������죬ֱ��ʹ����һ�����죻
	// ������Ҳ����ɾ���������죬���۱������Ƿ��Ż������ǲ������������������������ڣ�
	Coordinate coords[ 3 ]{ Coordinate( 3, 5 ), Coordinate( 1, 9 ) };
	cout << coords << endl;

	// �����������������ô��ݲ�������������ֵ�ȵȣ�Ҳ���õ���������;

	StartOneTest( "���ϳɣ�������ֵ�����һ��Ҫ�������������" );
	// �����ڶ��ظ�ֵ����ֵ���������жϵȳ����л��õ���
	Coordinate coord1, coord2, coord3;
	coord1 = coord2 = Coordinate( 1.5, 3 );
	if( coord3 = coord1 )
		cout << coord3 << endl;
	
	// ������ֵҪ��֤�Ը�ֵ������������
	TCVector< string > vec2{ "abc", "def", "ghi" };
	vec2 = vec2;
	cout << "�Ը�ֵ��" << endl << vec2 << endl;
}

void Test_RvalueMove()
{
	StartOneTest( "��ֵ����(Rvalue References)" );
	// ����ֻ�ܰ󶨵���ֵ�ϣ���ָ��Ķ����ŵ������ʧ�����Զ�ȡ������Դ
	string s1 = "abc", s2 = "123";
	string &&rrs1 = s1 + "def";	MCPP11
	// string &&rs2 = rs1;	// ERROR! ���������ܡ�ֱ�Ӱ󶨵�һ����������ʹ����һ����������
	string s3( std::move( rrs1 ) ), 
		s4( std::move( s2 ) );	MCPP11	// ʹ�ñ�׼�ⷽ�����һ��������s2�������
	cout << s2 << ", " << s3 << ", " << s4 << endl;

	StartOneTest( "�ƶ��������쳣" );
	// �����ǲ����쳣�ģ���C++11�¹ؼ���noexcept����
	// ��noexcept�ı�Ҫ�ԡ�������Ŀ������е�һ��������쳣��������;�ع�������ֵ������Ӱ�죬
	// ������ƶ����е�һ��������⣬����ֵ���Ѿ������ı䣬�ع���Ȼ����ʵ�������ƶ�����һ��
	// �����漰������Դ�ķ��䣬ֻ����Դ����Ȩ�Ľ��������������ܳ����⣻
	TCVector< int > ivec1 = { 1, 2, 3, 4, 5 };
	TCVector< int > ivec2( std::move( ivec1 ) );
	cout << ivec1 << endl2 << ivec2 << endl;


	StartOneTest( "�ϳ��ƶ�����" );
	// ���ࡾû���Զ��塿�����������ҳ�Ա�������ƶ�ʱ���Ż�ϳ��ƶ������������ƶ���delete��
	// ��const�����ó�Ա���ƶ�Ҳ��delete��
	struct XX	// XX��Ĭ�ϵ��ƶ�����
	{
		int ID;
		string Name;
	};
	struct YY	// YY���ƶ���delete�ģ��ƶ�����ĳ��Ի᡾תΪ���������죬��ֵ����
	{
		YY( int id, string lb ) : ID( id ), Label( lb ) {}
		YY( const YY &yy ) : ID( yy.ID ), Label( yy.Label ) {}
		int ID;
		string Label;
	};
	XX x1{ 1, "lilei" }, x2( std::move( x1 ) ), x3{ 5, "hanmeimei" }, x4;
	x4 = std::move( x3 );	// ������úϳɵ��ƶ���ֵ�����
	cout << x1.ID << ", " << x1.Name << endl << x2.ID << ", " << x2.Name << endl2;
	cout << x3.ID << ", " << x3.Name << endl << x4.ID << ", " << x4.Name << endl2;

	YY y1 = { 2, "shanghai" }, y2( std::move( y1 ) );
	cout << y1.ID << ", " << y1.Label << endl << y2.ID << ", " << y2.Label << endl2;


	cout << "һ���������ƶ��������ϳɿ�������delete��" << endl;
	struct ZZ
	{
		ZZ( int id, string nm ) : ID( id ), Name( nm ) {}
		ZZ( ZZ &&zz ) : ID( zz.ID ), Name( std::move( zz.Name ) ) {}
		int ID;
		string Name;
	};
	ZZ z1{ 1, "tom" };
	//ZZ z2( z1 );	// ERROR! ����������ɾ����


	StartOneTest( "����ֵ�ƶ�����ֵ������ԭ��" );
	// ������������ֵ��vec1�Զ�ʹ���ƶ����죬��C++11ǰ�ǲ����ܵģ����һ������Ҫ�Ŀ���
	TCVector< int > intvec1 = MakeRandiVec( 3 );
	TCVector< int > intvec2 = intvec1;
	cout << intvec1 << endl2 << intvec2 << endl;


	StartOneTest( "�ƶ�������" );
	// C++11�ṩ���·����������ƶ�����������صķ����ὫԪ���ƶ���
	TCVector< string > svec{ "abc", "def" };
	string sary[ 2 ]{};
	std::copy( make_move_iterator( svec.begin() ),	MCPP11
		make_move_iterator( svec.end() ), sary );
	cout << svec << endl;	// vec�е�ÿ��Ԫ�ر�ary͵���ˣ���vec��������
	cout << "sary:" << endl << sary[ 0 ] << endl << sary[ 1 ] << endl;


	StartOneTest( "�����βεĺ�������" );
	// �����βΰ汾�����أ���ֵ�����纯�����ء����ʽ����ֵ������ֵ�������и��õ�ƥ��
	TCVector< string > svec2;
	svec2.PushBack( sary[ 0 ] );			// ʹ�ó�������
	svec2.PushBack( sary[ 1 ] + "123" );	// ʹ����������
	svec2.PushBack( "mmnnbb" );				// ʹ����������
	cout << svec2 << endl;

	StartOneTest( "�����޶���" );
	TCVector< int > intvec3 = { 5, 8, 1, 4, 2 };
	intvec3 = intvec3.Sorted();	// ʹ�ó���汾
	TCVector< int > intvec4 = MakeRandiVec( 10 ).Sorted();	// ʹ����ֵ�汾
	cout << intvec3 << endl2 << intvec4 << endl;
}

void Test_CopyOther()
{
	StartOneTest( "��������" );
	// һ�㽻�����м�������Ҫ�ĸ�ֵ�Ϳ�����ͨ���Զ�����ƶ�����������٣�
	// ���һ����ʵ�����ƶ�������ƶ���ֵ����ô������std��swap���Լ����ص�swap��һ�����
	TCVector< string > vec1{ "abc", "def", "ghi" };
	TCVector< string > vec2{ "123", "789" };
	swap( vec1, vec2 );
	cout << vec1 << endl2 << vec2 << endl;

	// ������������һ����Ȥ��Ӧ�ã������á���������������ʵ��operator =


	StartOneTest( "TCVector�������ӿ�" );
	TCVector< Coordinate > vCoord{ { 1.0, 3 }, { 2.5, 3.3 } };
	vCoord.Reserve( 20 );
	vCoord.Resize( 6 );
	vCoord.PopBack( 2 );
	cout << vCoord << endl;


	StartOneTest( "allocator�����༰ԭ��" );
	// һ��new���ڴ�������Ĺ����ʼ���ϲ���һ�𣬼�ʹĳЩ������Զ�ò���Ҳ�ṹ�죬������˷ѣ�
	// ��deleteҲ������������ڴ���ͷźϲ���һ�𣬼�ʹ�ǿ��ڴ��㻹������ã�
	Coordinate *dynCoords = new Coordinate[ 3 ];
	delete[] dynCoords;

	// allocator�������4������ֿ�����������Ҳ���Ӹ�Ч��
	cout << "1��allocate continuous memory block" << endl;
	void *memCoords = ::operator new( sizeof( Coordinate ) * 3 );

	cout << "2��construct an object if needed:\t";
	Coordinate *pCoords = static_cast< Coordinate* >( memCoords );
	::new( ( void* )( pCoords + 0 ) ) Coordinate( 1.5, 2.2 );	// ʹ�ÿ���������ƶ�����
	cout << pCoords[ 0 ] << endl;

	cout << "3��destroy an object if no more used" << endl;
	( pCoords + 0 )->~Coordinate();

	cout << "4��deallocate the whole memory block" << endl;
	::operator delete( memCoords );

	cout << "uninitialized_copy�൱����һ��δ�����ѵ��ڴ����ظ���2����" << endl;
}

void Chapter13Init()
{
	auto ch13 = ChapterBase::AddChapter( 13, "��ʮ���� ��������" );
	ch13->AddSection( 1, "��Ҫ����", &Test_CopyBase );
	ch13->AddSection( 2, "�����͸�ֵ", &Test_CopyAssign );
	ch13->AddSection( 3, "��ֵ�������ƶ�", &Test_RvalueMove );
	ch13->AddSection( 4, "������Ҫ����", &Test_CopyOther );
}
static int _Init = ( Chapter13Init(), 0 );

TCVector< int > MakeRandiVec( size_t count, int imin, int imax )
{
	TCVector< int > temVec;
	temVec.Reserve( count );
	for( size_t i = 0; i < count; ++i )
		temVec.PushBack( Randi( imin, imax ) );

	return temVec;
}