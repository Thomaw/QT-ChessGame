#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "ui_mainwindow.h"
#include "chessview.h"
#include "QCheckBox"
#include "QLineEdit"

#include "QListWidget"

#include "QColorDialog"

#include "widget_aproposde.h"


class ChessAlgorithm;
class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);


public slots:
    void viewClicked(const QPoint &field);
    void RightClickColor(const QPoint &field);
    void StartArrowFunction(const QPoint &field);
    void EndArrowFunction(const QPoint &field);

    void CheckMateFinish();
    void PatFinish();

    void viewEchec();
    void viewNonEchec();
    void PromotionFonction(char, int);

    void LumiereCalcul();




protected:
    Ui::MainWindow *ui;


private:
    ChessView *m_view;
    ChessAlgorithm *m_algorithm;
    QPoint m_clickPoint;
    ChessView::FieldHighlight *m_selectedField;
    ChessView::FieldHighlight *EchecColor;

    QWidget *Promotion = new QWidget();
    QWidget *GameOver = new QWidget();
    QWidget *Pat = new QWidget();
    QWidget *OpenFEN = new QWidget();
    QWidget *ChangePiece = new QWidget();
    QWidget *ChangeBoard = new QWidget();
    QWidget *ChangeLine = new QWidget();
    QWidget *ChangeColors = new QWidget();
    QWidget *ChangeVolume = new QWidget();

    void SetupChangePiece();
    void SetupQuitChangePiece();

    void SetupChangeBoard();
    void SetupQuitChangeBoard();

    void SetupChangeLine();
    void SetupQuitChangeLine();

    void SetupChangeColors();
    void SetupQuitChangeColors();

    void HandleAProposDe(){Widget_AproposDe *CallAProposDe = new Widget_AproposDe(); };
    void ChangeQuitVolumeFunction();
    void ChangeVolumeFunction();

    void HandleButtonModeSombre();
    void HandleButtonModeJour();



    void SetupGameOver();
    void SetupPat();

    void HandleButtonQuit();
    void HandleButtonReplay();
    void HandleButtonMultipleGame();

    void HandleButtonOpen();
    void SetupOpen();
    void SetupQuitOpen();

    void HandleButtonChangePieces();
    void HandleButtonChangeBoard();
    void HandleButtonParametrageCouleurs();
    void HandleButtonChangeLignes();
    void HandleButtonVolumeControl();



    void PromotionFonctionQuit();
    void PromotionFonctionCavalier();
    void PromotionFonctionFou();
    void PromotionFonctionTour();
    void PromotionFonctionDame();

    void ImplementationArrow();


//    void AllPieces(ChessView *m_view, int p);
    void AllPieces(int p);
    void AllBoard(int p);


    QWidget *IAwidget = new QWidget();
    QSlider *SliderIALevel = new QSlider(Qt::Horizontal);
//    QWidget *AProposDe = new QWidget();


    QSlider *SlidersVolumeMovement = new QSlider(Qt::Horizontal);
    QSlider *SlidersVolumeVictory = new QSlider(Qt::Horizontal);
    QSlider *SlidersVolumePat = new QSlider(Qt::Horizontal);

    QLineEdit *TextFen = new QLineEdit;
    QListWidget *listWidget = new QListWidget(ChangePiece);


//    QVector<ChessView::FieldHighlight*> mutiple_selectedField;
    QCheckBox *checkBox = new QCheckBox(tr("Afficher toutes les\ncases atteignables"));

    QVector<ChessView::FieldHighlight*> mutiple_selectedField;
    QVector<ChessView::FieldHighlight*> mutiple_circle;

    QList<int> PositionCircle;

    QColorDialog *Qdialog1 = new QColorDialog();

    QColor Color1 = QColor(0, 100, 0, 160);
    QColor Color2 = QColor(0, 0, 255, 160);
    QColor Color3 = QColor(255, 0, 0, 160);
    QColor Color4 = QColor(200, 200, 0, 160);
    QColor Color5 = QColor(0, 128, 0, 160);

    void SetButton(QString, QPushButton*);

    int indexPromotion;
    char PieceDeplaceePromotion;
    int indexAllPieces = 0;


    QList<int> StartIndexArrow;
    QList<int> EndIndexArrow;

    QVector<ChessView::FieldHighlight*> testX;
    QVector<ChessView::FieldHighlight*> testY;

    QAction *actionIA = new QAction("Jouer contre l'IA",this);
    void HandleEngine();

    void ReturnLevelValue();

};

#endif // MAINWINDOW_H
