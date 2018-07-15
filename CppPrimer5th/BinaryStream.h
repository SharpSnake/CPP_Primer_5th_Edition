// =============================================================================
// 
// A simple implementation of binary IO stream.
//
// Reference:
// https://www.codeproject.com/tips/808776/cplusplus-simplistic-binary-streams
//
// 						              - WuYu, 2018.07.
// =============================================================================
#ifndef BINARYSTREAM_H
#define BINARYSTREAM_H

#include <vector>
#include <string>
#include <cstring>	// for memcpy


// 简单的二进制内存流实现，input/output合在一个类了；
// 更高效、通用的实现参考boost serialization的封装，那个省去缓冲区内存的来回拷贝，提高性能；
class BinaryStream
{
public:
	std::size_t Size() const { return m_BinBuf.size(); }

	// 从output流中获取已存储的数据，存到数据库前一般还要用zip算法压缩一下
	const char* Memory() const
	{
		return reinterpret_cast< const char* >( &m_BinBuf[ 0 ] );
	}

	// 设置input流的缓冲数据，同时读取位置归零
	void Memory( const char *buf, std::size_t size )
	{
		m_BinBuf.assign( buf, buf + size );
		m_Pos = 0;
	}

	// 写入一个范围的数据
	BinaryStream& Write( const char *begin, const char *end )
	{
		m_BinBuf.insert( m_BinBuf.end(), begin, end );
		return *this;
	}

	// 向前读取size个字节，并返回开始位置的指针
	const char* Read( std::size_t size )
	{
		const char *beg = &m_BinBuf[ m_Pos ];
		m_Pos += size;
		return beg;
	}

private:
	// 缓冲区用于存放数据，一般通过继承std::streambuf单独实现一个类，这里简单用vector代替；
	// 一般要有一个二级缓存，等到二级里面满了或调用flush再存到BinBuf里，避免频繁的插入数据；
	std::vector< char > m_BinBuf;
	std::size_t m_Pos;	// 记录当前缓冲区的位置，仅input时用
};


typedef BinaryStream oBinaryStream;
typedef BinaryStream iBinaryStream;


// 这个仅负责基本的内置类型，复杂类型需要重载，处理类型内的成员
template< typename T >
oBinaryStream& operator <<( oBinaryStream &obs, const T &val )
{
	const char *begin = reinterpret_cast< const char* >( &val );
	return obs.Write( begin, begin + sizeof( T ) );;
}

// 举一个储存字符串的例子，第一步要把字符串长度存起来
oBinaryStream& operator <<( oBinaryStream &obs, const std::string &str )
{
	obs << str.size();
	return obs.Write( str.c_str(), str.c_str() + str.size() );
}

// 读取基本内置类型
template< typename T >
iBinaryStream& operator >>( iBinaryStream &ibs, T &val )
{
	std::memcpy( &val, ibs.Read( sizeof( T ) ), sizeof( T ) );
	return ibs;
}

// 读取标准字符串
iBinaryStream& operator >>( iBinaryStream &ibs, std::string &str )
{
	std::size_t size;
	ibs >> size;
	str.assign( ibs.Read( size ), size );
	return ibs;
}

#endif // !BINARYSTREAM_H