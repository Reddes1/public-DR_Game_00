#pragma once

#include "JobDefinitions.h"

#include "EM_EntityInterface.h"

/*
	Collection of EM object type jobs.
*/


/*
	Generic update job that calls EM_Object update function.
*/
class EM_Job_Update : public JobInterface
{
public:

	EM_Job_Update(EM_EntityInterface2D& entity, GameTimer& gameTime)
		:m_Entity(entity), m_GameTime(gameTime)
	{}
	~EM_Job_Update() {}

	void DoJob() override
	{
		m_Entity.Update(m_GameTime);
	}

private:
	EM_EntityInterface2D& m_Entity;
	GameTimer& m_GameTime;
};

/*
	Bulk update job. Divide or pass individual vector arrays into job.
*/
class EM_Job_BulkUpdate : public JobInterface
{
public:

	EM_Job_BulkUpdate(std::vector<EM_EntityInterface2D*>& entities, GameTimer& gameTime)
		:m_Entities(entities), m_GameTime(gameTime)
	{}
	~EM_Job_BulkUpdate() {}

	void DoJob() override
	{
		for (auto& a : m_Entities)
			a->Update(m_GameTime);
	}

private:
	std::vector<EM_EntityInterface2D*> m_Entities;
	GameTimer& m_GameTime;
};