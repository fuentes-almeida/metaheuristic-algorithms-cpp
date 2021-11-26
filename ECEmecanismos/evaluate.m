%evaluate.m
function [F]=evaluate(f,X)
  npop=size(X,1);
  F=zeros(npop,1);
  for i=1:npop
    F(i)=feval(f,X(i,:));
  end
end