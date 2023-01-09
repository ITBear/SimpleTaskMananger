# SimpleTaskMananger
Welcome to the C++ Simple Task Manager library!

This library provides a simple and lightweight way to manage the execution of tasks concurrently in C++.

Class [`TaskManager`](/TaskManager.hpp) is responsible for scheduling and executing [`tasks`](/Task.hpp).
 
You can add tasks to the manager using the Add function, and the tasks will be executed by worker threads in the order they are received. The number of worker threads is configurable, and the default is to use as many worker threads as there are hardware threads on the machine.

The TaskManager class is thread-safe, so you can add tasks from any thread without worrying about synchronization.

To use the library, simply include all the cpp and hpp (except main.cpp) files to you project.

## Example:

```cpp
#include "TaskManager.hpp"
...
//Detect CPU cores count
const size_t cpuCoresCount = std::thread::hardware_concurrency();

//Create task manager
TaskManager taskManager(cpuCoresCount);

//Start
taskManager.Start();

//Add tasks
taskManager.Add([]()
{
  std::cout << "Hello from task 1\n";
  std::cout.flush();
});

taskManager.Add([]()
{
  std::cout << "Hello from task 2\n";
  std::cout.flush();
});

...

//Request stop
taskManager.RequestStop();

//Wait for stop
taskManager.Join();
...
```

## How to build main.cpp
In the SimpleTaskMananger folder:
```bash
cmake .
make
./TaskQueueDemo
```
