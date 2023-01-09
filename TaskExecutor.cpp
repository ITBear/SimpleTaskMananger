#include "TaskExecutor.hpp"

#include "TaskManager.hpp"

namespace Tasks {

void	TaskExecutor::Start (void)
{
	//Start internal thread with lambda
	iThread = std::thread
	(
		[&]()
		{
			//Repeat until stop requested
			while (iStopRequested.load(std::memory_order_acquire) == false)
			{
				//Execute next task and if there are no more tasks go to sleep (using CV)
				if (ExecNext() == false)
				{
					//Sleep
					std::unique_lock lock(iMutex);

					//Tell to task manager that current executor in IDLE mode
					iTaskManager._MarkAsIdle(*this);

					iIsIdle = true;
					iCV.wait_for
					(
						lock,
						std::chrono::milliseconds(500),
						[&](){return !iIsIdle || iStopRequested.load(std::memory_order_acquire);}
					);

					//Tell to task manager that current executor in BUSY mode
					iTaskManager._MarkAsBusy(*this);
				}
			}
		}
	);
}

void	TaskExecutor::RequestStop (void)
{
	iStopRequested.store(true, std::memory_order_release);
}

void	TaskExecutor::Join (void)
{
	if (iThread.joinable())
	{
		iThread.join();
	}
}

void	TaskExecutor::Wakeup (void)
{
	{
		std::scoped_lock lock(iMutex);
		iIsIdle = false;
	}

	iCV.notify_one();
}

bool	TaskExecutor::ExecNext (void)
{
	//Get next task from queue
	TaskQueue::NextTaskT nextTask = iTaskQueue->Next();

	//Check if there is a task
	if (nextTask.has_value() == false)
	{
		return false;
	}

	//Run task
	nextTask.value()->Run();

	return true;
}

}//namespace Tasks
