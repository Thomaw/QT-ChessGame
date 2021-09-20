#include "iostream"

#include "chessboard.h"
#include "player.h"

#include "QElapsedTimer"

#include <IA/board.h>
#include <QDebug>
#include <QPixmap>
#include "IA/engine.h"


/**
 * @brief ChessBoard::ChessBoard
 * @param ranks
 * @param columns
 * @param parent
 *
 * Constructeur de l'échiquier
 */
ChessBoard::ChessBoard(int ranks, int columns, QObject *parent): QObject(parent), m_ranks(ranks), m_columns(columns){
    initBoard();                                                    // Initialise le board

    SoundPlayer->setMedia(QUrl("qrc:/Sound/movement_sound.wav"));   // Charge le son des mouvements
    SoundVictory->setMedia(QUrl("qrc:/Sound/victory_sound.wav"));   // Charge le son de la victoire
    SoundPat->setMedia(QUrl("qrc:/Sound/Pat_sound.wav"));           // Charge le son de l'égalité

    SoundPlayer->setVolume(50);                                     // Définis le volume d'un mouvement à 50
    SoundVictory->setVolume(50);                                    // Définis le volume du son de victoire à 50
    SoundPat->setVolume(50);                                        // Définis le volume du son d'égalité à 50

    // Connection de l'IA avec l'UI
    QObject::connect(eng, SIGNAL(boardapplichanged(int, int, int, int)), this, SLOT(testUi(int, int, int, int)));
}


/**
 * @brief ChessBoard::setData
 * @param column
 * @param rank
 * @param value
 *
 * Fonction qui émet un signal envoyant la position de la pièce déplacée
 */
void ChessBoard::setData(int column, int rank, char value){
    if(setDataInternal(column, rank, value))    // Si la valeur du board a changé
        emit dataChanged(column, rank);         // Emet un signal pour dire qu'il a changé
}


/**
 * @brief ChessBoard::PieceDeplacement
 * @param toColumn
 * @param toRank
 * @param fromColumn
 * @param fromRank
 *
 * Fonction permettant de déplacer une pièce sur le board
 */
void ChessBoard::PieceDeplacement(int toColumn, int toRank, int fromColumn, int fromRank){
    setData(toColumn, toRank, data(fromColumn, fromRank));  // Déplace l'icône de la pièce à la position voulue
    setData(fromColumn, fromRank, ' ');                     // Supprime l'icône de la pièce à la position initiale
}


/**
 * @brief ChessBoard::PossiblemovePiece
 * @param start
 * @param m_boardData
 * @return QVector<int>
 *
 * Retourne l'ensemble des mouvements possibles d'un joueur
 */
QVector<int> ChessBoard::PossiblemovePiece(bool start, QVector<char> m_boardData){
    AllPossibleMove = m_algorithm->all_possible_movement(start, m_boardData);            // Regarde tous les mouvements possibles
    IndexationPossibleMove = m_algorithm->Indexation_possible_movement(AllPossibleMove); // Indexation de tous ces mouvements
    return IndexationPossibleMove;
}


/**
 * @brief ChessBoard::movePiece
 * @param fromColumn
 * @param fromRank
 * @param toColumn
 * @param toRank
 *
 * Fonction pour déplacer une pièce selon si l'on joue contre un autre joueur ou contre l'ordinateur
 */
