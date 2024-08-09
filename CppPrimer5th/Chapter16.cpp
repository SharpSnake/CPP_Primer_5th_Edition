#include <vector>
#include <type_traits>
#include <utility>	// std::forward
#include <unordered_set>

#include "TestBase.h"
#include "TestHelper.h"
#include "TCVector.h"
#include "Inheritance.hpp"

#pragma warning( disable:4101 )
#pragma warning( disable:4244 )
#pragma warning( disable:26800 )

using namespace std;


int iCha16 = 7;		// ����һ���о�̬�����ڵ�����

// ��ģ�徲̬��Ա�Ķ��弰��ʼ��
template< typename T > unsigned Vector2< T >::TotalCount = 0;

// C++11��ʽʵ����֮����
template class TCVector< unsigned >;	MCPP11
template void PrintArray1D( double ( & )[ 5 ] );


void Test_TemplateBase()
{
	StartOneTest( "��Ҫ����" );
	{
		CONSOLE_COLOR_AUTO_RESET;
		SetConsoleTextColor( CmdColor_Yellow );

		// ģ������б�ģ��ʵ�Ρ�ģ��ʵ��

		cout << "��ģ���ڿ���ֱ��ʹ��ģ�����֣�����Ҫ�ṩģ��ʵ�Ρ�" << endl;
		TCVector< int > vsort = TCVector< int >( { 5, 3, 9, 2 } ).Sorted();
		cout << vsort << endl2;

	}

	StartOneTest( "������ģ�����" );
	// 1�����ͣ��ұ���Ϊ�������ʽ
	cout << TFactorial< 5 >::value << endl;
	cout << TFactorial< sizeof( int ) >::value << endl;

	// 2���о�̬�����ڵ�ָ������ã����ߺ���ָ��
	cout << "feval Factorial\t" << Fevali< &iCha16, Factorial >() << endl;
	cout << "feval Cumsum\t" << Fevali< &iCha16, Cumsum >() << endl;


	StartOneTest( "ģ�ε����ͳ�Ա" );
	// ��ģ�ε����ͳ�Ա������������ʱ�������ùؼ���typename���Σ���Ϊ���﷨�Ͽ���
	// ����ȷ��T::typemember��һ����̬��Ա�������ͳ�Ա������ǰ����Ҫ��typenameǿ��һ��
	vector< int > ivec = { 1, 2, 3 };
	vector< int >::size_type orisize = FreeVector( ivec );
	cout << "origin size:\t" << orisize << endl <<  "current capacity:\t" << ivec.capacity() << endl;

	TCVector< TCPoint > vpnt = { { 2.5, 1.0 }, { 3.3, 2.2 }, { 1.5, 9.9 } };
	TCVector< TCPoint >::_iterator iter( vpnt.begin(), &vpnt );
	TCVector< TCPoint >::_iterator::reference vpnt_1 = *( ++iter );
	cout << "second point:\t" << vpnt_1 << endl;

	FreeVector( vpnt );
	cout << "current capacity:\t" << vpnt.Capacity() << endl;


	StartOneTest( "ģ���β��ƶ�" );
	// ����ģ���У��������������ת����1�����Զ���const��2�����鰴ָ��Դ���
	// ���������ת���ͼ̳��е�����ת�������ܡ��Զ����ƶϣ�
	double d1 = 3.5;
	const double cd1 = 4.0;
	int i1 = 3;
	BinaryFeval( d1, cd1 );		// OK!
	//BinaryFeval( d1, i1 );	// ERROE! int�����Զ�תdouble

	Distance dis1 = 3, dis2 = 5.5;
	DistanceEx disex1 = 4.5;
	BinaryFeval( dis1, dis2 );		// OK!
	//BinaryFeval( dis1, disex1 );	// ERROE!

	// ���ϴ���ĵ��ã������ǲ����Զ��ƶϣ��������ʽ��ָ��ģ��ʵ�Σ�Ҳ�ǿ��е�
	BinaryFeval< double >( d1, i1 );
	BinaryFeval< Distance >( dis1, disex1 );


	StartOneTest( "����ģ���뺯��ָ��" );
	// 1���ú�ģ��ʼ��һ������ָ��ʱ�������Զ�����ģ��ʵ���ƶ�
	using PFBinary_d = double( * )( const double&, const double&, plus< double > );
	using PFBinary_i = int( * )( const int&, const int&, minus< int > );
	PFBinary_d addfunc_d = BinaryFeval;
	PFBinary_i subfunc_i = BinaryFeval;
	cout << addfunc_d( 1.5, 1.3, plus< double >() ) << endl;
	cout << subfunc_i( 5.5, 1.3, minus< int >() ) << endl;

	// 2��������ָ�뱾����Ϊ�β�ʱ��������β������ĺ����ж������ʱ��һ����Ҫָ��ģ��ʵ��


	StartOneTest( "����ģ�����ع���" );
	// 1��ģ�����պ���ͬƥ��ʱ������ѡ���պ�
	DebugInfo( string( "std::string" ) );
	DebugInfo( "char string" );	// ���ƥ��ģ��char�������õİ汾���պ��Ļ��漰��һ��string�Ĺ���

	// 2��������е�ģ��֮�У�ѡ��������õ�
	DebugInfoT( string( "std::string" ) );
	DebugInfoT( "char string" );

	//Exercise 16.49:
	//template < typename T > void f( T );
	//template < typename T > void f( const T* );
	//int *p;
	//f( p );	// ����f( T )�汾�����p��const int*�����f( const T* )


	StartOneTest( "����ģ���ػ�" );
	// ��template<>��ͷ��Ϊÿ��ģ���ṩʵ�Σ��ػ���������ʵ����
	// ����ʱ����ģ����ǰ���ػ��汾�ں󣬶�����һ���ļ����ģû��ƫ�ػ��������أ�
	PrintArray1D( "abcdef" );
	PrintArray1D( "wxyz" );	// Specialization version
}


