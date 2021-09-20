#include "iostream"
#include "QPoint"

#include "chessalgorithm.h"
#include "chessboard.h"

#include "player.h"
#include "king_class.h"
#include "queen_class.h"
#include "rook_class.h"
#include "bishop_class.h"
#include "pawn_class.h"
#include "knight_class.h"


/**
 * @brief ChessAlgorithm::ChessAlgorithm
 * @param parent
 *
 * Constructeur de l'algorithme de jeu
 */
ChessAlgorithm::ChessAlgorithm(QObject *parent) : QObject(parent), m_board(0){
    AsMove.resize(64);
}


/**
 * @brief ChessAlgorithm::board
 * @return
 *
 * Fonction pour retourner l'échiquier
 */
ChessBoard *ChessAlgorithm::board() const {return m_board;}


/**
 * @brief ChessAlgorithm::newGame
 * @param Fen
 *
 * Fonction pour définir une nouvelle partie
 */
void ChessAlgorithm::newGame(QString Fen){
    setupBoard();                           // Initialise l'échiquier
    board()->setFen(Fen);                   // Positionne les pièces
}


/**
 * @brief ChessAlgorithm::setupBoard
 *
 * Création d'un nouveau tableau de jeu
 */
void ChessAlgorithm::setupBoard(){
    setBoard(new ChessBoard(8,8, this));    // Crée un plateau de jeu
}


/**
 * @brief ChessAlgorithm::setBoard
 * @param board
 *
 * Emet un signal pour signifier que le board a changé
 */
void ChessAlgorithm::setBoard(ChessBoard *board){
    if(board == m_board)                    // Si le nouveau board est le même que l'ancien
        return;                             // Rien ne se passe

    if(m_board)                             // Sinon
        delete m_board;                     // Supprime le board actuel

    m_board = board;                        // Construit le nouveau board
    emit boardChanged(m_board);             // Emet le signal de changement de board
}


/**
 * @brief ChessAlgorithm::accepted_movement
 * @param m_column
 * @param fromColumn
 * @param fromRank
 * @param toColumn
 * @param toRank
 * @param m_boardData
 * @return result
 *
 * Fonction permettant d'accpeter le déplacement d'une pièce de l'échiquier
 */
bool ChessAlgorithm::accepted_movement(int m_column, int fromColumn, int fromRank, int toColumn, int toRank, QVector<char> m_boardData){
    bool result = false;

    switch (m_boardData[(fromRank-1)*m_column+(fromColumn-1)]){ // Selon la pièce déplacée, on regarde si le mouvement est autorisé

    case 'k':{  // Roi noir
        result = Black_King->autorize_move(fromColumn, fromRank, toColumn, toRank, 'k', m_boardData, AsMove);
        break;
    }

    case 'K':{  // Roi blanc
        result = White_King->autorize_move(fromColumn, fromRank, toColumn, toRank, 'K', m_boardData, AsMove);
        break;
    }

    case 'q':{  // Dame noire
        result = Black_Queen->autorize_move(fromColumn, fromRank, toColumn, toRank, 'q', m_boardData);
        break;
    }

    case 'Q':{  // Dame blanche
        result = White_Queen->autorize_move(fromColumn, fromRank, toColumn, toRank, 'Q', m_boardData);
        break;
    }

    case 'r':{  // Tour noire
        result = Black_Rook->autorize_move(fromColumn, fromRank, toColumn, toRank, 'r', m_boardData);
        break;
    }

    case 'R':{  // Tour blanche
        result = White_Rook->autorize_move(fromColumn, fromRank, toColumn, toRank, 'R', m_boardData);
        break;
    }

    case 'n':{  // Cavalier noir
        result = Black_Knight->autorize_move(fromColumn, fromRank, toColumn, toRank, 'n', m_boardData);
        break;
    }

    case 'N':{  // Cavalier blanc
        result = White_Knight->autorize_move(fromColumn, fromRank, toColumn, toRank, 'N', m_boardData);
        break;
    }

    case 'b':{  // Fou noir
        result = Black_Bishop->autorize_move(fromColumn, fromRank, toColumn, toRank, 'b', m_boardData);
        break;
    }

    case 'B':{  // Fou blanc
        result = White_Bishop->autorize_move(fromColumn, fromRank, toColumn, toRank, 'B', m_boardData);
        break;
    }

    case 'p':{  // Pion noir
        result = Black_Pawn->autorize_move(fromColumn, fromRank, toColumn, toRank, 'p', m_boardData, AncienBoardEnPassantBlack);
        break;
    }

    case 'P':{  // Pion blanc
        result = White_Pawn->autorize_move(fromColumn, fromRank, toColumn, toRank, 'P', m_boardData, AncienBoardEnPassantWhite);
        break;
    }
    }
    return result;
}


/**
 * @brief ChessAlgorithm::all_possible_movement
 * @param start
 * @param m_boardData
 * @return  QVector<int>
 *
 * Fonction donnant tous les mouvements possibles pour un joueur
 */
QVector<int> ChessAlgorithm::all_possible_movement(bool start, QVector<char> m_boardData){
    QVector<int> AllPossibleMove(64);                 // Création du vecteur résultat
    int k;

    for (int i=0;i<64;i++){
        if (start == 0){                              // Pour les blancs
            k = White_pieces.indexOf(m_boardData[i]); // S'il y a une pièce blanche sur la case i
        }
        else{                                         // Pour les noirs
            k = Black_pieces.indexOf(m_boardData[i]); // S'il y a une pièce noire sur la case i
        }

        // S'il y a une pièce
        if (k != -1){
            for (int b=0;b<64;b++){                   // Pour chaque pièce de l'échiquier
                if (AllPossibleMove[b] == 0){
                    if (accepted_movement(8, GetColumnIndex(i)+1, GetRankIndex(i)+1, GetColumnIndex(b)+1, GetRankIndex(b)+1, m_boardData)){ // Si le mouvement est possible
                        AllPossibleMove[b]=1;         // Signifie que le mouvement est possible
                    }
                }
            }
        }
    }
    return AllPossibleMove;
}


