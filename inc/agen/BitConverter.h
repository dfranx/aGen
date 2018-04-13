#ifndef __AGEN_BIT_CONVERTER_H__
#define __AGEN_BIT_CONVERTER_H__
#include <agen/ByteCode.h>
#include <agen/Type.h>
#include <string>

namespace ag
{
	class BitConverter
	{
	public:	
		static std::vector<uint8_t> Get(std::string s, bool place_null = true);
		static std::vector<uint8_t> Get(uint16_t u);
		static std::vector<uint8_t> Get(int u);
		static std::vector<uint8_t> Get(unsigned int u);
		static std::vector<uint8_t> Get(float u);
		static std::vector<uint8_t> Get(bool u);
		static std::vector<uint8_t> Get(variant v);
	};
}

#endif //__AGEN_BIT_CONVERTER_H__