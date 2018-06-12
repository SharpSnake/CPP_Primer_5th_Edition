#ifndef CHAPTER01_H
#define CHAPTER01_H

#include <iostream>
#include "TestBase.h"

using namespace std;

class Chapter01 : public TestBase
{
public:
	Chapter01() {}
	~Chapter01() {}
public:
	virtual void Run();
};

#endif // !CHAPTER01_H

