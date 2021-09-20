#include "ia.h"
#include "iostream"
#include <random>
#include "QElapsedTimer"
#include "QList"

IA::IA(){}


int IA::EvaluateBoard(QVector<char> m_boardData, bool start){
    int sum = 0;

    if (start ==1){ // Noir
        sum = m_boardData.count('r')*Values[0] + m_boardData.count('n')*Values[1] + m_boardData.count('b')*Values[2] +
              m_boardData.count('q')*Values[3] + m_boardData.count('k')*Values[4] + m_boardData.count('p')*Values[5];
    }
    else{
        sum = m_boardData.count('R')*Values[0] + m_boardData.count('N')*Values[1] + m_boardData.count('B')*Values[2] +
              m_boardData.count('Q')*Values[3] + m_boardData.count('K')*Values[4] +m_boardData.count('P')*Values[5];
    }
    return sum;
}


///////////////////////////////////////////// A continuer /////////////////////////////////////////////////////////////////
/// \brief IA::BestMoveCalcul
/// \param m_boardData
/// \param start
/// \return
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// // Que pour les noirs pour le moment mais semble fontionner
QList<int> IA::BestMoveCalcul(QVector<char> m_boardData, bool start){

    int maxValue = 0; int maxPositionBefore = 0; int maxPositionAfter = 0;
    if (start==1){ maxValue = 100000;}
    QVector<int> const Indexation_movements = MovementsIndexation(m_boardData, start);
    QVector<char> const m_boardData_tampon2 = m_boardData;    // Variable tampon

//    std::random_device rd;
//    std::mt19937 g(rd());
//    std::shuffle(Indexation_movements.begin(), Indexation_movements.end(),g);

    bool false_movement = false;
    for(int i=0; i<Indexation_movements.length(); i++){
        int Rank_Index = m_algorithmIA->GetRankIndex(Indexation_movements[i])+1; int Column_Index = m_algorithmIA->GetColumnIndex(Indexation_movements[i])+1;

        // On veut savoir qu'elle pièce peut atteidre l'emplacement Indexation_movements[i]
        // Pour ça, on prend la position de chacune des pièces, puis on regarde si le mouvement
        // jusqu'à la position Indexation_checkmate_2[i] est possible

        for (int jj=0;jj<64;jj++){      // Pour chaque pièce de l'échiquier

            int k = Black_pieces.indexOf(m_boardData[jj]);              // Si c'est une pièce noire (start == 1)
            int kw = White_pieces.indexOf(m_boardData[jj]);             // Si c'est une pièce blanche (start == 0)
            if (((start == 1) and (k !=-1)) or ((start == 0) and (kw !=-1))){

                // Index de la pièce jj
                int Rank_piece =m_algorithmIA->GetRankIndex(jj)+1; int Column_piece = m_algorithmIA->GetColumnIndex(jj)+1;

                // On regarde si le mouvement à la position Indexation_movements[i] est possible
                false_movement = m_algorithmIA->accepted_movement(8,Column_piece,Rank_piece,Column_Index, Rank_Index,m_boardData);

                if (false_movement){                                    // Si le movement est possible

                    // Déplacement de la pièce (Erreur possible ici)
                    setDataInternalIA(m_boardData, Column_Index, Rank_Index, dataIA(m_boardData, Column_piece, Rank_piece));
                    setDataInternalIA(m_boardData, Column_piece, Rank_piece, ' ');

                    int CalculateValue = EvaluateBoard(m_boardData,!start);  // Evaluation de la meilleure position
//                    std::cout << "CalculateValue | "<< CalculateValue << std::endl;

                    // Position de la meilleure évaluation (A BIEN VERIFIER)
                    if((start and CalculateValue<maxValue) or (!start and CalculateValue>maxValue)){ //or (!start and CalculateValue<maxValue)){
                        maxValue = CalculateValue;
                        maxPositionBefore = jj;
                        maxPositionAfter = Indexation_movements[i];
                    }
                    m_boardData = m_boardData_tampon2;                      // Retour position d'avant
                }
            }
        }
    }

//    std::cout << "start | "<< start <<" | maxPositionBefore | "<< maxPositionBefore << " | maxPositionAfter | " << maxPositionAfter << "maxValue" << maxValue << std::endl;
    return QList<int> {maxPositionBefore,maxPositionAfter, maxValue};
}







