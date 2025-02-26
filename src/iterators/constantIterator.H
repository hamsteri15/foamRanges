#pragma once

#include <iterator>
#include "compilerPrimitives.H"

#ifdef __NVIDIA_COMPILER__
#include <thrust/iterator/constant_iterator.h>
#endif

namespace FoamRanges {

#ifdef __NVIDIA_COMPILER__

template <class T>
using constantIterator = thrust::constant_terator<T>;

#else

template <class T>
class constantIterator {
public:
    using self = constantIterator<T>;

    using value_type = T;
    using reference  = T; //No idea why this has to be a T instead of T&

    using difference_type   = std::ptrdiff_t;
    using pointer           = T*;
    using iterator_category = std::random_access_iterator_tag;

    constantIterator() = default;

    inline constexpr constantIterator(const T&        value,
                                          difference_type index = 0)
        : m_value(value)
        , m_index(index) {}

private:
    inline constexpr const T& dereference() const { return m_value; }
    inline constexpr T&       dereference() { return m_value; }

    inline constexpr bool equal(const constantIterator<T>& other) const {
        return m_value == other.m_value && m_index == other.m_index;
    }

    inline constexpr void increment() { m_index++; }

    inline constexpr void decrement() { m_index--; }

    inline constexpr void advance(difference_type n) { m_index += n; }

    inline constexpr difference_type
    distance_to(const constantIterator<T>& other) const {
        return static_cast<difference_type>(other.m_index - m_index);
    }

public:
    inline constexpr const T&  operator*() const { return dereference(); }
    inline constexpr T&        operator*() { return dereference(); }
    inline constexpr pointer   operator->() const { return &m_value; }
    inline constexpr T& operator[](difference_type) {
        return dereference();
    }

    inline constexpr const T& operator[](difference_type) const {
        return dereference();
    }

    inline constexpr self& operator++() {
        increment();
        return *this;
    }
    // inline constexpr self            operator++(int) {} //TODO:
    inline constexpr self& operator--() {
        decrement();
        return *this;
    }
    // inline constexpr self            operator--(int) {} //TODO:
    inline constexpr self& operator+=(difference_type i) {
        advance(i);
        return *this;
    }
    inline constexpr self& operator-=(difference_type i) {
        advance(-i);
        return *this;
    }

    inline constexpr self operator+(difference_type i) const {
        return self(m_value, m_index + i);
    }
    inline constexpr self operator-(difference_type i) const {
        return self(m_value, m_index - i);
    }
    inline constexpr difference_type operator-(const self& rhs) const {
        return this->m_index - rhs.m_index;
    }
    inline constexpr bool operator==(const self& rhs) const {
        return this->m_index == rhs.m_index; /*this->equal(rhs);*/
    }
    inline constexpr bool operator!=(const self& rhs) const {
        return !(*this == rhs);
    }

    inline constexpr bool operator<(const self& rhs) const {
        return this->m_index < rhs.m_index;
    }
    inline constexpr bool operator<=(const self& rhs) const {
        return this->m_index <= rhs.m_index;
    }
    inline constexpr bool operator>(const self& rhs) const {
        return this->m_index > rhs.m_index;
    }
    inline constexpr bool operator>=(const self& rhs) const {
        return this->m_index >= rhs.m_index;
    }

private:
    T               m_value;
    difference_type m_index;
};


#endif

} // namespace FoamRanges