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
	StartOneTest( "什么是动态内存？" );
	{
		CONSOLE_COLOR_AUTO_RESET;
		SetConsoleTextColor( CmdColor_Yellow );
		cout << "全局变量存在静态内存中，局部变量存在栈（stack）中，\r\n"
			"每个应用程序都有一个自己的内存池——堆（heap），用于存放动态对象（内存）。" << endl;
	}

	StartOneTest( "普通动态内存与初始化" );
	// 对内置类型来说，有默认初始化和值初始化的区别；
	int *defaulti = new int;	// 默认初始化，指向的对象值是未定义的
	int *valuei = new int(); // 值初始化，指向的对象将执行常规的值初始化
	cout << *defaulti << " " << *valuei << endl2;
	SafeDelete( defaulti ); SafeDelete( valuei );

	// 自定义类如果没有默认构造函数,默认也将执行默认初始化，对应的成员可能是未定义
	// 或者找对应的构造函数就，或者可以用C++11的初始化列表
	TCPoint *defaultpnt = new TCPoint;	// 默认初始化
	TCPoint *valuepnt = new TCPoint();	// 值初始化
	TCPoint *initlistpnt = new TCPoint{ 3, 4 };		MCPP11
	cout << *defaultpnt << "\t" << *valuepnt << "\t" << *initlistpnt << endl2;
	SafeDelete( defaultpnt ); SafeDelete( valuepnt ); SafeDelete( initlistpnt );

	// 新标准下可以由【单一初始化器】（对象）自动推断类型
	auto autop = new auto( Coordinate( 120, 30 ) );		MCPP11
	cout << "auto new:\t" << *autop << endl;
	SafeDelete( autop );


	StartOneTest( "申请内存异常" );
	// Chapter05的Test_Exception中演示动态内存不够用时会抛bad_alloc异常
	// 可以对new运算符进行配置使它不抛异常，而是当内存不够时返回一个空指针
	size_t buflen = 1024 * 1024 * 1024;		// 尝试申请8G内存
	double *buf = new( nothrow ) double[ buflen ];
	if( buf )
		cout << "memory alloced!" << endl;
	else
		cout << "out of memory!" << endl;
	SafeDelete( buf );


	StartOneTest( "动态数组的初始化和销毁" );
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

	// 数组的默认初始化，值是未定义的
	_MyInt *iary1 = new _MyInt[ 3 ];
	PrintMyIntArray( "iary1", iary1, 3 );

	// 数组的值初始化，加个括号就可以
	_MyInt *iary2 = new _MyInt[ 3 ]();
	PrintMyIntArray( "iary2", iary2, 3 );

	// 新标准下的列表初始化，不足的元素将进行值初始化
	// VC++里这条语句会触发两次_MyInt的析构，GCC不会，当初始化列表长度不足时按理说
	// 应该走默认构造，但是VC++创建了两个临时变量吗？还没弄明白！！！
	_MyInt *iary3 = new _MyInt[ 5 ]{ 1, 2, 3 };		MCPP11
	PrintMyIntArray( "iary3", iary3, 5 );

	// 有趣的是，数组元素释放时，是按【逆序】的
	cout << "delete iary3:" << endl;
	delete[] iary3;
}

