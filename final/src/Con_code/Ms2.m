function [clusteredImage, clusterNum] = Ms2(rgbImage)

redChannel=rgbImage(:, :, 1);
greenChannel=rgbImage(:, :, 2);
blueChannel=rgbImage(:, :, 3);
x_locate=zeros([size(rgbImage,2)*size(rgbImage,1),1]);
y_locate=zeros([size(rgbImage,2)*size(rgbImage,1),1]);
for i=1:size(rgbImage,2)
    for j=1:size(rgbImage,1)
        x_locate((i-1)*size(rgbImage,1)+j) = j;
        y_locate((i-1)*size(rgbImage,1)+j) = i;
    end
end
data=[double(redChannel(:)), double(greenChannel(:)), double(blueChannel(:))];
locate = [x_locate, y_locate];
[n, m, clustLocate] = MeanShiftCluster2(data', locate');
%%    
m=reshape(m',size(rgbImage,1),size(rgbImage,2));
n=n';
n=n/255;

clusteredImage=label2rgb(m,n);
clusterNum = size(n,1);
% imwrite(clusteredImage,sprintf('Sam_%u.jpg',z))
toc

end