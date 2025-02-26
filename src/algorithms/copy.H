#pragma once

#include "Range.H"
#include <algorithm>


#ifdef __NVIDIA_COMPILER__
#include <thrust/copy.h>
#endif

namespace FoamRanges {

#ifdef __NVIDIA_COMPILER__

template <class Range1_t, class Range2_t>
void copy(const Range1_t& src, Range2_t& dst) {
    thrust::copy(thrust::device, src.begin(), src.end(), dst.begin());
}

#else

template <class Range1_t, class Range2_t>
void copy(const Range1_t& src, Range2_t& dst) {
    std::copy(src.begin(), src.end(), dst.begin());
}

#endif

} // namespace FoamRanges