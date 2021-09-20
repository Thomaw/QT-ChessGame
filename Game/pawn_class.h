#ifndef PAWN_CLASS_H
#define PAWN_CLASS_H

#include <QList>

class Pawn_Class
{
public:
    Pawn_Class();

    bool autorize_move(int, int, int, int, char, QVector<char>, QVector<char>);

    QVector<char> AncienBoardEnPassantBlack;
    QVector<char> AncienBoardEnPassantWhite;

private:
    bool normal_move(int, int, int, int, char);
    bool opposition_move(int, int, int, int, char, QVector<char>);
    bool eat_move(int, int, int, int, char, QVector<char>, QVector<char>);


    QList<QChar> const White_pieces {'R', 'N', 'B', 'Q', 'K', 'P'};
    QList<QChar> const Black_pieces {'r', 'n', 'b', 'q', 'k', 'p'};

};

#endif // PAWN_CLASS_H
