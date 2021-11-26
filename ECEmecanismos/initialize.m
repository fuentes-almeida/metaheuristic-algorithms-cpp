%initialize.m
function [X]=initialize(xinf,xsup,npop,nvar)
  X=[];
  for i=1:nvar
    X=[X,unifrnd(xinf(i),xsup(i),npop,1)];
  end
end