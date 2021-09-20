#include "evaluate.h"

namespace Chess {

/**
 * @brief Evaluate::position
 * @param board
 * @return
 *
 * Fonction permettant d'évaluer la position des pièces sur l'échiquier
 */
real Evaluate::position(const Board &board){
    uint8 pieceCount[2][7] = {};

    // Compte les pièces de chaque couleur et de chaque type
    for (int i=0; i<64; ++i) {
        Piece p = board.piece(i);
        pieceCount[p.color()][p.type()]++;
    }

    // Evalue la position
    qreal material =
              9.0 * (pieceCount[Piece::White][Piece::Queen]  - pieceCount[Piece::Black][Piece::Queen])
            + 5.0 * (pieceCount[Piece::White][Piece::Rook]   - pieceCount[Piece::Black][Piece::Rook])
            + 3.0 * (pieceCount[Piece::White][Piece::Bishop] - pieceCount[Piece::Black][Piece::Bishop])
            + 3.0 * (pieceCount[Piece::White][Piece::Knight] - pieceCount[Piece::Black][Piece::Knight])
            + 1.0 * (pieceCount[Piece::White][Piece::Pawn]   - pieceCount[Piece::Black][Piece::Pawn]);


    return material;
}

} // Namespace Chess
