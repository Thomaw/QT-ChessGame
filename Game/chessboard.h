#ifndef CHESSBOARD_H
#define CHESSBOARD_H

#include "player.h"
#include "chessalgorithm.h"
#include <QObject>
#include <QVector>
#include <QMediaPlayer>
#include "QMediaPlaylist"
#include <QSound>
#include "QString"

#include <IA/board.h>
#include <QDebug>
#include <QPixmap>
#include "IA/engine.h"


class ChessBoard : public QObject{
    Q_OBJECT
    Q_PROPERTY(int ranks READ ranks NOTIFY ranksChanged)
    Q_PROPERTY(int columns READ columns NOTIFY columnsChanged)

public:
    explicit ChessBoard(int ranks = 8, int columns = 8, QObject *parent = 0);

    int ranks() const { return m_ranks; }
    int columns() const { return m_columns; }
    char data(int column, int rank) const {return m_boardData.at((rank-1)*columns()+(column-1));};
    void setData(int column, int rank, char value);

    void movePiece(int fromColumn, int fromRank, int toColumn, int toRank);
    QVector<int> PossiblemovePiece(bool, QVector<char>);

    void setFen(const QString &fen);
    void DefinePlayerStart(const QString &fen);

    void setPlayer(bool);
    bool start;


    void ChangeSoundVolumeMovement(int);
    void ChangeSoundVolumeVictory(int);
    void ChangeSoundVolumePat(int);

    QVector<char> Return_m_boardData(){return m_boardData;};
    void Change_m_boardData(char, int);

    bool Return_IsCheck(){return IsCheckV;};
    bool Return_IsPat(){return isPat;};

    void IsCheckMate();
    void Endgame();

    QVector<int> AllPossibleMove;

    Chess::Engine *eng = new Chess::Engine();


    bool UseEngine = false;
    void MovePieceUtilisateur(int, int, int, int);
    int level;



protected:
    void setRanks(int newRanks);
    void setColumns(int newColumns);
    void initBoard();
    //bool accepted_movement(int fromColumn, int fromRank, int toColumn, int toRank);
    bool setDataInternal(int column, int rank, char value);

signals:
    void ranksChanged(int);
    void columnsChanged(int);
    void boardReset();
    void dataChanged(int c, int r);
    void playerChanged(int);

    void testChangment();

    void MiseEnEchecs();
    void PlusMiseEnEchecs();
    void PatFinGame();

    void PromotionSignal(char, int);
    void CalculenCours();

public slots:
    void testUi(int,int,int,int);



private:
    Player *player = new Player();

    //QVector<int> AsMove;
    int m_ranks, m_columns;
    bool IsCheckV;
    bool isPat = false;

    QMediaPlayer *SoundPlayer = new QMediaPlayer();
    QMediaPlayer *SoundVictory = new QMediaPlayer();
    QMediaPlayer *SoundPat = new QMediaPlayer();
    void MusicPlayMovement();
    void MusicPlayVictory();
    void MusicPlayPat();

    bool EstMisEnEchec = false;


    void PathFunction(bool &, int, int, int, int);
    bool CheckMateFunction();

    void DeplacementEnPassant(int, int);
    void DeplacementTourRoque(bool, char, int, int, QVector<char>, QVector<int>);
    void PromotionPion(bool, char, int);

    QList<QChar> const White_pieces {'R', 'N', 'B', 'Q', 'K', 'P'};
    QList<QChar> const Black_pieces {'r', 'n', 'b', 'q', 'k', 'p'};

    QVector<char> m_boardData;

    ChessAlgorithm* m_algorithm = new ChessAlgorithm;

    QVector<int> IndexationPossibleMove;

    void PieceDeplacement(int, int, int, int);


};

#endif // CHESSBOARD_H
