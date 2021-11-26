#ifndef VEC_HPP_INCLUDED
#define VEC_HPP_INCLUDED
#define dim 30

using namespace std;

double VectorNorm(int d, double *v);
double SquareDistance(int d, double *v1, double *v2);
void VectorScalarProduct(int d, double *v, double scalar, double *result);
void VectorAdd(int d, double *v1, double *v2, double *result);
void VectorSubstract(int d, double *v1, double *v2, double *result);
void CopyIndividual(int d, double *src, double *dst);
int Compare (const void * a, const void * b);

#endif // VEC_HPP_INCLUDED
