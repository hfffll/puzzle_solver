#ifndef SEARCH_ALGORITHMS_H
#define SEARCH_ALGORITHMS_H

#include "puzzle_solver.h"
#include <vector>
#include <memory>
#include <chrono>

struct SearchResult {
    std::vector<std::pair<std::string, PuzzleState::Board>> path;
    int steps;
    int nodes_expanded;
    double time_taken;
    bool success;
    
    SearchResult() : steps(0), nodes_expanded(0), time_taken(0.0), success(false) {}
};

// 启发式函数类型
using HeuristicFunction = int(*)(const PuzzleState::Board&, const PuzzleState::Board&);

// 工具函数
bool isGoalState(const PuzzleState::Board& state, const PuzzleState::Board& goal);
std::vector<std::pair<std::string, PuzzleState::Board>> 
getSolutionPath(const std::shared_ptr<PuzzleState>& state);

// 启发式函数
int manhattanDistance(const PuzzleState::Board& board, const PuzzleState::Board& goal);
int misplacedTiles(const PuzzleState::Board& board, const PuzzleState::Board& goal);

// 搜索算法
SearchResult breadthFirstSearch(const PuzzleState::Board& initial, const PuzzleState::Board& goal);
SearchResult depthFirstSearch(const PuzzleState::Board& initial, const PuzzleState::Board& goal, int max_depth = 100);
SearchResult aStarSearch(const PuzzleState::Board& initial, const PuzzleState::Board& goal, HeuristicFunction heuristic);

#endif