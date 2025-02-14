#include "OXOState.h"
#include "ofxMSAmcts.h"  // 保留此文件依赖，如果其中没有 openFrameworks 部分
#include <iostream>
#include <sstream>
#include <cmath>
#include <cstdlib>
#include <ctime>

using namespace std;
using namespace oxo;
using namespace msa::mcts;

int main() {
    // 初始化随机数种子
    srand(static_cast<unsigned int>(time(NULL)));

    State state;
    Action action;
    UCT<State, Action> uct;

    // 配置 MCTS 算法参数
    uct.uct_k = sqrt(2);
    uct.max_millis = 0;          // 0 表示不限时
    uct.max_iterations = 1000;   // 最大迭代次数
    uct.simulation_depth = 1000; // 模拟深度

    cout << "欢迎来到命令行版井字棋（OXO）！" << endl;
    cout << "玩家 (O) 与 电脑 (X) 对战，玩家先手。" << endl;
    cout << "棋盘位置编号：0-8（从左上角开始，从左到右、从上到下）" << endl << endl;

    // 游戏循环：直到游戏结束
    while(!state.is_terminal()) {
        state.draw();
        cout << state.to_string() << endl;

        // 玩家回合：kPlayer1
        if(state.agent_id() == kPlayer1) {
            cout << "请输入你的落子位置 (0-8): ";
            int tile;
            cin >> tile;
            if(tile < 0 || tile > 8) {
                cout << "无效位置，请重新输入！" << endl;
                continue;
            }
            if(state.data.board[tile] != kNone) {
                cout << "该位置已有棋子，请重新选择！" << endl;
                continue;
            }
            state.apply_action(Action(tile));
        }
        // 电脑回合：kPlayer2，由 MCTS 算法决策
        else {
            cout << "电脑正在思考..." << endl;
            action = uct.run(state);
            cout << "电脑选择落子位置: " << action.tile << endl;
            state.apply_action(action);
        }
    }

    // 游戏结束，打印最终状态和结果
    state.draw();
    cout << "游戏结束！" << endl;
    cout << state.to_string() << endl;

    return 0;
}