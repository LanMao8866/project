#include "Board.h"  // 包含棋盤類別的標頭檔
#include <iostream>  // 包含輸入輸出流
#include <sstream>   // 包含字串流
#include <unordered_set>  // 包含雜湊集合
#include <algorithm>  // 包含演算法函式
#include <queue>      // 包含佇列容器
using namespace std;  // 使用標準命名空間

// 棋盤建構函式，呼叫初始化函式
Board::Board() {
    initializeBoard();  // 初始化棋盤布局
}

// 初始化棋盤，設置所有棋子和空格的初始位置
void Board::initializeBoard() {
    grid.clear();  // 清空棋盤格子映射

    // 使用字串模式定義棋盤布局，其中：
    // '0' = 不存在的位置
    // '1' = 棋子位置
    // '-' = 可移動的空格
    // '_' = 裝飾格（不可移動）
    const vector<string> pattern = {
        "0000001000000",  // 第1行
        "000001_100000",  // 第2行
        "-_-_1_1_1_-_-",  // 第3行
        "0-_-_-_-_-_-0",  // 第4行
        "001_-_-_-_100",  // 第5行
        "01_1_-_-_1_10",  // 第6行
        "1_1_1_-_1_1_1",  // 第7行
        "00000-_-00000",  // 第8行
        "000000-000000"   // 第9行
    };

    // 將字串模式轉換為六角座標系統（原點在中心第5行第7列）
    for (int row = 0; row < pattern.size(); ++row) {
        for (int col = 0; col < pattern[row].size(); ++col) {
            char c = pattern[row][col];  // 取得當前字元
            if (c == '0') continue;  // 跳過不存在的位置

            Hex pos(col - 6, row - 4);  // 座標轉換，將陣列索引轉為六角座標

            // 先處理空格和裝飾格
            if (c == '-') {
                grid[pos] = EMPTY;  // 設為可移動的空格
                continue;
            }
            else if (c == '_') {
                grid[pos] = SPACE;  // 設為裝飾格
                continue;
            }
            else if (c == '1') {
                // 根據位置分配棋子給不同隊伍
                if (row <= 1)       grid[pos] = RED;    // 上方紅隊
                else if (row == 2 && col == 4) grid[pos] = RED;   // 紅隊額外位置
                else if (row == 2 && col == 6) grid[pos] = RED;   // 紅隊額外位置
                else if (row == 2 && col == 8) grid[pos] = RED;   // 紅隊額外位置
                else if (row == 4 && col <= 3)  grid[pos] = BLUE;  // 左側藍隊
                else if (row == 5 && col <= 4)  grid[pos] = BLUE;  // 藍隊延伸
                else if (row == 6 && col <= 5)  grid[pos] = BLUE;  // 藍隊延伸
                else if (row == 4 && col >= 9)  grid[pos] = GREEN; // 右側綠隊
                else if (row == 5 && col >= 8)  grid[pos] = GREEN; // 綠隊延伸
                else if (row == 6 && col >= 7)  grid[pos] = GREEN; // 綠隊延伸
            }
        }
    }
}

// 將棋盤轉換為字串以便在終端機顯示
string Board::toString() const {
    ostringstream oss;  // 建立字串流
    
    // 定義顯示框架，用於格式化棋盤外觀
    const vector<string> displayFramework = {
        "             ",      // 行0：不使用
        "------+------",      // 行1
        "-----+-+-----",      // 行2
        "+-+-+-+-+-+-+",      // 行3
        "-+-+-+-+-+-+-",      // 行4
        "--+-+-+-+-+--",      // 行5
        "-+-+-+-+-+-+-",      // 行6
        "+-+-+-+-+-+-+",      // 行7
        "-----+-+-----",      // 行8
        "------+------"       // 行9
    };

    // 遍歷每一行來建構顯示字串
    for (int row = 1; row < displayFramework.size(); ++row) {
        oss << "  ";  // 行縮排
        
        // 遍歷每一列
        for (int col = 0; col < displayFramework[row].size(); ++col) {
            char frameworkChar = displayFramework[row][col];  // 取得框架字元
            Hex pos(col - 6, row - 5);  // 轉換為六角座標（中心為(0,0)）

            // 檢查這個位置是否在棋盤上
            if (grid.count(pos)) {
                char value = grid.at(pos);  // 取得該位置的值
                if (value == EMPTY)
                    oss << '-';  // 顯示可移動空格
                else if (value == SPACE)
                    oss << '.';  // 顯示裝飾格
                else
                    oss << value;  // 顯示棋子（R/B/G）
            }
            else if (frameworkChar == '+') {
                oss << '.';  // 顯示裝飾格（棋盤邊緣連接格）
            }
            else {
                oss << ' ';  // 顯示背景格
            }

            oss << ' ';  // 在每個格子間加入空格
        }
        oss << '\n';  // 換行
    }

    return oss.str();  // 回傳完整的棋盤字串
}

