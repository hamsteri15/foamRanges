#pragma once

#include "primitiveFields.H"
#include "traits.H"
#include "copy.H"
#include "operations.H"
#include "promotingTransforms.H"

namespace Foam{



template<class T>
struct NewField : public Field<T>{

    using base_type = Field<T>;

    //Inherit constructors
    using Field<T>::Field;

    static constexpr bool is_foam_field = true;
    static constexpr bool is_foam_field_field = false;
    static constexpr bool is_foam_dimensioned_field = false;
    static constexpr bool is_foam_geometric_field = false;

    //Allow construction from ranges
    template<class Rng, std::enable_if_t<IsFoamField_v<Rng>, int> = 0>
    NewField(const Rng& rng) : base_type(rng.size())
    {
        FoamRanges::copy(rng, *this);
    }

    //Allow copy assignment from ranges
    template<class Rng, std::enable_if_t<IsFoamField_v<Rng>, int> = 0>
    NewField& operator=(const Rng& rng)
    {
        FoamRanges::copy(rng, *this);
        return *this;
    }


};

template<class T>
static inline NewField<T> toNewField(const Field<T>& f)
{
    return NewField<T>(FoamRanges::makeRange(f));
}

template<class T>
static inline NewField<T> toNewField(const tmp<Field<T>>& f)
{
    return toNewField(f());
}


///These are the operations for newFields
template<class Range_t, std::enable_if_t<IsFoamField_v<Range_t>, bool> = true>
constexpr auto mag2(const Range_t& rng)
{
    return FoamRanges::transform(rng, Mag{});
}


template<class T1, class T2, std::enable_if_t<AtleastOneFoamField_v<T1, T2>, bool> = true>
constexpr auto plus(const T1& rng1, const T2& rng2)
{
    return detail::rngPromotingTransform(rng1, rng2, Plus{});
}


template<class T1, class T2, std::enable_if_t<AtleastOneFoamField_v<T1, T2>, bool> = true>
constexpr auto mult(const T1& rng1, const T2& rng2)
{
    return detail::rngPromotingTransform(rng1, rng2, Multiplies{});
}

} // namespace Foam