void ChessBoard::movePiece(int fromColumn, int fromRank, int toColumn, int toRank){

    // Si l'on n'utilise pas l'IA
    if (UseEngine == false){
        MovePieceUtilisateur(fromRank, fromColumn, toRank, toColumn);                                           // Déplace la pièce sur le board
        eng->makeMove(Chess::Move(Chess::Coord(fromColumn-1, fromRank-1), Chess::Coord(toColumn-1, toRank-1))); // Déplace la pièce dans l'IA
    }

    // Si on utilise l'IA
    else {
        int index = (fromRank-1)*columns()+(fromColumn-1);                                                      // Position de la pièce déplacée (avant déplacement)

        bool good_piece = player->Playable(index, m_boardData);                                                 // Vérification du mouvement des pièces blanches ou noires
        bool result = m_algorithm->accepted_movement(8,fromColumn, fromRank, toColumn, toRank, m_boardData);    // Vérifie que le mouvement est possible

        if (result and good_piece){                                         // Si le mouvement est autorisé
            emit CalculenCours();                                           // Emet un signal pour dire que l'IA réfléchit

            MovePieceUtilisateur(fromRank, fromColumn, toRank, toColumn);   // Déplace la pièce sur l'échiquier

            // Mouvement de l'ordinateur
            eng->userMoved(Chess::Move(Chess::Coord(fromColumn-1, fromRank-1), Chess::Coord(toColumn-1, toRank-1)),level);

            emit CalculenCours();                                           // Emet un signal pour dire que l'IA a fini de réfléchir
        }
    }

}


/**
 * @brief ChessBoard::MovePieceUtilisateur
 * @param fromRank
 * @param fromColumn
 * @param toRank
 * @param toColumn
 *
 * Fonction pour autoriser ou non le déplacement d'une pièce
 */
void ChessBoard::MovePieceUtilisateur(int fromRank, int fromColumn, int toRank, int toColumn){

    // Récupération de l'échiquier pour le "En Passant"
    if (!start){ m_algorithm->AncienBoardEnPassantBlack = m_boardData;}
    else{ m_algorithm->AncienBoardEnPassantWhite = m_boardData;}

    int index = (fromRank-1)*columns()+(fromColumn-1);      // Position de la pièce déplacée (avant déplacement)
    int new_index = (toRank-1)*columns()+(toColumn-1);      // Position de la pièce déplacée (après déplacement)

    QVector<int> Indexation_movement_test = PossiblemovePiece(!start, m_boardData); // Regarde le nombre de mouvements possibles

    bool IsCheck_test = m_algorithm->MiseEnEchec(!start, m_boardData);              // Regarde si l'on est en échec
    bool Check = Return_IsCheck();                                                  // Récupère si l'on en échec

    bool good_piece = player->Playable(index, m_boardData);                         // Vérification du mouvement pièces blanches ou noires

    // Informe si l'on peut réaliser le mouvement
    bool result = m_algorithm->accepted_movement(8,fromColumn, fromRank, toColumn, toRank, m_boardData);

    char PieceDeplacee = m_boardData[index];                    // Récupère la pièce déplacée
    QVector<char> m_boardData_tampon = m_boardData;             // Variable tampon

    if (result and good_piece){

        if (!Check){    // Acceptation de la mobilité des pièces (si le joueur n'est pas en échec)
            PathFunction(isPat, toColumn, toRank, fromColumn, fromRank); // Condition pour le Pat

            // Vérifier que si lorsque l'on fait un mouvement, on ne se retrouve pas en échec
            bool result = m_algorithm->accepted_movement(8,fromColumn, fromRank, toColumn, toRank, m_boardData);

            if (result and good_piece){    // Si le mouvement est autorisé
                PieceDeplacement(toColumn, toRank, fromColumn, fromRank);                         // Déplacement de la pièce
                m_algorithm->AsMove = m_algorithm->AsAlreadyMove(m_algorithm->AsMove, index, 1);  // La pièce a bougé
            }

            IsCheckV= m_algorithm->MiseEnEchec(!start, m_boardData);    // Calcul si l'on est en échec
            if (IsCheckV){                                              // Si c'est le cas, renvoie le board précédent
                m_boardData = m_boardData_tampon;
                m_algorithm->AsMove = m_algorithm->AsAlreadyMove(m_algorithm->AsMove, index, -1); // La pièce n'a finalement pas bougé
            }
            else{                            // Sinon
                player->who_turn(!start);    // Change le joueur car la pièce a bougé
                setPlayer(!start);
                MusicPlayMovement();         // Musique de déplacement

                if (EstMisEnEchec){
                    emit PlusMiseEnEchecs(); // Surlignage dans MainWindows
                    EstMisEnEchec = false;
                }

                IsCheckMate();               // Vérification du checkmate : On commence par vérifier le check
                Endgame();                   // Appel de la fin de la game
            }
        }

        else {  // Si le joueur est en échecs
            if (result and good_piece){    // Si le mouvement est autorisé
                PieceDeplacement(toColumn, toRank, fromColumn, fromRank);                        // Déplace la pièce
                m_algorithm->AsMove = m_algorithm->AsAlreadyMove(m_algorithm->AsMove, index, 1); // La pièce a bougé
            }

            IsCheckV= m_algorithm->MiseEnEchec(!start, m_boardData);    // Calcul si l'on est en échec
            if (IsCheckV){                                              // Si c'est le cas, renvoie le précédent board
                m_boardData = m_boardData_tampon;
                m_algorithm->AsMove = m_algorithm->AsAlreadyMove(m_algorithm->AsMove, index, -1); // La pièce n'a finalament pas bougé
            }
            else{                           // Sinon
                player->who_turn(!start);   // Change le joueur
                setPlayer(!start);
                MusicPlayMovement();        // Musique de déplacement

                if (EstMisEnEchec){
                    emit PlusMiseEnEchecs(); // Effacage du Surlignage dans MainWindows
                    EstMisEnEchec = false;
                }
            }
        }

        DeplacementEnPassant(index, new_index);                 // Boucle pour la suppression du pion adverse pour le "EnPassant"
        DeplacementTourRoque(IsCheck_test, PieceDeplacee, index, new_index, m_boardData_tampon, Indexation_movement_test); // Boucle pour le déplacement de la tour lors du Roque
        PromotionPion(IsCheck_test, PieceDeplacee, new_index);  // Fonction pour la promotion d'un pion
    }
}


