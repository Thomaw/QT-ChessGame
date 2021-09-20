#include "mainwindow.h"
#include "chessboard.h"
#include "chessview.h"
#include "chessalgorithm.h"

#include <QLayout>
#include "QLineEdit"
#include "QLabel"
#include "QPushButton"
#include "QString"
#include <QPixmap>
#include "QListWidget"
#include "QPlainTextEdit"
#include <QSlider>
#include "QLCDNumber"

#include "iostream"
#include "QColorDialog"
#include "QPalette"


/**
 * @brief MainWindow::MainWindow
 * @param parent
 *
 * Constructeur de l'interface graphique
 */
MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow), m_selectedField(0){
    ui->setupUi(this);

    setWindowIcon(QIcon(":/Icones/Chess.ico")); // Icône de l'application
    this->setWindowTitle("Jeu d'échec");        // Nom de l'application


    //
    // Définition de la bar des menus
    //
    QMenu *menuFichier = menuBar()->addMenu("&Fichier");
    QMenu *menuOption = menuBar()->addMenu("&Option");
    QMenu *menuEngine = menuBar()->addMenu("&IA");
    QMenu *menuAffichage = menuBar()->addMenu("&Affichage");
    QMenu *menuAProposDe = menuBar()->addMenu("A propos de");


    QAction *actionNouveau = new QAction("Nouvelle Partie",this);
    actionNouveau->setShortcuts(QKeySequence::New);

    QAction *actionNouvelleFenetre = new QAction("Nouvelle fenêtre",this);

    QAction *actionOuvrir = new QAction("Ouvrir",this);
    actionOuvrir->setShortcuts(QKeySequence::Open);

    QAction *actionQuitter = new QAction("Quitter",this);
    actionQuitter->setShortcuts(QKeySequence::Close);

    QAction *actionChangerPieces = new QAction("Pieces",this);
    actionChangerPieces->setShortcuts(QKeySequence::Replace);

    QAction *actionChangerBoard = new QAction("Board",this);
    actionChangerBoard->setShortcuts(QKeySequence::Bold);

    QAction *actionChangerLignes = new QAction("Style des lignes",this);
    actionChangerLignes->setShortcuts(QKeySequence::Underline);

    QAction *ParametrageCouleurs = new QAction("Paramètres",this);
    ParametrageCouleurs->setShortcuts(QKeySequence::Italic);

    QAction *VolumeControl = new QAction("Contrôle des volumes",this);

    QAction *ModeSombre = new QAction("Mode Sombre",this);
    QAction *ModeJour = new QAction("Mode Jour",this);

    QAction *actionAProposDe = new QAction("A propos de",this);
    actionAProposDe->setShortcuts(QKeySequence::HelpContents);


    // Ajout de l'action des menus
    menuFichier -> addAction(actionNouveau);
    menuFichier -> addAction(actionNouvelleFenetre);
    menuFichier -> addAction(actionOuvrir);
    menuFichier -> addAction(actionQuitter);

    menuOption -> addAction(actionChangerPieces);
    menuOption -> addAction(actionChangerBoard);
    menuOption -> addAction(actionChangerLignes);
    menuOption -> addAction(ParametrageCouleurs);
    menuOption -> addAction(VolumeControl);

    menuEngine -> addAction(actionIA);

    menuAffichage -> addAction(ModeJour);
    menuAffichage -> addAction(ModeSombre);

    menuAProposDe -> addAction(actionAProposDe);


    // Connection à l'appui des boutons
    connect(actionNouveau, &QAction::triggered, this, &MainWindow::HandleButtonReplay);
    connect(actionNouvelleFenetre, &QAction::triggered, this, &MainWindow::HandleButtonMultipleGame);
    connect(actionQuitter, &QAction::triggered, this, &MainWindow::HandleButtonQuit);
    connect(actionOuvrir, &QAction::triggered, this, &MainWindow::HandleButtonOpen);

    connect(actionChangerPieces, &QAction::triggered, this, &MainWindow::HandleButtonChangePieces);
    connect(actionChangerBoard, &QAction::triggered, this, &MainWindow::HandleButtonChangeBoard);
    connect(actionChangerLignes, &QAction::triggered, this, &MainWindow::HandleButtonChangeLignes);
    connect(ParametrageCouleurs, &QAction::triggered, this, &MainWindow::HandleButtonParametrageCouleurs);
    connect(VolumeControl, &QAction::triggered, this, &MainWindow::HandleButtonVolumeControl);

    connect(actionIA, &QAction::triggered, this, &MainWindow::HandleEngine);

    connect(ModeJour, &QAction::triggered, this, &MainWindow::HandleButtonModeJour);
    connect(ModeSombre, &QAction::triggered, this, &MainWindow::HandleButtonModeSombre);

    connect(actionAProposDe, &QAction::triggered, this, &MainWindow::HandleAProposDe);
    //
    // Fin de la cration du menu
    //

    // Création de la vue du jeu d'échec
    m_view = new ChessView;


    // Positionnement des pièces
    m_view->setPiece('P', QIcon(":/pieces/Chess_plt45.svg")); // Pion
    m_view->setPiece('K', QIcon(":/pieces/Chess_klt45.svg")); // Roi
    m_view->setPiece('Q', QIcon(":/pieces/Chess_qlt45.svg")); // Reine
    m_view->setPiece('R', QIcon(":/pieces/Chess_rlt45.svg")); // Tour
    m_view->setPiece('N', QIcon(":/pieces/Chess_nlt45.svg")); // Cavalier
    m_view->setPiece('B', QIcon(":/pieces/Chess_blt45.svg")); // Fou

    m_view->setPiece('p', QIcon(":/pieces/Chess_pdt45.svg")); // Pion
    m_view->setPiece('k', QIcon(":/pieces/Chess_kdt45.svg")); // Roi
    m_view->setPiece('q', QIcon(":/pieces/Chess_qdt45.svg")); // Reine
    m_view->setPiece('r', QIcon(":/pieces/Chess_rdt45.svg")); // Tour
    m_view->setPiece('n', QIcon(":/pieces/Chess_ndt45.svg")); // Cavalier
    m_view->setPiece('b', QIcon(":/pieces/Chess_bdt45.svg")); // Fou


    m_algorithm = new ChessAlgorithm(this); // Lancement de l'algorithme
    m_algorithm->newGame(m_algorithm->FEN); // nouvelle partie

    m_view->setBoard(m_algorithm->board()); // Initialisation du board
    m_algorithm->board()->eng->setBoard(Chess::Board::fromFEN(m_algorithm->FEN.toStdString()));

    setCentralWidget(m_view);

    // Affichage de la fenêtre
    m_view->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    m_view->setFieldSize(QSize(50,50));
    layout()->setSizeConstraint(QLayout::SetFixedSize);

    checkBox->setGeometry(500, 400, 150, 50);
    layout()->addWidget(checkBox);

    // Connection de la vue du jeu
    connect(m_view, &ChessView::clicked, this, &MainWindow::viewClicked);
    connect(m_view, &ChessView::tranfert_signal, this, &MainWindow::CheckMateFinish);
    connect(m_view, &ChessView::TranfertMiseEnEchec, this, &MainWindow::viewEchec);
    connect(m_view, &ChessView::TranfertPlusMiseEnEchec, this, &MainWindow::viewNonEchec);
    connect(m_view, &ChessView::TranfertPatFinGame, this, &MainWindow::PatFinish);
    connect(m_view, &ChessView::RightClicked, this, &MainWindow::RightClickColor);
    connect(m_view, &ChessView::TranfertEmissionPromotionSignal, this, &MainWindow::PromotionFonction);

    connect(m_view, &ChessView::StartArrow, this, &MainWindow::StartArrowFunction);
    connect(m_view, &ChessView::EndArrow, this, &MainWindow::EndArrowFunction);

    connect(m_view->board(), &ChessBoard::CalculenCours, this, &MainWindow::LumiereCalcul);
}


