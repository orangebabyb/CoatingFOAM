/*---------------------------------------------------------------------------*\
  Application
     phaseFieldCoatingFoam
\*---------------------------------------------------------------------------*/

#include <algorithm>
#include "fvCFD.H"
//#include "pimpleControl.H"
#include "fvModels.H"
#include "fvConstraints.H"
#include <nvtx3/nvToolsExt.h>
#include "simpleControl.H"
// #include "SRFModel.H" // spin-coating
// #include "pisoControl.H"      // 用不到就先關
// #include "PstreamReduceOps.H" // 只有用 gSum/gMax 才需要>

int main(int argc, char *argv[])
{
    #include "setRootCase.H"
    #include "createTime.H"
    #include "createMesh.H"
    #include "createControl.H"
    //#include "createTimeControls.H"
    #include "createFields.H" 
    #include "initContinuityErrs.H"
    #include "createFvModels.H"
    #include "createFvConstraints.H"

    //pimpleControl pimple(mesh);
    Info<< "\nStarting time loop\n" << endl;

    while (simple.loop(runTime))
    {
        Info<< "Time = " << runTime.userTimeName() << nl << endl;

        // Solve fi, U, p
        {
            // --- Phase-field equation
            //#include "fiEqn.H"

            // --- update property
            #include "updateProperties.H"

            // --- Momentum equation
            #include "UEqn.H"
            
            // --- pressure equation 
            #include "pEqn.H"

            // --- Phase-field equation
            #include "fiEqn.H"
        }

        runTime.write();

        Info<< "ExecutionTime = " << runTime.elapsedCpuTime() << " s"
            << "  ClockTime = " << runTime.elapsedClockTime() << " s"
            << nl << endl;
    }
    
    Info<< "End\n" << endl;
    return 0;
}
