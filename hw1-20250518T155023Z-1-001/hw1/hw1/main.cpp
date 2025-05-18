#include "board.h"
#include "Hex.h"
#include <iostream>
using namespace std;

int main() {
    Board game;

    while (true) {
        game.print();

        if (game.checkWin()) {
            cout << "玩家 " << (game.getCurrentPlayer() == 'R' ? "藍方" : "紅方")
                << " 獲勝了！\n";
            break;
        }

        cout << "玩家 " << (game.getCurrentPlayer() == 'R' ? "藍方" : "紅方")
            << " 請輸入移動 (from_q from_r to_q to_r)：";

        Hex from, to;
        cin >> from.q >> from.r >> to.q >> to.r;

        if (game.move(from, to)) {
            cout << "移動成功！\n";
        }
        else {
            cout << "無效的移動！請重新輸入。\n";
        }
    }
}
