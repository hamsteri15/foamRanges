/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     | Website:  https://openfoam.org
    \\  /    A nd           | Copyright (C) 2011-2024 OpenFOAM Foundation
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

#include <type_traits>

//These are shorthands for the std::enable_if_t to be used in the header file (REQUIRES) 
//and in a definition (REQUIRES_DEF) to avoid the the use of std::enable_if_t everywhere.
//See below for an example.

#define REQUIRES(...) std::enable_if_t<(__VA_ARGS__), int> = 0

//If a separate definition is needed this version must be used in the definition
#define REQUIRES_DEF(...) std::enable_if_t<(__VA_ARGS__), int>



//Example usage:

//In a header file:
//template<class T, REQUIRES(IsFoamField_v<T>)>
//void myFunction(const T& field);

//In a source file:
//template<class T, REQUIRES_DEF(IsFoamField_v<T>)>
//void myFunction(const T& field)
//{
//    //do something with field
//}
