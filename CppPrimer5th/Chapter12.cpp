#include "stdafx.h"

#include <stdlib.h>     // malloc, free
#include <memory>
#include <functional>
#include <stdexcept>
#include <utility>	// std::move

#include "TestBase.h"
#include "TestHelper.h"

using namespace std;

void Test_DynamicMemory()
{
	StartOneTest( "ʲô�Ƕ�̬�ڴ棿" );
	{
		CONSOLE_COLOR_AUTO_RESET;
		SetConsoleTextColor( CmdColor_Yellow );
		cout << "ȫ�ֱ������ھ�̬�ڴ��У��ֲ���������ջ��stack���У�\r\n"
			"ÿ��Ӧ�ó�����һ���Լ����ڴ�ء����ѣ�heap�������ڴ�Ŷ�̬�����ڴ棩��" << endl;
	}

	StartOneTest( "��ͨ��̬�ڴ����ʼ��" );
	// ������������˵����Ĭ�ϳ�ʼ����ֵ��ʼ��������
	int *defaulti = new int;	// Ĭ�ϳ�ʼ����ָ��Ķ���ֵ��δ�����
	int *valuei = new int(); // ֵ��ʼ����ָ��Ķ���ִ�г����ֵ��ʼ��
	cout << *defaulti << " " << *valuei << endl2;
	SafeDelete( defaulti ); SafeDelete( valuei );

	// �Զ��������û��Ĭ�Ϲ��캯��,Ĭ��Ҳ��ִ��Ĭ�ϳ�ʼ������Ӧ�ĳ�Ա������δ����
	// �����Ҷ�Ӧ�Ĺ��캯���ͣ����߿�����C++11�ĳ�ʼ���б�
	TCPoint *defaultpnt = new TCPoint;	// Ĭ�ϳ�ʼ��
	TCPoint *valuepnt = new TCPoint();	// ֵ��ʼ��
	TCPoint *initlistpnt = new TCPoint{ 3, 4 };		MCPP11
	cout << *defaultpnt << "\t" << *valuepnt << "\t" << *initlistpnt << endl2;
	SafeDelete( defaultpnt ); SafeDelete( valuepnt ); SafeDelete( initlistpnt );

	// �±�׼�¿����ɡ���һ��ʼ�������������Զ��ƶ�����
	auto autop = new auto( Coordinate( 120, 30 ) );		MCPP11
	cout << "auto new:\t" << *autop << endl;
	SafeDelete( autop );


	StartOneTest( "�����ڴ��쳣" );
	// Chapter05��Test_Exception����ʾ��̬�ڴ治����ʱ����bad_alloc�쳣
	// ���Զ�new�������������ʹ�������쳣�����ǵ��ڴ治��ʱ����һ����ָ��
	size_t buflen = 1024 * 1024 * 1024;		// ��������8G�ڴ�
	double *buf = new( nothrow ) double[ buflen ];
	if( buf )
		cout << "memory alloced!" << endl;
	else
		cout << "out of memory!" << endl;
	SafeDelete( buf );


	StartOneTest( "��̬����ĳ�ʼ��������" );
	struct _MyInt
	{
		_MyInt() = default;
		_MyInt( int val ) : Val( val ) {}
		~_MyInt() { cout << "\t~_MyInt:\t" << Val << endl; }
		int Val;
	};
	auto PrintMyIntArray = []( string name, const _MyInt *ary, size_t len )
	{
		cout << name << endl;
		for( size_t i = 0; i < len; ++i )
			cout << "\t" << ary[ i ].Val << endl;
	};

	// �����Ĭ�ϳ�ʼ����ֵ��δ�����
	_MyInt *iary1 = new _MyInt[ 3 ];
	PrintMyIntArray( "iary1", iary1, 3 );

	// �����ֵ��ʼ�����Ӹ����žͿ���
	_MyInt *iary2 = new _MyInt[ 3 ]();
	PrintMyIntArray( "iary2", iary2, 3 );

	// �±�׼�µ��б��ʼ���������Ԫ�ؽ�����ֵ��ʼ��
	// VC++���������ᴥ������_MyInt��������GCC���ᣬ����ʼ���б��Ȳ���ʱ����˵
	// Ӧ����Ĭ�Ϲ��죬����VC++������������ʱ�����𣿻�ûŪ���ף�����
	_MyInt *iary3 = new _MyInt[ 5 ]{ 1, 2, 3 };		MCPP11
	PrintMyIntArray( "iary3", iary3, 5 );

	// ��Ȥ���ǣ�����Ԫ���ͷ�ʱ���ǰ������򡿵�
	cout << "delete iary3:" << endl;
	delete[] iary3;
}

