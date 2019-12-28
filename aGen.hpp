#ifndef __AGEN_HPP__
#define __AGEN_HPP__
#include <vector>
#include <variant>
#include <string>
#include <algorithm>

namespace ag
{
	enum class OpCode
	{
		Unknown,
		FunctionStart,
		Return,
		PushStack,
		PopStack,
		GetConstant,
		Add,
		Subtract,
		Multiply,
		Divide,
		Increment,
		Decrement,
		Negate,
		Modulo,
		BitOr,
		BitAnd,
		BitNot,
		BitXor,
		BitLeftShift,
		BitRightShift,
		BoolOr,
		BoolAnd,
		Equal,
		NotEqual,
		Not,
		Greater,
		Less,
		GreaterEqual,
		LessEqual,
		Nop,
		Convert,
		Duplicate,
		Swap,
		GetLocal,
		SetLocal,
		GetGlobal,
		SetGlobal,
		NewArray,
		SetArrayElement,
		GetArrayElement,
		Call,
		CallReturn,
		IsTypeOf,
		If,
		Goto,
		NewObject,
		SetProperty,
		SetMyProperty,
		GetProperty,
		GetMyProperty,
		CallMethod,
		CallMyMethod,
		CallReturnMethod,
		CallMyReturnMethod,
		ScopeStart,
		ScopeEnd,
		Assign,
		GetLocalPointer,
		GetGlobalPointer,
		GetPropertyPointer,
		GetMyPropertyPointer,
		GetGlobalByName,
		GetGlobalPointerByName,
		SetGlobalByName,
		EmptyStack,
		DebugLineNumber,
		DebugFile,
		Breakpoint,
		NewObjectByName,
		PushStackFunction,
		CallStack
	};

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
		Array,
		Object,
		Pointer,
		Function
	};

	const int variant_type_count = 5;
	typedef std::variant<char, unsigned int, int, float, std::string> agvariant;

	inline Type VariantToType(agvariant v) {
		switch (v.index()) {
		case 0: return Type::Char;
		case 1: return Type::UInt;
		case 2: return Type::Int;
		case 3: return Type::Float;
		case 4: return Type::String;
		default: return Type::Void;
		}
	}

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

	class ByteCode
	{
	public:
		inline void Set(size_t ind, uint8_t val) { m_bytes[ind] = val; }
		inline void Write(size_t ind, const std::vector<uint8_t>& bytes) {
			for (size_t i = 0; i < bytes.size(); i++)
				m_bytes[i + ind] = bytes[i];
		}
		inline void Add(uint8_t b) { m_bytes.push_back(b); }
		inline void Add(OpCode b) { m_bytes.push_back((uint8_t)b); }
		inline void Add(Type b) { m_bytes.push_back((uint8_t)b); }
		inline void Add(ByteCode bc) { Add(bc.Get()); }
		inline void Add(const std::vector<uint8_t>& bytes) { m_bytes.insert(m_bytes.end(), bytes.begin(), bytes.end()); }
		inline size_t Count() { return m_bytes.size(); }
		inline uint8_t Get(size_t ind) { return m_bytes[ind]; }

		inline std::vector<uint8_t> Get() { return m_bytes; }

	private:
		std::vector<uint8_t> m_bytes;
	};

	namespace BitConverter
	{
		inline std::vector<uint8_t> GetString(std::string s, bool place_null = true)
		{
			std::vector<uint8_t> ret(s.size() + place_null);

			for (size_t i = 0; i < s.size(); i++)
				ret[i] = s[i];

			if (place_null)
				ret[ret.size() - 1] = 0;

			return ret;
		}
		inline std::vector<uint8_t> GetUInt16(uint16_t u)
		{
			std::vector<uint8_t> ret(2);

			ret[0] = u & 0x00FF;
			ret[1] = (u >> 8) & 0x00FF;

			return ret;
		}
		inline std::vector<uint8_t> GetInt32(int u)
		{
			std::vector<uint8_t> ret(4);

			ret[0] = u & 0x00FF;
			ret[1] = (u >> 8) & 0x00FF;
			ret[2] = (u >> 16) & 0x00FF;
			ret[3] = (u >> 24) & 0x00FF;

			return ret;
		}
		inline std::vector<uint8_t> GetUInt32(unsigned int u)
		{
			std::vector<uint8_t> ret(4);

			ret[0] = u & 0xFF;
			ret[1] = (u >> 8) & 0xFF;
			ret[2] = (u >> 16) & 0xFF;
			ret[3] = (u >> 24) & 0xFF;

			return ret;
		}
		inline std::vector<uint8_t> GetFloat(float u)
		{
			uint8_t const * p = reinterpret_cast<uint8_t const*>(&u);

			std::vector<uint8_t> ret(sizeof(float));
			for (size_t i = 0; i < sizeof(float); i++)
				ret[i] = p[i];

			return ret;
		}
		inline std::vector<uint8_t> GetBool(bool u)
		{
			return { (uint8_t)u };
		}
		inline std::vector<uint8_t> Get(agvariant v)
		{
			if (std::holds_alternative<int>(v))
				return BitConverter::GetInt32(std::get<int>(v));
			else if (std::holds_alternative<char>(v))
				return { (uint8_t)std::get<char>(v) };
			else if (std::holds_alternative<unsigned int>(v))
				return BitConverter::GetUInt32(std::get<unsigned int>(v));
			else if (std::holds_alternative<float>(v))
				return BitConverter::GetFloat(std::get<float>(v));
			else if (std::holds_alternative<std::string>(v))
				return BitConverter::GetString(std::get<std::string>(v));
			return std::vector<uint8_t>();
		}
	}

	class Generator
	{
	public:
		Generator() :
			m_ver_major(0),
			m_ver_minor(1),
			Function(this)
		{
		}

		inline void SetHeader(uint8_t major, uint16_t minor)
		{
			m_ver_major = major;
			m_ver_minor = minor;
		}
		inline uint16_t GetVersionMinor() { return m_ver_minor; }
		inline uint8_t GetVersionMajor() { return m_ver_major; }
		inline size_t AddGlobal(std::string name)
		{
			m_globals.push_back(name);
			return m_globals.size() - 1;
		}
		inline size_t AddConstant(agvariant var)
		{
			m_consts.push_back(var);

			int ret = 0;
			for (size_t i = 0; i < m_consts.size(); i++)
				if (m_consts[i].index() == var.index())
					ret++;
			return ret - 1;
		}
		inline size_t AddObject(std::string name)
		{
			size_t id = m_objNames.size();
			m_objNames.push_back(name);
			m_objProps.resize(id + 1);
			m_objMethods.resize(id + 1);
			return id;
		}
		inline void AddProperty(std::string obj, std::string prop)
		{
			for (size_t i = 0; i < m_objNames.size(); i++)
				if (m_objNames[i] == obj) {
					m_objProps[i].push_back(prop);
					break;
				}
		}
		inline void AddMethod(std::string obj, std::string name, uint8_t args = 0)
		{
			for (size_t i = 0; i < m_objNames.size(); i++)
				if (m_objNames[i] == obj) {
					m_objMethods[i].push_back(FunctionData(obj, name, args));
					break;
				}
		}
		inline FunctionData GetMethod(std::string obj, std::string name)
		{
			for (size_t i = 0; i < m_objNames.size(); i++)
				if (m_objNames[i] == obj)
					for (size_t j = 0; j < m_objMethods[i].size(); j++)
						if (m_objMethods[i][j].Name == name)
							return m_objMethods[i][j];

			return FunctionData();
		}

		inline int AddString(const std::string& str)
		{
			int ind = GetStringIndex(str);
			if (ind != -1)
				return ind;

			m_strtbl.push_back(str);

			return m_strtbl.size() - 1;
		}
		inline int GetStringIndex(const std::string& str)
		{
			for (size_t i = 0; i < m_strtbl.size(); i++)
				if (m_strtbl[i] == str)
					return i;
			return -1;
		}

		class FunctionManager
		{
		public:
			FunctionManager(Generator* gen) : m_gen(gen), m_cur(0) { }

			inline size_t Create(std::string name, uint8_t argc = 0, std::vector<ag::Type> atypes = {}, std::vector<std::string> anames = {})
			{
				size_t id = m_code.size();

				m_code.push_back(ByteCode());
				m_funcs.push_back(FunctionData(name, argc));
				m_linkAddr.resize(m_linkAddr.size() + 1);
				m_locals.push_back(0);

				m_code[id].Add(OpCode::FunctionStart);
				m_code[id].Add(argc);

				if (!(m_gen->GetVersionMajor() == 0 && m_gen->GetVersionMinor() == 1)) {
					// TODO: argument types, m_code[id].Add()
					if (atypes.size() == anames.size() && atypes.size() == argc) {
						for (uint8_t i = 0; i < atypes.size(); i++) {
							m_code[id].Add(atypes[i]);
							if (atypes[i] == ag::Type::Object)
								m_code[id].Add(BitConverter::GetString(anames[i]));
						}
					}
					else
						for (uint8_t i = 0; i < argc; i++)
							m_code[id].Add(ag::Type::Void);
				}

				m_lengthAddr.push_back(m_code[id].Count());

				m_code[id].Add(BitConverter::GetUInt32(0u));

				return id;
			}
			inline void SetCurrent(std::string name)
			{
				for (size_t i = 0; i < m_funcs.size(); i++)
					if (m_funcs[i].Name == name) {
						m_cur = i;
						break;
					}
			}
			inline void SetCurrent(size_t id)
			{
				m_cur = id;
			}
			inline void SetCurrent(std::string obj, std::string name)
			{
				// find if we already used SetCurrent on this method
				for (size_t i = 0; i < m_initializedMethods.size(); i++) {
					if (m_initializedMethods[i].first == obj &&
						m_initializedMethods[i].second == name) {

						for (size_t j = 0; j < m_funcs.size(); j++) {
							if (m_funcs[j].Name == name && m_funcs[j].Object == obj) {
								m_cur = j;
								break;
							}
						}

						return;
					}
				}


				// we reach this point only if we never used SetCurrent on this method
				size_t id = m_code.size();

				FunctionData data = m_gen->GetMethod(obj, name);
				m_initializedMethods.push_back(std::make_pair(obj, name));

				m_code.push_back(ByteCode());
				m_funcs.push_back(data);
				m_linkAddr.resize(m_linkAddr.size() + 1);
				m_locals.push_back(0);

				m_code[id].Add(OpCode::FunctionStart);
				m_code[id].Add(data.Arguments);

				m_lengthAddr.push_back(m_code[id].Count());

				m_code[id].Add(BitConverter::GetUInt32(0u));

				m_cur = id;
			}

			inline ByteCode Get(size_t i)
			{
				m_code[i].Write(m_lengthAddr[i],
					BitConverter::GetUInt32(
						m_code[i].Count() - (m_lengthAddr[i] + sizeof(uint32_t))
					)
				);

				for (size_t j = 0; j < m_linkAddr[i].size(); j++)
					m_code[i].Write(m_linkAddr[i][j].first,
						BitConverter::GetUInt32(
							m_linkAddr[i][j].second - (m_lengthAddr[i] + sizeof(uint32_t))
						)
					);

				return m_code[i];
			}
			inline ByteCode Get(const std::string& name, const std::string& obj)
			{
				size_t i = 0;
				for (; i < m_funcs.size(); i++)
					if (m_funcs[i].Name == name && m_funcs[i].Object == obj) break;

				return Get(i);
			}
			inline std::vector<ag::FunctionData>& GetData() { return m_funcs; }

			// OpCodes
			inline void Return()
			{
				m_code[m_cur].Add(OpCode::Return);
			}
			inline void PushStack(agvariant v)
			{
				m_code[m_cur].Add(OpCode::PushStack);
				m_code[m_cur].Add(VariantToType(v));

				if (VariantToType(v) == ag::Type::String)
					m_code[m_cur].Add(BitConverter::GetInt32(m_gen->AddString(std::get<std::string>(v)))); // only push string id (string table)
				else
					m_code[m_cur].Add(BitConverter::Get(v));
			}
			inline void PopStack()
			{
				m_code[m_cur].Add(OpCode::PopStack);
			}
			inline void GetConstant(uint16_t const_id)
			{
				m_code[m_cur].Add(OpCode::GetConstant);
				m_code[m_cur].Add(BitConverter::GetUInt16(const_id));
			}
			inline void Add()
			{
				m_code[m_cur].Add(OpCode::Add);
			}
			inline void Subtract()
			{
				m_code[m_cur].Add(OpCode::Subtract);
			}
			inline void Multiply()
			{
				m_code[m_cur].Add(OpCode::Multiply);
			}
			inline void Divide()
			{
				m_code[m_cur].Add(OpCode::Divide);
			}
			inline void Increment()
			{
				m_code[m_cur].Add(OpCode::Increment);
			}
			inline void Decrement()
			{
				m_code[m_cur].Add(OpCode::Decrement);
			}
			inline void Negate()
			{
				m_code[m_cur].Add(OpCode::Negate);
			}
			inline void Modulo()
			{
				m_code[m_cur].Add(OpCode::Modulo);
			}
			inline void BitOr()
			{
				m_code[m_cur].Add(OpCode::BitOr);
			}
			inline void BitAnd()
			{
				m_code[m_cur].Add(OpCode::BitAnd);
			}
			inline void BitNot()
			{
				m_code[m_cur].Add(OpCode::BitNot);
			}
			inline void BitXor()
			{
				m_code[m_cur].Add(OpCode::BitXor);
			}
			inline void BitLeftShift()
			{
				m_code[m_cur].Add(OpCode::BitLeftShift);
			}
			inline void BitRightShift()
			{
				m_code[m_cur].Add(OpCode::BitRightShift);
			}
			inline void BoolOr()
			{
				m_code[m_cur].Add(OpCode::BoolOr);
			}
			inline void BoolAnd()
			{
				m_code[m_cur].Add(OpCode::BoolAnd);
			}
			inline void Equal()
			{
				m_code[m_cur].Add(OpCode::Equal);
			}
			inline void NotEqual()
			{
				m_code[m_cur].Add(OpCode::NotEqual);
			}
			inline void Not()
			{
				m_code[m_cur].Add(OpCode::Not);
			}
			inline void Greater()
			{
				m_code[m_cur].Add(OpCode::Greater);
			}
			inline void Less()
			{
				m_code[m_cur].Add(OpCode::Less);
			}
			inline void GreaterEqual()
			{
				m_code[m_cur].Add(OpCode::GreaterEqual);
			}
			inline void LessEqual()
			{
				m_code[m_cur].Add(OpCode::LessEqual);
			}
			inline void Nop()
			{
				m_code[m_cur].Add(OpCode::Nop);
			}
			inline void Convert(ag::Type type)
			{
				m_code[m_cur].Add(OpCode::Convert);
				m_code[m_cur].Add(type);
			}
			inline void Duplicate()
			{
				m_code[m_cur].Add(OpCode::Duplicate);
			}
			inline void Swap()
			{
				m_code[m_cur].Add(OpCode::Swap);
			}
			inline void GetArgument(uint16_t arg_id)
			{
				m_code[m_cur].Add(OpCode::GetLocal);
				m_code[m_cur].Add(BitConverter::GetUInt16(arg_id));
			}
			inline void SetArgument(uint16_t arg_id)
			{
				m_code[m_cur].Add(OpCode::SetLocal);
				m_code[m_cur].Add(BitConverter::GetUInt16(arg_id));
			}
			inline void GetLocal(uint16_t loc_id)
			{
				m_code[m_cur].Add(OpCode::GetLocal);
				m_code[m_cur].Add(BitConverter::GetUInt16(loc_id + m_funcs[m_cur].Arguments));
			}
			inline void SetLocal(uint16_t loc_id)
			{
				m_locals[m_cur] = std::max(m_locals[m_cur], loc_id);

				m_code[m_cur].Add(OpCode::SetLocal);
				m_code[m_cur].Add(BitConverter::GetUInt16(loc_id + m_funcs[m_cur].Arguments));
			}
			inline void GetGlobal(uint16_t glob_id)
			{
				m_code[m_cur].Add(OpCode::GetGlobal);
				m_code[m_cur].Add(BitConverter::GetUInt16(glob_id));
			}
			inline void SetGlobal(uint16_t glob_id)
			{
				m_code[m_cur].Add(OpCode::SetGlobal);
				m_code[m_cur].Add(BitConverter::GetUInt16(glob_id));
			}
			inline void NewArray(uint8_t dim)
			{
				m_code[m_cur].Add(OpCode::NewArray);
				m_code[m_cur].Add(dim);
			}
			inline void SetArrayElement()
			{
				m_code[m_cur].Add(OpCode::SetArrayElement);
			}
			inline void GetArrayElement()
			{
				m_code[m_cur].Add(OpCode::GetArrayElement);
			}
			inline void Call(std::string name, uint8_t argc)
			{
				m_code[m_cur].Add(OpCode::Call);
				m_code[m_cur].Add(BitConverter::GetInt32(m_gen->AddString(name)));
				m_code[m_cur].Add(argc);
			}
			inline void CallReturn(std::string name, uint8_t argc)
			{
				m_code[m_cur].Add(OpCode::CallReturn);
				m_code[m_cur].Add(BitConverter::GetInt32(m_gen->AddString(name)));
				m_code[m_cur].Add(argc);
			}
			inline void IsTypeOf(ag::Type type)
			{
				m_code[m_cur].Add(OpCode::IsTypeOf);
				m_code[m_cur].Add(type);
			}
			inline void NewObject(uint16_t object_id, uint8_t argc = 0)
			{
				m_code[m_cur].Add(OpCode::NewObject);
				m_code[m_cur].Add(BitConverter::GetUInt16(object_id + 1));
				m_code[m_cur].Add(argc);
			}
			inline void PushNull()
			{
				m_code[m_cur].Add(OpCode::NewObject);
				m_code[m_cur].Add(BitConverter::GetUInt16(0));
				m_code[m_cur].Add(0);
			}
			inline void GetProperty(uint16_t loc_id, std::string prop)
			{
				GetLocal(loc_id);
				m_code[m_cur].Add(OpCode::GetProperty);
				m_code[m_cur].Add(BitConverter::GetInt32(m_gen->AddString(prop)));
			}
			inline void SetProperty(uint16_t loc_id, std::string prop)
			{
				GetLocalPointer(loc_id);
				m_code[m_cur].Add(OpCode::SetProperty);
				m_code[m_cur].Add(BitConverter::GetInt32(m_gen->AddString(prop)));
			}
			inline void GetProperty(std::string prop)
			{
				m_code[m_cur].Add(OpCode::GetProperty);
				m_code[m_cur].Add(BitConverter::GetInt32(m_gen->AddString(prop)));
			}
			inline void SetProperty(std::string prop)
			{
				m_code[m_cur].Add(OpCode::SetProperty);
				m_code[m_cur].Add(BitConverter::GetInt32(m_gen->AddString(prop)));
			}
			inline void GetMyProperty(std::string prop)
			{
				m_code[m_cur].Add(OpCode::GetMyProperty);
				m_code[m_cur].Add(BitConverter::GetInt32(m_gen->AddString(prop)));
			}
			inline void SetMyProperty(std::string prop)
			{
				m_code[m_cur].Add(OpCode::SetMyProperty);
				m_code[m_cur].Add(BitConverter::GetInt32(m_gen->AddString(prop)));
			}
			inline void CallMethod(std::string mtd, uint8_t argc)
			{
				m_code[m_cur].Add(OpCode::CallMethod);
				m_code[m_cur].Add(BitConverter::GetInt32(m_gen->AddString(mtd)));
				m_code[m_cur].Add(argc);
			}
			inline void CallMyMethod(std::string mtd, uint8_t argc)
			{
				m_code[m_cur].Add(OpCode::CallMyMethod);
				m_code[m_cur].Add(BitConverter::GetInt32(m_gen->AddString(mtd)));
				m_code[m_cur].Add(argc);
			}
			inline void CallReturnMethod(std::string mtd, uint8_t argc)
			{
				m_code[m_cur].Add(OpCode::CallReturnMethod);
				m_code[m_cur].Add(BitConverter::GetInt32(m_gen->AddString(mtd)));
				m_code[m_cur].Add(argc);
			}
			inline void CallMyReturnMethod(std::string mtd, uint8_t argc)
			{
				m_code[m_cur].Add(OpCode::CallMyReturnMethod);
				m_code[m_cur].Add(BitConverter::GetInt32(m_gen->AddString(mtd)));
				m_code[m_cur].Add(argc);
			}
			inline size_t If()
			{
				m_code[m_cur].Add(OpCode::If);

				size_t cur_if = m_linkAddr[m_cur].size();
				m_linkAddr[m_cur].resize(cur_if + 1);
				m_linkAddr[m_cur][cur_if] = std::make_pair(m_code[m_cur].Count(), 0);

				m_code[m_cur].Add(BitConverter::GetUInt32(0u));

				return cur_if;
			}
			inline size_t Goto()
			{
				m_code[m_cur].Add(OpCode::Goto);

				size_t cur_link = m_linkAddr[m_cur].size();
				m_linkAddr[m_cur].resize(cur_link + 1);
				m_linkAddr[m_cur][cur_link] = std::make_pair(m_code[m_cur].Count(), 0);

				m_code[m_cur].Add(BitConverter::GetUInt32(0u));

				return cur_link;
			}
			inline void ScopeStart()
			{
				m_code[m_cur].Add(OpCode::ScopeStart);
			}
			inline void ScopeEnd()
			{
				m_code[m_cur].Add(OpCode::ScopeEnd);
			}
			inline void Assign()
			{
				m_code[m_cur].Add(OpCode::Assign);
			}
			inline void GetArgumentPointer(uint16_t loc_id)
			{
				m_code[m_cur].Add(OpCode::GetLocalPointer);
				m_code[m_cur].Add(BitConverter::GetUInt16(loc_id));
			}
			inline void GetLocalPointer(uint16_t loc_id)
			{
				m_code[m_cur].Add(OpCode::GetLocalPointer);
				m_code[m_cur].Add(BitConverter::GetUInt16(loc_id + m_funcs[m_cur].Arguments));
			}
			inline void GetGlobalPointer(uint16_t loc_id)
			{
				m_code[m_cur].Add(OpCode::GetGlobalPointer);
				m_code[m_cur].Add(BitConverter::GetUInt16(loc_id));
			}
			inline void GetPropertyPointer(std::string prop)
			{
				m_code[m_cur].Add(OpCode::GetPropertyPointer);
				m_code[m_cur].Add(BitConverter::GetInt32(m_gen->AddString(prop)));
			}
			inline void GetMyPropertyPointer(std::string prop)
			{
				m_code[m_cur].Add(OpCode::GetMyPropertyPointer);
				m_code[m_cur].Add(BitConverter::GetInt32(m_gen->AddString(prop)));
			}
			inline void GetGlobalByName(std::string prop)
			{
				m_code[m_cur].Add(OpCode::GetGlobalByName);
				m_code[m_cur].Add(BitConverter::GetInt32(m_gen->AddString(prop)));
			}
			inline void GetGlobalPointerByName(std::string prop)
			{
				m_code[m_cur].Add(OpCode::GetGlobalPointerByName);
				m_code[m_cur].Add(BitConverter::GetInt32(m_gen->AddString(prop)));
			}
			inline void SetGlobalByName(std::string prop)
			{
				m_code[m_cur].Add(OpCode::SetGlobalByName);
				m_code[m_cur].Add(BitConverter::GetInt32(m_gen->AddString(prop)));
			}
			inline void EmptyStack()
			{
				m_code[m_cur].Add(OpCode::EmptyStack);
			}
			inline void DebugLineNumber(uint32_t line)
			{
				m_code[m_cur].Add(OpCode::DebugLineNumber);
				m_code[m_cur].Add(BitConverter::GetUInt32(line));
			}
			inline void DebugFile(std::string filename)
			{
				m_code[m_cur].Add(OpCode::DebugFile);
				m_code[m_cur].Add(BitConverter::GetInt32(m_gen->AddString(filename)));
			}
			inline void Breakpoint()
			{
				m_code[m_cur].Add(OpCode::Breakpoint);
			}
			inline void NewObjectByName(std::string name, uint8_t argc = 0)
			{
				m_code[m_cur].Add(OpCode::NewObjectByName);
				m_code[m_cur].Add(BitConverter::GetInt32(m_gen->AddString(name)));
				m_code[m_cur].Add(argc);
			}
			inline void PushStackFunction(std::string name)
			{
				m_code[m_cur].Add(OpCode::PushStackFunction);
				m_code[m_cur].Add(BitConverter::GetInt32(m_gen->AddString(name)));
			}
			inline void CallStack(uint8_t argc)
			{
				m_code[m_cur].Add(OpCode::CallStack);
				m_code[m_cur].Add(argc);
			}

			inline void SetAddress(size_t id, size_t addr)
			{
				m_linkAddr[m_cur][id].second = addr;
			}
			inline size_t GetCurrentAddress()
			{
				return m_code[m_cur].Count();
			}
			inline uint16_t GetNextLocal()
			{
				return m_locals[m_cur] + 1 + m_funcs[m_cur].Arguments;
			}

		private:
			Generator * m_gen;
			size_t m_cur;
			std::vector<uint16_t> m_locals;
			std::vector<size_t> m_lengthAddr;
			std::vector<std::vector<std::pair<size_t, size_t>>> m_linkAddr;
			std::vector<ByteCode> m_code;
			std::vector<FunctionData> m_funcs;
			std::vector<std::pair<std::string, std::string>> m_initializedMethods;
		} Function;

		inline ByteCode Get()
		{
			ByteCode bret;


			// header
			bret.Add(BitConverter::GetString(std::string("BVM"), false));


			// version
			bret.Add(m_ver_major);
			bret.Add(BitConverter::GetUInt16(m_ver_minor));


			// string table
			bret.Add(BitConverter::GetUInt32(m_strtbl.size()));
			for (size_t i = 0; i < m_strtbl.size(); i++)
				bret.Add(BitConverter::GetString(m_strtbl[i]));

			// globals
			bret.Add(BitConverter::GetUInt16(m_globals.size()));
			for (size_t i = 0; i < m_globals.size(); i++)
				bret.Add(BitConverter::GetString(m_globals[i]));	// name


															// function names
			std::vector<FunctionData> funcs = Function.GetData();
			std::vector<size_t> funcStartAddr;

			uint16_t funcCount = 0;
			for (size_t i = 0; i < funcs.size(); i++) {
				if (funcs[i].Object != "")
					continue;
				funcCount++;
			}
			bret.Add(BitConverter::GetUInt16(funcCount));

			for (size_t i = 0; i < funcs.size(); i++) {
				if (funcs[i].Object != "")
					continue;

				bret.Add(BitConverter::GetString(funcs[i].Name));
				funcStartAddr.push_back(bret.Count());
				bret.Add(BitConverter::GetUInt32(0u));
			}


			// constants
			bret.Add(m_consts.size());
			for (size_t t = 0; t < variant_type_count; t++) {
				bool written = false;
				for (size_t i = 0; i < m_consts.size(); i++) {
					if (m_consts[i].index() == t) {
						if (!written) {
							bret.Add((uint8_t)VariantToType(m_consts[i]));
							written = true;
						}
						bret.Add(BitConverter::Get(m_consts[i]));
					}
				}
			}


			// objects
			std::vector<size_t> methodStartAddr;
			bret.Add(BitConverter::GetUInt16(m_objNames.size()));
			for (size_t i = 0; i < m_objNames.size(); i++) {
				bret.Add(BitConverter::GetString(m_objNames[i]));

				bret.Add(BitConverter::GetUInt16(m_objProps[i].size()));
				for (size_t j = 0; j < m_objProps[i].size(); j++)
					bret.Add(BitConverter::GetString(m_objProps[i][j]));

				bret.Add(BitConverter::GetUInt16(m_objMethods[i].size()));
				for (size_t j = 0; j < m_objMethods[i].size(); j++) {
					bret.Add(BitConverter::GetString(m_objMethods[i][j].Name));
					methodStartAddr.push_back(bret.Count());
					bret.Add(BitConverter::GetUInt32(0u));
				}
			}


			// function code
			size_t curFunc = 0;
			for (size_t i = 0; i < funcs.size(); i++) {
				if (funcs[i].Object != "")
					continue;

				bret.Write(funcStartAddr[curFunc], BitConverter::GetUInt32(bret.Count()));
				bret.Add(Function.Get(i));
				curFunc++;
			}

			// method code
			size_t curMethod = 0;
			for (size_t i = 0; i < m_objNames.size(); i++) {
				for (size_t j = 0; j < m_objMethods[i].size(); j++) {
					bret.Write(methodStartAddr[curMethod], BitConverter::GetUInt32(bret.Count()));
					bret.Add(Function.Get(m_objMethods[i][j].Name, m_objMethods[i][j].Object));
					// TODO: method overloading
					curMethod++;
				}
			}

			return bret;
		}

	private:
		uint8_t m_ver_major;
		uint16_t m_ver_minor;
		std::vector<std::string> m_strtbl;
		std::vector<std::string> m_globals;
		std::vector<std::string> m_objNames;
		std::vector<std::vector<std::string>> m_objProps;
		std::vector<std::vector<FunctionData>> m_objMethods;
		std::vector<agvariant> m_consts;
	};
}

#endif //__AGEN_HPP__