QList<int> IA::BestMoveCalculDepthPruning(QVector<char> m_boardData, int depth, bool start, int alpha, int beta){

    if (depth == 1) { return BestMoveCalcul(m_boardData, start); }
    else {

        int maxValue = -1; int maxPositionBefore = 0; int maxPositionAfter = 0;
        bool false_movement = false;
        if (start == 1){maxValue = 20000;}

        QVector<char> const m_boardData_tampon2 = m_boardData;                             // Variable tampon


//      Gros problème d'optimisation ici
        QVector<int> const Indexation_movements = MovementsIndexation(m_boardData, start); // Regarde toutes cases atteignables
//        std::cout << "Methode 2 | " << timer.elapsed() << " milliseconds |" << std::endl;



//        QElapsedTimer timer;
//        timer.start();

        // Toutes les position des pièces noires
        QList<int> ListePositionPieceColours = FIndBlack(m_boardData);                     // Start == 1
        if (start == 0){ ListePositionPieceColours = FIndWhite(m_boardData);}              // Toutes les position des pièces blanches

//        count += timer.elapsed();
//        std::cout <<count << std::endl;

//        std::random_device rd;
//        std::mt19937 g(rd());
//        std::shuffle(Indexation_movements.begin(), Indexation_movements.end(),g);


        for(int i=0; i<Indexation_movements.length(); i++){
            int Rank_Index = m_algorithmIA->GetRankIndex(Indexation_movements[i])+1;     // Ligne de Indexation_checkmate_2[i]
            int Column_Index = m_algorithmIA->GetColumnIndex(Indexation_movements[i])+1; // Colonne de Indexation_checkmate_2[i]

            for (int jj=0;jj<ListePositionPieceColours.length();jj++){ // Pour chaque pièce
                int Rank_piece = m_algorithmIA->GetRankIndex(ListePositionPieceColours[jj])+1; int Column_piece = m_algorithmIA->GetColumnIndex(ListePositionPieceColours[jj])+1; // Colonne de pièce[j]

                false_movement = m_algorithmIA->accepted_movement(8,Column_piece,Rank_piece,Column_Index, Rank_Index, m_boardData);
                if (false_movement){                                    // Si le movement est possible

                    // Déplacement de la pièce
                    setDataInternalIA(m_boardData, Column_Index, Rank_Index, dataIA(m_boardData, Column_piece, Rank_piece));
                    setDataInternalIA(m_boardData, Column_piece, Rank_piece, ' ');

                    QElapsedTimer timer;
                    timer.start();


                    int CalculateValue = BestMoveCalculDepthPruning(m_boardData, depth-1,!start, alpha, beta)[2]; // Evaluation de la meilleure position

//                    std::cout << "Depth |" << depth <<" Methode 2 | " << timer.elapsed() << " milliseconds |" << std::endl;


                    if((start and CalculateValue<maxValue) or (!start and CalculateValue>maxValue)){
                        maxValue = CalculateValue;
                        maxPositionBefore = ListePositionPieceColours[jj];
                        maxPositionAfter = Indexation_movements[i];

                        if (!start and CalculateValue>maxValue){ alpha = qMax(alpha, CalculateValue); }
                        else{ beta = qMin(alpha, CalculateValue); }
                    }

                    m_boardData = m_boardData_tampon2; // Retour position d'avant
                    if (beta <= alpha){
                        break;
                    }
                }
            }
        }
        return QList<int> {maxPositionBefore,maxPositionAfter, maxValue};
    }
}









QVector<int> IA::MovementsIndexation(QVector<char> const m_boardData, bool const start) const{
    QVector<int> const Possible_movements = m_algorithmIA->all_possible_movement(start, m_boardData);
    QVector<int> const Indexation_movements = m_algorithmIA->Indexation_possible_movement(Possible_movements);
    return Indexation_movements;
}


char IA::dataIA(QVector<char> const m_boardData, int const column, int const rank) const{
    return m_boardData.at((rank-1)*8+(column-1));
}


QList<int> IA::FIndBlack(QVector<char> const m_boardData){
    QList<int> Result;

    for (int jj=0;jj<64;jj++){
        if (Black_pieces.indexOf(m_boardData[jj]) != -1){
            Result.append(jj);
        }
    }
    return Result;
}


QList<int> IA::FIndWhite(QVector<char> const m_boardData){
    QList<int> Result;

    for (int jj=0;jj<64;jj++){
        if (White_pieces.indexOf(m_boardData[jj]) != -1){
            Result.append(jj);
        }
    }
    return Result;
}


void IA::setDataInternalIA(QVector<char> &m_boardData, int const column, int const rank, char const value){
    m_boardData[(rank-1)*8+(column-1)] = value;
}

