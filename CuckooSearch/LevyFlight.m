function newNests = LevyFlight(Nests,Delta,Beta,BestNest)

% Read size
sNests   = size(Nests);

% Calculate std dev for N(0,Sigma) (Mantegna's algorithm)
Sigma       = (gamma(1 + Beta)*sin(pi*Beta/2)/(gamma((1 + Beta)/2)*Beta*...
    2^((Beta - 1)/2)))^(1/Beta);

% Obtain Levy random variable: Lambda
Lambda      = (randn(sNests)*Sigma)./(abs(randn(sNests)).^(1/Beta));

% Update Nests' positions
newNests = Nests + Delta*randn(sNests).*Lambda.*(Nests - ones(sNests(1),1)*BestNest);
endfunction
