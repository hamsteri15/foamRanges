#pragma once

#include "argList.H"


extern Foam::argList* argsPtr;

inline Foam::argList& getFoamArgs()
{
    return *argsPtr;
}

inline void setFoamArgs(int argc, char* argv[])
{
    argsPtr = new Foam::argList(argc,argv);
}

inline void clearFoamArgs()
{
    delete argsPtr;
}

