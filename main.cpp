#include "TaskManager.hpp"

#include <iostream>
#include <random>
#include <string_view>

using namespace Tasks;

std::mutex stdCoutMutex;

void    SCoutSync (std::function<void()> aFn)
{
    std::scoped_lock lock(stdCoutMutex);

    aFn();
    std::cout.flush();
}

int main ()
{
    //Detect CPU cores count
    const size_t cpuCoresCount = std::thread::hardware_concurrency();

    std::cout << "Current CPU cores count " << cpuCoresCount;

    //Create task manager
    std::cout << "\nCreate task manager..." << cpuCoresCount;
    TaskManager taskManager(cpuCoresCount);

    //Start
    std::cout << "\nStart task manager..." << cpuCoresCount;
    std::cout.flush();

    taskManager.Start();

    //Lambda function that represents task 'payload'
    auto taskFn = []()
    {
        SCoutSync([](){std::cout << "Hello from thread " << std::this_thread::get_id() << "\n";});


        //Do some calculations
        //Random generator
        std::mt19937        engine;
        std::random_device  rd;
        std::array<std::mt19937::result_type, std::mt19937::state_size> seed_data;
        std::generate_n(seed_data.data(), seed_data.size(), std::ref(rd));
        std::seed_seq seq(std::begin(seed_data), std::end(seed_data));
        engine.seed(seq);
        std::uniform_int_distribution<size_t> dist(0, 1000);

        //Storage
        std::vector<size_t> v;
        v.resize(1024);

        //Fill with random values
        for (size_t k = 0; k < v.size(); k++)
        {
            v[k] = dist(engine);
        }

        //Calculate avg
        size_t sum = 0;
        for (size_t k = 0; k < v.size(); k++)
        {
            sum += v[k];
        }

        const double avg = double(sum) / double(v.size());

        SCoutSync([aAvg = avg](){std::cout << "Task done (thread " << std::this_thread::get_id() << "), avg = " << aAvg << "\n";});
    };

    //Create task instances (count = 300)
    for (size_t taskId = 0; taskId < 300; taskId++)
    {
        taskManager.Add(taskFn);
    }

    SCoutSync([](){std::cout << "Wait...\n";});

    //We must suspend main thread for some time to delay RequestStop call. After RequestStop() is called:
    // - No new tasks can be added to the queue.
    // - Currently executing tasks will complete.
    // - Tasks in the queue will be deleted.
    // - All executors will stop working once the currently executing tasks are finished.
    std::this_thread::sleep_for(std::chrono::milliseconds(3000));

    SCoutSync([](){std::cout << "Request stop...\n";});
    taskManager.RequestStop();

    SCoutSync([](){std::cout << "Join...\n";});
    taskManager.Join();

    SCoutSync([](){std::cout << "DONE!\n";});

    return 0;
}
