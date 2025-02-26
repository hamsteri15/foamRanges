#pragma once

#include <iterator>
#include <type_traits>
#include "compilerPrimitives.H"


#ifdef __NVIDIA_COMPILER__
#include <thrust/iterator/transform_iterator.h>
#endif
namespace FoamRanges {

namespace detail {

#ifdef __NVIDIA_COMPILER__

template <class Func, class Iter>
using transformIterator = thrust::transform_iterator<Func, Iter>;

template <class Func, class Iter>
inline constexpr auto makeTransformIterator(Iter it, const Func& f) {
    return thrust::makeTransformIterator(it, f);
}

#else

template <class Func, class Iter>
class transformIterator {

public:
    Iter m_it;
    Func m_func;

    inline constexpr transformIterator() = default;

    inline constexpr transformIterator(Iter it, const Func& func)
        : m_it(it)
        , m_func(func) {}

    using difference_type =
        typename std::iterator_traits<Iter>::difference_type;
    using reference  = typename std::result_of<const Func(
        typename std::iterator_traits<Iter>::reference)>::type;
    using pointer    = void;
    using value_type = std::remove_reference_t<reference>;
    // using value_type = reference;

    // using iterator_category = std::input_iterator_tag;
    using iterator_category = std::random_access_iterator_tag;

    using my_type = transformIterator<Func, Iter>;

    inline constexpr reference dereference() const { return m_func(*m_it); }

    inline constexpr bool operator==(const my_type& rhs) const {
        return m_it == rhs.m_it;
    }
    inline constexpr bool operator!=(const my_type& rhs) const {
        return m_it != rhs.m_it;
    }

    inline constexpr bool operator<(const my_type& rhs) const {
        return m_it < rhs.m_it;
    }
    inline constexpr bool operator<=(const my_type& rhs) const {
        return m_it <= rhs.m_it;
    }
    inline constexpr bool operator>(const my_type& rhs) const {
        return m_it > rhs.m_it;
    }
    inline constexpr bool operator>=(const my_type& rhs) const {
        return m_it >= rhs.m_it;
    }

    inline constexpr difference_type operator-(const my_type& rhs) const {
        return m_it - rhs.m_it;
    }

    inline constexpr difference_type operator+(const my_type& rhs) const {
        return m_it + rhs.m_it;
    }

    inline constexpr my_type operator+(difference_type i) const {
        return my_type(m_it + i, m_func);
    }
    inline constexpr my_type operator-(difference_type i) const {
        return my_type(m_it - i, m_func);
    }

    inline constexpr my_type& operator+=(difference_type i) {
        m_it = m_it + i;
        return *this;
    }
    inline constexpr my_type& operator-=(difference_type i) {
        m_it = m_it - i;
        return *this;
    }

    inline constexpr auto operator[](difference_type i) const {
        return m_func(m_it[i]);
    }

    // auto& operator[](difference_type i) { return m_func(m_it[i]); }

    inline constexpr auto operator*() const { return dereference(); }

    inline constexpr my_type& operator++() {
        ++m_it;
        return *this;
    }
    inline constexpr my_type& operator--() {
        --m_it;
        return *this;
    }
    inline constexpr my_type& operator++(int) {
        auto prev = *this;
        ++m_it;
        return prev;
    }
};

template <class Func, class Iter>
inline constexpr transformIterator<Func, Iter>
                    makeTransformIterator(Iter it, Func f) {
    return transformIterator<Func, Iter>(it, f);
}

#endif
} // namespace detail



} // namespace FoamRanges