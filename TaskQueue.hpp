#pragma once

#include <optional>
#include <deque>
#include <mutex>
#include "Task.hpp"

namespace Tasks {

/**
 * @brief The TaskQueue class representing a task queue. Using a deque (double-ended queue) as internal container that stores the tasks.
 */

class TaskQueue
{
public:
    using SP                    = std::shared_ptr<TaskQueue>;
    using NextTaskT             = std::optional<Task::SP>;
    using InternalContainerT    = std::deque<Task::SP>;

public:
                        TaskQueue   (void) noexcept = default;
                        TaskQueue   (const TaskQueue&) noexcept = delete;
                        TaskQueue   (TaskQueue&&) noexcept = delete;
                        ~TaskQueue  (void) noexcept = default;

    /**
     * @brief Add - adds a task to the back of the task queue.
     * @param aTask - shared pointer to task
     */
    inline void         Add         (Task::SP aTask);

    /**
     * @brief Next - removes and returns the next task from the front of the task queue, or returns std::nullopt if the task queue is empty.
     * @return - next task from the front of the task queue, or returns std::nullopt if the task queue is empty.
     */
    inline NextTaskT    Next        (void);

private:
    mutable std::mutex  iMutex; //used to synchronize access to the task queue
    InternalContainerT  iTasks; //Stores the tasks in the task queue
};

void    TaskQueue::Add (Task::SP aTask)
{
    std::scoped_lock lock(iMutex);
    iTasks.emplace_back(std::move(aTask));
}

TaskQueue::NextTaskT    TaskQueue::Next (void)
{
    std::scoped_lock lock(iMutex);

    if (iTasks.size() == 0)
    {
        return std::nullopt;
    }

    NextTaskT res = std::move(iTasks.front());
    iTasks.pop_front();

    return res;
}

}//namespace Tasks