/**
 * @brief MainWindow::viewClicked
 * @param field
 *
 * Fonction qui modifie le plateau de jeu lors d'un clique
 */
void MainWindow::viewClicked(const QPoint &field){
    if (!m_view->board()->start){m_algorithm->AncienBoardEnPassantBlack = m_view->board()->Return_m_boardData();}
    else{m_algorithm->AncienBoardEnPassantWhite = m_view->board()->Return_m_boardData();}

    //CheckBox pour afficher toutes les cases atteignables par un joueur
    bool Checkbox_check;
    if (checkBox->isChecked()){Checkbox_check = true;}
    else{Checkbox_check = false;}


    if(m_clickPoint.isNull()) {
        if(m_view->board()->data(field.x(), field.y()) != ' ') {
            m_clickPoint = field;

            if (Checkbox_check==true){

                // Affichage de toutes les cases atteignables
                QVector<int> AllPossibleMove = m_algorithm->all_possible_movement(m_view->board()->start, m_view->board()->Return_m_boardData());
                QVector<int> IndexationMovement = m_algorithm->Indexation_possible_movement(AllPossibleMove);
                mutiple_selectedField.resize(IndexationMovement.length());

                int r1,c1;

                for (int i=0;i<IndexationMovement.length();i++){
                    r1 = m_algorithm->GetRankIndex(IndexationMovement[i])+1;
                    c1 = m_algorithm->GetColumnIndex(IndexationMovement[i])+1;

                    // Appel fonction graphique
                    mutiple_selectedField[i] = new ChessView::FieldHighlight(c1, r1, Color1);
                    m_view->addHighlight(mutiple_selectedField[i]);
                }
            }

            m_selectedField = new ChessView::FieldHighlight(field.x(), field.y(), Color3);
            m_view->addHighlight(m_selectedField);
        }
    }
    else {
        if(field != m_clickPoint) {
            m_view->board()->movePiece(m_clickPoint.x(), m_clickPoint.y(), field.x(), field.y());
        }

        m_clickPoint = QPoint();
        m_view->removeHighlight(m_selectedField);

        // Affichage de toutes les cases atteignables
        for (int i=0;i<mutiple_selectedField.length();i++){
            m_view->removeHighlight(mutiple_selectedField[i]);
        }

        delete m_selectedField;
        m_selectedField = 0;
    }
}

/**
 * @brief MainWindow::RightClickColor
 * @param field
 *
 * Modification de la couleur de l'échiquier lors du clique droit
 */
void MainWindow::RightClickColor(const QPoint &field){
    int px = field.x();
    int py = field.y();
    int Cindex = 8 * (py-1)+(px-1);

    int tampon = 0;
    if (PositionCircle.length() !=0){
        for (int i=0; i<PositionCircle.length();i++){

            if (PositionCircle[i]==Cindex){
                m_view->removeHighlight(mutiple_circle[i]);
                PositionCircle.removeAt(i);
                mutiple_circle.removeAt(i);
                tampon = 1;
                break;
            }
        }

        if (tampon == 0){
            PositionCircle.append(Cindex);
            mutiple_circle.resize(PositionCircle.length());
            int r1 = m_algorithm->GetRankIndex(PositionCircle.last())+1;
            int c1 = m_algorithm->GetColumnIndex(PositionCircle.last())+1;
            mutiple_circle.last() = new ChessView::FieldHighlight(c1, r1, Color2);
            m_view->addHighlight(mutiple_circle.last());
        }
    }
    else{
        PositionCircle.append(Cindex);
        mutiple_circle.resize(PositionCircle.length());
        int r1 = m_algorithm->GetRankIndex(PositionCircle.last())+1;
        int c1 = m_algorithm->GetColumnIndex(PositionCircle.last())+1;
        mutiple_circle.last() = new ChessView::FieldHighlight(c1, r1, Color2);
        m_view->addHighlight(mutiple_circle.last());
    }
}


/**
 * @brief MainWindow::CheckMateFinish
 *
 * Fonction qui appelle le widget de checkmate
 */
void MainWindow::CheckMateFinish(){
    int KIndex = m_algorithm->KingPosition(m_view->board()->Return_m_boardData(), m_view->board()->start);
    int rk = m_algorithm->GetRankIndex(KIndex)+1;
    int ck = m_algorithm->GetColumnIndex(KIndex)+1;

    ChessView::FieldHighlight *CheckMateColor = new ChessView::FieldHighlight(ck, rk, Color4);
    m_view->addHighlight(CheckMateColor);

    SetupGameOver();
}


/**
 * @brief MainWindow::PatFinish
 *
 * Fonction qui appelle le widget d'égalité
 */
void MainWindow::PatFinish(){SetupPat();}


/**
 * @brief MainWindow::viewEchec
 *
 * Fonction pour ajouter un surlignage de l'échiquier lors de l'échec
 */
void MainWindow::viewEchec(){
    int KIndex = m_algorithm->KingPosition(m_view->board()->Return_m_boardData(), m_view->board()->start);
    int rk = m_algorithm->GetRankIndex(KIndex)+1;
    int ck = m_algorithm->GetColumnIndex(KIndex)+1;

    EchecColor = new ChessView::FieldHighlight(ck, rk, Color5);
    m_view->addHighlight(EchecColor);
}


/**
 * @brief MainWindow::viewNonEchec
 *
 * Fonction pour supprimer un affichage de l'échec
 */
void MainWindow::viewNonEchec(){
    m_view->removeHighlight(EchecColor);

    delete EchecColor;
    EchecColor = 0;
}


/**
 * @brief MainWindow::SetupGameOver
 *
 * Fonction pour initialiser le widget du GameOver
 */
