#include <vector>
#include <algorithm>
#include <set>
#include <atomic>

#include "TestBase.h"
#include "TestHelper.h"

using namespace std;

template< typename T >
void PrintContainer09( const T &con )
{
	for( auto &i : con )
		cout << i << " ";
	cout << endl;
}



void Test_SeqContainerUtility()
{
	StartOneTest( "ɾ��Ԫ��" );
	// 1��˳������
	vector< int > ivec1 = { 3, 5, 1, 6, 2, 9 };
	for( auto i = ivec1.begin(); i != ivec1.end(); )	// ɾ�����ڵ���5��Ԫ��
	{
		if( *i >= 5 )	// ɾ���󣬺���ĵ�����ʧЧ�����Һ�erase�᷵�ؿ��õ���һ��������
			i = ivec1.erase( i );
		else
			++i;
	}
	cout << "capacity:\t" << ivec1.capacity() << endl;
	PrintContainer09( ivec1 );

	// 2����������
	set< int > iset1 = { 3, 6, 1, 2, 5 };
	for( auto i = iset1.begin(); i != iset1.end(); )
	{
		if( *i >= 5 )
			iset1.erase( i++ );	// ɾǰ�Ⱥ�������ɾ��i������ĵ���������ʧЧ
		else
			++i;
	}
	PrintContainer09( iset1 );
}

ChapterBase* Chapter09Init()
{
	ChapterBase *ch09 = new ChapterBase( "�ھ��� ˳������" );
	ch09->AddSection( 1, "ɾ��Ԫ��", &Test_SeqContainerUtility );
	return ch09;
}