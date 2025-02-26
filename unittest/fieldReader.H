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
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with OpenFOAM.  If not, see <http://www.gnu.org/licenses/>.

Class
    Foam::FieldReader

Description
    Class for reading scalar and vector fields from files.

SourceFiles

\*---------------------------------------------------------------------------*/

#pragma once


#include <filesystem>
#include "Time.H"
#include "volFields.H"
#include "fvMesh.H"
#include "args.H"
#include "fileName.H"

namespace Foam
{

class FieldReader
{
public:
    FieldReader(const argList& args)
    :
        runTime_(Time("controlDict", args)),
        mesh_
        (
            IOobject
            (
                fvMesh::defaultRegion,
                runTime_.name(),
                runTime_,
                IOobject::MUST_READ
            ),
            false
        )
    {}

    const fvMesh& mesh() const
    {
        return mesh_;
    }

    const Time& runTime() const
    {
        return runTime_;
    }

    volScalarField read_scalarField(const word& name) const
    {
        volScalarField ret
        (
            IOobject
            (
                name,
                runTime().name(),
                mesh(),
                IOobject::MUST_READ,
                IOobject::NO_WRITE
            ),
            mesh()
        );
        // NOTE: Cannot correct boundary conditions here because fields may have
        // field dependencies which are not read yet
        // ret.correctBoundaryConditions();
        return ret;
    }

    volVectorField read_vectorField(const word& name) const
    {
        volVectorField ret
        (
            IOobject
            (
                name,
                runTime().name(),
                mesh(),
                IOobject::MUST_READ,
                IOobject::NO_WRITE
            ),
            mesh()
        );
        // NOTE: Cannot correct boundary conditions here because fields may have
        // field dependencies which are not read yet
        // ret.correctBoundaryConditions();
        return ret;
    }

private:
    Time runTime_;
    fvMesh mesh_;
};

} // namespace Foam