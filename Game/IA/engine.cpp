#include "engine.h"
#include <QCoreApplication>
#include "iostream"

namespace Chess {

/**
 * @brief Engine::Engine
 * @param parent
 *
 * Constructeur de l'IA
 */
Engine::Engine(QObject *parent) : QObject(parent){}


/**
 * @brief Engine::userMoved
 * @param userMove
 * @param alpha
 *
 * Déplacement de la pièce sur le plateau de jeu
 */
void Engine::userMoved(Move userMove, int alpha){
    Vector<Move> possibleMoves = board.possibleMoves(board.side());      // Mouvements possibles
    std::random_shuffle(possibleMoves.begin(), possibleMoves.end());     // Mouvements jamais dans le même ordre

    Move validMove, minimaxMove;

    QCoreApplication::processEvents();                                   // Autorise la modification graphique pendant le calcul

    for (Move move : possibleMoves){                                     // Pour tous les mouvements possibles
        if ( move.sameVector(userMove)){
            validMove = move;
            break;
        }
    }

    if ( validMove.isValid() ) {makeMove(validMove);}                   // Si le mouvement est possible, on le fait

    SearchRequest request;                                              // Crée un objet pour utiliser l'algorithme minimax
    request.board = board;                                              // Insertion de l'echiquier
    request.depth = alpha;                                              // Insertion de la profondeur de la recherche
    request.movesFilter = Vector<Move>();

    SearchResult result = minimax.search(request);
    minimaxMove = result.moves.size() > 0 ? result.moves[0] : Move();

    QCoreApplication::processEvents();

    if (minimaxMove.isValid()) {                                        // Si le mouvement est possible
        makeMove(minimaxMove);                                          // On le réalise

        Chess::Coord testorigin = minimaxMove.origin();                 // Coordonnée initiale
        Chess::Coord testtarget = minimaxMove.target();                 // Coordonnée finale

        // Position lignes et colonnes position de départ et d'arrivée
        int originfile = testorigin.file(); int originrank = testorigin.rank();
        int targetfile = testtarget.file(); int targetrank = testtarget.rank();
        setappli(originfile,originrank,targetfile,targetrank);          // Emet un signal pour signaler qu'une pièce a été déplacée
    }
}


/**
 * @brief Engine::makeMove
 * @param move
 *
 * Fonction pour réaliser un mouvement dans l'IA
 */
void Engine::makeMove(Move move){ board.make(move);}


/**
 * @brief Engine::setPiece
 * @param coord
 * @param piece
 *
 * Fonction pour positionner les pièces sur l'échiquier
 */
void Engine::setPiece(Coord coord, Piece piece){board.setPiece(coord, piece);}


/**
 * @brief Engine::setBoard
 * @param newBoard
 *
 * Fonction pour redéfinir le board
 */
void Engine::setBoard(Board newBoard){ this->board = newBoard;}


/**
 * @brief Engine::setappli
 * @param a
 * @param b
 * @param c
 * @param d
 *
 * Fonction permettant de transmettre l'information que l'échiquier de l'IA a changé
 * C'est la fonction de connection entre l'IA et l'UI
 */
void Engine::setappli(int a, int b, int c, int d){ emit boardapplichanged(a,b,c,d);}

} // Namespace Chess
