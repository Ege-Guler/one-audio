#ifndef FILTER_H_
#define FILTER_H_

#include <vector>
#include <iostream>
#include <cstdint>

class Filter
{
public:

    static void volumeReduceFilter(std::vector<uint16_t>& , size_t);

};


#endif //FILTER_H_