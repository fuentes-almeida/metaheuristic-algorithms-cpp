function fObj = SpringDesign(x)

% Design variables:
% x(1)  - Wire diameter
% x(2)  - Mean coil diameter
% x(3)  - Number of active coils

% Adjust to integer variables
% --
x(3)    = fix(x(3));

% Optimisation problem:
% --
weight  = (x(3) + 2)*x(2)*x(1)^2;

% Constraits (g(i) <= 0)
constraints  = [ 1 - (x(2)^3*x(3))/(71785*x(1)^4);
  (4*x(2)^2 - x(1)*x(2))/(12566*(x(2)*x(1)^3 - x(1)^4)) + 1/(5108*x(1)^2) - 1;
  1 - 140.45*x(1)/(x(2)^2*x(3));
  (x(1)+x(2))/1.5 - 1];

% Penalty function
fObj    = weight + 10*sum(max(constraints,0));
