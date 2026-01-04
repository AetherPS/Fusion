#pragma once

class ThreadPool
{
public:
	static void Init(int poolSize);
	static void Term();
	static void QueueJob(const std::function<void()>& job);

private:
	static void WorkingLoop();

	static inline bool ShouldRun;
	static inline std::mutex JobQueueMtx;
	static inline std::condition_variable MtxCondition;
	static inline std::vector<OrbisPthread> ThreadsPool;
	static inline std::queue<std::function<void()>> JobQueue;
};