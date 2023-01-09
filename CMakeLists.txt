cmake_minimum_required(VERSION 3.5)

project(TaskQueueDemo LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCES
    main.cpp
    TaskLambda.cpp
    TaskManager.cpp
    TaskExecutor.cpp)

set(HEADERS
    Task.hpp
    TaskQueue.hpp
    TaskLambda.hpp
    TaskManager.hpp
    TaskExecutor.hpp)

add_executable(TaskQueueDemo ${SOURCES} ${HEADERS})
