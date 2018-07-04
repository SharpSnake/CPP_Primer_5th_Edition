#include "stdafx.h"

#include <string>
#include <cctype>
#include <cstring>
#include <vector>
#include <iterator>
#include <algorithm>

#include "TestBase.h"
#include "TestHelper.h"

using namespace std;


// ��׼���ַ����Լ����д
void _StringToUpper( string &str )
{
	std::transform( str.begin(), str.end(), str.begin(), 
		[]( unsigned char c ){ return toupper( c ); } );	MCPP11	// Lambda���ʽ
}

// ��ӡһά����
template< typename T, size_t N >
void _PrintArray( const T ( &ary )[ N ] )	// �����������������ã���û������
{
	for( auto &i : ary )	MCPP11	// ��̬������Ҳ������range for
		cout << i << " ";
	cout << endl;
}

// ��ӡ��ά����
template< typename T, size_t M, size_t N >
void _PrintArray( const T ( &ary )[ M ][ N ] )
{
	for( auto &i : ary )
	{
		for( auto &j : i )
			cout << j << " ";
		cout << endl;
	}
	cout << endl;
}


void Test_StdString()
{
	StartOneTest( "string IO" );
	string words;

	// ����һ�����ʣ������ո񡢻��С�tab��ֹͣ����ֹ�����Ǽ��� ctrl + z
	while ( cin >> words )
	{
		CONSOLE_COLOR_AUTO_RESET;
		SetConsoleTextColor( CmdColor_Magenta );
		cout << "\tInput word: " << words << endl;
	}
	cin.clear();

	// ����һ�У���ֹ����ͬ���Ǽ��� ctrl + z
	while ( getline( cin, words ) )
	{
		CONSOLE_COLOR_AUTO_RESET;
		SetConsoleTextColor( CmdColor_Magenta );
		cout << "\tInput words: " << words << endl;
	}
	cin.clear();

	StartOneTest( "string size" );
	// size_t��������<cstddef>�У�������޹أ��ô�����ǿ����Ŀ���ֲ��
	string str1 = "1234567";
	size_t len_str1 = str1.size();
	cout << "\"" << str1 << "\"'s length is: " << len_str1 << endl;

	// std��size����һ�㶼���޷������Σ���������size���з������ּ��㣬��������Ľ����
	// -3����ʽתΪunsigned�������ԭ�򽫻���һ���ǳ�������֣����������жϼ�����Զ��true��
	cout << "len_str1 < -3 ?: " << ( len_str1 < -3 ) << endl;


	StartOneTest( "string and character methods" );
	string str2 = "abc", str3 = "ghi";
	string str4 = str2 + "def" + str3 + "xyz";	// �ӷ�ԭ��'+'����������һ����std::string����
	cout << "adding strings: " << str4 << endl;

	// <cctype>���涨���˴�C�̳й�����C++�汾��ͷ�ļ���û��.h��׺�����ַ�������
	// ���ܴ���0~255��Χ��char�������ַ�û������
	// ���潫ÿ����������ĸ��д�����ֳ��Զ������ȫ����'#'���հ��ַ�ȫ����'-'
	string str5 = "functions, described in Table	3.3 (overleaf)!!?";
	cout << str5 << endl;

	// �õ�һ�����ʿ�ͷ�ı�ʶ�������������߿հף�Ԥʾ�ź������ĸ�ǵ��ʿ�ͷ
	bool wordHead = true;

	for ( auto &i : str5 )	MCPP11	// std::string range for
	{
		if ( wordHead && isalpha( i ) )	// ���i��һ�����ʵ�����ĸ����Сд
		{
			if( islower( i ) )	// ������Сд��ת��
				i = toupper( i );
			wordHead = false;
		}
		else
		{
			if( isdigit( i ) )	// ���i��һ������
			{
				int d = stoi( string( 1, i ) );	// �����õ�����ת������������<string>��
				d *= 2;
				i = to_string( d )[ 0 ];
				wordHead = false;
			}
			else if( ispunct( i ) )	// ���i�Ǳ��
			{
				i = '#';
				wordHead = true;
			}
			else if( isspace( i ) )	// ���i�ǿհ��ַ�
			{
				i = '-';
				wordHead = true;
			}
		}
	}
	cout << str5 << endl;


	StartOneTest( "��׼��string��C�ַ������ν�" );
	char cstr[ 4 ] = "abc";		// C�ַ���ĩβ��һ��'\0'�����䳤�ȱ���+1
	string str6 = cstr;			// std string����Ҫ����ĩβ����

	str6 += "def";	// ����ַ�����ƴ��
	char cstr2[ 7 ] = { '\0' };
	strcpy( cstr2, str6.c_str() );	// <cstring>�еľɺ������Լ�std string�鿴���ַ���
	cout << "copy std string to c string: " << endl << cstr2 
		<< "����length is " << strlen( cstr2 ) /*������ȼ���û�м�ĩβ��\0*/ << endl;
}

