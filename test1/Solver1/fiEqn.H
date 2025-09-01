// fiEqn.H --- Cahn–Hilliard

// explicit psi
psi = - sqr(epsilon) * fvc::laplacian(fi) + (sqr(fi) - scalar(1.0))*fi;
// tmp<volScalarField> tPsi =
//     - sqr(epsilon) * fvc::laplacian(fi)
//   + (sqr(fi) - scalar(1.0)) * fi;

// chi * lambda
const dimensionedScalar M = chi * lambda;

// fi equation
fvScalarMatrix fiEqn
(
    fvm::ddt(fi)
  + fvm::div(phi, fi)
 ==
    fvc::laplacian(M, psi)
  + fvModels.source(fi)
);

//fi = min(max(fi, scalar(-1)), scalar(1));
fiEqn.relax();
fiEqn.solve();
fi = min(max(fi, scalar(-1.0)), scalar(1.0));
fi.correctBoundaryConditions();