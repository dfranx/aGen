#ifndef __AGEN_FUNCTION_MANAGER_H__
#define __AGEN_FUNCTION_MANAGER_H__
#include <agen/ByteCode.h>
#include <agen/Type.h>
#include <vector>
#include <string>

namespace ag
{
	class FunctionManager
	{
	public:
		FunctionManager();
		
		size_t Create(std::string str, ag::Type ret, std::vector<ag::Type> args);
		void SetCurrent(std::string name);
		ByteCode Get(std::string name);
		inline std::vector<std::string> GetNames() { return m_names; }

		// OpCodes
		void Return();
		void PushStack(variant v);
		void PopStack();
		void GetConstant(uint16_t const_id);
		void Add();
		void Subtract();
		void Multiply();
		void Divide();
		void Increment();
		void Decrement();
		void Negate();
		void Modulo();
		void BitOr();
		void BitAnd();
		void BitNot();
		void BitXor();
		void BitLeftShift();
		void BitRightShift();
		void Equal();
		void NotEqual();
		void Not();
		void Greater();
		void Less();
		void GreaterEqual();
		void LessEqual();
		void Nop();
		void Convert(ag::Type type);
		void Duplicate();
		void Swap();
		void GetArgument(uint16_t arg_id);
		void SetArgument(uint16_t arg_id);
		void GetLocal(uint16_t loc_id);
		void SetLocal(uint16_t loc_id);
		void GetGlobal(uint16_t glob_id);
		void SetGlobal(uint16_t glob_id);
		void NewArray(uint8_t dim);
		void Call(std::string name);
		void CallReturn(std::string name);

		uint16_t GetNextLocal();

	private:
		size_t m_cur;
		std::vector<uint16_t> m_locals;
		std::vector<size_t> m_lengthAddr;
		std::vector<ByteCode> m_code;
		std::vector<std::string> m_names;
		std::vector<ag::Type> m_ret;
		std::vector<std::vector<ag::Type>> m_args;
	};
}

#endif // __AGEN_FUNCTION_MANAGER_H__