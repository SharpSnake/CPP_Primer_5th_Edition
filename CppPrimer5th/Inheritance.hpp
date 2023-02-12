#ifndef INHERITANCE_HPP
#define INHERITANCE_HPP

#include <string>
#include <iostream>
#include <set>
#include <boost/format.hpp>		// head-only


// ���ַ���
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


// ��Ϊ�ӿ�
struct IBehavior
{
	virtual void Speak( std::ostream& ) const = 0;			// ����
};

// pure virtual can be defined outside the class 
inline void IBehavior::Speak( std::ostream &ostm ) const
{
	ostm << "Behavior - Speak:" << std::endl;
}


// ����
class Creature
{
public:
	explicit Creature( unsigned age, std::string name = "�°��ϲ�" )
		: m_Age( age ), m_Name( name ) {}

	Creature() = default;
	virtual ~Creature() {}

public:
	unsigned Age() const { return m_Age; }
	std::string Name() const { return m_Name; }

	// �������
	virtual SpeciesClass Species() const = 0;

protected:
	unsigned m_Age{};				// ����
	std::string m_Name;			// ����
};


// ��
class TCPerson
	: public Creature, public IBehavior	// class derivation list���������б�
{
public:
	TCPerson()
		: m_IDNumber( _GenIDNumber() ) {}

	TCPerson( unsigned age, const std::string &name )
		: Creature( age, name ), m_IDNumber( _GenIDNumber() ) {}

	virtual ~TCPerson() {}

public:
	std::string IDNumber() const { return m_IDNumber; }

	// MCPP11 override��final���Ǹ��ڲ����б����
	SpeciesClass Species() const override final { return Human; }

	void Speak( std::ostream &ostm ) const override
	{
		IBehavior::Speak( ostm );
		ostm << "���䣺" << m_Age << std::endl 
			<< "������" << m_Name << std::endl
			<< "���֤�ţ�" << m_IDNumber << std::endl;
	}

	virtual TCPerson* Clone() const { return new TCPerson( *this ); }

	virtual void Sleep( unsigned hours = 8 )
	{
		std::cout << "Sleep " << hours << " hours" << std::endl;
	};

	// obj������Creature���ͣ�����protected��Ա���ܱ������friendֱ�ӷ���
	friend std::istream & operator >>( std::istream &istm, TCPerson &obj ){
		return istm >> obj.m_Age >> obj.m_Name;
	}

protected:
	std::string m_IDNumber;						// ���֤����

	static std::string _GenIDNumber();			// ���������֤����
	static boost::format IDFormater;			// ���֤�Ÿ�ʽ����
	static std::set< unsigned > IDNumberSet;	// ȫ�����֤����
};

// This is for all inheritance of TCPerson and itself
inline std::ostream & operator <<( std::ostream &ostm, const TCPerson &obj )
{
	obj.Speak( ostm );
	return ostm;
}


// ����
class TCMan : public TCPerson	// TCPerson��ֱ�ӻ��࣬Creature�Ǽ�ӻ���
{
public:
	TCMan( unsigned age, const std::string &name, double beardLen = 2.5 )
		: TCPerson( age, name ), m_BeardLength( beardLen ) {}

	TCMan() = default;
	virtual ~TCMan() {}

public:
	// ������ͬ�������������������򣬱�����ȫ����
	using TCPerson::IDNumber;

	// ���ػ����IDNumber
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
		ostm << "���볤�ȣ�" << m_BeardLength << std::endl 
			<< "I'm very strong! " << std::endl;
	}

	TCMan* Clone() const override { return new TCMan( *this ); }

	void Sleep( unsigned hours = 5 ) override
	{
		std::cout << "Sleep " << hours << " hours" << std::endl;
	};

protected:
	std::string m_IDNumber = "ManID";	// �����˻����m_IDNumber
	double m_BeardLength = 10;			// ���볤�ȣ���λcm
};

// ����ʿ��
class ManSoldier : TCMan	// class is private inheritance by default
{
public:
	// �̳й��죬���ܸı���๹��ķ��ʼ����������������Ҫ���ػ򸲸ǹ��캯��
	using TCMan::TCMan;

	// �ı���BeardLength���û��ķ���Ȩ�ޣ�Ȩ����using���ֵ�λ�ã��˴���public:��������
	// �������ԡ��������˳�Ա�ڻ����еķ��ʼ��𣡣���
	using TCMan::m_BeardLength;

	void Speak( std::ostream &ostm ) const override
	{
		TCMan::Speak( ostm );
		ostm << "I'm a glorious soldier. " << std::endl;
	}

	TCMan& BaseRef() { return *this; }
};


// Ů��
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
		ostm << "�������" << m_HandbagBrand << endl 
			<< "I'm beautiful!" << std::endl;
	}

	virtual TCWomen* Clone() const override { return new TCWomen( *this ); }

protected:
	std::string m_HandbagBrand = "LV";			// �����Ʒ��
};


#endif // !INHERITANCE_HPP