#pragma once

#include "ofxMSAmcts.h"
// 移除 ofMain.h 依赖（同时也要确认 ofxMSAmcts.h 内不依赖 ofMain.h，否则也需要相应修改）
#include <iostream>
#include <sstream>
#include <vector>
#include <cstdlib>
#include <cmath>

using namespace std;
using namespace msa::mcts;

namespace oxo {

//--------------------------------------------------------------
//--------------------------------------------------------------
struct Action {
public:
    Action(int t=0):tile(t) {}

    int tile;	// 落子位置，0..8，0：左上，8：右下
};

#define kNone		-1
#define kPlayer1	0
#define kPlayer2	1

//--------------------------------------------------------------
//--------------------------------------------------------------
class State {
public:

    //--------------------------------------------------------------
    // 必须实现的方法

    State() {
        reset();
    }

    // 判断是否终局
    bool is_terminal() const  {
        return data.is_terminal;
    }

    // 返回当前决策的玩家（从0开始）
    int agent_id() const {
        return data.player_turn;
    }

    // 在状态上应用动作
    void apply_action(const Action& action)  {
        // 简单检查：检查是否落在合法范围内
        if(action.tile < 0 || action.tile > 8) return;

        // 在棋盘上落子
        data.board[action.tile] = data.player_turn;

        // 交换玩家回合
        data.player_turn = 1 - data.player_turn;

        // 更新游戏状态：检查是否形成胜利组合
        static int win_combo[8][3] = { 
            {0,1,2}, {3,4,5}, {6,7,8}, 
            {0,3,6}, {1,4,7}, {2,5,8}, 
            {0,4,8}, {2,4,6} 
        };

        for (int i = 0; i < 8; i++) {
            if ( data.board[ win_combo[i][0] ] > kNone &&
                 data.board[ win_combo[i][0] ] == data.board[ win_combo[i][1] ] &&
                 data.board[ win_combo[i][1] ] == data.board[ win_combo[i][2] ] ) {
                data.winner = data.board[ win_combo[i][0] ];
                data.is_terminal = true;
                return;
            }
        }

        // 如果没有赢家，则检查是否没有空位
        int num_empty = 0;
        for (int i = 0; i < 9; i++)
            if(data.board[i] == kNone)
                num_empty++;
        if(num_empty == 0) {
            data.winner = kNone;
            data.is_terminal = true;
        }
    }

    // 返回所有可能的合法动作
    void get_actions(std::vector<Action>& actions) const  {
        actions.clear();
        for (int i = 0; i < 9; i++) {
            if(data.board[i] == kNone)
                actions.push_back(Action(i));
        }
    }

    // 随机返回一个动作，如果无可行动作返回 false
    bool get_random_action(Action& action) const {
        vector<Action> actions;
        get_actions(actions);

        if (actions.empty()) return false;

        // 使用 C 标准库的 rand，注意需先调用 srand 设置种子
        int index = rand() % actions.size();
        action = actions[index];
        return true;
    }

    // 对状态进行评估，返回每个玩家的奖励
    const vector<float> evaluate() const  {
        vector<float> rewards(2, 0.0);

        if(data.is_terminal) {
            switch(data.winner) {
            case kNone: // 平局
                rewards[kPlayer1] = rewards[kPlayer2] = 0.5;
                break;
            case kPlayer1: // 玩家1胜
                rewards[kPlayer1] = 1;
                rewards[kPlayer2] = 0;
                break;
            case kPlayer2: // 玩家2胜
                rewards[kPlayer1] = 0;
                rewards[kPlayer2] = 1;
                break;
            default:
                break;
            }
        } 
        else {
            rewards[kPlayer1] = rewards[kPlayer2] = 0.0;
        }
        return rewards;
    }

    // 返回当前状态的字符串描述（用于调试）
    std::string to_string() const  {
        stringstream str;
        str << "player_turn:" << player_to_string(data.player_turn) << "(" << data.player_turn << "), ";
        str << "is_terminal:" << data.is_terminal << ", ";
        str << "winner:" << player_to_string(data.winner) << "(" << data.winner << "), ";
        return str.str();
    }

    //--------------------------------------------------------------
    // 实现细节

    struct {
        int player_turn;	// 当前回合的玩家：kPlayer1 或 kPlayer2
        bool is_terminal;	// 是否已终局
        int winner;			// 胜利者：kNone（平局）、kPlayer1 或 kPlayer2
        int board[9];		// 棋盘状态：每个位置可能是 kNone, kPlayer1 或 kPlayer2
    } data;

    string player_to_string(int player_id) const {
        switch(player_id) {
        case kNone: return "None";
        case kPlayer1: return "Player 1";
        case kPlayer2: return "Player 2";
        default: return "";
        }
    }

    // 重置游戏状态
    void reset() {
        data.player_turn = kPlayer1;
        data.is_terminal = false;
        data.winner = kNone;
        for(int i = 0; i < 9; i++) {
            data.board[i] = kNone;
        }
    }

    // 修改后的 draw() 函数，在命令行打印棋盘状态
    void draw() {
        std::cout << "当前棋盘状态:" << std::endl;
        for (int j = 0; j < 3; j++) {
            for (int i = 0; i < 3; i++) {
                int idx = j * 3 + i;
                char mark;
                if(data.board[idx] == kPlayer1)
                    mark = 'O';
                else if(data.board[idx] == kPlayer2)
                    mark = 'X';
                else
                    mark = '.';
                std::cout << mark << " ";
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;
    }

    // 移除了与 openFrameworks 相关的绘图辅助函数
};

} // namespace oxo