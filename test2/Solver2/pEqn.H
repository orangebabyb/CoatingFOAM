// pEqn.H --- Pressure correction with PIMPLE

volScalarField rAU(1.0/UEqn.A());
volVectorField HbyA(constrainHbyA(rAU*UEqn.H(), U, p));

surfaceScalarField phiHbyA
(
    IOobject("phiHbyA", runTime.timeName(), mesh,
             IOobject::NO_READ, IOobject::NO_WRITE),
    fvc::flux(HbyA)
);

adjustPhi(phiHbyA, U, p);

//const dimensionedScalar oneOverA("oneOverA", rAU.dimensions(), 1.0);

while (pimple.correctNonOrthogonal())
{
    fvScalarMatrix pEqn
    (
        //fvm::laplacian(oneOverA, p) == fvc::div(phi)
        fvm::laplacian(rAU, p) == fvc::div(phiHbyA)
    );

    pEqn.solve();

    if (pimple.finalNonOrthogonalIter())
    {
        phi = phiHbyA - pEqn.flux();
    }
}

// ∇·U = 0
U = HbyA - rAU*fvc::grad(p);
U.correctBoundaryConditions();
rhoPhi = fvc::interpolate(rho) * phi;
