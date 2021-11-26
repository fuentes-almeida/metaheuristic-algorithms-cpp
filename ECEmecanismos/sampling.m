%sampling.m
function [X]=sampling(means,sds,npop)
  nvar=length(means);
  X=[];
  for i:nvar
    X=[X,normrnd(means(i),sds(i),npop-1,1)];
  end
end