// std��ģ��⣬ע��ģ������ͣ���Ҫ������ģ��ʵ�����������
// ���磺std::vector������ģ�壬std::vector< int >����һ���������
void Test_StdVector()
{
	StartOneTest( "�±�������������" );
	vector< string > vStr = { "123", "abc", "xxx" };

	// std�������±�������͵������Ľ����ã�����������ǣ�Ԫ�أ��������ͣ�
	// �����������ʱ��������ʱû�м�&���ͻᷢ��һ������������Ӱ�����ܣ�
	// ������������Ҫ����������Ԫ�ؽ��в�������������������Ϊ��const���������ͣ�
	decltype( vStr[ 0 ] ) item0 = vStr[ 0 ];	// string &
	item0 += "456";

	for( auto &i : vStr )	// auto��������ã���Ҫ�ֶ���&
		_StringToUpper( i );

	for( auto i = vStr.begin(); i != vStr.end(); ++i )	// ��ͨ������
	{
		// ��ע�⡿���õ���������ѭ�����̣���Ҫ�漰�����������ɾ����
		decltype( *i ) item = *i;	// string &
		item = "Item from iterator: " + item;
	}
	
	for( auto i = vStr.cbegin(); i != vStr.cend(); ++i )	MCPP11	// const��������Ԫ��Ϊֻ��
	{
		decltype( *i ) item = *i;	// const string &
		cout << item << endl;
	}
}

void Test_ArrayDeclaration()
{
	StartOneTest( "��⸴����������" );
	// ���͵Ľ���ھ����������⣬��������
	// �����������ʼ����Ҳ���ᷢ��ʲô����ֻ��Ԫ�ص�ֵ���ǲ���Ԥ֪��
	int iary[ 10 ] = { 1, 2, 3 };

	// ����10��intָ������飬����������ŵ�pary����ȫ��ͬ����������
	int *ptrs[ 10 ];
	for( auto i = 0; i < 10; ++i )
		ptrs[ i ] = &iary[ i ];
	*ptrs[ 3 ] = 4;

	// �������ڿ���pary��һ��ָ�룻���ҿ�����ָ����10��Ԫ�ص����飻���󿴣�Ԫ����int�͵ģ�
	int ( *pary )[ 10 ] = &iary;
	( *pary )[ 4 ] = 5;

	// ͬ��rary��һ����������ã����󶨵�����10��Ԫ�ص�int���飻
	// rptrsҲ��һ����������ã���ͬ���ǣ����󶨵�����Ԫ��������int*��
	int ( &rary )[ 10 ] = iary, *( &rptrs )[ 10 ] = ptrs;
	rary[ 8 ] = 9;
	*( rptrs[ 9 ] ) = 10;

	_PrintArray( iary );
}

