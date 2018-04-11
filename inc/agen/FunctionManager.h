#ifndef __AGEN_FUNCTION_MANAGER_H__
#define __AGEN_FUNCTION_MANAGER_H__
#include <agen/FunctionData.h>
#include <agen/ByteCode.h>
#include <agen/Type.h>
#include <vector>
#include <string>

namespace ag
{
	class Generator;
	class FunctionManager
	{
	public:
		FunctionManager(Generator& gen);
		
		size_t Create(std::string str, ag::Type ret, uint8_t args = 0);
		void SetCurrent(std::string name);
		void SetCurrent(std::string obj, std::string name);
		ByteCode Get(std::string name, std::string obj = "");
		inline std::vector<FunctionData> GetData() { return m_funcs; }

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
		void BoolOr();
		void BoolAnd();
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
		void SetArrayElement();
		void GetArrayElement();
		void Call(std::string name, uint8_t argc);
		void CallReturn(std::string name, uint8_t argc);
		void IsTypeOf(ag::Type type);
		void NewObject(uint16_t object_id, uint8_t argc = 0);
		void PushNull();
		void GetProperty(uint16_t loc_id, std::string prop);
		void SetProperty(uint16_t loc_id, std::string prop);
		void GetProperty(std::string prop);
		void SetProperty(std::string prop);
		void GetMyProperty(std::string prop);
		void SetMyProperty(std::string prop);
		void CallMethod(std::string mtd, uint8_t argc);
		void CallMyMethod(std::string mtd, uint8_t argc);
		void CallReturnMethod(std::string mtd, uint8_t argc);
		void CallMyReturnMethod(std::string mtd, uint8_t argc);
		size_t If();
		size_t Goto();
		void ScopeStart();
		void ScopeEnd();

		void SetAddress(size_t id, size_t addr);
		size_t GetCurrentAddress();
		uint16_t GetNextLocal();

	private:
		Generator& m_gen;
		size_t m_cur;
		std::vector<uint16_t> m_locals;
		std::vector<size_t> m_lengthAddr;
		std::vector<std::vector<std::pair<size_t,size_t>>> m_linkAddr;
		std::vector<ByteCode> m_code;
		std::vector<FunctionData> m_funcs;
	};
}

#endif // __AGEN_FUNCTION_MANAGER_H__