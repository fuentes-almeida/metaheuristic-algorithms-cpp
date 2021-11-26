function [q1,q2]=cinemat_inv(l1,l2,x2,y2)
  x0=-y2;
  y0=x2;  
  q2=acos((x0.*x0+y0.*y0-l1*l1-l2*l2)/(2.0*l1*l2));
  if (isreal(q2))
    q1=atan2(y0,x0)-atan2((l2*sin(q2)),(l1+l2*cos(q2)));
  else
    q1=1i;
  end  
end
  
  
  
