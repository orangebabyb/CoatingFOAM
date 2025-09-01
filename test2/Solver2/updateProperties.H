// updatePropertise.H -- use the newest to update rho, mu

// compute VF1, VF2
//Vf1 = 0.5*(scalar(1.0) - fi);
//Vf2 = 0.5*(scalar(1.0) + fi);
Vf2 = min( max( 0.5*(scalar(1.0) + fi), scalar(0) ), scalar(1) );
//Vf1 = scalar(1) - Vf2;

//Heaviside
H_rho = pos((Vf2 - scalar(0.5))/scalar(0.9));
H_mu  = H_rho;

//update rho, mu
rho = rho1 + (rho2 - rho1) * H_rho;
mu  = mu1  + (mu2 - mu1 ) * H_mu;

rho.correctBoundaryConditions();
mu .correctBoundaryConditions();
Vf1.correctBoundaryConditions();
Vf2.correctBoundaryConditions();

//update 
rhoPhi = fvc::interpolate(rho) * phi;
