// pEqn.H --- Pressure correction with SIMPLE

// A = UEqn.A(), H = UEqn.H()
volScalarField rAU(1.0/UEqn.A());
volVectorField HbyA(constrainHbyA(rAU*UEqn.H(), U, p));

surfaceScalarField phiHbyA
(
    IOobject("phiHbyA", runTime.timeName(), mesh,
             IOobject::NO_READ, IOobject::NO_WRITE),
    fvc::flux(HbyA)
);

adjustPhi(phiHbyA, U, p);

//const int nNonOrthCorr = simple.nNonOrthCorr();
int nNonOrthCorr = 2;

// 非正交校正（一般 0~2 次）
for (int corr=0; corr<=nNonOrthCorr; ++corr)
{
    fvScalarMatrix pEqn
    (
        fvm::laplacian(rAU, p) == fvc::div(phiHbyA)
    );

    pEqn.solve();

    if (corr == nNonOrthCorr)
    {
        phi = phiHbyA - pEqn.flux();        // 通量一致性，關鍵
    }
}

// 壓力 under-relax（SIMPLE 經典步驟）
p.relax();

// ∇·U = 0
U = HbyA - rAU*fvc::grad(p);
U.correctBoundaryConditions();
rhoPhi = fvc::interpolate(rho) * phi;

#include "continuityErrs.H"