/**
 * @brief ChessBoard::MusicPlayMovement
 *
 * Fonction pour jouer la musique d'un mouvement
 */
void ChessBoard::MusicPlayMovement(){SoundPlayer->play();}


/**
 * @brief ChessBoard::MusicPlayVictory
 *
 * Fonction pour jouer la musique de la victoire
 */
void ChessBoard::MusicPlayVictory(){SoundVictory->play();}


/**
 * @brief ChessBoard::MusicPlayPat
 *
 * Fonction pour jouer la musique d'égalité
 */
void ChessBoard::MusicPlayPat(){SoundPat->play();}


/**
 * @brief ChessBoard::ChangeSoundVolumeMovement
 * @param volume
 *
 * Fonction pour définir le volume de la musique d'un mouvement
 */
void ChessBoard::ChangeSoundVolumeMovement(int volume){SoundPlayer->setVolume(volume);}


/**
 * @brief ChessBoard::ChangeSoundVolumeVictory
 * @param volume
 *
 * Fonction pour définir le volume de la musique de la victoire
 */
void ChessBoard::ChangeSoundVolumeVictory(int volume){SoundVictory->setVolume(volume);}


/**
 * @brief ChessBoard::ChangeSoundVolumePat
 * @param volume
 *
 * Fonction pour définir le volume de la musique d'égalité
 */
void ChessBoard::ChangeSoundVolumePat(int volume){SoundPat->setVolume(volume);}


/**
 * @brief ChessBoard::Endgame
 *
 * Fonction permettant de vérifier si l'on est en échec et Mat ou s'il y a Pat
 */
void ChessBoard::Endgame(){
    if (m_algorithm->Return_IsCheckMate() == true){                         // Si l'on est Chekmate
        m_algorithm->CallIsCheckMate(!m_algorithm->Return_IsCheckMate());
        MusicPlayVictory();                                                 // Musique de victoire
        emit testChangment();                                               // Emet un signal
    }
    else if (isPat == true){                                                // Si l'on est en Pat
        MusicPlayPat();
        emit PatFinGame();                                                  // Emet un signal
    }
}


/**
 * @brief ChessBoard::PromotionPion
 *
 * Fonction permettant de vérifier si l'on peut promouvoir un pion
 */
