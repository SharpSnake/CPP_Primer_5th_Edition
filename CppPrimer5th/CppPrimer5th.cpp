#include "TestHelper.h"
#include "TestBase.h"

// ���������������Ĳ�����
// argc : �ⲿ�ܹ��������������˼���������������������
// argv : argv[ 0 ]�ض��ǳ�������argv[ argc ]�ض���0��
// ��argv[ 1 ]��ʼ�����������ݽ����Ĳ�����
int main( int argc, char **argv )
{
	HelperInit();
	RunMainLoop();
	return 0;	// ����0��ʾû���⣬������ͬ�Ĵ��ű�ʾ��ͬ������
}

