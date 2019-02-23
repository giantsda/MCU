% % filePath='/home/chen/Desktop/pic/want/';
% % pics=dir([filePath '*']);
% % j=0;
% % for i=1:length(pics)
% %     if ~isdir([filePath pics(i).name])
% %         %         [filePath pics(i).name]
% %         p=imread([filePath pics(i).name]);
% %         imshow(p);
% %         rotate = input('You wanna rotate this image? yes=1, no=0 ');
% %         if isempty(rotate)
% %             p = imrotate(p,-90);
% %         end
% %         J = imresize(p,[320 240]);
% %         imwrite(J,['/home/chen/Desktop/pic/320/' num2str(j) '.bmp']);
% %         j=j+1;
% %     end
% % end
% 
% 
% 
% %% renum
% filePath='/home/chen/Desktop/pic/320/';
% pics=dir([filePath '*']);
% j=0;
% for i=1:length(pics)
%     if ~isdir([filePath pics(i).name])
%         p=imread([filePath pics(i).name]);
%             p = imrotate(p,180);
% %         J = imresize(p,[320 240]);
%         imwrite(p,['/home/chen/Desktop/pic/num/' num2str(j) '.bmp']);
%         j=j+1
%     end
% end


%% create txt
filePath='/home/chen/Desktop/pic/txt/';
for i=66:100
    i
fid = fopen([filePath num2str(i) '.txt'],'w');
fid = fclose(fid);
end