void ChessBoard::PromotionPion(bool IsCheck_test, char PieceDeplacee, int new_index){
    if (!IsCheck_test){ // Condition pour promouvoir un pion
        if ((PieceDeplacee=='P' and new_index>55 and new_index<64) or (PieceDeplacee=='p' and new_index>-1 and new_index<8)){
            emit PromotionSignal(PieceDeplacee, new_index); // Emet un signal pour créer l'interface de promotion des pions
        }
    }
}


/**
 * @brief ChessBoard::DeplacementEnPassant
 * @param index
 * @param new_index
 *
 * Fonction permettant d'autoriser ou non la prise "En Passant"
 */
void ChessBoard::DeplacementEnPassant(int index, int new_index){
    // Conditions sur le nouveau board (pour les blancs)
    if (new_index+8<64 and new_index-8>-1 and index>23 and index<32 and ((abs(index-new_index)== 9) or (abs(index-new_index)== 7))){ // Condition sur le déplacement voulu
        if (m_algorithm->AncienBoardEnPassantBlack[new_index-8]=='P'){ // Condition sur l'ancien board
            if (m_boardData[new_index+8]=='P' and m_boardData[new_index]=='p'){
                setData(m_algorithm->GetColumnIndex(new_index)+1, m_algorithm->GetRankIndex(new_index)+1+1, ' ');
            }
        }
    }

    // Conditions sur le nouveau board (pour les noirs)
    if (!UseEngine){    // Résous un gros bug d'utilisation de l'IA
        if (new_index+8<64 and new_index-8>-1 and index>31 and index<40 and ((abs(index-new_index)== 9) or (abs(index-new_index)== 7))){
            if (m_algorithm->AncienBoardEnPassantWhite[new_index+8]=='p'){ // Condition sur l'ancien board
                if (m_boardData[new_index-8]=='p' and m_boardData[new_index]=='P'){
                    setData(m_algorithm->GetColumnIndex(new_index)+1, m_algorithm->GetRankIndex(new_index)+1-1, ' ');
                }
            }
        }
    }
}


/**
 * @brief ChessBoard::DeplacementTourRoque
 * @param IsCheck_test
 * @param PieceDeplacee
 * @param index
 * @param new_index
 * @param m_boardData_tampon
 * @param Indexation_movement_test
 *
 * Fonction permettant d'autoriser ou non le roque
 */
