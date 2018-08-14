#include "stdafx.h"

#include <memory>
#include <strstream>

#include "TestBase.h"
#include "TestHelper.h"
#include "Inheritance.hpp"

using namespace std;

set< unsigned > TCPerson::IDNumberSet;
boost::format TCPerson::IDFormater( "1401%07d" );


TCMan	lilei( 22, "李雷", 0.7 );
TCWomen	hanmei( 21, "韩梅", "爱马仕" );

void Test_OOPBase()
{
	StartOneTest( "重要概念" );
	{
		CONSOLE_COLOR_AUTO_RESET;
		SetConsoleTextColor( CmdColor_Yellow );

		cout << "动态绑定：使用基类的引用或指针访问成员时，直到运行时才绑定\r\n"
			"对应的成员版本。" << endl2;

		cout << "基类子对象：我们可以将基类看做子类成员的一部分，但并不能保证\r\n"
			"在内存上是连续存储的，所以可以将基类引用或指针绑定到子类对象上。" << endl2;

		cout << "静态类型：写在代码中（编译器）能看到的，引用或指针的类型\r\n"
			"动态类型：当运行时，静态类型所真正绑定的对象类型\r\n"
			"多态的本质：引用或指针的静态类型与动态类型不同。" << endl2;

		cout << "派生访问说明符：子类是class时默认是private继承，\r\n"
			"子类是struct时默认是public继承" << endl2;

		cout << "合成函数：将基类看做子类的一个成员来判断子类合成函数的状态；\r\n"
			"例如子类的合成拷贝构造是delete的，那么子类的合成拷贝构造也是删除的。\r\n"
			"手动定义拷贝控制，首先处理基类（用基类对应的拷贝控制），再处理自己。"  << endl2;

		cout << "析构或构造中调用虚函数：其实是静态绑定，不存在虚调用；\r\n"
			"1、构造时，子类还没来得及创建，虚函数表也没有被覆盖；\r\n"
			"2、析构时，子类首先被销毁，虚函数表又还原到构造时的样子；"   << endl2;
	}


	StartOneTest( "继承构造" );
	// 继承链中的每一层各自负责自己的成员初始化，子类初始化首先在初始值列表中
	// 调用父类的构造，再初始化自己的成员
	cout << TCMan( 23, "Tom", 5.5 ) << endl;


	StartOneTest( "上下行转换" );
	// 上行转换：子类引用或指针转父类，隐式的
	TCPerson *pPer = &lilei, &rPer = hanmei;

	// 不存在隐式的下行转换（即父类引用或指针转子类），除非你确定转换安全强转
	TCWomen *women1 = dynamic_cast< TCWomen* >( pPer );
	if( !women1 )	// dynamic_cast有检测功能，类型不符结果为空
		cout << pPer->Name() << " is NOT a woman!!!" << endl;

	TCWomen &women2 = static_cast< TCWomen& >( rPer );
	cout << women2 << endl;		// static_cast 不做检测，由程序员保证转换安全


	StartOneTest( "子类被切割" );
	// 当传递过程，不是以引用或指针时，而是直接以基类类型传递，
	// 则拷贝构造仅拷贝基类子对象，子类信息丢失
	TCPerson copyhanmei = hanmei;
	copyhanmei.Speak( cout );


	StartOneTest( "protected访问权限不能传递给子类的成员或friend" );
	// 子类的成员或friend需要通过子类对象来间接访问基类的protected成员，不能直接访问
	TCPerson thompson;
	istrstream( "55 Thompson" ) >> thompson;
	thompson.Speak( cout );

	// 关于友元的另一个概念：【友元关系不能继承】，即基类中的友元仅可以访问基类的成员，
	// 这个友元对继承类的成员没有访问权限；
	// 这和现实中很相似：我和老王是朋友，但无论我和小李是什么关系，老王和小李也没有关系


	StartOneTest( "派生访问说明符" );
	// 1、派生访问说明符对继承类本身（内部）并【没有】任何约束和作用，
	// 它仅仅用来针对派生类的【用户】：调用者和后续的继承类
	ManSoldier soldier001( 25, "美国队长", 3 );	// ManSoldier是以private继承于TCMan

	// TCMan以及更靠上的基类的public成员对Soldier的使用者都是private的
	//soldier001.Sleep();	// ERROR! TCMan's member is private for user

	// 2、在某段代码，只要基类的public成员是可访问的，上行转换就可行
	//TCMan &soldier2base = soldier001;	// ERROR!
	TCMan &soldier2base = soldier001.BaseRef();
	soldier2base.Sleep();	// 对ManSoldier内部来说，基类仍然是public的，可以上行转换

	// 3、可以通过using改变部分基类成员的可访问性，
	soldier001.m_BeardLength += 3;
	soldier2base.Speak( cout );


	StartOneTest( "继承中的作用域" );
	// 从作用域角度来看，子类包含在父类中，也就是说，和普通的作用域规则一样，
	// 子类的同名成员将隐藏父类的成员，成员函数即使形参不同，也会隐藏
	cout << "lilei's Man ID " << lilei.IDNumber( false ) << endl;

	// 隐藏 ≠ 消失，可以用::显式调用基类的版本
	cout << "lilei's Base ID " << lilei.IDNumber( true ) << endl;

	// 用using引入基类的名字，避免覆盖，这样基类的函数仍可以照旧使用
	cout << "lilei's ID " << lilei.IDNumber() << endl;


	StartOneTest( "C++11继承构造" );
	// 子类构造时为了照顾到基类的初始化，常常要写好几个和基类构造形参相同的子类构造函数
	// C++11可以使用using将基类的构造引入子类，子类可以不必定义相似的构造
	ManSoldier soldier002( 100, "冬兵", 5 );		MCPP11
	soldier002.Speak( cout );

	// 当基类有默认实参时，这个默认实参不会被继承，而是将构造拆成若干个没有默认实参的构造，
	// 例如一个是包含所有形参的构造，一个是没有最后那个形参的版本，但是那个默认值会自动采用
	ManSoldier soldier003( 30, "飞鹰" );	// 基类中构造函数胡须默认是2.5cm长
	soldier003.Speak( cout );
}

