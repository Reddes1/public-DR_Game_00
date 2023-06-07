#include "CS_Survivor_00.h"

void CS_Survivor_00::ResetMapping()
{
	//Primary Mapping
	m_PrimaryMappedKeys[0] = (unsigned short)DefaultMappingPrimary::MOVE_UP;
	m_PrimaryMappedKeys[1] = (unsigned short)DefaultMappingPrimary::MOVE_DOWN;
	m_PrimaryMappedKeys[2] = (unsigned short)DefaultMappingPrimary::MOVE_LEFT;
	m_PrimaryMappedKeys[3] = (unsigned short)DefaultMappingPrimary::MOVE_RIGHT;
	m_PrimaryMappedKeys[4] = (unsigned short)DefaultMappingPrimary::ACTION_00;
	m_PrimaryMappedKeys[5] = (unsigned short)DefaultMappingPrimary::ACTION_01;
	m_PrimaryMappedKeys[6] = (unsigned short)DefaultMappingPrimary::BACK;

	//Secondary Mapping
	m_SecondaryMappedKeys[0] = (unsigned short)DefaultMappingSecondary::MOVE_UP;
	m_SecondaryMappedKeys[1] = (unsigned short)DefaultMappingSecondary::MOVE_DOWN;
	m_SecondaryMappedKeys[2] = (unsigned short)DefaultMappingSecondary::MOVE_LEFT;
	m_SecondaryMappedKeys[3] = (unsigned short)DefaultMappingSecondary::MOVE_RIGHT;
	m_SecondaryMappedKeys[4] = (unsigned short)DefaultMappingSecondary::ACTION_00;
	m_SecondaryMappedKeys[5] = (unsigned short)DefaultMappingSecondary::ACTION_01;
}

unsigned short CS_Survivor_00::GetPrimaryKeyAtIndex(MappedActions index)
{
	assert((unsigned)index < (unsigned)DMP::COUNT);
	return m_PrimaryMappedKeys[(unsigned)index];
}

unsigned short CS_Survivor_00::GetSecondaryKeyAtIndex(MappedActions index)
{
	assert((unsigned)index < (unsigned)DMS::COUNT);
	return m_SecondaryMappedKeys[(unsigned)index];
}
