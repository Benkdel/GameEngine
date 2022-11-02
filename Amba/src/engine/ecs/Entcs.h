#pragma once

 
#include "components.h"

// definitions
#define MAX_ENTITIES 1025
#define INVALID_ENTITY CreateEntityId(EntityIndex(-1), 0)

// typedefs to increase readability
typedef unsigned long long EntityId;
typedef unsigned int EntityIndex;
typedef unsigned int EntityVersion;

extern int s_CompotentCounter;

namespace Amba {

	template<class t>
	int GetComponentId()
	{
		static int s_ComponentId = s_CompotentCounter++;
		return s_ComponentId;
	}

	EntityId CreateEntityId(EntityIndex index, EntityVersion version);
	EntityIndex GetEntityIndex(EntityId id);
	EntityVersion GetEntityVersion(EntityId id);
	bool IsEntityValid(EntityId id);


	class ComponentPool
	{
	public:
	
		ComponentPool(size_t elementSize);
		~ComponentPool();
	
		inline void* get(size_t index) { return m_pData + index * m_ElementSize; }

		char* m_pData;
		size_t m_ElementSize;

	private:

	};
}

