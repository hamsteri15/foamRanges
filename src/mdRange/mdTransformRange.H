#pragma once

#include "mdConstantRange.H"
#include "mdRange.H"

// Here we do a bit of sorcery. Instead of having a cheap value type that we
// always create and store by value in MdTransformRange and MdTransformZipRange
// we use perfect forwarding for the input MdRange(s). Essentially, this means
// that the type of Rng in MdTransformRange or Rng1 and Rng2 in
// MdTransformZipRange can be wither rvalue references (temporaries which are
// moved and stored by value) or lvalue references (which are stored by
// reference). The reason for doing this is to avoid creating an array of 1D
// Ranges upon construction since generally the amount of ranges in MdRange is
// not known at compile time. Another (very viable) option would be to have a
// std::vector of 1D ranges and return by reference in the operator[] of the
// above classes. However, this would prevent constexpr creation of expression
// chains for MdRange types.

namespace FoamRanges {

template <class Rng, class UnaryFunction>
struct MdTransformRange : public MdRange {


    using transform_range_t
        = decltype(makeTransformRange(std::declval<std::decay_t<Rng>>()[0], std::declval<std::decay_t<UnaryFunction>>()));
    using iterator = typename transform_range_t::iterator;
    using value_type = typename std::iterator_traits<iterator>::value_type;



    inline constexpr MdTransformRange(Rng&& rng, UnaryFunction&& f)
        : m_rng(std::forward<Rng>(rng))
        , m_f(std::forward<UnaryFunction>(f)) {}

    constexpr auto operator[](size_t i) const {
        return makeTransformRange(m_rng[i], m_f);
    }
    constexpr auto size() const { return m_rng.size(); }

private:
    Rng           m_rng; //This can be either a reference (lvalue) or a value (rvalue)
    UnaryFunction m_f;
};






namespace detail{
template<class T, typename = void>
struct IsMdConstantRange : public std::false_type{};

template<class Value>
struct IsMdConstantRange<MdConstantRange<Value>> : public std::true_type{};

template<class Value>
struct IsMdConstantRange<const MdConstantRange<Value>&> : public std::true_type{};

template<class Value>
struct IsMdConstantRange<MdConstantRange<Value>&> : public std::true_type{};

template<class T>
static constexpr bool IsMdConstantRange_v = IsMdConstantRange<T>::value;

}

template <class Rng1, class Rng2, class UnaryFunction>
struct MdTransformZipRange : public MdRange {


    MdTransformZipRange(Rng1&& rng1, Rng2&& rng2, UnaryFunction&& f)
        : m_rng1(std::forward<Rng1>(rng1))
        , m_rng2(std::forward<Rng2>(rng2))
        , m_f(std::forward<UnaryFunction>(f)) {}

    inline constexpr size_t size() const {

        if constexpr (detail::IsMdConstantRange_v<Rng1>) {
            return m_rng2.size();
        }
        else {
            return m_rng1.size();
        }
    }

    constexpr auto operator[](size_t i) const {

        if constexpr (!detail::IsMdConstantRange_v<Rng1> && !detail::IsMdConstantRange_v<Rng2>) {

            return
            makeTransformRange
            (
                makeZipRange
                (
                    m_rng1[i],
                    m_rng2[i]
                ),
                m_f
            );

        }
        else if constexpr (detail::IsMdConstantRange_v<Rng1>) {
            return makeTransformRange
            (
                makeZipRange
                (
                    makeConstantRange(m_rng1.get_value(), m_rng2[i].size()),
                    m_rng2[i]
                ),
                m_f
            );
        }
        else{
            return makeTransformRange
            (
                makeZipRange
                (
                    m_rng1[i],
                    makeConstantRange(m_rng2.get_value(), m_rng1[i].size())
                ),
                m_f
            );
        }


    }

private:
    Rng1          m_rng1; //This can be either a reference (lvalue) or a value (rvalue)
    Rng2          m_rng2; //This can be either a reference (lvalue) or a value (rvalue)
    UnaryFunction m_f;
};


template <class Rng, class Function>
inline constexpr auto make_md_transform_range(Rng&& rng, Function&& f) {

    return MdTransformRange<Rng, Function>(std::forward<Rng>(rng),
                                           std::forward<Function>(f));
}

template <typename Rng1, typename Rng2, typename BinaryOp>
auto make_md_transform_range(Rng1&& rng1, Rng2&& rng2, BinaryOp&& op) {

    return MdTransformZipRange<Rng1, Rng2, BinaryOp>(
        std::forward<Rng1>(rng1),
        std::forward<Rng2>(rng2),
        std::forward<BinaryOp>(op));
}

} // namespace FoamRanges