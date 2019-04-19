close all;
path='/home/chen/Desktop/racewayMeasurements/RPM=13.6/Calibration4/';
filename='calibration4End';

RPMs=[];
fid = fopen([path filename]);
line=0;
i=1;
while  ~feof(fid)
    l = fgetl(fid);
    line=line+1;
    if length(l)>13
        if strcmp(l(1:13),'Forward start')
            velocity=sscanf(l,'Forward start. Velocity= %f');
            rpms=[];
            while(~strcmp(l(1:12),'Forward end.') )
                l = fgetl(fid);
                if strcmp(l(1:5),'Time:')
                    rpm=sscanf(l,'Time:%*f  pulseCount:%*d  currentRPM:%f');
                    rpms=[rpms rpm];
                end
            end
            rpms(find(rpms(:)>mean(rpms)*1.5))=[];
            sizeI=length(rpms);
            if (sizeI>0)
                startI=floor(1+sizeI*0.5);
                endI=ceil(sizeI-sizeI*0.2);
                RPMs(i,1)=velocity; % [velocity mean std]
                RPMs(i,2)=mean(rpms(startI:endI));
                RPMs(i,3)=std(rpms(startI:endI));
                i=i+1;
            end
        end
    end
end

%% copy from calibration.xlsx here
C=[RPMs(:,2) RPMs(:,1)];
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
% D=sortrows(D,3);
plot(D(:,1),D(:,2),'o-')
