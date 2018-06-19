// =============================================================================
// 
// Some classes or functions about console.
// 
// 						              - WuYu, 2018.06.
// =============================================================================
#ifndef CONSOLEUTILITY_H
#define CONSOLEUTILITY_H

#ifdef WIN32
#	include <Windows.h>
extern HANDLE HConsoleWindow;	// 控制台窗口句柄
#endif

#include <iostream>
#include <map>
#include <string>

using namespace std;


// 声明某段代码需要自动重置控制台文字颜色
#define CONSOLE_COLOR_AUTO_RESET ConsoleTextColorReseter tmpColorReseter;

// 表示退出的代号
#define CONSOLE_CODE_EXIT 0


// 控制台文字颜色枚举
enum ConsoleTextColor
{
	CmdColor_Green = 10,
	CmdColor_Cyan = 11,		// 青色
	CmdColor_Red = 12,
	CmdColor_Magenta = 13,	// 杨红
	CmdColor_Yellow = 14,
	CmdColor_White = 15,
};


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

	~ConsoleTextColorReseter() { Reset(); }

	void Reset( void )
	{
#ifdef WIN32
		SetConsoleTextAttribute( HConsoleWindow, m_OriginColor );
#endif
	}
};


// 设置控制台文字颜色
inline void SetConsoleTextColor( unsigned c )
{
#ifdef WIN32
	SetConsoleTextAttribute( HConsoleWindow, c );
#else
	// 其他操作系统
#endif
}


// 根据目录，用户输入测试用例代号
template< typename T >
inline int InputCodeByMap( const string &msg, const map< int, T > &m )
{
	system( "cls" );

	cout << msg << ":" << endl; 
	for ( const auto &i : m )
		cout << i.first << " : " << i.second << "." << endl;
	cout << CONSOLE_CODE_EXIT << " : Exit." << endl;

	int code{};
	while ( true )
	{
		if ( cin >> code )	// 这里输入一个字母会死循环，还没法解决
		{
			if( code == CONSOLE_CODE_EXIT || m.find( code ) != m.end() )
				return code;
			else
				cout << "请输入一个正确的代号:" << endl;
		}
		else
		{
			cin.clear();
			cin.sync();
			cout << "请输入一个正确的代号:" << endl;
		}
	}
}

// 询问用户是或否
inline bool InputYesOrNo( const string &msg )
{
	{
		CONSOLE_COLOR_AUTO_RESET;
		SetConsoleTextColor( CmdColor_Yellow );
		cout << endl << msg << "(Y/N): ";
		cout.flush();
	}

	char flag{};
	cin >> flag;
	return flag == 'y' || flag == 'Y';
}

#endif // !CONSOLEUTILITY_H

