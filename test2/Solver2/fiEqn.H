// fiEqn.H --- Cahn–Hilliard

// explicit psi
psi = - sqr(epsilon) * fvc::laplacian(fi) + (sqr(fi) - scalar(1.0))*fi;
// tmp<volScalarField> tPsi_fi =
//     - sqr(epsilon) * fvc::laplacian(fi)
//   + (sqr(fi) - scalar(1.0)) * fi;

// chi * lambda
dimensionedScalar M = chi * lambda;

// fi equation
fvScalarMatrix fiEqn
(
    fvm::ddt(fi)
  + fvm::div(phi, fi)
 ==
    //fvc::div( M * fvc::grad(tPsi_fi()))
    fvc::laplacian(M, psi)
  + fvModels.source(fi)
);

fiEqn.relax();
fiEqn.solve();