void Test_ClassTemplate()
{
	StartOneTest( "�����Ա�ĸ�ʽ" );
	// 1���ⲿ�����Ա����
	TCVector< int > fatv = { 1, 2, 3, 4, 5 };
	fatv.PopBack( 2 );
	cout << "fatv's capacity:\t" << fatv.Capacity() << endl;
	fatv.ShrinkToFit();
	cout << "fatv's capacity:\t" << fatv.Capacity() << endl;

	// 2��һ�����ģ���Ա
	Distance dis0( 10 );
	dis0 += 3;	dis0.operator-=< int >( 7.6 );
	cout << "dis0:\t" << ( double )dis0 << endl;

	// 3����ģ���ģ���Ա
	Vector2< float > fv( 10, 10 );
	fv.Move( 3, 3 );	fv.Move< int >( 2.5, -2.5 );
	cout << "fv after move:\t" << fv << endl;

	

	StartOneTest( "��ģ���Ա��ʵ����" );
	// 1����Ա����ֻ�е�����ʹ����ʱ��ʵ����
	TCVector< TSPoint > spntv = { { 3, 5 }, { 6, 1.5 } };
	cout << spntv << endl;
	// TSPoint��û�ж���Ƚ�����������Բ�֧�����򣬵�ֻҪ��ʹ��Sorted������TCVector�Կ���������
	//spntv = spntv.Sorted();	// ERROR!


	StartOneTest( "��Ԫ" );
	// 1����Ԫ������������ģ��ġ�ĳ��ʵ�����������Ԫ��Ҫ�С�ǰ����������
	// ��ģ���ǰ�����������Ժܼ��ԣ����߱����м���ģ�μ���
	Extent2<> ext1( Vector2<>( 1, 1 ), Vector2<>( 3, 5 ) );
	Vector2<> extCenter( ext1 );
	cout << ext1 << endl << extCenter << endl;

	// 2��ͨ��ģ����Ԫ����ζ����Ԫģ�������ʵ������������ȫ�����Լ�
	// ͨ�õ�������Ҫ��������ģ���б���Ҫ��һ�����Լ�ģ�β�ͬ������
	Extent2<> ext2( Vector2< int >( 2, 1 ), Vector2< int >( 5, 3 ) );
	cout << ext2 << endl;

	// 3��C++11����ģ����Ԫ������ֱ�ӽ�ģ������Ϊ��Ԫ���������д�����ӣ�
	Vector2< Distance > v1( 3, 0 ), v2( 0, 4 );
	Distance dis( v1, v2 );		MCPP11
	cout << "Distance between v1 and v2:\t" << ( double )dis << endl;


	StartOneTest( "��̬��Ա" );
	// 1����Ա�����Ķ��弰��ʼ�������ļ�����

	// 2����ͬģ�ε�ģ��ʵ��������һ��static��Ա����ͬģ�εĵľ�̬���󻥲�����
	// ���⣬��һ���Ա����һ������̬��Ա����Ҳ���ⲿ��ʹ�ò�ʵ������
	Vector2< short > iv1( 1, 2 );
	cout << "double Vector2's TotalCount:\t" << Vector2<>::GetTotalCount() << endl;
	cout << "short Vector2's TotalCount:\t" << Vector2< short >::GetTotalCount() << endl;


	StartOneTest( "��ģ���ػ�" );
	// 1��ȫ�ػ���Ҫ����ԭģ����ͬһ��namespace����������������Զ����ϣ����������������׼���ϣ��
	hash< Coordinate > coordhash;
	unordered_set< Coordinate > coords = { { 1, 2 }, { 5, 6 }, { 2, 3 } };
	if( coords.find( Coordinate( 2, 3 ) ) != coords.end() )
		cout << "Coordinate( 2, 3 )�Ѿ�����" << endl;
	if( coords.find( Coordinate( 1, 3 ) ) == coords.end() )
		coords.insert( Coordinate( 1, 3 ) );
	for( auto &i : coords )
		cout << "\t" << i << endl;

	// 2��ƫ�ػ��������ػ���
	cout << "��1��ƫ�ػ����ģ���е�һ����" << endl;
	MArray< int, 5 > intary = { 1, 3, 5, 7, 9 };
	cout << intary << endl;
	MArray< string, 3 > strary = { "abc", "i'm hulk!", "you are awesome" };
	cout << strary << endl;

	cout << "��2��ƫ�ػ�ģ�ε��ض���ʽ" << endl;
	// ģ��remove_reference
	int i1 = 55, &ri1 = i1, &&rri1 = std::move( i1 );
	const int &cri1 = i1;
	RemoveRef< decltype( i1 ) >::type var1;					PrintTypeName( var1 );
	RemoveRef< decltype( ri1 ) >::type var2;				PrintTypeName( var2 );
	RemoveRef< decltype( rri1 ) >::type var3;				PrintTypeName( var3 );
	RemoveRef< decltype( cri1 ) >::type var4 = 0;			PrintTypeName( var4 );

	MArray< vector< int >, 2 > vecary = { { 2, 4, 6 }, { 11, 13, 15, 17, 19 } };
	cout << vecary << endl;

	// 3����Ա�ػ�����һ�㺯��ģ���ػ����ƣ���template<>��ͷ
	Extent2< double > extdbl = { Vector2<>{ 1, 1 }, Vector2<>{ 5, 5 } };
	Extent2< int > extint = { Vector2< int >{ 1, 1 }, Vector2< int >{ 5, 5 } };
	cout << "Extent2< double > width:\t" << extdbl.Width() << endl;
	cout << "Extent2< int > width:\t\t" << extint.Width() << endl;
}


