#include "stdafx.h"

#include "TestBase.h"
#include "TestHelper.h"

using namespace std;


void Test_OOPBase()
{

}






ChapterBase* Chapter15Init()
{
	ChapterBase *ch15 = new ChapterBase( "��ʮ���� ���������" );
	ch15->AddSection( 1, "OOP��������", Test_OOPBase );
	return ch15;
}

//int main( int argc, char **argv )
//{
//	HelperInit();
//	Test_OOPBase();
//	system( "pause" );
//	return 0;	// ����0��ʾû���⣬������ͬ�Ĵ��ű�ʾ��ͬ������
//}