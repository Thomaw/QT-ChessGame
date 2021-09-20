#ifndef IA_H
#define IA_H


#include "chessalgorithm.h"
#include "QVector"

class IA{

public:
    IA();

    int EvaluateBoard(QVector<char> const, bool const);
    QList<int> BestMoveCalculDepthPruning(QVector<char>, int, bool, int, int);


private:

//    void setDataIA(int, int, char);
    void setDataInternalIA(QVector<char> &, int const, int const, char const);

    char dataIA(QVector<char> const, int const, int const) const;


    QList<int> FIndBlack(QVector<char> const);
    QList<int> FIndWhite(QVector<char> const);

    ChessAlgorithm* m_algorithmIA = new ChessAlgorithm;

    bool start;

    QList<QChar> const White_pieces {'R', 'N', 'B', 'Q', 'K', 'P'};
    QList<QChar> const Black_pieces {'r', 'n', 'b', 'q', 'k', 'p'};
    QList<int> const Values {525, 350, 350, 1000, 10000, 100};

    QVector<int> MovementsIndexation(QVector<char> const, bool const) const;
    QList<int> BestMoveCalcul(QVector<char>, bool);

    int count = 0;

    int a = 1;
    int b= 0;
    double sigma = 0.5;
    double percentile = 1.5;

};

#endif // IA_H
