#ifndef FILTER_H_
#define FILTER_H_

#include <vector>
#include <iostream>
#include <cstdint>
#include <fftw3.h>
#include <math.h>
#include <cmath>
#include <algorithm>

class Filter
{
public:

    static void volumeReduceFilter(std::vector<uint16_t>& , size_t);
    static void muffleBreathing(std::vector<uint16_t>&, size_t, unsigned int, unsigned int);
};


#endif //FILTER_H_