#include "stdafx.h"
#include "TestBase.h"

#include "ConsoleUtility.h"

using namespace std;

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
