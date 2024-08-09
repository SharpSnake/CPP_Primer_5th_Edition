// ===========================================================================================================
//	Lambda表达式类型支持。
// 
// History:
//  WuYu, 2023.07.08 : Created.
// 
// Copyright (c) 2023 by Geoway.
// ===========================================================================================================
#ifndef METEOR_LAMBDA_TRAITS_HPP
#define METEOR_LAMBDA_TRAITS_HPP

#include <utility>


namespace Meteor::Lambda	{
/// @brief 检查Lambda是否包含捕获
template< typename T >
inline constexpr bool _Is_Captured	= false;

template< typename Functor, typename R, typename... Args >
inline constexpr bool _Is_Captured< R( Functor::* )( Args... ) const >	= 
	!std::is_convertible_v< Functor, R( * )( Args... ) >;

template< typename T >
inline constexpr bool Is_Captured = _Is_Captured< decltype( &T::operator() ) >;


template< typename T >
inline auto ToPtr( T &f )
{

}

}	// !Meteor::Lambda
#endif // !METEOR_LAMBDA_TRAITS_HPP