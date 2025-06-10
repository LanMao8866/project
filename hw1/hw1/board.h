#pragma once  // 防止標頭檔重複包含
#include "Hex.h"  // 包含六角座標系統
#include <unordered_map>  // 包含雜湊映射容器
#include <vector>  // 包含動態陣列容器
#include <string>  // 包含字串類別
#include <limits>  // 包含數值極限
#include <unordered_set>  // 包含雜湊集合容器

// 棋盤類別，負責管理中國跳棋的遊戲邏輯
class Board {
public:
    // 棋盤大小常數
    static constexpr int BOARD_SIZE = 4;

    // 棋子和格子類型的字元常數
    static constexpr char RED = 'R', BLUE = 'B', GREEN = 'G', EMPTY = '-', SPACE = '.';

    // 建構函式，初始化棋盤
    Board();

    // 將棋盤轉換為字串格式以便顯示
    std::string toString() const;

    // 執行棋子移動，從from位置移動到to位置
    bool move(const Hex& from, const Hex& to);

    // 檢查是否有隊伍獲勝
    bool checkWin() const;

    // 取得當前玩家的顏色
    char getCurrentPlayer() const { return currentPlayer; }

    // 取得獲勝隊伍的顏色
    char getWinner() const;

    // 取得從指定位置可以跳躍到的所有位置
    std::vector<Hex> getJumpMoves(const Hex& from, const Hex& excludePosition = Hex(-999, -999)) const;

    // 檢查是否處於連續跳躍狀態
    bool isInJumpSequence() const { return mustMoveFrom.q != -999 && mustMoveFrom.r != -999; }

    // 取得必須移動的棋子位置（連續跳躍時使用）
    Hex getMustMoveFrom() const { return mustMoveFrom; }

    // 中斷連續跳躍序列
    void stopJumpSequence();

private:
    // 棋盤格子映射，每個六角座標對應一個字元（棋子或空格）
    std::unordered_map<Hex, char> grid;

    // 當前玩家，預設為紅隊
    char currentPlayer = RED;

    // 記錄上一步移動的起始位置，用於防止跳躍時返回原位
    Hex lastMoveFrom = Hex(-999, -999);

    // 記錄必須移動的棋子位置，用於連續跳躍
    Hex mustMoveFrom = Hex(-999, -999);

    // 記錄跳躍歷史位置，防止無限循環跳躍
    std::unordered_set<Hex> jumpHistory;

    // 檢查六角座標是否在有效的棋盤範圍內
    bool isValidPosition(const Hex& hex) const;

    // 檢查指定位置是否在某隊伍的目標區域內
    bool isInTargetArea(const Hex& hex, char team) const;

    // 檢查兩個位置之間是否有有效的連線（相鄰格子）
    bool isValidConnection(const Hex& from, const Hex& to) const;

    // 檢查跳躍移動是否合法
    bool isValidJump(const Hex& from, const Hex& to) const;

    // 初始化棋盤，設置初始棋子位置
    void initializeBoard();

    // 切換到下一個玩家
    void switchPlayer();

    // 清除跳躍狀態的相關記錄
    void clearJumpState();
};