#ifndef RAND_HPP_INCLUDED
#define RAND_HPP_INCLUDED

#include <random>

using namespace std;

double randU(double a, double b)
{
    random_device device;
    mt19937 generator(device());
    uniform_real_distribution<double> distribution(a,b);

    return distribution(generator);
}

int randI(int a, int b)
{
    random_device device;
    mt19937 generator(device());
    uniform_int_distribution<int> distribution(a,b);

    return distribution(generator);
}

double randN(double mean, double sigma)
{
    random_device device;
    mt19937 generator(device());
    normal_distribution<double> distribution(mean,sigma);

    return distribution(generator);
}

double randC(double x0, double gamma)
{
    random_device device;
    mt19937 generator(device());
    cauchy_distribution<double> distribution(x0,gamma);

    return distribution(generator);
}

#endif // RAND_HPP_INCLUDED
