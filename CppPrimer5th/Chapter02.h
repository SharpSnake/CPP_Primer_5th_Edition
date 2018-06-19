#ifndef CHAPTER02_H
#define CHAPTER02_H

#include "TestBase.h"

extern int VChapter02extern;   // 这只是声明


class Chapter02 : public ChapterBase
{
public:
	Chapter02();
	~Chapter02() {}
};

#endif // !CHAPTER02_H