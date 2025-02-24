#pragma once

#include "range.hpp"
#include "tuple.hpp"
#include "zip_iterator.hpp"

#ifdef __NVIDIA_COMPILER__
#include <thrust/iterator/zip_iterator.h>
#endif
namespace FoamRanges {

namespace detail{


    #ifdef __NVIDIA_COMPILER__

        template<class T>
        using zip_iterator = thrust::zip_iterator<T>;

        using thrust::make_zip_iterator;


    #else

        /*

        template<class T>
        using zip_iterator = boost::zip_iterator<T>;

        using boost::make_zip_iterator;
        */


        template<class T>
        using zip_iterator = FoamRanges::zip_iterator<T>;


        using FoamRanges::make_zip_iterator;



    #endif
}

template <typename IteratorTuple>
struct ZipRange : public Range<detail::zip_iterator<IteratorTuple>> {

    using parent = Range<detail::zip_iterator<IteratorTuple>>;

    ZipRange() = default;

    inline constexpr ZipRange(IteratorTuple first, IteratorTuple last)
        : parent(first, last) {}
};

template <typename Range1_t, typename Range2_t>
inline constexpr auto make_zip_range(const Range1_t& rng1,
                                     const Range2_t& rng2) {

    using iter1   = decltype(std::begin(rng1));
    using iter2   = decltype(std::begin(rng2));
    using tuple_t = Tuple<iter1, iter2>;

    return ZipRange<tuple_t>(adl_make_tuple(std::begin(rng1), std::begin(rng2)),
                             adl_make_tuple(std::end(rng1), std::end(rng2)));
}

} // namespace FoamRanges