void Test_ArrayNew()
{
	StartOneTest( "�����Զ�����" );
	TCPoint pntary[ 3 ] = {};

	// auto�Ὣ��������ɶ�Ӧ���͵�ָ�룬��Ҳ��C++��Ĭ�ϵ���ʽת��
	auto ptr = pntary;
	ptr->x = 2.2;
	ptr->y = 4.4;
	cout << *ptr << endl;

	// decltype������Ȼ����������
	decltype( pntary ) pntary2 = { { 1.5, 3.3 }, { 2.2, 5.5 } };
	_PrintArray( pntary2 );

	StartOneTest( "C++11����\"������\"" );	// ��������˵����ʵ����ָ��
	// ������ͷ�ļ�<iterator>�У���Ӧ��ֻ��������������cbegin��cend
	for( auto i = begin( pntary2 ); i != end( pntary2 ); ++i )	MCPP11
		i->Offset( 2, 3 );
	cout << "After offset, points are: " << endl;
	_PrintArray( pntary2 );

	// ָ�����루������ָ������������һ�����������Σ�ͬ��������<cstddef>��
	ptrdiff_t arylen = end( pntary2 ) - begin( pntary2 );
	cout << "ͨ��ָ���õ�����ĳ���Ϊ��" << arylen << endl;

	StartOneTest( "�������������ϸ��" );

	// 1���������ʼ��vector�����������ǵȼ۵�
	vector< TCPoint > vPnt( pntary2, pntary2 + 3 );
	//vector< TCPoint > vPnt( begin( pntary2 ), end( pntary2 ) );

	cout << "initialize vector by array:" << endl;
	for( auto &i : vPnt )
		cout << i << endl;

	// 2����ά���飬��ʵC++��Ӧ�ý������������
	// mary��3��4�еĶ�ά���飬��ά���飺a[ 3 ][ 5 ][ 5 ]����ʾ��3�����Σ�ÿ��������5X5�Ķ�ά����
	// ��ʼ����Ƕ�׵Ļ����ţ�ʡ�Ե�Ԫ�ؽ��Զ���Ĭ��ֵ��ʼ����
	int mary[ 3 ][ 4 ] = { { 1, 2, 3, 4 }, { 5, 6, 7 }, { 9, 10 } };

	// ��ά���ݵĻ�ȡ�����û���ָ��
	int ( &row )[ 4 ] = mary[ 1 ];		// �����ʾmary�еĵڶ��е�����
	int ( *prow )[ 4 ] = &mary[ 1 ];	// ���ߵڶ��е�ָ�룬�ȼ���mary + 1

	// ��ά�����range for��ÿһά��ö������ã�һ������Ϊauto���Զ����������Ϊָ�룬
	// ���±���ʧ�ܣ������Ǳ��ⲻ��Ҫ���ڴ濽����
	cout << "Multidimensional Arrays:" << endl;
	for( auto &i : mary )
	{
		for( auto &j : i )
			cout << j << " ";
		cout << endl;
	}

	// 3���������͵ı�������
	using Vec4d = double[ 4 ];	MCPP11
	Vec4d v1 = { 1.1, 3.3, 4.4, 5.5 };
	cout << "Alias simplify for array with \"using\":" << endl;
	_PrintArray( v1 );

	typedef double Vec3d[ 3 ];
	Vec3d v2 = { 5.5, 9.9 };
	cout << "Alias simplify for array with \"typedef\":" << endl;
	_PrintArray( v2 );

	typedef double Vec2X2d[ 2 ][ 2 ];
	using Vec3X3d = double[ 3 ][ 3 ];
	Vec2X2d v3 = { { 1, 2 }, { 3, 4 } };
	Vec3X3d v4 = { { 1, 2, 3 }, { 4, 5, 6 }, { 7, 8, 9 } };
	cout << "Alias simplify for two-dimensional array:" << endl;
	_PrintArray( v3 );
	_PrintArray( v4 );
}

ChapterBase* Chapter03Init()
{
	ChapterBase *ch03 = new ChapterBase( "������ �ַ���������������" );
	ch03->AddSection( 1, "std::string", &Test_StdString );
	ch03->AddSection( 2, "std::vector", &Test_StdVector );
	ch03->AddSection( 3, "�������������", &Test_ArrayDeclaration );
	ch03->AddSection( 4, "�����������", &Test_ArrayNew );
	return ch03;
}
