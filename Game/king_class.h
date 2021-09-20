
#ifndef KING_CLASS_H
#define KING_CLASS_H

#include <QList>

class King_Class
{
public:
    King_Class();

    bool autorize_move(int const, int const, int const, int const, char const, QVector<char> const, QVector<int> const);

private:
    bool normal_move(int const, int const, int const, int const);
    bool opposition_move(int const, int const, char const, QVector<char> const);
    bool Castling(int const, int const, int const, int const, char const, QVector<char> const, QVector<int> const);

    QList<QChar> const White_pieces {'R', 'N', 'B', 'Q', 'K', 'P'};
    QList<QChar> const Black_pieces {'r', 'n', 'b', 'q', 'k', 'p'};

};

#endif // KING_CLASS_H
