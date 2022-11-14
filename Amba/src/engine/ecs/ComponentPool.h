#pragma once

// definitions
#define MAX_ENTITIES 1025
#define INVALID_ENTITY CreateEntityId(EntityIndex(-1), 0)

extern int s_CompotentCounter;

namespace Amba {
	
	template<class t>
	int GetComponentId()
	{
		static int s_ComponentId = s_CompotentCounter++;
		return s_ComponentId;
	}

	class ComponentPool
	{
	public:

		ComponentPool(size_t elementSize)
		{
			m_ElementSize = elementSize;
			m_pData = new char[m_ElementSize * MAX_ENTITIES];
		}

		~ComponentPool()
		{
			delete[] m_pData;
		}

		inline void* get(size_t index) { return m_pData + index * m_ElementSize; }

		char* m_pData;
		size_t m_ElementSize;

	private:

	};

}