// 切換到下一個玩家（紅->藍->綠->紅）
void Board::switchPlayer() {
    switch (currentPlayer) {
    case RED: currentPlayer = BLUE; break;    // 紅隊後換藍隊
    case BLUE: currentPlayer = GREEN; break;  // 藍隊後換綠隊
    case GREEN: currentPlayer = RED; break;   // 綠隊後換紅隊
    }
}

// 清除跳躍狀態的所有記錄
void Board::clearJumpState() {
    lastMoveFrom = Hex(-999, -999);    // 重設上一步起始位置
    mustMoveFrom = Hex(-999, -999);    // 重設必須移動位置
    jumpHistory.clear();               // 清空跳躍歷史
}

// 停止跳躍序列並切換玩家
void Board::stopJumpSequence() {
    clearJumpState();  // 清除跳躍狀態
    switchPlayer();    // 切換到下一個玩家
}

// 執行棋子移動的主要函式
bool Board::move(const Hex& from, const Hex& to) {
    // 如果處於連續跳躍狀態，檢查是否只能移動指定的棋子
    if (mustMoveFrom.q != -999 && mustMoveFrom.r != -999) {
        if (from.q != mustMoveFrom.q || from.r != mustMoveFrom.r) {
            return false;  // 連續跳躍時必須移動指定的棋子
        }
    }

    // 確認來源格存在且是當前玩家的棋子
    if (!grid.count(from)) {
        return false;  // 來源位置不存在
    }

    if (grid.at(from) != currentPlayer) {
        return false;  // 來源位置的棋子不屬於當前玩家
    }

    // 確認目的地格存在且是空格（EMPTY）
    if (!grid.count(to)) {
        return false;  // 目的地位置不存在
    }

    if (grid.at(to) != EMPTY) {
        return false;  // 目的地位置不是空格
    }

    int dist = from.distance(to);  // 計算移動距離

    // 處於連續跳躍狀態時，只允許跳躍移動
    if (mustMoveFrom.q != -999 && mustMoveFrom.r != -999) {
        // 檢查目標位置是否在跳躍歷史中（防止無限循環）
        if (jumpHistory.count(to)) {
            return false;  // 目標位置已經訪問過，會造成循環
        }

        // 取得可能的跳躍位置，排除上一步的起始位置
        vector<Hex> jumps = getJumpMoves(from, lastMoveFrom);

        // 確認目的地是否在合法跳躍範圍內
        if (find(jumps.begin(), jumps.end(), to) == jumps.end()) {
            return false;  // 目標不在有效跳躍範圍內
        }

        // 執行跳躍移動
        Hex previousFrom = from;  // 記錄這次移動的起始位置
        grid[to] = currentPlayer;  // 將棋子移動到目標位置
        grid[from] = EMPTY;        // 清空原始位置

        jumpHistory.insert(to);    // 將目標位置加入跳躍歷史

        // 檢查是否還能繼續跳躍（排除剛才的起始位置）
        vector<Hex> nextJumps = getJumpMoves(to, previousFrom);

        // 過濾掉會造成循環的跳躍位置
        vector<Hex> validNextJumps;
        for (const auto& jump : nextJumps) {
            if (jumpHistory.find(jump) == jumpHistory.end()) {
                validNextJumps.push_back(jump);  // 只保留未訪問過的位置
            }
        }

        if (validNextJumps.empty()) {
            // 無法繼續跳躍，切換玩家並清除所有記錄
            clearJumpState();
            switchPlayer();
        }
        else {
            // 保持同一玩家，更新跳躍狀態記錄
            lastMoveFrom = previousFrom;  // 記錄上一步起始位置
            mustMoveFrom = to;            // 跳躍後的位置成為下一步必須移動的位置
        }

        return true;  // 移動成功
    }

    // 不在連續跳躍狀態時的一般移動處理
    // 單步移動（距離1或2，如果是有效連線）
    if (dist <= 2) {
        // 檢查是否為合法的連線移動
        bool validConnection = isValidConnection(from, to);

        if (validConnection) {
            // 執行單步移動
            grid[to] = currentPlayer;  // 將棋子移動到目標位置
            grid[from] = EMPTY;        // 清空原始位置

            // 單步移動後切換玩家並清除所有記錄
            clearJumpState();
            switchPlayer();
            return true;  // 移動成功
        }
    }

    // 跳躍移動 - 檢查是否為合法的跳躍路徑
    if (dist > 2 || !isValidConnection(from, to)) {
        // 取得可能的跳躍位置
        vector<Hex> jumps = getJumpMoves(from, lastMoveFrom);

        // 確認目的地是否在合法跳躍範圍內
        if (find(jumps.begin(), jumps.end(), to) == jumps.end()) {
            return false;  // 目標不在有效跳躍範圍內
        }

        // 執行跳躍移動
        Hex previousFrom = from;  // 記錄這次移動的起始位置
        grid[to] = currentPlayer;  // 將棋子移動到目標位置
        grid[from] = EMPTY;        // 清空原始位置

        // 開始新的跳躍序列，初始化跳躍歷史
        jumpHistory.clear();
        jumpHistory.insert(from);  // 記錄起始位置
        jumpHistory.insert(to);    // 記錄目標位置

        // 檢查是否還能繼續跳躍（排除剛才的起始位置）
        vector<Hex> nextJumps = getJumpMoves(to, previousFrom);

        if (nextJumps.empty()) {
            // 無法繼續跳躍，切換玩家並清除所有記錄
            clearJumpState();
            switchPlayer();
        }
        else {
            // 保持同一玩家，更新跳躍狀態記錄
            lastMoveFrom = previousFrom;  // 記錄上一步起始位置
            mustMoveFrom = to;            // 跳躍後的位置成為下一步必須移動的位置
        }

        return true;  // 移動成功
    }

    return false;  // 移動失敗
}

