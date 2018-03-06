#ifndef __AGEN_TYPES_H__
#define __AGEN_TYPES_H__
#include <mpark/variant.hpp>

namespace ag
{
	enum class Type
	{
		Void,
		Int,
		Short,
		Char,
		UInt,
		UShort,
		UChar,
		Float,
		String,
		Array
	};

	const int variant_type_count = 5;
	typedef mpark::variant<char, unsigned int, int, float, std::string> variant;

	inline Type VariantToType(variant v) {
		switch (v.index()) {
		case 0: return Type::Char;
		case 1: return Type::UInt;
		case 2: return Type::Int;
		case 3: return Type::Float;
		case 4: return Type::String;
		default: return Type::Void;
		}
	}
}

#endif // __AGEN_TYPES_H__