#ifndef CHAPTER06_H
#define CHAPTER06_H

#include <iostream>
#include <string>
#include <initializer_list>
#include <cstdarg>	// for ellipsis parameters

#include "TestHelper.h"


// 函数声明和定义分离，头文件只是声明，声明时形参名可省，函数的具体定义在cpp里
// 调用者只要知道声明就可以使用一个函数，不需要操心定义在哪里

void CallLocalStaticObject( bool );

void PassingArgsByRef( const int & );


// 默认实参仅在声明处给定，定义处不写，下面演示默认实参可以是函数返回值或全局变量
extern TCPoint SchoolPosition;

void PrintSchoolInfo( const std::string &name, 
	size_t number = Randi( 500, 1000 ), const TCPoint &pos = SchoolPosition );


void PrintNames( std::initializer_list< std::string > );
void PrintNames( int,... );	// 逗号是可省的


using ArrayRowi = int[ 3 ];
//typedef int ArrayRowi[ 3 ];	// 等价的别名声明

// 返回数组类型的几种方法，其实返回的是数组的指针或引用
int ( *GetRow1 ( int ( *matrix )[ 3 ], int row ) )[ 3 ];
ArrayRowi* GetRow2( int ( *matrix )[ 3 ], int row );
ArrayRowi& GetRow2r( int ( *matrix )[ 3 ], int row );
auto GetRow3( int ( *matrix )[ 3 ], int row ) -> int ( * )[ 3 ];	MCPP11


void PrintPoint( TCPoint & );
void PrintPoint( const TCPoint & );	// 顶层const重载

void Sum( int, int );
void Sum( short, short );
void Sum( double, double );
void Subtract( int, int );
void Multiply( int, int );
void Multiply( float, float );


// I prefer the PFMath version, 第二种更加通用，也明显易懂
typedef void FMath( int, int );			// 准确讲，这个叫函数类型
typedef void ( *PFMath )( int, int );	// 这个叫指向函数类型的指针，一般简称函数指针
//using FMath = void( int, int );		// 和上面两个是等价的声明
//using PFMath = void ( * )( int, int );

// 当函数作为形参时，两种声明（带*和不带*）都可以，是等价的，编译器自动处理
void CalMathFunA( int, int, FMath );
void CalMathFunB( int, int, PFMath );

// 函数作为返回值时必须是指针形式，编译器不会自动处理
void ( *GetFunc1( int flag ) )( int, int );
FMath* GetFunc2( int flag );
PFMath GetFunc3( int flag );
auto GetFunc4( int flag ) -> void ( * )( int, int );	MCPP11

#endif // !CHAPTER06_H