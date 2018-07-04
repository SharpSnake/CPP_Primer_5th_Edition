#include "stdafx.h"

#include "TestBase.h"
#include "ConsoleUtility.h"

using namespace std;


void ChapterBase::AddSection( const int &code, const std::string &title, SectionMethod method )
{
	m_TestCases[ code ] = SectionTest( title, method );
}

// 由用户选择小节序号，循环测试，直到选择退出为止
void ChapterBase::RunLoop()
{
	int code{};
	while ( ( code = InputCodeByMap< SectionTest >( "选择测试用例代号", m_TestCases ) ) 
		!= CONSOLE_CODE_EXIT )
	{
		m_TestCases[ code ].Invoke();
		if( !InputYesOrNo( "是否继续本章测试？" ) )
			break;
	}
}


map< int, ChapterBase* > Chapters{};

void RunMainLoop()
{
	// some initialize stuff
	Chapters[ 1 ] = Chapter01Init();
	Chapters[ 2 ] = Chapter02Init();
	Chapters[ 3 ] = Chapter03Init();
	Chapters[ 4 ] = Chapter04Init();
	Chapters[ 5 ] = Chapter05Init();
	Chapters[ 6 ] = Chapter06Init();

	// main loop
	while ( true )
	{
		int code = InputCodeByMap< ChapterBase* >( "选择章节代号", Chapters );
		if( code == CONSOLE_CODE_EXIT )
			break;

		Chapters[ code ]->RunLoop();

		if( !InputYesOrNo( "是否继续选择其他章节测试？" ) )
			break;
	}

	// some clean stuff
	for ( auto &i : Chapters )
	{
		delete i.second;
		i.second = nullptr;
	}
	Chapters.clear();
}
