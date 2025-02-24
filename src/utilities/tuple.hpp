#pragma once

#include <tuple> //std::tuple
#include "compiler_primitives.hpp"

#ifdef __NVIDIA_COMPILER__
#include <thrust/tuple.h>
#endif



namespace FoamRanges {


    #ifdef __NVIDIA_COMPILER__

        template<class... Types>
        using Tuple = thrust::tuple<Types...>;

        using thrust::get;
        //using thurst::tuple_size;

        template<class T>
        using tuple_size = thrust::tuple_size<T>;

        template< class... Types >
        inline constexpr
        auto adl_make_tuple( Types&&... args ) {
            return thrust::make_tuple(std::forward<Types>(args)...);
        }

    #else


        template<class... Types>
        using Tuple = std::tuple<Types...>;

        template<class T>
        using tuple_size = std::tuple_size<T>;
        using std::get;

        template< class... Types >
        inline constexpr auto adl_make_tuple( Types&&... args ) {
            return std::make_tuple(std::forward<Types>(args)...);
        }



    #endif


} // namespace FoamRanges
