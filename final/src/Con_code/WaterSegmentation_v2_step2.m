%% select and merge segment
dataPath = '../result/kmeans_2/';
resPath = '../result/merge_2/';
mkdir(resPath);
selectCell = readcell("SegmentSelect2.xlsx");
[nrows,ncols] = size(selectCell);
for i = 1:imgNum
    mask = imread([dataPath, num2str(i), '_1.jpg']);
    mask = false(size(mask));
    for j = 1:ncols
        x = selectCell{i,j};
        if ~ismissing(x)
            imgSel = imread([dataPath, num2str(i), '_', num2str(x), '.jpg']);
            maskSel = imbinarize(imgSel);
            mask = mask + maskSel;
        end
        imwrite(mask, [resPath, int2str(i),'_merge.jpg']);
    end
end
postfix = '_merge.jpg';

%% post-processing (closing of opening)
dataPath = resPath;
resPath = '../result/post_2/';
mkdir(resPath);
% post-processing parameter
roundsize = 1;
shape = strel('disk',roundsize);
for i = 1:imgNum
    mask = imread([dataPath, num2str(i), postfix]);
    mask = imbinarize(mask);
    mask = imclose(imopen(mask, shape), shape);
    imwrite(mask, [resPath, int2str(i), '.jpg']);
end