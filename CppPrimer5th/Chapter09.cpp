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
	StartOneTest( "删除元素" );
	// 1、顺序容器
	vector< int > ivec1 = { 3, 5, 1, 6, 2, 9 };
	for( auto i = ivec1.begin(); i != ivec1.end(); )	// 删除大于等于5的元素
	{
		if( *i >= 5 )	// 删除后，后面的迭代器失效，但幸好erase会返回可用的下一个迭代器
			i = ivec1.erase( i );
		else
			++i;
	}
	cout << "capacity:\t" << ivec1.capacity() << endl;
	PrintContainer09( ivec1 );

	// 2、关联容器
	set< int > iset1 = { 3, 6, 1, 2, 5 };
	for( auto i = iset1.begin(); i != iset1.end(); )
	{
		if( *i >= 5 )
			iset1.erase( i++ );	// 删前先后自增，删除i，后面的迭代器不会失效
		else
			++i;
	}
	PrintContainer09( iset1 );
}

ChapterBase* Chapter09Init()
{
	ChapterBase *ch09 = new ChapterBase( "第九章 顺序容器" );
	ch09->AddSection( 1, "删除元素", &Test_SeqContainerUtility );
	return ch09;
}