void Test_shared_ptr()
{
	StartOneTest( "C++11 shared_ptr" );
	{
		CONSOLE_COLOR_AUTO_RESET;
		SetConsoleTextColor( CmdColor_Yellow );
		cout << "֧�ֿ�������ֵ�������������ָ��һ������ͨ�����ü�����������ʱ�ͷ��ڴ棬\r\n"
			"��Ϊ����13.2.2��HasPtr��ʹ���к���ָͨ������ƣ�ֻ�ǲ��ò����ڴ��ͷš�" << endl;
	}

	StartOneTest( "shared_ptr��ʼ��" );
	// �ȫ�ķ�ʽ��ʹ�ñ�׼���ṩ��make_shared����������ͬ���͵Ĺ��캯��
	shared_ptr< Coordinate > spCoord = make_shared< Coordinate >( 120, 35 );
	cout << "make_shared:\t" << *spCoord << endl;

	// Ҳ���Խ���new�Ķ��󣬵���������ʽ���죨shared_ptr����ָ��Ĺ�����explicit�ģ�
	// ���˹�������ָ�룬���������ò�Ҫ����new��shared_ptr��
	shared_ptr< Coordinate > spCoord2( new Coordinate( 111, 30 ) );
	cout << "construct with new:\t" << *spCoord2 << endl;

	// ����ָ��Ҳ���Խ���new�Ķ���
	spCoord2.reset( new Coordinate( 130, 20 ) );
	cout << "reset with new:\t" << *spCoord2 << endl;


	StartOneTest( "shared_ptr::get" );
	// ����get����get����ָ��������ڷ��ʣ���û���ʸ��û�ת��ָ��Ŀ���Ȩ
	Coordinate *pCoord2 = spCoord2.get();
	pCoord2->SetLongitude( 130.5 );
	cout << *spCoord2 << endl;
	// DO NOT delete pCoord2 or give pCoord2 to another shared_ptr!!!


	StartOneTest( "ָ��ɾ����" );
	// shared_ptrĬ�������ڴ�ķ�ʽ��C++��delete���������������ٷ�ʽ��ʵ���������ʱ
	// ����ָ��һ��ɾ���������Ϳ����Ǻ���ָ�롢��������function<>��lambda
	{
		char *pchar = ( char* )malloc( 100 );
		shared_ptr< char > spchar( pchar, []( char *pc ) {
			cout << "Lambda free" << endl;
			free( pc ); } );
	}

	{
		char *pchar = ( char* )malloc( 100 );
		shared_ptr< char > spchar( pchar, &FreeCMemory );
	}

	{
		char *pchar = ( char* )malloc( 100 );
		shared_ptr< char > spchar( pchar, Op_FreeCMemory() );
	}
	

	StartOneTest( "shared_ptr��������" );
	// ��shared_ptr����̬����ʱ��Ҳ��Ҫ��ָɾ��������Ϊ������Ҫdelete[]����
	{
		int *intary = new int[ 100 ]();
		shared_ptr< int > spintary( intary, Op_DeleteArray< int >() );

		// ��������ָ��ʱҲ��Ҫ����ָ��ɾ������������Ȼʹ��shared_ptrĬ�ϵ�delete
		int *intary2 = new int[ 100 ]();
		spintary.reset( intary2, []( int *iary ) {
			cout << "Lambda delete array" << endl;
			delete[] iary; } );
	}


	StartOneTest( "shared_ptr�İ���ָ�롪��weak_ptr" );
	// weak_ptr������һ��shared_ptr�����й����ֵ��������֧�ֿ����͸�ֵ
	// �����۹��컹����������ֵ��������ı���������shared_ptr�����ü���
	shared_ptr< TCPerson > spPer = make_shared< TCPerson >( "Tom", 26 );
	weak_ptr< TCPerson > wpPer1 = spPer;

	spPer.reset( new TCPerson( "Jack", 30 ) );
	weak_ptr< TCPerson > wpPer2 = spPer;

	// ���к˲������������ָ���Ƿ���ڵ����ã�����ΪJack��Tom��û�ˣ������Ѿ�����
	if( wpPer1.expired() )
		cout << "wpPer1 already expired, Tom never exist!" << endl;

	if( auto sp2 = wpPer2.lock() )
		cout << "use count:\t" << sp2.use_count() << "\t" << *sp2 << endl;


	StartOneTest( "��Ϊ�����õ�weak_ptr" );
	// weak_ptr��һ���ô��ǽ���������õ����⣺A����һ��B��sp��B��Ҳ��һ��A��sp��
	// ������ֱ���A��B��sp�������˳��������ڴ�й©������취���ڲ���wp���棻
	struct _Node
	{
		_Node( int id ) : ID( id ) {}
		~_Node() { cout << "Node " << ID << " released." << endl; }

		int ID;

		// �������Ϊ���ҵ����Լ����ӵ�ǰ��ڵ㣬��û�б�Ҫ�������ڵ�������������Լ���
		// �����ǿ����shared_ptr���ͣ��ͻ���ɽڵ���Զ�޷��ͷŵ�����
		weak_ptr< _Node > m_Pre;	// ��ǰ�ڵ��ǰһ���ڵ�
		weak_ptr< _Node > m_Next;	// ��ǰ�ڵ����һ���ڵ�
	};
	using PNode = shared_ptr< _Node >;

	auto LinkNodeTo = []( PNode pre, PNode next )		// ���������ڵ�
	{
		pre->m_Next = next;
		next->m_Pre = pre;
	};
	auto DumpNode = [ & ]( PNode &node )				// �ϳ�һ���ڵ�
	{
		if( auto preNode = node->m_Pre.lock() )
			if( auto nextNode = node->m_Next.lock() )
				LinkNodeTo( preNode, nextNode );

		// ֻҪǿ�������ã���ǰ�ڵ��m_Next�ͺ�ڵ��m_Pre�Զ�ʧЧ������û�б�Ҫ������������
		node.reset();
	};
	auto PrintNodeList = []( PNode node )				// ��ӡ�ڵ����������׽ڵ�
	{
		do{
			cout << "\tNode " << node->ID << endl;
			node = node->m_Next.lock();
		}
		while( node );
	};
	
	PNode node1 = make_shared< _Node >( 101 );
	PNode node2 = make_shared< _Node >( 102 );
	PNode node3 = make_shared< _Node >( 103 );
	LinkNodeTo( node1, node2 );
	LinkNodeTo( node2, node3 );

	cout << "Dumpint node " << node2->ID << " ..." << endl;
	DumpNode( node2 );
	
	cout << "Linked node list:" << endl;
	PrintNodeList( node1 );

	// ���⻹�����weak_ptr������ʲô������Ҫ���ô���
}

