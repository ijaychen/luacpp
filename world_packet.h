/*************************************************************************
	> File Name: WorldPacket.h
	> Author: cjj
	> Created Time: 2014年08月23日 星期六 17时11分28秒
 ************************************************************************/
#ifndef WORLD_PACKET_H
#define WORLD_PACKET_H

#include "./include/global.h"
#include "./include/lua.hpp"
#include <iostream>
#include <stdio.h>

class WorldPacket
{
public:
	static const uint32_t HEAD_SIZE = 4;
	union PacketHead {
		char data[4];
		struct {
			uint16_t size;
			uint16_t opCode;
		} head;
	};
	
	WorldPacket(uint16_t opCode, uint16_t size = 256)
	{
		m_pos = 0;
		m_storage.reserve(size);
		m_opCode = opCode;
		skipTo(HEAD_SIZE);
	}
	
	WorldPacket(const WorldPacket& pack) : m_storage(pack.m_storage)
	{
		m_pos = pack.m_pos;
		m_opCode = pack.m_opCode;
	}
	
	virtual ~WorldPacket()
	{
		m_storage.clear();
	}

	uint16_t GetOpcode() const 
	{
		return m_opCode;
	}

	const uint8_t * GetBuffer() const 
	{
		return &m_storage[0];
	}
	
	void Resize(size_t newsize)
	{
		m_storage.resize(newsize);
		m_pos = 0;
	}
	
	uint16_t GetSize() const
	{
		return m_storage.size();
	}
	
	void WriteUShort(uint16_t val)
	{
		PutByte(reinterpret_cast<const uint8_t *>(&val), 2);
	}
	
	void WriteUInt(uint32_t val)
	{
		PutByte(reinterpret_cast<const uint8_t *>(&val), 4);
	}

	void WriteString(const char * src)
	{
		uint16_t len = strlen(src);
		PutByte(reinterpret_cast<const uint8_t *>(&len), 2);
		PutByte((uint8_t*)src, len);
		//uint8_t ch = 0;
		//PutByte((uint8_t*)&ch, sizeof(ch));
	}

	void WriteByte(int8_t val)
	{
		PutByte(reinterpret_cast<const uint8_t *>(&val), 1);
	}
	
	uint16_t ReadUShort()
	{
		uint16_t val;
		GetByte(reinterpret_cast<char*>(&val), 2);
		return val;
	}
	
	uint32_t ReadUInt()
	{
		uint32_t val;
		GetByte(reinterpret_cast<char*>(&val), 4);
		return val;
	}

	void ReadString(std::string &str)
	{
		uint16_t len;
		GetByte(reinterpret_cast<char *>(&len), 2);
		str.resize(len);
		char * dst = const_cast<char*>(str.c_str());
		GetByte(dst, len);
	}

	std::string ReadString()
	{
		std::string str;
		ReadString(str);
		return str;
	}

	int8_t ReadByte()
	{
		int8_t val;
		GetByte(reinterpret_cast<char*>(&val), 1);
		return val;
	}
	
	void skipTo(uint32_t pos)
	{
		m_pos = pos;
	}
	
	void WriteHead()
	{
		skipTo(0);
		if(m_storage.size() < HEAD_SIZE){
			m_storage.resize(HEAD_SIZE);
		}
		WriteUShort(htons(m_storage.size() - HEAD_SIZE + 2));
		WriteUShort(m_opCode);
	}
	void print()
	{
		printf("-----------------------------------pack");
		size_t size = m_storage.size();
		printf("size:%d\n", m_storage.size());	
		//skipTo(HEAD_SIZE);
		//for(int i = 0; i )
		printf("-----------------------------------pack");
	}
private:
	void PutByte(const uint8_t * src, size_t count)
	{
		if(m_pos + count >= m_storage.size())
		{
			m_storage.resize(m_pos + count);
		}
		memcpy(&m_storage[m_pos], src, count);
		m_pos += count;
	}

	void GetByte(char * dst, size_t count)
	{
		if(m_pos < HEAD_SIZE)
		{
			skipTo(HEAD_SIZE);
		}
		//assert(m_pos + count <= m_storage.size());
		if(m_pos + count <= m_storage.size())
		{
			char ch = 0;
			memcpy(dst, &ch, count);
		}
		memcpy(dst, &m_storage[m_pos], count);
		m_pos += count;
	}

private:
	std::vector<uint8_t> m_storage;
	size_t m_pos;
	uint16_t m_opCode;
};

#endif

