l1=0.5; l2=0.5;
q1=0.5; q2=0.5;

wy=0.05:0.05:1.0;
n=size(wy,2);
global ws=[]; %el espacio de trabajo se declara global

for wx=0.05:0.05:1
  ws=[ws; [repmat(wx,1,n); wy]'];
  end
  
%scatter(ws(:,1),ws(:,2),'b','filled')

[x2,y2]=cinemat_dir(l1,l2,q1,q2);
[q12,q22]=cinemat_inv(l1,l2,x2,y2);

det=detJ(l1,l2,q2);

x=[l1,l2];

fobj=funcionObjetivo(x);

%prueba de la funcion
%definimos los limites de busqueda para l1 y l2
xinf=[0.01,0.01];
xsup=[1.5,1.5];
npop=20;
nvar=2;

X=initialize(xinf,xsup,npop,nvar);
F=evaluate("funcionObjetivo",X);

[ix,ibest]=selection(F);
xbest=X(ibest,:);
fbest=F(ibest);

[means,sds]=estimation(X(ix,:));
X=[xbest;sampling(means,sds,npop-1)];

scatter(X(:,1),X(:,2),10,'b','filled')
