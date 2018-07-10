#ifndef CHAPTER07_H
#define CHAPTER07_H

#include <string>
#include <vector>
#include <iostream>
#include <stdexcept>

#include "TestHelper.h"


class Student;	// ��ġ�ǰ��������
class JuniorClass;

// �����ĺ���������ʹ���ˡ�����ȫ���͡�Student����������Ķ��壨ʵ�֣���cpp�ļ���
ostream& operator <<( ostream &o, const Student &obj );


// ѧУ
class School
{
public:
	School& Add( const JuniorClass &one );
	const JuniorClass& Get( size_t clspos ) const;
	string StudentName( size_t clspos, size_t stupos ) const;
	unsigned ModifyStudentID( size_t clspos, size_t stupos, unsigned newID );
private:
	vector< JuniorClass > m_Classes;
};


// ѧ��
class Student
{
	// ��Ԫ�����������١�������������Щ�����������أ����˺���Ӧ��������ĳ��ͨ��������
	// ��Ԫ��û�С�����Ȩ�ޣ�������Ҳ��û�С��Ⱥ�˳�򣬵�һ����ڡ���ͷ��ͳһ����
	friend class JuniorClass;
	friend ostream& operator <<( ostream &, const Student & );

	// ��Ԫ��Ա����������������ʵ�֣�
	// 1�������ͳ�Ա����Ҫ�ڴ�֮ǰ�������ݲ�ʵ�֣����壩��
	// 2���ڴ˴�������Ԫ��Ա������
	// 3���ں���ʵ�֣����壩��Ա�����壻
	// ����ǰ��л����鷳������Ϊ�������Ҫ�Ļ�����������Ԫ���ֱ�ӡ���������
	friend unsigned School::ModifyStudentID( size_t, size_t, unsigned );

public:
	// ��������������������غ󣬱������͡������Զ�������Ĭ�Ϲ����ˣ���ĳЩ���
	// ��������Ϊ����Ԫ��ʱ����Ҫʹ��Ĭ�Ϲ��죬����һ�����Ҫ��һ��Ĭ�Ϲ��죻
	Student() = default;	MCPP11

	// �������Ĺ��죬�����϶�����һ�֡���ʽ����ת������
	// ���Ǳ��Ϊexplicit������ֹ�κθ�ֵ���������������Կɡ���ʽǿת����static_cast��
	explicit Student( const string &name )
		: m_Name( name ) {}

	// û�б��explicit����TCPerson���������ʽת��Ϊѧ��
	// �Ƿ���explicit��Ҫ����������ת���Ƿ���ʵ������
	Student( const TCPerson &person )
		: Student( person.Name(), person.Age() ) {}	MCPP11

	// ί�й��캯������ί�еĳ�ʼֵ�б�ͺ�����ִ����Ϻ󣬿���Ȩ�Żύ����ί���ߣ�
	Student( const string &name, unsigned age )
		: Student( name, age, 1 ) {}	MCPP11

	// ��Ա�����ĳ�ʼ��˳���롾����˳��һ�¡������ʼֵ�б��е�˳���޹�
	Student( const string &name, unsigned age, unsigned id )
		: m_ID( id ), m_Age( age ), m_Name( name ) {}

public:
	string Name() const { return m_Name; }

	// ����
	void Swap( const Student &other )
	{
		m_Name = other.m_Name;
		m_Age = other.m_Age;
		m_ID = other.m_ID;
	}

private:
	string m_Name;
	unsigned m_Age;		// ����û�����ڳ�ʼֵ��������첻�ܣ�����һ��δ֪������
	unsigned m_ID = 1;
};


// ���а༶
class JuniorClass
{
	friend class School;

public:
	// ���ͳ�Ա�С�����Ȩ�ޡ�����Ҫ������ʹ���ߡ�֮ǰ��������һ�������ġ��ͷ��
	typedef vector< Student >::size_type ClassSize;	// �༶��������

public:
	// ���в������С�Ĭ��ʵ�Ρ�����ô����ʵ���ǡ�Ĭ�Ϲ��졿����Ȼ�����Ĺ���ֻ����һ����
	JuniorClass( const string &name = "һ��һ��", ClassSize size = 0 )
		: m_Name( name ), m_Students( size ) {}

public:
	string Name() const { return m_Name; }

