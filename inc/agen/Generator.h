#ifndef __AGEN_GENERATOR_H__
#define __AGEN_GENERATOR_H__

#include <agen/FunctionManager.h>
#include <agen/BitConverter.h>
#include <agen/ByteCode.h>
#include <agen/Type.h>

namespace ag
{
	class Generator
	{
	public:
		Generator();

		void SetHeader(uint8_t major, uint16_t minor);
		size_t AddGlobal(std::string name);
		size_t AddConstant(variant i);

		FunctionManager Function;

		ByteCode Get();

	private:
		uint8_t m_ver_major;
		uint16_t m_ver_minor;
		std::vector<std::string> m_globals;
		std::vector<variant> m_consts;
	};
}

#endif //__AGEN_GENERATOR_H__