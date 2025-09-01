/*---------------------------------------------------------------------------*\
  Application
     phaseFieldCoatingFoam
\*---------------------------------------------------------------------------*/

#include <algorithm>
#include "fvCFD.H"
#include "pimpleControl.H"
#include "fvModels.H"
#include "fvConstraints.H"
#include <nvtx3/nvToolsExt.h>
// #include "SRFModel.H" // spin-coating
// #include "pisoControl.H"      // 用不到就先關
// #include "PstreamReduceOps.H" // 只有用 gSum/gMax 才需要>

int main(int argc, char *argv[])
{
    #include "setRootCase.H"
    #include "createTime.H"
    #include "createMesh.H"
    #include "createFields.H" 
    #include "initContinuityErrs.H"
    #include "createFvModels.H"
    #include "createFvConstraints.H"

    pimpleControl pimple(mesh);
    Info<< "\nStarting time loop\n" << endl;

    while (runTime.loop())
    {
        Info<< "Time = " << runTime.timeName() << nl << endl;

        //#include "updateProperties.H"

        while (pimple.loop())
        {
            // --- update property
            //#include "updateProperties.H"

            // --- Momentum equation
            #include "UEqn.H"

            // --- Pressure equation
            #include "pEqn.H"

            // --- Phase-field equation
            #include "fiEqn.H"
        }

        runTime.write();


        // --- Momentum  動量方程
        //#include "UEqn.H"

        // --- Pressure equation 壓力修正
        //#include "pEqn.H"

        // --- Phase-field (Cahn–Hilliard) 相場
        //#include "fiEqn.H"

        runTime.write();

        Info<< "ExecutionTime = " << runTime.elapsedCpuTime() << " s"
            << "  ClockTime = "   << runTime.elapsedClockTime() << " s"
            << nl << endl;
    }
    Info<< "End\n" << endl;
    return 0;
}
