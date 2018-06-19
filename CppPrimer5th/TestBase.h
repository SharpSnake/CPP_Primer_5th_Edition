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

// С�ڲ��Եĺ�����ʽ�������롢�����
typedef void ( *SectionMethod )( void );


// С�ڲ��Ե�����
class SectionTest
{
private:
	// С�ڱ���
	std::string m_Title = "";

	// С�ڲ��Եĺ���ָ��
	SectionMethod m_Method = nullptr;

public:
	SectionTest() : m_Title( "" ), m_Method( nullptr ) {}

	SectionTest( const std::string &title, SectionMethod method ) 
		: m_Title( title ), m_Method( method ) {}

	~SectionTest() { m_Method = nullptr; }

	// ִ�в���
	void Invoke() { m_Method(); }

	// ��ӡ���ڵı���
	friend std::ostream & operator <<( std::ostream &o, const SectionTest &obj )
	{
		return o << obj.m_Title;
	}
};


// �²��ԵĻ���
class ChapterBase
{
protected:
	// ���µı���
	std::string m_Title;

	// С�����-С��ʵ�������๹�캯���и������
	std::map< int, SectionTest > m_TestCases;
	
public:
	ChapterBase() : m_TestCases() {}
	virtual ~ChapterBase() { m_TestCases.clear(); }
	
public:
	// �����½ڲ���
	void RunLoop();

	// ��ӡ���µı���
	friend std::ostream & operator <<( std::ostream &o, const ChapterBase *obj )
	{
		return o << obj->m_Title;
	}
};

#endif // !TESTBASE_H
