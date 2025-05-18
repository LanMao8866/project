#include "board.h"
#include "Hex.h"
#include <iostream>
using namespace std;

Board::Board() {
    // 初始化紅方棋子 (左側三角形)
    for (int q = -4; q <= 0; q++)
        for (int r = 0; r <= 4; r++)
            if (q + r <= 0) pieces[{q, r}] = 'R';

    // 初始化藍方棋子 (右側三角形)
    for (int q = 0; q <= 4; q++)
        for (int r = -4; r <= 0; r++)
            if (q + r >= 0) pieces[{q, r}] = 'B';
}
//棋盤製作
void Board::print() const {
    for (int r = -4; r <= 4; r++) {
        string line(abs(r) * 2, ' ');
        for (int q = -4; q <= 4; q++) {
            Hex pos(q, r);
            if (pieces.count(pos)) line += pieces.at(pos);
            else if (abs(q) <= 4 && abs(r) <= 4 && abs(-q - r) <= 4) line += '.';
            else line += '.';
            line += " ";
        }
        cout << line << endl;
    }
}

bool Board::move(const Hex& from, const Hex& to) {
    // 基礎驗證
    if (!pieces.count(from)) return false;
    if (pieces.at(from) != currentPlayer) return false;
    if (pieces.count(to)) return false;

    // 計算移動距離
    int dq = to.q - from.q;
    int dr = to.r - from.r;
    int distance = abs(dq) + abs(dr) + abs(dq + dr);

    // 允許單步移動(距離2)
    if (distance == 2) {
        pieces[to] = currentPlayer;
        pieces.erase(from);
        currentPlayer = (currentPlayer == 'R') ? 'B' : 'R';
        return true;
    }
    return false;
}

bool Board::checkWin() const {
    // 檢查當前玩家所有棋子是否達到目標區域
    for (auto& [pos, player]: pieces) {
        if (player != currentPlayer) continue;
        if (currentPlayer == 'R' && (pos.q < 3 || pos.r > -3)) return false;
        if (currentPlayer == 'B' && (pos.q > -3 || pos.r < 3)) return false;
    }
    return true;
}
//查詢目前輪到誰
char Board::getCurrentPlayer() const {
    return currentPlayer;
}