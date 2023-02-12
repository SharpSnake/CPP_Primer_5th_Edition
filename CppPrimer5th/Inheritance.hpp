#ifndef INHERITANCE_HPP
#define INHERITANCE_HPP

#include <string>
#include <iostream>
#include <set>
#include <boost/format.hpp>		// head-only


// 物种分类
enum SpeciesClass
{
	Human,
	Ape,
	Bird
};

inline std::ostream & operator <<( std::ostream &ostm, const SpeciesClass &obj )
{
	switch ( obj )
	{
	case Human:
		return ostm << "Human";
	case Ape:
		return ostm << "Ape";
	case Bird:
		return ostm << "Bird";
	default:
		return ostm << "Unknown species";
	}
}


// 行为接口
struct IBehavior
{
	virtual void Speak( std::ostream& ) const = 0;			// 发声
};

// pure virtual can be defined outside the class 
inline void IBehavior::Speak( std::ostream &ostm ) const
{
	ostm << "Behavior - Speak:" << std::endl;
}


// 生物
class Creature
{
public:
	explicit Creature( unsigned age, std::string name = "穆阿迪布" )
		: m_Age( age ), m_Name( name ) {}

	Creature() = default;
	virtual ~Creature() {}

public:
	unsigned Age() const { return m_Age; }
	std::string Name() const { return m_Name; }

	// 物种类别
	virtual SpeciesClass Species() const = 0;

protected:
	unsigned m_Age{};				// 年龄
	std::string m_Name;			// 名称
};


// 人
class TCPerson
	: public Creature, public IBehavior	// class derivation list，类派生列表
{
public:
	TCPerson()
		: m_IDNumber( _GenIDNumber() ) {}

	TCPerson( unsigned age, const std::string &name )
		: Creature( age, name ), m_IDNumber( _GenIDNumber() ) {}

	virtual ~TCPerson() {}

public:
	std::string IDNumber() const { return m_IDNumber; }

	// MCPP11 override和final都是跟在参数列表最后
	SpeciesClass Species() const override final { return Human; }

	void Speak( std::ostream &ostm ) const override
	{
		IBehavior::Speak( ostm );
		ostm << "年龄：" << m_Age << std::endl 
			<< "姓名：" << m_Name << std::endl
			<< "身份证号：" << m_IDNumber << std::endl;
	}

	virtual TCPerson* Clone() const { return new TCPerson( *this ); }

	virtual void Sleep( unsigned hours = 8 )
	{
		std::cout << "Sleep " << hours << " hours" << std::endl;
	};

	// obj不能是Creature类型，基类protected成员不能被子类的friend直接访问
	friend std::istream & operator >>( std::istream &istm, TCPerson &obj ){
		return istm >> obj.m_Age >> obj.m_Name;
	}

protected:
	std::string m_IDNumber;						// 身份证号码

	static std::string _GenIDNumber();			// 生成新身份证号码
	static boost::format IDFormater;			// 身份证号格式化器
	static std::set< unsigned > IDNumberSet;	// 全部身份证号码
};

// This is for all inheritance of TCPerson and itself
inline std::ostream & operator <<( std::ostream &ostm, const TCPerson &obj )
{
	obj.Speak( ostm );
	return ostm;
}


// 男人
class TCMan : public TCPerson	// TCPerson是直接基类，Creature是间接基类
{
public:
	TCMan( unsigned age, const std::string &name, double beardLen = 2.5 )
		: TCPerson( age, name ), m_BeardLength( beardLen ) {}

	TCMan() = default;
	virtual ~TCMan() {}

public:
	// 将所有同名重载引入子类作用域，避免完全覆盖
	using TCPerson::IDNumber;

	// 隐藏基类的IDNumber
	std::string IDNumber( bool flag ) const
	{
		if( flag )
			return TCPerson::m_IDNumber;
		else
			return m_IDNumber;	// TCMan::m_IDNumber
	}
	
	double BeardLength() const { return m_BeardLength; }

	void Speak( std::ostream &ostm ) const
	{
		TCPerson::Speak( ostm );
		ostm << "胡须长度：" << m_BeardLength << std::endl 
			<< "I'm very strong! " << std::endl;
	}

	TCMan* Clone() const override { return new TCMan( *this ); }

	void Sleep( unsigned hours = 5 ) override
	{
		std::cout << "Sleep " << hours << " hours" << std::endl;
	};

protected:
	std::string m_IDNumber = "ManID";	// 隐藏了基类的m_IDNumber
	double m_BeardLength = 10;			// 胡须长度，单位cm
};

// 男性士兵
class ManSoldier : TCMan	// class is private inheritance by default
{
public:
	// 继承构造，不能改变基类构造的访问级别，子类允许根据需要重载或覆盖构造函数
	using TCMan::TCMan;

	// 改变了BeardLength对用户的访问权限，权限由using出现的位置（此处是public:）决定，
	// 甚至可以【提升】此成员在基类中的访问级别！！！
	using TCMan::m_BeardLength;

	void Speak( std::ostream &ostm ) const override
	{
		TCMan::Speak( ostm );
		ostm << "I'm a glorious soldier. " << std::endl;
	}

	TCMan& BaseRef() { return *this; }
};


// 女人
struct TCWomen final : TCPerson	// struct is public inheritance by default
{
	TCWomen( unsigned age, const std::string &name, std::string handbag )
		: TCPerson( age, name ), m_HandbagBrand( handbag ) {}

	TCWomen() = default;
	virtual ~TCWomen() {}


	std::string HandbagBrand() const { return m_HandbagBrand; }

	void Speak( std::ostream &ostm ) const override
	{
		TCPerson::Speak( ostm );
		ostm << "手提包：" << m_HandbagBrand << endl 
			<< "I'm beautiful!" << std::endl;
	}

	virtual TCWomen* Clone() const override { return new TCWomen( *this ); }

protected:
	std::string m_HandbagBrand = "LV";			// 手提包品牌
};


#endif // !INHERITANCE_HPP