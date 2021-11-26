function [new_positions,new_fitness] = updatePositions(Na,Nd,the_function,...
                            fitness,positions,best_positions)

% Evaluate Function Block
evaluated_function     = evaluateFunction(Na,the_function,positions);

% Update best positions
condition     = evaluated_function < fitness;

new_positions  = (condition*ones(1,Nd)).*positions + ...
                (!condition*ones(1,Nd)).*best_positions;
new_fitness   = min(evaluated_function,fitness);
endfunction
