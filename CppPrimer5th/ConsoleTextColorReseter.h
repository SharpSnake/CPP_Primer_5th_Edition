#ifndef CONSOLETEXTCOLORRESTER_H
#define CONSOLETEXTCOLORRESTER_H

#ifdef WIN32
#	include <Windows.h>
extern HANDLE HConsoleWindow;	// ����̨���ھ��
#endif


// ����ĳ�δ�����Ҫ�Զ����ÿ���̨������ɫ
#define CONSOLE_COLOR_AUTO_RESET ConsoleTextColorReseter tmpColorReseter;

// ���ÿ���̨������ɫ
inline void SetConsoleTextColor( unsigned c )
{
#ifdef WIN32
	SetConsoleTextAttribute( HConsoleWindow, c );
#else
	// ��������ϵͳ
#endif
}


// ����̨������ɫ�Զ�������
class ConsoleTextColorReseter
{
	unsigned m_OriginColor = 0;

public:
	static void Init(void)
	{
#ifdef WIN32
		HConsoleWindow = GetStdHandle( STD_OUTPUT_HANDLE );
#endif
	}

	ConsoleTextColorReseter()
	{
#ifdef WIN32
		CONSOLE_SCREEN_BUFFER_INFO consoleInfo{};
		GetConsoleScreenBufferInfo( HConsoleWindow, &consoleInfo );
		m_OriginColor = consoleInfo.wAttributes;
#endif
	}

	~ConsoleTextColorReseter()
	{
		Reset();
	}

	void Reset( void )
	{
#ifdef WIN32
		SetConsoleTextAttribute( HConsoleWindow, m_OriginColor );
#endif
	}
};

#endif // !CONSOLETEXTCOLORRESTER_H

