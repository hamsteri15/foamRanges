#pragma once

#include "dimensioned_range.hpp"
#include "transform_iterator.hpp"

namespace FoamRanges {

template <class UnaryFunction, class Iterator, class GeoMesh>
struct DimensionedTransformRange
    : public DimensionedRange<
          detail::transform_iterator<UnaryFunction, Iterator>,
          GeoMesh> {

    using iterator = detail::transform_iterator<UnaryFunction, Iterator>;
    using parent   = DimensionedRange<iterator, GeoMesh>;

    template <class Func, class Dims, class Mesh, class Name>
    inline constexpr DimensionedTransformRange(Iterator begin,
                                               Iterator end,
                                               Func&&   f,
                                               Dims&&   dims,
                                               Mesh&&   mesh,
                                               Name&&   name)
        : parent(
              make_range(
                  detail::make_transform_iterator(begin, std::forward<Func>(f)),
                  detail::make_transform_iterator(end, std::forward<Func>(f))),
              std::forward<Dims>(dims),
              std::forward<Mesh>(mesh),
              std::forward<Name>(name)) {}
};

template <class Rng, class Func, class Dims, class Name>
constexpr auto make_dimensioned_transform_range(const Rng& rng,
                                                Func&&     f,
                                                Dims&&     newDims,
                                                Name&&     newName) {


    using iterator = decltype(std::begin(rng.primitiveField()));
    using mesh     = decltype(get_mesh(rng));

    return DimensionedTransformRange<std::decay_t<Func>, iterator, mesh>{
        std::begin(rng.primitiveField()),
        std::end(rng.primitiveField()),
        std::forward<Func>(f),
        std::forward<Dims>(newDims),
        get_mesh(rng),
        std::forward<Name>(newName)};
}

template <class Rng1, class Rng2, class Func, class Dims, class Name>
constexpr auto make_dimensioned_transform_range(const Rng1& rng1,
                                                const Rng2& rng2,
                                                Func&&      f,
                                                Dims&&      newDims,
                                                Name&&      newName) {

    return make_dimensioned_transform_range
    (
        make_dimensioned_zip_range(rng1, rng2),
        std::forward<Func>(f),
        std::forward<Dims>(newDims),
        std::forward<Name>(newName)
    );

}

} // namespace FoamRanges