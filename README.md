# p2o_fastlio_util

## 使い方

### 準備
FAST-LIOの結果をROS Bagに保存しておきます．
このファイルから，/cloud_registered_bodyの点群をファイルに保存しておきます．
```
python3 extract_pcd.py fastlio-output-rosbag.bag
```

p2oの処理結果のポーズ配列と，PCDファイルのリストを統合します
```
tail -n +2 graph.p2o_out.txt > poses.txt
find . | grep pcd > clouds.txt
paste clouds.txt poses.txt > concat.txt 
```

### ビルド，実行
```
cd p2o_fastlio_util
cmake -Bbuild .
cmake --build build
./build/rearrange_pointcloud concat.txt
```

### 確認
```
pcl_viewer merged_cloud.pcd
```
