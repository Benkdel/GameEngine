#include "Entcs.h"

int s_CompotentCounter = 0;

// entity id and versioning to avoid referrencing a deleted entity
// assumes we dont wrap around 32 bits by creating to many entities

namespace Amba {


	EntityId CreateEntityId(EntityIndex index, EntityVersion version)
	{
		// Shifts the index up 32, and puts the version in the bottom
		return ((EntityId)index << 32) | ((EntityId)version);
	}

	EntityIndex GetEntityIndex(EntityId id)
	{
		// Shifts down 32 so we lose the version part and get our index
		return id >> 32;
	}

	EntityVersion GetEntityVersion(EntityId id)
	{
		// Cast to a 32 bit int to get our version number (loosing the top 32 bits)
		return (EntityVersion)id;
	}

	bool IsEntityValid(EntityId id)
	{
		// check if the index is the invalid index
		return (id >> 32) != EntityIndex(-1);
	}

	ComponentPool::ComponentPool(size_t elementSize)
	{
		m_ElementSize = elementSize;
		m_pData = new char[m_ElementSize * MAX_ENTITIES];
	}

	ComponentPool::~ComponentPool()
	{
		delete[] m_pData;
	}
}

