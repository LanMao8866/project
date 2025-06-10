#include "Board.h"           // 包含棋盤類別的標頭檔
#include <iostream>           // 標準輸入輸出串流
#include <limits>             // 數值極限定義
#include <windows.h>          // Windows API函數
#include <string>             // 字串類別
using namespace std;          // 使用標準命名空間

// 取得六角座標輸入的函數
Hex getHexInput(const string& prompt) {
    Hex h;                    // 宣告六角座標物件
    while (true) {            // 無限迴圈直到輸入正確
        cout << prompt;       // 顯示提示訊息
        if (cin >> h.q >> h.r) return h;  // 嘗試讀取q和r座標，成功則返回
        cin.clear();          // 清除輸入串流的錯誤狀態
        cin.ignore((numeric_limits<streamsize>::max)(), '\n');  // 忽略輸入緩衝區直到換行符
        cout << "Wrong,please re-enter.\n";  // 顯示錯誤訊息
    }
}

// 將隊伍字元轉換為隊伍名稱的函數
string getTeamName(char team) {
    switch (team) {           // 根據隊伍字元判斷
    case Board::RED: return "Red Team";      // 紅隊
    case Board::BLUE: return "Blue Team";    // 藍隊
    case Board::GREEN: return "Green Team";  // 綠隊
    default: return "Unknown";               // 未知隊伍
    }
}

// 獲取用戶是否/否選擇的函數
bool getUserChoice(const string& prompt) {
    string input;             // 宣告輸入字串
    while (true) {            // 無限迴圈直到輸入有效選項
        cout << prompt << " (y/n): ";  // 顯示提示訊息和選項格式
        cin >> input;         // 讀取用戶輸入
        if (input == "y" || input == "Y" || input == "yes" || input == "Yes") {  // 檢查肯定回答
            return true;      // 返回真值
        }
        else if (input == "n" || input == "N" || input == "no" || input == "No") {  // 檢查否定回答
            return false;     // 返回假值
        }
        else {                // 輸入無效
            cout << "Please enter 'y' for yes or 'n' for no.\n";  // 提示正確輸入格式
        }
    }
}

int main() {                  // 主函數開始
    SetConsoleOutputCP(65001);  // 設定控制台輸出編碼為UTF-8
    Board game;               // 建立棋盤遊戲物件
    int turn = 0;             // 初始化回合數

    while (true) {            // 主遊戲迴圈
        system("cls");        // 清除螢幕畫面
        cout << "----- Turn " << ++turn << " -----\n";  // 顯示當前回合數（先遞增）
        cout << game.toString();  // 顯示棋盤狀態

        // 檢查遊戲是否結束
        if (game.checkWin()) {
            char winner = game.getWinner();  // 取得獲勝隊伍
            cout << "\n*** GAME OVER ***\n";  // 顯示遊戲結束訊息
            cout << getTeamName(winner) << " Wins!\n";  // 顯示獲勝隊伍
            cout << "Congratulations!\n";  // 顯示恭喜訊息
            break;            // 跳出主迴圈
        }

        cout << "Current Player: " << getTeamName(game.getCurrentPlayer()) << "\n";  // 顯示當前玩家

        // 檢查是否處於連續跳躍狀態
        if (game.isInJumpSequence()) {
            Hex mustMove = game.getMustMoveFrom();  // 取得必須移動的棋子位置
            cout << "*** JUMP SEQUENCE: You must continue jumping with the piece at ("
                << mustMove.q << "," << mustMove.r << ") ***\n";  // 顯示連續跳躍提示

            // 提供選項選單
            cout << "Options:\n";
            cout << "1. Continue jumping\n";        // 選項1：繼續跳躍
            cout << "2. Stop jumping and end turn\n";  // 選項2：停止跳躍並結束回合

            bool continueJumping = getUserChoice("Do you want to continue jumping?");  // 取得用戶選擇

            if (!continueJumping) {  // 如果選擇不繼續跳躍
                game.stopJumpSequence();  // 停止跳躍序列
                cout << "Jump sequence stopped. Turn ended.\n";  // 顯示停止訊息
                cout << "Press Enter to continue...";  // 提示按Enter繼續
                cin.ignore((numeric_limits<streamsize>::max)(), '\n');  // 清除輸入緩衝區
                cin.get();        // 等待用戶按Enter
                continue;         // 繼續下一次迴圈
            }
        }

        Hex from = getHexInput("Enter the coordinates of the chess piece to be moved (q r): ");  // 取得起始位置
        Hex to = getHexInput("Enter the target location (q r): ");  // 取得目標位置

        if (game.move(from, to)) {  // 嘗試執行移動
            cout << "Move success!\n";  // 顯示移動成功訊息

            // 移動成功後立即檢查勝利條件
            if (game.checkWin()) {   // 檢查是否有隊伍獲勝
                cout << "Press Enter to see the final result...";  // 提示按Enter查看結果
                cin.ignore((numeric_limits<streamsize>::max)(), '\n');  // 清除輸入緩衝區
                cin.get();       // 等待用戶按Enter
                continue;        // 回到主迴圈顯示勝利訊息
            }
        }
        else {                   // 移動失敗
            cout << "Invalid Move!\n";  // 顯示無效移動訊息
            if (game.isInJumpSequence()) {  // 如果處於跳躍序列中
                Hex mustMove = game.getMustMoveFrom();  // 取得必須移動的棋子位置
                cout << "You must continue jumping with the piece at ("
                    << mustMove.q << "," << mustMove.r << ")!\n";  // 提示必須繼續跳躍
            }
        }

        cout << "Press Enter to continue...";  // 提示按Enter繼續
        cin.ignore((numeric_limits<streamsize>::max)(), '\n');  // 清除輸入緩衝區
        cin.get();               // 等待用戶按Enter
    }

    cout << "Press Enter to exit...";  // 提示按Enter退出
    cin.ignore((numeric_limits<streamsize>::max)(), '\n');  // 清除輸入緩衝區
    cin.get();                   // 等待用戶按Enter
    return 0;                    // 程式正常結束
}