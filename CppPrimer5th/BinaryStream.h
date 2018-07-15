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


// �򵥵Ķ������ڴ���ʵ�֣�input/output����һ�����ˣ�
// ����Ч��ͨ�õ�ʵ�ֲο�boost serialization�ķ�װ���Ǹ�ʡȥ�������ڴ�����ؿ�����������ܣ�
class BinaryStream
{
public:
	std::size_t Size() const { return m_BinBuf.size(); }

	// ��output���л�ȡ�Ѵ洢�����ݣ��浽���ݿ�ǰһ�㻹Ҫ��zip�㷨ѹ��һ��
	const char* Memory() const
	{
		return reinterpret_cast< const char* >( &m_BinBuf[ 0 ] );
	}

	// ����input���Ļ������ݣ�ͬʱ��ȡλ�ù���
	void Memory( const char *buf, std::size_t size )
	{
		m_BinBuf.assign( buf, buf + size );
		m_Pos = 0;
	}

	// д��һ����Χ������
	BinaryStream& Write( const char *begin, const char *end )
	{
		m_BinBuf.insert( m_BinBuf.end(), begin, end );
		return *this;
	}

	// ��ǰ��ȡsize���ֽڣ������ؿ�ʼλ�õ�ָ��
	const char* Read( std::size_t size )
	{
		const char *beg = &m_BinBuf[ m_Pos ];
		m_Pos += size;
		return beg;
	}

private:
	// ���������ڴ�����ݣ�һ��ͨ���̳�std::streambuf����ʵ��һ���࣬�������vector���棻
	// һ��Ҫ��һ���������棬�ȵ������������˻����flush�ٴ浽BinBuf�����Ƶ���Ĳ������ݣ�
	std::vector< char > m_BinBuf;
	std::size_t m_Pos;	// ��¼��ǰ��������λ�ã���inputʱ��
};


typedef BinaryStream oBinaryStream;
typedef BinaryStream iBinaryStream;


// ���������������������ͣ�����������Ҫ���أ����������ڵĳ�Ա
template< typename T >
oBinaryStream& operator <<( oBinaryStream &obs, const T &val )
{
	const char *begin = reinterpret_cast< const char* >( &val );
	return obs.Write( begin, begin + sizeof( T ) );;
}

// ��һ�������ַ��������ӣ���һ��Ҫ���ַ������ȴ�����
oBinaryStream& operator <<( oBinaryStream &obs, const std::string &str )
{
	obs << str.size();
	return obs.Write( str.c_str(), str.c_str() + str.size() );
}

// ��ȡ������������
template< typename T >
iBinaryStream& operator >>( iBinaryStream &ibs, T &val )
{
	std::memcpy( &val, ibs.Read( sizeof( T ) ), sizeof( T ) );
	return ibs;
}

// ��ȡ��׼�ַ���
iBinaryStream& operator >>( iBinaryStream &ibs, std::string &str )
{
	std::size_t size;
	ibs >> size;
	str.assign( ibs.Read( size ), size );
	return ibs;
}

#endif // !BINARYSTREAM_H