#include "bishop_class.h"
#include "iostream"
#include <QtMath>

/**
 * @brief Bishop_Class::Bishop_Class
 *
 * Constructeur du fou
 */
Bishop_Class::Bishop_Class(){}


/**
 * @brief Bishop_Class::autorize_move
 * @param fromColumn
 * @param fromRank
 * @param toColumn
 * @param toRank
 * @param p
 * @param m_boardData
 * @return result
 *
 * Fonction permettant de valider ou non le déplacement d'un fou.
 * On décompose le mouvement d'un fou en deux étapes:
 *  - La vérification que le déplacement se fait en diagonale
 *  - La vérification qu'il n'y a pas de pièce entre le début et la fin du déplacement
 */
bool Bishop_Class::autorize_move(int fromColumn, int fromRank, int toColumn, int toRank, char p, QVector<char> m_boardData){
    bool result =  false;                                                                                   // Mouvement par défaut interdit

    bool result_normal_move = normal_move( fromColumn,  fromRank,  toColumn,  toRank);                      // Autorisation mouvement normal
    bool result_opposite_move = opposition_move(fromColumn, fromRank, toColumn, toRank, p, m_boardData);    // Moteur de collision du fou

    if (result_normal_move==true and result_opposite_move==true){                                           // Si le mouvement est autorisé
            result = true;
    }

    return result;
}


/**
 * @brief Bishop_Class::normal_move
 * @param fromColumn
 * @param fromRank
 * @param toColumn
 * @param toRank
 * @return bool
 *
 * Fonction vérifiant que le fou réalise bien un déplacement en diagonale
 */
bool Bishop_Class::normal_move(int fromColumn, int fromRank, int toColumn, int toRank){
    if (abs(fromColumn-toColumn)==abs(fromRank-toRank) and abs(fromColumn-toColumn)>0){ // Si l'on se déplace dans une diagonale
        return true;                                                                    // Mouvement autorisé
    }
    else {
        return false;                                                                   // Mouvement refusé
    }
}


/**
 * @brief Bishop_Class::opposition_move
 * @param fromColumn
 * @param fromRank
 * @param toColumn
 * @param toRank
 * @param p
 * @param m_boardData
 * @return result
 *
 * Foncton vérifiant que le fou ne rencontre aucune autres pièces avant la fin de son déplacement
 */
bool Bishop_Class::opposition_move(int fromColumn, int fromRank, int toColumn, int toRank, char p, QVector<char> m_boardData){
    bool result = true;

    int new_index = 8 * (toRank-1)+(toColumn-1);    // Position finale de déplacement

    if (abs(fromColumn-toColumn)==abs(fromRank-toRank) and abs(fromColumn-toColumn)>0){ // Si l'on se déplace en diagonale

        bool inter = false;
        inter = check_eat(new_index, p, m_boardData);    // Vérifie que la position que l'on souhaite atteindre est une pièce adverse

    // On a quatre cas possible :
    // fromColumn >toColumn and fromRank>toRank
    // fromColumn <toColumn and fromRank>toRank
    // fromColumn >toColumn and fromRank<toRank
    // fromColumn <toColumn and fromRank<toRank

        // Cas 1
        if (fromColumn > toColumn and fromRank>toRank){

            // Pour toutes les cases entre la position de départ et d'arrivée
            for (int i =fromColumn-1; i>=toColumn; i--){
                int j = 8 * (fromRank-1-(fromColumn-i))+(i-1);

                if (m_boardData[j] != ' '){                         // S'il y a une pièce sur le déplacement
                    result = false;                                 // Mouvement refusé

                    // Si la dernière case contient une pièce et est de couleur opposée
                    if(i==toColumn and inter==true){result = true;} // Mouvement autorisé
                    break;
                }
            }
        }


        // Cas 2
        if (fromColumn <toColumn and fromRank>toRank){

            // Pour toutes les cases entre la position de départ et d'arrivée
            for (int i =fromRank-1; i>=toRank; i--){
                int j = 8 * (i-1)+(fromColumn-1+(fromRank-i));

                if (m_boardData[j] != ' '){                          // S'il y a une pièce sur le déplacement
                    result = false;                                  // Mouvement refusé

                    // Si la dernière case contient une pièce et est de couleur opposée
                    if(i==toRank  and inter==true){result = true;}   // Mouvement autorisé
                    break;
                }
            }
        }

        // Cas 3
        if (fromColumn >toColumn and fromRank<toRank){

            // Pour toutes les cases entre la position de départ et d'arrivée
            for (int i =fromRank+1; i<=toRank; i++){
                int j = 8 * (i-1)+(fromColumn-1+(fromRank-i));

                    if (m_boardData[j] != ' '){                        // S'il y a une pièce sur le déplacement
                        result = false;                                // Mouvement refusé

                    // Si la dernière case contient une pièce et est de couleur opposée
                    if(i==toRank  and inter==true){result = true;}     // Mouvement autorisé
                    break;
                }
            }

        }

        // Cas 4
        if (fromColumn <toColumn and fromRank<toRank){

            // Pour toutes les cases entre la position de départ et d'arrivée
            for (int i =fromColumn+1; i<=toColumn; i++){
                int j = 8 * (fromRank-1-(fromColumn-i))+(i-1);

                    if (m_boardData[j] != ' '){                        // S'il y a une pièce sur le déplacement
                        result = false;                                // Mouvement refusé

                    // Si la dernière case contient une pièce et est de couleur opposée
                    if(i==toColumn  and inter==true){result = true;}   // Mouvement autorisé
                    break;
                }
            }
        }
    }

    return result;
}


/**
 * @brief Bishop_Class::check_eat
 * @param new_index
 * @param p
 * @param m_boardData
 * @return result
 *
 * Fonction vérifiant que la dernière pièce du déplacement est une pièce opposée
 */
bool Bishop_Class::check_eat(int new_index, char p, QVector<char> m_boardData){
    bool result = false;

    switch (p){

    case 'b':{  // Si c'est un fou noir
        int k = White_pieces.indexOf(m_boardData[new_index]);               // Recherche si la dernière position contient une pièce blanche
        if ((m_boardData[new_index] != ' ') and (k != -1)){result = true;}  // Si c'est le cas, renvoie que le mouvement est possible
        break;
    }

    case 'B':{  // Si c'est un fou blanc
        int k = Black_pieces.indexOf(m_boardData[new_index]);               // Recherche si la dernière position contient une pièce noire
        if ((m_boardData[new_index] != ' ') and (k != -1)){ result = true;} // Si c'est le cas, renvoie que le mouvement est possible
        break;
    }
    }
    return result;
}

