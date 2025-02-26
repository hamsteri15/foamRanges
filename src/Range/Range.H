/*---------------------------------------------------------------------------*\
    =========                 |
    \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
     \\    /   O peration     | Website:  https://openfoam.org
        \\  /    A nd           | Version:  9
         \\/     M anipulation  |
-------------------------------------------------------------------------------
License
        This file is part of OpenFOAM.

        OpenFOAM is free software: you can redistribute it and/or modify it
        under the terms of the GNU General Public License as published by
        the Free Software Foundation, either version 3 of the License, or
        (at your option) any later version.

        OpenFOAM is distributed in the hope that it will be useful, but WITHOUT
        ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
        FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
        for more details.

        You should have received a copy of the GNU General Public License
        along with OpenFOAM.  If not, see <http://www.gnu.org/licenses/>.

\*---------------------------------------------------------------------------*/

#pragma once

#include "compilerPrimitives.H"
#include "traits.H"

namespace FoamRanges {

template <typename Iterator>
class Range
{
public:

        using iterator = Iterator;
        using value_type = typename std::iterator_traits<Iterator>::value_type;
        using reference = typename std::iterator_traits<Iterator>::reference;
        using difference_type = typename std::iterator_traits<Iterator>::difference_type;

        static constexpr bool is_foam_field = true;
        static constexpr bool is_foam_field_field = false;
        static constexpr bool is_foam_dimensioned_field = false;
        static constexpr bool is_foam_geometric_field = false;

        inline constexpr Range() = default;

        inline constexpr Range(iterator first, iterator last)
        : m_begin(first), m_end(last)
        {}

        template <class Range_t>
        inline constexpr Range(Range_t& rng)
        : Range(std::begin(rng), std::end(rng))
        {}

        template <class Range_t>
        inline constexpr Range(const Range_t& rng)
        : Range(std::begin(rng), std::end(rng))
        {}

        inline constexpr iterator begin() const { return m_begin; }
        inline constexpr iterator begin() { return m_begin; }

        inline constexpr iterator end() const { return m_end; }
        inline constexpr iterator end() { return m_end; }

        inline constexpr difference_type size() const { return end() - begin(); }

        inline constexpr bool empty() const { return begin() == end(); }

        inline constexpr reference operator[](const difference_type& i) const
        {
                return begin()[i];
        }

private:
        iterator m_begin, m_end;
};


template <typename Iterator>
constexpr auto size(const Range<Iterator>& rng)
{
        return rng.size();
}

template <typename Iterator>
constexpr auto makeRange(Iterator first, Iterator last)
{
        return Range<Iterator>(first, last);
}

template <class Range_t>
constexpr auto makeRange(Range_t& rng)
{
        using iterator = decltype(std::begin(rng));
        return Range<iterator>(rng);
}

template <class Range_t>
constexpr auto makeRange(const Range_t& rng)
{
        using iterator = decltype(std::begin(rng));
        return Range<iterator>(rng);
}


} // namespace FoamRanges