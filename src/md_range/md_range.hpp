#pragma once

#include "range.hpp"

namespace FoamRanges {

struct MdRange{

    static constexpr bool is_foam_field = false;
    static constexpr bool is_foam_field_field = true;
    static constexpr bool is_foam_dimensioned_field = false;
    static constexpr bool is_foam_geometric_field = false;
};



} // namespace FoamRanges