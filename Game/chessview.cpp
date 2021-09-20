#include "chessview.h"
#include "chessboard.h"
#include <QMouseEvent>
#include <QPainter>
#include "iostream"
#include "QGraphicsTextItem"

#include "QTimer"

#include <math.h>
#define M_PI 3.14159265358979323846


/**
 * @brief ChessView::ChessView
 * @param parent
 *
 * Constructeur graphique de l'Ui
 */
ChessView::ChessView(QWidget *parent) : QWidget(parent){
    this->grabKeyboard();   // Récupère le clavier
}


/**
 * @brief ChessView::setBoard
 * @param board
 *
 * Connecte tous les signaux de l'algorithme au board
 */
void ChessView::setBoard(ChessBoard *board){
    if(m_board == board)
        return;

    if(m_board) {
        m_board->disconnect(this); // Déconnecte toutes les connexions de signaux entre m_board et ceci
    }

    m_board = board;

    if(board){ // Connecte les signaux
        connect(board, SIGNAL(dataChanged(int,int)), this, SLOT(update()));                 // Si une pièce est déplacée
        connect(board, SIGNAL(boardReset()), this, SLOT(update()));                         // Si on réinitialise le board

        connect(board, SIGNAL(playerChanged(int)), this, SLOT(update()));                   // Si le joueur change

        connect(board, SIGNAL(testChangment()), this, SLOT(tentative_Emission()));          // Si l'on est en échec et Mat

        connect(board, SIGNAL(MiseEnEchecs()), this, SLOT(EmissionMiseEnEchec()));          // Si l'on est mis en échec
        connect(board, SIGNAL(PlusMiseEnEchecs()), this, SLOT(EmissionPlusMiseEnEchec()));  // Si l'on n'est plus mis en échec

        connect(board, SIGNAL(PatFinGame()), this, SLOT(EmissionPatFinGame()));             // S'il y a Pat
        connect(board, SIGNAL(PromotionSignal(char, int)), this, SLOT(EmissionPromotionSignal(char, int))); // S'il y a promotion d'un pion
    }

    updateGeometry();   // Mise à jour de la géométrie
}


/**
 * @brief ChessView::ChessView::board
 * @return
 *
 * Fonction qui retourne le board
 */
ChessBoard *ChessView::ChessView::board() const {return m_board;}


/**
 * @brief ChessView::fieldSize
 * @return
 *
 * Renvoie la taille du champ (d'un carré de l'échiquier)
 */
QSize ChessView::fieldSize() const{
    return m_fieldSize;
}


/**
 * @brief ChessView::setFieldSize
 * @param fieldSize
 *
 * Définie la taille du champ (d'un carré de l'échiquier)
 */
void ChessView::setFieldSize(QSize fieldSize){
    if (m_fieldSize == fieldSize)
        return;

    m_fieldSize = fieldSize;
    emit fieldSizeChanged(fieldSize);
    updateGeometry();
}


/**
 * @brief ChessView::sizeHint
 * @return
 *
 * Définie la taille du board
 */
QSize ChessView::sizeHint() const{
    if(!m_board)
        return QSize(100,100);

    // Taille du plateau
    QSize boardSize = QSize(fieldSize().width()*m_board->columns()+1, m_fieldSize.height()*m_board->ranks()+1);

    // Bords de la figure
    int rankSize = fontMetrics().maxWidth()+200;
    int columnSize = fontMetrics().height()+4;

    // Retourne la taille de la fenêtre
    return boardSize+QSize(rankSize, columnSize);
}


/**
 * @brief ChessView::fieldRect
 * @param column
 * @param rank
 * @return
 *
 * Définie graphiquement le board
 */
QRect ChessView::fieldRect(int column, int rank) const {
    if(!m_board)
        return QRect();

    const QSize fs = fieldSize();   // Récupère la taille du board

    // Création d'un rectangle
    QRect fRect = QRect(QPoint((column-1)*fs.width(), (m_board->ranks()-rank)*fs.height()), fs);

    int offset = fontMetrics().maxWidth();
    return fRect.translated(offset+4, 0);
}


/**
 * @brief ChessView::setPiece
 * @param type
 * @param icon
 *
 * Fonction permettant d'implémenter des icônes sur les cases
 */
void ChessView::setPiece(char type, const QIcon &icon){
    m_pieces.insert(type, icon);    // Insertion d'une pièce
    update();                       // Met à jour le board
}


/**
 * @brief ChessView::piece
 * @param type
 * @return
 *
 * Retourne le type de la pièce
 */
QIcon ChessView::piece(char type) const{
    return m_pieces.value(type, QIcon());
}


