# For DL method use jupyter notebook
## image_segmentation.ipynb (in DL_code file)

0. 需要 install 一些 necessary package
1. 修改讀取影像的路徑 (x_train_dir, y_train_dir, x_valid_dir, y_valid_dir, x_test_dir, y_test_dir)
ex: 
DATA_DIR = './dataset'
x_train_dir = os.path.join(DATA_DIR, 'train', 'image')

2. 參照官方提供的 document(https://segmentation-modelspytorch.readthedocs.io/en/latest/#models)，根據要使用的 pretrain model backbone，設定 ENCODER、ENCODER_WEIGHTS 的參數值
ex:
ENCODER = 'dpn68b'                 # pretrain encoder name
ENCODER_WEIGHTS = 'imagenet+5k'    # pretrain image dataset

3. 挑選 pretrain model 架構
ex: 
model = smp.UnetPlusPlus(
    encoder_name = ENCODER,
    encoder_weights = ENCODER_WEIGHTS,
    classes = len(CLASSES),
    activation = ACTIVATION,
)

4. 從上到下按照順序執行每個 block
5. 如只要 testing，可以跳過 training 部分，直接跑 test_no_iou() 這個 function，他會將輸入的照片，依序預測的結果輸出 idx.jpg
*** 因為 DL model 需要固定圖片的大小作訓練，所以輸入和輸出的圖片都會被 resize 成 512x512
ex: test_no_iou()


## ensemble-training.ipynb

0. 需要 install 一些 necessary package
1. 修改讀取影像的路徑 (x_train_dir, y_train_dir, x_valid_dir, y_valid_dir, x_test_dir, y_test_dir)
ex: 
DATA_DIR = './dataset'
x_train_dir = os.path.join(DATA_DIR, 'train', 'image')
2. 從上到下按照順序執行每個 block

3. 如只要 testing，可以跳過 training 部分，直接跑 test_no_iou() or test() function，他會將輸入的照片，依序預測的結果輸出 idx.jpg
|- test_no_iou() - testing when you don't have ground truth 
|- test() - testing when you have ground truth and calculate IOU score

*** 因為 DL model 需要固定圖片的大小作訓練，所以輸入和輸出的圖片都會被 resize 成 512x512
ex: test_no_iou()

# For conventional method use matlab (3 version)
6 script and 4 function 1 excel
 - [1] v0: 
 - [2] v1: 
 - [3] v3: final version, resize image before cluster

2 script should run by step
## WaterSegmentation_v3_step1.m (in Con_code file)
1. Modify data path and file name
2. Run
2. Result image of each stage in result file

## WaterSegmentation_v3_step2.m (in Con_code file)
3. Select segment in ./result/04_kmeans/ and record index in SegmentSelect.xlsx (each data image one row)
4. Run
5. Final result in ./result/04_kmeans/06_post
