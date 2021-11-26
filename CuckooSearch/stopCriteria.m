function [condition,counter] = stopCriteria(bestFitness,bestPosition,Fitness,...
                Positions,counter,eps1,eps2,nSaturation)

% First criterion : | bestFitness - mu_Fitness | <= eps1 * sigma_Fitness
mu_Fitness    = mean(Fitness);
sigma_Fitness = std(Fitness);
conditions(1) = abs(bestFitness - mu_Fitness) <= eps1*sigma_Fitness;

% Second criterion: || (bestPosition - Positions)^2 ||_inf^0.5 <= eps2
dPositions    = ones(size(Positions,1),1)*bestPosition - Positions;
conditions(2) = sqrt(norm(dPositions.^2,inf)) <= eps2;

% Convergence criterion
condition     = 0;
if sum(conditions) != 0,
  if ++counter >= nSaturation,
    condition   = 1;
  endif
else
  counter     = 0;
endif
