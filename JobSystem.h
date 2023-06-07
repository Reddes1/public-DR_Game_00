#pragma once

#include <Windows.h>
#include <queue>

#define DEFAULT_THREAD_RESERVE_COUNT 12

/*
	Interface class for creating jobs for "JobSystem_Type1"
*/
class JobInterface
{
public:

	JobInterface() {}
	
	//Define this, will be called by job system
	virtual void DoJob() = 0;
};


/*
	Thread pool system for packaging together 
*/
class JobSystem
{
public:

	////////////////////
	/// Constructors ///
	////////////////////

	JobSystem();
	JobSystem(unsigned int threadCount);
	~JobSystem();


	//////////////////
	/// Operations ///
	//////////////////

	//Adds new job to the queue
	void AddNewJob(JobInterface* j);


private:

	//////////////////
	/// Operations ///
	//////////////////

	void Init();
	//Window function call
	static DWORD WorkerThread(void* userData);
	//Runs worker thread
	void RunWorkerThread();

	////////////
	/// Data ///
	////////////

	//Job Storage
	std::queue<JobInterface*> m_Jobs;

	std::vector<HANDLE> m_Threads;
	std::vector<LPDWORD> m_ThreadIDs;

	//Available thread count
	unsigned int m_ThreadCount;
	//The Train Conductor
	HANDLE m_Semaphore;
	//Data Protector
	SRWLOCK m_Mutex;
	//Flag for shutdown
	bool m_ShutDown = false;

};