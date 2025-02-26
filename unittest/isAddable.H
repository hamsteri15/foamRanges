#pragma once

template <class Lhs, class Rhs, class = void>
struct is_addable : std::false_type 
{
};

template <class Lhs, class Rhs>
struct is_addable<Lhs, Rhs, std::void_t<decltype(std::declval<Lhs>() + std::declval<Rhs>())>> : std::true_type {};

template <class Lhs, class Rhs>
constexpr bool is_addable_v = is_addable<Lhs, Rhs>::value;