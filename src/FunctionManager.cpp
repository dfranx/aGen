#include <agen/FunctionManager.h>
#include <agen/BitConverter.h>
#include <agen/Generator.h>
#include <agen/OpCode.h>

namespace ag
{
	FunctionManager::FunctionManager(Generator& g) : m_gen(g), m_cur(0) { }

	size_t FunctionManager::Create(std::string name, ag::Type ret, uint8_t arg_count)
	{
		size_t id = m_code.size();

		m_code.push_back(ByteCode());
		m_funcs.push_back(FunctionData(name, ret, arg_count));
		m_linkAddr.resize(m_linkAddr.size() + 1);
		m_locals.push_back(0);
		
		m_code[id].Add(OpCode::FunctionStart);
		m_code[id].Add(ret);
		m_code[id].Add(arg_count);
		
		m_lengthAddr.push_back(m_code[id].Count());

		m_code[id].Add(BitConverter::Get(0u));

		return id;
	}
	void FunctionManager::SetCurrent(std::string n) {
		for (size_t i = 0; i < m_funcs.size(); i++)
			if (m_funcs[i].Name == n) {
				m_cur = i;
				break;
			}
	}

	void FunctionManager::SetCurrent(std::string obj, std::string name)
	{
		size_t id = m_code.size();

		FunctionData data = m_gen.GetMethod(obj, name);

		m_code.push_back(ByteCode());
		m_funcs.push_back(data);
		m_linkAddr.resize(m_linkAddr.size() + 1);
		m_locals.push_back(0);

		m_code[id].Add(OpCode::FunctionStart);
		m_code[id].Add(data.Return);
		m_code[id].Add(data.Arguments);

		m_lengthAddr.push_back(m_code[id].Count());

		m_code[id].Add(BitConverter::Get(0u));

		m_cur = id;
	}

	ByteCode FunctionManager::Get(std::string name, std::string obj)
	{
		size_t i = 0;
		for (; i < m_funcs.size(); i++)
			if (m_funcs[i].Name == name && m_funcs[i].Object == obj) break;
		
		m_code[i].Write(m_lengthAddr[i],
			BitConverter::Get(
				m_code[i].Count() - (m_lengthAddr[i] + sizeof(uint32_t))
			)
		);

		for (size_t j = 0; j < m_linkAddr[i].size(); j++)
			m_code[i].Write(m_linkAddr[i][j].first,
				BitConverter::Get(
					m_linkAddr[i][j].second - (m_lengthAddr[i] + sizeof(uint32_t))
				)
			);

		return m_code[i];
	}

