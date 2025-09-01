Info<< "Reading transportProperties\n" << endl;
IOdictionary transportProperties
(
    IOobject
    (
        "transportProperties",
        runTime.constant(),
        mesh,
        IOobject::MUST_READ_IF_MODIFIED,
        IOobject::NO_WRITE
    )
);

//TODO basic field
Info<< "Reading field p\n" << endl;
volScalarField p
(
    IOobject("p", runTime.timeName(), mesh,
    IOobject::MUST_READ, IOobject::AUTO_WRITE),
    mesh
);

Info<< "Reading field U\n" << endl;
volVectorField U
(
    IOobject("U", runTime.timeName(), mesh,
    IOobject::MUST_READ, IOobject::AUTO_WRITE),
    mesh
);

Info<< "Reading field fi (phase field)\n" << endl;
volScalarField fi
(
    IOobject("fi", runTime.timeName(), mesh,
    IOobject::MUST_READ, IOobject::AUTO_WRITE),
    mesh
);

// Chemical potential ψ
volScalarField psi
(
    IOobject("psi", runTime.timeName(), mesh,
    IOobject::READ_IF_PRESENT, IOobject::AUTO_WRITE),
    mesh,
    dimensionedScalar("zero", fi.dimensions(), 0.0)
);

//TODO Transport properties
// coating parameters
dimensionedScalar rho1(transportProperties.lookup("rho1"));
dimensionedScalar rho2(transportProperties.lookup("rho2"));
dimensionedScalar mu1 (transportProperties.lookup("mu1"));
dimensionedScalar mu2 (transportProperties.lookup("mu2"));

// Cahn–Hilliard
dimensionedScalar epsilon(transportProperties.lookup("epsilon"));
dimensionedScalar sigma  (transportProperties.lookup("sigma"));
dimensionedScalar chi    (transportProperties.lookup("chi"));

//gravity
dimensionedVector g
(
    "g", 
    dimensionSet(0,1,-2,0,0,0,0), 
    vector(0, -9.81, 0)
);

// λ = 3εσ/√8
dimensionedScalar lambda
(
    "lambda",
    (3.0/(2.0*Foam::sqrt(scalar(2.0))))* sigma * epsilon
);

dimensionedScalar M
(
    "M",
    chi * lambda
);

//TODO Fluid fractions
volScalarField Vf1
(
    IOobject("Vf1", runTime.timeName(), mesh,
    IOobject::NO_READ, IOobject::AUTO_WRITE),
    0.5*(scalar(1.0) - fi)
);

volScalarField Vf2
(
    IOobject("Vf2", runTime.timeName(), mesh,
    IOobject::NO_READ, IOobject::AUTO_WRITE),
    0.5*(scalar(1.0) + fi)
);

//TODO Rho & mu
// Heaviside
volScalarField H_rho
(
    IOobject("H_rho", runTime.timeName(), mesh,
    IOobject::NO_READ, IOobject::NO_WRITE),
    pos((Vf2 - scalar(0.5))/scalar(0.9))
);

volScalarField H_mu
(
    IOobject("H_mu", runTime.timeName(), mesh,
    IOobject::NO_READ, IOobject::NO_WRITE),
    pos((Vf2 - scalar(0.5))/scalar(0.9))
);

//rho
volScalarField rho
(
    IOobject("rho", runTime.timeName(), mesh,
    IOobject::READ_IF_PRESENT, IOobject::AUTO_WRITE),
    rho1 + (rho2 - rho1) * H_rho
);

//mu
volScalarField mu
(
    IOobject("mu", runTime.timeName(), mesh,
    IOobject::READ_IF_PRESENT, IOobject::AUTO_WRITE),
    mu1 + (mu2 - mu1) * H_mu
);

//TODO Flux & forces
// volumetric flux (m^3/s):
surfaceScalarField phi
(
    IOobject("phi", runTime.timeName(), mesh,
    IOobject::READ_IF_PRESENT, IOobject::NO_WRITE),
    fvc::flux(U)
);

// mass flux (kg/s):
surfaceScalarField rhoPhi
(
    IOobject("rhoPhi", runTime.timeName(), mesh,
             IOobject::READ_IF_PRESENT, IOobject::NO_WRITE),
    fvc::interpolate(rho) * phi
);

volVectorField Fst
(
    IOobject("Fst", runTime.timeName(), mesh,
    IOobject::READ_IF_PRESENT, IOobject::AUTO_WRITE),
    mesh,
    dimensionedVector("zero", dimensionSet(1,-2,-2,0,0,0,0), vector::zero)
);

volVectorField Fg
(
    IOobject("Fg", runTime.timeName(), mesh,
    IOobject::READ_IF_PRESENT, IOobject::AUTO_WRITE),
    mesh,
    dimensionedVector("zero", dimensionSet(1,-2,-2,0,0,0,0), vector::zero)
);