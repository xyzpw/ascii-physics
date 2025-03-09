#include <chrono>
#include "utils/epoch_utils.h"

using std::chrono::system_clock;
using std::chrono::milliseconds;
using std::chrono::duration_cast;

double getEpochAsDecimal()
{
    auto epochChrono = duration_cast<milliseconds>(
        system_clock::now().time_since_epoch()
    );

    double epoch = static_cast<double>(epochChrono.count());
    epoch /= 1000.0;

    return epoch;
}
