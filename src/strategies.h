#ifndef STRATEGIES_H
#define STRATEGIES_H

#include <vector>

std::queue<Signal> SMA (int thread_id, float capital, int movingAvgShort, int movingAvgLong, const std::vector<float>& prices);

#endif