/**
 * @brief ChessView::paintEvent
 *
 * Affichage graphique de tous les éléments de l'application
 */
void ChessView::paintEvent(QPaintEvent *){
    if(!m_board)
        return;

    QPainter painter(this); // Création d'un painter pour tracer les éléments sur notre fenêtre

    // Dessine les nombres de 1 à 8 sur le côté du board
    for(int r = m_board->ranks(); r>0; --r) {
        painter.save();
        drawRank(&painter, r);
        painter.restore();
    }


    // Dessine les lettre de a à h sur le bas du board
    for(int c = 1; c<=m_board->columns();++c) {
        painter.save();
        drawColumn(&painter, c);
        painter.restore();
    }

    // Dessine le quadrillage
    for(int r = 1; r<=m_board->ranks();++r) {
        for(int c = 1; c<=m_board->columns();++c) {
            painter.save();
            drawField(&painter, c, r);
            painter.restore();
        }
    }

    // Dessin des surlignages
    drawHighlights(&painter);

    // Dessine toutes les pièces
    for(int r = m_board->ranks(); r>0; --r) {
        for(int c = 1; c<=m_board->columns();++c) {
            drawPiece(&painter, c, r);
        }
    }

    // Dessine les deux rectangles sur les côtés contenant le roi
    int length = 75;
    drawPlayerRectangle(&painter,sizeHint().rwidth()-100-length/2, sizeHint().rheight()*1/4-length/2, 'k');
    drawPlayerRectangle(&painter,sizeHint().rwidth()-100-length/2, sizeHint().rheight()*3/4-length/2, 'K');

    // Dessine quel joueur joue
    drawPlayerTurn(&painter);

    // Dessine toutes les flèches faites
    DrawLineWithArrow(&painter);


    // Cercle en haut à gauche de la fenêtre pour l'IA
    if (IaCalculation){
        painter.setBrush(QBrush(Qt::red));
        painter.setPen(Qt::red);
        DrawIaCalculation(&painter);
    }
    else{
        painter.setBrush(QBrush(Qt::gray));
        painter.setPen(Qt::gray);
        DrawIaCalculation(&painter);
    }

}


/**
 * @brief ChessView::drawRank
 * @param painter
 * @param rank
 *
 * Fonction pour dessiner les lignes de 1 à 8
 */
void ChessView::drawRank(QPainter *painter, int rank){
    QRect r = fieldRect(1, rank);
    QRect rankRect = QRect(0, r.top(), r.left(), r.height()).adjusted(2, 0, -2, 0);
    QString rankText = QString::number(rank);
    painter->setPen(ColorIndex);
    painter->drawText(rankRect, Qt::AlignVCenter|Qt::AlignRight, rankText);
}



/**
 * @brief ChessView::drawColumn
 * @param painter
 * @param column
 *
 * Fonction pour dessiner les lettres de a à h
 */
void ChessView::drawColumn(QPainter *painter, int column){
    QRect r = fieldRect(column, 1);
    QRect columnRect = QRect(r.left(), r.bottom(), r.width(), height()-r.bottom()).adjusted(0, 2, 0, -2);
    painter->setPen(ColorIndex);
    painter->drawText(columnRect, Qt::AlignHCenter|Qt::AlignTop, QChar('a'+column-1));
}


/**
 * @brief ChessView::drawField
 * @param painter
 * @param column
 * @param rank
 *
 * Fonction pour afficher graphiquement le quadrillage
 */
void ChessView::drawField(QPainter *painter, int column, int rank){
    QRect rect = fieldRect(column, rank);
    QColor fillColor = (column+rank) % 2 ? Color1
                                         : Color2;
    painter->setPen(palette().color(QPalette::Dark));
    painter->setBrush(fillColor);
    painter->drawRect(rect);
}


/**
 * @brief ChessView::drawPiece
 * @param painter
 * @param column
 * @param rank
 *
 * Fonction pour dessiner les pièces sur les carrés
 */
void ChessView::drawPiece(QPainter *painter, int column, int rank){
    QRect rect = fieldRect(column, rank);
    char value = m_board->data(column, rank);
    if(value != ' ') {
        QIcon icon = piece(value);

        if(!icon.isNull()) {
            icon.paint(painter, rect, Qt::AlignCenter);
        }
    }
}


/**
 * @brief ChessView::drawPlayerRectangle
 * @param painter
 * @param x
 * @param y
 * @param value
 *
 * Fonction pour afficher les rois sur le côté du plateau
 */
