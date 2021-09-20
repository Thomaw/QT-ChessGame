QT       += core gui \
            multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    IA/abstractthread.cpp \
    IA/board.cpp \
    IA/engine.cpp \
    IA/evaluate.cpp \
    IA/search.cpp \
    bishop_class.cpp \
    chessalgorithm.cpp \
    chessboard.cpp \
    chessview.cpp \
    king_class.cpp \
    knight_class.cpp \
    main.cpp \
    mainwindow.cpp \
    pawn_class.cpp \
    player.cpp \
    queen_class.cpp \
    rook_class.cpp \
    widget_aproposde.cpp

HEADERS += \
    IA/abstractthread.h \
    IA/board.h \
    IA/engine.h \
    IA/enginetypes.h \
    IA/evaluate.h \
    IA/search.h \
    bishop_class.h \
    chessalgorithm.h \
    chessboard.h \
    chessview.h \
    king_class.h \
    knight_class.h \
    mainwindow.h \
    pawn_class.h \
    player.h \
    queen_class.h \
    rook_class.h \
    widget_aproposde.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    Icone.qrc \
    pieces.qrc \
    sound.qrc

DISTFILES += \
    Icone/snake.ico \
    Sound/movement_sound.wav \
    pieces/Chess_bdt45.svg \
    pieces/Chess_blt45.svg \
    pieces/Chess_kdt45.svg \
    pieces/Chess_klt45.svg \
    pieces/Chess_ndt45.svg \
    pieces/Chess_nlt45.svg \
    pieces/Chess_pdt45.svg \
    pieces/Chess_plt45.svg \
    pieces/Chess_qdt45.svg \
    pieces/Chess_qlt45.svg \
    pieces/Chess_rdt45.svg \
    pieces/Chess_rlt45.svg \
    pieces/LICENSE.txt \
    pieces_extravagantes/Chess_bdt_extravagant.png \
    pieces_extravagantes/Chess_blt_extravagant.png \
    pieces_extravagantes/Chess_kdt_extravagant.png \
    pieces_extravagantes/Chess_klt_extravagant.png \
    pieces_extravagantes/Chess_ndt_extravagant.png \
    pieces_extravagantes/Chess_nlt_extravagant.png \
    pieces_extravagantes/Chess_pdt_extravagant.png \
    pieces_extravagantes/Chess_plt_extravagant.png \
    pieces_extravagantes/Chess_qdt_extravagant.png \
    pieces_extravagantes/Chess_qlt_extravagant.png \
    pieces_extravagantes/Chess_rdt_extravagant.png \
    pieces_extravagantes/Chess_rlt_extravagant.png \
    pieces_medievales/Chess_bdt_medieval.png \
    pieces_medievales/Chess_blt_medieval.png \
    pieces_medievales/Chess_kdt_medieval.png \
    pieces_medievales/Chess_klt_medieval.png \
    pieces_medievales/Chess_ndt_medieval.png \
    pieces_medievales/Chess_nlt_medieval.png \
    pieces_medievales/Chess_pdt_medieval.png \
    pieces_medievales/Chess_plt_medieval.png \
    pieces_medievales/Chess_qdt_medieval.png \
    pieces_medievales/Chess_qlt_medieval.png \
    pieces_medievales/Chess_rdt_medieval.png \
    pieces_medievales/Chess_rlt_medieval.png \
    pieces_retros/Chess_bdt_retro.png \
    pieces_retros/Chess_blt_retro.png \
    pieces_retros/Chess_kdt_retro.png \
    pieces_retros/Chess_klt_retro.png \
    pieces_retros/Chess_ndt_retro.png \
    pieces_retros/Chess_nlt_retro.png \
    pieces_retros/Chess_pdt_retro.png \
    pieces_retros/Chess_plt_retro.png \
    pieces_retros/Chess_qdt_retro.png \
    pieces_retros/Chess_qlt_retro.png \
    pieces_retros/Chess_rdt_retro.png \
    pieces_retros/Chess_rlt_retro.png

SUBDIRS += \
    IA/ChessEngine.pro