void Test_shared_ptr()
{
	StartOneTest( "C++11 shared_ptr" );
	{
		CONSOLE_COLOR_AUTO_RESET;
		SetConsoleTextColor( CmdColor_Yellow );
		cout << "支持拷贝、赋值，但多个副本都指向一个对象，通过引用计数来决定何时释放内存，\r\n"
			"行为类似13.2.2的HasPtr，使用中和普通指针很相似，只是不用操心内存释放。" << endl;
	}

	StartOneTest( "shared_ptr初始化" );
	// 最安全的方式是使用标准库提供的make_shared方法，参数同类型的构造函数
	shared_ptr< Coordinate > spCoord = make_shared< Coordinate >( 120, 35 );
	cout << "make_shared:\t" << *spCoord << endl;

	// 也可以接受new的对象，但必须是显式构造（shared_ptr常规指针的构造是explicit的）
	// 除了构造智能指针，其他情况最好不要混用new和shared_ptr；
	shared_ptr< Coordinate > spCoord2( new Coordinate( 111, 30 ) );
	cout << "construct with new:\t" << *spCoord2 << endl;

	// 重置指针也可以接受new的对象
	spCoord2.reset( new Coordinate( 130, 20 ) );
	cout << "reset with new:\t" << *spCoord2 << endl;


	StartOneTest( "shared_ptr::get" );
	// 慎用get，你get到的指针仅仅用于访问，你没有资格获得或转移指针的控制权
	Coordinate *pCoord2 = spCoord2.get();
	pCoord2->SetLongitude( 130.5 );
	cout << *spCoord2 << endl;
	// DO NOT delete pCoord2 or give pCoord2 to another shared_ptr!!!


	StartOneTest( "指定删除器" );
	// shared_ptr默认销毁内存的方式是C++的delete运算符，当这个销毁方式与实际情况不符时
	// 可以指定一个删除器，类型可以是函数指针、函数对象、function<>、lambda
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
	

	StartOneTest( "shared_ptr管理数组" );
	// 当shared_ptr管理动态数组时，也需要特指删除器，因为数组需要delete[]才行
	{
		int *intary = new int[ 100 ]();
		shared_ptr< int > spintary( intary, Op_DeleteArray< int >() );

		// 另外重置指针时也需要重新指定删除器，否则仍然使用shared_ptr默认的delete
		int *intary2 = new int[ 100 ]();
		spintary.reset( intary2, []( int *iary ) {
			cout << "Lambda delete array" << endl;
			delete[] iary; } );
	}


	StartOneTest( "shared_ptr的伴随指针——weak_ptr" );
	// weak_ptr仅接受一个shared_ptr来进行构造或赋值，它本身支持拷贝和赋值
	// 但无论构造还是自身拷贝赋值，都不会改变和它伴随的shared_ptr的引用计数
	shared_ptr< TCPerson > spPer = make_shared< TCPerson >( "Tom", 26 );
	weak_ptr< TCPerson > wpPer1 = spPer;

	spPer.reset( new TCPerson( "Jack", 30 ) );
	weak_ptr< TCPerson > wpPer2 = spPer;

	// 它有核查与它最初伴随指针是否过期的作用，重置为Jack后，Tom就没了，所以已经过期
	if( wpPer1.expired() )
		cout << "wpPer1 already expired, Tom never exist!" << endl;

	if( auto sp2 = wpPer2.lock() )
		cout << "use count:\t" << sp2.use_count() << "\t" << *sp2 << endl;


	StartOneTest( "作为弱引用的weak_ptr" );
	// weak_ptr另一个用处是解决环形引用的问题：A中有一个B的sp，B中也有一个A的sp，
	// 在外面分别构造A和B的sp，程序退出后会造成内存泄漏，解决办法是内部用wp代替；
	struct _Node
	{
		_Node( int id ) : ID( id ) {}
		~_Node() { cout << "Node " << ID << " released." << endl; }

		int ID;

		// 这里仅仅为了找到与自己连接的前后节点，并没有必要将其他节点的生命周期与自己绑定
		// 如果用强引用shared_ptr类型，就会造成节点永远无法释放的问题
		weak_ptr< _Node > m_Pre;	// 当前节点的前一个节点
		weak_ptr< _Node > m_Next;	// 当前节点的下一个节点
	};
	using PNode = shared_ptr< _Node >;

	auto LinkNodeTo = []( PNode pre, PNode next )		// 连接两个节点
	{
		pre->m_Next = next;
		next->m_Pre = pre;
	};
	auto DumpNode = [ & ]( PNode &node )				// 废除一个节点
	{
		if( auto preNode = node->m_Pre.lock() )
			if( auto nextNode = node->m_Next.lock() )
				LinkNodeTo( preNode, nextNode );

		// 只要强引用重置，它前节点的m_Next和后节点的m_Pre自动失效，所以没有必要对弱引用重置
		node.reset();
	};
	auto PrintNodeList = []( PNode node )				// 打印节点链表，给定首节点
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

	// 此外还不清楚weak_ptr究竟有什么其他必要的用处？
}

