#pragma once

#include <filesystem>
#include "Time.H"
#include "volFields.H"
#include "fvMesh.H"
#include "args.H"
#include "fileName.H"




namespace Foam{




struct FieldReader{


    FieldReader(const argList& args) :
    runTime_(Time("controlDict", args)),
    mesh_
    (
        IOobject
        (
            Foam::fvMesh::defaultRegion,
            runTime_.name(),
            runTime_,
            Foam::IOobject::MUST_READ
        ),
        false
    )
    {}


    const fvMesh& mesh() const
    {
        return mesh_;
    }


    const Time& runTime() const {return runTime_;}


    auto read_scalarField(word name) const
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
        //NOTE! Can not correct boundary conditions here because fields may have
        //field dependencies which are not read yet
        //ret.correctBoundaryConditions();
        return ret;
    }

    auto read_vectorField(word name) const
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
        //NOTE! Can not correct boundary conditions here because fields may have
        //field dependencies which are not read yet
        //ret.correctBoundaryConditions();
        return ret;
    }

private:

    Time runTime_;
    fvMesh mesh_;
};





}

