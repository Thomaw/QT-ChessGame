#ifndef CHESSVIEW_H
#define CHESSVIEW_H


#include <QWidget>
#include <QPointer>
#include <QMap>
#include <QIcon>
#include "QTimer"



class ChessBoard;
class ChessView : public QWidget{
    Q_OBJECT
    Q_PROPERTY(QSize fieldSize
               READ fieldSize WRITE setFieldSize
               NOTIFY fieldSizeChanged)
public:
    bool IaCalculation = false;
    class Highlight {
    public:
        Highlight() {}
        virtual ~Highlight() {}
        virtual int type() const { return 0; }
    };

    class FieldHighlight : public Highlight {

    public:
        enum { Type = 1 };
        FieldHighlight(int column, int rank, QColor color): m_field(column, rank), m_color(color) {}
        inline int column() const { return m_field.x(); }
        inline int rank() const { return m_field.y(); }
        inline QColor color() const { return m_color; }
        int type() const { return Type; }

    private:
        QPoint m_field;
        QColor m_color;
    };

    explicit ChessView(QWidget *parent = 0);
    void setBoard(ChessBoard*);
    ChessBoard* board() const;
    QSize fieldSize() const;
    QSize sizeHint() const;
    QRect fieldRect(int column, int rank) const;
    void setPiece(char type, const QIcon &icon);
    QIcon piece(char type) const;
    QPoint fieldAt(const QPoint &pt) const;

    void addHighlight(Highlight *hl);
    void removeHighlight(Highlight *hl);
    void addHighlightArrow(Highlight *hlx, Highlight *hly);
    void removeHighlightArrow(Highlight *hlx, Highlight *hly);

    inline Highlight *highlight(int index) const { return m_highlights.at(index); }
    inline Highlight *highlightArrowX(int index) const { return m_highlightsArrowX.at(index); }
    inline Highlight *highlightArrowY(int index) const { return m_highlightsArrowY.at(index); }

    inline int highlightCount() const { return m_highlights.size(); }
    inline int highlightArrowCount() const { return m_highlightsArrowY.size(); }

    QColor Color1 = palette().color(QPalette::Light); //Qt::red;
    QColor Color2 = palette().color(QPalette::Mid); // Qt::blue;
    QColor ColorIndex = QColor(0,0,0,255);
    QColor ColorLine = Qt::green;
    int StyleLine = 1;

    void DrawIaCalculation(QPainter *);




public slots:
    void setFieldSize(QSize fieldSize);

    void tentative_Emission();
    void EmissionPromotionSignal(char, int);

    void EmissionMiseEnEchec(){emit TranfertMiseEnEchec();};
    void EmissionPlusMiseEnEchec(){emit TranfertPlusMiseEnEchec();};
    void EmissionPatFinGame(){emit TranfertPatFinGame();};



signals:
    void fieldSizeChanged(QSize fieldSize);

    void clicked(const QPoint &);
    void RightClicked(const QPoint &);
    void StartArrow(const QPoint &);
    void EndArrow(const QPoint &);


    void tranfert_signal();

    void TranfertMiseEnEchec();
    void TranfertPlusMiseEnEchec();
    void TranfertPatFinGame();
    void TranfertEmissionPromotionSignal(char, int);


protected:
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);

    virtual void drawColumn(QPainter *painter, int column);
    virtual void drawRank(QPainter *painter, int rank);
    virtual void drawField(QPainter *painter, int column, int rank);
    virtual void drawPiece(QPainter *painter, int column, int rank);
    virtual void drawHighlights(QPainter *painter);


    virtual void drawPlayerRectangle(QPainter *painter, int column, int rank, char value); // test
    virtual void drawPlayerTurn(QPainter *painter);

    virtual void DrawLineWithArrow(QPainter *painter);

private:

    QPointer<ChessBoard> m_board;
    QSize m_fieldSize;

    QMap<char,QIcon> m_pieces;
    QList<Highlight*> m_highlights;

    QList<Highlight*> m_highlightsArrowX;
    QList<Highlight*> m_highlightsArrowY;

    bool ctrlIsPressed = false;
    bool ArrowBug = false;

//    QSound bells("mysounds/bells.wav");
};

#endif // CHESSVIEW_H
