% x is honrizontal; y is vetical;
figure;
set(gcf,'outerposition',get(0,'screensize'));
x1=-2.5;
x2=1;
y1=-1;
y2=1;
xLength=abs(x1-x2);
yLength=abs(y1-y2);
xPixel=720;
yPixel=480;
dx=xLength/xPixel;
dy=yLength/yPixel;
x=linspace(x1,x2,xPixel);
y=linspace(y1,y2,yPixel);

haha=zeros(yPixel,xPixel);
        

zm=1;
x0=-0.21503361460851339;
y0=0.67999116792639069;

while 1
    x1=x0-2*exp(-zm/20);
    x2=x0+2*exp(-zm/20);
    y1=y0-1.13*exp(-zm/20);
    y2=y0+1.13*exp(-zm/20);
    x=linspace(x1,x2,xPixel);
    y=linspace(y1,y2,yPixel);
    
    for j  =1:yPixel
%     j
    for i  =1:xPixel
        cR=x(i); %c=X+i*Y;
        cI=y(j);
        R=0;
        I=0;
        iteration=0;
        while R*R+I*I<=4 && iteration < 100     %z=z.^2+c;  
            Rtemp=R*R-I*I+cR;
            I=2*R*I+cI;
            R=Rtemp;
            iteration=iteration+1;
        end
         haha(j,i)=iteration;
    end
end
imagesc(x,y,haha);
colormap jet;
colorbar;
axis equal;
axis tight


pause(0);

zm=zm+2.35;
end


    
%% Nonvecterized version
for j  =1:yPixel
    j
    for i  =1:xPixel
        cR=x(i); %c=X+i*Y;
        cI=y(j);
        R=0;
        I=0;
        iteration=0;
        while R*R+I*I<=4 && iteration < 100     %z=z.^2+c;  
            Rtemp=R*R-I*I+cR;
            I=2*R*I+cI;
            R=Rtemp;
            iteration=iteration+1;
        end
         haha(j,i)=iteration;
    end
end
imagesc(x,y,haha);
colormap jet;
colorbar;
axis equal;
axis tight

 
%% vecterized version
% [X,Y] = meshgrid(x,y); % for vectorized calculation
% c=X+1i*Y;
% R=zeros(size(c));
% I=zeros(size(c));
% 
% haha=zeros(size(c));
% for i  =1:yPixel
%     fprintf("i=%d \n",i);
%     for j=1:xPixel
%         iteration=0;
%         while R(i,j)*R(i,j)+I(i,j)*I(i,j)<=4 && iteration < 100
%             Rt=R(i,j)*R(i,j)-I(i,j)*I(i,j)+real(c(i,j));
%             I(i,j)=2*R(i,j)*I(i,j)+imag(c(i,j));
%             R(i,j)=Rt;
%             iteration=iteration+1;
%         end
%         haha(i,j)=iteration;
%     end
% end

% imagesc(x,y,haha);
% colormap jet;
% colorbar;
% axis equal;


