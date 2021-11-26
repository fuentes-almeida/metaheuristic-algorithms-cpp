#include <cmath>
#include "functions.hpp"

double Ackley(int d, double *x)
{
    double a=20.0, b=0.2, c=2*PI;
    double exp1=0, exp2=0;

    for (int i=0;i<d;i++)
    {
        exp1+=x[i]*x[i];
        exp2+=cos(c*x[i]);
    }
    exp1=-a*exp(-b*sqrt(exp1/d));
    exp2=-exp(exp2/d);
    double fx=exp1+exp2+a+exp(1.0);

    return fx;
}

double Griewank(int d, double *x)
{
    double sum=0, product=1.0;

    for (int i=1;i<=d;i++)
    {
        sum+=x[i-1]*x[i-1];
        product*=cos(x[i-1]/sqrt(i));
    }

    double fx=sum/4000-product+1;

    return fx;
}

double Rastrigin(int d, double *x)
{
    double sum=0;

    for (int i=0;i<d;i++)
        sum+=(x[i]*x[i]-10*cos(2*PI*x[i]));

    double fx=10*d+sum;

    return fx;
}

double Sphere(int d, double *x)
{
    double fx=0;

    for (int i=0; i<d; i++)
        fx+=x[i]*x[i];

    return fx;
}

double Rosenbrock (int d, double *x)
{
    double fx=0;

    for (int i=0;i<d-1;i++)
        fx+=100*(x[i+1]-x[i]*x[i])*(x[i+1]-x[i]*x[i])+(x[i]-1)*(x[i]-1);

    return fx;
}
