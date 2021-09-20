#ifndef ENGINE_H
#define ENGINE_H

#include <QObject>
#include "board.h"
#include "search.h"


namespace Chess{

class Engine : public QObject{
    Q_OBJECT

public:
    Board board;
    Search minimax;

public:
    explicit Engine(QObject *parent = 0);
    void setBoard(Board newBoard);

public slots:
    void userMoved(Chess::Move userMove, int);
    void makeMove(Chess::Move move);
    void setPiece(Chess::Coord coord, Chess::Piece piece);
    void setappli(int, int, int, int);

signals:
    void boardapplichanged(int, int, int, int);
};

} // Namespace Chess

#endif // ENGINE_H
