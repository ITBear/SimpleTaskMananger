#include "TaskManager.hpp"

namespace Tasks {

void    TaskManager::Start (void)
{
    //Check state
    if (iState != State::NOT_STARTED)
    {
        throw std::runtime_error("[TaskManager::Start]: Wrong state. Must be NOT_STARTED");
    }

    //Check executors count
    if (iExecutorsCount < 1)
    {
        throw std::runtime_error("[TaskManager::Start]: Wrong executors count. Must be > 0");
    }

    //Check executors count
    if (iExecutorsCount > iExecutors.size())
    {
        throw std::runtime_error("[TaskManager::Start]: Wrong executors count. Must be <= " + std::to_string(iExecutors.size()));
    }

    //Create task queue
    iTaskQueue = std::make_shared<TaskQueue>();

    //Create and start executors
    for (size_t i = 0; i < iExecutorsCount; i++)
    {
        TaskExecutor::SP executor = std::make_shared<TaskExecutor>
        (
            i,
            iTaskQueue,
            *this
        );

        executor->Start();
        iExecutors[i] = std::move(executor);
    }

    //Update state
    iState = State::STARTED;
}

void    TaskManager::RequestStop (void)
{
    //Check state
    if (iState != State::STARTED)
    {
        throw std::runtime_error("[TaskManager::RequestStop]: Wrong state. Must be STARTED");
    }

    //Request stop
    for (TaskExecutor::SP& executor: iExecutors)
    {
        if (executor != nullptr)
        {
            executor->RequestStop();
            executor->Wakeup();
        }
    }

    iState = State::STOP_REQUESTED;
}

void    TaskManager::Join (void)
{
    //Check state
    if (iState != State::STOP_REQUESTED)
    {
        throw std::runtime_error("[TaskManager::Join]: Wrong state. Must be STOP_REQUESTED");
    }

    //Join
    for (TaskExecutor::SP& executor: iExecutors)
    {
        if (executor != nullptr)
        {
            executor->Wakeup();
            executor->Join();
        }
    }

    iState = State::STOPPED;
}

void    TaskManager::Add (Task::SP aTask)
{
    //Check state
    if (iState != State::STARTED)
    {
        throw std::runtime_error("[TaskManager::Add]: Wrong state. Must be STARTED");
    }

    //Add task to queue
    iTaskQueue->Add(std::move(aTask));

    //Wakeup one executor
    WakeupIdle();
}

void    TaskManager::Add (TaskLambda::FnT aTaskFn)
{
    Add
    (
        std::make_shared<TaskLambda>
        (
            std::move(aTaskFn)
        )
    );
}

}//namespace Tasks
