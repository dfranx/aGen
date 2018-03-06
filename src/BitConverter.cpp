#include <agen/BitConverter.h>
#include <mpark/variant.hpp>

namespace ag
{
	std::vector<uint8_t> BitConverter::Get(std::string s, bool place_null)
	{
		std::vector<uint8_t> ret(s.size()+place_null);
		
		for (size_t i = 0; i < s.size(); i++)
			ret[i] = s[i];

		if (place_null)
			ret[ret.size() - 1] = 0;

		return ret;
	}
	std::vector<uint8_t> BitConverter::Get(uint16_t u)
	{
		std::vector<uint8_t> ret(2);
		
		ret[0] = u & 0x00FF;
		ret[1] = (u >> 8) & 0x00FF;

		return ret;
	}
	std::vector<uint8_t> BitConverter::Get(int u)
	{
		std::vector<uint8_t> ret(4);

		ret[0] = u & 0x00FF;
		ret[1] = (u >> 8) & 0x00FF;
		ret[2] = (u >> 16) & 0x00FF;
		ret[3] = (u >> 24) & 0x00FF;

		return ret;
	}
	std::vector<uint8_t> BitConverter::Get(unsigned int u)
	{
		std::vector<uint8_t> ret(4);

		ret[0] = u & 0xFF;
		ret[1] = (u >> 8) & 0xFF;
		ret[2] = (u >> 16) & 0xFF;
		ret[3] = (u >> 24) & 0xFF;

		return ret;
	}
	std::vector<uint8_t> BitConverter::Get(float u)
	{
		uint8_t const * p = reinterpret_cast<uint8_t const*>(&u);

		std::vector<uint8_t> ret(sizeof(float));
		for (size_t i = 0; i < sizeof(float); i++)
			ret[i] = p[i];

		return ret;
	}
	std::vector<uint8_t> BitConverter::Get(bool u)
	{
		return { (uint8_t)u };
	}
	std::vector<uint8_t> BitConverter::Get(variant v)
	{
		if (mpark::holds_alternative<int>(v))
			return BitConverter::Get(mpark::get<int>(v));
		else if (mpark::holds_alternative<char>(v))
			return BitConverter::Get(mpark::get<char>(v));
		else if (mpark::holds_alternative<unsigned int>(v))
			return BitConverter::Get(mpark::get<unsigned int>(v));
		else if (mpark::holds_alternative<float>(v))
			return BitConverter::Get(mpark::get<float>(v));
		else if (mpark::holds_alternative<std::string>(v))
			return BitConverter::Get(mpark::get<std::string>(v), true);
		return std::vector<uint8_t>();
	}
}