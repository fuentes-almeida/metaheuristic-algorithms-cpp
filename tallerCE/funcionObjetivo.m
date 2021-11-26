function [fobj]=funcionObjetivo(x)
global ws; %el espacio de trabajo es global
l1=x(1); l2=x(2);
ncoords=size(ws)(1);
reached=0;
for i=1:ncoords
    wy=ws(i,2);
    wx=ws(i,1);
    [q1,q2]=cinemat_inv(l1,l2,wx,wy);
    if (isreal(q1) && isreal(q2) && determinante(l1,l2,q2)>0.2)
       reached=reached+1;
    end
wsr=reached/ncoords;    
fobj=wsr/(l1+l2+1);
end

