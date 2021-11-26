
function [fobj]=plot_sim(l1,l2)

cla
x0=0;
y0=0;
%  l1=0.5;
%  l2=0.5;
%  l1=0.4;
%  q1=0.35; 
%  q2=0.6
%  l2=0.35

%  wx=0.05:0.05:1;
wy=0.05:0.05:1.0;
n=size(wy,2);
coordsws=[];
%  coordws
for wx= 0.05:0.05:1
coordsws=[coordsws; [repmat(wx,1,n); wy]'];
end
%Distancia  del piso al origen x0,y0=0.5, 
%ancho del eslabo 0.1
%figure('Position',[startx,starty,width,height]);
pos=[-0.075 -0.5 0.15 0.55];
%  curv=[0.0 0.1];
reached=[];
ncoords=size(coordsws)(1);
for i=1:ncoords
    wy=coordsws(i,2);
    wx=coordsws(i,1);
    [q1,q2]=cinemat_inv(l1,l2,wx,wy);
    if (isreal(q1) && isreal(q2) && determinante(l1,l2,q2)>0.2)
        reached=[reached; [wx wy]];
        hold off
        cla
        hold on
        axis([-1 1.7 -1 1.0944]);
        scatter(coordsws(:,1),coordsws(:,2),'b');
        scatter(reached(:,1),reached(:,2),'r','filled');
        axis([-1 1.7 -1 1.0944]);
        rectangle("Position",pos,"EdgeColor",[0 0 0.5], "FaceColor",[0.4 0.6 1]);
        xl=[0 0 -0.2 0.2];
        yl=[-0.6 0.6 0 0];
        line(xl(1:2),yl(1:2));
        line(xl(3:4),yl(3:4));
        text(0,0, "x0,y0");
        x1=x0+l1*sin(q1);
        y1=y0-l1*cos(q1);
        dir=[(x1-x0) (y1-y0)];
        normdir=norm(dir);
        dir=dir/normdir;
        ndir=[(y1-y0) -(x1-x0)];
        normndir=norm(ndir);
        ndir=ndir/normndir;
        line([0 1.2*normdir*dir(1)],[0, 1.2*normdir*dir(2)])
        p1=[0 0]-0.05.*dir-0.05.*ndir;
        p2=p1+(l1+0.05).*dir;
        p3=p2+0.1.*ndir;
        p4=p3-(l1+0.05).*dir;
        pts=[p1;p2;p3;p4];
        patch(pts(:,1),pts(:,2),[0.99 0.5 0.2])
        text(x1,y1, "x1,y1");
        %segundo eslabon
        x2=x1+l2*sin(q1+q2);
        y2=y1-l2*cos(q1+q2);
        dir=[(x2-x1) (y2-y1)];
        normdir=norm(dir);
        dir=dir/normdir;
        ndir=[(y2-y1) -(x2-x1)];
        normndir=norm(ndir);
        ndir=ndir/normndir;
        line([x1 x1+1.2*normdir*dir(1)],[y1 y1+1.2*normdir*dir(2)])
        p1=[x1 y1]-0.05.*dir-0.05.*ndir;
        p2=p1+(l2+0.05).*dir;
        p3=p2+0.1.*ndir;
        p4=p3-(l2+0.05).*dir;
        pts=[p1;p2;p3;p4];
        patch(pts(:,1),pts(:,2),[0.5 0.99 0.2])
        text(x2,y2, "x2,y2");
        drawnow ();
    end
end
fobj=size(reached)(1)/ncoords;
end
