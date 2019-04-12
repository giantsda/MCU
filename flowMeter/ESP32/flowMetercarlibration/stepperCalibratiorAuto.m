close all;
path='testBath/';
filename='OneRunDifferentVelocityResolution=0.0125.txtTry=3';

A=[];
fid = fopen([path filename]);
line=0;
while  ~feof(fid)
    l = fgetl(fid);
    line=line+1;
    if ~isempty(l)
        if strcmp(l(1:5),'Time_') && contains(l, 'RPM_1s:0.00')
            s = sscanf(l,'Time_:%f');
            s=[s 0 0 0];
            A= [A; s];
        elseif strcmp(l(1:5),'Time:')
            s = sscanf(l,'Time:%f pulseCount:%d  currentRPM:%f  RPM_1s:%f');
            A=   [A; s.'];
        end
        
    end
end

plot(A(:,1), A(:,3),'b*-')
hold on;
%%filter
filter1=find(A(:,1)>200&A(:,3)>A(:,1)/1270*1037+20) ;
plot(A(filter1,1), A(filter1,3),'r*-')
filter2=find(A(:,1)>400 & A(:,3)>177 & A(:,3)<A(:,1)/1270*1000-100) ;
plot(A(filter2,1), A(filter2,3),'g*-')
hold off;
filter=[filter1;filter2];
disp("Do you think the filter is good?")
pause();
A(filter,:)=[];
plot(A(:,1), A(:,3),'b*-')

%% detect forward and backward move
nonZeros= find(A(:,3)~=0);
nonZeros=[0;nonZeros];
interval=[];
for i=2:length(nonZeros)
    if nonZeros(i)-nonZeros(i-1)~=1
        interval=[interval  nonZeros(i-1)  nonZeros(i)];
    end
    
end

interval(1)=[];
interval=[interval nonZeros(end)];
interval=reshape(interval,2, []).';

RPMs=zeros(length(interval),2);
for i=1:length(interval)
    sizeI=interval(i,2)-interval(i,1)+1;
    startI=floor(interval(i,1)+sizeI*0.5);
    endI=ceil(interval(i,2)-sizeI*0.2);
    RPMs(i,1)=mean(A(startI:endI,3));
    RPMs(i,2)=std(A(startI:endI,3));
end
plot(RPMs(:,1))
%% you may want to modified RPMs a liitle bit so that the pattern is consistent
% RPMs=[RPMs(1:116,:); [130 15]; RPMs(117:end,:)];
RPMs=RPMs(2:2:end,:); 
%  =0.05;velocity<1.5;velocity=velocity+0.0125
velocity=0.05+0.0125:0.0125:1.5;
plot(velocity,RPMs(:,1),'*-') % propeller does not at 0.05 
%% copy from calibration.xlsx here
C=[RPMs(:,1) velocity.'];
x1=C(:,1);
y1=C(:,2);
plot(x1,y1,'*-')
xlabel('RPM');
ylabel('Stepper velocity (m/s)');
[p S] = polyfit(x1,y1,1);
yfit = polyval(p,x1);
yresid =y1 - yfit;
SSresid = sum(yresid.^2);
SStotal = (length(y1)-1) * var(y1);
p
R2=1 - SSresid/SStotal

hold on;
D=sortrows(D,3);
plot(D(:,1),D(:,3),'o-')
