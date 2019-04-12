path='LDVCalibration/';
files=dir([path 'test*.txt']);

for i=1:length(files)
    filename=files(i).name
    A=[];
    fid = fopen([path filename]);
    while  ~feof(fid)
        line = fgetl(fid);
        if strcmp(line(1:5),'Time:')
            s = sscanf(line,'Time:%f pulseCount:%d  currentRPM:%f  RPM_1s:%f');
            A=   [A; s.'];
        end
    end

    % plot(A(:,1), A(:,3),'b*-')
    % hold on;
    if ~isempty(A)
        meanRPM=mean(A(:,3));
        fileter=find(A(:,3)>meanRPM*1.5 | A(:,3)<meanRPM*0.6667);
        % plot(A(fileter,1), A(fileter,3),'r*-')
        A(fileter,:)=[];
        plot(A(:,1), A(:,3),'b*-')
        meanRPM=mean(A(:,3))
        RPMstd=std(A(:,3))
%         pause()
    end
end

path='LDVCalibration/';
files=dir([path 'Test.*.txt']);
for i=1:length(files)
    filename=files(i).name
    A=[];
    delimiterIn = '\t'; %read txt file
    headerlinesIn = 6;
    data_m = importdata([path filename],delimiterIn,headerlinesIn);
    data=data_m.data;
    meanLDV=mean(data(:,4))
    LDVstd=std(data(:,4))
    pause()
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
 