void ChessView::drawPlayerRectangle(QPainter *painter, int x, int y, char value){
    int length = 75;
    QRect fRect = QRect(x,y,length,length);

    painter->setPen(QColor(0, 0, 255, 255));
    painter->drawRect(fRect);
    QIcon icon = piece(value);
    icon.paint(painter, fRect, Qt::AlignCenter);
}


/**
 * @brief ChessView::drawPlayerTurn
 * @param painter
 *
 * Fonction pour afficher quel joueur joue
 */
void ChessView::drawPlayerTurn(QPainter *painter){
    int start = m_board->start;
    int length = 75;

    painter->setPen(Qt::blue);
    painter->setFont(QFont("Arial", 16));

    // Tour des blancs: Affichage du texte et du rectangle coloré
    if (start == 0){
        QRect fRect = QRect(sizeHint().rwidth()-100-length/2, sizeHint().rheight()*3/4-length/2, length,length);
        painter->fillRect(fRect,QColor(0, 0, 255, 50));
        painter->drawText(sizeHint().rwidth()-100-length/2, sizeHint().rheight()/2,"C'est le tour");
        painter->drawText(sizeHint().rwidth()-100-length/2, sizeHint().rheight()/2+20,"des blancs");
    }

    // Tour des noirs: Affichage du texte et du rectangle coloré
    else{
        QRect fRect = QRect(sizeHint().rwidth()-100-length/2,  sizeHint().rheight()*1/4-length/2, length,length);
        painter->fillRect(fRect,QColor(0, 0, 255, 50));
        painter->drawText(sizeHint().rwidth()-100-length/2, sizeHint().rheight()/2,"C'est le tour");
        painter->drawText(sizeHint().rwidth()-100-length/2, sizeHint().rheight()/2+20,"des noirs");
    }

}


/**
 * @brief ChessView::drawHighlights
 * @param painter
 *
 * Fonction pour surligner l'affichage de la sélection des pièces
 */
void ChessView::drawHighlights(QPainter *painter){
    for(int idx=0; idx < highlightCount(); ++idx) {
        Highlight *hl = highlight(idx);
        if(hl->type() == FieldHighlight::Type) {
            FieldHighlight *fhl = static_cast<FieldHighlight*>(hl);
            QRect rect = fieldRect(fhl->column(), fhl->rank());
            painter->fillRect(rect, fhl->color());
        }
    }
}


/**
 * @brief ChessView::fieldAt
 * @param pt
 * @return
 *
 * Fonction qui regarde si la souris est sur le plateau de jeu
 */
QPoint ChessView::fieldAt(const QPoint &pt) const{
    if(!m_board) return QPoint();
    const QSize fs = fieldSize();
    int offset = fontMetrics().maxWidth()+4;

    if(pt.x() < offset)
        return QPoint();

    int c = (pt.x()-offset) / fs.width();
    int r = pt.y()/fs.height();

    if(c < 0 || c >= m_board->columns() || r<0 || r >= m_board->ranks())
        return QPoint();
    return QPoint(c+1, m_board->ranks() - r); // max rank - r
}


/**
 * @brief ChessView::keyPressEvent
 * @param event
 *
 * Fonction qui fait réagir l'application à l'appui du Shift clique du clavier
 */
void ChessView::keyPressEvent(QKeyEvent *event){
    if(event->key() == Qt::Key_Shift){
        ctrlIsPressed = true;
    }
}


/**
 * @brief ChessView::keyReleaseEvent
 * @param event
 *
 * Fonction qui fait réagir l'application au relâchement du Shift clique du clavier
 */
void ChessView::keyReleaseEvent(QKeyEvent *event){
    if(event->key() == Qt::Key_Shift){
        ctrlIsPressed = false;
    }
}


/**
 * @brief ChessView::mousePressEvent
 * @param event
 *
 * Fonction qui fait réagir l'application à l'appui d'un bouton de la souris
 */
void ChessView::mousePressEvent(QMouseEvent *event){
    if(ctrlIsPressed){
        ArrowBug = true;
        if(event->button() == Qt::RightButton){ // Clique droit de la souris + SHIFT clique du clavier
            QPoint pt = fieldAt(event->pos());
            if(pt.isNull()) return;
            emit StartArrow(pt);                // Emet un signal pour l'affichage des flèches
        }
    }
}


/**
 * @brief ChessView::mouseReleaseEvent
 * @param event
 *
 * Fonction qui fait réagir l'application au relâchement du clique de la souris
 */
