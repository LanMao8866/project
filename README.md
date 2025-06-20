# 中國跳棋♙
## 組別號碼 : 12
## 系級班級 : 資工1A
## 物件專題介紹
 這是一個可以支援3位玩家的跳棋程式

## 小組分工
* 組長 : 黃允
* 組員 : 藍元懋、施旭嶸 
* 程式碼 : 黃允
* 報告 : 黃允
* GitHub、簡報製作 : 藍元懋
* uml : 藍元懋
* 除錯 : 施旭嶸
## 程式遊戲規則
跳棋的棋盤有6個角，可供3人進行遊戲，每個人把各自同顏色的棋子擺滿一個角，按照規則輪流走棋，以最早旗子全部抵達並擺滿對角為優勝。對局將棋子互相間隔一個角擺放以平均分布。
每方棋子6枚，擺成等邊三角形。
## 遊戲玩法
先輸入想移動之旗子座標位置，再輸入該棋子欲移動之位置即可完成移動
## 程式內容
1. **Hex 結構體** - 六角座標系統的核心
2. **Board 類別** - 遊戲邏輯的主要管理者  
3. **std::hash<Hex> 模板特化** - 讓Hex能與STL容器配合
4. **封裝性**：Board類別的內部實作細節都是私有的
## 程式如何安裝執行
* 在GitHub下載跳棋資料夾
* 點擊Download ZIP
* 解壓縮
* 用Visual Stdio開啟檔案
* 即可直接執行

## UML
![GitHub图像](UML.png)

## 程式運行畫面
### 初始棋盤
![GitHub图像](/readme_repository/1.png)

### 輸入座標移動
![GitHub图像](/readme_repository/2.png)

### 棋子在棋盤上移動
![GitHub图像](/readme_repository/3.png)

### 遊戲結束
![GitHub图像](/readme_repository/4.png)



