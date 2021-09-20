#include "pawn_class.h"
#include "iostream"

/**
 * @brief Pawn_Class::Pawn_Class
 *
 * Construction de la classe des pions
 */
Pawn_Class::Pawn_Class(){}




/**
 * @brief Pawn_Class::autorize_move
 * @param fromColumn
 * @param fromRank
 * @param toColumn
 * @param toRank
 * @param p
 * @param m_boardData
 * @param AncienBoard
 * @return
 *
 * Fonction permettant de valider ou non le déplacement d'un pion.
 * Celle-ci prend en compte les mouvements suivants:
 *  - La vérification que le déplacement est possible
 *  - La vérification qu'il n'y a pas de pièce entre le début et la fin du déplacement
 *  - La prise classique
 *  - La prise en passant
 */
bool Pawn_Class::autorize_move(int fromColumn, int fromRank, int toColumn, int toRank, char p, QVector<char> m_boardData, QVector<char> AncienBoard){

    bool final_result = false;

    bool result_normal_move = normal_move(fromColumn, fromRank, toColumn, toRank, p);
    bool result_opposite_move = opposition_move(fromColumn, fromRank, toColumn, toRank, p, m_boardData);
    bool result_eat_move = eat_move(fromColumn, fromRank, toColumn, toRank, p, m_boardData, AncienBoard);

    if ((result_normal_move or result_eat_move) and (result_opposite_move)){
        final_result = true;
    }

    return final_result;
}


/**
 * @brief Pawn_Class::normal_move
 * @param fromColumn
 * @param fromRank
 * @param toColumn
 * @param toRank
 * @param p
 * @return
 *
 * Fonction qui regarde si le déplacement est possible
 */
bool Pawn_Class::normal_move(int fromColumn, int fromRank, int toColumn, int toRank, char p){
    switch (p){

    // Cas noir
    case 'p':{
        if (fromRank == 7){
            if (fromColumn==toColumn and ((toRank-fromRank == -1) or (toRank-fromRank == -2))){
                return true;
            }
            else {return false;}
            break;
        }

        else {
            if (fromColumn==toColumn and (toRank-fromRank == -1)){
                return true;
            }
            else {return false;}
            break;
        }
    }

    // Cas blanc
    case 'P':{
        if (fromRank == 2){
            if (fromColumn==toColumn and ((toRank-fromRank == 1) or (toRank-fromRank == 2))){
                return true;
            }
            else {return false;}
            break;
        }

        else{
            if (fromColumn==toColumn and (toRank-fromRank == 1)){
                return true;
            }
            else {return false;}
            break;
        }
    }

    // Cas si erreur
    default:{
        std::cout<<"error in Pawn Class - normal_move"<<std::endl;
        return false;
        }

    }
}


/**
 * @brief Pawn_Class::opposition_move
 * @param fromColumn
 * @param fromRank
 * @param toColumn
 * @param toRank
 * @param p
 * @param m_boardData
 * @return
 *
 * Fonction qui vérifie qu'aucune pièce empêche de se déplacer
 */
bool Pawn_Class::opposition_move(int fromColumn, int fromRank, int toColumn, int toRank, char p, QVector<char> m_boardData){
    bool result = true;
    int new_index = 8 * (toRank-1)+(toColumn-1);

    switch (p){

    case 'p':{  // Cas noir
        if (fromRank == 7){
            for (int i=fromRank-1; i>=toRank; i--){
                int j = 8 * (i-1)+(toColumn-1);

                if ((fromColumn==toColumn) and (m_boardData[j] !=' ')){
                    result = false;
                    break;
                }
            }
        }
        else {
            if ((fromColumn==toColumn) and (m_boardData[new_index] !=' ')){result = false;}
        }

        break;
    }

    case 'P':{  // Cas blanc
        if (fromRank == 2){

            for (int i=fromRank+1; i<=toRank; i++){
                int j = 8 * (i-1)+(toColumn-1);

                if ((fromColumn==toColumn) and (m_boardData[j] !=' ')){
                    result = false;
                    break;
                }
            }

        }
        else {
            if ((fromColumn==toColumn) and m_boardData[new_index] !=' '){result = false;}
        }

        break;
    }

    // Cas si erreur
    default:{
        result = false;
        break;
    }
    }

    return result;
}


/**
 * @brief Pawn_Class::eat_move
 * @param fromColumn
 * @param fromRank
 * @param toColumn
 * @param toRank
 * @param p
 * @param m_boardData
 * @param AncienBoard
 * @return
 *
 * Fonction pour les prises (classique et En Passant)
 */
bool Pawn_Class::eat_move(int fromColumn, int fromRank, int toColumn, int toRank, char p, QVector<char> m_boardData, QVector<char> AncienBoard){
    bool result = true;
    int old_index = 8 * (fromRank-1)+(fromColumn-1);
    int new_index = 8 * (toRank-1)+(toColumn-1);

    // Prise d'un pion
    if ((abs(old_index-new_index)== 7 or abs(old_index-new_index)== 9) and abs(toColumn-fromColumn)<2){
        switch (p){

        case 'p':{
            int k = White_pieces.indexOf(m_boardData[new_index]);
            if ((m_boardData[new_index] != ' ') and (old_index>new_index) and (k != -1)){
                result = true;
            }
            else{result = false;}
            break;
        }

        case 'P':{
            int k = Black_pieces.indexOf(m_boardData[new_index]);
            if ((m_boardData[new_index] != ' ') and (new_index>old_index) and (k != -1)){
                result = true;
            }

            else{result = false;}
            break;
        }

        default:
            break;
        }
    }

    else {result=false;}


    //
    // Condition du "En Passant"
    //
    switch (p){
    case 'p':{
        if (new_index+8<64 and new_index-8>-1 and old_index>23 and old_index<32 and ((abs(old_index-new_index)== 9) or (abs(old_index-new_index)== 7)) and AncienBoard.length() != 0){ // Condition sur le déplacement voulu
            if (AncienBoard[new_index-8]=='P'){ // Condition(s) sur l'ancien board
                if (m_boardData[new_index+8]=='P' and m_boardData[new_index]==' '){
                    result = true;
                }
            }
        }
        break;
    }

    case 'P':{
        if (new_index+8<64 and new_index-8>-1 and old_index>31 and old_index<40 and ((abs(old_index-new_index)== 9) or (abs(old_index-new_index)== 7)) and AncienBoard.length() != 0){
            if (AncienBoard[new_index+8]=='p'){
                if (m_boardData[new_index-8]=='p' and m_boardData[new_index]==' '){
                    result = true;
                }
            }
        }
        break;
    }
    }

    return result;
}

