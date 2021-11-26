% Plotting surfaces
function plotting(name_of_function,N,a,b)
%name_of_function  = 'Bird';
func  = str2func(name_of_function);

[x,y] = meshgrid(linspace(a,b,50));
z     = nan(size(x));

for k = 1 : numel(z),
  z(k)  = func([x(k),y(k)]);
endfor

surf(x,y,z), shading interp;
xlabel('x-axis','FontSize',18), ylabel('y-axis','FontSize',18),
zlabel('z-axis','FontSize',18), title(name_of_function,'FontSize',18);
set(gca,'LineWidth',1.5,'FontSize',18);
set(gcf,'Units','Normalized','Position',[.09 .08 .5 .8]), view(N);
