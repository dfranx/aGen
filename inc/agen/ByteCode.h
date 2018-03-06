#ifndef __AGEN_BYTECODE_H__
#define __AGEN_BYTECODE_H__
#include <vector>
#include <algorithm>
#include <agen/Type.h>
#include <agen/OpCode.h>

namespace ag
{
	class ByteCode
	{
	public:
		inline void Set(size_t ind, uint8_t val) { m_bytes[ind] = val; }
		inline void Write(size_t ind, const std::vector<uint8_t>& bytes) {
			for (size_t i = 0; i < bytes.size(); i++)
				m_bytes[i+ind] = bytes[i];
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
}

#endif //__AGEN_BYTECODE_H__