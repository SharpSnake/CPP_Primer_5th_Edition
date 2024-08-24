#include "TestBase.h"
#include "ConsoleUtility.h"

using namespace std;


void ChapterBase::AddSection( const int &code, const std::string &title, SectionMethod method )
{
	m_TestCases[ code ] = SectionTest( title, method );
}

// 由用户选择小节序号，循环测试，直到选择退出为止
void ChapterBase::RunLoop( int sec )
{
	if( !sec )
	{
		while ( ( sec = InputCodeByMap< SectionTest >( "选择测试用例代号", m_TestCases ) ) 
			!= CONSOLE_CODE_EXIT )
		{
			m_TestCases[ sec ].Invoke();
			if( !InputYesOrNo( "是否继续本章测试？" ) )
				break;
		}
	}
	else
		m_TestCases[ sec ].Invoke();	
}


ChapterBase* ChapterBase::AddChapter( int code, const std::string &title )
{
	auto newchp = new ChapterBase( title );
	Chapters[ code ] = newchp;
	return newchp;
}

void ChapterBase::RunMainLoop( int code, int sec )
{
	if( !code )
	{
		// main loop
		while ( true )
		{
			code = InputCodeByMap< ChapterBase* >( "选择章节代号", Chapters );
			if( code == CONSOLE_CODE_EXIT )
				break;

			Chapters[ code ]->RunLoop();
		}
	}
	else if( sec )
		Chapters[ code ]->RunLoop( sec );

	// some clean stuff
	for ( auto &i : Chapters )
	{
		delete i.second;
		i.second = nullptr;
	}
	Chapters.clear();
}
