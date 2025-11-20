#ifdef _WIN32
#include <windows.h>
#endif

#include "search_algorithms.h"
#include <iostream>
#include <iomanip>

void printSolution(const SearchResult& result, const std::string& algorithm_name, 
                   const PuzzleState::Board& initial_board) {
    std::cout << "\n" << algorithm_name << " 结果:\n";
    
    if (!result.success) {
        std::cout << "未找到解决方案\n";
        return;
    }
    
    std::cout << "解决方案步数: " << result.steps - 1 << "\n";
    std::cout << "扩展节点数: " << result.nodes_expanded << "\n";
    std::cout << "耗时: " << std::fixed << std::setprecision(6) << result.time_taken << " 秒\n";
    std::cout << "\n解决方案路径:\n";
    
    std::cout << "初始状态:\n";
    for (const auto& row : initial_board) {
        for (int val : row) {
            std::cout << val << " ";
        }
        std::cout << "\n";
    }
    std::cout << "\n";
    
    for (size_t i = 0; i < result.path.size(); ++i) {
        const auto& [move, board] = result.path[i];
        std::cout << "步骤 " << i + 1 << ": " << move << "\n";
        for (const auto& row : board) {
            for (int val : row) {
                std::cout << val << " ";
            }
            std::cout << "\n";
        }
        std::cout << "\n";
    }
}

void performanceTest() {
    // 更复杂的测试用例
    PuzzleState::Board hard_initial = {
        std::array<int, 3>{8, 6, 7},
        std::array<int, 3>{2, 5, 4},
        std::array<int, 3>{3, 0, 1}
    };
    
    PuzzleState::Board goal = {
        std::array<int, 3>{1, 2, 3},
        std::array<int, 3>{4, 5, 6},
        std::array<int, 3>{7, 8, 0}
    };
    
    std::cout << "性能测试 - 复杂八数码问题\n";
    std::cout << "初始状态:\n";
    for (const auto& row : hard_initial) {
        for (int val : row) {
            std::cout << val << " ";
        }
        std::cout << "\n";
    }
    
    auto bfs_result = breadthFirstSearch(hard_initial, goal);
    printSolution(bfs_result, "宽度优先搜索(BFS)", hard_initial);

    auto depth_result = depthFirstSearch(hard_initial,goal,100);
    printSolution(depth_result,"深度优先搜索(DFS)",hard_initial);
    
    auto A_star_result = aStarSearch(hard_initial,goal,manhattanDistance);
    printSolution(A_star_result,"A*)",hard_initial);
    // 可以继续测试其他算法...
}

int main() 
{
    #ifdef _WIN32
        // 设置控制台输出编码为 UTF-8
        SetConsoleOutputCP(65001);
    #endif

    // 设置控制台编码为 UTF-8
    // SetConsoleOutputCP(65001);

    PuzzleState::Board initial_board = {
        std::array<int, 3>{3, 2, 1},
        std::array<int, 3>{4, 0, 8},
        std::array<int, 3>{7, 5, 6}
    };
    
    PuzzleState::Board goal_board = {
        std::array<int, 3>{1, 2, 3},
        std::array<int, 3>{4, 5, 6},
        std::array<int, 3>{7, 8, 0}
    };
    
    std::cout << "八数码问题求解 (C++版本)\n";
    std::cout << "初始状态:\n";
    for (const auto& row : initial_board) {
        for (int val : row) {
            std::cout << val << " ";
        }
        std::cout << "\n";
    }
    
    std::cout << "\n目标状态:\n";
    for (const auto& row : goal_board) {
        for (int val : row) {
            std::cout << val << " ";
        }
        std::cout << "\n";
    }
    
    // 测试各种算法
    auto bfs_result = breadthFirstSearch(initial_board, goal_board);
    printSolution(bfs_result, "宽度优先搜索(BFS)", initial_board);

    auto depth_result = depthFirstSearch(initial_board,goal_board,100);
    printSolution(depth_result,"深度优先搜索(DFS)",initial_board);

    auto A_star_result_result = aStarSearch(initial_board,goal_board,manhattanDistance);
    printSolution(A_star_result_result,"A*",initial_board);
    
    //运行性能测试
    performanceTest();

    std::cout << "按回车键退出..." << std::endl;
    std::cin.get();
    
    return 0;
}