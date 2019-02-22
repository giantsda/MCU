% path='/home/chen/Desktop/PIC/TXT/ASCII/';
% for i=201:300
%     i
%    fid=fopen([path num2str(i) '.txt'],'w');
%    fclose(fid);
% end



%% remove space
% files=dir('/home/chen/Desktop/PIC/TXT/ASCII/*.txt');
% for f=1:length(files)
%     %     if f==17
%     %         pause();
%     %     end
%     file=['/home/chen/Desktop/PIC/TXT/ASCII/' files(f).name]
%     A=[];
%     [fid, msg] = fopen(file);
%     if fid < 0
%         error('Failed to open file "%s" because "%s"', filename, msg);
%     end
%     n = 1;
%     while true
%         t = fgetl(fid);
%         if ~ischar(t)
%             break;
%         else
%             A{n,1} = t;
%             n = n + 1;
%         end
%     end
%     n=n-1;
%     fclose(fid);
%     B=[];
%     j=1;
%     for i=1:length(A)
%         if ~isempty(A{i})
%             B{j}=A{i};
%             j=j+1;
%         end
%     end
%     fid = fopen(file, 'w');
%     for i = 1:length(B)
%         fprintf(fid,'%s\n', B{i});
%     end
% end

TFTTotalLine=40;
% criticalLine=25;
%%  move ASCII ART to center
files=dir('/home/chen/Desktop/PIC/TXT/ASCII/*.txt');
for f=1:length(files)
    %     if f==12
    %         pause();
    %     end
    file=['/home/chen/Desktop/PIC/TXT/ASCII/' files(f).name]
    A=[];
    [fid, msg] = fopen(file);
    if fid < 0
        error('Failed to open file "%s" because "%s"', filename, msg);
    end
    n = 1;
    while true
        t = fgetl(fid);
        if ~ischar(t)
            break;
        else
            A{n,1} = t;
            n = n + 1;
        end
    end
    n=n-1;
    fclose(fid);
    if n<40
        A{40,1}=[];
        centerLine=floor(n/2);
        moveLine=20-centerLine;
        lastLine=n+moveLine;
        
        A(lastLine-n+1:lastLine)=A(1:n);
        for i=1:lastLine-n
            A{i,1}='';
        end
        
        fid = fopen(file, 'w');
        for i = 1:length(A)
            fprintf(fid,'%s\n', A{i});
        end
    end
end
