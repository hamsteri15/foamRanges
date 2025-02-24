#pragma once

#pragma once

#include "dimensioned_range.hpp"
#include "zip_iterator.hpp"
#include "zip_range.hpp"

namespace FoamRanges {

// Note! Here we do not derive from DimensionedRange since there is no need to
// store name and dimensions for zip ranges. Zip ranges are only constructed as
// temporaries for used in transform ranges of binary operations. This class
// could very well be defined inside the DimensionedTransformRange
template <class Rng1, class Rng2> struct DimensionedZipRange {

    using GeoMeshType = decltype(get_mesh(std::declval<Rng1>()));

    constexpr DimensionedZipRange(const Rng1& rng1, const Rng2& rng2)
        : m_rng1(rng1)
        , m_rng2(rng2) {}

    constexpr auto primitiveField() const {
        return make_zip_range(m_rng1.primitiveField(), m_rng2.primitiveField());
    }

    constexpr auto mesh() const { return get_mesh(m_rng1); }

private:
    const Rng1& m_rng1;
    const Rng2& m_rng2;
};

template <class Rng1, class Rng2>
constexpr auto make_dimensioned_zip_range(const Rng1& rng1, const Rng2& rng2) {

    return DimensionedZipRange<Rng1, Rng2>(rng1, rng2);
}

} // namespace FoamRanges
