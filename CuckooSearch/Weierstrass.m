function fObj = Weierstrass(X)

a     = 0.5;
b     = 3;
n     = numel(X);
kmax  = 20;

[x,k] = meshgrid(X,0:kmax);

fObj  = sum(sum((a.^k).*cos(2*pi*(b.^k).*(x + 0.5))) - n*sum((a.^k).*cos(pi*b.^k)));

endfunction
