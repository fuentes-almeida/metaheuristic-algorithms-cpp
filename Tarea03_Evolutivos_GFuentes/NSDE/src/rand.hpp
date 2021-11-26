#ifndef RAND_HPP_INCLUDED
#define RAND_HPP_INCLUDED

#include <random>

using namespace std;

double randU(void)
{
    static default_random_engine generator{};
    static uniform_real_distribution<double> distribution(0.0,1.0);

    return distribution(generator);
}

double randN(double mean, double sigma)
{
    static default_random_engine generator{};
    static normal_distribution<double> distribution(mean,sigma);

    return distribution(generator);
}

double randC(double x0, double gamma)
{
    static default_random_engine generator{};
    static cauchy_distribution<double> distribution(x0,gamma);

    return distribution(generator);
}
#endif // RAND_HPP_INCLUDED