	void FunctionManager::Return()
	{
		m_code[m_cur].Add(OpCode::Return);
	}
	void FunctionManager::PushStack(variant v)
	{
		m_code[m_cur].Add(OpCode::PushStack);
		m_code[m_cur].Add(VariantToType(v));

		if (VariantToType(v) == ag::Type::String)
			m_code[m_cur].Add(BitConverter::Get(m_gen.AddString(mpark::get<std::string>(v)))); // only push string id (string table)
		else
			m_code[m_cur].Add(BitConverter::Get(v));
	}
	void FunctionManager::PopStack()
	{
		m_code[m_cur].Add(OpCode::PopStack);
	}
	void FunctionManager::GetConstant(uint16_t const_id)
	{
		m_code[m_cur].Add(OpCode::GetConstant);
		m_code[m_cur].Add(BitConverter::Get(const_id));
	}
	void FunctionManager::Add()
	{
		m_code[m_cur].Add(OpCode::Add);
	}
	void FunctionManager::Subtract()
	{
		m_code[m_cur].Add(OpCode::Subtract);
	}
	void FunctionManager::Multiply()
	{
		m_code[m_cur].Add(OpCode::Multiply);
	}
	void FunctionManager::Divide()
	{
		m_code[m_cur].Add(OpCode::Divide);
	}
	void FunctionManager::Increment()
	{
		m_code[m_cur].Add(OpCode::Increment);
	}
	void FunctionManager::Decrement()
	{
		m_code[m_cur].Add(OpCode::Decrement);
	}
	void FunctionManager::Negate()
	{
		m_code[m_cur].Add(OpCode::Negate);
	}
	void FunctionManager::Modulo()
	{
		m_code[m_cur].Add(OpCode::Modulo);
	}
	void FunctionManager::BitOr()
	{
		m_code[m_cur].Add(OpCode::BitOr);
	}
	void FunctionManager::BitAnd()
	{
		m_code[m_cur].Add(OpCode::BitAnd);
	}
	void FunctionManager::BitNot()
	{
		m_code[m_cur].Add(OpCode::BitNot);
	}
	void FunctionManager::BitXor()
	{
		m_code[m_cur].Add(OpCode::BitXor);
	}
	void FunctionManager::BitLeftShift()
	{
		m_code[m_cur].Add(OpCode::BitLeftShift);
	}
	void FunctionManager::BitRightShift()
	{
		m_code[m_cur].Add(OpCode::BitRightShift);
	}
	void FunctionManager::BoolOr()
	{
		m_code[m_cur].Add(OpCode::BoolOr);
	}
	void FunctionManager::BoolAnd()
	{
		m_code[m_cur].Add(OpCode::BoolAnd);
	}
	void FunctionManager::Equal()
	{
		m_code[m_cur].Add(OpCode::Equal);
	}
	void FunctionManager::NotEqual()
	{
		m_code[m_cur].Add(OpCode::NotEqual);
	}
	void FunctionManager::Not()
	{
		m_code[m_cur].Add(OpCode::Not);
	}
	void FunctionManager::Greater()
	{
		m_code[m_cur].Add(OpCode::Greater);
	}
	void FunctionManager::Less()
	{
		m_code[m_cur].Add(OpCode::Less);
	}
	void FunctionManager::GreaterEqual()
	{
		m_code[m_cur].Add(OpCode::GreaterEqual);
	}
	void FunctionManager::LessEqual()
	{
		m_code[m_cur].Add(OpCode::LessEqual);
	}
	void FunctionManager::Nop()
	{
		m_code[m_cur].Add(OpCode::Nop);
	}
	void FunctionManager::Convert(ag::Type type)
	{
		m_code[m_cur].Add(OpCode::Convert);
		m_code[m_cur].Add(type);
	}
	void FunctionManager::Duplicate()
	{
		m_code[m_cur].Add(OpCode::Duplicate);
	}
	void FunctionManager::Swap()
	{
		m_code[m_cur].Add(OpCode::Swap);
	}
	void FunctionManager::GetArgument(uint16_t arg_id)
	{
		m_code[m_cur].Add(OpCode::GetLocal);
		m_code[m_cur].Add(BitConverter::Get(arg_id));
	}
	void FunctionManager::SetArgument(uint16_t arg_id)
	{
		m_code[m_cur].Add(OpCode::SetLocal);
		m_code[m_cur].Add(BitConverter::Get(arg_id));
	}
	void FunctionManager::GetLocal(uint16_t loc_id)
	{
		m_code[m_cur].Add(OpCode::GetLocal);
		m_code[m_cur].Add(BitConverter::Get((uint16_t)(loc_id + m_funcs[m_cur].Arguments)));
	}
	void FunctionManager::SetLocal(uint16_t loc_id)
	{
		m_locals[m_cur] = std::max(m_locals[m_cur], loc_id);

		m_code[m_cur].Add(OpCode::SetLocal);
		m_code[m_cur].Add(BitConverter::Get((uint16_t)(loc_id + m_funcs[m_cur].Arguments)));
	}
	void FunctionManager::GetGlobal(uint16_t glob_id)
	{
		m_code[m_cur].Add(OpCode::GetGlobal);
		m_code[m_cur].Add(BitConverter::Get(glob_id));
	}
	void FunctionManager::SetGlobal(uint16_t glob_id)
	{
		m_code[m_cur].Add(OpCode::SetGlobal);
		m_code[m_cur].Add(BitConverter::Get(glob_id));
	}
	void FunctionManager::NewArray(uint8_t dim)
	{
		m_code[m_cur].Add(OpCode::NewArray);
		m_code[m_cur].Add(dim);
	}
	void FunctionManager::SetArrayElement()
	{
		m_code[m_cur].Add(OpCode::SetArrayElement);
	}
	void FunctionManager::GetArrayElement()
	{
		m_code[m_cur].Add(OpCode::GetArrayElement);
	}
	void FunctionManager::Call(std::string name, uint8_t argc)
	{
		m_code[m_cur].Add(OpCode::Call);
		m_code[m_cur].Add(BitConverter::Get(m_gen.AddString(name)));
		m_code[m_cur].Add(argc);
	}
	void FunctionManager::CallReturn(std::string name, uint8_t argc)
	{
		m_code[m_cur].Add(OpCode::CallReturn);
		m_code[m_cur].Add(BitConverter::Get(m_gen.AddString(name)));
		m_code[m_cur].Add(argc);
	}
	void FunctionManager::IsTypeOf(ag::Type type)
	{
		m_code[m_cur].Add(OpCode::IsTypeOf);
		m_code[m_cur].Add(type);
	}
	void FunctionManager::NewObject(uint16_t object_id, uint8_t argc)
	{
		m_code[m_cur].Add(OpCode::NewObject);
		m_code[m_cur].Add(BitConverter::Get(object_id+1));
		m_code[m_cur].Add(argc);
	}
	void FunctionManager::PushNull()
	{
		m_code[m_cur].Add(OpCode::NewObject);
		m_code[m_cur].Add(BitConverter::Get((uint16_t)0));
		m_code[m_cur].Add(0);
	}
	void FunctionManager::GetProperty(uint16_t loc_id, std::string prop)
	{
		GetLocal(loc_id);
		m_code[m_cur].Add(OpCode::GetProperty);
		m_code[m_cur].Add(BitConverter::Get(m_gen.AddString(prop)));
	}
	void FunctionManager::SetProperty(uint16_t loc_id, std::string prop)
	{
		GetLocal(loc_id);
		m_code[m_cur].Add(OpCode::SetProperty);
		m_code[m_cur].Add(BitConverter::Get(m_gen.AddString(prop)));
		SetLocal(loc_id);
	}
	void FunctionManager::GetProperty(std::string prop)
	{
		m_code[m_cur].Add(OpCode::GetProperty);
		m_code[m_cur].Add(BitConverter::Get(m_gen.AddString(prop)));
	}
	void FunctionManager::SetProperty(std::string prop)
	{
		m_code[m_cur].Add(OpCode::SetProperty);
		m_code[m_cur].Add(BitConverter::Get(m_gen.AddString(prop)));
	}
	void FunctionManager::GetMyProperty(std::string prop)
	{
		m_code[m_cur].Add(OpCode::GetMyProperty);
		m_code[m_cur].Add(BitConverter::Get(m_gen.AddString(prop)));
	}
	void FunctionManager::SetMyProperty(std::string prop)
	{
		m_code[m_cur].Add(OpCode::SetMyProperty);
		m_code[m_cur].Add(BitConverter::Get(m_gen.AddString(prop)));
	}
	void FunctionManager::CallMethod(std::string mtd, uint8_t argc)
	{
		m_code[m_cur].Add(OpCode::CallMethod);
		m_code[m_cur].Add(BitConverter::Get(m_gen.AddString(mtd)));
		m_code[m_cur].Add(argc);
	}
	void FunctionManager::CallMyMethod(std::string mtd, uint8_t argc)
	{
		m_code[m_cur].Add(OpCode::CallMyMethod);
		m_code[m_cur].Add(BitConverter::Get(m_gen.AddString(mtd)));
		m_code[m_cur].Add(argc);
	}
	void FunctionManager::CallReturnMethod(std::string mtd, uint8_t argc)
	{
		m_code[m_cur].Add(OpCode::CallReturnMethod);
		m_code[m_cur].Add(BitConverter::Get(m_gen.AddString(mtd)));
		m_code[m_cur].Add(argc);
	}
	void FunctionManager::CallMyReturnMethod(std::string mtd, uint8_t argc)
	{
		m_code[m_cur].Add(OpCode::CallMyReturnMethod);
		m_code[m_cur].Add(BitConverter::Get(m_gen.AddString(mtd)));
		m_code[m_cur].Add(argc);
	}
	size_t FunctionManager::If()
	{
		m_code[m_cur].Add(OpCode::If);
		
		size_t cur_if = m_linkAddr[m_cur].size();
		m_linkAddr[m_cur].resize(cur_if + 1);
		m_linkAddr[m_cur][cur_if] = std::make_pair(m_code[m_cur].Count(), 0);

		m_code[m_cur].Add(BitConverter::Get(0u));

		return cur_if;
	}
	size_t FunctionManager::Goto()
	{
		m_code[m_cur].Add(OpCode::Goto);

		size_t cur_link = m_linkAddr[m_cur].size();
		m_linkAddr[m_cur].resize(cur_link + 1);
		m_linkAddr[m_cur][cur_link] = std::make_pair(m_code[m_cur].Count(), 0);

		m_code[m_cur].Add(BitConverter::Get(0u));

		return cur_link;
	}
	void FunctionManager::ScopeStart()
	{
		m_code[m_cur].Add(OpCode::ScopeStart);
	}
	void FunctionManager::ScopeEnd()
	{
		m_code[m_cur].Add(OpCode::ScopeEnd);
	}
	void FunctionManager::SetAddress(size_t id, size_t addr)
	{
		m_linkAddr[m_cur][id].second = addr;
	}
	size_t FunctionManager::GetCurrentAddress()
	{
		return m_code[m_cur].Count();
	}
	uint16_t FunctionManager::GetNextLocal()
	{
		return m_locals[m_cur] + 1 + m_funcs[m_cur].Arguments;
	}
}