/**
 * @brief ChessAlgorithm::Indexation_possible_movement
 * @param AllPossibleMove
 * @return QVector<int>
 *
 * Fonction donnant la position d'arrivée de tout les déplacements possibles d'un joueur
 * Fonction liée de près avec @brief ChessAlgorithm::all_possible_movement
 */
QVector<int> ChessAlgorithm::Indexation_possible_movement(QVector<int> AllPossibleMove){
    int compteur = AllPossibleMove.count(1);            // Compte le nombre de mouvements autorisé
    IndexationPossibleMove.resize(compteur);            // Redéfinit la taille du vecteur

    int m = 0;
    for (int i=0;i<AllPossibleMove.length();i++){       // Pour chaque pièce de l'échiquier
        if(AllPossibleMove[i]){                         // Si le déplacement est possible
            IndexationPossibleMove[m]=i;                // Index le résultat
            m++;                                        // Incrémente le compteur
        }
    }
    return IndexationPossibleMove;
}


/**
 * @brief ChessAlgorithm::IsCheck
 * @param start
 * @param m_boardData
 * @param Indexation_possible_movement
 * @return bool
 *
 * Fonction permettant de vérifier si un roi est en échec
 */
bool ChessAlgorithm::IsCheck(bool start, QVector<char> m_boardData, QVector<int> Indexation_possible_movement){
    int k_pos = 0;                                      // Position du roi
    bool result = false;

    if (start == 1){                                    // Pour les noirs  -> Recherche du roi blanc
        k_pos = m_boardData.indexOf('K');
    }
    else{                                               // Pour les blancs -> Recherche du roi noir
        k_pos = m_boardData.indexOf('k');
    }
    result = Indexation_possible_movement.count(k_pos); // Recherche dans tous les mouvements possibles si un mouvement peut atteindre la case du roi
    return result;
}


/**
 * @brief ChessAlgorithm::AsAlreadyMove
 * @param AsMove
 * @param i
 * @param j
 * @return QVector<int>
 *
 * Fonction donnant si une pièce a déjà été déplacée
 * Fonction très importante pour le roque
 */
QVector<int> ChessAlgorithm::AsAlreadyMove(QVector<int> AsMove, int i, int j){
    AsMove[i] += j; // Ajoute un 1 si l'on a déplacé la pièce
    return AsMove;
}


/**
 * @brief ChessAlgorithm::SetIsCheckMate
 * @param p
 * @return bool
 *
 * Fonction qui retourne si un joueur est échec et Mat
 */
bool ChessAlgorithm::SetIsCheckMate(bool p){
    IsCheckMate = p;       // Change la variable de CheckMat
    return IsCheckMate;
}


/**
 * @brief ChessAlgorithm::CallIsCheckMate
 * @param p
 *
 * Emet un signal si un joueur est échec et Mat
 */
void ChessAlgorithm::CallIsCheckMate(bool p){
    if (IsCheckMate == p){
        return;
    }

    emit CheckMateVariable(IsCheckMate);    // Emet un signal pour dire que l'on est CheckMat
}


/**
 * @brief ChessAlgorithm::KingPosition
 * @param board
 * @param start
 * @return int
 *
 * Fonction qui retourne la position du roi d'un joueur
 */
int ChessAlgorithm::KingPosition(QVector<char> board,bool start){
    int index = 0;

    if (start == 1){index = board.indexOf('k');}   // Trouve la position du roi noir
    else {index = board.indexOf('K');}             // Trouve la position du roi blanc

    return index;
}


/**
 * @brief ChessAlgorithm::MiseEnEchec
 * @param start
 * @param m_boardData
 * @return bool
 *
 * Fonction qui donne la position des pièces qui mettent en échec un roi
 */
bool ChessAlgorithm::MiseEnEchec(bool start, QVector<char> m_boardData){
    QVector<int> PossibleMovements = all_possible_movement(start, m_boardData);                 // Regarde tous les mouvements possibles
    QVector<int> IndexationPossibleMovements = Indexation_possible_movement(PossibleMovements); // Indexation de tous ces mouvements
    bool IsCheckMovements = IsCheck(start, m_boardData, IndexationPossibleMovements);           // Regarde si l'on est en échec

    return IsCheckMovements;
}


/**
 * @brief ChessAlgorithm::PositionPieces
 * @param start
 * @param m_boardData
 * @return QList<int>
 *
 * Fonction qui retourne la position de toutes les pièces
 */
QList<int> ChessAlgorithm::PositionPieces(bool start, QVector<char> m_boardData){
    int k;
    QList<int> PositionPieces;

    for (int i=0;i<64;i++){
        if (start == 0){ // Pour les blancs
            k = White_pieces.indexOf(m_boardData[i]); // S'il y a une pièce blanche sur la case i
        }
        else{            // Pour les noirs
            k = Black_pieces.indexOf(m_boardData[i]); // S'il y a une pièce noire sur la case i
        }

        if (k != -1){PositionPieces.append(i);}
    }

    return PositionPieces;
}


