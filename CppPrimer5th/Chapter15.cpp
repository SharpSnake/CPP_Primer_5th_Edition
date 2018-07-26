#include "stdafx.h"

#include "TestBase.h"
#include "TestHelper.h"

using namespace std;


void Test_OOPBase()
{

}






ChapterBase* Chapter15Init()
{
	ChapterBase *ch15 = new ChapterBase( "第十五章 面向对象编程" );
	ch15->AddSection( 1, "OOP基本概念", Test_OOPBase );
	return ch15;
}

//int main( int argc, char **argv )
//{
//	HelperInit();
//	Test_OOPBase();
//	system( "pause" );
//	return 0;	// 返回0表示没问题，其他不同的代号表示不同的问题
//}