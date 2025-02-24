#define CATCH_CONFIG_ENABLE_BENCHMARKING
#define CATCH_CONFIG_RUNNER
#include "args.H"
#include "catch.H"
#include <iostream>

Foam::argList* argsPtr = nullptr;

int main(int argc, char *argv[]){


    Catch::Session session;


    // Add the command line argument to switch between parallel mode
    // Build a new parser on top of Catch2's
    bool parallelRun = false;




    using namespace Catch::clara;
    auto cli
    = session.cli()                     // Get Catch2's command line parser
    | Opt( parallelRun )                // bind variable to a new option, with
                                        // a hint string the option
        ["-p"]["--parallel"]            // names it will respond to
        ("parallel run");                // description string for the help


    // Now pass the new composite back to Catch2 so it uses that
    session.cli( cli );


    // Let Catch2 (using Clara) parse the command line
    int returnCode = session.applyCommandLine( argc, argv );
    if( returnCode != 0 ) // Indicates a command line error
        return returnCode;



    // Create OpenFOAM arguments
    // Has to be done here for MPI support

    int argcOF = 1;
    if (parallelRun)
    {
        argcOF++;
    }



    //char **argvOF = static_cast<char**>(malloc(sizeof(char*)));
    char*  argvOF[argcOF];
    char executable[] = "parallelTest";
    char flags[] = "-parallel";


    argvOF[0] = executable;
    if (parallelRun)
    {
        argvOF[1] = flags;
    }

    setFoamArgs(argcOF, argvOF);

    Foam::argList& arguments = getFoamArgs();
    if (!arguments.checkRootCase())
    {
        Foam::FatalError.exit();
    }

    // Start the session
    const int result = session.run();

    // Clean up
    clearFoamArgs();


    return result;

}