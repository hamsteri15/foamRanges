#pragma once

#include <array>

namespace FoamRanges {

template <class T>
using small_array = std::array<T, 100>;

}