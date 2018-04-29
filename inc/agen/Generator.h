#ifndef __AGEN_GENERATOR_H__
#define __AGEN_GENERATOR_H__

#include <agen/FunctionManager.h>
#include <agen/BitConverter.h>
#include <agen/ByteCode.h>
#include <agen/Type.h>
#include <string>

namespace ag
{
	class Generator
	{
	public:
		Generator();

		void SetHeader(uint8_t major, uint16_t minor);
		size_t AddGlobal(std::string name);
		size_t AddConstant(variant i);
		size_t AddObject(std::string name);
		void AddProperty(std::string obj, std::string prop);
		void AddMethod(std::string obj, std::string name, uint8_t args = 0);
		FunctionData GetMethod(std::string obj, std::string name);

		int AddString(const std::string& str);
		int GetStringIndex(const std::string& str);

		FunctionManager Function;

		ByteCode Get();

	private:
		uint8_t m_ver_major;
		uint16_t m_ver_minor;
		std::vector<std::string> m_strtbl;
		std::vector<std::string> m_globals;
		std::vector<std::string> m_objNames;
		std::vector<std::vector<std::string>> m_objProps;
		std::vector<std::vector<FunctionData>> m_objMethods;
		std::vector<variant> m_consts;
	};
}

#endif //__AGEN_GENERATOR_H__