function updated_Nests = proportionallyDiscoveringEggs(Na,Nd,pa,Nest,fitness)

sort_Id     = sort(fitness);
updated_Nests = Nest + rand(Na,Nd).*(Nest(randperm(Na),:) - ...
                  Nest(randperm(Na),:)).*repmat(sort_Id > pa*Na,1,Nd);

endfunction
