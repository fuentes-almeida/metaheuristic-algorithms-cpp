%UMDA
%Generacion del espacio de trabajo
wy=0.05:0.05:1.0;
n=size(wy,2);
global ws=[]; %el espacio de trabajo se declara global
for wx= 0.05:0.05:1
 ws=[ws; [repmat(wx,1,n); wy]'];
end

cla
%definimos los limites de busqueda para l1 y l2
xinf=[0.01 0.01];
xsup=[1.5 1.5];
npop=20;
nvar=2; %2 variables a optimizar
X=initialize(xinf, xsup,npop, nvar);
F=evaluate("funcionObjetivo",X);
[ix,ibest]=selection(F);
xbest=X(ibest,:);
fbest=F(ibest);
sds=[1000 1000];
scatter(X(:,1),X(:,2),10,F,'filled');
axis([0 2 0 2]);
while(norm(sds)>1e-2)
	[means,sds]=estimation(X(ix,:));
	X=[xbest; sampling(means,sds,npop)];
	F=[fbest; evaluate("funcionObjetivo",X(2:npop,:))];
	[ix,ibest]=selection(F);
	xbest=X(ibest,:);
	fbest=F(ibest);
	disp([xbest fbest]);
	fflush(stdout);
	cla
	scatter(X(:,1),X(:,2),10,F,'filled');
	drawnow()
end

