x0=-0.21503361460851339;
y0=0.67999116792639069;
resx=1024; % resolution
resy=576;
R=8; % if abs(z)>R then z is considered as infinity
n=80; % maximal number of iterations, 100 is close to infinity
clear F;
fc=1;
figure('units','normalized','position',[0.1-0.08 0.073 0.8 0.8]);
%for zm=1:300
tic;
zmmx=200;
for zm=1:0.35:zmmx
    
%for sz=2:-0.2:0.1
    % x (real(c))
    zmf=exp(-zm/20);
    x1=x0-2*zmf;
    x2=x0+2*zmf;
    %x1=x0-sz;
    %x2=x0+sz;
    % y (imag(c))
    y1=y0-1.13*exp(-zm/20);
    y2=y0+1.13*exp(-zm/20);
    
    %y1=y0-sz;
   %y2=y0+sz;
    x=linspace(x1,x2,resx);
    y=linspace(y1,y2,resy);
    [X,Y] = meshgrid(x,y); % for vectorized calculation
    c=X+1i*Y;
    
    z=zeros(size(c)); % starts from zeros
    I=zeros(size(c));
    for nc=1:n
        z=z.^2+c; % vectorized
        bw=abs(z)<R;
        I(bw)=nc;
    end
    cla;
    imagesc(x,y,I);
    set(gca,'YDir','normal');
    xlabel('Re(c)');
    ylabel('Im(c)');
    axis equal;
    %colorbar;
    set(gca,'clim',[0 n]);
    axis off;
    %zoom(1.2);
    set(gca,'units','normalized','position',[0 0 1 1]);
    drawnow;
    F(fc)=getframe(gcf);
    
    fc=fc+1;
    jr=zm/zmmx;
    v=jr/toc;
    et=(1-jr)/v;
    disp(['zm=' num2str(zm) '  et=' num2str(et)]);
end
F=[F(2:end) F(end:-10:2)];
movie2avi(F,'mandelbrot10','fps',20,'compression','Cinepak');