#pragma once

#include "dimensionSet.H"
#include "word.H"
#include "scalar.H"

namespace Foam{

struct PlusOne{

    constexpr
    int operator()(Foam::scalar e ) const {return e + 1;}

    auto operator() (const Foam::dimensionSet& d) const {return d;}

    auto operator() (const Foam::word& w) const {return w;}
};



struct Mag{

    template<class T>
    inline  auto operator()(const T& t) const {return Foam::mag(t);}

    inline  auto operator()(const Foam::dimensionSet& d) const
    {
        return d;
    }

    inline  auto operator()(const Foam::word& t) const
    {
        return "mag(" + t + ')';
    }

};


struct Plus{

    template<class Lhs, class Rhs>
    inline  auto operator()(const Lhs& lhs, const Rhs& rhs) const {
        return lhs + rhs;
    }


    inline  auto operator() (const Foam::word& lhs, const Foam::word& rhs) const
    {
        return "(" + lhs + '+' + rhs + ')';
    }
};



struct Multiplies{

    template<class Lhs, class Rhs>
    inline  auto operator()(const Lhs& lhs, const Rhs& rhs) const {
        return lhs * rhs;
    }


    inline  auto operator() (const Foam::word& lhs, const Foam::word& rhs) const
    {
        return "(" + lhs + '*' + rhs + ')';
    }
};


}
