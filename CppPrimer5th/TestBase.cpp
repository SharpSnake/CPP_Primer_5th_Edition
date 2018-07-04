#include "stdafx.h"

#include "TestBase.h"
#include "ConsoleUtility.h"

using namespace std;


void ChapterBase::AddSection( const int &code, const std::string &title, SectionMethod method )
{
	m_TestCases[ code ] = SectionTest( title, method );
}

// ���û�ѡ��С����ţ�ѭ�����ԣ�ֱ��ѡ���˳�Ϊֹ
void ChapterBase::RunLoop()
{
	int code{};
	while ( ( code = InputCodeByMap< SectionTest >( "ѡ�������������", m_TestCases ) ) 
		!= CONSOLE_CODE_EXIT )
	{
		m_TestCases[ code ].Invoke();
		if( !InputYesOrNo( "�Ƿ�������²��ԣ�" ) )
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
		int code = InputCodeByMap< ChapterBase* >( "ѡ���½ڴ���", Chapters );
		if( code == CONSOLE_CODE_EXIT )
			break;

		Chapters[ code ]->RunLoop();

		if( !InputYesOrNo( "�Ƿ����ѡ�������½ڲ��ԣ�" ) )
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
