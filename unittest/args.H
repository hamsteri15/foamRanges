/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     | Website:  https://openfoam.org
    \\  /    A nd           | Version:  10
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
    FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with OpenFOAM.  If not, see <http://www.gnu.org/licenses/>.

Description
    Utility functions for managing Foam::argList.

SourceFiles

\*---------------------------------------------------------------------------*/

#pragma once

#include "argList.H"

extern Foam::argList* argsPtr;

inline Foam::argList& getFoamArgs()
{
    return *argsPtr;
}

inline void setFoamArgs(int argc, char* argv[])
{
    argsPtr = new Foam::argList(argc, argv);
}

inline void clearFoamArgs()
{
    delete argsPtr;
}