void MainWindow::SetupGameOver(){
    GameOver->setWindowFlags(Qt::WindowTitleHint | Qt::WindowMinimizeButtonHint);
    GameOver->resize(240,160);
    GameOver->setWindowTitle("Game Over");

    QString txt = "";
    QLabel *label2  = new QLabel();

    QPixmap picNoir(":/pieces/Chess_kdt45.svg");
    QPixmap picBlanc(":/pieces/Chess_klt45.svg");

    if (m_view->board()->start == 0){
        txt = "Victoire des noirs";
        label2->setPixmap(picNoir);
        label2->setPixmap(picNoir.scaled(60,60,Qt::KeepAspectRatio));
    }
    else {
        txt = "Victoire des blancs";
        label2->setPixmap(picBlanc);
        label2->setPixmap(picBlanc.scaled(60,60,Qt::KeepAspectRatio));
    }

    QLabel *label  = new QLabel(txt);
    QFont f( "Arial", 19, QFont::Bold);
    label->setFont(f);

    QHBoxLayout *layout = new QHBoxLayout();
    QVBoxLayout *Vlayout = new QVBoxLayout();

    QPushButton *buttonQuit = new QPushButton(tr("Quitter"),GameOver);
    QPushButton *buttonReplay = new QPushButton(tr("Rejouer"),GameOver);

    layout ->addWidget(buttonQuit);
    layout ->addWidget(buttonReplay);

    Vlayout ->addWidget(label);
    Vlayout->setAlignment(label,Qt::AlignCenter);
    Vlayout ->addWidget(label2);
    Vlayout->setAlignment(label2,Qt::AlignCenter);
    Vlayout ->addLayout(layout,Qt::AlignCenter);

    GameOver->setLayout(Vlayout);
    GameOver->show();

    connect(buttonQuit, &QPushButton::released, this, &MainWindow::HandleButtonQuit);
    connect(buttonReplay, &QPushButton::released, this, &MainWindow::HandleButtonReplay);
}


/**
 * @brief MainWindow::SetupPat
 *
 * Fonction pour initialiser le widget de l'égalité
 */
void MainWindow::SetupPat(){
    Pat->resize(240,160);
    Pat->setWindowTitle("Pat");
    Pat->setWindowFlags(Qt::WindowTitleHint | Qt::WindowMinimizeButtonHint);

    QLabel *label  = new QLabel("Pat");
    QFont f( "Arial", 20, QFont::Bold);
    label->setFont(f);

    QHBoxLayout *layout = new QHBoxLayout();

    QPushButton *buttonQuit = new QPushButton(tr("Quitter"),Pat);
    layout ->addWidget(buttonQuit);

    QPushButton *buttonReplay = new QPushButton(tr("Rejouer"),Pat);
    layout ->addWidget(buttonReplay);

    QVBoxLayout *Vlayout = new QVBoxLayout();
    Vlayout ->addWidget(label);
    Vlayout->setAlignment(label,Qt::AlignCenter);
    Vlayout ->addLayout(layout);

    Pat->setLayout(Vlayout);
    Pat->show();

    connect(buttonQuit, &QPushButton::released, this, &MainWindow::HandleButtonQuit);
    connect(buttonReplay, &QPushButton::released, this, &MainWindow::HandleButtonReplay);
}


/**
 * @brief MainWindow::HandleButtonQuit
 *
 * Fonction pour fermer le widget de fin de Partie
 */
void MainWindow::HandleButtonQuit(){
    this->close();
    GameOver->close();
    Pat->close();
}


/**
 * @brief MainWindow::HandleButtonReplay
 *
 * Fonction pour recommencer une partie
 */
void MainWindow::HandleButtonReplay(){
    HandleButtonQuit();

    delete ui;

    MainWindow *ui1 = new MainWindow;
    ui1->show();
}


/**
 * @brief MainWindow::HandleButtonMultipleGame
 *
 * Fonction pour jouer de multiples parties
 */
void MainWindow::HandleButtonMultipleGame(){
    MainWindow *ui = new MainWindow;
    ui->show();
}


/**
 * @brief MainWindow::HandleButtonOpen
 *
 * Fonction pour ouvrir le widget de changement de board
 */
void MainWindow::HandleButtonOpen(){
    OpenFEN->setWindowFlags(Qt::WindowTitleHint | Qt::WindowMinimizeButtonHint);
    OpenFEN->setWindowTitle("Open FEN");

    QHBoxLayout *layout = new QHBoxLayout();
    QVBoxLayout *layout2 = new QVBoxLayout();
    QHBoxLayout *layout3 = new QHBoxLayout();
    QLabel *setFEN = new QLabel(tr("Set FEN: "));

    TextFen->setPlaceholderText("Set FEN Code");

    layout ->addWidget(setFEN);
    layout ->addWidget(TextFen);
    layout2->addLayout(layout);

    QPushButton *buttonOK = new QPushButton(tr("Ok"),OpenFEN);
    QPushButton *buttonQuitter = new QPushButton(tr("Fermer"),OpenFEN);

    layout3 ->addWidget(buttonQuitter);
    layout3 ->addWidget(buttonOK);
    layout2 ->addLayout(layout3);

    OpenFEN->setFixedSize(QSize(300, 100));
    OpenFEN->setLayout(layout2);
    OpenFEN->show();
    TextFen->setFocus();
    TextFen->grabKeyboard();

    connect(buttonOK, &QPushButton::released, this, &MainWindow::SetupOpen);
    connect(buttonQuitter, &QPushButton::released, this, &MainWindow::SetupQuitOpen);
}


/**
 * @brief MainWindow::SetupQuitOpen
 *
 * Fonction pour fermer le widget de changement de board
 */
void MainWindow::SetupQuitOpen(){
    delete OpenFEN->layout();
    OpenFEN->close();

    m_view->grabKeyboard();
}


/**
 * @brief MainWindow::SetupOpen
 *
 * Fonction pour changer la configuration du board
 */
void MainWindow::SetupOpen(){
   QString NewFEN = TextFen->text();

   if (NewFEN.isEmpty()){
       TextFen->setPlaceholderText("Aucun FEN ! Veuillez réessayer");
   }
   else{
       SetupQuitOpen();

       TextFen->setText("");
       m_algorithm->FEN = NewFEN;

       m_algorithm->newGame(m_algorithm->FEN);
       m_view->setBoard(m_algorithm->board());
       setCentralWidget(m_view);

       m_algorithm->board()->eng->setBoard(Chess::Board::fromFEN(m_algorithm->FEN.toStdString()));
       update();
   }

   m_view->grabKeyboard();
}


/**
 * @brief MainWindow::HandleButtonChangePieces
 *
 * Fonction pour changer les pièces de l'échiquier
 */
