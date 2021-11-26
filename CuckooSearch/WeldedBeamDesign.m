function [fObj,cost,constraints] = WeldedBeamDesign (x)

% Design variables:
% x(1)  - Thickness of the weld
% x(2)  - Length of the welded joint
% x(3)  - Width of the weld
% x(4)  - Thickness of the beam

% Design specifications:
% --
P         = 6000;     % in lb
L         = 14;       % in in
E         = 30e6;     % in psi
G         = 12e6;     % in psi
Tau_max   = 13600;    % in psi
Sigma_max = 30000;    % in psi
Delta_max = 0.25;     % in in

% Adjust to integer variables
% --
x(1)  = fix(x(1))*0.0065;
x(2)  = fix(x(2))*0.0065;

% Some parameters:
% --
M     = P*(L + x(2)/2);
R     = sqrt(x(2)^2/4 + ((x(1) + x(3))/2)^2);
J     = 2*(sqrt(2)*x(1)*x(2)*(x(2)^2/12 + ((x(1) + x(3))/2)^2));

Tau_1 = P/(sqrt(2)*x(1)*x(2));
Tau_2 = M*R/J;

% Physical magnitudes:
% --
% Tau   - Shear stress in the beam
Tau   = sqrt(Tau_1^2 + 2*Tau_1*Tau_2^(x(2)/2/R) + Tau_2^2);

% Sigma - Bending stress in the beam
Sigma = 6*P*L/(x(4)*x(3)^2);

% Pc    - Buckling load on the bar
Pc    = (4.013*E/L^2)*sqrt((x(3)^2*x(4)^6)/36)*(1 - (x(3)/(2*L))*sqrt(E/4/G));

% Delta - Deflection of the beam
Delta = 4*P*L^3/(E*x(3)^3*x(4));

% Optimisation problem:
% --
% Overall fabrication cost of the welded beam
cost  =  1.10471*x(1)^2*x(2) + 0.04811*x(3)*x(4)*(14 + x(2));

% Constraints (g(i) <= 0)
constraints     = [ Tau - Tau_max;
                    Sigma - Sigma_max;
                    x(1) - x(4);
                    0.10471*x(1)^2 + 0.04811*x(3)*x(4)*(14 + x(2)) - 5;
                    0.125 - x(1);
                    Delta - Delta_max;
                    P - Pc];

% Penalty function
fObj  = cost + 10*sum(max(constraints,0));
