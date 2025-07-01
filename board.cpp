#include <bits/stdc++.h>
using namespace std;

enum class Color
{
    WHITE,
    BLACK,
    NONE
};
enum class PieceType
{
    KING,
    QUEEN,
    ROOK,
    BISHOP,
    KNIGHT,
    PAWN,
    NONE
};

struct Piece
{
    Color color;
    PieceType type;
};
class Board
{
    vector<vector<Piece>> board;
    bool check;
    bool completed;

public:
    Board() : board(8, vector<Piece>(8, {Color::NONE, PieceType::NONE}))
    {
        // pawns
        for (int j = 0; j < 8; ++j)
        {
            board[1][j] = {Color::WHITE, PieceType::PAWN};
            board[6][j] = {Color::BLACK, PieceType::PAWN};
        }
        // rooks
        board[0][0] = board[0][7] = {Color::WHITE, PieceType::ROOK};
        board[7][0] = board[7][7] = {Color::BLACK, PieceType::ROOK};

        // knights
        board[0][1] = board[0][6] = {Color::WHITE, PieceType::KNIGHT};
        board[7][1] = board[7][6] = {Color::BLACK, PieceType::KNIGHT};

        // bishops
        board[0][2] = board[0][5] = {Color::WHITE, PieceType::BISHOP};
        board[7][2] = board[7][5] = {Color::BLACK, PieceType::BISHOP};

        // king
        board[0][3] = {Color::WHITE, PieceType::KING};
        board[7][3] = {Color::BLACK, PieceType::KING};

        // queens
        board[0][4] = {Color::WHITE, PieceType::QUEEN};
        board[7][4] = {Color::BLACK, PieceType::QUEEN};
    }

    bool insideBoard(int row, int col)
    {
        return (row < 8 && col < 8 && row >= 0 && col >= 0) && true;
    }

    bool emptySquare(int row, int col)
    {
        return (board[row][col].type == PieceType::NONE) && true;
    }

    bool isOpponent(int row, int col, Color color)
    {
        return (board[row][col].color != color) && true;
    }

    void pawnCapture(int row, int col, Color color, vector<pair<int, int>> &legalMoves)
    {
        if (insideBoard(row + 1, col + 1) && isOpponent(row + 1, col + 1, color))
            legalMoves.push_back({row + 1, col + 1});
        if (insideBoard(row + 1, col - 1) && isOpponent(row + 1, col - 1, color))
            legalMoves.push_back({row + 1, col - 1});
    }
    vector<pair<int, int>> getLegalMoves(int row, int col)
    {
        vector<pair<int, int>> legalMoves;

        if (!insideBoard(row, col) || emptySquare(row, col))
            return legalMoves;

        PieceType type = board[row][col].type;
        Color color = board[row][col].color;
        // white pawns
        if (type == PieceType::PAWN && color == Color::WHITE)
        {
            if (insideBoard(row + 1, col) && emptySquare(row + 1, col))
                legalMoves.push_back({row + 1, col});

            if (row == 1 && emptySquare(row + 1, col) && emptySquare(row + 2, col))
                legalMoves.push_back({row + 2, col});

            // capture logic
            pawnCapture(row, col, color, legalMoves);
        }
        // black pawns
        if (type == PieceType::PAWN && color == Color::BLACK)
        {
            if (insideBoard(row - 1, col) && emptySquare(row - 1, col))
                legalMoves.push_back({row - 1, col});
            if (row == 6 && emptySquare(row - 1, col) && emptySquare(row - 2, col))
                legalMoves.push_back({row - 2, col});

            // capture logic
            pawnCapture(row, col, color, legalMoves);
        }

        // rooks
        if (type == PieceType::ROOK)
        {
            vector<pair<int, int>> moves = {{1, 0}, {0, 1}, {-1, 0}, {0, -1}};
            for (int j = 0; j < 4; ++j)
            {
                auto [dr, dc] = moves[j];
                for (int i = 1; i < 8; ++i)
                {
                    if (insideBoard(row + i*dr, col + i*dc) && emptySquare(row + i*dr, col + i*dc))
                        legalMoves.push_back({row + i*dr, col + i*dc});
                    // capture
                    else if (insideBoard(row + i*dr, col + i*dc) && isOpponent(row + i*dr , col + i*dc, color))
                    {
                        legalMoves.push_back({row + i*dr, col + i*dc});
                        break;
                    }
                    else
                        break;
                }
            }
        }

        // bishops
        if (type == PieceType::BISHOP)
        {
           vector<pair<int, int>> moves = {{1, 1}, {1, -1}, {-1, 1}, {-1, -1}};
            for (int j = 0; j < 4; ++j)
            {
                auto [dr, dc] = moves[j];
                for (int i = 1; i < 8; ++i)
                {
                    if (insideBoard(row + i*dr, col + i*dc) && emptySquare(row + i*dr, col + i*dc))
                        legalMoves.push_back({row + i*dr, col + i*dc});
                    // capture
                    else if (insideBoard(row + i*dr, col + i*dc) && isOpponent(row + i*dr , col + i*dc, color))
                    {
                        legalMoves.push_back({row + i*dr, col + i*dc});
                        break;
                    }
                    else
                        break;
                }
            }
        }

        // Knights

        if (type == PieceType::KNIGHT)
        {
            vector<pair<int, int>> moves = {{2, 1}, {-2, 1}, {2, -1}, {-2, -1}, {1, 2}, {1, -2}, {-1, 2}, {-1, -2}};

            for (int i = 0; i < 8; ++i)
            {
                auto [dr, dc] = moves[i];
                if (insideBoard(row + dr, col + dc) && emptySquare(row + dr, col + dc))
                    legalMoves.push_back({row + dr, col + dc});
            }
        }

        // Queen

        if (type == PieceType::QUEEN)
        {
            vector<pair<int, int>> moves = {{1, 0}, {0, 1}, {-1, 0}, {0, -1},{1, 1}, {1, -1}, {-1, 1}, {-1, -1}};
            for (int j = 0; j < 4; ++j)
            {
                auto [dr, dc] = moves[j];
                for (int i = 1; i < 8; ++i)
                {
                    if (insideBoard(row + i*dr, col + i*dc) && emptySquare(row + i*dr, col + i*dc))
                        legalMoves.push_back({row + i*dr, col + i*dc});
                    // capture
                    else if (insideBoard(row + i*dr, col + i*dc) && isOpponent(row + i*dr , col + i*dc, color))
                    {
                        legalMoves.push_back({row + i*dr, col + i*dc});
                        break;
                    }
                    else
                        break;
                }
            }
        }

        // King
        if (type == PieceType::KING)
        {
            vector<pair<int, int>> moves = {{0, 1}, {0, -1}, {1, 0}, {-1, 0}, {1, 1}, {1, -1}, {-1, 1}, {-1, -1}};
            for (int i = 0; i < 8; ++i)
            {
                auto [dr, dc] = moves[i];
                if (insideBoard(row + dr, col + dc) && emptySquare(row + dr, col + dc))
                    legalMoves.push_back({row + dr, col + dc});
            }
        }
        return legalMoves;
    }
};