// 檢查兩個位置是否有有效連線（相鄰格子）
bool Board::isValidConnection(const Hex& from, const Hex& to) const {
    int dq = to.q - from.q;     // 計算q軸差值
    int dr = to.r - from.r;     // 計算r軸差值
    int distance = from.distance(to);  // 計算距離

    // 對於距離為1的移動，檢查六個主要方向
    if (distance == 1) {
        static const vector<pair<int, int>> validDirections = {
            {1, 0},   // 右
            {0, 1},   // 右下  
            {-1, 1},  // 左下
            {-1, 0},  // 左
            {0, -1},  // 左上
            {1, -1}   // 右上
        };

        // 檢查移動方向是否為有效的六角方向之一
        for (const auto& [validDq, validDr] : validDirections) {
            if (dq == validDq && dr == validDr) {
                return true;  // 找到有效方向
            }
        }
    }

    // 對於距離為2的移動，檢查是否為有效的相鄰連線
    else if (distance == 2) {
        static const vector<pair<int, int>> extendedDirections = {
            {2, 0},   // 水平向右
            {-2, 0},  // 水平向左
            {2, -1},  // 右上延伸
            {1, 1},   // 右下延伸  
            {-1, 2},  // 左下延伸
            {-2, 1},  // 左延伸
            {-1, -1}, // 左上延伸
            {1, -2}   // 右延伸
        };

        // 檢查移動方向是否為有效的延伸方向之一
        for (const auto& [validDq, validDr] : extendedDirections) {
            if (dq == validDq && dr == validDr) {
                return true;  // 找到有效延伸方向
            }
        }
    }

    return false;  // 無有效連線
}

// 取得從指定位置可以跳躍到的所有位置
vector<Hex> Board::getJumpMoves(const Hex& from, const Hex& excludePosition) const {
    vector<Hex> jumps;  // 儲存所有可能的跳躍位置
    if (!grid.count(from)) return jumps;  // 如果起始位置不存在，回傳空向量

    unordered_set<Hex> visited;  // 記錄已訪問的位置
    queue<Hex> bfs_queue;        // 廣度優先搜尋的佇列

    bfs_queue.push(from);   // 將起始位置加入佇列
    visited.insert(from);   // 標記起始位置為已訪問

    // 使用廣度優先搜尋找出所有可能的跳躍位置
    while (!bfs_queue.empty()) {
        Hex current = bfs_queue.front();  // 取得佇列前端的位置
        bfs_queue.pop();                  // 移除佇列前端元素

        // 檢查所有實際存在的相鄰位置
        for (const auto& [neighbor_pos, neighbor_piece] : grid) {
            // 只考慮有棋子的位置作為跳躍點
            if (neighbor_piece != RED && neighbor_piece != BLUE && neighbor_piece != GREEN) {
                continue;  // 跳過空格和裝飾格
            }

            // 檢查是否為相鄰位置（距離為1或2的有效連線）
            int dist = current.distance(neighbor_pos);
            if (dist > 2 || !isValidConnection(current, neighbor_pos)) {
                continue;  // 跳過不相鄰的位置
            }

            // 計算跳躍目標位置（沿著相同方向延伸）
            int dq = neighbor_pos.q - current.q;
            int dr = neighbor_pos.r - current.r;
            Hex target(neighbor_pos.q + dq, neighbor_pos.r + dr);

            // 檢查目標位置是否存在且為空
            if (grid.count(target) && grid.at(target) == EMPTY) {
                // 排除指定的位置（通常是上一步的起始位置）
                if (excludePosition.q != -999 && excludePosition.r != -999 &&
                    target.q == excludePosition.q && target.r == excludePosition.r) {
                    continue;  // 跳過被排除的位置
                }

                // 如果這個位置還沒有訪問過，加入結果並繼續BFS
                if (visited.find(target) == visited.end()) {
                    visited.insert(target);    // 標記為已訪問
                    jumps.push_back(target);   // 加入跳躍結果
                    bfs_queue.push(target);    // 加入佇列繼續搜尋
                }
            }
        }
    }

    return jumps;  // 回傳所有可能的跳躍位置
}

