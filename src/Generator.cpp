#include <agen/Generator.h>

namespace ag
{
	Generator::Generator() :
		m_ver_major(0),
		m_ver_minor(1)
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
		std::vector<std::string> funcNames = Function.GetNames();
		std::vector<size_t> funcStartAddr(funcNames.size());

		bret.Add(BitConverter::Get((uint16_t)funcNames.size()));
		for (size_t i = 0; i < funcNames.size(); i++) {
			bret.Add(BitConverter::Get(funcNames[i], true));
			funcStartAddr[i] = bret.Count();
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

		// function code
		for (size_t i = 0; i < funcNames.size(); i++) {
			bret.Write(funcStartAddr[i], BitConverter::Get(bret.Count()));
			bret.Add(Function.Get(funcNames[i]));
		}
		
		return bret;
	}
}