void MainWindow::HandleButtonChangePieces(){


    new QListWidgetItem(tr("Défault"), listWidget);
    new QListWidgetItem(tr("Medieval"), listWidget);
    new QListWidgetItem(tr("Retro"), listWidget);
    new QListWidgetItem(tr("Extravagant"), listWidget);

    QVBoxLayout *layoutz = new QVBoxLayout();
    QHBoxLayout *buttonLayout = new QHBoxLayout();

    QPushButton *buttonOk = new QPushButton(tr("Ok"),ChangePiece);
    QPushButton *buttonQuitter = new QPushButton(tr("Fermer"),ChangePiece);

    layoutz->addWidget(listWidget);
    buttonLayout->addWidget(buttonQuitter);
    buttonLayout->addWidget(buttonOk);
    layoutz->addLayout(buttonLayout);

    ChangePiece->setWindowFlags(Qt::WindowTitleHint | Qt::WindowMinimizeButtonHint);
    ChangePiece->setFixedSize(QSize(300, 250));
    ChangePiece->setWindowTitle("Changer les pièces");
    ChangePiece->setLayout(layoutz);
    ChangePiece->show();


    connect(buttonOk, &QPushButton::released, this, &MainWindow::SetupChangePiece);
    connect(buttonQuitter, &QPushButton::released, this, &MainWindow::SetupQuitChangePiece);

    listWidget->item(0)->setSelected(true);
}


/**
 * @brief MainWindow::SetupQuitChangePiece
 *
 * Fonction pour quitter le widget de changement de pièce
 */
void MainWindow::SetupQuitChangePiece(){
    delete ChangePiece->layout();
    listWidget->clear();
    ChangePiece->close();
}


/**
 * @brief MainWindow::SetupChangePiece
 *
 * FOnction pour mettre à jour le changement des pièces
 */
void MainWindow::SetupChangePiece(){
    int p = listWidget->currentRow();

    AllPieces(p);
    update();

    SetupQuitChangePiece();
}

/**
 * @brief MainWindow::AllPieces
 * @param p
 *
 * Fonction de changement de design des pièces
 */
void MainWindow::AllPieces(int p){

    indexAllPieces = p;
    switch(p){

    case 0:{
        m_view->setPiece('P', QIcon(":/pieces/Chess_plt45.svg"));
        m_view->setPiece('K', QIcon(":/pieces/Chess_klt45.svg"));
        m_view->setPiece('Q', QIcon(":/pieces/Chess_qlt45.svg"));
        m_view->setPiece('R', QIcon(":/pieces/Chess_rlt45.svg"));
        m_view->setPiece('N', QIcon(":/pieces/Chess_nlt45.svg"));
        m_view->setPiece('B', QIcon(":/pieces/Chess_blt45.svg"));

        m_view->setPiece('p', QIcon(":/pieces/Chess_pdt45.svg"));
        m_view->setPiece('k', QIcon(":/pieces/Chess_kdt45.svg"));
        m_view->setPiece('q', QIcon(":/pieces/Chess_qdt45.svg"));
        m_view->setPiece('r', QIcon(":/pieces/Chess_rdt45.svg"));
        m_view->setPiece('n', QIcon(":/pieces/Chess_ndt45.svg"));
        m_view->setPiece('b', QIcon(":/pieces/Chess_bdt45.svg"));

        break;
    }

    case 1:{
        m_view->setPiece('P', QIcon(":/pieces_medievales/Chess_plt_medieval.png")); // Pion
        m_view->setPiece('K', QIcon(":/pieces_medievales/Chess_klt_medieval.png")); // Roi
        m_view->setPiece('Q', QIcon(":/pieces_medievales/Chess_qlt_medieval.png")); // Reine
        m_view->setPiece('R', QIcon(":/pieces_medievales/Chess_rlt_medieval.png")); // Tour
        m_view->setPiece('N', QIcon(":/pieces_medievales/Chess_nlt_medieval.png")); // Cavalier
        m_view->setPiece('B', QIcon(":/pieces_medievales/Chess_blt_medieval.png")); // Fou

        m_view->setPiece('p', QIcon(":/pieces_medievales/Chess_pdt_medieval.png")); // Pion
        m_view->setPiece('k', QIcon(":/pieces_medievales/Chess_kdt_medieval.png")); // Roi
        m_view->setPiece('q', QIcon(":/pieces_medievales/Chess_qdt_medieval.png")); // Reine
        m_view->setPiece('r', QIcon(":/pieces_medievales/Chess_rdt_medieval.png")); // Tour
        m_view->setPiece('n', QIcon(":/pieces_medievales/Chess_ndt_medieval.png")); // Cavalier
        m_view->setPiece('b', QIcon(":/pieces_medievales/Chess_bdt_medieval.png")); // Fou

        break;
    }

    case 2:{
        m_view->setPiece('P', QIcon(":/pieces_retros/Chess_plt_retro.png")); // Pion
        m_view->setPiece('K', QIcon(":/pieces_retros/Chess_klt_retro.png")); // Roi
        m_view->setPiece('Q', QIcon(":/pieces_retros/Chess_qlt_retro.png")); // Reine
        m_view->setPiece('R', QIcon(":/pieces_retros/Chess_rlt_retro.png")); // Tour
        m_view->setPiece('N', QIcon(":/pieces_retros/Chess_nlt_retro.png")); // Cavalier
        m_view->setPiece('B', QIcon(":/pieces_retros/Chess_blt_retro.png")); // Fou

        m_view->setPiece('p', QIcon(":/pieces_retros/Chess_pdt_retro.png")); // Pion
        m_view->setPiece('k', QIcon(":/pieces_retros/Chess_kdt_retro.png")); // Roi
        m_view->setPiece('q', QIcon(":/pieces_retros/Chess_qdt_retro.png")); // Reine
        m_view->setPiece('r', QIcon(":/pieces_retros/Chess_rdt_retro.png")); // Tour
        m_view->setPiece('n', QIcon(":/pieces_retros/Chess_ndt_retro.png")); // Cavalier
        m_view->setPiece('b', QIcon(":/pieces_retros/Chess_bdt_retro.png")); // Fou

        break;
    }

    case 3:{
        m_view->setPiece('P', QIcon(":/pieces_extravagantes/Chess_plt_extravagant.png")); // Pion
        m_view->setPiece('K', QIcon(":/pieces_extravagantes/Chess_klt_extravagant.png")); // Roi
        m_view->setPiece('Q', QIcon(":/pieces_extravagantes/Chess_qlt_extravagant.png")); // Reine
        m_view->setPiece('R', QIcon(":/pieces_extravagantes/Chess_rlt_extravagant.png")); // Tour
        m_view->setPiece('N', QIcon(":/pieces_extravagantes/Chess_nlt_extravagant.png")); // Cavalier
        m_view->setPiece('B', QIcon(":/pieces_extravagantes/Chess_blt_extravagant.png")); // Fou

        m_view->setPiece('p', QIcon(":/pieces_extravagantes/Chess_pdt_extravagant.png")); // Pion
        m_view->setPiece('k', QIcon(":/pieces_extravagantes/Chess_kdt_extravagant.png")); // Roi
        m_view->setPiece('q', QIcon(":/pieces_extravagantes/Chess_qdt_extravagant.png")); // Reine
        m_view->setPiece('r', QIcon(":/pieces_extravagantes/Chess_rdt_extravagant.png")); // Tour
        m_view->setPiece('n', QIcon(":/pieces_extravagantes/Chess_ndt_extravagant.png")); // Cavalier
        m_view->setPiece('b', QIcon(":/pieces_extravagantes/Chess_bdt_extravagant.png")); // Fou

        break;
    }
    }
}