// ģ�����������ȫ�֡������ռ������������
template< typename T > using MyVec = TCVector< T, std::allocator< T > >;	MCPP11

// ����ģ��ı�����Ŀǰֻ֪�����������ã�ֻ������һ������ģ��ָ��
template< typename T > using PFArray3 = void ( * )( T ( & )[ 3 ] );			MCPP11
PFArray3< float > PrintAry3f = PrintArray1D< float, 3 >;

void Test_NewTemplate()
{
	StartOneTest( "ģ�����" );
	// 1�����﷨��ֻ��Ϊģ���ĳ��ʵ�������
	typedef TCVector< int > TIntVec;
	TIntVec tiv = { 1, 2, 3 };

	// 2���±�׼����using���Ը������������ֱ��Ϊ����ģ�������
	MyVec< double > mvec = { 1, 2, 5.5 };
	float ary3f[ 3 ] = { 1.5, 2.0, 0.5 };
	PrintAry3f( ary3f );


	StartOneTest( "Ĭ��ģ��ʵ��" );
	// �±�׼�£�����ģ��Ҳ������Ĭ��ģʵ�Σ�֮ǰֻ����ģ����
	cout << "default is plus:\t" << BinaryFeval( 3, 6 ) << endl;	MCPP11
	cout << "multiplies:\t" << BinaryFeval( 3, 6, multiplies< double >() ) << endl;
	cout << Vector2<>( 1.5, 2.7 ) << endl;	// ��ģ������Ҫ��һ��������


	StartOneTest( "ģ����ʽʵ����" );
	// ��ĳ��ģ���ʵ���ܳ��ã���ô�����Դ�ļ��ж��������ģ���һ��ʵ����
	// �����ӱ���ʱ�䣬��ʹ�����ú��Ӵ��±�׼������ǰʵ����һ��ģ��ʵ������ҿ�������������
	// 1��ʵ������������TCVector.hĩβ������ÿ�����ô�ͷ�ļ��ĵط�������ʹ��ǰ���������������ж��
	// 2��ʵ�����������ĳ��Դ�ļ��У�������ͨ�����ӣ���ÿ���õ���ʵ���������ط������ҵ����ʵ��
	TCVector< unsigned > uvec = { 1, 3, 5, 7 };	// ��ʵ��������Chapter16.cpp��ͷ
	cout << uvec << endl;	// ��һ���ʵ������ͬ���ǣ���ģ����ʽʵ��������ʵ���������г�Ա��

	double ary5d[ 5 ] = { 1, 1.5, 2, 2.5, 3 };
	PrintArray1D( ary5d );	// Functions.hppĩβ


	StartOneTest( "��ģ��ʹ��β�÷�������" );
	// β�õĻ���������decltype�Ʋ��βν����ú��������Ϊ�������ͣ�ֱ�ӵĻ�������
	TCVector< unsigned >::_iterator iter1( uvec.begin(), &uvec );
	TCVector< unsigned >::_iterator iter2( uvec.end() - 1, &uvec );
	cout << "_iterator plus:\t" << IteratorFeval( iter1, iter2, plus< unsigned >() ) << endl;


	StartOneTest( "����ת��������֧��ģ��" );	MCPP11
	// ͷ�ļ�<type_traits>�ж�����һϵ����������ת����ģ�壬IteratorFeval�õ���ȥ����ģ��
	// ʵ��ԭ��ģ���ػ��������۵���ע��remove_reference������ T& ���� T&& ����ΪT
	remove_reference< decltype( *iter1 ) >::type var1;							PrintTypeName( var1 );
	add_lvalue_reference< decltype( var1 ) >::type var2 = var1;					PrintTypeName( var2 );
	add_rvalue_reference< decltype( var1 ) >::type var3 = std::move( var1 );	PrintTypeName( var3 );
	remove_pointer< decltype( &var1 ) >::type var4;								PrintTypeName( var4 );
	add_pointer< decltype( var1 ) >::type var5;									PrintTypeName( var5 );
	make_unsigned< int >::type var6;											PrintTypeName( var6 );
	double dary[ 5 ][ 10 ];
	remove_all_extents< decltype( dary ) >::type var7;							PrintTypeName( var7 );
	add_const< decltype( dary ) >::type var8 = {};								PrintTypeName( var8 );

	// ����һЩ���õĻ�ȡ���Ե�ģ��
	cout << extent< decltype( dary ), 1 >::value << endl;	// ����ǲ�����ĳ��ά�ȵĳ��ȵ�
	cout << rank< decltype( dary ) >::value << endl;	// ����ǲ������ܹ��м���ά��
	cout << is_class< TCPoint >::value << " " << is_class< ProgramLan >::value << endl;
	cout << "var5 is_pointer:\t" << is_pointer< decltype( var5 ) >::value << endl;
	cout << "TCMan is_abstract:\t" << is_abstract< TCMan >::value << endl;
	cout << "Creature is_abstract:\t" << is_abstract< Creature >::value << endl;


	StartOneTest( "�����۵�" );		MCPP11
	// ����T &���βΣ����ն����۵�Ϊ��ֵ���ã���T��������ֵ
	// ����T &&���βΣ�����T��ʲô���ͣ����ա�����ı䡿����ֵ�����ԣ�Ҳ�᡾����const������
	// ���β���T &&��ʽʱ��ʵ�ο������κ����ͣ����Գ�T &&Ϊ������ģ���βΡ�
	CollapseKeepReference( var1 );	// �����ñ���������ֵ���ÿ���
	CollapseKeepReference( var2 );
	CollapseKeepReference( std::move( var1 ) );	// ֻ��T&& &&���۵�ΪT&&�����඼��T&
	CollapseKeepReference( 5 );
	CollapseKeepReference( var8 );	// const -> const

	// std::move��ʵ��ԭ��������������βΣ�Ȼ���ȶ�Tȥ���ú���static_castΪ &&
	// std::move˵���˾���ǿת��
	string s1 = "abcde", s2 = static_cast< string&& >( s1 );	MCPP11
	cout << "s1: " << s1 /* ��ʱs1�Ѿ���s2�Ӷ���� */ << " s2: " << s2 << endl2;

	// C++11����ת����std::forward����ʹ������������βδ���һ��������
	// ģ���ڻ���Ҫ��������������������ʱ������ϣ���ı����������ֵ���ԣ�����forward���ݻ���
	string s3 = "12345", s4 = "fghij";
	TCVector< string > svec;
	svec.EmplaceBack( s3 );		MCPP11
	svec.EmplaceBack( std::move( s4 ) );
	svec.EmplaceBack( 10, 'c' );
	svec.EmplaceBack( "I'm Iron Man!", 8 );
	cout << "s3: " << s3 << " s4: " << s4 << endl;
	cout << svec << endl;
	// ����ָ���make_shared��vector��emplace_back����Ҫ�õ�����ת������ԭ��Ҳ�������۵�


	StartOneTest( "�ɱ����ģ��" );		MCPP11
	// 1��ģ��������ͺ�����������sizeof...��������һ����
	cout << "ArgsSize:\t" << ArgsSize() << endl;		MCPP11
	cout << "ArgsSize:\t" << ArgsSize( 4, 4.6, "one string" ) << endl;

	// 2���򵥵Ŀɱ������ģ��һ���õݹ�ķ�ʽ��һȡ���������е���Ԫ��
	string strcom = StringCombine( '-', 8.999, 23, "i'm groot", 44U, 's' );		MCPP11
	cout << "StringCombine:\t" << strcom << endl;
	cout << "StringCombine:\t" << StringCombine( ' ' ) << endl;

	// 3��ģ�ΰ���չ���γ��˺������βΣ�Args... args
	// ���ΰ�����չ��ָ���Ƕ�ÿһ���βηֱ�Ӧ����ͬ�ġ�ģʽ�����������һ�����βκ�����һ���ɱ��βκ���
	ErrorMsg( "It's wrong", "It's totally wrong", "I'm sorry" );		MCPP11
	// �����StringCombine�ǽ����������ÿɱ��������������TCVector��EmplaceBackҲ��һ������
}


ChapterBase* Chapter16Init()
{
	ChapterBase *ch16 = new ChapterBase( "��ʮ���� ģ�塢����" );
	ch16->AddSection( 1, "��������", Test_TemplateBase );
	ch16->AddSection( 2, "��ģ��", Test_ClassTemplate );
	ch16->AddSection( 3, "ģ��������", Test_NewTemplate );
	return ch16;
}
