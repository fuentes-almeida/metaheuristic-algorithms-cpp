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

double Elliptic(int d, double *x)
{
    double a=1e6, fx=0;

    for (int i=0;i<d;i++)
        fx+=x[i]*x[i]*pow(a,(double)(i)/(d-1));

    return fx;
}

double Schwefel(int d, double *x)
{
    double sum, fx=0;
    for (int i=0;i<d;i++)
    {
        sum=0;
        for (int j=0; j<i; j++)
            sum+=x[j];
        fx+=sum*sum;
    }

    return fx;
}

double Weierstrass(int d, double *x)
{
    double a=0.5, b=3.0;
    int kmax=20;
    double sum1=0,sum2=0;

    for (int k=0;k<=kmax;k++)
        sum1+=pow(a,k)*cos(PI*pow(b,k));

    for (int i=0;i<d;i++)
        for (int k=0;k<=kmax;k++)
            sum2+=pow(a,k)*cos(2*PI*pow(b,k)*(x[i]+0.5));

    double fx=sum2-d*sum1;

    return fx;
}

double SchafferFx(double x, double y)
{
    double fx=0.5+(pow(sin(sqrt(x*x+y*y)),2)-0.5)/pow(1+0.001*(x*x+y*y),2);

    return fx;
}

double Schaffer(int d, double *x)
{
    double fx=0;

    for (int i=0;i<d;i++)
        fx+=SchafferFx(x[i],x[(i+1)%d]);

    return fx;
}

double Norm(int d, double *x)
{
    double fx=0;

    for (int i=0;i<d;i++)
        fx+=x[i]*x[i];

    return sqrt(fx);
}

double Salomon(int d, double *x)
{
    double normx=Norm(d,x);

    double fx=1-cos(2*PI*normx)+0.1*normx;

    return fx;
}






