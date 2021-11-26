function updated_Nests = randomlyDiscoveringEggs(Na,Nd,pD,Nest,fitness)

updated_Nests = Nest + rand(Na,Nd).*(Nest(randperm(Na),:) - ...
                  Nest(randperm(Na),:)).*double(rand(Na,Nd) > pD);
endfunction
