#pragma once

#include "tuple.hpp"

namespace FoamRanges {

namespace detail {

// This is a wrapper class for a binary operation of a dereferenced zip iterator
// which returns a tuple.
template <typename BinaryOp> struct ApplyBinaryOp {

    BinaryOp op;

    inline constexpr ApplyBinaryOp() = default;

    inline constexpr ApplyBinaryOp(const BinaryOp& f)
        : op(f) {}

    template <typename Tuple>
    inline constexpr auto operator()(Tuple&& t) const {

        return op(get<0>(std::forward<Tuple>(t)),
                  get<1>(std::forward<Tuple>(t)));
    }
};

} // namespace detail

}