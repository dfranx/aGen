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
		FunctionData() : Name(""), Return(ag::Type::Void) {}
		FunctionData(std::string n, ag::Type ret, std::vector<ag::Type> args) : Name(n), Return(ret), Arguments(args) { }

		std::string Name;
		ag::Type Return;
		std::vector<ag::Type> Arguments;
	};
}

#endif //__AGEN_FUNCTION_DATA_H__