void ChessBoard::DeplacementTourRoque(bool IsCheck_test, char PieceDeplacee, int index, int new_index, QVector<char> m_boardData_tampon, QVector<int> Indexation_movement_test){

    if ((PieceDeplacee == 'k' or PieceDeplacee == 'K') and (abs(new_index-index) == 2)){ // Si l'on veut réaliser un roque
        bool RoqueCheck = false;
        bool sequence = false;

        if (IsCheck_test){  // Si l'on est en échec, on ne peut pas roque
            m_boardData = m_boardData_tampon;
            m_algorithm->AsMove = m_algorithm->AsAlreadyMove(m_algorithm->AsMove, index, -1);

            // Fais rejouer le joueur
            player->who_turn(!start);
            setPlayer(!start);
        }

        else{               // Sinon, on peut réaliser le roque

            // Roque blanc à gauche
            if (new_index == 2){
               for(int i=0; i<Indexation_movement_test.length();i++){
                   // On regarde si aucune pièce adverse peut atteindre le roi pendant le roque
                    if (Indexation_movement_test[i]==1  or Indexation_movement_test[i]==2 or Indexation_movement_test[i]==3){
                        if (Indexation_movement_test[i]==2){
                            sequence = true;
                        }
                        RoqueCheck = true;
                        break;
                    }
               }

               if (RoqueCheck == false){        // Si le roque peut s'effectuer
                   PieceDeplacement(4, 1, 1, 1);// Déplace la tour
               }
               else{                            // Sinon, restaure le board précédent
                   m_boardData = m_boardData_tampon;

                   if (!sequence){
                       m_algorithm->AsMove = m_algorithm->AsAlreadyMove(m_algorithm->AsMove, index, -1);

                       // Fais rejouer le joueur
                       player->who_turn(!start);
                       setPlayer(!start);
                   }
               }
            }

            // Roque blanc à droite
            if (new_index == 6){
                for(int i=0; i<Indexation_movement_test.length();i++){
                     if (Indexation_movement_test[i]==5  or Indexation_movement_test[i]==6){
                         if (Indexation_movement_test[i]==6){
                             sequence = true;
                         }
                         RoqueCheck = true;
                         break;
                     }
                }

                if (RoqueCheck == false){        // Si le roque peut s'effectuer
                    PieceDeplacement(6, 1, 8, 1);// Déplace la tour
                }
                else{                            // Sinon, restaure le board précédent
                    m_boardData = m_boardData_tampon;

                    if (!sequence){
                        m_algorithm->AsMove = m_algorithm->AsAlreadyMove(m_algorithm->AsMove, index, -1);

                        // Fais rejouer le joueur
                        player->who_turn(!start);
                        setPlayer(!start);
                    }
                }
            }

            // Roque noir à gauche
            if (new_index == 58){
               for(int i=0; i<Indexation_movement_test.length();i++){
                    if (Indexation_movement_test[i]==57  or Indexation_movement_test[i]==58 or Indexation_movement_test[i]==59){
                        if (Indexation_movement_test[i]==58){
                            sequence = true;
                        }
                        RoqueCheck = true;
                        break;
                    }
               }

               if (RoqueCheck == false){        // Si le roque peut s'effectuer
                   PieceDeplacement(4, 8, 1, 8);// Déplace la tour
               }
               else{                            // Sinon, restaure le board précédent
                   m_boardData = m_boardData_tampon;

                   if (!sequence){
                       m_algorithm->AsMove = m_algorithm->AsAlreadyMove(m_algorithm->AsMove, index, -1);

                       // Fais rejouer le joueur
                       player->who_turn(!start);
                       setPlayer(!start);
                   }
               }
            }

            // Roque noir à droite
            if (new_index == 62){
                for(int i=0; i<Indexation_movement_test.length();i++){
                     if (Indexation_movement_test[i]==61 or Indexation_movement_test[i]==62){
                         if (Indexation_movement_test[i]==62){
                             sequence = true;
                         }
                         RoqueCheck = true;
                         break;
                     }
                }

                if (RoqueCheck == false){        // Si le roque peut s'effectuer
                    PieceDeplacement(6, 8, 8, 8);// Déplace la tour
                }
                else{                            // Sinon, restaure le board précédent
                    m_boardData = m_boardData_tampon;

                    if (!sequence){
                        m_algorithm->AsMove = m_algorithm->AsAlreadyMove(m_algorithm->AsMove, index, -1);
                        player->who_turn(!start);   // Change le joueur
                        setPlayer(!start);
                    }
                }
            }
        }

        IsCheckMate();
    }

}


/**
 * @brief ChessBoard::IsCheckMate
 *
 * Définie si l'on est en échec et/ou échec et Mat
 */
void ChessBoard::IsCheckMate(){
   bool IsCheck_for_Checkmate = m_algorithm->MiseEnEchec(!start, m_boardData);

    if (IsCheck_for_Checkmate){                // Si l'on a mis l'adversaire mis en échec
        EstMisEnEchec = true;
        emit MiseEnEchecs();                   // Emet un signal pour signaler que le joueur est en échec

        bool check_arbre = CheckMateFunction();// Cherche les mouvements pour contrer l'échec
        if (check_arbre){                      // Si aucun mouvement ne permet de contrer l'échec
            m_algorithm->SetIsCheckMate(true); // On est CheckMate
        }
    }
}


/**
 * @brief ChessBoard::CheckMateFunction
 * @return
 *
 * Regarde si l'on est en échec et Mat
 */
