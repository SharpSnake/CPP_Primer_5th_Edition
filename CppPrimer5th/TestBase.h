// =============================================================================
// 
// The whole book's test cases are constituted by chapters,
// and chapter's test cases are constituted by sections.
// 						              - WuYu, 2018.06.
// =============================================================================
#ifndef TESTBASE_H
#define TESTBASE_H

#include <map>
#include <string>
#include <iostream>

// 小节测试的函数形式，无输入、无输出
typedef void ( *SectionMethod )( void );


// 小节测试的内容
class SectionTest
{
private:
	// 小节标题
	std::string m_Title = "";

	// 小节测试的函数指针
	SectionMethod m_Method = nullptr;

public:
	SectionTest() : m_Title( "" ), m_Method( nullptr ) {}

	SectionTest( const std::string &title, SectionMethod method ) 
		: m_Title( title ), m_Method( method ) {}

	~SectionTest() { m_Method = nullptr; }

	// 执行测试
	void Invoke() { m_Method(); }

	// 打印本节的标题
	friend std::ostream & operator <<( std::ostream &o, const SectionTest &obj )
	{
		return o << obj.m_Title;
	}
};


// 章测试的基类
class ChapterBase
{
protected:
	// 本章的标题
	std::string m_Title;

	// 小节序号-小节实例，子类构造函数中负责填充
	std::map< int, SectionTest > m_TestCases;
	
public:
	ChapterBase() : m_TestCases() {}
	virtual ~ChapterBase() { m_TestCases.clear(); }
	
public:
	// 运行章节测试
	void RunLoop();

	// 打印本章的标题
	friend std::ostream & operator <<( std::ostream &o, const ChapterBase *obj )
	{
		return o << obj->m_Title;
	}
};

#endif // !TESTBASE_H
