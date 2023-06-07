#pragma once
#include "EM_EntityInterface.h"	//Has all the includes needed for operation

/*
	Templatable, semi-generic pool class (See: https://gameprogrammingpatterns.com/object-pool.html).
	For this game, assumed to use allocated memory (due to polymorphic classes with virtuals).
	Do NOT used with un-allocated types.
*/
template <class ENTITY_TYPE>
class EntityPool
{
public:

	////////////////////
	/// Constructors ///
	////////////////////

	EntityPool() { FlagAllObjects(false); }
	~EntityPool()
	{
		for (auto& a : m_Pool)
		{
			delete a;
			a = nullptr;
		}
	}

	//////////////////
	/// Operations ///
	//////////////////

	//Update Pool (only if object in use)
	void UpdatePool(GameTimer& gt)
	{
		//Reset info counters
		m_InUseCount = 0;
		m_HighestInUseIndex = 0;
		for (unsigned i(0); i < POOL_SIZE; ++i) {
			if (m_InUse[i])
			{
				//Set counter data then update
				++m_InUseCount;
				m_HighestInUseIndex = i;
				m_Pool[i]->Update(gt);
			}
		
		}
	}
	//Render Pool (only if object in use)
	void RenderPool(DirectX::SpriteBatch& batch, DirectX::DescriptorHeap* heap)
	{
		for (unsigned i(0); i < POOL_SIZE; ++i)
			if (m_InUse[i])
				m_Pool[i]->Render(batch, heap);
	}
	//Free Target Object via Object
	bool FreeObject(ENTITY_TYPE& obj)
	{
		assert(obj);
		//Cycle array, looking for object match and freeing the object for use
		for (unsigned i(0); i < POOL_SIZE; ++i)
			if (obj == m_Pool[i])
			{
				m_InUse[i] = false;
				//Object successfully released, return true
				return true;
			}

		//Object not found, may not be in this pool
		return false;
	}
	//Flag all pool entities one flag or another
	void FlagAllObjects(bool flag) { std::fill_n(m_InUse, POOL_SIZE, false); }


	///////////
	/// Get ///
	///////////

	//Index Access for Arrays
	ENTITY_TYPE& GetObjectAtIndex(unsigned index) { assert(index < POOL_SIZE); return m_Pool[index]; }
	bool GetUseFlagAtIndex(unsigned index) { assert(index < POOL_SIZE); return m_InUse[index]; }

	unsigned GetPoolSize() { return POOL_SIZE; }
	int* GetArrayStart() { return m_Pool; }

	unsigned GetInUseCount() { return m_InUseCount; }
	unsigned GetHighestInUseIndex() { return m_HighestInUseIndex; }

	//Finds a free object in the pool, automatically marking it in use if found
	ENTITY_TYPE FindFreeObject()
	{
		//Cycle array, returning and marking first free object as in use
		for (unsigned i(0); i < POOL_SIZE; ++i)
			if (!m_InUse[i])
			{
				m_InUse[i] = true;
				return m_Pool[i];
			}

		//No object found, return nullptr
		return nullptr;
	}

	//Finds a free object matching the ID value provided.
	//Class must have and return a value via "GetID()" call.
	//Flags object automatically in use
	ENTITY_TYPE FindFreeObjectByInternalTypeID(unsigned id)
	{
		//Cycle array, looking for ID match
		for (unsigned i(0); i < POOL_SIZE; ++i)
			if ((!m_InUse[i]) && (id == m_Pool[i]->GetID()))
			{
				m_InUse[i] = true;
				return m_Pool[i];
			}

		//No object found, return nullptr
		return nullptr;
	}

	///////////
	/// Set ///
	///////////

	//Set Object Status at Index (Use carefully and only if accociated object is correctly configured)
	bool SetObjectStatus(unsigned short id, bool status)
	{
		assert(id < POOL_SIZE);
		m_InUse[id] = status;

		if (!status)
			--m_InUseCount;

		return true;
	}

private:

	////////////
	/// Data ///
	////////////

	static const unsigned POOL_SIZE = 1024;

	ENTITY_TYPE m_Pool[POOL_SIZE];
	bool m_InUse[POOL_SIZE];

	unsigned m_InUseCount = 0;
	unsigned m_HighestInUseIndex = 0;
};