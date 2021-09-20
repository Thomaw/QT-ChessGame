#ifndef BISHOP_CLASS_H
#define BISHOP_CLASS_H

#include <QList>

class Bishop_Class
{
public:
    Bishop_Class();

    bool autorize_move(int const, int const, int const, int const, char const, QVector<char> const);

private:
    bool normal_move(int const, int const, int const, int const);
    bool opposition_move(int const, int const, int const, int const, char const, QVector<char> const);
    bool check_eat(int const, char const, QVector<char> const);

    QList<QChar> const White_pieces {'R', 'N', 'B', 'Q', 'K', 'P'};
    QList<QChar> const Black_pieces {'r', 'n', 'b', 'q', 'k', 'p'};

};

#endif // BISHOP_CLASS_H
