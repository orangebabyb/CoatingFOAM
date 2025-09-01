// UEqn.H --- Navier-Stokes equation

//update psi
psi = - sqr(epsilon) * fvc::laplacian(fi) + (sqr(fi) - scalar(1.0))*fi;

//Fst, Fg
Fst = (lambda/sqr(epsilon)) * psi * fvc::grad(fi);
Fst.correctBoundaryConditions();

Fg = rho*g;

// Eqn
//phi *= fvc::interpolate(rho);
fvVectorMatrix UEqn
(
      fvm::ddt(rho, U)      
    + fvm::div(rhoPhi, U)    
    - fvm::laplacian(mu, U)
  ==
      Fst
    + Fg
);

UEqn.relax();
solve(UEqn == -fvc::grad(p));