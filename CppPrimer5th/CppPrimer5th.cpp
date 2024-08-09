#include "TestHelper.h"
#include "TestBase.h"

// 【理解程序主函数的参数】
// argc : 外部总共向主函数传递了几个参数（包括程序名）
// argv : argv[ 0 ]必定是程序名，argv[ argc ]必定是0，
// 从argv[ 1 ]开始才是真正传递进来的参数；
int main( int argc, char **argv )
{
	HelperInit();
	RunMainLoop();
	return 0;	// 返回0表示没问题，其他不同的代号表示不同的问题
}

