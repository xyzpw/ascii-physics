#include <thread>
#include <chrono>

void sleepCurrentThread(const int ms)
{
    auto dur = std::chrono::milliseconds(ms);
    std::this_thread::sleep_for(dur);
}
