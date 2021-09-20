#ifndef KNIGHT_CLASS_H
#define KNIGHT_CLASS_H

#include <QList>

class Knight_Class{

public:
    Knight_Class();
    bool autorize_move(int const, int const, int const, int const, char const, QVector<char> const);

private:
    bool normal_move(int const, int const, int const, int const);
    bool opposition_move(int const, int const, char const, QVector<char> const);

    QList<QChar> const White_pieces {'R', 'N', 'B', 'Q', 'K', 'P'};
    QList<QChar> const Black_pieces {'r', 'n', 'b', 'q', 'k', 'p'};

};

#endif // KNIGHT_CLASS_H
