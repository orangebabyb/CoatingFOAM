// UEqn.H --- Navier-Stokes equation

// explicit psi
psi = - sqr(epsilon) * fvc::laplacian(fi) + (sqr(fi) - scalar(1.0))*fi;
// tmp<volScalarField> tPsi =
//   - sqr(epsilon) * fvc::laplacian(fi)
//   + (sqr(fi) - scalar(1.0)) * fi;

//Fst, Fg
Fst = (lambda/sqr(epsilon)) * psi * fvc::grad(fi);
Fg = rho*g;

// Eqn
//phi *= fvc::interpolate(rho);
fvVectorMatrix UEqn
(
    fvm::ddt(rho, U)      
  + fvm::div(rhoPhi, U)    
  - fvm::laplacian(mu, U) 
);

UEqn.relax();
solve(UEqn == (- fvc::grad(p) + Fg + Fst)); 