%el espacio de trabajo ws es global, l1=x(1) y l2=x(2)
function [fobj]=funcionObjetivo(x)
global ws;

l1=x(1); l2=x(2);

nws=size(ws,1);
reached=0;

for i=1:nws
  wx=ws(i,2);
  wy=ws(i,1);
  [q1,q2]=cinemat_inv(l1,l2,wx,wy);
  if (isreal(q1) && isreal(q2) && detJ(l1,l2,q2)>0.2)
    reached=reached+1;
  end
end
wsr=reached/nws;
fobj=wsr/(l1+l2+1);