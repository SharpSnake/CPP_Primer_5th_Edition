#ifndef CONSOLETEXTCOLORRESTER_H
#define CONSOLETEXTCOLORRESTER_H

#ifdef WIN32
#	include <Windows.h>
extern HANDLE HConsoleWindow;	// 控制台窗口句柄
#endif


// 声明某段代码需要自动重置控制台文字颜色
#define CONSOLE_COLOR_AUTO_RESET ConsoleTextColorReseter tmpColorReseter;

// 设置控制台文字颜色
inline void SetConsoleTextColor( unsigned c )
{
#ifdef WIN32
	SetConsoleTextAttribute( HConsoleWindow, c );
#else
	// 其他操作系统
#endif
}


// 控制台文字颜色自动重置器
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