/**
 * @brief MainWindow::HandleButtonChangeBoard
 *
 * Fonction pour ouvrir le widget qui renvoie le changement de couleur du board
 */
void MainWindow::HandleButtonChangeBoard(){
    new QListWidgetItem(tr("Défault"), listWidget);
    new QListWidgetItem(tr("Bleu-Blanc"), listWidget);
    new QListWidgetItem(tr("Rose-Blanc"), listWidget);
    new QListWidgetItem(tr("Violet"), listWidget);
    new QListWidgetItem(tr("Jaune-Bleu"), listWidget);
    new QListWidgetItem(tr("Vert-Blanc"), listWidget);

    QVBoxLayout *layout = new QVBoxLayout();
    QHBoxLayout *Buttonlayout = new QHBoxLayout();
    QPushButton *buttonOk = new QPushButton(tr("Ok"), ChangeBoard);
    QPushButton *buttonQuitter = new QPushButton(tr("Fermer"), ChangeBoard);

    layout ->addWidget(listWidget);
    Buttonlayout ->addWidget(buttonQuitter);
    Buttonlayout->addWidget(buttonOk);
    layout -> addLayout(Buttonlayout);

    ChangeBoard->setWindowFlags(Qt::WindowTitleHint | Qt::WindowMinimizeButtonHint);
    ChangeBoard->setFixedSize(QSize(300, 250));
    ChangeBoard->setWindowTitle("Couleurs de l'échiquier");
    ChangeBoard->setLayout(layout);
    ChangeBoard->show();

    connect(buttonOk, &QPushButton::released, this, &MainWindow::SetupChangeBoard);
    connect(buttonQuitter, &QPushButton::released, this, &MainWindow::SetupQuitChangeBoard);

    listWidget->item(0)->setSelected(true);
}


/**
 * @brief MainWindow::SetupQuitChangeBoard
 *
 * Fonction pour quitter le changement de couleur du board
 */
void MainWindow::SetupQuitChangeBoard(){
    delete ChangeBoard->layout();
    listWidget->clear();
    ChangeBoard->close();
}


/**
 * @brief MainWindow::SetupChangeBoard
 *
 * Fonction d'application de changement de couleur du board
 */
void MainWindow::SetupChangeBoard(){
    int p = listWidget->currentRow();

    AllBoard(p);
    update();

    SetupQuitChangeBoard();
}

/**
 * @brief MainWindow::AllBoard
 * @param p
 *
 * Changement de couleur du board
 */
void MainWindow::AllBoard(int p){
     switch(p){

     case 0:{
         m_view->Color1 = palette().color(QPalette::Light);
         m_view->Color2 = palette().color(QPalette::Mid);
         break;
     }

     case 1:{
         m_view->Color1 = QColor(94,162,247,255);
         m_view->Color2 = Qt::white;
         break;
     }

     case 2:{
         m_view->Color1 = QColor(226,229,233,255);
         m_view->Color2 = QColor(254,65,208,255);
         break;
     }

     case 3:{
         m_view->Color1 = QColor(133,111,198,255);
         m_view->Color2 = QColor(47,18,131,255);
         break;
     }

     case 4:{
         m_view->Color1 = QColor(183,139,21,255);
         m_view->Color2 = QColor(177,245,237,255);
         break;
     }

     case 5:{
         m_view->Color1 = QColor(78,189,56,255);
         m_view->Color2 = QColor(219,238,215,255);
         break;
     }
     }
}

/**
 * @brief MainWindow::HandleButtonChangeLignes
 *
 * Fonction pour créer le widget permettant de changer le type de flèches
 */
void MainWindow::HandleButtonChangeLignes(){
    new QListWidgetItem(tr("SolidLine"), listWidget);
    new QListWidgetItem(tr("DashLine"), listWidget);
    new QListWidgetItem(tr("DotLine"), listWidget);
    new QListWidgetItem(tr("DashDotLine"), listWidget);
    new QListWidgetItem(tr("DashDotDotLine"), listWidget);

    QVBoxLayout *layout = new QVBoxLayout();
    QHBoxLayout *ButtonLayout = new QHBoxLayout();
    QPushButton *buttonOk = new QPushButton(tr("Ok"), ChangeLine);
    QPushButton *buttonQuitter = new QPushButton(tr("Fermer"), ChangeLine);

    layout->addWidget(listWidget);
    ButtonLayout->addWidget(buttonQuitter);
    ButtonLayout->addWidget(buttonOk);
    layout->addLayout(ButtonLayout);

    ChangeLine->setWindowFlags(Qt::WindowTitleHint | Qt::WindowMinimizeButtonHint);
    ChangeLine->setFixedSize(QSize(300, 250));
    ChangeLine->setWindowTitle("Changer le style de lignes");
    ChangeLine->setLayout(layout);
    ChangeLine->show();

    connect(buttonOk, &QPushButton::released, this, &MainWindow::SetupChangeLine);
    connect(buttonQuitter, &QPushButton::released, this, &MainWindow::SetupQuitChangeLine);

    listWidget->item(0)->setSelected(true);
}


/**
 * @brief MainWindow::SetupQuitChangeLine
 *
 * Fonction pour quitter le widget de changement de lignes
 */
void MainWindow::SetupQuitChangeLine(){
    delete ChangeLine->layout();
    listWidget->clear();
    ChangeLine->close();
}


/**
 * @brief MainWindow::SetupChangeLine
 *
 * Fonction de changement de type de lignes
 */
void MainWindow::SetupChangeLine(){
    int p = listWidget->currentRow();
    m_view->StyleLine = p+1;

    SetupQuitChangeLine();
}

/**
 * @brief MainWindow::HandleButtonParametrageCouleurs
 *
 * Fonction créant un widget pour configurer différents paramètres du jeu
 */
void MainWindow::HandleButtonParametrageCouleurs(){
    new QListWidgetItem(tr("Toutes les cases atteignables"), listWidget);
    new QListWidgetItem(tr("Couleur du clique droit"), listWidget);
    new QListWidgetItem(tr("Couleur du clique gauche"), listWidget);
    new QListWidgetItem(tr("Couleur de l'échec et Mat"), listWidget);
    new QListWidgetItem(tr("Couleur de l'échec"), listWidget);
    new QListWidgetItem(tr("Couleur des fleches"), listWidget);

    QVBoxLayout *layout = new QVBoxLayout();
    QHBoxLayout *ButtonLayout = new QHBoxLayout();
    QPushButton *buttonOk = new QPushButton(tr("Ok"), ChangeColors);
    QPushButton *buttonQuitter = new QPushButton(tr("Fermer"), ChangeColors);

    layout->addWidget(listWidget);
    ButtonLayout->addWidget(buttonQuitter);
    ButtonLayout->addWidget(buttonOk);
    layout->addLayout(ButtonLayout);

    ChangeColors->setWindowFlags(Qt::WindowTitleHint | Qt::WindowMinimizeButtonHint);
    ChangeColors->setFixedSize(QSize(300, 250));
    ChangeColors->setLayout(layout);
    ChangeColors->setWindowTitle("Changer les couleurs");
    ChangeColors->show();

    connect(buttonOk, &QPushButton::released, this, &MainWindow::SetupChangeColors);
    connect(buttonQuitter, &QPushButton::released, this, &MainWindow::SetupQuitChangeColors);

    listWidget->item(0)->setSelected(true);
}


