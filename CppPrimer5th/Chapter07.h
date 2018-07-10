#ifndef CHAPTER07_H
#define CHAPTER07_H

#include <string>
#include <vector>
#include <iostream>
#include <stdexcept>

#include "TestHelper.h"


class Student;	// 类的【前向声明】
class JuniorClass;

// 真正的函数声明，使用了【不完全类型】Student，这个函数的定义（实现）在cpp文件里
ostream& operator <<( ostream &o, const Student &obj );


// 学校
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


// 学生
class Student
{
	// 友元函数声明【≠】函数声明（有些编译器不遵守），此函数应该在类外某处通常地声明
	// 友元【没有】访问权限，在类中也【没有】先后顺序，但一般放在【开头】统一管理
	friend class JuniorClass;
	friend ostream& operator <<( ostream &, const Student & );

	// 友元成员函数，按三个步骤实现：
	// 1、相关类和成员函数要在此之前声明，暂不实现（定义）；
	// 2、在此处声明友元成员函数；
	// 3、在后面实现（定义）成员函数体；
	// 这样前后夹击好麻烦，我认为如果有需要的话还是声明友元类更直接、更方便简洁
	friend unsigned School::ModifyStudentID( size_t, size_t, unsigned );

public:
	// 当你添加了其他构造重载后，编译器就【不会自动】生成默认构造了，但某些情况
	// （比如作为容器元素时）仍要使用默认构造，所以一般最好要有一个默认构造；
	Student() = default;	MCPP11

	// 单参数的构造，本质上定义了一种【隐式类型转换】，
	// 除非标记为explicit，将禁止任何赋值、拷贝动作，但仍可【显式强转】（static_cast）
	explicit Student( const string &name )
		: m_Name( name ) {}

	// 没有标记explicit，则TCPerson对象可以隐式转换为学生
	// 是否标记explicit，要看这种类型转换是否有实际意义
	Student( const TCPerson &person )
		: Student( person.Name(), person.Age() ) {}	MCPP11

	// 委托构造函数，受委托的初始值列表和函数体执行完毕后，控制权才会交还给委托者；
	Student( const string &name, unsigned age )
		: Student( name, age, 1 ) {}	MCPP11

	// 成员变量的初始化顺序与【声明顺序一致】，与初始值列表中的顺序无关
	Student( const string &name, unsigned age, unsigned id )
		: m_ID( id ), m_Age( age ), m_Name( name ) {}

public:
	string Name() const { return m_Name; }

	// 交换
	void Swap( const Student &other )
	{
		m_Name = other.m_Name;
		m_Age = other.m_Age;
		m_ID = other.m_ID;
	}

private:
	string m_Name;
	unsigned m_Age;		// 年龄没有类内初始值，如果构造不管，将是一个未知的数字
	unsigned m_ID = 1;
};


// 初中班级
class JuniorClass
{
	friend class School;

public:
	// 类型成员有【访问权限】，且要出现在使用者【之前】，所以一般放在类的【最开头】
	typedef vector< Student >::size_type ClassSize;	// 班级人数类型

public:
	// 所有参数都有【默认实参】，那么它其实就是【默认构造】，当然这样的构造只能有一个！
	JuniorClass( const string &name = "一年一班", ClassSize size = 0 )
		: m_Name( name ), m_Students( size ) {}

public:
	string Name() const { return m_Name; }

	ClassSize NumberOfStudents() const;

	size_t VisitTime() const { return m_VisitTime; }

	// 班级是学生的友元类，所以可以访问私有成员
	string StudentName( ClassSize pos ) const { return m_Students[ pos ].m_Name; }

	unsigned StudentID( ClassSize pos ) const { return m_Students[ pos ].m_ID; }

	// 本班的占座率
	double StudentsRatio() const { return NumberOfStudents() * 1.0 / MaxNumberOfStudents; }


	// 重新改变班级人数，演示静态成员可以作为【默认实参】
	void Resize( ClassSize newsize = MaxNumberOfStudents )
	{
		m_Students.resize( newsize );
	}

	// 定义在类外，只在外边加inline就够了，类内【可省略】
	JuniorClass& Add( const Student &one );

	// 类内定义的函数，是【隐式inline】的
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
			cout << "一般访问" << endl;
		return const_cast< Student& >( _Get( pos ) );
	}

	// 两个Get唯一区别是const修饰，也算【重载】，调用哪个取决于调用【对象】是否是const的
	const Student& Get( ClassSize pos, bool inform = false ) const
	{
		if( inform )
			cout << "const访问" << endl;
		return _Get( pos );
	}

	friend ostream& operator <<( ostream &o, const JuniorClass &cls )
	{
		o << "班级:\t" << cls.m_Name << endl
			<< "人数:\t" << cls.NumberOfStudents() << endl
			<< "课程数:\t" << cls.NumberOfLesson << endl
			<< "占座率:\t" << cls.StudentsRatio() << endl;
		
		if( cls.NumberOfStudents() )
			o << "第一名:\r\n" << cls.m_Students[ 0 ];
		return o;
	}

private:
	string m_Name;
	vector< Student > m_Students;
	mutable size_t m_VisitTime = 0;					// 记录学生被访问的总次数

	static ClassSize MaxNumberOfStudents;			// 一个班的最多人数
	static constexpr size_t NumberOfLesson = 10;	// constexpr静态成员可以在类内初始化
};

// 在类外定义的成员函数，只有返回值在类的【作用域之外】，所以这里需要用::声明
// 类其实也是一个作用域，重名函数和变量的隐藏规则和一般作用域相似：也是内层隐藏外层
inline JuniorClass::ClassSize JuniorClass::NumberOfStudents() const
{
	return m_Students.size();
}

// 只要在同一个文件里，也算inline函数
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
	// 学校是班级的友元类，因此可以访问班级的学生列表，
	// 但友元关系【没有传递性】，所以学校不能直接访问某个学生的私有成员
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