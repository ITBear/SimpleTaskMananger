#pragma once

#include <memory>

namespace Tasks {

/**
 * @class Task
 * @brief Base class for representing a task to be done.
 *
 * This class provides a base for creating objects that represent a task to be done.
 * Derived classes must implement the Run() function to define the behavior of the task.
 */


class Task
{
public:
	/**
	 * @brief Alias for a shared pointer to a Task object.
	 */
	using SP = std::shared_ptr<Task>;

protected:
					Task	(void) noexcept = default;
					Task	(const Task&) noexcept = delete;
					Task	(Task&&) noexcept = delete;

public:
	virtual			~Task	(void) noexcept = default;

public:
	/**
	 * @brief This function defines the behavior of the task.
	 */
	virtual void	Run		(void) = 0;
};

}//namespace Tasks