bool ChessBoard::CheckMateFunction(){
    QVector<int> Indexation_checkmate_2 = PossiblemovePiece(start, m_boardData);    // Regarde toutes les cases atteignables

    bool false_movement = false;
    bool check_arbre = true;

    for(int i=0; i<Indexation_checkmate_2.length(); i++){                           // Pour chaque mouvement
        int Rank_Index =m_algorithm->GetRankIndex(Indexation_checkmate_2[i])+1;     // Indexation en ligne
        int Column_Index = m_algorithm->GetColumnIndex(Indexation_checkmate_2[i])+1;// Indexation en colonne

        // On veut savoir qu'elle pièce peut atteindre l'emplacement Indexation_checkmate_2[i]
        // Pour ça, on prend la position de chacune des pièces, puis on regarde si le mouvement
        // jusqu'à la position Indexation_checkmate_2[i] est possible

        for (int jj=0;jj<64;jj++){                                          // Pour chaque pièce de l'échiquier
            int PieceIndexationBlack, PieceIndexationWhite;
            PieceIndexationBlack = Black_pieces.indexOf(m_boardData[jj]);   // Pièce noire
            PieceIndexationWhite = White_pieces.indexOf(m_boardData[jj]);   // Pièce blanche

            if (((start == 1) and (PieceIndexationBlack != -1)) or ((start == 0) and (PieceIndexationWhite != -1))){
                int Rank_piece =m_algorithm->GetRankIndex(jj)+1;        // Ligne de la pièce j
                int Column_piece = m_algorithm->GetColumnIndex(jj)+1;   // Colonne de la pièce j

                // Reagrde si le mouvement est possible
                false_movement = m_algorithm->accepted_movement(8,Column_piece,Rank_piece,Column_Index, Rank_Index,m_boardData);

                if (false_movement == true){                            // Si le mouvement est possible
                    QVector<char> m_boardData_tampon2 = m_boardData;    // Variable tampon

                    PieceDeplacement(Column_Index, Rank_Index, Column_piece, Rank_piece);   // Déplacement de la pièce
                    bool IsCheck_nouveau = m_algorithm->MiseEnEchec(!start, m_boardData);   // Vérification que l'on n'est plus en échec

                    m_boardData = m_boardData_tampon2;  // Restaure le board précédent
                    if (!IsCheck_nouveau){              // Si l'on est plus en échec
                        check_arbre = false;            // Pas d'échec et mat
                        break;
                    }
                }
            }
        }

        if (check_arbre == false){ // Pour ne par refaire la boucle for
            break;
        }
    }

    // Si check_arbre == false, cela signifie que l'on n'est pas échec et Mat
    // Sinon, on l'est
    return check_arbre;
}


/**
 * @brief ChessBoard::PathFunction
 * @param isPat
 * @param toColumn
 * @param toRank
 * @param fromColumn
 * @param fromRank
 *
 * Fonction vérifiant s'il y a égalité
 */
