#include "search_algorithms.h"
#include <queue>
#include <stack>
#include <unordered_set>
#include <functional>
#include <iostream>

bool isGoalState(const PuzzleState::Board& state, const PuzzleState::Board& goal) {
    return state == goal;
}

std::vector<std::pair<std::string, PuzzleState::Board>> 
getSolutionPath(const std::shared_ptr<PuzzleState>& state) {
    std::vector<std::pair<std::string, PuzzleState::Board>> path;
    auto current = state;
    
    while (current) {
        if (!current->move.empty()) {
            path.emplace_back(current->move, current->board);
        }
        current = current->parent;
    }
    
    std::reverse(path.begin(), path.end());
    return path;
}

int manhattanDistance(const PuzzleState::Board& board, const PuzzleState::Board& goal) {
    int distance = 0;
    std::array<std::pair<int, int>, 9> goal_positions;
    
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            goal_positions[goal[i][j]] = {i, j};
        }
    }
    
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            if (board[i][j] != 0) {
                auto [goal_i, goal_j] = goal_positions[board[i][j]];
                distance += std::abs(i - goal_i) + std::abs(j - goal_j);
            }
        }
    }
    return distance;
}

int misplacedTiles(const PuzzleState::Board& board, const PuzzleState::Board& goal) {
    int count = 0;
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            if (board[i][j] != 0 && board[i][j] != goal[i][j]) {
                ++count;
            }
        }
    }
    return count;
}

//宽度优先算法
SearchResult breadthFirstSearch(const PuzzleState::Board& initial, const PuzzleState::Board& goal) {
    SearchResult result;
    auto start_time = std::chrono::high_resolution_clock::now();
    
    auto initial_state = std::make_shared<PuzzleState>(initial);
    
    if (isGoalState(initial, goal)) {
        result.path = {{"start", initial}};
        result.steps = 1;
        result.nodes_expanded = 0;
        result.success = true;
        
        auto end_time = std::chrono::high_resolution_clock::now();
        result.time_taken = std::chrono::duration<double>(end_time - start_time).count();
        return result;
    }
    
    std::queue<std::shared_ptr<PuzzleState>> queue;
    std::unordered_set<std::shared_ptr<PuzzleState>> visited;
    
    queue.push(initial_state);
    visited.insert(initial_state);
    
    while (!queue.empty()) {
        auto current_state = queue.front();
        queue.pop();
        result.nodes_expanded++;
        
        for (const auto& [move, new_board] : current_state->getPossibleMoves()) {
            auto new_state = std::make_shared<PuzzleState>(new_board, current_state, move);
            size_t new_hash = new_state->hash();

            if (isGoalState(new_board, goal)) {
                result.path = getSolutionPath(new_state);
                result.steps = result.path.size();
                result.success = true;
                
                auto end_time = std::chrono::high_resolution_clock::now();
                result.time_taken = std::chrono::duration<double>(end_time - start_time).count();
                return result;
            }
            
            if (visited.find(new_state) == visited.end()) {
                visited.insert(new_state);
                queue.push(new_state);
            }
        }
    }
    
    auto end_time = std::chrono::high_resolution_clock::now();
    result.time_taken = std::chrono::duration<double>(end_time - start_time).count();
    return result;
}

//深度优先算法
SearchResult depthFirstSearch(const PuzzleState::Board& initial, const PuzzleState::Board& goal, int max_depth)
{
    SearchResult result;
    auto start_time = std::chrono::high_resolution_clock::now();

    auto initial_state = std::make_shared<PuzzleState>(initial);

    if (isGoalState(initial,goal))
    {
        result.path = {{"start",initial}};
        result.steps = 1;
        result.nodes_expanded = 0;
        result.success = true;

        auto end_time = std::chrono::high_resolution_clock::now();
        result.time_taken = std::chrono::duration<double>(end_time - start_time).count();
        return result;
    }

    //深度优先算法使用栈来进行
    std::stack<std::pair<std::shared_ptr<PuzzleState>,int>> stack;   //存储状态和当前深度
    std::unordered_set<std::shared_ptr<PuzzleState>> visited;

    stack.push(std::make_pair(initial_state,0));
    visited.insert(initial_state);

    std::cout << "开始深度优先搜索 (最大深度: " << max_depth << ")..." << std::endl;

    while (!stack.empty())
    {
        auto [current_state,depth] = stack.top();
        stack.pop();
        result.nodes_expanded++;

        //检查是否达到目标
        if (isGoalState(current_state->board,goal))
        {
            std::cout << "\n找到解决方案!" << std::endl;
            result.path = getSolutionPath(current_state);
            result.steps = result.path.size();
            result.success = true;
            
            auto end_time = std::chrono::high_resolution_clock::now();
            result.time_taken = std::chrono::duration<double>(end_time - start_time).count();
            return result;
        }

        //如果未达到最大深度，继续扩展
        if (depth < max_depth)
        {
            auto moves = current_state->getPossibleMoves();

            // 为了保持搜索顺序的一致性，我们可以反转移动列表
            // 这样在栈中弹出的顺序就是原始顺序
            std::reverse(moves.begin(), moves.end());

            for (const auto& [move, new_board] : moves) {
                auto new_state = std::make_shared<PuzzleState>(new_board, current_state, move);
                
                if (visited.find(new_state) == visited.end()) {
                    visited.insert(new_state);
                    stack.push(std::make_pair(new_state, depth + 1));
                }
            }

        }

    }
    std::cout << "搜索完成，未找到解决方案" << std::endl;
    auto end_time = std::chrono::high_resolution_clock::now();
    result.time_taken = std::chrono::duration<double>(end_time - start_time).count();
    return result;

}