// �����࣬����ʱ���Դ�ӡ��Ϣ
struct _TmpPnt
{
	double X, Y;
	_TmpPnt( double x, double y ) : X( x ), Y( y ) {}
	~_TmpPnt() { cout << "~_TmpPnt:\t" << *this << endl; }

	friend ostream & operator <<( ostream &ostm, const _TmpPnt &obj )
	{
		return ostm << obj.X << ", " << obj.Y;
	}
};

void Test_unique_ptr()
{
	StartOneTest( "C++11 unique_ptr" );
	{
		CONSOLE_COLOR_AUTO_RESET;
		SetConsoleTextColor( CmdColor_Yellow );
		cout << "��shared_ptr��ͬ��unique_ptr����Դ��Ψһ�Ŀ���Ȩ�����������ܿ�����ֵ��\r\n"
			"��֧���ƶ��������ƶ���ֵ�������Ը��ա�ת������Ȩ�������µ���Դ" << endl;
	}

	StartOneTest( "unique_ptr��������" );
	// ֻ����newֱ�ӳ�ʼ��������
	unique_ptr< _TmpPnt > upCoord( new _TmpPnt( 1, 2 ) );
	upCoord.reset( new _TmpPnt( 3, 4 ) );

	// �ƽ�����Ȩ��ע��release���ƽ�����Ȩ�������ͷţ��ͷ���reset()
	_TmpPnt *pTmp = upCoord.release();	// ( 3, 4 )
	if( !upCoord )
		cout << "upCoord is empty" << endl;

	upCoord.reset( pTmp );
	if( upCoord )
		cout << "upCoord gets new control" << endl;

	// ���գ����ͷ���Դ
	upCoord = nullptr;
	cout << "unique_ptr = nullptr:" << endl;


	StartOneTest( "unique_ptrֻ���ƶ����ܿ���" );
	// unique_ptr�������ƶ�������ƶ���ֵ�������Ļ�������delete��
	// 1����Ϊ��������ֵ������ֵ�����ƶ�
	auto GenIntUPtr = []( double x, double y ) {
		return unique_ptr< _TmpPnt >( new _TmpPnt( x, y ) ); };
	unique_ptr< _TmpPnt > upCoord2 = GenIntUPtr( 5, 6 );

	// 2��ǿ���ƶ�
	using std::swap;
	unique_ptr< _TmpPnt > upCoord3( new _TmpPnt( 7, 8 ) );
	upCoord2 = std::move( upCoord3 );
	upCoord2.reset();


	StartOneTest( "unique_ptrɾ�����ڱ����ھ���" );
	// ��ͬ��shared_ptr��ɾ�����Ƕ�̬�ģ�unique_ptrɾ�������������ָ��ģ�����ͣ����ܸ�
	char *pchar1 = ( char* )malloc( 100 );
	unique_ptr< char, Op_FreeCMemory > upChar1( pchar1, Op_FreeCMemory() );

	char *pchar2 = ( char* )malloc( 100 );
	unique_ptr< char, function< void( void* ) > > upChar2( pchar2, &FreeCMemory );
	upChar2 = std::move( upChar1 );
	upChar2.reset();


	StartOneTest( "����汾��unique_ptr" );
	// �볣���unique_ptr���в�ͬ��ģ��������Ҫ��һ��[]����������汾�����±������
	unique_ptr< _TmpPnt[] > upAry( new _TmpPnt[ 3 ]{ { 1, 2 }, { 5, 6 }, { 9, 10 } } );
	for( size_t i = 0; i < 3; ++i )
		cout << "\t" << upAry[ i ] << endl;

	// ����汾��ɾ����Ĭ�Ͼ���delete[]������Ҫ����ָ��
	upAry.reset();	// ����˵release���ͷ���Դ��Ӧ��д���ˣ��ͷ���Ȼ��reset
}

ChapterBase* Chapter12Init()
{
	ChapterBase *ch12 = new ChapterBase( "��ʮ���� ��̬�ڴ�������ָ��" );
	ch12->AddSection( 1, "��̬�ڴ����", &Test_DynamicMemory );
	ch12->AddSection( 2, "C++11 shared_ptr", &Test_shared_ptr );
	ch12->AddSection( 3, "C++11 unique_ptr", &Test_unique_ptr );

	// allocator��ʹ�òο���ʮ����"allocator�����༰ԭ��"���Լ�TCVector��ʵ��

	return ch12;
}
