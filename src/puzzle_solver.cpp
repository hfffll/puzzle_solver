#include "puzzle_solver.h"
#include <sstream>
#include <functional>

PuzzleState::PuzzleState(const Board& b, 
                         std::shared_ptr<PuzzleState> p, 
                         const std::string& m, 
                         int g_val, 
                         int h_val)
    : board(b), parent(p), move(m), g(g_val), h(h_val), f(g_val + h_val) {}

bool PuzzleState::operator==(const PuzzleState& other) const {
    return board == other.board;
}

bool PuzzleState::operator<(const PuzzleState& other) const {
    if (f == other.f) {
        return h < other.h;
    }
    return f < other.f;
}

std::pair<int, int> PuzzleState::getBlankPosition() const {
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            if (board[i][j] == 0) {
                return {i, j};
            }
        }
    }
    return {-1, -1};
}

// 修复这里：使用 PuzzleState::Board 而不是 Board
std::vector<std::pair<std::string, PuzzleState::Board>> PuzzleState::getPossibleMoves() const {
    std::vector<std::pair<std::string, Board>> moves;
    auto [i, j] = getBlankPosition();

    if (i > 0) {
        Board newBoard = board;
        std::swap(newBoard[i][j], newBoard[i - 1][j]);
        moves.emplace_back("up", newBoard);
    }

    if (i < 2) {
        Board newBoard = board;
        std::swap(newBoard[i][j], newBoard[i + 1][j]);
        moves.emplace_back("down", newBoard);
    }

    if (j > 0) {
        Board newBoard = board;
        std::swap(newBoard[i][j], newBoard[i][j - 1]);
        moves.emplace_back("left", newBoard);
    }

    if (j < 2) {
        Board newBoard = board;
        std::swap(newBoard[i][j], newBoard[i][j + 1]);
        moves.emplace_back("right", newBoard);
    }

    return moves;
}

std::string PuzzleState::toString() const {
    std::stringstream ss;
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            ss << board[i][j] << " ";
        }
        ss << "\n";
    }
    return ss.str();
}

size_t PuzzleState::hash() const {
    size_t seed = 0;
    for (const auto& row : board) {
        for (int val : row) {
            seed ^= std::hash<int>{}(val) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        }
    }
    return seed;
}