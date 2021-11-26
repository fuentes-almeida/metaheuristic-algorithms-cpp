#include "shell.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "function.hpp"
#define PI 3.141592
#define screen_size 600

point ComputeCxCy(point C, mechanism mecha, double theta2)
{
    double Cxr=mecha.r2*cos(theta2)+mecha.rcx*cos(C.theta3_1)-mecha.rcy*sin(C.theta3_1);
    double Cyr=mecha.r2*sin(theta2)+mecha.rcx*sin(C.theta3_1)+mecha.rcy*cos(C.theta3_1);

    C.x1= (Cxr-mecha.x0)*cos(mecha.theta0)+(Cyr-mecha.y0)*sin(mecha.theta0);
    C.y1=-(Cxr-mecha.x0)*sin(mecha.theta0)+(Cyr-mecha.y0)*cos(mecha.theta0);

    Cxr=mecha.r2*cos(theta2)+mecha.rcx*cos(C.theta3_2)-mecha.rcy*sin(C.theta3_2);
    Cyr=mecha.r2*sin(theta2)+mecha.rcx*sin(C.theta3_2)+mecha.rcy*cos(C.theta3_2);

    C.x2= (Cxr-mecha.x0)*cos(mecha.theta0)+(Cyr-mecha.y0)*sin(mecha.theta0);
    C.y2=-(Cxr-mecha.x0)*sin(mecha.theta0)+(Cyr-mecha.y0)*cos(mecha.theta0);

    return C;
}

point ComputePrecisionPoint(mechanism mecha, double theta2)
{
    point C;

    double k1=mecha.r1/mecha.r2;
    double k2=mecha.r1/mecha.r3;
    double k3=(pow(mecha.r4,2)-pow(mecha.r1,2)-pow(mecha.r2,2)-pow(mecha.r3,2))/(2*mecha.r2*mecha.r3);

    double a=k3+(k2+1)*cos(theta2)-k1;
    double b=-2*sin(theta2);
    double c=k1+(k2-1)*cos(theta2)+k3;

    C.theta3_1=2*atan2(-b+sqrt(b*b-4*a*c),2*a);
    C.theta3_2=2*atan2(-b-sqrt(b*b-4*a*c),2*a);

    return C;
}

