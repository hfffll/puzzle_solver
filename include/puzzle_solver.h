#ifndef PUZZLE_SOLVER_H
#define PUZZLE_SOLVER_H

#include <vector>
#include <array>
#include <memory>
#include <string>

class PuzzleState{
public:
    using Board = std::array<std::array<int, 3>, 3>;  //起一个别名

    Board board;
    std::shared_ptr<PuzzleState> parent;     // 指向父状态的智能指针（用于回溯解路径）
    std::string move;                        // 从父状态到当前状态的移动方向
    int g;  // 实际代价
    int h;  // 启发式估计代价
    int f;  // 总代价

    PuzzleState(const Board& b, 
                std::shared_ptr<PuzzleState> p = nullptr, 
                const std::string& m = "", 
                int g_val = 0, 
                int h_val = 0);
    
    // 重载比较运算符
    bool operator==(const PuzzleState& other) const;
    bool operator<(const PuzzleState& other) const;

    // 获取空格位置
    std::pair<int, int> getBlankPosition() const;
    
    // 获取可能的移动
    std::vector<std::pair<std::string, Board>> getPossibleMoves() const;  //函数后加const 只读成员函数
    
    // 转换为字符串表示
    std::string toString() const;
    
    // 计算哈希值
    size_t hash() const;
};

// 哈希函数特化
namespace std {
    template<>
    struct hash<PuzzleState> {
        size_t operator()(const PuzzleState& state) const {
            return state.hash();
        }
    };
    
    template<>
    struct hash<std::shared_ptr<PuzzleState>> {
        size_t operator()(const std::shared_ptr<PuzzleState>& state) const {
            return state->hash();
        }
    };
    
    template<>
    struct equal_to<std::shared_ptr<PuzzleState>> {
        bool operator()(const std::shared_ptr<PuzzleState>& a, 
                       const std::shared_ptr<PuzzleState>& b) const {
            return *a == *b;
        }
    };
}

#endif