/**
 * @brief MainWindow::SetupQuitChangeColors
 *
 * Fonction pour quitter le widget de changement de couleur
 */
void MainWindow::SetupQuitChangeColors(){
    delete ChangeColors->layout();
    listWidget->clear();
    ChangeColors->close();
}


/**
 * @brief MainWindow::SetupChangeColors
 *
 * Fonction pour changer les couleurs du jeu
 */
void MainWindow::SetupChangeColors(){
    QColor NewColor2 = Qdialog1->getColor();
    QColor NewColor = QColor(NewColor2.red(), NewColor2.green(), NewColor2.blue(), 160);

    int p = listWidget->currentRow();

    switch(p){
    case 0:{Color1 = NewColor; break;}
    case 1:{Color2 = NewColor; break;}
    case 2:{Color3 = NewColor; break;}
    case 3:{Color4 = NewColor; break;}
    case 4:{Color5 = NewColor; break;}
    case 5:{m_view->ColorLine = NewColor; break;}
    }

    SetupQuitChangeColors();
}

/**
 * @brief MainWindow::HandleButtonModeSombre
 *
 * Fonction pour mettre en mode sombre le plateau de jeu
 */
void MainWindow::HandleButtonModeSombre(){
    m_view->ColorIndex = QColor(255,255,255,255);
    this->setStyleSheet("background-color: rgb(38,36,33) ;");

    menuBar()->setStyleSheet("color: white");
    checkBox->setStyleSheet("QCheckBox { color: white }");
}

/**
 * @brief MainWindow::HandleButtonModeJour
 *
 * Fonction pour mettre en mode jour le plateau de jeu
 */
void MainWindow::HandleButtonModeJour(){
    m_view->ColorIndex = QColor(0,0,0,255);
    this->setStyleSheet("background-color: rgb(240,240,240) ;");

    menuBar()->setStyleSheet("color: black");
    checkBox->setStyleSheet("QCheckBox { color: black }");
}

/**
 * @brief MainWindow::HandleButtonVolumeControl
 *
 * Fonction créant un widget permettant de controler le volume des sons du jeu
 */
void MainWindow::HandleButtonVolumeControl(){
    QVBoxLayout *Vlayout = new QVBoxLayout();
    QHBoxLayout *Hlayout1 = new QHBoxLayout();
    QHBoxLayout *Hlayout2 = new QHBoxLayout();
    QHBoxLayout *Hlayout3 = new QHBoxLayout();
    QHBoxLayout *ButtonLayout = new QHBoxLayout();

    ChangeVolume->setWindowFlags(Qt::WindowTitleHint | Qt::WindowMinimizeButtonHint);
    ChangeVolume->setWindowTitle("Changer le volume");
    ChangeVolume->setFixedSize(QSize(200, 100));

    QLabel *label  = new QLabel("Contrôle des volumes");
    QFont f( "Arial", 14, QFont::Bold);
    label->setFont(f);


    QLabel *label1  = new QLabel("Volume des mouvements");
    QLabel *label2 = new QLabel("Volume de la victoire");
    QLabel *label3 = new QLabel("Volume du Pat");

    QLCDNumber *lcd = new QLCDNumber(3);
    lcd->setSegmentStyle(QLCDNumber::Filled);
    lcd->display(50);
    lcd->setMaximumHeight(40);


    SlidersVolumeMovement->setRange(0,100);
    SlidersVolumeMovement->setSingleStep(1);
    SlidersVolumeMovement->setValue(50);

    QLCDNumber *lcd2 = new QLCDNumber(3);
    lcd2->setSegmentStyle(QLCDNumber::Filled);
    lcd2->display(50);
    lcd2->setMaximumHeight(40);


    SlidersVolumeVictory->setRange(0,100);
    SlidersVolumeVictory->setSingleStep(1);
    SlidersVolumeVictory->setValue(50);


    QLCDNumber *lcd3 = new QLCDNumber(3);
    lcd3->setSegmentStyle(QLCDNumber::Filled);
    lcd3->display(50);
    lcd3->setMaximumHeight(40);


    SlidersVolumePat->setRange(0,100);
    SlidersVolumePat->setSingleStep(1);
    SlidersVolumePat->setValue(50);

    QPushButton *buttonOk = new QPushButton(tr("Ok"),ChangeVolume);
    QPushButton *buttonQuitter = new QPushButton(tr("Fermer"),ChangeVolume);

    Hlayout1->addWidget(label1);
    Hlayout1->addWidget(SlidersVolumeMovement);
    Hlayout1->addWidget(lcd);

    Hlayout2->addWidget(label2);
    Hlayout2->addSpacing(22);
    Hlayout2->addWidget(SlidersVolumeVictory);
    Hlayout2->addWidget(lcd2);

    Hlayout3->addWidget(label3);
    Hlayout3->addSpacing(52);
    Hlayout3->addWidget(SlidersVolumePat);
    Hlayout3->addWidget(lcd3);

    Vlayout->addWidget(label);
    Vlayout->setAlignment(label,Qt::AlignCenter);
    Vlayout->addLayout(Hlayout1);
    Vlayout->addSpacing(20);
    Vlayout->addLayout(Hlayout2);
    Vlayout->addSpacing(20);
    Vlayout->addLayout(Hlayout3);
    Vlayout->addSpacing(20);

    ButtonLayout->addWidget(buttonQuitter);
    ButtonLayout->addWidget(buttonOk);
    Vlayout->addLayout(ButtonLayout);

    ChangeVolume->setFixedSize(QSize(300, 250));
    ChangeVolume->setLayout(Vlayout);
    ChangeVolume->show();

    connect(SlidersVolumeMovement, SIGNAL(valueChanged(int)),lcd, SLOT(display(int)));
    connect(SlidersVolumeVictory, SIGNAL(valueChanged(int)),lcd2, SLOT(display(int)));
    connect(SlidersVolumePat, SIGNAL(valueChanged(int)),lcd3, SLOT(display(int)));
    connect(buttonOk, &QPushButton::released, this, &MainWindow::ChangeVolumeFunction);
    connect(buttonQuitter, &QPushButton::released, this, &MainWindow::ChangeQuitVolumeFunction);
}


/**
 * @brief MainWindow::ChangeQuitVolumeFunction
 *
 * Fonction permettant de quitter le widget de changement de volume
 */
