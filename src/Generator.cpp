#include <agen/Generator.h>

namespace ag
{
	Generator::Generator() :
		m_ver_major(0),
		m_ver_minor(1),
		Function(*this)
	{
	}
	void Generator::SetHeader(uint8_t major, uint16_t minor)
	{
		m_ver_major = major;
		m_ver_minor = minor;
	}
	size_t Generator::AddGlobal(std::string name)
	{
		m_globals.push_back(name);
		return m_globals.size() - 1;
	}
	size_t Generator::AddConstant(variant var)
	{
		m_consts.push_back(var);

		int ret = 0;
		for (size_t i = 0; i < m_consts.size(); i++)
			if (m_consts[i].index() == var.index())
				ret++;
		return ret - 1;
	}
	size_t Generator::AddObject(std::string name)
	{
		size_t id = m_objNames.size();
		m_objNames.push_back(name);
		m_objProps.resize(id + 1);
		m_objMethods.resize(id + 1);
		return id;
	}
	void Generator::AddProperty(std::string obj, std::string prop)
	{
		for (size_t i = 0; i < m_objNames.size(); i++)
			if (m_objNames[i] == obj) {
				m_objProps[i].push_back(prop);
				break;
			}
	}
	void Generator::AddMethod(std::string obj, std::string name, ag::Type type, std::vector<ag::Type> args)
	{
		for (size_t i = 0; i < m_objNames.size(); i++)
			if (m_objNames[i] == obj) {
				m_objMethods[i].push_back(FunctionData(obj, name, type, args));
				break;
			}
	}
	FunctionData Generator::GetMethod(std::string obj, std::string name)
	{
		for (size_t i = 0; i < m_objNames.size(); i++)
			if (m_objNames[i] == obj)
				for (size_t j = 0; j < m_objMethods[i].size(); j++)
					if (m_objMethods[i][j].Name == name)
						return m_objMethods[i][j];

		return FunctionData();
	}
	ByteCode Generator::Get()
	{
		ByteCode bret;
	

		// header
		bret.Add(BitConverter::Get(std::string("BVM")));
		

		// version
		bret.Add(m_ver_major);
		bret.Add(BitConverter::Get(m_ver_minor));


		// globals
		bret.Add(BitConverter::Get((uint16_t)m_globals.size()));
		for (size_t i = 0; i < m_globals.size(); i++) {
			bret.Add(BitConverter::Get((uint16_t)i));	// id
			bret.Add(BitConverter::Get(m_globals[i], true));	// name
		}


		// function names
		std::vector<FunctionData> funcs = Function.GetData();
		std::vector<size_t> funcStartAddr;

		uint16_t funcCount = 0;
		for (size_t i = 0; i < funcs.size(); i++) {
			if (funcs[i].Object != "")
				continue;
			funcCount++;
		}
		bret.Add(BitConverter::Get(funcCount));

		for (size_t i = 0; i < funcs.size(); i++) {
			if (funcs[i].Object != "")
				continue;

			bret.Add(BitConverter::Get(funcs[i].Name, true));
			funcStartAddr.push_back(bret.Count());
			bret.Add(BitConverter::Get(0u));
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
		bret.Add(BitConverter::Get((uint16_t)m_objNames.size()));
		for (size_t i = 0; i < m_objNames.size(); i++) {
			bret.Add(BitConverter::Get(m_objNames[i], true));

			bret.Add(BitConverter::Get((uint16_t)m_objProps[i].size()));
			for (size_t j = 0; j < m_objProps[i].size(); j++) {
				bret.Add(BitConverter::Get((uint16_t)j));
				bret.Add(BitConverter::Get(m_objProps[i][j], true));
			}

			bret.Add(BitConverter::Get((uint16_t)m_objMethods[i].size()));
			for (size_t j = 0; j < m_objMethods[i].size(); j++) {
				bret.Add(BitConverter::Get(m_objMethods[i][j].Name, true));
				methodStartAddr.push_back(bret.Count());
				bret.Add(BitConverter::Get(0u)); // address [todo]
			}
		}


		// function code
		size_t curFunc = 0;
		for (size_t i = 0; i < funcs.size(); i++) {
			if (funcs[i].Object != "")
				continue;
			
			bret.Write(funcStartAddr[curFunc], BitConverter::Get(bret.Count()));
			bret.Add(Function.Get(funcs[i].Name));
			curFunc++;
		}

		// method code
		size_t curMethod = 0;
		for (size_t i = 0; i < m_objNames.size(); i++) {
			for (size_t j = 0; j < m_objMethods[i].size(); j++) {
				bret.Write(methodStartAddr[curMethod], BitConverter::Get(bret.Count()));
				bret.Add(Function.Get(m_objMethods[i][j].Name, m_objMethods[i][j].Object));
				curMethod++;
			}
		}

		return bret;
	}
}