// 测试类，析构时可以打印消息
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
		cout << "与shared_ptr不同，unique_ptr对资源有唯一的控制权，所以它不能拷贝或赋值，\r\n"
			"仅支持移动拷贝和移动赋值，还可以赋空、转交控制权、控制新的资源" << endl;
	}

	StartOneTest( "unique_ptr基本操作" );
	// 只能用new直接初始化或重置
	unique_ptr< _TmpPnt > upCoord( new _TmpPnt( 1, 2 ) );
	upCoord.reset( new _TmpPnt( 3, 4 ) );

	// 移交控制权，注意release是移交控制权，不是释放，释放是reset()
	_TmpPnt *pTmp = upCoord.release();	// ( 3, 4 )
	if( !upCoord )
		cout << "upCoord is empty" << endl;

	upCoord.reset( pTmp );
	if( upCoord )
		cout << "upCoord gets new control" << endl;

	// 赋空，即释放资源
	upCoord = nullptr;
	cout << "unique_ptr = nullptr:" << endl;


	StartOneTest( "unique_ptr只能移动不能拷贝" );
	// unique_ptr定义了移动构造和移动赋值，其他的基本都是delete的
	// 1、作为函数返回值，是右值，算移动
	auto GenIntUPtr = []( double x, double y ) {
		return unique_ptr< _TmpPnt >( new _TmpPnt( x, y ) ); };
	unique_ptr< _TmpPnt > upCoord2 = GenIntUPtr( 5, 6 );

	// 2、强制移动
	using std::swap;
	unique_ptr< _TmpPnt > upCoord3( new _TmpPnt( 7, 8 ) );
	upCoord2 = std::move( upCoord3 );
	upCoord2.reset();


	StartOneTest( "unique_ptr删除器在编译期决定" );
	// 不同于shared_ptr的删除器是动态的，unique_ptr删除器必须编译期指定模板类型，不能改
	char *pchar1 = ( char* )malloc( 100 );
	unique_ptr< char, Op_FreeCMemory > upChar1( pchar1, Op_FreeCMemory() );

	char *pchar2 = ( char* )malloc( 100 );
	unique_ptr< char, function< void( void* ) > > upChar2( pchar2, &FreeCMemory );
	upChar2 = std::move( upChar1 );
	upChar2.reset();


	StartOneTest( "数组版本的unique_ptr" );
	// 与常规的unique_ptr略有不同，模板类型需要加一个[]，而且数组版本还有下标运算符
	unique_ptr< _TmpPnt[] > upAry( new _TmpPnt[ 3 ]{ { 1, 2 }, { 5, 6 }, { 9, 10 } } );
	for( size_t i = 0; i < 3; ++i )
		cout << "\t" << upAry[ i ] << endl;

	// 数组版本的删除器默认就是delete[]，不需要另外指定
	upAry.reset();	// 书上说release是释放资源，应该写错了，释放仍然是reset
}

ChapterBase* Chapter12Init()
{
	ChapterBase *ch12 = new ChapterBase( "第十二章 动态内存与智能指针" );
	ch12->AddSection( 1, "动态内存基础", &Test_DynamicMemory );
	ch12->AddSection( 2, "C++11 shared_ptr", &Test_shared_ptr );
	ch12->AddSection( 3, "C++11 unique_ptr", &Test_unique_ptr );

	// allocator的使用参考第十三章"allocator的真相及原理"，以及TCVector的实现

	return ch12;
}
