#ifndef CHAPTER06_H
#define CHAPTER06_H

#include <iostream>
#include <string>
#include <initializer_list>
#include <cstdarg>	// for ellipsis parameters

#include "TestHelper.h"


// ���������Ͷ�����룬ͷ�ļ�ֻ������������ʱ�β�����ʡ�������ľ��嶨����cpp��
// ������ֻҪ֪�������Ϳ���ʹ��һ������������Ҫ���Ķ���������

void CallLocalStaticObject( bool );

void PassingArgsByRef( const int & );


// Ĭ��ʵ�ν������������������崦��д��������ʾĬ��ʵ�ο����Ǻ�������ֵ��ȫ�ֱ���
extern TCPoint SchoolPosition;

void PrintSchoolInfo( const std::string &name, 
	size_t number = Randi( 500, 1000 ), const TCPoint &pos = SchoolPosition );


void PrintNames( std::initializer_list< std::string > );
void PrintNames( int,... );	// �����ǿ�ʡ��


using ArrayRowi = int[ 3 ];
//typedef int ArrayRowi[ 3 ];	// �ȼ۵ı�������

// �����������͵ļ��ַ�������ʵ���ص��������ָ�������
int ( *GetRow1 ( int ( *matrix )[ 3 ], int row ) )[ 3 ];
ArrayRowi* GetRow2( int ( *matrix )[ 3 ], int row );
ArrayRowi& GetRow2r( int ( *matrix )[ 3 ], int row );
auto GetRow3( int ( *matrix )[ 3 ], int row ) -> int ( * )[ 3 ];	MCPP11


void PrintPoint( TCPoint & );
void PrintPoint( const TCPoint & );	// ����const����

void Sum( int, int );
void Sum( short, short );
void Sum( double, double );
void Subtract( int, int );
void Multiply( int, int );
void Multiply( float, float );


// I prefer the PFMath version, �ڶ��ָ���ͨ�ã�Ҳ�����׶�
typedef void FMath( int, int );			// ׼ȷ��������к�������
typedef void ( *PFMath )( int, int );	// �����ָ�������͵�ָ�룬һ���ƺ���ָ��
//using FMath = void( int, int );		// �����������ǵȼ۵�����
//using PFMath = void ( * )( int, int );

// ��������Ϊ�β�ʱ��������������*�Ͳ���*�������ԣ��ǵȼ۵ģ��������Զ�����
void CalMathFunA( int, int, FMath );
void CalMathFunB( int, int, PFMath );

// ������Ϊ����ֵʱ������ָ����ʽ�������������Զ�����
void ( *GetFunc1( int flag ) )( int, int );
FMath* GetFunc2( int flag );
PFMath GetFunc3( int flag );
auto GetFunc4( int flag ) -> void ( * )( int, int );	MCPP11

#endif // !CHAPTER06_H