void Test_VirtualFunctions()
{
	StartOneTest( "虚函数关键字virtual" );
	// virtual在子类中不是必须的，一个函数在基类中标注了virtual，
	// 则它的继承链子类中相同的函数都自动是虚的，可以不用再标注
	IBehavior &manBehave = TCMan( 25, "xiaoming", 0.3 );
	manBehave.Speak( cout );


	StartOneTest( "子类虚函数匹配" );
	// 继承链中V函数的形参和返回类型必须一致，
	// 但返回类型是自身引用或指针可以破例。
	shared_ptr< TCPerson > spClone1( lilei.Clone() );
	shared_ptr< TCPerson > spClone2( hanmei.Clone() );
	spClone1->Speak( cout );
	spClone2->Speak( cout );


	StartOneTest( "默认实参" );
	// 如果Vfun有默认实参，最终由对象的静态类型决定使用哪个类的默认值
	// 所以，除非有意设计，基类和子类使用相同的默认值
	lilei.Sleep();
	( ( TCPerson& )lilei ).Sleep();
	lilei.TCPerson::Sleep();		// 或者可以用::指定vfun的版本


	StartOneTest( "纯虚函数，可以在自己的外部定义，并且子类可以实实在在地调用" );
	lilei.Speak( cout );
}

void Test_NewInheritance()
{
	StartOneTest( "防止继承和虚函数重写" );		MCPP11
	// TCWomen标注了final，所以它不能再被继承，但TCMan下面还可以有一个ManSoldier子类
	ManSoldier soldier001( 22, "刘勇", 0.5 );
	soldier001.Speak( cout );

	// 当一个虚函数标注了final，则这个虚函数不能再被子类重写覆盖；


	StartOneTest( "override声明重写虚函数" );		MCPP11
	// 在子类函数后面标注override，告诉编译器你是要重写基类的虚函数，有很好的检测功能
	// 练习15.12：成员函数可以同时标注override和final

	// TCPerson的Species不仅标注override重写，而且标注了final，
	// 它的子类都一样都是人类，所以Species没必要继续允许重写
	TCPerson per;
	cout << per.Species() << endl;

}

void Test_VirtualFunctionTable()
{
	StartOneTest( "总爱问却没有卵用的虚函数表来了！" );
	// 虚函数表其实就是一组函数指针，存在一个对象内存的最开始，
	// 将对象的地址看做一个二维数组，它继承了几个包含虚函数的类，就有几张虚函数表，
	// 分别是二维数组的第0行、第1行......

	// 由于成员函数隐式的this参数不方便测试，这里构造一个简单的继承链，不涉及成员变量
	struct IBase{
		virtual ~IBase() {}
		virtual void Talk() = 0;
	};
	struct Base1{
		virtual ~Base1()	{}
		virtual void Fun1()	{ cout << "Base1::Fun1" << endl; }
		virtual void Fun2()	{ cout << "Base1::Fun2" << endl; }
	};
	struct Derived1 : public Base1, public IBase{
		virtual ~Derived1()		{}
		void Fun1() override	{ cout << "Derived1::Fun1" << endl; }
		void Talk() override	{ cout << "Derived1::Talk" << endl; };

		// 第一张表包括Base1和自身的虚函数
		virtual void DerFun()	{ cout << "Derived1::DerFun" << endl; };
	};
	struct Derived2 : Derived1{
		virtual ~Derived2()		{}
		void Fun2() override	{ cout << "Derived2::Fun2" << endl; }
		void DerFun() override	{ cout << "Derived2::DerFun" << endl; };
	};
	Derived1 &derObj = Derived2();

	// 将derObj的地址按int**解析，再依次转为独立的函数指针
	typedef void ( *PV_Func )();
	int **pVfunMap = ( int** )( &derObj );

	// 虚析构一般都是每张表的第一个，所以虚成员函数索引从1开始
	// Base1::vftable
	PV_Func der_Fun1 = ( PV_Func )pVfunMap[ 0 ][ 1 ];
	PV_Func der_Fun2 = ( PV_Func )pVfunMap[ 0 ][ 2 ];

	// Derived1::vftable，第一张表也用作自己本身的vftable，它自己的虚函数排在基类的后面
	PV_Func der_DerFun = ( PV_Func )pVfunMap[ 0 ][ 3 ];

	// IBase::vftable
	PV_Func der_Talk = ( PV_Func )pVfunMap[ 1 ][ 1 ];

	der_Fun1();
	der_Fun2();
	der_DerFun();
	der_Talk();
}

ChapterBase* Chapter15Init()
{
	ChapterBase *ch15 = new ChapterBase( "第十五章 面向对象编程" );
	ch15->AddSection( 1, "OOP基本概念", Test_OOPBase );
	ch15->AddSection( 2, "虚函数", Test_VirtualFunctions );
	ch15->AddSection( 3, "继承的新特性", Test_NewInheritance );
	ch15->AddSection( 4, "题外话：虚函数表", Test_VirtualFunctionTable );
	return ch15;
}

string TCPerson::_GenIDNumber()
{
	unsigned newid = 0;
	if( IDNumberSet.size() )
		newid = *--IDNumberSet.end();
	IDNumberSet.insert( ++newid );

	return ( IDFormater % newid ).str();	// this return will be moved
}
