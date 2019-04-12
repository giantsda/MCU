path='testBath/';
files=dir([path 'velocity*.txt']);

for i=1:length(files)
    filename=files(i).name
    A=[];
    fid = fopen([path filename]);
    line=0;
    while  ~feof(fid)
        l = fgetl(fid);
        line=line+1;
        if ~isempty(l)
            if strcmp(l(1:5),'Time:')
                s = sscanf(l,'Time:%f pulseCount:%d  currentRPM:%f  RPM_1s:%f');
                A=   [A; s.'];
            end
        end
    end
    
    
    plot(A(:,1), A(:,3),'b*-')
    upLimit = input('upLimit=');
    lowLimit = input('lowLimit=');
    
    
    fileter=find(A(:,3)>lowLimit & A(:,3)<upLimit);
    A=A(fileter,:);
    
    meanRPM=mean(A(:,3))
    RPMstd=std(A(:,3))
end
 
%% copy from calibration.xlsx here
C=sortrows(C,3);
x1=C(:,1);
y1=C(:,3);
plot(x1,y1,'*-')
xlabel('LDV velocity (m/s)');
ylabel('RPM');
[p S] = polyfit(x1,y1,1);
yfit = polyval(p,x1);
yresid =y1 - yfit;
SSresid = sum(yresid.^2);
SStotal = (length(y1)-1) * var(y1);
p
R2=1 - SSresid/SStotal

