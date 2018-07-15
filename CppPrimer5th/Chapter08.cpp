#include "stdafx.h"

#include <fstream>
#include <sstream>
#include <string>	// getline

#include "TestBase.h"
#include "TestHelper.h"
#include "BinaryStream.h"

using namespace std;


void Test_IOBase()
{
	StartOneTest();

	CONSOLE_COLOR_AUTO_RESET;
	SetConsoleTextColor( CmdColor_Yellow );

	cout << "1、IO对象不支持拷贝或赋值操作，一般以引用传递" << endl2;

	cout << "2、在IO操作前，要对IO对象进行状态检查：\r\n"
			"\t（1）、直接将IO对象当做条件语句，或使用good()、fail()作总体的检查；\r\n"
			"\t（2）、eof()判断是否到流末尾；\r\n"
			"\t（3）、rdstate()、setstate获取和设置状态，可以用位运算判断和设置状态；\r\n"
			"\t（4）、clear()重置状态；" << endl2;

	cout << "3、fstream在析构时自动会close；" << endl2;
}

void Test_CinCout()
{
	StartOneTest( "Flushing the Output Buffer" );
	// 有三种强制刷缓冲区的模式
	cout << "hello" << endl;
	cout << "hello" << flush;	// 只刷，尾部不添任何东西
	cout << "hello" << ends;	// 刷，同时尾部加一个空字符（似乎是空格）
	cout << "打印完毕" << endl;

	StartOneTest( "Output Buffer是否立即刷新" );
	ofstream fstm1( "testunitbuf.txt" );
	fstm1 << unitbuf;	// 开启立即刷新
	fstm1 << "test1\r\n";
	fstm1 << "test2\r\n";

	fstm1 << nounitbuf;
	fstm1 << "test3\r\n";
	fstm1 << "test4\r\n";
	fstm1 << "test finish" << endl;	// 可以设置断点查看后两行是否立刻刷
	fstm1.close();

	StartOneTest( "输入与输出对象绑定" );
	// cin一般与cout绑定，cin要求输入时cout会【自动刷缓冲区】
	string word;
	ostream *oldstm = cin.tie( nullptr );	// 解除绑定
	cout << "input a word";
	cin >> word;
	cout << "cin read a word from cout:\t" << word << endl;
	cin.tie( oldstm );						// 再重新绑定
}

void Test_Fstream()
{
	StartOneTest( "ofstream打开默认是截断的" );
	ofstream writef( "截断与追加.txt" );	// 隐式ofstream::out | ofstream::trunc
	writef << "test1" << endl;
	writef << "test over" << endl;
	writef.close();

	writef.open( "截断与追加.txt" );	// 默认打开，会直接覆盖原来文件内容
	writef << "test2" << endl;
	writef.close();

	writef.open( "截断与追加.txt", ofstream::app );	// 再以追加模式打开
	writef << "test3" << endl;
	writef.close();

	ifstream readf( "截断与追加.txt" );
	cout << "截断与追加.txt" << endl;
	string line;
	while( getline( readf, line ) )
		cout << line << endl;
	readf.close();


	StartOneTest( "二进制读写测试" );	// 类似序列化和反序列化
	ofstream binf( "BinaryIOTest.dat", ofstream::binary );
	Coordinate coord{ 120.5, 31.4 };
	double dary[ 2 ] = { 10.5, 1999.9 };
	int ival = -167;

	binf.write( reinterpret_cast< const char* >( &coord ), sizeof( Coordinate ) );
	binf.write( reinterpret_cast< const char* >( &dary ), sizeof( dary ) );
	binf.write( reinterpret_cast< const char* >( &ival ), sizeof( int ) );
	binf.close();

	ifstream binread( "BinaryIOTest.dat", ifstream::binary );
	Coordinate rcoord;
	double rdary[ 2 ];
	int rival;

	binread.read( reinterpret_cast< char* >( &rcoord ), sizeof( Coordinate ) );
	binread.read( reinterpret_cast< char* >( &rdary ), sizeof( rdary ) );
	binread.read( reinterpret_cast< char* >( &rival ), sizeof( int ) );
	binread.close();

	cout << rcoord << endl;
	cout << rdary[ 0 ] << " " << rdary[ 1 ] << endl;
	cout << rival << endl;
}

// 房屋信息，测试stringstream
class _HouseInfo
{
public:
	string Name;
	double Area;
	int Altitude;

	friend ostream& operator <<( ostream &os, const _HouseInfo &obj )
	{
		return os << obj.Name << " " << obj.Area << " " << obj.Altitude;
	}

	friend oBinaryStream& operator <<( oBinaryStream &os, const _HouseInfo &obj )
	{
		return os << obj.Name << obj.Area << obj.Altitude;
	}

	friend istream& operator >>( istream &is, _HouseInfo &obj )
	{
		return is >> obj.Name >> obj.Area >> obj.Altitude;
	}

	friend iBinaryStream& operator >>( iBinaryStream &is, _HouseInfo &obj )
	{
		return is >> obj.Name >> obj.Area >> obj.Altitude;
	}
};

void Test_OtherStream()
{
	StartOneTest( "字符串流" );
	// 简单的理解为将对象以字符串的形式序列化、反序列化，流对象可以重复利用
	ostringstream ostr;
	_HouseInfo oh1{ "Tom", 200.584, 56 }, oh2{ "Jack", 150.5, -12 };

	ostr << oh1;
	string s1 = ostr.str();
	
	ostr.str( "" );	// 清空
	ostr << oh2;
	string s2 = ostr.str();
	cout << "_HouseInfo -> string buffer:" << endl << s1 << endl << s2 << endl2;

	istringstream istr;
	_HouseInfo ih1, ih2;

	istr.str( s1 );
	istr >> ih1;

	// 重设后，需要将读的位置设置到开头，或者调用clear也可以
	istr.str( s2 );
	istr.seekg( 0, istringstream::beg );
	istr >> ih2;
	cout << "string buffer -> _HouseInfo:" << endl << ih1 << endl << ih2 << endl;


	StartOneTest( "二进制内存流" );
	oBinaryStream obin;
	obin << oh1 << oh2;
	cout << "Output binary buffer size:\t" << obin.Size() << endl;

	iBinaryStream ibin;
	ibin.Memory( obin.Memory(), obin.Size() );

	_HouseInfo ih3, ih4;
	ibin >> ih3 >> ih4;
	cout << "binary stream -> _HouseInfo:" << endl << ih3 << endl << ih4 << endl;
}


ChapterBase* Chapter08Init()
{
	ChapterBase *ch08 = new ChapterBase( "第八章 std IO库" );
	ch08->AddSection( 1, "基础概念", &Test_IOBase );
	ch08->AddSection( 2, "标准输入、输出", &Test_CinCout );
	ch08->AddSection( 3, "文件输入、输出", &Test_Fstream );
	ch08->AddSection( 4, "字符串、二进制内存流", &Test_OtherStream );
	return ch08;
}