void MainWindow::ChangeQuitVolumeFunction(){
    delete ChangeVolume->layout();
    ChangeVolume->close();
}

/**
 * @brief MainWindow::ChangeVolumeFunction
 *
 * Fonciton pour modifier la puissance des sons
 */
void MainWindow::ChangeVolumeFunction(){
    m_view->board()->ChangeSoundVolumeMovement(SlidersVolumeMovement->value());
    m_view->board()->ChangeSoundVolumeVictory(SlidersVolumeVictory->value());
    m_view->board()->ChangeSoundVolumePat(SlidersVolumePat->value());

    ChangeQuitVolumeFunction();
}

/**
 * @brief MainWindow::PromotionFonction
 * @param PieceDeplacee
 * @param index
 *
 * Fonction de promotion des pions
 */
void MainWindow::PromotionFonction(char PieceDeplacee, int index){
    indexPromotion =index;
    PieceDeplaceePromotion = PieceDeplacee;

    Promotion->setFixedSize(QSize(160, 160));
    Promotion->setWindowTitle("Promotion");

    QVBoxLayout *Vlayout = new QVBoxLayout();
    QHBoxLayout *Hlayout1 = new QHBoxLayout();
    QHBoxLayout *Hlayout2 = new QHBoxLayout();

    QPushButton *buttonCavalier = new QPushButton();
    QPushButton *buttonFou = new QPushButton();
    QPushButton *buttonTour = new QPushButton();
    QPushButton *buttonDame = new QPushButton();

    switch(indexAllPieces){
    case 0:{
        if (PieceDeplacee == 'P'){
            SetButton(":/pieces/Chess_nlt45.svg", buttonCavalier);
            SetButton(":/pieces/Chess_blt45.svg", buttonFou);
            SetButton(":/pieces/Chess_rlt45.svg", buttonTour);
            SetButton(":/pieces/Chess_qlt45.svg", buttonDame);
        }
        else{
            SetButton(":/pieces/Chess_ndt45.svg", buttonCavalier);
            SetButton(":/pieces/Chess_bdt45.svg", buttonFou);
            SetButton(":/pieces/Chess_rdt45.svg", buttonTour);
            SetButton(":/pieces/Chess_qdt45.svg", buttonDame);
        }
        break;
    }

    case 1:{
        if (PieceDeplacee == 'P'){
            SetButton(":/pieces_medievales/Chess_nlt_medieval.png", buttonCavalier);
            SetButton(":/pieces_medievales/Chess_blt_medieval.png", buttonFou);
            SetButton(":/pieces_medievales/Chess_rlt_medieval.png", buttonTour);
            SetButton(":/pieces_medievales/Chess_qlt_medieval.png", buttonDame);
        }
        else{
            SetButton(":/pieces_medievales/Chess_ndt_medieval.png", buttonCavalier);
            SetButton(":/pieces_medievales/Chess_bdt_medieval.png", buttonFou);
            SetButton(":/pieces_medievales/Chess_rdt_medieval.png", buttonTour);
            SetButton(":/pieces_medievales/Chess_qdt_medieval.png", buttonDame);
        }
        break;
    }

    case 2:{
        if (PieceDeplacee == 'P'){
            SetButton(":/pieces_retros/Chess_nlt_retro.png", buttonCavalier);
            SetButton(":/pieces_retros/Chess_blt_retro.png", buttonFou);
            SetButton(":/pieces_retros/Chess_rlt_retro.png", buttonTour);
            SetButton(":/pieces_retros/Chess_qlt_retro.png", buttonDame);
        }
        else{
            SetButton(":/pieces_retros/Chess_ndt_retro.png", buttonCavalier);
            SetButton(":/pieces_retros/Chess_bdt_retro.png", buttonFou);
            SetButton(":/pieces_retros/Chess_rdt_retro.png", buttonTour);
            SetButton(":/pieces_retros/Chess_qdt_retro.png", buttonDame);;
        }
        break;
    }

    case 3:{
        if (PieceDeplacee == 'P'){
            SetButton(":/pieces_extravagantes/Chess_nlt_extravagant.png", buttonCavalier);
            SetButton(":/pieces_extravagantes/Chess_blt_extravagant.png", buttonFou);
            SetButton(":/pieces_extravagantes/Chess_rlt_extravagant.png", buttonTour);
            SetButton(":/pieces_extravagantes/Chess_qlt_extravagant.png", buttonDame);
        }
        else{
            SetButton(":/pieces_extravagantes/Chess_ndt_extravagant.png", buttonCavalier);
            SetButton(":/pieces_extravagantes/Chess_bdt_extravagant.png", buttonFou);
            SetButton(":/pieces_extravagantes/Chess_rdt_extravagant.png", buttonTour);
            SetButton(":/pieces_extravagantes/Chess_qdt_extravagant.png", buttonDame);
        }
        break;
    }
    }

    Hlayout1 ->addWidget(buttonCavalier);
    Hlayout1 ->addWidget(buttonFou);
    Hlayout2 ->addWidget(buttonTour);
    Hlayout2 ->addWidget(buttonDame);

    Vlayout ->addLayout(Hlayout1);
    Vlayout ->addLayout(Hlayout2);

    connect(buttonCavalier, &QPushButton::released, this, &MainWindow::PromotionFonctionCavalier);
    connect(buttonFou, &QPushButton::released, this, &MainWindow::PromotionFonctionFou);
    connect(buttonTour, &QPushButton::released, this, &MainWindow::PromotionFonctionTour);
    connect(buttonDame, &QPushButton::released, this, &MainWindow::PromotionFonctionDame);

    Promotion->setWindowFlags(Qt::WindowTitleHint | Qt::WindowMinimizeButtonHint);
    Promotion->setLayout(Vlayout);
    Promotion->show();
}


/**
 * @brief MainWindow::PromotionFonctionCavalier
 *
 * Promotion du cavalier
 */
void MainWindow::PromotionFonctionCavalier(){
    if (PieceDeplaceePromotion == 'P'){ m_view->board()->Change_m_boardData('N', indexPromotion);}
    else{ m_view->board()->Change_m_boardData('n', indexPromotion);}

    PromotionFonctionQuit();
}


/**
 * @brief MainWindow::PromotionFonctionFou
 *
 * Promotion du fou
 */
void MainWindow::PromotionFonctionFou(){
    if (PieceDeplaceePromotion == 'P'){m_view->board()->Change_m_boardData('B', indexPromotion);}
    else{m_view->board()->Change_m_boardData('b', indexPromotion);}

    PromotionFonctionQuit();
}


/**
 * @brief MainWindow::PromotionFonctionTour
 *
 * Promotion de la tour
 */
void MainWindow::PromotionFonctionTour(){
    if (PieceDeplaceePromotion == 'P'){ m_view->board()->Change_m_boardData('R', indexPromotion);}
    else{m_view->board()->Change_m_boardData('r', indexPromotion);}

    PromotionFonctionQuit();
}


