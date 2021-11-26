#ifndef FUNCTIONS_HPP_INCLUDED
#define FUNCTIONS_HPP_INCLUDED

#define PI 3.14159265358979

double Ackley(int d, double *x);
double Griewank(int d, double *x);
double Rastrigin(int d, double *x);
double Sphere(int d, double *x);
double Rosenbrock (int d, double *x);
double Elliptic(int d, double *x);
double Schwefel(int d, double *x);
double Weierstrass(int d, double *x);
double SchafferFx(double x, double y);
double Schaffer(int d, double *x);
double Norm(int d, double *x);
double Salomon(int d, double *x);

#endif // FUNCTIONS_H_INCLUDED
