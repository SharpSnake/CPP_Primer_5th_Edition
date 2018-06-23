// CppPrimer5th.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "TestHelper.h"
#include "Chapter01.h"
#include "Chapter02.h"
#include "Chapter03.h"

using namespace std;


map< int, ChapterBase* > Chapters;

int main()
{
	ConsoleInit();
	
	Chapters[ 1 ] = new Chapter01();
	Chapters[ 2 ] = new Chapter02();
	Chapters[ 3 ] = new Chapter03();

	while ( true )
	{
		int code = InputCodeByMap< ChapterBase* >( "ѡ���½ڴ���", Chapters );
		if( code == CONSOLE_CODE_EXIT )
			break;

		Chapters[ code ]->RunLoop();

		if( !InputYesOrNo( "�Ƿ����ѡ�������½ڲ��ԣ�" ) )
			break;
	}
	
	for ( auto &i : Chapters )
	{
		delete i.second;
		i.second = nullptr;
	}
	Chapters.clear();

	system( "pause" );
	return 0;
}