void RunSimulation(mechanism mecha, target T,int n)
{
    double theta3;
    double infX=mecha.x0;
    double infY=mecha.y0;

    Window W("Mecanismo", 600,600);

    int nobj=16;
    DOBJ objs[nobj];

    objs[0].type=POLYGON;
    objs[0].ncoord=3;
    objs[0].lwd=2; //Ancho de linea
    objs[0].FillCol=BLUE2;
    objs[0].BorderCol=BLACK;

    objs[1].type=LINE;
    objs[1].ncoord=2;
    objs[1].lwd=2; //Ancho de linea
    objs[1].BorderCol=BLACK;

    objs[2].type=LINE;
    objs[2].ncoord=2;
    objs[2].lwd=2; //Ancho de linea
    objs[2].BorderCol=BLACK;

    objs[4].type=LINE;
    objs[4].ncoord=2;
    objs[4].lwd=2; //Ancho de linea
    objs[4].BorderCol=BLACK;

    objs[5].type=CIRCLE;
    objs[5].ncoord=2;
    objs[5].lwd=1;
    objs[5].FillCol=YELLOW;
    objs[5].BorderCol=BLACK;
    objs[5].coordX[0]=T.x[0];
    objs[5].coordY[0]=T.y[0];
    objs[5].radius=3;

    objs[6].type=CIRCLE;
    objs[6].ncoord=2;
    objs[6].lwd=1;
    objs[6].FillCol=YELLOW;
    objs[6].BorderCol=BLACK;
    objs[6].coordX[0]=T.x[1];
    objs[6].coordY[0]=T.y[1];
    objs[6].radius=3;

    objs[7].type=CIRCLE;
    objs[7].ncoord=2;
    objs[7].lwd=1;
    objs[7].FillCol=YELLOW;
    objs[7].BorderCol=BLACK;
    objs[7].coordX[0]=T.x[2];
    objs[7].coordY[0]=T.y[2];
    objs[7].radius=3;

    objs[8].type=CIRCLE;
    objs[8].ncoord=2;
    objs[8].lwd=1;
    objs[8].FillCol=YELLOW;
    objs[8].BorderCol=BLACK;
    objs[8].coordX[0]=T.x[3];
    objs[8].coordY[0]=T.y[3];
    objs[8].radius=3;

    objs[9].type=CIRCLE;
    objs[9].ncoord=2;
    objs[9].lwd=1;
    objs[9].FillCol=YELLOW;
    objs[9].BorderCol=BLACK;
    objs[9].coordX[0]=T.x[4];
    objs[9].coordY[0]=T.y[4];
    objs[9].radius=3;

    objs[10].type=CIRCLE;
    objs[10].ncoord=2;
    objs[10].lwd=1;
    objs[10].FillCol=YELLOW;
    objs[10].BorderCol=BLACK;
    objs[10].coordX[0]=T.x[5];
    objs[10].coordY[0]=T.y[5];
    objs[10].radius=3;

    objs[11].type=CIRCLE;
    objs[11].ncoord=2;
    objs[11].lwd=1;
    objs[11].FillCol=BLUE;
    objs[11].BorderCol=BLUE;
    objs[11].radius=3;

    objs[12].type=CIRCLE;
    objs[12].ncoord=2;
    objs[12].lwd=1;
    objs[12].FillCol=BLUE;
    objs[12].BorderCol=BLUE;
    objs[12].radius=3;

    objs[13].type=CIRCLE;
    objs[13].ncoord=2;
    objs[13].lwd=1;
    objs[13].FillCol=BLUE;
    objs[13].BorderCol=BLUE;
    objs[13].radius=3;

    objs[14].type=CIRCLE;
    objs[14].ncoord=2;
    objs[14].lwd=1;
    objs[14].FillCol=BLUE;
    objs[14].BorderCol=BLUE;
    objs[14].radius=3;

    objs[15].type=CIRCLE;
    objs[15].ncoord=2;
    objs[15].lwd=1;
    objs[15].FillCol=BLUE;
    objs[15].BorderCol=BLUE;
    objs[15].radius=3;

  for (double theta2=mecha.input[0]; theta2<=mecha.input[n-1]; theta2+=0.02)
    {
        point C=ComputePrecisionPoint(mecha,theta2);
        if (mecha.result1<mecha.result2)
            theta3 = C.theta3_2;
        else
            theta3 = C.theta3_1;

      W.Cls(); //Limpia la pantalla

      objs[1].coordX[0]=infX;
      objs[1].coordY[0]=infY;
      objs[1].coordX[1]=objs[1].coordX[0]+mecha.r1*cos(mecha.theta0);
      objs[1].coordY[1]=objs[1].coordY[0]-mecha.r1*sin(mecha.theta0);

      objs[2].coordX[0]=objs[1].coordX[0];
      objs[2].coordY[0]=objs[1].coordY[0];
      objs[2].coordX[1]=objs[2].coordX[0]+mecha.r2*cos(theta2+mecha.theta0);
      objs[2].coordY[1]=objs[2].coordY[0]-mecha.r2*sin(theta2+mecha.theta0);

      objs[0].coordX[0]=objs[2].coordX[1];
      objs[0].coordY[0]=objs[2].coordY[1];
      objs[0].coordX[1]=objs[0].coordX[0]+mecha.r3*cos(theta3+mecha.theta0);
      objs[0].coordY[1]=objs[0].coordY[0]-mecha.r3*sin(theta3+mecha.theta0);
      objs[0].coordX[2]=objs[0].coordX[0]+mecha.rcx*cos(theta3+mecha.theta0)-mecha.rcy*sin(theta3+mecha.theta0);
      objs[0].coordY[2]=objs[0].coordY[0]-mecha.rcx*sin(theta3+mecha.theta0)-mecha.rcy*cos(theta3+mecha.theta0);

      objs[4].coordX[0]=objs[1].coordX[1];
      objs[4].coordY[0]=objs[1].coordY[1];
      objs[4].coordX[1]=objs[0].coordX[1];
      objs[4].coordY[1]=objs[0].coordY[1];

      objs[11].coordX[0]=infX;
      objs[11].coordY[0]=infY;
      objs[12].coordX[0]=objs[2].coordX[1];
      objs[12].coordY[0]=objs[2].coordY[1];
      objs[13].coordX[0]=objs[0].coordX[2];
      objs[13].coordY[0]=objs[0].coordY[2];
      objs[14].coordX[0]=objs[0].coordX[1];
      objs[14].coordY[0]=objs[0].coordY[1];
      objs[15].coordX[0]=objs[1].coordX[1];
      objs[15].coordY[0]=objs[1].coordY[1];

      W.DrawObjects(nobj,objs);
      W.Paint();
    }
}
