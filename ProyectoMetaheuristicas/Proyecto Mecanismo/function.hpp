#ifndef FUNCTION_HPP_INCLUDED
#define FUNCTION_HPP_INCLUDED

typedef struct typetarget{
    double x[6],y[6];
}target;

typedef struct typepoint{
    double x1,y1,theta3_1,x2,y2,theta3_2;
    int flag;
}point;

typedef struct MECHA{
    double r1,r2,r3,r4,rcx,rcy,x0,y0,theta0;
    double *input;
    double result1, result2;
}mechanism;

point ComputeCxCy(point, mechanism, double);
point ComputePrecisionPoint(mechanism, double);

void RunSimulation(mechanism,target,int);

#endif // FUNCTION_HPP_INCLUDED
