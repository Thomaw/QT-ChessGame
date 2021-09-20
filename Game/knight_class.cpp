#include "knight_class.h"
#include "iostream"
#include "QtMath"


/**
 * @brief Knight_Class::Knight_Class
 *
 * Constructeur du cavalier
 */
Knight_Class::Knight_Class(){}


/**
 * @brief Knight_Class::autorize_move
 * @param fromColumn
 * @param fromRank
 * @param toColumn
 * @param toRank
 * @param p
 * @param m_boardData
 * @return result
 *
 * Fonction permettant de valider ou non le déplacement d'un cavalier.
 * On décompose le mouvement d'un fou en deux étapes:
 *  - La vérification que le déplacement est possible
 *  - La vérification qu'il n'y a pas de pièce entre le début et la fin du déplacement
 */
bool Knight_Class::autorize_move(int fromColumn, int fromRank, int toColumn, int toRank, char p, QVector<char> m_boardData){
    bool result = false;

    bool result_normal_move = normal_move( fromColumn,  fromRank,  toColumn,  toRank);
    bool result_opposite_move = opposition_move(toColumn, toRank, p, m_boardData);

    if (result_normal_move==true and result_opposite_move==true){
            result = true;
    }

    return result;
}


/**
 * @brief Knight_Class::normal_move
 * @param fromColumn
 * @param fromRank
 * @param toColumn
 * @param toRank
 * @return bool
 *
 * Fonction vérifiant que le cavalier réalise bien un déplacement du cavalier
 */
bool Knight_Class::normal_move(int fromColumn, int fromRank, int toColumn, int toRank){
    if ((abs(fromColumn-toColumn)==2 and abs(fromRank-toRank)==1) or
            (abs(fromColumn-toColumn)==1 and abs(fromRank-toRank)==2)){
        return true;
    }
    else
        return false;
}


/**
 * @brief Knight_Class::opposition_move
 * @param fromColumn
 * @param fromRank
 * @param toColumn
 * @param toRank
 * @param p
 * @param m_boardData
 * @return result
 *
 * Fonction vérifiant que le cavalier ne rencontre aucune autre pièce avant la fin de son déplacement
 */
bool Knight_Class::opposition_move(int toColumn, int toRank, char p, QVector<char> m_boardData){
    int new_index = 8 * (toRank-1)+(toColumn-1);
    bool result = false;

    switch (p){

    case 'n':{
        if (m_boardData[new_index] != ' '){
            int k = White_pieces.indexOf(m_boardData[new_index]);
            if (k != -1){                                              // S'il n'y a pas de pièces blanches
                result = true;                                         // Renvoie vrai
            }
        }
        else {result = true;}                                          // Renvoie vrai
        break;
    }

    case 'N':{
        if (m_boardData[new_index] != ' '){
            int k = Black_pieces.indexOf(m_boardData[new_index]);
            if (k != -1){                                              // S'il n'y a pas de pièces noires
                result = true;                                         // Renvoie vrai
            }
        }
        else {result = true;}                                          // Renvoie vrai
        break;
    }

    default:
        break;
    }

    return result;
}

