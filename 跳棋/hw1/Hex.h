#pragma once  // 防止標頭檔重複包含
#include <functional>  // 包含函數物件的標準庫

// 六角座標結構體，用於表示棋盤上的位置
struct Hex {
    int q, r;  // q和r是六角座標系統的兩個軸

    // 建構函式，預設座標為(0,0)
    Hex(int q = 0, int r = 0) : q(q), r(r) {}

    // 等號運算子重載，用於比較兩個六角座標是否相等
    bool operator==(const Hex& other) const {
        return q == other.q && r == other.r;  // 當q和r都相等時回傳true
    }

    // 計算兩個六角座標之間的曼哈頓距離
    int distance(const Hex& other) const {
        // 六角座標系統中的距離公式
        return (abs(q - other.q) + abs(r - other.r) + abs(q + r - other.q - other.r)) / 2;
    }
};

// 為std命名空間添加Hex的雜湊函數特化，用於unordered_map和unordered_set
namespace std {
    template <> struct hash<Hex> {
        // 雜湊函數實作，將q和r組合成唯一的雜湊值
        size_t operator()(const Hex& h) const noexcept {
            return hash<int>()(h.q * 65536 + h.r);  // 使用位移的方式組合q和r
        }
    };
}