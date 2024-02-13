# p2o_fastlio_util

## 使い方

### 準備
FAST-LIOの結果をROS Bagに保存しておきます．
このファイルから，/cloud_registered_bodyの点群をファイルに出力しておきます．（多数のpcdファイルが生成されます）
```
python3 extract_pcd.py fastlio-output-rosbag.bag
```

p2oの処理結果のポーズ配列と，PCDファイルのリストを統合します．
```
tail -n +2 graph.p2o_out.txt > poses.txt #GNSSを用いて0を原点にする場合は最初の行を取り除く
find . | grep pcd > clouds.txt
paste clouds.txt poses.txt > concat.txt 
```

### ビルド，実行
p2oの結果を用いてpcdファイルの座標を変換し，一つに統合します．
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
