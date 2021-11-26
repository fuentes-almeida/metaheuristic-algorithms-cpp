function fObj = Rosenbrock(x)

fObj = 0;
for i = 1:(length(x)-1)
	xi = x(i);
	xnext = x(i+1);
	fObj = fObj + 100*(xnext-xi^2)^2 + (xi-1)^2;
end

endfunction