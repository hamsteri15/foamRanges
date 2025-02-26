#pragma once

#include "dimensionedRange.H"
#include "transformIterator.H"

namespace FoamRanges {

template <class UnaryFunction, class Iterator, class GeoMesh>
struct DimensionedTransformRange
    : public DimensionedRange<
          detail::transformIterator<UnaryFunction, Iterator>,
          GeoMesh> {

    using iterator = detail::transformIterator<UnaryFunction, Iterator>;
    using parent   = DimensionedRange<iterator, GeoMesh>;

    template <class Func, class Dims, class Mesh, class Name>
    inline constexpr DimensionedTransformRange(Iterator begin,
                                               Iterator end,
                                               Func&&   f,
                                               Dims&&   dims,
                                               Mesh&&   mesh,
                                               Name&&   name)
        : parent(
              makeRange(
                  detail::makeTransformIterator(begin, std::forward<Func>(f)),
                  detail::makeTransformIterator(end, std::forward<Func>(f))),
              std::forward<Dims>(dims),
              std::forward<Mesh>(mesh),
              std::forward<Name>(name)) {}
};

template <class Rng, class Func, class Dims, class Name>
constexpr auto makeDimensionedTransformRange(const Rng& rng,
                                                Func&&     f,
                                                Dims&&     newDims,
                                                Name&&     newName) {


    using iterator = decltype(std::begin(rng.primitiveField()));
    using mesh     = decltype(getGeoMesh(rng));

    return DimensionedTransformRange<std::decay_t<Func>, iterator, mesh>{
        std::begin(rng.primitiveField()),
        std::end(rng.primitiveField()),
        std::forward<Func>(f),
        std::forward<Dims>(newDims),
        getGeoMesh(rng),
        std::forward<Name>(newName)};
}

template <class Rng1, class Rng2, class Func, class Dims, class Name>
constexpr auto makeDimensionedTransformRange(const Rng1& rng1,
                                                const Rng2& rng2,
                                                Func&&      f,
                                                Dims&&      newDims,
                                                Name&&      newName) {

    return makeDimensionedTransformRange
    (
        makeDimensionedZipRange(rng1, rng2),
        std::forward<Func>(f),
        std::forward<Dims>(newDims),
        std::forward<Name>(newName)
    );

}

} // namespace FoamRanges