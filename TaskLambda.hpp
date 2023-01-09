#pragma once

#include <functional>
#include "Task.hpp"

namespace Tasks {

/**
 * @class TaskLambda
 * @brief This is a class representing a task that is implemented as a lambda function.
 * Constructor takes a lambda function and initializes the corresponding member variable.
 * Run() is the implementation of the Task virtual function, which calls the lambda function when the task is executed.
 */

class TaskLambda final: public Task
{
public:
	using SP	= std::shared_ptr<Task>;
	using FnT	= std::function<void()>;

public:
					TaskLambda	(void) noexcept = delete;
					TaskLambda	(const TaskLambda&) noexcept = delete;
					TaskLambda	(TaskLambda&&) noexcept = delete;

	/**
	 * @brief TaskLambda - constructor
	 * @param aFnT - lambda function
	 */
	inline			TaskLambda	(FnT aFnT) noexcept;
	virtual			~TaskLambda	(void) noexcept override final;

	virtual void	Run			(void) override final;

private:
	FnT				iFn;
};

TaskLambda::TaskLambda (FnT aFnT) noexcept:
iFn(std::move(aFnT))
{
}

}//namespace Tasks