SearchResult aStarSearch(const PuzzleState::Board& initial, const PuzzleState::Board& goal, HeuristicFunction heuristic)
{
    SearchResult result;
    auto start_time = std::chrono::high_resolution_clock::now();

    // 计算初始状态的启发式值
    int initial_h = heuristic(initial, goal);
    auto initial_state = std::make_shared<PuzzleState>(initial, nullptr, "", 0, initial_h);

    if (isGoalState(initial, goal)) {
        result.path = {{"start", initial}};
        result.steps = 1;
        result.nodes_expanded = 0;
        result.success = true;
        
        auto end_time = std::chrono::high_resolution_clock::now();
        result.time_taken = std::chrono::duration<double>(end_time - start_time).count();
        return result;
    }

    // 优先队列（最小堆），按f值排序
    auto compare = [](const std::shared_ptr<PuzzleState>& a, const std::shared_ptr<PuzzleState>& b) {
        return a->f > b->f; // 最小堆，所以用大于号
    };

    //将f根据从小到大进行优先队列排序
    std::priority_queue<
                        // 模板参数1：队列中存储的元素类型
                        std::shared_ptr<PuzzleState>, 
                         // 模板参数2：底层存储容器
                        std::vector<std::shared_ptr<PuzzleState>>, 
                        // 模板参数3：比较函数的类型
                        decltype(compare)
                        > open_list(compare);  // 构造函数：传入比较函数的实例

    // 记录已访问状态及其g值
    std::unordered_map<size_t, int> visited_f_values;  //哈希表键值对
    
    open_list.push(initial_state);
    visited_f_values[initial_state->hash()] = initial_state->f;   //初始时g为0

    while (!open_list.empty()) {
        // 1. 取出open表中f值最小的状态（当前最优状态）
        auto current_state = open_list.top();
        open_list.pop();
        result.nodes_expanded++;

        // 如果这个状态已经不是最佳路径了，跳过它
        size_t current_hash = current_state->hash();
        if (visited_f_values.find(current_hash) != visited_f_values.end() && 
            current_state->f > visited_f_values[current_hash]) {
            continue;
        }

        // 检查是否达到目标
        if (isGoalState(current_state->board, goal)) {
            std::cout << "\n找到解决方案!" << std::endl;
            result.path = getSolutionPath(current_state);
            result.steps = result.path.size();
            result.success = true;
            
            auto end_time = std::chrono::high_resolution_clock::now();
            result.time_taken = std::chrono::duration<double>(end_time - start_time).count();
            return result;
        }
        
        // 扩展当前状态
        for (const auto& [move, new_board] : current_state->getPossibleMoves()) {
            // 计算新状态的g值（比父状态多一步）
            int new_g = current_state->g + 1;
            
            // 计算新状态的启发式值
            int new_h = heuristic(new_board, goal);

            // 计算新状态的f值
            int new_f = new_g + new_h;
            
            // 创建新状态
            auto new_state = std::make_shared<PuzzleState>(new_board, current_state, move, new_g, new_h);
            
            // 检查是否已访问过该状态，或者找到了更优路径
            size_t state_hash = new_state->hash();
            auto visited_it = visited_f_values.find(state_hash);
            
            // 条件：要么没访问过，要么访问过但当前路径的g值更小（更优）
            if (visited_it == visited_f_values.end() || new_f < visited_it->second) {
                // 如果未访问过或找到更优路径
                visited_f_values[state_hash] = new_f;
                open_list.push(new_state);
            }
        }
    }
    std::cout << "搜索完成，未找到解决方案" << std::endl;
    auto end_time = std::chrono::high_resolution_clock::now();
    result.time_taken = std::chrono::duration<double>(end_time - start_time).count();
    return result;
}