#pragma once
#include <unordered_map>
#include "hex.h"

class Board {
    std::unordered_map<Hex, char> pieces; // 棋子存放 (R=紅方, B=藍方)
    char currentPlayer = 'R';

public:
    Board();                    // 建構函式初始化棋盤
    void print() const;         // 列印棋盤
    bool move(const Hex& from, const Hex& to); // 移動棋子
    bool checkWin() const;      // 檢查勝利條件
    char getCurrentPlayer() const;
};