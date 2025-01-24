#include "filter.h"


void Filter::volumeReduceFilter(std::vector<uint16_t>& buffer, size_t frames){
    for (size_t i = 0; i < frames; ++i) {
        buffer[i] *= 100; 
    }
}