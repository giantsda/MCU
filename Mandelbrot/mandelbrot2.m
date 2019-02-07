% x (real(c))
dx=0.001; % step
x1=-2;
x2=0.5;
% y (imag(c))
dy=0.001; % step
y1=-1;
y2=1;
x=x1:dx:x2;
y=y1:dy:y2;
[X,Y] = meshgrid(x,y); % for vectorized calculation
c=X+1i*Y;
R=10; % if abs(z)>R then z is considered as infinity
n=100; % maximal number of iterations, 100 is close to infinity
z=zeros(size(c)); % starts from zeros
for nc=1:n
    z=z.^2+c; % vectorized
end
% I=abs(z)<R; % logical image
a=abs(z);
a(a==inf)=0;
L=1e3;
a(a>=L)=0;
a=log(a);
imagesc(x,y,a);
colormap jet;
colorbar;

% max(max(a))

% imagesc(x,y,abs(z));
% set(gca,'YDir','normal');
% xlabel('Re(c)');
% ylabel('Im(c)');