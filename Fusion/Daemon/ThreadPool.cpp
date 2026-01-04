#include "stdafx.h"
#include "ThreadPool.h"

void ThreadPool::WorkingLoop()
{
	while (true)
	{
		try
		{
			std::function<void()> job;
			{
				// Create a unique_lock on the JobQueueMtx mutex. This lock ensures exclusive access to the mutex.
				std::unique_lock<std::mutex> lock(JobQueueMtx);

				// Wait until the condition in MtxCondition is satisfied.
				MtxCondition.wait(lock, []
				{
					// The lambda function returns true if either of the conditions is met:
					// 1. JobQueue is not empty (there are jobs in the queue).
					// 2. ShouldRun is false (indicating that the processing should stop).
					return !JobQueue.empty() || !ShouldRun;
				});

				// If ShouldRun is false, exit this block early (stop processing).
				if (!ShouldRun)
					return;

				// Assign the job at the front of the JobQueue to the 'job' std::function.
				job = JobQueue.front();

				// Remove the job from the front of the JobQueue.
				JobQueue.pop();
			}

			// Call the 'job' function object, which executes the job that was dequeued from JobQueue.
			job();
		}
		catch (const std::exception& ex)
		{
			klog("Std Error: %s", ex.what());
		}
		catch (...)
		{
			klog("Other Unknown Error Occured in Worker Thread.");
		}
	}
}

void ThreadPool::Init(int poolSize)
{
	ShouldRun = true;				// Set the run flag on.
	ThreadsPool.resize(poolSize);	// Specify the thread pool size.

	// Init all the thread pools on any cpu.
	for (int i = 0; i < poolSize; i++)
	{
		char threadName[0x200];
		snprintf(threadName, sizeof(threadName), "WorkerThread%i", i);
		scePthreadCreate(&ThreadsPool.at(i), nullptr, [](void*) -> void*
		{
			ThreadPool::WorkingLoop();

			// Clean up the thread.
			scePthreadExit(nullptr);
			return nullptr;
		}, nullptr, threadName);

		scePthreadSetaffinity(ThreadsPool.at(i), 0x3F); // TODO: PR on oosdk for scePthreadSetaffinity & ORBIS_KERNEL_CPUMASK_6CPU_ALL
	}
}

void ThreadPool::Term()
{
	// Lock the JobQueue then set the shut down flag.
	{
		std::unique_lock<std::mutex> lock(JobQueueMtx);
		ShouldRun = false;
	}

	// Notify all threads to shutdown.
	MtxCondition.notify_all();

	// Wait for all the threads to shutdown.
	for (auto& activeThread : ThreadsPool)
	{
		scePthreadJoin(activeThread, nullptr);
	}

	// Clear out.
	ThreadsPool.clear();
}

void ThreadPool::QueueJob(const std::function<void()>& job)
{
	// Lock the JobQueue then push the new job.
	{
		std::unique_lock<std::mutex> lock(JobQueueMtx);
		JobQueue.push(job);
	}

	// Notify any free thread that there is work.
	MtxCondition.notify_one();
}