// 檢查是否有隊伍獲勝
bool Board::checkWin() const {
    // 檢查所有三個隊伍的勝利條件
    for (char team : {RED, BLUE, GREEN}) {
        int piecesInTarget = 0;  // 在目標區域的棋子數量
        int totalPieces = 0;     // 該隊伍的總棋子數量

        // 計算該隊伍的棋子數量和在目標區域的棋子數量
        for (const auto& [pos, piece] : grid) {
            if (piece == team) {
                totalPieces++;  // 增加總棋子數量
                if (isInTargetArea(pos, team)) {
                    piecesInTarget++;  // 增加目標區域內的棋子數量
                }
            }
        }

        // 當該隊伍所有棋子都在目標區域時，該隊伍獲勝
        if (totalPieces > 0 && piecesInTarget == totalPieces) {
            return true;  // 找到獲勝隊伍
        }
    }

    return false;  // 沒有隊伍獲勝
}

// 取得獲勝隊伍的顏色
char Board::getWinner() const {
    // 檢查每個隊伍是否獲勝
    for (char team : {RED, BLUE, GREEN}) {
        int piecesInTarget = 0;  // 在目標區域的棋子數量
        int totalPieces = 0;     // 該隊伍的總棋子數量

        // 計算該隊伍的棋子數量和在目標區域的棋子數量
        for (const auto& [pos, piece] : grid) {
            if (piece == team) {
                totalPieces++;  // 增加總棋子數量
                if (isInTargetArea(pos, team)) {
                    piecesInTarget++;  // 增加目標區域內的棋子數量
                }
            }
        }

        // 如果該隊伍所有棋子都在目標區域，該隊伍獲勝
        if (totalPieces > 0 && piecesInTarget == totalPieces) {
            return team;  // 回傳獲勝隊伍
        }
    }

    return '\0';  // 沒有獲勝者
}

// 檢查六角座標是否在有效的棋盤範圍內
bool Board::isValidPosition(const Hex& hex) const {
    // 檢查是否在有效六邊形範圍內
    return abs(hex.q) <= 6 &&
           abs(hex.r) <= 6 &&
           abs(hex.q + hex.r) <= 6;
}

// 檢查指定位置是否在某隊伍的目標區域內
bool Board::isInTargetArea(const Hex& hex, char team) const {
    switch (team) {
    case RED:
        // 紅隊目標：下方三角形（基於獲勝範例的座標分析）
        // 範例中紅隊獲勝時位於: (-1,3), (1,3), (0,4), (-2,2), (0,2), (2,2)
        return ((hex.r == 2 && hex.q >= -2 && hex.q <= 2 && hex.q % 2 == 0) ||  // 最上排: (-2,2), (0,2), (2,2)
            (hex.r == 3 && (hex.q == -1 || hex.q == 1)) ||                    // 中間排: (-1,3), (1,3)
            (hex.r == 4 && hex.q == 0));                                      // 最下排: (0,4)

    case BLUE:
        // 藍隊目標：右上方三角形（基於獲勝範例的座標分析）
        // 範例中藍隊獲勝時位於: (4,-2), (3,-1), (5,-1), (2,-2), (4,0), (6,-2)
        return ((hex.r == 0 && hex.q == 4) ||                                    // 最下排: (4,0)
            (hex.r == -1 && (hex.q == 3 || hex.q == 5)) ||                   // 中間排: (3,-1), (5,-1)
            (hex.r == -2 && hex.q >= 2 && hex.q <= 6 && hex.q % 2 == 0));     // 最上排: (2,-2), (4,-2), (6,-2)

    case GREEN:
        // 綠隊目標：左上方三角形（基於獲勝範例的座標分析）
        // 範例中綠隊獲勝時位於: (-6,0), (-5,-1), (-3,-1), (-4,-2), (-2,0), (-1,-1)
        return ((hex.r == 0 && hex.q == -4) ||                                   // 最下排: (-4,0)
            (hex.r == -1 && (hex.q == -5 || hex.q == -3)) ||  // 中間排: (-5,-1), (-3,-1)
            (hex.r == -2 && (hex.q == -6 || hex.q == -2 || hex.q == -4)));  // 最上排: (-6,-2),(-4,-2), (-2,-2)

    default:
        return false;
    }
}