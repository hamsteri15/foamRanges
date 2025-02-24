#pragma once

#include "compiler_primitives.hpp"

#include "traits.hpp"

namespace FoamRanges {

template <typename Iterator>
class Range {
public:

    using iterator   = Iterator;
    using value_type = typename std::iterator_traits<Iterator>::value_type;
    using reference  = typename std::iterator_traits<Iterator>::reference;
    using difference_type = typename std::iterator_traits<Iterator>::difference_type;

    static constexpr bool is_foam_field = true;
    static constexpr bool is_foam_field_field = false;
    static constexpr bool is_foam_dimensioned_field = false;
    static constexpr bool is_foam_geometric_field = false;

    inline constexpr Range() = default;

    inline constexpr Range(iterator first, iterator last)
        : m_begin(first)
        , m_end(last) {}

    template <class Range_t>
    inline constexpr Range(Range_t& rng)
        : Range(std::begin(rng), std::end(rng)) {}

    template <class Range_t>
    inline constexpr Range(const Range_t& rng)
        : Range(std::begin(rng), std::end(rng)) {}

    inline constexpr iterator begin() const { return m_begin; }
    inline constexpr iterator begin() { return m_begin; }

    inline constexpr iterator end() const { return m_end; }
    inline constexpr iterator end()  { return m_end; }

    inline constexpr difference_type size() const { return end() - begin(); }

    inline constexpr bool empty() const { return begin() == end(); }

    inline constexpr reference operator[](const difference_type& i) const {
        //return static_cast<value_type>(begin()[i]);
        return begin()[i];
        //return static_cast<value_type>(begin()[i].value());
    }

private:
    iterator m_begin, m_end;
};


template <typename Iterator>
constexpr auto size(const Range<Iterator>& rng) {
    return rng.size();
}

template <typename Iterator>
constexpr auto make_range(Iterator first, Iterator last) {
    return Range<Iterator>(first, last);
}

template <class Range_t>
constexpr auto make_range(Range_t& rng) {
    using iterator = decltype(std::begin(rng));
    return Range<iterator>(rng);
}

template <class Range_t>
constexpr auto make_range(const Range_t& rng) {
    using iterator = decltype(std::begin(rng));
    return Range<iterator>(rng);
}

template <class Range_t, typename Size>
constexpr auto slice(Range_t& rng, Size first, Size last) {
    return make_range(std::begin(rng) + first, std::begin(rng) + last);
}

template <class Range_t, typename Size>
constexpr auto slice(const Range_t& rng, Size first, Size last) {
    return make_range(std::begin(rng) + first, std::begin(rng) + last);
}

template <class Range_t, typename Size>
constexpr auto take(Range_t& rng, Size n) {
    return slice(rng, Size(0), n);
}

template <class Range_t, typename Size>
constexpr auto take(const Range_t& rng, Size n) {
    return slice(rng, Size(0), n);
}

} // namespace FoamRanges