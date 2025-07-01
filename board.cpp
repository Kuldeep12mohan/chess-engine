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
    Board() : board(8,vector<Piece>(8,{Color::NONE,PieceType::NONE}))
    {
        //pawns
        for(int j=0;j<8;++j)
        {
            board[1][j] = {Color::WHITE,PieceType::PAWN};
            board[6][j] = {Color::BLACK,PieceType::PAWN};
        }
        //rooks
        board[0][0] = board[0][7] = {Color::WHITE,PieceType::ROOK};
        board[7][0] = board[7][7] = {Color::BLACK,PieceType::ROOK};

        //knights
        board[0][1] = board[0][6] = {Color::WHITE,PieceType::KNIGHT};
        board[7][1] = board[7][6] = {Color::BLACK,PieceType::KNIGHT};

        //bishops
        board[0][2] = board[0][5] = {Color::WHITE,PieceType::BISHOP};
        board[7][2] = board[7][5] = {Color::BLACK,PieceType::BISHOP};

        //king
        board[0][3] = {Color::WHITE,PieceType::KING};
        board[7][3] = {Color::BLACK,PieceType::KING};

        //queens
        board[0][4] = {Color::WHITE,PieceType::QUEEN};
        board[7][4] = {Color::BLACK,PieceType::QUEEN};
    }
    bool insideBoard(int row,int col)
    {
        return(row < 8 && col < 8  && row >= 0 && col >= 0)?true:false;
    }
    bool emptySquare(int row,int col)
    {
        return(board[row][col].type == PieceType::NONE)?true:false;
    }
    vector<pair<int, int>> getLegalMoves(int row, int col)
    {
        vector<pair<int,int>> legalMoves;
        //white pawns
        if(board[row][col].type == PieceType::PAWN && board[row][col].color == Color::WHITE)
        {
            if(emptySquare(row+1,col) && insideBoard(row+1,col))legalMoves.push_back({row+1,col});

            if(row == 1 && emptySquare(row+1,col) && emptySquare(row+2,col))legalMoves.push_back({row+2,col});
        }
        //black pawns
        if(board[row][col].type == PieceType::PAWN && board[row][col].color == Color::BLACK)
        {
            if(insideBoard(row-1,col) && emptySquare(row-1,col))legalMoves.push_back({row-1,col});
            if(row == 6 && emptySquare(row-1,col) && emptySquare(row-2,col))legalMoves.push_back({row-2,col});
        }

        //rooks
        if(board[row][col].type == PieceType::ROOK)
        {
            //up
            for(int i=1;i<8;++i)
            {
                if(insideBoard(row+i,col) && emptySquare(row+i,col))legalMoves.push_back({row+i,col});
                else break;
            }
            //down
            for(int i=1;i<8;++i)
            {
                if(insideBoard(row-i,col) && emptySquare(row-i,col))legalMoves.push_back({row-i,col});
                else break;
            }

            //left
            for(int i=1;i<8;++i)
            {
                if(insideBoard(row,col+i) && emptySquare(row,col+i))legalMoves.push_back({row,col+i});
                else break;
            }
            
            //right
            for(int i=1;i<8;++i)
            {
                if(insideBoard(row,col-i) && emptySquare(row,col-i))legalMoves.push_back({row,col-i});
                else break;
            }
        }
        return legalMoves;

    }
};