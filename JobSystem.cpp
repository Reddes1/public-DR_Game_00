#include "JobSystem.h"

#include <assert.h>

JobSystem::JobSystem()
	:m_ThreadCount(DEFAULT_THREAD_RESERVE_COUNT)
{
	m_Threads.insert(m_Threads.end(), DEFAULT_THREAD_RESERVE_COUNT, HANDLE());
	m_ThreadIDs.insert(m_ThreadIDs.end(), DEFAULT_THREAD_RESERVE_COUNT, unsigned());

	Init();
}

JobSystem::JobSystem(unsigned int threadCount)
{
	m_Threads.insert(m_Threads.end(), threadCount, HANDLE());
	m_ThreadIDs.insert(m_ThreadIDs.end(), threadCount, unsigned());

	Init();
}

JobSystem::~JobSystem()
{
	//Flag for shutdown
	m_ShutDown = true;
	//Finish up current threads
	ReleaseSemaphore(m_Semaphore, m_ThreadCount, nullptr);

	//Wait for current threads to finish
	for (unsigned i(0); i < m_ThreadCount; ++i)
	{
		//Join/Wait till thread done
		WaitForSingleObject(m_Threads[i], INFINITE);
		CloseHandle(m_Threads[i]);
	}

	//Clean up thread and semephore memory
	CloseHandle(m_Semaphore);
	for (auto& a : m_Threads)
		delete a;

}

void JobSystem::AddNewJob(JobInterface* j)
{
	//Secure data and add new job to queue
	AcquireSRWLockExclusive(&m_Mutex);
	m_Jobs.push(j);
	//Release lock and add job to semaphore
	ReleaseSRWLockExclusive(&m_Mutex);
	ReleaseSemaphore(m_Semaphore, 1, nullptr);
}

void JobSystem::Init()
{
	//Setup semaphore for use, and mutex
	m_Semaphore = CreateSemaphoreA(nullptr, 0, LONG_MAX, "Jobs Queue");
	InitializeSRWLock(&m_Mutex);

	for (unsigned i(0); i < m_ThreadCount; ++i)
		m_Threads[i] = CreateThread(nullptr, 0, WorkerThread, this, 0, m_ThreadIDs[i]);

}

DWORD JobSystem::WorkerThread(void* userData)
{
	JobSystem* system = static_cast<JobSystem*>(userData);

	system->RunWorkerThread();

	return 0;
}

void JobSystem::RunWorkerThread()
{
	for (;;)
	{
		//Set thread to sleep till job is passed to it (semaphore != 0)
		WaitForSingleObject(m_Semaphore, INFINITE);
	
		//Job found, so get ready to grab hold of it
		JobInterface* job = nullptr;

		//Work to do now so lock the job queue briefly
		AcquireSRWLockExclusive(&m_Mutex);
		if (!m_Jobs.empty())
		{
			//Grab the job from front of the queue and pop it off
			job = m_Jobs.front();
			m_Jobs.pop();
		}
		ReleaseSRWLockExclusive(&m_Mutex);
		
		//Confirm job found, and run it
		if (job)
			job->DoJob();
		//If no jobs, and signalled to shut down, break out of infinite loop
		else if (m_ShutDown)
			break;
		else
			//Something broke if it hits here
			assert(false);
	}
}
