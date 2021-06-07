#include <ctime>

#include <chrono>

double microtime()
{
    return (double
            (std::chrono::duration_cast<std::chrono::microseconds>
             (std::chrono::system_clock::now().time_since_epoch()).count()) / double(1000000));
}
