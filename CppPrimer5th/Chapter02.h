#ifndef CHAPTER02_H
#define CHAPTER02_H

#include <iostream>
#include "TestBase.h"

using namespace std;

extern int VChapter02extern;   // ��ֻ������

class Chapter02 : public TestBase
{
public:
	Chapter02() {}
	~Chapter02() {}
public:
	virtual void Run();
};

#endif // !CHAPTER02_H