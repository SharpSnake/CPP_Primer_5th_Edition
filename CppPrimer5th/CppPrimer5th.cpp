// CppPrimer5th.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "Chapter01.h"
#include "Chapter02.h"

int main()
{
	TestBase *test = new Chapter02();
	test->Run();
	delete test;
	test = nullptr;

	system( "pause" );
	return 0;
}

