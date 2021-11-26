function evaluated_function = evaluateFunction(Na,the_function,points)

% Preallocate the evaluated function vector
evaluated_function   = nan(Na,1);

% Evaluate each nest
    for s = 1 : Na,
        evaluated_function(s) = the_function(points(s,:));
    endfor
endfunction
