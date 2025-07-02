#include <bits/stdc++.h>
#include <string>
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

struct Move
{
    int fromRow, fromCol;
    int toRow, toCol;
    Piece captured;
};

class Board
{
    vector<vector<Piece>> board;
    bool check;
    bool completed;
    Color turn;
    stack<Move> moveHistory;

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
        board[0][4] = {Color::WHITE, PieceType::KING};
        board[7][4] = {Color::BLACK, PieceType::KING};

        // queens
        board[0][3] = {Color::WHITE, PieceType::QUEEN};
        board[7][3] = {Color::BLACK, PieceType::QUEEN};

        // set turn
        turn = Color::WHITE;
        completed = false;
    }

    // helper functions
    bool insideBoard(int row, int col)
    {
        return (row < 8 && col < 8 && row >= 0 && col >= 0);
    }

    bool emptySquare(int row, int col)
    {
        return (board[row][col].type == PieceType::NONE);
    }

    bool isOpponent(int row, int col, Color color)
    {
        return (board[row][col].color != color && board[row][col].color != Color::NONE);
    }

    void toggleTurn()
    {
        turn = (turn == Color::WHITE) ? Color::BLACK : Color::WHITE;
    }

    void printBoard()
    {
        cout << "  a b c d e f g h\n";
        for (int i = 7; i >= 0; --i)
        {
            cout << i + 1 << " ";
            for (int j = 0; j < 8; ++j)
            {
                char c = '.';
                if (board[i][j].type != PieceType::NONE)
                {
                    switch (board[i][j].type)
                    {
                    case PieceType::PAWN:
                        c = 'P';
                        break;
                    case PieceType::ROOK:
                        c = 'R';
                        break;
                    case PieceType::KNIGHT:
                        c = 'N';
                        break;
                    case PieceType::BISHOP:
                        c = 'B';
                        break;
                    case PieceType::QUEEN:
                        c = 'Q';
                        break;
                    case PieceType::KING:
                        c = 'K';
                        break;
                    default:
                        break;
                    }
                    if (board[i][j].color == Color::BLACK)
                        c = tolower(c);
                }
                cout << c << " ";
            }
            cout << i + 1 << "\n";
        }
        cout << "  a b c d e f g h\n\n";
    }

    Piece getPiece(int row, int col)
    {
        return board[row][col];
    }

    bool isInCheck(Color color)
    {
        int kRow = -1, kCol = -1;
        for (int i = 0; i < 8; ++i)
        {
            bool flag = false;
            for (int j = 0; j < 8; ++j)
            {
                if (board[i][j].color == color && board[i][j].type == PieceType::KING)
                {
                    kRow = i, kCol = j;
                    flag = true;
                    break;
                }
            }
            if (flag)
                break;
        }

        for (int i = 0; i < 8; ++i)
        {
            for (int j = 0; j < 8; ++j)
            {
                // opponent pieces
                if (board[i][j].color != color && board[i][j].type != PieceType::NONE)
                {
                    auto legalMoves = getLegalMoves(i, j);
                    for (auto [r, c] : legalMoves)
                    {
                        if (r == kRow && c == kCol)
                        {
                            return true;
                        }
                    }
                }
            }
        }
        return false;
    }

    bool noLegalMoves(Color color)
    {
        for (int i = 0; i < 8; ++i)
        {
            for (int j = 0; j < 8; ++j)
            {
                if (board[i][j].color == color)
                {
                    auto legalMoves = getLegalMoves(i, j);

                    for (auto [r, c] : legalMoves)
                    {
                        Piece tempFrom = board[i][j];
                        Piece tempTo = board[r][c];

                        // simulating a move

                        board[r][c] = board[i][j];
                        board[i][j] = {Color::NONE, PieceType::NONE};

                        bool illegal = isInCheck(color);
                        board[i][j] = tempFrom;
                        board[r][c] = tempTo;
                        if (!illegal)
                            return false;
                    }
                }
            }
        }
        return true;
    }

    bool isGameOver()
    {
        return completed;
    }

    bool getTurn()
    {
        if (turn == Color::WHITE)
            return true;
        else
            return false;
    }
    void makeMove(int fromRow, int fromCol, int toRow, int toCol)
    {
        Move m = {fromRow, fromCol, toRow, toCol, board[toRow][toCol]};
        moveHistory.push(m);

        board[toRow][toCol] = board[fromRow][fromCol];            // move piece
        board[fromRow][fromCol] = {Color::NONE, PieceType::NONE}; // empty the source
        toggleTurn();
    }

    void undoMove()
    {
        if (moveHistory.empty())
            return;
        int fromRow, fromCol, toRow, toCol;
        Piece captured;
        Move m = moveHistory.top();
        fromRow = m.fromRow;
        fromCol = m.fromCol;
        toRow = m.toRow;
        toCol = m.toCol;
        captured = m.captured;
        moveHistory.pop();
        board[fromRow][fromCol] = board[toRow][toCol];
        board[toRow][toCol] = captured;
        toggleTurn();
    }
    // pawn logic
    void pawnCapture(int row, int col, Color color, vector<pair<int, int>> &legalMoves)
    {
        int dr;
        if (color == Color::WHITE)
            dr = 1;
        else
            dr = -1;
        if (insideBoard(row + dr, col + 1) && isOpponent(row + dr, col + 1, color))
            legalMoves.push_back({row + dr, col + 1});
        if (insideBoard(row + dr, col - 1) && isOpponent(row + dr, col - 1, color))
            legalMoves.push_back({row + dr, col - 1});
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
                    if (insideBoard(row + i * dr, col + i * dc) && emptySquare(row + i * dr, col + i * dc))
                        legalMoves.push_back({row + i * dr, col + i * dc});
                    // capture
                    else if (insideBoard(row + i * dr, col + i * dc) && isOpponent(row + i * dr, col + i * dc, color))
                    {
                        legalMoves.push_back({row + i * dr, col + i * dc});
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
                    if (insideBoard(row + i * dr, col + i * dc) && emptySquare(row + i * dr, col + i * dc))
                        legalMoves.push_back({row + i * dr, col + i * dc});
                    // capture
                    else if (insideBoard(row + i * dr, col + i * dc) && isOpponent(row + i * dr, col + i * dc, color))
                    {
                        legalMoves.push_back({row + i * dr, col + i * dc});
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
                if (insideBoard(row + dr, col + dc) && (emptySquare(row + dr, col + dc) || isOpponent(row + dr, col + dc, color)))
                    legalMoves.push_back({row + dr, col + dc});
            }
        }

        // Queen

        if (type == PieceType::QUEEN)
        {
            vector<pair<int, int>> moves = {{1, 0}, {0, 1}, {-1, 0}, {0, -1}, {1, 1}, {1, -1}, {-1, 1}, {-1, -1}};
            for (int j = 0; j < 8; ++j)
            {
                auto [dr, dc] = moves[j];
                for (int i = 1; i < 8; ++i)
                {
                    if (insideBoard(row + i * dr, col + i * dc) && emptySquare(row + i * dr, col + i * dc))
                        legalMoves.push_back({row + i * dr, col + i * dc});
                    // capture
                    else if (insideBoard(row + i * dr, col + i * dc) && isOpponent(row + i * dr, col + i * dc, color))
                    {
                        legalMoves.push_back({row + i * dr, col + i * dc});
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
                if (insideBoard(row + dr, col + dc) && (emptySquare(row + dr, col + dc) || isOpponent(row + dr, col + dc, color)))
                    legalMoves.push_back({row + dr, col + dc});
            }
        }
        return legalMoves;
    }

    vector<Move> generateAllMoves(Color color)
    {
        vector<Move> result;

        for (int i = 0; i < 8; ++i)
        {
            for (int j = 0; j < 8; ++j)
            {
                if (board[i][j].color == color)
                {
                    auto legalMoves = getLegalMoves(i, j);
                    for (auto [r, c] : legalMoves)
                    {
                        Piece captured = board[r][c];
                        makeMove(i, j, r, c);
                        bool legal = isInCheck(color);
                        undoMove();
                        if (!legal)
                            result.push_back({i, j, r, c, captured});
                    }
                }
            }
        }
        return result;
    }
    void movePiece(int fromRow, int fromCol, int toRow, int toCol)
    {
        if (insideBoard(fromRow, fromCol) && board[fromRow][fromCol].type != PieceType::NONE && board[fromRow][fromCol].color == turn)
        {
            auto legalMoves = getLegalMoves(fromRow, fromCol);

            bool legal = false;

            for (auto [r, c] : legalMoves)
            {
                if (r == toRow && c == toCol)
                {
                    legal = true;
                    break;
                }
            }
            if (!legal)
                return;
            if (!insideBoard(toRow, toCol))
                return;

            Piece tempFrom = board[fromRow][fromCol];
            Piece tempTo = board[toRow][toCol];

            // simulating a move

            board[toRow][toCol] = board[fromRow][fromCol];
            board[fromRow][fromCol] = {Color::NONE, PieceType::NONE};

            bool illegal = isInCheck(turn);
            if (illegal)
            {
                board[fromRow][fromCol] = tempFrom;
                board[toRow][toCol] = tempTo;
                return;
            }
            toggleTurn();
        }
        checkGameStatus();
    }

    void checkGameStatus()
    {
        if (noLegalMoves(turn))
        {
            if (isInCheck(turn))
            {
                cout << "Checkmate" << endl;
                completed = true;
                cout << ((turn == Color::WHITE) ? "Black" : "White") << " wins!" << endl;
            }
            else
            {
                cout << "Stalemete" << endl;
                completed = true;
            }
        }
    }
    int eval()
    {
        int res = 0;
        for (int i = 0; i < 8; ++i)
        {
            for (int j = 0; j < 8; ++j)
            {
                PieceType type = board[i][j].type;
                Color color = board[i][j].color;
                // pawns
                if (type == PieceType::PAWN)
                {
                    if (color == Color::WHITE)
                        res += 1;
                    if (color == Color::BLACK)
                        res -= 1;
                }
                // knight/bishops
                if (type == PieceType::BISHOP || type == PieceType::KNIGHT)
                {
                    if (color == Color::WHITE)
                        res += 3;
                    if (color == Color::BLACK)
                        res -= 3;
                }
                // rook
                if (type == PieceType::ROOK)
                {
                    if (color == Color::WHITE)
                        res += 5;
                    if (color == Color::BLACK)
                        res -= 5;
                }
                // queen
                if (type == PieceType::QUEEN)
                {
                    if (color == Color::WHITE)
                        res += 9;
                    if (color == Color::BLACK)
                        res -= 9;
                }
                // king
                if (type == PieceType::KING)
                {
                    if (color == Color::WHITE)
                        res += 100;
                    if (color == Color::BLACK)
                        res -= 100;
                }
            }
        }
        if (noLegalMoves(Color::WHITE) && isInCheck(Color::WHITE))
            return -10000;
        if (noLegalMoves(Color::BLACK) && isInCheck(Color::BLACK))
            return +10000;
        return res;
    }
};
pair<Move, int> minimax(Board &board, int depth, bool maximizingPlayer)
{
    if (depth == 0 || board.isGameOver())
    {
        return {{-1, -1, -1, -1, {Color::NONE, PieceType::NONE}}, board.eval()};
    }

    Color color = maximizingPlayer ? Color::WHITE : Color::BLACK;
    auto moves = board.generateAllMoves(color);

    pair<Move, int> bestMove;
    bestMove.second = maximizingPlayer ? INT_MIN : INT_MAX;

    for (const Move &move : moves)
    {
        board.makeMove(move.fromRow, move.fromCol, move.toRow, move.toCol);
        int eval = minimax(board, depth - 1, !maximizingPlayer).second;
        board.undoMove();

        if (maximizingPlayer && eval > bestMove.second)
        {
            bestMove = {move, eval}; // 💡 store current move, not the recursive one
        }
        if (!maximizingPlayer && eval < bestMove.second)
        {
            bestMove = {move, eval}; // 💡 store current move
        }
    }

    return bestMove;
}

int main()
{
    Board board;

    cout << "------- Chess mini‑engine -------\n";
    cout << "Press 1 to start: ";
    int start;
    cin >> start;
    if (start != 1)
        return 0;

    while (!board.isGameOver())
    {
        /* -------- PLAYER TURN -------- */
        board.printBoard();

        cout << "Your move (e.g. e2 e4): ";
        string from, to;
        cin >> from >> to;

        int fromRow = from[1] - '1';
        int fromCol = from[0] - 'a';
        int toRow = to[1] - '1';
        int toCol = to[0] - 'a';

        board.movePiece(fromRow, fromCol, toRow, toCol);

        if (board.isGameOver()) 
        {
            board.printBoard();
            break;
        }

        /* -------- ENGINE TURN -------- */
        cout << "\nEngine thinking …\n";
        auto best = minimax(board, 3, board.getTurn());

        board.makeMove(best.first.fromRow,
                       best.first.fromCol,
                       best.first.toRow,
                       best.first.toCol);

        board.printBoard(); 
        Piece moved = board.getPiece(best.first.toRow, best.first.toCol);
        const char *names[] = {
            "king", "queen", "rook", "bishop", "knight", "pawn", "none"};
        cout << names[static_cast<int>(moved.type)]
             << " to " << char('a' + best.first.toCol) << best.first.toRow + 1
             << "  (eval = " << best.second << ")\n\n";
    }

    cout << "Game over!\n";
}
