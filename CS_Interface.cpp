#include "CS_Interface.h"
#include "RapidJSONLoaderUtils.h"

CS_Interface::CS_Interface()
{
	m_MappedActionKeys.reserve(16);
	m_ActionPressed.reserve(16);

	for (unsigned i(0); i < 16; ++i)
	{
		m_MappedActionKeys.push_back(0);
		m_ActionPressed.push_back(false);
	}
}

CS_Interface::CS_Interface(unsigned reserveCount)
{
	m_MappedActionKeys.reserve(reserveCount);
	m_ActionPressed.reserve(reserveCount);

	for (unsigned i(0); i < reserveCount; ++i)
	{
		m_MappedActionKeys.push_back(0);
		m_ActionPressed.push_back(false);
	}
}

void CS_Interface::MapKeyToIndex(unsigned short newKey, unsigned index)
{
	assert(index >= 0 && index <= m_MappedActionKeys.size());
	m_MappedActionKeys[index] = newKey;
}

bool CS_Interface::GetPressedStateAtIndex(unsigned index)
{
	assert(index >= 0 && index <= m_ActionPressed.size());
	return m_ActionPressed[index];
}

unsigned short CS_Interface::GetMappedKeyAtIndex(unsigned index)
{
	assert(index >= 0 && index <= m_ActionPressed.size());
	return m_MappedActionKeys[index];
}

unsigned CS_Interface::GetIndexOfMatchedKey(char key)
{
	for (unsigned i(0); i < m_MappedActionKeys.size(); ++i)
	{
		if (m_MappedActionKeys[i] == static_cast<unsigned>(key))
			return i;
	}
	//Return massive, unusable number meaning no index was found
	return 0xFFFFFFFF;
}

void CS_Interface::Release()
{
	m_MappedActionKeys.clear();
	m_ActionPressed.clear();
}