void ChessView::mouseReleaseEvent(QMouseEvent *event){

    if( !ctrlIsPressed ){
        if(event->button() == Qt::LeftButton){
            QPoint pt = fieldAt(event->pos());
            if(pt.isNull()) return;
            emit clicked(pt);
        }

        if(event->button() == Qt::RightButton){
            QPoint pt = fieldAt(event->pos());
            if(pt.isNull()) return;
            emit RightClicked(pt);
        }
    }
    else{
        if (ArrowBug){
            if(event->button() == Qt::RightButton){
                QPoint pt = fieldAt(event->pos());
                if(pt.isNull()) return;
                emit EndArrow(pt);

                ArrowBug = false;
            }
        }
    }
}



/**
 * @brief ChessView::addHighlight
 * @param hl
 *
 * Affichage du surlignage du clique droit
 */
void ChessView::addHighlight(ChessView::Highlight *hl) {
    m_highlights.append(hl);
    update();
}



/**
 * @brief ChessView::removeHighlight
 * @param hl
 *
 * Supprime l'affichage du surlignage du clique droit
 */
void ChessView::removeHighlight(ChessView::Highlight *hl) {
    m_highlights.removeOne(hl);
    update();
}


/**
 * @brief ChessView::tentative_Emission
 *
 * Signal pour signifier qu'il y a checkmate
 */
void ChessView::tentative_Emission(){
    emit tranfert_signal();
}


/**
 * @brief ChessView::EmissionPromotionSignal
 * @param PieceDeplacee
 * @param index
 *
 * Signal pour signifier la promotion d'un pion
 */
void ChessView::EmissionPromotionSignal(char PieceDeplacee, int index){
    emit TranfertEmissionPromotionSignal(PieceDeplacee, index);
}


/**
 * @brief ChessView::DrawLineWithArrow
 * @param painter
 *
 * Fonction pour afficher les lignes d'une flèche
 */
void ChessView::DrawLineWithArrow(QPainter *painter) {
    painter->setPen(ColorLine);
    painter->setBrush(ColorLine);
    painter->setRenderHint(QPainter::Antialiasing, true);
    QPen pen(ColorLine, 5, Qt::PenStyle(StyleLine), Qt::RoundCap, Qt::RoundJoin);
    painter->setPen(pen);

    qreal arrowSize = 20;         // Taille de la tête

    for(int idx=0; idx < highlightArrowCount(); idx++){
        Highlight *hlx = highlightArrowX(idx);
        Highlight *hly = highlightArrowY(idx);

        if(hlx->type() == FieldHighlight::Type and hly->type() == FieldHighlight::Type) {
            FieldHighlight *fhlx = static_cast<FieldHighlight*>(hlx);
            FieldHighlight *fhly = static_cast<FieldHighlight*>(hly);

            QLineF line(fhly->column()*m_fieldSize.height()-15+m_fieldSize.height()/2,
                        (8-fhly->rank())*m_fieldSize.height()+m_fieldSize.height()/2,
                        fhlx->column()*m_fieldSize.height()-15+m_fieldSize.height()/2,
                        (8-fhlx->rank())*m_fieldSize.height()+m_fieldSize.height()/2);

            double angle = std::atan2(-line.dy(), line.dx());
            QPointF arrowP1 = line.p1() + QPointF(sin(angle + M_PI / 3) * arrowSize, cos(angle + M_PI / 3) * arrowSize);
            QPointF arrowP2 = line.p1() + QPointF(sin(angle + 2*M_PI / 3) * arrowSize, cos(angle + 2*M_PI / 3) * arrowSize);

            QPolygonF arrowHead;
            arrowHead.clear();
            arrowHead << line.p1() << arrowP1 << arrowP2;
            painter->drawLine(line);
            painter->drawPolygon(arrowHead);
        }
    }
}


/**
 * @brief ChessView::addHighlightArrow
 * @param hlx
 * @param hly
 *
 * Fonction pour ajouter une flèche sur le plateau
 */
void ChessView::addHighlightArrow(ChessView::Highlight *hlx, ChessView::Highlight *hly){
    m_highlightsArrowX.append(hlx);
    m_highlightsArrowY.append(hly);
    update();
}


/**
 * @brief ChessView::removeHighlightArrow
 * @param hlx
 * @param hly
 *
 * Fonction pour supprimer une flèche sur le plateau
 */
void ChessView::removeHighlightArrow(ChessView::Highlight *hlx, ChessView::Highlight *hly){
    m_highlightsArrowX.removeOne(hlx);
    m_highlightsArrowY.removeOne(hly);
    update();
}

/**
 * @brief ChessView::DrawIaCalculation
 * @param painter
 *
 * Fonction pour afficher le fait que l'IA calcul
 */
void ChessView::DrawIaCalculation(QPainter *painter){
    painter->drawEllipse(QPointF(10,10), 5, 5);
}
