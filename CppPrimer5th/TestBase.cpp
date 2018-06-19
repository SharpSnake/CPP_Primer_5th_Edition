#include "stdafx.h"
#include "TestBase.h"

#include "ConsoleUtility.h"

using namespace std;

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