/**
 * @brief MainWindow::PromotionFonctionDame
 *
 * Promotion de la dame
 */
void MainWindow::PromotionFonctionDame(){
    if (PieceDeplaceePromotion == 'P'){m_view->board()->Change_m_boardData('Q', indexPromotion);}
    else{ m_view->board()->Change_m_boardData('q', indexPromotion);}

    PromotionFonctionQuit();
}


/**
 * @brief MainWindow::PromotionFonctionQuit
 *
 * Fonction pour quitter le widget de promotion des pions
 */
void MainWindow::PromotionFonctionQuit(){
    delete Promotion->layout();
    Promotion->close();

    m_view->board()->IsCheckMate();
    m_view->board()->Endgame();
}


/**
 * @brief MainWindow::SetButton
 * @param txt
 * @param button
 *
 * Fonction pour modifier le style des boutons lors de la promotion
 */
void MainWindow::SetButton(QString txt, QPushButton *button){
    button->setStyleSheet("height: 50px; width: 50px;");
    QPixmap pixmap(txt);
    QIcon ButtonIcon(pixmap);
    button->setIcon(ButtonIcon);

    if (indexAllPieces == 0){button->setIconSize(pixmap.rect().size());}
    else{button->setIconSize(pixmap.rect().size()/2);}
}


/**
 * @brief MainWindow::StartArrowFunction
 * @param field
 *
 * Fonction pour ajouter une flèche
 */
void MainWindow::StartArrowFunction(const QPoint &field){
    int px = field.x();
    int py = field.y();
    StartIndexArrow.append(8 * (py-1)+(px-1));
}


/**
 * @brief MainWindow::EndArrowFunction
 * @param field
 *
 * Fonction pour retirer une flèche
 */
void MainWindow::EndArrowFunction(const QPoint &field){
    int px = field.x();
    int py = field.y();
    EndIndexArrow.append(8 * (py-1)+(px-1));

    if (StartIndexArrow.last() == EndIndexArrow.last()){
        StartIndexArrow.removeLast();
        EndIndexArrow.removeLast();
    }
    else{
        ImplementationArrow();
    }
}


/**
 * @brief MainWindow::ImplementationArrow
 *
 * Fonction pour implémenter graphiquement une flèche
 */
void MainWindow::ImplementationArrow(){
    if (StartIndexArrow.length() != EndIndexArrow.length()){
        if (StartIndexArrow.length() > EndIndexArrow.length()){
            StartIndexArrow = StartIndexArrow.mid(0,EndIndexArrow.length());
        }
        else{
            EndIndexArrow = EndIndexArrow.mid(0,StartIndexArrow.length());
        }
    }

    int r1 = m_algorithm->GetRankIndex(StartIndexArrow.last())+1;
    int c1 = m_algorithm->GetColumnIndex(StartIndexArrow.last())+1;
    int r2 = m_algorithm->GetRankIndex(EndIndexArrow.last())+1;
    int c2 = m_algorithm->GetColumnIndex(EndIndexArrow.last())+1;

    bool Repet = false;
    if (testX.length() != 0){
        for (int i=0; i<testX.length(); i++){
            if (testX[i]->rank() == r1 and testX[i]->column() == c1 and testY[i]->rank() == r2 and testY[i]->column() == c2){
                StartIndexArrow.removeAt(i);
                EndIndexArrow.removeAt(i);

                StartIndexArrow.removeLast();
                EndIndexArrow.removeLast();

                m_view->removeHighlightArrow(testX[i],testY[i]);
                testX.removeAt(i);
                testY.removeAt(i);

                Repet = true;
            }
        }
    }

    if (!Repet){
        testX.append(new ChessView::FieldHighlight(c1, r1, Color1));
        testY.append(new ChessView::FieldHighlight(c2, r2, Color1));
        m_view->addHighlightArrow(testX.last(), testY.last());
    }
}


/**
 * @brief MainWindow::HandleEngine
 *
 * Fonction créant une interface pour jouer contre l'ordinateur
 */
void MainWindow::HandleEngine(){
    m_view->board()->UseEngine = !m_view->board()->UseEngine;
    if (m_view->board()->UseEngine){
        actionIA->setText("Jouer contre une personne");

        QVBoxLayout *Vlayout = new QVBoxLayout();

        IAwidget->setWindowFlags(Qt::WindowTitleHint | Qt::WindowMinimizeButtonHint);
        IAwidget->setWindowTitle("IA");
        IAwidget->setFixedSize(QSize(225, 160));

        QLabel *label  = new QLabel("Niveau de l'IA");
        QFont f( "Arial", 14, QFont::Bold);
        label->setFont(f);

        QLCDNumber *lcd = new QLCDNumber(1);
        lcd->setSegmentStyle(QLCDNumber::Filled);
        lcd->display(2);
        lcd->setMaximumHeight(150);
        lcd->setMaximumWidth(150);

        QPalette palette = lcd->palette();
        palette.setColor(palette.Background, QColor(0, 170, 255, 160));
        palette.setColor(palette.Light, QColor(255, 0, 0));
        palette.setColor(palette.Dark, QColor(0, 255, 0));
        lcd->setPalette(palette);


        SliderIALevel->setRange(2,5);
        SliderIALevel->setValue(2);
        SliderIALevel->setSingleStep(1);
        SliderIALevel->setFocusPolicy(Qt::StrongFocus);

        QPushButton *buttonQuitIaWidget = new QPushButton(tr("Quitter"),GameOver);
        buttonQuitIaWidget->setMaximumSize(100,50);

        Vlayout ->addWidget(label);
        Vlayout ->setAlignment(label, Qt::AlignCenter);
        Vlayout ->addWidget(SliderIALevel);
        Vlayout ->addWidget(lcd);
        Vlayout ->setAlignment(lcd, Qt::AlignCenter);
        Vlayout ->addWidget(buttonQuitIaWidget);
        Vlayout ->setAlignment(buttonQuitIaWidget, Qt::AlignCenter);


        IAwidget->setLayout(Vlayout);
        IAwidget->show();

        connect(SliderIALevel, SIGNAL(valueChanged(int)),lcd, SLOT(display(int)));
        connect(buttonQuitIaWidget, &QPushButton::released, this, &MainWindow::ReturnLevelValue);

    }
    else{
        actionIA->setText("Jouer contre l'IA");
    }

}


/**
 * @brief MainWindow::ReturnLevelValue
 *
 * Fonction récupérant le niveau souhaité de l'ordinateur
 */
void MainWindow::ReturnLevelValue(){
    m_algorithm->board()->level = SliderIALevel->value();
    delete IAwidget->layout();
    IAwidget->close();
}


/**
 * @brief MainWindow::LumiereCalcul
 *
 * Fonction pour afficher que l'ordinateur réfléchit
 */
void MainWindow::LumiereCalcul(){
    m_view->IaCalculation=!m_view->IaCalculation;
    update();
}