void ChessBoard::PathFunction(bool &isPat, int toColumn, int toRank, int fromColumn, int fromRank){
    QVector<char> m_boardData_tampon = m_boardData;             // Variable tampon

    PieceDeplacement(toColumn, toRank, fromColumn, fromRank);   // Déplacement de la pièce


    //
    // Comptage de toutes les pièces sur l'échiquier
    //
    int nrPionB = m_boardData.count('P');
    int nrPionN = m_boardData.count('p');

    int nrTourB = m_boardData.count('R');
    int nrTourN = m_boardData.count('r');

    int nrCavalierB = m_boardData.count('N');
    int nrCavalierN = m_boardData.count('n');

    int nrDameB = m_boardData.count('Q');
    int nrDameN = m_boardData.count('q');

    int nrFouB = m_boardData.count('B');
    int nrFouN = m_boardData.count('b');

    int sigma = nrPionB+nrPionN+nrTourB+nrTourN+nrCavalierB+nrCavalierN+nrDameB+nrDameN+nrFouB+nrFouN;
    int sigmaN = nrPionN+nrTourN+nrCavalierN+nrDameN+nrFouN;
    int sigmaB = nrPionB+nrTourB+nrCavalierB+nrDameB+nrFouB;

    QVector<int> Indexation_movement_test2 = PossiblemovePiece(!start, m_boardData);    // Mouvements possibles

    QList<int> PositionPieces = m_algorithm->PositionPieces(!start, m_boardData);       // Positionnement des pièces
    bool SolutionPat = false;
    bool ResultPat, CheckPat;

    // Condition de pat à cause d'un manque de pièce
    if (( sigma== 0) or                                                                            // Roi contre roi
            (sigma == 2 and (nrCavalierN==2 or nrCavalierB==2)) or                                 // Roi contre roi + deux cavaliers
            (sigma == 2 and ((nrCavalierN==1 and nrFouN==1) or (nrCavalierB==1 and nrFouB==1))) or // Roi contre roi + 1 cavalier et 1 fou
            (sigma == 1 and (nrCavalierN==1 or nrCavalierB==1)) or                                 // Roi contre roi + un cavalier
            (sigma == 1 and (nrFouN==1 or nrFouB==1)) or
            (sigmaB <= 1 and (nrCavalierB==1 or nrFouB==1) and sigmaN <= 1 and (nrCavalierN==1 or nrFouN==1))){                                            // Roi contre roi + un fou
       // Rien ne se passe
    }
    else{
        // Pour chaque pièce
        for(int i=0;i<PositionPieces.length();i++){
            int fromRank1 = m_algorithm->GetRankIndex(PositionPieces[i])+1;
            int fromColumn1 = m_algorithm->GetColumnIndex(PositionPieces[i])+1;

            // Pour chaque case de l'échiquier
            for(int j=0;j<Indexation_movement_test2.length();j++){
                int toRank1 = m_algorithm->GetRankIndex(Indexation_movement_test2[j])+1;
                int toColumn1 = m_algorithm->GetColumnIndex(Indexation_movement_test2[j])+1;

                // On regarde si le mouvement est possible
                ResultPat =  m_algorithm->accepted_movement(8, fromColumn1, fromRank1, toColumn1, toRank1, m_boardData);

                // Si c'est le cas
                if (ResultPat){
                    QVector<char> m_boardData_tampon3 = m_boardData;
                    PieceDeplacement(toColumn1, toRank1, fromColumn1, fromRank1); // Déplace la pièce
                    CheckPat = m_algorithm->MiseEnEchec(start, m_boardData);      // on vérifie que l'on est pas mis en échec après le déplacement
                    m_boardData = m_boardData_tampon3;                            // Restaure le board précédent
                }

                // Si on peut se déplacer (ResultPat) et qu'il n'y a pas échec
                if (ResultPat and !CheckPat){
                    SolutionPat = true;         // Il n'y a pas Pat
                    break;
                }
            }

            if (SolutionPat){                   //  Il n'y a pas Pat
                break;
            }
        }
    }

    if (!SolutionPat){isPat = true;}        // Si aucune pièce ne peut se déplacer, il y a pat
    else{isPat = false;}                    // Retourne qu'il n'y a pas Pat

    m_boardData = m_boardData_tampon;
}


/**
 * @brief ChessBoard::setPlayer
 * @param start2
 *
 * Fonction pour émettre le fait que le joueur change
 */
void ChessBoard::setPlayer(bool start2){
    if(start == start2)
        return;

    start = start2;
    emit playerChanged(start);  // Emet un signal pour signaler que le joueur a changé
}


/**
 * @brief ChessBoard::setRanks
 * @param newRanks
 *
 * Renvoie le changement de ligne d'une pièce
 */
void ChessBoard::setRanks(int newRanks){
    if(ranks() == newRanks)
        return;

    m_ranks = newRanks;
    initBoard();
    emit ranksChanged(m_ranks);     // Emet un signal pour signaler que la ligne a changé
}


/**
 * @brief ChessBoard::setColumns
 * @param newColumns
 *
 * Renvoie le changement de colonne d'une pièce
 */
