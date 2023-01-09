#pragma once

#include <thread>
#include <atomic>
#include <condition_variable>
#include "TaskQueue.hpp"

namespace Tasks {

class TaskManager;

/**
 * @brief class for representing a task executor.
 */

class TaskExecutor
{
public:
	using SP = std::shared_ptr<TaskExecutor>;

public:
						TaskExecutor	(void) noexcept = delete;
						TaskExecutor	(const TaskExecutor&) noexcept = delete;
						TaskExecutor	(TaskExecutor&&) noexcept = delete;
	/**
	 * @brief TaskExecutor - Constructor
	 * @param aId - ID
	 * @param aTaskQueue - shared pointer to a task queue
	 * @param aTaskManager - a reference to a task manager
	 */
	inline				TaskExecutor	(const size_t	aId,
										 TaskQueue::SP	aTaskQueue,
										 TaskManager&	aTaskManager) noexcept;
						~TaskExecutor	(void) noexcept = default;

	/**
	 * @brief Id - returns the ID of the task executor.
	 * @return ID of the task executor
	 */
	size_t				Id				(void) const noexcept {return iId;}

	/**
	 * @brief Start - starts the task executor's internal thread.
	 */
	void				Start			(void);

	/**
	 * @brief RequestStop - sets a flag to request that the task executor stop.
	 */
	void				RequestStop		(void);

	/**
	 * @brief Join - waits for the task executor's internal thread to finish.
	 */
	void				Join			(void);

	/**
	 * @brief Wakeup - wakes up the task executor's internal thread if it is waiting on a condition variable.
	 */
	void				Wakeup			(void);

private:
	/**
	 * @brief ExecNext - execute next task from queue (if there are)
	 * @return true - if there are task was executed, false - otherway
	 */
	bool				ExecNext		(void);

private:
	const size_t			iId;					//ID of the task executor
	TaskQueue::SP			iTaskQueue;				//Shared pointer to the task queue
	TaskManager&			iTaskManager;			//Reference to the task manager

	std::thread				iThread;				//Internal thread
	std::atomic_bool		iStopRequested = false;	//A flag indicating whether a stop has been requested

	mutable std::mutex		iMutex;					//Mutex used in condition variable
	bool					iIsIdle = true;			//Flag used in condition variable
	std::condition_variable	iCV;					//Condition variable used to Wakeup sleeping thread.
};

TaskExecutor::TaskExecutor
(
	const size_t	aId,
	TaskQueue::SP	aTaskQueue,
	TaskManager&	aTaskManager
) noexcept:
iId(aId),
iTaskQueue(std::move(aTaskQueue)),
iTaskManager(aTaskManager)
{
}

}//namespace Tasks
