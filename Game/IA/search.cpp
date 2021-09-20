#include "search.h"
#include "enginetypes.h"
#include "evaluate.h"

namespace Chess {

// Fonction générique
// Tout d'abord, le mot-clé template prévient le compilateur que la prochaine chose dont on va lui
// parler sera générique ;
// Puis le mot-clé typename indique au compilateur que T sera le nom que l'on va utiliser
template<typename T>
Vector<Vector<T> > splitVector(const Vector<T>& vect, int splitTo=2);

/**
 * @brief Search::Search
 * @param threadsCount
 *
 * Constructeur de l'algorithme minimax
 */
Search::Search(unsigned int threadsCount){
    threads.resize(threadsCount);                       // Définis la taille des threads
    for(int i = 0; i< (int) threadsCount; ++i) {        // Pour chaque thread
        threads[i] = new MinimaxSearchThread();         // Crée un algorithme minimax
    }
}

SearchResult Search::search(SearchRequest request){
    Vector<Move> possibleMoves = request.board.possibleMoves(request.board.side()); // Obtiens tous les mouvements possibles
    Vector<Vector<Move>> threadMoves = splitVector(possibleMoves, threads.size() ); // Divise le nombre de mouvements entre tous les threads

    for (int i=0; i < (int) threads.size(); ++i){       // Pour chaque thread
        SearchRequest threadRequest = request;          // Renvoie les paramètres donnés par l'échiquier
        threadRequest.movesFilter = threadMoves[i];
        threads[i]->setSearchRequest(threadRequest);
        threads[i]->start();
    }

    Vector<SearchResult> threadResults(threads.size());

    for (int i=0; i < (int) threads.size(); ++i){
        threadResults[i] = threads[i]->getSearchResult();
    }

    std::random_shuffle(threadResults.begin(), threadResults.end());            // Rends aléatoire l'ordre des mouvements

    SearchResult result;
    result.request = request;
    result.moveCnt = 0;
    result.score = (request.board.side() == Piece::White) ? -100000 : +100000;  // Maximise ou minimise l'évaluation de l'échiquier

    for (int i=0; i < (int) threads.size(); ++i) {      // Pour chaque thread
        result.moveCnt += threadResults[i].moveCnt;

        // Si les pièces blanches maximisent le score
        if (request.board.side() == Piece::White && threadResults[i].score > result.score) {
            result.score = threadResults[i].score;      // Change le score maximal
            result.moves = threadResults[i].moves;      // Change le mouvement à éxecuter

        // Si les pièces noires minimisent le score
        } else if (request.board.side() == Piece::Black && threadResults[i].score < result.score) {
            result.score = threadResults[i].score;      // Change le score minimal
            result.moves = threadResults[i].moves;      // Change le mouvement à éxecuter
        }
    }

    return result;
}


/**
 * @brief MinimaxSearchThread::setSearchRequest
 * @param request
 *
 * Initialise une recherche en fonction des paramètres demandés
 */
void MinimaxSearchThread::setSearchRequest(const SearchRequest &request){
    board = request.board;

    sr.request = request;
    sr.moves.clear();                   // Réinitialise les mouvements
    sr.moves.resize(sr.request.depth);  // Initialise la profondeur de recherche
    sr.moveCnt = 0;                     // Réinitialise le compteur de mouvements
}


/**
 * @brief MinimaxSearchThread::getSearchResult
 * @return
 *
 * Fonction renvoyant la recherche du mouvement
 */
SearchResult MinimaxSearchThread::getSearchResult(){
    waitForFinish();                    // Modification du thread
    return sr;                          // Retourne la recherche du mouvement
}


/**
 * @brief MinimaxSearchThread::minimax
 * @param depth
 * @return
 *
 * Algorithme du minimax
 */
real MinimaxSearchThread::minimax(int depth){
    if (depth == 0)                                             // Profondeur nulle
        return Evaluate::position(board);                       // Renvoie l'évaluation de l'échiquier

    Vector<Move> movesList;                                     // Vecteur mouvement
    if (depth == sr.request.depth && sr.request.movesFilter.size() > 0 )
        movesList = sr.request.movesFilter;
    else
        movesList = board.possibleMoves(board.side());          // Recherche des mouvements possibles

    std::random_shuffle(movesList.begin(), movesList.end());    // Ordre des coups aléatoires


    if (movesList.size() == 0) {                                // Si aucun coup n'est possible
        if (board.isKingAttacked(board.side())) {               // Si le roi est attaqué
            // Calcul s'il y a échec et mat
            return board.side() == Piece::White ? -1000.0+(sr.request.depth-depth) : +1000.0-(sr.request.depth-depth);
        } else {                                                // Sinon, le roi n'est pas attaqué
            return 0.0;                                         // Egalité
        }
    }

    // Définis la valeur du meilleur mouvement possible
    float bestValue = (board.side() == Piece::White) ? -100000 : +100000;

    for (Move move : movesList) {                               // Pour chaque mouvement
        board.make(move);                                       // On le réalise
        float val = minimax( depth-1 );                         // Récursion de la fonction pour la prodondeur précédente

        if (board.side() != Piece::White && val > bestValue) {
            bestValue = val;                                    // Sauvegarde de la valeur du meilleur mouvement
            sr.moves[sr.request.depth-depth] = move;            // Sauvegarde du meilleur mouvement
        } else if (board.side() != Piece::Black && val < bestValue) {
            bestValue = val;                                    // Sauvegarde de la valeur du meilleur mouvement
            sr.moves[sr.request.depth-depth] = move;            // Sauvegarde du meilleur mouvement
        }

        board.unmake();                                         // Annule le coup
        sr.moveCnt++;                                           // Incrémente le nombre de mouvements réalisés
    }

    return bestValue;                                           // Renvoie la valeur du meilleur coup
}


/**
 * @brief MinimaxSearchThread::run
 *
 * Fonction pour appeler l'utilisation de l'algorithme minimax
 */
void MinimaxSearchThread::run(){sr.score = minimax(sr.request.depth);}



template<typename T>
/**
 * @brief splitVector
 * @param vect
 * @param splitTo
 * @return
 *
 * Fonction pour réaliser la division de tous les mouvements pour le nombre de thread
 */
Vector<Vector<T> > splitVector(const Vector<T>& vect, int splitTo) {

    Vector<Vector<T>> result(splitTo);              // Vecteur résultat

    int splitSize = vect.size()/splitTo;            // Nombre d'élément par thread
    splitSize     = std::max(splitSize, 1);         // Vérifie la division par 0

    for (int i = 0; i < (int) vect.size(); ++i) {
        int whichSplit = (i / splitSize) % splitTo; // Indéxation
        result[whichSplit].push_back(vect[i]);      // Partition du vecteur mouvement
    }
    return result;
}

} // Namespace Chess

