%selection.m
function [ix,ibest]=selection(F)
  npop=size(F,1);
  [Ft,ix]=sort(F,'descend');
  ix=ix(1:(npop/2));
  ibest=ix(1);
end