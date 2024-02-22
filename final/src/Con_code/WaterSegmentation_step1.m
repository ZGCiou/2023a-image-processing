%% clear command windows
clc
clear
close

%% Parameter
imgNum = 60;
srcPath = '../training_dataset/image/';
postfix = '.jpg';

%% pre-processong
dataPath = srcPath;
resPath = '../result/pre/';
mkdir(resPath);
parfor i = 1:imgNum
    img = imread([dataPath, num2str(i), postfix]);

    img = double(img);
    mean_r = mean(mean(img(:,:,1)));
    mean_g = mean(mean(img(:,:,2)));
    mean_b = mean(mean(img(:,:,3)));
    max_mean = max([mean_r mean_g mean_b]);
    adjusted_img = img;
    adjusted_img(:,:,1) = img(:,:,1)*max_mean/mean_r;
    adjusted_img(:,:,2) = img(:,:,2)*max_mean/mean_g;
    adjusted_img(:,:,3) = img(:,:,3)*max_mean/mean_b;
    
    img = uint8(img);
    adjusted_img = uint8(adjusted_img);

    adjusted_img(:,:,1) = histeq(adjusted_img(:,:,1));
    adjusted_img(:,:,2) = histeq(adjusted_img(:,:,2));
    adjusted_img(:,:,3) = histeq(adjusted_img(:,:,3));

    imwrite(adjusted_img,[resPath, sprintf('%d_pre.jpg',i)])
end
postfix = '_pre.jpg';

%% smooth
dataPath = resPath;
resPath = '../result/smooth/';
mkdir(resPath);
% smooth parameter
sigma = 0.2;
alpha = 5.0;
numLevels = 64;
parfor i = 1:imgNum
    img = imread([dataPath, int2str(i), postfix]);
    imgS = locallapfilt(img, sigma, alpha, 'NumIntensityLevels', numLevels);
    imwrite(imgS, [resPath, int2str(i), '_smooth.jpg']);
end
postfix = '_smooth.jpg';

%% mean shift (color + spatial)
dataPath = resPath;
resPath = '../result/meanshift/';
mkdir(resPath);
% meanshift parameter
bw = 0.2; % Mean Shift Bandwidth
clusterNum = [];
parfor i = 1:imgNum
    img = imread([dataPath, int2str(i), postfix]);
    [imgMS, clusterNum(i)] = Ms(img,bw);
    imwrite(imgMS, [resPath, int2str(i),'_ms.jpg']);
end
postfix = '_ms.jpg';

%% k-means and write mask
dataPath = resPath;
resPath = '../result/kmeans/';
mkdir(resPath);
% k-means parameter
segNum = 10;
for i = 1:imgNum
    img = imread([dataPath, int2str(i), postfix]);
    src = imread([srcPath, int2str(i), '.jpg']);
    if clusterNum(i)<segNum
        n = clusterNum(i);
    else
        n = segNum;
    end
    L = imsegkmeans(img,n,"NormalizeInput",true);
    C = labeloverlay(src,L);
    imwrite(src, [resPath, int2str(i), '_src.jpg']);
    imwrite(C, [resPath, int2str(i), '_seg.jpg']);
    parfor j = 1:n
        mask = L == j;
        maskname = [resPath, int2str(i), '_', int2str(j), '.jpg'];
        imwrite(mask, maskname);
    end
end

%% select and merge segment

%% post-processing

