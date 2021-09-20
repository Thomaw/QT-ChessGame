#include "king_class.h"
#include "iostream"

#include <QtMath>


/**
 * @brief King_Class::King_Class
 *
 * Constructeur du roi
 */
King_Class::King_Class(){}


/**
 * @brief King_Class::autorize_move
 * @param fromColumn
 * @param fromRank
 * @param toColumn
 * @param toRank
 * @param p
 * @param m_boardData
 * @param AsMove
 * @return
 *
 * Fonction permettant de valider ou non le déplacement d'un fou.
 * On décompose le mouvement d'un fou en deux étapes:
 *  - La vérification que le déplacement ne se fait que d'une case
 *  - La vérification qu'il n'y a pas de pièce entre le début et la fin du déplacement
 *  - Les conditions de roque
 */
bool King_Class::autorize_move(int fromColumn, int fromRank, int toColumn, int toRank, char p, QVector<char> m_boardData, QVector<int> AsMove){

    bool result = false;

    bool result_normal_move = normal_move( fromColumn,  fromRank,  toColumn,  toRank);
    bool result_opposite_move = opposition_move(toColumn, toRank, p, m_boardData);
    bool result_Castling = Castling(fromColumn, fromRank, toColumn, toRank, p, m_boardData, AsMove);

    if (result_Castling){
        result = true;
    }
    else {
        if (result_normal_move==true and result_opposite_move==true){
            result = true;
        }
    }

    return result;
}


/**
 * @brief King_Class::normal_move
 * @param fromColumn
 * @param fromRank
 * @param toColumn
 * @param toRank
 * @return
 *
 * Fonction vérifiant que le roi réalise bien un déplacement d'une case
 */
bool King_Class::normal_move(int fromColumn, int fromRank, int toColumn, int toRank){

    if (abs(fromColumn-toColumn)<=1 and abs(fromRank-toRank)<=1){
        return true;
    }
    else
        return false;
}



/**
 * @brief King_Class::opposition_move
 * @param toColumn
 * @param toRank
 * @param p
 * @param m_boardData
 * @return
 *
 * Fonction vérifiant qu'il n'y a pas de pièce de même couleur sur le déplacement
 */
bool King_Class::opposition_move(int toColumn, int toRank, char p, QVector<char> m_boardData){
    int new_index = 8 * (toRank-1)+(toColumn-1);

    bool result = false;

    switch (p){
    case 'k':{
        if (m_boardData[new_index] != ' '){
            int k = White_pieces.indexOf(m_boardData[new_index]);
            if (k != -1){
                    result = true;
                }
        }
        else { result = true; }

        break;
    }

    case 'K':{
        if (m_boardData[new_index] != ' '){
            int k = Black_pieces.indexOf(m_boardData[new_index]);
            if (k != -1){
                result = true;
            }
        }
        else { result = true;}

        break;
    }
    }

    return result;
}


/**
 * @brief King_Class::Castling
 * @param fromColumn
 * @param fromRank
 * @param toColumn
 * @param toRank
 * @param p
 * @param m_boardData
 * @param AsMove
 * @return
 *
 * Fonction qui autorise ou non le déplacement pour le roque
 */
bool King_Class::Castling(int fromColumn, int fromRank, int toColumn, int toRank, char p, QVector<char> m_boardData, QVector<int> AsMove){
    bool result =false;

    int old_index = 8 * (fromRank-1)+(fromColumn-1);
    int new_index = 8 * (toRank-1)+(toColumn-1);

    if (abs(old_index-new_index) < 3){

        if (p == 'K'){ // Cas du roi blanc

            if (new_index==2 and old_index>2 and m_boardData[0]=='R'){ // Cas de la tour de gauche
                if (AsMove[0]==0 and AsMove[4]==0){                    // Roque blanc de 2 cases sur la gauche

                    // Cases 1,2 et 3 sans pièces
                    if ((m_boardData[1] != ' ') or (m_boardData[2] != ' ') or (m_boardData[3] != ' ')){
                        result = false;
                    }
                    else{ result = true; }
                }
            }

            if (new_index==6 and old_index<6 and m_boardData[7]=='R'){
                if (AsMove[7]==0 and AsMove[4]==0){                    // Roque blanc de 2 cases sur la droite

                    // Cases 5,6 sans pièces
                    if ((m_boardData[5] != ' ') or (m_boardData[6] != ' ')){
                        result = false;
                    }
                    else{ result = true; }
                }
            }
        }


        if (p == 'k'){ // Cas du roi noir

            if (new_index==58 and old_index>58 and m_boardData[56]=='r' ){ // Cas de la tour de gauche
                if (AsMove[56]==0 and AsMove[60]==0){                      // Roque noir de 2 cases sur la gauche

                    // Cases 57, 58 et 59 sans pièces
                    if ((m_boardData[57] != ' ') or (m_boardData[58] != ' ') or (m_boardData[59] != ' ')){
                        result = false;
                    }
                    else{ result = true; }
                }
            }


            if (new_index==62 and old_index<62 and m_boardData[63]=='r'){ // Tour de droite
                if (AsMove[63]==0 and AsMove[60]==0){                     // Roque noir de 2 cases sur la droite

                    // Cases 61,62 sans pièces
                    if ((m_boardData[61] != ' ') or (m_boardData[62] != ' ')){
                        result = false;
                    }
                    else{ result = true; }
                }
            }
        }
    }

    return result;
}

