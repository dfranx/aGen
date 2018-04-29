#ifndef __AGEN_FUNCTION_DATA_H__
#define __AGEN_FUNCTION_DATA_H__
#include <agen/Type.h>
#include <vector>
#include <string>

namespace ag
{
	class FunctionData
	{
	public:
		FunctionData() : Object(""), Name(""), Arguments(0) {}
		FunctionData(std::string n, uint8_t args) : Object(""), Name(n), Arguments(args) { }
		FunctionData(std::string obj, std::string n, uint8_t args) : Object(obj), Name(n), Arguments(args) { }

		std::string Object;
		std::string Name;
		uint8_t Arguments;
	};
}

#endif //__AGEN_FUNCTION_DATA_H__