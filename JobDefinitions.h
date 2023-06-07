#pragma once

#include "JobSystem.h"

#include <functional>

/*
	Define jobs to run in job system here
*/


class PointerJob : public JobInterface
{
public:

	void DoJob() override
	{

	}

private:
	std::function<void(int)> m_Function;

};