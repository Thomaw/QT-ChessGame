#ifndef CHESSALGORITHM_H
#define CHESSALGORITHM_H

#include <QObject>

#include "king_class.h"
#include "queen_class.h"
#include "rook_class.h"
#include "bishop_class.h"
#include "pawn_class.h"
#include "knight_class.h"


class ChessBoard;
class ChessAlgorithm : public QObject
{
    Q_OBJECT
public:
    explicit ChessAlgorithm(QObject *parent = 0);
    bool accepted_movement(int const m_column, int const fromColumn, int const fromRank, int const toColumn, int const toRank, QVector<char> const);

    int GetRankIndex(int const index){return ((index-GetColumnIndex(index))/8);};
    int GetColumnIndex(int const index){return(index%8);};


    ChessBoard* board() const;


    bool Return_IsCheckMate(){return IsCheckMate;};
    bool SetIsCheckMate(bool const);
    bool MiseEnEchec(bool const, QVector<char> const);


    void CallIsCheckMate(bool const);


    QVector<int> all_possible_movement(bool const, QVector<char> const);
    QVector<int> Indexation_possible_movement(QVector<int> const);

    bool IsCheck(bool const start, QVector<char> const m_boardData, QVector<int> const Indexation_possible_movement);

    QVector<int> AsAlreadyMove(QVector<int>, int const, int const);
    QVector<int> AsMove;

    int KingPosition(QVector<char> const, bool const);

    QList<int> PositionPieces(bool const, QVector<char> const);

    QVector<char> AncienBoardEnPassantBlack;
    QVector<char> AncienBoardEnPassantWhite;

    QVector<QVector<int>> AllMovementsForPieces(bool, QVector<char>);

    QString FEN ="rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";


public slots:
    virtual void newGame(QString);

signals:
    void boardChanged(ChessBoard*);
    void CheckMateVariable(bool);

protected:
    virtual void setupBoard();
    void setBoard(ChessBoard *board);



private:
    ChessBoard* m_board;

    bool IsCheckMate = false;

    QList<QChar> const White_pieces {'R', 'N', 'B', 'Q', 'K', 'P'};
    QList<QChar> const Black_pieces {'r', 'n', 'b', 'q', 'k', 'p'};

    QVector<int> IndexationPossibleMove;


    King_Class *Black_King = new King_Class();
    King_Class *White_King = new King_Class();
    Queen_Class *Black_Queen = new Queen_Class();
    Queen_Class *White_Queen = new Queen_Class();
    Rook_Class *Black_Rook = new Rook_Class();
    Rook_Class *White_Rook = new Rook_Class();
    Knight_Class *Black_Knight = new Knight_Class();
    Knight_Class *White_Knight = new Knight_Class();
    Bishop_Class *Black_Bishop = new Bishop_Class();
    Bishop_Class *White_Bishop = new Bishop_Class();
    Pawn_Class *Black_Pawn = new Pawn_Class();
    Pawn_Class *White_Pawn = new Pawn_Class();

};

#endif // CHESSALGORITHM_H
