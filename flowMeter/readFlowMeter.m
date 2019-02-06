filename="log";
fileID = fopen(filename);
data=[];
while ~feof(fileID)
    line=fgetl(fileID);
%     line="Time= 19.01900s;pulseCount= 48;flowRate= 10.6560106L/min;v= 0.6231082m/s;vAverage= 0.0327952m/s;"
    C = textscan(line,'%*s%f%*s%f%*s%f%*s%f%*s%f%*s')
    data=[data;C];
end
 