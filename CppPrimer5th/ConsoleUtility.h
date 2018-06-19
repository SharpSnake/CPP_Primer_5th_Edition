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
extern HANDLE HConsoleWindow;	// ����̨���ھ��
#endif

#include <iostream>
#include <map>
#include <string>

using namespace std;


// ����ĳ�δ�����Ҫ�Զ����ÿ���̨������ɫ
#define CONSOLE_COLOR_AUTO_RESET ConsoleTextColorReseter tmpColorReseter;

// ��ʾ�˳��Ĵ���
#define CONSOLE_CODE_EXIT 0


// ����̨������ɫö��
enum ConsoleTextColor
{
	CmdColor_Green = 10,
	CmdColor_Cyan = 11,		// ��ɫ
	CmdColor_Red = 12,
	CmdColor_Magenta = 13,	// ���
	CmdColor_Yellow = 14,
	CmdColor_White = 15,
};


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

	~ConsoleTextColorReseter() { Reset(); }

	void Reset( void )
	{
#ifdef WIN32
		SetConsoleTextAttribute( HConsoleWindow, m_OriginColor );
#endif
	}
};


// ���ÿ���̨������ɫ
inline void SetConsoleTextColor( unsigned c )
{
#ifdef WIN32
	SetConsoleTextAttribute( HConsoleWindow, c );
#else
	// ��������ϵͳ
#endif
}


// ����Ŀ¼���û����������������
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
		if ( cin >> code )	// ��������һ����ĸ����ѭ������û�����
		{
			if( code == CONSOLE_CODE_EXIT || m.find( code ) != m.end() )
				return code;
			else
				cout << "������һ����ȷ�Ĵ���:" << endl;
		}
		else
		{
			cin.clear();
			cin.sync();
			cout << "������һ����ȷ�Ĵ���:" << endl;
		}
	}
}

// ѯ���û��ǻ��
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

