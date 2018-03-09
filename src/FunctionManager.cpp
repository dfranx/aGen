#include <agen/FunctionManager.h>
#include <agen/BitConverter.h>
#include <agen/OpCode.h>

namespace ag
{
	FunctionManager::FunctionManager()
	{
		m_cur = 0;
	}

	size_t FunctionManager::Create(std::string name, ag::Type ret, std::vector<ag::Type> args)
	{
		size_t id = m_code.size();

		m_code.push_back(ByteCode());
		m_names.push_back(name);
		m_ret.push_back(ret);
		m_linkAddr.resize(m_linkAddr.size() + 1);
		m_locals.push_back(0);
		m_args.resize(id + 1);
		m_args[id] = args;

		m_code[id].Add(OpCode::FunctionStart);
		m_code[id].Add(ret);
		m_code[id].Add((uint8_t)args.size());
		for (size_t i = 0; i < args.size(); i++)
			m_code[id].Add(args[i]);
		
		m_lengthAddr.push_back(m_code[id].Count());

		m_code[id].Add(BitConverter::Get(0u));

		return id;
	}
	void FunctionManager::SetCurrent(std::string n) {
		for (size_t i = 0; i < m_names.size(); i++)
			if (m_names[i] == n) {
				m_cur = i;
				break;
			}
	}

	ByteCode FunctionManager::Get(std::string name)
	{
		size_t i = 0;
		for (; i < m_names.size(); i++)
			if (m_names[i] == name) break;
		
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
		m_code[m_cur].Add(BitConverter::Get(loc_id + (uint16_t)m_args.size()));
	}
	void FunctionManager::SetLocal(uint16_t loc_id)
	{
		m_locals[m_cur] = std::max(m_locals[m_cur], loc_id);

		m_code[m_cur].Add(OpCode::SetLocal);
		m_code[m_cur].Add(BitConverter::Get(loc_id + (uint16_t)m_args.size()));
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
	void FunctionManager::Call(std::string name, uint8_t argc)
	{
		m_code[m_cur].Add(OpCode::Call);
		m_code[m_cur].Add(BitConverter::Get(name, true));
		m_code[m_cur].Add(argc);
	}
	void FunctionManager::CallReturn(std::string name, uint8_t argc)
	{
		m_code[m_cur].Add(OpCode::CallReturn);
		m_code[m_cur].Add(BitConverter::Get(name, true));
		m_code[m_cur].Add(argc);
	}
	void FunctionManager::IsTypeOf(ag::Type type)
	{
		m_code[m_cur].Add(OpCode::IsTypeOf);
		m_code[m_cur].Add(type);
	}
	size_t& FunctionManager::If()
	{
		m_code[m_cur].Add(OpCode::If);
		
		size_t cur_if = m_linkAddr[m_cur].size();
		m_linkAddr[m_cur].resize(cur_if + 1);
		m_linkAddr[m_cur][cur_if] = std::make_pair(m_code[m_cur].Count(), 0);

		m_code[m_cur].Add(BitConverter::Get(0u));

		return m_linkAddr[m_cur][cur_if].second;
	}
	size_t & FunctionManager::Goto()
	{
		m_code[m_cur].Add(OpCode::Goto);

		size_t cur_link = m_linkAddr[m_cur].size();
		m_linkAddr[m_cur].resize(cur_link + 1);
		m_linkAddr[m_cur][cur_link] = std::make_pair(m_code[m_cur].Count(), 0);

		m_code[m_cur].Add(BitConverter::Get(0u));

		return m_linkAddr[m_cur][cur_link].second;
	}
	void FunctionManager::GetCurrentAddress(size_t& adr)
	{
		adr = m_code[m_cur].Count();
	}
	uint16_t FunctionManager::GetNextLocal()
	{
		return m_locals[m_cur] + 1 + (uint16_t)m_args.size();
	}
}