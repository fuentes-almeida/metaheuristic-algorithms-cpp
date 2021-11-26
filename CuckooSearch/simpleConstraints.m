function Nests = simpleConstraints(Nests,lowerBoundaries,upperBoundaries)

% Check if solutions are inside the search space (feasible region)
check = Nests < lowerBoundaries;
Nests  = ~check.*Nests + check.*lowerBoundaries;

check = Nests > upperBoundaries;
Nests  = ~check.*Nests + check.*upperBoundaries;

endfunction
