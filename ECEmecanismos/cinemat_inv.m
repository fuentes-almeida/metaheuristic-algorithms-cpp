%cinemat_inv.m
%Funcion para el calculo de la cinematica inversa
function [q1,q2]=cinemat_inv (l1,l2,x2,y2)
  q2=acos(((-y2)^2+x2^2-l1^2-l2^2)/(2*l1*l2));
  if (isreal(q2))
    q1=atan2(x2,-y2)-atan2(l2*sin(q2),l1+l2*cos(q2));
  else
    q1=i;
end