void ChessBoard::setColumns(int newColumns){
    if(columns() == newColumns) return;
    m_columns = newColumns;
    initBoard();
    emit columnsChanged(m_columns);  // Emet un signal pour signaler que la colonne a changé
}



/**
 * @brief ChessBoard::initBoard
 *
 * Emet le board sans aucune pièce
 */
void ChessBoard::initBoard(){
    m_boardData.fill(' ', ranks()*columns());
    emit boardReset();                // Emet un signal pour initialiser le board
}


/**
 * @brief ChessBoard::setDataInternal
 * @param column
 * @param rank
 * @param value
 * @return
 *
 * Définie les pièces dans la matrice représentant le board
 */
bool ChessBoard::setDataInternal(int column, int rank, char value){
    int index = (rank-1)*columns()+(column-1);

    if(m_boardData.at(index) == value)
        return false;

    m_boardData[index] = value;
    return true;
}


/**
 * @brief ChessBoard::DefinePlayerStart
 * @param fen
 *
 * Défini, quel joueur joue à partir du FEN
 */
void ChessBoard::DefinePlayerStart(const QString &fen){
    for(int i=1;i<fen.length();i++) {

        if ((fen.at(i) == 'w' or fen.at(i) == 'b') and fen.at(i-1) == ' '){
            if (fen.at(i) == 'w'){  // Si la FEN contient un w, c'est le joueur blanc qui commence
                start = 0;
                break;
            }
            else{                   // Si la FEN contient un b, c'est le joueur noir qui commence
                start = 1;
                break;
            }
        }
    }
}


/**
 * @brief ChessBoard::setFen
 * @param fen
 *
 * Définie la position à partir d'une FEN
 */
void ChessBoard::setFen(const QString &fen){
    int index = 0;
    int skip = 0;
    const int columnCount = columns();

    QChar ch;

    for(int rank = ranks(); rank >0; --rank) {                  // Pour chaque ligne
        for(int column = 1; column <= columnCount; ++column) {  // Pour chaque colonne
            if(skip > 0) {
                ch = ' ';
                skip--;
            } else {
                ch = fen.at(index++);
                if(ch.isDigit()) {                              // Si la lette est un nombre
                    skip = ch.toLatin1()-'0';                   // Convertis le char en nombre
                    ch = ' ';
                    skip--;
                }
            }
            setDataInternal(column, rank, ch.toLatin1());       // Positionne dans la matrice la pièce
        }

        QChar next = fen.at(index++);

        if(next != '/' && next != ' ') {
            initBoard();
            return;
        }
    }

    DefinePlayerStart(fen);                                     // Définie quel joueur commence
    player->who_turn(start);

    emit boardReset();
}


/**
 * @brief ChessBoard::Change_m_boardData
 * @param PieceDeplacee
 * @param index
 *
 * Fonction pour positionner la nouvelle pièce jouée
 */
void ChessBoard::Change_m_boardData(char PieceDeplacee, int index){
    m_boardData[index] = PieceDeplacee;                         // Déplace dans la matrice du board la pièce déplacée du board
}


/**
 * @brief ChessBoard::testUi
 * @param originfile
 * @param originrank
 * @param targetfile
 * @param targetrank
 *
 * Fonction pour l'IA : Déplace sur le board la pièce que l'IA a joué
 */
void ChessBoard::testUi(int originfile, int originrank, int targetfile, int targetrank){

    // Pour les noirs
    IsCheckV = m_algorithm->MiseEnEchec(!start, m_boardData);    // Calcul si l'on est en échec

    PieceDeplacement(targetfile+1, targetrank+1, originfile+1, originrank+1);
    IsCheckMate();

    if (EstMisEnEchec and Return_IsCheck()){
        emit PlusMiseEnEchecs(); // Surlignage dans MainWindows
        EstMisEnEchec = false;
    }

    player->who_turn(!start);    // Change le joueur
    setPlayer(!start);


    // Pour les blancs
    IsCheckV = m_algorithm->MiseEnEchec(!start, m_boardData);    // Calcul si l'on est en échec
    IsCheckMate();


}

