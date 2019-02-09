% x is honrizontal; y is vetical;
close all;
figure;
set(gcf,'outerposition',get(0,'screensize')*1);
xPixel=320*4;
yPixel=240*4;
maxIter=200;
iterMap=zeros(yPixel,xPixel);
zm=1;
x0=-0.21503361460851339;
y0=0.67999116792639069;
PICK=0;
FIRST=1;
frame=1;
screenExamLength=floor(yPixel/100);
dx=1;

clear F;
while dx>1e-14
    dx
    if PICK
        [maxvalues, ind] = maxk(gradientMag(:), 100); % find n largest
        [I,J]=ind2sub(size(gradientMag),ind); % get row and column number
        pickS=[I J];
        
        if FIRST
            pickI=randi(length(I));
        else
            for i=1:length (pickS)
                xp=x1+dx*(J(i)-1);
                yp=y1+dy*(I(i)-1);
                pickS(i,3)=sqrt((xp-x0)^2+(yp-y0)^2);% distance
            end
            [~, pickI]=min(pickS(:,3)); % find the smallest distance one;
        end
        x0=x1+dx*(J(pickI)-1);
        y0=y1+dy*(I(pickI)-1);
        if FIRST
            hold on;
            plot(x0,y0,'R*','MarkerSize',100);
            hold off;
        end
        title("New zoom point");
        %         pause();
        PICK=0;
    end
    
    x1=x0-2*exp(-zm/20);
    x2=x0+2*exp(-zm/20);
    y1=y0-1.13*exp(-zm/20);
    y2=y0+1.13*exp(-zm/20);
    x=linspace(x1,x2,xPixel);
    y=linspace(y1,y2,yPixel);
    dx = (x2 - x1) / (xPixel-1);
    dy = (y2 - y1) / (yPixel-1);
    for j  =1:yPixel
        for i  =1:xPixel
            cR = (i-1) * dx + x1;%c=X+i*Y;
            cI = (j-1) * dy + y1;
            R=0;
            I=0;
            iteration=0;
            while R*R+I*I<=4 && iteration < maxIter     %z=z.^2+c;
                Rtemp=R*R-I*I+cR;
                I=2*R*I+cI;
                R=Rtemp;
                iteration=iteration+1;
            end
            iterMap(j,i)=iteration;
        end
    end
    
    
    [FX,FY] = gradient(iterMap);
    gradientMag=abs(log(FX.*FX+FY.*FY));
    gradientMag(gradientMag==Inf)=0;
    maxGradientMag=max(max(gradientMag));
    meanGradientMag=mean(mean(gradientMag));
    
    if FIRST  % pick a arbitary point at first time.
        PICK=1;
    end
    
    examedWindowMeangradientMag=mean(mean(gradientMag(yPixel/2-screenExamLength:yPixel/2+screenExamLength,xPixel/2-screenExamLength:xPixel/2+screenExamLength)));
    if examedWindowMeangradientMag<=meanGradientMag*0.5
        PICK=1;
    end
    
    imagesc(x,y,(iterMap));
    %     title([num2str(examedWindowMeangradientMag) ...
    %         '  ' num2str(meanGradientMag*0.1) ' dx=' num2str(dx)] );
    
    if FIRST
        dx0=dx;
        FIRST=0;
    end
    
    
    title(['zoom: X ' num2str(dx0/dx)]);
    %     hold on;
    %     plot(x0,y0,'w*','MarkerSize',30);
    %     hold off;
    
    %     colormap summer;
    %     colormap parula
    colormap jet;
    
    colorbar;
    axis equal;
    axis tight
    F(frame)=getframe(gcf);
    frame=frame+1;
    zm=zm+1.2;
    pause(0);
end

F=[F(1:end) F(end:-1:1)];
outputVideo = VideoWriter('haha.avi');
outputVideo.FrameRate =8;
open(outputVideo);
for i=1:length(F)
    i/length(F)*100
    writeVideo(outputVideo,F(i).cdata);
end
close(outputVideo)





%% vecterized version
% while 1
% x1=x0-2*exp(-zm/20);
% x2=x0+2*exp(-zm/20);
% y1=y0-1.13*exp(-zm/20);
% y2=y0+1.13*exp(-zm/20);
% x=linspace(x1,x2,xPixel);
% y=linspace(y1,y2,yPixel);
%
% [X,Y] = meshgrid(x,y); % for vectorized calculation
% c=X+1i*Y;
% R=10; % if abs(z)>R then z is considered as infinity
% n=100; % maximal number of iterations, 100 is close to infinity
% z=zeros(size(c)); % starts from zeros
% for nc=1:n
%     z=z.^2+c; % vectorized
% end
%
% a=abs(z);
% a(a==inf)=0;
% L=1e3;
% a(a>=L)=0;
% a=log(a);
% imagesc(x,y,a);
% colormap jet;
% colorbar;
%  zm=zm+11;
%  pause(0)
% end
