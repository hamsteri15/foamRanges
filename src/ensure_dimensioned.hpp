#pragma once

#include "dimensionSet.H"


namespace Foam{

//Note! This overload also promotes scalars to dimensioned scalars. Slightly dangerous but necessary for some operations

template<class T>
struct EnsureDimensioned{

    constexpr EnsureDimensioned(const T& t) : type(t) {

    }

    Foam::dimensioned<T> type;

};

template<class T>
struct EnsureDimensioned<Foam::dimensioned<T>>{

    constexpr EnsureDimensioned(const Foam::dimensioned<T>& t) : type(t) {

    }

    Foam::dimensioned<T> type;
};




}