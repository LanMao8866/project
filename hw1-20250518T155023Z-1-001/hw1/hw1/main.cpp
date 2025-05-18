#include "board.h"
#include "Hex.h"
#include <iostream>
using namespace std;

int main() {
    Board game;

    while (true) {
        game.print();

        if (game.checkWin()) {
            cout << "���a " << (game.getCurrentPlayer() == 'R' ? "�Ť�" : "����")
                << " ��ӤF�I\n";
            break;
        }

        cout << "���a " << (game.getCurrentPlayer() == 'R' ? "�Ť�" : "����")
            << " �п�J���� (from_q from_r to_q to_r)�G";

        Hex from, to;
        cin >> from.q >> from.r >> to.q >> to.r;

        if (game.move(from, to)) {
            cout << "���ʦ��\�I\n";
        }
        else {
            cout << "�L�Ī����ʡI�Э��s��J�C\n";
        }
    }
}
