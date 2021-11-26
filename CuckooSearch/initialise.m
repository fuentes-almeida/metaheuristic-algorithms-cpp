function [Nd,bnd_1,bnd_2,positions] = initialise(bnd,Na)

% Read problem's dimensions
Nd          = size(bnd,1);

% Sort and make-up the boundaries
bnd         = [min(bnd,[],2) max(bnd,[],2)];
bnd_1       = ones(Na,1)*bnd(:,1)';
bnd_2       = ones(Na,1)*bnd(:,2)';

% Initialise nests
positions   = bnd_1 + rand(Na,Nd).*(bnd_2 - bnd_1);

endfunction