	ClassSize NumberOfStudents() const;

	size_t VisitTime() const { return m_VisitTime; }

	// �༶��ѧ������Ԫ�࣬���Կ��Է���˽�г�Ա
	string StudentName( ClassSize pos ) const { return m_Students[ pos ].m_Name; }

	unsigned StudentID( ClassSize pos ) const { return m_Students[ pos ].m_ID; }

	// �����ռ����
	double StudentsRatio() const { return NumberOfStudents() * 1.0 / MaxNumberOfStudents; }


	// ���¸ı�༶��������ʾ��̬��Ա������Ϊ��Ĭ��ʵ�Ρ�
	void Resize( ClassSize newsize = MaxNumberOfStudents )
	{
		m_Students.resize( newsize );
	}

	// ���������⣬ֻ����߼�inline�͹��ˣ����ڡ���ʡ�ԡ�
	JuniorClass& Add( const Student &one );

	// ���ڶ���ĺ������ǡ���ʽinline����
	/* inline */ const Student& _Get( ClassSize pos ) const
	{
		if( pos >= m_Students.size() )
			throw std::out_of_range( "ClassTeacher::Get : position out of range" );

		++m_VisitTime;	MCPP11
		return m_Students[ pos ];
	}

	Student& Get( ClassSize pos, bool inform = false )
	{
		if( inform )
			cout << "һ�����" << endl;
		return const_cast< Student& >( _Get( pos ) );
	}

	// ����GetΨһ������const���Σ�Ҳ�㡾���ء��������ĸ�ȡ���ڵ��á������Ƿ���const��
	const Student& Get( ClassSize pos, bool inform = false ) const
	{
		if( inform )
			cout << "const����" << endl;
		return _Get( pos );
	}

	friend ostream& operator <<( ostream &o, const JuniorClass &cls )
	{
		o << "�༶:\t" << cls.m_Name << endl
			<< "����:\t" << cls.NumberOfStudents() << endl
			<< "�γ���:\t" << cls.NumberOfLesson << endl
			<< "ռ����:\t" << cls.StudentsRatio() << endl;
		
		if( cls.NumberOfStudents() )
			o << "��һ��:\r\n" << cls.m_Students[ 0 ];
		return o;
	}

private:
	string m_Name;
	vector< Student > m_Students;
	mutable size_t m_VisitTime = 0;					// ��¼ѧ�������ʵ��ܴ���

	static ClassSize MaxNumberOfStudents;			// һ������������
	static constexpr size_t NumberOfLesson = 10;	// constexpr��̬��Ա���������ڳ�ʼ��
};

// �����ⶨ��ĳ�Ա������ֻ�з���ֵ����ġ�������֮�⡿������������Ҫ��::����
// ����ʵҲ��һ�����������������ͱ��������ع����һ�����������ƣ�Ҳ���ڲ��������
inline JuniorClass::ClassSize JuniorClass::NumberOfStudents() const
{
	return m_Students.size();
}

// ֻҪ��ͬһ���ļ��Ҳ��inline����
inline JuniorClass& JuniorClass::Add( const Student &one )
{
	m_Students.push_back( one );
	return *this;
}

inline School& School::Add( const JuniorClass &one )
{
	m_Classes.push_back( one );
	return *this;
}

inline const JuniorClass& School::Get( size_t clspos ) const
{
	return m_Classes[ clspos ];
}

inline string School::StudentName( size_t clspos, size_t stupos ) const
{
	// ѧУ�ǰ༶����Ԫ�࣬��˿��Է��ʰ༶��ѧ���б�
	// ����Ԫ��ϵ��û�д����ԡ�������ѧУ����ֱ�ӷ���ĳ��ѧ����˽�г�Ա
	const JuniorClass &cls = m_Classes[ clspos ];
	return cls.m_Students[ stupos ].Name();
}

inline unsigned School::ModifyStudentID( size_t clspos, size_t stupos, unsigned newID )
{
	JuniorClass &cls = m_Classes[ clspos ];
	unsigned oldID = cls.m_Students[ stupos ].m_ID;
	cls.m_Students[ stupos ].m_ID = newID;
	return oldID;
}

#endif // !CHAPTER07_H