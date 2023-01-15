#pragma once

#include "TaskExecutor.hpp"
#include "TaskLambda.hpp"

#include <bitset>
#include <array>

namespace Tasks {

/**
 * @brief This is a class representing a task manager.
 */

class TaskManager
{
    friend class TaskExecutor;

public:
    using SP            = std::shared_ptr<TaskManager>;
    using IdleFlagsT    = std::bitset<128>;
    using ExecutorsT    = std::array<TaskExecutor::SP, 128>;

    /**
     * @brief The State enum
     */
    enum class State
    {
        NOT_STARTED,
        STARTED,
        STOP_REQUESTED,
        STOPPED
    };

public:
                            TaskManager     (void) noexcept = delete;
                            TaskManager     (const TaskManager&) noexcept = delete;
                            TaskManager     (TaskManager&&) noexcept = delete;
    /**
     * @brief TaskManager - constructor
     * @param aExecutorsCount - number of executors.
     */
    inline                  TaskManager     (const size_t aExecutorsCount) noexcept;
                            ~TaskManager    (void) noexcept = default;

    /**
     * @brief Start - starts all of the task executors
     */

    void                    Start           (void);

    /**
     * @brief RequestStop - sets a flag to request that all of the task executors stop
     */
    void                    RequestStop     (void);

    /**
     * @brief Join - waits for all of the task executors to finish
     */
    void                    Join            (void);

    /**
     * @brief Add - adds a task to the task queue
     * @param aTask - task to add
     */
    void                    Add             (Task::SP aTask);

    /**
     * @brief Add - adds a task to the task queue (from the given lambda function)
     * @param aTaskFn - lambda function
     */
    void                    Add             (TaskLambda::FnT aTaskFn);

private:
    /**
     * @brief _MarkAsBusy - marks the given task executor as busy
     * @param aExecutor - reference to task executor
     */
    inline void             _MarkAsBusy     (const TaskExecutor& aExecutor);

    /**
     * @brief _MarkAsIdle - marks the given task executor as idle
     * @param aExecutor - reference to task executor
     */
    inline void             _MarkAsIdle     (const TaskExecutor& aExecutor);

    /**
     * @brief WakeupIdle - wakes up one idle task executor
     */
    inline void             WakeupIdle      (void);

private:
    mutable std::mutex      iMutex;                         //mutex used to synchronize access to iExecutorsIdle
    State                   iState = State::NOT_STARTED;    //current state of the task manager
    const size_t            iExecutorsCount;                //the number of task executors (can be no more than 128 in current implementation, see IdleFlagsT)
    ExecutorsT              iExecutors;                     //is an array of shared pointers to the task executors
    IdleFlagsT              iExecutorsIdle;                 //bitset indicating which task executors are currently idle
    TaskQueue::SP           iTaskQueue;                     //is a shared pointer to the task queue
};

TaskManager::TaskManager (const size_t aExecutorsCount) noexcept:
iExecutorsCount(aExecutorsCount)
{
    iExecutorsIdle.set();
}

void    TaskManager::_MarkAsBusy (const TaskExecutor& aExecutor)
{
    std::scoped_lock lock(iMutex);
    iExecutorsIdle.reset(aExecutor.Id());
}

void    TaskManager::_MarkAsIdle (const TaskExecutor& aExecutor)
{
    std::scoped_lock lock(iMutex);
    iExecutorsIdle.set(aExecutor.Id());
}

void    TaskManager::WakeupIdle (void)
{
    size_t id;

    {
        std::scoped_lock lock(iMutex);

#if defined (_GLIBCXX_BITSET)
        //SGI STL extension
        id = iExecutorsIdle._Find_first();
#else
#       error Implement without _Find_first
#endif
    }

    if (id < iExecutors.size())
    {
        iExecutors.at(id)->Wakeup();
    }
}

}//namespace Tasks
