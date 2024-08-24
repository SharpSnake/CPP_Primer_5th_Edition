#include "TestBase.h"
#include "ConsoleUtility.h"

using namespace std;


void ChapterBase::AddSection( const int &code, const std::string &title, SectionMethod method )
{
	m_TestCases[ code ] = SectionTest( title, method );
}

// ���û�ѡ��С����ţ�ѭ�����ԣ�ֱ��ѡ���˳�Ϊֹ
void ChapterBase::RunLoop( int sec )
{
	if( !sec )
	{
		while ( ( sec = InputCodeByMap< SectionTest >( "ѡ�������������", m_TestCases ) ) 
			!= CONSOLE_CODE_EXIT )
		{
			m_TestCases[ sec ].Invoke();
			if( !InputYesOrNo( "�Ƿ�������²��ԣ�" ) )
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
			code = InputCodeByMap< ChapterBase* >( "ѡ���½ڴ���", Chapters );
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
